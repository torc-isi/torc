// TORC - Copyright 2010 University of Southern California.  All Rights Reserved.
// $HeadURL$
// $Id$

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "torc/physical/Design.hpp"
#include "torc/architecture/XilinxDatabaseTypes.hpp"

int xdlmain(int argc, char *argv[]);

namespace torc {

BOOST_AUTO_TEST_SUITE(portability)

/// \brief Unit test for the directory tree.
BOOST_AUTO_TEST_CASE(xdl_parsing) {
	// look up the command line arguments
	int& argc = boost::unit_test::framework::master_test_suite().argc;
	char**& argv = boost::unit_test::framework::master_test_suite().argv;
	// make sure that we at least have the name under which we were invoked
	BOOST_REQUIRE(argc >= 1);
	// resolve symbolic links if applicable
	xdlmain(argc, argv);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace torc
