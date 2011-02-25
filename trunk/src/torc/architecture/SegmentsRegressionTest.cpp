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
/// \brief Source for the Segments regression test.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/Segments.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/common/Devices.hpp"
#include "torc/architecture/DDBStreamHelper.hpp"
#include "torc/architecture/OutputStreamHelpers.hpp"
#include <iostream>
#include <iomanip>

namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

	// functions not tested:
	//		const Segments::IrregularArc* getIrregularArc(TileIndex inTileIndex, 
	//			WireIndex inSourceWireIndex, WireIndex inSinkWireIndex);
	//		uint32_t getTotalSegmentCount(void) const;
	//		uint32_t getCompactSegmentCount(void) const;
	//		uint32_t getIrregularArcCount(void) const;
	//		const Segments::SegmentReference& getTilewireSegment(const Tilewire& inTilewire) const;
	// members not tested:
	//		Array2D<CompactSegmentTilewire> mCompactSegments;
	//		Array2D<IrregularArc> mIrregularArcs;
	//		CompactSegmentCount mCompactSegmentCount;
	//		uint32_t mIrregularArcCount;
	//		uint32_t mTotalWireCount;
	//		uint32_t mTotalSegmentCount;

/// \brief Regression test class for segments consistency across all devices.
class segments_regression_test_helper {
	typedef xilinx::TileCount TileCount;
	typedef xilinx::TileIndex TileIndex;
	typedef xilinx::TileTypeIndex TileTypeIndex;
	typedef xilinx::WireCount WireCount;
	typedef xilinx::WireIndex WireIndex;
	typedef Segments::SegmentReference SegmentReference;
	typedef xilinx::CompactSegmentIndex CompactSegmentIndex;
	typedef boost::uint64_t uint64_t;
	DDB& mDDB;
	const Tiles& mTiles;
	const Segments& mSegments;
	std::map<Tilewire, CompactSegmentIndex> mUsage;
	const CompactSegmentIndex cUsagePruned;
	const CompactSegmentIndex cUsageUndefined;
	uint64_t mTilewireCount;
	uint64_t mTilewiresAnalyzed;
	uint64_t mTilewiresPruned;
	uint64_t mTilewiresUndefined;
	uint64_t mTilewireMismatches;
	static uint64_t sTotalTilewireCount;
	static uint64_t sTotalTilewiresAnalyzed;
	static uint64_t sTotalTilewiresPruned;
	static uint64_t sTotalTilewiresUndefined;
	static uint64_t sTotalTilewireMismatches;
public:
	/// \brief Basic constructor.
	segments_regression_test_helper(DDB& inDDB) : mDDB(inDDB), mTiles(mDDB.getTiles()), 
		mSegments(mDDB.getSegments()), cUsagePruned(CompactSegmentIndex(-1)), 
		cUsageUndefined(CompactSegmentIndex(-2)), mTilewireCount(0), mTilewiresAnalyzed(0), 
		mTilewiresPruned(0), mTilewiresUndefined(0), mTilewireMismatches(0) {
		// functions tested during database initialization and deletion:
		//		Segments(void);
		//		size_t readTilewireSegments(DigestStream& inStream);
		//		size_t readSegments(DigestStream& inStream);
		//		size_t readIrregularArcs(DigestStream& inStream);
	}
	/// \brief Outputs statistics on segment expansion.
	static void statistics(void) {
		std::cerr << "=======================================" << std::endl;
		std::cerr << "Total tilewire count:      " << std::setw(12) << sTotalTilewireCount 
			<< std::endl;
		std::cerr << "Total tilewires analyzed:  " << std::setw(12) << sTotalTilewiresAnalyzed 
			<< std::endl;
		std::cerr << "Total tilewires pruned:    " << std::setw(12) << sTotalTilewiresPruned 
			<< std::endl;
		std::cerr << "Total tilewires undefined: " << std::setw(12) << sTotalTilewiresUndefined 
			<< std::endl;
		std::cerr << "Total tilewire mismatches: " << std::setw(12) << sTotalTilewireMismatches 
			<< std::endl;
	}
	/// \brief Analyze segment expansion, and detect any tilewire conflicts.
	void operator ()(void) {
		TileCount tileCount = mTiles.getTileCount();
		mUsage.clear();

		// members tested:
		//		Array2D<SegmentReference> mTilewireSegments;
		// clear the usage information for all tilewires
		for(TileIndex tileIndex; tileIndex < tileCount; tileIndex++) {
			const Array<SegmentReference>& tilewireSegments 
				= mSegments.mTilewireSegments[tileIndex];
			WireCount wireCount = WireCount(tilewireSegments.getSize());
			for(WireIndex wireIndex; wireIndex < wireCount; wireIndex++) {
				Tilewire tilewire(tileIndex, wireIndex);
				mUsage[tilewire] = cUsageUndefined;
				mTilewireCount++;
			}
		}
		// iterate through all tilewires and look for any conflicting segment ownership
		for(TileIndex tileIndex; tileIndex < tileCount; tileIndex++) {
			const Array<SegmentReference>& tilewireSegments 
				= mSegments.mTilewireSegments[tileIndex];
			WireCount wireCount = WireCount(tilewireSegments.getSize());
			for(WireIndex wireIndex; wireIndex < wireCount; wireIndex++) {
				Tilewire tilewire(tileIndex, wireIndex);
				analyzeSegmentTilewire(tilewire);
				TilewireVector segmentTilewires;
				mDDB.expandSegment(tilewire, segmentTilewires, DDB::eExpandDirectionNone);
				TilewireVector::const_iterator p = segmentTilewires.begin();
				TilewireVector::const_iterator e = segmentTilewires.end();
				while(p < e) {
					analyzeSegmentTilewire(*p++);
				}
			}
		}
		// look for any tilewires without segment definitions
		for(TileIndex tileIndex; tileIndex < tileCount; tileIndex++) {
			const Array<SegmentReference>& tilewireSegments 
				= mSegments.mTilewireSegments[tileIndex];
			WireCount wireCount = WireCount(tilewireSegments.getSize());
			for(WireIndex wireIndex; wireIndex < wireCount; wireIndex++) {
				Tilewire tilewire(tileIndex, wireIndex);
				CompactSegmentIndex usage = mUsage[tilewire];
				if(usage == cUsageUndefined) {
					mTilewiresUndefined++;
					std::cerr << "Undefined: " << tilewire << std::endl;
					BOOST_CHECK(mUsage[tilewire] != cUsageUndefined);
				} else if(usage == cUsagePruned) {
					mTilewiresPruned++;
				}
			}
		}
		std::cerr << "---------------------------------------" << std::endl;
		std::cerr << "Tilewire count:            " << std::setw(12) << mTilewireCount 
			<< std::endl;
		std::cerr << "Tilewires analyzed:        " << std::setw(12) << mTilewiresAnalyzed 
			<< std::endl;
		std::cerr << "Tilewires pruned:          " << std::setw(12) << mTilewiresPruned 
			<< std::endl;
		std::cerr << "Tilewires undefined:       " << std::setw(12) << mTilewiresUndefined 
			<< std::endl;
		std::cerr << "Tilewire mismatches:       " << std::setw(12) << mTilewireMismatches 
			<< std::endl;
		sTotalTilewireCount += mTilewireCount;
		sTotalTilewiresAnalyzed += mTilewiresAnalyzed;
		sTotalTilewiresPruned += mTilewiresPruned;
		sTotalTilewiresUndefined += mTilewiresUndefined;
		sTotalTilewireMismatches += mTilewireMismatches;

		// functions tested:
		//		uint32_t getTotalWireCount(void) const;
		BOOST_CHECK_EQUAL(mTilewireCount - mTilewiresPruned, mSegments.getTotalWireCount());
		BOOST_CHECK(mTilewireMismatches == 0);
	}
	/// \brief Verifies and enforces segment ownership of the specified tilewire.
	void analyzeSegmentTilewire(const Tilewire& inTilewire) {
		mTilewiresAnalyzed++;
		// extract the tilewire information
		TileIndex tileIndex = inTilewire.getTileIndex();
		WireIndex wireIndex = inTilewire.getWireIndex();
		const Array<SegmentReference>& tilewireSegments = mSegments.mTilewireSegments[tileIndex];
		// look up the segment reference information
		const SegmentReference& segmentReference = tilewireSegments[wireIndex];
		CompactSegmentIndex compactSegmentIndex = segmentReference.getCompactSegmentIndex();
		TileIndex anchorTileIndex = segmentReference.getAnchorTileIndex();
		// look up the current tilewire membership
		CompactSegmentIndex usageCompactSegmentIndex = mUsage[inTilewire];
		if(compactSegmentIndex == usageCompactSegmentIndex) {
			// there has been no change
			//std::cerr << "Match: " << static_cast<int>(compactSegmentIndex) << " for " 
			//	<< inTilewire << std::endl;
		} else if(usageCompactSegmentIndex == cUsageUndefined) {
			// this wire was not previously defined, so we accept the new value
			mUsage[inTilewire] = compactSegmentIndex;
			//std::cerr << "Definition: " << static_cast<int>(compactSegmentIndex) << " for " 
			//	<< inTilewire << std::endl;
		} else {
			mTilewireMismatches++;
			std::cerr << "Mismatch (" << static_cast<int>(compactSegmentIndex) << " vs. " 
				<< static_cast<int>(usageCompactSegmentIndex) << ") for " << inTilewire 
				<< std::endl;
			BOOST_CHECK_EQUAL(static_cast<int>(compactSegmentIndex), 
				static_cast<int>(usageCompactSegmentIndex));
		}
	}
};

