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

#include "torc/generic/edif/Driver.hpp"
#include "torc/generic/edif/EdifContext.hpp"
#include "torc/generic/edif/EdifParser.hpp"
#include "torc/generic/edif/ParserOptions.hpp"
#include "torc/generic/Root.hpp"
#include "torc/generic/edif/Linker.hpp"

namespace torc {
namespace generic {

EdifParser::EdifParser() {}

EdifParser::~EdifParser() throw () {}

EdifParser::EdifParser(const EdifParser& inSource) {}

EdifParser& EdifParser::operator=(const EdifParser& inSource) {
	return *this;
}

void EdifParser::parse(const std::string& inFileName, const RootSharedPtr& outRoot,
	const LinkerSharedPtr& outLinker, const ObjectFactorySharedPtr& inFactory,
	const ParserOptions& inOptions) throw (Error) {
	EdifContextSharedPtr context(new EdifContext(outRoot, outLinker, inFactory, inOptions));
	if(!outRoot || !outLinker || !inFactory) {
		Error e(eMessageIdErrorNullPointer, __FUNCTION__, __FILE__, __LINE__);
		throw;
	}
	Driver parserDriver(context);
	if(false == parserDriver.parseFile(inFileName)) {
		throw parserDriver.getParserError();
	}
	return;
}

void EdifParser::parse(const std::vector<std::string>& inFileNames, const RootSharedPtr& outRoot,
	const LinkerSharedPtr& outLinker, const ObjectFactorySharedPtr& inFactory,
	const ParserOptions& inOptions) throw (Error) {
	std::vector<std::string>::const_iterator it = inFileNames.begin();
	std::vector<std::string>::const_iterator end = inFileNames.end();
	try {
		for(; it != end; ++it) {
			parse(*it, outRoot, outLinker, inFactory, inOptions);
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

} // namespace generic
} // namespace torc
