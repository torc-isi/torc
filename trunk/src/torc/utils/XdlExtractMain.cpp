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
/// \brief Source for a Xdl extracter command line utility.

#include <fstream>
#include "MarkExtracter.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/physical/XdlImporter.hpp"
#include "torc/physical/XdlExporter.hpp"

/// \brief Main entry point for the xdlextract tool.
int main(int argc, char *argv[]) {
	typedef std::string string;
	torc::common::DirectoryTree directoryTree(argv[0]);
	
	std::cout << "XDL Extracter: " << __DATE__ << " " << __TIME__ << std::endl;	
	
	if(argc != 4) {
		std::cout << "Usage: " << argv[0] << " <pattern> <xdlfile> <outxdlfile>" << std::endl;
		return 1;
	}
	
	string argpattern = argv[1];
	string argxdl = argv[2];
	string argoutxdl = argv[3];
	
	// create the appropriate file paths
	boost::filesystem::path xdlPath = directoryTree.getExecutablePath() / argxdl;
	boost::filesystem::path xdlOutPath = directoryTree.getExecutablePath() / argoutxdl;

	std::fstream xdlFile(xdlPath.string().c_str());
	
	// import the designs
	torc::physical::XdlImporter importer;
	
	importer(xdlFile, xdlPath.string());
	torc::physical::DesignSharedPtr designPtr = importer.getDesignPtr();
	
	// create the physical diff object and perform the diff.
	torc::MarkExtracter extract(std::cout, designPtr, argpattern);
	torc::physical::DesignSharedPtr newDesignPtr = extract.extract();
	
	std::fstream xdlOutFile(xdlOutPath.string().c_str(), std::ios_base::out);
	torc::physical::XdlExporter exporter(xdlOutFile);
	//exporter(designOut);
	exporter(newDesignPtr);
	
	std::cout << "Output: " << xdlOutPath.string() << std::endl;
	
	return 0;
}

