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
#include <boost/serialization/list.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/PortList.hpp"
#include "torc/generic/Net.hpp"
#include "torc/generic/Port.hpp"
#include "torc/generic/PortReference.hpp"
#include "torc/generic/ConnectionHandler.hpp"

namespace torc {
namespace generic {

/**
 * Create a port list.
 *
 * @param[in] inPorts List of ports to this composition.
 * @param[in] inPortReferences List of port references to this composition.
 *
 * @return Pointer to created  port list.
 */

PortListSharedPtr PortList::Factory::newPortListPtr(const std::list<PortSharedPtr>& inPorts,
	const std::list<PortReferenceSharedPtr>& inPortReferences) throw (Error) {
	try {
		PortListSharedPtr newPortList;
		create(newPortList);
		std::list<PortSharedPtr>::const_iterator portIt = inPorts.begin();
		for(; portIt != inPorts.end(); portIt++) {
			newPortList->addChildPort(*portIt);
		}
		std::list<PortReferenceSharedPtr>::const_iterator portRefIt = inPortReferences.begin();
		for(; portRefIt != inPortReferences.end(); portRefIt++) {
			newPortList->addChildPortReference(*portRefIt);
		}
		return newPortList;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

void PortList::accept(BaseVisitor& inoutVisitor) throw (Error) {
	try {
		runVisitor(*this, inoutVisitor);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <typename _Tp> void PortList::connectElementToNet(const NetSharedPtr& inNet,
	const boost::shared_ptr<_Tp>& inType, size_t& inoutCurrentWidth) throw (Error) {
	try {
		typedef boost::shared_ptr<_Tp> Pointer;
		size_t updatedSize = inoutCurrentWidth + inType->getSize();
		if(updatedSize > inNet->getSize()) {
			//TBD::ERROR
		}
		//Element can be connected to scalar or Vector only
		switch(inNet->getCompositionType()) {
		case eCompositionTypeScalar:
		case eCompositionTypeVectorBit: {
			if(1 == inType->getSize()) {
				Connectable::Connection conn = inType->connect(inNet);
				inoutCurrentWidth++;
				(void) conn; // NJS
			}
			break;
		}
		case eCompositionTypeVector: {
			//We explode the vector net
			std::vector<NetSharedPtr> children;
			inNet->getChildren(children);

			switch(inType->getCompositionType()) {
			case eCompositionTypeScalar:
			case eCompositionTypeVectorBit:
				//One element vector net to a scalar port
			{
				connectElementToNet(children[inoutCurrentWidth], inType, inoutCurrentWidth);
				break;
			}
			case eCompositionTypeVector:
				//Part of vector net to vector port
			{
				std::vector<Pointer> cTypes;
				inType->getChildren(cTypes);
				size_t j = 0;
				for(size_t i = inoutCurrentWidth; i < updatedSize; i++, j++) {
					connectElementToNet(children[i], cTypes[j], inoutCurrentWidth);
				}
				break;
			}
			case eCompositionTypeBundle: {
				std::vector<Pointer> cTypes;
				inType->getChildren(cTypes);
				for(typename std::vector<Pointer>::iterator it = cTypes.begin(); it != cTypes.end();
					++it) {
					connectElementToNet(inNet, *it, inoutCurrentWidth);
				}
				break;
			}
			}
		}
		case eCompositionTypeBundle:
		default: {
			//NOT POSSIBLE
		}
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

Connectable::Connection PortList::connect(const NetSharedPtr& inNet) throw (Error) {
	size_t count = 0;
	for(std::list<PortListElement>::iterator it = mElements.begin(); it != mElements.end(); ++it) {
		try {
			switch((*it).getType()) {
			case PortListElement::eElementTypePort: {
				connectElementToNet(inNet, (*it).getPort(), count);
				break;
			}
			case PortListElement::eElementTypePortReference: {
				connectElementToNet(inNet, (*it).getPortReference(), count);
				break;
			}
			}
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
	Connectable::Connection newConnection;
	try {
		ConnectionHandler handler(inNet);
		handler.connectPortListToNet(getSharedThis());
		newConnection = Connectable::connect(inNet);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
	return newConnection;
}

void PortList::disconnect(const Connectable::Connection& inConnection) throw (Error) {
	NetSharedPtr inNet = *inConnection;
	//Do not clear components from here ...
	//They are connected to individual bits of some net
	// and would be cleared in due course
	try {
		ConnectionHandler handler(inNet);
		handler.disconnectPortListFromNet(getSharedThis());
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

size_t PortList::getSize() const {
	size_t size = 0;
	for(std::list<PortListElement>::const_iterator it = mElements.begin(); it != mElements.end();
		++it) {
		switch((*it).getType()) {
		case PortListElement::eElementTypePort: {
			size += (*it).getPort()->getSize();
			break;
		}
		case PortListElement::eElementTypePortReference: {
			size += (*it).getPortReference()->getSize();
			break;
		}
		}
	}
	return size;
}

void PortList::addChildPort(const PortSharedPtr& inPort) {
	mElements.push_back(PortListElement(inPort));
}

void PortList::addChildPortReference(const PortReferenceSharedPtr& inPortRef) {
	mElements.push_back(PortListElement(inPortRef));
}

void PortList::getChildren(std::list<PortListElement>& outElements) {
	outElements.insert(outElements.end(), mElements.begin(), mElements.end());
}

PortList::PortList() : Connectable(), mElements() {}

PortList::~PortList() throw () {
	mElements.clear();
}

#ifdef GENOM_SERIALIZATION

template <class Archive> void PortList::PortListElement::serialize(Archive& ar, unsigned int ) {
	ar & mType;
	ar & mPort;
	ar & mPortReference;
}

template <class Archive> void PortList::serialize(Archive& ar, unsigned int) {
	ar & boost::serialization::base_object<Connectable>(*this);
	ar & boost::serialization::base_object<SelfReferencing<PortList> >(*this);
	ar & mElements;
}

//TO SATISFY THE LINKER
template void PortList::serialize<boost::archive::binary_iarchive>(
	boost::archive::binary_iarchive& ar, const unsigned int);

template void PortList::serialize<boost::archive::binary_oarchive>(
	boost::archive::binary_oarchive& ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace generic
} // namespace torc
