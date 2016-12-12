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
/// \brief Regression test for Heuristic class.

#include <boost/test/unit_test.hpp>
#include "torc/router/NetRouterHeuristic.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/OutputStreamHelpers.hpp"
#include <iostream>
#include <vector>

namespace torc {
namespace router {

BOOST_AUTO_TEST_SUITE(router)

/// \brief Unit test for the Heuristic.
BOOST_AUTO_TEST_CASE(NetRouterHeuristicT) {

	typedef architecture::Tilewire Tilewire;
	
	architecture::DDB db("xc5vlx30");
	//architecture::DDB db("torc/devices/xc5vlx30");
	NetRouterHeuristic h(db);
	
	Tilewire tw1(architecture::xilinx::TileIndex(3000), architecture::xilinx::WireIndex(100));
	Tilewire tw2(architecture::xilinx::TileIndex(4004), architecture::xilinx::WireIndex(225));
	
	RouteNode* node1 = new RouteNode(tw1, tw1, 50, 50, 0, 0);
	RouteNode* node2 = new RouteNode(tw1, tw1, 0, 0, 1, node1);
	
//	ExtendedWireInfo ewi1(db, tw1);
//	ExtendedWireInfo ewi2(db, tw2);
	
//	std::cout << ewi1 << std::endl;
//	std::cout << ewi2 << std::endl;
	
	h.setSink(tw2);
	h.nodeCostInitial(*node1);
	h.nodeCost(*node2);

	BOOST_CHECK_EQUAL(node1->getCost() == 74, true);
	BOOST_CHECK_EQUAL(node2->getCost() == 148, true);
	
	delete node1;
	delete node2;
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace router
} // namespace torc
