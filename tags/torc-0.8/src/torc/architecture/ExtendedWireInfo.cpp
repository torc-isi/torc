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
/// \brief Source for the ExtendedWireInfo class.

#include "torc/architecture/ExtendedWireInfo.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/TileInfo.hpp"
#include "torc/architecture/WireInfo.hpp"

namespace torc {
namespace architecture {

	void ExtendedWireInfo::set(const Tilewire& inTilewire) {
		// be sure to handle invalid tilewires appropriately
		if(inTilewire.isInvalid()) {
			mWireIndex = WireIndex(-1);
			mTileIndex = TileIndex(-1);
			mTileTypeIndex = TileTypeIndex(-1);
			mTileRow = TileRow(-1);
			mTileCol = TileCol(-1);
			mTileName = "";
			mTileTypeName = "";
			mWireName = "";
			mWireFlags = WireFlags();
			return;
		}
		// extract the tile and wire indexes
		mWireIndex = inTilewire.getWireIndex();
		mTileIndex = inTilewire.getTileIndex();
		const Tiles& tiles = mDDB.getTiles();
		// get information about the tile itself
		const TileInfo& tileInfo = tiles.getTileInfo(mTileIndex);
		mTileTypeIndex = tileInfo.getTypeIndex();
		mTileRow = tileInfo.getRow();
		mTileCol = tileInfo.getCol();
//mTileName = "";
		mTileName = tileInfo.getName();
		// get information about the tile type
		mTileTypeName = mDDB.getTiles().getTileTypeName(mTileTypeIndex);
		// get information about the wire
		const WireInfo& wireInfo = tiles.getWireInfo(mTileTypeIndex, mWireIndex);
		mWireName = wireInfo.getName();
		mWireFlags = wireInfo.getFlags();
	}

} // namespace architecture
} // namespace torc
