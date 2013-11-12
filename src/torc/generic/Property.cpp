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
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/is_abstract.hpp>
#include <boost/serialization/string.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/Property.hpp"

namespace torc {
namespace generic {

void Property::accept(BaseVisitor& inoutVisitor) throw (Error) {
	try {
		runVisitor(*this, inoutVisitor);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

Property::Property() : Commentable(), Renamable(), SelfReferencing<Property>(), Visitable() {}

Property::~Property() throw () {}

/**
 * Set the Property inSource.
 *
 * @param[in] inSource Value of property.
 */
void Property::setValue(const Value& inSource) {
	mValue = inSource;
}

/**
 * Set the Property inSource unit.
 *
 * @param[in] inSource Unit of property.
 */
void Property::setUnit(const Unit& inSource) {
	mUnit = inSource;
}

/**
 * Set the owner/source of this property.
 *
 * @param[in] inSource Owner of this property
 */
void Property::setOwner(const std::string& inSource) {
	mOwner = inSource;
}

/**
 * Set the nested properties.
 *
 * @param[in] inSource Map containing properties
 */
void Property::setChildren(const std::map<std::string, PropertySharedPtr>& inSource) {
	std::map<std::string, PropertySharedPtr>::const_iterator entry = inSource.begin();
	std::map<std::string, PropertySharedPtr>::const_iterator end = inSource.end();
	for(; entry != end; ++entry) {
		addChildProperty((*entry).first, (*entry).second);
	}
}

/**
 * Add a nested property to this property
 *
 * @param[in] inName Nested property name
 * @param[in] inProperty Nested property
 */
bool Property::addChildProperty(const std::string& inName, const PropertySharedPtr& inProperty) {
	if(!inName.empty()) {
		return mChildren.set(inName, inProperty);
	}
	return false;
}

#ifdef GENOM_SERIALIZATION

template <class Archive> void Property::serialize(Archive& ar, unsigned int) {
	ar & boost::serialization::base_object<Commentable>(*this);
	ar & boost::serialization::base_object<Nameable>(*this);
	ar & boost::serialization::base_object<Renamable>(*this);
	ar & boost::serialization::base_object< SelfReferencing<Property> >(*this);
	ar & boost::serialization::base_object< Visitable >(*this);
	ar & mOwner;
	ar & mUnit;
	ar & mValue;
	ar & mChildren;
}

//TO SATISFY THE LINKER
template void Property::serialize<boost::archive::binary_iarchive>(
	boost::archive::binary_iarchive& ar, const unsigned int);

template void Property::serialize<boost::archive::binary_oarchive>(
	boost::archive::binary_oarchive& ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace generic
} // namespace torc
