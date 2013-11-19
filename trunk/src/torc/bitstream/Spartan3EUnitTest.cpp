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
/// \brief Unit test for the Spartan3E class.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/Spartan3E.hpp"
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


/// \brief Unit test for the Spartan3E class.
BOOST_AUTO_TEST_CASE(Spartan3EUnitTest) {

	// enums tested:
	//		EPacket
	//		EFar
	boost::uint32_t mask;
	// type 1 packet subfield masks
	mask = Spartan3E::ePacketMaskType + Spartan3E::ePacketMaskOpcode 
		+ Spartan3E::ePacketMaskType1Address + Spartan3E::ePacketMaskType1Reserved 
		+ Spartan3E::ePacketMaskType1Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// type 2 packet subfield masks
	mask = Spartan3E::ePacketMaskType + Spartan3E::ePacketMaskOpcode 
		+ Spartan3E::ePacketMaskType2Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// frame address register subfield masks
	mask = Spartan3E::eFarMaskColumn + Spartan3E::eFarMaskMajor + Spartan3E::eFarMaskMinor;
	BOOST_CHECK_EQUAL(mask, 0x07FFFE00u);

	// members tested:
	//		Spartan3E::sPacketTypeName and EPacketTypeName
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[0],							"[UNKNOWN TYPE 0]");
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[Spartan3E::ePacketType1],		"TYPE1");
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[Spartan3E::ePacketType2],		"TYPE2");
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[3],							"[UNKNOWN TYPE 3]");
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[4],							"[UNKNOWN TYPE 4]");
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[5],							"[UNKNOWN TYPE 5]");
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[6],							"[UNKNOWN TYPE 6]");
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[7],							"[UNKNOWN TYPE 7]");

	// members tested:
	//		Spartan3E::sOpcodeName and EOpcode
	BOOST_CHECK_EQUAL(Spartan3E::sOpcodeName[Spartan3E::eOpcodeNOP],			"NOP");
	BOOST_CHECK_EQUAL(Spartan3E::sOpcodeName[Spartan3E::eOpcodeRead],			"READ");
	BOOST_CHECK_EQUAL(Spartan3E::sOpcodeName[Spartan3E::eOpcodeWrite],			"WRITE");
	BOOST_CHECK_EQUAL(Spartan3E::sOpcodeName[Spartan3E::eOpcodeReserved],		"RESERVED");

	// members tested:
	//		Spartan3E::sRegisterName and ERegister
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterCRC],		"CRC");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterFAR],		"FAR");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterFDRI],		"FDRI");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterFDRO],		"FDRO");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterCMD],		"CMD");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterCTL],		"CTL");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterMASK],		"MASK");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterSTAT],		"STAT");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterLOUT],		"LOUT");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterCOR],		"COR");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterMFWR],		"MFWR");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterFLR],		"FLR");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterIDCODE],		"IDCODE");

	// members tested:
	//		Spartan3E::sCommandName and ECommand
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandWCFG],			"WCFG");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandMFWR],			"MFWR");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandLFRM],			"LFRM");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandRCFG],			"RCFG");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandSTART],		"START");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandRCAP],			"RCAP");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandRCRC],			"RCRC");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandAGHIGH],		"AGHIGH");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandSWITCH],		"SWITCH");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandGRESTORE],		"GRESTORE");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandSHUTDOWN],		"SHUTDOWN");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandGCAPTURE],		"GCAPTURE");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandDESYNCH],		"DESYNCH");

	// build the file paths
	boost::filesystem::path referencePath = torc::common::DirectoryTree::getExecutablePath()
		/ "torc" / "bitstream" / "Spartan3EUnitTest.reference.bit";
	boost::filesystem::path generatedPath = torc::common::DirectoryTree::getExecutablePath()
		/ "regression" / "Spartan3EUnitTest.generated.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	Spartan3E bitstream;
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







































void testSpartan3EDevice(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath);

