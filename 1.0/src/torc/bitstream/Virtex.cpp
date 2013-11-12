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
/// \brief Source for the Virtex class.

#include "torc/bitstream/Virtex.hpp"
#include <iostream>

/// \todo Warning: this will need to be moved elsewhere.
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <fstream>


namespace torc {
namespace bitstream {

	const char* Virtex::sPacketTypeName[ePacketTypeCount] = {
		"[UNKNOWN TYPE 0]", "TYPE1", "TYPE2", "[UNKNOWN TYPE 3]", "[UNKNOWN TYPE 4]", 
		"[UNKNOWN TYPE 5]", "[UNKNOWN TYPE 6]", "[UNKNOWN TYPE 7]"
	};

		/// \see configuration data: XAPP151, v1.7, October 20, 2004, Figure 12.
	const char* Virtex::sOpcodeName[eOpcodeCount] = {
		"[UNKNOWN OP 0]", "READ", "WRITE", "[UNKNOWN OP 3]"
	};

	const char* Virtex::sRegisterName[eRegisterCount] = {
		"CRC", "FAR", "FDRI", "FDRO", "CMD", "CTL", "MASK", "STAT", "LOUT", "COR", "[UNKNOWN REG 0]", "FLR"
	};

	const char* Virtex::sCommandName[eCommandCount] = {
		"[UNKNOWN CMD 0]", "WCFG", "", "LFRM", "RCFG", "START", "RCAP", "RCRC", "AGHIGH", "SWITCH"
	};

#define VALUES (const char*[])

