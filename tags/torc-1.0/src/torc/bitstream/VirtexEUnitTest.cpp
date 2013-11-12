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
/// \brief Unit test for the VirtexE class.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/VirtexE.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/common/Devices.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/common/DeviceDesignator.hpp"
#include "torc/bitstream/OutputStreamHelpers.hpp"
#include "torc/bitstream/build/DeviceInfoHelper.hpp"
#include "torc/common/TestHelpers.hpp"
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)


/// \brief Unit test for the VirtexE class.
BOOST_AUTO_TEST_CASE(VirtexEUnitTest) {

	// enums tested:
	//		EPacket
	//		EFar
	boost::uint32_t mask;
	// type 1 packet subfield masks
	mask = VirtexE::ePacketMaskType + VirtexE::ePacketMaskOpcode 
		+ VirtexE::ePacketMaskType1Address + VirtexE::ePacketMaskType1Reserved 
		+ VirtexE::ePacketMaskType1Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// type 2 packet subfield masks
	mask = VirtexE::ePacketMaskType + VirtexE::ePacketMaskOpcode 
		+ VirtexE::ePacketMaskType2Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// frame address register subfield masks
	mask = VirtexE::eFarMaskBlockType+ VirtexE::eFarMaskMajor +
	 	VirtexE::eFarMaskMinor;
	BOOST_CHECK_EQUAL(mask, 0x07FFFE00u);

	// members tested:
	//		VirtexE::sPacketTypeName and EPacketTypeName
	BOOST_CHECK_EQUAL(VirtexE::sPacketTypeName[0],							"[UNKNOWN TYPE 0]");
	BOOST_CHECK_EQUAL(VirtexE::sPacketTypeName[VirtexE::ePacketType1],		"TYPE1");
	BOOST_CHECK_EQUAL(VirtexE::sPacketTypeName[VirtexE::ePacketType2],		"TYPE2");
	BOOST_CHECK_EQUAL(VirtexE::sPacketTypeName[3],							"[UNKNOWN TYPE 3]");
	BOOST_CHECK_EQUAL(VirtexE::sPacketTypeName[4],							"[UNKNOWN TYPE 4]");
	BOOST_CHECK_EQUAL(VirtexE::sPacketTypeName[5],							"[UNKNOWN TYPE 5]");
	BOOST_CHECK_EQUAL(VirtexE::sPacketTypeName[6],							"[UNKNOWN TYPE 6]");
	BOOST_CHECK_EQUAL(VirtexE::sPacketTypeName[7],							"[UNKNOWN TYPE 7]");

	// members tested:
	//		VirtexE::sOpcodeName and EOpcode
	BOOST_CHECK_EQUAL(VirtexE::sOpcodeName[VirtexE::eOpcodeRead],			"READ");
	BOOST_CHECK_EQUAL(VirtexE::sOpcodeName[VirtexE::eOpcodeWrite],			"WRITE");

	// members tested:
	//		VirtexE::sRegisterName and ERegister
	BOOST_CHECK_EQUAL(VirtexE::sRegisterName[VirtexE::eRegisterCRC],		"CRC");
	BOOST_CHECK_EQUAL(VirtexE::sRegisterName[VirtexE::eRegisterFAR],		"FAR");
	BOOST_CHECK_EQUAL(VirtexE::sRegisterName[VirtexE::eRegisterFDRI],		"FDRI");
	BOOST_CHECK_EQUAL(VirtexE::sRegisterName[VirtexE::eRegisterFDRO],		"FDRO");
	BOOST_CHECK_EQUAL(VirtexE::sRegisterName[VirtexE::eRegisterCMD],		"CMD");
	BOOST_CHECK_EQUAL(VirtexE::sRegisterName[VirtexE::eRegisterCTL],		"CTL");
	BOOST_CHECK_EQUAL(VirtexE::sRegisterName[VirtexE::eRegisterMASK],		"MASK");
	BOOST_CHECK_EQUAL(VirtexE::sRegisterName[VirtexE::eRegisterSTAT],		"STAT");
	BOOST_CHECK_EQUAL(VirtexE::sRegisterName[VirtexE::eRegisterLOUT],		"LOUT");
	BOOST_CHECK_EQUAL(VirtexE::sRegisterName[VirtexE::eRegisterCOR],		"COR");
	BOOST_CHECK_EQUAL(VirtexE::sRegisterName[VirtexE::eRegisterFLR],		"FLR");

	// members tested:
	//		VirtexE::sCommandName and ECommand
	BOOST_CHECK_EQUAL(VirtexE::sCommandName[VirtexE::eCommandWCFG],			"WCFG");
	BOOST_CHECK_EQUAL(VirtexE::sCommandName[VirtexE::eCommandLFRM],			"LFRM");
	BOOST_CHECK_EQUAL(VirtexE::sCommandName[VirtexE::eCommandRCFG],			"RCFG");
	BOOST_CHECK_EQUAL(VirtexE::sCommandName[VirtexE::eCommandSTART],		"START");
	BOOST_CHECK_EQUAL(VirtexE::sCommandName[VirtexE::eCommandRCAP],			"RCAP");
	BOOST_CHECK_EQUAL(VirtexE::sCommandName[VirtexE::eCommandRCRC],			"RCRC");
	BOOST_CHECK_EQUAL(VirtexE::sCommandName[VirtexE::eCommandAGHIGH],		"AGHIGH");
	BOOST_CHECK_EQUAL(VirtexE::sCommandName[VirtexE::eCommandSWITCH],		"SWITCH");

	// build the file paths
	boost::filesystem::path referencePath = torc::common::DirectoryTree::getExecutablePath()
		/ "torc" / "bitstream" / "VirtexEUnitTest.reference.bit";
	boost::filesystem::path generatedPath = torc::common::DirectoryTree::getExecutablePath()
		/ "regression" / "VirtexEUnitTest.generated.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	VirtexE bitstream;
	bitstream.read(fileStream, false);
	// write the bitstream digest to the console
	std::cout << bitstream << std::endl;

	std::string designName = bitstream.getDesignName();
	std::string deviceName = bitstream.getDeviceName();
	std::string designDate = bitstream.getDesignDate();
	std::string designTime = bitstream.getDesignTime();
	torc::common::DeviceDesignator deviceDesignator(deviceName);
	std::cout << "family of " << deviceName << " is " << deviceDesignator.getFamily() << std::endl;

	// write the bitstream back out
	std::fstream outputStream(generatedPath.string().c_str(), std::ios::binary | std::ios::out);
	BOOST_REQUIRE(outputStream.good());
	bitstream.write(outputStream);
	outputStream.flush();

	// compare the reference and generated XDL
	BOOST_CHECK(torc::common::fileContentsAreEqual(generatedPath, referencePath));
}








































