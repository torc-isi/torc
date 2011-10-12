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
/// \brief Header for the FrameSet class.

#ifndef TORC_BITSTREAM_FRAMESET_HPP
#define TORC_BITSTREAM_FRAMESET_HPP

#include "torc/bitstream/Frame.hpp"
#include "torc/bitstream/VirtexFrameAddress.hpp"

namespace torc {
namespace bitstream {

    class VirtexFrameAddress;
	/// \brief Set of contiguous frames.
	template <class FRAME_TYPE> class FrameSet 
		: public std::vector<boost::shared_ptr<FRAME_TYPE> > {
	protected:
	// typedefs
		/// \brief Imported type name.
		typedef boost::uint32_t uint32_t;
	public:
	// typedefs
		/// \brief FrameSet frame type.
		typedef FRAME_TYPE frame_t;
		/// \brief FrameSet word type.
		typedef typename FRAME_TYPE::word_t word_t;
		/// \brief Shared pointer encapsulation of a Frame.
		typedef boost::shared_ptr<FRAME_TYPE> FrameSharedPtr;
		/// \brief Shared pointer for frame addressing.
		typedef boost::shared_ptr<VirtexFrameAddress> VirtexFarSharedPtr;
		/// \brief Word shared array for frame words
		typedef boost::shared_array<uint32_t> WordSharedArray;
	// constructors
		/// \brief Null constructor.
		FrameSet(void) {}
		/// \brief Basic constructor for full bitstream.
		/// \var inFrameLength The default length of each frame in the set.
		/// \var inFrameCount The number of frames in the set.
		FrameSet(uint32_t inFrameLength, uint32_t inFrameCount) {
		  	for(uint32_t i = 0; i < inFrameCount; i++)
				push_back(FrameSharedPtr(new FRAME_TYPE(inFrameLength)));
		}
		/// \brief Basic constructor for partial bitstream.
		/// \var inFrameLength The default length of each frame in the set.
		/// \var inFrameCount The number of frames in the set.
		/// \var inWords The frame words
		/// \var inFar Address of the frames in the set
		FrameSet(uint32_t inFrameLength, uint32_t inFrameCount, WordSharedArray inWords, VirtexFrameAddress& inFar) {
		    VirtexFarSharedPtr farPtr = inFar;
		  	for(uint32_t i = 0; i < inFrameCount; i++)
				push_back(FrameSharedPtr(new FRAME_TYPE(inFrameLength, &inWords[i*inFrameLength])));
		}
	};

	typedef FrameSet<VirtexFrame> VirtexFrameSet;		///< \brief Virtex frame set type.
	typedef FrameSet<SpartanFrame> SpartanFrameSet;		///< \brief Spartan frame set type.
	typedef FrameSet<Spartan6Frame> Spartan6FrameSet;	///< \brief Spartan6 frame set type.

	/// \brief Set of contiguous frames for each of the eight possible block types.
	template <typename FRAME_TYPE> class FrameBlocks {
	protected:
	public:
	// enumerations
		/// \brief The block type count is fixed at eight across all Xilinx architectures.
		enum { eBlockTypeCount = 8 };
	// typedefs
		/// \brief FrameSet frame type.
		typedef FRAME_TYPE frame_t;
		/// \brief FrameSet word type.
		typedef typename FRAME_TYPE::word_t word_t;
	// members
		/// \brief FrameSets for each of the eight block types.
		FrameSet<FRAME_TYPE> mBlock[eBlockTypeCount];
	};

	typedef FrameBlocks<VirtexFrame> VirtexFrameBlocks;		///< \brief Virtex frame blocks type.
	typedef FrameBlocks<SpartanFrame> SpartanFrameBlocks;	///< \brief Spartan frame blocks type.
	typedef FrameBlocks<Spartan6Frame> Spartan6FrameBlocks;	///< \brief Spartan6 frame blocks type.

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_FRAMESET_HPP
