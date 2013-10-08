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

#include "torc/generic/om/ForbiddenEvent.hpp"
#include "torc/generic/om/Event.hpp"
#include "torc/generic/om/Timing.hpp"

namespace torc {

namespace generic {

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
ForbiddenEvent::Factory::newForbiddenEventPtr( 
        const EventSharedPtr & inStartTimeInterval,
        const EventSharedPtr & inEndTimeInterval,
        const std::list< EventSharedPtr > & inEvents,
        const TimingSharedPtr & inTiming,
        const Value & inDuration ) throw(Error) {
    try
    {
        ForbiddenEventSharedPtr newForbiddenEvent;
        create( newForbiddenEvent );
        newForbiddenEvent->setStartTimeInterval( inStartTimeInterval );
        newForbiddenEvent->setEndTimeInterval( inEndTimeInterval );
        newForbiddenEvent->setEvents( inEvents );
        if( inDuration.getIsSet() )
        {
            newForbiddenEvent->setDuration( inDuration );
        }
        if( inTiming )
        {
            inTiming->addForbiddenEvent( newForbiddenEvent );
        }
        return newForbiddenEvent;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
ForbiddenEvent::accept(BaseVisitor & inoutVisitor) throw(Error) {
    try
    {
        runVisitor( *this, inoutVisitor );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
 * Set the pointer to start event/offsetEvent of timeInterval
 * @param[in] inSource Pointer to start event/offsetEvent of timeInterval
 */
void 
ForbiddenEvent::setStartTimeInterval(const EventSharedPtr & inSource) throw() {
    mStartTimeInterval = inSource;
}

/**
 * Set the pointer to end event/offsetEvent of timeInterval
 * @param[in] inSource Pointer to end event/offsetEvent of timeInterval
 */
void 
ForbiddenEvent::setEndTimeInterval(const EventSharedPtr & inSource) throw() {
    mEndTimeInterval = inSource;
}

/**
 * Set the duration of timeInterval, if end event is absent.
 * @param[in] inValue Duration of timeInterval, if end event is absent.
 */
void 
ForbiddenEvent::setDuration(const Value & inValue) throw() {
    mDuration = inValue;
}

/**
 * Set all the events of this forbiddenEvent.
 *
 * @param[out] inEvents List of events to be appended to
 */
void 
ForbiddenEvent::setEvents( const std::list< EventSharedPtr > & inEvents ) throw() {
    std::list< EventSharedPtr >::const_iterator it = inEvents.begin();
    for( ; it != inEvents.end(); it++ )
    {
        try
        {
            addEvent( *it );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                    __FUNCTION__, __FILE__, __LINE__ );
        }
    }
}

/**
 * Add an event to the list of events
 *
 * @param[in] inSource Pointer to event to be appended to
 */
void
ForbiddenEvent::addEvent(const EventSharedPtr & inSource) throw() {
    try
    {
        mEvents.push_back( inSource );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

ForbiddenEvent::ForbiddenEvent() 
    : Visitable(),
    SelfReferencing<ForbiddenEvent>(), 
    mStartTimeInterval(),
    mEndTimeInterval(),
    mDuration(),
    mEvents() {
}

ForbiddenEvent::~ForbiddenEvent() throw() {
}


} // namespace torc::generic

} // namespace torc
