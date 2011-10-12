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

#ifndef TORC_GENERIC_PROPERTY_HPP
#define TORC_GENERIC_PROPERTY_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Commentable.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/Nameable.hpp"
#include "torc/generic/om/Renamable.hpp"
#include "torc/generic/om/SelfReferencing.hpp"
#include "torc/generic/om/SymTab.hpp"
#include "torc/generic/om/Unit.hpp"
#include "torc/generic/om/Value.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/Visitable.hpp"
#include "torc/generic/om/PropertyContainer.hpp"
#include <boost/shared_ptr.hpp>

namespace torc { namespace generic { class BaseVisitor; }  }

namespace torc {

namespace generic {

/**
 * Represents an EDIF property. EDIF supports placing of simple and nested properties on different objects.
 */
class Property :
    public Commentable,
    public Nameable,
    public Renamable,
    public SelfReferencing<Property>,
	public Visitable
{
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif

    friend class FactoryType<Property>;

  public:

    typedef VisitorType<Property> Visitor;

    /**
     * Convenience class to create a property.
     */
    class Factory: public FactoryType<Property>
    {
        public:
            using FactoryType<Property>::create;
        /**
         * Create a property.
         *
         * @param[in] inName Name of the property to be created. 
         * @param[in] inContainer Pointer of the property container.
         * @param[in] inValue Value of the property.
         * @param[in] inUnit Unit of the property.
         *
         * @return Pointer to created property.
         */
            inline PropertySharedPtr
            virtual newPropertyPtr( const std::string &inName,
                const PropertyContainerSharedPtr &inContainer,
                const Value &inValue,
                const Unit  &inUnit = eUnitUndefined,
                const PropertySharedPtr &inParentProperty = PropertySharedPtr(),
                const std::string &inOriginalName = std::string()) throw(Error);
    };

    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    virtual
    ~Property() throw();

    /**
     * Get the Property inSource.
     *
     * @return Value of property.
     */
    inline const Value
    getValue() const throw();

    /**
     * Set the Property inSource.
     *
     * @param[in] inSource Value of property.
     */
    void
    setValue(const Value & inSource) throw();

    /**
     * Get the Unit inSource.
     *
     * @return Unit of property.
     */
    inline const Unit
    getUnit() const throw();

    /**
     * Set the Property inSource unit.
     *
     * @param[in] inSource Unit of property.
     */
    void
    setUnit(const Unit & inSource) throw();

    /**
     * Get the owner/source of this property.
     *
     * @return string Owner of this property
     */
    inline const std::string
    getOwner() const throw();

    /**
     * Set the owner/source of this property.
     *
     * @param[in] inSource Owner of this property
     */
    void
    setOwner(const std::string & inSource) throw();

    /**
     * Get the nested properties.
     *
     * @return map Containing properties
     */
    inline void
    getChildren(
        std::map< std::string,
            PropertySharedPtr > &outValues) const throw();

   /**
    * Apply action on all children.
    * @param[in] action Action to be applied
    *
    */
    template<typename _Action>
    inline void
    applyOnAllChildren( const _Action &action ) throw(Error); 

    /**
     * Set the nested properties.
     *
     * @param[in] inSource Map containing properties
     */
    void
    setChildren(const std::map< std::string,
        PropertySharedPtr > & inSource) throw();

    /**
     * Add a nested property to this property
     *
     * @param[in] inName Nested property name
     * @param[in] inProperty Nested property
     */
    bool
    addChildProperty(const std::string &inName,
        const PropertySharedPtr &inProperty) throw();

  protected:
    Property();

  private:
#ifdef GENOM_SERIALIZATION
	template<class Archive> void
	serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    std::string mOwner;
    Unit mUnit;
    Value mValue;
    SymTab<std::string, PropertySharedPtr > mChildren;
};

/**
 * Get the Property inSource.
 *
 * @return Value of property.
 */
inline const Value
Property::getValue() const throw() {
  return mValue;
}

/**
 * Get the Unit inSource.
 *
 * @return Unit of property.
 */
inline const Unit
Property::getUnit() const throw() {
  return mUnit;
}

/**
 * Get the owner/source of this property.
 *
 * @return string Owner of this property
 */
inline const std::string
Property::getOwner() const throw() {
  return mOwner;
}

/**
 * Get the nested properties.
 *
 * @param[out] outValues map Containing properties
 */
inline void
Property::getChildren(
    std::map< std::string, PropertySharedPtr > &outValues) const throw() {
  mChildren.getValueMap( outValues );
}

template<typename _Action>
inline void
Property::applyOnAllChildren( const _Action &action ) throw(Error)
{
    try
    {
        mChildren.applyOnAll( action );
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
 * Create a property.
 *
 * @param[in] inName Name of the property to be created.
 * @param[in] inContainer Pointer of the property container.
 * @param[in] inValue Value of the property.
 * @param[in] inUnit Unit of the property.
 *
 * @return Pointer to created property.
 */

inline PropertySharedPtr
Property::Factory::newPropertyPtr( const std::string &inName,
    const PropertyContainerSharedPtr &inContainer,
    const Value &inValue,
    const Unit  &inUnit,
    const PropertySharedPtr &inParentProperty,
    const std::string &inOriginalName ) throw(Error) {
    try
    {
        PropertySharedPtr newProperty;
        create( newProperty );
        newProperty->setName( inName );
        newProperty->setValue( inValue );
        newProperty->setUnit( inUnit );
        newProperty->setOriginalName( inOriginalName );
        if( inParentProperty )
        {
            inParentProperty->addChildProperty( inName, newProperty );
        }
        else
        {        
            inContainer->setProperty( inName, newProperty );
        }
        return newProperty;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}
} // namespace torc::generic

} // namespace torc
#endif
