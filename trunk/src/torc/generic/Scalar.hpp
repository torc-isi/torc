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

#ifndef TORC_GENERIC_SCALAR_HPP
#define TORC_GENERIC_SCALAR_HPP

#include <vector>

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/Composite.hpp"
#include "torc/generic/CompositionType.hpp"
#include "torc/generic/Error.hpp"

namespace torc {
namespace generic {

/**
 * @brief A single object with no child objects
 */
template <typename _Type> class Scalar : virtual public Composite<_Type> {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION
public:
	typedef typename Composite<_Type>::Type Type;
	typedef typename Composite<_Type>::Pointer Pointer;
	typedef typename Composite<_Type>::List List;
	typedef typename Composite<_Type>::SizeType SizeType;

	virtual ~Scalar() throw ();

	/**
	 * Get composition type for this object
	 *
	 * @return The CompositionType inSource eCompositionTypeScalar is returned
	 */
	virtual CompositionType
	getCompositionType() const;

	/**
	 * Get the total number of bits of the composition
	 * @return Number of bits
	 */
	virtual SizeType getSize() const;

	/**
	 * Get a specific member of this composition.
	 *
	 * @param[in] indices A list of indices to be accessed. The number of indices must be equal to
	 * the number of dimensions.
	 *
	 * @return An empty pointer is returned
	 */
	virtual const Pointer get(const std::vector<SizeType>& indices) const throw (Error);

	/**
	 * Get children of this composition.
	 * @param[out] outChildren A list of all children for this composition
	 * @note This wil return empty list for scalar
	 */
	virtual void getChildren(List& outChildren) const throw (Error);

protected:
	Scalar();

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
	Scalar(const Scalar<_Type>& source);
	Scalar<_Type>& operator=(const Scalar<_Type>& source);
};

template <typename _Type> Scalar<_Type>::~Scalar() throw () {}

/**
 * Get composition type for this object
 *
 * @return The CompositionType inSource eCompositionTypeScalar is returned
 */
template <typename _Type> CompositionType Scalar<_Type>::getCompositionType() const {
	return eCompositionTypeScalar;
}

/**
 * Get the total number of bits of the composition
 * @return Number of bits
 */
template <typename _Type> typename Scalar<_Type>::SizeType Scalar<_Type>::getSize() const {
	return 1;
}

/**
 * Get a specific member of this composition.
 *
 * @param[in] indices A list of indices to be accessed. The number of indices must be equal to the
 * number of dimensions.
 *
 * @return An empty pointer is returned
 */
template <typename _Type> const typename Scalar<_Type>::Pointer Scalar<_Type>::get(
	const std::vector<typename Scalar<_Type>::SizeType>& indices) const throw (Error) {
	return Pointer();
}

/**
 * Get children of this composition.
 *
 * @param[out] outChildren A list of all children for this composition
 */
template <typename _Type> void Scalar<_Type>::getChildren(
	typename Scalar<_Type>::List& outChildren) const throw (Error) {
	return;
}

template <typename _Type> Scalar<_Type>::Scalar() : Composite<_Type>() {}

#ifdef GENOM_SERIALIZATION
template <typename _Type> template <class Archive> void Scalar<_Type>::serialize( Archive& ar,
	unsigned int ) {
	//NOTHING TO DUMP
}
#endif //GENOM_SERIALIZATION

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_SCALAR_HPP
