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
/// \brief Source for the Virtex4 class.

#include "torc/bitstream/Virtex4.hpp"
#include <iostream>

/// \todo Warning: this will need to be moved elsewhere.
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <fstream>


namespace torc {
namespace bitstream {

	const char* Virtex4::sPacketTypeName[ePacketTypeCount] = {
		"[UNKNOWN TYPE 0]", "TYPE1", "TYPE2", "[UNKNOWN TYPE 3]", "[UNKNOWN TYPE 4]", 
		"[UNKNOWN TYPE 5]", "[UNKNOWN TYPE 6]", "[UNKNOWN TYPE 7]"
	};

	const char* Virtex4::sOpcodeName[eOpcodeCount] = {
		"NOP", "READ", "WRITE", "RESERVED"
	};

	const char* Virtex4::sRegisterName[eRegisterCount] = {
		"CRC", "FAR", "FDRI", "FDRO", "CMD", "CTL", "MASK", "STAT", "LOUT", "COR", "MFWR", "CBC", 
		"IDCODE", "AXSS"
	};

	const char* Virtex4::sCommandName[eCommandCount] = {
		"NULL", "WCFG", "MFWR", "LFRM", "RCFG", "START", "RCAP", "RCRC", "AGHIGH", "SWITCH", 
		"GRESTORE", "SHUTDOWN", "GCAPTURE", "DESYNC"
	};

#define VALUES (const char*[])

