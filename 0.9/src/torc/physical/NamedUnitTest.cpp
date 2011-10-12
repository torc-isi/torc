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
/// \brief Source for the Named unit test.

#include <boost/test/unit_test.hpp>
#include "torc/physical/Named.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical)

/// \brief Unit test for the Named class.
BOOST_AUTO_TEST_CASE(NamedUnitTest) {
	// functions tested:
	//		Named(const string& inName);
	//		const string& getName(void) const;
	//		bool operator ==(const Named& rhs) const;
	// create two Named objects
	std::string name1 = "name1";
	std::string name2 = "name2";
	Named named1(name1);
	Named named2(name2);
	BOOST_CHECK(named1.getName() == name1);
	BOOST_CHECK_EQUAL(named1 == named2, false);

	// functions tested:
	//		NameComparator(const string& inName);
	//		bool operator() (const Named& inNamed) const;
	//		bool operator() (const NamedSharedPtr& inNamedPtr) const;
	// compare Named objects with a comparator
	NameComparator comparator(name1);
	NamedSharedPtr named1Ptr = NamedSharedPtr(new Named(name1));
	BOOST_CHECK_EQUAL(comparator(named1Ptr), true);
	BOOST_CHECK_EQUAL(comparator(named1), true);
	BOOST_CHECK_EQUAL(comparator(named2), false);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
