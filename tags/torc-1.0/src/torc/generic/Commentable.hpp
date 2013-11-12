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

#ifndef TORC_GENERIC_COMMENTABLE_HPP
#define TORC_GENERIC_COMMENTABLE_HPP

#include <string>
#include <vector>

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/weak_ptr.hpp>
#endif //GENOM_SERIALIZATION
namespace torc {
namespace generic {

/**
 * @brief Represents all classes that can hold user comments
 *
 * The Commentable class holds a list of user comments that the users can modify. This class is
 * inherited by all classes that can hold comments by user. Comments can be programatically
 * inserted by clients or can be specified in edif files by <i>(comment "string1" ..)</i> syntax.
 */
class Commentable {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif

public:
	/**
	 * Add a comment to the object
	 *
	 * @param[in] comment The comment to add to an existing list of comments
	 */
	void addComment(const std::string& comment);

	/**
	 * Get the list of all comments
	 *
	 * @return A list of all comments on this object
	 */
	inline const std::vector<std::string>& getComments() const;

	/**
	 * Get the list of all comments
	 *
	 * @param[in] inSource A list of all comments to be set on this object
	 */
	void setComments(const std::vector<std::string>& inSource);

protected:
	Commentable();

public:
	virtual ~Commentable() throw ();

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
	Commentable(const Commentable& source);

	Commentable& operator=(const Commentable& source);

	std::vector<std::string> mComments;
};

/**
 * Get the list of all comments
 *
 * @return A list of all comments on this object
 */
inline const std::vector<std::string>& Commentable::getComments() const {
	return mComments;
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_COMMENTABLE_HPP
