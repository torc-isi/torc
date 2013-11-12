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

#ifndef TORC_GENERIC_LOGICVALUE_HPP
#define TORC_GENERIC_LOGICVALUE_HPP

#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/Nameable.hpp"
#include "torc/generic/Renamable.hpp"
#include "torc/generic/Commentable.hpp"
#include "torc/generic/ParentedObject.hpp"
#include "torc/generic/PropertyContainer.hpp"
#include "torc/generic/UserDataContainer.hpp"
#include "torc/generic/Visitable.hpp"
#include "torc/generic/VisitorType.hpp"
#include "torc/generic/SelfReferencing.hpp"
#include "torc/generic/Error.hpp"
#include "torc/generic/FactoryType.hpp"

namespace torc { namespace generic { class LogicValueAttributes; } }
namespace torc { namespace generic { class SimulationInfo; } }

namespace torc {
namespace generic {

/**
 * @brief This class is used within simulationInfo construct to define a 
 * logic value to use for modeling in the logicModel view.
 */
class LogicValue : public Nameable, public Renamable, public Commentable, public Visitable,
	public ParentedObject<SimulationInfo>, public PropertyContainer, public UserDataContainer,
	public SelfReferencing<LogicValue> {

	friend class FactoryType<LogicValue> ;

public:

	/**
	 * Convenience class to visit a LogicValue.
	 */
	typedef VisitorType<LogicValue> Visitor;

	/**
	 * Convenience class to create a LogicValue.
	 */
	class Factory : public FactoryType<LogicValue> {
	public:
		using FactoryType<LogicValue>::create;
		/**
		 * Create a LogicValue.
		 *
		 * @param[in] inName Name of the LogicValue to be created.
		 * @param[in] inAttributes Pointer to LogicValueAttributes object.
		 * @param[in] inOriginalName Original name of the LogicValue [optional].
		 * @param[in] inSimulationInfo Pointer to parented(SimulationInfo) object.
		 *            If not mentioned LogicValue will not be added to SimulationInfo.
		 *
		 * @return Pointer to created LogicValue.
		 */
		virtual LogicValueSharedPtr newLogicValuePtr(const std::string& inName,
			const LogicValueAttributesSharedPtr& inAttributes, const std::string& inOriginalName
				= std::string(),
			const SimulationInfoSharedPtr& inSimulationInfo = SimulationInfoSharedPtr())
			throw (Error);
	};

	/**
	 * Receive a visitor to this class. The visit method of the visitor is called
	 * and a reference to this object is passed as a parameter. It has to be noted however,
	 * that a dynamic_cast is performed inside this method. If the cast fails,
	 * an appropriate exception is thrown by this method. This situation can arise when
	 * the passed Visitor object does not inherit from the appropriate visitor specialization.
	 * See Visitor documentation for more details.
	 *
	 * @param[in,out] inoutVisitor A reference to the visitor object
	 * @exception Error Visitor type inappropriate for visiting this object
	 * or any other error thrown by the Visitor::throw() method.
	 */
	virtual void accept(BaseVisitor& inoutVisitor) throw (Error);

	/**
	 * Get the attributes of the LogicValue. Attributes include voltageMap, currentMap,
	 * booleanMap, compound, weak, strong, dominates etc.
	 *
	 * @return Pointer to LogicValueAttributes object.
	 */
	inline const LogicValueAttributesSharedPtr getAttributes() const;

	/**
	 * Set the attributes of the LogicValue. Attributes include voltageMap, currentMap,
	 * booleanMap, compound, weak, strong, dominates etc.
	 *
	 * @param[in] inSource Pointer to LogicValueAttributes object.
	 */
	void setAttributes(const LogicValueAttributesSharedPtr& inSource);

	virtual ~LogicValue() throw ();

protected:
	LogicValue();

private:
	LogicValueAttributesSharedPtr mAttributes;

};

/**
 * Get the attributes of the LogicValue. Attributes include voltageMap, currentMap,
 * booleanMap, compound, weak, strong, dominates etc.
 *
 * @return Pointer to LogicValueAttributes object.
 */
inline const LogicValueAttributesSharedPtr LogicValue::getAttributes() const {
	return mAttributes;
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_LOGICVALUE_HPP
