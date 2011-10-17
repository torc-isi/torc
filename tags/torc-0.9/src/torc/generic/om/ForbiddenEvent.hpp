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

#ifndef TORC_GENERIC_OM_FORBIDDENEVENT_HPP
#define TORC_GENERIC_OM_FORBIDDENEVENT_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/Visitable.hpp"
#include "torc/generic/om/SelfReferencing.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/Value.hpp"

#include <list>

namespace torc { namespace generic { class Event; }  } 

namespace torc {

namespace generic {

/**
 * @brief ForbiddenEvent class lists events which are forbidden 
 * during a period of times which is specified by time interval.
 * Time interval is used to describe an interval between two times. 
 * Times can be described by events or offset events.
 * 
 */
class ForbiddenEvent
    : public Visitable,
    public SelfReferencing<ForbiddenEvent>
{
    friend class FactoryType<ForbiddenEvent>;

  public:
    /**
     * Convenience class to visit an ForbiddenEvent.
     */
    typedef VisitorType<ForbiddenEvent> Visitor;

    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    /**
     * Convenience class to create an ForbiddenEvent.
     */
    class Factory: public FactoryType<ForbiddenEvent>
    {
        public:
            using FactoryType<ForbiddenEvent>::create;
        /**
         * Create a ForbiddenEvent.
         *
         * @param[in] inStartTimeInterval Pointer to start event/offsetEvent of timeInterval.
         * @param[in] inEndTimeInterval Pointer to end event/offsetEvent of timeInterval.
         * @param[in] inEvents List of events which are forbidden during a period of time
         *            specified by TimeInterval( StartTimeInterval and EndTimeInterval ).
         * @param[in] inTiming Pointer to parented object (Timing) [optional].
         *            If not mentioned PathDelay will not be added to timing.
         * @param[in] inDuration Duration of timeInterval, if end event is absent [optional].
         *
         * @return Pointer to created ForbiddenEvent. 
         */
         ForbiddenEventSharedPtr
         virtual newForbiddenEventPtr( const EventSharedPtr & inStartTimeInterval,
                const EventSharedPtr & inEndTimeInterval,
                const std::list< EventSharedPtr > & inEvents,
                const TimingSharedPtr & inTiming
                            = TimingSharedPtr(),
                const Value & inDuration = Value() ) throw(Error);
    };

    /**
     * Get the pointer to start event/offsetEvent of timeInterval
     * @return Pointer to start event/offsetEvent of timeInterval
     */
    inline const EventSharedPtr 
    getStartTimeInterval() const throw();

    /**
     * Set the pointer to start event/offsetEvent of timeInterval
     * @param[in] inSource Pointer to start event/offsetEvent of timeInterval
     */
    void 
    setStartTimeInterval(const EventSharedPtr & inSource) throw();

    /**
     * Get the pointer to end event/offsetEvent of timeInterval
     * @return Pointer to end event/offsetEvent of timeInterval
     */
    inline const EventSharedPtr 
    getEndTimeInterval() const throw();

    /**
     * Set the pointer to end event/offsetEvent of timeInterval
     * @param[in] inSource Pointer to end event/offsetEvent of timeInterval
     */
    void 
    setEndTimeInterval(const EventSharedPtr & inSource) throw();

    /**
     * Get the duration of timeInterval, if end event is absent.
     * @return Value duration of timeInterval, if end event is absent.
     */
    inline const Value 
    getDuration() const throw();

    /**
     * Set the duration of timeInterval, if end event is absent.
     * @param[in] inValue Duration of timeInterval, if end event is absent.
     */
    void 
    setDuration(const Value & inValue) throw();

    /**
     * Get all the events of this forbiddenEvent.
     *
     * @param[out] outEvents List of events to be appended to
     */
    inline void 
    getEvents( std::list< EventSharedPtr > & outEvents ) const throw();

    /**
     * Set all the events of this forbiddenEvent.
     *
     * @param[in] inEvents List of events to be appended to
     */
    void 
    setEvents( const std::list< EventSharedPtr > & inEvents ) throw();

    /**
     * Add an event to the list of events
     *
     * @param[in] inSource Pointer to event to be appended to
     */
    void 
    addEvent(const EventSharedPtr & inSource) throw();

    virtual
    ~ForbiddenEvent() throw();

  protected:  
    ForbiddenEvent();

  private:
    ForbiddenEvent(const ForbiddenEvent & source) throw();

    ForbiddenEvent & 
    operator=(const ForbiddenEvent & source) throw();

    EventSharedPtr mStartTimeInterval;
    EventSharedPtr mEndTimeInterval;
    Value mDuration;
    std::list< EventSharedPtr > mEvents;

};

/**
 * Get the pointer to start event/offsetEvent of timeInterval
 * @return Pointer to start event/offsetEvent of timeInterval
 */
inline const EventSharedPtr 
ForbiddenEvent::getStartTimeInterval() const throw() {
    return mStartTimeInterval;
}

/**
 * Get the pointer to end event/offsetEvent of timeInterval
 * @return Pointer to end event/offsetEvent of timeInterval
 */
inline const EventSharedPtr 
ForbiddenEvent::getEndTimeInterval() const throw() {
    return mEndTimeInterval;
}

/**
 * Get the duration of timeInterval, if end event is absent.
 * @return Value duration of timeInterval, if end event is absent.
 */
inline const Value 
ForbiddenEvent::getDuration() const throw() {
    return mDuration;
}

/**
 * Get all the events of this forbiddenEvent.
 *
 * @param[out] outEvents List of events to be appended to
 */
inline void 
ForbiddenEvent::getEvents( 
        std::list< EventSharedPtr > & outEvents ) const throw() {
    outEvents.insert( outEvents.end(),
        mEvents.begin(), mEvents.end() );
}

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_FORBIDDENEVENT_HPP
