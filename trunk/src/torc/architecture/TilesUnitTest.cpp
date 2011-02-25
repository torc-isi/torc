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
/// \brief Source for the Tiles unit test.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/Tiles.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/common/Devices.hpp"

namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

// prototype for the worker function
void testDeviceTiles(DDB& inDDB);

/// \brief Unit test for the Tiles class.
BOOST_AUTO_TEST_CASE(architecture_tiles) {

	// iterate over the devices
	const torc::common::DeviceVector& devices = torc::common::Devices::getAllDevices();
	torc::common::DeviceVector::const_iterator dp = devices.begin();
	torc::common::DeviceVector::const_iterator de = devices.end();
	while(dp < de) {
		const std::string& device = *dp++;
		if(device.empty()) break;
		DDB ddb(device);
		testDeviceTiles(ddb);
	}

}

void testDeviceTiles(DDB& inDDB) {
	// functions tested during database initialization and deletion:
	//		Tiles(void);
	//		~Tiles(void);
	//		size_t readTileTypes(DigestStream& inStream);
	//		size_t readTileWireInfo(DigestStream& inStream);
	//		size_t readTileMap(DigestStream& inStream);
	const Tiles& tiles = inDDB.getTiles();

	// functions tested:
	//		TileTypeCount getTileTypeCount(void) const;
	// members tested:
	//		TileTypeCount mTileTypeCount;
	xilinx::TileTypeCount tileTypeCount = tiles.getTileTypeCount();
	if(tileTypeCount == xilinx::TileTypeCount(0)) // to reduce output
		BOOST_CHECK(tileTypeCount != 0);

	// functions tested:
	//		const char* getTileTypeName(TileTypeIndex inTileTypeIndex) const;
	// members tested:
	//		Array<const CharStringWrapper> mTileTypeNames;
	for(xilinx::TileTypeIndex tileTypeIndex; tileTypeIndex < tileTypeCount; tileTypeIndex++) {
		const char* tileTypeName = tiles.getTileTypeName(tileTypeIndex);
		if(*tileTypeName == 0) // to reduce output
			BOOST_CHECK(*tileTypeName != 0);
	}

	// functions tested:
	//		TileCount getTileCount(void) const;
	//		TileRow getRowCount(void) const;
	//		TileCol getColCount(void) const;
	// members tested:
	//		TileCount mTileCount;
	//		TileRow mRowCount;
	//		TileCol mColCount;
	xilinx::TileCount tileCount = tiles.getTileCount();
	xilinx::TileRow rowCount = tiles.getRowCount();
	xilinx::TileCol colCount = tiles.getColCount();
	if(tileCount == xilinx::TileCount(0)) // to reduce output
		BOOST_CHECK(tileCount != 0);
	if(rowCount == xilinx::TileRow(0)) // to reduce output
		BOOST_CHECK(rowCount != 0);
	if(colCount == xilinx::TileCol(0)) // to reduce output
		BOOST_CHECK(colCount != 0);
	if(static_cast<int>(tileCount) != rowCount * colCount) // to reduce output
		BOOST_CHECK_EQUAL(static_cast<int>(tileCount), rowCount * colCount);

	// functions tested:
	//		const TileInfo& getTileInfo(TileIndex inTileIndex) const;
	//		TileIndex getTileIndex(TileRow inRow, TileCol inCol) const;
	//		TileIndex findTileIndex(const string& inName) const;
	//		static bool CompareTilePairByName(const TileNameIndexPair& inA, 
	//			const TileNameIndexPair& inB);
	// members tested:
	//		TileIndex** mTileMap;
	//		Array<const TileInfo> mTiles;
	//		Array<const TileNameIndexPair> mOrderedTileNames;
	for(xilinx::TileIndex tileIndex; tileIndex < tileCount; tileIndex++) {
		// look up the tile info
		const TileInfo& tileInfo = tiles.getTileInfo(tileIndex);
		// verify that tile indexes and coordinates are consistent
		xilinx::TileRow tileRow = tileInfo.getRow();
		xilinx::TileCol tileCol = tileInfo.getCol();
		const char* tileName = tileInfo.getName();
		if(*tileName == 0) // to reduce output
			BOOST_CHECK(*tileName != 0);
		xilinx::TileIndex rowColTileIndex = tiles.getTileIndex(tileRow, tileCol);
		if(tileIndex != rowColTileIndex) // to reduce output
			BOOST_CHECK_EQUAL(tileIndex, rowColTileIndex);
		xilinx::TileIndex findTileIndex = tiles.findTileIndex(tileName);
		if(tileIndex != findTileIndex) // to reduce output
			BOOST_CHECK_EQUAL(tileIndex, findTileIndex);
	}

	// functions tested:
	//		const Array<const WireInfo>& getWireInfo(TileTypeIndex inTileTypeIndex) const;
	//		const WireInfo& getWireInfo(TileTypeIndex inTileTypeIndex, WireIndex inWireIndex) const;
	//		static bool CompareWirePairByName(const WireNameIndexPair& inA, 
	//			const WireNameIndexPair& inB);
	//		WireIndex findWireIndex(TileTypeIndex inTileTypeIndex, const string& inName) const;
	// members tested:
	//		Array2D<const WireInfo> mWires;
	//		Array2D<const WireNameIndexPair> mOrderedWireNames;
	for(xilinx::TileTypeIndex tileTypeIndex; tileTypeIndex < tileTypeCount; tileTypeIndex++) {
	// verify that wire indexes are consistent
		xilinx::WireCount wireCount = tiles.getWireCount(tileTypeIndex);
		const Array<const WireInfo>& wireInfoArray = tiles.getWireInfo(tileTypeIndex);
		for(xilinx::WireIndex wireIndex; wireIndex < wireCount; wireIndex++) {
			const WireInfo& wireInfo = tiles.getWireInfo(tileTypeIndex, wireIndex);
			if(&wireInfo != &wireInfoArray[wireIndex]) // to reduce output
				BOOST_CHECK(&wireInfo == &wireInfoArray[wireIndex]);
			const char* wireName = wireInfo.getName();
			if(*wireName == 0) // to reduce output
				BOOST_CHECK(*wireName != 0);
			xilinx::WireIndex findWireIndex = tiles.findWireIndex(tileTypeIndex, wireName);
			if(wireIndex != findWireIndex) // to reduce output
				BOOST_CHECK_EQUAL(wireIndex, findWireIndex);
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
