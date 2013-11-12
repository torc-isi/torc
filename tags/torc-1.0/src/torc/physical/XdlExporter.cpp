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
/// \brief Source for the XdlExporter class.

#include "torc/physical/XdlExporter.hpp"
#include <iosfwd>

namespace torc {
namespace physical {

	void XdlExporter::operator() (const DesignSharedPtr& inDesignPtr) {
		//	std::cout << "Design " << inDesignPtr->getName() << std::endl;
		//	std::cout << "    module count: " << inDesignPtr->getModuleCount() << std::endl;
		//	std::cout << "    instance count: " << inDesignPtr->getInstanceCount() << std::endl;
		//	std::cout << "    net count: " << inDesignPtr->getNetCount() << std::endl;

		/// \todo Acquire mutex.

		// export the design statement
		DesignSharedPtr designPtr(inDesignPtr);
		const Design& design = *designPtr;
		write(design);

		/// \todo Release mutex.
	}

	void XdlExporter::write(const Circuit& circuit) {
		// export the instances
		Circuit::InstanceSharedPtrConstIterator pi = circuit.instancesBegin();
		Circuit::InstanceSharedPtrConstIterator ei = circuit.instancesEnd();
		while(pi < ei) write(**pi++);

		// export the nets
		Circuit::NetSharedPtrConstIterator pn = circuit.netsBegin();
		Circuit::NetSharedPtrConstIterator en = circuit.netsEnd();
		while(pn < en) write(**pn++);
	}

	void XdlExporter::write(const Design& design) {
		indent();
		mStream << "design \"" << design.getName() << "\" " << design.getDevice() 
			<< design.getPackage() << design.getSpeedGrade() << " " << design.getXdlVersion();
		if(!design.configIsEmpty()) 
			{ mStream << ", "; write(static_cast<const ConfigMap&>(design)); }
		mStream << ";" << std::endl;
		// export the modules
		Design::ModuleSharedPtrConstIterator p = design.modulesBegin();
		Design::ModuleSharedPtrConstIterator e = design.modulesEnd();
		while(p < e) write(**p++);
		// export the instances and nets
		write(static_cast<Circuit>(design));

	}

	void XdlExporter::write(const Module& module) {
		indent();
		mStream << "module \"" << module.getName() << "\" \"" << module.getAnchor() << "\"";
		if(!module.configIsEmpty()) 
			{ mStream << ", "; write(static_cast<const ConfigMap&>(module)); }
		mStream << ";" << std::endl;
		// increment the indent
		mIndentCount++;
		// export the ports
		Module::PortSharedPtrConstIterator p = module.portsBegin();
		Module::PortSharedPtrConstIterator e = module.portsEnd();
		while(p < e) write(**p++);
		// export the instances and nets
		write(static_cast<Circuit>(module));
		// decrement the indent
		mIndentCount--;
		// close out the module
		indent();
		mStream << "endmodule \"" << module.getName() << "\";" << std::endl;
	}

	void XdlExporter::write(const Port& port) {
		indent();
		std::string instanceName = port.getInstancePtr().expired() 
			? "[expired instance]" : port.getInstancePtr().lock()->getName();
		mStream << "port \"" << port.getName() << "\" \"" << instanceName << "\" \"" 
			<< port.getPinName() << "\";" << std::endl;
	}

	void XdlExporter::write(const ConfigMap& configMap) {
		// nothing to do if the configMap is empty
		if(configMap.configIsEmpty()) return;
		// write the config keyword and double quotes
		mStream << "cfg \"";
		// iterate through the config settings
		ConfigMap::const_iterator p = configMap.configBegin();
		ConfigMap::const_iterator e = configMap.configEnd();
		while(p != e) {
			const std::string& setting = p->first;
			const Config& config = p->second;
			mStream << setting << ":" << config.getName() << ":" << config.getValue();
			p++;
			if(p != e) mStream << " ";
		}
		// close the double quotes
		mStream << "\"";
	}

