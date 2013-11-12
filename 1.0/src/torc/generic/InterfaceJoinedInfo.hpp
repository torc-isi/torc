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

#ifndef TORC_GENERIC_INTERFACEJOINEDINFO_HPP
#define TORC_GENERIC_INTERFACEJOINEDINFO_HPP

#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/Error.hpp"
#include "torc/generic/FactoryType.hpp"
#include "torc/generic/Visitable.hpp"
#include "torc/generic/VisitorType.hpp"
#include "torc/generic/SelfReferencing.hpp"

#include <list>
#include <vector>

namespace torc { namespace generic { class Port; } }
namespace torc { namespace generic { class PortList; } }
namespace torc { namespace generic { class BaseVisitor; } }

namespace torc {
namespace generic {

/**
 * @brief Represents the Interface joining information.
 * 
 * The InterfaceJoinedInfo class is used to hold interface joining information.
 * Joining information specify which ports (scalar port, array of ports, bundle of ports)
 * or port lists are shorted together.
 */
class InterfaceJoinedInfo : public Visitable, public SelfReferencing<InterfaceJoinedInfo> {
	friend class FactoryType<InterfaceJoinedInfo> ;

public:
	/**
	 * @enum JoinedType
	 * Interface joining type
	 */
	enum JoinedType {
		eJoinedTypeJoin = 0, eJoinedTypeWeak, eJoinedTypeMust
	};

	/**
	 * @enum RelationType
	 *
	 * Type of the InterfaceJoinedInfo, Is it parent or child.
	 */
	enum RelationType {
		eRelationTypeParent = 0, eRelationTypeChild
	};

	/**
	 * Convenience class to visit an interface joining information.
	 */
	typedef VisitorType<InterfaceJoinedInfo> Visitor;

	/**
	 * Convenience class to create an interface joining information.
	 */
	class Factory : public FactoryType<InterfaceJoinedInfo> {
	public:
		using FactoryType<InterfaceJoinedInfo>::create;
		/**
		 * Create an interface joining information .
		 *
		 * @param[in] inPorts List of ports to this joining information.
		 * @param[in] inPortLists List of portLists to this joining information.
		 * @param[in] inViewPtr Pointer to parented(View) object.
		 * @param[in] inParentJoinedInfo Pointer to parent joining information.
		 *
		 * @return Pointer to created interface joining information.
		 */
		InterfaceJoinedInfoSharedPtr virtual newInterfaceJoinedInfoPtr(
			const std::list<PortSharedPtr>& inPorts,
			const std::list<PortListSharedPtr>& inPortLists, const ViewSharedPtr& inViewPtr,
			const InterfaceJoinedInfoSharedPtr& inParentJoinedInfo = InterfaceJoinedInfoSharedPtr())
				throw (Error);
	};

	/**
	 * @brief Receive a visitor to this class.
	 * The visit method of the visitor is called and a reference to this
	 * object is passed as a parameter. It has to be noted however,
	 * that a dynamic_cast is performed inside this method. If the cast fails,
	 * an appropriate exception is thrown by this method. This situation can arise
	 * when the passed Visitor object does not inherit from the appropriate
	 * visitor specialization. See Visitor documentation for more details.
	 *
	 * @param[in,out] visitor A reference to the visitor object
	 * @exception Error Visitor type inappropriate for visiting this object
	 * or any other error thrown by the Visitor::throw() method.
	 */
	virtual void accept(BaseVisitor& visitor) throw (Error);

	/**
	 * Get the interface joining type
	 *
	 * @return Interface joining type
	 */
	inline const JoinedType getJoinedType() const;

	/**
	 * Set the interface joining type
	 *
	 * @param[in] inSource Interface joining type
	 */
	void setJoinedType(const JoinedType& inSource);

	/**
	 * Add a port to the list of ports. Empty pointer is ignored.
	 *
	 * @param[in] inPort Pointer to port to be added.
	 * @exception Error Could not add port, because Port name is empty
	 */
	void addPort(const PortSharedPtr& inPort) throw (Error);

	/**
	 * Get the list of pointers to the joined ports of an interface
	 *
	 * @return outPorts List of ports to be appended to
	 */
	inline void getPorts(std::list<PortSharedPtr>& outPorts) const;

	/**
	 * Set the list of joined ports of an interface
	 *
	 * @param[in] inSource List of ports to be joined to an interface
	 */
	void setPorts(const std::list<PortSharedPtr>& inSource);

