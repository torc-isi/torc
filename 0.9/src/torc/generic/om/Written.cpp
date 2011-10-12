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

#include "torc/generic/om/Written.hpp"
#include "torc/generic/om/Status.hpp"

namespace torc {

namespace generic {

/**
 * Create a written.
 *
 * @param[in] inTimeStamp TimeStamp in Universal Time Coordinate.
 * @param[in] inStatusPtr Pointer to parented(Status) object.
 * @param[in] inAuthorName Name of the author
 * @param[in] inProgramName Name of the program
 * @param[in] inProgramVersion Version of the program
 * @param[in] inDataOriginLocName Data Origin Location Name
 * @param[in] inDataOriginVersion Version of the DataOrigin
 *
 * @return Pointer to created written.
 **/
WrittenSharedPtr
Written::Factory::newWrittenPtr( const TimeStamp & inTimeStamp,
            const StatusSharedPtr &inStatusPtr,
            const std::string & inAuthorName,
            const std::string & inProgramName,
            const std::string & inProgramVersion,
            const std::string & inDataOriginLocName,
            const std::string & inDataOriginVersion ) throw(Error) {
    try
    {
        WrittenSharedPtr newWritten;
        create( newWritten );
        newWritten->setTimeStamp( inTimeStamp );
        newWritten->setAuthorName( inAuthorName );
        newWritten->setProgramName( inProgramName );
        newWritten->setProgramVersion( inProgramVersion );
        newWritten->setDataOriginLocationName( inDataOriginLocName );
        newWritten->setDataOriginVersion( inDataOriginVersion );
        if( inStatusPtr )
        {
            inStatusPtr->addWritten( newWritten );
        }
        return newWritten;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }    
}

/**
 * Set time stamp
 * 
 * @param[in] inTimeStamp TimeStamp in Universal Time Coordinate.
 */
void 
Written::setTimeStamp(const TimeStamp & inTimeStamp) throw() {
    mTimeStamp = inTimeStamp;
}

/**
 * Set author name
 * 
 * @param[in] inAuthorName Name of the author
 */
void 
Written::setAuthorName(const std::string & inAuthorName) throw() {
    mAuthorName = inAuthorName;
}

/**
 * Set program name
 * 
 * @param[in] inProgramName Name of the program
 */
void 
Written::setProgramName(const std::string & inProgramName) throw() {
    mProgramName = inProgramName;
}

/**
 * Set program version
 * 
 * @param[in] inProgramVersion Version of the program
 */
void 
Written::setProgramVersion(const std::string & inProgramVersion) throw() {
    mProgramVersion = inProgramVersion;
}

/**
 * Set Data origin location name 
 * 
 * @param[in] value Data Origin Location Name
 */
void 
Written::setDataOriginLocationName(const std::string & value) throw() {
  mDataOriginLocationName = value;
}

/**
 * Set DataOrigin version
 * 
 * @param[in] inDataOriginVersion Version of the DataOrigin
 */
void 
Written::setDataOriginVersion(const std::string & inDataOriginVersion) throw() {
    mDataOriginVersion = inDataOriginVersion;
}

Written::Written()
    :Commentable(),
    PropertyContainer(),
    UserDataContainer(),
    SelfReferencing<Written>(),
    mTimeStamp(),
    mAuthorName(),
    mProgramName(),
    mProgramVersion(),
    mDataOriginLocationName(),
    mDataOriginVersion() {
}

Written::~Written() throw() {
}

} // namespace torc::generic

} // namespace torc
