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
/// \brief Unit test for the Virtex2P class.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/Virtex2P.hpp"
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

/// \brief Unit test for the Virtex2 CRC
BOOST_AUTO_TEST_CASE(Virtex2PCrcUnitTest) {
	std::fstream fileStream("Virtex2PUnitTest.reference.bit", std::ios::binary | std::ios::in);
	Virtex2P bitstream;
	bitstream.read(fileStream, false);
	std::cout << bitstream << std::endl;
	bitstream.preflightPackets();
	BOOST_REQUIRE(true);
}

/// \brief Unit test for the Virtex2p class.
BOOST_AUTO_TEST_CASE(Virtex2PUnitTest) {

	// enums tested:
	//		EPacket
	//		EFar
	boost::uint32_t mask;
	// type 1 packet subfield masks
	mask = Virtex2P::ePacketMaskType + Virtex2P::ePacketMaskOpcode 
		+ Virtex2P::ePacketMaskType1Address + Virtex2P::ePacketMaskType1Reserved 
		+ Virtex2P::ePacketMaskType1Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// type 2 packet subfield masks
	mask = Virtex2P::ePacketMaskType + Virtex2P::ePacketMaskOpcode 
		+ Virtex2P::ePacketMaskType2Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// frame address register subfield masks
	mask = Virtex2::eFarMaskBlockType + Virtex2::eFarMaskMajor + Virtex2::eFarMaskMinor;
	BOOST_CHECK_EQUAL(mask, 0x07FFFE00u);

	// members tested:
	//		Virtex2P::sPacketTypeName and EPacketTypeName
	BOOST_CHECK_EQUAL(Virtex2P::sPacketTypeName[0],							"[UNKNOWN TYPE 0]");
	BOOST_CHECK_EQUAL(Virtex2P::sPacketTypeName[Virtex2P::ePacketType1],		"TYPE1");
	BOOST_CHECK_EQUAL(Virtex2P::sPacketTypeName[Virtex2P::ePacketType2],		"TYPE2");
	BOOST_CHECK_EQUAL(Virtex2P::sPacketTypeName[3],							"[UNKNOWN TYPE 3]");
	BOOST_CHECK_EQUAL(Virtex2P::sPacketTypeName[4],							"[UNKNOWN TYPE 4]");
	BOOST_CHECK_EQUAL(Virtex2P::sPacketTypeName[5],							"[UNKNOWN TYPE 5]");
	BOOST_CHECK_EQUAL(Virtex2P::sPacketTypeName[6],							"[UNKNOWN TYPE 6]");
	BOOST_CHECK_EQUAL(Virtex2P::sPacketTypeName[7],							"[UNKNOWN TYPE 7]");

	// members tested:
	//		Virtex2P::sOpcodeName and EOpcode
	BOOST_CHECK_EQUAL(Virtex2P::sOpcodeName[Virtex2P::eOpcodeNOP],			"NOP");
	BOOST_CHECK_EQUAL(Virtex2P::sOpcodeName[Virtex2P::eOpcodeRead],			"READ");
	BOOST_CHECK_EQUAL(Virtex2P::sOpcodeName[Virtex2P::eOpcodeWrite],			"WRITE");
	BOOST_CHECK_EQUAL(Virtex2P::sOpcodeName[Virtex2P::eOpcodeReserved],		"RESERVED");

	// members tested:
	//		Virtex2P::sRegisterName and ERegister
	BOOST_CHECK_EQUAL(Virtex2P::sRegisterName[Virtex2P::eRegisterCRC],		"CRC");
	BOOST_CHECK_EQUAL(Virtex2P::sRegisterName[Virtex2P::eRegisterFAR],		"FAR");
	BOOST_CHECK_EQUAL(Virtex2P::sRegisterName[Virtex2P::eRegisterFDRI],		"FDRI");
	BOOST_CHECK_EQUAL(Virtex2P::sRegisterName[Virtex2P::eRegisterFDRO],		"FDRO");
	BOOST_CHECK_EQUAL(Virtex2P::sRegisterName[Virtex2P::eRegisterCMD],		"CMD");
	BOOST_CHECK_EQUAL(Virtex2P::sRegisterName[Virtex2P::eRegisterCTL],		"CTL");
	BOOST_CHECK_EQUAL(Virtex2P::sRegisterName[Virtex2P::eRegisterMASK],		"MASK");
	BOOST_CHECK_EQUAL(Virtex2P::sRegisterName[Virtex2P::eRegisterSTAT],		"STAT");
	BOOST_CHECK_EQUAL(Virtex2P::sRegisterName[Virtex2P::eRegisterLOUT],		"LOUT");
	BOOST_CHECK_EQUAL(Virtex2P::sRegisterName[Virtex2P::eRegisterCOR],		"COR");
	BOOST_CHECK_EQUAL(Virtex2P::sRegisterName[Virtex2P::eRegisterMFWR],		"MFWR");
	BOOST_CHECK_EQUAL(Virtex2P::sRegisterName[Virtex2P::eRegisterFLR],		"FLR");
	BOOST_CHECK_EQUAL(Virtex2P::sRegisterName[Virtex2P::eRegisterKEY],		"KEY");
	BOOST_CHECK_EQUAL(Virtex2P::sRegisterName[Virtex2P::eRegisterCBC],		"CBC");
	BOOST_CHECK_EQUAL(Virtex2P::sRegisterName[Virtex2P::eRegisterIDCODE],		"IDCODE");

	// members tested:
	//		Virtex2P::sCommandName and ECommand
	BOOST_CHECK_EQUAL(Virtex2P::sCommandName[Virtex2P::eCommandWCFG],			"WCFG");
	BOOST_CHECK_EQUAL(Virtex2P::sCommandName[Virtex2P::eCommandMFWR],			"MFWR");
	BOOST_CHECK_EQUAL(Virtex2P::sCommandName[Virtex2P::eCommandLFRM],			"LFRM");
	BOOST_CHECK_EQUAL(Virtex2P::sCommandName[Virtex2P::eCommandRCFG],			"RCFG");
	BOOST_CHECK_EQUAL(Virtex2P::sCommandName[Virtex2P::eCommandSTART],		"START");
	BOOST_CHECK_EQUAL(Virtex2P::sCommandName[Virtex2P::eCommandRCAP],			"RCAP");
	BOOST_CHECK_EQUAL(Virtex2P::sCommandName[Virtex2P::eCommandRCRC],			"RCRC");
	BOOST_CHECK_EQUAL(Virtex2P::sCommandName[Virtex2P::eCommandAGHIGH],		"AGHIGH");
	BOOST_CHECK_EQUAL(Virtex2P::sCommandName[Virtex2P::eCommandSWITCH],		"SWITCH");
	BOOST_CHECK_EQUAL(Virtex2P::sCommandName[Virtex2P::eCommandGRESTORE],		"GRESTORE");
	BOOST_CHECK_EQUAL(Virtex2P::sCommandName[Virtex2P::eCommandSHUTDOWN],		"SHUTDOWN");
	BOOST_CHECK_EQUAL(Virtex2P::sCommandName[Virtex2P::eCommandGCAPTURE],		"GCAPTURE");
	BOOST_CHECK_EQUAL(Virtex2P::sCommandName[Virtex2P::eCommandDESYNCH],		"DESYNCH");

	// build the file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "Virtex2PUnitTest.generated.bit";
	boost::filesystem::path referencePath = regressionPath / "Virtex2PUnitTest.reference.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	Virtex2P bitstream;
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








































void testVirtex2PDevice(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath);

/// \brief Unit test for the Virtex2P class Frame Address Register mapping.
BOOST_AUTO_TEST_CASE(Virtex2PFarUnitTest) {

	// look up the command line arguments
	int& argc = boost::unit_test::framework::master_test_suite().argc;
	char**& argv = boost::unit_test::framework::master_test_suite().argv;
	// make sure that we at least have the name under which we were invoked
	BOOST_REQUIRE(argc >= 1);
	// resolve symbolic links if applicable
	torc::common::DirectoryTree directoryTree(argv[0]);

	// iterate over the devices
	const torc::common::DeviceVector& devices = torc::common::Devices::getVirtex2PDevices();
	torc::common::DeviceVector::const_iterator dp = devices.begin();
	torc::common::DeviceVector::const_iterator de = devices.end();
	while(dp < de) {
		const std::string& device = *dp++;
		if(device.empty()) break;
//std::cout << "device " << ": " << device << std::endl;
		testVirtex2PDevice(device, torc::common::DirectoryTree::getWorkingPath());
	}
}

void testVirtex2PDevice(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath) {

	// build the file paths
	boost::filesystem::path debugBitstreamPath = inWorkingPath / "torc" / "bitstream" / "regression";
	//boost::filesystem::path generatedPath = debugBitstreamPath / (inDeviceName + ".debug.bit");
	boost::filesystem::path referencePath = debugBitstreamPath / (inDeviceName + ".debug.bit");
std::cerr << "TRYING TO FIND " << referencePath << std::endl;

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	std::cerr << "Trying to read: " << referencePath << std::endl;
	BOOST_REQUIRE(fileStream.good());
	Virtex2P bitstream;
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
	Virtex2P::FrameAddressToIndex farRemaining = bitstream.mFrameAddressToIndex;
	Virtex2P::FrameAddressToIndex farVisited;
	{
		bool first = true;
		Virtex2P::const_iterator p = bitstream.begin();
		Virtex2P::const_iterator e = bitstream.end();
		uint32_t header = VirtexPacket::makeHeader(VirtexPacket::ePacketType1, 
			VirtexPacket::eOpcodeWrite, Virtex2P::eRegisterLOUT, 1);
		while(p < e) {
			const VirtexPacket& packet = *p++;
			if(packet.getHeader() != header) continue;
			if(first) { first = false; continue; }
			Virtex2P::FrameAddress far = packet[1];
			//std::cout << std::endl << "Debug Far Address: " << Hex32(packet[1]) << std::endl;
			farVisited[far] = 0;
			Virtex2P::FrameAddressToIndex::iterator found = farRemaining.find(far);
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

void testVirtex2PFullMapping(const boost::filesystem::path& inWorkingPath);

/// \brief Unit test for the Virtex2P bitstream to bitmap conversion.
BOOST_AUTO_TEST_CASE(Virtex2PMapUnitTest) {
	// look up the command line arguments
	int& argc = boost::unit_test::framework::master_test_suite().argc;
	char**& argv = boost::unit_test::framework::master_test_suite().argv;
	// make sure that we at least have the name under which we were invoked
	BOOST_REQUIRE(argc >= 1);
	// resolve symbolic links if applicable
	torc::common::DirectoryTree directoryTree(argv[0]);
	testVirtex2PFullMapping(torc::common::DirectoryTree::getWorkingPath());
}


void testVirtex2PFullMapping(const boost::filesystem::path& inWorkingPath) {
	// build the file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "Virtex2PUnitTest.generatedFull.bit";
	boost::filesystem::path referencePath = regressionPath / "Virtex2PUnitTest.reference.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	// read and gather bitstream frames
	Virtex2P bitstream;
	bitstream.read(fileStream, false);

	// initialize frame map
	bitstream.initializeDeviceInfo("xc2vp20");
	bitstream.initializeFrameMaps();

	// load bitstream frames in data structure
	bitstream.initializeFullFrameBlocks();

	// write full bitstream from frame blocks data structure
	uint32_t frameLength = bitstream.getFrameLength();
	typedef boost::shared_array<uint32_t> WordSharedArray;
	Virtex2P::iterator p = bitstream.begin();
	Virtex2P::iterator e = bitstream.end();
	while (p < e) {
		const VirtexPacket& packet = *p++;
		if (packet.isType2()) {
			WordSharedArray words = packet.getWords();
			uint32_t* ptr = words.get();
			for (uint32_t block = 0; block < 8; block++) {
				for (uint32_t frame = 0; frame < bitstream.mBlockFrameIndexBounds[block]; frame++) {
					VirtexFrameBlocks::word_t* words = const_cast<VirtexFrameBlocks::word_t*>(bitstream.mFrameBlocks.mBlock[block][frame]->getWords());
					for (uint32_t index = 0; index < frameLength; index++) {
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
/// \brief Unit test for the Virtex2P static device info generation.
BOOST_AUTO_TEST_CASE(Virtex2PGenerateUnitTest) {

	Virtex2P bitstream;
	DeviceInfoHelper::buildFamilyDeviceInfo("Virtex2P", "Virtex2PDeviceInfo.template", 
		"Virtex2PDeviceInfo.cpp", torc::common::Devices::getVirtex2PDevices(), bitstream);

}
*/

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
