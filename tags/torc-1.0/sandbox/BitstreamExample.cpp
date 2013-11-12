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
/// \brief Example program to open a device bitstream and display its packets.

#include "torc/Bitstream.hpp"
#include "torc/Common.hpp"
#include <fstream>
#include <iostream>

using namespace torc::common;
using namespace torc::bitstream;

int main(int argc, char* argv[]) {

	// we need an input bitstream
	if(argc != 2) {
		std::cout << "Usage: " << argv[0] << " bitstream.bit" << std::endl;
		exit(-1);
	}

	// read the bitstream
	boost::filesystem::path bitstreamPath = argv[1];
	BitstreamSharedPtr bitstreamPtr = Factory::newBitstreamPtr(bitstreamPath);
	// write the bitstream digest to the console
	std::cout << *bitstreamPtr << std::endl;

	return 0;
}
