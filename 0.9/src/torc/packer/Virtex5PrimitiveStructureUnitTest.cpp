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
/// \brief Unit test for the Virtex2PrimitiveStructure class.

#include <boost/test/unit_test.hpp>
#include "torc/packer/Virtex5PrimitiveStructure.hpp"
#include "torc/architecture/DDB.hpp"
#include <iostream>

namespace torc {
namespace packer {

BOOST_AUTO_TEST_SUITE(packer)

/// \brief Unit test for the Virtex5PrimitiveStructure class.
BOOST_AUTO_TEST_CASE(Virtex5PrimitiveStructureUnitTest) {

	using namespace torc::architecture;
	// open and initialize a database
//	DDB ddb("xc2vp4");
	DDB ddb("xc5vlx30");
	// look up the logic sites in the device
	const Sites& sites = ddb.getSites();
	// define a vector of PrimitiveStructure objects
	typedef std::map<std::string, PrimitiveStructureSharedPtr> PrimitiveStructuresSharedPtrMap;
	PrimitiveStructuresSharedPtrMap primitiveStructures;
	// look up the primitive def types
	typedef const Array<const PrimitiveDef> PrimitiveDefArray;
	PrimitiveDefArray& primitiveDefs = sites.getSiteTypes();
	PrimitiveDefArray::const_iterator p = primitiveDefs.begin();
	PrimitiveDefArray::const_iterator e = primitiveDefs.end();
	while(p < e) {
		// create a PrimitiveStructure object for this PrimitiveDef
		PrimitiveStructureSharedPtr primitiveStructurePtr(new Virtex5PrimitiveStructure(&*p++));
		const torc::architecture::PrimitiveDef* primitiveDefPtr 
			= primitiveStructurePtr->getPrimitiveDefPtr();
		const std::string& primitiveDefName = primitiveDefPtr->getName();
		// insert the PrimitiveStructure into the map
		primitiveStructures[primitiveDefName] = primitiveStructurePtr;
	}

	// iterate through the PrimitiveStructure entries in the map
	// (just to make sure we can read out what we put in)
	PrimitiveStructuresSharedPtrMap::iterator psp = primitiveStructures.begin();
	PrimitiveStructuresSharedPtrMap::iterator pse = primitiveStructures.end();
	while(psp != pse) {
		// look up the PrimitiveDef name and the PrimitiveStructure object
		std::string primitiveDefName = psp->first;
		PrimitiveStructureSharedPtr primitiveStructurePtr = psp->second;
		(void) primitiveStructurePtr;
		//const torc::architecture::PrimitiveDef* primitiveDefPtr 
		//	= primitiveStructurePtr->getPrimitiveDefPtr();
	//	std::cout << "PrimitiveDef: " << primitiveDefName << std::endl;
		psp++;
	}

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