/// \brief Unit test for the Spartan3E class Frame Address Register mapping.
BOOST_AUTO_TEST_CASE(Spartan3EFarUnitTest) {

	// look up the command line arguments
	int& argc = boost::unit_test::framework::master_test_suite().argc;
	char**& argv = boost::unit_test::framework::master_test_suite().argv;
	// make sure that we at least have the name under which we were invoked
	BOOST_REQUIRE(argc >= 1);
	// resolve symbolic links if applicable
	torc::common::DirectoryTree directoryTree(argv[0]);

	// iterate over the devices
	const torc::common::DeviceVector& devices = torc::common::Devices::getSpartan3EDevices();
	torc::common::DeviceVector::const_iterator dp = devices.begin();
	torc::common::DeviceVector::const_iterator de = devices.end();
	while(dp < de) {
		const std::string& device = *dp++;
		if(device.empty()) break;
//std::cout << "device " << ": " << device << std::endl;
		testSpartan3EDevice(device, torc::common::DirectoryTree::getWorkingPath());
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

void testSpartan3EDevice(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath) {

	// build the file paths
	boost::filesystem::path debugBitstreamPath = inWorkingPath / "torc" / "bitstream" / "regression";
	//boost::filesystem::path generatedPath = debugBitstreamPath / (inDeviceName + ".debug.bit");
	boost::filesystem::path referencePath = debugBitstreamPath / (inDeviceName + ".debug.bit");
std::cerr << "TRYING TO FIND " << referencePath << std::endl;

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	std::cerr << "Trying to read: " << referencePath << std::endl;
	BOOST_REQUIRE(fileStream.good());
	Spartan3E bitstream;
	bitstream.read(fileStream, false);
	// write the bitstream digest to the console
	//std::cout << bitstream << std::endl;

//	// initialize the bitstream frame maps
//	boost::filesystem::path deviceColumnsPath = inWorkingPath / "regression" 
//		/ (inDeviceName + ".cpp");
//	std::fstream deviceColumnsStream(deviceColumnsPath.string().c_str(), std::ios::out);
	bitstream.initializeDeviceInfo(inDeviceName);
	bitstream.initializeFrameMaps();

	// iterate through the packets, and extract all of the FARs
	Spartan3E::FrameAddressToIndex farRemaining = bitstream.mFrameAddressToIndex;
	Spartan3E::FrameAddressToIndex farVisited;
	{
		bool first = true;
		Spartan3E::const_iterator p = bitstream.begin();
		Spartan3E::const_iterator e = bitstream.end();
		uint32_t header = SpartanPacket::makeHeader(SpartanPacket::ePacketType1, 
			SpartanPacket::eOpcodeWrite, Spartan3E::eRegisterLOUT, 1);
		while(p < e) {
			const SpartanPacket& packet = *p++;
			if(packet.getHeader() != header) continue;
			if(first) { first = false; continue; }
			Spartan3E::FrameAddress far = packet[1];
			//std::cout << std::endl << "Debug Far Address: " << Hex32(packet[1]) << std::endl;
			farVisited[far] = 0;
			Spartan3E::FrameAddressToIndex::iterator found = farRemaining.find(far);
			if(found != farRemaining.end()) {
				farRemaining.erase(found);
			} else {
				std::cerr << "missing " << far << " ";
			}
		}
	}
	{
		std::cerr << std::endl;
		Spartan3E::FrameAddressToIndex::const_iterator p = farRemaining.begin();
		Spartan3E::FrameAddressToIndex::const_iterator e = farRemaining.end();
		while(p != e) {
			std::cerr << "remaining " << p->first << " ";
			p++;
		}
		std::cerr << std::endl;
	}
	// verify that we have visited all of the expected FARs and no others
	std::cout << "Device: " << inDeviceName << std::endl;
	std::cout << "Size of farRemaining: " << farRemaining.size() << std::endl;
	std::cout << "Size of farVisited: " << farVisited.size() << std::endl;
	BOOST_REQUIRE_EQUAL(bitstream.mFrameAddressToIndex.size(), farVisited.size());
	BOOST_REQUIRE_EQUAL(farRemaining.size(), 0u);

return;
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
