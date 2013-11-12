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

#include "torc/packer/ParsePrimitive.hpp"
#include "torc/packer/WritePrimitive.hpp"

using namespace torc::physical;
	
int main(int argc, char *argv[]) {
	typedef std::string string;
	
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <input xdlrc file>" << " <output xdlrc file>" <<std::endl;
		return 1;
	}

	ParsePrimitive parser;
	std::cout << "\n\nReading " <<argv[1] << std::endl;
	PrimitiveSetSharedPtr ps = parser.ParsePrimitiveFile(argv[1]);
	std::cout << "DONE!" << std::endl;
		
	WritePrimitive writer;
	std::cout << "\n\nwriting into " <<argv[2] << std::endl;
	writer.WritePrimitiveFile(argv[2],ps);
	std::cout << "DONE!" << std::endl;
		

	return 0;
}
