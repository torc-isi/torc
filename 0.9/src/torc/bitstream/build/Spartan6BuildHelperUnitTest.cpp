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
/// \brief Source for the Spartan6BuildHelper unit test.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/build/Spartan6BuildHelper.hpp"
#include "torc/bitstream/Spartan6.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/common/Devices.hpp"
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)

void buildSpartan6Partials(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath);
void buildSpartan6Partials(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath) {

	// build the file paths
	boost::filesystem::path regressionPath = inWorkingPath / "torc" / "bitstream" / "regression";
	boost::filesystem::path generatedPathOdd = regressionPath / (inDeviceName + ".odd.bit");
	boost::filesystem::path generatedPathEven = regressionPath / (inDeviceName + ".even.bit");
	boost::filesystem::path referencePath = regressionPath / (inDeviceName + ".full.bit");

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	std::cerr << "Trying to read: " << referencePath << std::endl;
	BOOST_REQUIRE(fileStream.good());
	Spartan6 bitstream;
	bitstream.read(fileStream, false);
	// write the bitstream digest to the console
//	std::cout << bitstream << std::endl;

	// determine the device IO frame length and the FDRI word count; then touch every other frame 
	// in the device
	uint16_t stdFrameLength = 65;
	uint16_t ioFrameLength = 0;
	uint32_t wordCount = 0;
	Spartan6::iterator p = bitstream.begin();
	Spartan6::iterator e = bitstream.end();
	// describe the FLR and FDRI write headers
	uint32_t writeFLRHeader = Spartan6Packet::makeHeader(Spartan6Packet::ePacketType1, 
		Spartan6Packet::eOpcodeWrite, Spartan6::eRegisterFLR, 1);
	uint32_t writeFDRIHeader = Spartan6Packet::makeHeader(Spartan6Packet::ePacketType2, 
		Spartan6Packet::eOpcodeWrite, Spartan6::eRegisterFDRI, 1);
	while(p < e) {
		const Spartan6Packet& packet = *p++;
		// determine the IO frame length
		if(packet.getHeader() == writeFLRHeader) {
			ioFrameLength = packet[1] + 1;
			std::cout << inDeviceName << " IO frame length: " << ioFrameLength << std::endl;
		// determine the FDRI word count, and touch every other frame
		} else if(packet.getHeader() == writeFDRIHeader) {
			// get the word count
			wordCount = packet.getWordCount();
			boost::shared_array<uint16_t> words = packet.getWords();
			std::cout << inDeviceName << " FDRI word count: " << wordCount << std::endl;
			std::cout << inDeviceName << " stdandard words: " << (wordCount - ioFrameLength) 
				<< std::endl;

			// invert a word in every odd frame and write the resulting bitstream

			{
				// iterate over every other frame, breaking right after we touch the IO frame
				size_t pos = 1;
				size_t end = wordCount - ioFrameLength - stdFrameLength;
				while(pos <= end) {
					words[pos] = ~words[pos];
					pos += (stdFrameLength << 1);
					// note that we add twice the frame length, effectively skipping every other 
					// frame
				}
				// write the modified bitstream for use as a partial bitstream reference
				std::fstream outputStream(generatedPathOdd.string().c_str(), 
					std::ios::binary | std::ios::out);
				BOOST_REQUIRE(outputStream.good());
				bitstream.write(outputStream);
				outputStream.flush();
			}

			// undo inversions to odd frames, and invert a word in every even frame

			{
				// iterate over every other frame, breaking right after we touch the IO frame
				size_t pos = 1;
				size_t end = wordCount - ioFrameLength - stdFrameLength;
				while(pos <= end) {
					words[pos] = ~words[pos];
					pos += stdFrameLength;
					// note that by flipping a word in *every* frame, we also undo the previous 
					// inversions to odd frames
				}
				// write the modified bitstream for use as a partial bitstream reference
				std::fstream outputStream(generatedPathEven.string().c_str(), 
					std::ios::binary | std::ios::out);
				BOOST_REQUIRE(outputStream.good());
				bitstream.write(outputStream);
				outputStream.flush();
			}

		}
	}

}

/// \brief Unit test for the Spartan6BuildHelper class.
BOOST_AUTO_TEST_CASE(bitstream_spartan6_build_helper) {

	BOOST_REQUIRE(true);

	// look up the command line arguments
	int& argc = boost::unit_test::framework::master_test_suite().argc;
	char**& argv = boost::unit_test::framework::master_test_suite().argv;
	// make sure that we at least have the name under which we were invoked
	BOOST_REQUIRE(argc >= 1);
	// resolve symbolic links if applicable
	torc::common::DirectoryTree directoryTree(argv[0]);

	// iterate over the Spartan6 devices
	{
		const torc::common::DeviceVector& devices = torc::common::Devices::getSpartan6Devices();
		torc::common::DeviceVector::const_iterator dp = devices.begin();
		torc::common::DeviceVector::const_iterator de = devices.end();
		while(dp < de) {
			const std::string& device = *dp++;
			if(device.empty()) break;
			buildSpartan6Partials(device, torc::common::DirectoryTree::getWorkingPath());
		}
	}

	// iterate over the Spartan6L devices
	{
		const torc::common::DeviceVector& devices = torc::common::Devices::getSpartan6LDevices();
		torc::common::DeviceVector::const_iterator dp = devices.begin();
		torc::common::DeviceVector::const_iterator de = devices.end();
		while(dp < de) {
			const std::string& device = *dp++;
			if(device.empty()) break;
			buildSpartan6Partials(device, torc::common::DirectoryTree::getWorkingPath());
		}
	}

}

/// \brief Unit test to dump Spartan6 bitstream packets.
BOOST_AUTO_TEST_CASE(bitstream_spartan6_dump_packets) {

	// look up the command line arguments
	int& argc = boost::unit_test::framework::master_test_suite().argc;
	char**& argv = boost::unit_test::framework::master_test_suite().argv;
	// make sure that we at least have the name under which we were invoked
	BOOST_REQUIRE(argc >= 1);
	// resolve symbolic links if applicable
	torc::common::DirectoryTree directoryTree(argv[0]);

	// build the file paths
	std::string inDeviceName = "xc6slx4";
	boost::filesystem::path regressionPath = torc::common::DirectoryTree::getWorkingPath() 
		/ "torc" / "bitstream" / "regression";
	boost::filesystem::path referencePathOdd = regressionPath / (inDeviceName + ".partial.odd.bit");
	boost::filesystem::path referencePathEven = regressionPath / (inDeviceName + ".partial.even.bit");

	{
		// read the bitstream
		std::fstream fileStream(referencePathOdd.string().c_str(), std::ios::binary | std::ios::in);
		std::cerr << "Trying to read: " << referencePathOdd << std::endl;
		BOOST_REQUIRE(fileStream.good());
		Spartan6 bitstream;
		bitstream.read(fileStream, false);
		// write the bitstream digest to the console
		std::cout << bitstream << std::endl;
	}

	{
		// read the bitstream
		std::fstream fileStream(referencePathEven.string().c_str(), std::ios::binary | std::ios::in);
		std::cerr << "Trying to read: " << referencePathEven << std::endl;
		BOOST_REQUIRE(fileStream.good());
		Spartan6 bitstream;
		bitstream.read(fileStream, false);
		// write the bitstream digest to the console
		std::cout << bitstream << std::endl;
	}

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
