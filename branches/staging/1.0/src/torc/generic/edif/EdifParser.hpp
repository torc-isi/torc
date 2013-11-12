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

#ifndef TORC_GENERIC_EDIF_EDIFPARSER_HPP
#define TORC_GENERIC_EDIF_EDIFPARSER_HPP

//BOOST
#include <boost/shared_ptr.hpp>

#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/edif/ParserPointerTypes.hpp"
#include "torc/generic/Error.hpp"

namespace torc { namespace generic { class ObjectFactory; } }
namespace torc { namespace generic { class ParserOptions; } }
namespace torc { namespace generic { class Root; } }
namespace torc { namespace generic { class Linker; } }


namespace torc {
namespace generic {

/**
 * @brief The Top level parser for parsing EDIF files.
 * 
 * Most of this class will be generated during implementation. This document will be updated
 * accordingly.
 */
class EdifParser {
public:

	/**
	 * Parse a given edif file
	 * @param[in] inFileName Name of file to be parsed
	 * @param[out] outRoot Root to be populated
	 * @param[out] outLinker Linker to be used for this parse 
	 * @param[in] inFactory Factory object to be used for parsing
	 * @param[in] inOptions Options to the parser
	 */
	void parse(const std::string& inFileName, const RootSharedPtr& outRoot,
		const LinkerSharedPtr& outLinker, const ObjectFactorySharedPtr& inFactory,
		const ParserOptions& inOptions) throw (Error);

	/**
	 * Parse a given set of edif files
	 * @param[in] inFileNames Names of file to be parsed
	 * @param[out] outRoot Root to be populated
	 * @param[out] outLinker Linker to be used for this parse 
	 * @param[in] inFactory Factory object to be used for parsing
	 * @param[in] inOptions Options to the parser
	 */
	void parse(const std::vector<std::string>& inFileNames, const RootSharedPtr& outRoot,
		const LinkerSharedPtr& outLinker, const ObjectFactorySharedPtr& inFactory,
		const ParserOptions& inOptions) throw (Error);

	EdifParser();
	EdifParser(const EdifParser& inSource);

	virtual ~EdifParser() throw ();

	EdifParser& operator=(const EdifParser& inSource);
};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_EDIF_EDIFPARSER_HPP
