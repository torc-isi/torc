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

#include "torc/bitstream/assembler/Virtex7Assembler.hpp"

namespace torc {
namespace bitstream {

/// \details Store frame data and bit offset for given tile index
void Virtex7Assembler::initializeFrameDataForTile(TileIndex tileIndex) {
	const TileInfo& tileInfo = mTiles.getTileInfo(tileIndex);

	const TileRow &tileRow = tileInfo.getRow();
	const TileCol &tileCol = tileInfo.getCol();


	torc::bitstream::Bitstream * bitstreamPtr = mBitstreamPtr.get();
	torc::bitstream::Virtex7 *virtex7BitStream =
		dynamic_cast<torc::bitstream::Virtex7 *> (bitstreamPtr);

	uint32_t bitIndexBegin = 0, bitIndexEnd = 0;

	uint32_t primaryXdlCol = virtex7BitStream->getPrimaryXdlColumn(tileRow, tileCol);

	mCurrFrameBlocks = virtex7BitStream->getXdlFrames(tileRow, primaryXdlCol, bitIndexBegin,
		bitIndexEnd, torc::bitstream::Bitstream::eBlockTypeCount);
	
	// Call getXdlFrames with original column to get bit index offset
	virtex7BitStream->getXdlFrames(tileRow, tileCol, bitIndexBegin,
		bitIndexEnd, torc::bitstream::Bitstream::eBlockTypeCount);
	// Each frame covers the full height of clock region which contains many rows of tiles.
	// The library contains configuration bits only for the first row. So the word index has to
	// be offset to reach the correct row. The bit index remains same across rows.
	/// \todo Check for ordering mismatch between this and the library generation code.
	mCurrWordOffset = 0;
	if(bitIndexBegin != bitIndexEnd) {
		mCurrWordOffset = virtex7BitStream->getFrameLength() - (bitIndexEnd >> 5); // eFrameLength is number of words in a frame
//		mCurrWordOffset = bitIndexBegin >> 5;
	}
}

}
}
