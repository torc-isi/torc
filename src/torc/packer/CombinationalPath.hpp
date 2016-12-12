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
/// \brief Header for the CombinationalPath class.

#ifndef TORC_PACKER_COMBINATIONALPATH_HPP
#define TORC_PACKER_COMBINATIONALPATH_HPP

#include "torc/physical/Net.hpp"
#include "torc/packer/PrimitivePin.hpp"
#include "torc/packer/RoutingNet.hpp"
#include "torc/physical/Design.hpp"
#include <iostream>
#include <vector>

namespace torc {
namespace physical {

	/// \brief Routing net.
	
	class CombinationalPath {
	// friends
		/// \brief The Factory class has direct access to our internals.
		friend class RcFactory;
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
				
	// members
		/// \brief Vector of routing net shared pointers.
		RoutingNetSharedPtrVector mRoutingNets;
		
	// constructors
		/// \brief Protected constructor.
		/// \param original net
		/// \param inNetType The net power type.
		CombinationalPath(){}
	public:
		
		/// \brief Constant iterator to Routing Net shared pointers.
		typedef RoutingNetSharedPtrVector::const_iterator RoutingNetSharedPtrConstIterator;
		/// \brief Non-constant iterator to Routing Net shared pointers.
		typedef RoutingNetSharedPtrVector::iterator RoutingNetSharedPtrIterator;
			
		/// \brief Find a  net by name.
		/// \param inName The net name to look for.
		/// \returns an iterator to the specified net, or netsEnd() if the name was not found.
		RoutingNetSharedPtrIterator findRoutingNet(const string& inName) {
			NameComparator predicate(inName);
			return std::find_if(routingNetsBegin(), routingNetsEnd(), predicate);
		}
		/// \brief Add a net to the set.
		/// \param inRoutingNetPtr The net to add.
		/// \returns true if the net was added, or false if a net with the same name already exists 
		///		in the circuit.
		bool addRoutingNet(RoutingNetSharedPtr& inRoutingNetPtr) {
			/// \todo Acquire mutex.
			RoutingNetSharedPtrIterator e = mRoutingNets.end();
			RoutingNetSharedPtrIterator result = findRoutingNet(inRoutingNetPtr->getName());
			if(result != e) return false;
			mRoutingNets.push_back(inRoutingNetPtr);
			return true;
			/// \todo Release mutex.
		}
		/// \brief Remove a net from the circuit.
		/// \param inRoutingNetPtr The net to remove.
		/// \returns true if the net was removed, or false if the net did not exist.
		bool removeRoutingNet(RoutingNetSharedPtr& inRoutingNetPtr) {
			/// \todo Acquire mutex.
			RoutingNetSharedPtrIterator e = mRoutingNets.end();
			RoutingNetSharedPtrIterator result = findRoutingNet(inRoutingNetPtr->getName());
			if(result == e) return false;
			mRoutingNets.erase(result);
			/// \todo Release mutex.
			return true;
		}
			
