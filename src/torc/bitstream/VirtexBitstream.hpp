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
/// \brief Header for the VirtexBitstream class.

#ifndef TORC_BITSTREAM_VIRTEXBITSTREAM_HPP
#define TORC_BITSTREAM_VIRTEXBITSTREAM_HPP

#include <boost/cstdint.hpp>
#include "torc/bitstream/Bitstream.hpp"
#include "torc/bitstream/VirtexPacket.hpp"
#include "torc/bitstream/FrameSet.hpp"
#include "torc/common/DeviceDesignator.hpp"
#include <fstream>

namespace torc {
namespace bitstream {

namespace bitstream { class VirtexBitstreamUnitTest; }

	/// \brief Virtex-class bitstream.
	class VirtexBitstream : virtual public Bitstream, public VirtexPacketVector, 
		public VirtexPacketConstants, public VirtexFrameBlocks {
		friend class torc::bitstream::bitstream::VirtexBitstreamUnitTest;
	protected:
	// typedefs
		/// \brief Imported type name.
		typedef boost::uint32_t uint32_t;
	// functions
		/// \brief Reports the named function as not yet implemented in the current architecture.
		void unimplemented(std::string inName);
		/// \brief Update CRC-16 packets.
		virtual void updateCrc16(torc::common::DeviceDesignator::EFamily inFamily);
		/// \brief Update CRC-32 packets.
		virtual void updateCrc32(torc::common::DeviceDesignator::EFamily inFamily);
		/// \brief Read frame packets in for Virtex4/5/6/7 devices.
		template <class ARCH> void readFramePackets4567(uint32_t inBlockFrameIndexBounds[], 
			std::map<typename ARCH::FrameAddress, uint32_t>& inFrameAddressToIndex, 
			std::map<uint32_t, typename ARCH::FrameAddress>& inFrameIndexToAddress);
		/// \brief Discard the existing frame packets for Virtex4/5/6/7 devices.
		template <class ARCH> VirtexPacketVector::iterator deleteFramePackets4567(void);
		/// \brief Return a packet vector with full frame data for Virtex4/5/6/7 devices.
		template <class ARCH> VirtexPacketVector generateFullBitstreamPackets4567(
			uint32_t inBlockFrameIndexBounds[]);
		/// \brief Return a packet vector with partial frame data for Virtex4/5/6/7 devices.
		template <class ARCH> VirtexPacketVector generatePartialBitstreamPackets4567(
			EFrameInclude inFrameInclusion, 
			std::map<typename ARCH::FrameAddress, uint32_t>& inFrameAddressToIndex, 
			std::map<uint32_t, typename ARCH::FrameAddress>& inFrameIndexToAddress);
	// members
		/// \brief Input Frame blocks.
		VirtexFrameBlocks mFrameBlocks;
		uint32_t mIdCode;
	public:
	// constructors
		/// \brief Basic constructor.
		VirtexBitstream(void) : Bitstream(), VirtexPacketVector(), VirtexPacketConstants(), 
			VirtexFrameBlocks(), mIdCode(0) {}
	// functions
		/// \brief Read bitstream packets from a stream.
		/// \note This function should be called after the bitstream header has been read.
		virtual void readPackets(std::istream& inStream);
		/// \brief Write bitstream packets to a stream.
		virtual void writePackets(std::ostream& inStream);
		/// \brief Read frame data into the frame blocks structure.
		virtual void readFramePackets(void);
		/// \brief Update the bitstream frame packets
		/// \detail By default this updates the bitstream with full frame data.
		/// \param inBitstreamType The type of bitstream to generate: full or partial.
		/// \param inFrameInclusion The type of frames to include in partial bitstreams: only dirty 
		///		frames or all frames.
		virtual void updateFramePackets(EBitstreamType inBitstreamType = eBitstreamTypeFull, 
			EFrameInclude inFrameInclusion = eFrameIncludeAllUsedFrames);
		/// \brief Discard existing packets and generate bitstream packets from frame data.
		/// \detail By default this generates packets for a full bitstream.
		/// \param inBitstreamType The type of bitstream to generate: full, active partial, or 
		///		shutdown partial.
		/// \param inFrameInclusion The type of frames to include in partial bitstreams: only dirty 
		///		frames or all frames.
		virtual void generateBitstream(EBitstreamType inBitstreamType = eBitstreamTypeFull, 
			EFrameInclude inFrameInclusion = eFrameIncludeAllUsedFrames);
		/// \brief Discard the existing frame packets and return an iterator to the start position.
		virtual VirtexPacketVector::iterator deleteFramePackets(void);
		/// \brief Return a packet vector with full frame data.
		virtual VirtexPacketVector generateFullBitstreamPackets(void);
		/// \brief Return a packet vector with the full bitstream prefix.
		virtual VirtexPacketVector generateFullBitstreamPrefix(void);
		/// \brief Return a packet vector with the full bitstream suffix.
		virtual VirtexPacketVector generateFullBitstreamSuffix(void);
		/// \brief Return a packet vector with partial frame data.
		/// \param inFrameInclusion The type of frames to include: only dirty frames or all frames.
		virtual VirtexPacketVector generatePartialBitstreamPackets(EFrameInclude inFrameInclusion);
		/// \brief Return a packet vector with the partial bitstream prefix.
		/// \param inBitstreamType The type of partial bitstream to generate: active or shutdown.
		virtual VirtexPacketVector generatePartialBitstreamPrefix(EBitstreamType inBitstreamType);
		/// \brief Return a packet vector with the partial bitstream suffix.
		/// \param inBitstreamType The type of partial bitstream to generate: active or shutdown.
		virtual VirtexPacketVector generatePartialBitstreamSuffix(EBitstreamType inBitstreamType);
		/// \brief Preflight the packets.
		virtual void preflightPackets(void);
		/// \brief Update the header packet length.
		virtual void updatePacketLength(void);
		/// \brief Regular std::vector inserter.
		iterator insert(iterator position, const value_type& val) {
			return VirtexPacketVector::insert(position, val);
		}
		/// \brief Regular std::vector inserter.
		void insert(iterator position, size_type n, const value_type& val) {
			VirtexPacketVector::insert(position, n, val);
		}
		/// \brief Regular std::vector inserter.
		template <class InputIterator> void insert(iterator position, InputIterator first, 
			InputIterator last) {
			VirtexPacketVector::insert(position, first, last);
		}
		/// \brief Insert the contents of a packet vector at the specified position.
		void insert(VirtexPacketVector::iterator inPosition, 
			const VirtexPacketVector& inVector) {
			VirtexPacketVector::insert(inPosition, inVector.begin(), inVector.end());
		}
		/// \brief Append the contents of a packet vector to the end of the bitstream.
		void append(const VirtexPacketVector& inVector) {
			VirtexPacketVector::insert(end(), inVector.begin(), inVector.end());
		}
		uint32_t getIdCode() {
			return mIdCode;
		}
	// accessors
		/// \brief Return the configuration frame blocks.
		VirtexFrameBlocks& getFrameBlocks(void) { return mFrameBlocks; }
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_VIRTEXBITSTREAM_HPP
