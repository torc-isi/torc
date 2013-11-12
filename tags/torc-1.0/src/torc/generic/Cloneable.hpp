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

#ifndef TORC_GENERIC_CLONEABLE_HPP
#define TORC_GENERIC_CLONEABLE_HPP

//BOOST
#include <boost/shared_ptr.hpp>

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/weak_ptr.hpp>
#endif //GENOM_SERIALIZATION
/**
 * @file Cloneable.hpp
 * @brief Interface for Cloneable
 *
 * This file contains the interface for the Cloneable class
 */
namespace torc {
namespace generic {

/**
 * @brief Represents all EOM classes that can be cloned(copied).
 *
 * The Cloneable class defines a method called clone() that is overridden by derived classes to
 * provide a polymorphic copy operation.
 */
template <class _CloneableType> class Cloneable {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif
protected:
	/**
	 * Constructor to be used by inheriting classes
	 */
	Cloneable();

public:
	virtual ~Cloneable() throw ();

private:
	Cloneable(const Cloneable<_CloneableType>& source);

	Cloneable<_CloneableType>& operator=(const Cloneable<_CloneableType>& source);

public:
	/**
	 * Create a copy of this object. This method is polymorphic.
	 *
	 * @return Pointer to a copy of this object
	 */
	virtual boost::shared_ptr<_CloneableType>
	clone() = 0;

#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
};
/**
 * Constructor to be used by inheriting classes
 */
template <class _CloneableType> Cloneable<_CloneableType>::Cloneable() {}

template <class _CloneableType> Cloneable<_CloneableType>::~Cloneable() throw () {}

#ifdef GENOM_SERIALIZATION
template <class _CloneableType> template <class Archive> void
Cloneable<_CloneableType>::serialize(Archive& ar, unsigned int) {}
#endif //GENOM_SERIALIZATION
} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_CLONEABLE_HPP
