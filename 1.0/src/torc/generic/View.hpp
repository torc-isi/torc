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

#ifndef TORC_GENERIC_VIEW_HPP
#define TORC_GENERIC_VIEW_HPP

#include "torc/generic/PointerTypes.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/Commentable.hpp"
#include "torc/generic/Error.hpp"
#include "torc/generic/Extern.hpp"
#include "torc/generic/FactoryType.hpp"
#include "torc/generic/Nameable.hpp"
#include "torc/generic/ParameterMap.hpp"
#include "torc/generic/ParentedObject.hpp"
#include "torc/generic/PropertyContainer.hpp"
#include "torc/generic/Renamable.hpp"
#include "torc/generic/SelfReferencing.hpp"
#include "torc/generic/SymTab.hpp"
#include "torc/generic/Visitable.hpp"
#include "torc/generic/VisitorType.hpp"
#include "torc/generic/UserDataContainer.hpp"
#include "torc/generic/StatusContainer.hpp"

namespace torc { namespace generic { class BaseVisitor; } }
namespace torc { namespace generic { class Cell; } }
namespace torc { namespace generic { class Instance; } }
namespace torc { namespace generic { class Net; } }
namespace torc { namespace generic { class Port; } }
namespace torc { namespace generic { class Permutable; } }
namespace torc { namespace generic { class InterfaceAttributes; } }
namespace torc { namespace generic { class InterfaceJoinedInfo; } }

