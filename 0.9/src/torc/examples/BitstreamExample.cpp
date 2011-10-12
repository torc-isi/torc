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
/// \brief Example program to open a device bitstream and display its packets.

#include "torc/Bitstream.hpp"
#include "torc/Common.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {

	// build the file paths
	(void) argc;
	torc::common::DirectoryTree directoryTree(argv[0]);
	boost::filesystem::path referencePath = torc::common::DirectoryTree::getExecutablePath() 
		/ "regression" / "Virtex5UnitTest.reference.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	if(!fileStream.good()) return -1;
	torc::bitstream::Virtex5 bitstream;
	bitstream.read(fileStream, false);
	// write the bitstream digest to the console
	std::cout << bitstream << std::endl;

	return 0;
}
