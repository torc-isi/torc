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

#include "torc/generic/InterfaceJoinedInfo.hpp"
#include "torc/generic/Port.hpp"
#include "torc/generic/PortList.hpp"

namespace torc {
namespace generic {

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
InterfaceJoinedInfoSharedPtr InterfaceJoinedInfo::Factory::newInterfaceJoinedInfoPtr(
	const std::list<PortSharedPtr>& inPorts, const std::list<PortListSharedPtr>& inPortLists,
	const ViewSharedPtr& inViewPtr, const InterfaceJoinedInfoSharedPtr& inParentJoinedInfo)
		throw (Error) {
	try {
		InterfaceJoinedInfoSharedPtr newInterfaceJoinedInfo;
		create(newInterfaceJoinedInfo);
		newInterfaceJoinedInfo->setPorts(inPorts);
		if(inParentJoinedInfo) {
			inParentJoinedInfo->addChildJoinedInfo(newInterfaceJoinedInfo);
		} else if(inViewPtr) {
			inViewPtr->addInterfaceJoinedInfo(newInterfaceJoinedInfo);
		}
		return newInterfaceJoinedInfo;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

/**
 * Set the interface joining type
 * @param[in] inSource Interface joining type
 */
void InterfaceJoinedInfo::setJoinedType(const JoinedType& value) {
	mJoinedType = value;
}

/**
 * Add a port to the list of ports. Empty pointer is ignored.
 *
 * @param[in] inPort Pointer to port to be added.
 * @exception Error Could not add port, because Port name is empty   
 */
void InterfaceJoinedInfo::addPort(const PortSharedPtr& inPort) throw (Error) {
	if(!inPort) {
		return;
	}
	std::string name = inPort->getName();
	if(name.empty()) {
		Error e(eMessageIdErrorEmptyItemName, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Port name", name);
		throw e;
	}
	mJoinedPorts.push_back(inPort);
}

/**
 * Add a portList to the list of portLists. Empty pointer is ignored.
 *
 * @param[in] inPortList Pointer to portList to be added.
 */
void InterfaceJoinedInfo::addPortList(const PortListSharedPtr& inPortList) {
	if(!inPortList) {
		return;
	}
	mJoinedPortLists.push_back(inPortList);
}

/**
 * Set the list of joined ports of an interface
 *
 * @param[in] inSource List of ports to be joined to an interface
 */
void InterfaceJoinedInfo::setPorts(const std::list<PortSharedPtr>& inSource) {
	std::list<PortSharedPtr>::const_iterator port = inSource.begin();
	std::list<PortSharedPtr>::const_iterator end = inSource.end();
	for(; port != end; ++port) {
		try {
			addPort(*port);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

/**
 * Set the list of joined portLists of an interface
 *
 * @param[in] inSource List of portLists to be joined to an interface
 */
void InterfaceJoinedInfo::setPortLists(const std::list<PortListSharedPtr>& inSource) {
	std::list<PortListSharedPtr>::const_iterator portList = inSource.begin();
	std::list<PortListSharedPtr>::const_iterator end = inSource.end();
	for(; portList != end; ++portList) {
		try {
			addPortList(*portList);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

/**
 * Set the nested joining information
 *
 * @param[in] inSource Vector containing joining informations
 * @exception Error Could not add child joined info because
 * pointer to the joined info does not exist
 */
void InterfaceJoinedInfo::setChildren(const std::vector<InterfaceJoinedInfoSharedPtr>& inSource) {
	std::vector<InterfaceJoinedInfoSharedPtr>::const_iterator entry = inSource.begin();
	std::vector<InterfaceJoinedInfoSharedPtr>::const_iterator end = inSource.end();
	for(; entry != end; ++entry) {
		try {
			addChildJoinedInfo(*entry);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
}

/**
 * Add a joined info to parent joined info
 *
 * @param[in] inChildJoinInfo Child interface joined info to be added to parent
 */
bool InterfaceJoinedInfo::addChildJoinedInfo(const InterfaceJoinedInfoSharedPtr& inChildJoinInfo) {
	if(!inChildJoinInfo) {
		Error e(eMessageIdErrorPointerToItemDoesNotExist, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Pointer to the joined info object does not exist", inChildJoinInfo);
		throw e;
	}
	if(inChildJoinInfo) {
		mChildren.push_back(inChildJoinInfo);
		return true;
	} else {
		return false;
	}
}

void InterfaceJoinedInfo::accept(BaseVisitor& visitor) throw (Error) {
	try {
		runVisitor(*this, visitor);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

/**
 * Set the relation type.
 *
 * @param[in] inSource RelationType
 */
void InterfaceJoinedInfo::setRelationType(const RelationType& inSource) {
	mRelationType = inSource;
}

/**
 * Get the total number of bits of the composition
 * @return Number of bits
 */
size_t InterfaceJoinedInfo::getSize() const {
	size_t size = 0;
	size_t pSize = 0;
	size_t cSize = 0;
	InterfaceJoinedInfo::RelationType rType = getRelationType();

	std::vector<InterfaceJoinedInfoSharedPtr> outJoinedInfos;
	getChildren(outJoinedInfos);
	std::vector<InterfaceJoinedInfoSharedPtr>::iterator joinedInfoIt = outJoinedInfos.begin();

	std::list<PortSharedPtr> outPorts;
	getPorts(outPorts);
	std::list<PortSharedPtr>::iterator portIt = outPorts.begin();

	std::list<PortListSharedPtr> outPortLists;
	getPortLists(outPortLists);
	std::list<PortListSharedPtr>::iterator portListIt = outPortLists.begin();

	if(InterfaceJoinedInfo::eRelationTypeParent == rType) {
		if(!outPorts.empty()) {
			pSize = (*portIt)->getSize();
		} else if(!outPortLists.empty()) {
			pSize = (*portListIt)->getSize();
		} else if(!outJoinedInfos.empty()) {
			for(; joinedInfoIt != outJoinedInfos.end(); joinedInfoIt++) {
				pSize += (*joinedInfoIt)->getSize();
				for(; portIt != outPorts.end(); portIt++) {
					pSize += (*portIt)->getSize();
				}
				for(; portListIt != outPortLists.end(); portListIt++) {
					pSize += (*portListIt)->getSize();
				}
			}
		} else {
			pSize = 0;
		}
	} else {
		if(!outPorts.empty()) {
			for(; portIt != outPorts.end(); portIt++) {
				cSize += (*portIt)->getSize();
			}
		}
		if(!outPortLists.empty()) {
			for(; portListIt != outPortLists.end(); portListIt++) {
				cSize += (*portListIt)->getSize();
			}
		}
		if(!outJoinedInfos.empty()) {
			for(; joinedInfoIt != outJoinedInfos.end(); joinedInfoIt++) {
				cSize += (*joinedInfoIt)->getSize();
			}
		}
	}
	size = pSize + cSize;
	return size;
}

InterfaceJoinedInfo::InterfaceJoinedInfo() : Visitable(), SelfReferencing<InterfaceJoinedInfo>(),
	mJoinedType(), mRelationType(), mJoinedPorts(), mJoinedPortLists(), mChildren() {}

InterfaceJoinedInfo::~InterfaceJoinedInfo() throw () {}

} // namespace generic
} // namespace torc
