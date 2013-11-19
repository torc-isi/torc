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
/// \brief Unit test for the Virtex5 class.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/Virtex5.hpp"
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

#include <torc/Architecture.hpp>

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)

/// \brief Unit test for the Virtex5 FX130T getPrimaryXdlColumn()
BOOST_AUTO_TEST_CASE(Virtex5PrimaryXdlColumnUnitTest) {
	torc::architecture::DDB ddb("xc5vfx130t");
	const torc::architecture::Tiles& tiles = ddb.getTiles();
	torc::architecture::xilinx::TileRow rows = tiles.getRowCount();
	torc::architecture::xilinx::TileCol cols = tiles.getColCount();
	torc::architecture::xilinx::TileRow row(4);
	for(torc::architecture::xilinx::TileCol col(0); col < cols; col++) {
		torc::architecture::xilinx::TileIndex tileIndex = tiles.getTileIndex(row, col);
		const torc::architecture::TileInfo& tileInfo = tiles.getTileInfo(tileIndex);
		std::cerr << col << ": " << tileInfo.getName() << ", ";
	}
	std::cerr << std::endl;
	(void) rows;

	Virtex5 bitstream;
	bitstream.setDevice("xc5vfx130t");
	bitstream.initializeDeviceInfo("xc5vfx130t");
	bitstream.initializeFrameMaps();
	for(uint32_t xdlCol = 0; xdlCol < 190; xdlCol++) {
		uint32_t primaryXdlCol = bitstream.getPrimaryXdlColumn(xdlCol);
		std::cerr << xdlCol << ": " << primaryXdlCol << ", ";
	}
	std::cerr << std::endl;
	BOOST_REQUIRE(true);
}

