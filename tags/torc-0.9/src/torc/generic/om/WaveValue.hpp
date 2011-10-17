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

#ifndef TORC_GENERIC_OM_WAVEVALUE_HPP
#define TORC_GENERIC_OM_WAVEVALUE_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/Nameable.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/SelfReferencing.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/Value.hpp"

namespace torc { namespace generic { class LogicElement; }  } 

namespace torc {

namespace generic {

/**
 * @brief This class is used within simulate to describe input stimuli and expected
 * responces over a certain time interval
 */
class WaveValue 
    : public Nameable,
    public SelfReferencing<WaveValue>
{
    friend class FactoryType<WaveValue>;

  public:

    /**
     * Convenience class to visit a WaveValue.
     */
    typedef VisitorType<WaveValue> Visitor;

    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    /**
     * Convenience class to create a WaveValue.
     */
    class Factory: public FactoryType<WaveValue>
    {
        public:
            using FactoryType<WaveValue>::create;
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
         WaveValueSharedPtr
         virtual newWaveValuePtr( const std::string & inName,
                const Value & inDeltaTimeDuration,
                const LogicElementSharedPtr inLogicWaveForm,
                const SimulateSharedPtr & inSimulate
                            = SimulateSharedPtr() ) throw(Error);
    };

    /**
     * Get the delta time duration between the values contained in the logic waveform.
     * @return Value Delta time duration between the values contained in the logic waveform.
     */
    inline const Value 
    getDeltaTimeDuration() const throw();

    /**
     * Set the delta time duration between the values contained in the logic waveform.
     * @param[in] value Delta time duration between the values contained in the logic waveform.
     */
    void 
    setDeltaTimeDuration(const Value & value) throw();

    /**
     * Get the logic waveform.
     *
     * @return Pointer to LogicElement.
     */
    inline const LogicElementSharedPtr  
    getLogicWaveform() const throw();

    /**
     * Set the logic waveform.
     *
     * @param[in] inSource Pointer to logic element object.
     */
    void 
    setLogicWaveform( const LogicElementSharedPtr & inSource ) throw();

    ~WaveValue() throw();

  protected:  
    WaveValue();

  private:
    WaveValue( const WaveValue & source ) throw();

    WaveValue & 
    operator=( const WaveValue & source ) throw();

    Value mDeltaTimeDuration;
    LogicElementSharedPtr mLogicWaveform;

};

/**
 * Get the delta time duration between the values contained in the logic waveform.
 * @return Value Delta time duration between the values contained in the logic waveform.
 */
inline const Value 
WaveValue::getDeltaTimeDuration() const throw() {
    return mDeltaTimeDuration;
}

/**
 * Get the logic waveform.
 *
 * @param[out] outLogicElements List of LogicElement (s) to be appended to
 */
inline const LogicElementSharedPtr 
WaveValue::getLogicWaveform() const throw() {
    return mLogicWaveform;
}

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_WAVEVALUE_HPP
