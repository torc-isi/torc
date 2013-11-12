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
/// \brief Unit test for the Spartan6Packet class.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/Spartan6Packet.hpp"
#include "torc/bitstream/Spartan6.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)

BOOST_AUTO_TEST_CASE(Spartan6PacketUnitTest) {

	// build the file paths
	boost::filesystem::path referencePath = torc::common::DirectoryTree::getExecutablePath()
		/ "torc" / "bitstream" / "Spartan6UnitTest.reference.bit";
	boost::filesystem::path generatedPath = torc::common::DirectoryTree::getExecutablePath()
		/ "regression" / "Spartan6PacketUnitTest.generated.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	Spartan6 bitstream;
	bitstream.readHeader(fileStream);
	std::string designName = bitstream.getDesignName();
	std::string deviceName = bitstream.getDeviceName();
	std::string designDate = bitstream.getDesignDate();
	std::string designTime = bitstream.getDesignTime();
	uint32_t bitstreamWordLength = bitstream.getBitstreamByteLength() >> 2;
	std::cout << bitstream << std::endl;

	// add up the words lengths
	uint32_t cumulativeWordLength = 0;
	std::vector<Spartan6Packet> packets;
	while(cumulativeWordLength < bitstreamWordLength) {
		Spartan6Packet packet = Spartan6Packet::read(fileStream);
		packets.push_back(packet);
		cumulativeWordLength += packet.getWordSize();
	}

	cumulativeWordLength = 0;
	std::vector<Spartan6Packet>::iterator p = packets.begin();
	std::vector<Spartan6Packet>::iterator e = packets.end();
	while(p < e) {
		Spartan6Packet& packet = *p++;
		cumulativeWordLength += packet.getWordSize();
	}

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
