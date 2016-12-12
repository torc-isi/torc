// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
// $HeadURL$
// $Id$

// This program is free software: you can redistribute it and/or modify it under the terms of the 
// GNU General Public License as published by the Free Software Foundation, either version 3 of the 
// License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
// the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with this program.  If 
// not, see <http://www.gnu.org/licenses/>.

/// \file
/// \brief Header for the Placement class.

#ifndef TORC_PLACER_PLACEMENT_HPP
#define TORC_PLACER_PLACEMENT_HPP

//#include "torc/placer/PlacerNetlist.hpp"
#include "torc/physical/Design.hpp"
#include "torc/placer/DeviceWrapper.hpp"
#include "torc/placer/PlacerAnnotations.hpp"
#include "torc/placer/PlacerHeuristicBase.hpp"
#include "torc/placer/PlacerHelpers.hpp"
#include <boost/timer.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

#include <iostream>

namespace torc {
namespace placer {

	/// \brief Wrapper of the Design for placing the design.
	
	class Placement {
	protected:
	//types
		typedef architecture::DDB DDB;
		typedef architecture::Sites Sites;
		typedef architecture::Site Site;
		typedef std::vector<const Site*> SitePtrVector;
		typedef std::vector<SitePtrVector> SitePtrVectorVector;

		typedef physical::DesignSharedPtr DesignSharedPtr;
		typedef boost::uint32_t uint32;
		typedef boost::uint64_t uint64;
		
		typedef physical::ENetType ENetType;
		typedef physical::NetSharedPtr NetSharedPtr;
		typedef physical::NetSharedPtrVector NetSharedPtrVector;
		typedef physical::InstanceSharedPtr InstanceSharedPtr;
		typedef physical::InstanceSharedPtrVector InstanceSharedPtrVector;
		typedef std::vector<InstanceSharedPtrVector> InstanceSharedPtrVectorVector;
		typedef physical::InstancePinSharedPtr InstancePinSharedPtr;
		typedef physical::InstancePinSharedPtrVector InstancePinSharedPtrVector;
		
		typedef boost::unordered_map<const Site*, uint32> SitePtrToIntMap;
		typedef boost::unordered_map<const Site*, InstanceSharedPtr> SitePtrToInstanceSharedPtrMap;
		typedef boost::unordered_map<InstanceSharedPtr, Site*> InstanceSharedPtrToSitePtrMap;

		typedef boost::mt19937 mt19937;
		typedef boost::uniform_int<uint32> uinform_range;
		typedef boost::variate_generator<mt19937&, uinform_range> random_generator;

		//typedef std::set<SiteSharedPtr> SiteSharedPtrSet;
		//typedef std::vector<SiteSharedPtrSet> SiteSharedPtrSetVector;
		
		// THE DEVICE:
		DeviceWrapper& mDevice;
		
		// THE HEURISTIC - REPLACES DEVICE
		//PlacerHeuristicBase& mHeuristic;
		
		// THE DESIGN
		DesignSharedPtr mDesign; // the master design shared pointer of the design
		//PlacerNetlist mNetlist;

		
		// INTERNAL STRUCTURES
		
		// the sites
		const architecture::Array<const Site>& mAllSites;
		SitePtrVectorVector mAllSitesByType;
		SitePtrVectorVector mCandidatesByType; // extend to group? yes
		SitePtrToIntMap mSiteTypeLookup;
		
		// site assignment
		SitePtrToInstanceSharedPtrMap mAssignedSites;
		
		// instances
		InstanceSharedPtrVector mAllInstances;
		InstanceSharedPtrVectorVector mAllInstancesByType;
		
		// nets
		NetSharedPtrVector mAllNets;
		
		// instance assignment
		//InstanceSharedPtrToSitePtrMap mInstanceAssigment; // don't want to go this route...
		
		// RANDOM STUFF
		boost::mt19937 mRandomSource;
		boost::uniform_int<uint32> mUniformInstanceRange;
		boost::variate_generator<mt19937&, boost::uniform_int<uint32> > mInstanceRandomGen;
		
		std::vector<boost::uniform_int<uint32> > mUniformTypeRanges;
		// these much be pointers because they're not assignable...
		std::vector<boost::variate_generator<mt19937&, 
			boost::uniform_int<uint32> >* > mTypeRandomGen;
			
		uint32 mCost;
		//uint32 mOldCost;
		
