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
/// \brief Unit test for the PrimitiveConn class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/PrimitiveConn.hpp"

namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the PrimitiveConn class.
BOOST_AUTO_TEST_CASE(PrimitiveConnUnitTest) {
	// members tested:
	//		const PrimitiveElementPin* mSourcePtr;
	//		PrimitiveElementPinPtrVector mSinks;
	// functions tested:
	//		const PrimitiveElementPin* getSourcePtr(void) const
	//		const PrimitiveElementPinPtrVector& getSinks(void) const
	PrimitiveElementPin pin1;
	PrimitiveElementPin pin2;
	PrimitiveElementPin pin3;
	PrimitiveConn conn;
	conn.mSourcePtr = &pin1;
	conn.mSinks.push_back(&pin2);
	conn.mSinks.push_back(&pin3);
	BOOST_CHECK_EQUAL(conn.getSourcePtr(), &pin1);
	const PrimitiveElementPinPtrVector& sinks = conn.getSinks();
	PrimitiveElementPinPtrVector::const_iterator p = sinks.begin();
	BOOST_CHECK_EQUAL(*p++, &pin2);
	BOOST_CHECK_EQUAL(*p++, &pin3);
	BOOST_CHECK(p == sinks.end());
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
