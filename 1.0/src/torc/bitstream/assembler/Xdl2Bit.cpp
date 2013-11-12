// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
// $HeadURL: https://svn.east.isi.edu/torc/trunk/src/torc/bitstream/assembler/lut/parser.yy $
// $Id: parser.yy 1303 2013-02-25 23:18:16Z nsteiner $

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

/// \file Xdl2Bit.cpp
/// \brief Convert an XDL to bitstream by merging the compressed bitstreams from library.

#include <algorithm>

#include "Xdl2Bit.h"
#include "torc/packer/Virtex5PrimitiveStructure.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/Physical.hpp"
#include "torc/Bitstream.hpp"
#include "SharedFunctions.hpp"
//#include "LibraryDatabaseStitch.hpp"

using namespace torc::architecture;
using namespace torc::packer;
using namespace torc::physical;
using namespace torc::bitstream;

// Global variables used only in this file
static vector<vector<uint32_t> > gStackOfFrames;
static uint32_t gCurrentReferenceFrameIndex;
static uint32_t gCUrrentReferenceWordIndex;
static std::vector<uint32_t> gLUTWordsForOne(4, 0xFFFFFFFF);

static ConfigBitMap gcurrentConfigBitMap;
static std::string gLUTEquationLeft;

// Function declarations
// Parent function to convert xdl to bitstream. This function call the child functions for instances and nets
void convertXdlToBitstream(DesignSharedPtr inDesignPtr, Bitstream &outBitstream,
	const Sites &sites, const Tiles &tiles, TiletypeElementMap &library);

// This function takes in a design pointer, goes over all the instances, finds compressed
// bitstream in library for each config and merges it with outBitstream.
void assembleInstances(DesignSharedPtr inDesignPtr, Bitstream &outBitstream,
	const Sites &sites, const Tiles &tiles, TiletypeElementMap &library);

void assembleRambInstance(InstanceSharedPtr instance, Bitstream &outBitstream,
	VirtexFrameBlocks &frameBlocks, uint32_t wordOffset, TiletypeElementMap &library);

// This function takes in a design pointer, goes over all the nets, finds compressed
// bitstream in library for each pip and merges it with outBitstream.
void assembleNets(DesignSharedPtr inDesignPtr, Bitstream &outBitstream,
	const Sites &sites, const Tiles &tiles, TiletypeElementMap &library);

VirtexFrameBlocks GetFrameBlocksFromSiteName(Bitstream &inBitstream, const std::string &siteName,
	const Sites &sites, const Tiles & tiles, uint32_t &outWordOffset);

VirtexFrameBlocks GetFrameBlocksFromTileIndex(Bitstream &inBitstream, TileIndex tileIndex,
	const Tiles & tiles, uint32_t &outWordOffset);

void checkValidityAndMergeBitstream(std::string elementName, std::string configVal,
	const ElementConfigMap &elementMap, VirtexFrameBlocks &frameBlocks, uint32_t wordOffset,
	Bitstream &outBitstream);

// Go over all the bitaddresses, disect it into frame address, word address and bit address.
// Merge with outBitstream after handling offset.
void MergeCompressedBitToMainBits(const std::vector<uint32_t> &bitAddresses,
	Bitstream &outBitstream, VirtexFrameBlocks &frameBlocks, uint32_t wordOffset,
	uint32_t blockIndex = 0);

void mergeLutBits(const std::string &elementName,
	const std::string &configValue, Bitstream &outBitstream, VirtexFrameBlocks &frameBlocks,
	uint32_t wordOffset, ElementConfigMap &elementMap);

void mergeLutRamOrRomBits(const std::string &elementName, const std::string &configVal,
	Bitstream &outBitstream, VirtexFrameBlocks &frameBlocks, uint32_t wordOffset,
	TiletypeElementMap &library);

void mergeCompoundSettingBits(std::string element1Name, std::string config1Val,
	InstanceSharedPtr instancePtr, const ElementConfigMap &elementMap,
	VirtexFrameBlocks &frameBlocks, uint32_t wordOffset, Bitstream &outBitstream);

void mergeDspMaskOrPattern(std::string elementName, std::string configVal,
	InstanceSharedPtr instancePtr, const ElementConfigMap &elementMap,
	VirtexFrameBlocks &frameBlocks, uint32_t wordOffset, Bitstream &outBitstream);

void
mergeRambInitBits(const std::string &configVal, uint32_t memoryInitRow,
	Bitstream &outBitstream, VirtexFrameBlocks &frameBlocks, uint32_t wordOffset, const vector<
		uint32_t> &bitAddresses, uint32_t block);

// Returns true if the setting is for LUT
bool isLutEquationSetting(const std::string &configValue) {
	return regex_match(configValue, kLUTConfigRegEx);
}

bool isLutRamOrRomSetting(const std::string &configVal) {
	return regex_match(configVal, kLUTRAMorROMConfigRegEx);
}

bool isMemoryInitSetting(const std::string &configName) {
	return configName.length() == 7 && configName.substr(0, 5).compare("INIT_") == 0;
}

bool isMemoryParityInitSetting(const std::string &configName) {
	return configName.substr(0, 6).compare("INITP_") == 0;
}

