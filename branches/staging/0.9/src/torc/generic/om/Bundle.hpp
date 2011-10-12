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

#ifndef TORC_GENERIC_OM_BUNDLE_HPP
#define TORC_GENERIC_OM_BUNDLE_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

#include <algorithm>
#include <vector>

#ifdef GENOM_SERIALIZATION
#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Composite.hpp"
#include "torc/generic/om/CompositionType.hpp"
#include "torc/generic/util/Error.hpp"


namespace torc {

namespace generic {

/**
 * @brief Represents a "bundle" in the EDIF sense
 *
 * A bundle in EDIF is an ordered collection of items. For example a bundle of ports is a collection of other scalar and vector ports. The Bundle class represents an implementation of functionality for such a collection.
 */
template<typename _Type>
class Bundle : public virtual Composite<_Type> {
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif
  public:
    typedef Composite<_Type>             BaseType;
    typedef typename BaseType::Type      Type;
    typedef typename BaseType::Pointer   Pointer;
    typedef typename BaseType::List      List;
    typedef typename BaseType::SizeType  SizeType;

    typedef Type ChildType;

  protected:
    Bundle();

  public:
    virtual
    ~Bundle() throw();

    /**
     * Get composition type for this object
     *
     * @return The CompositionType inSource VECTOR is returned
     */
    virtual CompositionType
    getCompositionType() const throw();

    /**
     * Get the total number of bits of the composition
     * @return Number of bits
     */
    virtual SizeType
    getSize() const throw();

    /**
     * Add a child to the bundle. Note that, the new element is added to the end of the array.
     *
     * @param[in] child A pointer to a child
     *
     * @exception Error The composition is not a container or the child type is not proper
     */
    virtual void
    addChild(const boost::shared_ptr<Type> & child) throw(Error);

    /**
     * Get children of this composition.
     *
     * @param[out] outChildren A list of all children for this composition
     */
    virtual void
    getChildren( List &outChildren ) const throw(Error);

    template<typename _Action>
    void
    applyOnAllChildren( const _Action &action ) throw(Error);

    /**
     * Get a specific member of this composition.
     *
     * @return An empty pointer is returned
     *
     * @exception Error Index dimensions mismatch
     */
    virtual const Pointer
    get(const std::vector<SizeType> & indices) const throw(Error);

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    load( Archive &ar, unsigned int );

    template<class Archive> void
    save( Archive &ar, unsigned int ) const;
    
    BOOST_SERIALIZATION_SPLIT_MEMBER()
#endif //GENOM_SERIALIZATION

    List mChildren;
};


template<typename _Type>
Bundle<_Type>::Bundle()
    :Composite<_Type>() {
}

template<typename _Type>
Bundle<_Type>::~Bundle() throw() {
}

/**
 * Get composition type for this object
 *
 * @return The CompositionType inSource VECTOR is returned
 */
template<typename _Type>
CompositionType
Bundle<_Type>::getCompositionType() const throw()
{
    return eCompositionTypeBundle;
}

/**
 * Get the total number of bits of the composition
 * @return Number of bits
 */
template<typename _Type>
typename Bundle<_Type>::SizeType
Bundle<_Type>::getSize() const throw()
{
    size_t size = 0;
    for( typename Bundle<_Type>::List::const_iterator it
            = mChildren.begin(); it != mChildren.end(); ++ it )
    {
        size += (*it)->getSize();
    }
    return size;
}

/**
 * Add a child to the bundle. Note that, the new element is added to the end of the array.
 *
 * @param[in] child A pointer to a child
 *
 * @exception Error The composition is not a container or the child type is not proper
 */
template<typename _Type>
void
Bundle<_Type>::addChild(
    const boost::shared_ptr<Type> & child) throw(Error) {
    mChildren.push_back( child );
    child->setParentCollection(
            SelfReferencing<_Type>::getSharedThis() );
}

/**
 * Get children of this composition.
 *
 * @param[out] outChildren A list of all children for this composition
 */
template<typename _Type>
void
Bundle<_Type>::getChildren(
    typename Bundle<_Type>::List &outChildren ) const throw(Error) {
    outChildren.insert( outChildren.end(),
                mChildren.begin(), mChildren.end() );
    return;
}

template<typename _Type>
template<typename _Action>
void
Bundle<_Type>::applyOnAllChildren(
                    const _Action &action ) throw(Error) {
    try
    {
        std::for_each( mChildren.begin(), mChildren.end(), action );
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

}

/**
 * Get a specific member of this composition.
 *
 * @return An empty pointer is returned
 *
 * @exception Error Index dimensions mismatch
 */
template<typename _Type>
const typename Bundle<_Type>::Pointer
Bundle<_Type>::get(
    const std::vector<typename Bundle<_Type>::SizeType> & indices) const throw(Error) {
    return Pointer();
}

#ifdef GENOM_SERIALIZATION
template<typename _Type>
template<class Archive> void
Bundle<_Type>::load( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object<BaseType>(*this);
    ar & mChildren;
    for_each( mChildren.begin(), mChildren.end(),
            boost::bind( boost::mem_fn(
                    &Composite<_Type>::setParentCollection ), _1,
                SelfReferencing<_Type>::getSharedThis() ));
}

template<typename _Type>
template<class Archive> void
Bundle<_Type>::save( Archive &ar, unsigned int ) const {
    ar & boost::serialization::base_object<BaseType>(*this);
    ar & mChildren;
}

#endif //GENOM_SERIALIZATION


} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_BUNDLE_HPP
