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

#ifndef TORC_GENERIC_OM_INTERFACEATTRIBUTES_HPP
#define TORC_GENERIC_OM_INTERFACEATTRIBUTES_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/Commentable.hpp"
#include "torc/generic/om/UserDataContainer.hpp"

namespace torc {

namespace generic {

/**
 * @brief Represents attributes of a view interface.
 * 
 * The InterfaceAttributes class stores some optional attributes of a view interface. 
 */
class InterfaceAttributes :
    public Commentable,
    public UserDataContainer {

  public:
    /**
     * Get the designated number.
     *
     * @return std::string Value representing designated number of a view interface.
    */
    inline const std::string
    getDesignator() const throw();

    /**
     * Set the designated number.
     *
     * @param[in] inSource std::string representing designated number of a view interface.
    */
    void 
    setDesignator(const std::string & inSource) throw();

    /**
     * Get the pointer to the simulate.
     *
     * @return Pointer to the simulatie
     */
    inline const SimulateSharedPtr
    getSimulate() const throw();

    /**
     * Set the pointer to the simulate.
     *
     * @param[in] inSource Pointer to the simulate
     */
    void
    setSimulate(const SimulateSharedPtr & inSource ) throw();

    /**
     * Get the pointer to the timing object
     *
     * @return Pointer to the timing object
     */
    inline const TimingSharedPtr
    getTiming() const throw();

    /**
     * Set the pointer to the timing object
     *
     * @param[in] inSource Pointer to the timing object
     */
    void
    setTiming(const TimingSharedPtr & inSource ) throw();

    InterfaceAttributes();

    ~InterfaceAttributes() throw();

    InterfaceAttributes(const InterfaceAttributes & source);

    InterfaceAttributes & 
    operator=(const InterfaceAttributes & source) throw();

  private:
    std::string mDesignator;
    SimulateSharedPtr mSimulate;
    TimingSharedPtr mTiming;
    
};

inline const std::string 
InterfaceAttributes::getDesignator() const throw() {
    return mDesignator;
}

/**
 * Get the pointer to the simulate.
 *
 * @return Pointer to the simulatie
 */
inline const SimulateSharedPtr
InterfaceAttributes::getSimulate() const throw() {
    return mSimulate;
}

/**
 * Get the pointer to the timing object
 *
 * @return Pointer to the timing object
 */
inline const TimingSharedPtr
InterfaceAttributes::getTiming() const throw() {
    return mTiming;
}


} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_INTERFACEATTRIBUTES_HPP