bool siteExistsInLibrary(const std::string &siteType, TiletypeElementMap &library) {
	TiletypeElementMap::iterator pLibrary = library.find(siteType);
	return (pLibrary != library.end());
}

bool elementAndConfigExistInLibrary(const std::string &elementName, const std::string &configValue, ElementConfigMap &elementMap) {
	// Check if element exists
	ElementConfigMap::iterator pElement = elementMap.find(elementName);
	if(pElement != elementMap.end()) {

		// Check if config exists
		ConfigBitMap::iterator pConfig = pElement->second.find(configValue);
		if(pConfig != pElement->second.end())
			return true;
	}
	std::cout << "Element " << elementName << " with config " << configValue << " not found in library" << std::endl;
	return false;
}
// Populate map data structure from the library database.
static TiletypeElementMap populateLibraryMap(boost::filesystem::path libDBPath);

/// \brief Converts XDL file to bitstream by merging primitive bitstreams from library
//  Main function starts here
int main(int argc, char **argv) {

	if(argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <XDL file path> <library path>" << std::endl;
		return -1;
	}

	// Initialize the DirectoryTree class
	torc::common::DirectoryTree directoryTree(argv[0]);

	// Open the XDL file and check if it opens successfully
	std::string xdlFileName(argv[1]);
	std::fstream xdlFileStream(xdlFileName.c_str());
	if(!xdlFileStream.good()) {
		std::cout << "Could not open file " << xdlFileName << std::endl;
		return -1;
	}

	// Check if the library path exists
	boost::filesystem::path libraryPath(argv[2]);
	if(!boost::filesystem::exists(libraryPath)) {
		std::cout << "Library folder - " << libraryPath.string() << " - does not exist "
			<< std::endl;
		return -1;
	}

	// This map holds which elements have compound setting
	InitializeCompoundSettingsMap();

	// Import the XDL file
	torc::physical::XdlImporter importer;
	importer(xdlFileStream, xdlFileName);
	// Get the design pointer
	DesignSharedPtr designPtr = importer.getDesignPtr();
	// Get the part number
	std::string partNumber = designPtr->getDevice();
	// Open the database and get sites and tiles
	DDB ddb(partNumber);

	const Sites &sites = ddb.getSites();
	const Tiles &tiles = ddb.getTiles();

	// Get null bitstream file path
	boost::filesystem::path nullBTPath = libraryPath / "null_bitstreams";
	std::string nullBitFileName = partNumber + ".bit";
	nullBTPath = nullBTPath / nullBitFileName;
	if(!boost::filesystem::exists(nullBTPath)) {
		std::cout << "Null bitstream file - " << nullBTPath.string() << " - does not exist."
			<< std::endl;
		return -1;
	}

	// Get the compressed bitstreams library database file path
	std::string libDBFileName = ddb.getFamilyName() + ".ldb";
	boost::filesystem::path libDBPath = libraryPath / libDBFileName;
	if(!boost::filesystem::exists(libDBPath)) {
		std::cout << "Library database file - " << libDBPath.string() << " - does not exist."
			<< std::endl;
		return -1;
	}

	// Populate map data structure from the library database
	TiletypeElementMap library = populateLibraryMap(libDBPath);

	// Open a NULL bitstream of the Device
	std::ifstream nullBitstream;
	nullBitstream.open(nullBTPath.string().c_str());

	Virtex5 bitstream;
	bitstream.read(nullBitstream);
	bitstream.setDesignName(designPtr->getName());
	bitstream.initializeDeviceInfo(bitstream.getDeviceName());
	bitstream.initializeFrameMaps();
	bitstream.initializeFullFrameBlocks();

	// Go over the XDL and merge the required bits to bitstream
	convertXdlToBitstream(designPtr, bitstream, sites, tiles, library);

	// Write the bitstream to a file.
	std::string customBitstreamPath =
		boost::filesystem::path(xdlFileName).replace_extension().string() + ".custom.bit";
	std::ofstream customBitstreamFile(customBitstreamPath.c_str(), std::ios::binary);
	std::cout << "Writing custom bitstream file " << customBitstreamPath << std::endl;
	bitstream.write(customBitstreamFile);

	xdlFileStream.close();
	customBitstreamFile.close();
	return 0;
}

// Parent function to convert xdl to bitstream. This function calls the child functions for instances and nets
void convertXdlToBitstream(DesignSharedPtr inDesignPtr, Bitstream &outBitstream,
	const Sites &sites, const Tiles &tiles, TiletypeElementMap &library) {

	assembleInstances(inDesignPtr, outBitstream, sites, tiles, library);

	assembleNets(inDesignPtr, outBitstream, sites, tiles, library);

	// Update the frame words in packets
	Virtex5 *virtexBitstreamPtr = dynamic_cast<Virtex5 *>(&outBitstream);
	if(virtexBitstreamPtr) {
		virtexBitstreamPtr->updateFullFrameBlocks();
	}

}

