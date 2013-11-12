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

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/ScalarNet.hpp"
#include "torc/generic/NetBundle.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT(torc::generic::ScalarNet)
#endif //GENOM_SERIALIZATION
namespace torc {
namespace generic {

/**
 * Create a scalar net.
 *
 * @param[in] inName Name of the scalar net to be created.
 * @param[in] inViewPtr Pointer to parented(View) object.
 * @param[in] inOriginalName Original name of the scalar net [optional].
 *
 * @return Pointer to created scalar net.
 **/
ScalarNetSharedPtr ScalarNet::Factory::newScalarNetPtr(const std::string& inName,
	const ViewSharedPtr& inViewPtr, const NetBundleSharedPtr& inParentCollection,
	const std::string& inOriginalName) throw (Error) {
	try {
		ScalarNetSharedPtr newScalarNet;
		create(newScalarNet);
		newScalarNet->setName(inName);
		newScalarNet->setOriginalName(inOriginalName);
		if(inParentCollection) {
			inParentCollection->addChild(newScalarNet);
		} else {
			inViewPtr->addNet(newScalarNet);
		}
		return newScalarNet;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

void ScalarNet::accept(BaseVisitor& inoutVisitor) throw (Error) {
	try {
		runVisitor(*this, inoutVisitor);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

Connectable::Connection ScalarNet::connect(const NetSharedPtr& inNet) throw (Error) {
	if(!inNet) {
		Error e(eMessageIdErrorPointerToItemDoesNotExist, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Pointer to Net", inNet);
		throw e;
	}
	if(inNet->getSize() != getSize()) {
		Error e(eMessageIdErrorItemSizeMismatch, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Net Size", inNet->getSize());
		e.saveContextData("Connectable object Size", getSize());
		throw e;
	}
	return Connectable::connect(inNet);
}

void ScalarNet::disconnect(const Connection& inConnection) throw (Error) {
	Connectable::disconnect(inConnection);
}

ScalarNet::ScalarNet() : Net(), Scalar<Net>() {}

ScalarNet::~ScalarNet() throw () {}

#ifdef GENOM_SERIALIZATION
template <class Archive> void ScalarNet::serialize(Archive& ar, unsigned int) {
	ar & boost::serialization::base_object< Net >(*this);
	ar & boost::serialization::base_object< Scalar<Net> >(*this);
}

//TO SATISFY THE LINKER
template void ScalarNet::serialize<boost::archive::binary_iarchive>(
	boost::archive::binary_iarchive& ar, const unsigned int);

template void ScalarNet::serialize<boost::archive::binary_oarchive>(
	boost::archive::binary_oarchive& ar, const unsigned int);

#endif //GENOM_SERIALIZATION
}// namespace generic
} // namespace torc
