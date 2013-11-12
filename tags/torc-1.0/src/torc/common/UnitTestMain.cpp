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

/// \brief Test suite visitor to disable regression tests.
class RegressionFilter : public boost::unit_test::test_tree_visitor {
// typedefs
	typedef boost::unit_test::test_suite test_suite; ///< Imported type.
	typedef std::string string; ///< Imported type.
// members
	/// \brief Vector of test suite pointers.
	std::vector<const test_suite*> mTestSuiteVector;
	/// \brief Vector of test suite usage flags.
	std::vector<bool> mTestSuiteUsed;
	/// \brief Vector of test suites to remove.
	std::vector<boost::unit_test::test_unit_id> mTestSuitePruningIDs;
public:
	/// \brief Removes all test suites marked for pruning.
	virtual ~RegressionFilter(void) {
		for(size_t i = 0; i < mTestSuitePruningIDs.size(); i++) {
			boost::unit_test::framework::master_test_suite().remove(mTestSuitePruningIDs[i]);
		}
	}
	/// \brief Returns the current fully qualified test suite path.
	virtual string getFullyQualifiedPath(void) {
		// assemble all of the test suite names
		string name = "";
		for(size_t i = 0; i < mTestSuiteVector.size(); i++) {
			name += mTestSuiteVector[i]->p_name.get() + "/";
		}
		return name;
	}
	/// \brief Returns a fully qualified name for the given test case.
	virtual string getFullyQualifiedName(const boost::unit_test::test_case& inTestCase) {
		// assemble the fully qualified name
		return getFullyQualifiedPath() + inTestCase.p_name.get();
	}
	/// \brief Flags the given test case and its parents as being used.
	virtual void useTest(const boost::unit_test::test_case& inTestCase) {
		// mark this suite and all parents as used
		for(size_t i = 0; i < mTestSuiteVector.size(); i++) mTestSuiteUsed[i] = true;
	}
	/// \brief Determines whether or not to include the given test case.
	virtual void visit(const boost::unit_test::test_case& inTestCase) {
		string name = getFullyQualifiedName(inTestCase);
		// determine whether to include the test
		if(name.find("Regression") == string::npos && name.find("regression") == string::npos) {
std::cout << "keeping case  " << name << std::endl;
			useTest(inTestCase);
		} else {
			inTestCase.p_enabled.set(false);
			boost::unit_test::framework::master_test_suite().remove(inTestCase.p_id);
std::cout << "pruning case  " << name << std::endl;
		}
	}
	/// \brief Enters a new test suite.
	virtual bool test_suite_start(const boost::unit_test::test_suite& inTestSuite) {
		// push the new suite information
		mTestSuiteVector.push_back(&inTestSuite);
		mTestSuiteUsed.push_back(false);
//std::cout << "entering test suite " << getFullyQualifiedPath() << std::endl;
		return true;
	}
	/// \brief Exits a test suite and prunes it if no test cases remain in it.
	virtual void test_suite_finish(const boost::unit_test::test_suite& inTestSuite) {
//std::cout << "leaving test suite " << getFullyQualifiedPath() << std::endl;
		// if nobody needs this test suite, remove it
		if(!mTestSuiteUsed.back()) {
			inTestSuite.p_enabled.set(false);
			mTestSuitePruningIDs.push_back(inTestSuite.p_id);
			std::cout << "pruning suite " << getFullyQualifiedPath() << std::endl;
		}
		// pop the suite information
		mTestSuiteVector.pop_back();
		mTestSuiteUsed.pop_back();
	}
};

/// \brief Test suite visitor to disable tests for debugging.
class DebugFilter : public boost::unit_test::test_tree_visitor {
public:
	virtual void visit(const boost::unit_test::test_case& inTestCase) {
		std::string enabled = 
			"architecture/iterate_configmaps"
			"bitstream/VirtexEMapUnitTest"
		;
		inTestCase.p_enabled.set((enabled.find(inTestCase.p_name.get()) != std::string::npos));
	//	std::string disabled = 
	//		"architecture/iterate_configmaps"
	//	;
	//	inTestCase.p_enabled.set((disabled.find(inTestCase.p_name.get()) == std::string::npos));
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

		// allow for special debug filtering
		//DebugFilter debugTestFilter;
		//boost::unit_test::traverse_test_tree(boost::unit_test::framework::master_test_suite(), 
		//	debugTestFilter);

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
