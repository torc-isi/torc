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
/// \brief Example program to read in a structural Verilog file and write it back out.

#include "torc/Generic.hpp"
#include <fstream>

/// \brief Standard main() function.
int main(int argc, char* argv[]) {

	// import the XDL design
	if(argc < 2) return 0;
	std::string inFileName(argv[1]);
	torc::generic::VerilogImporter importer;
	//importer(inFileName); // if no other parameters are required
	importer(inFileName, "-v cp65npksdst.v"); // if a cell library or other parameters are required

	// look up the design (and do something with it ...)
	torc::generic::RootSharedPtr rootPtr = importer.getRootPtr();

	torc::generic::VerilogExporter exporter(rootPtr);
	exporter(boost::filesystem::path("output.v"));

	return 0;
}
