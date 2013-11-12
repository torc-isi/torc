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

#include "torc/generic/Simulate.hpp"
#include "torc/generic/WaveValue.hpp"
#include "torc/generic/Apply.hpp"
#include "torc/generic/InterfaceAttributes.hpp"
#include "torc/generic/View.hpp"

namespace torc {
namespace generic {

/**
 * Create a simulate.
 *
 * @param[in] inName Name of the simulate to be created.
 * @param[in] inPortListAliases Vector of port list aliases to this simulate.
 * @param[in] inAllApply Vector of apply to this simulate.
 * @param[in] inWaveValues Vector of WaveValues to this simulate.
 * @param[in] inView Pointer to parented (view) object.
 * @param[in] inOriginalName Original name of the simulate [optional].
 * @param[in] inInterfaceAttributes Pointer to
 *  parented (InterfaceAttributes) object [optional]. If mentioned then this
 *  will decompile within (contents ...) construct.
 *
 * @return Pointer to created simulate.
 */
SimulateSharedPtr Simulate::Factory::newSimulatePtr(const std::string& inName,
	const std::vector<PortListAliasSharedPtr>& inPortListAliases,
	const std::vector<ApplySharedPtr>& inAllApply,
	const std::vector<WaveValueSharedPtr>& inWaveValues, const ViewSharedPtr& inView,
	const std::string& inOriginalName, const InterfaceAttributesSharedPtr& inInterfaceAttributes)
		throw (Error) {
	try {
		SimulateSharedPtr newSimulate;
		create(newSimulate);
		newSimulate->setName(inName);
		newSimulate->setPortListAliases(inPortListAliases);
		newSimulate->setAllApply(inAllApply);
		newSimulate->setWaveValues(inWaveValues);
		newSimulate->setOriginalName(inOriginalName);
		if(inInterfaceAttributes) {
			inInterfaceAttributes->setSimulate(newSimulate);
			inView->setInterfaceAttributes(inInterfaceAttributes);
		} else {
			inView->setSimulate(newSimulate);
		}
		return newSimulate;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

void Simulate::accept(BaseVisitor& inoutVisitor) throw (Error) {
	try {
		runVisitor(*this, inoutVisitor);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

/**
 * Add a port list alias to the symbol table of port list alias.
 * If an empty pointer is supplied, it returns without doing anything.
 *
 * @param[in] inPortListAlias A pointer to a port list alias object.
 *
 * @exception Error PortListAlias could not be added, empty PortListAlias name
 * @exception Error PortListAlias already exists in simulate
 */
void Simulate::addPortListAlias(const PortListAliasSharedPtr& inPortListAlias) throw (Error) {
	if(!inPortListAlias) {
		return;
	}
	std::string name = inPortListAlias->getName();
	if(name.empty()) {
		Error e(eMessageIdErrorEmptyItemName, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Empty port list alias name ", name);
		throw e;
	}
	if(false == mPortListAliasSymTab.set(name, inPortListAlias)) {
		Error e(eMessageIdErrorItemAlreadyExists, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Port list alias name", name);
		throw e;
	}
}

/**
 * Set all the PortListAlias of this simulation.
 *
 * @param[in] inPortListAliases Vector of PortListAlias to be appended to
 */
void Simulate::setPortListAliases(const std::vector<PortListAliasSharedPtr>& inPortListAliases)
	throw (Error) {
	std::vector<PortListAliasSharedPtr>::const_iterator it = inPortListAliases.begin();
	for(; it != inPortListAliases.end(); it++) {
		try {
			addPortListAlias(*it);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

/**
 * Find a PortListAlias by name, in the symbol table of port list alias.
 *
 * @param[in] inName String specifying the name of the PortListAlias.
 *
 * @return A pointer to the PortListAlias if found, an empty pointer otherwise.
 */
PortListAliasSharedPtr Simulate::findPortListAlias(const std::string& inName) {
	if(inName.empty()) {
		Error e(eMessageIdErrorEmptyItemName, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Empty PortListAlias name", inName);
		throw e;
	}
	PortListAliasSharedPtr portListAlias;
	mPortListAliasSymTab.get(inName, portListAlias);
	return portListAlias;
}

/**
 * Add an apply to the vector of Apply objects.
 *
 * @param[in] inSource Pointer to Apply object to be appended to
 */
void Simulate::addApply(const ApplySharedPtr& inSource) throw (Error) {
	try {
		mAllApply.push_back(inSource);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

/**
 * Set all the apply objects.
 *
 * @param[in] inAllApply Vector of apply objects to be appended to
 */
void Simulate::setAllApply(const std::vector<ApplySharedPtr>& inAllApply) throw (Error) {
	std::vector<ApplySharedPtr>::const_iterator it = inAllApply.begin();
	for(; it != inAllApply.end(); it++) {
		try {
			addApply(*it);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

/**
 * Set the vector of wave values.
 *
 * @param[in] inWaveValues Vector of wave values to be appended to
 */
void Simulate::setWaveValues(const std::vector<WaveValueSharedPtr>& inWaveValues) throw (Error) {
	std::vector<WaveValueSharedPtr>::const_iterator it = inWaveValues.begin();
	for(; it != inWaveValues.end(); it++) {
		try {
			addWaveValue(*it);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

/**     
 * A    dd a wave value to the symbol table of wave values.
 *
 * @param[in] inSource Pointer to WaveValue to be appended to
 *
 * @exception Error WaveValue could not be added, empty WaveValue name
 * @exception Error WaveValue already exists in simulate
 */
void Simulate::addWaveValue(const WaveValueSharedPtr& inSource) throw (Error) {
	if(!inSource) {
		return;
	}
	std::string name = inSource->getName();
	if(name.empty()) {
		Error e(eMessageIdErrorEmptyItemName, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Empty WaveValue name ", name);
		throw e;
	}
	if(false == mWaveValueSymTab.set(name, inSource)) {
		Error e(eMessageIdErrorItemAlreadyExists, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("WaveValue name", name);
		throw e;
	}
}

/**
 * Find a WaveValue by name, in the symbol table of port list alias.
 *
 * @param[in] inName String specifying the name of the WaveValue.
 *
 * @return A pointer to the WaveValue if found, an empty pointer otherwise.
 */
WaveValueSharedPtr Simulate::findWaveValue(const std::string& inName) {
	if(inName.empty()) {
		Error e(eMessageIdErrorEmptyItemName, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Empty WaveValue name", inName);
		throw e;
	}
	WaveValueSharedPtr waveValue;
	mWaveValueSymTab.get(inName, waveValue);
	return waveValue;
}

Simulate::Simulate() : Nameable(), Renamable(), Commentable(), Visitable(),
	SelfReferencing<Simulate>(), UserDataContainer(), mPortListAliasSymTab(), mWaveValueSymTab(),
	mAllApply() {}

Simulate::~Simulate() throw () {}

} // namespace generic
} // namespace torc
