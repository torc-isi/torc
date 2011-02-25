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
/// \brief Source for the Tiles class.

#include "torc/architecture/Tiles.hpp"
#include <iostream>

namespace torc {
namespace architecture {

	size_t Tiles::readTileTypes(DigestStream& inStream) {
		// prepare to read from the stream
		size_t bytesReadOffset = inStream.getBytesRead();
		char scratch[1 << 10];			// scratch read buffer
		uint16_t nameLength = 0;		// length of tile type name

		// read the section header
		string sectionName;
		inStream.readSectionHeader(sectionName);
		/// \todo Throw a proper exception.
		if(sectionName != ">>>>TileType>>>>") throw -1;

		// initialize the tile type array
		inStream.read(mTileTypeCount);
		mTileTypeNames.setSize(mTileTypeCount);
		std::cout << "\tReading " << mTileTypeCount << " tile types..." << std::endl;
		// loop through each tile type
		for(TileTypeCount i; i < mTileTypeCount; i++) {
			// read the tile type name
			inStream.read(nameLength);
			/// \todo Throw a proper exception.
			if(nameLength > sizeof(scratch)) throw -1;
			inStream.read(scratch, nameLength);
			scratch[nameLength] = 0;
			const_cast<CharStringWrapper&>(mTileTypeNames[i]) = scratch;
			//std::cerr << "\t\t" << mTileTypeNames[i] << ": " 
			//	<< (void*) (const char*) mTileTypeNames[i] << std::endl;
		}

		// return the number of bytes read
		return inStream.getBytesRead() - bytesReadOffset;
	}

