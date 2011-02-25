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
/// \brief Source for the InstanceReference unit test.

#include <boost/test/unit_test.hpp>
#include "torc/physical/Factory.hpp"
#include "torc/physical/InstancePin.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical_database)

/// \brief Unit test for the InstanceReference class.
BOOST_AUTO_TEST_CASE(physical_instance_reference) {
	// create the accessory instance and module
	ModuleSharedPtr modulePtr = Factory::newModulePtr("name", "anchor");
	InstanceSharedPtr instancePtr = Factory::newInstancePtr("name", "type", "tile", "site");

	// functions tested:
	//		InstanceReference(const string& inInstantiationName, ModuleSharedPtr inModulePtr, 
	//			InstanceSharedPtr inInstancePtr);
	// create an instance reference and verify that we can recover what we put into it
	std::string name = "name";
	InstanceReferenceSharedPtr instanceReference1Ptr = Factory::newInstanceReferencePtr(name, 
		modulePtr, instancePtr);
	InstanceReferenceSharedPtr instanceReference2Ptr = Factory::newInstanceReferencePtr(name, 
		modulePtr, instancePtr);
	BOOST_REQUIRE(instanceReference1Ptr != 0);
	BOOST_REQUIRE(instanceReference2Ptr != 0);

	// functions tested:
	//		const ModuleWeakPtr& getModulePtr(void) const;
	//		const InstanceWeakPtr& getInstancePtr(void) const;
	//		const string& getInstantiationName(void) const;
	BOOST_CHECK(*(instanceReference1Ptr->getInstancePtr().lock()) == *instancePtr);
	BOOST_CHECK(*(instanceReference1Ptr->getModulePtr().lock()) == *modulePtr);
	BOOST_CHECK(instanceReference1Ptr->getInstantiationName() == name);

	// functions tested:
	//		bool operator ==(const InstanceReference& rhs) const;
	BOOST_CHECK(*instanceReference1Ptr == *instanceReference2Ptr);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
