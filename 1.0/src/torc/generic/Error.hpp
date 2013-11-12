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

#ifndef TORC_GENERIC_ERROR_HPP
#define TORC_GENERIC_ERROR_HPP

#include <map>
#include <string>
#include <vector>

//BOOST
#include <boost/any.hpp>
#include <boost/cstdint.hpp>

#include "torc/generic/MessageId.hpp"

namespace torc {
namespace generic {

/**
 * @brief The Error object thrown by different methods of EdifOM
 * 
 * The Error class is used to convey exception information to the user. The error object is
 * constructed by the throwing method and propagated upwards by the calling functions.
 * 
 * @note This class does not inherit from std::exception by design. Clients should keep that in
 * mind while using this class.
 */
class Error {
public:
	/**
	 * Context sensitive data for the error object. The Context map stores a boost::any inSource
	 * corresponding to a name. As the Error bubbles up, this data may be augmented by catching
	 * contexts.
	 */
	typedef std::map<std::string, boost::any> Context;

	/**
	 * Represents the throw and catch locations of the exception. Contains function, file and line
	 * numbers.
	 */
	struct StackFrameInfo {
	private:
		std::string mFunction;
		std::string mFile;
		uint32_t mLine;

	public:
		StackFrameInfo(const std::string& inFunction, const std::string& inFile, uint32_t inLine);

		~StackFrameInfo() throw ();

		StackFrameInfo(const StackFrameInfo& source);

		StackFrameInfo& operator=(const StackFrameInfo& source);

		inline const std::string getFunction() const;

		inline const std::string getFile() const;

		inline const uint32_t getLine() const;

	};

	/**
	 * Constructor.
	 *
	 * @param[in] inId Error message identifier that can be used to look up the message in
	 * MessageTable.
	 * @param[in] inContext Context data that will be saved inside this error object.
	 * @param[in] inFunction Function name from where this exception is being thrown. This will
	 * typically be set to __FUNCTION__.
	 * @param[in] inFile File name from where this exception is being thrown. This will typically
	 * be set to __FILE__.
	 * @param[in] inLine Line No. in the file from where this exception is being thrown. This will
	 * typically be set to __LINE__.
	 */
	Error(MessageId inId, const Context& inContext, const std::string& inFunction,
		const std::string& inFile, uint32_t inLine);

	/**
	 * @overload
	 */
	Error(MessageId inId, const std::string& inFunction, const std::string& inFile,
		uint32_t inLine);

	~Error() throw ();

	Error(const Error& source);

	Error& operator=(const Error& source);

	/**
	 * Get the complete stack trace.
	 *
	 * @return vector containing the StackFrameInfo objects.
	 */
	inline const std::vector<StackFrameInfo>& getStackTrace() const;

	/**
	 * Set the current location. This method should be used by catching contexts to push location
	 * data into the error object.
	 *
	 * @param[in] inFunction Function name from where this exception is being thrown. This will
	 * typically be set to __FUNCTION__.
	 * @param[in] inFile File name from where this exception is being thrown. This will typically
	 * be set to __FILE__.
	 * @param[in] inLine Line No. in the file from where this exception is being thrown. This will
	 * typically be set to __LINE__.
	 */
	void setCurrentLocation(const std::string& inFunction, const std::string& inFile,
		uint32_t inLine);

	/**
	 * Get the map of context data for this exception. This can be looked up by interested parties
	 * for context sensitive information. Note that the value_type for this map is boost::any and
	 * therefore an appropriate boost::any_cast is required to get the actual data.
	 *
	 * @return const reference to a Context object.
	 */
	inline const Context getContextData() const;

	/**
	 * Save a context sensitive data with a meaningful name, that can be retreived by interested
	 * catching contexts.
	 *
	 * @param[in] inName Name of the data.
	 * @param[in] inSource Any type of data. The only restrictions are that the type of data should
	 * be copy constructible and assignable.
	 */
	void saveContextData(const std::string& inName, const boost::any& inSource);

	/**
	 * Get the error message Id for this error.
	 *
	 * @return MessageId corresponding to this error
	 */
	inline const MessageId getErrorMessageId() const;

private:
	std::vector<StackFrameInfo> mStackTrace;
	Context mContextData;
	MessageId mErrorMessageId;
};

inline const std::string Error::StackFrameInfo::getFunction() const {
	return mFunction;
}

inline const std::string Error::StackFrameInfo::getFile() const {
	return mFile;
}

inline const uint32_t Error::StackFrameInfo::getLine() const {
	return mLine;
}

/**
 * Get the complete stack trace.
 * 
 * @return vector containing the StackFrameInfo objects.
 */
inline const std::vector<Error::StackFrameInfo>& Error::getStackTrace() const {
	return mStackTrace;
}

/**
 * Get the map of context data for this exception. This can be looked up by interested parties for
 * context sensitive information. Note that the value_type for this map is boost::any and therefore
 * an appropriate boost::any_cast is required to get the actual data.
 * 
 * @return const reference to a Context object.
 */
inline const Error::Context Error::getContextData() const {
	return mContextData;
}

/**
 * Get the error message Id for this error.
 * 
 * @return MessageId corresponding to this error
 */
inline const MessageId Error::getErrorMessageId() const {
	return mErrorMessageId;
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_ERROR_HPP
