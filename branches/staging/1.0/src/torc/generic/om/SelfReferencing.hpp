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

#ifndef TORC_GENERIC_OM_SELFREFERENCING_HPP
#define TORC_GENERIC_OM_SELFREFERENCING_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/weak_ptr.hpp>
#endif //GENOM_SERIALIZATION

namespace torc {
namespace generic {

template <typename _Tp>
class SelfReferencing
{
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

  public:
    typedef _Tp Type;
    typedef boost::shared_ptr<Type> Pointer;
    typedef boost::weak_ptr<Type> WeakPointer;

    /**
     * Set a weak pointer to this object. This will be used later to get a shared pointer to this object from within other member methods if required. This should be called by the Factory creating the object.
     *
     * @param[in] weakThis A weak pointer to this object
     */
    inline void
    setWeakThis(const WeakPointer & inWeakThis) throw();

    /**
     * Get a shared pointer to this object. A valid pointer is returned if weakThis was set. Otherwise this returns a NULL pointer. For Vector types, the second scenario is an exception condition and should be treated as such.
     *
     * @return A shared pointer to this object
     */
    inline Pointer
    getSharedThis() const throw();

  protected:
    SelfReferencing();

  public:
    ~SelfReferencing() throw();

  private:
#ifdef GENOM_SERIALIZATION
	template<class Archive> void
	serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    WeakPointer mWeakThis;
};

template<typename _Type>
SelfReferencing<_Type>::SelfReferencing()
    :mWeakThis() {
}

template<typename _Type>
SelfReferencing<_Type>::~SelfReferencing() throw() {
}

/**
 * Set a weak pointer to this object. This will be used later to get a shared pointer to this object from within other member methods if required. This should be called by the Factory creating the object.
 *
 * @param[in] weakThis A weak pointer to this object
 */
template<typename _Type>
inline void
SelfReferencing<_Type>::setWeakThis(
    const SelfReferencing<_Type>::WeakPointer &inWeakThis) throw() {
    mWeakThis = inWeakThis;
}

/**
 * Get a shared pointer to this object. A valid pointer is returned if weakThis was set. Otherwise this returns a NULL pointer. For Vector types, the second scenario is an exception condition and should be treated as such.
 *
 * @param[out] ptr Set to a shared pointer to this object
 */
template<typename _Type>
inline typename SelfReferencing<_Type>::Pointer
SelfReferencing<_Type>::getSharedThis() const throw() {
    return mWeakThis.lock();
}

#ifdef GENOM_SERIALIZATION
template<typename _Type>
template<class Archive> void
SelfReferencing<_Type>::serialize( Archive &ar, unsigned int ) {
	ar & mWeakThis;
}
#endif //GENOM_SERIALIZATION

} //namespace generic
} //namespace torc

#endif // TORC_GENERIC_OM_SELFREFERENCING_HPP