void testVirtexEDevice(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath);

/// \brief Unit test for the VirtexE class Frame Address Register mapping.
BOOST_AUTO_TEST_CASE(VirtexEFarUnitTest) {

	// look up the command line arguments
	int& argc = boost::unit_test::framework::master_test_suite().argc;
	char**& argv = boost::unit_test::framework::master_test_suite().argv;
	// make sure that we at least have the name under which we were invoked
	BOOST_REQUIRE(argc >= 1);
	// resolve symbolic links if applicable
	torc::common::DirectoryTree directoryTree(argv[0]);

	// iterate over the devices
	const torc::common::DeviceVector& devices = torc::common::Devices::getVirtexEDevices();
	torc::common::DeviceVector::const_iterator dp = devices.begin();
	torc::common::DeviceVector::const_iterator de = devices.end();
	while(dp < de) {
		const std::string& device = *dp++;
		if(device.empty()) break;
//std::cout << "device " << ": " << device << std::endl;
		testVirtexEDevice(device, torc::common::DirectoryTree::getWorkingPath());
	}
}

/*
	class TileTypeWidths {
	public:
		uint32_t mWidth[8];
		TileTypeWidths(uint32_t in0 = 0, uint32_t in1 = 0, uint32_t in2 = 0, uint32_t in3 = 0, 
			uint32_t in4 = 0, uint32_t in5 = 0, uint32_t in6 = 0, uint32_t in7 = 0) {
			int i = 0;
			mWidth[i++] = in0; mWidth[i++] = in1; mWidth[i++] = in2; mWidth[i++] = in3;
			mWidth[i++] = in4; mWidth[i++] = in5; mWidth[i++] = in6; mWidth[i++] = in7;
		}
		void clear(void) { for(int i = 0; i < 8; i++) mWidth[i] = 0; }
		uint32_t operator[] (int inIndex) const { return mWidth[inIndex]; }
	};
*/

	extern std::ostream& operator<< (std::ostream& os, const Virtex::FrameAddress& rhs);
	//std::ostream& operator<< (std::ostream& os, const Virtex::FrameAddress& rhs) {
	//	return os << rhs.mBlockType << "(" << rhs.mMajor << "." << rhs.mMinor << ")";
	//}

