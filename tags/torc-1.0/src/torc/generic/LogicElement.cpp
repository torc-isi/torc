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

#include "torc/generic/LogicElement.hpp"
#include "torc/generic/LogicValue.hpp"

namespace torc {
namespace generic {

/**
 * Create a logic element.
 *
 * @param[in] inType Type of the logic value.
 * @param[in] inLogicValue Pointer to the logic value [optional], 
 *            needed for single logic element creation.  
 * @param[in] inParentLogicElement Pointer to parent logic element [optional].
 *
 * @return Pointer to created permutable.
 */
LogicElementSharedPtr LogicElement::Factory::newLogicElementPtr(const LogicElement::Type& inType,
	const LogicValueSharedPtr& inLogicValue, const LogicElementSharedPtr& inParentLogicElement)
		throw (Error) {
	try {
		LogicElementSharedPtr newLogicElement;
		create(newLogicElement);
		newLogicElement->setType(inType);
		if(inLogicValue) {
			newLogicElement->setName(inLogicValue->getName());
		}
		if(inParentLogicElement) {
			inParentLogicElement->addChildLogicElement(newLogicElement);
		}
		return newLogicElement;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

/**
 * Set the Logic element type
 *
 * @param[in] inSource Logic element type
 */
void LogicElement::setType(const Type& inSource) {
	mType = inSource;
}

/**
 * Set the nested logic elements.
 *
 * @param[in] inSource Vector containing logic elements.
 * @exception Error Could not add child logic elements because
 * pointer to the logic elements does not exist
 */
void LogicElement::setChildren(const std::vector<LogicElementSharedPtr>& inSource) {
	std::vector<LogicElementSharedPtr>::const_iterator entry = inSource.begin();
	std::vector<LogicElementSharedPtr>::const_iterator end = inSource.end();
	for(; entry != end; ++entry) {
		try {
			addChildLogicElement(*entry);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

/**
 * Add a logic element to parent logic element
 *
 * @param[in] inChildLogicElement Child logic element to be added to parent
 */
bool LogicElement::addChildLogicElement(const LogicElementSharedPtr& inChildLogicElement) {
	if(!inChildLogicElement) {
		Error e(eMessageIdErrorPointerToItemDoesNotExist, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Pointer to the logic element object does not exist",
			inChildLogicElement);
		throw e;
	} else {
		mChildren.push_back(inChildLogicElement);
		return true;
	}
}

void LogicElement::accept(BaseVisitor& visitor) throw (Error) {
	try {
		runVisitor(*this, visitor);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

/**
 * Set the relation type.
 *
 * @param[in] inSource RelationType
 */
void LogicElement::setRelationType(const RelationType& inSource) {
	mRelationType = inSource;
}

/**
 * Get the total number of bits of the composition
 * @return Number of bits
 */
size_t LogicElement::getSize() const {
	size_t size = 0;
	size_t pSize = 0;
	size_t cSize = 0;
	LogicElement::RelationType rType = getRelationType();

	std::vector<LogicElementSharedPtr> outLogicElements;
	getChildren(outLogicElements);

	std::vector<LogicElementSharedPtr>::iterator logicElemIt = outLogicElements.begin();
	if(LogicElement::eTypeSingle == getType() || LogicElement::eTypeIgnored == getType()) {
		return 1;
	}

	if(LogicElement::eTypeOneOf == getType()) {
		if(!outLogicElements.empty()) {
			size += (*logicElemIt)->getSize();
		} else {
			size = 0;
		}
		return size;
	}

	if(LogicElement::eTypeWaveForm == getType()) {
		if(!outLogicElements.empty()) {
			size += (*logicElemIt)->getSize();
		} else {
			size = 0;
		}
		return size;
	}

	if(LogicElement::eRelationTypeParent == rType) {
		if(!outLogicElements.empty()) {
			pSize += (*logicElemIt)->getSize();
		} else {
			pSize = 0;
		}
	} else {
		if(!outLogicElements.empty()) {
			for(; logicElemIt != outLogicElements.end(); logicElemIt++) {
				cSize += (*logicElemIt)->getSize();
			}
		}
	}
	size = pSize + cSize;
	return size;
}

LogicElement::LogicElement() : LogicValue(), mType(), mRelationType(), mChildren() {}

LogicElement::~LogicElement() throw () {}

} // namespace generic
} // namespace torc
