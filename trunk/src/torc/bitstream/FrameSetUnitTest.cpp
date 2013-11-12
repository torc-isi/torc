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
/// \brief Unit test for the FrameSet class.

#include "torc/bitstream/FrameSet.hpp"
#include "torc/bitstream/Virtex5.hpp"
#include <boost/test/unit_test.hpp>

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)

/// \brief Unit test for the FrameSet class.
BOOST_AUTO_TEST_CASE(FrameSetUnitTest) {
	// functions tested:
	//		FrameSet(uint32_t inFrameLength, uint32_t inFrameCount);
	const boost::uint32_t length = 0x41;
	const boost::uint32_t count = 5;
	VirtexFrameSet virtexFrameSet(length, count);
	BOOST_CHECK_EQUAL(virtexFrameSet.size(), count);
	VirtexFrameSet::const_iterator p = virtexFrameSet.begin();
	VirtexFrameSet::const_iterator e = virtexFrameSet.end();
	while(p < e) {
		const VirtexFrameSharedPtr& vfsp = *p++;
		BOOST_CHECK_EQUAL(vfsp->getLength(), length);
//		const VirtexFrame& vf = **p++;
//		BOOST_CHECK_EQUAL(vf.getLength(), length);
	}
}

/// \brief Unit test for the FrameBlocks class.
BOOST_AUTO_TEST_CASE(FrameBlocksUnitTest) {
	// members tested:
	//		mBlock[];
	const boost::uint32_t length = 0x41;
	VirtexFrameBlocks blocks;
	int count[Bitstream::eBlockTypeCount] = { 1000, 1000, 500 };
	for(int i = 0; i < Bitstream::eBlockTypeCount; i++) {
		for(int j = 0; j < count[i]; j++) {
			blocks.mBlock[i].push_back(
				VirtexFrameSet::FrameSharedPtr(new VirtexFrame(length)));
		}
	}
	BOOST_CHECK_EQUAL(blocks.mBlock[Virtex5::eFarBlockType0].size(), 1000u);
	BOOST_CHECK_EQUAL(blocks.mBlock[Virtex5::eFarBlockType1].size(), 1000u);
	BOOST_CHECK_EQUAL(blocks.mBlock[Virtex5::eFarBlockType2].size(), 500u);
	BOOST_CHECK_EQUAL(blocks.mBlock[Virtex5::eFarBlockType3].size(), 0u);
	BOOST_CHECK_EQUAL(blocks.mBlock[Virtex5::eFarBlockType4].size(), 0u);
	BOOST_CHECK_EQUAL(blocks.mBlock[Virtex5::eFarBlockType5].size(), 0u);
	BOOST_CHECK_EQUAL(blocks.mBlock[Virtex5::eFarBlockType6].size(), 0u);
	BOOST_CHECK_EQUAL(blocks.mBlock[Virtex5::eFarBlockType7].size(), 0u);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
