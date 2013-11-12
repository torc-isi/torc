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
#endif //GENOM_SERIALIZATION
#include "torc/generic/PropertyContainer.hpp"
#include "torc/generic/Property.hpp"

namespace torc {
namespace generic {

/**
 * Get a property
 *
 * @param[in] name Name of the property to be retreived
 *
 * @return Pointer to the property object if present, empty pointer otherwise.
 */
PropertySharedPtr PropertyContainer::getProperty(const std::string& inName) {
	PropertySharedPtr property;
	if(!inName.empty()) {
		mProperties.get(inName, property);
	}
	return property;
}

/**
 * Save the inSource of  a property
 *
 * @param[in] inName Name of the property to be saved
 * @param[in] inPoperty Pointer to the property object if present, empty pointer otherwise.
 *
 * @return bool True if saved, false otherwise.
 */
bool PropertyContainer::setProperty(const std::string& inName,
	const PropertySharedPtr& inProperty) {
	if(!inName.empty()) {
		return mProperties.set(inName, inProperty);
	}
	return false;
}

/**
 * Set the map of properties
 *
 * @param[in] inSource The map of properties
 */
void PropertyContainer::setProperties(const std::map<std::string, PropertySharedPtr>& inSource) {
	std::map<std::string, PropertySharedPtr>::const_iterator it = inSource.begin();
	for(; it != inSource.end(); it++) {
		setProperty((*it).first, (*it).second);
	}
}

PropertyContainer::PropertyContainer() : mProperties() {}

PropertyContainer::~PropertyContainer() throw () {}

#ifdef GENOM_SERIALIZATION
template <class Archive> void PropertyContainer::serialize(Archive& ar, unsigned int) {
	ar & mProperties;
}

//TO SATISFY THE LINKER
template void PropertyContainer::serialize<boost::archive::binary_iarchive>(
	boost::archive::binary_iarchive& ar, const unsigned int);

template void PropertyContainer::serialize<boost::archive::binary_oarchive>(
	boost::archive::binary_oarchive& ar, const unsigned int);

#endif //GENOM_SERIALIZATION
}

// namespace generic
}// namespace torc
