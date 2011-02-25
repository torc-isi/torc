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
/// \brief Header for the DottedVersion class.

#ifndef TORC_COMMON_DOTTED_VERSION_HPP
#define TORC_COMMON_DOTTED_VERSION_HPP

#include <boost/cstdint.hpp>

namespace torc {
namespace common {

	/// \brief Encapsulation of dotted decimal DottedVersion numbers.
	struct DottedVersion {
		typedef boost::uint8_t uint8_t;
		uint8_t mMajor;
		uint8_t mMinor;
		uint8_t mRevision;
		uint8_t mReserved;
		DottedVersion(uint8_t inMajor = 0, uint8_t inMinor = 0, uint8_t inRevision = 0, 
			uint8_t inReserved = 0) : mMajor(inMajor), mMinor(inMinor), mRevision(inRevision), 
			mReserved(inReserved) {}
	};

} // namespace common
} // namespace torc

#endif // TORC_COMMON_DOTTED_VERSION_HPP
