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
/// \brief Source for the Virtex7PrimitiveStructure class.

#include "torc/packer/Virtex7PrimitiveStructure.hpp"
#include <iostream>
#include <map>

namespace torc {
namespace packer {

	using namespace torc::architecture;
	/// \brief The type name of slice primitives
	const std::string cSliceLTypeName = "SLICEL";
	const std::string cSliceMTypeName = "SLICEM";

	void Virtex7PrimitiveStructure::initialize(void) {
		if(mPrimitiveDefPtr->getName() == cSliceLTypeName 
			|| mPrimitiveDefPtr->getName() == cSliceMTypeName) {
			
			const PrimitiveElementArray& elements = mPrimitiveDefPtr->getElements();
			PrimitiveElementArray::const_iterator ep = elements.begin();
			PrimitiveElementArray::const_iterator ee = elements.end();
			while(ep < ee) {
				const PrimitiveElement& element = *ep++;
				const PrimitiveElement* elementPtr = &element;
				if(element.getName() == "SYNC_ATTR"){
					mPrincipalsToOrphans["AFF"].push_back(elementPtr);
					mPrincipalsToOrphans["BFF"].push_back(elementPtr);
					mPrincipalsToOrphans["CFF"].push_back(elementPtr);
					mPrincipalsToOrphans["DFF"].push_back(elementPtr);
					mPrincipalsToOrphans["A5FF"].push_back(elementPtr);
					mPrincipalsToOrphans["B5FF"].push_back(elementPtr);
					mPrincipalsToOrphans["C5FF"].push_back(elementPtr);
					mPrincipalsToOrphans["D5FF"].push_back(elementPtr);
				} else if(element.getName() == "AFFSR" || element.getName() == "AFFINIT"){
					mPrincipalsToOrphans["AFF"].push_back(elementPtr);
				} else if(element.getName() == "BFFSR" || element.getName() == "AFFINIT"){
					mPrincipalsToOrphans["BFF"].push_back(elementPtr);
				} else if(element.getName() == "CFFSR" || element.getName() == "AFFINIT"){
					mPrincipalsToOrphans["CFF"].push_back(elementPtr);
				} else if(element.getName() == "DFFSR" || element.getName() == "AFFINIT"){
					mPrincipalsToOrphans["DFF"].push_back(elementPtr);
				} else if(element.getName() == "A5FFSR" || element.getName() == "A5FFINIT"){
					mPrincipalsToOrphans["A5FF"].push_back(elementPtr);
				} else if(element.getName() == "B5FFSR" || element.getName() == "B5FFINIT"){
					mPrincipalsToOrphans["B5FF"].push_back(elementPtr);
				} else if(element.getName() == "C5FFSR" || element.getName() == "C5FFINIT"){
					mPrincipalsToOrphans["C5FF"].push_back(elementPtr);
				} else if(element.getName() == "D5FFSR" || element.getName() == "D5FFINIT"){
					mPrincipalsToOrphans["D5FF"].push_back(elementPtr);
				} else if(element.getName() == "A5RAMMODE"){
					mPrincipalsToOrphans["A5LUT"].push_back(elementPtr);
				} else if(element.getName() == "B5RAMMODE"){
					mPrincipalsToOrphans["B5LUT"].push_back(elementPtr);
				} else if(element.getName() == "C5RAMMODE"){
					mPrincipalsToOrphans["C5LUT"].push_back(elementPtr);
				} else if(element.getName() == "D5RAMMODE"){
					mPrincipalsToOrphans["D5LUT"].push_back(elementPtr);
				} else if(element.getName() == "A6RAMMODE"){
					mPrincipalsToOrphans["A6LUT"].push_back(elementPtr);
				} else if(element.getName() == "B6RAMMODE"){
					mPrincipalsToOrphans["B6LUT"].push_back(elementPtr);
				} else if(element.getName() == "C6RAMMODE"){
					mPrincipalsToOrphans["C6LUT"].push_back(elementPtr);
				} else if(element.getName() == "D6RAMMODE"){
					mPrincipalsToOrphans["D6LUT"].push_back(elementPtr);
				}
			}
		}
	}

} // namespace architecture
} // namespace torc
