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
/// \brief Source for the SiteTypes unit test.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/Sites.hpp"
#include "torc/architecture/DDB.hpp"


namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the SiteTypes class.
BOOST_AUTO_TEST_CASE(SiteTypesUnitTest) {

        // open and initialize a database
        DDB ddb("xc5vlx30");
        // look up the logic sites in the device
        const Sites& sites = ddb.getSites();
        // look up the primitive def types
        typedef const Array<const Sites::PrimitiveDef> PrimitiveDefArray;
        PrimitiveDefArray& primitiveDefs = sites.getSiteTypes();
        PrimitiveDefArray::const_iterator p = primitiveDefs.begin();
        PrimitiveDefArray::const_iterator e = primitiveDefs.end();
        int i = 0;
        // iterate over the primitive defs
	while(p < e) {
		// look up the current primitive def
		const Sites::PrimitiveDef& primitiveDef = *p++;
		std::cout << i << ": " << primitiveDef.getName() << std::endl;
		typedef const Array<const Sites::SitePin> SitePinArray;
		// look up the primitive def pins
		const SitePinArray& sitePins = primitiveDef.getPins();
		SitePinArray::const_iterator sp = sitePins.begin();
		SitePinArray::const_iterator se = sitePins.end();
		// iterate over the primitive def pins
		if(false && sitePins.getSize() > 0) {
			std::cout << "    ";
			// output each of the pin names along with the flags (2-INPUT, 4-OUTPUT)
			while(sp < se) {
				const Sites::SitePin& sitePin = *sp++;
				std::cout << sitePin.getName() << " ";
				std::cout << sitePin.getFlags() << " ";
			}
			std::cout << std::endl;
		}
		// look up the primitive def elements
		typedef Array<const Sites::PrimitiveElement> PrimitiveElementArray;
		const PrimitiveElementArray& elements = primitiveDef.getElements();
		PrimitiveElementArray::const_iterator ep = elements.begin();
		PrimitiveElementArray::const_iterator ee = elements.end();
		// iterate over the primitive def elements
		if(true && elements.getSize() > 0) {
			// output each of the element names and if the pin is a basic element
			while(ep < ee) {
				const Sites::PrimitiveElement& element = *ep++;
				std::cout << "ELEMENT NAME:" << element.getName() << " "<< std::endl;
				//                                std::cout << "    ";
				//                                std::cout<< "ELEMENT IS Basic ELement:" << element.isBel() << " " << std::endl;
				typedef const Array<const Sites::ElementPin> ElementPinArray;
				const ElementPinArray& elementPins = element.getPins();
				ElementPinArray::const_iterator pp = elementPins.begin();
				ElementPinArray::const_iterator pe = elementPins.end();
				// iterate over the element pins
				if(elementPins.getSize()> 0) {
					
					// output each of the element pin names along with the element flags (2-Input, 4-Output)
					while(pp < pe) {
					//	std::cout << "    ";
						const Sites::ElementPin& elementPin = *pp++;
						std::cout << elementPin.getElementPtr()->getName() << "." << elementPin.getName() << " ";
//						std::cout << ((elementPin.getElement()->getName() == element.getName()) ? "" : (elementPin.getElement()->getName() + " "));
						//                                                std::cout<< "ELEMENT PIN NAME:" << elementPin.getName() << " " << std::endl;
						//                                                std::cout << "    ";
						//                                                std::cout<< "ELEMENT FLAG NAME:" << elementPin.getFlags() << " " << std::endl;
					//	typedef std::set<std::string> StringSet;
					//	const StringSet elementCfgs = element.getCfgs();
					//	StringSet::const_iterator cp = elementCfgs.begin();
					//	StringSet::const_iterator ce = elementCfgs.end();
					//	// iterate over the configs
					//	if(elementCfgs.size() != 0){
					//		// output each of the configs
					//		while(cp != ce){
					//			//                                                                std::cout << "    ";
					//			//                                                                std::cout << "CONFIG NAME: " << *cp << " " << std::endl;
					//			*cp++;
					//		}
					//	}
					}
					std::cout << std::endl;
				}
			}
			std::cout << std::endl;
		}
		const Sites::PrimitiveDef::PrimitiveConnArray& connections = primitiveDef.getConnections();
		Sites::PrimitiveDef::PrimitiveConnArray::const_iterator cp = connections.begin();
		Sites::PrimitiveDef::PrimitiveConnArray::const_iterator ce = connections.end();
		// iterate over the primitive element connections
		while(true && cp < ce) {
			typedef std::vector<const Sites::ElementPin*> ElementPinPtrVector;
			const Sites::PrimitiveConn& connection = *cp++;
			const Sites::ElementPin& source = *connection.getSourcePtr();
			const ElementPinPtrVector& sinks = connection.getSinks();
			std::cout << source.getElementPtr()->getName() << "." << source.getName() << " ==> ";
			ElementPinPtrVector::const_iterator pp = sinks.begin();
			ElementPinPtrVector::const_iterator pe = sinks.end();
			while(pp < pe) {
				const Sites::ElementPin& sink = **pp++;
				std::cout << sink.getElementPtr()->getName() << "." << sink.getName() << " ";
			}
			std::cout << std::endl;
		}
		i++;
	}
	BOOST_REQUIRE(true);
	
}
	
BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc

