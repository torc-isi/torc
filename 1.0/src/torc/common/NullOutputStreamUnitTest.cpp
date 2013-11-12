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
/// \brief Unit test for NullOutputStream class.

#include <boost/test/unit_test.hpp>
#include "torc/common/NullOutputStream.hpp"

namespace torc {
namespace common {

BOOST_AUTO_TEST_SUITE(common)

/// \brief Unit test for the NullOutputStream class.
BOOST_AUTO_TEST_CASE(NullOutputStreamUnitTest) {
	// enable all exceptions on the global null output stream, and try writing a bunch of stuff
	bool passed = true;
	try {
		cnull.exceptions(std::ios_base::eofbit | std::ios_base::failbit | std::ios_base::badbit);
		for(int i = 0; i < 1000000; i++) cnull << i;
	}
	catch(...) {
		passed = false;
	}
	BOOST_CHECK(passed);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace common
} // namespace torc
