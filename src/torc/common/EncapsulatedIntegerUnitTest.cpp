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
/// \brief Unit test for EncapsulatedInteger class.

#include <boost/test/unit_test.hpp>
#include <boost/cstdint.hpp>
#include <boost/integer_traits.hpp>
#include "torc/common/EncapsulatedInteger.hpp"

namespace torc {
namespace common {

BOOST_AUTO_TEST_SUITE(common)

/// \brief Unit test for standard integer sizes.
BOOST_AUTO_TEST_CASE(EncapsulatedIntegerSizeUnitTest) {
	// types tested: 
	//		int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t
	// verify the sizes of 8-, 16-, 32-, and 64-bit integers
	BOOST_REQUIRE_EQUAL((size_t) 1, sizeof(boost::int8_t));
	BOOST_REQUIRE_EQUAL((size_t) 1, sizeof(boost::uint8_t));
	BOOST_REQUIRE_EQUAL((size_t) 2, sizeof(boost::int16_t));
	BOOST_REQUIRE_EQUAL((size_t) 2, sizeof(boost::uint16_t));
	BOOST_REQUIRE_EQUAL((size_t) 4, sizeof(boost::int32_t));
	BOOST_REQUIRE_EQUAL((size_t) 4, sizeof(boost::uint32_t));
	BOOST_REQUIRE_EQUAL((size_t) 8, sizeof(boost::int64_t));
	BOOST_REQUIRE_EQUAL((size_t) 8, sizeof(boost::uint64_t));

	// types tested:
	//		char, short, int, long, long long
	// verify the sizes of standard integers
	BOOST_WARN_EQUAL((size_t) 1, sizeof(char));
	BOOST_WARN_EQUAL((size_t) 2, sizeof(short));
	BOOST_WARN_EQUAL((size_t) 4, sizeof(int));
	BOOST_WARN_EQUAL((size_t) 8, sizeof(long));
	BOOST_WARN_EQUAL((size_t) 8, sizeof(long long));
}

/// \brief Standard macro to be used for each of the encapsulated integer types.
#define STANDARD_ENCAPSULATED_INTEGER_BLOCK(TYPE) \
{ \
	typedef TYPE type; \
	typedef EncapsulatedInteger<type> EInt; \
	\
	/* functions tested: */ \
	/*		EncapsulatedInteger(const T& rhs); */ \
	/*		EncapsulatedInteger(const type& rhs); */ \
	/*		EncapsulatedInteger(void); */ \
	EInt eint1 = boost::integer_traits<TYPE>::const_min; /* primitive constructor */ \
	EInt eint2 = eint1; /* object constructor */ \
	EInt eint3, eint4, eint5; /* null constructors */ \
    (void) eint2; /* be sure to test the constructor even if the object is unused */ \
	\
	/* functions tested: */ \
	/*		type& operator =(const type& rhs); */ \
	/*		type& operator =(const T& rhs); */ \
	eint3 = eint1; /* object assignment */ \
	eint4 = boost::integer_traits<TYPE>::const_max; /* primitive assignment */ \
	\
	/* functions tested: */ \
	/*		operator T&(void); */ \
	type int1 = eint3; /* non-const primitive reference typecast */ \
	type int2 = eint4; /* non-const primitive reference typecast */ \
	\
	/* functions tested: */ \
	/*		bool operator ==(const T& rhs) const; */ \
	/*		bool operator ==(const type& rhs) const; */ \
	/*		operator const T&(void) const; */ \
	BOOST_CHECK_EQUAL(boost::integer_traits<TYPE>::const_min, int1); /* minimum value */ \
	BOOST_CHECK_EQUAL(boost::integer_traits<TYPE>::const_max, int2); /* maximum value */ \
	BOOST_CHECK_EQUAL(boost::integer_traits<TYPE>::const_min, eint3); /* minimum value */ \
	BOOST_CHECK_EQUAL(boost::integer_traits<TYPE>::const_max, eint4); /* maximum value */ \
}

/// \brief Unit test for encapsulated integers.
BOOST_AUTO_TEST_CASE(EncapsulatedIntegerLimitsUnitTest) {
	// 8-bit unsigned
	STANDARD_ENCAPSULATED_INTEGER_BLOCK(boost::uint8_t);
	// 8-bit signed
	STANDARD_ENCAPSULATED_INTEGER_BLOCK(boost::int8_t);
	// 16-bit unsigned
	STANDARD_ENCAPSULATED_INTEGER_BLOCK(boost::uint16_t);
	// 16-bit signed
	STANDARD_ENCAPSULATED_INTEGER_BLOCK(boost::int16_t);
	// 32-bit unsigned
	STANDARD_ENCAPSULATED_INTEGER_BLOCK(boost::uint32_t);
	// 32-bit signed
	STANDARD_ENCAPSULATED_INTEGER_BLOCK(boost::int32_t);
	// 64-bit unsigned
	STANDARD_ENCAPSULATED_INTEGER_BLOCK(boost::uint64_t);
	// 64-bit signed
	STANDARD_ENCAPSULATED_INTEGER_BLOCK(boost::int64_t);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace common
} // namespace torc
