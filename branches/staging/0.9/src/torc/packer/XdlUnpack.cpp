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

#include "XdlUnpack.hpp"
#include "RcFactory.hpp"
#include <iostream>

namespace torc {
namespace physical {

	DesignSharedPtr XdlUnpack::operator() (const DesignSharedPtr& inDesignPtr) {

		// export the design statement
		DesignSharedPtr designPtr(inDesignPtr);
		const Design& design = *designPtr;
		DesignSharedPtr newDesign = unpack(design);
		
		return newDesign;

		/// \todo Release mutex.
	}

	void XdlUnpack::unpack(const Circuit& circuit, CircuitSharedPtr newCircuit ) {
		
		// export the nets
		Circuit::NetSharedPtrConstIterator pn = circuit.netsBegin();
		Circuit::NetSharedPtrConstIterator en = circuit.netsEnd();
		while(pn < en){
			NetSharedPtr netPtr = unpack(**pn++);
			newCircuit->addNet(netPtr);
		}
		
		// export the instances
		Circuit::InstanceSharedPtrConstIterator pi = circuit.instancesBegin();
		Circuit::InstanceSharedPtrConstIterator ei = circuit.instancesEnd();
		while(pi < ei) {
			unpack(**pi++, newCircuit);
		}
	}

	DesignSharedPtr XdlUnpack::unpack(const Design& design) {
		string designName;
		designName = design.getName();
		designName += "_unpacked";
		
		DesignSharedPtr newDesign = Factory::newDesignPtr (designName, design.getDevice(), "", "", design.getXdlVersion());
		
		string deviceName = design.getDevice();
		deviceName += ".xdlrc";
		readPrimitives(deviceName.c_str());
		removeExtraElements(*primitives->findPrimitive("SLICEL"));
		removeExtraElements(*primitives->findPrimitive("SLICEM"));
				
		if(!design.configIsEmpty()) {
			newDesign->addConfigs(static_cast<const ConfigMap&>(design));
		}
		
		// export the modules
		Design::ModuleSharedPtrConstIterator p = design.modulesBegin();
		Design::ModuleSharedPtrConstIterator e = design.modulesEnd();
		while(p < e){
			ModuleSharedPtr modulePtr= unpack(**p++);
			 newDesign->addModule(modulePtr);
		}
		// export the instances and nets
		unpack(static_cast<Circuit>(design), static_cast<CircuitSharedPtr>(newDesign));
	
		return newDesign;

	}

	ModuleSharedPtr XdlUnpack::unpack(const Module& module) {
		
		ModuleSharedPtr newModule = Factory::newModulePtr (module.getName(), module.getAnchor());
		
		if(!module.configIsEmpty()) 
			{newModule->addConfigs(static_cast<const ConfigMap&>(module)); }
		
		// export the ports
		Module::PortSharedPtrConstIterator p = module.portsBegin();
		Module::PortSharedPtrConstIterator e = module.portsEnd();
		while(p < e){
			PortSharedPtr portPtr = unpack(**p++);
			newModule->addPort(portPtr);
		}
		
		// export the instances and nets
		unpack(static_cast<Circuit>(module), static_cast<CircuitSharedPtr>(newModule));
		
		return newModule;
	}

	PortSharedPtr 	XdlUnpack::unpack(const Port& port) {
		InstanceSharedPtr instPtr = port.getInstancePtr().lock(); 
		PortSharedPtr newPort = Factory::newPortPtr (port.getName(), instPtr, port.getPinName());
		return newPort;
	}

