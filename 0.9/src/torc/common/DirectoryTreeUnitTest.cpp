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
/// \brief Unit test for DirectoryTree class.

#include <boost/test/unit_test.hpp>
#include "torc/common/DirectoryTree.hpp"

namespace torc {
namespace common {

BOOST_AUTO_TEST_SUITE(common)

/// \brief Unit test for the directory tree.
BOOST_AUTO_TEST_CASE(DirectoryTreeUnitTest) {
	// look up the command line arguments
	int& argc = boost::unit_test::framework::master_test_suite().argc;
	char**& argv = boost::unit_test::framework::master_test_suite().argv;
	// make sure that we at least have the name under which we were invoked
	BOOST_REQUIRE(argc >= 1);

	// functions tested: 
	//		DirectoryTree(const char* argv0);
	// resolve symbolic links if applicable
	DirectoryTree directoryTree(argv[0]);

	// functions tested: 
	//		static const boost::filesystem::path& getRelativePath(void);
	//		static const boost::filesystem::path& getWorkingPath(void);
	//		static const boost::filesystem::path& getExecutablePath(void);
	//		static const boost::filesystem::path& getDevicesPath(void);
	//		static const boost::filesystem::path& getLogPath(void);
	BOOST_CHECK(boost::filesystem::exists(directoryTree.getRelativePath()));
	BOOST_CHECK(boost::filesystem::exists(directoryTree.getWorkingPath()));
	BOOST_CHECK(boost::filesystem::exists(directoryTree.getExecutablePath()));
	BOOST_CHECK(boost::filesystem::exists(directoryTree.getDevicesPath()));
	BOOST_CHECK(boost::filesystem::exists(directoryTree.getLogPath()));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace common
} // namespace torc
