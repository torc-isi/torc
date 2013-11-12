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

#ifndef TORC_UTILS_PHYSICALDIFF_HPP
#define TORC_UTILS_PHYSICALDIFF_HPP

#include <iostream>
#include "torc/Physical.hpp"
#include "torc/physical/OutputStreamHelpers.hpp"
#include <vector>

namespace torc {

namespace utils {
	// forward declarations of unit test classes within their namespace.
	class PhysicalDiffDesignUnitTest;
	class PhysicalDiffModuleUnitTest;
	class PhysicalDiffCircuitUnitTest;
	class PhysicalDiffConfigMapUnitTest;
	class PhysicalDiffInstanceUnitTest;
	class PhysicalDiffNetUnitTest;
}


/// \brief Diff utility class for comparing physical netlists.
/// \details This class takes two design pointers and compares them.
class PhysicalDiff {
	/// \brief Unit test access to internal functions.
	friend class utils::PhysicalDiffDesignUnitTest;
	/// \brief Unit test access to internal functions.
	friend class utils::PhysicalDiffModuleUnitTest;
	/// \brief Unit test access to internal functions.
	friend class utils::PhysicalDiffCircuitUnitTest;
	/// \brief Unit test access to internal functions.
	friend class utils::PhysicalDiffConfigMapUnitTest;
	/// \brief Unit test access to internal functions.
	friend class utils::PhysicalDiffInstanceUnitTest;
	/// \brief Unit test access to internal functions.
	friend class utils::PhysicalDiffNetUnitTest;
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

	/// \brief Output stream to use for comparison results.
	std::ostream& mStream;
	
public:
	/// \todo Improve Pip comparison.
	/// \todo Port comparison should use port name.
	/// \todo Config sequence index is not used.
	/// \todo Instance does not check the reference pointer.
	PhysicalDiff(std::ostream& inOutStream) : mStream(inOutStream)
	{
		mStream << "WARNING: Pip comparison uses only tile, source, direction "
			<< "and sink!" << std::endl;
		mStream << "WARNING: Port comparison does not use the port name!" << std::endl;
		mStream << "WARNING: Config sequence index not used!" << std::endl;
		mStream << "WARNING: Instance does not check reference pointer!" << std::endl;
	}

