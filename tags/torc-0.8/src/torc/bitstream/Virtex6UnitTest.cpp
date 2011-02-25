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
/// \brief Source for the Virtex6 unit test.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/Virtex6.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/DeviceDesignator.hpp"
#include "torc/bitstream/OutputStreamHelpers.hpp"
#include "torc/common/TestHelpers.hpp"
#include <fstream>
#include <iostream>

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)


/// \brief Unit test for the Virtex6 class.
BOOST_AUTO_TEST_CASE(bitstream_virtex6) {

	// enums tested:
	//		EPacket
	//		EFar
	boost::uint32_t mask;
	// type 1 packet subfield masks
	mask = Virtex6::ePacketMaskType + Virtex6::ePacketMaskOpcode 
		+ Virtex6::ePacketMaskType1Address + Virtex6::ePacketMaskType1Reserved 
		+ Virtex6::ePacketMaskType1Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// type 2 packet subfield masks
	mask = Virtex6::ePacketMaskType + Virtex6::ePacketMaskOpcode 
		+ Virtex6::ePacketMaskType2Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// frame address register subfield masks
	mask = Virtex6::eFarMaskBlockType + Virtex6::eFarMaskTopBottom + Virtex6::eFarMaskRow 
		+ Virtex6::eFarMaskMajor + Virtex6::eFarMaskMinor;
	BOOST_CHECK_EQUAL(mask, 0x00FFFFFFu);

	// members tested:
	//		Virtex6::sPacketTypeName and EPacketTypeName
	BOOST_CHECK_EQUAL(Virtex6::sPacketTypeName[0],							"[UNKNOWN TYPE 0]");
	BOOST_CHECK_EQUAL(Virtex6::sPacketTypeName[Virtex6::ePacketType1],		"TYPE1");
	BOOST_CHECK_EQUAL(Virtex6::sPacketTypeName[Virtex6::ePacketType2],		"TYPE2");
	BOOST_CHECK_EQUAL(Virtex6::sPacketTypeName[3],							"[UNKNOWN TYPE 3]");
	BOOST_CHECK_EQUAL(Virtex6::sPacketTypeName[4],							"[UNKNOWN TYPE 4]");
	BOOST_CHECK_EQUAL(Virtex6::sPacketTypeName[5],							"[UNKNOWN TYPE 5]");
	BOOST_CHECK_EQUAL(Virtex6::sPacketTypeName[6],							"[UNKNOWN TYPE 6]");
	BOOST_CHECK_EQUAL(Virtex6::sPacketTypeName[7],							"[UNKNOWN TYPE 7]");

	// members tested:
	//		Virtex6::sOpcodeName and EOpcode
	BOOST_CHECK_EQUAL(Virtex6::sOpcodeName[Virtex6::eOpcodeNOP],			"NOP");
	BOOST_CHECK_EQUAL(Virtex6::sOpcodeName[Virtex6::eOpcodeRead],			"READ");
	BOOST_CHECK_EQUAL(Virtex6::sOpcodeName[Virtex6::eOpcodeWrite],			"WRITE");
	BOOST_CHECK_EQUAL(Virtex6::sOpcodeName[Virtex6::eOpcodeReserved],		"RESERVED");

	// members tested:
	//		Virtex6::sRegisterName and ERegister
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterCRC],		"CRC");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterFAR],		"FAR");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterFDRI],		"FDRI");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterFDRO],		"FDRO");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterCMD],		"CMD");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterCTL0],		"CTL0");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterMASK],		"MASK");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterSTAT],		"STAT");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterLOUT],		"LOUT");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterCOR0],		"COR0");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterMFWR],		"MFWR");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterCBC],		"CBC");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterIDCODE],		"IDCODE");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterAXSS],		"AXSS");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterCOR1],		"COR1");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterCSOB],		"CSOB");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterWBSTAR],		"WBSTAR");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterTIMER],		"TIMER");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterBOOTSTS],	"BOOTSTS");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterCTL1],		"CTL1");
	BOOST_CHECK_EQUAL(Virtex6::sRegisterName[Virtex6::eRegisterDWC],		"DWC");

	// members tested:
	//		Virtex6::sCommandName and ECommand
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandNULL],			"NULL");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandWCFG],			"WCFG");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandMFW],			"MFW");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandLFRM],			"DGHIGH/LFRM");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandRCFG],			"RCFG");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandSTART],		"START");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandRCAP],			"RCAP");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandRCRC],			"RCRC");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandAGHIGH],		"AGHIGH");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandSWITCH],		"SWITCH");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandGRESTORE],		"GRESTORE");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandSHUTDOWN],		"SHUTDOWN");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandGCAPTURE],		"GCAPTURE");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandDESYNCH],		"DESYNCH");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandReserved],		"Reserved");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandIPROG],		"IPROG");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandCRCC],		"CRCC");
	BOOST_CHECK_EQUAL(Virtex6::sCommandName[Virtex6::eCommandLTIMER],		"LTIMER");

	// build the file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "Virtex6UnitTest.generated.bit";
	boost::filesystem::path referencePath = regressionPath / "Virtex6UnitTest.reference.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	Virtex6 bitstream;
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








































