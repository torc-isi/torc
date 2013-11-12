// Torc - Copyright 2013-2013 University of Southern California.  All Rights Reserved.
// $HeadURL: https://svn.east.isi.edu/torc/trunk/src/torc/bitstream/assembler/Assembler.cpp $
// $Id: Assembler.cpp 1303 2013-02-25 23:18:16Z rsoni $

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

/// \file Assembler.cpp
/// \brief Implementation of base class Assembler for Xdl to bitstream conversion

#include <iostream>
#include "torc/bitstream/assembler/Assembler.hpp"
#include "torc/bitstream/assembler/AssemblerFactory.hpp"
#include "torc/bitstream/assembler/lut/LutScanner.hpp"
#include "torc/Bitstream.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/Physical.hpp"
#include <boost/regex.hpp>
#include <sstream>

using namespace torc::architecture;
using namespace torc::physical;
using namespace torc::bitstream;

namespace torc {
namespace bitstream {

const string Assembler::sLibraryRelativePath = "torc/bitstream/assembler/libraries";
const string Assembler::sConfigOff = "#OFF";
const string Assembler::sLibraryExtension = ".ldb";
const boost::regex Assembler::sLutConfigRegEx("^(#LUT).*");
const boost::regex Assembler::sLutRamOrRomConfigRegEx("^(#RAM:|#ROM).*");
const boost::regex Assembler::sRoutethroughRegEx("^_ROUTETHROUGH.*");

/// \details Called from constructor to initialize the object.
///	Populates mirco-bitstream data from library in a map.
void Assembler::initialize(void) {

	mUnsupportedTileCount = 0;
	mUnsupportedTileTypeCount = 0;
	mUnsupportedPipCount = 0;
	mMissingConfigs = 0;
	mLibraryPath = torc::common::DirectoryTree::getExecutablePath() / sLibraryRelativePath;
	if(!boost::filesystem::exists(mLibraryPath)) {
		std::cerr << "ERROR: Library folder " << mLibraryPath << " does not exist." << std::endl;
		throw 1;
	}

	mBigEndian = isBigEndianMachine();
}

///// \details Public function to be called by user to initiate bitstream generation process.
/////	The parameter inBaseBitstreamPath will act as base bitstream for micro-bitstream assembly process.
/////	This function might be implemented in derived class for additional architecture specific processing.
int Assembler::generateBitstream(DesignSharedPtr inDesignPtr,
		const path inTargetBitstreamPath,
		EMergeMode inMergeMode,
		path baseBitstreamPath) {
	
	mDesignPtr = inDesignPtr;
	mMergeMode = inMergeMode;
	mTargetBitstreamPath = inTargetBitstreamPath;

	if(mDesignPtr->getDevice() != mDB.getDeviceName()) {
		std::cout << "Xdl device differs from database device" << std::endl;
		return -1;
	}
	// User hasn't specified base bitstream path
	if(baseBitstreamPath.empty()) {
		// Null bitstreams are stored in library folder

		const string partNumber = mDesignPtr->getDevice();
		string nullBitFileName = partNumber + ".bit";
		baseBitstreamPath = mLibraryPath / "null_bitstreams" / nullBitFileName;
	} 
	if(!boost::filesystem::exists(baseBitstreamPath)) {
		std::cout << "Base bitstream file - " << baseBitstreamPath.string()
			<< " - does not exist." << std::endl;
		return -1;
	}
	// Load the base bitstream file
	std::cout << "Loading base bitstream " << baseBitstreamPath << std::endl;
	std::ifstream baseBitstream;
	baseBitstream.open(baseBitstreamPath.string().c_str());
	mBitstreamPtr = torc::bitstream::Factory::newBitstreamPtr(baseBitstreamPath);
	// Initialize different settings of bitstream object.
	mBitstreamPtr->setDesignName(mDesignPtr->getName());
	mBitstreamPtr->initializeDeviceInfo(mBitstreamPtr->getDeviceName());
	mBitstreamPtr->initializeFrameMaps();
	return 0;
}

/// \details Protected function to internally initiate micro-bitstream assembly process.
///	This function should be called after design ptr, base bitstream, and library have be initialized.
void Assembler::convertXdlToBitstream() {
	assembleInstances();

	assembleNets();
	
	std::cout << "Unsupported tile type count " << mUnsupportedTileTypeSet.size() << std::endl;
	for(std::set<std::string>::iterator iter = mUnsupportedTileTypeSet.begin(); iter != mUnsupportedTileTypeSet.end(); iter++) {
		std::cout << "   " << *iter << std::endl;
	}
	std::cout << "Unsupported tile count : " << mUnsupportedTileCount << std::endl;
	std::cout << "Unsupported pip count : " << mUnsupportedPipCount << std::endl;
	std::cout << "Missing configs of supported tiles : " << mMissingConfigs << std::endl;
	
}
/// \details Function to assemble micro-bitstreams for instances of Xdl design.
/// 	This function iterates over instances, stores frame blocks and bit offset within for site location,
///	and then iterates over config settings of the instance. Ramb instances are handled in separate function.
///	Special cases for configurations are - lut equations, lut in ram/rom mode, compound configuration, and
///	configuration with hex values.
void Assembler::assembleInstances() {

	std::cout << "Assembling micro-bitstream for instances... count: " << mDesignPtr->getInstanceCount() <<
		std::endl;
	// Get instances begin and end
	InstanceSharedPtrVector::const_iterator pInstance = mDesignPtr->instancesBegin();
	InstanceSharedPtrVector::const_iterator eInstance = mDesignPtr->instancesEnd();

	// Iterate over all the instances
	while(pInstance != eInstance) {

		InstanceSharedPtr instancePtr = *pInstance++;
		string siteType = instancePtr->getType();
		// Check if instance is placed on supported site type
		//if(isSiteTypeSupported(siteType) ) {

			const string &siteName = instancePtr->getSite();
			if(siteName.empty()) {
				std::cout << "   WARNING: Unplaced instance " << instancePtr->getName() << endl;
				continue;
			}
			const string &tileType = getTiletypeFromSitename(siteName);
			std::cout << "  Processing instance " << instancePtr->getName() << " placed on site "
				<< siteName << " in tile " << tileType << " with " << instancePtr->getConfigCount() << " configs set."
				<< std::endl;

			getAnnotatedSiteTypeForSlicel(siteType, siteName);
			if(!tileAndSiteExistInLibrary(tileType, siteType))
				continue;
			// Store frame blocks and bit offset for current site location
			initializeFrameDataForSite(siteName);

			// RAMB sites are handled in a slightly different manner
			if(isRambSite(siteType)) {
				assembleRamb(instancePtr);
			} else {
				
				// Get the element to config map for the given site type
				SiteTypeToConfigSettings sitetypeToConfigSettings = mLibrary[tileType];
				ConfigSettingsToValues configSettingToValuesMap = sitetypeToConfigSettings[siteType];

				// Go over all the configurations of the instance
				ConfigMap::const_iterator pConfig = instancePtr->configBegin();
				ConfigMap::const_iterator eConfig = instancePtr->configEnd();

				while(pConfig != eConfig) {

					// Ignore routethrough config setting as they don't set bits in bitstream
					if(isRoutethrough(pConfig->first)) {
						pConfig++;
						continue;
					}

					// std::cout << "\tWorking on config " << pConfig->first << "::"
					//	<< pConfig->second.getValue() << std::endl;
					// Lut equation has to be handled differently from other config setting.
					// The boolean operation in the LUT equation have to be performed on the relevant bits.
					if(isLutEquationSetting(pConfig->second.getValue())) {
						mergeLutEquationBits(pConfig->first, pConfig->second.getValue(), configSettingToValuesMap);
					} // LUT ROM and RAM settings also need special care
					else if(isLutRamOrRomSetting(pConfig->second.getValue())) {
						mergeLutRamOrRomBits(pConfig->first, pConfig->second.getValue(), configSettingToValuesMap);
					} // Some elements together effect the bitstream.
					else if(isCompoundSetting(pConfig->first)) {
						mergeCompoundSettingBits(pConfig->first, pConfig->second.getValue(),
							instancePtr, configSettingToValuesMap);
					} // The DSP MASK and PATTERN have hex values
					else if(isConfigValHexString(instancePtr->getType(), pConfig->first)) {
						mergeHexConfigBits(pConfig->first, pConfig->second.getValue(), configSettingToValuesMap);
					} else {
						// Merge compressed bitstream to main bitstream if valid setting
						checkValidityAndMergeBitstream(pConfig->first, pConfig->second.getValue(),
							configSettingToValuesMap);
					}
					//std::cout << "\t------------------" << std::endl;
					pConfig++;
				}
			}
	//	} else {
	//		std::cout << "WARNING: Site " << instancePtr->getType() << " not supported."
	//			<< std::endl;
	//	}
	}

}

/// \details Function to assemble micro-bitstreams for net of Xdl design.
void Assembler::assembleNets() {
	// Get the iterators to nets
	NetSharedPtrVector::const_iterator pNets = mDesignPtr->netsBegin();
	NetSharedPtrVector::const_iterator eNets = mDesignPtr->netsEnd();

	std::cout << "Assembling micro-bitstreams for Nets... count: " << mDesignPtr->getNetCount()
		<< std::endl;
	// Iterate over nets
	while(pNets != eNets) {

		NetSharedPtr netPtr = *pNets;
		// std::cout << "  Processing net: " << netPtr->getName() << std::endl;

		// Iterate over pips in the net
		Net::PipConstIterator pPips = netPtr->pipsBegin();
		Net::PipConstIterator ePips = netPtr->pipsEnd();
		while(pPips != ePips) {
			Pip pip = *pPips++;
			// Store frame blocks and bit offset for this tile
			TileIndex tileIndex = mTiles.findTileIndex(pip.getTileName());
			initializeFrameDataForTile(tileIndex);

			const TileInfo& tileInfo = mTiles.getTileInfo(tileIndex);
			string tileType = mTiles.getTileTypeName(tileInfo.getTypeIndex());
			if(!tileAndSiteExistInLibrary(tileType, "routing")) {
				mUnsupportedPipCount++;
				continue;
			}
			ConfigSettingsToValues configSettingToValuesMap = mLibrary[tileType]["routing"];
			// std::cout << "\tTile " << tileInfo.getName() << " Src: " << pip.getSourceWireName()
			//	<< " Sink: " << pip.getSinkWireName() << std::endl;
			checkValidityAndMergeBitstream(pip.getSourceWireName(), pip.getSinkWireName(),
				configSettingToValuesMap);

		}

		pNets++;
	}

}

/// \brief Different architectures will have different sites supported.
///	Derived classes should override this function.
bool Assembler::isSiteTypeSupported(const string &inSiteType) {
	return false;
}

/// \brief Assemble micro-bitstream for ramb site.
///	First the base micro-bitstream is merged. Micro-bitstream info for memory and
///	parity init values are gathered from ll files.
void Assembler::assembleRamb(InstanceSharedPtr inInstancePtr) {
	// Get the element to config map for the given site type
	const string &tileType = getTiletypeFromSitename(inInstancePtr->getSite());
	ConfigSettingsToValues configSettingToValuesMap = mLibrary[tileType][inInstancePtr->getType()];

	// Open the RAMBIT memory file and store data in a vector
	// This file stores memory address to bitstream address map
	string memoryMapFileName = mParentFamilyName + "-" + tileType + "-map.bits";
	string rambMemoryMapFilePath = mLibraryPath.string() + "/memory/" + memoryMapFileName;
	std::ifstream rambMemoryBitFile(rambMemoryMapFilePath.c_str(), std::ios::binary);
	if(!rambMemoryBitFile.is_open()) {
		std::cout << "Could not open bram memory map file " << rambMemoryMapFilePath << std::endl;
		return;
	}

	std::vector<uint32_t> bitAddresses;
	uint32_t count = 0, bitAddress = 0;
	while(!rambMemoryBitFile.eof()) {
		// Alternate number is ignored.
		rambMemoryBitFile.read((char *) &count, 4);
		rambMemoryBitFile.read((char *) &bitAddress, 4);
		bitAddress = ntohl(bitAddress);
		bitAddresses.push_back(bitAddress);
	}
	rambMemoryBitFile.close();

	// Open the RAMB parity bit file and store the data in a vector
	string parMapFileName = mParentFamilyName + "-" + tileType + "-map-par.bits";
	string rambParityBitFilePath = mLibraryPath.string() + "/memory/" + parMapFileName;
	std::ifstream rambParityBitFile(rambParityBitFilePath.c_str(), std::ios::binary);
	if(!rambParityBitFile.is_open()) {
		std::cerr << "ERROR: Could not open file " << rambParityBitFilePath << std::endl;
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
	mergeWithBaseBitstream(configSettingToValuesMap[inInstancePtr->getType()]["BASE"], 0);

	// Go over all the configurations of the instance
	ConfigMap::const_iterator pConfig = inInstancePtr->configBegin();
	ConfigMap::const_iterator eConfig = inInstancePtr->configEnd();
	while(pConfig != eConfig) {

		std::cout << "\tWorking on config " << pConfig->first << "::"
			<< pConfig->second.getValue() << std::endl;

		// ToDo: Most of the code for memory and parity is same with only the bit address being different.
		//	Try to merge the two if statements.
		// RAMB memory and parity init values have to be handled differently.
		if(isMemoryInitSetting(pConfig->first)) {

			// Get the row number from config name. Last two character are row number in hex form.
			stringstream ssRow;
			uint32_t memoryInitRow;
			ssRow << std::hex << pConfig->first.substr(pConfig->first.length() - 2, 2);
			ssRow >> memoryInitRow;

			mergeRambInitBits(pConfig->second.getValue(), memoryInitRow, bitAddresses, 1);
		} else if(isMemoryParityInitSetting(pConfig->first)) {

			// Get the row number from config name. Last two character are row number in hex form.
			stringstream ssRow;
			uint32_t memoryInitRow;
			ssRow << std::hex << pConfig->first.substr(pConfig->first.length() - 2, 2);
			ssRow >> memoryInitRow;
			mergeRambInitBits(pConfig->second.getValue(), memoryInitRow, parityBitAddresses, 1);
		} else {
			// Merge compressed bitstream to main bitstream
			checkValidityAndMergeBitstream(pConfig->first, pConfig->second.getValue(), configSettingToValuesMap);
		}
		pConfig++;
	}
}

/// \details Get tile type from site location(name)
/// ToDo: A lot of this code is present in initializeFrameDataForSite()
string Assembler::getTiletypeFromSitename(const string &inSiteName) {
	SiteIndex siteIndex = mSites.findSiteIndex(inSiteName);
	const Site& site = mSites.getSite(siteIndex);
	TileIndex tileIndex = site.getTileIndex();
	const TileInfo& tileInfo = mTiles.getTileInfo(tileIndex);
	string tileType = mTiles.getTileTypeName(tileInfo.getTypeIndex());
	return tileType; 

}


/// \brief Check if configurations is present in library. If yes, merge the corresponding bits with the bitstream
void Assembler::checkValidityAndMergeBitstream(string inElementName, string inConfigVal,
	const ConfigSettingsToValues &inConfigSettingToValues) {

	ConfigSettingsToValues::const_iterator pConfigSettings = inConfigSettingToValues.find(inElementName);
	// If element found in the map
	if(pConfigSettings != inConfigSettingToValues.end()) {
		ConfigValuesToBits::const_iterator pConfigValues = pConfigSettings->second.find(inConfigVal);
		//If config value found in the config to bit map
		if(pConfigValues != pConfigSettings->second.end()) {
			// std::cout << "  Setting: " << inElementName << " . Config: " << inConfigVal << std::endl;
			mergeWithBaseBitstream(pConfigValues->second, 0);
		} else {
			mMissingConfigs++;
			std::cout << "WARNING: Config value " << inConfigVal << " for setting " << inElementName
				<< " not found in library." << std::endl;
		}
	} else {
		mMissingConfigs++;
		std::cout << "WARNING: Config setting " << inElementName << " not found in library." << std::endl;
	}
}
/// \brief Returns true if configuration is preseRnt in library
bool Assembler::elementAndConfigExistInLibrary(const string &inElementName,
	const string &inConfigValue, ConfigSettingsToValues &inConfigSettingToValues) {
	// Check if element exists
	ConfigSettingsToValues::iterator pConfigSettings = inConfigSettingToValues.find(inElementName);
	if(pConfigSettings != inConfigSettingToValues.end()) {

		// Check if config exists
		ConfigValuesToBits::iterator pConfigValues = pConfigSettings->second.find(inConfigValue);
		if(pConfigValues != pConfigSettings->second.end())
			return true;
	}
	std::cout << "WARNING: Config setting " << inElementName << " with value " << inConfigValue
		<< " not found in library" << std::endl;
	return false;
}

/// \brief If slice site type, annotate type as per even or odd column
void Assembler::getAnnotatedSiteTypeForSlicel(string &inOutSiteType, const string &siteLocation) {

    if(boost::regex_search(inOutSiteType.begin(), inOutSiteType.end(), boost::regex("SLICEL"))){
        // Get the column number of site location
        boost::smatch matchResults;
        boost::regex columnExpr("_X(\\d+)Y");
        if(boost::regex_search(siteLocation, matchResults, columnExpr)) {
            // First match is the whole string
            int column = boost::lexical_cast<int>(matchResults[1]);
			if(column % 2 == 0) 
				inOutSiteType = inOutSiteType + "E";
			else
				inOutSiteType = inOutSiteType + "O";
        }
    }
}

/// \details Merge micro-bitstream for Lut equation.
/// 	The bit operations (AND, OR, etc.) are performed on the micro-bitstreams corresponding to every literal.
///	Bit operation NOT is performed by XORing with micro-bitstream for LUT output assigned to 1.
///	The lut equation is parsed using bison and flex. When a literal is encountered, the set of frames
///	corresponding to literal is pushed on to a stack. When a bit operator is encountered, two set of frames
///	is poped from the stack and the bit operations is performed on the two set of frames and result is pushed back
///	to stack.
void Assembler::mergeLutEquationBits(const string &inElementName, const string &inConfigValue,
	ConfigSettingsToValues &inConfigSettingToValues) {

	// Get the right hand side and lefthand side of the LUT equation.
	std::vector<string> splitVector;
	boost::algorithm::split(splitVector, inConfigValue, boost::algorithm::is_any_of("="));

	// The left side tell which output (O5 or O6) is being affected. Store it in a global
	// so that it can be used by functions called from bison
	mLutCurrentEquationLhs = splitVector[0];
	string equationRight = splitVector[1];

	// Store the frames for LUT setting constant 1. XOR with these frames give NOT functionality.
	string lutEquationForOne = mLutCurrentEquationLhs + "=1";

	// Currently all the frames of a LUT setting are not stored. We know only 4 words get affected by LUT setting -
	// 4 frames and a word in each frame. So instead we store only 4 words and the frame index and word index.
	if(elementAndConfigExistInLibrary(inElementName, lutEquationForOne, inConfigSettingToValues)) {
		mCurrentConfigToBitMap = inConfigSettingToValues[inElementName];
		std::vector<uint32_t> bitAddressesForOne = mCurrentConfigToBitMap[lutEquationForOne];

		mLutCurrentReferenceFrameIndex = bitAddressesForOne[0] >> 16;
		mCurrentReferenceWordIndex = (bitAddressesForOne[0] & 0x0000FF00) >> 8;

		mLutFrameSetForOne.clear();
		mLutFrameSetForOne.resize(4, uint32_t(0));
		for(std::vector<uint32_t>::const_iterator bitIter = bitAddressesForOne.begin(); bitIter
			!= bitAddressesForOne.end(); bitIter++) {
			int32_t bitIndex = (*bitIter) & 0x000000FF;
			int32_t frameIndex = (*bitIter) >> 16;
			uint32_t frameWord = 1 << (bitIndex - 1);

			mLutFrameSetForOne[frameIndex - mLutCurrentReferenceFrameIndex]
				= mLutFrameSetForOne[frameIndex - mLutCurrentReferenceFrameIndex] | frameWord;
		}

		if(!processLut(equationRight.c_str(), inConfigValue)) {
			std::cout << "ERROR: Error in parsing LUT equation " << inConfigValue << std::endl;
			/// \todo Do we really want to abort here just because the user has an incorrect expression?
			exit(-1);
		}

		// Build bit addresses out of the last frame in gStackOfFrames
		std::vector<uint32_t> combinedLUTWords = mLutFrameSetStack[0];
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
					bitAddresses.push_back(((mLutCurrentReferenceFrameIndex + frameOffset) << 16)
						| (mCurrentReferenceWordIndex << 8) | bitIndex);
				}
				tempWord = tempWord >> 1;
				bitIndex++;
			}
		}

