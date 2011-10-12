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
/// \brief Header for torc::physical output stream helpers.
/// \details The coverage and support of classes is quite limited at present, but may be expanded 
///		upon request.

#ifndef TORC_ARCHITECTURE_OUTPUT_STREAM_HELPERS_HPP
#define TORC_ARCHITECTURE_OUTPUT_STREAM_HELPERS_HPP

#include <iosfwd>

namespace torc {
namespace architecture {

	class Tilewire;
	class ExtendedWireInfo;
	class Arc;

	/// \brief Tilewire stream inserter.
	std::ostream& operator <<(std::ostream& os, const Tilewire& rhs);
	/// \brief ExtendedWireInfo stream inserter.
	std::ostream& operator <<(std::ostream& os, const ExtendedWireInfo& rhs);
	/// \brief Arc stream inserter.
	std::ostream& operator <<(std::ostream& os, const Arc& rhs);

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_OUTPUT_STREAM_HELPERS_HPP
