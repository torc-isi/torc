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
/// \brief Unit Test for PathFinder class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/XdlImporter.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/common/DirectoryTree.hpp"

#include "PathFinder.hpp"
#include "NetRouter.hpp"
//#include "NetRouterHeuristic.hpp"
#include "PathFinderHeuristic.hpp"
#include "RouteUtilities.hpp"
#include "PathFinderNetRouterHeuristic.hpp"

namespace torc {
namespace router {

BOOST_AUTO_TEST_SUITE(router)

/// \brief Unit test for the NetRouter.
BOOST_AUTO_TEST_CASE(PathFinder) {

	int& argc = boost::unit_test::framework::master_test_suite().argc;;
	BOOST_REQUIRE(argc >= 1);
/*	char**&argv = boost::unit_test::framework::master_test_suite().argv;;
	torc::common::DirectoryTree directoryTree(argv[0]);
	boost::filesystem::path testPath(directoryTree.getExecutablePath() / "torc" / "router");
	boost::filesystem::path referencePath(testPath / "pathfindertest1.xdl");
std::cout << referencePath.string() << std::endl;

	std::fstream fileStream(referencePath.string().c_str());
	BOOST_REQUIRE(fileStream.good());
	
	architecture::XdlImporter importer;
std::cout << "HERE" << std::endl;
	importer(fileStream, referencePath.string());
std::cout << "AND HERE" << std::endl;
	
	torc::physical::DesignSharedPtr designPtr = importer.getDesignPtr();
	BOOST_REQUIRE(designPtr.get() != 0);
	BOOST_CHECK_EQUAL(designPtr->getNetCount(), 2376u);
	architecture::DDB* ddbPtr = importer.releaseDDBPtr();



//	architecture::DDB ddb("xc5vlx110t");
//	NetRouterHeuristicBase* h = new NetRouterHeuristic(*ddbPtr);
	NetRouterHeuristicBase* h = new PathFinderNetRouterHeuristic(*ddbPtr);
	NetRouterBase* r = new NetRouter(*ddbPtr, h);
	NetVectorRouterHeuristicBase* vh = new PathFinderHeuristic(*ddbPtr);
	NetVectorRouterBase* vr = new PathFinder(*ddbPtr, vh, r);
	
	RouteUtilities ru;
	RouteNetVector nets;
	std::cout << "RouteNets: " << nets.size() << std::endl;
	
	ru.design2routenets(designPtr->netsBegin(), designPtr->netsEnd(), nets);
	
	std::cout << "RouteNets: " << nets.size() << std::endl;
	std::cout << *ddbPtr;
	std::cout << "Clearing usage from import" << std::endl;
	ddbPtr->clearUsage();
	
	vr->route(nets);
	
	std::cout << "Finished" << std::endl;*/
	
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace router
} // namespace torc
