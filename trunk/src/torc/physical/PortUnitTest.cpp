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
/// \brief Source for the Port unit test.

#include <boost/test/unit_test.hpp>
#include "torc/physical/Factory.hpp"
#include "torc/physical/Pip.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical_database)

/// \brief Unit test for the Port class.
BOOST_AUTO_TEST_CASE(physical_port) {
	// create an accessory instance
	InstanceSharedPtr instancePtr = Factory::newInstancePtr("name", "type", "tile", "site", 
		eInstanceBondingUnknown);
	BOOST_REQUIRE(instancePtr.get() != 0);

	// functions tested:
	//		Port(const string& inName, InstanceSharedPtr inInstancePtr, const string& inPinName);
	std::string name = "name";
	std::string pin = "pin";
	PortSharedPtr port1Ptr = Factory::newPortPtr(name, instancePtr, pin);
	PortSharedPtr port2Ptr = Factory::newPortPtr(name, instancePtr, pin);
	BOOST_REQUIRE(port1Ptr.get() != 0);
	BOOST_REQUIRE(port2Ptr.get() != 0);

	// functions tested:
	//		const InstanceWeakPtr& getInstancePtr(void) const;
	//		const PinName& getPinName(void) const;
	BOOST_CHECK(port1Ptr->getInstancePtr().lock() == instancePtr);
	BOOST_CHECK(port1Ptr->getPinName() == pin);

	// functions tested:
	//		bool operator ==(const Port& rhs) const;
	BOOST_CHECK(*port1Ptr == *port2Ptr); // name based comparison
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
