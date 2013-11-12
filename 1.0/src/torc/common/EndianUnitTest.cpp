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
/// \brief Unit test for endian conversion.

#include <boost/test/unit_test.hpp>
#include "torc/common/Endian.hpp"

namespace torc {
namespace common {

using torc::common::htonll;
using torc::common::ntohll;

BOOST_AUTO_TEST_SUITE(common)

/// \brief Unit test for endian conversion.
BOOST_AUTO_TEST_CASE(EndianUnitTest) {
	// ensure that BOOST_BIG_ENDIAN or BOOST_LITTLE_ENDIAN are properly defined
#if defined(BOOST_BIG_ENDIAN) && defined(BOOST_LITTLE_ENDIAN)
	#error("Both BOOST_BIG_ENDIAN and BOOST_LITTLE_ENDIAN are defined.")
#endif
#if !defined(BOOST_BIG_ENDIAN) && !defined(BOOST_LITTLE_ENDIAN)
	#error("Neither BOOST_BIG_ENDIAN nor BOOST_LITTLE_ENDIAN are defined.")
#endif

	// functions tested:
	//		htons(uint16_t);
	//		ntohs(uint16_t);
	// verify 16-bit conversion to and from network order
	union { char c[2]; boost::uint16_t n; } be16 = {{0x01,0x23}};
	boost::uint16_t uint16_i_actual = be16.n;
#if defined(BOOST_BIG_ENDIAN)
	boost::uint16_t uint16_h_expected = 0x0123;
#else // defined(BOOST_LITTLE_ENDIAN)
	boost::uint16_t uint16_h_expected = 0x2301;
#endif
	BOOST_CHECK_EQUAL(uint16_h_expected, uint16_i_actual);
	boost::uint16_t uint16_n_actual = htons(uint16_i_actual);
	boost::uint16_t uint16_n_expected = 0x0123;
	BOOST_CHECK_EQUAL(uint16_n_expected, uint16_n_actual);
	boost::uint16_t uint16_h_actual = ntohs(uint16_n_actual);
	BOOST_CHECK_EQUAL(uint16_h_expected, uint16_h_actual);

	// functions tested:
	//		htons(uint32_t);
	//		ntohs(uint32_t);
	// verify 32-bit conversion to and from network order
	union { uint8_t c[4]; boost::uint32_t n; } be32 = {{0x01,0x23,0x45,0x67}};
	boost::uint32_t uint32_i_actual = be32.n;
#if defined(BOOST_BIG_ENDIAN)
	boost::uint32_t uint32_h_expected = 0x01234567;
#else // defined(BOOST_LITTLE_ENDIAN)
	boost::uint32_t uint32_h_expected = 0x67452301;
#endif
	BOOST_CHECK_EQUAL(uint32_h_expected, uint32_i_actual);
	boost::uint32_t uint32_n_actual = htonl(uint32_i_actual);
	boost::uint32_t uint32_n_expected = 0x01234567;
	BOOST_CHECK_EQUAL(uint32_n_expected, uint32_n_actual);
	boost::uint32_t uint32_h_actual = ntohl(uint32_n_actual);
	BOOST_CHECK_EQUAL(uint32_h_expected, uint32_h_actual);

	// functions tested:
	//		htons(uint64_t);
	//		ntohs(uint64_t);
	// verify 64-bit conversion to and from network order
	union { uint8_t c[8]; boost::uint64_t n; } be64 = {{0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef}};
	boost::uint64_t uint64_i_actual = be64.n;
#if defined(BOOST_BIG_ENDIAN)
	boost::uint64_t uint64_h_expected = 0x0123456789abcdefll;
#else // defined(BOOST_LITTLE_ENDIAN)
	boost::uint64_t uint64_h_expected = 0xefcdab8967452301ll;
#endif
	BOOST_CHECK_EQUAL(uint64_h_expected, uint64_i_actual);
	boost::uint64_t uint64_n_actual = htonll(uint64_i_actual);
	boost::uint64_t uint64_n_expected = 0x0123456789abcdefll;
	BOOST_CHECK_EQUAL(uint64_n_expected, uint64_n_actual);
	boost::uint64_t uint64_h_actual = ntohll(uint64_n_actual);
	BOOST_CHECK_EQUAL(uint64_h_expected, uint64_h_actual);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace common
} // namespace torc