	size_t Tiles::readTileWireInfo(DigestStream& inStream) {
		// prepare to read from the stream
		size_t bytesReadOffset = inStream.getBytesRead();
		char scratch[1 << 10];			// scratch read buffer
		uint16_t nameLength = 0;		// length of tile type name
		TileTypeCount typeCount;		// number of tile types
		WireCount wireCount;			// number of wires
		WireCount sinkCount;			// number of sinks or tied sinks
		WireIndex sink;					// wire index of sink or tied sink
		uint16_t offset = 0;			// offset into the arc usage bitmap

		// read the section header
		string sectionName;
		inStream.readSectionHeader(sectionName);
		/// \todo Throw a proper exception.
		if(sectionName != ">>>>TileNode>>>>") throw -1;

		// initialize the tile type array
		inStream.read(typeCount);
		/// \todo Throw a proper exception.
		if(typeCount != mTileTypeCount) throw -1;
		mWires.setSize(mTileTypeCount);
		mOrderedWireNames.setSize(mTileTypeCount);
		std::cout << "\tReading wire info for " << mTileTypeCount << " tile types..." 
			<< std::endl;
		// loop through each tile type
		for(TileTypeCount i; i < mTileTypeCount; i++) {
			//std::cerr << "\t\t" << mTileTypeNames[i] << ":" << std::endl;
			offset = 0;
			// read the wire count
			inStream.read(wireCount);
			mWires[i].setSize(wireCount);
			mOrderedWireNames[i].setSize(wireCount);
			// get a reference to this tile type's wire info array
			Array<const WireInfo>& wires = mWires[i];
			// get a reference to this tile type's wire name array
			Array<const WireNameIndexPair>& wireNameIndexPairs = mOrderedWireNames[i];
			// prepare to track the source and tied source counts for each wire in the tile type
			Array<uint16_t> sourceCounts(wireCount);
			Array<uint16_t> tiedSourceCounts(wireCount);
			Array<uint16_t> irregularSourceCounts(wireCount);
			Array<uint16_t> routethroughSourceCounts(wireCount);
			for(WireCount j; j < wireCount; j++) { sourceCounts[j] = tiedSourceCounts[j] 
				= irregularSourceCounts[j] = routethroughSourceCounts[j] = 0; }
			// loop over all of the wires
			for(WireIndex j; j < wireCount; j++) {
				// look up a reference for the wire info, and discard the const trait
				WireInfo& wireInfo = const_cast<WireInfo&>(wires[j]);
				// store the cumulative arc offset for this tile type
				wireInfo.mArcOffset = offset;
				// read the flags
				inStream.read(wireInfo.mFlags);
				// read the wire name
				inStream.read(nameLength);
				/// \todo Throw a proper exception.
				if(nameLength > sizeof(scratch)) throw -1;
				inStream.read(scratch, nameLength);
				scratch[nameLength] = 0;
				wireInfo.mName = strdup(scratch);
				// also add the entry to the ordered wire names array
				const_cast<WireNameIndexPair&>(wireNameIndexPairs[j]) 
					= WireNameIndexPair(wireInfo.mName, WireIndex(j));
				// read the tied sinks, and update the tied source count for each one
				inStream.read(sinkCount);
				wireInfo.mTiedSinks.setSize(sinkCount);
				for(WireIndex k; k < sinkCount; k++) {
					inStream.read(sink);
					const_cast<WireIndex&>(wireInfo.mTiedSinks[k]) = sink;
					tiedSourceCounts[sink]++;
				}
				// read the regular sinks, and update the source count for each one
				inStream.read(sinkCount);
				wireInfo.mSinks.setSize(sinkCount);
				for(WireIndex k; k < sinkCount; k++) {
					offset++; // increment the arc count
					inStream.read(sink);
					const_cast<WireIndex&>(wireInfo.mSinks[k]) = sink;
					sourceCounts[sink]++;
					//// take care to allow for negative numbers in remote arcs
					//sourceCounts[sink >= 0 ? sink : -1 - sink]++;
				}
				// read the irregular sinks, and update the irregular source count for each one
				inStream.read(sinkCount);
				wireInfo.mIrregularSinks.setSize(sinkCount);
				for(WireIndex k; k < sinkCount; k++) {
					offset++; // increment the arc count
					inStream.read(sink);
					const_cast<WireIndex&>(wireInfo.mIrregularSinks[k]) = sink;
					irregularSourceCounts[sink]++;
				}
				// read the routethrough sinks, and update the routethrough source count for each 
				// one
				inStream.read(sinkCount);
				wireInfo.mRoutethroughSinks.setSize(sinkCount);
				for(WireIndex k; k < sinkCount; k++) {
					offset++; // increment the arc count
					inStream.read(sink);
					const_cast<WireIndex&>(wireInfo.mRoutethroughSinks[k]) = sink;
					routethroughSourceCounts[sink]++;
				}
			}

			// initialize the source arrays
			for(WireIndex j; j < wireCount; j++) {
				// look up a reference for the wire info, and discard the const trait
				WireInfo& wireInfo = const_cast<WireInfo&>(wires[j]);
				wireInfo.mSources.setSize(sourceCounts[j]);
				wireInfo.mTiedSources.setSize(tiedSourceCounts[j]);
				wireInfo.mIrregularSources.setSize(irregularSourceCounts[j]);
				wireInfo.mRoutethroughSources.setSize(routethroughSourceCounts[j]);
			}

			// and infer the source information from the sinks
			for(WireIndex j; j < wireCount; j++) {
				// look up a reference for the wire info, and discard the const trait
				WireInfo& wireInfo = const_cast<WireInfo&>(wires[j]);
				// handle the tied sources
				const WireArray& tiedSinks = wireInfo.getTiedSinks();
				sinkCount = WireIndex(tiedSinks.getSize());
				for(WireIndex k; k < sinkCount; k++) {
					const WireIndex tiedSink = tiedSinks[k];
					tiedSourceCounts[tiedSink]--;
					const_cast<WireIndex&>(wires[tiedSink]
						.mTiedSources[tiedSourceCounts[tiedSink]]) = WireIndex(j);
				}
				// handle the normal sources
				const WireArray& sinks = wireInfo.getSinks();
				sinkCount = WireIndex(sinks.getSize());
				for(WireIndex k; k < sinkCount; k++) {
					const WireIndex sink = sinks[k];
					sourceCounts[sink]--;
					const_cast<WireIndex&>(wires[sink]
						.mSources[sourceCounts[sink]]) = WireIndex(j);
				}
				// handle the irregular sources
				const WireArray& irregularSinks = wireInfo.getIrregularSinks();
				sinkCount = WireIndex(irregularSinks.getSize());
				for(WireIndex k; k < sinkCount; k++) {
					const WireIndex irregularSink = irregularSinks[k];
					irregularSourceCounts[irregularSink]--;
					const_cast<WireIndex&>(wires[irregularSink]
						.mIrregularSources[irregularSourceCounts[irregularSink]]) = WireIndex(j);
				}
				// handle the routethrough sources
				const WireArray& routethroughSinks = wireInfo.getRoutethroughSinks();
				sinkCount = WireIndex(routethroughSinks.getSize());
				for(WireIndex k; k < sinkCount; k++) {
					const WireIndex routethroughSink = routethroughSinks[k];
					routethroughSourceCounts[routethroughSink]--;
					const_cast<WireIndex&>(wires[routethroughSink]
						.mRoutethroughSources[routethroughSourceCounts[routethroughSink]]) 
						= WireIndex(j);
				}
			}

			// sort the ordered wire names
			std::sort(
				const_cast<Array<WireNameIndexPair>::iterator>(wireNameIndexPairs.begin()), 
				const_cast<Array<WireNameIndexPair>::iterator>(wireNameIndexPairs.end()), 
				&CompareWirePairByName
			);

		}

		// return the number of bytes read
		return inStream.getBytesRead() - bytesReadOffset;
	}

