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
/// \brief Source for the PrimitiveStructure class.

#include "torc/packer/PrimitiveStructure.hpp"
#include <iostream>

namespace torc {
namespace packer {
	
	using namespace torc::architecture;

	boost::regex PrimitiveStructure::sRoutethroughRegEx("^_ROUTETHROUGH.*");
	boost::regex PrimitiveStructure::sPrincipalRegEx("^(PAD)$");
	boost::regex PrimitiveStructure::sLUTRegEx("^(#LUT:|#RAM:|#ROM:)?<eqn>$");
	boost::regex PrimitiveStructure::sFlopRegEx("^(#FF|#LATCH)$");
	boost::regex PrimitiveStructure::sPowerRegEx("^.*(VCC|VDD|POWER).*$");
	boost::regex PrimitiveStructure::sGroundRegEx("^.*(GND|GROUND).*$");
	boost::regex PrimitiveStructure::sInvertingInputRegEx("^.*_B$");


	bool PrimitiveStructure::isPreclassified(const PrimitiveElement& inElement) {
		return mPreclassified.find(inElement.getName()) != mPreclassified.end();
	}

	bool PrimitiveStructure::isPrincipal(const PrimitiveElement& inElement) {
		return inElement.getName() == mPrimitiveDefPtr->getName() 
			|| regex_match(inElement.getName(), sPrincipalRegEx);
	}

	bool PrimitiveStructure::isTerminal(const PrimitiveElement& inElement) {
		if(inElement.getPins().getSize() != 1) return false;
		return !mPrimitiveDefPtr->findPinIndexByName(inElement.getName()).isUndefined();
	}

	bool PrimitiveStructure::isOrphan(const PrimitiveElement& inElement) {
		return inElement.getPins().getSize() == 0;
	}

	bool PrimitiveStructure::isMux(const PrimitiveElement& inElement, bool& outIsSwitch) {
		outIsSwitch = false;
		// look up information about the primitive
		const PrimitiveElement::StringSet& cfgs = inElement.getCfgs();
		PrimitiveElement::StringSet::size_type cfgsCount = cfgs.size();
		const PrimitiveElementPinArray& elementPins = inElement.getPins();
		boost::uint32_t elementPinsCount = elementPins.getSize();

		// if the configuration set is empty, this cannot be a mux
		if(cfgsCount == 0) return false;

		// configurable muxes will have cfgsCount input pins plus one output pin
		if(elementPinsCount != cfgsCount + 1) return false;
		// copy the input pins into a set
		PrimitiveElement::StringSet inputs;
		PrimitiveElementPinArray::const_iterator pp = elementPins.begin();
		PrimitiveElementPinArray::const_iterator pe = elementPins.end();
		while(pp < pe) { if(pp->isInput()) inputs.insert(pp->getName()); pp++; }
		PrimitiveElement::StringSet::size_type inputCount = inputs.size();

		// compare the implicitly sorted cfgs and input pins sets for equality
		PrimitiveElement::StringSet::const_iterator ip = inputs.begin();
		PrimitiveElement::StringSet::const_iterator ie = inputs.end();
		PrimitiveElement::StringSet::const_iterator cp = cfgs.begin();
		while(ip != ie) if(*ip++ != *cp++) return false;

		// now that we know this is a mux, make note of all the inverting inputs
		ip = inputs.begin();
		while(ip != ie) {
			const std::string& input = *ip++;
			if(regex_match(input, sInvertingInputRegEx)) {
				torc::architecture::xilinx::PinIndex pinIndex = inElement.findPinIndexByName(input);
				mInvertedInputs.insert(&elementPins[pinIndex]);
			}
		}

		// if we got this far, the element is a mux (and it may also be a switch)
		if(inputCount == 1) outIsSwitch = true;
		return true;
	}

	bool PrimitiveStructure::isPower(const PrimitiveElement& inElement) {
		const PrimitiveElementPinArray& elementPins = inElement.getPins();
		return elementPins.getSize() == 1 && elementPins[0].getName() == "1" 
			&& regex_match(inElement.getName(), sPowerRegEx);
	}

	bool PrimitiveStructure::isGround(const PrimitiveElement& inElement) {
		const PrimitiveElementPinArray& elementPins = inElement.getPins();
		return elementPins.getSize() == 1 && elementPins[0].getName() == "0" 
			&& regex_match(inElement.getName(), sGroundRegEx);
	}

	bool PrimitiveStructure::isLUT(const PrimitiveElement& inElement, const string& inConfig) {
		return regex_match(inConfig, sLUTRegEx);
	}

