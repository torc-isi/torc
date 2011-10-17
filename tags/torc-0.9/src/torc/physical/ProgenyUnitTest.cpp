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
/// \brief Unit test for the Progeny class.

#include <boost/test/unit_test.hpp>
#include "torc/physical/Factory.hpp"
#include "torc/physical/Pip.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical)

/// \cond OMIT_FROM_DOXYGEN
class A : public Progenitor<class A> {};
class B : public Progeny<class A> {};
/// \endcond

/// \brief Unit test for the Progeny class.
BOOST_AUTO_TEST_CASE(ProgenyUnitTest) {
	// create an accessory progenitor
	typedef Progenitor<A> progenitor_t;
	progenitor_t::SharedPtrType progenitorPtr(new A());
	BOOST_REQUIRE(progenitorPtr.get() != 0);
	progenitorPtr->setSelfWeakPtr(progenitorPtr);

	// functions tested:
	//		Progeny(void);
	//		Progeny(const WeakPtrType& inParentPtr);
	//		void setParentWeakPtr(WeakPtrType inParentPtr);
	//		const WeakPtrType& getParentWeakPtr(void) const;
	//		void resetParentWeakPtr(void);
	B progeny1;
	B progeny2/*(progenitorPtr)*/;
	BOOST_CHECK_EQUAL(progeny1.getParentWeakPtr().expired(), true);
	BOOST_CHECK_EQUAL(progeny2.getParentWeakPtr().expired(), true);
	progeny2.setParentWeakPtr(progenitorPtr);
	BOOST_CHECK_EQUAL(progeny2.getParentWeakPtr().expired(), false);
	BOOST_CHECK(progeny2.getParentWeakPtr().lock() == progenitorPtr);
	progeny1.setParentWeakPtr(progeny2.getParentWeakPtr());
	progeny2.resetParentWeakPtr();
	BOOST_CHECK(progeny1.getParentWeakPtr().lock() == progenitorPtr);
	BOOST_CHECK_EQUAL(progeny1.getParentWeakPtr().expired(), false);
	BOOST_CHECK_EQUAL(progeny2.getParentWeakPtr().expired(), true);

	// functions broken:
	//		const SharedPtrType& getParentSharedPtr(void) const;
	//const progenitor_t::SharedPtrType otherPtr(progeny1.getParentSharedPtr());
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
