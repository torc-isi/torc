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

/// \file
/// \brief Header for the Torc version data.

#ifndef TORC_VERSION_HPP
#define TORC_VERSION_HPP

#include "torc/common/DottedVersion.hpp"
#include <string>

namespace torc {

	/// \brief The current Torc version as a DottedVersion object.
	extern const torc::common::DottedVersion cTorcVersion;

	/// \brief The current Torc version as a constant string.
	extern const std::string cTorcVersionString;

} // namespace torc

#endif // TORC_VERSION_HPP
