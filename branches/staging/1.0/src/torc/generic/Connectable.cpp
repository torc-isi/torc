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

#include <algorithm>

//BOOST
#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/shared_ptr.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/Connectable.hpp"
#include "torc/generic/Net.hpp"
#include "torc/generic/Log.hpp"

namespace torc {
namespace generic {

/**
 * Get the vector of Nets that are Connected to the current object. The connected elements are
 * appended to the given vector
 *
 * @return A vector of Connected nets
 */
void Connectable::getConnectedNets(std::vector<NetSharedPtr>& outNets, bool inSkipChildConnections)
	const throw (Error) {
	outNets.insert(outNets.end(), mConnectedNets.begin(), mConnectedNets.end());
}

/**
 * Connect a Net to this object.
 *
 * @note This metod can be overridden by derived classes. However, the method must call the
 * on_connected() method after this. The sigConnected_ signal must also be invoked in the
 * overriding method.
 *
 * @param[in] inNet A pointer to the Net object that eeds to be Connected
 * @return A connection that has been established. This can be used later for disconnection.
 */
Connectable::Connection Connectable::connect(const NetSharedPtr& inNet) throw (Error) {
	Connection connection = mConnectedNets.insert(mConnectedNets.end(), inNet);
	try {
		onConnect();
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
	signalNetConnected()(inNet);
	return connection;
}

/**
 *
 * Disconnect a Net from this object.
 * @note This metod can be overridden by derived classes. However, the method must call the
 * on_connected() method after this. The sigConnected_ signal must also be invoked in the
 * overriding method.
 *
 * @param[in] inConnection A connection as returned by the connect() method
 * @exception Error Provided connection is invalid
 *
 */
void Connectable::disconnect(const Connectable::Connection& inConnection) throw (Error) {
	if(inConnection == mConnectedNets.end()) {
		return;
	}

	NetSharedPtr net = *inConnection;
	mConnectedNets.erase(inConnection);
	try {
		onDisconnect();
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
	signalNetDisconnected()(net);
	return;
}

/**
 *
 * Disconnect the named Net from this object.
 * @note This metod can be overridden by derived classes. However, the method must call the
 * on_connected() method after this. The sigConnected_ signal must also be invoked in the
 * overriding method.
 *
 * @param[in] inNname Name of the net to be Disconnected
 * @exception Error Provided net was not found
 *
 */
void Connectable::disconnect(const std::string& inName) throw (Error) {
	disconnect(std::find_if(mConnectedNets.begin(), mConnectedNets.end(),
		boost::bind<bool>(std::equal_to<std::string>(),
		boost::bind(boost::mem_fn(&Net::getName), _1), inName)));
}

/**
 *
 * Disconnect the given Net from this object.
 * @note This metod can be overridden by derived classes. However, the method must call the
 * on_disconnected() method after this. The sigDisconnected_ signal must also be invoked in the
 * overriding method.
 *
 * @param[in] inNet Pointer to a net
 * @exception Error Provided net was not found
 *
 */
void Connectable::disconnect(const NetSharedPtr& inNet) throw (Error) {
	disconnect(std::find(mConnectedNets.begin(), mConnectedNets.end(), inNet));
}

void Connectable::disconnect() throw (Error) {
	log("Disconnecting all\n");
	std::vector<NetSharedPtr> nets;
	getConnectedNets(nets, true);
	for(std::vector<NetSharedPtr>::iterator it = nets.begin(); it != nets.end(); ++it) {
		disconnect(*it);
	}
}

/**
 * A polymorphic function that is called after a net is Connected to this object
 */
void Connectable::onConnect() throw (Error) {}

/**
 * A polymorphic function that is called after a net is Disconnected from this object
 */
void Connectable::onDisconnect() throw (Error) {}

Connectable::Connectable() : mConnectedNets(), mSigNetConnected(), mSigNetDisconnected() {}

Connectable::~Connectable() throw () {
	mConnectedNets.clear();
}

#ifdef GENOM_SERIALIZATION
template <class Archive> void Connectable::serialize(Archive& ar, unsigned int) {
	ar & mConnectedNets;
}

//TO SATISFY THE LINKER
template void Connectable::serialize<boost::archive::binary_iarchive>(
	boost::archive::binary_iarchive& ar, const unsigned int);

template void Connectable::serialize<boost::archive::binary_oarchive>(
	boost::archive::binary_oarchive& ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace generic
} // namespace torc
