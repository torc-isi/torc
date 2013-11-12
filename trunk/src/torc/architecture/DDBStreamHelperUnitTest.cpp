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
/// \brief Unit test for the DDBStreamHelper class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/DDBStreamHelper.hpp"
#include "torc/architecture/OutputStreamHelpers.hpp"
#include <sstream>

namespace torc {
namespace architecture {

using namespace torc::architecture::xilinx;

BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the DDBStreamHelper class.
BOOST_AUTO_TEST_CASE(DDBStreamHelperUnitTest) {
	// functions tested:
	//		std::ostream& operator <<(std::ostream& os, const class DDB& ddb);
	//		static const class DDB* getDDBPtr(std::ostream& os);
	DDB ddb("xc2vp2");
	std::stringstream s;
	s << ddb;
	BOOST_CHECK(DDBStreamHelper::getDDBPtr(s) == &ddb);

	// functions tested:
	//		std::ostream& operator <<(std::ostream& os, const DDB& ddb);
	//		std::ostream& operator <<(std::ostream& os, const Tilewire& rhs);
	s << ddb << Tilewire(TileIndex(1), WireIndex(0));
	BOOST_CHECK_EQUAL(s.str(), "CNR_TBTERM_N6A1@[0,1] CNR_TTERM \"LIOITTERM\" (0@1)");
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
