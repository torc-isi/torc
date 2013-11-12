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

#ifndef TORC_ROUTER_PATHFINDER_HPP
#define TORC_ROUTER_PATHFINDER_HPP

#include "torc/architecture/DDB.hpp"
#include "NetRouterBase.hpp"
#include "NetVectorRouterHeuristicBase.hpp"
#include "RouteNet.hpp"
#include "RouteNode.hpp"
#include <boost/functional/hash.hpp>


#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include "NetVectorRouterBase.hpp"
#include <boost/timer.hpp>
#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>
#include <boost/cstdint.hpp>

#include "torc/architecture/Tilewire.hpp"

namespace torc {

namespace router {
	/// \brief Pathfinder annotations for Tilewires.
	struct TilewireData {
		boost::uint32_t mPresentSharing;
		boost::uint32_t mHistorySharing;
	};

	class PathFinder : public NetVectorRouterBase
	{
		typedef std::string string;
		typedef architecture::DDB DDB;
		typedef architecture::WireUsage WireUsage;
		typedef architecture::Tilewire Tilewire;
		typedef architecture::Arc Arc;
		typedef architecture::TilewireVector TilewireVector;
		typedef TilewireVector::const_iterator TilewireConstIterator;
		
		typedef boost::unordered_map<Tilewire, TilewireData> PathFinderSharingMap;
		
	protected:
	
		PathFinderSharingMap mConflicts;
		WireUsage mConflictWireUsage;
		
		TilewireVector mTempWireSources;
		TilewireVector mTempWireSinks;
		
		boost::timer routetimer;
		boost::timer iterationtimer;
		boost::timer totaltimer;
		boost::timer updatetimer;
		
		long deleteCount;
		
	public:
		/// \brief Pathfinder constructor.
		PathFinder(DDB& inDB, NetVectorRouterHeuristicBase* inHeuristic,
			NetRouterBase* inNetRouter) : NetVectorRouterBase(inDB, inHeuristic, inNetRouter), 
			mConflictWireUsage(mDB.getTiles()) {
			
			mTempWireSources.reserve(200);
			mTempWireSinks.reserve(200);
			mTempWireSources.clear();
			mTempWireSinks.clear();
			deleteCount = 0;
			
			mConflictWireUsage.autosize();
			mNetRouter->getHeuristic()->setParameter(0, &mConflicts);
			mNetRouter->getHeuristic()->processParameters();
		}
		
		/// \brief Destructor.
		~PathFinder() {}

