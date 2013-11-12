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
/// \brief Unit test for the Sites class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/Sites.hpp"
#include "torc/architecture/DDB.hpp"


#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>


namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(regression)
BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the Sites class.
BOOST_AUTO_TEST_CASE(SitesUnitTest) {
/*
	typedef boost::bimap< boost::bimaps::multiset_of<std::string>, boost::bimaps::multiset_of<std::string> > bm_type;
	bm_type bm;

	bm.insert( bm_type::value_type( "SLICEL", "SLICEL" ) );
	bm.insert( bm_type::value_type( "SLICEL", "SLICEM" ) );
	bm.insert( bm_type::value_type( "SLICEM", "SLICEM" ) );

	bm.insert( bm_type::value_type( "BUFG", "BUFGCTRL" ) );
	bm.insert( bm_type::value_type( "BUFGCTRL", "BUFGCTRL" ) );

	bm.insert( bm_type::value_type( "IOBS", "IOBS" ) );
	bm.insert( bm_type::value_type( "IOB", "IOBS" ) );
	bm.insert( bm_type::value_type( "IOB", "IOBM" ) );
	bm.insert( bm_type::value_type( "IOBM", "IOBM" ) );

	bm.upper_bound("IOBS");

	BOOST_CHECK(bm.size() == 9);
	assert( bm.size() == 9 );
*/
	/// \todo Write a unit test for torc::architecture::Sites.
	BOOST_REQUIRE(false);

}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
