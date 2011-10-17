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

#ifndef TORC_GENERIC_OM_TIMESTAMP_HPP
#define TORC_GENERIC_OM_TIMESTAMP_HPP

//BOOST
#include <boost/cstdint.hpp>

namespace torc {

namespace generic {

/**
 * @brief Represents  the time in Universal Time Coordinate 
 * (year, month, day, hour, minute, second)
 */
class TimeStamp
{
  private:
    uint32_t mYear;
    uint32_t mMonth;
    uint32_t mDay;
    uint32_t mHour;
    uint32_t mMinute;
    uint32_t mSecond;

  public:
    TimeStamp(uint32_t inYear, uint32_t inMonth, uint32_t inDay, 
                uint32_t inHour, uint32_t inMinute, uint32_t inSecond);
    
    TimeStamp();

    ~TimeStamp() throw();

    TimeStamp( const TimeStamp & source );

    TimeStamp &
    operator =(const TimeStamp & source) throw();

    /**
     * Get year information
     * 
     * @return mYear 
     */
    inline uint32_t 
    getYear() const throw();

    /**
     * Set year information
     * 
     * @param[in] inYear Year information 
     */
    void 
    setYear(const uint32_t & inYear) throw();

    /**
     * Get month information
     * 
     * @return mMonth 
     */
    inline uint32_t 
    getMonth() const throw();

    /**
     * Set month information
     * 
     * @param[in] inMonth Month information 
     */
    void 
    setMonth(const uint32_t & inMonth) throw();

    /**
     * Get day information
     * 
     * @return mDay 
     */
    inline uint32_t 
    getDay() const throw();

    /**
     * Set day information
     * 
     * @param[in] inDay Day information 
     */
    void 
    setDay(const uint32_t & inDay) throw();

    /**
     * Get hour information
     * 
     * @return mHour 
     */
    inline uint32_t 
    getHour() const throw();

    /**
     * Set hour information
     * 
     * @param[in] inHour Hour information 
     */
    void 
    setHour(const uint32_t & inHour) throw();

    /**
     * Get minute information
     * 
     * @return mMinute 
     */
    inline uint32_t 
    getMinute() const throw();

    /**
     * Set minute information
     * 
     * @param[in] inMinute Minute information 
     */
    void 
    setMinute(uint32_t inMinute) throw();

    /**
     * Get second information
     * 
     * @return mSecond 
     */
    inline uint32_t 
    getSecond() const throw();

    /**
     * Set second information
     * 
     * @param[in] inSecond Second information 
     */
    void 
    setSecond(uint32_t inSecond) throw();

};
/**
 * Get year information
 * 
 * @return mYear 
 */
inline uint32_t 
TimeStamp::getYear() const throw() {
    return mYear;
}

/**
 * Get month information
 * 
 * @return mMonth 
 */
inline uint32_t 
TimeStamp::getMonth() const throw() {
    return mMonth;
}

/**
 * Get day information
 * 
 * @return mDay 
 */
inline uint32_t 
TimeStamp::getDay() const throw() {
    return mDay;
}

/**
 * Get hour information
 * 
 * @return mHour 
 */
inline uint32_t 
TimeStamp::getHour() const throw() {
    return mHour;
}

/**
 * Get minute information
 * 
 * @return mMinute 
 */
inline uint32_t 
TimeStamp::getMinute() const throw() {
    return mMinute;
}

/**
 * Get second information
 * 
 * @return mSecond 
 */
inline uint32_t 
TimeStamp::getSecond() const throw() {
    return mSecond;
}


} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_TIMESTAMP_HPP
