// Torc - Copyright 2011 University of Southern California.  All Rights Reserved.
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
/// \brief Header for the PlacerNetlist class.

#ifndef TORC_PLACER_PLACERNETLIST_HPP
#define TORC_PLACER_PLACERNETLIST_HPP

//#include <boost/timer.hpp>
//#include <math.h>
#include "torc/physical/Design.hpp"
#include "torc/placer/NetlistInstance.hpp"
#include "torc/placer/NetlistNet.hpp"
#include "torc/placer/NetlistPin.hpp"
#include "torc/placer/DeviceWrapper.hpp"
#include <map>
#include <vector>

namespace torc {
namespace placer {
	class PlacerNetlist {
	// types
		typedef physical::DesignSharedPtr DesignSharedPtr;
		typedef physical::InstanceSharedPtr InstanceSharedPtr;
		typedef physical::InstanceSharedPtrVector InstanceSharedPtrVector;
		typedef physical::Circuit::InstanceSharedPtrConstIterator InstanceSharedPtrConstIterator;
		typedef physical::Circuit::NetSharedPtrConstIterator NetSharedPtrConstIterator;
		typedef physical::Net::InstancePinSharedPtrConstIterator InstancePinSharedPtrConstIterator;
		typedef physical::ENetType ENetType;
		typedef boost::uint32_t uint32;
	protected:
	
		DesignSharedPtr mDesign;
		DeviceWrapper& mDevice;
	
		std::vector<NetlistInstance*> mInstances;
		std::vector<NetlistNet*> mNets;
		std::vector<NetlistPin*> mPins; // probably not useful at all
		
		std::vector<std::vector<NetlistInstance*> > mInstancesByType;
		
		// looks up string name of instance, returns index in the vector
		std::map<std::string, uint32> mInstanceLookup; // used for construction of the netlist
		std::map<std::string, uint32> mNetLookup; // used for nothing maybe....
		//std::map<std::string, int> mInstanceTypeLookup;
		
		
		
