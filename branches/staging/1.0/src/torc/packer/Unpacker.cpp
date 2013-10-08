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

#include "torc/packer/Unpacker.hpp"
#include "torc/packer/PrimitiveStructure.hpp"
#include "torc/packer/Virtex5PrimitiveStructure.hpp"
#include "torc/common/DeviceDesignator.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/Sites.hpp"
#include "torc/architecture/XdlImporter.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/physical/Design.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/physical/ConfigMap.hpp"
#include "torc/physical/Circuit.hpp"
#include "torc/physical/Factory.hpp"
#include "torc/physical/Instance.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <map>

namespace torc {
namespace packer {

	using namespace torc::architecture;
	
	void Unpacker::initialize(void) {


		torc::common::DeviceDesignator deviceDesignator(mDesignPtr->getDevice() +  
						  mDesignPtr->getPackage() 
						  +  mDesignPtr->getSpeedGrade());
                mDDBPtr = new DDB(deviceDesignator);
		const Sites& sites = mDDBPtr->getSites();
		
		// look up the primitive def types
		typedef const Array<const PrimitiveDef> PrimitiveDefArray;
		PrimitiveDefArray& primitiveDefs = sites.getSiteTypes();
		PrimitiveDefArray::const_iterator p = primitiveDefs.begin();
		PrimitiveDefArray::const_iterator e = primitiveDefs.end();
		while(p < e) {
			// create a PrimitiveStructure object for this PrimitiveDef
			PrimitiveStructureSharedPtr primitiveStructurePtr
				(new Virtex5PrimitiveStructure(&*p++));
			const torc::architecture::PrimitiveDef* primitiveDefPtr
				= primitiveStructurePtr->getPrimitiveDefPtr();
			const std::string& primitiveDefName = primitiveDefPtr->getName();
			// insert the PrimitiveStructure into the map
			if(primitiveDefName != "IOB" && primitiveDefName != "IOBM" 
			   && primitiveDefName != "IOBS") 
				mPrimitiveStructures[primitiveDefName] = primitiveStructurePtr;
		}		
	}


