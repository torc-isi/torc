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

#include "torc/generic/SimulationInfo.hpp"
#include "torc/generic/LogicValue.hpp"
#include "torc/generic/Library.hpp"

namespace torc {
namespace generic {

/**
 * Create a simulation info.
 *
 * @param[in] inLogicValues Vector of logic values to be appended to
 * @param[in] inLibraryPtr Pointer to parented(Library) object.
 *
 * @return Pointer to simulation info.
 **/
SimulationInfoSharedPtr SimulationInfo::Factory::newSimulationInfoPtr(
	const std::vector<LogicValueSharedPtr>& inLogicValues, const LibrarySharedPtr& inLibraryPtr)
		throw (Error) {
	try {
		SimulationInfoSharedPtr newSimulationInfo;
		create(newSimulationInfo);
		newSimulationInfo->setLogicValues(inLogicValues);
		inLibraryPtr->setSimulationInfo(newSimulationInfo);
		return newSimulationInfo;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

void SimulationInfo::accept(BaseVisitor& inoutVisitor) throw (Error) {
	try {
		runVisitor(*this, inoutVisitor);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

/**
 * Set the list of logic values.
 *
 * @param[in] inLogicValues Vector of logic values to be appended to
 */
void SimulationInfo::setLogicValues(const std::vector<LogicValueSharedPtr>& inLogicValues)
	throw (Error) {
	std::vector<LogicValueSharedPtr>::const_iterator it = inLogicValues.begin();
	for(; it != inLogicValues.end(); it++) {
		try {
			addLogicValue(*it);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

/**
 * Add a logic value to the list of logic values. If an empty pointer is supplied, 
 * it returns without doing anything.
 * 
 * @param[in] inLogicValue A pointer to a logic value object.
 * 
 * @exception Error Logic value could not be added.
 */
void SimulationInfo::addLogicValue(const LogicValueSharedPtr& inLogicValue) throw (Error) {
	if(!inLogicValue) {
		return;
	}
	std::string name = inLogicValue->getName();
	if(name.empty()) {
		Error e(eMessageIdErrorEmptyItemName, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Empty logic value name ", name);
		throw e;
	}
	if(false == mLogicValueSymTab.set(name, inLogicValue)) {
		Error e(eMessageIdErrorItemAlreadyExists, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Logic value name", name);
		throw e;
	}
	inLogicValue->setParent(getSharedThis());
}

/**
 * Find a logic value by name, in the list of logic values in simulationInfo.
 * 
 * @param[in] name String value specifying the name of the logic value.
 * 
 * @return A pointer to the logic value if found, an empty pointer otherwise.
 */
LogicValueSharedPtr SimulationInfo::findLogicValue(const std::string& inName) {
	if(inName.empty()) {
		Error e(eMessageIdErrorEmptyItemName, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Empty logic value name", inName);
		throw e;
	}
	LogicValueSharedPtr logicValue;
	mLogicValueSymTab.get(inName, logicValue);
	return logicValue;
}

/**
 * Remove the specified logic value from the list of logic values. 
 * If an empty pointer is passed, it returns without doing anything
 * 
 * @param[in] inName name of the object to be removed
 * 
 * @exception Error Logic Value not preset in collection.
 */
void SimulationInfo::removeLogicValue(const std::string& inName) throw (Error) {
	if(inName.empty()) {
		Error e(eMessageIdErrorEmptyItemName, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Logic value name", inName);
		throw e;
	}
	if(false == mLogicValueSymTab.remove(inName)) {
		Error e(eMessageIdErrorItemNotFound, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Logic value name", inName);
		throw e;
	}
	return;
}

SimulationInfo::SimulationInfo() : Commentable(), Visitable(), ParentedObject<Library>(),
	SelfReferencing<SimulationInfo>(), UserDataContainer(), mLogicValueSymTab() {}

SimulationInfo::~SimulationInfo() throw () {}

} // namespace generic
} // namespace torc
