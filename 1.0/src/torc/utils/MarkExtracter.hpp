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
/// \brief PhysicalDiff class for comparing physical netlists.

#ifndef TORC_UTILS_MARKEXTRACTER_HPP
#define TORC_UTILS_MARKEXTRACTER_HPP

#include <iostream>
#include "torc/Physical.hpp"
#include "torc/physical/OutputStreamHelpers.hpp"
#include <vector>
#include <boost/regex.hpp>

namespace torc {

/*namespace physical_diff {
	// forward declarations of unit test classes within their namespace.
	class diff_design;
	class diff_module;
	class diff_circuit;
	class diff_config_map;
	class diff_instance;
	class diff_net;
}*/


/// \brief Diff utility class for comparing physical netlists.
/// \details This class takes two design pointers and compares them.
class MarkExtracter {
protected:
	/// \brief Imported type name.
	typedef std::string string;
	/// \brief Imported type name.
	typedef torc::physical::DesignSharedPtr DesignSharedPtr;
	/// \brief Imported type name.
	typedef torc::physical::ModuleSharedPtr ModuleSharedPtr;
	/// \brief Imported type name.
	typedef std::vector<ModuleSharedPtr> ModuleSharedPtrVector;
	/// \brief Imported type name.
	typedef torc::physical::CircuitSharedPtr CircuitSharedPtr;
	/// \brief Imported type name.
	typedef torc::physical::InstanceSharedPtr InstanceSharedPtr;
	/// \brief Imported type name.
	typedef std::vector<InstanceSharedPtr> InstanceSharedPtrVector;
	/// \brief Imported type name.
	typedef torc::physical::NetSharedPtr NetSharedPtr;
	/// \brief Imported type name.
	typedef std::vector<NetSharedPtr> NetSharedPtrVector;
	/// \brief Imported type name.
	typedef torc::physical::Circuit::InstanceSharedPtrConstIterator InstanceSharedPtrConstIterator;
	/// \brief Imported type name.
	typedef torc::physical::Circuit::InstanceSharedPtrIterator InstanceSharedPtrIterator;
	/// \brief Imported type name.
	typedef torc::physical::Circuit::NetSharedPtrConstIterator NetSharedPtrConstIterator;
	/// \brief Imported type name.
	typedef torc::physical::Circuit::NetSharedPtrIterator NetSharedPtrIterator;
	/// \brief Imported type name.
	typedef torc::physical::InstanceWeakPtr InstanceWeakPtr;
	/// \brief Imported type name.
	typedef torc::physical::Design::ModuleSharedPtrConstIterator ModuleSharedPtrConstIterator;
	/// \brief Imported type name.
	typedef torc::physical::Design::ModuleSharedPtrIterator ModuleSharedPtrIterator;
	/// \brief Imported type name.
	typedef torc::physical::ConfigMap::const_iterator ConfigMapConstIterator;
	/// \brief Imported type name.
	typedef torc::physical::Net::InstancePinSharedPtrConstIterator 
		InstancePinSharedPtrConstIterator;
	/// \brief Imported type name.
	typedef torc::physical::Net::PipConstIterator PipConstIterator;
	/// \brief Imported type name.
	typedef boost::shared_ptr<torc::physical::Named> NamedSharedPtr;
	/// \brief Imported type name.
	typedef boost::shared_ptr<torc::physical::ConfigMap> ConfigMapSharedPtr;
	/// \brief Imported type name.
	typedef torc::physical::Module::PortSharedPtrConstIterator PortSharedPtrConstIterator;
	/// \brief Imported type name.
	typedef torc::physical::Net::InstancePinSharedPtrIterator InstancePinSharedPtrIterator;
	/// \brief Imported type name.
	typedef torc::physical::Pip Pip;
	typedef torc::physical::PipVector PipVector;
	typedef torc::physical::Net::PipIterator PipIterator;

	/// \brief Output stream to use for comparison results.
	std::ostream& mStream;
	DesignSharedPtr mDesign;
	DesignSharedPtr mNewDesign;
	const string mPattern;
	boost::regex mRegex;
	
public:
	MarkExtracter(std::ostream& inOutStream, const DesignSharedPtr& inDesign, 
		string inPattern) : mStream(inOutStream), mDesign(inDesign), mPattern(inPattern) {
		mStream << "Searching for '" << mPattern << "'" << std::endl;
		mStream << "NEAT" << std::endl;
		mRegex.assign(mPattern);
	}