// This function takes in a design pointer, goes over all the instances, finds compressed
// bitstream in library for each config and merges the bits with outBitstream.
void assembleInstances(DesignSharedPtr inDesignPtr, Bitstream &outBitstream,
	const Sites &sites, const Tiles &tiles, TiletypeElementMap &library) {

	std::cout << "Going through instances..." << std::endl;
	// Get instances begin and end
	InstanceSharedPtrVector::const_iterator pInstance = inDesignPtr->instancesBegin();
	InstanceSharedPtrVector::const_iterator eInstance = inDesignPtr->instancesEnd();

	// Iterate over all the instances
	while(pInstance != eInstance) {

		InstanceSharedPtr instancePtr = *pInstance;
		const std::string &siteType = instancePtr->getType();

		// Check if instance is placed on supported site type
		if(isSiteTypeSupported(instancePtr->getType()) && siteExistsInLibrary(siteType, library)) {

			std::string siteName = instancePtr->getSite();

			// Get the frame set for this site
			uint32_t wordOffset = 0;
			VirtexFrameBlocks frameBlocks = GetFrameBlocksFromSiteName(outBitstream, siteName,
				sites, tiles, wordOffset);

			std::cout << "Instance " << instancePtr->getName() << " placed on site "
				<< siteName << " has " << instancePtr->getConfigCount()
				<< " configs set." << std::endl;

			// RAMB sites are handled in a slightly different manner
			if(isRambSite(siteType)) {
				assembleRambInstance(instancePtr, outBitstream, frameBlocks,
					wordOffset, library);
			} else {

				// Get the element to config map for the given site type
				ElementConfigMap elementMap = library[siteType];

				// Go over all the configurations of the instance
				ConfigMap::const_iterator pConfig = instancePtr->configBegin();
				ConfigMap::const_iterator eConfig = instancePtr->configEnd();

				while(pConfig != eConfig) {

					std::cout << "\tWorking on config " << pConfig->first << "::"
						<< pConfig->second.getValue() << std::endl;
					// Lut equation has to be handled differently from other config setting.
					// The boolean operation in the LUT equation have to be performed on the relevant bits.
					if(isLutEquationSetting(pConfig->second.getValue())) {
						mergeLutBits(pConfig->first, pConfig->second.getValue(), outBitstream,
							frameBlocks, wordOffset, elementMap);
					} // LUT ROM and RAM settings also need special care
					else if(isLutRamOrRomSetting(pConfig->second.getValue())) {
						mergeLutRamOrRomBits(pConfig->first,
							pConfig->second.getValue(), outBitstream, frameBlocks, wordOffset,
							library);
					}  // Some elements together effect the bitstream.
					else if(elementNeedsCompoundSetting(pConfig->first)) {
						mergeCompoundSettingBits(pConfig->first, pConfig->second.getValue(),
							instancePtr, elementMap, frameBlocks, wordOffset, outBitstream);
					} // The DSP MASK and PATTERN have hex values
					else if(DSPMaskOrPatternConfig(instancePtr->getType(), pConfig->first)) {
						mergeDspMaskOrPattern(pConfig->first, pConfig->second.getValue(),
							instancePtr, elementMap, frameBlocks, wordOffset, outBitstream);
					} else {
						// Merge compressed bitstream to main bitstream if valid setting
						checkValidityAndMergeBitstream(pConfig->first, pConfig->second.getValue(),
							elementMap, frameBlocks, wordOffset, outBitstream);
					}
					std::cout << "\t------------------" << std::endl;
					pConfig++;
				}
			}
		} else {
			std::cout << "Site " << instancePtr->getType() << " not supported." << std::endl;
		}
		pInstance++;
	}
}

