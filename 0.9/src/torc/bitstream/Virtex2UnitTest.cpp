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
/// \brief Source for the Virtex2 unit test.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/Virtex2.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/common/Devices.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/DeviceDesignator.hpp"
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
BOOST_AUTO_TEST_CASE(crc_virtex2) {
	std::fstream fileStream("Virtex2UnitTest.reference.bit", std::ios::binary | std::ios::in);
	Virtex2 bitstream;
	bitstream.read(fileStream, false);
	std::cout << bitstream << std::endl;
	bitstream.preflightPackets();
	BOOST_REQUIRE(true);
}

/// \brief Unit test for the Virtex2 class.
BOOST_AUTO_TEST_CASE(bitstream_virtex2) {

	// enums tested:
	//		EPacket
	//		EFar
	boost::uint32_t mask;
	// type 1 packet subfield masks
	mask = Virtex2::ePacketMaskType + Virtex2::ePacketMaskOpcode 
		+ Virtex2::ePacketMaskType1Address + Virtex2::ePacketMaskType1Reserved 
		+ Virtex2::ePacketMaskType1Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// type 2 packet subfield masks
	mask = Virtex2::ePacketMaskType + Virtex2::ePacketMaskOpcode 
		+ Virtex2::ePacketMaskType2Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// frame address register subfield masks
	mask = Virtex2::eFarMaskBlockType + Virtex2::eFarMaskMajor + Virtex2::eFarMaskMinor;
	BOOST_CHECK_EQUAL(mask, 0x07FFFE00u);

	// members tested:
	//		Virtex2::sPacketTypeName and EPacketTypeName
	BOOST_CHECK_EQUAL(Virtex2::sPacketTypeName[0],							"[UNKNOWN TYPE 0]");
	BOOST_CHECK_EQUAL(Virtex2::sPacketTypeName[Virtex2::ePacketType1],		"TYPE1");
	BOOST_CHECK_EQUAL(Virtex2::sPacketTypeName[Virtex2::ePacketType2],		"TYPE2");
	BOOST_CHECK_EQUAL(Virtex2::sPacketTypeName[3],							"[UNKNOWN TYPE 3]");
	BOOST_CHECK_EQUAL(Virtex2::sPacketTypeName[4],							"[UNKNOWN TYPE 4]");
	BOOST_CHECK_EQUAL(Virtex2::sPacketTypeName[5],							"[UNKNOWN TYPE 5]");
	BOOST_CHECK_EQUAL(Virtex2::sPacketTypeName[6],							"[UNKNOWN TYPE 6]");
	BOOST_CHECK_EQUAL(Virtex2::sPacketTypeName[7],							"[UNKNOWN TYPE 7]");

	// members tested:
	//		Virtex2::sOpcodeName and EOpcode
	BOOST_CHECK_EQUAL(Virtex2::sOpcodeName[Virtex2::eOpcodeNOP],			"NOP");
	BOOST_CHECK_EQUAL(Virtex2::sOpcodeName[Virtex2::eOpcodeRead],			"READ");
	BOOST_CHECK_EQUAL(Virtex2::sOpcodeName[Virtex2::eOpcodeWrite],			"WRITE");
	BOOST_CHECK_EQUAL(Virtex2::sOpcodeName[Virtex2::eOpcodeReserved],		"RESERVED");

	// members tested:
	//		Virtex2::sRegisterName and ERegister
	BOOST_CHECK_EQUAL(Virtex2::sRegisterName[Virtex2::eRegisterCRC],		"CRC");
	BOOST_CHECK_EQUAL(Virtex2::sRegisterName[Virtex2::eRegisterFAR],		"FAR");
	BOOST_CHECK_EQUAL(Virtex2::sRegisterName[Virtex2::eRegisterFDRI],		"FDRI");
	BOOST_CHECK_EQUAL(Virtex2::sRegisterName[Virtex2::eRegisterFDRO],		"FDRO");
	BOOST_CHECK_EQUAL(Virtex2::sRegisterName[Virtex2::eRegisterCMD],		"CMD");
	BOOST_CHECK_EQUAL(Virtex2::sRegisterName[Virtex2::eRegisterCTL],		"CTL");
	BOOST_CHECK_EQUAL(Virtex2::sRegisterName[Virtex2::eRegisterMASK],		"MASK");
	BOOST_CHECK_EQUAL(Virtex2::sRegisterName[Virtex2::eRegisterSTAT],		"STAT");
	BOOST_CHECK_EQUAL(Virtex2::sRegisterName[Virtex2::eRegisterLOUT],		"LOUT");
	BOOST_CHECK_EQUAL(Virtex2::sRegisterName[Virtex2::eRegisterCOR],		"COR");
	BOOST_CHECK_EQUAL(Virtex2::sRegisterName[Virtex2::eRegisterMFWR],		"MFWR");
	BOOST_CHECK_EQUAL(Virtex2::sRegisterName[Virtex2::eRegisterFLR],		"FLR");
	BOOST_CHECK_EQUAL(Virtex2::sRegisterName[Virtex2::eRegisterKEY],		"KEY");
	BOOST_CHECK_EQUAL(Virtex2::sRegisterName[Virtex2::eRegisterCBC],		"CBC");
	BOOST_CHECK_EQUAL(Virtex2::sRegisterName[Virtex2::eRegisterIDCODE],		"IDCODE");

	// members tested:
	//		Virtex2::sCommandName and ECommand
	BOOST_CHECK_EQUAL(Virtex2::sCommandName[Virtex2::eCommandWCFG],			"WCFG");
	BOOST_CHECK_EQUAL(Virtex2::sCommandName[Virtex2::eCommandMFWR],			"MFWR");
	BOOST_CHECK_EQUAL(Virtex2::sCommandName[Virtex2::eCommandLFRM],			"LFRM");
	BOOST_CHECK_EQUAL(Virtex2::sCommandName[Virtex2::eCommandRCFG],			"RCFG");
	BOOST_CHECK_EQUAL(Virtex2::sCommandName[Virtex2::eCommandSTART],		"START");
	BOOST_CHECK_EQUAL(Virtex2::sCommandName[Virtex2::eCommandRCAP],			"RCAP");
	BOOST_CHECK_EQUAL(Virtex2::sCommandName[Virtex2::eCommandRCRC],			"RCRC");
	BOOST_CHECK_EQUAL(Virtex2::sCommandName[Virtex2::eCommandAGHIGH],		"AGHIGH");
	BOOST_CHECK_EQUAL(Virtex2::sCommandName[Virtex2::eCommandSWITCH],		"SWITCH");
	BOOST_CHECK_EQUAL(Virtex2::sCommandName[Virtex2::eCommandGRESTORE],		"GRESTORE");
	BOOST_CHECK_EQUAL(Virtex2::sCommandName[Virtex2::eCommandSHUTDOWN],		"SHUTDOWN");
	BOOST_CHECK_EQUAL(Virtex2::sCommandName[Virtex2::eCommandGCAPTURE],		"GCAPTURE");
	BOOST_CHECK_EQUAL(Virtex2::sCommandName[Virtex2::eCommandDESYNCH],		"DESYNCH");

	// build the file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "Virtex2UnitTest.generated.bit";
	boost::filesystem::path referencePath = regressionPath / "Virtex2UnitTest.reference.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	Virtex2 bitstream;
	bitstream.read(fileStream, false);
	// write the bitstream digest to the console
	std::cout << bitstream << std::endl;

	std::string designName = bitstream.getDesignName();
	std::string deviceName = bitstream.getDeviceName();
	std::string designDate = bitstream.getDesignDate();
	std::string designTime = bitstream.getDesignTime();
	torc::architecture::DeviceDesignator deviceDesignator(deviceName);
	std::cout << "family of " << deviceName << " is " << deviceDesignator.getFamily() << std::endl;

	// write the bitstream back out
	std::fstream outputStream(generatedPath.string().c_str(), std::ios::binary | std::ios::out);
	BOOST_REQUIRE(outputStream.good());
	bitstream.write(outputStream);
	outputStream.flush();

	// compare the reference and generated XDL
	BOOST_CHECK(torc::common::fileContentsAreEqual(generatedPath, referencePath));
}








































