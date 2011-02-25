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
/// \brief Source for the RouteNode unit test.

#include <boost/test/unit_test.hpp>
#include "torc/router/RouteNode.hpp"

namespace torc {
namespace router {

BOOST_AUTO_TEST_SUITE(router)

/// \brief Unit test for the RouteNode class.
BOOST_AUTO_TEST_CASE(RouteNodeT) {
	typedef architecture::Tilewire Tilewire;
	typedef architecture::Arc Arc;
	// create accessory wires and tiles
	Tilewire tilewire1(architecture::xilinx::TileIndex(100), architecture::xilinx::WireIndex(2));
	Tilewire tilewire2(architecture::xilinx::TileIndex(100), architecture::xilinx::WireIndex(3));
	Tilewire tilewire3(architecture::xilinx::TileIndex(120), architecture::xilinx::WireIndex(2));
	Tilewire tilewire4(architecture::xilinx::TileIndex(120), architecture::xilinx::WireIndex(3));
	boost::int32_t cost1 = 4;
	boost::int32_t cost2 = 5;
	Arc arc1(tilewire1, tilewire2);
	Arc arc2(tilewire2, tilewire3);
	
	
	// features tested:
	//		sizeof(RouteNode)
	BOOST_CHECK_EQUAL(sizeof(RouteNode), sizeof(architecture::Arc) + sizeof(RouteNode*)
		+ sizeof(boost::int32_t));
	BOOST_CHECK_EQUAL(sizeof(RouteNode), 24u);

	// functions tested:
	//		RouteNode(void);
	//		RouteNode(Tilewire inSource, Tilewire inSink, RouteNodeCost inCost, 
	//			RouteNode* inParent);
	RouteNode* routenode1 = new RouteNode(tilewire1, tilewire2, cost1, 0);
	RouteNode* routenode2 = new RouteNode(arc2, cost1, routenode1);
	RouteNode* routenode3 = new RouteNode(tilewire3, tilewire4, cost2, routenode2);
	RouteNode* routenode4 = new RouteNode();
	BOOST_CHECK_EQUAL(routenode1 != 0, true);
	BOOST_CHECK_EQUAL(routenode2 != 0, true);
	BOOST_CHECK_EQUAL(routenode3 != 0, true);
	BOOST_CHECK_EQUAL(routenode4 != 0, true);

	// functions tested:
	//		const Arc& getArc() const;
	//		const Tilewire& getSourceTilewire() const;
	//		const Tilewire& getSinkTilewire() const;
	//		const RouteNodeCost& getCost() const;
	//		void setCost(boost::int32_t inCost);
	//		RouteNode* getParent() const;
	//		RouteNode* getTop();
	architecture::Arc arc1t = routenode1->getArc();
	BOOST_CHECK_EQUAL(arc1t == arc1, true);
	BOOST_CHECK_EQUAL(routenode2->getSourceTilewire() == tilewire2, true);
	BOOST_CHECK_EQUAL(routenode2->getSinkTilewire() == tilewire3, true);
	BOOST_CHECK_EQUAL(routenode3->getCost() == cost2, true);
	BOOST_CHECK_EQUAL(routenode4->getCost() == 0, true);
	routenode4->setCost(42);
	BOOST_CHECK_EQUAL(routenode4->getCost() == 42, true);
	BOOST_CHECK_EQUAL(routenode3->getParent() == routenode2, true);
	BOOST_CHECK_EQUAL(routenode3->getTop() == routenode1, true);
	
	// Test the comparison class
	RouteNodePtrCostCompare compare;
	BOOST_CHECK_EQUAL(compare(routenode1, routenode1), false);
	BOOST_CHECK_EQUAL(compare(routenode2, routenode3), false);
	BOOST_CHECK_EQUAL(compare(routenode3, routenode2), true);
	
	delete routenode1;
	delete routenode2;
	delete routenode3;
	delete routenode4;
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace router
} // namespace torc
