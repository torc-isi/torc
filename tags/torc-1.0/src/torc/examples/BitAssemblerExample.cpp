// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
// $HeadURL: https://svn.code.sf.net/p/torc-isi/code/branches/staging/1.0/src/torc/examples/ArchitectureExample.cpp $
// $Id: ArchitectureExample.cpp 12 2011-10-13 15:44:09Z nsteiner $

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

/// \file BitAssemblerExample.cpp
/// \brief Example to show usage of bitassembler classes.
///	\author Ritesh Kumar Soni

#include "torc/bitstream/assembler/Assembler.hpp"
#include "torc/bitstream/assembler/AssemblerFactory.hpp"
#include "torc/Physical.hpp"
#include "torc/common/DirectoryTree.hpp"

int main(int argc, char **argv) {

	if(argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <XDL file path>" << std::endl;
		return -1;
	}

	// Open the XDL file and check if it opens successfully
	std::string xdlFilePath(argv[1]);
	std::fstream xdlFileStream(xdlFilePath.c_str());
	if(!xdlFileStream.good()) {
		std::cout << "Could not open file " << xdlFilePath << std::endl;
		return -1;
	}

	// Initialize the DirectoryTree class
	torc::common::DirectoryTree directoryTree(argv[0]);

	// Import the XDL file
	torc::physical::XdlImporter importer;
	importer(xdlFileStream, "XdlDesignExample");
	// Get the design pointer
	torc::physical::DesignSharedPtr designPtr = importer.getDesignPtr();

	torc::architecture::DDB ddb(designPtr->getDevice());

	boost::filesystem::path customBitFile = xdlFilePath;
	customBitFile.replace_extension(".custombit");
	
	boost::filesystem::path baseBitstreamPath = xdlFilePath;
	baseBitstreamPath.replace_extension(".bit");
	// Create bitstream assembler object
	torc::bitstream::AssemblerSharedPtr assemblerPtr 
		= torc::bitstream::AssemblerFactory::newAssemblerPtr(designPtr, ddb); 
	assemblerPtr->generateBitstream(designPtr, customBitFile);
}
