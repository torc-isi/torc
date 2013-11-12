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

#include "torc/generic/Apply.hpp"
#include "torc/generic/LogicalResponse.hpp"
#include "torc/generic/Simulate.hpp"

namespace torc {
namespace generic {

/**
 * Create an apply.
 *
 * @param[in] inNoOfCycle int32_t No of cycles for this apply.
 * @param[in] inCycleDuration Value Duration of each cycle.
 * @param[in] inLogicResponces List of LogicResponces.
 * @param[in] inSimulate Pointer to parented (Simulate) object [optional].
 *            If not mentioned apply will not be added to simulate.
 *
 * @return Pointer to created apply.
 */
ApplySharedPtr Apply::Factory::newApplyPtr(const int32_t& inNoOfCycle, const Value& inCycleDuration,
	const std::list<LogicalResponseSharedPtr>& inLogicResponces,
	const SimulateSharedPtr& inSimulate) throw (Error) {
	try {
		ApplySharedPtr newApply;
		create(newApply);
		newApply->setNoOfCycle(inNoOfCycle);
		newApply->setCycleDuration(inCycleDuration);
		newApply->setLogicResponses(inLogicResponces);
		if(inSimulate) {
			inSimulate->addApply(newApply);
		}
		return newApply;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

void Apply::accept(BaseVisitor& inoutVisitor) throw (Error) {
	try {
		runVisitor(*this, inoutVisitor);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

/**
 * Set the no of cycles
 * @param[in] inValue int32_t No of cycles
 */
void Apply::setNoOfCycle(const int32_t& inValue) {
	mNoOfCycle = inValue;
}

/**
 * Set the duration of each cycle
 * @param[in] value Duration of each cycle
 */
void Apply::setCycleDuration(const Value& value) {
	mCycleDuration = value;
}

/**
 * Set the list of LogicResponce.
 *
 * @param[in] inLogicResponces List of LogicResponce (s) to be appended to
 */
void Apply::setLogicResponses(const std::list<LogicalResponseSharedPtr>& inLogicResponces) {
	std::list<LogicalResponseSharedPtr>::const_iterator logicalResponse = inLogicResponces.begin();
	std::list<LogicalResponseSharedPtr>::const_iterator end = inLogicResponces.end();
	for(; logicalResponse != end; logicalResponse++) {
		try {
			addLogicResponse(*logicalResponse);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

/**
 * Add a LogicResponce to the list of LogicResponces.
 *
 * @param[in] inLogicResponce Pointer to LogicResponce to be appended to
 */
void Apply::addLogicResponse(const LogicalResponseSharedPtr& inLogicResponce) {
	try {
		mLogicResponses.push_back(inLogicResponce);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

Apply::Apply() : Commentable(), Visitable(), SelfReferencing<Apply>(), UserDataContainer(),
	mNoOfCycle(0), mCycleDuration(), mLogicResponses() {}

Apply::~Apply() throw () {}

} // namespace generic
} // namespace torc