	void XdlUnpack::unpack(Instance& instance, CircuitSharedPtr newCircuit) {
		
		// change SLICEL and SLICEM
		if((instance.getType()=="SLICEL")||(instance.getType()=="SLICEM")){
			InstanceSharedPtrVector affInstances,bffInstances,cffInstances,dffInstances, xffInstances, yffInstances;
			InstanceSharedPtrVector alutInstances,blutInstances,clutInstances,dlutInstances,xlutInstances,ylutInstances;
			ConfigMap instanceConfigs;
			ConfigMap affConfigs, bffConfigs, cffConfigs, dffConfigs,xffConfigs,yffConfigs;
			ConfigMap alutConfigs, blutConfigs, clutConfigs, dlutConfigs,xlutConfigs,ylutConfigs;
			PrimitiveSet::PrimitiveSharedPtrIterator it = primitives->findPrimitive(instance.getType());
			PrimitiveSharedPtr primitivePtr = *it;
			ConfigMap::const_iterator p = instance.configBegin();
			ConfigMap::const_iterator e = instance.configEnd();
			InstanceReferenceSharedPtr instanceReferencePtr = instance.getInstanceReferencePtr();
			InstanceReferenceSharedPtr newInstanceReferencePtr;
			
			if(instanceReferencePtr.get() != 0) {
				const InstanceReference& instanceReference = *instanceReferencePtr;
				std::string instanceName = instanceReference.getInstancePtr().expired()
					? "[expired instance]" : instanceReference.getInstancePtr().lock()->getName();
				newInstanceReferencePtr = Factory::newInstanceReferencePtr (
									instanceName, instanceReference.getModulePtr().lock(), instanceReference.getInstancePtr().lock());
			}
			
			// Define the instances based on configs
			while(p != e) {
				const std::string& setting = p->first;
				
				// skip if this is not a valid config
				if(setting == "_NO_USER_LOGIC")
					return;
				Primitive::ElementSharedPtrIterator elemIter = primitivePtr->findElement(setting);
				if(elemIter == primitivePtr->elementsEnd()){
					++p;
					continue;
				}
				string newName = instance.getName();
				
				const Config& config = p->second;
				newName = newName + ":" + setting + ":" + config.getName() ;
				string typeName;
				
				// If this is a configured mux
				if(isExtraMux(setting)){
					instanceConfigs.setConfig(setting, config);	
						++p;
						continue;
				}
				// if this is an inverter
				if(strstr(setting.c_str(),"INV")!= NULL){
					if(strstr(config.getValue().c_str() , "_B") == NULL){
						instanceConfigs.setConfig(setting, config);
						++p;
						continue;
					}
					typeName = "INV";
					instanceConfigs.setConfig(setting, config);
				}
				// luts
				else if((strstr(setting.c_str(),"LUT")!= NULL)|| (setting == "G")|| (setting == "F")){
					typeName = "LUT";
					instanceConfigs.setConfig(setting, config);
				}
				else if((strstr(setting.c_str(),"AND")!= NULL)){
					typeName = "AND";
					instanceConfigs.setConfig(setting, config);
				}
				else if((strstr(setting.c_str(),"XOR")!= NULL)){
					typeName = "XOR";
					instanceConfigs.setConfig(setting, config);
				}
				else if((setting == "AFF") || (setting == "BFF") || (setting == "CFF") || (setting == "DFF") || (setting == "FFX") || (setting == "FFY")){
					typeName = "DFF";
					instanceConfigs.setConfig(setting, config);
				}
				else if( ( setting == "AFFSR") ||
						 (setting == "AFFINIT") ){
							affConfigs.setConfig(setting,config);
							++p;
							continue;
				}
				else if( ( setting == "BFFSR") ||
						 (setting == "BFFINIT") ){
							bffConfigs.setConfig(setting,config);
							++p;
							continue;
				}
				else if( ( setting == "CFFSR") ||
						 (setting == "CFFINIT") ){
							cffConfigs.setConfig(setting,config);
							++p;
							continue;
				}
				else if( ( setting == "DFFSR") ||
						 (setting == "DFFINIT") ){
							dffConfigs.setConfig(setting,config);
							++p;
							continue;
				}
				else if( ( setting == "FFY_INIT_ATTR") ||
						 (setting == "FFY_SR_ATTR") ){
							yffConfigs.setConfig(setting,config);
							++p;
							continue;
				}
				else if( ( setting == "FFX_INIT_ATTR") ||
						 (setting == "FFX_SR_ATTR") ){
							xffConfigs.setConfig(setting,config);
							++p;
							continue;
				}
				else if(setting == "A6RAMMODE"){
					alutConfigs.setConfig(setting,config);
					++p;
					continue;
				}
				else if(setting == "B6RAMMODE"){
					blutConfigs.setConfig(setting,config);
					++p;
					continue;
				}
				else if(setting == "C6RAMMODE"){
					clutConfigs.setConfig(setting,config);
					++p;
					continue;
				}
				else if(setting == "D6RAMMODE"){
					dlutConfigs.setConfig(setting,config);
					++p;
					continue;
				}
				else if(setting == "D6RAMMODE"){
					dlutConfigs.setConfig(setting,config);
					++p;
					continue;
				}
				else if(setting == "F_ATTR"){
					xlutConfigs.setConfig(setting,config);
					++p;
					continue;
				}
				else if(setting == "G_ATTR"){
					ylutConfigs.setConfig(setting,config);
					++p;
					continue;
				}
				else if((strstr(setting.c_str(), "MUX")!=NULL)||(strstr(setting.c_str(), "CY0")!=NULL)||
					(setting=="PRECYINIT")){
					typeName = "MUX";
					instanceConfigs.setConfig(setting, config);						
					if(isExtraMux(setting)){
						++p;
						continue;
					}
				}
				else if(strstr(setting.c_str(),"SYNC_ATTR")!= NULL){
							affConfigs.setConfig(setting,config);
							bffConfigs.setConfig(setting,config);
							cffConfigs.setConfig(setting,config);
							dffConfigs.setConfig(setting,config);
							xffConfigs.setConfig(setting,config);
							yffConfigs.setConfig(setting,config);
							
							++p;
							continue;
				}
				else if((setting == "_INST_PROP") || (setting == "_ROUTETHROUGH")){
						++p;
						continue;
				}
				else{
					typeName = setting;
					instanceConfigs.setConfig(setting, config);
				}
				
				
				InstanceSharedPtr newInst = Factory::newInstancePtr(newName, typeName, "", 
																	"", instance.getBonding(), newInstanceReferencePtr);
				if(typeName == "DFF") {
					if(setting == "AFF")
						affInstances.push_back(newInst);
					else if(setting == "BFF")
						bffInstances.push_back(newInst);
					else if(setting == "CFF")
						cffInstances.push_back(newInst);
					else if(setting == "DFF")
						dffInstances.push_back(newInst);
					else if(setting == "FFX")
						xffInstances.push_back(newInst);
					else if(setting == "FFY")
						yffInstances.push_back(newInst);
				} else if(typeName == "LUT") {
					if(setting == "A6LUT")
						alutInstances.push_back(newInst);
					else if(setting == "B6LUT")
						blutInstances.push_back(newInst);
					else if(setting == "C6LUT")
						clutInstances.push_back(newInst);
					else if(setting == "D6LUT")
						dlutInstances.push_back(newInst);
					else if(setting == "F")
						xlutInstances.push_back(newInst);
					else if(setting == "G")
						ylutInstances.push_back(newInst);
				} else if((typeName != "INV") && (typeName != "MUX") && (typeName != "AND") && (typeName != "XOR")  && (typeName != "WSGEN")) {
					newInst->setConfig(p->first, config);				
				}
				newCircuit->addInstance(newInst);
				p++;
			}
			
			// set flipflop configs if any
			for(Circuit::InstanceSharedPtrIterator it = affInstances.begin(); it != affInstances.end(); ++it)
				(*it)->addConfigs(affConfigs);
			for(Circuit::InstanceSharedPtrIterator it = bffInstances.begin(); it != bffInstances.end(); ++it)
				(*it)->addConfigs(bffConfigs);
			for(Circuit::InstanceSharedPtrIterator it = cffInstances.begin(); it != cffInstances.end(); ++it)
				(*it)->addConfigs(cffConfigs);
			for(Circuit::InstanceSharedPtrIterator it = dffInstances.begin(); it != dffInstances.end(); ++it)
				(*it)->addConfigs(dffConfigs);
			for(Circuit::InstanceSharedPtrIterator it = xffInstances.begin(); it != xffInstances.end(); ++it)
				(*it)->addConfigs(xffConfigs);
			for(Circuit::InstanceSharedPtrIterator it = yffInstances.begin(); it != yffInstances.end(); ++it)
				(*it)->addConfigs(yffConfigs);
				
			// set lut configs if any
			for(Circuit::InstanceSharedPtrIterator it = alutInstances.begin(); it != alutInstances.end(); ++it)
				(*it)->addConfigs(alutConfigs);
			for(Circuit::InstanceSharedPtrIterator it = blutInstances.begin(); it != blutInstances.end(); ++it)
				(*it)->addConfigs(blutConfigs);
			for(Circuit::InstanceSharedPtrIterator it = clutInstances.begin(); it != clutInstances.end(); ++it)
				(*it)->addConfigs(clutConfigs);
			for(Circuit::InstanceSharedPtrIterator it = dlutInstances.begin(); it != dlutInstances.end(); ++it)
				(*it)->addConfigs(dlutConfigs);
			for(Circuit::InstanceSharedPtrIterator it = xlutInstances.begin(); it != xlutInstances.end(); ++it)
				(*it)->addConfigs(xlutConfigs);
			for(Circuit::InstanceSharedPtrIterator it = ylutInstances.begin(); it != ylutInstances.end(); ++it)
				(*it)->addConfigs(ylutConfigs);
				
			Circuit::NetSharedPtrConstIterator pn = newCircuit->netsBegin();
			Circuit::NetSharedPtrConstIterator en = newCircuit->netsEnd();
			while(pn < en){
				NetSharedPtr netPtr = *pn++;
				// Change current nets
								
				// change the sources
				
				Net::InstancePinSharedPtrIterator sop = netPtr->sourcesBegin();
				while(sop < netPtr->sourcesEnd()){
					
					if((*sop)->getInstancePtr().lock()->getName()==instance.getName()){
						string pinName = (*sop)->getPinName();
						netPtr->removeSource(*sop);
						PrimitivePinSharedPtr pinTmp = *primitivePtr->findPrimitivePin(pinName);
						ElementSharedPtr elementTmp = *primitivePtr->findElement(pinName);
						Element::ConnectionSharedPtrIterator cb = elementTmp->connectionsBegin();
						ConnectionPin pinPtr = * (*cb)->getSource();
						string newName = instance.getName();
						Config config;
						instanceConfigs.getConfig(pinPtr.getElementName(), config);
						
						if(isExtraMux(pinPtr.getElementName())){
							ElementSharedPtr muxElement = *primitivePtr->findElement(pinPtr.getElementName());
							Element::ConnectionSharedPtrIterator cb1 = muxElement->connectionsBegin();
							Element::ConnectionSharedPtrIterator ce1 = muxElement->connectionsEnd();
							while(cb1 < ce1){
								ConnectionPin pinPtr1 = * (*cb1)->getSink();
								if(pinPtr1.getPinName()== config.getValue()){
									pinPtr = * (*cb1)->getSource();
									instanceConfigs.getConfig(pinPtr.getElementName(),config);
									break;
								}
								++cb1;
							}
						}
						if(!instanceConfigs.hasConfig(pinPtr.getElementName())){
								++sop;
								continue;
						}
						newName = newName + ":" + pinPtr.getElementName()+ ":" +config.getName();					
						InstanceSharedPtr instPtr = *newCircuit->findInstance(newName);
						InstancePinSharedPtr instPin = Factory::newInstancePinPtr (instPtr, pinPtr.getPinName());
						netPtr->addSource(instPin);		
						sop = netPtr->sourcesBegin();			
					}
					else
					++sop;
				}
				
				// change the sinks
				
				Net::InstancePinSharedPtrIterator sip = netPtr->sinksBegin();
				while(sip != netPtr->sinksEnd()){	
					if((*sip)->getInstancePtr().lock()->getName()==instance.getName()){
						string pinName = (*sip)->getPinName();
						
						netPtr->removeSink(*sip);
						PrimitivePinSharedPtr pinTmp = *primitivePtr->findPrimitivePin(pinName);
						ElementSharedPtr elementTmp = *primitivePtr->findElement(pinName);
						Element::ConnectionSharedPtrIterator cb = elementTmp->connectionsBegin();
						Element::ConnectionSharedPtrIterator ce = elementTmp->connectionsEnd();
						while(cb < ce){
							ConnectionPin pinPtr = * (*cb)->getSink();
							if(!instanceConfigs.hasConfig(pinPtr.getElementName())){
								++cb;
								continue;
							}
							
							Config config;
							instanceConfigs.getConfig(pinPtr.getElementName(),config);
							if(isExtraMux(pinPtr.getElementName()) || (strstr(pinPtr.getElementName().c_str() , "INV")!=NULL && strstr(config.getValue().c_str(), "_B")==NULL)){
								
								if(config.getValue() != pinPtr.getPinName()){
									++cb;
									continue;
								}
								ElementSharedPtr muxElement = *primitivePtr->findElement(pinPtr.getElementName());
								Element::ConnectionSharedPtrIterator cb1 = muxElement->connectionsBegin();
								Element::ConnectionSharedPtrIterator ce1 = muxElement->connectionsEnd();
								while(cb1 < ce1){
									ConnectionPin pinPtr1 = * (*cb1)->getSource();
									if(pinPtr1.getElementName()== muxElement->getName()){
										pinPtr = * (*cb1)->getSink();
										if(!instanceConfigs.getConfig(pinPtr.getElementName(),config)){
											++cb1;
											continue;
										}
										
										if(isExtraMux(pinPtr.getElementName()) || (strstr(pinPtr.getElementName().c_str() , "INV")!=NULL && strstr(config.getValue().c_str(), "_B")==NULL)){
																			
											if(config.getValue() != pinPtr.getPinName()){
												++cb1;
												continue;
											}
											muxElement = *primitivePtr->findElement(pinPtr.getElementName());
											cb1 = muxElement->connectionsBegin();
											ce1 = muxElement->connectionsEnd();
											continue;
										}
										string newName = instance.getName();
										newName = newName + ":" + pinPtr.getElementName() + ":" + config.getName();						
										
										InstanceSharedPtr instPtr = *newCircuit->findInstance(newName);
										InstancePinSharedPtr instPin = Factory::newInstancePinPtr (instPtr, pinPtr.getPinName());
										netPtr->addSink(instPin);
									}
									++cb1;
								}
							}
							else{
								
								string newName = instance.getName();
								newName = newName + ":" + pinPtr.getElementName() + ":" + config.getName();						
								
								InstanceSharedPtr instPtr = *newCircuit->findInstance(newName);
								InstancePinSharedPtr instPin = Factory::newInstancePinPtr (instPtr, pinPtr.getPinName());
								netPtr->addSink(instPin);
							}
								++cb;
						}
						sip = netPtr->sinksBegin();
					}
					else
					++sip;
				}
				
			}
			
			// add new nets
			p = instanceConfigs.configBegin();
			e = instanceConfigs.configEnd();
			while(p != e) {
				const std::string& setting = p->first;
					Config config=p->second;
				if(isExtraMux(setting)|| (strstr(setting.c_str() , "INV")!=NULL && strstr(config.getValue().c_str(), "_B")==NULL)){
					++p;
					continue;
				}
				string instName = instance.getName();
				instName = instName + ":" + setting + ":" +config.getName();	
				
				ElementSharedPtr elemPtr = * primitivePtr->findElement(setting);
				Element::ConnectionSharedPtrIterator cb = elemPtr->connectionsBegin();
				Element::ConnectionSharedPtrIterator ce = elemPtr->connectionsEnd();
				while(cb != ce){
					ConnectionPin srcPtr = * (*cb)->getSource();
					ConnectionPin sinkPtr = * (*cb)->getSink();
					if(srcPtr.getElementName() == elemPtr->getName()){
						string netName = instName;
						netName += ":" + srcPtr.getPinName();
						if(instanceConfigs.hasConfig(sinkPtr.getElementName())){
							if(isExtraMux(sinkPtr.getElementName())){
								Config config;
								instanceConfigs.getConfig(sinkPtr.getElementName(),config);
								if(config.getValue() != sinkPtr.getPinName()){
									++cb;
									continue;
								}
								ElementSharedPtr muxElement = *primitivePtr->findElement(sinkPtr.getElementName());
								Element::ConnectionSharedPtrIterator cb1 = muxElement->connectionsBegin();
								Element::ConnectionSharedPtrIterator ce1 = muxElement->connectionsEnd();
								while(cb1 < ce1){
									ConnectionPin pinPtr1 = * (*cb1)->getSource();
									if(pinPtr1.getElementName()== muxElement->getName()){
										sinkPtr = * (*cb1)->getSink();
										instanceConfigs.getConfig(sinkPtr.getElementName(),config);
										break;
									}
									++cb1;
								}
							}
							if(isExtraMux(sinkPtr.getElementName())){
								Config config;
								instanceConfigs.getConfig(sinkPtr.getElementName(),config);
								if(config.getValue() != sinkPtr.getPinName()){
									++cb;
									continue;
								}
								ElementSharedPtr muxElement = *primitivePtr->findElement(sinkPtr.getElementName());
								Element::ConnectionSharedPtrIterator cb1 = muxElement->connectionsBegin();
								Element::ConnectionSharedPtrIterator ce1 = muxElement->connectionsEnd();
								while(cb1 < ce1){
									ConnectionPin pinPtr1 = * (*cb1)->getSource();
									if(pinPtr1.getElementName()== muxElement->getName()){
										sinkPtr = * (*cb1)->getSink();
										instanceConfigs.getConfig(sinkPtr.getElementName(),config);
										break;
									}
									++cb1;
								}
							}
							if(!instanceConfigs.hasConfig(sinkPtr.getElementName())){
								++cb;
								continue;
							}
							  Circuit::NetSharedPtrIterator netIt = newCircuit->findNet(netName);
							  if(netIt == newCircuit->netsEnd()){
							  	NetSharedPtr  newNet = Factory::newNetPtr (netName);
							  	InstanceSharedPtr instPtr = *newCircuit->findInstance(instName);
									InstancePinSharedPtr instPin = Factory::newInstancePinPtr (instPtr, srcPtr.getPinName());
									newNet->addSource(instPin);
							  	newCircuit->addNet(newNet);
							  	netIt = newCircuit->findNet(netName);
							  }
							  Config config;
							  instanceConfigs.getConfig(sinkPtr.getElementName(),config);
							  string sinkInstName = instance.getName()+":"+sinkPtr.getElementName()+":"+config.getName();
							  InstanceSharedPtr instPtr = *newCircuit->findInstance(sinkInstName);
								InstancePinSharedPtr instPin = Factory::newInstancePinPtr (instPtr, sinkPtr.getPinName());
								(*netIt)->addSink(instPin);
							  
						}					  	
					}
					++cb;
				}
				++p;
			}
			
			
			return;
		}
		
		InstanceReferenceSharedPtr instanceReferencePtr = instance.getInstanceReferencePtr();
		InstanceReferenceSharedPtr newInstanceReferencePtr;
		if(instanceReferencePtr.get() != 0) {
			const InstanceReference& instanceReference = *instanceReferencePtr;
			std::string instanceName = instanceReference.getInstancePtr().expired()
				? "[expired instance]" : instanceReference.getInstancePtr().lock()->getName();
			newInstanceReferencePtr = Factory::newInstanceReferencePtr (
								instanceName, instanceReference.getModulePtr().lock(), instanceReference.getInstancePtr().lock());
		}
		string typeName;		
		typeName = instance.getType();
			
		InstanceSharedPtr newInst = Factory::newInstancePtr(instance.getName(), typeName, instance.getTile(), 
															 instance.getSite(), instance.getBonding(), newInstanceReferencePtr);
		if(!instance.configIsEmpty()) 
			{ newInst->addConfigs(static_cast<const ConfigMap&>(instance)); }
		
		newCircuit->addInstance(newInst);
		
	}

