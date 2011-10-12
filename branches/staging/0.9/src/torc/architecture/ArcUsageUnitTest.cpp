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
/// \brief Unit test for the ArcUsage class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/ArcUsage.hpp"

namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

using namespace torc::architecture::xilinx;

/// \brief Unit test for the ArcUsage class.
BOOST_AUTO_TEST_CASE(ArcUsageUnitTest) {
	// supporting variables
	DDB ddb("xcv50");
	const Tiles& tiles = ddb.getTiles();
	Tilewire tilewire1 = ddb.lookupTilewire("R1C1", "S0_X");
	Tilewire tilewire2 = ddb.lookupTilewire("R1C1", "OUT0");
	Tilewire tilewire3(tilewire2.getTileIndex(), WireIndex(10000));
	Arc arc1(tilewire1, tilewire2);
	uint32_t virtexCenterArcCount = 1511u;

	// members tested:
	//		const Tiles& mTiles;
	//		Array<dynamic_bitset*> mBitsets;
	//		dynamic_bitset mTileDirty;
	// functions tested:
	//		ArcUsage(const Tiles& inTiles);
	//		void autosize(void);
	//		uint32_t getArcUsageCount(void) const;
	//		TileCount getTileUsageCount(void) const;
	//		uint32_t getBitCount(void) const;
	ArcUsage arcUsage(tiles);
	arcUsage.autosize();
	BOOST_CHECK(arcUsage.mBitsets.getSize() > 0);
	BOOST_CHECK(arcUsage.getArcUsageCount() == 0);
	BOOST_CHECK(arcUsage.getTileUsageCount() == TileCount(0));
	BOOST_CHECK(arcUsage.getBitCount() == 0);

	// members tested:
	//		TileCount mTileUsageCount;
	//		uint32_t mBitCount;
	// functions tested:
	//		inline bool isUsed(const Arc& inArc);
	//		bool isUsed(const Tilewire& inTilewire1, const Tilewire& inTilewire2) const;
	//		inline void use(const Arc& inArc) ;
	//		void use(const Tilewire& inTilewire1, const Tilewire& inTilewire2);
	//		inline void release(const Arc& inArc);
	//		void release(const Tilewire& inTilewire1, const Tilewire& inTilewire2);
	//		uint32_t getArcUsageCount(void) const;
	//		TileCount getTileUsageCount(void) const;
	//		uint32_t getBitCount(void) const;
	BOOST_CHECK_EQUAL(arcUsage.isUsed(arc1), false);
	arcUsage.use(arc1);
	BOOST_CHECK_EQUAL(arcUsage.isUsed(arc1), true);
	BOOST_CHECK_EQUAL(arcUsage.getArcUsageCount(), 1u);
	BOOST_CHECK_EQUAL(arcUsage.getTileUsageCount(), TileCount(1));
	BOOST_CHECK_EQUAL(arcUsage.getBitCount(), virtexCenterArcCount);
	arcUsage.release(arc1);
	BOOST_CHECK_EQUAL(arcUsage.isUsed(arc1), false);
	BOOST_CHECK_EQUAL(arcUsage.getArcUsageCount(), 0u);
	BOOST_CHECK_EQUAL(arcUsage.getTileUsageCount(), TileCount(1));
	BOOST_CHECK_EQUAL(arcUsage.getBitCount(), virtexCenterArcCount);

	// exceptions tested:
	//		InvalidArcException;
	// functions tested:
	//		uint32_t getArcOffset(const Tilewire& inTilewire1, const Tilewire& inTilewire2) const;
	bool threwInvalidArcException = false;
	try {
		// this arc's tilewires are undefined
		(void) arcUsage.getArcOffset(Tilewire(), Tilewire());
	}
	catch(InvalidArcException iae) {
		threwInvalidArcException = true;		
	}
	BOOST_CHECK_EQUAL(threwInvalidArcException, true);
	threwInvalidArcException = false;
	try {
		// this arc is valid (and its tilewires are defined)
		(void) arcUsage.getArcOffset(tilewire1, tilewire2);
	}
	catch(InvalidArcException iae) {
		threwInvalidArcException = true;		
	}
	BOOST_CHECK_EQUAL(threwInvalidArcException, false);
	threwInvalidArcException = false;
	try {
		// this arc's tilewires are defined, but the are is invalid
		(void) arcUsage.getArcOffset(tilewire1, tilewire3);
	}
	catch(InvalidArcException iae) {
		threwInvalidArcException = true;		
	}
	BOOST_CHECK_EQUAL(threwInvalidArcException, true);

	// functions tested:
	//		void clear(void);
	arcUsage.use(arc1);
	arcUsage.clear();
	BOOST_CHECK_EQUAL(arcUsage.getArcUsageCount(), 0u);
	BOOST_CHECK_EQUAL(arcUsage.getTileUsageCount(), TileCount(1));
	BOOST_CHECK_EQUAL(arcUsage.getBitCount(), virtexCenterArcCount);

	// functions not tested:
	//		~ArcUsage(void);
	ArcUsage* arcUsagePtr = new ArcUsage(tiles);
	arcUsagePtr->autosize();
	arcUsagePtr->use(arc1);
	arcUsagePtr->~ArcUsage();
	uint32_t bitsetsAfterDestruction = 0;
	for(uint32_t i = 0; i < arcUsagePtr->mBitsets.getSize(); i++) { 
		bitsetsAfterDestruction += (arcUsagePtr->mBitsets[i] != 0); 
	}
	BOOST_CHECK_EQUAL(bitsetsAfterDestruction, 0u);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
