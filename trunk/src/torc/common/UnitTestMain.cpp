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
/// \brief Main function and test fixture declaration for Boost.Test.

/// \brief Request dynamic linking of Boost.Test.
#define BOOST_TEST_DYN_LINK
/// \brief Define the top-level test module.
#define BOOST_TEST_MODULE torc
/// \brief Prototype for Boost.Test bool init_unit_test(void).
bool init_unit_test(void);
#include <boost/test/unit_test.hpp>
#include "torc/common/DirectoryTree.hpp"

/// \brief Convenience test fixture struct to request desired logging level from Boost.Test.
struct TestFixture {
	TestFixture(void) {
		boost::unit_test::log_level log_level = 
			boost::unit_test::log_successful_tests
		//	boost::unit_test::log_test_units
		//	boost::unit_test::log_messages
		//	boost::unit_test::log_warnings
		//	boost::unit_test::log_all_errors
		//	boost::unit_test::log_cpp_exception_errors
		//	boost::unit_test::log_system_errors
		//	boost::unit_test::log_fatal_errors
		//	boost::unit_test::log_nothing
			;
		boost::unit_test::unit_test_log.set_threshold_level(log_level);
		
		// initialize the directory tree
		char**& argv = boost::unit_test::framework::master_test_suite().argv;
		torc::common::DirectoryTree directoryTree(argv[0]);
	}
};
/// \brief Global test fixture to request desired logging level from Boost.Test.
BOOST_GLOBAL_FIXTURE(TestFixture)
