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

#include "torc/generic/om/NetAttributes.hpp"

namespace torc {

namespace generic {

void 
NetAttributes::setCriticality(const int32_t & inSource) throw() {
    mCriticality = inSource;
    mCriticalitySet = true;
}

void 
NetAttributes::setNetDelay(const NetDelay & inSource) throw() {
    mNetDelay = inSource;
    mNetDelaySet = true;
}

NetAttributes::NetAttributes() 
    : mCriticality(),
    mNetDelay(),
    mCriticalitySet( false ),
    mNetDelaySet( false ) {
}

NetAttributes::~NetAttributes() throw() {
}

NetAttributes::NetAttributes(
        const NetAttributes & inSource) 
    : mCriticality( inSource.mCriticality ),
    mNetDelay( inSource.mNetDelay ),
    mCriticalitySet( inSource.mCriticalitySet ),
    mNetDelaySet( inSource.mNetDelaySet) {
}

NetAttributes & 
NetAttributes::operator=(
        const NetAttributes & inSource) throw() {
    if( this != &inSource )
    {
        mCriticality = inSource.mCriticality;
        mNetDelay = inSource.mNetDelay;
        mCriticalitySet = inSource.mCriticalitySet;
        mNetDelaySet = inSource.mNetDelaySet;
    }
    return *this;
}

} // namespace torc::generic

} // namespace torc