	NetSharedPtr XdlUnpack::unpack(const Net& net) {
		
		NetSharedPtr  newNet = Factory::newNetPtr (net.getName(), net.getNetType());
		
		
		size_t left = net.getSourceCount() + net.getSinkCount() + net.getPipCount() 
			+ (net.configIsEmpty() ? 0 : 1);
				
		// unpack the sources
		Net::InstancePinSharedPtrConstIterator sop = net.sourcesBegin();
		Net::InstancePinSharedPtrConstIterator soe = net.sourcesEnd();
		while(sop < soe){
			 InstancePinSharedPtr instPin = unpack(*sop++, ePinDirectionOutpin, --left > 0);
			 newNet->addSource(instPin);
		}
		// unpack the sinks
		Net::InstancePinSharedPtrConstIterator sip = net.sinksBegin();
		Net::InstancePinSharedPtrConstIterator sie = net.sinksEnd();
		while(sip < sie) {
			InstancePinSharedPtr instPin = unpack(*sip++, ePinDirectionInpin, --left > 0);
			newNet->addSink(instPin);
		}
		
		// unpack the config string if there is one
		if(!net.configIsEmpty()) { 
			newNet->addConfigs(static_cast<const ConfigMap&>(net)); 			
		}
		return newNet;
	}

	InstancePinSharedPtr XdlUnpack::unpack(const InstancePinSharedPtr instancePinSharedPtr, EPinDirection pinDirection, 
		bool comma) {
		
		InstancePinSharedPtr instPin = Factory::newInstancePinPtr (instancePinSharedPtr->getInstancePtr().lock(), instancePinSharedPtr->getPinName());
		return instPin;
	}