void testVirtex6Device(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath);

/// \brief Unit test for the Virtex6 class Frame Address Register mapping.
BOOST_AUTO_TEST_CASE(bitstream_virtex6_far) {

	std::string devices[] = {
		// Virtex6 CXT
		"xc6vcx75t", "xc6vcx130t", "xc6vcx195t", "xc6vcx240t",
		// Virtex6 HXT
		"xc6vhx250t", "xc6vhx255t", "xc6vhx380t", "xc6vhx565t",
		// Virtex6 LXT
		"xc6vlx75t", "xc6vlx130t", "xc6vlx195t", "xc6vlx240t", "xc6vlx365t", "xc6vlx550t", 
			"xc6vlx760",
		// Virtex6 SXT
		"xc6vsx315t", "xc6vsx475t",

		// Virtex6L LXTL
		"xc6vlx75tl", "xc6vlx130tl", "xc6vlx195tl", "xc6vlx240tl", "xc6vlx365tl", "xc6vlx550tl", 
			"xc6vlx760l", 
		// Virtex6L SXTL
		"xc6vsx315tl", "xc6vsx475tl", 
		// termination
		""
	};

	for(int i = 0; ; i++) {
		std::string& device = devices[i];
		if(device.empty()) break;
std::cout << "device " << i << ": " << device << std::endl;
//if(i == 25) 
		testVirtex6Device(device, torc::common::DirectoryTree::getWorkingPath());
//break;
	}
}


















/*
2
NULL	NULL	BRKH	NULL	BRKH_IOI	BRKH	BRKH_CLB	BRKH	BRKH_CLB	NULL	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	NULL	BRKH_BRAM	NULL	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	NULL	BRKH_DSP	NULL	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	NULL	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	NULL	BRKH	NULL	NULL	NULL	CFG_CENTER	CLK_BUFGMUX	NULL	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	NULL	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	NULL	BRKH_BRAM	NULL	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	NULL	NULL	NULL	BRKH	NULL	NULL	BRKH_GT3
3
NULL	NULL	BRKH	NULL	BRKH_IOI	BRKH	BRKH_CLB	BRKH	BRKH_CLB	NULL	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	NULL	BRKH_BRAM	NULL	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	NULL	BRKH_DSP	NULL	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	NULL	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	NULL	BRKH	NULL	NULL	BRKH_IOI	NULL	GBRKC	NULL	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	NULL	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	NULL	BRKH_BRAM	NULL	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	BRKH_CLB	BRKH	NULL	PCIE_BRKH	NULL	BRKH	NULL	NULL	BRKH_GT3
*/





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

	std::ostream& operator<< (std::ostream& os, const Virtex6::FrameAddress& rhs);
	std::ostream& operator<< (std::ostream& os, const Virtex6::FrameAddress& rhs) {
		return os << (rhs.mTopBottom == Virtex6::eFarTop ? 'T' : 'B') << "" << rhs.mBlockType 
				<< "(" << rhs.mRow << "," << rhs.mMajor << "." << rhs.mMinor << ")";
	}

