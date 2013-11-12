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

#ifndef TORC_GENERIC_PARENTEDOBJECT_HPP
#define TORC_GENERIC_PARENTEDOBJECT_HPP

#include <string>

//BOOST
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/weak_ptr.hpp>
#endif //GENOM_SERIALIZATION
namespace torc {
namespace generic {

/**
 * @brief An object that has a parent
 *
 * EDIF 2 0 0 defines a nested hierarchy of objects in a design. As such most of the non-root
 * elements have a corresponding parent object. However, the type of parent depends on the type of
 * object. The ParentedObject template therefore provides a mechanism for storing a pointer to the
 * parent object.
 */
template <typename _ParentType>
class ParentedObject {

#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif

protected:
	ParentedObject();

public:
	virtual
	~ParentedObject() throw ();

private:
	ParentedObject(const ParentedObject<_ParentType>& source);

	ParentedObject<_ParentType>& operator=(const ParentedObject<_ParentType>& source);

public:
	/**
	 * Get a pointer to the parent object
	 *
	 * @return Pointer to parent
	 */
	inline const boost::shared_ptr<_ParentType> getParent() const;

	/**
	 * Set a pointer to the parent
	 *
	 * @param[in] inSource Set a pointer to the parent
	 */
	virtual void setParent(const boost::shared_ptr<_ParentType>& inSource);

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
	boost::weak_ptr<_ParentType> mParent;
};

template <typename _ParentType> ParentedObject<_ParentType>::ParentedObject() : mParent() {}

template <typename _ParentType> ParentedObject<_ParentType>::~ParentedObject() throw () {}

/**
 * Get a pointer to the parent object
 *
 * @return Pointer to parent
 */
template <typename _ParentType> inline const boost::shared_ptr<_ParentType>
	ParentedObject<_ParentType>::getParent() const {
	return mParent.lock();
}

/**
 * Set a pointer to the parent
 *
 * @param[in] inSource Set a pointer to the parent
 */
template <typename _ParentType> void ParentedObject<_ParentType>::setParent(
	const boost::shared_ptr<_ParentType>& inSource) {
	mParent = inSource;
}

#ifdef GENOM_SERIALIZATION
template <typename _ParentType> template <class Archive> void
	ParentedObject<_ParentType>::serialize(Archive& ar, unsigned int) {
	ar & mParent;
}
#endif //GENOM_SERIALIZATION

} // namespace generic
} // namespace torc
#endif // TORC_GENERIC_PARENTEDOBJECT_HPP
