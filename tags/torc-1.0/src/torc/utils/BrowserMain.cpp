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
#include "ArchitectureBrowser.hpp"
#include "torc/common/DirectoryTree.hpp"

/// \brief Main entry point for the architecture browser tool.
int main(int argc, char *argv[]) {
	typedef std::string string;
	torc::common::DirectoryTree directoryTree(argv[0]);
		
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " <device>" << std::endl;
		return 1;
	}
	string device_arg = argv[1];
	
	torc::common::DeviceDesignator device(device_arg);
	torc::architecture::DDB db(device);
	torc::ArchitectureBrowser ab(db);
	ab.browse();
	
	return 0;
}

