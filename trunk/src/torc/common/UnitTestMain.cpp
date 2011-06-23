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
#include <iostream>

/// \brief Test suite visitor to disable regression tests
class RegressionFilter : public boost::unit_test::test_tree_visitor {
public:
	virtual void visit(const boost::unit_test::test_case& inTestCase) {
		if(inTestCase.p_name.get().find("Regression") != std::string::npos) {
			inTestCase.p_enabled.set(false);
			//std::cout << "    removing test " << inTestCase.p_name.get() << std::endl;
			boost::unit_test::framework::master_test_suite().remove(inTestCase.p_id);
		}
	}
	virtual bool test_suite_start(const boost::unit_test::test_suite& inTestSuite) {
		if(inTestSuite.p_name.get().find("regression") != std::string::npos) {
			inTestSuite.p_enabled.set(false);
			//std::cout << "    removing test " << inTestSuite.p_name.get() << std::endl;
			boost::unit_test::framework::master_test_suite().remove(inTestSuite.p_id);
		}
		return true;
	}
};

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
		int argc = boost::unit_test::framework::master_test_suite().argc;
		torc::common::DirectoryTree directoryTree(argv[0]);

		// determine whether the user requested regression tests
		std::string regression("-regression");
		bool regressionRequested = false;
		for(int i = 1; i < argc; i++) {
			if(argv[i] == regression) {
				regressionRequested = true;
				break;
			}
		}

		// disable all regression tests unless the user requested them
		if(regressionRequested == false) {
			std::cout << "NOTE: Disabling all regression tests.  Use -regression to enable them." 
				<< std::endl;
			RegressionFilter regressionTestFilter;
			boost::unit_test::traverse_test_tree(boost::unit_test::framework::master_test_suite(), 
				regressionTestFilter);
		}
	}
};
/// \brief Global test fixture to request desired logging level from Boost.Test.
BOOST_GLOBAL_FIXTURE(TestFixture)