/// \brief Unit test for the Virtex5 class.
BOOST_AUTO_TEST_CASE(Virtex5UnitTest) {

	// enums tested:
	//		EPacket
	//		EFar
	boost::uint32_t mask;
	// type 1 packet subfield masks
	mask = Virtex5::ePacketMaskType + Virtex5::ePacketMaskOpcode 
		+ Virtex5::ePacketMaskType1Address + Virtex5::ePacketMaskType1Reserved 
		+ Virtex5::ePacketMaskType1Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// type 2 packet subfield masks
	mask = Virtex5::ePacketMaskType + Virtex5::ePacketMaskOpcode 
		+ Virtex5::ePacketMaskType2Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// frame address register subfield masks
	mask = Virtex5::eFarMaskBlockType + Virtex5::eFarMaskTopBottom + Virtex5::eFarMaskRow 
		+ Virtex5::eFarMaskMajor + Virtex5::eFarMaskMinor;
	BOOST_CHECK_EQUAL(mask, 0x00FFFFFFu);

	// members tested:
	//		Virtex5::sPacketTypeName and EPacketTypeName
	BOOST_CHECK_EQUAL(Virtex5::sPacketTypeName[0],							"[UNKNOWN TYPE 0]");
	BOOST_CHECK_EQUAL(Virtex5::sPacketTypeName[Virtex5::ePacketType1],		"TYPE1");
	BOOST_CHECK_EQUAL(Virtex5::sPacketTypeName[Virtex5::ePacketType2],		"TYPE2");
	BOOST_CHECK_EQUAL(Virtex5::sPacketTypeName[3],							"[UNKNOWN TYPE 3]");
	BOOST_CHECK_EQUAL(Virtex5::sPacketTypeName[4],							"[UNKNOWN TYPE 4]");
	BOOST_CHECK_EQUAL(Virtex5::sPacketTypeName[5],							"[UNKNOWN TYPE 5]");
	BOOST_CHECK_EQUAL(Virtex5::sPacketTypeName[6],							"[UNKNOWN TYPE 6]");
	BOOST_CHECK_EQUAL(Virtex5::sPacketTypeName[7],							"[UNKNOWN TYPE 7]");

	// members tested:
	//		Virtex5::sOpcodeName and EOpcode
	BOOST_CHECK_EQUAL(Virtex5::sOpcodeName[Virtex5::eOpcodeNOP],			"NOP");
	BOOST_CHECK_EQUAL(Virtex5::sOpcodeName[Virtex5::eOpcodeRead],			"READ");
	BOOST_CHECK_EQUAL(Virtex5::sOpcodeName[Virtex5::eOpcodeWrite],			"WRITE");
	BOOST_CHECK_EQUAL(Virtex5::sOpcodeName[Virtex5::eOpcodeReserved],		"RESERVED");

	// members tested:
	//		Virtex5::sRegisterName and ERegister
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterCRC],		"CRC");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterFAR],		"FAR");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterFDRI],		"FDRI");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterFDRO],		"FDRO");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterCMD],		"CMD");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterCTL0],		"CTL0");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterMASK],		"MASK");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterSTAT],		"STAT");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterLOUT],		"LOUT");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterCOR0],		"COR0");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterMFWR],		"MFWR");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterCBC],		"CBC");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterIDCODE],		"IDCODE");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterAXSS],		"AXSS");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterCOR1],		"COR1");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterCSOB],		"CSOB");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterWBSTAR],		"WBSTAR");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterTIMER],		"TIMER");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterBOOTSTS],	"BOOTSTS");
	BOOST_CHECK_EQUAL(Virtex5::sRegisterName[Virtex5::eRegisterCTL1],		"CTL1");

	// members tested:
	//		Virtex5::sCommandName and ECommand
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandNULL],			"NULL");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandWCFG],			"WCFG");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandMFW],			"MFW");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandLFRM],			"DGHIGH/LFRM");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandRCFG],			"RCFG");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandSTART],		"START");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandRCAP],			"RCAP");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandRCRC],			"RCRC");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandAGHIGH],		"AGHIGH");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandSWITCH],		"SWITCH");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandGRESTORE],		"GRESTORE");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandSHUTDOWN],		"SHUTDOWN");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandGCAPTURE],		"GCAPTURE");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandDESYNCH],		"DESYNCH");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandReserved],		"Reserved");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandIPROG],		"IPROG");
	BOOST_CHECK_EQUAL(Virtex5::sCommandName[Virtex5::eCommandLTIMER],		"LTIMER");

	uint32_t u1 = 0xffffffff & mask;
	BOOST_CHECK_EQUAL(uint32_t(Virtex5::FrameAddress(u1)), u1);
	uint32_t u2 = 0xffff0000 & mask;
	BOOST_CHECK_EQUAL(uint32_t(Virtex5::FrameAddress(u2)), u2);
	uint32_t u3 = 0xff00ff00 & mask;
	BOOST_CHECK_EQUAL(uint32_t(Virtex5::FrameAddress(u3)), u3);
	uint32_t u4 = 0xf0f0f0f0 & mask;
	BOOST_CHECK_EQUAL(uint32_t(Virtex5::FrameAddress(u4)), u4);
	uint32_t u5 = 0xcccccccc & mask;
	BOOST_CHECK_EQUAL(uint32_t(Virtex5::FrameAddress(u5)), u5);
	uint32_t u6 = 0xaaaaaaaa & mask;
	BOOST_CHECK_EQUAL(uint32_t(Virtex5::FrameAddress(u6)), u6);

	// build the file paths
	boost::filesystem::path referencePath = torc::common::DirectoryTree::getExecutablePath()
		/ "torc" / "bitstream" / "Virtex5UnitTest.reference.bit";
	boost::filesystem::path generatedPath = torc::common::DirectoryTree::getExecutablePath()
		/ "regression" / "Virtex5UnitTest.generated.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	Virtex5 bitstream;
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





































void testVirtex5Device(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath);

