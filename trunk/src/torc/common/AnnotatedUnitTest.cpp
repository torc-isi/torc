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
/// \brief Unit test for the Annotated class.

#include <boost/test/unit_test.hpp>
#include <boost/cstdint.hpp>
#include "torc/common/Annotated.hpp"

namespace torc {
namespace common {

BOOST_AUTO_TEST_SUITE(common)

/// \brief Unit test for the Annotated class.
BOOST_AUTO_TEST_CASE(AnnotatedUnitTest) {

	Annotated outer;
	Annotated inner;
	
	boost::uint32_t key0 = 0;
	boost::uint32_t key1 = 1;
	boost::uint32_t innerkey = 2;
	
	boost::uint32_t value0 = 42;
	boost::uint32_t innervalue = 99;
	
	inner.setAnnotation(innerkey, innervalue);
	
	outer.setAnnotation(key0, value0);
	outer.setAnnotation(key1, inner);
	
	BOOST_CHECK_EQUAL(inner.hasAnnotation(innerkey), true);
	BOOST_CHECK_EQUAL(inner.hasAnnotation(key0), false);
	
	BOOST_CHECK_EQUAL(outer.hasAnnotation(key0), true);
	BOOST_CHECK_EQUAL(outer.hasAnnotation(key1), true);
	
	boost::uint32_t val0 = boost::any_cast<boost::uint32_t>(outer.getAnnotation(key0));
	BOOST_CHECK_EQUAL(val0, value0);
	
	BOOST_CHECK_EQUAL(boost::any_cast<boost::uint32_t>((boost::any_cast<Annotated>(
		outer.getAnnotation(key1))).getAnnotation(innerkey)), innervalue);
	
	outer.removeAnnotation(key0);
	BOOST_CHECK_EQUAL(outer.hasAnnotation(key0), false);
	
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace common
} // namespace torc
