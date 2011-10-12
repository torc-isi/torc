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
/// \brief Source for the PhysicalDiff unit test.

#include <sstream>
#include <boost/test/unit_test.hpp>
#include "torc/utils/PhysicalDiff.hpp"

namespace torc {

BOOST_AUTO_TEST_SUITE(utils)

/// \brief Unit test for the design diff function.
BOOST_AUTO_TEST_CASE(PhysicalDiffDesignUnitTest) {
	std::ostringstream nullstream(std::ostringstream::out);
	PhysicalDiff pd(nullstream);
	// functions tested:
	//		bool diffDesign(const DesignSharedPtr& left, const DesignSharedPtr& right);

	std::string designName = "design";
	std::string designName2 = "design2"; // not comparing differing names for the moment
	std::string deviceName = "device";
	std::string deviceName2 = "device2";
	std::string devicePackage = "package";
	std::string devicePackage2 = "package2";
	std::string deviceSpeedGrade = "speed_grade";
	std::string deviceSpeedGrade2 = "speed_grade2";
	std::string xdlVersion = "xdl_version";
	std::string xdlVersion2 = "xdl_version2";

	physical::DesignSharedPtr designPtrLeft = physical::Factory::newDesignPtr(designName, 
		deviceName, devicePackage, deviceSpeedGrade, xdlVersion);
	BOOST_REQUIRE(designPtrLeft.get() != 0);
	physical::DesignSharedPtr designPtrRight = physical::Factory::newDesignPtr(designName, 
		deviceName, devicePackage, deviceSpeedGrade, xdlVersion);
	BOOST_REQUIRE(designPtrRight.get() != 0);
	
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == true);
	designPtrRight->setDevice(deviceName2);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == false);
	designPtrRight->setDevice(deviceName);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == true);
	
	designPtrLeft->setPackage(devicePackage2);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == false);
	designPtrLeft->setPackage(devicePackage);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == true);
	
	designPtrLeft->setSpeedGrade(deviceSpeedGrade2);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == false);
	designPtrLeft->setSpeedGrade(deviceSpeedGrade);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == true);
	
	designPtrRight->setXdlVersion(xdlVersion2);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == false);
	designPtrRight->setXdlVersion(xdlVersion);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == true);
	
	physical::ModuleSharedPtr module1PtrLeft
		= physical::Factory::newModulePtr("module1", "anchor1");
	physical::ModuleSharedPtr module2PtrLeft
		= physical::Factory::newModulePtr("module2", "anchor2");
	physical::ModuleSharedPtr module1PtrRight
		= physical::Factory::newModulePtr("module1", "anchor1");
	physical::ModuleSharedPtr module2PtrRight
		= physical::Factory::newModulePtr("module2", "anchor2");
	
	BOOST_REQUIRE(designPtrLeft->addModule(module1PtrLeft) == true);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == false);
	BOOST_REQUIRE(designPtrRight->addModule(module1PtrRight) == true);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == true);
	
	BOOST_REQUIRE(designPtrRight->addModule(module2PtrRight) == true);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == false);
	BOOST_REQUIRE(designPtrLeft->addModule(module2PtrLeft) == true);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == true);

}