		bool mDebug;
		
		// MOVE RELATED MEMBERS
		InstanceSharedPtr mSelectedInstance;
		InstanceSharedPtr mEvictedInstance;
		Site* mDepartureSite;
		Site* mTargetSite;
		
	public:
		Placement(DeviceWrapper& inDevice, DesignSharedPtr inDesign)
			: mDevice(inDevice), mDesign(inDesign), mAllSites(mDevice.mSites.getSites()),
			mRandomSource(42), mUniformInstanceRange(0, mDesign->getInstanceCount() - 1),
			mInstanceRandomGen(mRandomSource, mUniformInstanceRange),
			mCost(0), mDebug(false) {
std::cout << "PLACEMENT CONSTRUCTOR" << std::endl;
			// type preparation
			for (uint32 t = 0; t < mDevice.mSites.getSiteTypeCount(); t++) {
				mAllSitesByType.push_back(SitePtrVector());
				mCandidatesByType.push_back(SitePtrVector());
				mAllInstancesByType.push_back(InstanceSharedPtrVector());
			}
std::cout << "Prepped for " << mDevice.mSites.getSiteTypeCount() << " types" << std::endl;


			// instance setup
			InstanceSharedPtrVector::iterator p = mDesign->instancesBegin();
			InstanceSharedPtrVector::iterator e = mDesign->instancesEnd();
			for ( ; p != e; p++) {
				InstanceSharedPtr instance = *p;
				uint32 typeIndex = mDevice.mTypeMapping.getTypeIndex(instance->getType());
				instance->setAnnotation(ePlacerInstanceTypeIndex, typeIndex);
				
				mAllInstances.push_back(instance);
				mAllInstancesByType[typeIndex].push_back(instance);
			}
			
			// net setup
			NetSharedPtrVector::iterator q = mDesign->netsBegin();
			NetSharedPtrVector::iterator f = mDesign->netsEnd();
			for ( ; q != f; q++) {
				NetSharedPtr net = *q;
				mAllNets.push_back(net);
			}
			
			uint32 a = 0;
			for (uint32 i = 0; i < mAllInstancesByType.size(); i++) {
				uint32 b = mAllInstancesByType[i].size();
				if (b != 0)
					std::cout << mDevice.mTypeMapping.getName(i) << ": " << b << std::endl;
				a += b;
				
			}
			std::cout << "Summary INSTANCES: " << a << " = " << mAllInstances.size() << std::endl;

			// site population
std::cout << "There are " << mAllSites.getSize() << " sites" << std::endl;
			for (uint32 i = 0; i < mAllSites.getSize(); i++) {
			
				const architecture::Site& site = mAllSites[i];
				//SiteSharedPtr ssp(site);
				const architecture::PrimitiveDef* siteType = site.getPrimitiveDefPtr();
				uint32 typeIndex = mDevice.mTypeMapping.getTypeIndex(siteType->getName());
				std::vector<uint32>& map = mDevice.mTypeMapping.getLegalInstancesForSite(typeIndex);
				
				//mAllSites.push_back(site);
				mAllSitesByType[typeIndex].push_back(&site);
				mSiteTypeLookup[&site] = typeIndex;
				
				// for each instance type that can map to this site, put it in as a candidate
				for (uint32 j = 0; j < map.size(); j++) {
					mCandidatesByType[map[j]].push_back(&site);
				}
			}
			
			//mRandomSource, mUniformInstanceRange, mInstanceRandomGen already set up in init list
			for (uint32 i = 0; i < mCandidatesByType.size(); i++) {
				uint32 x = mCandidatesByType[i].size();
				mUniformTypeRanges.push_back(boost::uniform_int<uint32>(0, x - 1));
				
				mTypeRandomGen.push_back(new boost::variate_generator<mt19937&, 
					boost::uniform_int<uint32> >(mRandomSource, mUniformTypeRanges[i]));
			}
			
			for (uint32 t = 0; t < mDevice.mSites.getSiteTypeCount(); t++) {
				std::cout << mDevice.mTypeMapping.getName(t) << " has "
					<< mAllSitesByType[t].size() << " sites, " << mCandidatesByType[t].size()
					<< " is the candidate count for this type" << std::endl;
			}
			
			initialPlacement();
			
		}
		~Placement() {
		}
		void initialPlacement() {
			std::cout << "INITIAL PLACEMENT" << std::endl;
		
			boost::mt19937 gen;
			
			// first place all the instances on 
			// check there are enough 
			// place all instance of site of same type
			// for overage, check for 
			
			//std::vector<std::map<uint32, const Site*> > sitesByType;
			std::vector<std::vector<const Site*> > sitesByType;
			
			for (uint32 t = 0; t < mAllSitesByType.size(); t++) {
				sitesByType.push_back(std::vector<const Site*>());
				for (uint32 i = 0; i < mAllSitesByType[t].size(); i++) {
					sitesByType[t].push_back(mAllSitesByType[t][i]);
				}
			}
			
			// now have a structure with all sites available by type, with easy removal.
			// THIS IS NOT GOING TO WORK RIGHT FOR VERY FULL DESIGNS!
			for (uint32 i = 0; i < mAllInstances.size(); i++) {
				// place each instance
				InstanceSharedPtr instance = mAllInstances[i];
std::cout << "Placing instance: " << instance->getName() << std::endl;
				//uint32 typeIndex = boost::any_cast<uint32>(
				//	instance->getAnnotation(ePlacerInstanceTypeIndex));
				uint32 typeIndex = getInstanceTypeIndex(instance);
					
std::cout << "\tInstance type index: " << typeIndex << " (" 
	<< mDevice.mTypeMapping.getName(typeIndex) << ")" << std::endl;
	
				// check if the instance type index has any sites available, otherwise select from 
				// another site type
				uint32 siteTypeIndex = typeIndex;
				if (sitesByType[siteTypeIndex].size() == 0) {
					//try different legal types until a match is found
					std::vector<uint32>& map = mDevice.mTypeMapping.getLegalSitesForInstance(
						siteTypeIndex);
					for (uint32 j = 0; j < map.size(); j++) {
						siteTypeIndex = map[j];
						if (sitesByType[siteTypeIndex].size() > 0) break;
					}
				}
std::cout << "\tSite type index: " << siteTypeIndex << " (" 
	<< mDevice.mTypeMapping.getName(siteTypeIndex) << ")" << std::endl;
				

				// get the candidate type indices
				
				boost::uniform_int<uint32> range(0, sitesByType[siteTypeIndex].size() - 1);
				boost::variate_generator<mt19937&, boost::uniform_int<uint32> > random(gen, range);
				
				uint32 r = random();
std::cout << "\tSelected index " << r << " from " << sitesByType[siteTypeIndex].size() << std::endl;

				const Site* selectedSite = sitesByType[siteTypeIndex][r];

std::cout << "\tSelected Site: " << selectedSite->getName() << std::endl;
				
				instance->setAnnotation(ePlacerInstanceSitePtr, selectedSite);
				mAssignedSites[selectedSite] = instance;
//std::cout << "\tSetting: " << instance->getName() << " " << selectedSite->getName() << std::endl;

//std::cout << "\tSiteByType size before: " << sitesByType[typeIndex].size() << " ";
				//sitesByType[siteTypeIndex].erase(r);
				sitesByType[siteTypeIndex].erase(sitesByType[siteTypeIndex].begin() + r);
//std::cout << " and after: " << sitesByType[typeIndex].size() << std::endl;
				
			}
			
		}
		void writePlacementToDesign() {
			std::cout << "WRITING PLACEMENT TO DESIGN" << std::endl;
			for (uint32 i = 0; i < mAllInstances.size(); i++) {
				InstanceSharedPtr instance = mAllInstances[i];
//std::cout << "Recording: " << instance->getName() << " ";
				const Site* site = boost::any_cast<const Site*>(
					instance->getAnnotation(ePlacerInstanceSitePtr));
				std::string siteName = site->getName();
				std::string tileName = mDevice.mDB.getTiles().getTileInfo(
					site->getTileIndex()).getName();
//std::cout << siteName << " " << tileName << std::endl;
				instance->setSite(siteName);
				instance->setTile(tileName);
				
			}
		}
		uint32 getCost() {
			return mCost;
		}
		void updateCostFull(bool debug) {
		
			mCost = 0;
			for (uint32 i = 0; i < mAllNets.size(); i++) {
				NetSharedPtr net = mAllNets[i];
				if (debug) std::cout << "net : " << net->getName() << std::endl;
				if (net->getSourceCount() != 1 || net->getSinkCount() < 1) {
					if (debug) std::cout << "\tEmpty!" << std::endl;
					continue;
				}
				if (net->getNetType() != physical::eNetTypeNormal) {
					//std::cout << "Net is not a normal net" << std::endl;
					continue;
				}
				InstancePinSharedPtr pinPtr = *net->sourcesBegin();
				InstanceSharedPtr instance = pinPtr->getInstancePtr().lock();
				if (debug) std::cout << "\tsource instance: " << instance->getName() << std::endl;
				const Site* site = boost::any_cast<const Site*>(instance->getAnnotation(
					ePlacerInstanceSitePtr));
				if (site == NULL) {
					std::cout << instance->getName() << " doesn't have a site." << std::endl;
					continue;
				}
				if (debug) std::cout << "\t\tsource instance site: " << site->getName()
					<< std::endl;
					
				uint32 sum = 0;
			
				// semiperimeter of bounding rectangle
				uint32 row1 = mDevice.mDB.getTiles().getTileInfo(site->getTileIndex()).getRow();
				uint32 col1 = mDevice.mDB.getTiles().getTileInfo(site->getTileIndex()).getCol();
				uint32 row2 = row1;
				uint32 col2 = col1;
				InstancePinSharedPtrVector::iterator p = net->sinksBegin();
				InstancePinSharedPtrVector::iterator e = net->sinksEnd();
				for ( ; p != e; p++) {
					instance = (*p)->getInstancePtr().lock();
					if (debug) std::cout << "\tsink instance: " << instance->getName() << std::endl;
					const Site* sinkSite = boost::any_cast<const Site*>(instance->getAnnotation(
						ePlacerInstanceSitePtr));
					if (sinkSite == NULL) {
						std::cout << instance->getName() << " doesn't have a site (sink)." 
							<< std::endl;
						return;
					}
					
					uint32 trow = 
						mDevice.mDB.getTiles().getTileInfo(sinkSite->getTileIndex()).getRow();
					uint32 tcol = 
						mDevice.mDB.getTiles().getTileInfo(sinkSite->getTileIndex()).getCol();
					if (trow < row1) row1 = trow;
					if (tcol < col1) col1 = tcol;
					if (trow > row2) row2 = trow;
					if (tcol > col2) col2 = tcol;
				}
				if (debug) std::cout << "\t\tcost reported: " << sum << " / " << net->getSinkCount()
					<< " = " << ((float)sum / (float)net->getSinkCount()) << std::endl;
				mCost += (row2 - row1) + (col2 - col1);
				if (debug) std::cout << "\t\t" << mCost << std::endl;
			}
			std::cout << "updateCostFull: " << mCost << std::endl;
		}
		bool randomMove(bool debug) {
			if (debug) std::cout << "randomMove: " << std::endl;
			// pick a random instance - this will later be weighted
			uint32 r = mInstanceRandomGen();
			mSelectedInstance = mAllInstances[r];
			if (debug) std::cout << "\tselected inst: " 
				<< mSelectedInstance->getName() << std::endl;
			mDepartureSite = const_cast<Site*>(boost::any_cast<const Site*>(
				mSelectedInstance->getAnnotation(ePlacerInstanceSitePtr)));
			if (debug) std::cout << "\tdeparture site: " << mDepartureSite->getName() << std::endl;
			uint32 instanceType = getInstanceTypeIndex(mSelectedInstance);
			if (debug) std::cout << "\tinstance type: " << instanceType << std::endl;
			uint32 s = (*mTypeRandomGen[instanceType])();
			if (debug) std::cout << "\tselected: " << s << " of " 
				<< mCandidatesByType[instanceType].size() << std::endl;
			mTargetSite = const_cast<Site*>(mCandidatesByType[instanceType][s]);
			if (debug) std::cout << "\ttarget site: " << mTargetSite->getName() << std::endl;
			SitePtrToInstanceSharedPtrMap::iterator p = mAssignedSites.find(mTargetSite);
			mEvictedInstance = InstanceSharedPtr();
			if (p != mAssignedSites.end()) {
				mEvictedInstance = p->second;
				// we have an evicted instance and it needs to be checked for legal placement
				uint32 evictedType = getInstanceTypeIndex(mEvictedInstance);
				uint32 departureSiteType = mSiteTypeLookup[mDepartureSite];
				bool illegalMapping = true;
				std::vector<uint32>& v = mDevice.mTypeMapping.getLegalSitesForInstance(evictedType);
				for (uint32 i = 0; i < v.size(); i++) {
					if (departureSiteType == v[i]) illegalMapping = false;
				}
				if (illegalMapping) return false;
			}
			if (debug) mEvictedInstance == 0 ? std::cout << "\tEMPTY " << std::endl
				: std::cout << "\t" << mEvictedInstance->getName() << std::endl;
			
			// 4 things to update
			if (debug) std::cout << "\tCost before move: " << mCost << std::endl;
			updateCostRemovePair(mSelectedInstance, mEvictedInstance);
			if (debug) std::cout << "\tCost without instance nets: " << mCost << std::endl;
			
			mSelectedInstance->setAnnotation(ePlacerInstanceSitePtr, 
				const_cast<const Site*>(mTargetSite));
			mAssignedSites[mTargetSite] = mSelectedInstance;
			if (mEvictedInstance != 0) {
				mEvictedInstance->setAnnotation(ePlacerInstanceSitePtr, 
					const_cast<const Site*>(mDepartureSite));
				mAssignedSites[mDepartureSite] = mEvictedInstance;
			} else {
				mAssignedSites.erase(mDepartureSite);
			}
			
			updateCostAddPair(mSelectedInstance, mEvictedInstance);
			if (debug) std::cout << "\tCost after move: " << mCost << std::endl;
			return true;
		}
		void undoMove(bool debug) {
			if (debug) {
				std::cout << "undoMove: " << std::endl;
				std::cout << "\tinstance: " << mSelectedInstance->getName() << std::endl;
				std::cout << "\tdeparturesite: " << mDepartureSite->getName() << std::endl;
				std::cout << "\ttargetsite: " << mTargetSite->getName() << std::endl;
				if (mEvictedInstance != 0) std::cout << "\tevictedinstance: "
					<< mEvictedInstance->getName() << std::endl;
			}
			
			updateCostRemovePair(mSelectedInstance, mEvictedInstance);
			
			mSelectedInstance->setAnnotation(ePlacerInstanceSitePtr, 
				const_cast<const Site*>(mDepartureSite));
			mAssignedSites[mDepartureSite] = mSelectedInstance;

			if (mEvictedInstance != 0) {
				mEvictedInstance->setAnnotation(ePlacerInstanceSitePtr,
					const_cast<const Site*>(mTargetSite));
				mAssignedSites[mTargetSite] = mEvictedInstance;
			} else {
				mAssignedSites.erase(mTargetSite);
			}
			
			updateCostAddPair(mSelectedInstance, mEvictedInstance);
		}
		