	size_t Tiles::readTileMap(DigestStream& inStream) {
		// prepare to read from the stream
		size_t bytesReadOffset = inStream.getBytesRead();
		char scratch[1 << 10];			// scratch read buffer
		uint16_t nameLength = 0;		// length of tile type name
		TileTypeIndex type;				// tile type
		TileRow row;					// tile row
		TileCol col;					// tile column

		// read the section header
		string sectionName;
		inStream.readSectionHeader(sectionName);
		/// \todo Throw a proper exception.
		if(sectionName != ">>>>TileMap >>>>") throw -1;

		// read the characteristic dimensions
		inStream.read(mTileCount);
		inStream.read(mRowCount);
		inStream.read(mColCount);

		// initialize the tile map array
		mTiles.setSize(mTileCount);
		mOrderedTileNames.setSize(mTileCount);
		mTileMap = new TileIndex*[static_cast<TileRow::pod>(mRowCount)];
		mTileMap[0] = new TileIndex[static_cast<TileCount::pod>(mTileCount)];
		for(TileRow i(1); i < mRowCount; i++) mTileMap[i] = mTileMap[i-1] + mColCount;
		std::cout << "\tReading tile map for " << mTileCount << " tiles (" << mRowCount 
			<< " rows x " << mColCount << " columns)..." << std::endl;
		// loop through each tile
		for(TileIndex i; i < mTileCount; i++) {
			// read the info for each tile
			inStream.read(type);
			inStream.read(row);
			inStream.read(col);
			inStream.read(nameLength);
			/// \todo Throw a proper exception.
			if(nameLength > sizeof(scratch)) throw -1;
			inStream.read(scratch, nameLength);
			scratch[nameLength] = 0;
			// and store the info
			const_cast<TileInfo&>(mTiles[i]).set(type, row, col, scratch);
			mTileMap[row][col] = i; // point back to the tile index
			//std::cerr << "\t\t" << i << ": [" << row << "," << col << "]: \"" << scratch 
			//	<< "\" (" << mTileTypeNames[type] << ")" << std::endl;
			// also add the entry to the ordered tile names array
			const_cast<TileNameIndexPair&>(mOrderedTileNames[i]) 
				= TileNameIndexPair(mTiles[i].getName(), i);
		}

		// sort the ordered tile names
		std::sort(const_cast<Array<TileNameIndexPair>::iterator>(mOrderedTileNames.begin()), 
			const_cast<Array<TileNameIndexPair>::iterator>(mOrderedTileNames.end()), 
			&CompareTilePairByName
		);

		// return the number of bytes read
		return inStream.getBytesRead() - bytesReadOffset;
	}

} // namespace architecture
} // namespace torc
