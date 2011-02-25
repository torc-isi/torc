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
/// \brief Source for the DottedVersion unit test.

#include <boost/test/unit_test.hpp>
#include "torc/common/DottedVersion.hpp"

namespace torc {
namespace common {

BOOST_AUTO_TEST_SUITE(common)

/// \brief Unit test for the DottedVersion class.
BOOST_AUTO_TEST_CASE(dotted_version) {
	// functions tested:
	//		DottedVersion(uint8_t inMajor = 0, uint8_t inMinor = 0, uint8_t inRevision = 0, 
	//			uint8_t inReserved = 0);
	// members tested:
	//		uint8_t mMajor;
	//		uint8_t mMinor;
	//		uint8_t mRevision;
	//		uint8_t mReserved;
	DottedVersion version(1, 2, 3, 4);
	BOOST_CHECK_EQUAL(version.mMajor, 1);
	BOOST_CHECK_EQUAL(version.mMinor, 2);
	BOOST_CHECK_EQUAL(version.mRevision, 3);
	BOOST_CHECK_EQUAL(version.mReserved, 4);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace common
} // namespace torc
