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
/// \brief Unit test for the InstancePin class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/InstancePin.hpp"
#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/physical/InstancePin.hpp"
#include "torc/physical/Factory.hpp"
#include <boost/pointer_cast.hpp>

namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the InstancePin class.
BOOST_AUTO_TEST_CASE(InstancePinUnitTest) {
	// create objects to work with
	torc::physical::InstanceSharedPtr instancePtr 
		= torc::physical::Factory::newInstancePtr("name", "type", "tile", "site");
	torc::physical::InstancePinSharedPtr physicalInstancePinPtr 
		= torc::physical::Factory::newInstancePinPtr(instancePtr, "pin");

	// features tested:
	//		sizeof(InstancePin);
	BOOST_REQUIRE_EQUAL(sizeof(InstancePin), sizeof(torc::physical::InstancePin));

	// macros tested:
	//		INSTANCE_PIN_PHYSICAL_TO_ARCHITECTURE(x);
	//		INSTANCE_PIN_ARCHITECTURE_TO_PHYSICAL(x);
	InstancePinSharedPtr& architectureInstancePinPtr 
		= INSTANCE_PIN_PHYSICAL_TO_ARCHITECTURE(physicalInstancePinPtr);
	torc::physical::InstancePinSharedPtr& physicalInstancePin2Ptr 
		= INSTANCE_PIN_ARCHITECTURE_TO_PHYSICAL(architectureInstancePinPtr);
	BOOST_CHECK_EQUAL(architectureInstancePinPtr->getTileIndex(), xilinx::TileIndex(0));
	BOOST_CHECK_EQUAL(architectureInstancePinPtr->getWireIndex(), xilinx::WireIndex(0));

	// functions tested:
	//		Tilewire& getTilewire(void);
	Tilewire& tilewire = architectureInstancePinPtr->getTilewire();
	tilewire = Tilewire(xilinx::TileIndex(32), xilinx::WireIndex(16));
	BOOST_CHECK_EQUAL(tilewire.getTileIndex(), xilinx::TileIndex(32));
	BOOST_CHECK_EQUAL(tilewire.getWireIndex(), xilinx::WireIndex(16));
	BOOST_REQUIRE_EQUAL(sizeof(*physicalInstancePin2Ptr), sizeof(*architectureInstancePinPtr));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
