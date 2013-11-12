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
/// \brief Unit test for the SampleCode class.

#include <boost/test/unit_test.hpp>
#include "torc/physical/XdlImporter.hpp"
#include "torc/physical/XdlExporter.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/physical/Factory.hpp"
#include "torc/physical/Design.hpp"
#include <fstream>

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical)

/// \brief Unit test demonstrating some sample code.
BOOST_AUTO_TEST_CASE(SampleCodeUnitTest) {

	// create the appropriate file paths
	boost::filesystem::path referencePath = torc::common::DirectoryTree::getExecutablePath() 
		/ "torc" / "physical" / "DesignUnitTest.reference.xdl";
	boost::filesystem::path generatedPath = torc::common::DirectoryTree::getExecutablePath() 
		/ "regression" / "DesignUnitTest.reference.xdl";

	// import the XDL design
	std::fstream fileStream(referencePath.string().c_str());
	BOOST_REQUIRE(fileStream.good());
	XdlImporter importer;
	importer(fileStream, referencePath.string());

	// look up the design (and do something with it ...)
	DesignSharedPtr designPtr = importer.getDesignPtr();
	BOOST_REQUIRE(designPtr.get() != 0);

	// export the XDL design
	std::fstream xdlExport(generatedPath.string().c_str(), std::ios_base::out);
	XdlExporter fileExporter(xdlExport);
	fileExporter(designPtr);

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
