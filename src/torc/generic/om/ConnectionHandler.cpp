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

#ifdef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif //HAVE_CONFIG_H


#include "torc/generic/om/ConnectionHandler.hpp"
#include "torc/generic/om/Net.hpp"

namespace torc {
namespace generic {
void
ConnectionHandler::connectPortToNet(
                    const PortSharedPtr &inPort ) throw(Error)
try
{
    mNet->addConnectedPort( inPort );
}
catch( Error &e )
{
    e.setCurrentLocation(
        __FUNCTION__, __FILE__, __LINE__ );
    throw;
}

void
ConnectionHandler::disconnectPortFromNet(
                    const PortSharedPtr &inPort ) throw(Error)
try
{
    mNet->removeConnectedPort( inPort );
}
catch( Error &e )
{
    e.setCurrentLocation(
        __FUNCTION__, __FILE__, __LINE__ );
    throw;
}


void
ConnectionHandler::connectPortRefToNet(
        const PortReferenceSharedPtr &inPortRef ) throw(Error)
try
{
    mNet->addConnectedPortReference( inPortRef );
}
catch( Error &e )
{
    e.setCurrentLocation(
        __FUNCTION__, __FILE__, __LINE__ );
    throw;
}


void
ConnectionHandler::disconnectPortRefFromNet(
        const PortReferenceSharedPtr &inPortRef ) throw(Error)
try
{
    mNet->removeConnectedPortReference( inPortRef );
}
catch( Error &e )
{
    e.setCurrentLocation(
        __FUNCTION__, __FILE__, __LINE__ );
    throw;
}

void
ConnectionHandler::connectPortListToNet(
        const PortListSharedPtr &inPortList ) throw(Error)
try
{
    mNet->addConnectedPortList( inPortList );
}
catch( Error &e )
{
    e.setCurrentLocation(
        __FUNCTION__, __FILE__, __LINE__ );
    throw;
}


void
ConnectionHandler::disconnectPortListFromNet(
        const PortListSharedPtr &inPortList ) throw(Error)
try
{
    mNet->removeConnectedPortList( inPortList );
}
catch( Error &e )
{
    e.setCurrentLocation(
        __FUNCTION__, __FILE__, __LINE__ );
    throw;
}

} //namespace generic
} //namespace torc
