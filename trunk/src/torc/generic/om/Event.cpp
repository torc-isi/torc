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

#include "torc/generic/om/Event.hpp"
#include "torc/generic/om/Port.hpp"
#include "torc/generic/om/PortList.hpp"

namespace torc {

namespace generic {

/**
 * Create an event.
 *
 * @param[in] inEventType Event type.
 * @param[in] inPorts List of ports to this composition.
 * @param[in] inPortReferences List of port references to this composition.
 * @param[in] inNets List of nets to this composition.
 * @param[in] inTransition Pointer to logic state value( transition/becomes ) [optional].
 *            If no transition are specified, then the default meaning is any logic state change.  
 * @param[in] inPortList Connected Port list to this event [optional].   
 * @param[in] inOffsetTime Offset time retative to an event [optional].
 *            This is must for OffsetEvent, but optional for normal Event.  
 *
 * @return Pointer to created event.
 */
EventSharedPtr
Event::Factory::newEventPtr( const Event::Type & inEventType,
        const std::list< PortSharedPtr > & inPorts,
        const std::list < PortReferenceSharedPtr > & inPortReferences,
        const std::list< NetSharedPtr > & inNets,
        const LogicElementSharedPtr & inTransition,
        const PortListSharedPtr & inPortList,
        const Value & inOffsetTime ) throw(Error) {
    try
    {
        EventSharedPtr newEvent;
        create( newEvent );
        newEvent->setType( inEventType );
        std::list< PortSharedPtr >::const_iterator portIt = inPorts.begin();
        for( ; portIt != inPorts.end(); portIt++ )
        {
            newEvent->addPort( *portIt );
        } 

        std::list< PortReferenceSharedPtr >::const_iterator portRefIt
                                    = inPortReferences.begin();
        for( ; portRefIt != inPortReferences.end(); portRefIt++ )
        {
            newEvent->addPortReference( *portRefIt );
        }

        std::list< NetSharedPtr >::const_iterator netIt = inNets.begin();
        for( ; netIt != inNets.end(); netIt++ )
        {
            newEvent->addNet( *netIt );
        } 
        if( inTransition )
        {
            newEvent->setTransition( inTransition );
        }
        if( inPortList )
        {
            newEvent->setPortList( inPortList );
        }
        if( Event::eTypeOffsetEvent == inEventType )
        {
            if( inOffsetTime.getIsSet() )
            {
                newEvent->setOffsetTime( inOffsetTime );
            }
        }
        return newEvent;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
Event::accept(BaseVisitor & inoutVisitor) throw(Error) {
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
 * Set the event type
 *
 * @param[in] inSource Event type
 */
void
Event::setType( const Type & inSource ) throw() {
    mType = inSource;
}

/**
 * Add a port to the port list.
 *
 * @param[in] inPort Pointer to port to be added.
 */
void
Event::addPort( const PortSharedPtr &inPort ) throw() {
    mPortElements.push_back( PortElement( inPort ) );
}

/**
 * Add a port reference to the port list.
 *
 * @param[in] inPortRef Pointer to port reference to be added.
 */
void
Event::addPortReference(
        const PortReferenceSharedPtr &inPortRef ) throw() {
    mPortElements.push_back( PortElement( inPortRef ) );
}

/**
 * Set the pointer to portList
 *
 * @param[in] inPortList Pointer to portList
 */

void 
Event::setPortList(const  PortListSharedPtr & inPortList) throw() {
    mPortList = inPortList;
}

/**
 * Add a Net to the Net list.
 *
 * @param[in] inNet Pointer to Net to be added.
 */
void
Event::addNet( const NetSharedPtr &inNet ) throw() {
    mNets.push_back( inNet );
}

/**
 * Set the pointer to logic state value( transition/becomes ).
 *
 * @param[in] inSource Pointer to logic state value( transition/becomes ).
 */
void 
Event::setTransition(const LogicElementSharedPtr & inSource) throw() {
    mTransition = inSource;
}

/**
 * Set the offset time retative to an event.
 * @param[in] value Offset time retative to an event.
 */
void
Event::setOffsetTime(const Value & value) throw() {
    mOffsetTime = value;
}

Event::Event() 
    : Visitable(),
    SelfReferencing<Event>(), 
    mType(), 
    mPortElements(),
    mPortList(),
    mNets(),
    mTransition(),
    mOffsetTime() {
}

Event::~Event() throw() {
}

} // namespace torc::generic

} // namespace torc
