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
/// \brief Source for the VirtexE class.
#include "torc/bitstream/VirtexE.hpp"
#include <iostream>

/// \todo Warning: this will need to be moved elsewhere.
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <fstream>


namespace torc {
namespace bitstream {

//#define GENERATE_STATIC_DEVICE_INFO
#ifndef GENERATE_STATIC_DEVICE_INFO

	extern DeviceInfo xcv50e;
	extern DeviceInfo xcv100e;
	extern DeviceInfo xcv200e;
	extern DeviceInfo xcv300e;
	extern DeviceInfo xcv400e;
	extern DeviceInfo xcv405e;
	extern DeviceInfo xcv600e;
	extern DeviceInfo xcv812e;
	extern DeviceInfo xcv1000e;
	extern DeviceInfo xcv1600e;
	extern DeviceInfo xcv2000e;
	extern DeviceInfo xcv2600e;
	extern DeviceInfo xcv3200e;

	void VirtexE::initializeDeviceInfo(const std::string& inDeviceName) {
		using namespace torc::common;
		switch(mDevice) {
			case eXCV50E: setDeviceInfo(xcv50e); break;
			case eXCV100E: setDeviceInfo(xcv100e); break;
			case eXCV200E: setDeviceInfo(xcv200e); break;
			case eXCV300E: setDeviceInfo(xcv300e); break;
			case eXCV400E: setDeviceInfo(xcv400e); break;
			case eXCV405E: setDeviceInfo(xcv405e); break;
			case eXCV600E: setDeviceInfo(xcv600e); break;
			case eXCV812E: setDeviceInfo(xcv812e); break;
			case eXCV1000E: setDeviceInfo(xcv1000e); break;
			case eXCV1600E: setDeviceInfo(xcv1600e); break;
			case eXCV2000E: setDeviceInfo(xcv2000e); break;
			case eXCV2600E: setDeviceInfo(xcv2600e); break;
			case eXCV3200E: setDeviceInfo(xcv3200e); break;
			default: break;
		}
	}

#else
	
	void VirtexE::initializeDeviceInfo(const std::string& inDeviceName) {

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

	void VirtexE::initializeFrameMaps(void) {
	    
	    bool debug = 0;
	    int center = 0;
		int frameIndex = 0;
		int frameCount = 0;
		int farMajor = 0;
		int width = 0;
		ColumnIndex col;
		const ColumnTypeVector& columnTypes = mDeviceInfo.getColumnTypes();
		for(uint32_t i = 0; i < VirtexE::eFarBlockTypeCount; i++) {
			farMajor = i;
			mFrameIndexBounds = 0;
			EFarBlockType blockType = VirtexE::EFarBlockType(i);
			//Set first frame index to 0
			uint32_t bitIndex = 0;
			uint32_t xdlIndex = 0;
			mBitColumnIndexes[i].push_back(bitIndex);
			mXdlColumnIndexes[i].push_back(xdlIndex);
			uint16_t finalColumn = mDeviceInfo.getColCount()-1;
			uint32_t xdlColumnCount = 0;
			uint32_t bitColumnCount = 0;
			// Clock Column at the middle
			center = mDeviceInfo.getColCount()/2;
			col = center;
			prepareFrames(col, frameCount, frameIndex, blockType, farMajor, width);
			int numBrams = 4;
			int numIobs = 2;
			int numClocks = 1;
			int numClbs = mDeviceInfo.getColCount() - numClocks - numBrams - numIobs;
			// Extended memory devices have different frame mapping
			if(mDevice == torc::common::eXCV405E || mDevice == torc::common::eXCV812E) {
				// CLB Columns alternate around the clock column
				for (int j = 1; j < center; j++) {
					for (int k = -1; k < 2; k += 2) {
						col = center - (j * k);
						if(columnTypes[col] == eColumnTypeClb) {
						  prepareFrames(col, frameCount, frameIndex, blockType, farMajor, width);
						}
						//Indexes for Bitstream Columns, only stores non-empty tile types
						if(mDeviceInfo.getColumnTypes()[col] != Virtex::eColumnTypeEmpty) {
							mXdlIndexToBitIndex[bitColumnCount] = xdlColumnCount;
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
				for (int j = center; j < (center + 1); j++) {
					for (int k = -1; k < 2; k += 2) {
						col = center - (j * k);
						prepareFrames(col, frameCount, frameIndex, blockType, farMajor, width);
						//Indexes for Bitstream Columns, only stores non-empty tile types
						if(mDeviceInfo.getColumnTypes()[col] != Virtex::eColumnTypeEmpty) {
							mXdlIndexToBitIndex[bitColumnCount] = xdlColumnCount;
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
				for (int j = 1; j < center; j++) {
					for (int k = -1; k < 2; k += 2) {
						col = center - (j * k);
						if(columnTypes[col] == eColumnTypeBram) {
						  prepareFrames(col, frameCount, frameIndex, blockType, farMajor, width);
						}
						//Indexes for Bitstream Columns, only stores non-empty tile types
						if(mDeviceInfo.getColumnTypes()[col] != Virtex::eColumnTypeEmpty) {
							mXdlIndexToBitIndex[bitColumnCount] = xdlColumnCount;
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
			}
			else {
				// CLB and BRAM Columns alternate around the clock column
				for (int j = 1; j <= (numClbs + numBrams) / 2; j++) {
					for (int k = -1; k < 2; k += 2) {
						col = center - (j * k);
						prepareFrames(col, frameCount, frameIndex, blockType, farMajor, width);
						//Indexes for Bitstream Columns, only stores non-empty tile types
						if(mDeviceInfo.getColumnTypes()[col] != Virtex::eColumnTypeEmpty) {
							  mXdlIndexToBitIndex[bitColumnCount] = xdlColumnCount;
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
				for (int j = center; j < (center + 1); j++) {
					for (int k = -1; k < 2; k += 2) {
						col = center - (j * k);
						prepareFrames(col, frameCount, frameIndex, blockType, farMajor, width);
						//Indexes for Bitstream Columns, only stores non-empty tile types
						if(mDeviceInfo.getColumnTypes()[col] != Virtex::eColumnTypeEmpty) {
							  mXdlIndexToBitIndex[bitColumnCount] = xdlColumnCount;
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
			}
			//stores frame index bounds for each block type
			mBlockFrameIndexBounds[i] = mFrameIndexBounds;
			if (debug) std::cout << "***Block frame index bounds: " << mBlockFrameIndexBounds[i] << std::endl;
		}
		//Test to check proper indexing
		if (debug) {
  		    for(uint32_t i = 0; i < Virtex::eFarBlockTypeCount; i++) {
  			    for(uint32_t j = 0; j < mBitColumnIndexes[i].size(); j++) 
			        std::cout << "Bit Value at index: (" << i << ", " << j << ") : " << mBitColumnIndexes[i][j] << std::endl;
			    for(uint32_t k = 0; k < mXdlColumnIndexes[i].size(); k++)
			        std::cout << "Xdl Value at index: (" << i << ", " << k << ") : " << mXdlColumnIndexes[i][k] << std::endl;
		    }
		}
	}

} // namespace bitstream
}
