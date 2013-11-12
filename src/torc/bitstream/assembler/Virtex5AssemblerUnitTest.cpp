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

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/assembler/Virtex5Assembler.hpp"
#include "torc/bitstream/assembler/AssemblerFactory.hpp"
#include "torc/bitstream/Factory.hpp"
#include <boost/filesystem.hpp>

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)

bool CompareVirtexBitstreams(boost::filesystem::path bitFile1, boost::filesystem::path bitFile2);
void GenerateBitstreamAndCompare(boost::filesystem::path &xdlFilePath);


BOOST_AUTO_TEST_CASE(hexCharacterToDec) {
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('0'),  0);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('1'),  1);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('2'),  2);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('3'),  3);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('4'),  4);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('5'),  5);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('6'),  6);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('7'),  7);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('8'),  8);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('9'),  9);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('A'), 10);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('B'), 11);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('C'), 12);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('D'), 13);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('E'), 14);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('F'), 15);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('a'), 10);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('b'), 11);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('c'), 12);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('d'), 13);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('e'), 14);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('f'), 15);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('@'), -1);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('g'), -1);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec(' '), -1);
	BOOST_CHECK_EQUAL(Assembler::hexCharacterToDec('~'), -1);
}

BOOST_AUTO_TEST_CASE(Virtex5Slicel) {

	// look up the command line arguments
	int& argc = boost::unit_test::framework::master_test_suite().argc;
	char**& argv = boost::unit_test::framework::master_test_suite().argv;

	BOOST_REQUIRE(argc >= 1);
	// resolve symbolic links if applicable
	torc::common::DirectoryTree directoryTree(argv[0]);

	boost::filesystem::path testFolder = torc::common::DirectoryTree::getExecutablePath() / "torc/bitstream/test_cases";

	boost::filesystem::path xdlFilePath = testFolder / "slicel.xdl";
}

void GenerateBitstreamAndCompare(boost::filesystem::path &xdlFilePath) {
	// Open the XDL file and check if it opens successfully
	std::fstream xdlFileStream(xdlFilePath.string().c_str());
	BOOST_REQUIRE(xdlFileStream.good());

	// Import the XDL file
	torc::physical::XdlImporter importer;
	importer(xdlFileStream, "XdlDesignExample");
	// Get the design pointer
	torc::physical::DesignSharedPtr designPtr = importer.getDesignPtr();
	torc::architecture::DDB ddb(designPtr->getDevice());

	boost::filesystem::path customBitFile = xdlFilePath.replace_extension("custom.bit");
	boost::filesystem::path xilinxBitFile = xdlFilePath.replace_extension("bit");

	// Create bitstream assembler object
	torc::bitstream::AssemblerSharedPtr bitAssemblerPtr = torc::bitstream::AssemblerFactory::newAssemblerPtr(designPtr, ddb);
	bitAssemblerPtr->generateBitstream(designPtr, customBitFile);

	CompareVirtexBitstreams(customBitFile, xilinxBitFile);
}