// Given a RAMB instance, go over all the configs and merge the corresponding bits
// The memory init values have to be handled differently.
void assembleRambInstance(InstanceSharedPtr instance, Bitstream &outBitstream,
	VirtexFrameBlocks &frameBlocks, uint32_t wordOffset, TiletypeElementMap &library) {

	// Get the element to config map for the given site type
	ElementConfigMap elementMap = library[instance->getType()];

	std::cout << "  Instance " << instance->getName() << " has " << instance->getConfigCount()
		<< " configs set." << std::endl;

	// Open the RAMBIT file and store data in a vector
	// This file stores memory address to bitstream address map
	std::string rambBitFileName = "library/memory/bram-map.bits";
	std::ifstream rambBitFile(rambBitFileName.c_str(), std::ios::binary);
	if(!rambBitFile.is_open()) {
		std::cout << "Could not open file " << rambBitFileName << std::endl;
		return;
	}
	std::vector<uint32_t> bitAddresses;
	uint32_t count = 0, bitAddress = 0;
	while(!rambBitFile.eof()) {
		// One number is ignored.
		rambBitFile.read((char *) &count, 4);
		rambBitFile.read((char *) &bitAddress, 4);
		bitAddress = ntohl(bitAddress);
		bitAddresses.push_back(bitAddress);
	}
	rambBitFile.close();

	// Open the RAMB parity bit file and store the data in a vector
	std::string rambParityBitFileName = "library/memory/bram-map-par.bits";
	std::ifstream rambParityBitFile(rambParityBitFileName.c_str(), std::ios::binary);
	if(!rambParityBitFile.is_open()) {
		std::cout << "Could not open file " << rambParityBitFileName << std::endl;
		return;
	}
	std::vector<uint32_t> parityBitAddresses;
	count = 0, bitAddress = 0;
	while(!rambParityBitFile.eof()) {
		rambParityBitFile.read((char *) &count, 4);
		rambParityBitFile.read((char *) &bitAddress, 4);
		bitAddress = ntohl(bitAddress);
		parityBitAddresses.push_back(bitAddress);
	}
	rambParityBitFile.close();

	// Before going over configs, merge the RAMB base bits.
	MergeCompressedBitToMainBits(elementMap[instance->getType()]["BASE"], outBitstream,
		frameBlocks, wordOffset);

	// Go over all the configurations of the instance
	ConfigMap::const_iterator pConfig = instance->configBegin();
	ConfigMap::const_iterator eConfig = instance->configEnd();
	while(pConfig != eConfig) {

		std::cout << "\tBRAM Working on config " << pConfig->first << " value "
			<< pConfig->second.getValue() << std::endl;

		// RAMB memory and parity init values have to be handled differently.
		// The boolean operation in the LUT equation have to be performed on the relevant bits.
		if(isMemoryInitSetting(pConfig->first)) {

			// Get the row number from config name. Last two character are row number in hex form.
			std::stringstream ssRow;
			uint32_t memoryInitRow;
			ssRow << std::hex << pConfig->first.substr(pConfig->first.length() - 2, 2);
			ssRow >> memoryInitRow;

			mergeRambInitBits(pConfig->second.getValue(), memoryInitRow,
				outBitstream, frameBlocks, wordOffset, bitAddresses, 1);
		} else if(isMemoryParityInitSetting(pConfig->first)) {

			// Get the row number from config name. Last two character are row number in hex form.
			std::stringstream ssRow;
			uint32_t memoryInitRow;
			ssRow << std::hex << pConfig->first.substr(pConfig->first.length() - 2, 2);
			ssRow >> memoryInitRow;
			mergeRambInitBits(pConfig->second.getValue(), memoryInitRow,
				outBitstream, frameBlocks, wordOffset, parityBitAddresses, 1);
		} else {
			// Merge compressed bitstream to main bitstream
			checkValidityAndMergeBitstream(pConfig->first, pConfig->second.getValue(), elementMap,
				frameBlocks, wordOffset, outBitstream);
		}
		std::cout << "------------------" << std::endl;
		pConfig++;
	}
}

// This function takes in a design pointer, goes over all the nets, finds compressed
// bitstream in library for each pip and merges it with outBitstream.
void assembleNets(DesignSharedPtr inDesignPtr, Bitstream &outBitstream,
	const Sites &sites, const Tiles &tiles, TiletypeElementMap &library) {

	// Get the iterators to nets
	NetSharedPtrVector::const_iterator pNets = inDesignPtr->netsBegin();
	NetSharedPtrVector::const_iterator eNets = inDesignPtr->netsEnd();

	std::cout << "Working on NETs... count: " << inDesignPtr->getNetCount() << std::endl;
	// Go over all the nets
	while(pNets != eNets) {

		NetSharedPtr netPtr = *pNets;
		std::cout << "Net: " << netPtr->getName() << std::endl;

		// Go over all the pips in the net
		Net::PipConstIterator pPips = netPtr->pipsBegin();
		Net::PipConstIterator ePips = netPtr->pipsEnd();
		while(pPips != ePips) {

			// Get word offset for this tile
			TileIndex tileIndex = tiles.findTileIndex(pPips->getTileName());
			const TileInfo& tileInfo = tiles.getTileInfo(tileIndex);
			std::string tileType = tiles.getTileTypeName(tileInfo.getTypeIndex());
			uint32_t wordOffset = 0;
			VirtexFrameBlocks frameBlocks = GetFrameBlocksFromTileIndex(outBitstream, tileIndex,
				tiles, wordOffset);

			ElementConfigMap elementMap = library[tileType];
			std::cout << "\tTile " << tileInfo.getName() << " Src: " << pPips->getSourceWireName()
				<< std::endl;
			checkValidityAndMergeBitstream(pPips->getSourceWireName(), pPips->getSinkWireName(),
				elementMap, frameBlocks, wordOffset, outBitstream);
			pPips++;
		}

		pNets++;
	}
}

// Given a site, return the frame blocks in bitstream which are affected by the site configuration
VirtexFrameBlocks GetFrameBlocksFromSiteName(Bitstream &inBitstream, const std::string &siteName,
	const Sites &sites, const Tiles & tiles, uint32_t &outWordOffset) {

	SiteIndex siteIndex = sites.findSiteIndex(siteName);
	const Site& site = sites.getSite(siteIndex);

	// look up the site tile index
	TileIndex tileIndex = site.getTileIndex();
	return GetFrameBlocksFromTileIndex(inBitstream, tileIndex, tiles, outWordOffset);
}