	bool Unpacker::findNextElement(const PrimitiveStructure& inPrimitiveStructure, 
				       const torc::architecture::PrimitiveElement& element,
				       torc::physical::InstanceSharedPtr& inInstancePtr,
				       NameToElementPtrMap& inUsedElements,
				       std::vector<std::string> inInstanceInputPins, 
				       CandidateNet& inElementPinsForNet,
				       const torc::architecture::PrimitiveElementPin* 
				       &outNetSourcePin, 
				       std::vector<torc::physical::InstanceSharedPtr> 
				       &outNewInstances,
				       ElementPtrToInstancePtrMap &elementToInstanceMap) {

		// if we have already seen and marked this element as used, return true immediately
		if(inUsedElements.find(element.getName()) != inUsedElements.end()) return true;

		const PrimitiveElement* elementPtr = &element;
		std::string elementName = element.getName();
//std::cout << "Current element -- >" << elementName << std::endl;
		const PrimitiveElementPinArray& fetchNextElementPins = element.getPins();
		PrimitiveElementPinArray::const_iterator fpp = fetchNextElementPins.begin();
		PrimitiveElementPinArray::const_iterator fpe = fetchNextElementPins.end();
		std::vector<std::string>::const_iterator instanceInputit;
  		bool result = false;
		bool found = false;
		bool isInverter = false;
		bool isInputTerminal = false;
		bool isOutputTerminal = false;
		// Check if the current element is a terminal and check if it is input/output
		if(inPrimitiveStructure.mTerminals.find(elementName) 
		   != inPrimitiveStructure.mTerminals.end()) {
			const PrimitivePinArray& primitivePins 
				= inPrimitiveStructure.mPrimitiveDefPtr->getPins();
			int primitivePinIndex 
				= inPrimitiveStructure.mPrimitiveDefPtr->findPinIndexByName
				(elementName);
			const PrimitivePin& primitivePin = primitivePins[primitivePinIndex];
                        //look up pin
			isInputTerminal = (!primitivePin.isOutput());
			isOutputTerminal = (primitivePin.isOutput());
		}
		// if it is a used input terminal, return true
		if(isInputTerminal){
			instanceInputit = find(inInstanceInputPins.begin(), 
					       inInstanceInputPins.end(), elementName);
			if(instanceInputit != inInstanceInputPins.end()) {
				inUsedElements[elementName] 
					= inPrimitiveStructure.mElements.find(elementName)->second;
				return true;
			} else {
			 	return false;
			}
		}
		// look up config settings for the current element
		std::string cfgName;
		std::string cfgSetting;
		bool hasConfig = inInstancePtr->getConfig(elementName, cfgName, cfgSetting);
		//std::cout << "Config Name: " << elementName << "  " << cfgName  << "  " 
		//<< cfgSetting << std::endl;				

		int pinIndex;
		const PrimitiveElementPin* invPtr;
		// Check is the current element is a MUX
		if(inPrimitiveStructure.mMuxes.find(elementName) 
		   != inPrimitiveStructure.mMuxes.end()){
			// If the current element is a MUX and it has a config, then it *is* used
			if(hasConfig){
//std::cout << "Next element *is* a MUX.. Recurse!" << std::endl;
				pinIndex = element.findPinIndexByName(cfgSetting); 
				const PrimitiveElementPin* muxPtr 
					= fetchNextElementPins[pinIndex].getPrimitiveConn()
					->getSourcePtr();
				invPtr = &fetchNextElementPins[pinIndex];
//std::cout << "Is this the Pin?: " << invPtr->getName() << std::endl;
				// Check if it is an inverter
				std::set<const PrimitiveElementPin*>::const_iterator it 
					= inPrimitiveStructure.mInvertedInputs.find(invPtr); 
				if(it != inPrimitiveStructure.mInvertedInputs.end()) {
					outNetSourcePin = invPtr;
					isInverter = true;
				} else {
					outNetSourcePin = muxPtr;
					const PrimitiveElement* nextElementPtr2 
						= fetchNextElementPins[pinIndex].getPrimitiveConn()
						->getSourcePtr()->getElementPtr();
					const PrimitiveElement& nextElement2 = *nextElementPtr2;
					return findNextElement(inPrimitiveStructure, nextElement2,
							       inInstancePtr, inUsedElements, 
							       inInstanceInputPins, 
							       inElementPinsForNet, 
							       outNetSourcePin, outNewInstances, 
							       elementToInstanceMap);	
				} 
			// If the current element is a MUX and it does *NOT* have a config, 
		        // then we discard the trace and move on	
			} else {
				return false;
			}
		}
		// Check if the *current* element is a power/ground element
		if(inPrimitiveStructure.mPower.find(elementName) 
		   != inPrimitiveStructure.mPower.end()){
			result = true;
		} else if(inPrimitiveStructure.mGround.find(elementName) 
			  != inPrimitiveStructure.mGround.end()){
			result = true;
		}
		// If the current element is a non-MUX and has a config, 
		// we add a new instance for that element
		if(hasConfig) result = true;		
//std::cout << "Next element is a NON-MUX element.. Get its Inputs!" << std::endl;
//std::cout << "Element Name: " << elementName << std::endl;
		const PrimitiveElementPin* newNetSourcePin;
		//Iterate through Element PINS
		while(fpp < fpe) {

		        const PrimitiveElementPin& fetchNextElementPin = *fpp++;
			const PrimitiveElementPin* fetchNextElementPinPtr = &fetchNextElementPin;
			if(!fetchNextElementPin.isInput()) continue;
//std::cout << "Element Pin Name: " << fetchNextElementPin.getName() << std::endl;
			if(isInverter && (invPtr != fetchNextElementPinPtr)) continue;
			// Tracing back using primitive Conn
			const PrimitiveElement* nextElementPtr 
				= fetchNextElementPin.getPrimitiveConn()->getSourcePtr()
				->getElementPtr();
			const PrimitiveElementPin* sourcePtr 
				= fetchNextElementPin.getPrimitiveConn()->getSourcePtr();
			const PrimitiveElement& nextElement = *nextElementPtr;
			std::string nextElementName = fetchNextElementPin.getPrimitiveConn()
				->getSourcePtr()->getElementPtr()->getName();
//std::cout << "Next Element: " << nextElementName << std::endl;
//std::cout << "Next Element Pin: " << sourcePtr->getName() << std::endl;
//std::cout << "Current Element Pin: " << fetchNextElementPin.getName() << std::endl;
                        // Check if the *next* element is a power/ground element
			bool isPower = false;
			if(inPrimitiveStructure.mPower.find(nextElementName) 
			   != inPrimitiveStructure.mPower.end()){
				isPower = true;
			} else if(inPrimitiveStructure.mGround.find(nextElementName) 
				  != inPrimitiveStructure.mGround.end()){
				isPower = true;
			}
			newNetSourcePin = sourcePtr;				 
			found = findNextElement(inPrimitiveStructure, nextElement, inInstancePtr, 
						inUsedElements, inInstanceInputPins, 
						inElementPinsForNet, newNetSourcePin, 
						outNewInstances, elementToInstanceMap);
					
			if(found) {
//std::cout << "Adding to the Candidate Net: " << newNetSourcePin->getName() 
//<< "...." << fetchNextElementPinPtr->getName() << std::endl;
				inElementPinsForNet[newNetSourcePin]
					.push_back(fetchNextElementPinPtr);	
			}
			result |= found && !isPower;
		}
		// If result is true (and the element is not an output terminal) add a new Instance
		if (result && !isOutputTerminal){		       
			//std::cout << "Adding a Used Element: " << elementName << std::endl; 
			inUsedElements[elementName] = inPrimitiveStructure.mElements.find
				(elementName)->second;
			string newName = inInstancePtr->getName();
			newName = newName + ":" + elementName + ":" + cfgName;
			// We don't have definitive types yet so giving it the element name.
			string typeName = isInverter ? "INV" : elementName;
			torc::physical::InstanceSharedPtr newInstancePtr 
				= torc::physical::Factory::newInstancePtr(newName, typeName, 
									  "", "");   
			// Keeping the new instances in a vector to add them to the design later
			outNewInstances.push_back(newInstancePtr);
//std::cout << "THIS IS A NEW INSTANCE: " << newInstancePtr->getName() << std::endl;
			// Set the config for a new instance
			newInstancePtr->setConfig(elementName, cfgName, cfgSetting);
			std::map<std::string, 
				std::vector<const PrimitiveElement*> >::const_iterator it;
			// For non-MUX elements, copy over the configs of the 
			// orphans into the unpacked design 
			if(inPrimitiveStructure.mFlops.find(elementName) 
			   != inPrimitiveStructure.mFlops.end() 
			   || inPrimitiveStructure.mLUTs.find(elementName) 
			   != inPrimitiveStructure.mLUTs.end()){
				
				for(it = inPrimitiveStructure.mPrincipalstoOrphans.begin(); 
				    it != inPrimitiveStructure.mPrincipalstoOrphans.end(); it++){
					if(it->first != elementName) continue;
					for(std::vector<const PrimitiveElement*>
						    ::const_iterator iter = it->second.begin(); 
					    iter != it->second.end(); iter++){
							const PrimitiveElement* orphanPtr = *iter;
							const PrimitiveElement& orphan 
								= *orphanPtr;
							std::string orphanName = orphan.getName();
							std::string orphanCfgName, 
								orphanCfgSetting;
							bool configForOrphan = inInstancePtr
								->getConfig(orphanName, 
									    orphanCfgName, 
									    orphanCfgSetting);
							if(configForOrphan) 
								newInstancePtr->setConfig(
									orphanName, orphanCfgName,
									orphanCfgSetting);
					}					
				}
			}
			elementToInstanceMap[elementPtr] = newInstancePtr;
			return true;			
		}
		return false;			
	}
                                               