/// \brief Unit test for the module diff function.
BOOST_AUTO_TEST_CASE(PhysicalDiffModuleUnitTest) {
	std::ostringstream nullstream(std::ostringstream::out);
	PhysicalDiff pd(nullstream);
	// functions tested:
	//		bool diffModule(const ModuleSharedPtr& left, const ModuleSharedPtr& right);
	
	std::string anchor = "anchor";
	std::string anchor2 = "anchor2"; // not comparing differing names for the moment
	std::string portname = "port";
	std::string portname2 = "port2";
	std::string pinname = "pinname";
	std::string pinname2 = "pinname2";
	
	physical::InstanceSharedPtr instancePtrLeft
		= physical::Factory::newInstancePtr("instname", "type", "tile", "site",
			physical::eInstanceBondingUnknown);
	physical::InstanceSharedPtr instancePtrRight
		= physical::Factory::newInstancePtr("instname", "type", "tile", "site",
			physical::eInstanceBondingUnknown);
	
	physical::ModuleSharedPtr modulePtrLeft
		= physical::Factory::newModulePtr("module1", anchor);
	physical::ModuleSharedPtr modulePtrRight
		= physical::Factory::newModulePtr("module1", anchor);
	physical::PortSharedPtr portLeft
		= physical::Factory::newPortPtr(portname, instancePtrLeft, pinname);
	physical::PortSharedPtr portLeft2
		= physical::Factory::newPortPtr(portname, instancePtrLeft, pinname2);
	physical::PortSharedPtr portRight
		= physical::Factory::newPortPtr(portname, instancePtrRight, pinname);
	physical::PortSharedPtr portRight2
		= physical::Factory::newPortPtr(portname, instancePtrRight, pinname2);
	
	BOOST_REQUIRE(pd.diffModule(modulePtrLeft, modulePtrRight) == true);
	modulePtrRight->setAnchor(anchor2);
	BOOST_REQUIRE(pd.diffModule(modulePtrLeft, modulePtrRight) == false);
	modulePtrRight->setAnchor(anchor);
	BOOST_REQUIRE(pd.diffModule(modulePtrLeft, modulePtrRight) == true);
	
	modulePtrLeft->addPort(portLeft);
	BOOST_REQUIRE(pd.diffModule(modulePtrLeft, modulePtrRight) == false);
	modulePtrRight->addPort(portRight);
	BOOST_REQUIRE(pd.diffModule(modulePtrLeft, modulePtrRight) == true);
	
	modulePtrRight->removePort(portRight);
	modulePtrRight->addPort(portRight2);
	BOOST_REQUIRE(pd.diffModule(modulePtrLeft, modulePtrRight) == false);
	modulePtrLeft->removePort(portLeft);
	modulePtrLeft->addPort(portLeft2);
	BOOST_REQUIRE(pd.diffModule(modulePtrLeft, modulePtrRight) == true);
	
}

/// \brief Unit test for the circuit diff function.
BOOST_AUTO_TEST_CASE(PhysicalDiffCircuitUnitTest) {
	std::ostringstream nullstream(std::ostringstream::out);
	PhysicalDiff pd(nullstream);
	// functions tested:
	// 		bool diffCircuit(const CircuitSharedPtr& left, const CircuitSharedPtr& right);
	
	std::string designName = "design";
	std::string deviceName = "device";
	std::string devicePackage = "package";
	std::string deviceSpeedGrade = "speed_grade";
	std::string xdlVersion = "xdl_version";

	physical::DesignSharedPtr designPtrLeft = physical::Factory::newDesignPtr(designName, 
		deviceName, devicePackage, deviceSpeedGrade, xdlVersion);
	BOOST_REQUIRE(designPtrLeft.get() != 0);
	physical::DesignSharedPtr designPtrRight = physical::Factory::newDesignPtr(designName, 
		deviceName, devicePackage, deviceSpeedGrade, xdlVersion);
	BOOST_REQUIRE(designPtrRight.get() != 0);
	
	physical::InstanceSharedPtr inst1Left
		= physical::Factory::newInstancePtr("name1", "type1", "tile1", "site1");
	physical::InstanceSharedPtr inst2Left
		= physical::Factory::newInstancePtr("name2", "type2", "tile2", "site2");
	physical::InstanceSharedPtr inst1Right
		= physical::Factory::newInstancePtr("name1", "type1", "tile1", "site1");
	physical::InstanceSharedPtr inst2Right
		= physical::Factory::newInstancePtr("name2", "type2", "tile2", "site2");
		
	physical::NetSharedPtr net1Left
		= physical::Factory::newNetPtr("name1", physical::eNetTypeNormal);
	physical::NetSharedPtr net2Left
		= physical::Factory::newNetPtr("name2", physical::eNetTypeNormal);
	physical::NetSharedPtr net1Right
		= physical::Factory::newNetPtr("name1", physical::eNetTypeNormal);
	physical::NetSharedPtr net2Right
		= physical::Factory::newNetPtr("name2", physical::eNetTypeNormal);
	
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == true);
	
	designPtrLeft->addInstance(inst1Left);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == false);
	designPtrRight->addInstance(inst1Right);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == true);
	designPtrRight->addInstance(inst2Right);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == false);
	designPtrLeft->addInstance(inst2Left);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == true);
	
	designPtrRight->addNet(net1Right);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == false);
	designPtrLeft->addNet(net1Left);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == true);
	designPtrLeft->addNet(net2Left);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == false);
	designPtrRight->addNet(net2Right);
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == true);
	
}