	// functions
		/// \brief Returns instance pin type
		PinType getInstancePinType(InstancePinSharedPtr instancePinPtr){
				
			    NetSharedPtr netPtr = instancePinPtr->getParentWeakPtr().lock();
			    if(netPtr->containsSource(instancePinPtr))
			    	return OutputP;
			    return InputP;
			} 
		/// \brief Set combinational counts for all nets
		bool setPatchCounts(DesignSharedPtr inDesignPtr){	
			
			Circuit::NetSharedPtrConstIterator pn = inDesignPtr->netsBegin();
			Circuit::NetSharedPtrConstIterator en = inDesignPtr->netsEnd();
			while(pn < en){
				NetSharedPtr netPtr = *pn++;
				RoutingNetSharedPtr routingNetPtr(new RoutingNet(netPtr));
				mRoutingNets.push_back(routingNetPtr);
			}
			Circuit::InstanceSharedPtrConstIterator pi = inDesignPtr->instancesBegin();
			Circuit::InstanceSharedPtrConstIterator ei = inDesignPtr->instancesEnd();
			// set nets connected to flipflops
			while(pi < ei) {
				InstanceSharedPtr instance1Ptr = *pi++;
				if((instance1Ptr->getType()=="DFF")||(instance1Ptr->getType()=="IOB")){
					Instance::InstancePinSharedPtrConstIterator ipp = instance1Ptr->pinsBegin();
					Instance::InstancePinSharedPtrConstIterator ipe = instance1Ptr->pinsEnd();
					while(ipp != ipe) {
						InstancePinSharedPtr instancePinPtr = ipp->second;
						if(getInstancePinType(instancePinPtr) == InputP){
					    	NetSharedPtr netPtr = instancePinPtr->getParentWeakPtr().lock();
					    	RoutingNetSharedPtr routingNetPtr = *findRoutingNet(netPtr->getName());
							Net::InstancePinSharedPtrConstIterator sop = netPtr->sourcesBegin();
							Net::InstancePinSharedPtrConstIterator soe = netPtr->sourcesEnd();
							while(sop < soe){
								 InstancePinSharedPtr instPin = *sop++;
								 routingNetPtr->setPathCount(instPin, 1);
							}
						}
					++ipp;
					} 
				}
			}
			bool repeatNeeded = true;
			size_t maxTotalCount = 0;
			while(repeatNeeded){
				if(maxTotalCount>inDesignPtr->getInstanceCount()){
					cout<<"Combinational Loop !!!!!"<<endl;;
					break;
				}
				repeatNeeded = false;
				pi = inDesignPtr->instancesBegin();
				ei = inDesignPtr->instancesEnd();
				
				// set nets connected to other gates
				while(pi < ei) {
					InstanceSharedPtr instance1Ptr = *pi++;
					if((instance1Ptr->getType()!= "DFF")&&(instance1Ptr->getType()!= "IOB")){
						size_t maxCount = 0;
						Instance::InstancePinSharedPtrConstIterator ipp = instance1Ptr->pinsBegin();
						Instance::InstancePinSharedPtrConstIterator ipe = instance1Ptr->pinsEnd();
						while(ipp != ipe) {
							InstancePinSharedPtr instancePinPtr = ipp->second;
							if(getInstancePinType(instancePinPtr) == OutputP){
						    	NetSharedPtr netPtr = instancePinPtr->getParentWeakPtr().lock();
						    	RoutingNetSharedPtr routingNetPtr = *findRoutingNet(netPtr->getName());
								Net::InstancePinSharedPtrConstIterator sop = netPtr->sourcesBegin();
								Net::InstancePinSharedPtrConstIterator soe = netPtr->sourcesEnd();
								while(sop < soe){
									 InstancePinSharedPtr instPin = *sop++;
									 size_t currCount = routingNetPtr->getPathCount(instPin);
									 if(currCount>maxCount)
									 	maxCount = currCount;
									 if(maxTotalCount<maxCount)
									 	maxTotalCount=maxCount;
								}
							}
							++ipp;
						} 
						ipp = instance1Ptr->pinsBegin();
						ipe = instance1Ptr->pinsEnd();
						while(ipp != ipe) {
							InstancePinSharedPtr instancePinPtr = ipp->second;
							if(getInstancePinType(instancePinPtr) == InputP){
						    	NetSharedPtr netPtr = instancePinPtr->getParentWeakPtr().lock();
						    	RoutingNetSharedPtr routingNetPtr = *findRoutingNet(netPtr->getName());
						    	if(routingNetPtr->getPathCount(instancePinPtr)<(maxCount+1)){
						    		routingNetPtr->setPathCount(instancePinPtr,maxCount+1);
						    		repeatNeeded = true;
						    	}
								Net::InstancePinSharedPtrConstIterator sop = netPtr->sourcesBegin();
								Net::InstancePinSharedPtrConstIterator soe = netPtr->sourcesEnd();
								while(sop < soe){
									 InstancePinSharedPtr instPin = *sop++;
									 size_t currCount = routingNetPtr->getPathCount(instPin);
									 if(currCount<(maxCount+1)){
									 	routingNetPtr->setPathCount(instPin, maxCount+1);
									 	maxCount = currCount;
									 	if(maxTotalCount<maxCount)
									 		maxTotalCount=maxCount;
									 	repeatNeeded = true;
									}
								}
							}
							++ipp;
						}
					}
				}
			}
			pn = inDesignPtr->netsBegin();
			en = inDesignPtr->netsEnd();
			while(pn < en){
				NetSharedPtr netPtr = *pn++;
				
				RoutingNetSharedPtr routingNetPtr = *findRoutingNet(netPtr->getName());
				Net::InstancePinSharedPtrConstIterator sop = netPtr->sourcesBegin();
				Net::InstancePinSharedPtrConstIterator soe = netPtr->sourcesEnd();
				size_t setCount = 0;
				while(sop<soe){
					if(setCount<routingNetPtr->getPathCount(*sop))
						setCount = routingNetPtr->getPathCount(*sop);
					++sop;
				}
									
				Net::InstancePinSharedPtrConstIterator sip = netPtr->sinksBegin();
				Net::InstancePinSharedPtrConstIterator sie = netPtr->sinksEnd();
				while(sip!=sie){
					if(routingNetPtr->getPathCount(*sip)==0)
						routingNetPtr->setPathCount(*sip, setCount);
					++sip;
				}						
			}
			

			repeatNeeded = true;
			
			while(repeatNeeded){
				repeatNeeded = false;
				pi = inDesignPtr->instancesBegin();
				ei = inDesignPtr->instancesEnd();
				
				// set nets connected to other gates
				while(pi < ei) {
					InstanceSharedPtr instance1Ptr = *pi++;
					if((instance1Ptr->getType()!= "DFF")&&(instance1Ptr->getType()!= "IOB")){
						size_t maxCount = 0;
						Instance::InstancePinSharedPtrConstIterator ipp = instance1Ptr->pinsBegin();
						Instance::InstancePinSharedPtrConstIterator ipe = instance1Ptr->pinsEnd();
						while(ipp != ipe) {
							InstancePinSharedPtr instancePinPtr = ipp->second;
							if(getInstancePinType(instancePinPtr) == InputP){
						    	NetSharedPtr netPtr = instancePinPtr->getParentWeakPtr().lock();
						    	RoutingNetSharedPtr routingNetPtr = *findRoutingNet(netPtr->getName());
						    	size_t currCount = routingNetPtr->getPathCount(instancePinPtr);
								if(currCount>maxCount)
									maxCount = currCount;
							}
							++ipp;
						}
						 
						ipp = instance1Ptr->pinsBegin();
						ipe = instance1Ptr->pinsEnd();
						while(ipp != ipe) {
							InstancePinSharedPtr instancePinPtr = ipp->second;
							if(getInstancePinType(instancePinPtr) == OutputP){
						    	NetSharedPtr netPtr = instancePinPtr->getParentWeakPtr().lock();
						    	RoutingNetSharedPtr routingNetPtr = *findRoutingNet(netPtr->getName());
						    	if(routingNetPtr->getPathCount(instancePinPtr)<(maxCount)){
						    		routingNetPtr->setPathCount(instancePinPtr,maxCount);
						    		repeatNeeded = true;
						    		size_t minDiff = 100000000;
						    		Net::InstancePinSharedPtrConstIterator sop = netPtr->sinksBegin();
									Net::InstancePinSharedPtrConstIterator soe = netPtr->sinksEnd();
									while(sop < soe){
										 InstancePinSharedPtr instPin = *sop++;
										 size_t currCount = maxCount - routingNetPtr->getPathCount(instPin);
										 if(currCount<minDiff){
										 	minDiff = currCount; 
										}
									}
									sop = netPtr->sinksBegin();
									soe = netPtr->sinksEnd();
									while(sop < soe){
										 InstancePinSharedPtr instPin = *sop++;
										 size_t currCount = routingNetPtr->getPathCount(instPin);
										 routingNetPtr->setPathCount(instPin,currCount+minDiff);	 
									}	
						    	}
							}
							++ipp;
						}
					}
				}	
			}
			return true;
		}
		
		/// \brief Returns the begin constant iterator for routingNets.
		RoutingNetSharedPtrConstIterator routingNetsBegin(void) const { return mRoutingNets.begin(); }
		/// \brief Returns the end constant iterator for routingNets.
		RoutingNetSharedPtrConstIterator routingNetsEnd(void) const { return mRoutingNets.end(); }
		/// \brief Returns the begin non-constant iterator for routingNets.
		RoutingNetSharedPtrIterator routingNetsBegin(void) { return mRoutingNets.begin(); }
		/// \brief Returns the end non-constant iterator for routingNets.
		RoutingNetSharedPtrIterator routingNetsEnd(void) { return mRoutingNets.end(); }
		/// \brief Returns the number of routingNets in the circuit.
		size_t getRoutingNetCount(void) const { return mRoutingNets.size(); }
	};
	
	
	/// \brief Shared pointer encapsulation of a CombinationalPath.
	typedef boost::shared_ptr<CombinationalPath> CombinationalPathSharedPtr;

	/// \brief Vector of CombinationalPath shared pointers.
	typedef std::vector<CombinationalPathSharedPtr> CombinationalPathSharedPtrVector;

} // namespace physical
} // namespace torc

#endif // TORC_PACKER_COMBINATIONALPATH_HPP
