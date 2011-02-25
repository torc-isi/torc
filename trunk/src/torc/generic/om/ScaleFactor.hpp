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

#ifndef TORC_GENERIC_SCALE_FACTOR_HPP
#define TORC_GENERIC_SCALE_FACTOR_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Value.hpp"

namespace torc {

namespace generic {

/**
 * Represents the technology dependent scale factor. This is used by clients to perform calculations using the units involved.
 */
class ScaleFactor
{
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

  public:
    /**
     * Get the EDIF scale.
     *
     * @return EDIF scale.
     */
    inline const Value::Number
    getEdifValue() const throw();

    /**
     * Set the EDIF scale.
     *
     * @param[in] inSource EDIF scale.
     */
    void
    setEdifValue(const Value::Number & inSource) throw();

    /**
     * Get the SI untis scale.
     *
     * @return SI scale.
     */
    inline const Value::Number
    getSiValue() const throw();

    /**
     * Set the SI scale.
     *
     */
    void
    setSiValue(const Value::Number & inSource) throw();

    ScaleFactor();

    ScaleFactor( const ScaleFactor &inSource );

    ~ScaleFactor();

    ScaleFactor &
    operator = ( const ScaleFactor &inSource );

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    Value::Number mEdifValue;
    Value::Number mSiValue;
};

/**
 * Get the EDIF scale.
 *
 * @return EDIF scale.
 */
inline const Value::Number
ScaleFactor::getEdifValue() const throw() {
  return mEdifValue;
}

/**
 * Get the SI untis scale.
 *
 * @return SI scale.
 */
inline const Value::Number
ScaleFactor::getSiValue() const throw() {
  return mSiValue;
}


} // namespace torc::generic

} // namespace torc
#endif
