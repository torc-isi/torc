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

#include "torc/generic/om/StatusContainer.hpp"
#include "torc/generic/om/Status.hpp"

namespace torc {

namespace generic {

StatusContainer::StatusContainer() 
    :mStatuses() {
}

StatusContainer::~StatusContainer() throw() {
}

/**
 * Set a vector of pointer to Status object
 *
 * @param[in] inStatus The vector of pointer to Status object
 */
void 
StatusContainer::setStatuses(const std::vector< StatusSharedPtr > & inStatus) throw() {
    std::vector< StatusSharedPtr >::const_iterator it = inStatus.begin();
    for( ; it != inStatus.end(); it++ ) {
        mStatuses.push_back( *it );
    }
}

/**
 * Add a status to the list of statuses. If an empty pointer is supplied, 
 * it returns without doing anything.
 * 
 * @param[in] inStatus A pointer to a status object.
 * 
 * @exception Error Status could not be added.
 */
void 
StatusContainer::addStatus(const StatusSharedPtr & inStatus) throw(Error) {
    mStatuses.push_back( inStatus );
}

} // namespace torc::generic

} // namespace torc
