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
/// \brief Source for the Virtex4 unit test.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/Virtex4.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/common/Devices.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/DeviceDesignator.hpp"
#include "torc/bitstream/OutputStreamHelpers.hpp"
#include "torc/common/TestHelpers.hpp"
#include <fstream>
#include <iostream>

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)


/// \brief Unit test for the Virtex4 class.
BOOST_AUTO_TEST_CASE(bitstream_virtex4) {

	// enums tested:
	//		EPacket
	//		EFar
	boost::uint32_t mask;
	// type 1 packet subfield masks
	mask = Virtex4::ePacketMaskType + Virtex4::ePacketMaskOpcode 
		+ Virtex4::ePacketMaskType1Address + Virtex4::ePacketMaskType1Reserved 
		+ Virtex4::ePacketMaskType1Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// type 2 packet subfield masks
	mask = Virtex4::ePacketMaskType + Virtex4::ePacketMaskOpcode 
		+ Virtex4::ePacketMaskType2Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// frame address register subfield masks
	mask = Virtex4::eFarMaskTopBottom + Virtex4::eFarMaskBlockType + Virtex4::eFarMaskRow 
		+ Virtex4::eFarMaskMajor + Virtex4::eFarMaskMinor;
	BOOST_CHECK_EQUAL(mask, 0x007FFFFFu);

	// members tested:
	//		Virtex4::sPacketTypeName and EPacketTypeName
	BOOST_CHECK_EQUAL(Virtex4::sPacketTypeName[0],							"[UNKNOWN TYPE 0]");
	BOOST_CHECK_EQUAL(Virtex4::sPacketTypeName[Virtex4::ePacketType1],		"TYPE1");
	BOOST_CHECK_EQUAL(Virtex4::sPacketTypeName[Virtex4::ePacketType2],		"TYPE2");
	BOOST_CHECK_EQUAL(Virtex4::sPacketTypeName[3],							"[UNKNOWN TYPE 3]");
	BOOST_CHECK_EQUAL(Virtex4::sPacketTypeName[4],							"[UNKNOWN TYPE 4]");
	BOOST_CHECK_EQUAL(Virtex4::sPacketTypeName[5],							"[UNKNOWN TYPE 5]");
	BOOST_CHECK_EQUAL(Virtex4::sPacketTypeName[6],							"[UNKNOWN TYPE 6]");
	BOOST_CHECK_EQUAL(Virtex4::sPacketTypeName[7],							"[UNKNOWN TYPE 7]");

	// members tested:
	//		Virtex4::sOpcodeName and EOpcode
	BOOST_CHECK_EQUAL(Virtex4::sOpcodeName[Virtex4::eOpcodeNOP],			"NOP");
	BOOST_CHECK_EQUAL(Virtex4::sOpcodeName[Virtex4::eOpcodeRead],			"READ");
	BOOST_CHECK_EQUAL(Virtex4::sOpcodeName[Virtex4::eOpcodeWrite],			"WRITE");
	BOOST_CHECK_EQUAL(Virtex4::sOpcodeName[Virtex4::eOpcodeReserved],		"RESERVED");

	// members tested:
	//		Virtex4::sRegisterName and ERegister
	BOOST_CHECK_EQUAL(Virtex4::sRegisterName[Virtex4::eRegisterCRC],		"CRC");
	BOOST_CHECK_EQUAL(Virtex4::sRegisterName[Virtex4::eRegisterFAR],		"FAR");
	BOOST_CHECK_EQUAL(Virtex4::sRegisterName[Virtex4::eRegisterFDRI],		"FDRI");
	BOOST_CHECK_EQUAL(Virtex4::sRegisterName[Virtex4::eRegisterFDRO],		"FDRO");
	BOOST_CHECK_EQUAL(Virtex4::sRegisterName[Virtex4::eRegisterCMD],		"CMD");
	BOOST_CHECK_EQUAL(Virtex4::sRegisterName[Virtex4::eRegisterCTL],		"CTL");
	BOOST_CHECK_EQUAL(Virtex4::sRegisterName[Virtex4::eRegisterMASK],		"MASK");
	BOOST_CHECK_EQUAL(Virtex4::sRegisterName[Virtex4::eRegisterSTAT],		"STAT");
	BOOST_CHECK_EQUAL(Virtex4::sRegisterName[Virtex4::eRegisterLOUT],		"LOUT");
	BOOST_CHECK_EQUAL(Virtex4::sRegisterName[Virtex4::eRegisterCOR],		"COR");
	BOOST_CHECK_EQUAL(Virtex4::sRegisterName[Virtex4::eRegisterMFWR],		"MFWR");
	BOOST_CHECK_EQUAL(Virtex4::sRegisterName[Virtex4::eRegisterCBC],		"CBC");
	BOOST_CHECK_EQUAL(Virtex4::sRegisterName[Virtex4::eRegisterIDCODE],		"IDCODE");
	BOOST_CHECK_EQUAL(Virtex4::sRegisterName[Virtex4::eRegisterAXSS],		"AXSS");

	// members tested:
	//		Virtex4::sCommandName and ECommand
	BOOST_CHECK_EQUAL(Virtex4::sCommandName[Virtex4::eCommandNULL],			"NULL");
	BOOST_CHECK_EQUAL(Virtex4::sCommandName[Virtex4::eCommandWCFG],			"WCFG");
	BOOST_CHECK_EQUAL(Virtex4::sCommandName[Virtex4::eCommandMFWR],			"MFWR");
	BOOST_CHECK_EQUAL(Virtex4::sCommandName[Virtex4::eCommandLFRM],			"LFRM");
	BOOST_CHECK_EQUAL(Virtex4::sCommandName[Virtex4::eCommandRCFG],			"RCFG");
	BOOST_CHECK_EQUAL(Virtex4::sCommandName[Virtex4::eCommandSTART],		"START");
	BOOST_CHECK_EQUAL(Virtex4::sCommandName[Virtex4::eCommandRCAP],			"RCAP");
	BOOST_CHECK_EQUAL(Virtex4::sCommandName[Virtex4::eCommandRCRC],			"RCRC");
	BOOST_CHECK_EQUAL(Virtex4::sCommandName[Virtex4::eCommandAGHIGH],		"AGHIGH");
	BOOST_CHECK_EQUAL(Virtex4::sCommandName[Virtex4::eCommandSWITCH],		"SWITCH");
	BOOST_CHECK_EQUAL(Virtex4::sCommandName[Virtex4::eCommandGRESTORE],		"GRESTORE");
	BOOST_CHECK_EQUAL(Virtex4::sCommandName[Virtex4::eCommandSHUTDOWN],		"SHUTDOWN");
	BOOST_CHECK_EQUAL(Virtex4::sCommandName[Virtex4::eCommandGCAPTURE],		"GCAPTURE");
	BOOST_CHECK_EQUAL(Virtex4::sCommandName[Virtex4::eCommandDESYNC],		"DESYNC");

	// build the file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "Virtex4UnitTest.generated.bit";
	boost::filesystem::path referencePath = regressionPath / "Virtex4UnitTest.reference.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	Virtex4 bitstream;
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


void testVirtex4Device(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath);

/// \brief Unit test for the Virtex4 class Frame Address Register mapping.
BOOST_AUTO_TEST_CASE(bitstream_virtex4_far) {

	// look up the command line arguments
	int& argc = boost::unit_test::framework::master_test_suite().argc;
	char**& argv = boost::unit_test::framework::master_test_suite().argv;
	// make sure that we at least have the name under which we were invoked
	BOOST_REQUIRE(argc >= 1);
	// resolve symbolic links if applicable
	torc::common::DirectoryTree directoryTree(argv[0]);

	// iterate over the devices
	const torc::common::DeviceVector& devices = torc::common::Devices::getVirtex4Devices();
	torc::common::DeviceVector::const_iterator dp = devices.begin();
	torc::common::DeviceVector::const_iterator de = devices.end();
	while(dp < de) {
		const std::string& device = *dp++;
		if(device.empty()) break;
//if(i == 6) 
		testVirtex4Device(device, torc::common::DirectoryTree::getWorkingPath());
	}
}

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

	std::ostream& operator<< (std::ostream& os, const Virtex4::FrameAddress& rhs);
	std::ostream& operator<< (std::ostream& os, const Virtex4::FrameAddress& rhs) {
		return os << (rhs.mTopBottom == Virtex4::eFarTop ? 'T' : 'B') << "" << rhs.mBlockType 
				<< "(" << rhs.mRow << "," << rhs.mMajor << "." << rhs.mMinor << ")";
	}

using namespace torc::architecture::xilinx;
void testVirtex4Device(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath) {

	torc::architecture::DDB ddb(inDeviceName);
	BOOST_CHECK_EQUAL(ddb.getDeviceName(), inDeviceName);

	// build the file paths
	boost::filesystem::path debugBitstreamPath = inWorkingPath / "regression";
	//boost::filesystem::path generatedPath = debugBitstreamPath / (inDeviceName + ".debug.bit");
	boost::filesystem::path referencePath = debugBitstreamPath / (inDeviceName + ".debug.bit");

std::cerr << "looking for path: " << referencePath << std::endl;
	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	Virtex4 bitstream;
	bitstream.read(fileStream, false);
	// write the bitstream digest to the console
//	std::cout << bitstream << std::endl;

	// look up the device tile map
	const torc::architecture::Tiles& tiles = ddb.getTiles();
	TileCount tileCount = tiles.getTileCount();
	TileRow rowCount = tiles.getRowCount();
	TileCol colCount = tiles.getColCount();
std::cerr << "tileCount: " << tileCount << std::endl;
std::cerr << "rowCount: " << rowCount << std::endl;
std::cerr << "colCount: " << colCount << std::endl;

	typedef std::map<TileTypeIndex, std::string> TileTypeIndexToName;
	typedef std::map<std::string, TileTypeIndex> TileTypeNameToIndex;
	typedef std::map<TileTypeIndex, TileTypeWidths> TileTypeIndexToWidths;
	TileTypeIndexToName tileTypeIndexToName;
	TileTypeNameToIndex tileTypeNameToIndex;
	TileTypeIndexToWidths tileTypeWidths;
	TileTypeCount tileTypeCount = tiles.getTileTypeCount();
	for(TileTypeIndex tileTypeIndex(0); tileTypeIndex < tileTypeCount; tileTypeIndex++) {
		const std::string tileTypeName = tiles.getTileTypeName(tileTypeIndex);
		tileTypeIndexToName[tileTypeIndex] = tileTypeName;
		tileTypeNameToIndex[tileTypeName] = tileTypeIndex;
	}
	// reference row tile types
	// BRAM columns
	TileTypeIndex typeBram				= tileTypeNameToIndex["BRAM"];
	// CLB columns
	TileTypeIndex typeClb				= tileTypeNameToIndex["CLB"];
	// CLKV columns
	TileTypeIndex typeClkv				= tileTypeNameToIndex["CLKV"]; 
	TileTypeIndex typeClkvDcmT			= tileTypeNameToIndex["CLKV_DCM_T"]; 
	// DSP columns
	TileTypeIndex typeDsp				= tileTypeNameToIndex["DSP"]; 
	// DCM columns
	TileTypeIndex typeDcm				= tileTypeNameToIndex["DCM"]; 
	TileTypeIndex typeSysMon 			= tileTypeNameToIndex["SYS_MON"]; 
	// IOIS columns
	TileTypeIndex typeIoisLc 			= tileTypeNameToIndex["IOIS_LC"]; 
	TileTypeIndex typeIoisLcL 			= tileTypeNameToIndex["IOIS_LC_L"]; 
	// MGT columns
	TileTypeIndex typeMgtAl				= tileTypeNameToIndex["MGT_AL"]; 
	TileTypeIndex typeMgtAr				= tileTypeNameToIndex["MGT_AR"]; 
	// empty columns
	TileTypeIndex typeCfgVbrkFrame		= tileTypeNameToIndex["CFG_VBRK_FRAME"];
	TileTypeIndex typeClbBuffer 		= tileTypeNameToIndex["CLB_BUFFER"]; 
	TileTypeIndex typeEmptyMgt			= tileTypeNameToIndex["EMPTY_MGT"]; 
	TileTypeIndex typeInt				= tileTypeNameToIndex["INT"]; 
	TileTypeIndex typeIntSo 			= tileTypeNameToIndex["INT_SO"]; 
	TileTypeIndex typeIntSoDcm0 		= tileTypeNameToIndex["INT_SO_DCM0"]; 
	TileTypeIndex typeLTermInt 			= tileTypeNameToIndex["L_TERM_INT"]; 
	TileTypeIndex typeMgtR				= tileTypeNameToIndex["MGT_R"]; 
	TileTypeIndex typeRTermInt 			= tileTypeNameToIndex["R_TERM_INT"]; 
	// tile type widths by block type	  block type:     0   1   2   3   4   5   6   7
	// BRAM columns
	tileTypeWidths[typeBram]			= TileTypeWidths( 0, 20, 64,  0,  0,  0,  0,  0);
	// CLB columns
	tileTypeWidths[typeClb]				= TileTypeWidths(22,  0,  0,  0,  0,  0,  0,  0);
	// CLKV columns
	tileTypeWidths[typeClkv]			= TileTypeWidths( 3,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeClkvDcmT]		= TileTypeWidths( 3,  0,  0,  0,  0,  0,  0,  0);
	// DSP columns
	tileTypeWidths[typeDsp]				= TileTypeWidths(21,  0,  0,  0,  0,  0,  0,  0);
	// DCM columns
	tileTypeWidths[typeDcm]				= TileTypeWidths(30,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeSysMon]			= TileTypeWidths(30,  0,  0,  0,  0,  0,  0,  0);
	// IOIS columns
	tileTypeWidths[typeIoisLc]			= TileTypeWidths(30,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeIoisLcL]			= TileTypeWidths(30,  0,  0,  0,  0,  0,  0,  0);
	// MGT columns
	tileTypeWidths[typeMgtAl]			= TileTypeWidths(20,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeMgtAr]			= TileTypeWidths(20,  0,  0,  0,  0,  0,  0,  0);
	// empty columns
	tileTypeWidths[typeCfgVbrkFrame]	= TileTypeWidths( 0,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeClbBuffer] 		= TileTypeWidths( 0,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeEmptyMgt]		= TileTypeWidths( 0,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeInt]				= TileTypeWidths( 0,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeIntSo]			= TileTypeWidths( 0,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeIntSoDcm0] 		= TileTypeWidths( 0,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeLTermInt] 		= TileTypeWidths( 0,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeMgtR]			= TileTypeWidths( 0,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeRTermInt] 		= TileTypeWidths( 0,  0,  0,  0,  0,  0,  0,  0);

	TileRow referenceRow(8);
	TileRow centerRow((rowCount >> 1) + 1);
//	uint32_t frameRowCount = (rowCount / 20) >> 1;
	uint32_t frameCount = 0;
	for(uint32_t i = 0; i < Virtex4::eFarBlockTypeCount; i++) {
		Virtex4::EFarBlockType blockType = Virtex4::EFarBlockType(i);
		std::cout << "Block type " << blockType << std::endl;
		TileRow row = referenceRow;
//		for(TileRow row = referenceRow; row < rowCount; row += 20) {
			for(TileCol col; col < colCount; col++) {
				// look up the tile info
				const torc::architecture::TileInfo& tileInfo 
					= tiles.getTileInfo(tiles.getTileIndex(referenceRow, col));
				TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
				// determine whether the tile type widths are defined
				TileTypeIndexToWidths::iterator p = tileTypeWidths.find(tileTypeIndex);
				if(p == tileTypeWidths.end()) {
					std::cout << "Unknown " << tileTypeIndexToName[tileTypeIndex] 
						<< " width at column " << col << std::endl;
					p->second = TileTypeWidths();
				}
				uint32_t width = p->second[blockType];
				frameCount += width;
				std::cout << "    " << tiles.getTileTypeName(tileInfo.getTypeIndex()) << ": " 
					<< width << " (" << frameCount << ")" << std::endl;
//			}
		}
if(i == 2) break;
	}

	typedef std::map<uint32_t, Virtex4::FrameAddress> FrameIndexToAddress;
	typedef std::map<Virtex4::FrameAddress, uint32_t> FrameAddressToIndex;
	FrameIndexToAddress frameIndexToAddress;
	FrameAddressToIndex frameAddressToIndex;
	uint32_t farRowCount = (rowCount / 18) >> 1;
	uint32_t frameIndex = 0;
	for(uint32_t i = 0; i < Virtex4::eFarBlockTypeCount; i++) {
		Virtex4::EFarBlockType blockType = Virtex4::EFarBlockType(i);
		std::cout << "Block type " << blockType << std::endl;
		TileRow row = referenceRow;
		for(uint32_t half = 0; half < 2; half++) {
			for(uint32_t farRow = 0; farRow < farRowCount; farRow++) {
				uint32_t farMajor = 0;
				for(TileCol col; col < colCount; col++) {
					// look up the tile info
					const torc::architecture::TileInfo& tileInfo 
						= tiles.getTileInfo(tiles.getTileIndex(referenceRow, col));
					TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
					// determine whether the tile type widths are defined
					TileTypeIndexToWidths::iterator p = tileTypeWidths.find(tileTypeIndex);
					if(p == tileTypeWidths.end()) {
						std::cout << "Unknown " << tileTypeIndexToName[tileTypeIndex] 
							<< " width at column " << col << std::endl;
						p->second = TileTypeWidths();
					}
					uint32_t width = p->second[blockType];
					for(uint32_t farMinor = 0; farMinor < width; farMinor++) {
						Virtex4::FrameAddress far(Virtex4::EFarTopBottom(half), blockType, farRow, 
							farMajor, farMinor);
						frameIndexToAddress[frameIndex] = far;
						frameAddressToIndex[far] = frameIndex;
//std::cout << frameIndex << ":" << far << " ";
						frameIndex++;
					}
					if(width > 0) farMajor++;
					frameCount += width;
	//				std::cout << "    " << tiles.getTileTypeName(tileInfo.getTypeIndex()) << ": " 
	//					<< width << " (" << frameCount << ")" << std::endl;
				}
			}
		}
//std::cout << std::endl;
if(i == 2) break;
	}

std::cout << "size of frameAddressToIndex is " << frameAddressToIndex.size() << std::endl;
std::cout << "size of frameIndexToAddress is " << frameIndexToAddress.size() << std::endl;
/*
	std::cout << "\n\n\nSTART ADDRESSES\n";
	//FrameAddressToIndex farCopy = frameAddressToIndex;
	const FrameAddressToIndex& farCopy = frameAddressToIndex;
	{
		FrameAddressToIndex::const_iterator p = farCopy.begin();
		FrameAddressToIndex::const_iterator e = farCopy.end();
		while(p != e) {
			std::cout << p->second << ":" << p->first << " ";
			p++;
		}
		std::cout << std::endl;
	}
	std::cout << "END ADDRESSES\n\n\n";
*/

	boost::filesystem::path generatedMap = inWorkingPath / (inDeviceName + ".map.csv");
	std::fstream tilemapStream(generatedMap.string().c_str(), std::ios::out);
	BOOST_REQUIRE(tilemapStream.good());
	for(TileRow row; row < rowCount; row++) {
		for(TileCol col; col < colCount; col++) {
			const torc::architecture::TileInfo& tileInfo 
				= tiles.getTileInfo(tiles.getTileIndex(row, col));
			TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
			tilemapStream << tiles.getTileTypeName(tileTypeIndex);
			if(col + 1 < colCount) tilemapStream << ",";
		}
		tilemapStream << std::endl;
	}
	tilemapStream.close();

	// iterate through the packets, and extract all of the FARs
	for(int half = 0; half < 2; half++) {
		for(uint32_t row = 0; row < 2; row++) {
			typedef std::map<uint32_t, uint32_t> ColumnMaxFrame;
			ColumnMaxFrame maxFrames[Virtex4::eFarBlockTypeCount];
			Virtex4::const_iterator p = bitstream.begin();
			Virtex4::const_iterator e = bitstream.end();
			uint32_t header = VirtexPacket::makeHeader(VirtexPacket::ePacketType1, 
				VirtexPacket::eOpcodeWrite, Virtex4::eRegisterLOUT, 1);
			while(p < e) {
				const VirtexPacket& packet = *p++;
				if(packet.getHeader() != header) continue;
				Virtex4::FrameAddress far = packet[1];
		//		uint32_t far = packet[1];
		//		std::cerr << Hex32(far) << " ";
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
			frameCount = 0;
			for(uint32_t i = 0; i < Virtex4::eFarBlockTypeCount; i++) {
				Virtex4::EFarBlockType blockType = Virtex4::EFarBlockType(i);
				uint32_t majorCount = maxFrames[blockType].size();
				for(uint32_t major = 0; major < majorCount; major++) {
					frameCount += maxFrames[blockType][major] + 1;
					std::cout << blockType << "(" << major << "): " << (maxFrames[blockType][major] + 1) 
						<< " (" << frameCount << ")" << std::endl;
				}
			}
		}
	}

	// iterate through the packets, and extract all of the FARs
	FrameAddressToIndex farRemaining = frameAddressToIndex;
	FrameAddressToIndex farVisited;
	{
		bool first = true;
		Virtex4::const_iterator p = bitstream.begin();
		Virtex4::const_iterator e = bitstream.end();
		uint32_t header = VirtexPacket::makeHeader(VirtexPacket::ePacketType1, 
			VirtexPacket::eOpcodeWrite, Virtex4::eRegisterLOUT, 1);
		while(p < e) {
			const VirtexPacket& packet = *p++;
			if(packet.getHeader() != header) continue;
			if(first) { first = false; continue; }
			Virtex4::FrameAddress far = packet[1];
			farVisited[far] = 0;
			FrameAddressToIndex::iterator found = farRemaining.find(far);
			if(found != farRemaining.end()) {
				farRemaining.erase(found);
			} else {
				std::cerr << "missing " << far << " ";
			}
		}
	}
std::cout << "size of farRemaining is " << farRemaining.size() << std::endl;
std::cout << "size of farVisited is " << farVisited.size() << std::endl;
//std::cout << bitstream;

	BOOST_REQUIRE_EQUAL(frameAddressToIndex.size(), farVisited.size());
	BOOST_REQUIRE_EQUAL(farRemaining.size(), 0u);

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
