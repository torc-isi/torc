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
/// \brief Unit test for the XdlExporter class.
/// \see DesignRegressionTest.cpp and SampleCodeUnitTest.cpp.

/// \file
/// \brief Unit test for the XdlExporter class.

#include <boost/test/unit_test.hpp>
#include "torc/physical/XdlExporter.hpp"
#include "torc/physical/XdlImporter.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/common/TestHelpers.hpp"
#include <fstream>

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical)

/// \brief Unit test for the XdlExporter class.
BOOST_AUTO_TEST_CASE(XdlExporterUnitTest) {
	// create the appropriate file paths
	boost::filesystem::path generatedPath = torc::common::DirectoryTree::getExecutablePath() 
		/ "regression" / "XdlExporterUnitTest.generated.xdl";
	boost::filesystem::path referencePath = torc::common::DirectoryTree::getExecutablePath() 
		/ "torc" / "physical" / "ModuleTransformerUnitTest.Scenario.02.Flatten.xdl";

	// import the XDL design
	std::fstream fileStream(referencePath.string().c_str());
	BOOST_REQUIRE(fileStream.good());
	XdlImporter importer;
	importer(fileStream, referencePath.string());

	// look up the design
	DesignSharedPtr designPtr = importer.getDesignPtr();
	BOOST_REQUIRE(designPtr.get() != 0);

	// members tested:
	//		std::ostream& mStream;
	//		int mIndentCount;
	//		std::string mIndentString;
	// functions tested:
	//		XdlExporter(std::ostream& inStream, const std::string& inIndentString = "\t");
	//		void operator() (const DesignSharedPtr& inDesignPtr);
	//		void indent(void) const;
	//		void write(const Circuit& circuit);
	//		void write(const Design& design);
	//		void write(const Module& module);
	//		void write(const Port& port);
	//		void write(const Instance& instance);
	//		void write(const Net& net);
	//		void write(const InstancePin& instancePin, EPinDirection pinDirection, bool comma);
	//		void write(const Pip& pip, bool comma);
	//		void write(const Routethrough& routethrough);
	//		void write(const ConfigMap& configMap);
	// export the XDL design
	std::fstream xdlExport(generatedPath.string().c_str(), std::ios_base::out);
	XdlExporter fileExporter(xdlExport);
	fileExporter(designPtr);
	BOOST_CHECK(torc::common::fileContentsAreEqual(generatedPath, referencePath));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