	void Unpacker::unpack(void) {	
		torc::physical::Circuit::InstanceSharedPtrConstIterator ip 
			= mDesignPtr->instancesBegin();
		torc::physical::Circuit::InstanceSharedPtrConstIterator ie 
			= mDesignPtr->instancesEnd();
		unpack(ip, ie);
	}


	void Unpacker::unpack(torc::physical::Circuit::InstanceSharedPtrConstIterator ip, 
			      torc::physical::Circuit::InstanceSharedPtrConstIterator ie) {
		std::vector<torc::physical::InstanceSharedPtr> oldInstances;		
		std::vector<torc::physical::InstanceSharedPtr> newInstances;		
		std::vector<torc::physical::InstanceSharedPtr>::iterator instIt;		
		// Iterating through all the instances
		while(ip < ie){
//std::cout << ">>>> ip: " << &ip << std::endl;
			torc::physical::InstanceSharedPtr instancePtr = *ip++;
			torc::physical::Instance& instance = *instancePtr; 
			std::string instanceType = instance.getType();
			std::string instanceName = instance.getName();
			CandidateNet elementPinsForNet;
			CandidateNet::const_iterator it1;
			// Maintining a list of used elements
			NameToElementPtrMap usedElements;
			NameToElementPtrMap::const_iterator it;
			ElementPtrToInstancePtrMap elementToInstanceMap;
			ElementPtrToInstancePtrMap::const_iterator elementToInstanceMapit;
			std::vector<std::string> instanceInputPins;		
			std::vector<std::string> instanceOutputPins;
			std::vector<std::string> usedPower;
			std::vector<std::string> usedGround;
			const PrimitiveElementPin* outNetSourcePin = 0;
			
//std::cout << instanceName << std::endl;
			PrimitiveStructuresSharedPtrMap::const_iterator pos;
			// Finding the primitive structure that matches the instance type
			pos = mPrimitiveStructures.find(instanceType);
			if(pos != mPrimitiveStructures.end()){
				// we need to retain this primitive structure!
				const PrimitiveStructure& primitiveStructure = *(pos->second);
				const PrimitiveDef* primitiveDefPtr 
					= primitiveStructure.getPrimitiveDefPtr();
				// this should not happen for valid instances
				if(primitiveDefPtr == 0) return;                         
				const PrimitivePinArray& primitivePins 
					= primitiveDefPtr->getPins();

				using torc::physical::InstancePinSharedPtr;
				using torc::physical::InstancePinSharedPtrVector;
				using torc::physical::Instance;
				using torc::physical::NetSharedPtr;
				using torc::architecture::xilinx::PinIndex;

				// iterate over the instance pins
				InstancePinSharedPtrVector inputPins;
				InstancePinSharedPtrVector outputPins;
				Instance::InstancePinSharedPtrConstIterator ipp 
					= instancePtr->pinsBegin();
				Instance::InstancePinSharedPtrConstIterator ipe 
					= instancePtr->pinsEnd();
				InstancePinSharedPtr instancePinPtr;
				NetSharedPtr netPtr;
				while(ipp != ipe) {
					// look up the instance pin
					InstancePinSharedPtr instancePinPtr = ipp++->second;
					const std::string& pinName = instancePinPtr->getPinName();
					netPtr = instancePinPtr->getParentWeakPtr().lock();
//std::cout << "    found " << pinName << " connected to net " << netPtr->getName() << ": ";
					// determine whether the pin is an output or input
					PinIndex pinIndex 
						= primitiveDefPtr->findPinIndexByName(pinName);
                                        // this shouldn't happen
					if(static_cast<boost::int32_t>(pinIndex) < 0) continue;
					const PrimitivePin& primitivePin = primitivePins[pinIndex];
					if(primitivePin.isInput()) 
						inputPins.push_back(instancePinPtr);
					if(primitivePin.isOutput()) 
						outputPins.push_back(instancePinPtr);
//std::cout << (primitivePin.isInput() ? "INPUT" : (primitivePin.isOutput() ? "OUTPUT" : ""))
//<< std::endl;
				}
				// // iterate over the input pins
				InstancePinSharedPtrVector::const_iterator ipp2;
				InstancePinSharedPtrVector::const_iterator ipe2;
				//InstancePinSharedPtr instancePinPtr;
				ipp2 = inputPins.begin();
				ipe2 = inputPins.end();
				while(ipp2 != ipe2) {
					InstancePinSharedPtr instancePinPtr = *ipp2++;
					instanceInputPins.push_back(instancePinPtr->getPinName());
//std::cout << "    found INPUT " << instancePinPtr->getPinName() << std::endl;               
				}
				// iterate over the output pins
				ipp2 = outputPins.begin();
				ipe2 = outputPins.end();
				while(ipp2 != ipe2) {
					InstancePinSharedPtr instancePinPtr = *ipp2++;
					instanceOutputPins.push_back(instancePinPtr->getPinName());
					std::cout << "    found OUTPUT " << instancePinPtr
						->getPinName() << std::endl;
					std::string outPinName = instancePinPtr->getPinName();
//std::cout << "Used output terminal found:" << outPinName << std::endl;
					const PrimitiveElement* elementPtr 
						= primitiveStructure.mTerminals.find(outPinName)
						->second;
					const PrimitiveElement& element = *elementPtr;
					findNextElement(primitiveStructure, element, instancePtr, 
							usedElements, instanceInputPins, 
							elementPinsForNet, outNetSourcePin, 
							newInstances, elementToInstanceMap);
				}
				// Iterating over the candidate nets
				for(it1 = elementPinsForNet.begin(); 
				    it1 != elementPinsForNet.end(); it1++){
					torc::physical::NetSharedPtr  net;
					PrimitiveElementPinPtrVector terminals;
					const PrimitiveElementPin* sourcePinPtr =  it1->first;
					std::string drivingElementName = sourcePinPtr
						->getElementPtr()->getName();
					std::string sourcePinName = sourcePinPtr->getName();
//std::cout << drivingElementName << "." << sourcePinName << " ---->" << std::endl;
					// Checking if the candidate net source is a terminal
					if(primitiveStructure.mTerminals
					   .find(sourcePinPtr->getElementPtr()->getName()) 
					   != primitiveStructure.mTerminals.end()) {
//std::cout << "Source (terminal) Pin Name: " << sourcePinName << std::endl;
						terminals.push_back(sourcePinPtr);
					} 
					// Iterating over the candidate net sinks
					for(PrimitiveElementPinPtrVector::const_iterator iter 
						    = it1->second.begin(); iter 
						    != it1->second.end(); iter++){
						const PrimitiveElementPin* sinkPinPtr 
							= *iter;
//std::cout << "Sink Pin Element: " << sinkPinPtr->getElementPtr()->getName() << std::endl;
						// Checking if the candidate net sinks 
						//are terminals
						if(primitiveStructure.mTerminals.find(sinkPinPtr
						   ->getElementPtr()->getName()) 
						   != primitiveStructure.mTerminals.end()){
							terminals.push_back(sinkPinPtr);
							std::string sinkPinName 
								= sinkPinPtr->getName();
//std::cout << sinkPinPtr->getElementPtr()->getName() << "." << sinkPinName << std::endl;
//std::cout << "Terminal Pin Name: " << sinkPinName << std::endl;
						}
					}
					// Number of terminals in the candidate net is 
					// more than one.. create a new net and collapse 
					// the old one
					if(terminals.size() > 1){
						std::string netName = instancePtr->getName() 
							+ ":" + drivingElementName + ":" 
							+ sourcePinName; 
						net = torc::physical::Factory::newNetPtr(netName);
						for(std::vector<const PrimitiveElementPin*>
							    ::const_iterator iter 
							    = terminals.begin(); 
						    iter != terminals.end(); iter++){
							Instance::InstancePinSharedPtrConstIterator
								pin = instancePtr
								->findPin(torc::physical
									  ::PinName(sourcePinPtr
										    ->getName()));
							InstancePinSharedPtr pinPtr = pin->second;
							torc::physical::NetSharedPtr net2 
								= pinPtr
								->getParentWeakPtr().lock();
							for(torc::physical::Net
								    ::InstancePinSharedPtrIterator 
								    netSourceIter = net2
								    ->sourcesBegin(); 
							    netSourceIter != net2->sourcesEnd(); 
							    netSourceIter++ ){
								net->addSource(*netSourceIter);
							}
							for(torc::physical::Net
								    ::InstancePinSharedPtrIterator 
								    netSinkIter = net2
								    ->sourcesBegin(); 
							    netSinkIter != net2->sourcesEnd(); 
							    netSinkIter++ ){
								net->addSink(*netSinkIter);
							}	
							mDesignPtr->removeNet(net2);
						}
					// Number of terminals is one.. 
					// get the net it is a part of..
					// this is the net we *modify*	
					} else if(terminals.size() == 1){
						Instance::InstancePinSharedPtrConstIterator pin 
							= instancePtr
							->findPin(torc::physical::PinName
								  (terminals.front()->getName()));
						InstancePinSharedPtr pinPtr = pin->second;
						net = pinPtr->getParentWeakPtr().lock();
						net->unroute();
					// No terminals in the candidate net..
					// create a new net and add source and sink to it
					} else {
						//define a new net
						std::cout << "Adding a new net" << std::endl;
						std::string netName = instancePtr->getName() 
							+ ":" + drivingElementName + ":" 
							+ sourcePinName; 
						net = torc::physical::Factory::newNetPtr(netName);
						mDesignPtr->addNet(net);
					}
					// For the source pin
//std::cout << "Adjusting net " << net->getName() << std::endl;
					if(primitiveStructure.mTerminals.find(sourcePinPtr
									      ->getElementPtr()
									      ->getName()) 
					   != primitiveStructure.mTerminals.end()) {
						Instance::InstancePinSharedPtrConstIterator pin1 
							= instancePtr
							->findPin(torc::physical::PinName
								  (sourcePinPtr->getName()));
						InstancePinSharedPtr pin1Ptr = pin1->second;
//std::cout << "removing a terminal (sink) pin from the net:  " << pin1Ptr->getPinName() 
//<< std::endl;
						net->removeSink(pin1Ptr);
					}else{					
						torc::physical::InstanceSharedPtr instanceSourcePtr
							= elementToInstanceMap.find(sourcePinPtr
							  ->getElementPtr())->second;
						InstancePinSharedPtr instanceSourcePin 
							= torc::physical::Factory
							::newInstancePinPtr(instanceSourcePtr, 
									    sourcePinName);
//std::cout << "Adding source pin: " << instanceSourcePin->getPinName() << std::endl << std::endl;
						net->addSource(instanceSourcePin);
					}
					const PrimitiveElementPinPtrVector& sinkVector
						= it1->second;
					//Iterate over the sink pins
					for(PrimitiveElementPinPtrVector
						    ::const_iterator iter = sinkVector.begin(); 
					    iter != sinkVector.end(); iter++){
						if(primitiveStructure.mTerminals.find((*iter)
						   ->getElementPtr()->getName()) 
						   != primitiveStructure.mTerminals.end()) {
							Instance::InstancePinSharedPtrConstIterator
								pin2 = instancePtr
								->findPin(torc::physical::PinName
									  ((*iter)->getName()));
							InstancePinSharedPtr pin2Ptr 
								= pin2->second;
							net->removeSource(pin2Ptr);
//std::cout << "removing a terminal (source) pin from the net:  " << pin2Ptr->getPinName() 
//<< std::endl;
						}else{
							torc::physical::InstanceSharedPtr 
								instanceSinkPtr 
								= elementToInstanceMap.find((*iter)
								  ->getElementPtr())->second;
							InstancePinSharedPtr instanceSinkPin 
								= torc::physical::Factory
								::newInstancePinPtr
								(instanceSinkPtr, 
								 (*iter)->getName());
//std::cout << "INSTANCE SINK PIN: " << instanceSinkPin->getPinName() << std::endl << std::endl;
							net->addSink(instanceSinkPin);
						}					
					}
					terminals.clear();
				}				
				// remember to remove the original instance from the design
				oldInstances.push_back(instancePtr);
			}
			// std::cout << "----------- USED ELEMENTS ------------" << std::endl;
			// for(it = usedElements.begin(); it != usedElements.end(); it++){
			// 	std::cout << " " << it->first << std::endl;
			// }
		}
		std::cout << "-------------------- NEW INSTANCES -------------------" << std::endl;
		for(instIt = newInstances.begin(); instIt != newInstances.end(); instIt++){
			torc::physical::InstanceSharedPtr newInstancePtr = *instIt; 
			torc::physical::Instance newInstance = *newInstancePtr;
			std::cout << " " << newInstance.getName() << std::endl;
			mDesignPtr->addInstance(newInstancePtr);
		}
		std::cout << "-------------------- OLD INSTANCES -------------------" << std::endl;
		for(instIt = oldInstances.begin(); instIt != oldInstances.end(); instIt++){
			torc::physical::InstanceSharedPtr oldInstancePtr = *instIt; 
			torc::physical::Instance oldInstance = *oldInstancePtr;
			std::cout << " " << oldInstance.getName() << std::endl;
			mDesignPtr->removeInstance(oldInstancePtr);
		}

	}
}// namespace packer
}// namespace torc 
