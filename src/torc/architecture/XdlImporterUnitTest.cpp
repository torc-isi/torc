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
/// \brief Source for the XdlImporter unit test.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/XdlImporter.hpp"
#include "torc/physical/Design.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <fstream>


namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the device-aware XdlImporter class.

BOOST_AUTO_TEST_CASE(XdlImporterUnitTest) {
	// create the appropriate file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "DesignUnitTest.generated.xdl";
	boost::filesystem::path referencePath = regressionPath / "DesignUnitTest.reference.xdl";

	// import the XDL design
	std::fstream fileStream(referencePath.string().c_str());
	BOOST_REQUIRE(fileStream.good());
	XdlImporter importer;
std::cerr << "importer invocation" << std::endl;
	importer(fileStream, referencePath.string());
std::cerr << "importer finished" << std::endl;

	// look up the design (and do something with it ...)
	torc::physical::DesignSharedPtr designPtr = importer.getDesignPtr();
	BOOST_REQUIRE(designPtr.get() != 0);
	BOOST_REQUIRE(false);
	BOOST_TEST_MESSAGE("Need to fix XdlImporter::bind(torc::physical::Pip& inPip, EPipType inPipType) and XDL design regression test files for directionality of pips.");

	// look up the device database (and do something with it ...)
	DDB* ddbPtr = importer.releaseDDBPtr();
	(void) ddbPtr;
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
