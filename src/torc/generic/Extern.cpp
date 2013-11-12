// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
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

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif 

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/weak_ptr.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/Extern.hpp"

namespace torc {
namespace generic {

/**
 * Set whether an item is extern or not.
 * 
 * @param[in] isExtern True if extern, false otherwise
 */
void Extern::setIsExtern(bool inIsExtern) {
	mIsExtern = inIsExtern;
}

Extern::Extern() : mIsExtern(false) {}

Extern::~Extern() throw () {}

#ifdef GENOM_SERIALIZATION
template <class Archive> void Extern::serialize(Archive& ar, unsigned int) {
	ar & mIsExtern;
}

//TO SATISFY THE LINKER
template void Extern::serialize<boost::archive::binary_iarchive>(
	boost::archive::binary_iarchive& ar, const unsigned int);

template void Extern::serialize<boost::archive::binary_oarchive>(
	boost::archive::binary_oarchive& ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace generic
} // namespace torc
