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
/// \brief Header for the Spartan6Bitstream class.

#ifndef TORC_BITSTREAM_SPARTAN6BITSTREAM_HPP
#define TORC_BITSTREAM_SPARTAN6BITSTREAM_HPP

#include <boost/cstdint.hpp>
#include "torc/bitstream/Bitstream.hpp"
#include "torc/bitstream/Spartan6Packet.hpp"

namespace torc {
namespace bitstream {

namespace bitstream { class Spartan6BitstreamUnitTest; }

	/// \brief Spartan-class bitstream.
	class Spartan6Bitstream : virtual public Bitstream, public Spartan6PacketVector, 
		public Spartan6PacketConstants {
		friend class torc::bitstream::bitstream::Spartan6BitstreamUnitTest;
	protected:
	// typedefs
		/// \brief Imported type name.
		typedef boost::uint16_t uint16_t;
		typedef boost::uint32_t uint32_t;
	// inner classes
		/// \brief CRC class for the Spartan6 architecture.
		struct CRC {
			/// \brief Length of the CRC calculation.
			enum { eLen = 22 };
			/// \brief CRC calculation bits.
			uint8_t mBits[eLen];
			/// \brief CRC calculation value.
			uint32_t mValue;
			/// \brief Default constructor.
			CRC(void) { reset(); }
			/// \brief Function to clear the CRC calculation.
			void reset(void) { for(int32_t i = 0; i < eLen; i++) mBits[i] = 0; mValue = 0; }
			/// \brief Update the CRC with new data.
			void update(uint32_t inAddress, uint32_t inWord) {
				uint8_t input[eLen];
				uint8_t next[eLen];
				// initialize the input with the current register address and data word
				for(int32_t i = 21, mask = 0x20; i >= 16; i--, mask >>= 1) 
					input[i] = (inAddress & mask) ? 1 : 0;
				for(int32_t i = 15, mask = 0x8000; i >= 0; i--, mask >>= 1) 
					input[i] = (inWord & mask) ? 1 : 0;
				// update the CRC
				for(int32_t i = 21; i >= 16; i--) next[i] = mBits[i - 1] ^ input[i];
				next[15] = mBits[14] ^ input[15] ^ mBits[21];
				for(int32_t i = 14; i >= 13; i--) next[i] = mBits[i - 1] ^ input[i];
				next[12] = mBits[11] ^ input[12] ^ mBits[21];
				for(int32_t i = 11; i >= 8; i--) next[i] = mBits[i - 1] ^ input[i];
				next[7] = mBits[6] ^ input[7] ^ mBits[21];
				for(int32_t i = 6; i >= 1; i--) next[i] = mBits[i - 1] ^ input[i];
				next[0] = input[0] ^ mBits[21];
				// copy the updated bits into place
				mValue = 0;
				for(int32_t i = 0, mask = 1; i < eLen; i++, mask <<= 1) {
					mBits[i] = next[i];
					mValue |= mBits[i] ? mask : 0;
				}
			}
			/// \brief Index operator.
			uint8_t& operator[] (int i) { return mBits[i]; }
			/// \brief Cast operator to return the CRC as an integer.
			operator uint32_t (void) const { return mValue; }
		};
	// members
	public:
	// constructors
		/// \brief Basic constructor.
		Spartan6Bitstream(void) : Bitstream(), Spartan6PacketVector(), Spartan6PacketConstants() {}
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

#endif // TORC_BITSTREAM_SPARTAN6BITSTREAM_HPP
