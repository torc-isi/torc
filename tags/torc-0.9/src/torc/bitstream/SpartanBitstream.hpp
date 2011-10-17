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
/// \brief Header for the SpartanBitstream class.

#ifndef TORC_BITSTREAM_SPARTANBITSTREAM_HPP
#define TORC_BITSTREAM_SPARTANBITSTREAM_HPP

#include <boost/cstdint.hpp>
#include "torc/bitstream/Bitstream.hpp"
#include "torc/bitstream/SpartanPacket.hpp"

namespace torc {
namespace bitstream {

namespace bitstream { class SpartanBitstreamUnitTest; }

	/// \brief Spartan-class bitstream.
	class SpartanBitstream : public Bitstream, public SpartanPacketVector, 
		public SpartanPacketConstants {
		friend class torc::bitstream::bitstream::SpartanBitstreamUnitTest;
	protected:
	// typedefs
		/// \brief Imported type name.
		typedef boost::uint32_t uint32_t;
	// members
	public:
	// constructors
		/// \brief Basic constructor.
		SpartanBitstream(void) : Bitstream(), SpartanPacketVector(), SpartanPacketConstants() {}
	// functions
		/// \brief Read bitstream packets from a stream.
		/// \note This function should be called after the bitstream header has been read.
		virtual void readPackets(std::istream& inStream);
		/// \brief Write bitstream packets to a stream.
		virtual void writePackets(std::ostream& inStream);
		/// \brief Preflight the packets.
		virtual void preflightPackets(void);
		/// \brief Update the header packet length.
		virtual void updatePacketLength(void);
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_SPARTANBITSTREAM_HPP
