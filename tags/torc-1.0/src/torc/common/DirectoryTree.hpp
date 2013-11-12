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
/// \brief Header for the DirectoryTree class.

#ifndef TORC_COMMON_DIRECTORYTREE_HPP
#define TORC_COMMON_DIRECTORYTREE_HPP

#include <boost/filesystem/convenience.hpp>

namespace torc {
namespace common {

	/// \brief Encapsulation of filesystem paths that are used by the library.
	class DirectoryTree {
	protected:
	// static variables
		/// \brief Relative path from the working directory to the executable home.
		static boost::filesystem::path sRelativePath;
		/// \brief Absolute path to the working directory.
		static boost::filesystem::path sWorkingPath;
		/// \brief Absolute path to the executable directory.
		static boost::filesystem::path sExecutablePath;
		/// \brief Absolute path to the EDA libraries directory.
		static boost::filesystem::path sEdaPath;
		/// \brief Absolute path to the family and device database directory.
		static boost::filesystem::path sDevicesPath;
		/// \brief Absolute path to the log files directory.
		static boost::filesystem::path sLogPath;
	public:
	// constructors
		/// \brief Mandatory constructor.
		/// \param argv0 The zero-th argument passed to main in argv[].  DirectoryTree needs this 
		///		to derive the relative path from the working directory to the executable, until 
		///		such a time as Boost provides a better approach.
		/// \details Extracts the zero-th argument, and builds the various static paths used by the 
		///		executable.
		DirectoryTree(const char* argv0);
	// static functions
		/// \brief Returns the relative path from the working directory to the executable home.
		static const boost::filesystem::path& getRelativePath(void) { return sRelativePath; }
		/// \brief Returns the absolute path to the working directory.
		static const boost::filesystem::path& getWorkingPath(void) { return sWorkingPath; }
		/// \brief Returns the absolute path to the executable directory.
		static const boost::filesystem::path& getExecutablePath(void) { return sExecutablePath; }
		/// \brief Returns the absolute path to the family and device database directory.
		static const boost::filesystem::path& getDevicesPath(void) { return sDevicesPath; }
		/// \brief Returns the absolute path to the log files directory.
		static const boost::filesystem::path& getLogPath(void) { return sLogPath; }
	};

} // namespace common
} // namespace torc

#endif // TORC_COMMON_DIRECTORYTREE_HPP
