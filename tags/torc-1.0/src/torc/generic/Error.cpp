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

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

#include "torc/generic/Error.hpp"

namespace torc {
namespace generic {

Error::StackFrameInfo::StackFrameInfo(const std::string& inFunction, const std::string& inFile,
	uint32_t inLine) : mFunction(inFunction), mFile(inFile), mLine(inLine) {}

Error::StackFrameInfo::~StackFrameInfo() throw () {}

Error::StackFrameInfo::StackFrameInfo(const Error::StackFrameInfo& source) :
	mFunction(source.mFunction), mFile(source.mFile), mLine(source.mLine) {}

Error::StackFrameInfo& Error::StackFrameInfo::operator=(const Error::StackFrameInfo& source) {
	if(this != &source) {
		mFunction = source.mFunction;
		mFile = source.mFile;
		mLine = source.mLine;
	}
	return *this;
}

Error::Error(MessageId inId, const Context& inContext, const std::string& inFunction,
	const std::string& inFile, uint32_t inLine) : mStackTrace(), mContextData(inContext),
	mErrorMessageId(inId) {
	StackFrameInfo info(inFunction, inFile, inLine);
	mStackTrace.push_back(info);
}

/**
 * @overload
 */
Error::Error(MessageId inId, const std::string& inFunction, const std::string& inFile,
	uint32_t inLine) :
	mStackTrace(), mContextData(), mErrorMessageId(inId) {
	StackFrameInfo info(inFunction, inFile, inLine);
	mStackTrace.push_back(info);
}

Error::~Error() throw () {}

Error::Error(const Error& source) : mStackTrace(source.mStackTrace),
	mContextData(source.mContextData), mErrorMessageId(source.mErrorMessageId) {}

Error& Error::operator=(const Error& source) {
	if(this != &source) {
		mStackTrace = source.mStackTrace;
		mContextData = source.mContextData;
		mErrorMessageId = source.mErrorMessageId;
	}
	return *this;
}

void Error::setCurrentLocation(const std::string& inFunction, const std::string& inFile,
	uint32_t inLine) {
	StackFrameInfo info(inFunction, inFile, inLine);
	mStackTrace.push_back(info);
}

void Error::saveContextData(const std::string& inName, const boost::any& inSource) {
	mContextData[inName] = inSource;
}

} // namespace generic
} // namespace torc
