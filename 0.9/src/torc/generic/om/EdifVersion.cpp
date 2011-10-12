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

#include "torc/generic/om/EdifVersion.hpp"

namespace torc {

namespace generic {

EdifVersion::EdifVersion()
    :mFirst(2),
    mSecond(0),
    mThird(0) {
}

EdifVersion::~EdifVersion() throw() {
}

EdifVersion::EdifVersion(const EdifVersion & source) throw()
    :mFirst( source.mFirst ),
    mSecond( source.mSecond ),
    mThird( source.mThird ) {
}

EdifVersion &
EdifVersion::operator=(const EdifVersion & source) throw() {
    if( this != &source )
    {
        mFirst = source.mFirst;
        mSecond = source.mSecond;
        mThird = source.mThird;
    }
    return *this;
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
EdifVersion::serialize( Archive &ar, unsigned int ) {
    ar & mFirst;
    ar & mSecond;
    ar & mThird;
}

//TO SATISFY THE LINKER
template void
EdifVersion::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
EdifVersion::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);


#endif //GENOM_SERIALIZATION


} // namespace torc::generic

} // namespace torc
