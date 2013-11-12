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

#include "torc/generic/UserDataContainer.hpp"

namespace torc {
namespace generic {

/**
 * Set the list of userData
 *
 * @param[in] inSource The list of UserData
 */
void UserDataContainer::setUserData(const std::list<std::string>& inSource) {
	mUserData.insert(mUserData.end(), inSource.begin(), inSource.end());
}

/**
 * Add an user data to the list of user data 
 * @param[in] inSource An user data as string 
 */
void UserDataContainer::addUserData(const std::string& inSource) {
	mUserData.push_back(inSource);
}

UserDataContainer::UserDataContainer() :
	mUserData() {}

UserDataContainer::~UserDataContainer() throw () {}

} // namespace generic
} // namespace torc