using namespace torc::architecture::xilinx;
void testVirtex6Device(const std::string& inDeviceName, const boost::filesystem::path& inWorkingPath) {

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
	Virtex6 bitstream;
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

	// BRAM types
	TileTypeIndex typeBram				= tileTypeNameToIndex["BRAM"];
	TileTypeIndex typePciEBram			= tileTypeNameToIndex["PCIE_BRAM"];
	// CLB types
	TileTypeIndex typeClbLL				= tileTypeNameToIndex["CLBLL"];
	TileTypeIndex typeClbLM				= tileTypeNameToIndex["CLBLM"];
	// Clock types
	TileTypeIndex typeClkV				= tileTypeNameToIndex["CLKV"];
	// DSP types
	TileTypeIndex typeDsp				= tileTypeNameToIndex["DSP"];
	// GTX types
	TileTypeIndex typeGtx				= tileTypeNameToIndex["GTX"];
	TileTypeIndex typeGtxLeft			= tileTypeNameToIndex["GTX_LEFT"];
	TileTypeIndex typeGt3				= tileTypeNameToIndex["GT3"];
	// IOB types
	TileTypeIndex typeCIob				= tileTypeNameToIndex["CIOB"];
	TileTypeIndex typeLIob				= tileTypeNameToIndex["LIOB"];
	TileTypeIndex typeRIob				= tileTypeNameToIndex["RIOB"];

	// BRAM columns
	tileTypeWidths[typeBram]			= TileTypeWidths(30,128,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typePciEBram]		= TileTypeWidths(30,128,  0,  0,  0,  0,  0,  0);
	// CLB columns
	tileTypeWidths[typeClbLL]			= TileTypeWidths(36,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeClbLM]			= TileTypeWidths(36,  0,  0,  0,  0,  0,  0,  0);
	// Clock columns
	tileTypeWidths[typeClkV]			= TileTypeWidths( 4,  0,  0,  0,  0,  0,  0,  0);
	// DSP columns
	tileTypeWidths[typeDsp]				= TileTypeWidths(28,  0,  0,  0,  0,  0,  0,  0);
	// GTX columns
	tileTypeWidths[typeGtx]				= TileTypeWidths(32,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeGtxLeft]			= TileTypeWidths(32,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeGt3]				= TileTypeWidths(32,  0,  0,  0,  0,  0,  0,  0);
	// IOB columns
	tileTypeWidths[typeCIob]			= TileTypeWidths(54,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeLIob]			= TileTypeWidths(54,  0,  0,  0,  0,  0,  0,  0);
	tileTypeWidths[typeRIob]			= TileTypeWidths(54,  0,  0,  0,  0,  0,  0,  0);

	uint32_t* columnWidths[8];
	for(uint32_t i = 0; i < Virtex6::eFarBlockTypeCount; i++) {
		columnWidths[i] = new uint32_t[static_cast<uint32_t>(colCount)];
		for(TileCol col; col < colCount; col++) columnWidths[i][col] = 0;
	}
//	TileRow referenceRow(11);
//std::cout << "Reference row: " << referenceRow << std::endl;
//	TileRow centerRow((rowCount >> 1) + 1);
//	uint32_t frameRowCount = (rowCount / 20) >> 1;
	uint32_t frameCount = 0;
	for(uint32_t i = 0; i < Virtex6::eFarBlockTypeCount; i++) {
		columnWidths[i] = new uint32_t[static_cast<uint32_t>(colCount)];
		Virtex6::EFarBlockType blockType = Virtex6::EFarBlockType(i);
		std::cout << "Block type " << blockType << std::endl;
		for(TileCol col; col < colCount; col++) {
			bool found = false;
			TileTypeIndexToWidths::iterator ttwe = tileTypeWidths.end();
			TileTypeIndexToWidths::iterator ttwp = ttwe;
			for(TileRow row; row < rowCount; row++) {
				// look up the tile info
				const torc::architecture::TileInfo& tileInfo 
					= tiles.getTileInfo(tiles.getTileIndex(row, col));
				TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
				// determine whether the tile type widths are defined
				TileTypeIndexToWidths::iterator ttwp = tileTypeWidths.find(tileTypeIndex);
				if(ttwp != ttwe) {
					uint32_t width = ttwp->second[blockType];
					columnWidths[i][col] = width;
					frameCount += width;
					std::cout << "    " << tiles.getTileTypeName(tileInfo.getTypeIndex()) << ": " 
						<< width << " (" << frameCount << ")" << std::endl;
					found = true;
					break;
				}
			}
			if(!found) {
			//	std::cout << "Unrecognized column  " << col << std::endl;
				ttwp->second = TileTypeWidths();
				columnWidths[i][col] = 0;
			}
		}
if(i == 2) break;
	}





	typedef std::map<uint32_t, Virtex6::FrameAddress> FrameIndexToAddress;
	typedef std::map<Virtex6::FrameAddress, uint32_t> FrameAddressToIndex;
	FrameIndexToAddress frameIndexToAddress;
	FrameAddressToIndex frameAddressToIndex;
	uint32_t farRowCount = (rowCount / 22) >> 1;
	// the xc5vlx20t has 3 total FAR rows, with the bottom half having 1 row, and the top having 2
	bool shortBottomHalf = (farRowCount * 22 << 1) + 1 < rowCount;
std::cout << "rowCount is " << rowCount << std::endl;
std::cout << "farRowCount is " << farRowCount << std::endl;
std::cout << "(rowCount * 22 << 1) + 1 is " << ((rowCount * 22 << 1) + 1) << std::endl;
std::cout << "shortBottomHalf is " << (shortBottomHalf ? "true" : "false") << std::endl;
if(shortBottomHalf) farRowCount++;
	uint32_t frameIndex = 0;
	for(uint32_t i = 0; i < Virtex6::eFarBlockTypeCount; i++) {
		Virtex6::EFarBlockType blockType = Virtex6::EFarBlockType(i);
		std::cout << "Block type " << blockType << std::endl;
//		TileRow row = referenceRow;
		for(uint32_t half = 0; half < 2; half++) {
			for(uint32_t farRow = 0; farRow < farRowCount; farRow++) {
				if(shortBottomHalf && half == 0 && (farRow + 1 == farRowCount)) continue;	// fix short bottom half on xc5vlx20t
				uint32_t farMajor = 0;
				for(TileCol col; col < colCount; col++) {
					uint32_t width = columnWidths[i][col];
//					// look up the tile info
//					const torc::architecture::TileInfo& tileInfo 
//						= tiles.getTileInfo(tiles.getTileIndex(referenceRow, col));
//					TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
//					// determine whether the tile type widths are defined
//					TileTypeIndexToWidths::iterator p = tileTypeWidths.find(tileTypeIndex);
//					if(p == tileTypeWidths.end()) {
//						std::cout << "Unknown " << tileTypeIndexToName[tileTypeIndex] 
//							<< " width at column " << col << std::endl;
//						p->second = TileTypeWidths();
//					}
//					uint32_t width = p->second[blockType];
					for(uint32_t farMinor = 0; farMinor < width; farMinor++) {
						Virtex6::FrameAddress far(Virtex6::EFarTopBottom(half), blockType, farRow, 
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
#if 0
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
#endif




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
			ColumnMaxFrame maxFrames[Virtex6::eFarBlockTypeCount];
			Virtex6::const_iterator p = bitstream.begin();
			Virtex6::const_iterator e = bitstream.end();
			uint32_t header = VirtexPacket::makeHeader(VirtexPacket::ePacketType1, 
				VirtexPacket::eOpcodeWrite, Virtex6::eRegisterLOUT, 1);
			while(p < e) {
				const VirtexPacket& packet = *p++;
				if(packet.getHeader() != header) continue;
				Virtex6::FrameAddress far = packet[1];
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
			for(uint32_t i = 0; i < Virtex6::eFarBlockTypeCount; i++) {
				Virtex6::EFarBlockType blockType = Virtex6::EFarBlockType(i);
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
		Virtex6::const_iterator p = bitstream.begin();
		Virtex6::const_iterator e = bitstream.end();
		uint32_t header = VirtexPacket::makeHeader(VirtexPacket::ePacketType1, 
			VirtexPacket::eOpcodeWrite, Virtex6::eRegisterLOUT, 1);
		while(p < e) {
			const VirtexPacket& packet = *p++;
			if(packet.getHeader() != header) continue;
			if(first) { first = false; continue; }
			Virtex6::FrameAddress far = packet[1];
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
std::cout << "device is " << inDeviceName << std::endl;
	BOOST_REQUIRE_EQUAL(frameAddressToIndex.size(), farVisited.size());
	BOOST_REQUIRE_EQUAL(farRemaining.size(), 0u);

	for(uint32_t i = 0; i < Virtex6::eFarBlockTypeCount; i++) {
		delete columnWidths[i];
	}


}















BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
