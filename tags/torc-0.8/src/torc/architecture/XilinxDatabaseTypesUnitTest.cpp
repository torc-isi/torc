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
/// \brief Unit test for Xilinx device database types.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/XilinxDatabaseTypes.hpp"

namespace torc {

using namespace torc::architecture::xilinx;

BOOST_AUTO_TEST_SUITE(portability)

/// \brief Unit test for device database type sizes.
BOOST_AUTO_TEST_CASE(architecture_type_sizes) {
	// verify the sizes of device database types
	// wires
	BOOST_CHECK_EQUAL((size_t) 2, sizeof(WireCount));
	BOOST_CHECK_EQUAL((size_t) 2, sizeof(WireIndex));
	BOOST_CHECK_EQUAL((size_t) 2, sizeof(WireFlags));
	// tiles
	BOOST_CHECK_EQUAL((size_t) 4, sizeof(TileCount));
	BOOST_CHECK_EQUAL((size_t) 4, sizeof(TileIndex));
	BOOST_CHECK_EQUAL((size_t) 4, sizeof(TileOffset));
	BOOST_CHECK_EQUAL((size_t) 2, sizeof(TileRow));
	BOOST_CHECK_EQUAL((size_t) 2, sizeof(TileCol));
	// tile types
	BOOST_CHECK_EQUAL((size_t) 2, sizeof(TileTypeCount));
	BOOST_CHECK_EQUAL((size_t) 2, sizeof(TileTypeIndex));
	// compact segments
	BOOST_CHECK_EQUAL((size_t) 4, sizeof(CompactSegmentCount));
	BOOST_CHECK_EQUAL((size_t) 4, sizeof(CompactSegmentIndex));
	// sites
	BOOST_CHECK_EQUAL((size_t) 4, sizeof(SiteCount));
	BOOST_CHECK_EQUAL((size_t) 4, sizeof(SiteIndex));
	BOOST_CHECK_EQUAL((size_t) 2, sizeof(SiteFlags));
	// site types
	BOOST_CHECK_EQUAL((size_t) 2, sizeof(SiteTypeCount));
	BOOST_CHECK_EQUAL((size_t) 2, sizeof(SiteTypeIndex));
	// pins
	BOOST_CHECK_EQUAL((size_t) 4, sizeof(PinCount));
	BOOST_CHECK_EQUAL((size_t) 4, sizeof(PinIndex));
	BOOST_CHECK_EQUAL((size_t) 2, sizeof(PinFlags));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace torc
