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
#include "torc/generic/NetBundle.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT(torc::generic::NetBundle)
#endif //GENOM_SERIALIZATION

namespace torc {
namespace generic {

/**
 * Create a net bundle.
 *
 * @param[in] inName Name of the net bundle to be created.
 * @param[in] inViewPtr Pointer to parented(View) object.
 * @param[in] inOriginalName Original name of the vector net [optional].
 *
 * @return Pointer to created net bundle.
 **/
NetBundleSharedPtr NetBundle::Factory::newNetBundlePtr(const std::string& inName,
	const ViewSharedPtr& inViewPtr, const NetBundleSharedPtr& inParentCollection,
	const std::string& inOriginalName) throw (Error) {
	try {
		NetBundleSharedPtr newNetBundle;
		create(newNetBundle);
		newNetBundle->setName(inName);
		newNetBundle->setOriginalName(inOriginalName);
		if(inParentCollection) {
			inParentCollection->addChild(newNetBundle);
		} else {
			inViewPtr->addNet(newNetBundle);
		}
		return newNetBundle;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

void NetBundle::accept(BaseVisitor& inoutVisitor) throw (Error) {
	runVisitor(*this, inoutVisitor);
}

void NetBundle::setParent(const ViewSharedPtr& inParent) {
	ParentedObject<View>::setParent(inParent);
	applyOnAllChildren(boost::bind(boost::mem_fn(&Net::setParent), _1, getParent()));
}

//For net to net connection
Connectable::Connection NetBundle::connect(const NetSharedPtr& inNet) throw (Error) {
	if(!inNet) {
		Error e(eMessageIdErrorPointerToItemDoesNotExist, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Pointer to Net", inNet);
		throw e;
	}
	if(inNet->getSize() != getSize()) {
		Error e(eMessageIdErrorItemSizeMismatch, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Net Size", inNet->getSize());
		e.saveContextData("NetBundle Size", getSize());
		throw e;
	}
	Bundle<Net>::List children;
	getChildren(children);
	Bundle<Net>::List netChildren;
	inNet->getChildren(netChildren);
	Bundle<Net>::List::iterator childNet = children.begin();
	Bundle<Net>::List::iterator childEnd = children.end();
	Bundle<Net>::List::iterator net = netChildren.begin();
	for(; childNet != childEnd; ++childNet, ++net) {
		try {
			(*childNet)->connect(*net);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
	return Connectable::connect(inNet);
}

void NetBundle::disconnect(const Connection& inConnection) throw (Error) {
	NetSharedPtr connNet = *inConnection;
	Bundle<Net>::List children;
	getChildren(children);
	Bundle<Net>::List netChildren;
	connNet->getChildren(netChildren);
	Bundle<Net>::List::iterator childNet = children.begin();
	Bundle<Net>::List::iterator childEnd = children.end();
	Bundle<Net>::List::iterator net = netChildren.begin();
	//We find all bits from the connected net and remove them
	//from the children nets. This disconnect is a little slow
	//as lookup is performed for each net. Otherwise we
	//would have to save the iterators (Connection objects)
	//for each child connection
	for(; childNet != childEnd; ++childNet, ++net) {
		try {
			(*childNet)->Connectable::disconnect(*net);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
	Connectable::disconnect(inConnection);
}

void NetBundle::disconnect() throw (Error) {
	Bundle<Net>::List children;
	getChildren(children);
	Bundle<Net>::List::iterator childNet = children.begin();
	Bundle<Net>::List::iterator childEnd = children.end();
	for(; childNet != childEnd; ++childNet) {
		try {
			(*childNet)->disconnect();
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
	Net::disconnect();
}

NetBundle::NetBundle() : Net(), Bundle<Net>() {}

NetBundle::~NetBundle() throw () {}

#ifdef GENOM_SERIALIZATION
template <class Archive> void NetBundle::serialize(Archive& ar, unsigned int) {
	ar & boost::serialization::base_object< Net >(*this);
	ar & boost::serialization::base_object< Bundle<Net> >(*this);
}

//TO SATISFY THE LINKER
template void NetBundle::serialize<boost::archive::binary_iarchive>(
	boost::archive::binary_iarchive& ar, const unsigned int);

template void NetBundle::serialize<boost::archive::binary_oarchive>(
	boost::archive::binary_oarchive& ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace generic
} // namespace torc
