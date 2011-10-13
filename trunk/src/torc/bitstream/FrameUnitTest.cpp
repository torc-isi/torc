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
/// \brief Unit test for the Frame class.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/Frame.hpp"

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)

/// \brief Unit test for the Frame class.
BOOST_AUTO_TEST_CASE(FrameUnitTest) {
	// members tested:
	//		bool mIsUsed;
	//		bool mIsDirty;
	//		uint32_t mLength;
	//		WORD_TYPE* mWords;
	// functions tested:
	//		Frame(uint32_t inLength, word_t* inWords = 0);
	//		~Frame(void);
	//		uint32_t getLength(void) const;
	//		const word_t* getWords(void) const;
	//		bool isUsed(void) const;
	//		bool isDirty(void) const;
	//		word_t operator[] (int index);
	BOOST_REQUIRE(true);
	const boost::uint32_t length = 3;
	VirtexFrame::word_t data[length] = { 0, 1, 2 };
	VirtexFrame virtexFrame1(length, data);
	BOOST_CHECK_EQUAL(virtexFrame1.getLength(), length);
	BOOST_CHECK_EQUAL(virtexFrame1.isUsed(), true);
	BOOST_CHECK_EQUAL(virtexFrame1.isDirty(), false);
	const VirtexFrame::word_t* p1 = virtexFrame1.getWords();
	for(boost::uint32_t i = 0; i < length; i++) {
		BOOST_CHECK_EQUAL(virtexFrame1[i], data[i]);
		BOOST_CHECK_EQUAL(*(p1 + i), data[i]);
	}
	VirtexFrame virtexFrame2(length);
	virtexFrame2.touch();
	BOOST_CHECK_EQUAL(virtexFrame2.getLength(), length);
	BOOST_CHECK_EQUAL(virtexFrame2.isUsed(), true);
	BOOST_CHECK_EQUAL(virtexFrame2.isDirty(), true);
	const VirtexFrame::word_t* p2 = virtexFrame2.getWords();
	BOOST_CHECK(p2 == 0);
	for(boost::uint32_t i = 0; i < length; i++) {
		BOOST_CHECK_EQUAL(virtexFrame2[i], 0u);
	}
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