namespace torc {
namespace generic {

/**
 * @brief Represents and EDIF View.
 *
 * The View class represents an EDIF view. A view can be conrete or extern (black-box).
 *
 * @note Only views of type NETLIST are supported.
 */
class View : public Commentable, public Extern, public Nameable, public ParentedObject<Cell>,
	public PropertyContainer, public Renamable, public SelfReferencing<View>, public Visitable,
	public UserDataContainer, public StatusContainer {

#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
	friend class RestoredViewUpdater;
#endif //GENOM_SERIALIZATION
	friend class FactoryType<View> ;

public:
	/**
	 * @enum Type
	 * Type of the view
	 */
	enum Type {
		eTypeMaskLayout = 0, eTypePCBLayout, eTypeNetlist, eTypeSchematic, eTypeSymbolic,
		eTypeBehavior, eTypeLogicModel, eTypeDocument, eTypeGraphic, eTypeStranger
	};

	/**
	 * Convenience
	 */
	typedef VisitorType<View> Visitor;

	/**
	 * Convenience class to create a view.
	 */
	class Factory : public FactoryType<View> {
	public:
		using FactoryType<View>::create;
		/**
		 * Create a view
		 *
		 * @param[in] inName Name of the view to be created.
		 * @param[in] inCellPtr Pointer to parented(Cell) object.
		 * @param[in] inViewType View Type.
		 * @param[in] inOriginalName Original name of the view [optional].
		 *
		 * @return Pointer to created view.
		 **/
		virtual ViewSharedPtr newViewPtr(const std::string& inName, const CellSharedPtr& inCellPtr,
			const View::Type& inViewType = View::eTypeNetlist, const std::string& inOriginalName =
				std::string()) throw (Error);
	};

	virtual void accept(BaseVisitor& inoutVisitor) throw (Error);

	/**
	 * Returns the context that will be used to get parameters from
	 * the parameter map. For an unbound instance,
	 * this will return an invalid value
	 */
	inline ParameterContext getParameterContext() const;

	/**
	 * Get the ParameterMap corresponding to this cell
	 *
	 * @return A ParameterMap consisiting of only the overridden parameters.
	 */
	ParameterMapSharedPtr getParameters() throw (Error);

	void setParameters(const ParameterMapSharedPtr& inSource);

	/**
	 * Get the type of view.
	 *
	 * @return Type of view
	 */
	inline const Type getType() const;

	/**
	 * Set the type of view
	 *
	 * @param[in] inSource Type of view
	 */
	void setType(const Type& inSource);

	/**
	 * Get the list of instances for this view.
	 *
	 * @param[out] outInstances List of instances to be appended to
	 */
	inline void getInstances(std::vector<InstanceSharedPtr>& outInstances) const;

	/**
	 * Set the list of instances to this view. It will lead to a linear traversal on the list. So
	 * usage of this API is not recommended.
	 *
	 * @param[in] inSource List of instances.
	 */
	void setInstances(const std::vector<InstanceSharedPtr>& inSource) throw (Error);

	/**
	 * Add an instance to the list of instances. Empty pointer is ignored.
	 *
	 * @param[in] inInstance Pointer to instance to be added.
	 *
	 * @exception Error Could not add instance because pointer to the Instance does not exist
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorPointerToItemDoesNotExist
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Pointer to Instance - <i>boost::shared_ptr</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 * @exception Error Could not add instance because Instance name is empty
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorEmptyItemName
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Instance name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 * @exception Error Could not add instance because Instance name already exists
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorItemAlreadyExists
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Instance name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 */
	void addInstance(const InstanceSharedPtr& inInstance) throw (Error);

	/**
	 * Find an instance by name, in the list of instances.
	 *
	 * @param[in] inName String inSource specifying the name of the instance.
	 *
	 * @return A pointer to the instance if found, an empty pointer otherwise.
	 *
	 * @exception Error Empty Instance name
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorEmptyItemName
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Instance name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 */
	InstanceSharedPtr findInstance(const std::string& inName);

	/**
	 * Remove the specified instance from the list of cells.
	 *
	 * @param inName Name of the object to be delete
	 *
	 * @exception Error Empty Instance name
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorEmptyItemName
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Instance name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 * @exception Error Instance name not preset in collection.
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorItemNotFound
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Instance name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 */
	void removeInstance(const std::string& inName) throw (Error);

	/**
	 * Apply action on all Instances.
	 * @param[in] action Action to be applied
	 *
	 */
	template <typename _Action> inline void applyOnAllInstances(const _Action& action)
		throw (Error);

	/**
	 * Get the list of nets for this view.
	 *
	 * @param[out] outNets List of nets to be appended to
	 */
	inline void getNets(std::vector<NetSharedPtr>& outNets) const;

	/**
	 * Set the list of nets to this view. It will lead to a linear traversal on the list. So usage
	 * of this API is not recommended.
	 *
	 * @param[in] inSource List of nets.
	 */
	void setNets(const std::vector<NetSharedPtr>& inSource) throw (Error);

	/**
	 * Add a net to the list of nets. Empty pointer is ignored.
	 *
	 * @param[in] inNet Pointer to net to be added.
	 *
	 * @exception Error Could not add Net, because Net name is empty
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorEmptyItemName
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Net name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 * @exception Error Could not add Net, because Net name already exists
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorItemAlreadyExists
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Net name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 */
	void addNet(const NetSharedPtr& inNet) throw (Error);

	/**
	 * Find a net by name, in the list of net.
	 *
	 * @param[in] inName String inSource specifying the name of the Net
	 *
	 * @return A pointer to the net if found, an empty pointer otherwise.
	 *
	 * @exception Error Empty Net name
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorEmptyItemName
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Net name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 */
	NetSharedPtr findNet(const std::string& inName);

	/**
	 * Remove the specified net from the list of nets. If an empty pointer is passed, it returns
	 * without doing anything
	 *
	 * @param inName Name of the object to be delete
	 *
	 * @exception Error Empty Net name
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorEmptyItemName
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Net name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 * @exception Error Net not preset in collection.
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorItemNotFound
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Net name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 */
	void removeNet(const std::string& inName) throw (Error);

	/**
	 * Apply action on all Nets.
	 * @param[in] action Action to be applied
	 *
	 */
	template <typename _Action> inline void applyOnAllNets(const _Action& action) throw (Error);

	/**
	 * Get the list of ports for this view.
	 *
	 * @param[out] outPorts List of ports to be appended to
	 */
	inline void getPorts(std::vector<PortSharedPtr>& outPorts) const;

	/**
	 * Set the list of ports to this view. It will lead to a linear traversal on the list. So usage
	 * of this API is not recommended.
	 *
	 * @param[in] inSource List of ports
	 */
	void setPorts(const std::vector<PortSharedPtr>& inSource) throw (Error);

	/**
	 * Add a port to the list of ports. Empty pointer is ignored.
	 *
	 * @param[in] inPort Pointer to port to be added.
	 *
	 * @exception Error Could not add port, because Port name is empty
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorEmptyItemName
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Port name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 * @exception Error Could not add port, because Port name already exists
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorItemAlreadyExists
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Port name - <i>View</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 */

	void addPort(const PortSharedPtr& inPort) throw (Error);

	/**
	 * Find a port by name, in the list of ports.
	 *
	 * @param[in] inName String inSource specifying the name of the port.
	 *
	 * @return A pointer to the port if found, an empty pointer otherwise.
	 *
	 * @exception Error Empty Port name
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorEmptyItemName
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Port Name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 */

	PortSharedPtr findPort(const std::string& inName);

	/**
	 * Remove the specified port from the list of ports.
	 *
	 * @param inName Name of the port to be removed
	 *
	 * @exception Error Empty Port name
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorEmptyItemName
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Port name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 * @exception Error Port not preset in collection.
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorItemNotFound
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Port name - <i>String</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 */
	void removePort(const std::string& inName) throw (Error);

	/**
	 * Apply action on all Ports.
	 * @param[in] action Action to be applied
	 *
	 */
	template <typename _Action> inline void applyOnAllPorts(const _Action& action) throw (Error);

	/**
	 * Get the vector of permutables for this view.
	 *
	 * @param[out] outPermutables Vector of permutable to be appended to
	 */
	inline void getPermutables(std::vector<PermutableSharedPtr>& outPermutables) const;

	/**
	 * Set the vector of permutables to this view. It will lead to a linear traversal on the list.
	 *
	 * @param[in] inSource Vector of permutables to this view.
	 * @exception Error Could not add permutable because pointer to the permutable does not exist
	 */
	void setPermutables(const std::vector<PermutableSharedPtr> & inSource) throw (Error);
	/**
	 * Add a permutable to this view.
	 *
	 * @param[in] inPermutable Pointer to permutable to be added.
	 * @exception Error Could not add permutable because pointer to the permutable does not exist
	 */
	bool addPermutable(const PermutableSharedPtr& inPermutable) throw (Error);

	/**
	 * Apply action on all Permutables.
	 * @param[in] action Action to be applied
	 *
	 */
	template <typename _Action> inline void applyOnAllPermutables(const _Action& action)
		throw (Error);

	/**
	 * Get the vector of joined info for this view.
	 *
	 * @param[out] outJoinedInfos Vector of joined info to be appended to
	 */
	inline void getInterfaceJoinedInfos(
		std::vector<InterfaceJoinedInfoSharedPtr>& outJoinedInfos) const;

	/**
	 * Set the vector of joined info for this view.
	 *
	 * @param[in] inSource Vector of joined info to this view.
	 * @exception Error Could not add joined info because pointer to the joined info does not exist
	 */
	void setInterfaceJoinedInfos(const std::vector<InterfaceJoinedInfoSharedPtr> & inSource)
		throw (Error);
	/**
	 * Add a joined info to this view.
	 *
	 * @param[in] inJoinedInfo Pointer to joined info to be added.
	 * @exception Error Could not add joined info because pointer to the joined info does not exist
	 */
	bool addInterfaceJoinedInfo(const InterfaceJoinedInfoSharedPtr& inJoinedInfo) throw (Error);

	/**
	 * Apply action on all joined info.
	 * @param[in] action Action to be applied
	 *
	 */
	template <typename _Action> inline void applyOnAllInterfaceJoinedInfos(const _Action& action)
		throw (Error);

	/**
	 * Get the attributes of a view interface.
	 * Attributes include designator, simulate, timing, comments, userdata etc.
	 * This will decompile within (contents ...) construct.
	 *
	 * @return Pointer to InterfaceAttributes object.
	 */
	inline const InterfaceAttributesSharedPtr getInterfaceAttributes() const;

	/**
	 * Set the attributes of a view interface.
	 * Attributes include designator, simulate, timing, comments, userdata etc.
	 * This will decompile within (contents ...) construct.
	 *
	 * @param[in] inSource Pointer to InterfaceAttributes object.
	 */
	void setInterfaceAttributes(const InterfaceAttributesSharedPtr& inSource);

	inline const std::string& getNonNetlistViewData() const;

	void setNonNetlistViewData(const std::string& inData);

	/**
	 * Get the pointer to the simulate.
	 *
	 * @return Pointer to the simulatie
	 */
	inline const SimulateSharedPtr
	getSimulate() const;

	/**
	 * Set the pointer to the simulate.
	 *
	 * @param[in] inSource Pointer to the simulate
	 */
	void setSimulate(const SimulateSharedPtr& inSource);

	/**
	 * Get the pointer to the timing object
	 *
	 * @return Pointer to the timing object
	 */
	inline const TimingSharedPtr getTiming() const;

	/**
	 * Set the pointer to the timing object
	 *
	 * @param[in] inSource Pointer to the timing object
	 */
	void setTiming(const TimingSharedPtr& inSource);

	virtual ~View() throw ();

protected:
	View();

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void load(Archive& ar, unsigned int);

	template <class Archive> void save(Archive& ar, unsigned int) const;

	BOOST_SERIALIZATION_SPLIT_MEMBER()

	void restoreActions() throw(Error);
#endif //GENOM_SERIALIZATION
	ParameterMapSharedPtr mParameters;
	ParameterContext mMyContext;
	SymTab<std::string, InstanceSharedPtr> mInstanceSymTab;
	SymTab<std::string, NetSharedPtr> mNetSymTab;
	SymTab<std::string, PortSharedPtr> mPortSymTab;
	std::string mNonNetlistViewData;
	Type mType;
	std::vector<PermutableSharedPtr> mPermutables;
	std::vector<InterfaceJoinedInfoSharedPtr> mInterfaceJoinedInfos;
	InterfaceAttributesSharedPtr mAttributes;
	SimulateSharedPtr mSimulate;
	TimingSharedPtr mTiming;
};

inline ParameterContext View::getParameterContext() const {
	return mMyContext;
}

/**
 * Get the type of view.
 *
 * @return Type of view
 */
inline const View::Type View::getType() const {
	return mType;
}

inline void View::getInstances(std::vector<InstanceSharedPtr>& outInstances) const {
	mInstanceSymTab.getValues(outInstances);
}

template <typename _Action> inline void View::applyOnAllInstances(const _Action& action)
	throw (Error) {
	try {
		mInstanceSymTab.applyOnAll(action);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

inline void View::getNets(std::vector<NetSharedPtr>& outNets) const {
	mNetSymTab.getValues(outNets);
}

template <typename _Action> inline void View::applyOnAllNets(const _Action& action) throw (Error) {
	try {
		mNetSymTab.applyOnAll(action);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

inline void View::getPorts(std::vector<PortSharedPtr>& outPorts) const {
	mPortSymTab.getValues(outPorts);
}

template <typename _Action> inline void View::applyOnAllPorts(const _Action& action) throw (Error) {
	try {
		mPortSymTab.applyOnAll(action);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

inline const std::string& View::getNonNetlistViewData() const {
	return mNonNetlistViewData;
}

/**
 * Get the vector of permutables for this view.
 *
 * @param[out] outPermutables Vector of permutable to be appended to
 */

inline void View::getPermutables(std::vector<PermutableSharedPtr>& outPermutables) const {
	outPermutables.insert(outPermutables.end(), mPermutables.begin(), mPermutables.end());
}

/**
 * Apply action on all children.
 * @param[in] action Action to be applied
 *
 */
template <typename _Action> inline void View::applyOnAllPermutables(const _Action& action)
	throw (Error) {
	try {
		std::vector<PermutableSharedPtr>::iterator it = mPermutables.begin();
		for(; it != mPermutables.end(); ++it) {
			action(*it);
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

/**
 * Get the vector of joined info for this view.
 *
 * @param[out] outJoinedInfos Vector of joined info to be appended to
 */
inline void View::getInterfaceJoinedInfos(
	std::vector<InterfaceJoinedInfoSharedPtr>& outJoinedInfos) const {
	outJoinedInfos.insert(outJoinedInfos.end(), mInterfaceJoinedInfos.begin(),
		mInterfaceJoinedInfos.end());
}

/**
 * Apply action on all joined info.
 * @param[in] action Action to be applied
 *
 */
template <typename _Action> inline void View::applyOnAllInterfaceJoinedInfos(const _Action& action)
	throw (Error) {
	try {
		std::vector<InterfaceJoinedInfoSharedPtr>::iterator it = mInterfaceJoinedInfos.begin();
		for(; it != mInterfaceJoinedInfos.end(); ++it) {
			action(*it);
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

/**
 * Get the attributes of a view interface. Attributes include designator, comments, userdata etc.
 *
 * @return Pointer to InterfaceAttributes object.
 */
inline const InterfaceAttributesSharedPtr View::getInterfaceAttributes() const {
	return mAttributes;
}

/**
 * Get the pointer to the simulate.
 *
 * @return Pointer to the simulatie
 */
inline const SimulateSharedPtr View::getSimulate() const {
	return mSimulate;
}

/**
 * Get the pointer to the timing object
 *
 * @return Pointer to the timing object
 */
inline const TimingSharedPtr View::getTiming() const {
	return mTiming;
}

} // namespace generic
} // namespace torc
#endif // TORC_GENERIC_VIEW_HPP
