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
/// \brief Unit test for the Arc class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/Arc.hpp"
#include <iostream>

namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the Arc class.
BOOST_AUTO_TEST_CASE(ArcUnitTest) {
	// supporting variables
	Tilewire tilewire1(xilinx::TileIndex(5), xilinx::WireIndex(3));
	Tilewire tilewire2(xilinx::TileIndex(0), xilinx::WireIndex(9));

	// members tested:
	//		Tilewire mSourceTilewire;
	//		Tilewire mSinkTilewire;
	// functions tested:
	//		Arc(void);
	//		Arc(const Tilewire& inSourceTilewire, const Tilewire& inSinkTilewire);
	//		bool operator ==(const Arc& rhs) const;
	//		bool operator <(const Arc& rhs) const;
	//		bool isUndefined(void) const;
	Arc arc1;
	Arc arc2;
	Arc arc3(tilewire1, tilewire2);
	Arc arc4(tilewire2, tilewire1);
	BOOST_CHECK((arc1 == arc2) == true);
	BOOST_CHECK((arc1 == arc3) == false);
	BOOST_CHECK((arc3 < arc4) == true);
	BOOST_CHECK((arc4 < arc3) == false);
	BOOST_CHECK(arc1.isUndefined());

	// functions tested:
	//		const Tilewire& getSourceTilewire(void) const;
	//		const Tilewire& getSinkTilewire(void) const;
	BOOST_CHECK(arc3.getSourceTilewire() == tilewire1);
	BOOST_CHECK(arc3.getSinkTilewire() == tilewire2);

	// functions tested:
	//		friend std::size_t hash_value(const Arc& inArc);
	BOOST_CHECK_EQUAL(hash_value(arc1), hash_value(arc2));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
