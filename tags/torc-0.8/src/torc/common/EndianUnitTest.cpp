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
/// \brief Unit test for endian conversion.

#include <boost/test/unit_test.hpp>
#include "torc/common/Endian.hpp"

namespace torc {
namespace common {

using torc::common::htonll;
using torc::common::ntohll;

BOOST_AUTO_TEST_SUITE(portability)

/// \brief Unit test for endian conversion.
BOOST_AUTO_TEST_CASE(endian_conversion) {
	// functions tested:
	//		htons(uint16_t);
	//		ntohs(uint16_t);
	// verify 16-bit conversion to and from network order
	boost::uint16_t uint16_h_expected = 0x0123;
	boost::uint16_t uint16_n_expected = 0x2301;
	boost::uint16_t uint16_n_actual = htons(uint16_h_expected);
	boost::uint16_t uint16_h_actual = ntohs(uint16_n_expected);
	BOOST_CHECK_EQUAL(uint16_h_expected, uint16_h_actual);
	BOOST_CHECK_EQUAL(uint16_n_expected, uint16_n_actual);

	// functions tested:
	//		htons(uint32_t);
	//		ntohs(uint32_t);
	// verify 32-bit conversion to and from network order
	boost::uint32_t uint32_h_expected = 0x01234567;
	boost::uint32_t uint32_n_expected = 0x67452301;
	boost::uint32_t uint32_n_actual = htonl(uint32_h_expected);
	boost::uint32_t uint32_h_actual = ntohl(uint32_n_expected);
	BOOST_CHECK_EQUAL(uint32_h_expected, uint32_h_actual);
	BOOST_CHECK_EQUAL(uint32_n_expected, uint32_n_actual);

	// functions tested:
	//		htons(uint64_t);
	//		ntohs(uint64_t);
	// verify 64-bit conversion to and from network order
	boost::uint64_t uint64_h_expected = 0x0123456789abcdefll;
	boost::uint64_t uint64_n_expected = 0xefcdab8967452301ll;
	boost::uint64_t uint64_n_actual = htonll(uint64_h_expected);
	boost::uint64_t uint64_h_actual = ntohll(uint64_n_expected);
	BOOST_CHECK_EQUAL(uint64_h_expected, uint64_h_actual);
	BOOST_CHECK_EQUAL(uint64_n_expected, uint64_n_actual);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace common
} // namespace torc