	/// \see Configuration Options Register Description: UG071, v1.10, April 8, 2008, Table 7-10.
	const Bitstream::Subfield Virtex4::sCOR[] = { 
		{0x00000007,  0, "GWE_cycle", "GWE_CYCLE", 5,
			// bitgen: 6, 1, 2, 3, 4, 5, Done, Keep
			// config: 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x00000038,  3, "GTS_cycle", "GTS_CYCLE", 4,
			// bitgen: 5, 1, 2, 3, 4, 6, Done, Keep
			// config: 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x000001c0,  6, "LCK_cycle", "LOCK_CYCLE", 7,
			// bitgen: NoWait, 0, 1, 2, 3, 4, 5, 6
			// config: 000:"1", 001:"2", 010:"3", 011:"4", 100:"5", 101:"6", 111:"NO_WAIT"
			VALUES{"1", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "NoWait", 0}},
		{0x00000E00,  9, "Match_cycle", "MATCH_CYCLE", 7,
			// bitgen: Auto, NoWait, 0, 1, 2, 3, 4, 5, 6
			// config: 000:"1", 001:"2", 010:"3", 011:"4", 100:"5", 101:"6", 111:"NO_WAIT"
			VALUES{"1", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "NoWait", 0}},
		{0x00007000, 12, "DONE_cycle", "DONE_CYCLE", 3,
			// bitgen: 4, 1, 2, 3, 5, 6
			// config: 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x00018000, 15, "StartupClk", "SSCLKSRC", 0,
			// bitgen: Cclk, UserClk, JtagClk
			// config: 00:"CCLK", 01:"UserClk", 1x:"JTAGClk"
			VALUES{"Cclk", "UserClk", "JtagClk", "JtagClk", 0}},
		{0x007e0000, 17, "ConfigRate", "OSCFSEL", 0,
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
		{0x00800000, 23, "Capture", "SINGLE", 0,
			// bitgen: n/a -- this comes from the CAPTURE site ONESHOT setting
			// config: 0:"Readback is not single-shot", 1:"Readback is single-shot"
			VALUES{"Continuous", "OneShot", 0}},
		{0x01000000, 24, "DriveDone", "DRIVE_DONE", 0,
			// bitgen: No, Yes
			// config: 0:"DONE pin is open drain", 1:"DONE is actively driven high"
			VALUES{"No", "Yes", 0}}, 
		{0x02000000, 25, "DonePipe", "DONE_PIPE", 0,
			// bitgen: No, Yes
			// config: 0:"No pipeline stage for DONEIN", 1:"Add pipeline stage for DONEIN"
			VALUES{"No", "Yes", 0}},
		{0x10000000, 28, "CRC", "CRC_BYPASS", 0,
			// bitgen: Enable, Disable
			// config: 0:"CRC enabled", 1:"CRC disabled"
			VALUES{"Enable", "Disable", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Status Register Description: UG071, v1.10, April 8, 2008, Table 7-9.
	/// \note The "bitgen" names attempt to mimic the general bitgen convention.
	const Bitstream::Subfield Virtex4::sSTAT[] = { 
		{0x00000001,  0, "CRC_error", "CRC_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"No CRC error", 1:"CRC error"
			VALUES{"No", "Yes", 0}},
		{0x00000002,  1, "DecryptorSecuritySet", "PART_SECURED", 0, 
			// bitgen: n/a
			// config: 0:"Decryptor security not set", 1:"Decryptor security set"
			VALUES{"No", "Yes", 0}},
		{0x00000004,  2, "DCM_locked", "DCM_LOCK", 0, 
			// bitgen: n/a
			// config: 0:"DCMs not locked", 1:"DCMs are locked"
			VALUES{"No", "Yes", 0}},
		{0x00000008,  3, "DCI_matched", "DCI_MATCH", 0, 
			// bitgen: n/a
			// config: 0:"DCI not matched", 1:"DCI matched
			VALUES{"No", "Yes", 0}},
		{0x00000010,  4, "StartupFinished", "EOS", 0, 
			// bitgen: n/a
			// config: 0:"Startup sequence has not finished", 1:"Startup sequence has finished"
			VALUES{"No", "Yes", 0}},
		{0x00000020,  5, "GTS_CFG_B", "GTS_CFG_B", 0, 
			// bitgen: n/a
			// config: 0:"All I/Os are placed in high-Z state", 1:"All I/Os behave as configured"
			VALUES{"IoDisabled", "IoEnabled", 0}},
		{0x00000040,  6, "GWE", "GWE", 0, 
			// bitgen: n/a
			// config: 0:"FFs and block RAM are write disabled", 1:"FFs and block RAM are write 
			//	enabled"
			VALUES{"WriteDisabled", "WriteEnabled", 0}},
		{0x00000080,  7, "GHIGH_B", "GHIGH_B", 0, 
			// bitgen: n/a
			// config: 0:"GHIGH_B asserted", 1:"GHIGH_B deasserted"
			VALUES{"InterconnectDisabled", "InterconnectEnabled", 0}},
		{0x00000700,  8, "Mode", "MODE", 0, 
			// bitgen: n/a
			// config: Status of the MODE pins (M2:M0)
			VALUES{"MasterSerial", "SlaveSelectMap32", "[UNDEFINED 2]", "MasterSelectMap", 
				"[UNDEFINED 3]", "JTAG", "SlaveSelectMap8", "[UNDEFINED 6]", "SlaveSerial", 0}},
		{0x00000800, 11, "INIT_complete", "INIT_COMPLETE", 0, 
			// bitgen: n/a
			// config: 0:"Initializations has not finished", 1:"Initialization finished"
			VALUES{"No", "Yes", 0}},
		{0x00001000, 12, "INIT", "INIT", 0, 
			// bitgen: n/a
			// config: Value on INIT pin
			VALUES{"Deasserted", "Asserted", 0}},
		{0x00002000, 13, "DONE_released", "RELEASE_DONE", 0, 
			// bitgen: n/a
			// config: 0:"DONE signal not released", 1:"DONE signal released"
			VALUES{"DrivenLow", "Released", 0}},
		{0x00004000, 14, "DONE", "DONE", 0, 
			// bitgen: n/a
			// config: Value on DONE pin
			VALUES{"NotDone", "Done", 0}},
		{0x00008000, 15, "ID_error", "ID_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"No IE_ERROR", 1:"ID_ERROR"
			VALUES{"NoError", "Error", 0}}, 
		{0x00010000, 16, "Decrypt_error", "DEC_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"No DEC_ERROR", 1:"DEC_ERROR"
			VALUES{"NoError", "Error", 0}},/*,
		{0x00020000, 17, "Reserved", "Reserved", 0, 
			// bitgen: n/a
			// config: undefined
			VALUES{0}}*/
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Control Register Description: UG071, v1.10, April 8, 2008, Table 7-7.
	const Bitstream::Subfield Virtex4::sCTL[] = { 
		{0x00000001,  0, "GTS_USER_B", "GTS_USER_B", 0, 
			// bitgen: n/a?
			// config: 0:"I/Os placed in high-Z state", 1:"I/Os active"
			VALUES{"IoDisabled", "IoActive", 0}},
		{0x00000008,  3, "Persist", "PERSIST", 0, 
			// bitgen: No, Yes
			// config: 0:"No (default)", 1:"Yes"
			VALUES{"No", "Yes", 0}},
		{0x00000030,  4, "Security", "SBITS", 0, 
			// bitgen: None, Level1, Level2
			// config: 00:"Read/Write OK (default)", 01:"Readback disabled", 1x:"Readback disabled, 
			//	writing disabled except CRC register."
			VALUES{"None", "Level1", "Level2", "Level2", 0}},
		{0x00000100,  8, "GLUTMASK", "GLUTMASK", 0, 
			// bitgen: n/a
			// config: 0:"Readback all 0s from SRL16 and Distributed RAM. Use with active device 
			//	readback.", 1:"Readback dynamic values from SRL16 and Distributed RAM. Use with 
			//	shutdown readback."
			VALUES{"Masked", "Dynamic", 0}},
		{0x40000000, 30, "ICAP_sel", "ICAP_SEL", 0, 
			// bitgen: n/a
			// config: 0:"Top ICAP Port Enabled (default)", 1:"Bottom ICAP Port Enabled"
			VALUES{"Top", "Bottom", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Control Mask Register Description: Inferred from Table 7-7.
	const Bitstream::Subfield Virtex4::sMASK[] = { 
		{0x00000001,  0, "GTS_USER_B", "GTS_USER_B", 0, VALUES{"Protected", "Writable", 0}},
		{0x00000008,  3, "Persist", "PERSIST", 0, VALUES{"Protected", "Writable", 0}},
		{0x00000080,  4, "Security", "SBITS", 0, 
			VALUES{"Protected", "[UNKNOWN 1]", "[UNKNOWN 2]", "Writable", 0}},
		{0x00000100,  8, "GLUTMASK", "GLUTMASK", 0, VALUES{"Protected", "Writable", 0}},
		{0x40000000, 30, "ICAP_sel", "ICAP_SEL", 0, VALUES{"Protected", "Writable", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \brief Return the masked value for a subfield of the specified register.
	uint32_t Virtex4::makeSubfield(ERegister inRegister, const std::string& inSubfield, 
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


//	bool Virtex4::readPackets(std::istream& inStream) {
//		uint32_t bitstreamWordLength = mBitstreamByteLength >> 2;
//		uint32_t cumulativeWordLength = 0;
//		while(cumulativeWordLength < bitstreamWordLength) {
//			push_back(VirtexPacket::read(inStream));
//			cumulativeWordLength += back().getWordSize();
//		}
//		return true;
//	}

//#define GENERATE_STATIC_DEVICE_INFO
#ifndef GENERATE_STATIC_DEVICE_INFO

	extern DeviceInfo xc4vfx12;
	extern DeviceInfo xc4vfx20;
	extern DeviceInfo xc4vfx40;
	extern DeviceInfo xc4vfx60;
	extern DeviceInfo xc4vfx100;
	extern DeviceInfo xc4vfx140;
	extern DeviceInfo xc4vlx15;
	extern DeviceInfo xc4vlx25;
	extern DeviceInfo xc4vlx40;
	extern DeviceInfo xc4vlx60;
	extern DeviceInfo xc4vlx80;
	extern DeviceInfo xc4vlx100;
	extern DeviceInfo xc4vlx160;
	extern DeviceInfo xc4vlx200;
	extern DeviceInfo xc4vsx25;
	extern DeviceInfo xc4vsx35;
	extern DeviceInfo xc4vsx55;

	void Virtex4::initializeDeviceInfo(const std::string& inDeviceName) {
		using namespace torc::common;
		switch(mDevice) {
			case eXC4VFX12: setDeviceInfo(xc4vfx12); break;
			case eXC4VFX20: setDeviceInfo(xc4vfx20); break;
			case eXC4VFX40: setDeviceInfo(xc4vfx40); break;
			case eXC4VFX60: setDeviceInfo(xc4vfx60); break;
			case eXC4VFX100: setDeviceInfo(xc4vfx100); break;
			case eXC4VFX140: setDeviceInfo(xc4vfx140); break;
			case eXC4VLX15: setDeviceInfo(xc4vlx15); break;
			case eXC4VLX25: setDeviceInfo(xc4vlx25); break;
			case eXC4VLX40: setDeviceInfo(xc4vlx40); break;
			case eXC4VLX60: setDeviceInfo(xc4vlx60); break;
			case eXC4VLX80: setDeviceInfo(xc4vlx80); break;
			case eXC4VLX100: setDeviceInfo(xc4vlx100); break;
			case eXC4VLX160: setDeviceInfo(xc4vlx160); break;
			case eXC4VLX200: setDeviceInfo(xc4vlx200); break;
			case eXC4VSX25: setDeviceInfo(xc4vsx25); break;
			case eXC4VSX35: setDeviceInfo(xc4vsx35); break;
			case eXC4VSX55: setDeviceInfo(xc4vsx55); break;
			default: break;
		}
	}

#else

	void Virtex4::initializeDeviceInfo(const std::string& inDeviceName) {

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
		mFrameRowCount = 0;
		for(uint32_t blockType = 0; blockType < eFarBlockTypeCount; blockType++) {
			for(TileCol col; col < colCount; col++) {
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
						mFrameRowCount += width;
						//std::cout << "    " << tiles.getTileTypeName(tileInfo.getTypeIndex()) 
						// << ": " << width << " (" << mFrameRowCount << ")" << std::endl;
						columnTypes[col] = static_cast<EColumnType>(ttwp->second);
						break;
					}
				}
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
	}

#endif

	void Virtex4::initializeFrameMaps(void) {

		bool debug = 0;
		uint32_t farRowCount = (mDeviceInfo.getRowCount() / 18) >> 1;
		uint32_t frameIndex = 0;
		for(uint32_t i = 0; i < Virtex4::eFarBlockTypeCount; i++) {
			Virtex4::EFarBlockType blockType = Virtex4::EFarBlockType(i);
			uint32_t blockFrameIndexBounds = 0;
			//Set first frame index to 0
			uint32_t bitIndex = 0;
			uint32_t xdlIndex = 0;
			mBitColumnIndexes[i].push_back(bitIndex);
			mXdlColumnIndexes[i].push_back(xdlIndex);
			bool blockUsed = false;
			for(uint32_t half = 0; half < 2; half++) {
				for(uint32_t farRow = 0; farRow < farRowCount; farRow++) {
					//build the columns
					uint32_t farMajor = 0;
					typedef torc::common::EncapsulatedInteger<uint16_t> ColumnIndex;
					uint16_t finalColumn = mDeviceInfo.getColCount()-1;
					uint32_t xdlColumnCount = 0;
					uint32_t bitColumnCount = 0;
					for(ColumnIndex col; col < mDeviceInfo.getColCount(); col++) {
						uint32_t width = mColumnDefs[mDeviceInfo.getColumnTypes()[col]][i];
						//Allocate the frame maps
						for(uint32_t farMinor = 0; farMinor < width; farMinor++) {
							Virtex4::FrameAddress far(Virtex4::EFarTopBottom(half), blockType, 
								farRow, farMajor, farMinor);
							mFrameIndexToAddress[frameIndex] = far;
							mFrameAddressToIndex[far] = frameIndex;
							frameIndex++;
							blockFrameIndexBounds++;
						}
						if(width > 0) {
							farMajor++;
							blockUsed = true;
						}

						//Extract frame indexes for 1 row
						if(farRow == 0 && half == 0) {
						    //Indexes for Bitstream Columns, only stores non-empty tile types
						    if(mDeviceInfo.getColumnTypes()[col] != Virtex4::eColumnTypeEmpty) {
								mXdlColumnToBitColumn[xdlColumnCount] = bitColumnCount;
							    bitColumnCount++;
							    bitIndex += width;
							    mBitColumnIndexes[i].push_back(bitIndex);
							    if(col == finalColumn) {
							        bitIndex += getRowPadFrames();
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
						        xdlIndex += getRowPadFrames();
						        mXdlColumnIndexes[i].push_back(xdlIndex);
						    }
						}
					}
					// account for two pad frames after each frame row
					if(blockUsed) {
						frameIndex += getRowPadFrames();
						blockFrameIndexBounds += getRowPadFrames();
					}
					if(debug) std::cout << "Last frame index:   [" << i << ", " << frameIndex 
						<< "]" << std::endl;
				}
			}
			//stores frame index bounds for each block type
			mBlockFrameIndexBounds[i] = blockFrameIndexBounds;
			if(debug) std::cout << "***Block frame index bounds: " << mBlockFrameIndexBounds[i] << std::endl;
		}
		//Test to check proper indexing
		if(debug) {
  		  for(uint32_t i = 0; i < Virtex4::eFarBlockTypeCount; i++) {
  			  for(uint32_t j = 0; j < mBitColumnIndexes[i].size(); j++) 
			      std::cout << "Bit Value at index: (" << i << ", " << j << ") : " << mBitColumnIndexes[i][j] << std::endl;
			  for(uint32_t k = 0; k < mXdlColumnIndexes[i].size(); k++)
			      std::cout << "Xdl Value at index: (" << i << ", " << k << ") : " << mXdlColumnIndexes[i][k] << std::endl;
		    }
		}
	}

	VirtexPacketVector Virtex4::generateFullBitstreamPrefix(void) {
		//	0000004b: DUMMY
		//	0000004f: SYNC
		//	00000053: NOP x 1
		//	00000057: TYPE1 WRITE CMD RCRC
		//	0000005f: NOP x 2
		//	00000067: TYPE1 WRITE COR: 00043fe5 (CRC:Enable, DonePipe:No, DriveDone:No, 
		//				Capture:Continuous, ConfigRate:[UNKNOWN 2], StartupClk:Cclk, DONE_cycle:4, 
		//				Match_cycle:NoWait, LCK_cycle:NoWait, GTS_cycle:5, GWE_cycle:6)
		//	0000006f: TYPE1 WRITE IDCODE: 01eb4093
		//	00000077: TYPE1 WRITE CMD SWITCH
		//	0000007f: NOP x 1
		//	00000083: TYPE1 WRITE MASK: 00000600 (ICAP_sel:Protected, GLUTMASK:Protected, 
		//				Security:Protected, Persist:Protected, GTS_USER_B:Protected)
		//	0000008b: TYPE1 WRITE CTL: 00000600 (ICAP_sel:Top, GLUTMASK:Masked, Security:None, 
		//				Persist:No, GTS_USER_B:IoDisabled)
		//	00000093: NOP x 1150
		//	0000128b: TYPE1 WRITE MASK: 00000600 (ICAP_sel:Protected, GLUTMASK:Protected, 
		//				Security:Protected, Persist:Protected, GTS_USER_B:Protected)
		//	00001293: TYPE1 WRITE CTL: 00000000 (ICAP_sel:Top, GLUTMASK:Masked, Security:None, 
		//				Persist:No, GTS_USER_B:IoDisabled)
		//	0000129b: TYPE1 WRITE CMD NULL
		//	000012a3: NOP x 1

		// declare the packet vector and define a NOP packet
		VirtexPacketVector packets;
		VirtexPacket nop(VirtexPacket::makeHeader(ePacketType1, eOpcodeNOP, 0, 0));
		VirtexPacket dummy(eSynchronizationDummy);
		VirtexPacket sync(eSynchronizationSync);
		// dummy and sync words
		packets.push_back(dummy);
		packets.push_back(VirtexPacket(eSynchronizationSync));
		packets.push_back(nop);
		// reset CRC command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandRCRC));
		packets.push_back(nop);
		packets.push_back(nop);
		// configuration options register
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCOR, 
				makeSubfield(eRegisterCOR, "CRC", "Enable") |
				makeSubfield(eRegisterCOR, "DonePipe", "No") |
				makeSubfield(eRegisterCOR, "DriveDone", "No") |
				makeSubfield(eRegisterCOR, "Capture", "Continuous") |
				makeSubfield(eRegisterCOR, "ConfigRate", "[UNKNOWN 2]") |
				makeSubfield(eRegisterCOR, "StartupClk", "Cclk") |
				makeSubfield(eRegisterCOR, "DONE_cycle", "4") |
				makeSubfield(eRegisterCOR, "Match_cycle", "NoWait") |
				makeSubfield(eRegisterCOR, "LCK_cycle", "NoWait") |
				makeSubfield(eRegisterCOR, "GTS_cycle", "5") |
				makeSubfield(eRegisterCOR, "GWE_cycle", "6") |
			0));
		// write the ID code
		packets.push_back(VirtexPacket::makeType1Write(eRegisterIDCODE, 0x00000000));
		// clock and rate switch command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandSWITCH));
		packets.push_back(nop);
		// control register mask
		packets.push_back(VirtexPacket::makeType1Write(eRegisterMASK, 
				makeSubfield(eRegisterMASK, "ICAP_sel", "Protected") |
				makeSubfield(eRegisterMASK, "GLUTMASK", "Protected") |
				makeSubfield(eRegisterMASK, "Security", "Protected") |
				makeSubfield(eRegisterMASK, "Persist", "Protected") |
				makeSubfield(eRegisterMASK, "GTS_USER_B", "Protected") |
				0x00000600 /* these bits are not defined in the configuration guide */ |
			0));
		// control register
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCTL, 
				makeSubfield(eRegisterCTL, "ICAP_sel", "Top") |
				makeSubfield(eRegisterCTL, "GLUTMASK", "Masked") |
				makeSubfield(eRegisterCTL, "Security", "None") |
				makeSubfield(eRegisterCTL, "Persist", "No") |
				makeSubfield(eRegisterCTL, "GTS_USER_B", "IoDisabled") |
				0x00000600 /* these bits are not defined in the configuration guide */ |
			0));
		packets.insert(packets.end(), 1150, nop);
		// control register mask
		packets.push_back(VirtexPacket::makeType1Write(eRegisterMASK, 
				makeSubfield(eRegisterMASK, "ICAP_sel", "Protected") |
				makeSubfield(eRegisterMASK, "GLUTMASK", "Protected") |
				makeSubfield(eRegisterMASK, "Security", "Protected") |
				makeSubfield(eRegisterMASK, "Persist", "Protected") |
				makeSubfield(eRegisterMASK, "GTS_USER_B", "Protected") |
				0x00000600 /* these bits are not defined in the configuration guide */ |
			0));
		// control register
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCTL, 
				makeSubfield(eRegisterCTL, "ICAP_sel", "Top") |
				makeSubfield(eRegisterCTL, "GLUTMASK", "Masked") |
				makeSubfield(eRegisterCTL, "Security", "None") |
				makeSubfield(eRegisterCTL, "Persist", "No") |
				makeSubfield(eRegisterCTL, "GTS_USER_B", "IoDisabled") |
				0x00000000 /* these bits are not defined in the configuration guide */ |
			0));
		// NULL command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandNULL));
		packets.push_back(nop);
		// return the packet vector
		return packets;
	}

	VirtexPacketVector Virtex4::generateFullBitstreamSuffix(void) {
		//	00280d63: TYPE1 WRITE CRC: 789d0d51
		//	00280d6b: TYPE1 WRITE CMD GRESTORE
		//	00280d73: NOP x 1
		//	00280d77: TYPE1 WRITE CMD LFRM
		//	00280d7f: NOP x 100
		//	00280f0f: TYPE1 WRITE CMD GRESTORE
		//	00280f17: NOP x 1
		//	00280f1b: TYPE1 WRITE CMD NULL
		//	00280f23: NOP x 1
		//	00280f27: TYPE1 WRITE FAR: 00010f00
		//	00280f2f: TYPE1 WRITE CMD START
		//	00280f37: NOP x 1
		//	00280f3b: TYPE1 WRITE MASK: 00000000 (ICAP_sel:Protected, GLUTMASK:Protected, 
		//				Security:Protected, Persist:Protected, GTS_USER_B:Protected)
		//	00280f43: TYPE1 WRITE CTL: 00000000 (ICAP_sel:Top, GLUTMASK:Masked, Security:None, 
		//				Persist:No, GTS_USER_B:IoDisabled)
		//	00280f4b: TYPE1 WRITE CRC: 50ea82c0
		//	00280f53: TYPE1 WRITE CMD DESYNC
		//	00280f5b: NOP x 16

		// declare the packet vector and define a NOP packet
		VirtexPacketVector packets;
		VirtexPacket nop(VirtexPacket::makeHeader(ePacketType1, eOpcodeNOP, 0, 0));
		// write a placeholder CRC value
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCRC, 0));
		// restore command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandGRESTORE));
		packets.push_back(nop);
		// last frame command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandLFRM));
		packets.insert(packets.end(), 100, nop);
		// restore command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandGRESTORE));
		packets.push_back(nop);
		// NULL command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandNULL));
		packets.push_back(nop);
		// frame address register
		packets.push_back(VirtexPacket::makeType1Write(eRegisterFAR, 
			eFarMaskBlockType | eFarMaskRow)); // is this what the configuration controller wants?
		// start command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandSTART));
		packets.push_back(nop);
		// control register 0 mask
		packets.push_back(VirtexPacket::makeType1Write(eRegisterMASK, 
				makeSubfield(eRegisterMASK, "ICAP_sel", "Protected") |
				makeSubfield(eRegisterMASK, "GLUTMASK", "Protected") |
				makeSubfield(eRegisterMASK, "Security", "Protected") |
				makeSubfield(eRegisterMASK, "Persist", "Protected") |
				makeSubfield(eRegisterMASK, "GTS_USER_B", "Protected") |
			0));
		// control register 0
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCTL, 
				makeSubfield(eRegisterCTL, "ICAP_sel", "Top") |
				makeSubfield(eRegisterCTL, "GLUTMASK", "Masked") |
				makeSubfield(eRegisterCTL, "Security", "None") |
				makeSubfield(eRegisterCTL, "Persist", "No") |
				makeSubfield(eRegisterCTL, "GTS_USER_B", "IoDisabled") |
			0));
		// write the CRC value
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCRC, 0x00000000));
		// desynch command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandDESYNC));
		packets.insert(packets.end(), 16, nop);
		// return the packet vector
		return packets;
	}

	VirtexPacketVector Virtex4::generatePartialBitstreamPrefix(EBitstreamType inBitstreamType) {
		//	Packets marked S pertain to shutdown bitstreams only
		//		0000004b: DUMMY
		//		0000004f: SYNC
		//		00000053: NOP x 1
		//		00000057: TYPE1 WRITE CMD RCRC
		//		0000005f: NOP x 2
		//		00000067: TYPE1 WRITE IDCODE: 01eb4093
		//	S	0000006f: TYPE1 WRITE COR: 00043fe5 (CRC:Enable, DonePipe:No, DriveDone:No, 
		//			Capture:Continuous, ConfigRate:[UNKNOWN 2], StartupClk:Cclk, DONE_cycle:4, 
		//			Match_cycle:NoWait, LCK_cycle:NoWait, GTS_cycle:5, GWE_cycle:6)
		//	S	00000077: TYPE1 WRITE CMD SHUTDOWN
		//	S	0000007f: NOP x 1
		//	S	00000083: TYPE1 WRITE CRC: 7f2b44a8
		//	S	0000008b: NOP x 4
		//	S	0000009b: TYPE1 WRITE CMD AGHIGH
		//	S	000000a3: NOP x 1

		// declare the packet vector and define a NOP packet
		VirtexPacketVector packets;
		VirtexPacket nop(VirtexPacket::makeHeader(ePacketType1, eOpcodeNOP, 0, 0));
		VirtexPacket dummy(eSynchronizationDummy);
		VirtexPacket sync(eSynchronizationSync);
		// dummy and sync words
		packets.push_back(dummy);
		packets.push_back(VirtexPacket(eSynchronizationSync));
		packets.push_back(nop);
		// reset CRC command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandRCRC));
		packets.push_back(nop);
		packets.push_back(nop);
		// write the ID code
		packets.push_back(VirtexPacket::makeType1Write(eRegisterIDCODE, 0x00000000));
		// extra for shutdown bitstreams
		if(inBitstreamType == eBitstreamTypePartialShutdown) {
			// configuration options register 0
			packets.push_back(VirtexPacket::makeType1Write(eRegisterCOR, 
					makeSubfield(eRegisterCOR, "CRC", "Enable") |
					makeSubfield(eRegisterCOR, "DonePipe", "No") |
					makeSubfield(eRegisterCOR, "DriveDone", "No") |
					makeSubfield(eRegisterCOR, "Capture", "Continuous") |
					makeSubfield(eRegisterCOR, "ConfigRate", "[UNKNOWN 2]") |
					makeSubfield(eRegisterCOR, "StartupClk", "Cclk") |
					makeSubfield(eRegisterCOR, "DONE_cycle", "4") |
					makeSubfield(eRegisterCOR, "Match_cycle", "NoWait") |
					makeSubfield(eRegisterCOR, "LCK_cycle", "NoWait") |
					makeSubfield(eRegisterCOR, "GTS_cycle", "5") |
					makeSubfield(eRegisterCOR, "GWE_cycle", "6") |
				0));
			// shutdown command
			packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandSHUTDOWN));
			packets.push_back(nop);
			// write the CRC value
			packets.push_back(VirtexPacket::makeType1Write(eRegisterCRC, 0x00000000));
			packets.insert(packets.end(), 4, nop);
			// aghigh command
			packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandAGHIGH));
			packets.push_back(nop);
		}
		// return the packet vector
		return packets;
	}

	VirtexPacketVector Virtex4::generatePartialBitstreamSuffix(EBitstreamType inBitstreamType) {
		//	Packets marked S pertain to shutdown bitstreams only
		//	S	00004db7: TYPE1 WRITE CMD GRESTORE
		//	S	00004dbf: NOP x 1
		//		00004dc3: TYPE1 WRITE CMD LFRM
		//		00004dcb: NOP x 101
		//	S	00004f5f: TYPE1 WRITE CMD GRESTORE
		//	S	00004f67: NOP x 1
		//	S	00004f6b: TYPE1 WRITE CMD START
		//	S	00004f73: NOP x 1
		//		00004f77: TYPE1 WRITE CRC: f729c893
		//		00004f7f: TYPE1 WRITE CMD DESYNC
		//		00004f87: NOP x 4

		// declare the packet vector and define a NOP packet
		VirtexPacketVector packets;
		VirtexPacket nop(VirtexPacket::makeHeader(ePacketType1, eOpcodeNOP, 0, 0));
		// extra for shutdown bitstreams
		if(inBitstreamType == eBitstreamTypePartialShutdown) {
			// restore command
			packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandGRESTORE));
			packets.push_back(nop);
		}
		// last frame command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandLFRM));
		packets.insert(packets.end(), 101, nop);
		// extra for shutdown bitstreams
		if(inBitstreamType == eBitstreamTypePartialShutdown) {
			// restore command
			packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandGRESTORE));
			packets.push_back(nop);
			// start command
			packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandSTART));
			packets.push_back(nop);
		}
		// write the CRC value
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCRC, 0x00000000));
		// desynch command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandDESYNC));
		if(inBitstreamType == eBitstreamTypePartialShutdown) {
			packets.insert(packets.end(), 4, nop);
		} else {
			packets.insert(packets.end(), 1, nop);
		}
		// return the packet vector
		return packets;
	}

	void Virtex4::initializeFullFrameBlocks (void) {
		boost::shared_array<uint32_t> frameWords;
		// walk the bitstream and extract all frames 
		Virtex4::iterator p = begin();
		Virtex4::iterator e = end();
		while(p < e) {
		    const VirtexPacket& packet = *p++;
		    if(packet.isType2() && packet.isWrite()) 
  				frameWords = packet.getWords();
		}
		uint32_t index = 0;
		for (uint32_t i = 0; i < Bitstream::eBlockTypeCount; i++) {
			// all frames of block type are extracted
			uint32_t padExtra = getRowPadFrames() * getFrameRowCount();
			for (uint32_t j = 0; j < mBlockFrameIndexBounds[i] + padExtra; j++) {
				mFrameBlocks.mBlock[i].push_back(VirtexFrameSet::FrameSharedPtr
					(new VirtexFrame(getFrameLength(), &frameWords[index])));
				index += getFrameLength();
			}
		}
	}

	VirtexFrameBlocks Virtex4::getBitstreamFrames (uint32_t inBlockCount, uint32_t inBitCol, 
		uint32_t inFrameRow) {
		if(inFrameRow > getFrameRowCount()) return VirtexFrameBlocks();

		// index and extract frames
		int32_t bitColumnIndex[inBlockCount];
		int32_t bitColumnBound[inBlockCount];
		for(uint32_t i = 0; i < inBlockCount; i++) {
			// adjust for the frame row
			uint32_t lastIndex = mBitColumnIndexes[i].back();
			uint32_t indexOffset = inFrameRow * (lastIndex ? (lastIndex + getRowPadFrames()) : 0);
			// column Index of given frame index
			bitColumnIndex[i] = mBitColumnIndexes[i][inBitCol] + indexOffset;
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

	VirtexFrameBlocks Virtex4::getXdlFrames (uint32_t inBlockCount, uint32_t inXdlCol, 
		uint32_t inFrameRow) {
		if(inFrameRow > getFrameRowCount()) return VirtexFrameBlocks();

		// index and extract frames
		int32_t xdlColumnIndex[inBlockCount];
		int32_t xdlColumnBound[inBlockCount];
		for(uint32_t i = 0; i < inBlockCount; i++) {
			// adjust for the frame row
			uint32_t lastIndex = mXdlColumnIndexes[i].back();
			uint32_t indexOffset = inFrameRow * (lastIndex ? (lastIndex + getRowPadFrames()) : 0);
			// column Index of given frame index
			xdlColumnIndex[i] = mXdlColumnIndexes[i][inXdlCol] + indexOffset;
			// frame bounds for given column type
			xdlColumnBound[i] = mColumnDefs[mDeviceInfo.getColumnTypes()[inXdlCol]][i];
			//std::cout << "    block: " << i << ", start: " << xdlColumnIndex[i] << ", width: " 
			//	<< xdlColumnBound[i] << " ==== XDL column: " << inXdlCol << ", bit column: " 
			//	<< mXdlColumnToBitColumn[inXdlCol] << std::endl;
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
