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

#include <iostream>
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
#include "torc/generic/ConnectionHandler.hpp"
#include "torc/generic/Net.hpp"
#include "torc/generic/ScalarPort.hpp"
#include "torc/generic/PortBundle.hpp"
#include "torc/generic/VisitorType.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT(torc::generic::ScalarPort)
#endif //GENOM_SERIALIZATION
namespace torc {
namespace generic {

/**
 * Create a scalar port.
 *
 * @param[in] inName Name of the scalar port to be created.
 * @param[in] inDirection Direction of port.
 * @param[in] inViewPtr Pointer to parented(View) object.
 * @param[in] inOriginalName Original name of the scalar port [optional].
 *
 * @return Pointer to created scalar port.
 **/
ScalarPortSharedPtr ScalarPort::Factory::newScalarPortPtr(const std::string& inName,
	const EPortDirection& inDirection, const ViewSharedPtr& inViewPtr,
	const PortBundleSharedPtr& inParentCollection, const std::string& inOriginalName)
	throw (Error) {
	try {
		ScalarPortSharedPtr newScalarPort;
		create(newScalarPort);
		newScalarPort->setName(inName);
		newScalarPort->setDirection(inDirection);
		newScalarPort->setOriginalName(inOriginalName);
		if(inParentCollection) {
			inParentCollection->addChild(newScalarPort);
		} else {
			inViewPtr->addPort(newScalarPort);
		}
		return newScalarPort;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

void ScalarPort::accept(BaseVisitor& inoutVisitor) throw (Error) {
	try {
		runVisitor(*this, inoutVisitor);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

Connectable::Connection ScalarPort::connect(const NetSharedPtr& inNet) throw (Error) {
	if(!inNet) {
		Error e(eMessageIdErrorPointerToItemDoesNotExist, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Pointer to the Net object does not exist", inNet);
		throw e;
	}
	if(inNet->getSize() != getSize()) {
		Error e(eMessageIdErrorItemSizeMismatch, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Net Size", inNet->getSize());
		e.saveContextData("Scalar port Size", getSize());
		throw e;
	}
	Connectable::Connection newConnection;
	try {
		ConnectionHandler handler(inNet);
		handler.connectPortToNet(getSharedThis());
		newConnection = Connectable::connect(inNet);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
	return newConnection;
}

void ScalarPort::disconnect(const Connection& inConnection) throw (Error) {
	NetSharedPtr net = *inConnection;
	if(!net) {
		Error e(eMessageIdErrorConnectionInvalid, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Net is invalid", net);
		throw e;
	}
	try {
		ConnectionHandler handler(net);
		handler.disconnectPortFromNet(getSharedThis());
		Connectable::disconnect(inConnection);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
	return;
}

ScalarPort::ScalarPort() : Port(), Scalar<Port>() {}

ScalarPort::~ScalarPort() throw () {}

#ifdef GENOM_SERIALIZATION
template <class Archive> void ScalarPort::serialize(Archive& ar, unsigned int) {
	ar & boost::serialization::base_object<Port>(*this);
	ar & boost::serialization::base_object<
	Scalar<Port> >(*this);
}

//TO SATISFY THE LINKER
template void ScalarPort::serialize<boost::archive::binary_iarchive>(
	boost::archive::binary_iarchive& ar, const unsigned int);

template void ScalarPort::serialize<boost::archive::binary_oarchive>(
	boost::archive::binary_oarchive& ar, const unsigned int);

#endif //GENOM_SERIALIZATION

}// namespace generic
} // namespace torc
