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
/// \brief Source for Boost.Test helper functions.

#include "torc/common/TestHelpers.hpp"
#include <fstream>

namespace torc {
namespace common {

bool readFileIntoString(const boost::filesystem::path& inPath, std::string& outString) {
	// open the file and point to the end of it
	std::ifstream input(inPath.string().c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if(input.is_open()) {
		// resize the string according to the file length
		std::ifstream::pos_type size = input.tellg();
		outString.resize(size);
		// rewind to the beginning and read the entire file directly into the string
		input.seekg(0, std::ios::beg);
		input.read(const_cast<char*>(outString.data()), size);
		// close and return success
		input.close();
		return true;
	} else {
		return false;
	}
}

bool fileContentsAreEqual(const boost::filesystem::path& inA, const boost::filesystem::path& inB) {
	std::string a, b;
	if(!readFileIntoString(inA, a)) return false;
	if(!readFileIntoString(inB, b)) return false;
	return a == b;
}

} // namespace common
} // namespace torc
