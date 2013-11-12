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
#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/is_abstract.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/list.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/Log.hpp"
#include "torc/generic/Net.hpp"
#include "torc/generic/Port.hpp"
#include "torc/generic/PortReference.hpp"
#include "torc/generic/PortList.hpp"
#include "torc/generic/View.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_IS_ABSTRACT(torc::generic::Composite< torc::generic::Net >)
#endif //GENOM_SERIALIZATION
namespace torc {
namespace generic {

Net::Net() :
	Composite<Net>(), Commentable(), Connectable(), Nameable(), PropertyContainer(), Renamable(), Visitable(), ParentedObject<
		View>(), UserDataContainer(), mSubnets(), mConnectedPorts(), mConnectedPortLists(), mConnectedPortRefs(), mParentNet(), mAttributes() {}

Net::~Net() throw () {
	try {
		disconnect();
	} catch(Error& e) {
		log("Cannot propagate received error\n");
	}
}

/**
 * Add a subnet to this net.
 *
 * @param[in] inSubnet Subnet to be added
 *
 * @exception Error Could not add subnet, because subnet name is empty
 * @exception Error Could not add subnet, because subnet name is already exists
 */
void Net::addSubnet(const NetSharedPtr& inSubnet) throw (Error) {
	std::string name = inSubnet->getName();
	if(name.empty()) {
		Error e(eMessageIdErrorEmptyItemName, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Subnet name", name);
		throw e;
	}
	if(false == mSubnets.set(name, inSubnet)) {
		Error e(eMessageIdErrorItemAlreadyExists, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Subnet name", name);
		throw e;
	}
	inSubnet->setParentNet(getSharedThis());
	return;
}

/**
 * Find a subnet from belonging to this net.
 *
 * @param[in] inName Name of the subnet. If not found, empty pointer is returned
 *
 */
NetSharedPtr Net::findSubnet(const std::string& inName) throw (Error) {
	NetSharedPtr subnet;
	mSubnets.get(inName, subnet);
	return subnet;
}

/**
 * Remove a subnet from this net.
 *
 * @param inName Name of the object to be delete
 *
 * @exception Error Could not remove subnet, because subnet name is empty
 * @exception Error Could not remove subnet, because subnet not present.
 */
void Net::removeSubnet(const std::string& inName) throw (Error) {
	if(inName.empty()) {
		Error e(eMessageIdErrorEmptyItemName, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Subnet name", inName);
		throw e;
	}
	if(false == mSubnets.remove(inName)) {
		Error e(eMessageIdErrorItemNotFound, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Subnet name", inName);
		throw e;
	}
	return;
}

void Net::setParentNet(const NetSharedPtr& inParent) {
	mParentNet = inParent;
}

/**
 * Set the attributes of the net. Attributes include criticality, netDelay etc.
 *
 * @param[in] inSource Pointer to NetAttributes object.
 */
void Net::setAttributes(const NetAttributesSharedPtr& inSource) {
	mAttributes = inSource;
}

void Net::setSubnets(const std::vector<NetSharedPtr>& inSource) throw (Error) {
	std::vector<NetSharedPtr>::const_iterator subnet = inSource.begin();
	std::vector<NetSharedPtr>::const_iterator subEnd = inSource.end();
	for(; subnet != subEnd; ++subnet) {
		try {
			addSubnet(*subnet);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

/**
 * Connect a port to this net.
 *
 * @param[in] inPort port to be added. Empty pointer is ignored.
 *
 * @exception Error Could not add Port, because Port name is empty
 * @exception Error Could not add Port, because Port name is already exists
 */
void Net::addConnectedPort(const PortSharedPtr& inPort) throw (Error) {
	if(!inPort) {
		return;
	}
	std::string name = inPort->getName();
	if(name.empty()) {
		Error e(eMessageIdErrorEmptyItemName, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Port name", name);
		throw e;
	}
	mConnectedPorts.push_back(inPort);
	return;
}

/**
 * Find a port connected to this net.
 *
 * @param[in] inName Name of the port. If not found, empty pointer is returned
 */
PortSharedPtr Net::findConnectedPort(const std::string& inName) throw (Error) {
	PortSharedPtr port;
	std::list<PortSharedPtr>::iterator it = find_if(
		mConnectedPorts.begin(),
		mConnectedPorts.end(),
		boost::bind<bool>(std::equal_to<std::string>(),
			boost::bind(boost::mem_fn(&Port::getName), _1), inName));
	if(it != mConnectedPorts.end()) {
		port = *it;
	}
	return port;
}

void Net::getConnectedPorts(std::vector<PortSharedPtr>& outPorts,
	bool inSkipChildConnections) const {
	outPorts.insert(outPorts.end(), mConnectedPorts.begin(), mConnectedPorts.end());
	return;
}

void Net::removeConnectedPort(const PortSharedPtr& inPort) throw (Error) {
	std::list<PortSharedPtr>::iterator it = find(mConnectedPorts.begin(), mConnectedPorts.end(),
		inPort);
	if(it != mConnectedPorts.end()) {
		mConnectedPorts.erase(it);
	} else {
		Error e(eMessageIdErrorItemNotFound, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Name", inPort->getName());
		throw e;
	}
	return;

}

void Net::setConnectedPorts(const std::vector<PortSharedPtr>& inSource) throw (Error) {
	std::vector<PortSharedPtr>::const_iterator port = inSource.begin();
	std::vector<PortSharedPtr>::const_iterator portEnd = inSource.end();
	for(; port != portEnd; ++port) {
		try {
			addConnectedPort(*port);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

}

void Net::getConnectedPortLists(std::vector<PortListSharedPtr>& outPortList) const {
	outPortList.insert(outPortList.end(), mConnectedPortLists.begin(), mConnectedPortLists.end());
	return;
}

void Net::addConnectedPortList(const PortListSharedPtr& inPortList) throw (Error) {
	if(!inPortList) {
		return;
	}
	mConnectedPortLists.push_back(inPortList);
	return;
}

void Net::removeConnectedPortList(const PortListSharedPtr& inPortList) throw (Error) {
	std::list<PortListSharedPtr>::iterator it = find(mConnectedPortLists.begin(),
		mConnectedPortLists.end(), inPortList);
	if(it != mConnectedPortLists.end()) {
		mConnectedPortLists.erase(it);
	} else {
		Error e(eMessageIdErrorItemNotFound, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Name", std::string("PortList"));
		throw e;
	}
	return;

}

/**
 * Connect a port reference to this net.
 *
 * @param[in] inPortRef Port reference to be added
 *
 * @exception Error Could not add port reference
 */
void Net::addConnectedPortReference(const PortReferenceSharedPtr& inPortRef) throw (Error) {
	if(!inPortRef) {
		return;
	}
	std::string name = inPortRef->getName();
	if(name.empty()) {
		Error e(eMessageIdErrorEmptyItemName, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("PortReference name", name);
		throw e;
	}
	mConnectedPortRefs.push_back(inPortRef);
	return;
}

/**
 * Find a port reference connected to this net.
 *
 * @param[in] inName Name of the port reference. If not found, empty pointer is returned
 */
PortReferenceSharedPtr Net::findConnectedPortReference(const std::string& inName) throw (Error) {
	PortReferenceSharedPtr portRef;
	std::list<PortReferenceSharedPtr>::iterator it = find_if(
		mConnectedPortRefs.begin(),
		mConnectedPortRefs.end(),
		boost::bind<bool>(std::equal_to<std::string>(),
			boost::bind(boost::mem_fn(&PortReference::getName), _1), inName));
	if(it != mConnectedPortRefs.end()) {
		portRef = *it;
	}
	return portRef;
}

void Net::getConnectedPortRefs(std::vector<PortReferenceSharedPtr>& outPortRefs,
	bool inSkipChildConnections) const {
	outPortRefs.insert(outPortRefs.end(), mConnectedPortRefs.begin(), mConnectedPortRefs.end());
	return;
}

void Net::removeConnectedPortReference(const PortReferenceSharedPtr& inPortRef) throw (Error) {
	std::list<PortReferenceSharedPtr>::iterator it = find(mConnectedPortRefs.begin(),
		mConnectedPortRefs.end(), inPortRef);
	if(it != mConnectedPortRefs.end()) {
		mConnectedPortRefs.erase(it);
	} else {
		Error e(eMessageIdErrorItemNotFound, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("PortReference name", inPortRef->getName());
		throw e;
	}
	return;
}

void Net::setConnectedPortRefs(const std::vector<PortReferenceSharedPtr>& inSource) throw (Error) {
	std::vector<PortReferenceSharedPtr>::const_iterator portRef = inSource.begin();
	std::vector<PortReferenceSharedPtr>::const_iterator portRefEnd = inSource.end();
	for(; portRef != portRefEnd; ++portRef) {
		try {
			addConnectedPortReference(*portRef);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

void Net::disconnect() throw (Error) {
	if(!getName().empty()) {
		log("Clearing net %s\n", getName().c_str());
	}
	mSubnets.clear();
	//Don't try to optimize the following by using applyOnAll
	// it causes memory corruption as the SymTab itself changes
	// during disconnection
	std::vector<PortSharedPtr> ports;
	getConnectedPorts(ports, true);
	void (Connectable::*fp)(const NetSharedPtr& net)
	= &Connectable::disconnect;
	std::for_each(ports.begin(), ports.end(), boost::bind(fp, _1, getSharedThis()));
	std::vector<PortReferenceSharedPtr> portRefs;
	getConnectedPortRefs(portRefs, true);
	std::for_each(portRefs.begin(), portRefs.end(), boost::bind(fp, _1, getSharedThis()));
	std::vector<PortListSharedPtr> portLists;
	getConnectedPortLists(portLists);
	std::for_each(portLists.begin(), portLists.end(), boost::bind(fp, _1, getSharedThis()));
	Connectable::disconnect();
}

#ifdef GENOM_SERIALIZATION
template <class Archive> void Net::serialize(Archive& ar, unsigned int) {
	ar & boost::serialization::base_object < Commentable > (*this);
	ar & boost::serialization::base_object < Connectable > (*this);
	ar & boost::serialization::base_object < Nameable > (*this);
	ar & boost::serialization::base_object < PropertyContainer > (*this);
	ar & boost::serialization::base_object < Renamable > (*this);
	ar & boost::serialization::base_object < Visitable > (*this);
	ar & boost::serialization::base_object < Composite<Net> > (*this);
	ar & mSubnets;
	ar & mConnectedPorts;
	ar & mConnectedPortRefs;
	ar & mConnectedPortLists;
}
//TO SATISFY THE LINKER
template void Net::serialize<boost::archive::binary_iarchive>(boost::archive::binary_iarchive& ar,
	const unsigned int);

template void Net::serialize<boost::archive::binary_oarchive>(boost::archive::binary_oarchive& ar,
	const unsigned int);

#endif //GENOM_SERIALIZATION
} // namespace generic
} // namespace torc
