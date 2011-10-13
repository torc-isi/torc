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
/// \brief Source for the Virtex2PrimitiveStructure class.

#include "torc/packer/Virtex2PrimitiveStructure.hpp"
#include <iostream>

namespace torc {
namespace packer {

	using namespace torc::architecture;
	/// \brief The type name of slice primitives
	const std::string cSliceTypeName = "SLICE";
	/// \brief The name of the BYINVOUTUSED slice element
	const std::string cByInvOutUsedElementName = "BYINVOUTUSED";

	void Virtex2PrimitiveStructure::initialize(void) {
		// look up the SLICE BYINVOUTUSED element
		if(mPrimitiveDefPtr->getName() == cSliceTypeName) {
			// look for the BYINVOUTUSED element
			const PrimitiveElementArray& elements = mPrimitiveDefPtr->getElements();
			PrimitiveElementArray::const_iterator ep = elements.begin();
			PrimitiveElementArray::const_iterator ee = elements.end();
			while(ep < ee) {
				const PrimitiveElement& element = *ep++;
				if(element.getName() != cByInvOutUsedElementName) continue;
				// look for the one and only input pin, and add it to the inverted inputs set
				PrimitiveElementPinArray elementPins = element.getPins();
				PrimitiveElementPinArray::const_iterator pp = elementPins.begin();
				PrimitiveElementPinArray::const_iterator pe = elementPins.end();
				while(pp < pe) { if(pp->isInput()) mInvertedInputs.insert(&*pp); pp++; }
			}
		}

// oops: I need to structure things differently, because the base class has already invoked this
//		// proceed with the base class functionality
//		PrimitiveStructure::initialize();
	}

} // namespace architecture
} // namespace torc
