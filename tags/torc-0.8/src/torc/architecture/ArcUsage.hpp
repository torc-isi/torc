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
/// \brief Header for the ArcUsage class.

#ifndef TORC_ARCHITECTURE_ARC_USAGE_HPP
#define TORC_ARCHITECTURE_ARC_USAGE_HPP

#include "torc/architecture/Array.hpp"
#include "torc/architecture/Arc.hpp"
#include "torc/architecture/Tiles.hpp"
#include "torc/architecture/Tilewire.hpp"
#include <boost/dynamic_bitset.hpp>

namespace torc {
namespace architecture {

	/// \brief Encapsulation the design arc usage.
	/// \details This class uses a compact bitset representation to very efficiently track the arc 
	///		usage of a design in an entire device.  Internal bitset objects are maintained on a 
	///		per-tile basis, and are not allocated until at least one arc in the tile has been 
	///		marked used.
	class ArcUsage {
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
	// functions
		/// \brief Returns the offset into the bitset for the specified arc.
		uint32_t getArcOffset(const Tilewire& inTilewire1, const Tilewire& inTilewire2) const	{
			TileIndex tile1 = inTilewire1.getTileIndex();
			WireIndex wire1 = inTilewire1.getWireIndex();
			WireIndex wire2 = inTilewire2.getWireIndex();
			// begin by figuring out the tile type
			const TileInfo& tileInfo = mTiles.getTileInfo(tile1);
			TileTypeIndex type = tileInfo.getTypeIndex();
			// next get the wire's base arc offset
			const WireInfo& wireInfo = mTiles.getWireInfo(type, wire1);
			uint32_t offset = wireInfo.getArcOffset();
			// look for a regular sink
			const WireArray& sinks = wireInfo.getSinks();
			for(WireIndex i; i < sinks.getSize(); i++) {
				if(sinks[i] == wire2) return offset;
				offset++;
			}
			// look for an irregular sink
			const WireArray& irregularSinks = wireInfo.getIrregularSinks();
			for(WireIndex i; i < irregularSinks.getSize(); i++) {
				if(irregularSinks[i] == wire2) return offset;
				offset++;
			}
			// if we didn't find the sink in the regular or irregular arcs, the call failed
			/// \todo Throw a meaningful exception.
			throw -1;
		}
	public:
	// constructors
		/// \brief Public constructor.
		ArcUsage(const Tiles& inTiles) : mTiles(inTiles), mTileUsageCount(), mBitsets(0), 
			mBitCount(0), mTileDirty(0) {}
		/// \brief Non-virtual destructor.
		~ArcUsage(void) {
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
		/// \brief Marks the specified arc as being used.
		inline void use(const Arc& inArc) 
			{ use(inArc.getSourceTilewire(), inArc.getSinkTilewire()); }
		/// \brief Marks the specified arc as being used.
		void use(const Tilewire& inTilewire1, const Tilewire& inTilewire2) {
			// extract the tile indexes
			TileIndex tileIndex1 = inTilewire1.getTileIndex();
			TileIndex tileIndex2 = inTilewire2.getTileIndex();
			// ensure that these tilewires belong to the same tile
			/// \todo Throw a meaningful exception.
			if(tileIndex1 != tileIndex2) throw -1;

			// make sure we have a bitset for this tile
			dynamic_bitset* bitset = mBitsets[tileIndex1];
			if(bitset == 0) {
				// determine how many arcs are in this tile
				const TileInfo& tileInfo = mTiles.getTileInfo(tileIndex1);
				TileTypeIndex type = tileInfo.getTypeIndex();
				const Array<const WireInfo>& wires = mTiles.getWireInfo(type);
				if(wires.getSize() == 0) return;
				const WireInfo& wireInfo = mTiles.getWireInfo(type, WireIndex(wires.getSize() - 1));
				// caution: we have to add the regular and irregular sink count from the last wire
				size_t size = wireInfo.getArcOffset() + wireInfo.getSinks().getSize() 
					+ wireInfo.getIrregularSinks().getSize();
				bitset = mBitsets[tileIndex1] = new dynamic_bitset(size);
				// track the statistics
				mTileUsageCount++;
				mBitCount += size;
			}

			// set the bit and mark the tile dirty
			bitset->set(getArcOffset(inTilewire1, inTilewire2));
			mTileDirty.set(tileIndex1, true);
		}
		/// \brief Marks the specified arc as being unused.
		inline void release(const Arc& inArc) 
			{ release(inArc.getSourceTilewire(), inArc.getSinkTilewire()); }
		/// \brief Marks the specified arc as being unused.
		void release(const Tilewire& inTilewire1, const Tilewire& inTilewire2) {
			// extract the tile indexes
			TileIndex tileIndex1 = inTilewire1.getTileIndex();
			TileIndex tileIndex2 = inTilewire2.getTileIndex();
			// ensure that these tilewires belong to the same tile
			/// \todo Throw a meaningful exception.
			if(tileIndex1 != tileIndex2) throw -1;

			// if there is no entry for the tile, the arc is already implicitly unused.
			dynamic_bitset* bitset = mBitsets[tileIndex1];
			if(bitset == 0) return;

			// otherwise clear the bit and mark the tile dirty
			bitset->set(getArcOffset(inTilewire1, inTilewire2), false);
			mTileDirty.set(tileIndex1, true);
		}
		/// \brief Marks all arcs as being unused, without releasing the bitset objects.
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
		/// \brief Determines whether the specified arc is in use.
		inline bool isUsed(const Arc& inArc) 
			{ return isUsed(inArc.getSourceTilewire(), inArc.getSinkTilewire()); }
		/// \brief Determines whether the specified arc is in use.
		bool isUsed(const Tilewire& inTilewire1, const Tilewire& inTilewire2) const {
			// extract the tile indexes
			TileIndex tileIndex1 = inTilewire1.getTileIndex();
			TileIndex tileIndex2 = inTilewire2.getTileIndex();
			// ensure that these tilewires belong to the same tile
			/// \todo Throw a meaningful exception.
			if(tileIndex1 != tileIndex2) throw -1;

			// if there is no entry for the tile, the tilewire is already implicitly unused.
			dynamic_bitset* bitset = mBitsets[tileIndex1];
			if(bitset == 0) return false;

			// otherwise, interrogate the bit
			return bitset->test(getArcOffset(inTilewire1, inTilewire2));
		}
		/// \brief Returns the number of arcs in use.
		uint32_t getArcUsageCount(void) const {
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

#endif // TORC_ARCHITECTURE_ARC_USAGE_HPP