	Pip XdlUnpack::unpack(const Pip& pip, bool comma) {		
		RoutethroughSharedPtr 	newRoutethrough;
		if(pip.isRoutethrough()) 
			newRoutethrough = unpack(*pip.getRoutethroughPtr());
		else
			newRoutethrough = RoutethroughSharedPtr();
		Pip newPip = Factory::newPip(pip.getTileName(), pip.getSourceWireName(), pip.getSinkWireName(), pip.getDirection(), RoutethroughSharedPtr());
		
		return newPip;
	}

	RoutethroughSharedPtr XdlUnpack::unpack(const Routethrough& routethrough) {
		 RoutethroughSharedPtr 	newRoutethrough = Factory::newRoutethroughPtr (
		 	routethrough.getSetting(), routethrough.getName(), routethrough.getValue(), routethrough.getInstancePtr(), 
		 	routethrough.getSourceWireName(), routethrough.getSinkWireName());
		return newRoutethrough;
	}
	
	void XdlUnpack::removeExtraElements(PrimitiveSharedPtr primPtr){
		Primitive::ElementSharedPtrIterator elemIt;
		ElementSharedPtr elemPtr;
		for(elemIt=primPtr->elementsBegin(); elemIt != primPtr->elementsEnd(); ++elemIt){
			if(strstr((*elemIt)->getName().c_str(), "USED")!=NULL){
				primPtr->removeElement(*elemIt);
				continue;
			}
				
			char names[1024] ;
			strcpy(names,elementNames.c_str());
			string pch;
			pch = strtok (names," ");
			while (pch!="END")
			{
				if(pch == (*elemIt)->getName()){
					primPtr->removeElement(*elemIt);		
					break;
				}
				pch = strtok (NULL," ");
			}
		}
	}

} // namespace physical
} // namespace torc

