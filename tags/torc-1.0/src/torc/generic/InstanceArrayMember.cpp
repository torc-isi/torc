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
#include <boost/serialization/export.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/InstanceArrayMember.hpp"
#include "torc/generic/VisitorType.hpp"
#include "torc/generic/ParameterMap.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT(torc::generic::InstanceArrayMember)
#endif //GENOM_SERIALIZATION
namespace torc {
namespace generic {

/**
 * Receive an inoutVisitor to this class. The visit method of the inoutVisitor is called and a
 * reference to this object is passed as a parameter. It has to be noted however, that a
 * dynamic_cast is performed inside this method. If the cast fails, an appropriate exception is
 * thrown by this method. This situation can arise when the passed Visitor object does not inherit
 * from the appropriate inoutVisitor specialization. See Visitor documentation for more details.
 *
 * @param[in,out] inoutVisitor A reference to the inoutVisitor object
 * @exception Error Visitor type inappropriate for visiting this object or any other error thrown
 * by the Visitor::throw() method.
 */
void InstanceArrayMember::accept(BaseVisitor& inoutVisitor) throw (Error) {
	try {
		runVisitor(*this, inoutVisitor);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

ParameterContext InstanceArrayMember::getParameterContext() const {
	InstanceSharedPtr parent = getParentCollection();
	if(!parent) {
		return ParameterContext();
	}
	return parent->getParameterContext();
}

/**
 * Get the ParameterMap corresponding to this instance and it's master
 *
 * @return A ParameterMap consisiting of only the overridden parameters.
 */
ParameterMapSharedPtr InstanceArrayMember::getParameters() const throw (Error) {
	InstanceSharedPtr parent = getParentCollection();
	if(!parent) {
		Error e(eMessageIdErrorPointerToItemDoesNotExist, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Parent Collection", parent);
		throw e;
	}
	return parent->getParameters();
}

InstanceArrayMember::InstanceArrayMember() : Instance(), VectorBit<Instance>() {}

InstanceArrayMember::~InstanceArrayMember() throw () {}

#ifdef GENOM_SERIALIZATION
template <class Archive> void InstanceArrayMember::serialize(Archive& ar, unsigned int) {
	ar & boost::serialization::base_object<Instance>(*this);
	ar & boost::serialization::base_object<
	VectorBit<Instance> >(*this);
}

//TO SATISFY THE LINKER
template void InstanceArrayMember::serialize<boost::archive::binary_iarchive>(
	boost::archive::binary_iarchive& ar, const unsigned int);

template void InstanceArrayMember::serialize<boost::archive::binary_oarchive>(
	boost::archive::binary_oarchive& ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace generic
} // namespace torc
