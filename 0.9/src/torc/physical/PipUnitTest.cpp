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
/// \brief Source for the Pip unit test.

#include <boost/test/unit_test.hpp>
#include "torc/physical/Factory.hpp"
#include "torc/physical/Pip.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical)

/// \brief Unit test for the Pip class.
BOOST_AUTO_TEST_CASE(PipUnitTest) {
	// create an accessory instance and routethrough
	InstanceSharedPtr instancePtr = Factory::newInstancePtr("name", "type", "tile", "site", 
		eInstanceBondingUnknown);
	BOOST_REQUIRE(instancePtr.get() != 0);
	RoutethroughSharedPtr routethroughPtr = Factory::newRoutethroughPtr("setting", "name", "value", 
		instancePtr, "source", "sink");
	BOOST_REQUIRE(routethroughPtr.get() != 0);

	// functions tested:
	//		Pip(const string& inTileName, const string& inSourceWireName, 
	//			const string& inSinkWireName, EPipDirection inPipDirection, 
	//			RoutethroughSharedPtr inRoutethroughPtr);
	std::string tile = "tile";
	std::string source = "source";
	std::string sink = "sink";
	EPipDirection direction = ePipUnidirectionalBuffered;
	Pip pip1 = Factory::newPip(tile, source, sink, direction, routethroughPtr);
	Pip pip2 = Factory::newPip(tile, source, sink, direction, routethroughPtr);

	// functions tested:
	//		const TileName& getTileName(void) const;
	//		const WireName& getSourceWireName(void) const;
	//		const WireName& getSinkWireName(void) const;
	//		EPipDirection getDirection(void) const;
	//		const char* getDirectionString(void) const;
	//		static const char* getDirectionString(EPipDirection inPipDirection);
	BOOST_CHECK_EQUAL(pip1.getTileName(), tile);
	BOOST_CHECK_EQUAL(pip1.getSourceWireName(), source);
	BOOST_CHECK_EQUAL(pip1.getSinkWireName(), sink);
	BOOST_CHECK(pip1.getDirection() == direction);
	BOOST_CHECK(pip1.getDirectionString() == Pip::getDirectionString(direction));
	BOOST_CHECK_EQUAL("==", Pip::getDirectionString(ePipBidirectionalUnbuffered));
	BOOST_CHECK_EQUAL("=>", Pip::getDirectionString(ePipBidirectionalUnidirectionallyBuffered));
	BOOST_CHECK_EQUAL("=-", Pip::getDirectionString(ePipBidirectionalBidirectionallyBuffered));
	BOOST_CHECK_EQUAL("->", Pip::getDirectionString(ePipUnidirectionalBuffered));

	// functions tested:
	//		RoutethroughSharedPtr getRoutethroughPtr(void) const;
	//		bool isRoutethrough(void) const;
	BOOST_CHECK(pip1.isRoutethrough());
	BOOST_CHECK(pip1.getRoutethroughPtr() == routethroughPtr);

	// functions tested:
	//		bool operator ==(const Pip& rhs) const;
	BOOST_CHECK(pip1 == pip1); // name based comparison
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
