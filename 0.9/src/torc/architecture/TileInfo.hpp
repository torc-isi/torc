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
/// \brief Header for the TileInfo class.

#ifndef TORC_ARCHITECTURE_TILE_INFO_HPP
#define TORC_ARCHITECTURE_TILE_INFO_HPP

#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include <cstring>
#include <cstdlib>

namespace torc {
namespace architecture {

	/// \brief Encapsulation of a tile within a device tile map.
	class TileInfo {
	// friends
		/// \brief The Tiles class has access to our protected members.
		friend class Tiles;
	protected:
	// types
		typedef boost::uint16_t uint16_t;				///< \brief Imported type name.
		typedef xilinx::TileTypeIndex TileTypeIndex;	///< \brief Imported type name.
		typedef xilinx::TileRow TileRow;				///< \brief Imported type name.
		typedef xilinx::TileCol TileCol;				///< \brief Imported type name.
	// members
		/// \brief The tile type index for this tile.
		TileTypeIndex mTypeIndex;
		/// \brief The row for this tile.
		TileRow mRow;
		/// \brief The column for this tile.
		TileCol mCol;
		/// \brief The name for this tile.
		const char* mName;
	// initializer
		/// \brief Protected initializer intended for use by the Tiles class.
		/// \param inTypeIndex The tile type index.
		/// \param inRow The tile row.
		/// \param inCol The tile column.
		/// \param inName The tile name.
		void set(const TileTypeIndex& inTypeIndex, const TileRow& inRow, const TileCol& inCol, 
			const char* inName)  {
			// release the current name if applicable
			if(mName != 0) { free(const_cast<char*>(mName)); mName = 0; }
			// make a private copy of the tile name
			mName = strdup(inName);
			// copy the remaining fields
			mTypeIndex = inTypeIndex;
			mRow = inRow;
			mCol = inCol;
		}
	// constructors
		/// \brief Disabled copy constructor.
		TileInfo(const TileInfo& /*rhs*/) : mTypeIndex(), mRow(), mCol(), mName(0) {}
	public:
	// constructors
		/// \brief Null constructor.
		TileInfo(void) : mTypeIndex(), mRow(), mCol(), mName(0) {}
		/// \brief Non-virtual destructor.
		~TileInfo(void) {
			// release the tile name if allocated
			if(mName != 0) { free(const_cast<char*>(mName)); mName = 0; }
		}
	// accessors
		/// \brief Returns the tile type index for this tile.
		const TileTypeIndex& getTypeIndex(void) const { return mTypeIndex; }
		/// \brief Returns the row for this tile.
		const TileRow& getRow(void) const { return mRow; }
		/// \brief Returns the column for this tile.
		const TileCol& getCol(void) const { return mCol; }
		/// \brief Returns the name for this tile.
		const char* getName(void) const { return mName; }
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_TILE_INFO_HPP
