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

#ifndef TORC_GENERIC_VECTOR_HPP
#define TORC_GENERIC_VECTOR_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

#include <algorithm>
#include <numeric>
#include <vector>

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/split_member.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Composite.hpp"
#include "torc/generic/om/CompositionType.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/SymTab.hpp"


namespace torc {

namespace generic {

/**
 * @brief An array of objects.
 *
 * This provides methods for accessing and adding elements to the array. It also implements the VectorPreservation algorithm if cPreserve is set to true. The vector class creates elements derived from VectorBit class by using the provided factory.
 */
template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve = false>
class Vector : public virtual Composite<_Type>
{
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

  public:
    typedef Composite<_Type>              BaseType;
    typedef typename BaseType::Type       Type;
    typedef typename BaseType::List       List;
    typedef typename BaseType::Pointer    Pointer;
    typedef typename BaseType::SizeType   SizeType;
    typedef _ChildType                    ChildType;
    typedef boost::shared_ptr<ChildType>  ChildPointer;
    typedef _ChildFactoryType             ChildFactory;
    typedef boost::shared_ptr<_ChildFactoryType> ChildFactorySharedPtr;

  private:
    typedef SymTab<SizeType,Pointer> SparseElements;

  protected:
    Vector();


  public:
    virtual
    ~Vector() throw();

    /**
     * Get composition type for this object
     *
     * @return The CompositionType inSource eCompositionTypeVector is returned
     */
    virtual CompositionType
    getCompositionType() const throw();

    /**
     * Get the total number of bits of the composition
     * @return Number of bits
     */
    virtual SizeType
    getSize() const throw();

#if 0
    /**
     * Add a child to the vector at the specified position. Note, that the size of the indices vector must be same as the dimensions of the array.
     *
     * @param[in] inChild A pointer to a child
     * @param[in] inIndices A list of indices where this item is to be placed in the container vector
     *
     * @exception Error The composition is not a container, the child type is not proper or the dimensions of the indices differ from the dimensions of the array
     */
    virtual void
    addChild(const boost::shared_ptr<Type> & inChild,
            const std::vector<int> & inIndices) throw(Error);
#endif

    /**
     * Get a specific member of this composition.
     *
     * @note This is relevant for Vector composition only. Other compositions return a NULL pointer
     *
     * @param[in] inIndices A list of indices to be accessed. The number of indices must be equal to the number of dimensions.
     *
     * @return A pointer to the child situated at the specified indices. For non-relevant types a NULL pointer is returned
     *
     * @exception Error Index dimensions mismatch
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorArrayIndexSizeMismatch
     *         </li>
     *         <li> Context Data
     *           <ul> 
     *               <li>Array Index Size - <i>size_t</i></li>
     *                <li>Array Dimension Size - <i>size_t</i></li>
     *           </ul> 
     *         </li>
     *       </ul> 
     *
     *  @exception Error Array index out of bounds
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorArrayIndexOutOfBounds
     *         </li>
     *         <li> Context Data
     *           <ul> 
     *               <li>Array Index - <i>size_t</i></li>
     *                <li>Array Dimension - <i>size_t</i></li>
     *           </ul> 
     *         </li>
     *       </ul> 
     *
     **  @exception Error Empty Array 
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyArray
     *         </li>
     *         <li> Context Data
     *           <ul> 
     *               <li>Array Dimension - <i>size_t</i></li>
     *           </ul> 
     *         </li>
     *       </ul> 
     *       </ul> 
     *
    */
    virtual const Pointer
    get(const std::vector<SizeType> & inIndices) const throw(Error);

    /**
     * Get children of this composition.
     *
     * @param[out] outChildren A list of all children for this composition
     *
     * @exception Error   If factory is not set and preserve is true and the vector has not been blasted, an exception is generated.
     */
    virtual void
    getChildren( List &outChildren ) const throw(Error);


  protected:
    /**
     * Set list of children for unpreserved vector.
     *
     * @param[in] inSource List of children
     */
    void
    setChildren(const List & inSource) throw(Error);

