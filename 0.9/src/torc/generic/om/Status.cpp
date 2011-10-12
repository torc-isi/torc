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

#include "torc/generic/om/Status.hpp"

namespace torc {

namespace generic {

/**
 * Create a status.
 *
 * @param[in] inContainer Pointer of the status container.
 *
 * @return Pointer to created status.
 */
StatusSharedPtr
Status::Factory::newStatusPtr( 
        const StatusContainerSharedPtr & inContainer ) throw(Error) {
    try
    {
        StatusSharedPtr newStatus;
        create( newStatus );
        inContainer->addStatus( newStatus );
        return newStatus;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }    
}

Status::Status()
    :Commentable(),
    Visitable(),
    UserDataContainer(),
    SelfReferencing<Status>(),
    mWrittens() {
}

Status::~Status() throw() {
}

/**
 * Set the vector of written statements.
 *
 * @param[in] inSource Vector containing written statements
 */    
void
Status::setWrittens( const std::vector< WrittenSharedPtr > &inSource ) throw() {
    std::vector< WrittenSharedPtr >::const_iterator it = inSource.begin();
    for( ; it != inSource.end(); it++ ) {
        mWrittens.push_back( *it );
    }
}

/**
 * Add a written statement to the vector of written statements. 
 * If an empty pointer is supplied, it returns without doing anything.
 *
 * @param[in] inWritten A pointer to a written object.
 *
 * @exception Error Written could not be added.
 */
void
Status::addWritten(WrittenSharedPtr & inWritten) throw(Error) {
    mWrittens.push_back( inWritten );
}

void 
Status::accept(BaseVisitor & visitor) throw(Error) {
    try
    {
        runVisitor( *this, visitor );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

} // namespace torc::generic

} // namespace torc
