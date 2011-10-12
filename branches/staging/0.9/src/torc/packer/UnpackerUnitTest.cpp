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
/// \brief Unit test for the PrimitiveStructure class.

#include <boost/test/unit_test.hpp>
#include "torc/packer/Unpacker.hpp"
#include "torc/packer/PrimitiveStructure.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/XdlImporter.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/common/TestHelpers.hpp"
#include "torc/physical/Instance.hpp"
#include "torc/physical/Net.hpp"
#include "torc/Physical.hpp"
#include <iostream>
#include <fstream>

namespace torc {
namespace packer {

BOOST_AUTO_TEST_SUITE(packer)


/// \brief Unit test for the PrimitiveStructure class.
BOOST_AUTO_TEST_CASE(UnpackerUnitTest) {

	std::string xdls[] = {		
		"regression/PathFinderRegression.Virtex5.Test1.xdl", 
		"", 
//		"regression/cae_fpga_routed.xdl", 
//		"regression/system.xdl", 
		"system.xdl",
		"regression/cae_fpga_routed.xdl",		
		"regression/counter.xdl", 
//		"regression/DesignUnitTest.generated.xdl", 
		"regression/DesignUnitTest.reference.xdl", 
		"regression/PathFinderRegression.Virtex5.Test1.xdl", 
		"regression/TraceRegressionTest.Virtex5.xdl", 
		"regression/TraceRegressionTest.VirtexTbuf.xdl", 
		"regression/TraceVirtex5Test.xdl", 
		"regression/Virtex5UnitTest.reference.xdl", 
		"regression/a.xdl", 
		"regression/audio_test.xdl", 
		"regression/bitstreamTest.xdl", 
		"regression/blinker.xdl", 
		"regression/collatz1.xdl", 
		"regression/config_with_trailing_embedded_newline.xdl", 
		"regression/counter.xdl", 
		"regression/fullroutetest3_noio.xdl", 
		"regression/hardnets.xdl", 
		"regression/input.xdl", 
		"regression/loop154@24816_1_test0.xdl", 
		"regression/loop154@24816_1_test1.xdl", 
		"regression/minimips.xdl", 
		"regression/neil.xdl", 
		"regression/path276@24816_16.xdl", 
		"regression/pathfindertest1.xdl", 
		"regression/pathfindertest1_constrained.xdl", 
		"regression/pathfindertest1_onenet.xdl", 
		"regression/pathfindertest2.xdl", 
		"regression/powerandglobal.xdl", 
		"regression/prng_v0.xdl", 
		"regression/rfft.par.xdl", 
		"regression/routingtest1.xdl", 
		"regression/test.xdl", 
		"regression/test_noroute.xdl", 
		"regression/testsys_nopips.xdl", 
//		"regression/testsystem.xdl", 
		"regression/top_map.xdl", 
		"regression/twonet.xdl", 
//		"regression/xc5vlx30ff676-1.xdl", 
//		"regression/xc5vlx330tff1738-1.xdl", 
		"regression/xc5vlx50ff676_base_io_single_test.xdl", 
		""
	};

	// create the appropriate file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath 
		= regressionPath / "DesignUnitTest.generated.xdl";
	boost::filesystem::path referencePath 
		= regressionPath / "PathFinderRegression.Virtex5.Test1.xdl";

	for(int i = 0; xdls[i].size(); i++) {
		referencePath = xdls[i];
		std::cerr << "Unpacking " << referencePath << std::endl;

		// import the XDL design
		std::fstream fileStream(referencePath.string().c_str());
		BOOST_REQUIRE(fileStream.good());
		torc::architecture::XdlImporter importer;
		importer(fileStream, referencePath.string());

		// look up the design
		torc::physical::DesignSharedPtr mDesignPtr = importer.getDesignPtr();
		BOOST_REQUIRE(mDesignPtr.get() != 0);
		
		// invoking the unpacker 
		torc::packer::Unpacker unpacker(mDesignPtr);
		unpacker.unpack();       

		// export the XDL design
		std::string outFileName 
			= boost::filesystem::path(referencePath).replace_extension().string() + ".xdl.unpacked";
		std::fstream xdlExport(outFileName.c_str(), std::ios_base::out);
		torc::physical::XdlExporter fileExporter(xdlExport);
		fileExporter(mDesignPtr);
	}

}


BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
