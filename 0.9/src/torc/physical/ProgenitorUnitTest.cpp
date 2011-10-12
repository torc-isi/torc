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
/// \brief Unit test for the Progenitor class.

#include <boost/test/unit_test.hpp>
#include "torc/physical/Factory.hpp"
#include "torc/physical/Progenitor.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical)

/// \cond OMIT_FROM_DOXYGEN
class A : public Progenitor<class A> {};
/// \endcond

/// \brief Unit test for the Progenitor class.
BOOST_AUTO_TEST_CASE(ProgenitorUnitTest) {
	// functions tested:
	//		[null constructor]
	//		void setSelfWeakPtr(WeakPtrType inSelfPtr);
	//		const WeakPtrType& getSelfWeakPtr(void) const;
	typedef Progenitor<A> progenitor_t;
	progenitor_t::SharedPtrType progenitorPtr(new A());
	BOOST_REQUIRE(progenitorPtr.get() != 0);
	progenitorPtr->setSelfWeakPtr(progenitorPtr);
	BOOST_CHECK(progenitorPtr->getSelfWeakPtr().lock() == progenitorPtr);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
