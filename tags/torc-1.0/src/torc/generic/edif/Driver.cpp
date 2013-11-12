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

/*******************************************************************************
 * TORC - Copyright 2010 University of Southern California. All Rights Reserved.
 *
 * FILE : driver.cpp
 *
 * DATE : 08-July-2010
 *
 * DESCRIPTION : Defination of the torc::generic::Driver class
 *
 * REVISION HISTORY:
 *
 * SI        REVISION        AUTHOR               CHANGES          PRs
 *[0]    Initial Version    Niladri
 *[1]    Initial Version    Santanu
 *
 *******************************************************************************/

#include <fstream>
#include <sstream>

#include "torc/generic/edif/Driver.hpp"
#include "torc/generic/edif/ParserHelpers.hpp"
#include "torc/generic/PortAttributes.hpp"
#include "torc/generic/NetAttributes.hpp"
#include "torc/generic/edif/Scanner.hpp"
#include "torc/generic/InterfaceAttributes.hpp"
#include "torc/generic/LogicValueAttributes.hpp"

namespace torc {
namespace generic {

Driver::Driver(const EdifContextSharedPtr& inEdifCntx) : mTraceScanning(false),
	mTraceParsing(false), mEdifCntx(inEdifCntx), mErrorObj() {}

Driver::~Driver() throw () {}

bool Driver::parseStream(std::istream& inStream, const std::string& inStreamName) {
	mStreamName = inStreamName;

	Scanner scanner(&inStream);
	scanner.set_debug(mTraceScanning);
	this->mLexer = &scanner;

	Parser parser(*this);
	parser.set_debug_level(mTraceParsing);
	return (parser.parse() == 0);
}

bool Driver::parseFile(const std::string& inFileName) {
	std::ifstream in(inFileName.c_str());
	if(!in.good()) {
		return false;
	}
	return parseStream(in, inFileName);
}

bool Driver::parseString(const std::string& inString, const std::string& inStreamName) {
	std::istringstream iss(inString);
	return parseStream(iss, inStreamName);
}

void Driver::error(const location& inLocation, const std::string& inMessage) {
	std::cerr << "Error at:" << inLocation << ": " << inMessage << std::endl;
}

void Driver::error(const std::string& inMessage) {
	std::cerr << inMessage << std::endl;
}

void Driver::setParserError(const Error& inSource) {
	mErrorObj = ErrorSharedPtr(new Error(inSource));
}

} // namespace generic
} // namespace torc
