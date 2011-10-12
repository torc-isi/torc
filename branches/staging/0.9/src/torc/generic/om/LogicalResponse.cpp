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

#include "torc/generic/om/LogicalResponse.hpp"
#include "torc/generic/om/Port.hpp"
#include "torc/generic/om/PortList.hpp"
#include "torc/generic/om/LogicElement.hpp"

namespace torc {

namespace generic {

/**
 * Create a logical response.
 *
 * @param[in] inResponseType Response type.
 * @param[in] inPort Connected Port to this logical response.
 * @param[in] inLogicWaveForm Logic wave form for this logical response.
 * @param[in] inPortListAlias Connected Port list alias to this logical response [optional].
 *
 * @return Pointer to created logical response.
 */
LogicalResponseSharedPtr
LogicalResponse::Factory::newLogicalResponsePtr( const ResponseType & inResponseType,
        const PortSharedPtr & inPort,
        const LogicElementSharedPtr & inLogicWaveForm,
        const PortListAliasSharedPtr & inPortListAlias ) throw(Error) {
    try
    {
        LogicalResponseSharedPtr newLogicalResponse;
        create( newLogicalResponse );
        newLogicalResponse->setResponseType( inResponseType );
        newLogicalResponse->setConnectedPort( inPort );
        newLogicalResponse->setLogicWaveForm( inLogicWaveForm );
        newLogicalResponse->setConnectedPortListAlias( inPortListAlias );
        return newLogicalResponse;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
LogicalResponse::accept(BaseVisitor & inoutVisitor) throw(Error) {
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
 * Set the Logical response type
 *
 * @param[in] inSource Logical response type
 */
void 
LogicalResponse::setResponseType( const ResponseType & inSource ) throw() {
    mResponseType = inSource;
}

/**
 * Set the connected port shared pointer.
 *
 * @param[in] inPort Connected port shared pointer.
 */
void
LogicalResponse::setConnectedPort( const PortSharedPtr & inPort ) throw() {
    mConnectedPort = inPort;
}

/**
 * Set the connected port list shared pointer.
 *
 * @param[in] inPortList Connected port list shared pointer.
 */
void
LogicalResponse::setConnectedPortList( const PortListSharedPtr & inPortList ) throw() {
    mConnectedPortList = inPortList;
}

/**
 * Set the connected port list alias shared pointer.
 *
 * @param[in] inPortListAlias Connected port list alias shared pointer.
 */
void
LogicalResponse::setConnectedPortListAlias( 
        const PortListAliasSharedPtr & inPortListAlias ) throw() {
    mConnectedPortListAlias = inPortListAlias;
}

/**
 * Set the logic wave form for this response.
 *
 * @param[in] inSource Pointer to logic element object.
 */
void
LogicalResponse::setLogicWaveForm( const  LogicElementSharedPtr & inSource ) throw() {
    mLogicWaveForm = inSource;
}

LogicalResponse::LogicalResponse()
    : Visitable(),
    SelfReferencing<LogicalResponse>(), 
    mResponseType(),
    mConnectedPort(),
    mConnectedPortList(),
    mConnectedPortListAlias(),
    mLogicWaveForm() {
}

LogicalResponse::~LogicalResponse() throw() {
}

} // namespace torc::generic

} // namespace torc
