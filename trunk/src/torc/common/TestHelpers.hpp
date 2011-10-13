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
/// \brief Header for Boost.Test helper functions.

#ifndef TORC_COMMON_TESTHELPERS_HPP
#define TORC_COMMON_TESTHELPERS_HPP

#include <string>
#include <boost/filesystem/convenience.hpp>

namespace torc {
namespace common {

/// \brief Read the raw contents of the specified file into a std::string.
/// \param inPath The boost::filesystem::path for the file of interest.
/// \param outString The string in which to place the raw file contents.
/// \returns true if the file contents were read successfully.
bool readFileIntoString(const boost::filesystem::path& inPath, std::string& outString);

/// \brief Compare the raw contents of two files to determine whether they are identical.
/// \param inA The boost::filesystem::path for one of the two files.
/// \param inB The boost::filesystem::path for the other of the two files.
/// \returns true if the contents of the two files are identical.
bool fileContentsAreEqual(const boost::filesystem::path& inA, const boost::filesystem::path& inB);

} // namespace common
} // namespace torc

#endif // TORC_COMMON_TESTHELPERS_HPP
