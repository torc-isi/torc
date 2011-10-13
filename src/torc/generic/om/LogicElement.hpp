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

#ifndef TORC_GENERIC_OM_LOGICELEMENT_HPP
#define TORC_GENERIC_OM_LOGICELEMENT_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/LogicValue.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/VisitorType.hpp"

#include <list>
#include <vector>

namespace torc { namespace generic { class LogicValue; }  } 
namespace torc { namespace generic { class BaseVisitor; }  } 

namespace torc {

namespace generic {

/**
 * @brief Represents different logic elements which holds array of logic values. 
 * 
 * The LogicElement class is used to represent different logic elements constructs
 * in EDIF like logicWaveForm, logicOneOf, logicList, transition, becomes, 
 * ignore and logic value name. 
 */
class LogicElement 
    : public LogicValue {

    friend class FactoryType<LogicElement>;

  public:
    /**
     * @enum Type
     * Logic element type
     */
    enum Type
    {
      eTypeSingle = 0,  
      eTypeList,
      eTypeOneOf,
      eTypeWaveForm,  
      eTypeIgnored,
      eTypeTransition,
      eTypeBecomes    
    };

    /**
     * @enum RelationType
     *
     * Relation type of the logic elements, is it parent or child.
     */
    enum RelationType
    {
        eRelationTypeParent = 0,
        eRelationTypeChild
    };

    /**
     * Convenience class to visit a logic element.
     */
    typedef VisitorType<LogicElement> Visitor;

    /**
     * Convenience class to create a logic element.
     */
    class Factory: public FactoryType<LogicElement>
    {
        public:
            using FactoryType<LogicElement>::create;
        /**
         * Create a logic element.
         *
         * @param[in] inType Type of the logic value.
         * @param[in] inLogicValue Pointer to the logic value [optional], 
         *            needed for single logic element creation.  
         * @param[in] inParentLogicElement Pointer to parent logic element [optional].
         *
         * @return Pointer to created permutable.
         */
         LogicElementSharedPtr
         virtual newLogicElementPtr( const LogicElement::Type & inType,
                const LogicValueSharedPtr & inLogicValue 
                        = LogicValueSharedPtr(),
                const LogicElementSharedPtr & inParentLogicElement
                        = LogicElementSharedPtr() ) throw(Error);
    };

    /**
     * @brief Recive a visitor to this class. 
     * The visit method of the visitor is called and a reference to this 
     * object is passed as a parameter. It has to be noted however, 
     * that a dynamic_cast is performed inside this method. If the cast fails, 
     * an appropriate exception is thrown by this method. This sitation can arise 
     * when the passed Visitor object does not inherit from the appropriate 
     * visitor specialization. See Visitor documentation for more details.
     * 
     * @param[in,out] visitor A reference to the visitor object
     * @exception Error Visitor type inappropriate for visiting this object 
     * or any other error thrown by the Visitor::throw() method.
     */
    virtual void 
    accept(BaseVisitor & visitor) throw(Error);

    /**
     * Get the Logic element type
     *
     * @return Logic element type
     */
    inline const Type 
    getType() const throw();

    /**
     * Set the Logic element type
     *
     * @param[in] inSource Logic element type
     */
    void 
    setType( const Type & inSource ) throw();

    /**
     * Get the nested logic elements.
     *
     * @param[out] outLogicElements Vector of logic element to be appended to
     */
    inline void
    getChildren( std::vector< LogicElementSharedPtr >
            & outLogicElements ) const throw();

    /**
     * Set the nested logic elements.
     * 
     * @param[in] inSource Vector containing logic elements.
     * @exception Error Could not add child logic elements because 
     * pointer to the logic elements does not exist
     */
    void 
    setChildren(const std::vector< LogicElementSharedPtr > 
            & inSource) throw();

    /**
     * Add a logic element to parent logic element
     * 
     * @param[in] inChildLogicElement Child logic element to be added to parent
     */
    bool 
    addChildLogicElement( 
            const LogicElementSharedPtr & inChildLogicElement) throw();

    /**
     * Apply action on all children
     * @param[in] action Action to be applied 
     */
    template<typename _Action> 
    inline void applyOnAllChildren(const _Action & action) throw(Error);

    /**
     * Get the relation type.
     *
     * @return RelationType
     */
    inline const RelationType
    getRelationType() const throw();

    /**
     * Set the relation type.
     *
     * @param[in] inSource RelationType
     */
    void
    setRelationType(const RelationType & inSource) throw();

    /**
     * Get the total number of bits of the composition
     * @return Number of bits
     */
    size_t
    getSize() const throw();

    ~LogicElement() throw();

  protected:
    LogicElement();

  private:
    Type mType;
    RelationType mRelationType;
    std::vector< LogicElementSharedPtr > mChildren;

};

/**
 * Get the Logic element type
 *
 * @return Logic element type
 */
inline const LogicElement::Type 
LogicElement::getType() const throw() {
    return mType;
}

/**
 * Get the nested logic elements.
 *
 * @param[out] outLogicElements Vector of logic element to be appended to
 */
inline void
LogicElement::getChildren( std::vector< LogicElementSharedPtr >
        & outLogicElements ) const throw() {
    outLogicElements.insert( outLogicElements.end(),
        mChildren.begin(), mChildren.end() );
}

/**
 * Apply action on all children
 * @param[in] action Action to be applied 
 */
template<typename _Action> 
inline void 
LogicElement::applyOnAllChildren(const _Action & action) throw(Error) {
    try
    {
        std::vector< LogicElementSharedPtr >::iterator it 
                = mChildren.begin();
        for(; it != mChildren.end(); ++ it )
        {
            action( *it );
        }
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
 * Get the relation type.
 *
 * @return RelationType
 */
inline const LogicElement::RelationType
LogicElement::getRelationType() const throw() {
   return mRelationType; 
}

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_LOGICELEMENT_HPP
