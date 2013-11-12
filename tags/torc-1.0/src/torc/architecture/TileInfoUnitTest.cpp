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
/// \brief Unit test for the TileInfo class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/TileInfo.hpp"

namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the TileInfo class.
BOOST_AUTO_TEST_CASE(TileInfoUnitTest) {
	// functions tested:
	//		friend uint32_t distance(const TileInfo& inA, const TileInfo& inB);
	// create a few TileInfo objects
	typedef xilinx::TileTypeIndex TileTypeIndex;
	typedef xilinx::TileRow TileRow;
	typedef xilinx::TileCol TileCol;
	TileInfo tileInfo1; tileInfo1.set(TileTypeIndex(1), TileRow( 0), TileCol( 0), "tile1");
	TileInfo tileInfo2; tileInfo2.set(TileTypeIndex(2), TileRow(10), TileCol(10), "tile2");
	TileInfo tileInfo3; tileInfo3.set(TileTypeIndex(3), TileRow(30), TileCol( 0), "tile3");
	BOOST_CHECK_EQUAL(manhattanDistance(tileInfo1, tileInfo2), uint32_t(20));
	BOOST_CHECK_EQUAL(manhattanDistance(tileInfo1, tileInfo2), 
		manhattanDistance(tileInfo2, tileInfo1));
	BOOST_CHECK_EQUAL(manhattanDistance(tileInfo1, tileInfo3), uint32_t(30));
	BOOST_CHECK_EQUAL(manhattanDistance(tileInfo1, tileInfo3), 
		manhattanDistance(tileInfo3, tileInfo1));
	BOOST_CHECK_EQUAL(manhattanDistance(tileInfo2, tileInfo3), uint32_t(30));
	BOOST_CHECK_EQUAL(manhattanDistance(tileInfo2, tileInfo3), 
		manhattanDistance(tileInfo3, tileInfo2));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(regression)
BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the TileInfo class.
BOOST_AUTO_TEST_CASE(TileInfoUnitTest) {

	/// \todo Write a unit test for torc::architecture::TileInfo.
	BOOST_REQUIRE(false);

}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