// Given the tile index, return the frame blocks in bitstream which are affected by the site configuration
VirtexFrameBlocks GetFrameBlocksFromTileIndex(Bitstream &inBitstream, TileIndex tileIndex,
	const Tiles & tiles, uint32_t &outWordOffset) {

	const TileInfo& tileInfo = tiles.getTileInfo(tileIndex);
	const TileRow &tileRow = tileInfo.getRow();
	const TileCol &tileCol = tileInfo.getCol();

	Virtex5 *virtex5BitStream = dynamic_cast<Virtex5 *> (&inBitstream);
	uint32_t bitIndexBegin = 0, bitIndexEnd = 0;
	uint32_t primaryXdlCol = virtex5BitStream->getPrimaryXdlColumn(tileCol);
	VirtexFrameBlocks returnFrameBlocks = virtex5BitStream->getXdlFrames(tileRow, primaryXdlCol,
		bitIndexBegin, bitIndexEnd);

	// Each frame covers the full height of clock region which contains many rows of tiles.
	// The library contains configuration bits only for the first row. So the word index has to
	// be offset to reach the correct row. The bit index remains same across rows.
	outWordOffset = 0;
	if(bitIndexBegin != bitIndexEnd) {
		outWordOffset = Virtex5::eFrameLength - (bitIndexEnd >> 5); // eFrameLength is number of words in a frame
	}

	return returnFrameBlocks;
}

// Check if the element and config exist in library. If yes, merge the corresponding bits with the bitstream
void checkValidityAndMergeBitstream(std::string elementName, std::string configVal,
	const ElementConfigMap &elementMap, VirtexFrameBlocks &frameBlocks, uint32_t wordOffset,
	Bitstream &outBitstream) {

	ElementConfigMap::const_iterator elementIter = elementMap.find(elementName);
	// If element found in the map
	if(elementIter != elementMap.end()) {
		ConfigBitMap::const_iterator configIter = elementIter->second.find(configVal);
		//If config value found in the config to bit map
		if(configIter != elementIter->second.end()) {
			MergeCompressedBitToMainBits(configIter->second, outBitstream, frameBlocks, wordOffset,
				0);
		} else {
			std::cout << "Config " << configVal << " for element " << elementName
				<< " not found in library." << std::endl;
		}
	} else {
		std::cout << "Element " << elementName << " not found in library." << std::endl;
	}
}

// Go over all the bit addresses, disect it into frame address, word address and bit address.
// Merge with outBitstream after handling offset.
void MergeCompressedBitToMainBits(const std::vector<uint32_t> &bitAddresses,
	Bitstream &outBitstream, VirtexFrameBlocks &frameBlocks, uint32_t wordOffset,
	uint32_t blockIndex) {

	int32_t numSetBits = bitAddresses.size();
	std::cout << "\tNumber of bits set: " << numSetBits << std::endl;

	VirtexFrameSet& frameSet = frameBlocks.mBlock[blockIndex];
	if(!frameSet.empty()) {
		for(int i = 0; i < numSetBits; i++) {
			uint32_t bitAddr = bitAddresses[i];
			int32_t bitIndex = bitAddr & 0x000000FF; // get the lowest byte
			int32_t wordIndex = (bitAddr & 0x0000FF00) >> 8;
			// Library setting is for first row of clock region. Word index has to be offset to reach the correct row.
			wordIndex += wordOffset;
			int32_t frameIndex = (bitAddr & 0xFFFF0000) >> 16;
			std::cout << "\t  Frame: " << Hex32(frameIndex) << " Word: " << Hex32(wordIndex)
				<< " bit: " << Hex32(bitIndex) << std::endl;
			VirtexFrameSharedPtr virtexFrame = frameSet[frameIndex];

			VirtexFrame::word_t currentWord = virtexFrame->getWords()[wordIndex];
			VirtexFrame::word_t word = (1 << (bitIndex)) | currentWord;
			virtexFrame->setWord(wordIndex, word);

		}
	} else {
		std::cout << "Unknown frame type." << std::endl;
	}

}

// This funtion goes over the LUT equation and applies the boolean operation present in the
// LUT equation to the corresponding bitstreams of literals
void mergeLutBits(const std::string &elementName,
	const std::string &configValue, Bitstream &outBitstream, VirtexFrameBlocks &frameBlocks,
	uint32_t wordOffset, ElementConfigMap &elementMap) {

	// Get the right hand side and lefthand side of the LUT equation.
	std::vector<std::string> splitVector;
	boost::algorithm::split(splitVector, configValue, boost::algorithm::is_any_of("="));

	// The left side tell which output (O5 or O6) is being affected. Store it in a global
	// so that it can be used by functions called from bison
	gLUTEquationLeft = splitVector[0];
	std::string equationRight = splitVector[1];

	// Store the frames for LUT setting constant 1. XOR with these frames give NOT functionality.
	std::string lutEquationForOne = gLUTEquationLeft + "=1";

	// Currently all the frames of a LUT setting are not stored. We know only 4 words get affected by LUT setting -
	// 4 frames and a word in each frame. So instead we store only 4 words and the frame index and word index.
	if(elementAndConfigExistInLibrary(elementName, lutEquationForOne, elementMap)) {
		gcurrentConfigBitMap = elementMap[elementName];
		std::vector<uint32_t> bitAddressesForOne = gcurrentConfigBitMap[lutEquationForOne];

		gCurrentReferenceFrameIndex = bitAddressesForOne[0] >> 16;
		gCUrrentReferenceWordIndex = (bitAddressesForOne[0] & 0x0000FF00) >> 8;

		gLUTWordsForOne.clear();
		gLUTWordsForOne.resize(4, uint32_t(0));
		for(std::vector<uint32_t>::const_iterator bitIter = bitAddressesForOne.begin(); bitIter
			!= bitAddressesForOne.end(); bitIter++) {
			int32_t bitIndex = (*bitIter) & 0x000000FF;
			int32_t frameIndex = (*bitIter) >> 16;
			uint32_t frameWord = 1 << (bitIndex - 1);

			gLUTWordsForOne[frameIndex - gCurrentReferenceFrameIndex] = gLUTWordsForOne[frameIndex
				- gCurrentReferenceFrameIndex] | frameWord;
		}

		// Set the string to be scanned
		yy_scan_string(equationRight.c_str());
		// Call the Bison parse function
		int parseResult = yyparse();

		if(parseResult != 0) {
			std::cout << "Error in parsing LUT equation " << configValue << std::endl;
			exit(-1);
		}

		// Build bit addresses out of the last frame in gStackOfFrames
		std::vector<uint32_t> combinedLUTWords = gStackOfFrames[0];
		std::vector<uint32_t> bitAddresses;
		uint32_t frameOffset = 0;
		for(std::vector<uint32_t>::const_iterator wordIter = combinedLUTWords.begin(); wordIter
			!= combinedLUTWords.end(); wordIter++, frameOffset++) {

			uint32_t tempWord = *wordIter;
			uint32_t bitIndex = 1;
			while(tempWord != 0) {

				// check if the LSB is 1
				if(tempWord & 1) {
					// store the combined address
					bitAddresses.push_back(((gCurrentReferenceFrameIndex + frameOffset) << 16)
						| (gCUrrentReferenceWordIndex << 8) | bitIndex);
				}
				tempWord = tempWord >> 1;
				bitIndex++;
			}
		}

		MergeCompressedBitToMainBits(bitAddresses, outBitstream, frameBlocks, wordOffset);
		gStackOfFrames.clear();
	}
}

