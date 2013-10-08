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

#include "torc/generic/om/TimeStamp.hpp"

namespace torc {

namespace generic {

TimeStamp::TimeStamp(uint32_t inYear, uint32_t inMonth, uint32_t inDay, 
                uint32_t inHour, uint32_t inMinute, uint32_t inSecond)
    :mYear( inYear ),
    mMonth( inMonth ),
    mDay( inDay ),
    mHour( inHour ),
    mMinute( inMinute ),
    mSecond( inSecond ) {
}

TimeStamp::TimeStamp()
    :mYear(0),
    mMonth(0),
    mDay(0),
    mHour(0),
    mMinute(0),
    mSecond(0) {
}

TimeStamp::TimeStamp( const TimeStamp & source )
    :mYear( source.mYear ),
    mMonth( source.mMonth ),
    mDay( source.mDay ),
    mHour( source.mHour ),
    mMinute( source.mMinute ),
    mSecond( source.mSecond ) {
}

TimeStamp &
TimeStamp::operator =( const TimeStamp & source ) throw() {
    if( this != &source ) {
        mYear = source.mYear;
        mMonth = source.mMonth;
        mDay = source.mDay;
        mHour = source.mHour;
        mMinute = source.mMinute;
        mSecond = source.mSecond;
    }
    return *this; 
}

TimeStamp::~TimeStamp() throw() {
}

/**
 * Set year information
 * 
 * @param[in] inYear Year information 
 */
void 
TimeStamp::setYear(const uint32_t & inYear) throw() {
    mYear = inYear;
}

/**
 * Set month information
 * 
 * @param[in] inMonth Month information 
 */
void 
TimeStamp::setMonth(const uint32_t & inMonth) throw() {
    mMonth = inMonth;
}

/**
 * Set day information
 * 
 * @param[in] inDay Day information 
 */
void 
TimeStamp::setDay(const uint32_t & inDay) throw() {
    mDay = inDay;
}

/**
 * Set hour information
 * 
 * @param[in] inHour Hour information 
 */
void 
TimeStamp::setHour(const uint32_t & inHour) throw() {
    mHour = inHour;
}

/**
 * Set minute information
 * 
 * @param[in] inMinute Minute information 
 */
void 
TimeStamp::setMinute(uint32_t inMinute) throw() {
    mMinute = inMinute;
}

/**
 * Set second information
 * 
 * @param[in] inSecond Second information 
 */
void 
TimeStamp::setSecond(uint32_t inSecond) throw() {
    mSecond = inSecond;
}


} // namespace torc::generic

} // namespace torc
