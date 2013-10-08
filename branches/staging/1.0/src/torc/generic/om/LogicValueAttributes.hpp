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

#ifndef TORC_GENERIC_OM_LOGICVALUEATTRIBUTES_HPP
#define TORC_GENERIC_OM_LOGICVALUEATTRIBUTES_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/Value.hpp"
#include "torc/generic/util/Error.hpp"

#include <list>

namespace torc { namespace generic { class LogicValue; }  } 

namespace torc {

namespace generic {

/**
 * @brief This class is used within simulationInfo construct to define a logic value
 * to use for modeling.
 */
class LogicValueAttributes
{

  public:
    /**
     * @brief This structure is for logicRef and libraryRef used in logicMapInput/logicMapOutput.  
     */
    struct LogicMap
    {
        /**
         * @enum LogicMapType
         * LogicMap type, logicMapInput, logicMapOutput 
         */
        enum LogicMapType
        {
          eLogicMapTypeInput = 0,
          eLogicMapTypeOutput
        }; 
        
        std::string mLogicRef;
        std::string mLibraryRef;
        LogicMapType mLogicMapType;
    };

    /**
     * Get the logic value, which is used to indicate that the
     * current logic value is stronger than the return logic value.
     * 
     * @return Pointer to the logic value
     */
    inline const LogicValueSharedPtr 
    getStrongLogicValue() const throw();

    /**
     * Set the logic value, which is used to indicate that the
     * current logic value is stronger than this logic value.
     * 
     * @param[in] inSource Pointer to the logic value
     */
    void 
    setStrongLogicValue(const LogicValueSharedPtr & inSource) throw();

    /**
     * Get the logic value, which is used to indicate that the
     * current logic value is weaker than this logic value.
     * 
     * @return Pointer to the logic value
     */
    inline const LogicValueSharedPtr 
    getWeakLogicValue() const throw();

    /**
     * Set the logic value, which is used to indicate that the
     * current logic value is weaker than this logic value.
     * 
     * @param[in] inSource Pointer to the logic value
     */
    void 
    setWeakLogicValue(const LogicValueSharedPtr & inSource) throw();

    /**
     * Get the voltage value for this logic value.
     *
     * @return Value containing Value::MiNoMax
     */
    inline const Value::MiNoMax 
    getVoltageMap() const throw();

    /**
     * Set the voltage value for this logic value.
     *
     * @param[in] value Voltage value. Must be Value::MiNoMax
     */
    void 
    setVoltageMap(const Value::MiNoMax & value) throw();

    /**
     * Get whether this object is defined with a inSource, or is undefined.
     *
     * @return True if initialized.
     */
    inline const bool
    getIsVoltageMapSet() const throw();

    /**
     * Get the current value for this logic value.
     *
     * @return Value containing Value::MiNoMax
     */
    inline const Value::MiNoMax 
    getCurrentMap() const throw();

    /**
     * Set the current value for this logic value.
     *
     * @param[in] value Current value. Must be Value::MiNoMax
     */
    void 
    setCurrentMap(const Value::MiNoMax & value) throw();

    /**
     * Get whether this object is defined with a inSource, or is undefined.
     *
     * @return True if initialized.
     */
    inline const bool
    getIsCurrentMapSet() const throw();

    /**
     * Get the boolean value(true/false) for this logic value.
     *
     * @return Value containing Value::Boolean
     */
    inline const Value::Boolean 
    getBooleanMap() const throw();

    /**
     * Set the boolean value(true/false) for this logic value.
     *
     * @param[in] value Boolean value. Must be Value::Boolean 
     */
    void 
    setBooleanMap(const Value::Boolean & value) throw();

    /**
     * Get whether this object is defined with a inSource, or is undefined.
     *
     * @return True if initialized.
     */
    inline const bool
    getIsBooleanMapSet() const throw();

    /**
     * Get the list of compound logic values for this logic value.
     *
     * @param[out] outLogicValues List of logic values to be appended to
     */
    inline void 
    getCompoundLogicValues(
        std::list< LogicValueSharedPtr > & outLogicValues) const throw();

    /**
     * Set the list of compound logic values for this logic value.
     *
     * @param[in] inSource List of logic values to be appended to
     */
    void 
    setCompoundLogicValues(
        const std::list< LogicValueSharedPtr > & inSource) throw(Error);

    /**
     * Get the list of logic values, which are dominated by the current logic value
     * 
     * @param[out] outLogicValues List of logic values to be appended to
     */
    inline void
    getDominatedLogicValues(
        std::list< LogicValueSharedPtr > & outLogicValues) const throw();

    /**
     * Set the list of logic values, which are dominated by the current logic value
     * 
     * @param[in] inSource List of logic values to be appended to
     */
    void 
    setDominatedLogicValues(
        const std::list< LogicValueSharedPtr > & inSource) throw(Error);

    /**
     * Get whether the logic value is isolated or not.
     *
     * @return bool Whether the logic value is isolated or not.
     */
    inline const bool 
    getIsIsolated() const throw();

    /**
     * Set the logic value is isolated or not, default is false
     *
     * @param[in] inSource bool Whether the logic value is isolated or not.
     */
    void 
    setIsIsolated(const bool & inSource) throw();

    /**
     * Get the list of logic maps.
     *
     * @param[out] outLogicMaps List of logic maps to be appended to
     */
    inline void 
    getLogicMaps(std::list< LogicMap* > & outLogicMaps) const throw();

    /**
     * Set the list of logic maps.
     *
     * @param[in] inSource List of logic maps to be appended to
     */
    void 
    setLogicMaps(const std::list< LogicMap* > & inSource) throw();

