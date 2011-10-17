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

#ifndef TORC_GENERIC_OM_USERDATACONTAINER_HPP
#define TORC_GENERIC_OM_USERDATACONTAINER_HPP

#include <iostream>
#include <string>
#include <list>

#include "torc/generic/util/Error.hpp"

namespace torc { namespace generic { class BaseVisitor; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents class that can hold  userData
 * 
 * The UserDataContainer interface is generalized by classes that need to hold one or more userData. 
 */
class UserDataContainer {

  public:
    
    /**
     * Get the list of all userData
     *
     * @param[out]  outUserData The list of all userData
     */
    inline void 
    getUserData(std::list< std::string > & outUserData) const throw();

    /**
     * Set the list of userData
     *
     * @param[in] inSource The list of UserData
     */
    void 
    setUserData(const std::list< std::string > & inSource) throw();
    
    /**
     * Add an user data to the list of user data 
     * @param[in] inSource An user data as string
     */
    void
    addUserData( const std::string &inSource ) throw();

  protected:
    UserDataContainer();

  public:
    ~UserDataContainer() throw();

  private:
    UserDataContainer(const UserDataContainer & source) throw();

    UserDataContainer & 
    operator=(const UserDataContainer & source) throw();

    std::list< std::string > mUserData;
};

/**
 * Get the list of all userData
 *
 * @param[out]  outUserData The list of all userData
 */
inline void
UserDataContainer::getUserData(std::list< std::string > & outUserData) const throw() {
    outUserData.insert( outUserData.end(),
                mUserData.begin(), mUserData.end() );
    return;
}

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_USERDATACONTAINER_HPP
