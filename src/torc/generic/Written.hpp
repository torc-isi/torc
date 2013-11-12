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

#ifndef TORC_GENERIC_WRITTEN_HPP
#define TORC_GENERIC_WRITTEN_HPP

#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/Commentable.hpp"
#include "torc/generic/PropertyContainer.hpp"
#include "torc/generic/UserDataContainer.hpp"
#include "torc/generic/TimeStamp.hpp"
#include "torc/generic/SelfReferencing.hpp"
#include "torc/generic/FactoryType.hpp"

namespace torc {
namespace generic {

/**
 * @brief Represents an information container relating to the writer of the EDIF file
 */
class Written : public Commentable, public PropertyContainer, public UserDataContainer,
	public SelfReferencing<Written> {

	friend class FactoryType<Written> ;

public:

	/**
	 * Convenience class to create a written.
	 */
	class Factory : public FactoryType<Written> {
	public:
		using FactoryType<Written>::create;
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
		virtual WrittenSharedPtr
		newWrittenPtr(const TimeStamp& inTimeStamp, const StatusSharedPtr& inStatusPtr,
			const std::string& inAuthorName = std::string(),
			const std::string& inProgramName = std::string(),
			const std::string& inProgramVersion = std::string(),
			const std::string& inDataOriginLocName = std::string(),
			const std::string& inDataOriginVersion = std::string()) throw (Error);
	};

	/**
	 * Get the time stamp
	 *
	 * @return mTimeStamp The time in Universal Time Coordinate (year, month, day, hour, minute,
	 * 		second))
	 */
	inline const TimeStamp getTimeStamp() const;

	/**
	 * Set time stamp
	 *
	 * @param[in] inTimeStamp TimeStamp in Universal Time Coordinate.
	 */
	void setTimeStamp(const TimeStamp& inTimeStamp);

	/**
	 * Get author name
	 *
	 * @return mAuthorName
	 */
	inline const std::string getAuthorName() const;

	/**
	 * Set author name
	 *
	 * @param[in] inAuthorName Name of the author
	 */
	void setAuthorName(const std::string& inAuthorName);

	/**
	 * Get program name
	 *
	 * @return mProgramName
	 */
	inline const std::string getProgramName() const;

	/**
	 * Set program name
	 *
	 * @param[in] inProgramName Name of the program
	 */
	void setProgramName(const std::string& inProgramName);

	/**
	 * Get program version
	 *
	 * @return mProgramVersion
	 */
	inline const std::string getProgramVersion() const;

	/**
	 * Set program version
	 *
	 * @param[in] inProgramVersion Version of the program
	 */
	void setProgramVersion(const std::string& inProgramVersion);

	/**
	 * Get Data origin location name
	 *
	 * @return mDataOriginLocationName
	 */
	inline const std::string getDataOriginLocationName() const;

	/**
	 * Set Data origin location name
	 *
	 * @param[in] value Data Origin Location Name
	 */
	void setDataOriginLocationName(const std::string& value);

	/**
	 * Get Data origin version
	 *
	 * @return mDataOriginVersion
	 */
	inline const std::string getDataOriginVersion() const;

	/**
	 * Set Data origin version
	 *
	 * @param[in] inDataOriginVersion Version of the DataOrigin
	 */
	void setDataOriginVersion(const std::string& inDataOriginVersion);

	Written();

	~Written() throw ();

private:
	TimeStamp mTimeStamp;
	std::string mAuthorName;
	std::string mProgramName;
	std::string mProgramVersion;
	std::string mDataOriginLocationName;
	std::string mDataOriginVersion;

};
/**
 * Get the time stamp
 * 
 * @return mTimeStamp The time in Universal Time Coordinate (year, month, day, hour, minute, second))
 */
inline const TimeStamp Written::getTimeStamp() const {
	return mTimeStamp;
}

/**
 * Get author name
 * 
 * @return mAuthorName
 */
inline const std::string Written::getAuthorName() const {
	return mAuthorName;
}

/**
 * Get program name
 * 
 * @return mProgramName
 */
inline const std::string Written::getProgramName() const {
	return mProgramName;
}

/**
 * Get program version
 * 
 * @return mProgramVersion
 */
inline const std::string Written::getProgramVersion() const {
	return mProgramVersion;
}

/**
 * Get Data origin location name 
 * 
 * @return mDataOriginLocationName
 */
inline const std::string Written::getDataOriginLocationName() const {
	return mDataOriginLocationName;
}

/**
 * Get DataOrigin  version
 * 
 * @return mDataOriginVersion
 */
inline const std::string Written::getDataOriginVersion() const {
	return mDataOriginVersion;
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_WRITTEN_HPP