		void updateCostRemoveInstances(InstanceSharedPtrVector& inInstances) {
			// remove cost contributed by all nets touched by the specified instances
			updateCostHelper(inInstances, false);
		}
		void updateCostRemovePair(InstanceSharedPtr inInstance1, InstanceSharedPtr inInstance2) {
			// remove cost contributed by all nets touched by the specified instances
			updateCostHelper(inInstance1, inInstance2, false);
		}
		void updateCostAddInstances(InstanceSharedPtrVector& inInstances) {
			// add cost contributed by all nets touched by the specified instances
			updateCostHelper(inInstances, true);
		}
		void updateCostAddPair(InstanceSharedPtr inInstance1, InstanceSharedPtr inInstance2) {
			// remove cost contributed by all nets touched by the specified instances
			updateCostHelper(inInstance1, inInstance2, true);
		}
		
		void updateCostHelper(InstanceSharedPtrVector& inInstances, bool addCost) {
			torc::physical::Circuit::InstanceSharedPtrIterator p = inInstances.begin();
			torc::physical::Circuit::InstanceSharedPtrIterator e = inInstances.end();
			
			std::set<NetSharedPtr> modifiedNets;
			
			for ( ; p != e; p++) {
				if ((*p) != 0) {
					torc::physical::Instance::InstancePinSharedPtrIterator q = (*p)->pinsBegin();
					torc::physical::Instance::InstancePinSharedPtrIterator f = (*p)->pinsEnd();
					for ( ; q != f; q++) {
						NetSharedPtr n = (*q).second->getParentWeakPtr().lock();
						modifiedNets.insert(n);
					}
				}
			}
			
			std::set<NetSharedPtr>::iterator r = modifiedNets.begin();
			std::set<NetSharedPtr>::iterator g = modifiedNets.end();
			for ( ; r != g; r++) {
				addCost ? mCost += getNetCost(*r) : mCost -= getNetCost(*r);
			}
		}
		void updateCostHelper(InstanceSharedPtr inInstance1, InstanceSharedPtr inInstance2,
			bool addCost) {
			
			std::set<NetSharedPtr> modifiedNets;
			
			torc::physical::Instance::InstancePinSharedPtrIterator q;
			torc::physical::Instance::InstancePinSharedPtrIterator f;
			
			if (inInstance1 != 0) {
				q = inInstance1->pinsBegin();
				f = inInstance1->pinsEnd();
				for ( ; q != f; q++) {
					NetSharedPtr n = (*q).second->getParentWeakPtr().lock();
					modifiedNets.insert(n);
				}
			}
			if (inInstance2 != 0) {
				q = inInstance2->pinsBegin();
				f = inInstance2->pinsEnd();
				for ( ; q != f; q++) {
					NetSharedPtr n = (*q).second->getParentWeakPtr().lock();
					modifiedNets.insert(n);
				}
			}
			std::set<NetSharedPtr>::iterator r = modifiedNets.begin();
			std::set<NetSharedPtr>::iterator g = modifiedNets.end();
			for ( ; r != g; r++) {
				addCost ? mCost += getNetCost(*r) : mCost -= getNetCost(*r);
			}
		}
		
