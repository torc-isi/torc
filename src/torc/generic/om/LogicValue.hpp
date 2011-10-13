// Torc - Copyright 2011 University of Southern California.  All Rights Reserved.
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

#ifndef TORC_GENERIC_OM_LOGICVALUE_HPP
#define TORC_GENERIC_OM_LOGICVALUE_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/Nameable.hpp"
#include "torc/generic/om/Renamable.hpp"
#include "torc/generic/om/Commentable.hpp"
#include "torc/generic/om/ParentedObject.hpp"
#include "torc/generic/om/PropertyContainer.hpp"
#include "torc/generic/om/UserDataContainer.hpp"
#include "torc/generic/om/Visitable.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/SelfReferencing.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/FactoryType.hpp"

namespace torc { namespace generic { class LogicValueAttributes; }  } 
namespace torc { namespace generic { class SimulationInfo; }  } 


namespace torc {

namespace generic {

/**
 * @brief This class is used within simulationInfo construct to define a 
 * logic value to use for modeling in the logicModel view.
 */
class LogicValue : 
    public Nameable,
    public Renamable, 
    public Commentable,
    public Visitable,
    public ParentedObject<SimulationInfo>, 
    public PropertyContainer, 
    public UserDataContainer,
    public SelfReferencing<LogicValue> {

    friend class FactoryType<LogicValue>;

  public:

    /**
     * Convenience class to visit a LogicValue.
     */
    typedef VisitorType<LogicValue> Visitor;

    /**
     * Convenience class to create a LogicValue.
     */
    class Factory: public FactoryType<LogicValue>
    {
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
         virtual LogicValueSharedPtr
         newLogicValuePtr( const std::string & inName,
                const LogicValueAttributesSharedPtr & inAttributes,
                const std::string &inOriginalName = std::string(),
                const SimulationInfoSharedPtr & inSimulationInfo
                            = SimulationInfoSharedPtr() ) throw(Error);
    };

    /**
     * Recive a visitor to this class. The visit method of the visitor is called
     * and a reference to this object is passed as a parameter. It has to be noted however,
     * that a dynamic_cast is performed inside this method. If the cast fails,
     * an appropriate exception is thrown by this method. This sitation can arise when
     * the passed Visitor object does not inherit from the appropriate visitor specialization.
     * See Visitor documentation for more details.
     *
     * @param[in,out] inoutVisitor A reference to the visitor object
     * @exception Error Visitor type inappropriate for visiting this object
     * or any other error thrown by the Visitor::throw() method.
     */
    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    /**
     * Get the attributes of the LogicValue. Attributes include voltageMap, currentMap,
     * booleanMap, compound, weak, strong, dominates etc.
     *
     * @return Pointer to LogicValueAttributes object.
     */
    inline const LogicValueAttributesSharedPtr 
    getAttributes() const throw();

    /**
     * Set the attributes of the LogicValue. Attributes include voltageMap, currentMap,
     * booleanMap, compound, weak, strong, dominates etc.
     *
     * @param[in] inSource Pointer to LogicValueAttributes object.
     */
    void setAttributes(const  LogicValueAttributesSharedPtr & inSource) throw();

    virtual
    ~LogicValue() throw();

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
inline const  LogicValueAttributesSharedPtr 
LogicValue::getAttributes() const throw() {
    return mAttributes;
}

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_LOGICVALUE_HPP
