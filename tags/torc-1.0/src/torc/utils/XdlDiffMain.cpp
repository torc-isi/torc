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
/// \brief Source for a PhysicalDiff command line utility.
/// \details Opens up two designs with the XDLImporter and then compares them with PhysicalDiff.

#include <fstream>
#include "PhysicalDiff.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/physical/XdlImporter.hpp"

/// \brief Main entry point for the xdldiff tool.
int main(int argc, char *argv[]) {
	typedef std::string string;
	torc::common::DirectoryTree directoryTree(argv[0]);
		
	if(argc != 3) {
		std::cout << "Usage: " << argv[0] << " <xdlfile1> <xdlfile2>" << std::endl;
		return 1;
	}
	string argleft = argv[1];
	string argright = argv[2];
	
	// create the appropriate file paths
	boost::filesystem::path leftPath = directoryTree.getExecutablePath() / argleft;
	boost::filesystem::path rightPath = directoryTree.getExecutablePath() / argright;

	std::fstream fileLeft(leftPath.string().c_str());
	std::fstream fileRight(rightPath.string().c_str());
	
	// import the designs
	torc::physical::XdlImporter importer;
	
	importer(fileLeft, leftPath.string());
	torc::physical::DesignSharedPtr designPtrLeft = importer.getDesignPtr();
	
	importer(fileRight, rightPath.string());
	torc::physical::DesignSharedPtr designPtrRight = importer.getDesignPtr();

	// create the physical diff object and perform the diff.
	torc::PhysicalDiff diff(std::cout);
	bool result = diff.diff(designPtrLeft, designPtrRight);

	if(result) std::cout << "No differences found." << std::endl;

	return 0;
}

