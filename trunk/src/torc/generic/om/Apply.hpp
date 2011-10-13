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

#ifndef TORC_GENERIC_OM_APPLY_HPP
#define TORC_GENERIC_OM_APPLY_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/Commentable.hpp"
#include "torc/generic/om/Visitable.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/SelfReferencing.hpp"
#include "torc/generic/om/UserDataContainer.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/Value.hpp"

namespace torc { namespace generic { class LogicalResponse; }  } 

namespace torc {

namespace generic {

/**
 * @brief This class is used within simulate to describe input stimuli and expected
 * responces over a certain time interval
 */
class Apply 
    : public Commentable, 
    public Visitable,
    public SelfReferencing<Apply>,
    public UserDataContainer
{

    friend class FactoryType<Apply>;

  public:
    /**
     * Convenience class to visit apply .
     */
    typedef VisitorType<Apply> Visitor;

    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    /**
     * Convenience class to create apply .
     */
    class Factory: public FactoryType<Apply>
    {
        public:
            using FactoryType<Apply>::create;
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
         ApplySharedPtr
         virtual newApplyPtr( const int32_t & inNoOfCycle,
                const Value & inCycleDuration,
                const std::list< LogicalResponseSharedPtr > & inLogicResponces,
                const SimulateSharedPtr & inSimulate
                            = SimulateSharedPtr() ) throw(Error);
    };

    /**
     * Get the no of cycles 
     * @return int32_t No of cycles   
     */
    inline const int32_t 
    getNoOfCycle() const throw();

    /**
     * Set the no of cycles
     * @param[in] inValue int32_t No of cycles
     */
    void 
    setNoOfCycle( const int32_t & inValue ) throw();

    /**
     * Get the duration of each cycle
     * @return Duration of each cycle
     */
    inline const Value 
    getCycleDuration() const throw();

    /**
     * Set the duration of each cycle
     * @param[in] value Duration of each cycle
     */
    void 
    setCycleDuration(const Value & value) throw();

    /**
     * Get the list of LogicResponce.
     *
     * @param[out] outLogicResponces List of LogicResponce (s) to be appended to
     */
    inline void 
    getLogicResponses( std::list< LogicalResponseSharedPtr > & outLogicResponces ) const throw();

    /**
     * Set the list of LogicResponce.
     *
     * @param[in] inLogicResponces List of LogicResponce (s) to be appended to
     */
    void 
    setLogicResponses( const std::list< LogicalResponseSharedPtr > & inLogicResponces ) throw();

    /**
     * Add a LogicResponce to the list of LogicResponce.
     *
     * @param[in] inLogicResponce Pointer to LogicResponce to be appended to
     */
    void 
    addLogicResponse(const LogicalResponseSharedPtr & inLogicResponce ) throw();

    ~Apply() throw();

  protected:  
    Apply();

  private:
    Apply(const Apply & source) throw();

    Apply & 
    operator=(const Apply & source) throw();

    int32_t mNoOfCycle;
    Value mCycleDuration;
    std::list< LogicalResponseSharedPtr > mLogicResponses;

};

/**
 * Get the no of cycles 
 * @return int32_t No of cycles   
 */
inline const int32_t 
Apply::getNoOfCycle() const throw() {
    return mNoOfCycle;
}

/**
 * Get the duration of each cycle
 * @return Duration of each cycle
 */
inline const Value
Apply::getCycleDuration() const throw() {
    return mCycleDuration;
}

/**
 * Get the list of LogicResponce.
 *
 * @param[out] outLogicResponces List of LogicResponce (s) to be appended to
 */
inline void 
Apply::getLogicResponses( 
        std::list< LogicalResponseSharedPtr > & outLogicResponces ) const throw() {
    outLogicResponces.insert( outLogicResponces.end(),
            mLogicResponses.begin(), mLogicResponses.end() );            
}

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_APPLY_HPP