		void routeNets(RouteNetVector& nets) {
			
			bool flag_resources = true;
			
			unsigned int numNets = nets.size();
			
			flag_resources = true;

			int routingPasses = 0;


			// Initial Routes
			totaltimer.restart();
			double updatetime = 0;
			iterationtimer.restart();
			for (unsigned int i = 0; i < numNets; i++) {
				RouteNet& net = nets[i];
			
				//double routetime;
				if (!(net.hasOneSource() && net.hasAnySinks())) {
					std::cout << "BAD NET: " << net.getName() << std::endl;
					continue;
				}
				try {
					//std::cout << "### Routing net " << net.getName() << " " 
					//	<< " index: " << i << " of " << numNets 
					//	<< std::endl;
					routetimer.restart();
					unmarkSourcesAndSinks(net);
					mNetRouter->route(net);
					markSourcesAndSinks(net);
					updateSharing(net.routeNodes());
					//routetime = routetimer.elapsed();
				}
				catch (...) {
						std::cout << "Failed routing net " << i << ": " << std::endl;
						throw;
				}
				mConflictWireUsage.clear();
			}

			mConflicts.clear();
			std::cout << "Initial routes time: " << iterationtimer.elapsed() << std::endl;

			for (unsigned int i = 0; i < numNets; i++) {
				for (unsigned int j = 0; j < nets[i].routeNodes().size(); j++) {
					updateSharing(nets[i].routeNodes()[j]->getSinkTilewire());
				}
			}
			
			// Main Loop
			while (flag_resources) { // while shared resources exist
				iterationtimer.restart();
				std::cout << "." << std::flush;
						
				double avgroutetime = 0;
				double minroutetime = std::numeric_limits<double>::max();
				double maxroutetime = std::numeric_limits<double>::min();
				double routetime;
				int netsRouted = 0;
				
				for (unsigned int n = 0; n < numNets; n++) { //for each net
					RouteNet& net = nets[n];
					routetimer.restart();
					//if (testReroute(nets[n].routeNodes())) {
					if (true) {
					
						unrouteNet(net.routeNodes(), net.getName());
						
						try {
							unmarkSourcesAndSinks(net);
							mNetRouter->route(net);
							markSourcesAndSinks(net);
							updateSharing(net.routeNodes());
						}
						catch (...)	{
							std::cout << "Failed routing net " << net.getName() << " index: "
								<< n << " of " << nets.size() << std::endl;
							throw;
						}

						routetime = routetimer.elapsed();

						if (routetime < minroutetime)
							minroutetime = routetime;
						if (routetime > maxroutetime)
							maxroutetime = routetime;
						avgroutetime += routetime;
						netsRouted++;
					}
				}
				avgroutetime = avgroutetime / netsRouted;
				
				updatetimer.restart();
				PathFinderSharingMap::iterator p;
				unsigned int conflicts = 0;
				unsigned int maxpresent = 0;
				unsigned int maxhistory = 0;
				Tilewire maxPresentTilewire;
				Tilewire maxHistoryTilewire;
				for (p = mConflicts.begin(); p != mConflicts.end(); p++)
				{
					if ((*p).second.mPresentSharing > 1)
					{
						(*p).second.mHistorySharing += (*p).second.mPresentSharing; 
						conflicts++;
					}
					
					if ((*p).second.mPresentSharing > maxpresent)
					{
						maxpresent = (*p).second.mPresentSharing;
						maxPresentTilewire = (*p).first;
					}
					if ((*p).second.mHistorySharing > maxhistory)
					{
						maxhistory = (*p).second.mHistorySharing;
						maxHistoryTilewire = (*p).first;
					}
				}
				updatetime += updatetimer.elapsed();
				
//				std::cout << "Conflicts this iteration: " << conflicts << " max present: " 
//					<< maxpresent << " max history: " << maxhistory << std::endl;
//				std::cout << "Total conflict records: " << mConflicts.size() << std::endl;
//				std::cout << "Nets rerouted: " << netsRouted << "/" << numNets << std::endl;
//				std::cout << "Total time: " << totaltimer.elapsed() 
//					<< " iteration time: " << iterationtimer.elapsed() 
//					<< " average route time: " << avgroutetime
//					<< " total update time: " << updatetime
//					<< std::endl;
					
				if (netsRouted == 1) {
					flag_resources = false;
					for (unsigned int i = 0; i < numNets; i++) {
						RouteNet& net = nets[i];
						if (testReroute(net.routeNodes())) {
							std::cout << "FOUND SELF CONFLICT NET: " << net.getName() << std::endl;
							std::cout << mDB;
							for (unsigned int j = 0; j < net.routeNodes().size(); j++) {
								std::cout << "\t" << net.routeNodes()[j]->getArc() << std::endl;
							}
						}
					}
				}
				
//				std::cout << "Max Present Tilewire: " << maxPresentTilewire 
//					<< " Max History Tilewire: " << maxHistoryTilewire << std::endl;
				
				if (conflicts == 0) flag_resources = false;
				routingPasses++;
			} // end iteraton while loop
			std::cout << std::endl;
			std::cout << "Total time: " << totaltimer.elapsed() << " Update time: " << updatetime 
				<< std::endl;
			
//			std::cout << "RECORD RESULT" << std::endl;
		} // end of function

