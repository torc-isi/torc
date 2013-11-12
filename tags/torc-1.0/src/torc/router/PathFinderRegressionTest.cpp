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
/// \brief Regression Tests for PathFinder class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/XdlImporter.hpp"
#include "torc/physical/XdlExporter.hpp"
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
BOOST_AUTO_TEST_SUITE(regression)

void testRouteDesign(std::string path, std::string exportpath);
void testRouteDesign(std::string path, std::string exportpath) {

	typedef torc::physical::Circuit::NetSharedPtrIterator NetSharedPtrIterator; 

	std::fstream fileStream(path.c_str());
	BOOST_REQUIRE(fileStream.good());
	
	architecture::XdlImporter importer;
	importer(fileStream, path);
	
	torc::physical::DesignSharedPtr designPtr = importer.getDesignPtr();
	BOOST_REQUIRE(designPtr.get() != 0);
	//BOOST_CHECK_EQUAL(designPtr->getNetCount(), netCount);
	architecture::DDB* ddbPtr = importer.releaseDDBPtr();

	NetRouterHeuristicBase* h = new PathFinderNetRouterHeuristic(*ddbPtr);
	NetRouterBase* r = new NetRouter(*ddbPtr, h);
	NetVectorRouterHeuristicBase* vh = new PathFinderHeuristic(*ddbPtr);
	NetVectorRouterBase* vr = new PathFinder(*ddbPtr, vh, r);
	
	RouteUtilities ru;
	RouteNetVector nets;
	
	std::cout << "RouteNets: " << nets.size() << std::endl;
	ru.design2routenets(designPtr->netsBegin(), designPtr->netsEnd(), nets);
	BOOST_CHECK_EQUAL(designPtr->getNetCount(), nets.size());
	
	std::cout << "Unrouting nets..." << std::endl;
	NetSharedPtrIterator p;
	for (p = designPtr->netsBegin(); p != designPtr->netsEnd(); p++) {
		(*p)->unroute();
	}
	
	std::cout << "RouteNets: " << nets.size() << std::endl;
	std::cout << *ddbPtr;
	std::cout << "Clearing usage from import" << std::endl;
	ddbPtr->clearUsage();
	vr->route(nets);
	std::cout << "Finished routing: " << path << std::endl;
	
	std::cout << "Exporting route nets to physical" << std::endl;
	ru.routenets2design(nets, designPtr->netsBegin(), designPtr->netsEnd(), *ddbPtr);
	
	std::cout << "Exporting XDL" << std::endl;
	std::fstream xdlExport(exportpath.c_str(), std::ios_base::out);
	physical::XdlExporter fileExporter(xdlExport);
	fileExporter(designPtr);
}