/// \brief Unit test for the Virtex5 class Frame Address Register mapping.
BOOST_AUTO_TEST_CASE(Virtex5FarUnitTest) {

	// look up the command line arguments
	int& argc = boost::unit_test::framework::master_test_suite().argc;
	char**& argv = boost::unit_test::framework::master_test_suite().argv;
	// make sure that we at least have the name under which we were invoked
	BOOST_REQUIRE(argc >= 1);
	// resolve symbolic links if applicable
	torc::common::DirectoryTree directoryTree(argv[0]);

	// iterate over the devices
	const torc::common::DeviceVector& devices = torc::common::Devices::getVirtex5Devices();
	torc::common::DeviceVector::const_iterator dp = devices.begin();
	torc::common::DeviceVector::const_iterator de = devices.end();
	while(dp < de) {
		const std::string& device = *dp++;
		if(device.empty()) break;
//std::cout << "device " << ": " << device << std::endl;
		testVirtex5Device(device, torc::common::DirectoryTree::getWorkingPath());
	}
}


void testVirtex5Device(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath) {

	// build the file paths
	boost::filesystem::path debugBitstreamPath = inWorkingPath / "torc" / "bitstream" / "regression";
	//boost::filesystem::path generatedPath = debugBitstreamPath / (inDeviceName + ".debug.bit");
	boost::filesystem::path referencePath = debugBitstreamPath / (inDeviceName + ".debug.bit");
std::cerr << "TRYING TO FIND " << referencePath << std::endl;

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	std::cerr << "Trying to read: " << referencePath << std::endl;
	BOOST_REQUIRE(fileStream.good());
	Virtex5 bitstream;
std::cerr << "test0" << std::endl;
	bitstream.read(fileStream, false);
std::cerr << "test1" << std::endl;
	// write the bitstream digest to the console
//	std::cout << bitstream << std::endl;

//	// initialize the bitstream frame maps
//	boost::filesystem::path deviceColumnsPath = inWorkingPath / "torc" / "bitstream" / "regression" 
//		/ (inDeviceName + ".cpp");
//	std::fstream deviceColumnsStream(deviceColumnsPath.string().c_str(), std::ios::out);
	bitstream.initializeDeviceInfo(inDeviceName);
std::cerr << "test2" << std::endl;
	bitstream.initializeFrameMaps();
std::cerr << "test3" << std::endl;

	// iterate through the packets, and extract all of the FARs
	Virtex5::FrameAddressToIndex farRemaining = bitstream.mFrameAddressToIndex;
	Virtex5::FrameAddressToIndex farVisited;
	{
		bool first = true;
		Virtex5::const_iterator p = bitstream.begin();
		Virtex5::const_iterator e = bitstream.end();
		uint32_t header = VirtexPacket::makeHeader(VirtexPacket::ePacketType1, 
			VirtexPacket::eOpcodeWrite, Virtex5::eRegisterLOUT, 1);
		while(p < e) {
			const VirtexPacket& packet = *p++;
			if(packet.getHeader() != header) continue;
			if(first) { first = false; continue; }
			Virtex5::FrameAddress far = packet[1];
			farVisited[far] = 0;
			Virtex5::FrameAddressToIndex::iterator found = farRemaining.find(far);
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

	// iterate through the debug bitstream packets, and extract all of the FARs
	// this isn't currently being used, but it may come in handy for debugging
	for(int half = 0; half < 2; half++) {
		for(uint32_t row = 0; row < 2; row++) {
			typedef std::map<uint32_t, uint32_t> ColumnMaxFrame;
			ColumnMaxFrame maxFrames[Virtex5::eFarBlockTypeCount];
			Virtex5::const_iterator p = bitstream.begin();
			Virtex5::const_iterator e = bitstream.end();
			uint32_t header = VirtexPacket::makeHeader(VirtexPacket::ePacketType1, 
				VirtexPacket::eOpcodeWrite, Virtex5::eRegisterLOUT, 1);
			while(p < e) {
				const VirtexPacket& packet = *p++;
				if(packet.getHeader() != header) continue;
				Virtex5::FrameAddress far = packet[1];
//				uint32_t far = packet[1];
//				std::cerr << Hex32(far) << " ";
				if(far.mTopBottom == half && far.mRow == row) {
//					std::cerr << far << " ";
					ColumnMaxFrame::iterator i = maxFrames[far.mBlockType].find(far.mMajor);
					if(i == maxFrames[far.mBlockType].end()) {
						maxFrames[far.mBlockType][far.mMajor] = 0;
					} else {
						if(maxFrames[far.mBlockType][far.mMajor] < far.mMinor) 
							maxFrames[far.mBlockType][far.mMajor] = far.mMinor;
					}
				}
			}
			std::cerr << std::endl;
			uint32_t frameCount = 0;
			for(uint32_t i = 0; i < Virtex5::eFarBlockTypeCount; i++) {
				Virtex5::EFarBlockType blockType = Virtex5::EFarBlockType(i);
				uint32_t majorCount = maxFrames[blockType].size();
				for(uint32_t major = 0; major < majorCount; major++) {
					frameCount += maxFrames[blockType][major] + 1;
					std::cerr << blockType << "(" << major << "): " 
						<< (maxFrames[blockType][major] + 1) << " (" << frameCount << ")" 
						<< std::endl;
				}
			}
		}
	}

}

void testVirtex5FullMapping(const boost::filesystem::path& inWorkingPath);
void testVirtex5PartialMapping(const boost::filesystem::path& inWorkingPath);

/// \brief Unit test for the Virtex5 bitstream to bitmap conversion.
BOOST_AUTO_TEST_CASE(Virtex5MapUnitTest) {
	// look up the command line arguments
	int& argc = boost::unit_test::framework::master_test_suite().argc;
	char**& argv = boost::unit_test::framework::master_test_suite().argv;
	// make sure that we at least have the name under which we were invoked
	BOOST_REQUIRE(argc >= 1);
	// resolve symbolic links if applicable
	torc::common::DirectoryTree directoryTree(argv[0]);
	testVirtex5FullMapping(torc::common::DirectoryTree::getWorkingPath());
	//testVirtex5PartialMapping(torc::common::DirectoryTree::getWorkingPath());
}


void testVirtex5FullMapping(const boost::filesystem::path& inWorkingPath) {
	// build the file paths
	boost::filesystem::path referencePath = torc::common::DirectoryTree::getExecutablePath()
		/ "torc" / "bitstream" / "Virtex5UnitTest.reference.bit";
	boost::filesystem::path generatedPath = torc::common::DirectoryTree::getExecutablePath()
		/ "regression" / "Virtex5MapUnitTest.generated.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	// read and gather bitstream frames
	Virtex5 bitstream;
	bitstream.read(fileStream, false);

	// initialize frame map
	bitstream.initializeDeviceInfo("xc5vlx20t");
	bitstream.initializeFrameMaps();

	// load bitstream frames in data structure
	bitstream.readFramePackets();

	// write full bitstream from frame blocks data structure
	uint32_t frameLength = bitstream.getFrameLength();
	typedef boost::shared_array<uint32_t> WordSharedArray;
	Virtex5::iterator p = bitstream.begin();
	Virtex5::iterator e = bitstream.end();
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

/*
void testVirtex5PartialMapping(const boost::filesystem::path& inWorkingPath) {
	// build the file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "Virtex5UnitTest.generatedPartial.bit";
	boost::filesystem::path referencePath = regressionPath / "Virtex5UnitTest.reference.bit";

	// read the bitstream
	std::fstream fileStream("partial.bit", std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	// read and gather bitstream frames
	Virtex5 bitstream;
	bitstream.read(fileStream, false);

	// initialize frame map
	bitstream.initializeDeviceInfo("xc5vlx110t");
	bitstream.initializeFrameMaps();

	// load bitstream frames in data structure
	bitstream.initializePartialFrameBlocks();

	uint32_t frameLength = bitstream.getFrameLength();
	typedef boost::shared_array<uint32_t> WordSharedArray;
	Virtex5::iterator p = bitstream.begin();
	Virtex5::iterator e = bitstream.end();
	while(p < e) {
		const VirtexPacket& packet = *p++;
		if(packet.isType2()) {
			WordSharedArray words = packet.getWords();
			uint32_t* ptr = words.get();
		}
	}
	// write the test bitstream back out
	std::fstream outputStream(generatedPath.string().c_str(), std::ios::binary | std::ios::out);
	BOOST_REQUIRE(outputStream.good());
	bitstream.write(outputStream);
	outputStream.flush();

	return;
}
*/

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
