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

#ifndef TORC_GENERIC_OM_VECTORBIT_HPP
#define TORC_GENERIC_OM_VECTORBIT_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

#include <vector>

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Composite.hpp"
#include "torc/generic/om/CompositionType.hpp"
#include "torc/generic/util/Error.hpp"


namespace torc {

namespace generic {

/**
 * @brief Represents a single element of a vector composition
 *
 * A VectorBit is used to represent a bit of a vector. It contains a pointer to the vector and the particular index where it was stored. For a Vector where the _PRESERVE parameter is true, this is setup during the blast operation. Otherwise, this is moreor less similar to the Scalar composition.
 */
template<typename _Type>
class VectorBit : public virtual Composite<_Type>
{
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

  public:
    typedef _Type                       Type;
    typedef Composite<Type>             BaseType;
    typedef typename BaseType::List     List;
    typedef typename BaseType::Pointer  Pointer;
    typedef typename BaseType::SizeType SizeType;

    /**
     * Get composition type for this object
     *
     * @return The CompositionType inSource eCompositionTypeVectorBit is returned
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
     * Get children of this composition.
     * @param[out] outChildren A list of all children for this composition
     * @note This wil return empty list for scalar
     */
    virtual void
    getChildren( List &outChildren ) const throw(Error);

    /**
     * Get a specific member of this composition.
     *
     * @param[in] inIndices A list of indices to be accessed. The number of indices must be equal to the number of dimensions.
     *
     * @return An empty pointer is returned
     */
    virtual const Pointer
    get(const std::vector<SizeType> &inIndices) const throw(Error);

    /**
     * Get the index of this bit in the vector.
     *
     * @return A vector containing the indices to which this element belonged.
     */
    inline const std::vector<SizeType> &
    getIndices() const throw();

    /**
     * Set the index of this bit in the vector.
     *
     * @param[in] inSource A vector of indices for this element
     */
    void
    setIndices(const std::vector<SizeType> & inSource) throw();

    //TBD::BLOCK FROM DOC
    inline SizeType
    getAbsoluteIndex() const throw();

    void
    setAbsoluteIndex( SizeType inSource ) throw();

  protected:
    VectorBit();

  public:
    virtual
    ~VectorBit() throw();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    std::vector<SizeType> mIndices;
    SizeType mAbsoluteIndex;

    VectorBit(const VectorBit<_Type> & source) throw();
    VectorBit<_Type> &
    operator=(const VectorBit<_Type> & source) throw();

};
/**
 * Get composition type for this object
 *
 * @return The CompositionType inSource eCompositionTypeScalar is returned
 */
template<typename _Type>
CompositionType
VectorBit<_Type>::getCompositionType() const throw() {
    return eCompositionTypeVectorBit;
}

/**
 * Get the total number of bits of the composition
 * @return Number of bits
 */
template<typename _Type>
typename VectorBit<_Type>::SizeType
VectorBit<_Type>::getSize() const throw()
{
    return 1;
}

/**
 * Get children of this composition.
 *
 * @param[out] outChildren A list of all children for this composition
 */
template<typename _Type>
void
VectorBit<_Type>::getChildren(
    typename VectorBit<_Type>::List &outChildren ) const throw(Error) {
    return;
}

/**
 * Get a specific member of this composition.
 *
 * @param[in] indices A list of indices to be accessed. The number of indices must be equal to the number of dimensions.
 *
 * @return An empty pointer is returned
 */
template<typename _Type>
const typename VectorBit<_Type>::Pointer
VectorBit<_Type>::get(
    const std::vector<typename VectorBit<_Type>::SizeType> & indices) const throw(Error) {
    return Pointer();
}

/**
 * Get the index of this bit in the vector.
 *
 * @return A vector containing the indices to which this element belonged.
 */
template<typename _Type>
inline const std::vector<typename VectorBit<_Type>::SizeType> &
VectorBit<_Type>::getIndices() const throw() {
  return mIndices;
}

/**
 * Set the index of this bit in the vector.
 *
 * @param[in] inSource A vector of indices for this element
 */
template<typename _Type>
void
VectorBit<_Type>::setIndices(
    const std::vector<SizeType> & inSource) throw() {
  mIndices = inSource;
}

template<typename _Type>
inline typename VectorBit<_Type>::SizeType
VectorBit<_Type>::getAbsoluteIndex() const throw() {
  return mAbsoluteIndex;
}

template<typename _Type>
void
VectorBit<_Type>::setAbsoluteIndex( SizeType inSource ) throw() {
  mAbsoluteIndex = inSource;
}

template<typename _Type>
VectorBit<_Type>::VectorBit()
    :BaseType(),
    mIndices() {
}

template<typename _Type>
VectorBit<_Type>::~VectorBit() throw() {
}
#ifdef GENOM_SERIALIZATION
template<typename _Type>
template<class Archive> void
VectorBit<_Type>::serialize( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object< Composite<_Type> >( *this );
    ar & mIndices;
    ar & mAbsoluteIndex;
}
#endif //GENOM_SERIALIZATION



} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_VECTORBIT_HPP
