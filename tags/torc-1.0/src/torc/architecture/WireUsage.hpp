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
/// \brief Header for the WireUsage class.

#ifndef TORC_ARCHITECTURE_WIREUSAGE_HPP
#define TORC_ARCHITECTURE_WIREUSAGE_HPP

#include "torc/architecture/Array.hpp"
#include "torc/architecture/Tiles.hpp"
#include "torc/architecture/Tilewire.hpp"
#include <boost/dynamic_bitset.hpp>

namespace torc {
namespace architecture {

	/// \brief Encapsulation the design wire usage.
	/// \details This class uses a compact bitset representation to very efficiently track the wire 
	///		usage of a design in an entire device.  Internal bitset objects are maintained on a 
	///		per-tile basis, and are not allocated until at least one wire in the tile has been 
	///		marked used.
	class WireUsage {
	protected:
	// types
		typedef boost::dynamic_bitset<> dynamic_bitset;		///< \brief Imported type name.
		typedef xilinx::TileIndex TileIndex;				///< \brief Imported type name.
		typedef xilinx::TileCount TileCount;				///< \brief Imported type name.
		typedef xilinx::TileTypeIndex TileTypeIndex;		///< \brief Imported type name.
		typedef xilinx::WireIndex WireIndex;				///< \brief Imported type name.
	// members
		/// \brief Reference to the database Tiles object.
		const Tiles& mTiles;
		/// \brief The number of tiles for which bitsets are allocated.
		TileCount mTileUsageCount;
		/// \brief The wire usage bitset array.
		Array<dynamic_bitset*> mBitsets;
		/// \brief The number of bits allocated by the usage bitsets.
		uint32_t mBitCount;
		/// \brief The mask of tile bitsets that contain changes.
		dynamic_bitset mTileDirty;
	public:
	// constructors
		/// \brief Public constructor.
		WireUsage(const Tiles& inTiles) : mTiles(inTiles), mTileUsageCount(), mBitsets(0), 
			mBitCount(0), mTileDirty(0) {}
		/// \brief Non-virtual destructor.
		~WireUsage(void) {
			size_t tileCount = mBitsets.getSize();
			for(TileIndex i; i < tileCount; i++) {
				if(mBitsets[i] != 0) { delete mBitsets[i]; mBitsets[i] = 0; }
			}
		}
	// functions
		/// \brief Size the wire usage according to the number of device tiles.
		void autosize(void) {
			// release any existing bitsets
			for(TileCount i; i < mBitsets.getSize(); i++) {
				if(mBitsets[i] != 0) { delete mBitsets[i]; mBitsets[i] = 0; }
				mTileDirty.reset(i);
			}
			// resize for the new dimensions
			TileCount tileCount = mTiles.getTileCount();
			mBitsets.setSize(tileCount);
			for(TileCount i; i < tileCount; i++) mBitsets[i] = 0;
			mTileDirty.resize(tileCount);
		}
		/// \brief Marks the specified tilewire as being used.
		void use(const Tilewire& inTilewire) {
			// extract the tile and wire indexes
			TileIndex tileIndex = inTilewire.getTileIndex();
			WireIndex wireIndex = inTilewire.getWireIndex();

			// look up the appropriate bitset, allocating it if necessary
			dynamic_bitset* bitset = mBitsets[tileIndex];
			if(bitset == 0) {
				// determine how many wires are in this tile
				const TileInfo& tileInfo = mTiles.getTileInfo(tileIndex);
				TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
				size_t size = mTiles.getWireInfo(tileTypeIndex).getSize();
				bitset = mBitsets[tileIndex] = new dynamic_bitset(size);
				// track the statistics
				mTileUsageCount++;
				mBitCount += size;
			}

			// set the bit and mark the tile dirty
			bitset->set(wireIndex, true);
			mTileDirty.set(tileIndex, true);
		}
		/// \brief Marks the specified tilewire as being unused.
		void release(const Tilewire& inTilewire) {
			// extract the tile and wire indexes
			TileIndex tileIndex = inTilewire.getTileIndex();
			WireIndex wireIndex = inTilewire.getWireIndex();

			// if there is no entry for the tile, the tilewire is already implicitly unused.
			dynamic_bitset* bitset = mBitsets[tileIndex];
			if(bitset == 0) return;

			// otherwise clear the bit and mark the tile dirty
			bitset->set(wireIndex, false);
			mTileDirty.set(tileIndex, true);
		}
		/// \brief Marks all wires as being unused, without releasing the bitset objects.
		/// \details This capability allows the tracer to track the wires that it has visited while 
		///		processing a particular net, and then to start again from scratch without incurring 
		///		allocation and construction overheads.
		void clear(void) {
			// iterate over all of the tiles
			size_t tileCount = mBitsets.getSize();
			for(TileIndex i; i < tileCount; i++) {
				// skip this tile if it isn't dirty
				if(!mTileDirty[i]) continue;
				// mark the tile clean
				mTileDirty.reset(i);
				// look up the bitset for this tile
				dynamic_bitset* bitset = mBitsets[i];
				// skip tiles without an associated bitset (should never happen for dirty tiles)
				if(bitset == 0) continue;
				// clear the entire bitset
				bitset->reset();
			}
		}
		/// \brief Determines whether the specified tilewire is in use.
		bool isUsed(const Tilewire& inTilewire) {
			// extract the tile and wire indexes
			TileIndex tileIndex = inTilewire.getTileIndex();
			WireIndex wireIndex = inTilewire.getWireIndex();

			// if there is no entry for the tile, the tilewire is already implicitly unused.
			dynamic_bitset* bitset = mBitsets[tileIndex];
			if(bitset == 0) return false;

			// otherwise, interrogate the bit
			return bitset->test(wireIndex);
		}
		/// \brief Returns the number of wires in use.
		uint32_t getWireUsageCount(void) const {
			uint32_t usageCount = 0;
			size_t tileCount = mBitsets.getSize();
			for(TileIndex i; i < tileCount; i++) {
				/// \todo Question: can we get away with only checking dirty bitsets?
				//// skip this tile if it isn't dirty
				//if(!mTileDirty[i]) continue;
				// look up the bitset for this tile
				dynamic_bitset* bitset = mBitsets[i];
				// skip tiles without an associated bitset
				if(bitset == 0) continue;
				// clear the entire bitset
				usageCount += bitset->count();
			}
			return usageCount;
		}
	// accessors
		/// \brief Returns the number of tiles that have been touched.
		TileCount getTileUsageCount(void) const { return mTileUsageCount; }
		/// \brief Returns the number of bits allocated.
		uint32_t getBitCount(void) const { return mBitCount; }
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_WIREUSAGE_HPP
