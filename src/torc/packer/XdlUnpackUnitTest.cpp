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
/// \brief Unit test for the XdlUnpack class.

#include <boost/test/unit_test.hpp>
#include <boost/filesystem/convenience.hpp>
#include "torc/packer/XdlUnpack.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/physical/XdlImporter.hpp"
#include "torc/physical/XdlExporter.hpp"
#include "torc/physical/OutputStreamHelpers.hpp"
#include "torc/common/TestHelpers.hpp"
#include <fstream>

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(packer)

/// \brief Unit test for the XdlUnpack class.
BOOST_AUTO_TEST_CASE(XdlUnpackUnitTest) {
	BOOST_TEST_MESSAGE("DEPRECATED: XdlUnpack is deprecated in favor of Unpacker.");
	return;

	// create the appropriate file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "XdlUnpackUnitTest.xdl";
	boost::filesystem::path referencePath = torc::common::DirectoryTree::getExecutablePath() 
		/ "torc" / "physical" / "DesignUnitTest.reference.xdl";

	// import the XDL design
	std::fstream fileStream(referencePath.string().c_str());
	BOOST_REQUIRE(fileStream.good());
	torc::physical::XdlImporter importer;
	importer(fileStream, referencePath.string());

	// unpack the design
	torc::physical::DesignSharedPtr designPtr = importer.getDesignPtr();
	torc::physical::XdlUnpack unpacker;
	torc::physical::DesignSharedPtr unpackedDesignPtr = unpacker(designPtr);

	// export the created design
	std::fstream xdlExport(generatedPath.string().c_str(), std::ios_base::out);
	BOOST_REQUIRE(xdlExport.good());
	XdlExporter fileExporter(xdlExport);
	fileExporter(unpackedDesignPtr);

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc

