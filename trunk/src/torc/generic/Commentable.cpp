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
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/Commentable.hpp"

namespace torc {
namespace generic {

/**
 * Add a comment to the object
 * 
 * @param[in] comment The comment to add to an existing list of comments
 */
void Commentable::addComment(const std::string& comment) {
	mComments.push_back(comment);
}

/**
 * Get the list of all comments
 * 
 * @param[in] inSource A list of all comments to be set on this object
 */
void Commentable::setComments(const std::vector<std::string>& inSource) {
	mComments.insert(mComments.end(), inSource.begin(), inSource.end());
}

Commentable::Commentable() : mComments() {}

Commentable::~Commentable() throw () {}

#ifdef GENOM_SERIALIZATION
template <class Archive> void Commentable::serialize(Archive& ar, unsigned int) {
	ar & mComments;
}

//TO SATISFY THE LINKER
template void
Commentable::serialize<boost::archive::binary_iarchive>(boost::archive::binary_iarchive& ar,
	const unsigned int);

template void
Commentable::serialize<boost::archive::binary_oarchive>(boost::archive::binary_oarchive& ar,
	const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace generic
} // namespace torc
