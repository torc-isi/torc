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
/// \brief Header for the TilewirePlaceholder class.

#ifndef TORC_PHYSICAL_TILEWIRE_PLACEHOLDER_HPP
#define TORC_PHYSICAL_TILEWIRE_PLACEHOLDER_HPP

#include <boost/cstdint.hpp>

namespace torc {
namespace physical {

	// Pack Tilewire objects tightly.
	#ifdef __GNUC__
		#pragma pack(push, 2)
	#endif

	/// \brief Drop-in placeholder for a Tilewire with no torc::architecture dependencies.
	class TilewirePlaceholder {
	protected:
	// members
		/// \brief The tile index placeholder.
		boost::uint32_t m32;
		/// \brief The wire index placeholder.
		boost::uint16_t m16;
	public:
	// constructors
		/// \brief Null constructor.
		TilewirePlaceholder(void) : m32(0), m16(0) {}
	};

	#ifdef __GNUC__
		#pragma pack(pop)
	#endif

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_TILEWIRE_PLACEHOLDER_HPP