void testVirtexEDevice(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath) {

	// build the file paths
	boost::filesystem::path debugBitstreamPath = inWorkingPath / "torc" / "bitstream" / "regression";
	//boost::filesystem::path generatedPath = debugBitstreamPath / (inDeviceName + ".debug.bit");
	boost::filesystem::path referencePath = debugBitstreamPath / (inDeviceName + ".debug.bit");
std::cerr << "TRYING TO FIND " << referencePath << std::endl;

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	std::cerr << "Trying to read: " << referencePath << std::endl;
	BOOST_REQUIRE(fileStream.good());
	VirtexE bitstream;
	bitstream.read(fileStream, false);
	// write the bitstream digest to the console
//	std::cout << bitstream << std::endl;

//	// initialize the bitstream frame maps
//	boost::filesystem::path deviceColumnsPath = inWorkingPath / "regression" 
//		/ (inDeviceName + ".cpp");
//	std::fstream deviceColumnsStream(deviceColumnsPath.string().c_str(), std::ios::out);
	bitstream.initializeDeviceInfo(inDeviceName);
	bitstream.initializeFrameMaps();

	// iterate through the packets, and extract all of the FARs
	VirtexE::FrameAddressToIndex farRemaining = bitstream.mFrameAddressToIndex;
	VirtexE::FrameAddressToIndex farVisited;
	{
	    bool first = true;
		VirtexE::const_iterator p = bitstream.begin();
		VirtexE::const_iterator e = bitstream.end();
		uint32_t header = VirtexPacket::makeHeader(VirtexPacket::ePacketType1, 
			VirtexPacket::eOpcodeWrite, VirtexE::eRegisterLOUT, 1);
		while(p < e) {
			const VirtexPacket& packet = *p++;
			if(packet.getHeader() != header) continue;
			if(first) { first = false; continue; }
			VirtexE::FrameAddress far = packet[1];
			//std::cout << std::endl << "Debug Far Address: " << Hex32(packet[1]) << std::endl;
			farVisited[far] = 0;
			VirtexE::FrameAddressToIndex::iterator found = farRemaining.find(far);
			if(found != farRemaining.end()) {
				farRemaining.erase(found);
			} else {
				std::cerr << "missing " << far << " ";
			}
		}
	}
	// verify that we have visited all of the expected FARs and no others
	std::cout << "Device: " << inDeviceName << std::endl;
	std::cout << "Size of farRemaining: " << farRemaining.size() << std::endl;
	std::cout << "Size of farVisited: " << farVisited.size() << std::endl;
	BOOST_REQUIRE_EQUAL(bitstream.mFrameAddressToIndex.size(), farVisited.size());
	BOOST_REQUIRE_EQUAL(farRemaining.size(), 0u);

return;
}

void testVirtexEFullMapping(const boost::filesystem::path& inWorkingPath);

/// \brief Unit test for the VirtexE bitstream to bitmap conversion.
BOOST_AUTO_TEST_CASE(VirtexEMapUnitTest) {
	// look up the command line arguments
	int& argc = boost::unit_test::framework::master_test_suite().argc;
	char**& argv = boost::unit_test::framework::master_test_suite().argv;
	// make sure that we at least have the name under which we were invoked
	BOOST_REQUIRE(argc >= 1);
	// resolve symbolic links if applicable
	torc::common::DirectoryTree directoryTree(argv[0]);
	testVirtexEFullMapping(torc::common::DirectoryTree::getWorkingPath());
}


void testVirtexEFullMapping(const boost::filesystem::path& inWorkingPath) {
	// build the file paths
	boost::filesystem::path referencePath = torc::common::DirectoryTree::getExecutablePath()
		/ "torc" / "bitstream" / "VirtexEUnitTest.reference.bit";
	boost::filesystem::path generatedPath = torc::common::DirectoryTree::getExecutablePath()
		/ "regression" / "VirtexEMapUnitTest.generated.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	// read and gather bitstream frames
	VirtexE bitstream;
	bitstream.read(fileStream, false);

	// initialize frame map
	bitstream.initializeDeviceInfo("xcv50e");
	bitstream.initializeFrameMaps();

	// load bitstream frames in data structure
	bitstream.initializeFullFrameBlocks();

	// write full bitstream from frame blocks data structure
	uint32_t frameLength = bitstream.getFrameLength();
	typedef boost::shared_array<uint32_t> WordSharedArray;
	VirtexE::iterator p = bitstream.begin();
	VirtexE::iterator e = bitstream.end();
	while(p < e) {
		const VirtexPacket& packet = *p++;
		if(packet.isType2()) {
			WordSharedArray words = packet.getWords();
			uint32_t* ptr = words.get();
			for(uint32_t block = 0; block < 8; block++) {
				for(uint32_t frame = 0; frame < bitstream.mBlockFrameIndexBounds[block]; frame++) {
					VirtexFrameBlocks::word_t* words = const_cast<VirtexFrameBlocks::word_t*>(bitstream.mFrameBlocks.mBlock[block][frame]->getWords());
					for(uint32_t index = 0; index < frameLength; index++) {
						*ptr++ = words[index];
					}
				}
				// only block type 0 is present in this particular bitstream
				// if we try to copy block type 1 frames into the packet, we will trash memory
				break;
			}
		}
	}
	// write the test bitstream back out
	std::fstream outputStream(generatedPath.string().c_str(), std::ios::binary | std::ios::out);
	BOOST_REQUIRE(outputStream.good());
	bitstream.write(outputStream);
	outputStream.flush();
	BOOST_REQUIRE(torc::common::fileContentsAreEqual(referencePath, generatedPath));

	return;
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
