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
/// \brief Source for the Array unit test.

#include <boost/test/unit_test.hpp>
#define DEBUG // test the debug functionality as well
#include "torc/architecture/Array.hpp"
#include "torc/architecture/Tilewire.hpp"
#include <iostream>

namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

using namespace torc::architecture::xilinx;

/// \brief Unit test for the Array class.
BOOST_AUTO_TEST_CASE(ArrayUnitTest) {
	// functions not tested:
	//		~Array<T>(void);

	// functions tested:
	//		Array<T>(void);
	//		Array<T>(uint32_t inSize);
	//		void allocate(uint32_t inSize);
	//		void deallocate(void);
	uint32_t size5 = 5;
	uint32_t size3 = 3;
	typedef Array<int> int_array_t;
	typedef Array<Tilewire> tilewire_array_t;
	int_array_t intArray;
	tilewire_array_t tilewireArray(size3);

	// macros tested:
	//		enforceBounds(index)
	bool boundsEnforced = false;
	try {
		// if bounds checking is enabled and correct, we won't be able to read index 3
		Tilewire outOfRange = tilewireArray[size3];
	} catch(std::out_of_range oor) {
		std::cerr << "Bounds checking generated message \"" << oor.what() << "\"" << std::endl;
		boundsEnforced = true;
	}
	BOOST_CHECK(boundsEnforced);

	// functions tested:
	//		inline int getSize(void) const;
	//		inline void setSize(uint32_t inSize);
	BOOST_CHECK_EQUAL(intArray.getSize(), 0u);
	BOOST_CHECK_EQUAL(tilewireArray.getSize(), size3);
	intArray.setSize(size5);
	BOOST_CHECK_EQUAL(intArray.getSize(), size5);

	// functions tested:
	//		T& operator [](uint32_t inIndex);
	//		T* begin(void);
	//		T* end(void);
	intArray[0] = 0;
	intArray[1] = 1;
	intArray[2] = 2;
	intArray[3] = 3;
	intArray[4] = 4;
	int_array_t::iterator ip = intArray.begin();
	int_array_t::iterator ie = intArray.end();
	BOOST_CHECK(*ip++ == 0);
	BOOST_CHECK(*ip++ == 1);
	BOOST_CHECK(*ip++ == 2);
	BOOST_CHECK(*ip++ == 3);
	BOOST_CHECK(*ip++ == 4);
	BOOST_CHECK_EQUAL(ip, ie);

	// functions tested:
	//		const T& operator [](uint32_t inIndex) const;
	//		const T* begin(void) const;
	//		const T* end(void) const;
	Tilewire tilewire1(TileIndex(1), WireIndex(1));
	Tilewire tilewire2(TileIndex(1), WireIndex(2));
	Tilewire tilewire3(TileIndex(4), WireIndex(9));
	tilewireArray[0] = tilewire1;
	tilewireArray[1] = tilewire2;
	tilewireArray[2] = tilewire3;
	tilewire_array_t::const_iterator tp = tilewireArray.begin();
	tilewire_array_t::const_iterator te = tilewireArray.end();
	BOOST_CHECK(*tp++ == tilewireArray[0]);
	BOOST_CHECK(*tp++ == tilewireArray[1]);
	BOOST_CHECK(*tp++ == tilewireArray[2]);
	BOOST_CHECK_EQUAL(tp, te);

	// template tested:
	//		template <class T> class Array2D;
	int i = 1;
	typedef Array2D<int> int_array2d_t;
	int_array2d_t intArray2d(2);
	intArray2d[0].setSize(3);
	intArray2d[1].setSize(1);
	intArray2d[0][0] = i++;
	intArray2d[0][1] = i++;
	intArray2d[0][2] = i++;
	intArray2d[1][0] = i++;
	Array<Array<int> >::const_iterator l1p = intArray2d.begin();
	int_array2d_t::const_iterator l1e = intArray2d.end();
	i = 1;
	while(l1p < l1e) {
		int_array_t::const_iterator l2p = l1p->begin();
		int_array_t::const_iterator l2e = l1p->end();
		while(l2p < l2e) {
			BOOST_CHECK_EQUAL(*l2p++, i++);
		}
		l1p++;
	}
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
