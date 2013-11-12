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

#include "torc/generic/LogicValueAttributes.hpp"
#include <iostream>

namespace torc {
namespace generic {

/**
 * Set the logic value, which is used to indicate that the
 * current logic value is stronger than this logic value.
 * 
 * @param[in] inSource Pointer to the logic value
 */
void LogicValueAttributes::setStrongLogicValue(const LogicValueSharedPtr& inSource) {
	mStrong = inSource;
}

/**
 * Set the logic value, which is used to indicate that the
 * current logic value is weaker than this logic value.
 * 
 * @param[in] inSource Pointer to the logic value
 */
void LogicValueAttributes::setWeakLogicValue(const LogicValueSharedPtr& inSource) {
	mWeak = inSource;
}

/**
 * Set the voltage value for this logic value.
 *
 * @param[in] value Voltage value. Must be Value::MiNoMax
 */
void LogicValueAttributes::setVoltageMap(const Value::MiNoMax& value) {
	mVoltageMap = value;
	mIsVoltageMapSet = true;
}

/**
 * Set the current value for this logic value.
 *
 * @param[in] value Current value. Must be Value::MiNoMax
 */
void LogicValueAttributes::setCurrentMap(const Value::MiNoMax& value) {
	mCurrentMap = value;
	mIsCurrentMapSet = true;
}

/**
 * Set the boolean value(true/false) for this logic value.
 *
 * @param[in] value Boolean value. Must be Value::Boolean 
 */
void LogicValueAttributes::setBooleanMap(const Value::Boolean& value) {
	mBooleanMap = value;
	mIsBooleanMapSet = true;
}

/**
 * Set the list of compound logic values for this logic value.
 *
 * @param[in] inSource List of logic values to be appended to
 */
void LogicValueAttributes::setCompoundLogicValues(const std::list<LogicValueSharedPtr>& inSource)
	throw (Error) {
	std::list<LogicValueSharedPtr>::const_iterator it = inSource.begin();
	for(; it != inSource.end(); it++) {
		try {
			mCompounds.push_back(*it);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

/**
 * Set the list of logic values, which are dominated by the current logic value
 * 
 * @param[in] inSource List of logic values to be appended to
 */
void LogicValueAttributes::setDominatedLogicValues(const std::list<LogicValueSharedPtr>& inSource)
	throw (Error) {
	std::list<LogicValueSharedPtr>::const_iterator it = inSource.begin();
	for(; it != inSource.end(); it++) {
		try {
			mDominates.push_back(*it);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

/**
 * Set the logic value is isolated or not, default is false
 *
 * @param[in] inSource bool Whether the logic value is isolated or not.
 */
void LogicValueAttributes::setIsIsolated(const bool& inSource) {
	mIsIsolated = inSource;
}

/**
 * Set the list of logic maps.
 *
 * @param[in] inSource List of logic maps to be appended to
 */
void LogicValueAttributes::setLogicMaps(const std::list<LogicMap*>& inSource) {
	std::list<LogicMap*>::const_iterator it = inSource.begin();
	for(; it != inSource.end(); it++) {
		try {
			mLogicMaps.push_back(*it);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

/**
 * Add a logic map to the list of logic maps. If an empty pointer is supplied,
 * it returns without doing anything.
 *
 * @param[in] inLogicMap A pointer to a logic map object.
 *
 * @exception Error Logic map could not be added.
 */
void LogicValueAttributes::addLogicMap(LogicMap*& inLogicMap) throw (Error) {
	if(!inLogicMap) {
		return;
	}
	mLogicMaps.push_back(inLogicMap);
}

/**
 * Set the list of logic values, which are resolved by the current logic value
 *
 * @param[in] inSource List of logic values to be appended to
 */
void LogicValueAttributes::setResolvedLogicValues(const std::list<LogicValueSharedPtr>& inSource) {
	std::list<LogicValueSharedPtr>::const_iterator it = inSource.begin();
	for(; it != inSource.end(); it++) {
		try {
			mResolves.push_back(*it);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

LogicValueAttributes::LogicValueAttributes() : mVoltageMap(), mIsVoltageMapSet(false),
	mCurrentMap(), mIsCurrentMapSet(false), mBooleanMap(), mIsBooleanMapSet(false), mCompounds(),
	mWeak(), mStrong(), mDominates(), mLogicMaps(), mIsIsolated(false), mResolves() {}

LogicValueAttributes::~LogicValueAttributes() throw () {
	std::list<LogicMap *>::iterator it = mLogicMaps.begin();
	for(; it != mLogicMaps.end(); it++) {
		if(*it) {
			delete *it;
			*it = NULL;
		}
	}
}

LogicValueAttributes::LogicValueAttributes(const LogicValueAttributes& source) :
	mVoltageMap(source.mVoltageMap), mIsVoltageMapSet(source.mIsVoltageMapSet),
	mCurrentMap(source.mCurrentMap), mIsCurrentMapSet(source.mIsCurrentMapSet),
	mBooleanMap(source.mBooleanMap), mIsBooleanMapSet(source.mIsBooleanMapSet),
	mCompounds(source.mCompounds), mWeak(source.mWeak), mStrong(source.mStrong),
	mDominates(source.mDominates), mLogicMaps(source.mLogicMaps), mIsIsolated(source.mIsIsolated),
	mResolves(source.mResolves) {}

LogicValueAttributes& LogicValueAttributes::operator=(const LogicValueAttributes& source) {
	if(this != &source) {
		mVoltageMap = source.mVoltageMap;
		mIsVoltageMapSet = source.mIsVoltageMapSet;
		mCurrentMap = source.mCurrentMap;
		mIsCurrentMapSet = source.mIsCurrentMapSet;
		mBooleanMap = source.mBooleanMap;
		mIsBooleanMapSet = source.mIsBooleanMapSet;
		mCompounds = source.mCompounds;
		mWeak = source.mWeak;
		mStrong = source.mStrong;
		mDominates = source.mDominates;
		mLogicMaps = source.mLogicMaps;
		mIsIsolated = source.mIsIsolated;
		mResolves = source.mResolves;
	}
	return *this;
}

} // namespace generic
} // namespace torc
