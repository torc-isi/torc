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

#ifndef TORC_GENERIC_SIMULATIONINFO_HPP
#define TORC_GENERIC_SIMULATIONINFO_HPP

#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/Error.hpp"
#include "torc/generic/Commentable.hpp"
#include "torc/generic/ParentedObject.hpp"
#include "torc/generic/Visitable.hpp"
#include "torc/generic/VisitorType.hpp"
#include "torc/generic/FactoryType.hpp"
#include "torc/generic/SelfReferencing.hpp"
#include "torc/generic/SymTab.hpp"
#include "torc/generic/UserDataContainer.hpp"

#include <list>
#include <vector>

namespace torc { namespace generic { class BaseVisitor; } }
namespace torc { namespace generic { class Library; } }
namespace torc { namespace generic { class LogicValue; } }

namespace torc {
namespace generic {

/**
 * @brief This class is used to hold all information about the logic values used 
 * within a library.
 */
class SimulationInfo : public Commentable, public Visitable, public ParentedObject<Library>,
	public SelfReferencing<SimulationInfo>, public UserDataContainer {

	friend class FactoryType<SimulationInfo> ;

public:
	/**
	 * Convenience typedef for visiting a simulation info
	 */
	typedef VisitorType<SimulationInfo> Visitor;

	/**
	 * Convenience class for creating a simulation info
	 */
	class Factory : public FactoryType<SimulationInfo> {
	public:
		using FactoryType<SimulationInfo>::create;

		/**
		 * Create a simulation info.
		 *
		 * @param[in] inLogicValues Vector of logic values to be appended to
		 * @param[in] inLibraryPtr Pointer to parented(Library) object.
		 *
		 * @return Pointer to simulation info.
		 **/
		virtual SimulationInfoSharedPtr
		newSimulationInfoPtr(const std::vector<LogicValueSharedPtr>& inLogicValues,
			const LibrarySharedPtr& inLibraryPtr) throw (Error);
	};

	virtual void accept(BaseVisitor& inoutVisitor) throw (Error);

	/**
	 * Add a logic value to the list of logic values. If an empty pointer is supplied,
	 * it returns without doing anything.
	 *
	 * @param[in] inLogicValue A pointer to a logic value object.
	 *
	 * @exception Error Logic value could not be added.
	 */
	void addLogicValue(const LogicValueSharedPtr& inLogicValue) throw (Error);

	/**
	 * Find a logic value by name, in the list of logic values in simulationInfo.
	 *
	 * @param[in] inName String value specifying the name of the logic value.
	 *
	 * @return A pointer to the logic value if found, an empty pointer otherwise.
	 */
	LogicValueSharedPtr findLogicValue(const std::string& inName);

	/**
	 * Remove the specified logic value from the list of logic values.
	 * If an empty pointer is passed, it returns without doing anything
	 *
	 * @param[in] inName name of the object to be removed
	 *
	 * @exception Error Logic Value not preset in collection.
	 */
	void removeLogicValue(const std::string& inName) throw (Error);

	/**
	 * Get the list of logic values.
	 *
	 * @param[out] outLogicValues Vector of logic values to be appended to
	 */
	inline void getLogicValues(std::vector<LogicValueSharedPtr>& outLogicValues) const;

	/**
	 * Set the list of logic values.
	 *
	 * @param[in] inLogicValues Vector of logic values to be appended to
	 */
	void setLogicValues(const std::vector<LogicValueSharedPtr>& inLogicValues) throw (Error);

	/**
	 * Apply action on all logic values.
	 * @param[in] action Action to be applied
	 *
	 */
	template <typename _Action> inline void applyOnAllLogicValues(const _Action& action)
		throw (Error);

	virtual ~SimulationInfo() throw ();

protected:
	SimulationInfo();

private:
	SimulationInfo(const SimulationInfo& source);

	SimulationInfo& operator=(const SimulationInfo& source);

	SymTab<std::string, LogicValueSharedPtr> mLogicValueSymTab;

};

/**
 * Get the list of logic values.
 *
 * @param[out] outLogicValues Vector of logic values to be appended to
 */
inline void SimulationInfo::getLogicValues(std::vector<LogicValueSharedPtr>& outLogicValues) const {
	mLogicValueSymTab.getValues(outLogicValues);
}

/**
 * Apply action on all logic values.
 * @param[in] action Action to be applied
 *
 */
template <typename _Action> inline void SimulationInfo::applyOnAllLogicValues(const _Action& action)
	throw (Error) {
	try {
		mLogicValueSymTab.applyOnAll(action);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_SIMULATIONINFO_HPP