    /**
     * Add a logic map to the list of logic maps. If an empty pointer is supplied,
     * it returns without doing anything.
     *
     * @param[in] inLogicMap A pointer to a logic map object.
     *
     * @exception Error Logic map could not be added.
     */
    void
    addLogicMap(LogicMap* & inLogicMap) throw(Error);

    /**
     * Get the list of logic values, which are resolved by the current logic value
     * 
     * @param[out] outLogicValues List of logic values to be appended to
     */
    inline void
    getResolvedLogicValues(
        std::list< LogicValueSharedPtr > & outLogicValues) const throw();

    /**
     * Set the list of logic values, which are resolved by the current logic value
     *
     * @param[in] inSource List of logic values to be appended to
     */
    void 
    setResolvedLogicValues(
        const std::list< LogicValueSharedPtr > & inSource) throw();

    LogicValueAttributes();

    ~LogicValueAttributes() throw();   

    LogicValueAttributes(const LogicValueAttributes & source);

    LogicValueAttributes &
    operator=(const LogicValueAttributes & source) throw();

  private:
    Value::MiNoMax mVoltageMap;
    bool mIsVoltageMapSet;    
    Value::MiNoMax mCurrentMap;
    bool mIsCurrentMapSet;
    Value::Boolean mBooleanMap;
    bool mIsBooleanMapSet;
    std::list< LogicValueSharedPtr > mCompounds;
    LogicValueSharedPtr mWeak;
    LogicValueSharedPtr mStrong;
    std::list< LogicValueSharedPtr > mDominates;
    std::list< LogicMap* > mLogicMaps;
    bool mIsIsolated;
    std::list< LogicValueSharedPtr > mResolves;

};

/**
 * Get the logic value, which is used to indicate that the
 * current logic value is stronger than the return logic value.
 * 
 * @return Pointer to the logic value
 */
inline const LogicValueSharedPtr 
LogicValueAttributes::getStrongLogicValue() const throw() {
    return mStrong;
}

/**
 * Get the logic value, which is used to indicate that the
 * current logic value is weaker than this logic value.
 * 
 * @return Pointer to the logic value
 */
inline const LogicValueSharedPtr 
LogicValueAttributes::getWeakLogicValue() const throw() {
    return mWeak;
}

/**
 * Get the voltage value for this logic value.
 *
 * @return Value containing Value::MiNoMax
 */
inline const Value::MiNoMax 
LogicValueAttributes::getVoltageMap() const throw() {
    return mVoltageMap;    
}

/**
 * Get whether this object is defined with a inSource, or is undefined.
 *
 * @return True if initialized.
 */
inline const bool
LogicValueAttributes::getIsVoltageMapSet() const throw() {
    return mIsVoltageMapSet;
}

/**
 * Get the current value for this logic value.
 *
 * @return Value containing Value::MiNoMax
 */
inline const Value::MiNoMax 
LogicValueAttributes::getCurrentMap() const throw() {
    return mCurrentMap;
}

/**
 * Get whether this object is defined with a inSource, or is undefined.
 *
 * @return True if initialized.
 */
inline const bool
LogicValueAttributes::getIsCurrentMapSet() const throw() {
    return mIsCurrentMapSet;
}

/**
 * Get the boolean value(true/false) for this logic value.
 *
 * @return Value containing Value::Boolean
 */
inline const Value::Boolean 
LogicValueAttributes::getBooleanMap() const throw() {
    return mBooleanMap;
}

/**
 * Get whether this object is defined with a inSource, or is undefined.
 *
 * @return True if initialized.
 */
inline const bool
LogicValueAttributes::getIsBooleanMapSet() const throw() {
    return mIsBooleanMapSet;
}

/**
 * Get the list of compound logic values for this logic value.
 *
 * @param[out] outLogicValues List of logic values to be appended to
 */
inline void 
LogicValueAttributes::getCompoundLogicValues(
    std::list< LogicValueSharedPtr > & outLogicValues) const throw() {
    outLogicValues.insert( outLogicValues.end(),
        mCompounds.begin(), mCompounds.end() );
}

/**
 * Get the list of logic values, which are dominated by the current logic value
 * 
 * @param[out] outLogicValues List of logic values to be appended to
 */
inline void
LogicValueAttributes::getDominatedLogicValues(
    std::list< LogicValueSharedPtr > & outLogicValues) const throw() {
    outLogicValues.insert( outLogicValues.end(),
        mDominates.begin(), mDominates.end() );
}

/**
 * Get whether the logic value is isolated or not.
 *
 * @return bool Whether the logic value is isolated or not.
 */
inline const bool 
LogicValueAttributes::getIsIsolated() const throw() {
    return mIsIsolated;
}

/**
 * Get the list of logic values, which are resolved by the current logic value
 * 
 * @param[out] outLogicValues List of logic values to be appended to
 */
inline void
LogicValueAttributes::getResolvedLogicValues(
    std::list< LogicValueSharedPtr > & outLogicValues) const throw() {
    outLogicValues.insert ( outLogicValues.end(),
        mResolves.begin(), mResolves.end() );
}

/**
 * Get the list of logic maps.
 *
 * @param[out] outLogicMaps List of logic maps to be appended to
 */
inline void 
LogicValueAttributes::getLogicMaps(
    std::list< LogicMap* > & outLogicMaps) const throw() {
    outLogicMaps.insert( outLogicMaps.end(),
        mLogicMaps.begin(), mLogicMaps.end() );
}

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_LOGICVALUEATTRIBUTES_HPP
