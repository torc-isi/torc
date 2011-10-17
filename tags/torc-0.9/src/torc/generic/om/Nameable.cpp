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

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/string.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Nameable.hpp"

namespace torc {

namespace generic {

Nameable::Nameable() {
}

Nameable::~Nameable() throw() {
}

/**
 * Set a name for this object
 *
 * @param[in] inSource Name of the object
 *
 * @exception Error Could not set name, because name is empty (Will be used by array members)
 */
void
Nameable::setName(const std::string & inSource) throw(Error) {
    if( inSource.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Nameable", inSource);
        throw e;
    }
    mName = inSource;
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
Nameable::serialize( Archive &ar, unsigned int ) {
    ar & mName;
}

//TO SATISFY THE LINKER
template void
Nameable::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
Nameable::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION
} // namespace torc::generic

} // namespace torc