	bool PrimitiveStructure::isFlop(const PrimitiveElement& inElement, const string& inConfig) {
		return regex_match(inConfig, sFlopRegEx);
	}

	bool PrimitiveStructure::isRoutethrough(const PrimitiveElement& inElement) {
		return regex_match(inElement.getName(), sRoutethroughRegEx);
	}

	void PrimitiveStructure::initialize(void) {
		// look up the primitive's pins
		const PrimitivePinArray& primitivePins = mPrimitiveDefPtr->getPins();
		const std::string& primitiveDefName = mPrimitiveDefPtr->getName();
		(void) primitivePins;
		(void) primitiveDefName;
//std::cout << primitiveDefName << ": " << primitivePins.getSize() << " pins" << std::endl;

		// iterate over the elements
		const PrimitiveElementArray& elements = mPrimitiveDefPtr->getElements();    
		PrimitiveElementArray::const_iterator ep = elements.begin();    
		PrimitiveElementArray::const_iterator ee = elements.end();
		while(ep < ee) {

			// look up the current element
			const PrimitiveElement& primitiveElement = *ep++;
			const std::string& elementName = primitiveElement.getName();

			// add the element to the comprehensive list
			mElements[elementName] = &primitiveElement;

			// skip elements that have been preclassified
			if(isPreclassified(primitiveElement)) {
				continue;
			}

			// identify principals
			if(isPrincipal(primitiveElement)) {
				mPrincipals[elementName] = &primitiveElement;
				continue;
			}

			// identify terminals
			if(isTerminal(primitiveElement)) {
				mTerminals[elementName] = &primitiveElement;
				continue;
			}

			// identify orphans
			if(isOrphan(primitiveElement)) {
				mOrphans[elementName] = &primitiveElement;
				continue;
			}

			// for all remaining elements, it's easier to track status with a flag
			bool processed = false;

			// identify muxes
			bool isSwitch = false;
			if(isMux(primitiveElement, isSwitch)) {
				// mark this element as a mux (and also as a switch if it has a single input)
				mMuxes[elementName] = &primitiveElement;
				if(isSwitch) mSwitches[elementName] = &primitiveElement;
				processed = true;
				continue;
			}

			// identify power sources
			if(isPower(primitiveElement)) {
				mPower[elementName] = &primitiveElement;
				processed = true;
				continue;
			}
			// identify ground sources
			if(isGround(primitiveElement)) {
				mGround[elementName] = &primitiveElement;
				processed = true;
				continue;
			}

			// identify LUTs and flops
			const PrimitiveElement::StringSet& cfgs = primitiveElement.getCfgs();
			PrimitiveElement::StringSet::const_iterator cp = cfgs.begin();
			PrimitiveElement::StringSet::const_iterator ce = cfgs.end();
			while(cp != ce) {
				const std::string& config = *cp++;
				// identify LUTs
				if(regex_match(config, sLUTRegEx)) {
					mLUTs[elementName] = &primitiveElement;
					processed = true;
					break;
				}
				// identify flops
				if(isFlop(primitiveElement, config)) {
					mFlops[elementName] = &primitiveElement;
					processed = true;
					break;
				}
			}
			if(processed) continue;

			// ignore routethroughs
			if(isRoutethrough(primitiveElement)) {
				mRoutethroughs[elementName] = &primitiveElement;
				processed = true;
				continue;
			}

			// note all unprocessed elements
			mUnprocessed[elementName] = &primitiveElement;
//std::cout << "    Unprocessed element " << elementName << std::endl;
debug(primitiveElement);
		}
	}

	void PrimitiveStructure::debug(const PrimitiveElement& inPrimitiveElement) {
		const PrimitiveElementPinArray& elementPins = inPrimitiveElement.getPins();
		PrimitiveElementPinArray::const_iterator pp = elementPins.begin();
		PrimitiveElementPinArray::const_iterator pe = elementPins.end();
		//std::cout << "        pins: ";
		while(pp < pe) {
			break;//std::cout << (*pp++).getName() << " ";
		}
		//std::cout << std::endl;

		const PrimitiveElement::StringSet& cfgs = inPrimitiveElement.getCfgs();
		PrimitiveElement::StringSet::const_iterator cp = cfgs.begin();
		PrimitiveElement::StringSet::const_iterator ce = cfgs.end();
		//std::cout << "        configs: ";
		while(cp != ce) {
			break;//std::cout << *cp++ << " ";
		}
		//std::cout << std::endl;
	}

} // namespace architecture
} // namespace torc
