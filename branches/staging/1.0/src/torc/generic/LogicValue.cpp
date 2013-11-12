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

#include "torc/generic/LogicValue.hpp"
#include "torc/generic/LogicValueAttributes.hpp"
#include "torc/generic/SimulationInfo.hpp"

namespace torc {
namespace generic {

/**
 * Create a LogicValue.
 *
 * @param[in] inName Name of the LogicValue to be created.
 * @param[in] inAttributes Pointer to LogicValueAttributes object.
 * @param[in] inOriginalName Original name of the LogicValue [optional].
 * @param[in] inSimulationInfo Pointer to parented(SimulationInfo) object.
 *            If not mentioned LogicValue will not be added to SimulationInfo.
 *
 * @return Pointer to created LogicValue.
 */
LogicValueSharedPtr LogicValue::Factory::newLogicValuePtr(const std::string& inName,
	const LogicValueAttributesSharedPtr& inAttributes, const std::string& inOriginalName,
	const SimulationInfoSharedPtr& inSimulationInfo) throw (Error) {
	try {
		LogicValueSharedPtr newLogicValue;
		create(newLogicValue);
		newLogicValue->setName(inName);
		newLogicValue->setAttributes(inAttributes);
		newLogicValue->setOriginalName(inOriginalName);
		if(inSimulationInfo) {
			inSimulationInfo->addLogicValue(newLogicValue);
		}
		return newLogicValue;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

/**
 * Set the attributes of the LogicValue. Attributes include voltageMap, currentMap,
 * booleanMap, compound, weak, strong, dominates etc.
 *
 * @param[in] inSource Pointer to LogicValueAttributes object.
 */
void LogicValue::setAttributes(const LogicValueAttributesSharedPtr& inSource) {
	mAttributes = inSource;
}

void LogicValue::accept(BaseVisitor& inoutVisitor) throw (Error) {
	try {
		runVisitor(*this, inoutVisitor);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

LogicValue::LogicValue() : Nameable(), Renamable(), Commentable(), Visitable(),
	ParentedObject<SimulationInfo>(), PropertyContainer(), UserDataContainer(),
	SelfReferencing<LogicValue>(), mAttributes() {}

LogicValue::~LogicValue() throw () {}

} // namespace generic
} // namespace torc
