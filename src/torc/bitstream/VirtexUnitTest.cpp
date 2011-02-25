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
/// \brief Source for the Virtex unit test.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/Virtex.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/DeviceDesignator.hpp"
#include "torc/bitstream/OutputStreamHelpers.hpp"
#include "torc/common/TestHelpers.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <fstream>
#include <iostream>

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)


/// \brief Unit test for the Virtex class.
BOOST_AUTO_TEST_CASE(bitstream_virtex) {

	// enums tested:
	//		EPacket
	//		EFar
	boost::uint32_t mask;
	// type 1 packet subfield masks
	mask = Virtex::ePacketMaskType + Virtex::ePacketMaskOpcode 
		+ Virtex::ePacketMaskType1Address + Virtex::ePacketMaskType1Reserved 
		+ Virtex::ePacketMaskType1Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// type 2 packet subfield masks
	mask = Virtex::ePacketMaskType + Virtex::ePacketMaskOpcode 
		+ Virtex::ePacketMaskType2Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// frame address register subfield masks
	mask = Virtex::eFarMaskBlockType+ Virtex::eFarMaskMajor +
	 	Virtex::eFarMaskMinor;
	BOOST_CHECK_EQUAL(mask, 0x07FFFE00u);

	// members tested:
	//		Virtex::sPacketTypeName and EPacketTypeName
	BOOST_CHECK_EQUAL(Virtex::sPacketTypeName[0],						"[UNKNOWN TYPE 0]");
	BOOST_CHECK_EQUAL(Virtex::sPacketTypeName[Virtex::ePacketType1],	"TYPE1");
	BOOST_CHECK_EQUAL(Virtex::sPacketTypeName[Virtex::ePacketType2],	"TYPE2");
	BOOST_CHECK_EQUAL(Virtex::sPacketTypeName[3],						"[UNKNOWN TYPE 3]");
	BOOST_CHECK_EQUAL(Virtex::sPacketTypeName[4],						"[UNKNOWN TYPE 4]");
	BOOST_CHECK_EQUAL(Virtex::sPacketTypeName[5],						"[UNKNOWN TYPE 5]");
	BOOST_CHECK_EQUAL(Virtex::sPacketTypeName[6],						"[UNKNOWN TYPE 6]");
	BOOST_CHECK_EQUAL(Virtex::sPacketTypeName[7],						"[UNKNOWN TYPE 7]");

	// members tested:
	//		Virtex::sOpcodeName and EOpcode
	BOOST_CHECK_EQUAL(Virtex::sOpcodeName[Virtex::eOpcodeRead],			"READ");
	BOOST_CHECK_EQUAL(Virtex::sOpcodeName[Virtex::eOpcodeWrite],		"WRITE");

	// members tested:
	//		Virtex::sRegisterName and ERegister
	BOOST_CHECK_EQUAL(Virtex::sRegisterName[Virtex::eRegisterCRC],		"CRC");
	BOOST_CHECK_EQUAL(Virtex::sRegisterName[Virtex::eRegisterFAR],		"FAR");
	BOOST_CHECK_EQUAL(Virtex::sRegisterName[Virtex::eRegisterFDRI],		"FDRI");
	BOOST_CHECK_EQUAL(Virtex::sRegisterName[Virtex::eRegisterFDRO],		"FDRO");
	BOOST_CHECK_EQUAL(Virtex::sRegisterName[Virtex::eRegisterCMD],		"CMD");
	BOOST_CHECK_EQUAL(Virtex::sRegisterName[Virtex::eRegisterCTL],		"CTL");
	BOOST_CHECK_EQUAL(Virtex::sRegisterName[Virtex::eRegisterMASK],		"MASK");
	BOOST_CHECK_EQUAL(Virtex::sRegisterName[Virtex::eRegisterSTAT],		"STAT");
	BOOST_CHECK_EQUAL(Virtex::sRegisterName[Virtex::eRegisterLOUT],		"LOUT");
	BOOST_CHECK_EQUAL(Virtex::sRegisterName[Virtex::eRegisterCOR],		"COR");
	BOOST_CHECK_EQUAL(Virtex::sRegisterName[Virtex::eRegisterFLR],		"FLR");

	// members tested:
	//		Virtex::sCommandName and ECommand
	BOOST_CHECK_EQUAL(Virtex::sCommandName[Virtex::eCommandWCFG],		"WCFG");
	BOOST_CHECK_EQUAL(Virtex::sCommandName[Virtex::eCommandLFRM],		"LFRM");
	BOOST_CHECK_EQUAL(Virtex::sCommandName[Virtex::eCommandRCFG],		"RCFG");
	BOOST_CHECK_EQUAL(Virtex::sCommandName[Virtex::eCommandSTART],		"START");
	BOOST_CHECK_EQUAL(Virtex::sCommandName[Virtex::eCommandRCAP],		"RCAP");
	BOOST_CHECK_EQUAL(Virtex::sCommandName[Virtex::eCommandRCRC],		"RCRC");
	BOOST_CHECK_EQUAL(Virtex::sCommandName[Virtex::eCommandAGHIGH],		"AGHIGH");
	BOOST_CHECK_EQUAL(Virtex::sCommandName[Virtex::eCommandSWITCH],		"SWITCH");


	// build the file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "VirtexUnitTest.generated.bit";
	boost::filesystem::path referencePath = regressionPath / "VirtexUnitTest.reference.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	Virtex bitstream;
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


void testVirtexDevice(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath);

/// \brief Unit test for the Virtex class Frame Address Register mapping.
BOOST_AUTO_TEST_CASE(bitstream_virtex_far) {

	std::string devices[] = {
		// Virtex
		"xcv50", "xcv100", "xcv150", "xcv200", "xcv300", "xcv400", "xcv600", "xcv800", "xcv1000",

		// VirtexE
		"xcv50e", "xcv100e", "xcv200e", "xcv300e", "xcv400e", "xcv405e", "xcv600e", "xcv812e", 
			"xcv1000e", "xcv1600e", "xcv2000e", "xcv2600e", "xcv3200e",

		// termination
		""
	};

	for(int i = 0; ; i++) {
		std::string& device = devices[i];
		if(device.empty()) break;
//if(i == 6) 
		testVirtexDevice(device, torc::common::DirectoryTree::getWorkingPath());
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

	std::ostream& operator<< (std::ostream& os, const Virtex::FrameAddress& rhs);
	std::ostream& operator<< (std::ostream& os, const Virtex::FrameAddress& rhs) {
		return os << rhs.mBlockType	<< "(" << rhs.mMajor << "." << rhs.mMinor << ")";
	}

using namespace torc::architecture::xilinx;
void testVirtexDevice(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath) {

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
	Virtex bitstream;
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
	for(uint32_t i = 0; i < Virtex::eFarBlockTypeCount; i++) {
		Virtex::EFarBlockType blockType = Virtex::EFarBlockType(i);
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
	/*Need to remove top/bottom & row functionality for Virtex

	typedef std::map<uint32_t, Virtex::FrameAddress> FrameIndexToAddress;
	typedef std::map<Virtex::FrameAddress, uint32_t> FrameAddressToIndex;
	FrameIndexToAddress frameIndexToAddress;
	FrameAddressToIndex frameAddressToIndex;
	uint32_t farRowCount = (rowCount / 18) >> 1;
	uint32_t frameIndex = 0;
	for(uint32_t i = 0; i < Virtex::eFarBlockTypeCount; i++) {
		Virtex::EFarBlockType blockType = Virtex::EFarBlockType(i);
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
						Virtex::FrameAddress far(Virtex::EFarTopBottom(half), blockType, farRow, 
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
std::cout << "size of frameIndexToAddress is " << frameIndexToAddress.size() << std::endl;		*/

/*Need to remove top/bottom & row functionality for Virtex

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
*/

	/* Need to remove top/bottom & row functionality for Virtex
	// iterate through the packets, and extract all of the FARs
	for(int half = 0; half < 2; half++) {
		for(uint32_t row = 0; row < 2; row++) {
			typedef std::map<uint32_t, uint32_t> ColumnMaxFrame;
			ColumnMaxFrame maxFrames[Virtex::eFarBlockTypeCount];
			Virtex::const_iterator p = bitstream.begin();
			Virtex::const_iterator e = bitstream.end();
			uint32_t header = VirtexPacket::makeHeader(VirtexPacket::ePacketType1, 
				VirtexPacket::eOpcodeWrite, Virtex::eRegisterLOUT, 1);
			while(p < e) {
				const VirtexPacket& packet = *p++;
				if(packet.getHeader() != header) continue;
				Virtex::FrameAddress far = packet[1];
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
			for(uint32_t i = 0; i < Virtex::eFarBlockTypeCount; i++) {
				Virtex::EFarBlockType blockType = Virtex::EFarBlockType(i);
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
		Virtex::const_iterator p = bitstream.begin();
		Virtex::const_iterator e = bitstream.end();
		uint32_t header = VirtexPacket::makeHeader(VirtexPacket::ePacketType1, 
			VirtexPacket::eOpcodeWrite, Virtex::eRegisterLOUT, 1);
		while(p < e) {
			const VirtexPacket& packet = *p++;
			if(packet.getHeader() != header) continue;
			if(first) { first = false; continue; }
			Virtex::FrameAddress far = packet[1];
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
	*/

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