	/// \brief Top level call to diff two designs.
	bool diff(const DesignSharedPtr& left, const DesignSharedPtr& right) {
		return diffDesign(left, right);
	}
protected:
	/// \brief Diff design pointers and then recursively compare modules and inherited members.
	bool diffDesign(const DesignSharedPtr& left, const DesignSharedPtr& right) {
		bool rval = true;
		string designNameLeft = left->getName();
		string designNameRight = right->getName();

		if (!(designNameLeft == designNameRight)) {
			mStream << "! Design Name: " << designNameLeft << " | " 
				<< designNameRight << std::endl;
			rval = false;
		}
		if (!(left->getDevice() == right->getDevice())) {
			mStream << "! Device: " << left->getDevice() << " | " 
				<< right->getDevice() << std::endl;
			rval = false;
		}
		if (!(left->getPackage() == right->getPackage())) {
			mStream << "! Package: " << left->getPackage() << " | "
				<< right->getPackage() << std::endl;
			rval = false;
		}
		if (!(left->getSpeedGrade() == right->getSpeedGrade())) {
			mStream << "! Speed grade: " << left->getSpeedGrade() 
				<< " | " << right->getSpeedGrade() << std::endl;
			rval = false;
		}
		if (!(left->getXdlVersion() == right->getXdlVersion())) {
			mStream << "! XDL version differs: " << left->getXdlVersion() 
				<< " | " << right->getXdlVersion() << std::endl;
			rval = false;
		}
		
		ModuleSharedPtrVector leftModules;
		ModuleSharedPtrVector rightModules;
		
		for (ModuleSharedPtrConstIterator p = left->modulesBegin(); p!= left->modulesEnd(); p++) {
			leftModules.push_back(*p);
		}
		for (ModuleSharedPtrConstIterator p = right->modulesBegin(); p!= right->modulesEnd(); p++) {
			rightModules.push_back(*p);
		}
		
		// look left to right
		ModuleSharedPtrIterator ileft;
		ModuleSharedPtrIterator iright;
		for (ileft = leftModules.begin(); ileft != leftModules.end(); ileft++) {
			bool found = false;
			for (iright = rightModules.begin(); iright != rightModules.end(); iright++) {
				if ((*ileft)->getName() == (*iright)->getName()) {
					found = true;
					break;
				}
			}
			if (found) {
				rval = diffModule(*ileft, *iright) && rval;
				rightModules.erase(iright);
			} else {
				mStream << "< Module " << (*ileft)->getName() << std::endl;
				rval = false;
			}
		}
		// anything left on the right doesn't appear on the left
		for (iright = rightModules.begin(); iright != rightModules.end(); iright++) {
			mStream << "> Module " << (*iright)->getName() << std::endl;
			rval = false;
		}
		// A design is a circuit
		rval = diffCircuit(left, right) && rval;
		return rval;
	}
	/// \brief Diff module pointers and then recursively compare inherited members from circuit.
	bool diffModule(const ModuleSharedPtr& left, const ModuleSharedPtr& right) {
		bool rval = true;
		
		if (!(left->getAnchor() == right->getAnchor())) {
			mStream << "! Module " << left->getName() << " anchor: " << left->getAnchor()
				<< " | " << right->getAnchor() << std::endl;
			rval = false;
		}
		
		std::vector<string> leftPorts;
		std::vector<string> rightPorts;
		std::vector<string>::iterator leftport;
		std::vector<string>::iterator rightport;
		for (PortSharedPtrConstIterator leftport = left->portsBegin(); 
			leftport != left->portsEnd(); leftport++) {
			string t = (*leftport)->getInstancePtr().lock()->getName();
			t.append(".");
			t.append((*leftport)->getPinName());
			leftPorts.push_back(t);
		}
		for (PortSharedPtrConstIterator rightport = right->portsBegin(); 
			rightport != right->portsEnd(); rightport++) {
			string t = (*rightport)->getInstancePtr().lock()->getName();
			t.append(".");
			t.append((*rightport)->getPinName());
			rightPorts.push_back(t);
		}
		for (leftport = leftPorts.begin(); leftport != leftPorts.end(); leftport++) {
			bool found = false;
			for (rightport = rightPorts.begin(); rightport != rightPorts.end(); rightport++) {
				if (*leftport == *rightport) {
					found = true;
					break;
				}
			}
			if (found) {
				rightPorts.erase(rightport);
			} else {
				mStream << "< Module " << left->getName() << " port: "
					<< *leftport << std::endl;
				rval = false;
			}
		}
		for (rightport = rightPorts.begin(); rightport != rightPorts.end(); rightport++) {
			mStream << "> Module " << right->getName() << " port: "
				<< *rightport << std::endl;
			rval = false;
		}
		leftPorts.clear();
		rightPorts.clear();

		// A module is a circuit
		rval = diffCircuit(left, right) && rval;
		return rval;
	}
	/// \brief Diff circuit pointers, recursively compare nets, instances and config map.
	bool diffCircuit(const CircuitSharedPtr& left, const CircuitSharedPtr& right) {
		bool rval = true;
		
		InstanceSharedPtrVector leftInstances;
		InstanceSharedPtrVector rightInstances;
		InstanceSharedPtrIterator ileft;
		InstanceSharedPtrIterator iright;
		
		for (ileft = left->instancesBegin(); ileft!= left->instancesEnd(); ileft++) {
			leftInstances.push_back(*ileft);
		}
		for (iright = right->instancesBegin(); iright!= right->instancesEnd(); iright++) {
			rightInstances.push_back(*iright);
		}
		for (ileft = leftInstances.begin(); ileft != leftInstances.end(); ileft++) {
			bool found = false;
			for (iright = rightInstances.begin(); iright != rightInstances.end(); iright++) {
				if ((*ileft)->getName() == (*iright)->getName()) {
					found = true;
					break;
				}
			}
			if (found) {
				//mStream << "Instance " << (*ileft)->getName() << std::endl;
				rval = diffInstance(*ileft, *iright) && rval;
				rightInstances.erase(iright);
			} else {
				mStream << "< Instance " << (*ileft)->getName() << std::endl;
				rval = false;
			}
		}
		for (iright = rightInstances.begin(); iright != rightInstances.end(); iright++) {
			mStream << "> Instance " << (*iright)->getName() << std::endl;
			rval = false;
		}
		leftInstances.clear();
		rightInstances.clear();
		
		NetSharedPtrVector leftNets;
		NetSharedPtrVector rightNets;
		NetSharedPtrIterator nleft;
		NetSharedPtrIterator nright;
		
		for (nleft = left->netsBegin(); nleft!= left->netsEnd(); nleft++) {
			leftNets.push_back(*nleft);
		}
		for (nright = right->netsBegin(); nright!= right->netsEnd(); nright++) {
			rightNets.push_back(*nright);
		}
		for (nleft = leftNets.begin(); nleft != leftNets.end(); nleft++) {
			bool found = false;
			for (nright = rightNets.begin(); nright != rightNets.end(); nright++) {
				if ((*nleft)->getName() == (*nright)->getName()) {
					found = true;
					break;
				}
			}
			if (found) {
				rval = diffNet(*nleft, *nright) && rval;
				rightNets.erase(nright);
			} else {
				mStream << "< Net " << (*nleft)->getName() << std::endl;
				rval = false;
			}
		}
		for (nright = rightNets.begin(); nright != rightNets.end(); nright++) {
			mStream << "> Net " << (*nright)->getName() << std::endl;
			rval = false;
		}
		leftNets.clear();
		rightNets.clear();
		
		string circuitname = "Circuit ";
		circuitname.append(left->getName());
		// A circuit is a configmap.
		rval = diffConfigMap(left, right, circuitname) && rval;
		return rval;
	}
	/// \brief Diff configuration information, this is called from circuits, instances and nets.
	bool diffConfigMap(const ConfigMapSharedPtr& left, const ConfigMapSharedPtr& right, 
		const string& parentStr) {
		bool rval = true;
		
		std::vector<string> leftConfigs;
		std::vector<string> rightConfigs;
		std::vector<string>::iterator ileft;
		std::vector<string>::iterator iright;
		
		torc::physical::ConfigMap::const_iterator p = left->configBegin();
		torc::physical::ConfigMap::const_iterator e = left->configEnd();
		while (p != e) {
			const torc::physical::Config& config = p->second;
			string t = p->first;
			t.append(":");
			t.append(config.getName());
			t.append(":");
			t.append(config.getValue());
			leftConfigs.push_back(t);
			p++;
		}
		p = right->configBegin();
		e = right->configEnd();
		while (p != e) {
			const torc::physical::Config& config = p->second;
			string t = p->first;
			t.append(":");
			t.append(config.getName());
			t.append(":");
			t.append(config.getValue());
			rightConfigs.push_back(t);
			p++;
		}
		for (ileft = leftConfigs.begin(); ileft != leftConfigs.end(); ileft++) {
			bool found = false;
			for (iright = rightConfigs.begin(); iright != rightConfigs.end(); iright++) {
				if (*ileft == *iright) {
					found = true;
					break;
				}
			}
			if (found) {
				//mStream << "Net " << (*nleft)->getName() << std::endl;
				rightConfigs.erase(iright);
			} else {
				mStream << "< " << parentStr << " Config " << *ileft << std::endl;
				rval = false;
			}
		}
		for (iright = rightConfigs.begin(); iright != rightConfigs.end(); iright++) {
			mStream << "> " << parentStr << " Config " << *iright << std::endl;
			rval = false;
		}
		leftConfigs.clear();
		rightConfigs.clear();
		return rval;
	}
	/// \brief Diff instance pointers and then the underlying config map
	bool diffInstance(const InstanceSharedPtr& left, const InstanceSharedPtr& right) {
		bool rval = true;
		if (!(left->getType() == right->getType())) {
			mStream << "! Instance " << left->getName() << " type: " << left->getType() 
				<< " | " << right->getType() << std::endl;
			rval = false;
		}
		if (!(left->getTile() == right->getTile())) {
			mStream << "! Instance " << left->getName() << " tile: " << left->getTile() 
				<< " | " << right->getTile() << std::endl;;
			rval = false;
		}
		if (!(left->getSite() == right->getSite())) {
			mStream << "! Instance " << left->getName() << "site: " << left->getSite() 
				<< " | " << right->getSite() << std::endl;
			rval = false;
		}
		if (!(left->getBonding() == right->getBonding())) {
			mStream << "! Instance " << left->getName() << " bonding: " << left->getBonding() 
				<< " | " << right->getBonding() << std::endl;
			rval = false;
		}

		string instancename = "Instance ";
		instancename.append(left->getName());
		// An instance is a configmap.
		rval = diffConfigMap(left, right, instancename) && rval;
		return rval;
	}
	/// \brief Diff net pointers and then the underlying config map.
	bool diffNet(const NetSharedPtr& left, const NetSharedPtr& right) {
		bool rval = true;
		if (!(left->getNetType() == right->getNetType())) {
			mStream << "! Net " << left->getName() << " type: " << left->getNetType()
				<< " | " << right->getNetType() << std::endl;
			rval = false;
		}
		
		std::vector<string> leftPins;
		std::vector<string> rightPins;
		std::vector<string>::iterator leftpin;
		std::vector<string>::iterator rightpin;
		
		for (InstancePinSharedPtrConstIterator leftpinPtr = left->sourcesBegin(); 
			leftpinPtr != left->sourcesEnd(); leftpinPtr++) {
			string t = (*leftpinPtr)->getInstancePtr().lock()->getName();
			t.append(".");
			t.append((*leftpinPtr)->getPinName());
			leftPins.push_back(t);
		}
		for (InstancePinSharedPtrConstIterator rightpinPtr = right->sourcesBegin(); 
			rightpinPtr != right->sourcesEnd(); rightpinPtr++) {
			string t = (*rightpinPtr)->getInstancePtr().lock()->getName();
			t.append(".");
			t.append((*rightpinPtr)->getPinName());
			rightPins.push_back(t);
		}
		for (leftpin = leftPins.begin(); leftpin != leftPins.end(); leftpin++) {
			bool found = false;
			for (rightpin = rightPins.begin(); rightpin != rightPins.end(); rightpin++) {
				if (*leftpin == *rightpin) {
					found = true;
					break;
				}
			}
			if (found) {
				rightPins.erase(rightpin);
			} else {
				mStream << "< Net " << left->getName() << " outpin: "
					<< *leftpin << std::endl;
				rval = false;
			}
		}
		for (rightpin = rightPins.begin(); rightpin != rightPins.end(); rightpin++) {
			mStream << "> Net " << right->getName() << " outpin: "
				<< *rightpin << std::endl;
			rval = false;
		}
		leftPins.clear();
		rightPins.clear();
		
		for (InstancePinSharedPtrConstIterator leftpinPtr = left->sinksBegin(); 
			leftpinPtr != left->sinksEnd(); leftpinPtr++) {
			string t = (*leftpinPtr)->getInstancePtr().lock()->getName();
			t.append(".");
			t.append((*leftpinPtr)->getPinName());
			leftPins.push_back(t);
		}
		for (InstancePinSharedPtrConstIterator rightpinPtr = right->sinksBegin(); 
			rightpinPtr != right->sinksEnd(); rightpinPtr++) {
			string t = (*rightpinPtr)->getInstancePtr().lock()->getName();
			t.append(".");
			t.append((*rightpinPtr)->getPinName());
			rightPins.push_back(t);
		}
		for (leftpin = leftPins.begin(); leftpin != leftPins.end(); leftpin++) {
			bool found = false;
			for (rightpin = rightPins.begin(); rightpin != rightPins.end(); rightpin++) {
				if (*leftpin == *rightpin) {
					found = true;
					break;
				}
			}
			if (found) {
				rightPins.erase(rightpin);
			} else {
				mStream << "< Net " << left->getName() << " inpin: "
					<< *leftpin << std::endl;
				rval = false;
			}
		}
		for (rightpin = rightPins.begin(); rightpin != rightPins.end(); rightpin++) {
			mStream << "> Net " << right->getName() << " inpin: "
				<< *rightpin << std::endl;
			rval = false;
		}
		leftPins.clear();
		rightPins.clear();
		
		std::vector<string> leftPips;
		std::vector<string> rightPips;
		std::vector<string>::iterator leftpip;
		std::vector<string>::iterator rightpip;
		for (PipConstIterator leftpip = left->pipsBegin(); 
			leftpip != left->pipsEnd(); leftpip++) {
			string t = leftpip->getTileName();
			t.append(" ");
			t.append(leftpip->getSourceWireName());
			t.append(" ");
			t.append(leftpip->getDirectionString());
			t.append(" ");
			t.append(leftpip->getSinkWireName());
			leftPips.push_back(t);
		}
		for (PipConstIterator rightpip = right->pipsBegin(); 
			rightpip != right->pipsEnd(); rightpip++) {
			string t = rightpip->getTileName();
			t.append(" ");
			t.append(rightpip->getSourceWireName());
			t.append(" ");
			t.append(rightpip->getDirectionString());
			t.append(" ");
			t.append(rightpip->getSinkWireName());
			rightPips.push_back(t);
		}
		for (leftpip = leftPips.begin(); leftpip != leftPips.end(); leftpip++) {
			bool found = false;
			for (rightpip = rightPips.begin(); rightpip != rightPips.end(); rightpip++) {
				if (*leftpip == *rightpip) {
					found = true;
					break;
				}
			}
			if (found) {
				rightPips.erase(rightpip);
			} else {
				mStream << "< Net " << left->getName() << " pip: "
					<< *leftpip << std::endl;
				rval = false;
			}
		}
		for (rightpip = rightPips.begin(); rightpip != rightPips.end(); rightpip++) {
			mStream << "> Net " << right->getName() << " pip: "
				<< *rightpip << std::endl;
			rval = false;
		}
		leftPips.clear();
		rightPips.clear();
		
		string netname = "Net ";
		netname.append(left->getName());
		// A net is a configmap.
		rval = diffConfigMap(left, right, netname) && rval;
		return rval;
	}

};

} // namespace torc

#endif // TORC_UTILS_PHYSICALDIFF_HPP
