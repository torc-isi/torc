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
#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>
#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/ConnectionHandler.hpp"
#include "torc/generic/Net.hpp"
#include "torc/generic/Port.hpp"
#include "torc/generic/PortBundleReference.hpp"
#include "torc/generic/PortRefCreator.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT(torc::generic::PortBundleReference)
#endif //GENOM_SERIALIZATION
#include <iostream>

namespace torc {
namespace generic {

PortBundleReferenceSharedPtr PortBundleReference::Factory::newPortBundleReferencePtr(
	const InstanceSharedPtr& inInstancePtr, const PortSharedPtr& inPortPtr,
	const ObjectFactorySharedPtr& inFactory, const PortBundleReferenceSharedPtr& inParentCollection)
		throw (Error) {
	try {
		PortBundleReferenceSharedPtr newPortBundleReference;
		PortRefCreator<PortBundleReferenceSharedPtr> creator(inFactory, inInstancePtr,
			inParentCollection);
		inPortPtr->getParent()->applyOnAllPorts(
			VisitorApplier<PortRefCreator<PortBundleReferenceSharedPtr> >(creator));
		newPortBundleReference = creator.getReturnValue();
		newPortBundleReference->bindToMasterPort(inPortPtr);
		return newPortBundleReference;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}

}

void PortBundleReference::accept(BaseVisitor& inoutVisitor) throw (Error) {
	try {
		runVisitor(*this, inoutVisitor);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

void PortBundleReference::setParent(const InstanceSharedPtr& inParent) {
	ParentedObject<Instance>::setParent(inParent);
	applyOnAllChildren(boost::bind(boost::mem_fn(&PortReference::setParent), _1, getParent()));
}

Connectable::Connection PortBundleReference::connect(const NetSharedPtr& inNet) throw (Error) {
	if(getSize() != getMaster()->getSize()) {
		Error e(eMessageIdErrorItemSizeMismatch, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("PortBundleReference Size", getSize());
		e.saveContextData("Master Port Size", getMaster()->getSize());
		throw e;
	}
	if(inNet->getSize() != getSize()) {
		Error e(eMessageIdErrorItemSizeMismatch, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Net Size", inNet->getSize());
		e.saveContextData("PortBundleReference Size", getSize());
		throw e;
	}
	Bundle<PortReference>::List children;
	BundleFlattener<PortReference, ScalarPortReference, VectorPortReference, VectorPortBitReference,
		PortBundleReference> flattener;
	accept(flattener);
	flattener.getChildren(children);

	Bundle<Net>::List netChildren;
	inNet->getChildren(netChildren);
	Bundle<PortReference>::List::iterator childPortRef = children.begin();
	Bundle<PortReference>::List::iterator childEnd = children.end();
	Bundle<Net>::List::iterator net = netChildren.begin();
	for(; childPortRef != childEnd; ++childPortRef, ++net) {
		try {
			(*childPortRef)->connect(*net);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
	Connectable::Connection newConnection;
	try {
		ConnectionHandler handler(inNet);
		handler.connectPortRefToNet(getSharedThis());
		newConnection = Connectable::connect(inNet);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
	return newConnection;
}

void PortBundleReference::disconnect(const Connection& inConnection) throw (Error) {
	NetSharedPtr connNet = *inConnection;
	if(!connNet) {
		Error e(eMessageIdErrorConnectionInvalid, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Pointer to Net", connNet);
		throw e;
	}
	Bundle<PortReference>::List children;
	BundleFlattener<PortReference, ScalarPortReference, VectorPortReference, VectorPortBitReference,
		PortBundleReference> flattener;
	accept(flattener);
	flattener.getChildren(children);

	Bundle<Net>::List netChildren;
	connNet->getChildren(netChildren);
	Bundle<PortReference>::List::iterator childPortRef = children.begin();
	Bundle<PortReference>::List::iterator childEnd = children.end();
	Bundle<Net>::List::iterator net = netChildren.begin();
	//We find all bits from the connected net and remove them
	//from the children nets. This disconnect is a little slow
	//as lookup is performed for each net. Otherwise we
	//would have to save the iterators (Connection objects)
	//for each child connection
	for(; childPortRef != childEnd; ++childPortRef, ++net) {
		try {
			(*childPortRef)->disconnect(*net);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
	try {
		ConnectionHandler handler(connNet);
		handler.disconnectPortRefFromNet(getSharedThis());
		Connectable::disconnect(inConnection);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
	return;
}

/**
 * Set master port.
 *
 * @param[in] inSource Set the master port
 *
 */
void PortBundleReference::bindToMasterPort(const PortSharedPtr& inMaster) throw (Error) {
	if(inMaster->getSize() != getSize()) {
		Error e(eMessageIdErrorItemSizeMismatch, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Master Port Size", inMaster->getSize());
		e.saveContextData("Port bundle reference Size", getSize());
		throw e;
	}
	PortReference::bindToMasterPort(inMaster);
#if 0 //Bind during reference creation
	std::vector< PortReferenceSharedPtr > children;
	getChildren(children);
	std::vector< PortSharedPtr > childrenPort;
	inMaster->getChildren(childrenPort); //TBD::OPTMIZE
	std::vector< PortReferenceSharedPtr >::iterator pRef = children.begin();
	std::vector< PortReferenceSharedPtr >::iterator pRefEnd = children.end();
	std::vector< PortSharedPtr >::iterator pIt = childrenPort.begin();
	for(; pRef != pRefEnd; ++pRef, ++pIt ) {
		PortReferenceSharedPtr childRef;
		PortSharedPtr childPort = *pIt;
		try {
			childRef->bindToMasterPort(childPort);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
#endif    
}

PortBundleReference::PortBundleReference() : PortReference(), Bundle<PortReference>() {}

PortBundleReference::~PortBundleReference() throw () {}

#ifdef GENOM_SERIALIZATION
template <class Archive> void PortBundleReference::serialize(Archive& ar, unsigned int) {
	ar & boost::serialization::base_object<PortReference >(*this);
	ar & boost::serialization::base_object<Bundle<PortReference> >(*this);
}

//TO SATISFY THE LINKER
template void PortBundleReference::serialize<boost::archive::binary_iarchive>(
	boost::archive::binary_iarchive& ar, const unsigned int);

template void PortBundleReference::serialize<boost::archive::binary_oarchive>(
	boost::archive::binary_oarchive& ar, const unsigned int);

#endif //GENOM_SERIALIZATION
}// namespace generic
} // namespace torc
