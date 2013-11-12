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
/// \brief Source for the DirectoryTree class.

#include "torc/common/DirectoryTree.hpp"
#include <boost/filesystem/convenience.hpp>
#include <cstdio>

namespace torc {
namespace common {

	boost::filesystem::path DirectoryTree::sRelativePath;
	boost::filesystem::path DirectoryTree::sWorkingPath;
	boost::filesystem::path DirectoryTree::sExecutablePath;
	boost::filesystem::path DirectoryTree::sDevicesPath;
	boost::filesystem::path DirectoryTree::sLogPath;

	/// \brief Name of the devices directory.
	/// \todo This should be obtained from a configuration file.
	const std::string cDevicesNameConst("devices");

	/// \brief Name of the torc directory.
	/// \todo This should be obtained from a configuration file.
	const std::string cTorcNameConst("torc");

	DirectoryTree::DirectoryTree(const char* argv0) {
		// resolve symbolic links for the executable if necessary
		boost::filesystem::path argvPath(argv0);
		char buffer[FILENAME_MAX];
		ssize_t count = readlink(argv0, buffer, sizeof(buffer));
		if(count != -1) {
			buffer[count] = 0;
			argvPath = buffer;
			//std::cout << "Resolving symbolic link " << argv[0] << " to executable name " << path 
			//	<< std::endl;
		}
		//std::cout << ">>>> argvPath: " << argvPath << std::endl;
		// get the working directory and the relative executable path
		sWorkingPath = boost::filesystem::initial_path();
		//std::cout << ">>>> sWorkingPath: " << sWorkingPath << std::endl;
		sRelativePath = argvPath.parent_path(); // so much for relative paths ...
		//std::cout << ">>>> sRelativePath: " << sRelativePath << std::endl;
		// get the executable path
		sExecutablePath = sRelativePath;
		// build the eda and log paths
		sLogPath = sExecutablePath;
		// build the database path
		sDevicesPath = sExecutablePath / cDevicesNameConst;
		if(!exists(sDevicesPath)) 
			sDevicesPath = sExecutablePath / cTorcNameConst / cDevicesNameConst;

		// normalize each of the paths
		sRelativePath.normalize();
		sWorkingPath.normalize();
		sExecutablePath.normalize();
		sDevicesPath.normalize();
		sLogPath.normalize();
	}

} // namespace common
} // namespace torc