	/// \see Configuration Options Register Description: XAPP151, v1.7, October 20, 2004, Table 20.
	const Bitstream::Subfield Virtex::sCOR[] = { 
		{0x00000007,  0, "GSR_cycle", "GSR_CYCLE", 5,
			// bitgen: 6, 1, 2, 3, 4, 5, Done, Keep
			// config: 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x00000038,  3, "GWE_cycle", "GWE_CYCLE", 5,
			// bitgen: 6, 1, 2, 3, 4, 5, Done, Keep
			// config: 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x000001c0,  6, "GTS_cycle", "GTS_CYCLE", 4,
			// bitgen: 5, 1, 2, 3, 4, 6, Done, Keep
			// config: 000:"1", 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x00000e00,  9, "LCK_cycle", "LCK_CYCLE", 7,
			// bitgen: NoWait, 0, 1, 2, 3, 4, 5, 6
			// config: 000:"1", 001:"2", 010:"3", 011:"4", 100:"5", 101:"6", 111:"NO_WAIT"
			VALUES{"1", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "NoWait", 0}},
		{0x00007000, 12, "DONE_cycle", "DONE_CYCLE", 3,
			// bitgen: 4, 1, 2, 3, 5, 6
			// config: 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x00008000, 15, "", "SHUTDOWN", 0,
			// config: 0:"Startup", 1:"Shutdown sequence"
			VALUES{"0", "1", 0}},
		{0x000f0000, 16, "", "LOCK_WAIT", 0,
			// config: 000:"bottom-right", 001:"bottom-left", 010:"top-right", 011:"top-left"
			VALUES{"1", "2", "3", "4", 0}},
		{0x00300000, 20, "StartupClk", "SSCLKSRC", 0,
			// bitgen: Cclk, UserClk, JtagClk
			// config: 00:"CCLK", 01:"UserClk", 1x:"JTAGClk"
			VALUES{"Cclk", "UserClk", "JtagClk", "JtagClk", 0}},
		{0x0fc00000, 22, "ConfigRate", "OSCFSEL", 2,
			// bitgen: 4, 5, 7, 8, 9, 10, 13, 15, 20, 26, 30, 34, 41, 45, 51, 55, 60
			// config: values undefined
			VALUES{
				"[UNKNOWN 0]", "[UNKNOWN 1]", "[UNKNOWN 2]", "[UNKNOWN 3]", 
				"[UNKNOWN 4]", "[UNKNOWN 5]", "[UNKNOWN 6]", "[UNKNOWN 7]", 
				"[UNKNOWN 8]", "[UNKNOWN 9]", "[UNKNOWN 10]", "[UNKNOWN 11]", 
				"[UNKNOWN 12]", "[UNKNOWN 13]", "[UNKNOWN 14]", "[UNKNOWN 15]", 
				"[UNKNOWN 16]", "[UNKNOWN 17]", "[UNKNOWN 18]", "[UNKNOWN 19]", 
				"[UNKNOWN 20]", "[UNKNOWN 21]", "[UNKNOWN 22]", "[UNKNOWN 23]", 
				"[UNKNOWN 24]", "[UNKNOWN 25]", "[UNKNOWN 26]", "[UNKNOWN 27]", 
				"[UNKNOWN 28]", "[UNKNOWN 29]", "[UNKNOWN 30]", "[UNKNOWN 31]", 
			0}},
		{0x10000000, 28, "Capture", "SINGLE", 0,
			// bitgen: n/a -- this comes from the CAPTURE site ONESHOT setting
			// config: 0:"Readback is not single-shot", 1:"Readback is single-shot"
			VALUES{"Continuous", "OneShot", 0}},
		{0x20000000, 29, "DriveDone", "DRIVE_DONE", 0,
			// bitgen: No, Yes
			// config: 0:"DONE pin is open drain", 1:"DONE is actively driven high"
			VALUES{"No", "Yes", 0}}, 
		{0x40000000, 30, "DonePipe", "DONE_PIPE", 0,
			// bitgen: No, Yes
			// config: 0:"No pipeline stage for DONEIN", 1:"Add pipeline stage for DONEIN"
			VALUES{"No", "Yes", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Status Register Description: XAPP151, v1.7, October 20, 2004, Table 25.
	/// \note The "bitgen" names attempt to mimic the general bitgen convention.
	const Bitstream::Subfield Virtex::sSTAT[] = { 
		{0x00000001,  0, "CRC_error", "CRC_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"No CRC error", 1:"CRC error"
			VALUES{"No", "Yes", 0}},
		{0x00000002,  1, "TR_locked", "TR_LOCK", 0, 
			// TR - top right
			// bitgen: n/a
			// config: 0:"TR not locked", 1:"TR locked"
			VALUES{"0", "1", 0}},
		{0x00000004,  2, "TL_locked", "TL_LOCK", 0, 
			// TL - top left
			// bitgen: n/a
			// config: 0:"TL not locked", 1:"TL locked"
			VALUES{"0", "1", 0}},
		{0x00000008,  3, "BR_locked", "BR_LOCK", 0, 
			// BR - bottom right
			// bitgen: n/a
			// config: 0:"BR locked", 1:"BR locked
			VALUES{"0", "1", 0}},
		{0x00000010,  4, "BL_locked", "BL_LOCK", 0,
			// BL - bottom left
			// bitgen: n/a
			// config: 0:"BL locked", 1:"BL locked"
			VALUES{"No", "Yes", 0}},
		{0x00000020,  5, "IN_error", "IN_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"Not In Error", 1:"In Error"
			VALUES{"0", "1", 0}},
		{0x00000040,  6, "GTS_cfg", "GTS_CFG", 0, 
			// bitgen: n/a
			// config: 0:"I/O's tri-stated", 1:"I/O's not tri-stated"
			VALUES{"0", "1", 0}},
		{0x00000080,  7, "GWE_B", "GWE_B", 0, 
			// bitgen: n/a
			// config: 0:"FF & BRAM are write enabled", 1:"FF & BRAM are write disabled"
			VALUES{"0", "1", 0}},
		{0x00000100,  8, "GSR_B", "GSR_B", 0, 
			// bitgen: n/a
			// config: 0:"all FFs are Reset/Set", 1:"all FFs are not Reset/set"
			VALUES{"0", "1", 0}},
		{0x00000200, 9, "GHIGH_B", "GHIGH_B", 0, 
			// bitgen: n/a
			// config: 0:"GHIGH_B asserted", 1:"GHIGH_B not asserted"
			VALUES{"0", "1", 0}},
		{0x000001c0, 10, "MODE", "MODE", 0, 
			// bitgen: n/a
			// config: Status of the MODE pins (M2:M0)
			VALUES{"MasterSerial", "SlaveSelectMap32", "[UNDEFINED 2]", "MasterSelectMap", 
				"[UNDEFINED 3]", "JTAG", "SlaveSelectMap8", "[UNDEFINED 6]", "SlaveSerial", 0}},
		{0x00002000, 13, "INIT", "INIT", 0, 
			// bitgen: n/a
			// config: Value on INIT pin
			VALUES{"Deasserted", "Asserted", 0}},
		{0x00004000, 14, "DONE", "DONE", 0, 
			// bitgen: n/a
			// config: Value on DONE pin
			VALUES{"NotDone", "Done", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Status Register Description: XAPP151, v1.7, October 20, 2004, Table 23.
	const Bitstream::Subfield Virtex::sCTL[] = { 
		{0x00000001,  0, "GTS_USER_B", "GTS_USER_B", 0, 
			// bitgen: n/a?
			// config: 0:"I/Os placed in high-Z state", 1:"I/Os active"
			VALUES{"IoDisabled", "IoActive", 0}},
		{0x00000040,  6, "Persist", "PERSIST", 0, 
			// bitgen: No, Yes
			// config: 0:"No (default)", 1:"Yes"
			VALUES{"No", "Yes", 0}},
		{0x00000180,  7, "Security", "SBITS", 0, 
			// bitgen: None, Level1, Level2
			// config: 00:"Read/Write OK (default)", 01:"Readback disabled", 1x:"Readback disabled, 
			//	writing disabled except CRC register."
			VALUES{"None", "Level1", "Level2", "Level2", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Control Mask Register Description: Inferred from Table 23.
	const Bitstream::Subfield Virtex::sMASK[] = { 
		{0x00000001,  0, "GTS_USER_B", "GTS_USER_B", 0, VALUES{"Protected", "Writable", 0}},
		{0x00000040,  6, "Persist", "PERSIST", 0, VALUES{"Protected", "Writable", 0}},
		{0x00000180,  7, "Security", "SBITS", 0, 
			VALUES{"Protected", "[UNKNOWN 1]", "[UNKNOWN 2]", "Writable", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \brief Return the masked value for a subfield of the specified register.
	uint32_t Virtex::makeSubfield(ERegister inRegister, const std::string& inSubfield, 
		const std::string& inSetting) {
		const Subfield* subfields;
		switch(inRegister) {
		case eRegisterCOR: subfields = sCOR; break;
		case eRegisterSTAT: subfields = sSTAT; break;
		case eRegisterCTL: subfields = sCTL; break;
		case eRegisterMASK: subfields = sMASK; break;
		default: return 0;
		}
		for(uint32_t field = 0; subfields[field].mMask != 0; field++) {
			const Subfield& subfield = subfields[field];
			if(inSubfield != subfield.mBitgenName && inSubfield != subfield.mConfigGuideName) 
				continue;
			const char** ptr = subfield.mValues;
			for(uint32_t i = 0; *ptr != 0; i++, ptr++) {
				if(inSetting == *ptr) return (i << subfield.mShift) & subfield.mMask;
			}
		}
		return 0;
	}


//#define GENERATE_STATIC_DEVICE_INFO
#ifndef GENERATE_STATIC_DEVICE_INFO

	extern DeviceInfo xcv50;
	extern DeviceInfo xcv100;
	extern DeviceInfo xcv150;
	extern DeviceInfo xcv200;
	extern DeviceInfo xcv300;
	extern DeviceInfo xcv400;
	extern DeviceInfo xcv600;
	extern DeviceInfo xcv800;
	extern DeviceInfo xcv1000;

	void Virtex::initializeDeviceInfo(const std::string& inDeviceName) {
		using namespace torc::common;
		switch(mDevice) {
			case eXCV50: setDeviceInfo(xcv50); break;
			case eXCV100: setDeviceInfo(xcv100); break;
			case eXCV150: setDeviceInfo(xcv150); break;
			case eXCV200: setDeviceInfo(xcv200); break;
			case eXCV300: setDeviceInfo(xcv300); break;
			case eXCV400: setDeviceInfo(xcv400); break;
			case eXCV600: setDeviceInfo(xcv600); break;
			case eXCV800: setDeviceInfo(xcv800); break;
			case eXCV1000: setDeviceInfo(xcv1000); break;
			default: break;
		}
		//setRowCounts(inDeviceName);
	}

#else

	void Virtex::initializeDeviceInfo(const std::string& inDeviceName) {

		typedef torc::architecture::xilinx::TileCount TileCount;
		typedef torc::architecture::xilinx::TileRow TileRow;
		typedef torc::architecture::xilinx::TileCol TileCol;
		typedef torc::architecture::xilinx::TileTypeIndex TileTypeIndex;
		typedef torc::architecture::xilinx::TileTypeCount TileTypeCount;

		// look up the device tile map
		torc::architecture::DDB ddb(inDeviceName);
		const torc::architecture::Tiles& tiles = ddb.getTiles();
		uint32_t tileCount = tiles.getTileCount();
		uint16_t rowCount = tiles.getRowCount();
		uint16_t colCount = tiles.getColCount();
		ColumnTypeVector columnTypes;

		// set up the tile index and name mappings, and the index to column def mapping
		typedef std::map<TileTypeIndex, std::string> TileTypeIndexToName;
		typedef std::map<std::string, TileTypeIndex> TileTypeNameToIndex;
		TileTypeIndexToName tileTypeIndexToName;
		TileTypeNameToIndex tileTypeNameToIndex;
		TileTypeCount tileTypeCount = tiles.getTileTypeCount();
		for(TileTypeIndex tileTypeIndex(0); tileTypeIndex < tileTypeCount; tileTypeIndex++) {
			const std::string tileTypeName = tiles.getTileTypeName(tileTypeIndex);
			tileTypeIndexToName[tileTypeIndex] = tileTypeName;
			tileTypeNameToIndex[tileTypeName] = tileTypeIndex;
			TileTypeNameToColumnType::iterator ttwp = mTileTypeNameToColumnType.find(tileTypeName);
			TileTypeNameToColumnType::iterator ttwe = mTileTypeNameToColumnType.end();
			if(ttwp != ttwe) mTileTypeIndexToColumnType[tileTypeIndex] = EColumnType(ttwp->second);
		}

		// identify every column that contains known frames
		columnTypes.resize(colCount);
		uint32_t frameCount = 0;
		for(uint32_t blockType = 0; blockType < Virtex::eFarBlockTypeCount; blockType++) {
			for(TileCol col; col < colCount; col++) {
				bool found = false;
				columnTypes[col] = eColumnTypeEmpty;
				TileTypeIndexToColumnType::iterator ttwe = mTileTypeIndexToColumnType.end();
				TileTypeIndexToColumnType::iterator ttwp = ttwe;
				for(TileRow row; row < rowCount; row++) {
					// look up the tile info
					const torc::architecture::TileInfo& tileInfo 
						= tiles.getTileInfo(tiles.getTileIndex(row, col));
					TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
					// determine whether the tile type widths are defined
					ttwp = mTileTypeIndexToColumnType.find(tileTypeIndex);
					if(ttwp != ttwe) {
						uint32_t width = mColumnDefs[ttwp->second][blockType];
						frameCount += width;
						//std::cout << "    " << tiles.getTileTypeName(tileInfo.getTypeIndex()) 
						 //<< ": " << width << " (" << frameCount << ")" << std::endl;
						columnTypes[col] = static_cast<EColumnType>(ttwp->second);
						found = true;
						break;
					}
				}
				(void) found;
			}
			//std::cout << std::endl;
			if(blockType == 2) break;
		}

		boost::filesystem::path workingPath = torc::common::DirectoryTree::getWorkingPath();
		boost::filesystem::path generatedMap = workingPath / (inDeviceName + ".map.csv");
		std::fstream tilemapStream(generatedMap.string().c_str(), std::ios::out);
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

		// update bitstream device information
		setDeviceInfo(DeviceInfo(tileCount, rowCount, colCount, columnTypes));
		// update the frame length
	}

#endif

	void Virtex::initializeFrameMaps(void) {
		
	    bool debug = 0;
	    int center = 0;
		int frameIndex = 0;
		int frameCount = 0;
		int farMajor = 0;
		int width = 0;
		ColumnIndex col;
		for(uint32_t i = 0; i < Virtex::eFarBlockTypeCount; i++) {
			farMajor = 0;
			EFarBlockType blockType = Virtex::EFarBlockType(i);
			mFrameIndexBounds = 0;
			//Set first frame index to 0
			uint32_t bitIndex = 0;
			uint32_t xdlIndex = 0;
			mBitColumnIndexes[i].push_back(bitIndex);
			mXdlColumnIndexes[i].push_back(xdlIndex);
			uint16_t finalColumn = mDeviceInfo.getColCount()-1;
			uint32_t xdlColumnCount = 0;
			uint32_t bitColumnCount = 0;
			// Clock Column at the middle
			center = mDeviceInfo.getColCount() / 2;
			col = center;
			prepareFrames(col, frameCount, frameIndex, blockType, farMajor, width);
			int numBrams = 2;
			int numIobs = 2;
			int numClocks = 1;
			int numClbs = mDeviceInfo.getColCount() - numClocks - numBrams - numIobs;
			// CLB Columns alternate around the clock column
			for(int j = 1; j <= numClbs / 2; j++) {
				for(int k = -1; k < 2; k += 2) {
					col = center - (j * k);
					prepareFrames(col, frameCount, frameIndex, blockType, farMajor, width);
					//Indexes for Bitstream Columns, only stores non-empty tile types
					if(mDeviceInfo.getColumnTypes()[col] != Virtex::eColumnTypeEmpty) {
						mXdlColumnToBitColumn[bitColumnCount] = xdlColumnCount;
						bitColumnCount++;
						bitIndex += width;
						mBitColumnIndexes[i].push_back(bitIndex);
						if(col == finalColumn) {
							bitIndex += mColumnDefs[mDeviceInfo.getColumnTypes()[col]][i];
							mBitColumnIndexes[i].push_back(bitIndex);
						}
					}
					//Indexes for XDL Columns, stores interconnect and tile indexes for
					//non-empty tiles
					xdlIndex += width;
					mXdlColumnIndexes[i].push_back(xdlIndex);
					xdlColumnCount++;
					if(col == finalColumn)
					{    
						xdlIndex += mColumnDefs[mDeviceInfo.getColumnTypes()[col]][i];
						mXdlColumnIndexes[i].push_back(xdlIndex);
					}
				}
			}
			// IOB Columns alternate after the CLB's
			for(int j = center; j < (center + 1); j++) {
				for(int k = -1; k < 2; k += 2) {
					col = center - (j * k);
					prepareFrames(col, frameCount, frameIndex, blockType, farMajor, width);
					//Indexes for Bitstream Columns, only stores non-empty tile types
					if(mDeviceInfo.getColumnTypes()[col] != Virtex::eColumnTypeEmpty) {
						mXdlColumnToBitColumn[bitColumnCount] = xdlColumnCount;
						bitColumnCount++;
						bitIndex += width;
						mBitColumnIndexes[i].push_back(bitIndex);
						if(col == finalColumn) {
							bitIndex += mColumnDefs[mDeviceInfo.getColumnTypes()[col]][i];
							mBitColumnIndexes[i].push_back(bitIndex);
						}
					}
					//Indexes for XDL Columns, stores interconnect and tile indexes for
					//non-empty tiles
					xdlIndex += width;
					mXdlColumnIndexes[i].push_back(xdlIndex);
					xdlColumnCount++;
					if(col == finalColumn)
					{    
						xdlIndex += mColumnDefs[mDeviceInfo.getColumnTypes()[col]][i];
						mXdlColumnIndexes[i].push_back(xdlIndex);
					}
				}
			}
			// BRAM Columns alternate after the IOB's
			for(int j = (center - 1); j < center; j++) {
				for(int k = -1; k < 2; k += 2) {
					col = center - (j * k);
					prepareFrames(col, frameCount, frameIndex, blockType, farMajor, width);
					//Indexes for Bitstream Columns, only stores non-empty tile types
					if(mDeviceInfo.getColumnTypes()[col] != Virtex::eColumnTypeEmpty) {
						mXdlColumnToBitColumn[bitColumnCount] = xdlColumnCount;
						bitColumnCount++;
						bitIndex += width;
						mBitColumnIndexes[i].push_back(bitIndex);
						if(col == finalColumn) {
							bitIndex += mColumnDefs[mDeviceInfo.getColumnTypes()[col]][i];
							mBitColumnIndexes[i].push_back(bitIndex);
						}
					}
					//Indexes for XDL Columns, stores interconnect and tile indexes for
					//non-empty tiles
					xdlIndex += width;
					mXdlColumnIndexes[i].push_back(xdlIndex);
					xdlColumnCount++;
					if(col == finalColumn)
					{    
						xdlIndex += mColumnDefs[mDeviceInfo.getColumnTypes()[col]][i];
						mXdlColumnIndexes[i].push_back(xdlIndex);
					}
				}
			}
			//stores frame index bounds for each block type
			mBlockFrameIndexBounds[i] = mFrameIndexBounds;
			if(debug) std::cout << "***Block frame index bounds: " << mBlockFrameIndexBounds[i] << std::endl;
		}
		//Test to check proper indexing
		if(debug) {
  		    for(uint32_t i = 0; i < Virtex::eFarBlockTypeCount; i++) {
  			    for(uint32_t j = 0; j < mBitColumnIndexes[i].size(); j++) 
			        std::cout << "Bit Value at index: (" << i << ", " << j << ") : " << mBitColumnIndexes[i][j] << std::endl;
			    for(uint32_t k = 0; k < mXdlColumnIndexes[i].size(); k++)
			        std::cout << "Xdl Value at index: (" << i << ", " << k << ") : " << mXdlColumnIndexes[i][k] << std::endl;
		    }
		}
	}

	void Virtex::prepareFrames(ColumnIndex &inCol, int &inFrameCount, int &inFrameIndex, EFarBlockType &inBlockType, int &inFarMajor, int &inWidth) {
		inWidth = mColumnDefs[mDeviceInfo.getColumnTypes()[inCol]][inBlockType];
		if(inWidth == 0) return;
		inFrameCount += inWidth;
		for(int farMinor = 0; farMinor < inWidth; farMinor++) {
			Virtex::FrameAddress far(inBlockType, inFarMajor, farMinor);
		    //std::cout << inBlockType << "(" << inFarMajor << "," << farMinor << ")" << std::endl;
			mFrameIndexToAddress[inFrameIndex] = far;
			mFrameAddressToIndex[far] = inFrameIndex;
			inFrameIndex++;
		    mFrameIndexBounds++;
		}
		if(inWidth > 0) inFarMajor++;
		return;
	}


	void Virtex::initializeFullFrameBlocks (void) {
		boost::shared_array<uint32_t> frameWords;
		// walk the bitstream and extract all frames 
		Virtex::iterator p = begin();
		Virtex::iterator e = end();
		while(p < e) {
		    const VirtexPacket& packet = *p++;
		    if(packet.isType2() && packet.isWrite()) 
  				frameWords = packet.getWords();
		}
		uint32_t index = 0;
		for(uint32_t i = 0; i < Bitstream::eBlockTypeCount; i++) {
			// all frames of block type are extracted
			for(uint32_t j = 0; j < mBlockFrameIndexBounds[i]; j++) {
				mFrameBlocks.mBlock[i].push_back(VirtexFrameSet::FrameSharedPtr
					(new VirtexFrame(getFrameLength(), &frameWords[index])));
				index += getFrameLength();
			}
		}
	}


	VirtexFrameBlocks Virtex::getBitstreamFrames (uint32_t inBlockCount, uint32_t inBitCol) {

		// index and extract frames
		int32_t bitColumnIndex[inBlockCount];
		int32_t bitColumnBound[inBlockCount];

		for(uint32_t i = 0; i < inBlockCount; i++) {
			// column Index of given frame index
			bitColumnIndex[i] = mBitColumnIndexes[i][inBitCol];
			// frame bounds for given column type
			bitColumnBound[i] = mColumnDefs[mDeviceInfo.getColumnTypes()[inBitCol]][i];
		}
		// extract the tile frames for the specified FAR 
		VirtexFrameBlocks frameBlocks;
		for(uint32_t i = 0; i < inBlockCount; i++) {
		    int startIndex = bitColumnIndex[i];
		    for(int j = 0; j < bitColumnBound[i]; j++)
				frameBlocks.mBlock[i].push_back(mFrameBlocks.mBlock[i][startIndex+j]);
		}
		return frameBlocks;
	}

	VirtexFrameBlocks Virtex::getXdlFrames (uint32_t inBlockCount, uint32_t inXdlCol) {

		// index and extract frames
		int32_t xdlColumnIndex[inBlockCount];
		int32_t xdlColumnBound[inBlockCount];
		for(uint32_t i = 0; i < inBlockCount; i++) {
			// column Index of given frame index
			xdlColumnIndex[i] = mXdlColumnIndexes[i][inXdlCol];
			// frame bounds for given column type
			xdlColumnBound[i] = 
				mColumnDefs[mDeviceInfo.getColumnTypes()[mXdlColumnToBitColumn[inXdlCol]]][i];
		}
		// extract the tile frames for the specified FAR 
		VirtexFrameBlocks frameBlocks;
		for(uint32_t i = 0; i < inBlockCount; i++) {
		    int startIndex = xdlColumnIndex[i];
		    for(int j = 0; j < xdlColumnBound[i]; j++)
				frameBlocks.mBlock[i].push_back(mFrameBlocks.mBlock[i][startIndex+j]);
		}
		return frameBlocks;
	}
} // namespace bitstream
} // namespace torc