/// \brief Unit test for the config_map diff function.
BOOST_AUTO_TEST_CASE(PhysicalDiffConfigMapUnitTest) {
	std::ostringstream nullstream(std::ostringstream::out);
	PhysicalDiff pd(nullstream);
	// functions tested:
	// 		bool diffConfigMap(const ConfigMapSharedPtr& left, const ConfigMapSharedPtr& right, 
	//			const string& parentStr);
	
	std::string designName = "design";
	std::string deviceName = "device";
	std::string devicePackage = "package";
	std::string deviceSpeedGrade = "speed_grade";
	std::string xdlVersion = "xdl_version";
	
	std::string setting1 = "setting1";
	std::string setting2 = "setting2";
	
	physical::DesignSharedPtr designPtrLeft = physical::Factory::newDesignPtr(designName, 
		deviceName, devicePackage, deviceSpeedGrade, xdlVersion);
	BOOST_REQUIRE(designPtrLeft.get() != 0);
	physical::DesignSharedPtr designPtrRight = physical::Factory::newDesignPtr(designName, 
		deviceName, devicePackage, deviceSpeedGrade, xdlVersion);
	BOOST_REQUIRE(designPtrRight.get() != 0);
	
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == true);
	
	designPtrLeft->setConfig("setting1", "name1", "value1");
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == false);
	designPtrRight->setConfig("setting1", "name1", "value1");
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == true);
	designPtrRight->setConfig("setting2", "name2", "value2");
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == false);
	designPtrLeft->setConfig("setting2", "name2", "value3");
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == false);
	designPtrLeft->setConfig("setting2", "name2", "value2");
	BOOST_REQUIRE(pd.diffDesign(designPtrLeft, designPtrRight) == true);
	
}

/// \brief Unit test for the instance diff function.
BOOST_AUTO_TEST_CASE(PhysicalDiffInstanceUnitTest) {
	std::ostringstream nullstream(std::ostringstream::out);
	PhysicalDiff pd(nullstream);
	// functions tested:
	// 		bool diffInstance(const InstanceSharedPtr& left, const InstanceSharedPtr& right);
	
	std::string name = "name";
	std::string type = "type";
	std::string type2 = "type2";
	std::string tile = "tile";
	std::string tile2 = "tile2";
	std::string site = "site";
	std::string site2 = "site2";
	
	physical::InstanceSharedPtr instancePtrLeft
		= physical::Factory::newInstancePtr(name, type, tile, site);
	BOOST_REQUIRE(instancePtrLeft.get() != 0);
	physical::InstanceSharedPtr instancePtrRight
		= physical::Factory::newInstancePtr(name, type, tile, site);
	BOOST_REQUIRE(instancePtrRight.get() != 0);
	
	BOOST_REQUIRE(pd.diffInstance(instancePtrLeft, instancePtrRight) == true);
	
	instancePtrLeft->setType(type2);
	BOOST_REQUIRE(pd.diffInstance(instancePtrLeft, instancePtrRight) == false);
	instancePtrRight->setType(type2);
	BOOST_REQUIRE(pd.diffInstance(instancePtrLeft, instancePtrRight) == true);
	
	instancePtrRight->setTile(tile2);
	BOOST_REQUIRE(pd.diffInstance(instancePtrLeft, instancePtrRight) == false);
	instancePtrLeft->setTile(tile2);
	BOOST_REQUIRE(pd.diffInstance(instancePtrLeft, instancePtrRight) == true);
	
	instancePtrLeft->setSite(site2);
	BOOST_REQUIRE(pd.diffInstance(instancePtrLeft, instancePtrRight) == false);
	instancePtrRight->setSite(site2);
	BOOST_REQUIRE(pd.diffInstance(instancePtrLeft, instancePtrRight) == true);
	
	instancePtrLeft->setBonding(physical::eInstanceBondingBonded);
	BOOST_REQUIRE(pd.diffInstance(instancePtrLeft, instancePtrRight) == false);
	instancePtrRight->setBonding(physical::eInstanceBondingBonded);
	BOOST_REQUIRE(pd.diffInstance(instancePtrLeft, instancePtrRight) == true);
	
}