		mergeWithBaseBitstream(bitAddresses, 0);
		mLutFrameSetStack.clear();
	}
}

/// \details Merge micro-bitstream for lut in ram/rom mode. The lut ram/rom memory address
///	to bit address map is obtained from ll files.
void Assembler::mergeLutRamOrRomBits(const string &inElementName, const string &inConfigVal, 
					ConfigSettingsToValues &inConfigSettingToValues) {

	// Since memory content are similar for SLICEL and SLICEM,
	// they are stored in library under element name SLICE
	// ConfigSettingsToValues &configSettingToValuesMap = mLibrary["CLBLM"]["SLICE"];

	// Get the right hand side and lefthand side of the LUT equation.
	std::vector<string> splitVector;
	boost::algorithm::split(splitVector, inConfigVal, boost::algorithm::is_any_of("="));
	// Remove the first two characters - 0x - from the hex value
	string memoryValue = splitVector[1].substr(2);

	if(memoryValue.length() != 8 && memoryValue.length() != 16) {
		std::cout << "WARNING: Illegal configuration of element " << inElementName << std::endl;
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
	string elementNameForMemoryMode = inElementName.substr(0, 1);
	std::vector<uint32_t> bitAddressesForROM;

	if(!elementAndConfigExistInLibrary(elementNameForMemoryMode, "ROM", inConfigSettingToValues))
		return;

	bitAddressesForROM = inConfigSettingToValues[elementNameForMemoryMode]["ROM"];
	std::vector<uint32_t> bitAddresses;
	// Iterate over the memory string from left to right
	for(uint32_t charIndex = 0; charIndex < memoryValue.length(); charIndex++) {

		stringstream sshexChar;
		sshexChar << std::hex << memoryValue[charIndex];
		uint32_t hexDigit;
		sshexChar >> hexDigit;
		// Go over the bits of the hex digit from left to right
		for(int bitIndex = 0; hexDigit != 0; bitIndex++) {
			if(hexDigit & 8) {
				uint32_t memoryAddress = (charIndex << 2) + bitIndex;
				stringstream ssConfigBitIndex;
				ssConfigBitIndex << memoryAddress;
				bitAddresses.push_back(bitAddressesForROM[memoryAddress]);
			}
			hexDigit = hexDigit << 1;
		}
	}
	mergeWithBaseBitstream(bitAddresses, 0);
}

/// \details Merge micro-bitstream for compound setting. There are some settings which
///	together affect the bitstream.
void Assembler::mergeCompoundSettingBits(string inElement1Name, string inConfig1Val,
	InstanceSharedPtr inInstancePtr, const ConfigSettingsToValues &inConfigSettingToValues) {

	std::vector<string> compoundElements = getDependantConfigs(inElement1Name);
	// For now assume there is only on element in the vector, i.e. there is a compound
	// setting with only two elements.
	if(compoundElements.size() != 1) {
		std::cout
			<< "WARNING: Compound setting with more than two configurations not handled currently"
			<< std::endl;
	} else {

		// The two element names will be concatenated to form a compound element name Element1Element2.
		// Similarly two config settings will be concatenated to form a compound config name.
		std::string element2Name = compoundElements[0];
		std::string compoundElementName = inElement1Name + element2Name;

		// Find the 2nd element config in the instance
		std::string config2Val, config2Name;
		inInstancePtr->getConfig(element2Name, config2Name, config2Val);

		// Element 2 config not set
		if(config2Val.compare(sConfigOff) == 0) {
			std::cout << "Illegal configuration. Element " << element2Name << " must be set "
				<< " along with element " << inElement1Name << std::endl;
			return;
		}

		// Get compound config val
		std::string compoundConfigVal = inConfig1Val + config2Val;

		// Verify setting and merge bits
		checkValidityAndMergeBitstream(compoundElementName, compoundConfigVal, inConfigSettingToValues);
	}

}

/// \details Merge micro-bitstream for configurations with hex values. The hex string is parsed
///	character by character and micro-bitstream for every set bit is retrived from library and
///	merged with base bitstream.
void Assembler::mergeHexConfigBits(string inElementName, string inConfigVal,
	const ConfigSettingsToValues &inConfigSettingToValues) {
	std::vector<uint32_t> inBitAddresses;

	// Go over all the characters of the config val string
	for(uint32_t charIndex = 0; charIndex < inConfigVal.length(); charIndex++) {
		stringstream sshexChar;
		sshexChar << std::hex << inConfigVal[inConfigVal.length() - charIndex - 1];
		uint32_t hexDigit;
		sshexChar >> hexDigit;
		// Go over all the bits of the hex digit
		for(int bitIndex = 0; hexDigit != 0; bitIndex++) {
			if(hexDigit & 1) {
				int configBitIndex = (charIndex << 2) + bitIndex;
				stringstream ssConfigBitIndex;
				ssConfigBitIndex << configBitIndex;
				checkValidityAndMergeBitstream(inElementName, ssConfigBitIndex.str(), inConfigSettingToValues);
			}
			hexDigit = hexDigit >> 1;
		}
	}
}

/// \details Merges micro-bitstream for ramb init (both memory and parity) values.
void Assembler::mergeRambInitBits(const string &inConfigVal, uint32_t inMemoryInitRow,
	const vector<uint32_t> &inRamBitAddress, uint32_t inBlock) {

	vector<uint32_t> bitAddresses; // vector to hold bit addresses of set memory bits

	uint32_t configValLength = inConfigVal.length();
	uint32_t numBitsPerConfig = configValLength << 2; // 4 bits every hex character

	// Go over the memory string from right to left
	for(uint32_t charIndex = 0; charIndex < configValLength; charIndex++) {
		//stringstream sshexChar;
		//sshexChar << std::hex << configVal[configValLength - charIndex - 1];
		int hexDigit;
		hexDigit = hexCharacterToDec(inConfigVal[configValLength - charIndex - 1]);
		//sshexChar >> hexDigit;
		// Go over the bits of hex digit from right to left
		for(int bitIndex = 0; hexDigit != 0; bitIndex++) {
			if(hexDigit & 1) {
				// Get memory address of set bit taking into account which row of memory is being processed
				uint32_t configBitIndex = (charIndex << 2) + bitIndex;
				uint32_t memoryAddress = inMemoryInitRow * numBitsPerConfig + configBitIndex;

				bitAddresses.push_back(inRamBitAddress[memoryAddress]);
			}
			hexDigit = hexDigit >> 1;
		}
	}
	mergeWithBaseBitstream(bitAddresses, inBlock);
}

/// \details Loads micro-bitstream library from given path and populates the data
///	in a map.
void Assembler::populateLibraryMap(boost::filesystem::path inLibDBPath) {

	std::ifstream libDB(inLibDBPath.string().c_str(), std::ios::binary);
	if(!libDB.good()) {
		std::cout << "Could not open micro-bitstream DB file " << inLibDBPath.string() << std::endl;
		libDB.exceptions(std::ios::failbit);
		exit(-1);
	}
	std::cout << "Opened micro-bitstream DB file " << inLibDBPath.string() << std::endl;
	char buffer[1024];
	libDB.read(buffer, 16);
	buffer[16] = '\0';
	string libDBSanity(buffer);
	std::cout << "Sanity string " << libDBSanity << std::endl;
	if(libDBSanity != "<<<<BITLIBDB>>>>") {
		std::cout << "This file seems to be currupt- " << inLibDBPath.string() << std::endl;
		exit(-1);
	}
	uint32_t tileTypeCount = 0;
	readWord(libDB, tileTypeCount);
	std::cout << "Tile type count " << tileTypeCount << std::endl;

	for(uint32_t tileIndex = 0; tileIndex < tileTypeCount; tileIndex++) {
		uint32_t tileNameCharCount = 0;
		readWord(libDB, tileNameCharCount);
		libDB.read(buffer, tileNameCharCount);
		buffer[tileNameCharCount] = '\0';
		string tileType(buffer);
		uint32_t sitetypeCount = 0;
		readWord(libDB, sitetypeCount);
		std::cout << "Tile type: " << tileType << ". Site count: " << sitetypeCount << std::endl;

		SiteTypeToConfigSettings sitetypeToSettingsMap;

		// Iterate over the elements
		for(uint32_t sitetypeIndex = 0; sitetypeIndex < sitetypeCount; sitetypeIndex++) {
			// Get element name char count
			uint32_t sitetypeCharCount = 0;
			readWord(libDB, sitetypeCharCount);
			libDB.read(buffer, sitetypeCharCount);
			buffer[sitetypeCharCount] = '\0';
			string sitetype(buffer);

			uint32_t configSettingCount = 0;
			readWord(libDB, configSettingCount);
			//std::cout << "   Site " << sitetype << ". Config setting count " << configSettingCount << std::endl;

			ConfigSettingsToValues configSettingToValues;

			// Iterate over configs
			for(uint32_t configSettingIndex = 0; configSettingIndex < configSettingCount; configSettingIndex++) {
				uint32_t configSettingCharCount = 0;
				readWord(libDB, configSettingCharCount);
				//				std::cout << "\tConfig char count " << configSettingCharCount;
				libDB.read(buffer, configSettingCharCount);
				buffer[configSettingCharCount] = '\0';
				string configSetting(buffer);
				//std::cout << "\t" << configSetting << std::endl;

				uint32_t configValuesCount = 0;
				readWord(libDB, configValuesCount);

				ConfigValuesToBits configMap;

				for(uint32_t configValueIndex = 0; configValueIndex < configValuesCount; configValueIndex++) {
					uint32_t configValueCharCount = 0;
					readWord(libDB, configValueCharCount);
					libDB.read(buffer, configValueCharCount);
					buffer[configValueCharCount] = 0;
					string configValue(buffer);
					// Read the compressed word count
					uint32_t wordCount = 0;
					readWord(libDB, wordCount);
					std::vector<uint32_t> addresses;
					uint32_t word;
					for(uint32_t i = 0; i < wordCount; i++) {
						readWord(libDB, word);
						addresses.push_back(word);
					}
					configMap[configValue] = addresses;	
				}

				configSettingToValues[configSetting] = configMap;
			}
			sitetypeToSettingsMap[sitetype] = configSettingToValues;
		}
		mLibrary[tileType] = sitetypeToSettingsMap;
	}
}

/// \details Save the assembled bitstream.
void Assembler::saveBitstream() {

	std::ofstream customBitstreamFile(mTargetBitstreamPath.string().c_str(), std::ios::binary);
	std::cout << "Writing custom bitstream file " << mTargetBitstreamPath << std::endl;
	mBitstreamPtr->write(customBitstreamFile);

	customBitstreamFile.close();
}

/// \details Perform bit operation on two frame set from stack for lut operation.
void Assembler::binaryLutFrameOperation(Assembler::EOperation inOperation) {

	bool debug = false;
	if(debug) {
		std::cout << "In DoBinaryOperation " << inOperation << std::endl;
	}
	std::vector<uint32_t> lutWords1 = mLutFrameSetStack.back();
	std::vector<uint32_t> lutWordsNew;
	mLutFrameSetStack.pop_back();
	std::vector<uint32_t> lutWords2;
	if(inOperation == eOR || inOperation == eAND || inOperation == eXOR) {
		lutWords2 = mLutFrameSetStack.back();
		mLutFrameSetStack.pop_back();
	} else if(inOperation == eNOT) {
		lutWords2 = mLutFrameSetForOne;
	} else {
		cout << "WARNING: Unknown LUT operation " << inOperation << std::endl;
		return;
	}
	std::vector<uint32_t>::const_iterator pWord1 = lutWords1.begin();
	std::vector<uint32_t>::const_iterator eWord1 = lutWords1.end();
	std::vector<uint32_t>::const_iterator pWord2 = lutWords2.begin();
	std::vector<uint32_t>::const_iterator eWord2 = lutWords2.end();
	if(inOperation == eOR) {
		while(pWord1 != eWord1 && pWord2 != eWord2) {
			lutWordsNew.push_back((*pWord1 | *pWord2));
			pWord1++;
			pWord2++;
		}
	} else if(inOperation == eAND) {
		while(pWord1 != eWord1) {
			lutWordsNew.push_back((*pWord1 & *pWord2));
			pWord1++;
			pWord2++;
		}
	} else if(inOperation == eXOR || inOperation == eNOT) {
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

	mLutFrameSetStack.push_back(lutWordsNew);
}

/// \details Push set of frames corresponding to literal passed for current lut equation.
void Assembler::pushLutFrame(string inLiteral) {
	bool debug = false;

	if(debug) {
		std::cout << "In PushFrameToStack " << inLiteral << std::endl;
	}

	std::vector<uint32_t> bitAddresses = mCurrentConfigToBitMap[mLutCurrentEquationLhs + "="
		+ inLiteral];

	/// \todo May need to find a more general to access and push passthrough frame sets
	// We know LUT bits go across 4 frames and effect the same word in each frame
	std::vector<uint32_t> lutWords(4, 0);
	for(std::vector<uint32_t>::const_iterator bitIter = bitAddresses.begin(); bitIter
		!= bitAddresses.end(); bitIter++) {
		int32_t bitIndex = (*bitIter) & 0x000000FF;
		int32_t frameIndex = (*bitIter) >> 16;
		uint32_t frameWord = 1 << (bitIndex - 1);

		lutWords[frameIndex - mLutCurrentReferenceFrameIndex] |= frameWord;
	}

	if(debug) {
		for(std::vector<uint32_t>::const_iterator iter = lutWords.begin(); iter != lutWords.end(); iter++) {
			std::cout << "   " << Hex32(*iter);
		}
		std::cout << std::endl;
	}

	mLutFrameSetStack.push_back(lutWords);
}

bool Assembler::processLut(const string& in, const string& name) {
	mStreamName = name;
	istringstream iss(in);
	mSuccess = true;

	LutScanner scanner(&iss);
	scanner.set_debug(mTraceScanning);
	this->lexer = &scanner;
	LutParser parser(*this);
	parser.set_debug_level(mTraceParsing);
	bool result = parser.parse() == 0;
	return mSuccess && result;
}

void Assembler::error(const location& l, const string& m) {
	failure();
	cerr << l << ": " << m << std::endl;
}

void Assembler::error(const string& m) {
	failure();
	cerr << m << std::endl;
}

} // namespace bitstream
} // namespace torc