	void XdlExporter::write(const Instance& instance) {
		indent();
		mStream << "inst \"" << instance.getName() << "\" \"" << instance.getType() << "\", ";
		const std::string& site = instance.getSite();
		std::string tile = instance.getTile();
		EInstanceBonding bonding = instance.getBonding();
		if(site.empty()) {
			mStream << "unplaced";
			// bonding may be specified for unplaced IOBs, but is ignored for unplaced instances of 
			// other types, and is illegal for placed instances of any type
			if(bonding != eInstanceBondingUnknown) {
				mStream << " " << (bonding == eInstanceBondingUnbonded ? "unbonded" : "bonded");
			}
		} else {
			if(tile.empty()) tile = "unknown";
			mStream << "placed " << tile << " " << site;
		}
		InstanceReferenceSharedPtr instanceReferencePtr = instance.getInstanceReferencePtr();
		if(instanceReferencePtr.get() != 0) {
			const InstanceReference& instanceReference = *instanceReferencePtr;
			std::string moduleName = instanceReference.getModulePtr().expired() 
				? "[expired module]" : instanceReference.getModulePtr().lock()->getName();
			std::string instanceName = instanceReference.getInstancePtr().expired()
				? "[expired instance]" : instanceReference.getInstancePtr().lock()->getName();
			mStream << ", module \"" << instanceReference.getInstantiationName() << "\" \"" 
				<< moduleName << "\" \"" << instanceName << "\"";
		}
		if(!instance.configIsEmpty()) 
			{ mStream << ", "; write(static_cast<const ConfigMap&>(instance)); }
		mStream << ";" << std::endl;
	}

	void XdlExporter::write(const Net& net) {
		indent();
		// write the header
		mStream << "net \"" << net.getName() << "\"";
		switch(net.getNetType()) {
			case eNetTypePower: mStream << " power"; break;
			case eNetTypeGround: mStream << " ground"; break;
			default: break;
		}
		size_t left = net.getSourceCount() + net.getSinkCount() + net.getPipCount() 
			+ (net.configIsEmpty() ? 0 : 1);
		bool empty = (left == 0);
		mStream << (empty ? "" : ",") << std::endl;
		// write the sources
		Net::InstancePinSharedPtrConstIterator sop = net.sourcesBegin();
		Net::InstancePinSharedPtrConstIterator soe = net.sourcesEnd();
		while(sop < soe) write(**sop++, ePinDirectionOutpin, --left > 0);
		// write the sinks
		Net::InstancePinSharedPtrConstIterator sip = net.sinksBegin();
		Net::InstancePinSharedPtrConstIterator sie = net.sinksEnd();
		while(sip < sie) write(**sip++, ePinDirectionInpin, --left > 0);
		// write the pips
		Net::PipConstIterator pip = net.pipsBegin();
		Net::PipConstIterator pie = net.pipsEnd();
		while(pip < pie) write(*pip++, --left > 0);
		// write the config string if there is one
		if(!net.configIsEmpty()) { 
			indent(); 
			mStream << mIndentString;
			write(static_cast<const ConfigMap&>(net)); 
			mStream << std::endl; 
		}
		// indent before the closing semicolon if the net was not completely empty
		if(!empty) { indent(); mStream << mIndentString; }
		mStream << ";" << std::endl;
	}

	void XdlExporter::write(const InstancePin& instancePin, EPinDirection pinDirection, 
		bool comma) {
		indent();
		std::string instanceName = instancePin.getInstancePtr().expired() 
			? "[expired instance]" : instancePin.getInstancePtr().lock()->getName();
		mStream << mIndentString << (pinDirection == ePinDirectionInpin ? "inpin" : "outpin") 
			<< " \"" << instanceName << "\" " << instancePin.getPinName() << (comma ? "," : "") 
			<< std::endl;
	}

	void XdlExporter::write(const Pip& pip, bool comma) {
		indent();
		mStream << mIndentString 
			<< "pip " << pip.getTileName() << " " << pip.getSourceWireName() 
			<< " " << pip.getDirectionString() << " " << pip.getSinkWireName() 
			<< (comma ? "," : "");
		if(pip.isRoutethrough()) write(*pip.getRoutethroughPtr());
		mStream << std::endl;
	}

	void XdlExporter::write(const Routethrough& routethrough) {
		std::string instanceName = routethrough.getInstancePtr().expired() 
			? "[expired instance]" : routethrough.getInstancePtr().lock()->getName();
		mStream << " # " << routethrough.getSetting() << ":" << routethrough.getName() << ":" 
			<< routethrough.getValue() << " \"" << instanceName << "\" " 
			<< routethrough.getSourceWireName() << " -> " << routethrough.getSinkWireName();
	}

} // namespace physical
} // namespace torc
