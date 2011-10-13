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
/// \brief Unit test for the XdlImporter class.

#include <boost/test/unit_test.hpp>
#include "torc/physical/XdlImporter.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/physical/Design.hpp"
#include <fstream>

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical)

/// \brief Unit test for the XdlImporter class.
/// \todo Finish testing XdlImporter functionality.
BOOST_AUTO_TEST_CASE(XdlImporterUnitTest) {
	// members not tested:
	//		bool mTraceScanning;
	//		bool mTraceParsing;
	//		string mStreamName;
	// functions not tested:
	//		virtual ~XdlImporter(void);
	//		class torc::XdlScanner* lexer;
	//		DEPRECATED bool import(istream& in, const string& name = "stream input") 
	//		DEPRECATED bool import(const string& input, const string& name = "string stream") 
	//		bool operator()(const string& input, const string& name = "string stream");
	//		DEPRECATED bool import(const boost::filesystem::path& filename);
	//		bool operator()(const boost::filesystem::path& filename);
	//		void error(const location& l, const string& m);
	//		void error(const string& m);
	//		void failure(void);
	// functions not tested (because they are null virtual functions):
	//		virtual void initializeDatabase(void);
	//		virtual void bind(torc::physical::InstancePinSharedPtr&);
	//		virtual void bind(torc::physical::Pip&, EPipType);

	// create the appropriate file paths
	boost::filesystem::path regressionPath = 
		torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path referencePath = regressionPath / "DesignUnitTest.reference.xdl";

	// functions tested:
	//		XdlImporter(void);
	//		bool operator()(istream& in, const string& name = "stream input");
	// import the XDL design
	std::fstream fileStream(referencePath.string().c_str());
	BOOST_REQUIRE(fileStream.good());
	XdlImporter importer;
	importer(fileStream, referencePath.string());
	BOOST_CHECK_EQUAL(referencePath.string(), importer.mStreamName);

	// functions tested:
	//		DesignSharedPtr getDesignPtr(void);
	DesignSharedPtr designPtr = importer.getDesignPtr();
	BOOST_REQUIRE(designPtr.get() != 0);
	BOOST_CHECK_EQUAL(designPtr->getName(), "blinker");
	BOOST_CHECK_EQUAL(designPtr->getDevice(), "xc5vlx30");
	BOOST_CHECK_EQUAL(designPtr->getPackage(), "ff324");
	BOOST_CHECK_EQUAL(designPtr->getSpeedGrade(), "-1");
	BOOST_CHECK_EQUAL(designPtr->getModuleCount(), 0u);
	BOOST_CHECK_EQUAL(designPtr->getInstanceCount(), 8u);
	BOOST_CHECK_EQUAL(designPtr->getNetCount(), 7u);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
