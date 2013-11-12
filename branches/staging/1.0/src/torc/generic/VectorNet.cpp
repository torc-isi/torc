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
#include "torc/generic/VectorNet.hpp"
#include "torc/generic/NetBundle.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT(torc::generic::VectorNet)
#endif //GENOM_SERIALIZATION
namespace torc {
namespace generic {

/**
 * Create a vector net.
 *
 * @param[in] inName Name of the vector net to be created.
 * @param[in] inViewPtr Pointer to parented(View) object.
 * @param[in] inSize Size of the net array.
 * @param[in] inFactory Factory for the child.
 * @param[in] inOriginalName Original name of the vector net [optional].
 *
 * @return Pointer to created vector net.
 **/
VectorNetSharedPtr VectorNet::Factory::newVectorNetPtr(const std::string& inName,
	const ViewSharedPtr& inViewPtr, const size_t& inSize,
	const NetBundleSharedPtr& inParentCollection, const ChildFactorySharedPtr& inFactory,
	const std::string& inOriginalName) throw (Error) {
	try {
		std::vector < size_t > limits;
		limits.push_back(inSize);
		return newVectorNetPtr(inName, inViewPtr, limits, inParentCollection, inFactory,
			inOriginalName);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

/**
 * Create a vector net.
 *
 * @param[in] inName Name of the vector net to be created.
 * @param[in] inViewPtr Pointer to parented(View) object.
 * @param[in] inLimits Dimensions of the vector.
 * @param[in] inFactory Factory for the child.
 * @param[in] inOriginalName Original name of the vector net [optional].
 *
 * @return Pointer to created vector net.
 **/
VectorNetSharedPtr VectorNet::Factory::newVectorNetPtr(const std::string& inName,
	const ViewSharedPtr& inViewPtr, const std::vector<size_t>& inLimits,
	const NetBundleSharedPtr& inParentCollection, const ChildFactorySharedPtr& inFactory,
	const std::string& inOriginalName) throw (Error) {
	try {
		VectorNetSharedPtr newVectorNet;
		create(newVectorNet);
		newVectorNet->setName(inName);
		newVectorNet->constructChildren(inFactory, inLimits);
		newVectorNet->setOriginalName(inOriginalName);
		if(inParentCollection) {
			inParentCollection->addChild(newVectorNet);
		} else {
			inViewPtr->addNet(newVectorNet);
		}
		return newVectorNet;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}
void VectorNet::accept(BaseVisitor& inoutVisitor) throw (Error) {
	try {
		runVisitor(*this, inoutVisitor);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

void VectorNet::setParent(const ViewSharedPtr& inParent) {
	ParentedObject<View>::setParent(inParent);
	BaseType::List children;
	getCreatedChildren(children);
	std::for_each(children.begin(), children.end(),
		boost::bind(boost::mem_fn(&Net::setParent), _1, getParent()));
}

/**
 * Connect a Net to this object.
 *
 * @note This metod can be overridden by derived classes. However, the method must call the
 * on_connected() method after this. The sigConnected_ signal must also be invoked in the
 * overriding method.
 *
 * @param[in] inNet A pointer to the Net object that eeds to be connected
 * @return A connection that has been established. This can be used later for disconnection.
 */
Connectable::Connection VectorNet::connect(const NetSharedPtr& inNet) throw (Error) {
	if(!inNet) {
		Error e(eMessageIdErrorPointerToItemDoesNotExist, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Pointer to the Net object does not exist", inNet);
		throw e;
	}
	if(inNet->getSize() != getSize()) {
		Error e(eMessageIdErrorItemSizeMismatch, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Net Size", inNet->getSize());
		e.saveContextData("Vector Net Size", getSize());
		throw e;
	}
	return Connectable::connect(inNet);
}

/**
 * Disconnect a Net from this object.
 * @note This metod can be overridden by derived classes. However, the method must call the
 * on_connected() method after this. The sigConnected_ signal must also be invoked in the
 * overriding method.

 * @param[in] inConnection A connection as returned by the connect() method
 * @exception Error Provided connection is invalid
 */
void VectorNet::disconnect(const Connectable::Connection& inConnection) throw (Error) {
	Connectable::disconnect(inConnection);
}

void VectorNet::disconnect() throw (Error) {
	Net::disconnect();
	BaseVectorType::List children;
	getCreatedChildren(children); //We need only the
	// bits that have been created
	BaseVectorType::List::iterator net = children.begin();
	BaseVectorType::List::iterator nEnd = children.end();
	for(; net != nEnd; ++net) {
		try {
			(*net)->disconnect();
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

void VectorNet::getConnectedNets(std::vector<NetSharedPtr>& outNets,
	bool inSkipChildConnections) const throw (Error) {
	Net::getConnectedNets(outNets);
	if(!inSkipChildConnections) {
		BaseVectorType::List children;
		getCreatedChildren(children); //We need only the
		// bits that have been created
		BaseVectorType::List::iterator net = children.begin();
		BaseVectorType::List::iterator nEnd = children.end();
		for(; net != nEnd; ++net) {
			(*net)->getConnectedNets(outNets, true);
		}
	}
	return;
}

void VectorNet::getConnectedPorts(std::vector<PortSharedPtr>& outPorts,
	bool inSkipChildConnections) const {
	Net::getConnectedPorts(outPorts);
	if(!inSkipChildConnections) {
		BaseVectorType::List children;
		getCreatedChildren(children); //We need only the
		// bits that have been created
		BaseVectorType::List::iterator net = children.begin();
		BaseVectorType::List::iterator nEnd = children.end();
		for(; net != nEnd; ++net) {
			(*net)->getConnectedPorts(outPorts, true);
		}
	}
	return;
}
void VectorNet::getConnectedPortRefs(std::vector<PortReferenceSharedPtr>& outPortRefs,
	bool inSkipChildConnections) const {
	Net::getConnectedPortRefs(outPortRefs);
	if(!inSkipChildConnections) {
		BaseVectorType::List children;
		getCreatedChildren(children); //We need only the
		// bits that have been created
		BaseVectorType::List::iterator net = children.begin();
		BaseVectorType::List::iterator nEnd = children.end();
		for(; net != nEnd; ++net) {
			(*net)->getConnectedPortRefs(outPortRefs, true);
		}
	}
	return;
}

void VectorNet::onChildCreate(
	const boost::shared_ptr<BaseVectorType::ChildType>& inCreatedChild) const throw (Error) {
	inCreatedChild->setName(getName());
}

VectorNet::VectorNet() : Net(), Vector<Net, VectorNetBit, VectorNetBit::Factory, true>() {}

VectorNet::~VectorNet() throw () {}

#ifdef GENOM_SERIALIZATION
template <class Archive> void VectorNet::serialize(Archive& ar, unsigned int) {
	ar & boost::serialization::base_object<Net>(*this);
	ar & boost::serialization::base_object<BaseVectorType>(*this);
}

//TO SATISFY THE LINKER
template void VectorNet::serialize<boost::archive::binary_iarchive>(
	boost::archive::binary_iarchive& ar, const unsigned int);

template void VectorNet::serialize<boost::archive::binary_oarchive>(
	boost::archive::binary_oarchive& ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace generic
} // namespace torc
