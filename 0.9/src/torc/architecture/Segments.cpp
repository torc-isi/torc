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
/// \brief Source for the Segments class.

#include "torc/architecture/Segments.hpp"
#include <iostream>

namespace torc {
namespace architecture {

	const Segments::SegmentReference Segments::SegmentReference::sTrivialSegmentReference;

	Segments::Segments(void) : mCompactSegments(), mTilewireSegments(), mIrregularArcs(), 
		mCompactSegmentCount(), mIrregularArcCount(), mTotalWireCount(), mTotalSegmentCount() {
		std::cerr << "WARNING: Need to check segment packing." << std::endl;
	}

	size_t Segments::readTilewireSegments(DigestStream& inStream) {
		// prepare to read from the stream
		size_t bytesReadOffset = inStream.getBytesRead();
		TileCount tileCount;			// number of tiles
		uint16_t tileWireCount = 0;		// number of wires in the current tile
		uint16_t extraWireCount = 0;	// number of extra wires in the current tile
		WireIndex wireIndex;			// index of extra wire

		//std::cerr << "NOTICE: Remove unnecessary clearing of mTilewireSegment wires." 
		//	<< std::endl;
		// read the section header
		string sectionName;
		inStream.readSectionHeader(sectionName);
		/// \todo Throw a proper exception.
		if(sectionName != ">>>>TileSegs>>>>") throw -1;

		// initialize the tile segment array
		inStream.read(tileCount);
		mTilewireSegments.setSize(tileCount);
		std::cout << "\tReading " << tileCount << " tiles..." << std::endl;
		// loop through each tile
		for(TileIndex i; i < tileCount; i++) {
			// initialize each tile sub-array
			inStream.read(tileWireCount);
			mTilewireSegments[i].setSize(tileWireCount);
			// the following clearing of SegementReference entris should no longer be necessary
			// for(WireIndex j; j < tileWireCount; j++) 
			//		mTilewireSegments[i][j] = SegmentReference();
			// read the extra wires that we can't infer from the segments
			inStream.read(extraWireCount);
			// std::cerr << i << "#" << tileWireCount << ":" << extraWireCount << "=" 
			//	<< mTotalWireCount << " ";
			mTotalWireCount += tileWireCount - extraWireCount;
			// std::cerr << "\t" << i << ": ";
			for(WireIndex j; j < extraWireCount; j++) {
				// read the wire index
				inStream.read(wireIndex);
				// the current database implicitly assumes that only -1 (non-existent) wires are 
				// encoded, so we don't need to read the segment index; that assumption would not 
				// hold if we had to handle remove wires.
				//// read the segment value
				//inStream.read(segmentIndex);
				// store the data
				mTilewireSegments[i][wireIndex].undefine();
				// std::cerr << wireIndex << " ";
			}
			// std::cerr << std::endl;
		}
		//std::cout << "\t" << mTotalWireCount << " total wires" << std::endl;

		// return the number of bytes read
		return inStream.getBytesRead() - bytesReadOffset;
	}