void testVirtex2Device(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath);

/// \brief Unit test for the Virtex2 class Frame Address Register mapping.
BOOST_AUTO_TEST_CASE(bitstream_virtex2_far) {

	// look up the command line arguments
	int& argc = boost::unit_test::framework::master_test_suite().argc;
	char**& argv = boost::unit_test::framework::master_test_suite().argv;
	// make sure that we at least have the name under which we were invoked
	BOOST_REQUIRE(argc >= 1);
	// resolve symbolic links if applicable
	torc::common::DirectoryTree directoryTree(argv[0]);

	// iterate over the devices
	const torc::common::DeviceVector& devices = torc::common::Devices::getVirtex2Devices();
	torc::common::DeviceVector::const_iterator dp = devices.begin();
	torc::common::DeviceVector::const_iterator de = devices.end();
	while(dp < de) {
		const std::string& device = *dp++;
		if(device.empty()) break;
//std::cout << "device " << ": " << device << std::endl;
		testVirtex2Device(device, torc::common::DirectoryTree::getWorkingPath());
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

	std::ostream& operator<< (std::ostream& os, const Virtex2::FrameAddress& rhs);
	std::ostream& operator<< (std::ostream& os, const Virtex2::FrameAddress& rhs) {
		return os << rhs.mBlockType	<< "(" << rhs.mMajor << "." << rhs.mMinor << ")";
	}

void testVirtex2Device(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath) {

	// build the file paths
	boost::filesystem::path debugBitstreamPath = inWorkingPath / "torc" / "bitstream" / "regression";
	//boost::filesystem::path generatedPath = debugBitstreamPath / (inDeviceName + ".debug.bit");
	boost::filesystem::path referencePath = debugBitstreamPath / (inDeviceName + ".debug.bit");
std::cerr << "TRYING TO FIND " << referencePath << std::endl;

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	std::cerr << "Trying to read: " << referencePath << std::endl;
	BOOST_REQUIRE(fileStream.good());
	Virtex2 bitstream;
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
	Virtex2::FrameAddressToIndex farRemaining = bitstream.mFrameAddressToIndex;
	Virtex2::FrameAddressToIndex farVisited;
	{
		//bool first = true;
		Virtex2::const_iterator p = bitstream.begin();
		Virtex2::const_iterator e = bitstream.end();
		uint32_t header = VirtexPacket::makeHeader(VirtexPacket::ePacketType1, 
			VirtexPacket::eOpcodeWrite, Virtex2::eRegisterLOUT, 1);
		while(p < e) {
			const VirtexPacket& packet = *p++;
			if(packet.getHeader() != header) continue;
			//if(first) { first = false; continue; }
			Virtex2::FrameAddress far = packet[1];
			//std::cout << std::endl << "Debug Far Address: " << Hex32(packet[1]) << std::endl;
			farVisited[far] = 0;
			Virtex2::FrameAddressToIndex::iterator found = farRemaining.find(far);
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







/// \brief Unit test for the Virtex2 static device info generation.
BOOST_AUTO_TEST_CASE(bitstream_virtex2_generate) {

	Virtex2 bitstream;
	DeviceInfoHelper::buildFamilyDeviceInfo("Virtex2", "Virtex2DeviceInfo.template", 
		"Virtex2DeviceInfo.cpp", torc::common::Devices::getVirtex2Devices(), bitstream);

}


BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