	/**
	 * Add a portList to the list of portLists. Empty pointer is ignored.
	 *
	 * @param[in] inPortList Pointer to portList to be added.
	 */
	void addPortList(const PortListSharedPtr& inPortList);

	/**
	 * Get the list of joined portLists of an interface
	 *
	 * @return outPortLists List of joined portLists to be appended to
	 */
	inline void getPortLists(std::list<PortListSharedPtr>& outPortLists) const;

	/**
	 * Set the list of joined portLists of an interface
	 *
	 * @param[in] inSource List of portLists to be joined to an interface
	 */
	void setPortLists(const std::list<PortListSharedPtr>& inSource);

	/**
	 * Get the nested interface joined info.
	 *
	 * @param[out] outJoinedInfos Vector of interface joined info to be appended to
	 */
	inline void getChildren(std::vector<InterfaceJoinedInfoSharedPtr> & outJoinedInfos) const;

	/**
	 * Set the nested joining information
	 *
	 * @param[in] inSource Vector containing joining informations
	 * @exception Error Could not add child joined info because
	 * pointer to the joined info does not exist
	 */
	void setChildren(const std::vector<InterfaceJoinedInfoSharedPtr> & inSource);

	/**
	 * Add a joined info to parent joined info
	 *
	 * @param[in] inChildJoinInfo Child interface joined info to be added to parent
	 */
	bool addChildJoinedInfo(const InterfaceJoinedInfoSharedPtr& inChildJoinInfo);

	/**
	 * Apply action on all children
	 * @param[in] action Action to be applied
	 */
	template <typename _Action> inline void applyOnAllChildren(const _Action& action) throw (Error);

	/**
	 * Get the relation type.
	 *
	 * @return RelationType
	 */
	inline const RelationType getRelationType() const;

	/**
	 * Set the relation type.
	 *
	 * @param[in] inSource RelationType
	 */
	void setRelationType(const RelationType& inSource);

	/**
	 * Get the total number of bits of the composition
	 * @return Number of bits
	 */
	size_t getSize() const;

	virtual ~InterfaceJoinedInfo() throw ();

protected:
	InterfaceJoinedInfo();

private:
	JoinedType mJoinedType;
	RelationType mRelationType;
	std::list<PortSharedPtr> mJoinedPorts;
	std::list<PortListSharedPtr> mJoinedPortLists;
	std::vector<InterfaceJoinedInfoSharedPtr> mChildren;

};

/**
 * Get the interface joining type
 *
 * @return Interface joining type
 */
inline const InterfaceJoinedInfo::JoinedType InterfaceJoinedInfo::getJoinedType() const {
	return mJoinedType;
}

/**
 * Get the list of pointers to the joined ports of an interface
 *
 * @return outPorts List of ports to be appended to
 */
inline void InterfaceJoinedInfo::getPorts(std::list<PortSharedPtr>& outPorts) const {
	outPorts.insert(outPorts.end(), mJoinedPorts.begin(), mJoinedPorts.end());
}

/**
 * Get the list of joined portLists of an interface
 *
 * @return outPorts List of joined portLists to be appended to
 */
inline void InterfaceJoinedInfo::getPortLists(std::list<PortListSharedPtr>& outPortLists) const {
	outPortLists.insert(outPortLists.end(), mJoinedPortLists.begin(), mJoinedPortLists.end());
}

/**
 * Get the nested interface joined info.
 *
 * @param[out] outJoinedInfos Vector of interface joined info to be appended to
 */
inline void InterfaceJoinedInfo::getChildren(
	std::vector<InterfaceJoinedInfoSharedPtr> & outJoinedInfos) const {
	outJoinedInfos.insert(outJoinedInfos.end(), mChildren.begin(), mChildren.end());
}

/**
 * Apply action on all children
 * @param[in] action Action to be applied 
 */
template <typename _Action> inline void InterfaceJoinedInfo::applyOnAllChildren(
	const _Action& action) throw (Error) {
	try {
		std::vector<InterfaceJoinedInfoSharedPtr>::iterator it = mChildren.begin();
		for(; it != mChildren.end(); ++it) {
			action(*it);
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

/**
 * Get the relation type.
 *
 * @return RelationType
 */
inline const InterfaceJoinedInfo::RelationType InterfaceJoinedInfo::getRelationType() const {
	return mRelationType;
}

} // namespace generic
} // namespace torc
#endif // TORC_GENERIC_INTERFACEJOINEDINFO_HPP