		uint32 getNetCost(NetSharedPtr inNet) {
			uint32 returnCost = 0;
			
			if (mDebug) std::cout << "getNetCost - net : " << inNet->getName() << std::endl;
			if (inNet->getSourceCount() != 1 || inNet->getSinkCount() < 1) {
				if (mDebug) std::cout << "\tEmpty!" << std::endl;
				return returnCost;
			}
			if (inNet->getNetType() != physical::eNetTypeNormal) {
				//std::cout << "Net is not a normal net" << std::endl;
				return returnCost;
			}
			InstancePinSharedPtr pinPtr = *inNet->sourcesBegin();
			InstanceSharedPtr instance = pinPtr->getInstancePtr().lock();
			if (mDebug) std::cout << "\tsource instance: " << instance->getName() << std::endl;
			const Site* site = boost::any_cast<const Site*>(instance->getAnnotation(
				ePlacerInstanceSitePtr));
			if (site == NULL) {
				std::cout << instance->getName() << " doesn't have a site." << std::endl;
				return returnCost;
			}
			if (mDebug) std::cout << "\t\tsource instance site: " << site->getName()
				<< std::endl;

			uint32 sum = 0;

			// semiperimeter of bounding rectangle
			uint32 row1 = mDevice.mDB.getTiles().getTileInfo(site->getTileIndex()).getRow();
			uint32 col1 = mDevice.mDB.getTiles().getTileInfo(site->getTileIndex()).getCol();
			uint32 row2 = row1;
			uint32 col2 = col1;
			InstancePinSharedPtrVector::iterator p = inNet->sinksBegin();
			InstancePinSharedPtrVector::iterator e = inNet->sinksEnd();
			for ( ; p != e; p++) {
				instance = (*p)->getInstancePtr().lock();
				if (mDebug) std::cout << "\tsink instance: " << instance->getName() << std::endl;
				const Site* sinkSite = boost::any_cast<const Site*>(instance->getAnnotation(
					ePlacerInstanceSitePtr));
				if (sinkSite == NULL) {
					std::cout << instance->getName() << " doesn't have a site (sink)." 
						<< std::endl;
					return returnCost;
				}

				uint32 trow = 
					mDevice.mDB.getTiles().getTileInfo(sinkSite->getTileIndex()).getRow();
				uint32 tcol = 
					mDevice.mDB.getTiles().getTileInfo(sinkSite->getTileIndex()).getCol();
				if (trow < row1) row1 = trow;
				if (tcol < col1) col1 = tcol;
				if (trow > row2) row2 = trow;
				if (tcol > col2) col2 = tcol;
			}
			if (mDebug) std::cout << "\t\tcost reported: " << sum << " / " << inNet->getSinkCount()
				<< " = " << ((float)sum / (float)inNet->getSinkCount()) << std::endl;
			returnCost = (row2 - row1) + (col2 - col1);
			if (mDebug) std::cout << "\t\t" << mCost << std::endl;
		
			return returnCost;
		}
		
	}; // class Placement
} // namespace placer
} // namespace torc

#endif // TORC_PLACER_PLACEMENT_HPP
