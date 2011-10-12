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
/// \brief Header for the VirtexBitstream class.

#ifndef TORC_BITSTREAM_VIRTEXFRAMEADDRESS_HPP
#define TORC_BITSTREAM_VIRTEXFRAMEADDRESS_HPP

#include <boost/cstdint.hpp>

namespace torc {
namespace bitstream {

	/// \brief Virtex Frame Address base-class.
	class VirtexFrameAddress {
	protected:
	// typedefs
		/// \brief Imported type name.
		typedef boost::uint32_t uint32_t;
	// functions
		/// \brief Assigns members of FAR.
		virtual void assign(uint32_t inAddress) = 0;
	public:
	// destructors
		virtual ~VirtexFrameAddress() {}
	// functions
		/// \brief Equality operator FAR.
		virtual bool operator== (const VirtexFrameAddress& rhs) const = 0;
		/// \brief Comparison operator FAR.
		virtual bool operator< (const VirtexFrameAddress& rhs) const = 0;
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_VIRTEXFRAMEADDRESS_HPP
