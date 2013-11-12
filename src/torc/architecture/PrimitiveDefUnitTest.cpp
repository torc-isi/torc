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
/// \brief Unit test for the PrimitiveDef class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/PrimitiveDef.hpp"

namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(regression)
BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the PrimitiveDef class.
BOOST_AUTO_TEST_CASE(PrimitiveDefUnitTest) {
	// elements not tested:
	//		std::string mName;
	//		PrimitivePinArray mPins;
	//		PrimitiveElementArray mElements;
	//		PrimitiveConnSharedPtrArray mConnections;
	//		PinNameToPinIndexMap mPinNameToPinIndex;
	// functions not tested:
	//		PrimitivePinArray& getPins(void);
	//		PrimitiveDef(void);
	//		PinIndex findPinIndexByName(const std::string& inName) const;
	//		const string& getName(void) const;
	//		const PrimitivePinArray& getPins(void) const;
	//		const PrimitiveElementArray& getElements(void) const;
	//		const PrimitiveConnSharedPtrArray& getConnections(void) const;
	/// \todo Write a unit test for torc::architecture::PrimitiveDef.
	BOOST_REQUIRE(false);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