bool CompareVirtexBitstreams(boost::filesystem::path bitFile1, boost::filesystem::path bitFile2) {

	using namespace torc::bitstream;

	bool returnCode = false;
	const int crcWordIndex = 20; // There are 41 words in the frame and CRC word is the middle one

	// Read the two bitstreams
	BitstreamSharedPtr bitstreamPtr1 = torc::bitstream::Factory::newBitstreamPtr(bitFile1);
	BitstreamSharedPtr bitstreamPtr2 = torc::bitstream::Factory::newBitstreamPtr(bitFile2);

	// Proceed only if both bitstreams were read successfully
	BOOST_REQUIRE(bitstreamPtr1 != NULL);
	BOOST_REQUIRE(bitstreamPtr2 != NULL);

	// dynamically cast the bitstream pointer into a Virtex5 bitstream pointer
	boost::shared_ptr<Virtex> virtexBitstreamPtr1 = boost::dynamic_pointer_cast<Virtex>(
			bitstreamPtr1);

	boost::shared_ptr<Virtex> virtexBitstreamPtr2 = boost::dynamic_pointer_cast<Virtex>(
			bitstreamPtr2);
	BOOST_REQUIRE(virtexBitstreamPtr1 != NULL);
	BOOST_REQUIRE(virtexBitstreamPtr2 != NULL);

	// initialize frame map
	virtexBitstreamPtr1->initializeDeviceInfo(bitstreamPtr1->getDesignName());
	virtexBitstreamPtr1->initializeFrameMaps();
	virtexBitstreamPtr1->initializeFullFrameBlocks();

	virtexBitstreamPtr2->initializeDeviceInfo(bitstreamPtr2->getDesignName());
	virtexBitstreamPtr2->initializeFrameMaps();
	virtexBitstreamPtr2->initializeFullFrameBlocks();

	// look up the frame blocks
	VirtexFrameBlocks& frameBlocks1 = virtexBitstreamPtr1->getFrameBlocks();
	VirtexFrameBlocks& frameBlocks2 = virtexBitstreamPtr2->getFrameBlocks();

	// iterate over the block types
	for(int blockIndex = 0; blockIndex < Virtex5::eFarBlockTypeCount; blockIndex++) {
		// look up the frame set for this block
		VirtexFrameSet& frameSet1 = frameBlocks1.mBlock[blockIndex];
		VirtexFrameSet& frameSet2 = frameBlocks2.mBlock[blockIndex];

		BOOST_CHECK_EQUAL(frameSet1.size(), frameSet2.size());

		int usedFrameCount = 0;
		size_t frameIndex = 0;
		// iterate over the frames
		for(; frameIndex < frameSet1.size(); frameIndex++) {
			// look up the frame for this set
			VirtexFrameSharedPtr framePtr1 = frameSet1[frameIndex];
			VirtexFrameSharedPtr framePtr2 = frameSet2[frameIndex];

			const VirtexFrame::word_t* words1 = framePtr1->getWords();
			const VirtexFrame::word_t* words2 = framePtr2->getWords();

			const VirtexFrame::word_t* pWords1 = words1;
			const VirtexFrame::word_t* pWords2 = words2;
			const VirtexFrame::word_t* eWords1 = pWords1 + framePtr1->getLength();
			const VirtexFrame::word_t* eWords2 = pWords2 + framePtr2->getLength();

			BOOST_CHECK_EQUAL(framePtr1->getLength(), framePtr2->getLength());

			if(framePtr1->isUsed())
			usedFrameCount++;
			// iterate over the frame words
			bool found = false;
			int wordIndex = 0;

			// Iterate over the frame words
			while(pWords1 < eWords1) {

				// If the word is not zero and not same as that of null bitstream and not crc word
				if(*pWords1 != *pWords2 && wordIndex != crcWordIndex) {
					found = true;
					returnCode = false;
					break;
				}
				pWords1++;
				pWords2++;
				wordIndex++;
			}
			// if we found any non-zero words, display them
			if(found) {
				std::cout << "Found mis-match frame. Block: " << blockIndex << ", Frame Index: " << Hex32(
						frameIndex) << std::endl;

				pWords1 = words1;
				std::cout << "    Frame 1: ";
				while(pWords1 < eWords1) {
					std::cout << Hex32(*pWords1++) << " ";
				}
				std::cout << std::endl;

				pWords2 = words2;
				std::cout << "    Frame 2: ";
				while(pWords2 < eWords2) {
					std::cout << Hex32(*pWords2++) << " ";
				}
				std::cout << std::endl;
			}
		}

		std::cout << std::endl;
		//		std::cout << "Used frame count " << usedFrameCount << std::endl;
		//		std::cout << "Total frame count " << frameIndex << std::endl;
	}
	BOOST_CHECK_EQUAL(returnCode, true);
	return returnCode;
}

BOOST_AUTO_TEST_SUITE_END()
}
}