/// \brief Total number of tilewires visited.
uint64_t segments_regression_test_helper::sTotalTilewireCount = 0;
/// \brief Total number of tilewires analyzed (visited or expanded).
uint64_t segments_regression_test_helper::sTotalTilewiresAnalyzed = 0;
/// \brief Total number of tilewires pruned from the device (absent from XDLRC).
uint64_t segments_regression_test_helper::sTotalTilewiresPruned = 0;
/// \brief Total number of tilewires never defined (sanity check).
uint64_t segments_regression_test_helper::sTotalTilewiresUndefined = 0;
/// \brief Total number of tilewire mismatches (more than one segment contending for one tilewire).
uint64_t segments_regression_test_helper::sTotalTilewireMismatches = 0;

/// \brief Regression test for the Segments class.
BOOST_AUTO_TEST_CASE(architecture_segments_regression) {

	// iterate over the devices
	const torc::common::DeviceVector& devices = torc::common::Devices::getAllDevices();
	torc::common::DeviceVector::const_iterator dp = devices.begin();
	torc::common::DeviceVector::const_iterator de = devices.end();
	while(dp < de) {
		const std::string& device = *dp++;
		if(device.empty()) break;
		DDB ddb(device);
		std::cerr << ddb;
		segments_regression_test_helper segmentTester(ddb);
		segmentTester();
break;
	}
	segments_regression_test_helper::statistics();

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
