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

#include "torc/generic/om/Timing.hpp"
#include "torc/generic/om/PathDelay.hpp"
#include "torc/generic/om/ForbiddenEvent.hpp"
#include "torc/generic/om/InterfaceAttributes.hpp"
#include "torc/generic/om/View.hpp"
#include "torc/generic/om/Instance.hpp"

namespace torc {

namespace generic {

/**
 * Create a Timing.
 *
 * @param[in] inDerivation Derivation value   
 * @param[in] inPathDelays List of path delays of this timing to be appended to.
 * @param[in] inForbiddentEvents List of forbidden events to be appended to
 * @param[in] inView Pointer to parented (view) object.
 * @param[in] inInterfaceAttributes Pointer to
 *            parented (InterfaceAttributes) object [optional]. 
 *            If mentioned then this will decompile within (contents ...) construct.
 * @param[in] inInstance Pointer to parented object( Instance ) [optional]. 
 *            If mentioned then this will decompile within (instance ...) construct.
 *
 * @return Pointer to created Timing.
 */
TimingSharedPtr
Timing::Factory::newTimingPtr( const Derivation & inDerivation,
        const std::list< PathDelaySharedPtr > & inPathDelays,
        const std::list< ForbiddenEventSharedPtr > & inForbiddentEvents,
        const ViewSharedPtr & inView,
        const InterfaceAttributesSharedPtr & inInterfaceAttributes,
        const InstanceSharedPtr & inInstance ) throw(Error) {
    try
    {
        TimingSharedPtr newTiming;
        create( newTiming );
        newTiming->setDerivation( inDerivation );
        newTiming->setPathDelays( inPathDelays );
        newTiming->setForbiddentEvents( inForbiddentEvents );
        if( inInstance )
        {
            inInstance->setTiming( newTiming );
        }
        else if( inInterfaceAttributes )
        {
            inInterfaceAttributes->setTiming( newTiming );
            inView->setInterfaceAttributes( inInterfaceAttributes );
        }
        else
        {
            inView->setTiming( newTiming );
        }
        return newTiming;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
Timing::accept(BaseVisitor & inoutVisitor) throw(Error) {
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
 * Set derivation
 *
 * @param[in] value Derivation value
 */
void 
Timing::setDerivation(const Derivation & value) throw() {
    mDerivation = value;
}

/**
 * Set all the path delays of this timing.
 *
 * @param[in] inPathDelays List of path delays of this timing to be appended to
 */
void
Timing::setPathDelays( 
    const std::list< PathDelaySharedPtr > & inPathDelays ) throw() {
    std::list< PathDelaySharedPtr >::const_iterator it  = inPathDelays.begin();
    for( ; it != inPathDelays.end(); it++ )
    {
        try
        {
            addPathDelay( *it );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
}

/**
 * Set all the forbidden events of this timing.
 *
 * @param[in] inForbiddentEvents List of events to be appended to
 */
void
Timing::setForbiddentEvents(
    const std::list< ForbiddenEventSharedPtr > & inForbiddentEvents ) throw() {
    std::list< ForbiddenEventSharedPtr >::const_iterator it = inForbiddentEvents.begin();
    for( ; it != inForbiddentEvents.end(); it++ )
    {
        try
        {
            addForbiddenEvent( *it );   
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
}

/**
 * Add a path delay to the list of path delays.
 *
 * @param[in] inSource Pointer to path delay to be appended to
 */
void 
Timing::addPathDelay(const PathDelaySharedPtr & inSource) throw() {
    try
    {
        mPathDelays.push_back( inSource );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
 * Add a forbidden event of the list of forbidden events.
 *
 * @param[in] inSource Pointer to forbidden event to be appended to
 */
void 
Timing::addForbiddenEvent(const ForbiddenEventSharedPtr & inSource) throw(){
    try
    {
        mForbiddentEvents.push_back( inSource );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

Timing::Timing() 
    : Visitable(),
    SelfReferencing<Timing>(),
    Commentable(),
    UserDataContainer(),
    mDerivation(),
    mPathDelays(),
    mForbiddentEvents() {
}

Timing::~Timing() throw() {
}

} // namespace torc::generic

} // namespace torc
