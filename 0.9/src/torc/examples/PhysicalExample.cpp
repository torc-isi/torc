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
/// \brief Example program to read in an XDL file, do something with it, and write it back out.

#include "torc/Physical.hpp"
#include <fstream>
#include <boost/regex.hpp>

/// \brief Standard main() function.
int main(int argc, char* argv[]) {

	// import the XDL design
	if(argc < 2) return 0;
	std::string inFileName(argv[1]);
	std::fstream fileStream(inFileName.c_str());
	if(!fileStream.good()) return -1;
	torc::physical::XdlImporter importer;
	importer(fileStream, inFileName);

	// look up the design (and do something with it ...)
	torc::physical::DesignSharedPtr designPtr = importer.getDesignPtr();

	// export the XDL design
	boost::filesystem::path outputPath = inFileName;
	outputPath.replace_extension(".mod.xdl");
	std::string outFileName = outputPath.string();
	std::fstream xdlExport(outFileName.c_str(), std::ios_base::out);
	torc::physical::XdlExporter fileExporter(xdlExport);
	fileExporter(designPtr);

	return 0;
}
