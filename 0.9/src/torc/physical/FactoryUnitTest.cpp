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
/// \brief Source for the Factory unit test.

#include <boost/test/unit_test.hpp>
#include "torc/physical/Factory.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical)

/// \brief Unit test for the Factory class.
BOOST_AUTO_TEST_CASE(FactoryUnitTest) {
	// functions tested:
	//	static DesignSharedPtr newDesignPtr(const string& inName, const string& inDevice, 
	//		const string& inPackage, const string& inSpeedGrade, const string& inXdlVersion);
	DesignSharedPtr designPtr = Factory::newDesignPtr("design", "device", "package", "speed", 
		"version");
	BOOST_REQUIRE(designPtr.get() != 0);

	// functions tested:
	//	static ModuleSharedPtr newModulePtr(const string& inName, const string& inAnchor);
	ModuleSharedPtr modulePtr = Factory::newModulePtr("name", "anchor");
	BOOST_REQUIRE(modulePtr.get() != 0);

	// functions tested:
	//	static InstanceSharedPtr newInstancePtr(const string& inName, const string& inType, 
	//		const string& inTile, const string& inSite, EInstanceBonding inBonding, 
	//		InstanceReferenceSharedPtr inInstanceReferencePtr);
	InstanceSharedPtr instance1Ptr = Factory::newInstancePtr("name", "type", "tile", "site");
	BOOST_REQUIRE(instance1Ptr.get() != 0);

	// functions tested:
	//	static PortSharedPtr newPortPtr(const string& inName, InstanceSharedPtr inInstancePtr, 
	//		const string& inPinName);
	PortSharedPtr portPtr = Factory::newPortPtr("name", instance1Ptr, "pin");
	BOOST_REQUIRE(portPtr.get() != 0);

	// functions tested:
	//	static NetSharedPtr newNetPtr(const string& inName, ENetType inNetType = eNetTypeNormal);
	NetSharedPtr net1Ptr = Factory::newNetPtr("name");
	BOOST_REQUIRE(net1Ptr.get() != 0);

	// functions tested:
	//	static InstanceReferenceSharedPtr newInstanceReferencePtr(const string& inInstantiationName, 
	//		ModuleSharedPtr inModulePtr, InstanceSharedPtr inInstancePtr);
	InstanceReferenceSharedPtr instanceReferencePtr = Factory::newInstanceReferencePtr("name", 
		modulePtr, instance1Ptr);
	BOOST_REQUIRE(instanceReferencePtr.get() != 0);

	// functions tested:
	InstanceSharedPtr instance2Ptr = Factory::newInstancePtr("name", "type", "tile", "site", 
		eInstanceBondingUnknown, instanceReferencePtr);
	BOOST_REQUIRE(instance2Ptr.get() != 0);

	// functions tested:
	//	static torc::physical::InstancePin newInstancePinPtr(InstanceSharedPtr inInstancePtr, 
	//		const string& inPinName);
	InstancePinSharedPtr instancePinPtr = Factory::newInstancePinPtr(instance2Ptr, "pin");
	BOOST_REQUIRE(instancePinPtr->getInstancePtr().lock() == instance2Ptr);

	// functions tested:
	//	static RoutethroughSharedPtr newRoutethroughPtr(const string& inSetting, 
	//		const string& inName, const string& inValue, const InstanceWeakPtr& inInstancePtr, 
	//		const string& inSourceWireName, const string& inSinkWireName);
	RoutethroughSharedPtr routethroughPtr = Factory::newRoutethroughPtr("setting", "name", "value", 
		instance2Ptr, "source", "sink");
	BOOST_REQUIRE(routethroughPtr.get() != 0);

	// functions tested:
	//	static torc::physical::Pip newPip(const string& inTileName, const string& inSourceWireName, 
	//		const string& inSinkWireName, EPipDirection inPipDirection, 
	//		RoutethroughSharedPtr inRoutethroughPtr);
	Pip pip = Factory::newPip("tile", "source", "sink", ePipUnidirectionalBuffered, 
		routethroughPtr);
	BOOST_REQUIRE(pip.getRoutethroughPtr() == routethroughPtr);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
