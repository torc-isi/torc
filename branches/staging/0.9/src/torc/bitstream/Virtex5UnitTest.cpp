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
/// \brief Source for the Virtex5 unit test.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/Virtex5.hpp"
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

/// \brief Unit test for the Virtex5 CRC
BOOST_AUTO_TEST_CASE(crc_virtex5) {
	std::fstream fileStream("Virtex5UnitTest.reference.bit", std::ios::binary | std::ios::in);
	Virtex5 bitstream;
	bitstream.read(fileStream, false);
	std::cout << bitstream << std::endl;
	bitstream.preflightPackets();
	BOOST_REQUIRE(true);
}

/// \brief Unit test for the Virtex5 class.
BOOST_AUTO_TEST_CASE(bitstream_virtex5) {

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

	// build the file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "torc" / "bitstream" / "regression";
	boost::filesystem::path generatedPath = regressionPath / "Virtex5UnitTest.generated.bit";
	boost::filesystem::path referencePath = regressionPath / "Virtex5UnitTest.reference.bit";

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





































void testVirtex5Device(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath);

/// \brief Unit test for the Virtex5 class Frame Address Register mapping.
BOOST_AUTO_TEST_CASE(bitstream_virtex5_far) {

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

	std::ostream& operator<< (std::ostream& os, const Virtex5::FrameAddress& rhs);
	std::ostream& operator<< (std::ostream& os, const Virtex5::FrameAddress& rhs) {
		return os << (rhs.mTopBottom == Virtex5::eFarTop ? 'T' : 'B') << "" << rhs.mBlockType 
				<< "(" << rhs.mRow << "," << rhs.mMajor << "." << rhs.mMinor << ")";
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
	bitstream.read(fileStream, false);
	// write the bitstream digest to the console
//	std::cout << bitstream << std::endl;

//	// initialize the bitstream frame maps
//	boost::filesystem::path deviceColumnsPath = inWorkingPath / "torc" / "bitstream" / "regression" 
//		/ (inDeviceName + ".cpp");
//	std::fstream deviceColumnsStream(deviceColumnsPath.string().c_str(), std::ios::out);
	bitstream.initializeDeviceInfo(inDeviceName);
	bitstream.initializeFrameMaps();

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

/// \brief Unit test for the Virtex5 bitstream to bitmap conversion.
BOOST_AUTO_TEST_CASE(Virtex5BitmapConversion) {
	// read the bitstream
	boost::filesystem::path referencePath = torc::common::DirectoryTree::getExecutablePath() 
		/ "regression" / "Virtex5UnitTest.reference.bit";
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	Virtex5 bitstream;
	bitstream.read(fileStream, false);

	// initialize the bitstream frame maps
	bitstream.initializeDeviceInfo(bitstream.getDeviceName());
	bitstream.initializeFrameMaps();

	// walk the bitstream and process all FAR and FDRI writes
	Virtex5::const_iterator p = bitstream.begin();
	Virtex5::const_iterator e = bitstream.end();
/*
	uint32_t farHeader = VirtexPacket::makeHeader(VirtexPacket::ePacketType1, 
		VirtexPacket::eOpcodeWrite, Virtex5::eRegisterFAR, 1);
	uint32_t fdriHeader = VirtexPacket::makeHeader(VirtexPacket::ePacketType1, 
		VirtexPacket::eOpcodeWrite, Virtex5::eRegisterFAR, 1);
*/
	while(p < e) {
		const VirtexPacket& packet = *p++;
		if(!packet.isWrite()) continue;
		switch(packet.getAddress()) {
		}
/*
		EPacketType getType(void) const { return mType; }
		EOpcode getOpcode(void) const { return mOpcode; }
		int getAddress(void) const { return mAddress; }
		uint32_t getHeader(void) const { return mHeader; }
		/// \brief Returns the number of payload words in the packet, excluding the header word.
		uint32_t getWordCount(void) const { return mCount; }
		/// \brief Returns the total number of words in the packet, including the header word.
		uint32_t getWordSize(void) const { return mCount + 1; }
//		const uint32_t* getWords(void) const { return mWords; }
// tests
bool isType1(void) const { return mType == ePacketType1; }
bool isType2(void) const { return mType == ePacketType2; }
bool isNop(void) const { return mOpcode == eOpcodeNOP; }
bool isReserved(void) const { return mOpcode == eOpcodeReserved; }
bool isRead(void) const { return mOpcode == eOpcodeRead; }
bool isWrite(void) const { return mOpcode == eOpcodeWrite; }
bool isDummyWord(void) const { return mHeader == eSynchronizationDummy; }
bool isSyncWord(void) const { return mHeader == eSynchronizationSync; }
bool isBusWidthSyncWord(void) const { return mHeader == eSynchronizationBusWidthSync; }
bool isBusWidthDetectWord(void) const { return mHeader == eSynchronizationBusWidthDetect; }
*/
		Virtex5::FrameAddress far = packet[1];
	}

}




/*
/// \brief Unit test for the Virtex5 static device info generation.
BOOST_AUTO_TEST_CASE(bitstream_virtex5_generate) {

	Virtex5 bitstream;
	DeviceInfoHelper::buildFamilyDeviceInfo("Virtex5", "Virtex5DeviceInfo.template", 
		"Virtex5DeviceInfo.cpp", torc::Devices::getVirtex5Devices(), bitstream);

}
*/





BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
