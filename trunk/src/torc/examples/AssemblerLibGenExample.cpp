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

/// \file
/// \brief Implementation of class to encapsulate micro-bitstream library generation code
/// \author Ritesh Kumar Soni

#include "torc/bitstream/assembler/build/LibGenFactory.hpp"
#include "torc/bitstream/assembler/build/AssemblerLibGen.hpp"
#include <iostream>

// Main function to create a executable
int main(int argc, char **argv) {

	if(argc != 4) {
		std::cerr << "Invalid arguments. Usage: " << argv[0]
			<< " <device> <harness_folder> <xdl_generation_folder>" << std::endl;
		exit(-1);
	}
	// get the part number
	std::string partNumber = argv[1];
	boost::filesystem::path harnessFolder = argv[2];
	boost::filesystem::path xdlGenerationFolder = argv[3];
	// Initialize the DirectoryTree class
	torc::common::DirectoryTree directoryTree(argv[0]);
	
	torc::bitstream::AssemblerLibGenSharedPtr libGen 
		= torc::bitstream::LibGenFactory::newLibGenPtr(partNumber, xdlGenerationFolder);
	libGen->generateLogicXdls();
	libGen->generateWireXdls();
	libGen->generateBitstreams();
	libGen->compressBitFiles();
	libGen->stitchMicroBitstreams();
}