	/// \brief Top level call to diff two designs.
	DesignSharedPtr extract() {

		std::cout << "### " << mPattern << " ###" << std::endl;

		ModuleSharedPtrConstIterator p;
		for (p = mDesign->modulesBegin(); p!= mDesign->modulesEnd(); p++) {
			std::cout << "MODULES ARE NOT CURRENTLY SUPPORTED!" << std::endl;
		}
		mNewDesign = physical::Factory::newDesignPtr(mDesign->getName(), mDesign->getDevice(),
			mDesign->getPackage(), mDesign->getSpeedGrade(), mDesign->getXdlVersion());
		
		pruneCircuit(mDesign);
		//filterOnPips(mDesign);
		
		return mNewDesign;
	}
	void pruneCircuit(CircuitSharedPtr circuit) {
		InstanceSharedPtrIterator inst_i;
		NetSharedPtrIterator net_i;
		
		std::map<InstanceSharedPtr, bool> instanceMap;
		std::map<NetSharedPtr, bool> netMap;
		
		boost::smatch smatches;
		
		for (inst_i = circuit->instancesBegin(); inst_i != circuit->instancesEnd(); inst_i++) {
			bool test = regex_match((*inst_i)->getName(), smatches, mRegex);
			std::multimap<std::string, physical::Config>::const_iterator p;
			for (p = (*inst_i)->configBegin(); p != (*inst_i)->configEnd(); p++) {
				test = test || regex_match((*p).first, smatches, mRegex);
				test = test || regex_match((*p).second.getName(), smatches, mRegex);
				test = test || regex_match((*p).second.getValue(), smatches, mRegex);
			}
			//std::cout << "INSTANCE: " << (*inst_i)->getName() << " -- " << test << std::endl;
			instanceMap[*inst_i] = test;
		}
		for (net_i = circuit->netsBegin(); net_i != circuit->netsEnd(); net_i++) {
			bool test = regex_match((*net_i)->getName(), smatches, mRegex);
			//std::cout << "NET: " << (*net_i)->getName() << " -- " << test << std::endl;
			netMap[*net_i] = test;
			if (!test) continue;
			InstancePinSharedPtrIterator pin_i;
			for (pin_i = (*net_i)->sourcesBegin(); pin_i != (*net_i)->sourcesEnd(); pin_i++) {
				std::cout << "NET INST source: " << ((*pin_i)->getInstancePtr().lock())->getName() 
					<< " -- " << true << std::endl;
				instanceMap[(*pin_i)->getInstancePtr().lock()] = true;
			}
			for (pin_i = (*net_i)->sinksBegin(); pin_i != (*net_i)->sinksEnd(); pin_i++) {
				std::cout << "NET INST sink: " << ((*pin_i)->getInstancePtr().lock())->getName() 
					<< " -- " << true << std::endl;
				instanceMap[(*pin_i)->getInstancePtr().lock()] = true;
			}
		}
		
		std::map<InstanceSharedPtr, bool>::iterator imap_i;
		std::map<NetSharedPtr, bool>::iterator nmap_i;
		std::cout << "Copying marked instances and nets..." << std::endl;
		for (imap_i = instanceMap.begin(); imap_i != instanceMap.end(); imap_i++) {
			//std::cout << "check INST: " << (*(imap_i->first)).getName() 
			//	<< " -- " << imap_i->second << std::endl;
			if (imap_i->second) { // this is inverted for removing!
				InstanceSharedPtr isp = imap_i->first;
				mNewDesign->addInstance(isp);
			}
		}
		for (nmap_i = netMap.begin(); nmap_i != netMap.end(); nmap_i++) {
			//std::cout << "check NET: " << (*(nmap_i->first)).getName() 
			//	<< " -- " << nmap_i->second << std::endl;
			if (nmap_i->second) {
				NetSharedPtr nsp = nmap_i->first;
				mNewDesign->addNet(nsp);
			}
		}

	}
	void filterOnPips(CircuitSharedPtr circuit) {
		InstanceSharedPtrIterator inst_i;
		NetSharedPtrIterator net_i;
		
		std::map<InstanceSharedPtr, bool> instanceMap;
		std::map<NetSharedPtr, bool> netMap;
		
		boost::smatch smatches;
		
		for (inst_i = circuit->instancesBegin(); inst_i != circuit->instancesEnd(); inst_i++) {
			instanceMap[*inst_i] = false;
		}
		for (net_i = circuit->netsBegin(); net_i != circuit->netsEnd(); net_i++) {
			bool test = false;
			for (PipIterator pip_i = (*net_i)->pipsBegin(); pip_i != (*net_i)->pipsEnd(); pip_i++) {
				test = test || regex_match(pip_i->getTileName(), smatches, mRegex);
				test = test || regex_match(pip_i->getSourceWireName(), smatches, mRegex);
				test = test || regex_match(pip_i->getSinkWireName(), smatches, mRegex);
			}
			//bool test = regex_match((*net_i)->getName(), smatches, mRegex);
			//std::cout << "NET: " << (*net_i)->getName() << " -- " << test << std::endl;
			netMap[*net_i] = test;
			if (!test) continue;
			InstancePinSharedPtrIterator pin_i;
			for (pin_i = (*net_i)->sourcesBegin(); pin_i != (*net_i)->sourcesEnd(); pin_i++) {
				std::cout << "NET INST source: " << ((*pin_i)->getInstancePtr().lock())->getName() 
					<< " -- " << true << std::endl;
				instanceMap[(*pin_i)->getInstancePtr().lock()] = true;
			}
			for (pin_i = (*net_i)->sinksBegin(); pin_i != (*net_i)->sinksEnd(); pin_i++) {
				std::cout << "NET INST sink: " << ((*pin_i)->getInstancePtr().lock())->getName() 
					<< " -- " << true << std::endl;
				instanceMap[(*pin_i)->getInstancePtr().lock()] = true;
			}
		}
		
		std::map<InstanceSharedPtr, bool>::iterator imap_i;
		std::map<NetSharedPtr, bool>::iterator nmap_i;
		std::cout << "Copying marked instances and nets..." << std::endl;
		for (imap_i = instanceMap.begin(); imap_i != instanceMap.end(); imap_i++) {
			//std::cout << "check INST: " << (*(imap_i->first)).getName() 
			//	<< " -- " << imap_i->second << std::endl;
			if (imap_i->second) { // this is inverted for removing!
				InstanceSharedPtr isp = imap_i->first;
				mNewDesign->addInstance(isp);
			}
		}
		for (nmap_i = netMap.begin(); nmap_i != netMap.end(); nmap_i++) {
			//std::cout << "check NET: " << (*(nmap_i->first)).getName() 
			//	<< " -- " << nmap_i->second << std::endl;
			if (nmap_i->second) {
				NetSharedPtr nsp = nmap_i->first;
				mNewDesign->addNet(nsp);
			}
		}

	}

};

} // namespace torc

#endif // TORC_UTILS_MARKEXTRACTER_HPP