    virtual void
    onAutoBlast() const throw(Error);

  public:
    //This is an implementation convenience function
    //for derived classes. It returns the list of children
    //that have already been created in a preserved vector
    //For an unpreserved vector, use of this function is meaningless
    //and behavior is same as getChildren() function
    virtual void
    getCreatedChildren( List &outChildren ) const throw(Error);

  protected:
    //This is a function for implementors to do stuff on freshly created children
    //For example, for preserved vectors (eg. PortRef ) will use this function.
    //When the portref is already bound and a new bit is created,
    //it is expected, that the newly created child will be bound to the
   //corresponding bit of the existing master
    virtual void
    onChildCreate( const boost::shared_ptr<ChildType> &inCreatedChild
            ) const throw(Error);

  public:
    /**
     * Create list of children using the provided factory and limits
     *
     * @param[in] inFactory ChildFactory to use for construcing children
     * @param[in] inLimits Array dimensions
     * @exception Error Children could not be created
     *
     * @note setName() must have already been called before calling this method. For preserved arrays this does not actually do anything other than storing the factory and limits inside itself for later use.
     */
    inline void
    constructChildren(
        const boost::shared_ptr<ChildFactory> & inFactory,
        const std::vector<SizeType> &inLimits) throw(Error);


  private:
    inline void
    autoBlast() const throw(Error);

    void
    incrementIndices( std::vector<SizeType> &indices,
        const std::vector<SizeType> &limits ) const throw();


    inline SizeType
    indicesToAbsoluteIndex(
        const std::vector<SizeType> &inIndices ) const throw();

    inline SizeType
    storageSize() const;

  public:
    /**
     * Get dimensions of the array.
     *
     * @param[in] outLimits Dimensions of the vector
     */
    inline void
    getLimits(std::vector<SizeType> &outLimits) const throw();

    inline bool
    getIsPreserved() const throw();

  protected:
    /**
     * Set dimensions of the array.
     *
     * @param[in] inSource Dimensions of the vector
     */
    void
    setLimits(const std::vector<SizeType> & inSource) throw();

    /**
     * Get current factory
     *
     * @return Return ChildFactory object
     * 
     *  @exception Error Null Child Factory
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorNullChildfactory
     *         </li>
     *         <li> Context Data
     *           <ul> 
     *               <li>Child factory - <i>ChildFactory</i></li>
     *           </ul> 
     *         </li>
     *
 
     */
    inline const  boost::shared_ptr<ChildFactory>
    getFactory() const throw();

    /**
     * Set current factory
     *
     * @param[in] inSource ChildFactory object
     */
    void
    setFactory(const  boost::shared_ptr<ChildFactory> & inSource) throw();


  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    load( Archive &ar, unsigned int );

    template<class Archive> void
    save( Archive &ar, unsigned int ) const;

    BOOST_SERIALIZATION_SPLIT_MEMBER()
#endif //GENOM_SERIALIZATION

    std::vector<SizeType> mLimits;
    mutable List mChildren;
    mutable SparseElements mSparseElements;
    boost::shared_ptr<ChildFactory> mFactory;
    bool mIsPreserved;
    SizeType mPreservationThreshold;

    Vector(const Vector<_Type, _ChildType, _ChildFactoryType, cPreserve> & source) throw();