/// \brief Spartan3E regression.
/*BOOST_AUTO_TEST_CASE(pathfinder_spartan3e) {

	int& argc = boost::unit_test::framework::master_test_suite().argc;;
	BOOST_REQUIRE(argc >= 1);
	char**&argv = boost::unit_test::framework::master_test_suite().argv;;
	torc::common::DirectoryTree directoryTree(argv[0]);
	boost::filesystem::path testPath(directoryTree.getExecutablePath() 
		/ "torc" / "router" / "PathFinderRegression");
	boost::filesystem::path referencePath(testPath / "PathFinderRegression.Spartan3E.Test1.xdl");
	boost::filesystem::path generatedPath(testPath / 
		"PathFinderRegression.Spartan3E.Test1.generated.xdl");
std::cout << referencePath.string() << std::endl;

	testRouteDesign(referencePath.string(), generatedPath.string());
}*/
/// \brief Spartan6 regression
/*BOOST_AUTO_TEST_CASE(pathfinder_spartan6) {

	int& argc = boost::unit_test::framework::master_test_suite().argc;;
	BOOST_REQUIRE(argc >= 1);
	char**&argv = boost::unit_test::framework::master_test_suite().argv;;
	torc::common::DirectoryTree directoryTree(argv[0]);
	boost::filesystem::path testPath(directoryTree.getExecutablePath() 
		/ "torc" / "router" / "PathFinderRegression");
	boost::filesystem::path referencePath(testPath / "PathFinderRegression.Spartan6.Test1.xdl");
	boost::filesystem::path generatedPath(testPath / 
		"PathFinderRegression.Spartan6.Test1.generated.xdl");
std::cout << referencePath.string() << std::endl;

	testRouteDesign(referencePath.string(), generatedPath.string());
}*/
/// \brief Spartan6 regression
/*BOOST_AUTO_TEST_CASE(pathfinder_spartan6_big) {

	int& argc = boost::unit_test::framework::master_test_suite().argc;;
	BOOST_REQUIRE(argc >= 1);
	char**&argv = boost::unit_test::framework::master_test_suite().argv;;
	torc::common::DirectoryTree directoryTree(argv[0]);
	boost::filesystem::path testPath(directoryTree.getExecutablePath() 
		/ "torc" / "router" / "PathFinderRegression");
	boost::filesystem::path referencePath(testPath / "PathFinderRegression.Spartan6.Test2.xdl");
	boost::filesystem::path generatedPath(testPath / 
		"PathFinderRegression.Spartan6.Test2.generated.xdl");
std::cout << referencePath.string() << std::endl;

	testRouteDesign(referencePath.string(), generatedPath.string());
}*/
/// \brief VirtexE regression
/*BOOST_AUTO_TEST_CASE(pathfinder_virtexE) {

	int& argc = boost::unit_test::framework::master_test_suite().argc;;
	BOOST_REQUIRE(argc >= 1);
	char**&argv = boost::unit_test::framework::master_test_suite().argv;;
	torc::common::DirectoryTree directoryTree(argv[0]);
	boost::filesystem::path testPath(directoryTree.getExecutablePath() 
		/ "torc" / "router" / "PathFinderRegression");
	boost::filesystem::path referencePath(testPath / "PathFinderRegression.VirtexE.Test1.xdl");
	boost::filesystem::path generatedPath(testPath / 
		"PathFinderRegression.VirtexE.Test1.generated.xdl");
std::cout << referencePath.string() << std::endl;

	testRouteDesign(referencePath.string(), generatedPath.string());
}*/
/// \brief Virtex2P regression
BOOST_AUTO_TEST_CASE(pathfinder_virtex2p) {

	int& argc = boost::unit_test::framework::master_test_suite().argc;;
	BOOST_REQUIRE(argc >= 1);
	char**&argv = boost::unit_test::framework::master_test_suite().argv;;
	torc::common::DirectoryTree directoryTree(argv[0]);
	boost::filesystem::path testPath(directoryTree.getExecutablePath() 
		/ "torc" / "router" / "PathFinderRegression");
	boost::filesystem::path referencePath(testPath / "PathFinderRegression.Virtex2P.Test1.xdl");
	boost::filesystem::path generatedPath(testPath / 
		"PathFinderRegression.Virtex2P.Test1.generated.xdl");
	std::cout << referencePath.string() << std::endl;

	testRouteDesign(referencePath.string(), generatedPath.string());
}
/// \brief Virtex4 regression
BOOST_AUTO_TEST_CASE(pathfinder_virtex4) {

	int& argc = boost::unit_test::framework::master_test_suite().argc;;
	BOOST_REQUIRE(argc >= 1);
	char**&argv = boost::unit_test::framework::master_test_suite().argv;;
	torc::common::DirectoryTree directoryTree(argv[0]);
	boost::filesystem::path testPath(directoryTree.getExecutablePath() 
		/ "torc" / "router" / "PathFinderRegression");
	boost::filesystem::path referencePath(testPath / "PathFinderRegression.Virtex4.Test1.xdl");
	boost::filesystem::path generatedPath(testPath / 
		"PathFinderRegression.Virtex4.Test1.generated.xdl");
	std::cout << referencePath.string() << std::endl;

	testRouteDesign(referencePath.string(), generatedPath.string());
}
/// \brief Virtex5 regression
BOOST_AUTO_TEST_CASE(pathfinder_virtex5) {

	int& argc = boost::unit_test::framework::master_test_suite().argc;;
	BOOST_REQUIRE(argc >= 1);
	char**&argv = boost::unit_test::framework::master_test_suite().argv;;
	torc::common::DirectoryTree directoryTree(argv[0]);
	boost::filesystem::path testPath(directoryTree.getExecutablePath() 
		/ "torc" / "router" / "PathFinderRegression");
	boost::filesystem::path referencePath(testPath / "PathFinderRegression.Virtex5.Test1.xdl");
	boost::filesystem::path generatedPath(testPath / 
		"PathFinderRegression.Virtex5.Test1.generated.xdl");
	std::cout << referencePath.string() << std::endl;

	testRouteDesign(referencePath.string(), generatedPath.string());
}
/// \brief Virtex6 regression
BOOST_AUTO_TEST_CASE(pathfinder_virtex6) {

	int& argc = boost::unit_test::framework::master_test_suite().argc;;
	BOOST_REQUIRE(argc >= 1);
	char**&argv = boost::unit_test::framework::master_test_suite().argv;;
	torc::common::DirectoryTree directoryTree(argv[0]);
	boost::filesystem::path testPath(directoryTree.getExecutablePath() 
		/ "torc" / "router" / "PathFinderRegression");
	boost::filesystem::path referencePath(testPath / "PathFinderRegression.Virtex6.Test1.xdl");
	boost::filesystem::path generatedPath(testPath / 
		"PathFinderRegression.Virtex6.Test1.generated.xdl");
	std::cout << referencePath.string() << std::endl;

	testRouteDesign(referencePath.string(), generatedPath.string());
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

} // namespace router
} // namespace torc
