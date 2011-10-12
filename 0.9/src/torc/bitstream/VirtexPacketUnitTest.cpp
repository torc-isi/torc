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
/// \brief Source for the VirtexPacket unit test.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/VirtexPacket.hpp"
#include "torc/bitstream/Virtex4.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)

/// \brief Unit test for the VirtexPacket class.
BOOST_AUTO_TEST_CASE(bitstream_virtex_packet) {

	// build the file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "Virtex4UnitTest.generated.bit";
	boost::filesystem::path referencePath = regressionPath / "Virtex4UnitTest.reference.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	Virtex4 bitstream;
	bitstream.readHeader(fileStream);
	std::string designName = bitstream.getDesignName();
	std::string deviceName = bitstream.getDeviceName();
	std::string designDate = bitstream.getDesignDate();
	std::string designTime = bitstream.getDesignTime();
	uint32_t bitstreamWordLength = bitstream.getBitstreamByteLength() >> 2;
	std::cout << bitstream << std::endl;

	// add up the words lengths
	uint32_t cumulativeWordLength = 0;
	std::vector<VirtexPacket> packets;
	while(cumulativeWordLength < bitstreamWordLength) {
		VirtexPacket packet = VirtexPacket::read(fileStream);
		packets.push_back(packet);
		cumulativeWordLength += packet.getWordSize();
	}

	cumulativeWordLength = 0;
	std::vector<VirtexPacket>::iterator p = packets.begin();
	std::vector<VirtexPacket>::iterator e = packets.end();
	while(p < e) {
		VirtexPacket& packet = *p++;
		cumulativeWordLength += packet.getWordSize();
	}

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
