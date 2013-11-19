// Torc - Copyright 2013 University of Southern California.  All Rights Reserved.
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

/// \brief Class to encapsulate micro-bitstream library generation code
/// \author Ritesh Kumar Soni

#include "Virtex5LibGen.hpp"
#include "torc/packer/Virtex5PrimitiveStructure.hpp"

namespace torc {
namespace bitstream {

const int Virtex5LibGen::sCrcWordIndex = 20;
const string Virtex5LibGen::sTilesSupportedForPips[] = {
								"INT",
								"CLBLL",
								"CLBLM",
								"CLK_HROW",
								"HCLK",
								"CLK_BUFGMUX",
								"CFG_CENTER",
								"CFG_HCLK_INTERFACE",
								"CLKV",
								"CLKV_MC",
								"CLK_CMT_BOT",
								"CLK_CMT_BOT_MGT",
								"CLK_CMT_TOP",
								"CLK_CMT_TOP_MGT",
								"CLK_HROW_MGT",
								"CLK_IOB_B",
								"CLK_IOB_T",
								"CLK_MGT_BOT",
								"CLK_MGT_BOT_MGT",
								"CLK_MGT_TOP",
								"CLK_MGT_TOP_MGT",
								"CLK_TERM_BOT",
								"CLK_TERM_TOP",
								"HCLK_BRAM",
								"HCLK_BRAM_MGT",
								"HCLK_BRAM_MGT_LEFT",
								"HCLK_CLB",
								"HCLK_CMT_CMT",
								"HCLK_CMT_CMT_MGT",
								"HCLK_CMT_IOI",
								"HCLK_DSP",
								"HCLK_GT3",
								"HCLK_GTX",
								"HCLK_GTX_LEFT",
								"HCLK_IOB",
								"HCLK_IOB_CMT_BOT",
								"HCLK_IOB_CMT_BOT_MGT",
								"HCLK_IOB_CMT_MID",
								"HCLK_IOB_CMT_MID_MGT",
								"HCLK_IOB_CMT_TOP",
								"HCLK_IOB_CMT_TOP_MGT",
								"HCLK_IOI",
								"HCLK_IOI_BOTCEN",
								"HCLK_IOI_BOTCEN_MGT",
								"HCLK_IOI_CENTER",
								"HCLK_IOI_CMT",
								"HCLK_IOI_CMT_MGT",
								"HCLK_IOI_TOPCEN",
								"HCLK_IOI_TOPCEN_MGT",
								"HCLK_LIOB",
								"HCLK_PCIE_BRAM",
								"HCLK_PPC",
								"HCLK_PPC_TERM",
								"HCLK_TERM_L",
								"HCLK_TERM_R",
								"HCLK_VBRK",
								"INT_HCLK_BUFS",
								"INT_BUFS_L",
								"INT_BUFS_R",
								"IOI",
								"LAST_TILE"
							};

/// \details Architecture specific part of initializing the object
void Virtex5LibGen::initialize() {

	mCompoundSettingMap = getCompoundSettingsMap();
	mReferenceConfigMap = getReferenceConfigMap();
}

/// \details Returns true for the sites which are supported by library generation process
bool Virtex5LibGen::isSiteTypeSupported(const string &siteType) {
	return (siteType.compare("SLICEM") == 0 	|| 
			siteType.compare("SLICELO") == 0 	|| siteType.compare("SLICELE") == 0 ||
			siteType.compare("RAMB36_EXP") == 0 || siteType.compare("DSP48E") == 0 || 
//			siteType.compare("BUFGCTRL") == 0 	|| siteType.compare("BUFDS") == 0 || 
			siteType.compare("*BUFR") == 0 		|| siteType.compare("*BUFG") == 0);
}

/// \brief Returns list of supported tiles.
std::vector<std::string> Virtex5LibGen::getTilesSupportedForPips() {
	uint32_t supportedTileIndex = 0;
	std::vector<std::string> tilesSupported;
	while(sTilesSupportedForPips[supportedTileIndex].compare("LAST_TILE") != 0) {
		tilesSupported.push_back(sTilesSupportedForPips[supportedTileIndex++]);
	}
	return tilesSupported;
}

/// \brief Compress given bitstream file against reference bitstream file
void Virtex5LibGen::compressBitFile(path primaryFile, path referenceFile) {

	bool debug = false;

	// Read the bitstream files
	torc::bitstream::BitstreamSharedPtr referenceBitstreamPtr = Factory::newBitstreamPtr(referenceFile);
	torc::bitstream::BitstreamSharedPtr primBitstreamPtr = Factory::newBitstreamPtr(primaryFile);

	// Check if both the bitstreams were read successfully.
	if(referenceBitstreamPtr && primBitstreamPtr) {
		std::cout << "Read the two bit files - " << primaryFile.filename() << " and " << referenceFile.filename() << " successfully" << std::endl;
	} else {
		std::cout << "Could not read the bit files " << primaryFile << " " << referenceFile << std::endl;
		return;
	}

	// dynamically cast the bitstream pointer into a Virtex5 bitstream pointer
	// ToDo: This part should move to architecture specific derived class
	boost::shared_ptr<torc::bitstream::Virtex5> refVirtex5BitstreamPtr = boost::dynamic_pointer_cast<torc::bitstream::Virtex5>(
		referenceBitstreamPtr);

	boost::shared_ptr<torc::bitstream::Virtex5> primVirtexBitstreamPtr = boost::dynamic_pointer_cast<torc::bitstream::Virtex5>(
		primBitstreamPtr);

	// initialize frame map
	refVirtex5BitstreamPtr->initializeDeviceInfo(referenceBitstreamPtr->getDesignName());
	refVirtex5BitstreamPtr->initializeFrameMaps();
	refVirtex5BitstreamPtr->readFramePackets();

	primVirtexBitstreamPtr->initializeDeviceInfo(primBitstreamPtr->getDesignName());
	primVirtexBitstreamPtr->initializeFrameMaps();
	primVirtexBitstreamPtr->readFramePackets();

	uint32_t wordOffset = getWordOffsetFromXdlFile(primaryFile, primVirtexBitstreamPtr);

	// look up the frame blocks
	torc::bitstream::VirtexFrameBlocks& refFrameBlocks = refVirtex5BitstreamPtr->getFrameBlocks();
	torc::bitstream::VirtexFrameBlocks& primFrameBlocks = primVirtexBitstreamPtr->getFrameBlocks();
	std::vector<boost::int32_t> combinedAddressVector;


	// iterate over the block types
	for(int blockIndex = 0; blockIndex < Bitstream::eBlockTypeCount; blockIndex++) {
		// look up the frame set for this block
		torc::bitstream::VirtexFrameSet& nullFrameSet = refFrameBlocks.mBlock[blockIndex];
		torc::bitstream::VirtexFrameSet& primFrameSet = primFrameBlocks.mBlock[blockIndex];

		if(nullFrameSet.size() != primFrameSet.size()) {
			std::cout << "Frame set sizes do not match" << std::endl;
			return;
		}

		// We expect the majorIndex to remain same for all the mismatching part of the bitstream.
		uint32_t relativeFrameIndex = 0, currentMajorIndex = 0, previousMajorIndex = 0;
		bool firstMismatch = true;

		// iterate over the frames
		for(uint32_t frameIndex = 0; frameIndex < nullFrameSet.size(); frameIndex++) {
			// look up the frame for this set
			VirtexFrameSharedPtr nullFramePtr = nullFrameSet[frameIndex];
			VirtexFrameSharedPtr primFramePtr = primFrameSet[frameIndex];

			const VirtexFrame::word_t* wordsReference = nullFramePtr->getWords();
			const VirtexFrame::word_t* wordsPrim = primFramePtr->getWords();

			const VirtexFrame::word_t* pReferenceWords = wordsReference;
			const VirtexFrame::word_t* pPrimWords = wordsPrim;
			const VirtexFrame::word_t* eReferenceWords = pReferenceWords
				+ nullFramePtr->getLength();
			const VirtexFrame::word_t* ePrimWords = pPrimWords + primFramePtr->getLength();

			if(nullFramePtr->getLength() != primFramePtr->getLength()) {
				std::cout << "WARNING: Frames of different sizes " << nullFramePtr->getLength() << " "
					<< primFramePtr->getLength() << std::endl;
			}

			// iterate over the frame words
			bool found = false;
			int wordIndex = 0;

			// Iterate over the frame words
			while(pReferenceWords < eReferenceWords) {
				// If the word is not zero and not same as that of reference bitstream and not crc word
				if(*pPrimWords != 0 && *pReferenceWords != *pPrimWords && wordIndex != sCrcWordIndex) {

					boost::int32_t bitIndex = 0;

					primVirtexBitstreamPtr->splitFrameIndex(frameIndex, currentMajorIndex,
						relativeFrameIndex);

					// If this is not the first mismatch, compare the major index with previous mismatching word's major index.
					if(!firstMismatch) {
						if(currentMajorIndex != previousMajorIndex) {
							std::cout << "WARNING: Bitstream mismatch across major indices: " << currentMajorIndex << " and " << previousMajorIndex << std::endl;
							//exit (-1);
							return;
						}
					} else {
						firstMismatch = false;
						previousMajorIndex = currentMajorIndex;
					}

					// Get the 1 bits in primary design which are zero in reference design
					VirtexFrame::word_t tempWord = (*pPrimWords ^ *pReferenceWords) & *pPrimWords;
					// Find all the 1s in the word
					while(tempWord != 0) {
						// check if the LSB is 1
						if(tempWord & 1) {
							// store the combined address
							combinedAddressVector.push_back((relativeFrameIndex << 16) | ((wordIndex - wordOffset)
								<< 8) | bitIndex);

						}
						tempWord = tempWord >> 1;
						bitIndex++;
					}
					found = true;
				}
				pReferenceWords++;
				pPrimWords++;
				wordIndex++;
			}
			// if we found any non-zero words, display them. This if for debugging purpose only.
			if(found) {
				std::cout << "Found unmatching frame: block " << blockIndex << ", relative index "
					<< Hex32(relativeFrameIndex) << ", global index: " << Hex32(frameIndex)
					<< ", major index: " << Hex32(currentMajorIndex) << std::endl;
				pReferenceWords = wordsReference;
				if(debug) {
					std::cout << "    Referen: ";
					while(pReferenceWords < eReferenceWords) {
						std::cout << Hex32(*pReferenceWords++) << " ";
					}
					std::cout << std::endl;
					pPrimWords = wordsPrim;
					std::cout << "    Primary: ";
					while(pPrimWords < ePrimWords) {
						std::cout << Hex32(*pPrimWords++) << " ";
					}
					std::cout << std::endl;
				}
			}
		}
	}

	// Store all the combined addresses in the sparse bit file.
	boost::filesystem::path sparceBitstreamPath = primaryFile.replace_extension().string()
		+ ".cbit";

	std::ofstream sparceBitstreamFile;
	sparceBitstreamFile.open(sparceBitstreamPath.string().c_str(), std::ios::binary | std::ios::out);
	if(sparceBitstreamFile.good()) {
		std::cout << " Opened library file to write " << sparceBitstreamPath.string().c_str()
			<< std::endl;
	} else {
		std::cerr << " Could not open library file to write - "
			<< sparceBitstreamPath.string().c_str() << std::endl;
		exit(-1);
	}

	boost::int32_t vectorSize = combinedAddressVector.size();
	sparceBitstreamFile.write((char *) &vectorSize, 4);
	std::cout << "Different words -" << std::endl;
	std::cout << "    ";
	for(std::vector<boost::int32_t>::const_iterator iter = combinedAddressVector.begin(); iter
		!= combinedAddressVector.end(); iter++) {
		sparceBitstreamFile.write((char *) (&*iter), 4);
		std::cout << Hex32((*iter)) << " ";
	}
	std::cout << std::endl;

	std::cout << "Wrote data to the library file." << std::endl;

	sparceBitstreamFile.close();

}

uint32_t Virtex5LibGen::getWordOffsetFromTileLocation(TileRow tileRow, TileCol tileCol, BitstreamSharedPtr inBitstreamPtr){
	uint32_t bitIndexBegin = 0, bitIndexEnd = 0;
	boost::shared_ptr<Virtex5> virtex5Ptr = boost::dynamic_pointer_cast<Virtex5>(inBitstreamPtr);
	uint32_t primaryXdlCol = virtex5Ptr->getPrimaryXdlColumn(tileCol);
	virtex5Ptr->getXdlFrames(tileRow, primaryXdlCol,
		bitIndexBegin, bitIndexEnd);

	// Each frame covers the full height of clock region which contains many rows of tiles.
	// The library contains configuration bits only for the first row. So the word index has to
	// be offset to reach the correct row. The bit index remains same across rows.
	uint32_t outWordOffset = 0;
	if(bitIndexBegin != bitIndexEnd) {
		outWordOffset = Virtex5::eFrameLength - (bitIndexEnd >> 5); // eFrameLength is number of words in a frame
	}

//	std::cout << "*******Word offset " << outWordOffset << std::endl;

	return outWordOffset;
}

/// \details Creates map from primitive name to primitive structure object pointer
AssemblerLibGen::PrimitiveStructuresSharedPtrMap Virtex5LibGen::getPrimitiveNameToStructreMap() {
	PrimitiveStructuresSharedPtrMap primitiveStructures;
	// look up the primitive def types
	typedef const torc::architecture::Array<const torc::architecture::PrimitiveDef>
		PrimitiveDefArray;
	PrimitiveDefArray & primitiveDefs = mSites.getSiteTypes();
	PrimitiveDefArray::const_iterator pPrimitiveDefs = primitiveDefs.begin();
	PrimitiveDefArray::const_iterator ePrimitiveDefs = primitiveDefs.end();
	// Store the corresponding primitive structures in a map.
	while(pPrimitiveDefs < ePrimitiveDefs) {
		// create a PrimitiveStructure object for this PrimitiveDef
		torc::packer::PrimitiveStructureSharedPtr primitiveStructurePtr(
			new torc::packer::Virtex5PrimitiveStructure(&*pPrimitiveDefs++));
		const torc::architecture::PrimitiveDef *primitiveDefPtr =
			primitiveStructurePtr->getPrimitiveDefPtr();
		const std::string & primitiveDefName = primitiveDefPtr->getName();
		// insert the PrimitiveStructure into the map
		if(!isSlicelType(primitiveDefName)) {
			primitiveStructures[primitiveDefName] = primitiveStructurePtr;
		} else {
			string sliceEven = primitiveDefName + "E";
			string sliceOdd = primitiveDefName + "O";
			primitiveStructures[sliceEven] = primitiveStructurePtr;
			primitiveStructures[sliceOdd] = primitiveStructurePtr;
		}
	}
	return primitiveStructures;
}

void Virtex5LibGen::generateMemoryMicroBitstream() {
	// Get compressed bit files for LUTs in RAM/ROM mode.
	path libraryMemFolder = mLibraryFolder / "memory";
	string perlScript = libraryMemFolder.string() + "/extract-bits-for-luts.pl";
	string llFile = mDB.getFamilyName() + "-CLBLM-SLICEM-X-ROM.ll";
	path llFilePath = libraryMemFolder / llFile;
	string systemCommand = "perl " +  perlScript + " " + llFilePath.string();
	int systemReturn = system(systemCommand.c_str());
	string cpyCmd = "mv " + libraryMemFolder.string() + "/*.cbit " + mXdlGenerationFolder.string();
	systemReturn = system(cpyCmd.c_str());
	(void) systemReturn;

}

} //namespace bitstream
} //namespace torc

