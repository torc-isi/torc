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

#ifndef TORC_GENERIC_NET_HPP
#define TORC_GENERIC_NET_HPP

#include "torc/generic/PointerTypes.hpp"

#include <algorithm>
#include <vector>

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/Commentable.hpp"
#include "torc/generic/Composite.hpp"
#include "torc/generic/Connectable.hpp"
#include "torc/generic/Error.hpp"
#include "torc/generic/Nameable.hpp"
#include "torc/generic/ParentedObject.hpp"
#include "torc/generic/PropertyContainer.hpp"
#include "torc/generic/Renamable.hpp"
#include "torc/generic/SymTab.hpp"
#include "torc/generic/Visitable.hpp"
#include "torc/generic/View.hpp"
#include "torc/generic/UserDataContainer.hpp"

namespace torc { namespace generic { class ConnectionHandler; } }
namespace torc { namespace generic { class Port; } }
namespace torc { namespace generic { class PortReference; } }
namespace torc { namespace generic { class NetAttributes; } }

namespace torc {
namespace generic {

/**
 * @brief Represents an EDIF Net
 *
 * The Net class is an interface to EDIF nets. Net is an interface for scalar nets, vector nets,
 * vector nets bits and net bundles. This class provides a simplified view of all three types of
 * nets. To get a handle to an actual net, the client will have to use a Visitor with proper
 * overloads of the visit() function, or dynamic_cast can be used. The latter method is however not
 * recommended. A net object supports storing properties for storing different EDIF properties. See
 * documentation of the Property and PropertyContainer classes for more details.
 */
class Net : public Commentable, public Connectable, public Nameable, public PropertyContainer,
	public Renamable, public Visitable, virtual public Composite<Net>, public ParentedObject<View>,
	public UserDataContainer {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION
	friend class ConnectionHandler;

public:
	/**
	 * Add a subnet to this net.
	 *
	 * @param[in] inSubnet Subnet to be added
	 *
	 * @exception Error Could not add subnet, because subnet name is empty
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorEmptyItemName
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Subnet name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 * @exception Error Could not add subnet, because subnet name is already exists
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorItemAlreadyExists
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Subnet name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 */
	void addSubnet(const NetSharedPtr& inSubnet) throw (Error);

	/**
	 * Find a subnet from belonging to this net.
	 *
	 * @param[in] inName Name of the subnet. If not found, empty pointer is returned
	 *
	 */
	NetSharedPtr findSubnet(const std::string& inName) throw (Error);

	/**
	 * Remove a subnet from this net.
	 *
	 * @param inName Name of the object to be delete
	 *
	 * @exception Error Could not remove subnet, because subnet name is empty
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorEmptyItemName
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Subnet name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 * @exception Error Could not remove subnet, because subnet not present.
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorItemNotFound
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Subnet name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 */
	void removeSubnet(const std::string& inName) throw (Error);

	inline void getSubnets(std::vector<NetSharedPtr>& outSubnets) const;

	void setSubnets(const std::vector<NetSharedPtr> & inSource) throw (Error);
	/**
	 * Apply action on all Subnets.
	 * @param[in] action Action to be applied
	 *
	 */
	template <typename _Action> inline void applyOnAllSubnets(const _Action& action) throw (Error);

	inline NetSharedPtr getParentNet() const;

	void setParentNet(const NetSharedPtr& inParent);

	/**
	 * Get the attributes of the net. Attributes include criticality, netDelay etc.
	 *
	 * @return Pointer to NetAttributes object.
	 */
	inline const NetAttributesSharedPtr getAttributes() const;

	/**
	 * Set the attributes of the net. Attributes include criticality, netDelay etc.
	 *
	 * @param[in] inSource Pointer to NetAttributes object.
	 */
	void setAttributes(const NetAttributesSharedPtr& inSource);

protected:
	void addConnectedPort(const PortSharedPtr& inPort) throw (Error);

	void removeConnectedPort(const PortSharedPtr& inPort) throw (Error);

	void addConnectedPortList(const PortListSharedPtr& inPort) throw (Error);

	void removeConnectedPortList(const PortListSharedPtr& inList) throw (Error);

public:
	/**
	 * Find a port connected to this net.
	 *
	 * @param[in] inName Name of the port. If not found, empty pointer is returned
	 */
	PortSharedPtr findConnectedPort(const std::string& inName) throw (Error);

	/**
	 * Appends vector of connected ports
	 * @param[in] outPort vector of port to be appended to
	 * @param[in] inSkipChildConnections Return only ports connected to this port and skip
	 * returning elements that have been connected to the child
	 */
	virtual void getConnectedPorts(std::vector<PortSharedPtr>& outPort,
		bool inSkipChildConnections = false) const;

	/**
	 * Apply action on all connected ports.
	 * @param[in] action Action to be applied
	 *
	 */
	template <typename _Action> inline void applyOnAllConnectedPorts(const _Action& action)
		throw (Error);

	/**
	 * Appends vector of connected port lists
	 * @param[in] outPortList vector of port lists to be appended to
	 */
	virtual void getConnectedPortLists(std::vector<PortListSharedPtr>& outPortList) const;

	/**
	 * Apply action on all connected port lists.
	 * @param[in] action Action to be applied
	 *
	 */
	template <typename _Action> inline void applyOnAllConnectedPortLists(const _Action& action)
		throw (Error);

	using Connectable::disconnect;

	virtual void disconnect() throw (Error);

protected:
	//TBD::UNUSED?
	void
	setConnectedPorts(const std::vector<PortSharedPtr>& inSource) throw (Error);

protected:

	void addConnectedPortReference(const PortReferenceSharedPtr& portRef) throw (Error);

	void removeConnectedPortReference(const PortReferenceSharedPtr& inPortRef) throw (Error);

public:
	/**
	 * Find a port reference connected to this net.
	 *
	 * @param[in] inName Name of the port reference. If not found, empty pointer is returned
	 */
	PortReferenceSharedPtr findConnectedPortReference(const std::string& inName) throw (Error);

	/**
	 * Appends vector of connected port references
	 * @param[in] outPortRefs vector of port refs to be appended to
	 */
	virtual void getConnectedPortRefs(std::vector<PortReferenceSharedPtr>& outPortRefs,
		bool inSkipChildConnections = false) const;

	/**
	 * Apply action on all connected port refs.
	 * @param[in] action Action to be applied
	 *
	 */
	template <typename _Action> inline void applyOnAllConnectedPortRefs(const _Action& action)
		throw (Error);

protected:
	Net();

public:
	virtual ~Net() throw ();

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
	Net(const Net& source);

	Net& operator=(const Net& source);

protected:
	//TBD::UNUSED?
	void setConnectedPortRefs(const std::vector<PortReferenceSharedPtr>& inSource) throw (Error);

private:
	SymTab<std::string, NetSharedPtr, true> mSubnets;
	std::list<PortSharedPtr> mConnectedPorts;
	std::list<PortListSharedPtr> mConnectedPortLists;
	std::list<PortReferenceSharedPtr> mConnectedPortRefs;
	NetSharedPtr mParentNet;
	NetAttributesSharedPtr mAttributes;
};

inline void Net::getSubnets(std::vector<NetSharedPtr>& outSubnets) const {
	mSubnets.getValues(outSubnets);
	return;
}

template <typename _Action> inline void Net::applyOnAllSubnets(const _Action& action)
	throw (Error) {
	try {
		mSubnets.applyOnAll(action);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

inline NetSharedPtr Net::getParentNet() const {
	return mParentNet;
}

/**
 * Get the attributes of the net. Attributes include criticality, netDelay etc.
 *
 * @return Pointer to NetAttributes object.
 */
inline const NetAttributesSharedPtr Net::getAttributes() const {
	return mAttributes;
}

template <typename _Action> inline void Net::applyOnAllConnectedPorts(const _Action& action)
	throw (Error) {
	try {
		std::for_each(mConnectedPorts.begin(), mConnectedPorts.end(), action);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <typename _Action> inline void Net::applyOnAllConnectedPortLists(const _Action& action)
	throw (Error) {
	try {
		std::for_each(mConnectedPortLists.begin(), mConnectedPortLists.end(), action);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <typename _Action> inline void Net::applyOnAllConnectedPortRefs(const _Action& action)
	throw (Error) {
	try {
		std::for_each(mConnectedPortRefs.begin(), mConnectedPortRefs.end(), action);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_NET_HPP
