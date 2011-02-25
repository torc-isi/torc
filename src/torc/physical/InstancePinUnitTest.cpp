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
/// \brief Source for the InstancePin unit test.

#include <boost/test/unit_test.hpp>
#include "torc/physical/Factory.hpp"
#include "torc/physical/InstancePin.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical_database)

/// \brief Unit test for the InstancePin class.
BOOST_AUTO_TEST_CASE(physical_instance_pin) {
	// create an accessory instance
	InstanceSharedPtr instance1Ptr = Factory::newInstancePtr("name1", "type", "tile", "site");
	InstanceSharedPtr instance2Ptr = Factory::newInstancePtr("name2", "type", "tile", "site");

	// functions tested:
	//		InstancePin(InstanceSharedPtr inInstancePtr, const string& inPinName);
	// create an instance pin and verify that we can recover what we put into it
	std::string pinName = "pin";
	InstancePinSharedPtr instancePin1Ptr = Factory::newInstancePinPtr(instance1Ptr, "");
	InstancePinSharedPtr instancePin2Ptr = Factory::newInstancePinPtr(instance2Ptr, pinName);

	// functions tested:
	//		const InstanceWeakPtr& getInstancePtr(void) const;
	//		const PinName& getPinName(void) const;
	//		void setInstancePtr(const InstanceWeakPtr& inInstancePtr);
	//		void setPinName(const string& inPinName);
	BOOST_CHECK(*(instancePin1Ptr->getInstancePtr().lock()) == *instance1Ptr);
	BOOST_CHECK(instancePin1Ptr->getPinName().empty());
	instancePin1Ptr->setInstancePtr(instance2Ptr);
	instancePin1Ptr->setPinName(pinName);
	BOOST_CHECK(*(instancePin1Ptr->getInstancePtr().lock()) == *instance2Ptr);
	BOOST_CHECK(instancePin1Ptr->getPinName() == pinName);

	// functions tested:
	//		bool operator ==(const InstancePin& rhs) const;
	BOOST_CHECK(*instancePin1Ptr == *instancePin2Ptr);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
