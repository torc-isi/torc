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

#include "torc/generic/om/PortListAlias.hpp"
#include "torc/generic/om/Simulate.hpp"

namespace torc {

namespace generic {

/**
 * Create a port list alias
 *
 * @param[in] inName Name of the port list alias to be created.
 * @param[in] inPortList Pointer to port list object.
 * @param[in] inSimulate Pointer to parented (Simulate) object [optional].
 *            If not mentioned PortListAlias will not be added to simulate.
 *
 * @return Pointer to created port list alias.
 **/
PortListAliasSharedPtr
PortListAlias::Factory::newPortListAliasPtr( const std::string & inName,
        const PortListSharedPtr & inPortList,
        const SimulateSharedPtr & inSimulate ) throw(Error) {
    try
    {
        PortListAliasSharedPtr newPortListAlias;
        create( newPortListAlias );
        newPortListAlias->setName( inName );
        newPortListAlias->setPortList( inPortList );
        if( inSimulate )
        {
            inSimulate->addPortListAlias( newPortListAlias );
        }
        return newPortListAlias;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

PortListAlias::PortListAlias()
    : Nameable(),
    SelfReferencing<PortListAlias>(),
    mPortList() {
}

void
PortListAlias::accept(BaseVisitor & inoutVisitor) throw(Error) {
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

void
PortListAlias::setPortList( const PortListSharedPtr & inPortList ) throw() {
    mPortList = inPortList;
}

PortListAlias::~PortListAlias() throw() {
}

} // namespace torc::generic

} // namespace torc