	public:
		PlacerNetlist(DesignSharedPtr inDesign, DeviceWrapper& inDevice)
			: mDesign(inDesign), mDevice(inDevice) {
			
			// populate the netlist with the design information (or at least wrap it)
			
			
			InstanceSharedPtrConstIterator p = inDesign->instancesBegin();
			InstanceSharedPtrConstIterator e = inDesign->instancesEnd();
			for ( ; p != e; p++) {
				//mInstances.push_back(new NetlistInstance(*p, 
				//	mDevice.mTypeMapping.getType((*p)->getType())));
				addInstance(*p);
			}
			
			NetSharedPtrConstIterator q = inDesign->netsBegin();
			NetSharedPtrConstIterator f = inDesign->netsEnd();
			for ( ; q != f; q++) {
				const std::string& netName = (*q)->getName();
				//mNets.push_back(new NetlistNet((*q)->getName(), (*q)->getNetType(),	mNets.size()));
				addNet(netName, (*q)->getNetType());
				
				InstancePinSharedPtrConstIterator r;
				InstancePinSharedPtrConstIterator g;
				r = (*q)->sourcesBegin();
				g = (*q)->sourcesEnd();
				for ( ; r != g; r++) {
					InstanceSharedPtr instance = (*r)->getInstancePtr().lock();
					addNetTerminal(netName, instance->getName(), (*r)->getPinName(), true);
				}
				r = (*q)->sinksBegin();
				g = (*q)->sinksEnd();
				for ( ; r != g; r++) {
					InstanceSharedPtr instance = (*r)->getInstancePtr().lock();
					addNetTerminal(netName, instance->getName(), (*r)->getPinName(), false);
				}
			}
		}
		~PlacerNetlist() {}
		void printInstances() {
			std::cout << "### PlacerNetlist Instances ###" << std::endl;
			std::vector<NetlistInstance*>::iterator p = mInstances.begin();
			std::vector<NetlistInstance*>::iterator e = mInstances.end();
			for ( ; p != e; p++) {
				std::cout << (*p)->getInstance()->getName() << " : " 
					<< (*p)->getInstance()->getType() << " ";
				if ((*p)->getSite() == NULL)
					std::cout << "UNPLACED!!!" << std::endl;
				else
					std::cout << (*p)->getSite()->getName() << std::endl;
			}
		}
		uint32 getInstanceType(std::string& inType) {
			throw "getInstanceType(string) not used";
			//std::map<std::string, int>::iterator typeit = mInstanceTypeLookup.find(inType);
			//return typeit->second;
			return 0;
		}
		//int addInstance(std::string inName, std::string inType, uint32 inIntType) {
		uint32 addInstance(InstanceSharedPtr inInstance) {
			uint32 index = mInstances.size(); // return index of added instance
			//mInstanceLookup.insert(std::pair<std::string, uint32>(inName, index));
			mInstanceLookup.insert(std::pair<std::string, uint32>(inInstance->getName(), index));
			
			// put the instance in a type indexed structure
			// this time, the same as the database!!
			//if (inIntType >= mInstancesByType.size()) {
			//	for (uint32 i = mInstancesByType.size(); i < inIntType + 1; i++) {
			//		mInstancesByType.push_back(std::vector<NetlistInstance*>());
			//	}
			//}
			
			//NetlistInstance* instance = new NetlistInstance(inName, inType, inIntType);
			NetlistInstance* instance = new NetlistInstance(inInstance, 
				mDevice.mTypeMapping.getTypeIndex(inInstance->getType()));
			mInstances.push_back(instance);
			//mInstancesByType[inIntType].push_back(instance);
			
			return index;
			
		}
		uint32 addNet(std::string inName, ENetType inType) {
			uint32 index = mNets.size(); // returns index of added instance
			mNets.push_back(new NetlistNet(inName, inType, index));
			mNetLookup.insert(std::pair<std::string, uint32>(inName, index));
			return index;
		}
		uint32 addNetTerminal(std::string inNetName, std::string inInstanceName, std::string inPortName, bool isNetSource) {
			NetlistPin* pin = new NetlistPin(inPortName, isNetSource);
			NetlistNet* net = getNet(inNetName);
			NetlistInstance* instance = getInstance(inInstanceName);
			mPins.push_back(pin);
			pin->setInstance(instance);
			pin->setNet(net);
			instance->addPin(pin);
			if (isNetSource) {
				return net->addSource(pin);
			} else {
				return net->addSink(pin);
			}
			if (pin->getInstance() == NULL) {
				std::cout << "BAD PIN DEFINITION!!! " << pin->getName() << " " << net->getName() << std::endl;
			}
		}
		// instance accessors
		NetlistInstance* getInstance(std::string inName) {
			return mInstances[mInstanceLookup[inName]];
		}
		NetlistInstance* getInstance(uint32 i) {
			return mInstances[i];
		}
		NetlistInstance* getInstance(uint32 type, uint32 index) {
			return mInstancesByType[type][index];
		}
		// net accessors
		NetlistNet* getNet(std::string inName) {
			return mNets[mNetLookup[inName]];
		}
		NetlistNet* getNet(uint32 i) {
			return mNets[i];
		}
		uint32 getNumNets() {
			return mNets.size();
		}
		uint32 getNumInstances() {
			return mInstances.size();
		}
		uint32 getNumInstancesByType(uint32 type) {
			if (type >= mInstancesByType.size()) {
				return -1; // does not exist
			}
			return mInstancesByType[type].size();
		}
		uint32 getNumTypes() {
			return mInstancesByType.size();
		}
		void print() {
			std::cout << "### INSTANCES ###" << std::endl;
			for (uint32 i = 0; i < mInstances.size(); i++) {
				NetlistInstance* instance = mInstances[i];
				std::cout << "\t" << instance->getInstance()->getName() 
					<< " type: " << instance->getType() << std::endl;
				for (uint32 j = 0; j < instance->getNumPins(); j++) {
					NetlistPin* pin = instance->getPin(j);
					std::cout << "\t\tpin: " << pin->getName() << std::endl;
				}
			}
			std::cout << "### NETS ###" << std::endl;
			for (uint32 i = 0; i < mNets.size(); i++) {
				NetlistNet* net = mNets[i];
				std::cout << "\t" << net->getName() << " type: " << net->getType() << std::endl;
				for (uint32 j = 0; j < net->getNumSources(); j++) {
					NetlistPin* pin = net->getSource(j);
					std::cout << "\t\tsource: " << pin->getInstance()->getInstance()->getName() << " pin " << pin->getName() << std::endl;
				}
				for (uint32 j = 0; j < net->getNumSinks(); j++) {
					NetlistPin* pin = net->getSink(j);
					std::cout << "\t\tsink: " << pin->getInstance()->getInstance()->getName() << " pin " << pin->getName() << std::endl;
				}
			}
			
		}
		void temporaryPrune() {
			std::vector<NetlistNet*> newNets;
			for (uint32 i = 0; i < mNets.size(); i++) {
				// if any part of a net is not touching a sliceL, kill it
				NetlistNet* net = mNets[i];
				NetlistPin* pin;
				bool killnet = false;
				//std::cout << "net: " << net->getName() << std::endl;
				for (uint32 j = 0; j < net->getNumSources(); j++) {
					pin = net->getSource(j);
					//std::cout << "\tsource: " << pin->getInstance()->getName() << " " << pin->getInstance()->getIntType() << std::endl;
					//if (pin->getInstance()->getInstance()->getIntType() != 46) {
					//	std::cout << "\t\tKILLED" << std::endl;
					//	killnet = true;
					//}
				}
				for (uint32 j = 0; j < net->getNumSinks(); j++) {
					pin = net->getSink(j);
					//std::cout << "\tsink: " << pin->getInstance()->getName() << " " << pin->getInstance()->getIntType() << std::endl;
					//if (pin->getInstance()->getInstance()->getIntType() != 46) {
					//	std::cout << "\t\tKILLED" << std::endl;
					//	killnet = true;
					//} else {
					//}
				}
				if (!killnet) {
					newNets.push_back(net);
				} else {
					std::cout << "KILL NET: " << net->getName() << std::endl;
				}
			}
			std::cout << "PRUNE: " << mNets.size() << " " << newNets.size() << std::endl;
			mNets.clear();
			for (uint32 i = 0; i < newNets.size(); i++) {
				mNets.push_back(newNets[i]);
			}
		}
	}; //class PlacerNetlist
} // namespace placer
} // namespace torc
#endif // TORC_PLACER_PLACERNETLIST_HPP