/// \brief Unit test for the net diff function.
BOOST_AUTO_TEST_CASE(PhysicalDiffNetUnitTest) {
	std::ostringstream nullstream(std::ostringstream::out);
	PhysicalDiff pd(nullstream);
	// functions tested:
	// 		bool diffNet(const NetSharedPtr& left, const NetSharedPtr& right);
	
	physical::InstanceSharedPtr instPtrLeft
		= physical::Factory::newInstancePtr("inst1", "itype", "tile", "site");
	physical::InstanceSharedPtr instPtrRight
		= physical::Factory::newInstancePtr("inst1", "itype", "tile", "site");
	physical::InstancePinSharedPtr instPin1LeftPtr 
		= physical::Factory::newInstancePinPtr(instPtrLeft, "pin1");
	physical::InstancePinSharedPtr instPin2LeftPtr 
		= physical::Factory::newInstancePinPtr(instPtrLeft, "pin2");
	physical::InstancePinSharedPtr instPin1RightPtr 
		= physical::Factory::newInstancePinPtr(instPtrRight, "pin1");
	physical::InstancePinSharedPtr instPin2RightPtr 
		= physical::Factory::newInstancePinPtr(instPtrRight, "pin2");
	physical::Pip pip1Left = physical::Factory::newPip(
		"tile1", "source1", "sink1", physical::ePipUnidirectionalBuffered);
	physical::Pip pip1Right = physical::Factory::newPip(
		"tile1", "source1", "sink1", physical::ePipUnidirectionalBuffered);
	
	std::string name = "name";
	physical::ENetType type = physical::eNetTypePower;

	physical::NetSharedPtr netPtrLeft = physical::Factory::newNetPtr(name, type);
	physical::NetSharedPtr netPtrRight = physical::Factory::newNetPtr(name, type);
	
	BOOST_REQUIRE(pd.diffNet(netPtrLeft, netPtrRight) == true);
	
	netPtrRight->setNetType(physical::eNetTypeNormal);
	BOOST_REQUIRE(pd.diffNet(netPtrLeft, netPtrRight) == false);
	netPtrLeft->setNetType(physical::eNetTypeNormal);
	BOOST_REQUIRE(pd.diffNet(netPtrLeft, netPtrRight) == true);
	
	netPtrLeft->addSource(instPin1LeftPtr);
	BOOST_REQUIRE(pd.diffNet(netPtrLeft, netPtrRight) == false);
	netPtrRight->addSource(instPin1RightPtr);
	BOOST_REQUIRE(pd.diffNet(netPtrLeft, netPtrRight) == true);
	
	netPtrLeft->addSink(instPin2LeftPtr);
	BOOST_REQUIRE(pd.diffNet(netPtrLeft, netPtrRight) == false);
	netPtrRight->addSink(instPin2RightPtr);
	BOOST_REQUIRE(pd.diffNet(netPtrLeft, netPtrRight) == true);
	
	netPtrRight->addPip(pip1Right);
	BOOST_REQUIRE(pd.diffNet(netPtrLeft, netPtrRight) == false);
	netPtrLeft->addPip(pip1Left);
	BOOST_REQUIRE(pd.diffNet(netPtrLeft, netPtrRight) == true);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace torc
