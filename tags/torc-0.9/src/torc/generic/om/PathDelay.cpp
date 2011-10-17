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

#include "torc/generic/om/PathDelay.hpp"
#include "torc/generic/om/Event.hpp"
#include "torc/generic/om/Timing.hpp"

namespace torc {

namespace generic {

/**
 * Create a PathDelay.
 *
 * @param[in] inDelay Value::MiNoMax Delay value.
 * @param[in] inEvents List of events to be appended to
 * @param[in] inTiming Pointer to parented object (Timing) [optional].
 *            If not mentioned PathDelay will not be added to timing.
 *
 * @return Pointer to created PathDelay.
 */
PathDelaySharedPtr
PathDelay::Factory::newPathDelayPtr( const Value::MiNoMax & inDelay,
        const std::list< EventSharedPtr > & inEvents,
        const TimingSharedPtr & inTiming ) throw(Error) {
    try
    {
        PathDelaySharedPtr newPathDelay;
        create( newPathDelay );
        newPathDelay->setDelay( inDelay );
        newPathDelay->setEvents( inEvents );
        if( inTiming )
        {
            inTiming->addPathDelay( newPathDelay );
        }
        return newPathDelay;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void 
PathDelay::setDelay(const Value::MiNoMax & inSource) throw() {
   mDelay = inSource; 
}

/**
 * Set all the events of this forbiddenEvent.
 *
 * @param[out] inEvents List of events to be appended to
 */
void
PathDelay::setEvents( const std::list< EventSharedPtr > & inEvents ) throw() {
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

void
PathDelay::addEvent( const EventSharedPtr & inEvent ) throw(Error) {
    if( !inEvent )
    {
        return;
    }
    mEvents.push_back( inEvent );
}
    
PathDelay::PathDelay() 
    : SelfReferencing<PathDelay>(),
    mDelay(),
    mEvents() {
}

PathDelay::~PathDelay() throw(){
}

} // namespace torc::generic

} // namespace torc
