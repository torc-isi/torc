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

/// \file
/// \brief Source for the ModuleTransformer class.

#include "torc/physical/ModuleTransformer.hpp"

namespace torc {
namespace physical {

	/// \brief Hierarchy separator
	const std::string ModuleTransformer::sHierarchySeparator = "/";

	/// \brief Port index separator
	const std::string ModuleTransformer::sPortIndexSeparator = "_";

	/// \brief Valid characters in a pin name.
	const boost::regex ModuleTransformer::sValidPinNameCharactersRegEx("[a-zA-Z0-9_.]");

	/// \brief Invalid first characters in a pin name.
	const boost::regex ModuleTransformer::sInvalidPinNameFirstCharactersRegEx("[0-9_.]");

} // namespace utils
} // namespace torc