// Function to parse the memory content string of LUTs configured as RAM or ROM
// The way memory addresssing works is the first bit in the string is LSB
// E.g. For memory string "80000000", the LSB is set.
void mergeLutRamOrRomBits(const std::string &elementName, const std::string &configVal,
	Bitstream &outBitstream, VirtexFrameBlocks &frameBlocks, uint32_t wordOffset,
	TiletypeElementMap &library) {

	// Since memory content are similar for SLICEL and SLICEM,
	// they are stored in library under element name SLICE
	ElementConfigMap &elementMap = library["SLICE"];

	// Get the right hand side and lefthand side of the LUT equation.
	std::vector<std::string> splitVector;
	boost::algorithm::split(splitVector, configVal, boost::algorithm::is_any_of("="));
	// Remove the first two characters - 0x - from the hex value
	std::string memoryValue = splitVector[1].substr(2);

	if(memoryValue.length() != 8 && memoryValue.length() != 16) {
		std::cout << "Error: Illegal configuration of element " << elementName << std::endl;
		return;
	}

	// When output O5 of LUT is configured, it has only 8 hex characters in the memory string,
	// and they belong to the higher memory address of 16 hex characters.
	if(memoryValue.length() == 8) {
		memoryValue = "00000000" + memoryValue;
	}

	// Get the bit address vector
	// Since X5LUT and X6LUT use the same memory address to bit address map,
	// element name is changed to use only 1st letter of the name, viz X (A, B, C or D)
	// and config name is changed to ROM
	std::string elementNameForMemoryMode = elementName.substr(0, 1);
	std::vector<uint32_t> bitAddressesForROM;

	if(!elementAndConfigExistInLibrary(elementNameForMemoryMode, "ROM", elementMap))
		return;

	bitAddressesForROM = elementMap[elementNameForMemoryMode]["ROM"];
	std::vector<uint32_t> bitAddresses;
	// Iterate over the memory string from left to right
	for(uint32_t charIndex = 0; charIndex < memoryValue.length(); charIndex++) {

		std::stringstream sshexChar;
		sshexChar << std::hex << memoryValue[charIndex];
		uint32_t hexDigit;
		sshexChar >> hexDigit;
		// Go over the bits of the hex digit from left to right
		for(int bitIndex = 0; hexDigit != 0; bitIndex++) {
			if(hexDigit & 8) {
				uint32_t memoryAddress = (charIndex << 2) + bitIndex;
				std::stringstream ssConfigBitIndex;
				ssConfigBitIndex << memoryAddress;
				bitAddresses.push_back(bitAddressesForROM[memoryAddress]);
			}
			hexDigit = hexDigit << 1;
		}
	}
	MergeCompressedBitToMainBits(bitAddresses, outBitstream, frameBlocks, wordOffset, 0);
}

void mergeCompoundSettingBits(std::string element1Name, std::string config1Val,
	InstanceSharedPtr instancePtr, const ElementConfigMap &elementMap,
	VirtexFrameBlocks &frameBlocks, uint32_t wordOffset, Bitstream &outBitstream) {

	std::vector<std::string> compoundElements = gCompoundSettingsMap[element1Name];
	// For now assume there is only on element in the vector, i.e. there is a compound
	// setting with only two elements.
	if(compoundElements.size() != 1) {
		std::cout << "Compound setting with more than two elements not handled currently"
			<< std::endl;
	} else {

		std::cout << "\tIt is a compound setting." << std::endl;

		// The two element names will be concatenated to form a compound element name Element1Element2.
		// Similarly two config settings will be concatenated to form a compound config name.
		std::string element2Name = compoundElements[0];
		std::string compoundElementName = element1Name + element2Name;

		// Find the 2nd element config in the instance
		std::string config2Val, config2Name;
		instancePtr->getConfig(element2Name, config2Name, config2Val);

		// Element 2 config not set
		if(config2Val.compare(kConfigOff) == 0) {
			std::cout << "Illegal configuration. Element " << element2Name << " must be set "
				<< " along with element " << element1Name << std::endl;
			return;
		}

		// Get compound config val
		std::string compoundConfigVal = config1Val + config2Val;

		// Verify setting and merge bits
		checkValidityAndMergeBitstream(compoundElementName, compoundConfigVal, elementMap,
			frameBlocks, wordOffset, outBitstream);
	}
}