    Vector<_Type, _ChildType, _ChildFactoryType, cPreserve> &
    operator=(const Vector<_Type, _ChildType, _ChildFactoryType, cPreserve> & source) throw();

};

template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
Vector<_Type,_ChildType,_ChildFactoryType,cPreserve>::~Vector() throw() {
}

template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
Vector<_Type,_ChildType,_ChildFactoryType,cPreserve>::Vector()
    :BaseType(),
    mLimits(),
    mChildren(),
    mFactory(),
    mIsPreserved( cPreserve ),
    mPreservationThreshold( 1024 ) {
}

/**
 * Get composition type for this object
 *
 * @return The CompositionType inSource eCompositionTypeVector is returned
 */
template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
CompositionType
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::getCompositionType() const throw() {
    return eCompositionTypeVector;
}

/**
 * Get the total number of bits of the composition
 * @return Number of bits
 */
template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
typename Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::SizeType
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::getSize() const throw()
{
    return storageSize();
}

#if 0

/**
 * Add a child to the vector at the specified position. Note, that the size of the indices vector must be same as the dimensions of the array.
 *
 * @param[in] inChild A pointer to a child
 * @param[in] inIndices A list of indices where this item is to be placed in the container vector
 *
 * @exception Error The composition is not a container, the child type is not proper or the dimensions of the indices differ from the dimensions of the array
 */
template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
void
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::addChild(
    const boost::shared_ptr<Type> & inChild,
    const std::vector<SizeType> & inIndices) throw(Error) {

    //Error out if out of limits
    if( inIndices.size() > mLimits.size() )
    {
        //TBD::Error
    }
    if( cPreserve && mChildren.empty() )
    {
        try
        {
            autoBlast();
        }
        catch(Error &e)
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
}

#endif

/**
 * Get a specific member of this composition.
 *
 * @note This is relevant for Vector composition only. Other compositions return a NULL pointer
 *
 * @param[in] inIndices A list of indices to be accessed. The number of indices must be equal to the number of dimensions.
 *
 * @return A pointer to the child situated at the specified indices. For non-relevant types a NULL pointer is returned
 *
 * @exception Error Index dimensions mismatch
 */
template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
const typename Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::Pointer
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::get(
    const std::vector<SizeType> &inIndices) const throw(Error) {
    if( inIndices.size() != mLimits.size() )
    {
        Error e( eMessageIdErrorArrayIndexSizeMismatch,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Array Index", inIndices.size());
        e.saveContextData("Array Dimension", mLimits.size());
        throw e;
    }
    else
    {
        typename std::vector<SizeType>::const_iterator lend = mLimits.end();
        typename std::vector<SizeType>::const_iterator limit
                                        = mLimits.begin();
        typename std::vector<SizeType>::const_iterator index
                                        = inIndices.begin();
        for(; limit != lend; ++limit, ++index )
        {
            if( *index >= *limit )
            {
                Error e( eMessageIdErrorArrayIndexOutOfBounds,
                    __FUNCTION__, __FILE__, __LINE__ );
                e.saveContextData("Array Index", *index );
                e.saveContextData("Array Dimension", *limit );
                throw e;
            }
        }
    }

    SizeType index = indicesToAbsoluteIndex( inIndices );

    Pointer thisPtr = BaseType::getSharedThis();
    boost::shared_ptr<ChildFactory> factory = getFactory();
    if( !factory )
    {
        Error e( eMessageIdErrorNullChildfactory,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Null Child Factory", factory);
        throw e;
    }

    Pointer child;
    if( cPreserve && mChildren.empty() )
    {
        if( false == mSparseElements.get( index, child ) )
        {
            ChildPointer newChild;
            try
            {
                factory->create( newChild );
                newChild->setParentCollection( thisPtr );
                newChild->setIndices( inIndices );
                newChild->setAbsoluteIndex( index );
                mSparseElements.set( index, newChild );
                child = newChild;
                onChildCreate( newChild );
            }
            catch(Error &e)
            {
                e.setCurrentLocation(
                    __FUNCTION__, __FILE__, __LINE__ );
                throw;
            }
        }
        //TBD::THRESHOLD BASED CHECKING?
    }
    else
    {
        child = mChildren[ index ];
    }

    return child;
}

/**
 * Get children of this composition.
 *
 * @param[out] outChildren A list of all children for this composition
 *
 * @exception Error   If factory is not set and preserve is true and the vector has not been blasted, an exception is generated.
 */
template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
void
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::getChildren(
    typename Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::List &outChildren
            ) const throw(Error) {
    if( cPreserve //Is the vector preserved? Otherwise it already has all the elements
        && !mLimits.empty() //Do we have sufficient info to blast
        && mChildren.empty() )//Haven't we already blasted
    {
        try
        {
            autoBlast();
        }
        catch(Error &e)
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
    outChildren.insert( outChildren.end(),
                mChildren.begin(), mChildren.end() );
    return;
}

/**
 * Set list of children for unpreserved vector.
 *
 * @param[in] inSource List of children
 */
template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
void
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::setChildren(
    const Vector::List & inSource) throw(Error) {
  mChildren = inSource;
}

template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
void
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::onAutoBlast() const throw(Error)
{
    //Do nothing here
    //meant to be overridden by clients
}


/**
 * Create list of children using the provided factory and limits
 *
 * @param[in] inFactory ChildFactory to use for construcing children
 * @param[in] inLimits Array dimensions
 * @exception Error Children could not be created
 *
 * @note For preserved arrays this does not actually do anything other than storing the factory and limits inside itself for later use.
 */
template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
inline void
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::constructChildren(
    const boost::shared_ptr<ChildFactory> & inFactory,
    const std::vector<SizeType> &inLimits) throw(Error) {
    if( inLimits.empty() )
    {
        Error e( eMessageIdErrorEmptyArray,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Empty Array,", inLimits.empty());
        throw e;                                          
    }
    else
    {
        typename std::vector<SizeType>::const_iterator index
                                        = inLimits.begin();
        typename std::vector<SizeType>::const_iterator lend
                                        = inLimits.end();
        for(; index != lend; index++ )
        {
            if( *index == 0 )
            {
                Error e( eMessageIdErrorEmptyArray,
                    __FUNCTION__, __FILE__, __LINE__ );
                e.saveContextData("Empty Array,", *index);
                throw e;                                         
            }
        }
    }

    setFactory( inFactory );
    setLimits( inLimits );
    if( !cPreserve )
    {
        try
        {
            autoBlast();
        }
        catch(Error &e)
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__);
            throw;
        }
    }
}

template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
inline void
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::autoBlast() const throw(Error) {
    SizeType size = storageSize();
    mChildren.resize( mChildren.size() + size ); //create empty
    Pointer thisPtr = BaseType::getSharedThis();
    std::vector<SizeType> indices( mLimits.size() );
    fill( indices.begin(), indices.end(), 0 );
    if( !thisPtr )
    {
        Error e( eMessageIdErrorPointerToItemDoesNotExist,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Pointer to this object does not exist", thisPtr);
        throw e;
    }
    boost::shared_ptr<ChildFactory> factory = getFactory();
    if( !factory )
    {
        Error e( eMessageIdErrorNullChildfactory,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Null Child Factory", factory);
        throw e;
    }
    for( SizeType index = 0; index < size; index++ )
    {
        Pointer child;
        bool createNew = ( cPreserve
                && mSparseElements.getSize() > 0
                && mSparseElements.get( index, child ) )
                    ? false : true;
        if( createNew )
        {
            ChildPointer newChild;
            try
            {
                factory->create( newChild );
                newChild->setParentCollection( thisPtr );
                newChild->setIndices( indices );
                newChild->setAbsoluteIndex( index );
                child = newChild;
                onChildCreate( newChild );
            }
            catch(Error &e)
            {
                mChildren.clear(); //For consistency
                e.setCurrentLocation(
                    __FUNCTION__, __FILE__, __LINE__ );
                throw;
            }
        }
        incrementIndices( indices, mLimits );
        mChildren[ index ] = child;
    }
    mSparseElements.clear();
    try
    {
        onAutoBlast();
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__,
                __FILE__, __LINE__ );
        throw;
    }    
}

template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
void
Vector<_Type,_ChildType,_ChildFactoryType,cPreserve>::getCreatedChildren(
    List &outChildren ) const throw(Error) {
    if( cPreserve && mChildren.empty() )
    {
        mSparseElements.getValues( outChildren );
    }
    else if( !mChildren.empty() )
    {
        try
        {
            getChildren( outChildren );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
    return;
}

template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
void
Vector<_Type,_ChildType,_ChildFactoryType,cPreserve>::onChildCreate(
        const boost::shared_ptr<ChildType> &inCreatedChild) const throw(Error) {
    //If required, inherited classes will override this
}

template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
void
Vector<_Type,_ChildType,_ChildFactoryType,cPreserve>::incrementIndices(
            std::vector<SizeType> &indices,
            const std::vector<SizeType> &limits ) const throw()
{
    SizeType size = limits.size();
    bool incr = false;
    for( SizeType i = 0; i < size; i++ )
    {
        if( i == 0 || incr )
        {
            ++indices[i];
            if( indices[i] == limits[i] )
            {
                indices[i] = 0;
                incr = true;
            }
            else
            {
                incr = false;
            }
        }
    }
}

template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
inline typename Vector<_Type,_ChildType,_ChildFactoryType,cPreserve>::SizeType
Vector<_Type,_ChildType,_ChildFactoryType,cPreserve>::indicesToAbsoluteIndex(
    const std::vector<SizeType> &inIndices ) const throw()
{
    SizeType idx = 0;
    SizeType size = mLimits.size();
    for( SizeType k = 1; k <= size; k++ )
    {
        SizeType prod = 1;
        for(SizeType l = k+1; l <= size; l++ )
        {
            prod *= mLimits[ l - 1 ];
        }
        idx += prod * inIndices[ k - 1 ];
    }
    return idx;
}

template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
inline typename Vector<_Type,_ChildType,_ChildFactoryType,cPreserve>::SizeType
Vector<_Type,_ChildType,_ChildFactoryType,cPreserve>::storageSize(
) const
{
    return std::accumulate(
                mLimits.begin(), mLimits.end(), 1,
                std::multiplies<typename Vector<_Type,_ChildType,_ChildFactoryType,cPreserve>::SizeType>() );
}

/**
 * Get dimensions of the array.
 *
 * @return Dimensions of the vector
 */
template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
inline void
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::getLimits(
    std::vector<SizeType> &outLimits) const throw() {
  outLimits.insert( outLimits.end(),
        mLimits.begin(), mLimits.end() );
}

template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
inline bool
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::getIsPreserved() const throw()
{
    return mIsPreserved;
}

/**
 * Set dimensions of the array.
 *
 * @param[in] inSource Dimensions of the vector
 */
template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
void
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::setLimits(
    const std::vector<SizeType> & inSource) throw() {
  mLimits = inSource;
}

/**
 * Get current factory
 *
 * @return Return ChildFactory object
 */
template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
inline const  boost::shared_ptr<_ChildFactoryType>
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::getFactory() const throw() {
  return mFactory;
}

/**
 * Set current factory
 *
 * @param[in] inSource ChildFactory object
 */
template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
void
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::setFactory(
    const  boost::shared_ptr<Vector::ChildFactory> & inSource) throw() {
  mFactory = inSource;
}

#ifdef GENOM_SERIALIZATION
template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
template<class Archive> void
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::load(
                                        Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object< BaseType >( *this );
    ar & mLimits;
    ar & mChildren;
    ar & mSparseElements;
    ar & mIsPreserved;
    ar & mPreservationThreshold;
    if( !mChildren.empty() )
    {
        for_each( mChildren.begin(), mChildren.end(),
            boost::bind( boost::mem_fn(
                    &ChildType::setParentCollection ), _1,
                SelfReferencing<_Type>::getSharedThis() ));
    }
    else
    {
        mSparseElements.applyOnAll(
            boost::bind( boost::mem_fn(
                    &ChildType::setParentCollection ), _1, 
                SelfReferencing<_Type>::getSharedThis() ));
    }
}

template<
    typename _Type,
    typename _ChildType,
    typename _ChildFactoryType,
    bool cPreserve>
template<class Archive> void
Vector<_Type, _ChildType, _ChildFactoryType, cPreserve>::save(
                                        Archive &ar, unsigned int ) const {
    ar & boost::serialization::base_object< BaseType >( *this );
    ar & mLimits;
    ar & mChildren;
    ar & mSparseElements;
    ar & mIsPreserved;
    ar & mPreservationThreshold;
}

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
#endif
