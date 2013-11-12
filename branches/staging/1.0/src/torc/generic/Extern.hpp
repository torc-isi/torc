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

#ifndef TORC_GENERIC_EXTERN_HPP
#define TORC_GENERIC_EXTERN_HPP

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION
namespace torc {
namespace generic {

/**
 * @brief Used to implement external object referencing.
 *
 * The Extern interface is used by objects to create placeholders for objects that could not be
 * found by the Linker. In itself, the class is pretty simple and uses a single boolean inSource to
 * indicate whether an object is a real object or a place holder. Simplistically, extern objects
 * are used to represent libraries, cells, views and ports declared using the EDIF (extern ...)
 * syntax. However, in case of multifile parsing, the Linker will try to remove externs from the
 * design hierarchy with newly discovered items.
 */
class Extern {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif

public:
	/**
	 * Get whether the item is an extern(placeholder) or an actual item.
	 *
	 * @return True if extern, false otherwise
	 */
	inline bool getIsExtern() const;

	/**
	 * Get whether the item is an extern(placeholder) or an actual item.
	 *
	 * @return True if extern, false otherwise
	 */
	inline bool isExtern() const;

	/**
	 * Set whether an item is extern or not.
	 *
	 * @param[in] isExtern True if extern, false otherwise
	 */
	void setIsExtern(bool inIsExtern);

protected:
	Extern();

public:
	virtual ~Extern() throw ();

private:
	Extern(const Extern& source);

	Extern& operator=(const Extern& source);

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
	bool mIsExtern;
};

/**
 * Get whether the item is an extern(placeholder) or an actual item.
 *
 * @return True if extern, false otherwise
 */
inline bool Extern::getIsExtern() const {
	return mIsExtern;
}

/**
 * Get whether the item is an extern(placeholder) or an actual item.
 *
 * @return True if extern, false otherwise
 */
inline bool Extern::isExtern() const {
	return mIsExtern;
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_EXTERN_HPP
