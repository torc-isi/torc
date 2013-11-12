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

#ifndef TORC_GENERIC_RENAMABLE_HPP
#define TORC_GENERIC_RENAMABLE_HPP

#include <string>

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION
namespace torc {
namespace generic {

/**
 * @brief Represents objects that can be renamed
 *
 * EDIF provides a way to store displayable string values with different objects using the
 * (rename ...) construct. The Renamable interface will be generalized by objects that need to
 * support this feature.
 */
class Renamable {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif
public:
	typedef std::string Name;

	/**
	 * Get the new name provided for the object.
	 *
	 * @return Name of the object
	 */
	inline virtual Name getOriginalName() const;

	/**
	 * Set the new name provided for the object.
	 *
	 * @param[in] inSource Name of the object
	 */
	virtual void setOriginalName(const Name& inSource);

protected:
	Renamable();

public:
	virtual ~Renamable() throw ();

private:
	Renamable(const Renamable&);
	Renamable& operator =(const Renamable&);

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
	Name mOriginalName;
};

/**
 * Get the new name provided for the object.
 *
 * @return Name of the object
 */
inline Renamable::Name Renamable::getOriginalName() const {
	return mOriginalName;
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_RENAMABLE_HPP
