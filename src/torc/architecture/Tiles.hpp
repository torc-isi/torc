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
/// \brief Header for the Tiles class.

#ifndef TORC_ARCHITECTURE_TILES_HPP
#define TORC_ARCHITECTURE_TILES_HPP

#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/architecture/DigestStream.hpp"
#include "torc/architecture/Array.hpp"
#include "torc/architecture/WireInfo.hpp"
#include "torc/architecture/TileInfo.hpp"

namespace torc {
namespace architecture {

	/// \brief Tile map, tile type, and wire information for the family and device.
	/// \details The tile map defines the tile layout for the current device.  Every tile is 
	///		associated with a tile type that has been defined for the family.  The wire type 
	///		information is likewise device independent, and is therefore included in this class.
	class Tiles {
	// friends
		/// \brief The database has access to our protected functions.
		friend class DDB;
	protected:
	// types
		typedef std::string string;						///< \brief Imported type name.
		typedef boost::uint16_t uint16_t;				///< \brief Imported type name.
		typedef boost::uint32_t uint32_t;				///< \brief Imported type name.
		typedef xilinx::TileRow TileRow;				///< \brief Imported type name.
		typedef xilinx::TileCol TileCol;				///< \brief Imported type name.
		typedef xilinx::TileCount TileCount;			///< \brief Imported type name.
		typedef xilinx::TileIndex TileIndex;			///< \brief Imported type name.
		typedef xilinx::TileTypeCount TileTypeCount;	///< \brief Imported type name.
		typedef xilinx::TileTypeIndex TileTypeIndex;	///< \brief Imported type name.
		typedef xilinx::WireCount WireCount;			///< \brief Imported type name.
		typedef xilinx::WireIndex WireIndex;			///< \brief Imported type name.
		/// \brief Wrapper for a tile name with corresponding tile index.
		typedef std::pair<std::string, xilinx::TileIndex> TileNameIndexPair;
		/// \brief Wrapper for a wire name with corresponding wire index.
		typedef std::pair<std::string, xilinx::WireIndex> WireNameIndexPair;
	// nested classes
		/// \brief Wrapper around char* for use with the Array template.
		class CharStringWrapper {
		// members
			/// \brief The char* string.
			char* mString;
		public:
		// constructors
			/// \brief Null constructor.
			CharStringWrapper(void) : mString(0) {}
			/// \brief Public constructor.
			CharStringWrapper(const char* inString) : mString(strdup(inString)) {}
			/// \brief Non-virtual destructor.
			~CharStringWrapper(void) { if(mString != 0) { free(mString); mString = 0; } }
		// accessors
			/// \brief Cast operator.
			operator const char*(void) const { return mString; }
		// operators
			/// \brief Assignment operator.
			CharStringWrapper& operator= (const char* inString) {
				if(mString != 0) free(mString);
				mString = strdup(inString);
				return *this;
			}
		};
	// members
		/// \brief The tile count for this device.
		TileCount mTileCount;
		/// \brief The tile row count for this device.
		TileRow mRowCount;
		/// \brief The tile column count for this device.
		TileCol mColCount;
		/// \brief The two-dimensional tile map for this device.
		TileIndex** mTileMap;
		/// \brief The tile information for this device.
		Array<const TileInfo> mTiles;
		/// \brief The tile name to tile index mapping for this device.
		Array<const TileNameIndexPair> mOrderedTileNames;
		/// \brief The number of tile types for this family.
		TileTypeCount mTileTypeCount;
		/// \brief The tile type names for this family.
		Array<const CharStringWrapper> mTileTypeNames;
		/// \brief The wire information for this family.
		Array2D<const WireInfo> mWires;
		/// \brief The wire name to wire index mapping for each tile type for this family.
		Array2D<const WireNameIndexPair> mOrderedWireNames;
	// functions
		/// \brief Read the family tile types.
		size_t readTileTypes(DigestStream& inStream);
		/// \brief Read the family wire info.
		size_t readTileWireInfo(DigestStream& inStream);
		/// \brief Read the device tile map.
		size_t readTileMap(DigestStream& inStream);
	// static
		/// \brief Compare tile pairs by name, for ordering purposes.
		static bool CompareTilePairByName(const TileNameIndexPair& inA, 
			const TileNameIndexPair& inB) {
			return inA.first < inB.first;
		}
		/// \brief Compare wire pairs by name, for ordering purposes.
		static bool CompareWirePairByName(const WireNameIndexPair& inA, 
			const WireNameIndexPair& inB) {
			return inA.first < inB.first;
		}
	// constructors
		/// \brief Protected constructor.
		Tiles(void) : mTileCount(), mRowCount(), mColCount(), mTileMap(0), mTiles(), 
			mTileTypeCount(), mTileTypeNames(), mWires() {}
	public:
	// constructors
		/// \brief Non-virtual destructor.
		~Tiles(void) {
			// release the tile map memory; the other objects will go away by themselves
			if(mTileMap != 0) {
				if(mTileMap[0] != 0) { delete[] mTileMap[0]; mTileMap[0] = 0; }
				delete[] mTileMap; mTileMap = 0;
			}
		}
	// accessors
		/// \brief Returns the TileInfo object for the specified tile.
		const TileInfo& getTileInfo(TileIndex inTileIndex) const { 
			return mTiles[inTileIndex]; }
		/// \brief Returns the WireInfo array for the specified tile type.
		const Array<const WireInfo>& getWireInfo(TileTypeIndex inTileTypeIndex) const 
			{ return mWires[inTileTypeIndex]; }
		/// \brief Returns the WireInfo object for the specified tile type and wire index.
		const WireInfo& getWireInfo(TileTypeIndex inTileTypeIndex, WireIndex inWireIndex) const 
			{ return mWires[inTileTypeIndex][inWireIndex]; }
		/// \brief Returns the tile count for this device.
		TileCount getTileCount(void) const { return mTileCount; }
		/// \brief Returns the tile type count for this device.
		TileTypeCount getTileTypeCount(void) const { return mTileTypeCount; }
		/// \brief Returns the row count for this device.
		TileRow getRowCount(void) const { return mRowCount; }
		/// \brief Returns the column count for this device.
		TileCol getColCount(void) const { return mColCount; }
		/// \brief Returns the wire count for the specified tile type.
		WireCount getWireCount(TileTypeIndex inTileTypeIndex) const 
			{ return WireCount(mWires[inTileTypeIndex].getSize()); }
	// functions
		/// \brief Returns the tile index for the given [row,column] pair.
		TileIndex getTileIndex(TileRow inRow, TileCol inCol) const 
			{ return mTileMap[inRow][inCol]; }
		/// \brief Returns the tile type name for the given tile type index.
		const char* getTileTypeName(TileTypeIndex inTileTypeIndex) const 
			{ return mTileTypeNames[inTileTypeIndex]; }
		/// \brief Returns the tile index for the given tile name.
		/// \details The search has logarithmic complexity.
		/// \returns TileIndex(-1) if the name does not exist, or the corresponding tile index 
		///		otherwise.
		TileIndex findTileIndex(const string& inName) const {
			Array<const TileNameIndexPair>::const_iterator b = mOrderedTileNames.begin();
			Array<const TileNameIndexPair>::const_iterator e = mOrderedTileNames.end();
			TileNameIndexPair value(inName, TileIndex(-1));
			Array<const TileNameIndexPair>::const_iterator p = lower_bound(b, e, value, 
				&CompareTilePairByName);
			return TileIndex((p == e || p->first != inName) ? -1 : p->second);
		}
		/// \brief Returns the wire index for the given wire name in the given tile type.
		/// \details The search has logarithmic complexity.
		/// \returns WireIndex(-1) if the name does not exist, or the corresponding wire index 
		///		otherwise.
		WireIndex findWireIndex(TileTypeIndex inTileTypeIndex, const string& inName) const {
			const Array<const WireNameIndexPair>& wireNameIndexPairArray 
				= mOrderedWireNames[inTileTypeIndex];
			Array<const WireNameIndexPair>::const_iterator b = wireNameIndexPairArray.begin();
			Array<const WireNameIndexPair>::const_iterator e = wireNameIndexPairArray.end();
			WireNameIndexPair value(inName, WireIndex(-1));
			Array<const WireNameIndexPair>::const_iterator p = lower_bound(b, e, value, 
				&CompareWirePairByName);
			return WireIndex((p == e || p->first != inName) ? -1 : p->second);
		}
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_TILES_HPP
