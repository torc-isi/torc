// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
// $HeadURL: https://svn.east.isi.edu/torc/trunk/src/torc/bitstream/assembler/lut/parser.yy $
// $Id: parser.yy 1303 2013-02-25 23:18:16Z nsteiner $

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

/// \file VirtexAssembler.cpp
/// \brief Class to implement common functionalities of Virtex architectures

#include "torc/bitstream/assembler/VirtexAssembler.hpp"
#include "torc/Bitstream.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/Physical.hpp"
#include <iostream>

using namespace torc::bitstream;
using namespace torc::physical;

namespace torc {
namespace bitstream {

/// \details Public function to be called by user to initiate bitstream generation process.
///	The parameter inBaseBitstreamPath will act as base bitstream for micro-bitstream assembly process.
///	This function might be implemented in derived class for additional architecture specific processing.
int VirtexAssembler::generateBitstream(DesignSharedPtr inDesignPtr, const path inTargetBitstreamPath, 
	EMergeMode inMergeMode, path baseBitstreamPath) {

	int error = Assembler::generateBitstream(inDesignPtr, inTargetBitstreamPath,inMergeMode, baseBitstreamPath);

	if(error)
		return error;

	initializeFullFrameBlocks();

	convertXdlToBitstream();

	updateFullFrameBlocks();

	saveBitstream();

	return 0;
}

/// \details Store frame data and bit offset for site
void VirtexAssembler::initializeFrameDataForSite(const std::string &siteName) {

	SiteIndex siteIndex = mSites.findSiteIndex(siteName);
	const Site& site = mSites.getSite(siteIndex);

	// look up the site tile index
	TileIndex tileIndex = site.getTileIndex();
	return initializeFrameDataForTile(tileIndex);
}

/// \details Go over all the bitaddresses, split it into frame address, word address, and bit address.
/// Merge bits with outBitstream after handling offset.
void VirtexAssembler::mergeWithBaseBitstream(const std::vector<uint32_t> &bitAddresses,
	uint32_t blockIndex) {
	int32_t numSetBits = bitAddresses.size();
	//std::cout << "\tNumber of bits set: " << numSetBits 
	//				<< " Word offset: " << Hex32(mCurrWordOffset) << std::endl;

	VirtexFrameSet& frameSet = mCurrFrameBlocks.mBlock[blockIndex];
	if(!frameSet.empty()) {
		for(int i = 0; i < numSetBits; i++) {
			uint32_t bitAddr = bitAddresses[i];
			int32_t bitIndex = bitAddr & 0x000000FF; // get the lowest byte
			int32_t wordIndex = (bitAddr & 0x0000FF00) >> 8;
			// Library setting is for first row of clock region. Word index has to be offset to reach the correct row.
			wordIndex += mCurrWordOffset;
			int32_t frameIndex = (bitAddr & 0xFFFF0000) >> 16;
			//std::cout << "\t  Frame: " << Hex32(frameIndex) << " Word: "
			//		<< Hex32(wordIndex) << " bit: " << Hex32(bitIndex)
			//		<< std::endl;
			VirtexFrameSharedPtr virtexFrame = frameSet[frameIndex];

			VirtexFrame::word_t currentWord = virtexFrame->getWords()[wordIndex];
			if(mMergeMode == eSet) {
				currentWord = (1 << (bitIndex)) | currentWord;
			} else if(mMergeMode == eClear) {
				currentWord = ((1 << (bitIndex)) ^ 0xFFFFFFFF) & currentWord;
			}
			virtexFrame->setWord(wordIndex, currentWord);

		}
	} else {
		/// \todo Determine what this error condition actually means.
		std::cout << "WARNING: Unknown frame type." << std::endl;
	}
}

} // namespace bitstream
} // namespace torc
