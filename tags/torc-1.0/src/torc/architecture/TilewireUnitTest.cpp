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
/// \brief Unit test for the Tilewire class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/Tilewire.hpp"
#include "torc/architecture/OutputStreamHelpers.hpp"

namespace torc {
namespace architecture {
namespace xilinx {

BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the Tilewire class.
BOOST_AUTO_TEST_CASE(TilewireUnitTest) {
	// create accessory wires and tiles
	WireIndex wire1(15);
	WireIndex wire2(300);
	TileIndex tile1(1);
	TileIndex tile2(1);

	// features tested:
	//		sizeof(Tilewire)
	BOOST_CHECK_EQUAL(sizeof(Tilewire), (sizeof(TileIndex) + sizeof(WireIndex)));
	BOOST_CHECK_EQUAL(sizeof(Tilewire), 6u);

	// functions tested:
	//		Tilewire(void);
	//		Tilewire(const TileIndex& inTileIndex, const WireIndex& inWireIndex);
	//		Tilewire(const Tilewire& inTilewire);
	//		bool operator ==(const Tilewire& rhs) const;
	Tilewire tilewire1(tile1, wire1);
	Tilewire tilewire2(tile2, wire2);
	Tilewire tilewire3(tilewire1);
	Tilewire tilewire4 = tilewire1;
	Tilewire tilewire5;
	BOOST_CHECK_EQUAL(tilewire1 == tilewire2, false);
	BOOST_CHECK_EQUAL(tilewire1 == tilewire3, true);

	// functions tested:
	//		const TileIndex& getTileIndex(void) const;
	//		const WireIndex& getWireIndex(void) const;
	//		void setTileIndex(const TileIndex& inTileIndex);
	//		void setWireIndex(const WireIndex& inWireIndex);
	BOOST_CHECK_EQUAL(tilewire3.getTileIndex(), tile1);
	BOOST_CHECK_EQUAL(tilewire3.getWireIndex(), wire1);
	BOOST_CHECK_EQUAL(tilewire5.getTileIndex(), TileIndex::undefined());
	BOOST_CHECK_EQUAL(tilewire5.getWireIndex(), WireIndex::undefined());
	tilewire3.setTileIndex(tile2);
	tilewire3.setWireIndex(wire2);
	BOOST_CHECK_EQUAL(tilewire3.getTileIndex(), tile2);
	BOOST_CHECK_EQUAL(tilewire3.getWireIndex(), wire2);

	// functions tested:
	//		const Tilewire& operator =(const Tilewire& rhs) const;
	tilewire4 = tilewire2;
	BOOST_CHECK_EQUAL(tilewire4.getTileIndex(), tile2);
	BOOST_CHECK_EQUAL(tilewire4.getWireIndex(), wire2);

	// functions tested:
	//		bool operator <(const Tilewire& rhs) const;
	BOOST_CHECK_EQUAL(tilewire1 < tilewire2, true);
	BOOST_CHECK_EQUAL(tilewire2 < tilewire1, false);
	BOOST_CHECK_EQUAL(tilewire4 < tilewire2, false);
	BOOST_CHECK_EQUAL(tilewire2 < tilewire4, false);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace xilinx
} // namespace architecture
} // namespace torc