	size_t Segments::readSegments(DigestStream& inStream, bool inExtendedAnchorTileCount) {
		// prepare to read from the stream
		size_t bytesReadOffset = inStream.getBytesRead();
		uint16_t wireCount = 0;			// number of wires in the segment
		uint32_t offsetCount = 0;		// number of offset tiles which use the segment
		WireIndex wireIndex;			// current wire index
		TileOffset tileOffset;			// current tile offset
		TileIndex rootTileIndex;		// compact segment root tile index

		// read the section header
		string sectionName;
		inStream.readSectionHeader(sectionName);
		/// \todo Throw a proper exception.
		if(sectionName != ">>>>Segments>>>>") throw -1;

		// initialize the tile segment array
		inStream.read(mCompactSegmentCount);
		mCompactSegments.setSize(mCompactSegmentCount);
		std::cout << "\tReading " << mCompactSegmentCount << " segments..." << std::endl;
		// loop through each segment (except segment zero)
		for(CompactSegmentIndex i; i < mCompactSegmentCount; i++) {
			// initialize each segment sub-array
			inStream.read(wireCount);
			mCompactSegments[i].setSize(wireCount);
			// loop through each wire in the segment
			for(WireIndex j; j < wireCount; j++) {
				// read the wire and tile indexes
				inStream.read(wireIndex);
				inStream.read(tileOffset);
				// store the data
				mCompactSegments[i][j] = CompactSegmentTilewire(wireIndex, tileOffset);
			}
			// initialize each offset location
			if(inExtendedAnchorTileCount) {
				// read a 32-bit anchor tile count
				inStream.read(offsetCount);
			} else {
				// read the standard 16-bit anchor tile count
				uint16_t shortOffsetCount;
				inStream.read(shortOffsetCount);
				offsetCount = shortOffsetCount;
			}
			// loop through each offset for this segment
			for(uint32_t j = 0; j < offsetCount; j++) {
				mTotalSegmentCount++;
				// read the root tile
				inStream.read(rootTileIndex);
				// expand the data into each applicable tile
				for(WireIndex k; k < wireCount; k++) {
					const CompactSegmentTilewire compactSegmentTilewire = mCompactSegments[i][k];
					TileIndex tileIndex = TileIndex(rootTileIndex + compactSegmentTilewire.getTileOffset());
					WireIndex wireIndex = compactSegmentTilewire.getWireIndex();
const SegmentReference& existing = mTilewireSegments[tileIndex][wireIndex];
if(existing.getCompactSegmentIndex() != 0 || existing.getAnchorTileIndex() != 0) {
	std::cerr << "WARNING: Overwriting mTilewireSegments[" << tileIndex << "][" << wireIndex << "]: "
		<< "conflict is " << i << "@" << rootTileIndex << " versus " << existing.getCompactSegmentIndex()
		<< "@" << existing.getAnchorTileIndex() << std::endl;
}
					mTilewireSegments[tileIndex][wireIndex] = SegmentReference(i, rootTileIndex);
				}
			}
		}
		std::cout << "\t" << mTotalSegmentCount << " total segments" << std::endl;

		// return the number of bytes read
		return inStream.getBytesRead() - bytesReadOffset;
	}

	size_t Segments::readIrregularArcs(DigestStream& inStream) {
		// prepare to read from the stream
		size_t bytesReadOffset = inStream.getBytesRead();
		TileCount tileCount;			// number of tiles
		uint16_t arcCount = 0;			// number of irregular arcs in the tile
		WireIndex sourceWireIndex;		// arc source wire index
		WireIndex sinkWireIndex;		// arc sink wire index

		// read the section header
		string sectionName;
		inStream.readSectionHeader(sectionName);
		/// \todo Throw a proper exception.
		if(sectionName != ">>>>IrrgArcs>>>>") throw -1;

		// initialize the irregular arc array
		inStream.read(tileCount);
		mIrregularArcs.setSize(tileCount);
		std::cout << "\tReading irregular arcs for " << tileCount << " tiles..." << std::endl;
		// loop through each tile
		for(TileIndex i; i < tileCount; i++) {
			Array<IrregularArc>& irregularArcs = mIrregularArcs[i];
			// read the number of irregular arcs
			inStream.read(arcCount);
			irregularArcs.setSize(arcCount);
			// read the irregular arcs
			for(uint16_t j = 0; j < arcCount; j++) {
				// read the arc wire indexes
				inStream.read(sourceWireIndex);
				inStream.read(sinkWireIndex);
				// store the irregular arc
				irregularArcs[j] = IrregularArc(sourceWireIndex, sinkWireIndex);
			}
		}

		// return the number of bytes read
		return inStream.getBytesRead() - bytesReadOffset;
	}


	// it would be nice to convert this to a binary search
	const Segments::IrregularArc* Segments::getIrregularArc(TileIndex inTileIndex, 
		WireIndex inSourceWireIndex, WireIndex inSinkWireIndex) {
		// look up the tile's irregular arcs
		const Array<IrregularArc>& irregularArcs = mIrregularArcs[inTileIndex];
		// iterate through the arcs and look for a match
		Array<IrregularArc>::const_iterator p = irregularArcs.begin();
		Array<IrregularArc>::const_iterator e = irregularArcs.end();
		while(p < e) {
			// look for a matching source wire index
			WireIndex sourceWireIndex = p->getSourceWireIndex();
			if(sourceWireIndex < inSourceWireIndex) { p++; continue; }
			if(sourceWireIndex > inSourceWireIndex) return NULL;
			// look for a matching sink wire index
			WireIndex sinkWireIndex = p->getSinkWireIndex();
			if(sinkWireIndex < inSinkWireIndex) { p++; continue; }
			if(sinkWireIndex > inSinkWireIndex) return NULL;
			// if we got here, we found the requested arc
			return p;
		}
		// we didn't find the arc
		return 0;
	}

} // namespace architecture
} // namespace torc