void mergeDspMaskOrPattern(std::string elementName, std::string configVal,
	InstanceSharedPtr instancePtr, const ElementConfigMap &elementMap,
	VirtexFrameBlocks &frameBlocks, uint32_t wordOffset, Bitstream &outBitstream) {

	std::vector<uint32_t> bitAddresses;

	// Go over all the characters of the config val string
	for(uint32_t charIndex = 0; charIndex < configVal.length(); charIndex++) {
		std::stringstream sshexChar;
		sshexChar << std::hex << configVal[configVal.length() - charIndex - 1];
		uint32_t hexDigit;
		sshexChar >> hexDigit;
		// Go over all the bits of the hex digit
		for(int bitIndex = 0; hexDigit != 0; bitIndex++) {
			if(hexDigit & 1) {
				int configBitIndex = (charIndex << 2) + bitIndex;
				std::stringstream ssConfigBitIndex;
				ssConfigBitIndex << configBitIndex;
				checkValidityAndMergeBitstream(elementName, ssConfigBitIndex.str(), elementMap,
					frameBlocks, wordOffset, outBitstream);
			}
			hexDigit = hexDigit >> 1;
		}
	}
}

// This function taken in the hex character string of memory and parity initial values, finds the
// bit address for set memory bits and merges the bits with outBitstream
void mergeRambInitBits(const std::string &configVal, uint32_t memoryInitRow,
	Bitstream &outBitstream, VirtexFrameBlocks &frameBlocks, uint32_t wordOffset, const vector<
		uint32_t> &ramBitAddress, uint32_t block) {

	vector<uint32_t> bitAddresses; // vector to hold bit addresses of set memory bits

	uint32_t configValLength = configVal.length();
	uint32_t numBitsPerConfig = configValLength << 2; // 4 bits every hex character

	// Go over the memory string from right to left
	for(uint32_t charIndex = 0; charIndex < configValLength; charIndex++) {
		std::stringstream sshexChar;
		sshexChar << std::hex << configVal[configValLength - charIndex - 1];
		uint32_t hexDigit;
		sshexChar >> hexDigit;
		// Go over the bits of hex digit from right to left
		for(int bitIndex = 0; hexDigit != 0; bitIndex++) {
			if(hexDigit & 1) {
				// Get memory address of set bit taking into account which row of memory is being processed
				uint32_t configBitIndex = (charIndex << 2) + bitIndex;
				uint32_t memoryAddress = memoryInitRow * numBitsPerConfig + configBitIndex;

				bitAddresses.push_back(ramBitAddress[memoryAddress]);
			}
			hexDigit = hexDigit >> 1;
		}
	}
	MergeCompressedBitToMainBits(bitAddresses, outBitstream, frameBlocks, wordOffset, block);
}

// This function is called by Bison parser when a literal is encountered.
void PushFrameToStack(std::string literal) {

	bool debug = false;

	if(debug) {
		std::cout << "In PushFrameToStack " << literal << std::endl;
	}

	std::vector<uint32_t> bitAddresses = gcurrentConfigBitMap[gLUTEquationLeft + "=" + literal];

	// We know LUT bits go across 4 frames and effect the same word in each frame
	std::vector<uint32_t> lutWords(4, 0);
	for(std::vector<uint32_t>::const_iterator bitIter = bitAddresses.begin(); bitIter
		!= bitAddresses.end(); bitIter++) {
		int32_t bitIndex = (*bitIter) & 0x000000FF;
		int32_t frameIndex = (*bitIter) >> 16;
		uint32_t frameWord = 1 << (bitIndex - 1);

		lutWords[frameIndex - gCurrentReferenceFrameIndex] = lutWords[frameIndex
			- gCurrentReferenceFrameIndex] | frameWord;
	}

	if(debug) {
		for(std::vector<uint32_t>::const_iterator iter = lutWords.begin(); iter != lutWords.end(); iter++) {
			std::cout << "   " << Hex32(*iter);
		}
		std::cout << std::endl;
	}

	gStackOfFrames.push_back(lutWords);

}

