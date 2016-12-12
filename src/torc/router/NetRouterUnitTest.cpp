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
/// \brief Regression test for NetRouter class.

#include <boost/test/unit_test.hpp>
#include <queue>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include "torc/router/RouteNode.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/router/NetRouterHeuristic.hpp"
#include "torc/router/NetRouter.hpp"
#include "torc/router/RouteNet.hpp"
#include "torc/architecture/ExtendedWireInfo.hpp"

namespace torc {
namespace router {

BOOST_AUTO_TEST_SUITE(router)

/// \brief Unit test for the NetRouter.
BOOST_AUTO_TEST_CASE(NetRouterT) {
	architecture::DDB ddb("xc5vlx30");
	NetRouterHeuristicBase* h = new NetRouterHeuristic(ddb);
	NetRouterBase* r = new NetRouter(ddb, h);
	
	architecture::Tilewire twSource = ddb.lookupTilewire("CLBLL_X16Y42", "L_A");
	architecture::Tilewire twSink1 = ddb.lookupTilewire("CLBLL_X16Y42", "L_C3");
	architecture::Tilewire twSink2 = ddb.lookupTilewire("CLBLL_X23Y16", "L_B2");
	std::string netname = "TEST_NET";
	RouteNet net(netname);
	net.addSource(twSource);
	net.addSink(twSink1);
	net.addSink(twSink2);
	
	BOOST_TEST_MESSAGE("ROUTE CALL");
	std::cout << net.routeNodes().size() << std::endl;
	r->route(net);
	double t = boost::any_cast<double>(net.mProperties[eRouteTime]);
	std::cout << t << std::endl;
	BOOST_TEST_MESSAGE("ROUTE FINISHED");
	std::cout << net.routeNodes().size() << std::endl;
	
	RouteNodePtrVector& v = net.routeNodes();
	std::cout << ddb;
	architecture::ExtendedWireInfo ewi(ddb);
	for (unsigned int i = 0; i < v.size(); i++) {
		//std::cout << "pip " << v[i]->getSourceTilewire() << " -> " < v[i]->getSinkTilewire()
		//std::cout << "pip " << v[i]->getArc() << std::endl;
		std::cout << "pip ";
		ewi = v[i]->getSourceTilewire();
		std::cout << ewi.mTileName << " " << ewi.mWireName << " -> ";
		ewi = v[i]->getSinkTilewire();
		std::cout << ewi.mWireName << "," << std::endl;
	}
	
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace router
} // namespace torc
