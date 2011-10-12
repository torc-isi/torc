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

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/ScaleFactor.hpp"

namespace torc {

namespace generic {

/**
 * Set the EDIF scale.
 *
 * @param[in] inSource EDIF scale.
 */
void ScaleFactor::setEdifValue(const Value::Number & inSource) throw() {
  mEdifValue = inSource;
}

/**
 * Set the SI scale.
 *
 */
void ScaleFactor::setSiValue(const Value::Number & inSource) throw() {
  mSiValue = inSource;
}

ScaleFactor::ScaleFactor()
    :mEdifValue(),
    mSiValue() {
}

ScaleFactor::ScaleFactor( const ScaleFactor &inSource )
    :mEdifValue( inSource.mEdifValue ),
    mSiValue( inSource.mEdifValue ) {
}

ScaleFactor::~ScaleFactor() {
}

ScaleFactor &
ScaleFactor::operator = ( const ScaleFactor &inSource ) {
    if( this != &inSource )
    {
        mEdifValue = inSource.mEdifValue;
        mSiValue = inSource.mSiValue;
    }
    return *this;
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
ScaleFactor::serialize( Archive &ar, unsigned int ) {
    ar & mEdifValue;
    ar & mSiValue;
}

//TO SATISFY THE LINKER
template void
ScaleFactor::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
ScaleFactor::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
