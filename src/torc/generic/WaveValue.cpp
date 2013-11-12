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

#include "torc/generic/WaveValue.hpp"
#include "torc/generic/LogicElement.hpp"
#include "torc/generic/Simulate.hpp"

namespace torc {
namespace generic {

/**
 * Create a WaveValue.
 *
 * @param[in] inName Name of the WaveValue to be created.
 * @param[in] inDeltaTimeDuration Value Delta time duration 
 *            between the values contained in the logic waveform.
 * @param[in] inLogicWaveForm Logic wave form for this WaveValue.
 * @param[in] inSimulate Pointer to parented (Simulate) object [optional].
 *            If not mentioned WaveValue will not be added to simulate.
 *
 * @return Pointer to created WaveValue.
 */
WaveValueSharedPtr WaveValue::Factory::newWaveValuePtr(const std::string& inName,
	const Value& inDeltaTimeDuration, const LogicElementSharedPtr inLogicWaveForm,
	const SimulateSharedPtr& inSimulate) throw (Error) {
	try {
		WaveValueSharedPtr newWaveValue;
		create(newWaveValue);
		newWaveValue->setName(inName);
		newWaveValue->setDeltaTimeDuration(inDeltaTimeDuration);
		newWaveValue->setLogicWaveform(inLogicWaveForm);
		if(inSimulate) {
			inSimulate->addWaveValue(newWaveValue);
		}
		return newWaveValue;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

WaveValue::WaveValue() : Nameable(), SelfReferencing<WaveValue>(), mDeltaTimeDuration(),
	mLogicWaveform() {}

WaveValue::~WaveValue() throw () {}

void WaveValue::accept(BaseVisitor& inoutVisitor) throw (Error) {
	try {
		runVisitor(*this, inoutVisitor);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

/**
 * Set the delta time duration between the values contained in the logic waveform.
 * @param[in] Value Delta time duration between the values contained in the logic waveform.
 */
void WaveValue::setDeltaTimeDuration(const Value& value) {
	mDeltaTimeDuration = value;
}

/**
 * Set the logic waveform.
 *
 * @param[in] inSource Pointer to logic element object.
 */
void WaveValue::setLogicWaveform(const LogicElementSharedPtr& inSource) {
	mLogicWaveform = inSource;
}

} // namespace generic
} // namespace torc