		void unrouteNet(RouteNodePtrVector& routeVector, const string& netname)
		{
			int rVecSize = routeVector.size();
			for (int x = 0; x < rVecSize; x++)
			{
				mTempWireSources.clear();
				mDB.expandSegment(routeVector[x]->getSinkTilewire(), mTempWireSources);
				int numWireSources = mTempWireSources.size();
				for (int i = 0; i < numWireSources; i++)
				{
					Tilewire tw = mTempWireSources[i];
					if (mConflicts.find(tw) != mConflicts.end())
					{
						mConflicts[tw].mPresentSharing--;
						if (mConflicts[tw].mPresentSharing == 0)
						{
							mConflictWireUsage.release(tw);
						}
						if (mConflicts[tw].mPresentSharing < 0)
						{
							std::cout << "ERROR PRESENT: " << tw << " net " << netname << std::endl;
							throw;
						}
					}
					else 
					{
						mConflictWireUsage.release(tw);
					}
				}
				delete routeVector[x];
				deleteCount++;
			}
			routeVector.clear();
		}
		void updateSharing(RouteNodePtrVector& outRoute) {
			for (unsigned int i = 0; i < outRoute.size(); i++) {
				updateSharing(outRoute[i]->getSinkTilewire());
			}
		}
		void updateSharing(const Tilewire& inTilewire) {
			mTempWireSources.clear();
			mDB.expandSegment(inTilewire, mTempWireSources);
			int numWireSources = mTempWireSources.size();
			for (int i = 0; i < numWireSources; i++) {
				Tilewire tw = mTempWireSources[i];
				if (mConflictWireUsage.isUsed(tw)) {
					if (mConflicts.find(tw) != mConflicts.end()) {
						mConflicts[tw].mPresentSharing++;
					} else {
						mConflicts[tw].mPresentSharing = 2;
						mConflicts[tw].mHistorySharing = 0;
					}
				} else {
					if (mConflicts.find(tw) != mConflicts.end()) {
						mConflicts[tw].mPresentSharing++;
					}
					mConflictWireUsage.use(tw);
				}
			}
			
		}
		
		void recordResult(std::vector<RouteNodePtrVector>& outRoutes, 
			std::vector<RouteNodePtrVector>& tempRoutes, 
			std::vector<unsigned int>& priorities, unsigned int plevel) {
			if (outRoutes.size() != tempRoutes.size()) {
				std::cout << "BAD SIZE: " << outRoutes.size() << " " << tempRoutes.size() << std::endl;
				return;
			}
			for (unsigned int i = 0; i < outRoutes.size(); i++) {
				recordResult(outRoutes[i], tempRoutes[i]);
			}
		}
		void recordResult(RouteNodePtrVector& outRoute, RouteNodePtrVector& tempRoute) {
			Arc arc;
			for (unsigned int i = 0; i < tempRoute.size(); i++) {
				outRoute.push_back(tempRoute[i]);
				arc = tempRoute[i]->getArc();
				mDB.useArc(arc);
			}
		}
		
		bool testReroute(RouteNodePtrVector& currentRoute) {
			for (unsigned int i = 0; i < currentRoute.size(); i++) {
				if (mConflicts.find(currentRoute[i]->getSinkTilewire()) != mConflicts.end() 
					&& mConflicts[currentRoute[i]->getSinkTilewire()].mPresentSharing > 1) {
					return true;
				}
			}
			return false;
		}
		
		void unmarkSourcesAndSinks(RouteNet& net) {
			mDB.releaseSegment(*net.sourcesBegin());
			RouteNet::TilewireConstIterator p;
			RouteNet::TilewireConstIterator e = net.sinksEnd();
			for (p = net.sinksBegin(); p != e; p++) {
				mDB.releaseSegment(*p);
			}
		}
		void markSourcesAndSinks(RouteNet& net)
		{
			mDB.useSegment(*net.sourcesBegin());
			RouteNet::TilewireConstIterator p;
			RouteNet::TilewireConstIterator e = net.sinksEnd();
			for (p = net.sinksBegin(); p != e; p++) {
				mDB.useSegment(*p);
			}
		}
		
	}; // class PathFinder
} // namespace router
} // namespace torc
#endif // TORC_ROUTER_PATHFINDER_HPP