void DoBinaryOperation(std::string operation) {

	bool debug = false;

	if(debug) {
		std::cout << "In DoBinaryOperation " << operation << std::endl;
	}
	std::vector<uint32_t> lutWords1 = gStackOfFrames.back();
	std::vector<uint32_t> lutWordsNew;
	gStackOfFrames.pop_back();
	std::vector<uint32_t> lutWords2;
	if(operation == "OR" || operation == "AND" || operation == "XOR") {
		lutWords2 = gStackOfFrames.back();
		gStackOfFrames.pop_back();
	} else if(operation == "NOT") {
		lutWords2 = gLUTWordsForOne;
	} else {
		cout << "Unknown operation " << operation << std::endl;
		return;
	}
	std::vector<uint32_t>::const_iterator pWord1 = lutWords1.begin();
	std::vector<uint32_t>::const_iterator eWord1 = lutWords1.end();
	std::vector<uint32_t>::const_iterator pWord2 = lutWords2.begin();
	std::vector<uint32_t>::const_iterator eWord2 = lutWords2.end();
	if(operation == "OR") {
		while(pWord1 != eWord1 && pWord2 != eWord2) {
			lutWordsNew.push_back((*pWord1 | *pWord2));
			pWord1++;
			pWord2++;
		}
	} else if(operation == "AND") {
		while(pWord1 != eWord1) {
			lutWordsNew.push_back((*pWord1 & *pWord2));
			pWord1++;
			pWord2++;
		}
	} else if(operation == "XOR" || operation == "NOT") {
		while(pWord1 != eWord1) {
			lutWordsNew.push_back((*pWord1 ^ *pWord2));
			pWord1++;
			pWord2++;
		}
	}

	if(debug) {
		std::cout << "LUT frames after the logic " << std::endl;
		for(std::vector<uint32_t>::const_iterator iter = lutWordsNew.begin(); iter
			!= lutWordsNew.end(); iter++) {
			std::cout << "   " << Hex32(*iter);
		}
		std::cout << std::endl;
	}

	gStackOfFrames.push_back(lutWordsNew);

}

//// Returns true if the site type is supported
bool isSiteTypeSupported(const std::string &siteType) {
	return (siteType.compare("SLICEL") == 0 || siteType.compare("SLICEM") == 0 || siteType.compare(
		"RAMB36_EXP") == 0 || siteType.compare("DSP48E") == 0);
}

//// Returns true if the site is some form of RAMB site.
bool isRambSite(const std::string &siteType) {
	if(siteType.length() > 3) {
		return siteType.substr(0, 4).compare("RAMB") == 0;
	}
	return false;
}

static TiletypeElementMap populateLibraryMap(boost::filesystem::path libDBPath) {

	TiletypeElementMap tileMap;
	std::ifstream libDB(libDBPath.string().c_str(), std::ios::binary);
	if(!libDB.good()) {
		std::cout << "Could not open DB file " << libDBPath.string() << std::endl;
		libDB.exceptions(std::ios::failbit);
		exit(-1);
	}
	std::cout << "Opened DB file " << libDBPath.string() << std::endl;
	char buffer[1024];
	libDB.read(buffer, 16);
	buffer[16] = '\0';
	std::string libDBSanity(buffer);
	std::cout << "Sanity string " << libDBSanity << std::endl;
	if(libDBSanity != "<<<<BITLIBDB>>>>") {
		std::cout << "This file seems to be currupt- " << libDBPath.string() << std::endl;
		exit(-1);
	}
	uint32_t tileTypeCount = 0;
	libDB.read((char *) &tileTypeCount, sizeof(uint32_t));
	std::cout << "Tile type count " << tileTypeCount << std::endl;
	for(uint32_t tileIndex = 0; tileIndex < tileTypeCount; tileIndex++) {
		uint32_t tileNameCharCount = 0;
		libDB.read((char *) &tileNameCharCount, 4);
		libDB.read(buffer, tileNameCharCount);
		buffer[tileNameCharCount] = '\0';
		std::string tileType(buffer);
		uint32_t elementCount = 0;
		libDB.read((char *) &elementCount, 4);
		//		std::cout << "Tile type " << tileType << ". Element count " << elementCount << std::endl;

		ElementConfigMap elementMap;

		// Iterate over the elements
		for(uint32_t elementIndex = 0; elementIndex < elementCount; elementIndex++) {
			// Get element name char count
			uint32_t elementCharCount = 0;
			libDB.read((char *) &elementCharCount, 4);
			libDB.read(buffer, elementCharCount);
			buffer[elementCharCount] = '\0';
			std::string element(buffer);

			uint32_t configCount = 0;
			libDB.read((char *) &configCount, 4);

			//			std::cout << "   Element " << element << ". Config count " << configCount << std::endl;

			ConfigBitMap configMap;

			// Iterate over configs
			for(uint32_t configIndex = 0; configIndex < configCount; configIndex++) {
				uint32_t configCharCount = 0;
				libDB.read((char *) &configCharCount, 4);
				//				std::cout << "\tConfig char count " << configCharCount;
				libDB.read(buffer, configCharCount);
				buffer[configCharCount] = '\0';
				std::string config(buffer);
				//				std::cout << "\t" << config << std::endl;

				// Read the compressed word count
				uint32_t wordCount = 0;
				libDB.read((char *) &wordCount, 4);
				std::vector<uint32_t> words;
				uint32_t word;
				for(uint32_t i = 0; i < wordCount; i++) {
					libDB.read((char *) &word, 4);

					words.push_back(word);
				}
				configMap[config] = words;
			}
			elementMap[element] = configMap;

		}
		tileMap[tileType] = elementMap;

	}
	return tileMap;
}
