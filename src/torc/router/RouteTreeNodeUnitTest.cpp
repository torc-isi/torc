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
/// \brief Unit test for the RouteNode class.

#include <boost/test/unit_test.hpp>
#include "torc/router/RouteTreeNode.hpp"

namespace torc {
namespace router {

BOOST_AUTO_TEST_SUITE(router)

/// \brief Unit test for the RouteNode class.
BOOST_AUTO_TEST_CASE(RouteTreeNodeT) {
	typedef architecture::Tilewire Tilewire;
	typedef architecture::Arc Arc;
	// create accessory wires and tiles
	Tilewire tilewire1(architecture::xilinx::TileIndex(100), architecture::xilinx::WireIndex(2));
	Tilewire tilewire2(architecture::xilinx::TileIndex(100), architecture::xilinx::WireIndex(3));
	Tilewire tilewire3(architecture::xilinx::TileIndex(120), architecture::xilinx::WireIndex(2));
	Tilewire tilewire4(architecture::xilinx::TileIndex(120), architecture::xilinx::WireIndex(3));
	Arc arc2(tilewire2, tilewire3);
	boost::int32_t cost1 = 4;
	boost::int32_t cost2 = 5;
	
	// features tested:
	//		sizeof(RouteNode)
	BOOST_CHECK_EQUAL(sizeof(RouteTreeNode), sizeof(RouteNode)
		+ sizeof(RouteTreeNode*) + sizeof(std::vector<RouteTreeNode*>*));
	BOOST_CHECK_EQUAL(sizeof(RouteNode), 32u);
	BOOST_CHECK_EQUAL(sizeof(RouteTreeNode), 48u);
	BOOST_CHECK_EQUAL(sizeof(RouteTreeNode*), 8u);
	BOOST_CHECK_EQUAL(sizeof(std::vector<RouteTreeNode*>*), 8u);
	BOOST_CHECK_EQUAL(sizeof(boost::int32_t), 4u);

	// functions tested:
	//		RouteTreeNode();
	//		RouteTreeNode(Tilewire inSource, Tilewire inSink, boost::int32_t inCost, 
	//		RouteTreeNode* inParent)
	RouteTreeNode* routenode1 = new RouteTreeNode(tilewire1, tilewire2, cost1, 0);
	RouteTreeNode* routenode2 = new RouteTreeNode(arc2, cost1, routenode1);
	RouteTreeNode* routenode3 = new RouteTreeNode(tilewire3, tilewire4, cost2, routenode2);
	RouteTreeNode* routenode4 = new RouteTreeNode(tilewire4, tilewire1, cost2, routenode2);
	RouteTreeNode* routenode5 = new RouteTreeNode();
	BOOST_CHECK_EQUAL(routenode1 != 0, true);
	BOOST_CHECK_EQUAL(routenode2 != 0, true);
	BOOST_CHECK_EQUAL(routenode3 != 0, true);
	BOOST_CHECK_EQUAL(routenode4 != 0, true);
	BOOST_CHECK_EQUAL(routenode5 != 0, true);

	// functions tested:
	//		boost::int32_t getDepth() const;
	//		addChildren(const std::vector<RouteTreeNode*>& newChildren);
	//		boost::uint16_t getNumChildren();
	//		RouteTreeNode* getChild(unsigned int index);
	//		makeParent(const Tilewire& inSource, const Tilewire& inSink;
	//		void normalizeDepth;
	//		void adjustDepth(int adjustment);
	std::vector<RouteTreeNode*> vec1;
	vec1.push_back(routenode2);
	std::vector<RouteTreeNode*> vec2;
	vec2.push_back(routenode3);
	vec2.push_back(routenode4);
	routenode1->addChildren(vec1);
	routenode2->addChildren(vec2);
	BOOST_CHECK_EQUAL(routenode1->getDepth() == 0, true);
	BOOST_CHECK_EQUAL(routenode2->getDepth() == 1, true);
	BOOST_CHECK_EQUAL(routenode3->getDepth() == 2, true);
	BOOST_CHECK_EQUAL(routenode4->getDepth() == 2, true);
	BOOST_CHECK_EQUAL(routenode5->getDepth() == -1, true);
	BOOST_CHECK_EQUAL(routenode1->getNumChildren() == 1, true);
	BOOST_CHECK_EQUAL(routenode2->getNumChildren() == 2, true);
	BOOST_CHECK_EQUAL(routenode3->getNumChildren() == 0, true);
	BOOST_CHECK_EQUAL(routenode4->getNumChildren() == 0, true);
	BOOST_CHECK_EQUAL(routenode1->getChild(0) == routenode2, true);
	BOOST_CHECK_EQUAL(routenode2->getChild(0) == routenode3, true);
	BOOST_CHECK_EQUAL(routenode2->getChild(1) == routenode4, true);
	
	routenode1->makeParent(tilewire4, tilewire3);
	BOOST_CHECK_EQUAL(tilewire1 == routenode1->getSourceTilewire(), true);
	BOOST_CHECK_EQUAL(tilewire2 == routenode1->getSinkTilewire(), true);
	RouteTreeNode* parentnode = (RouteTreeNode*)routenode1->getParent();
	BOOST_CHECK_EQUAL(tilewire4 == parentnode->getSourceTilewire(), true);
	BOOST_CHECK_EQUAL(tilewire3 == parentnode->getSinkTilewire(), true);
	BOOST_CHECK_EQUAL(parentnode->getDepth() == -1, true);
	routenode4->normalizeDepth();
	BOOST_CHECK_EQUAL(parentnode->getDepth() == 0, true);
	BOOST_CHECK_EQUAL(routenode1->getDepth() == 1, true);
	BOOST_CHECK_EQUAL(routenode2->getDepth() == 2, true);
	BOOST_CHECK_EQUAL(routenode3->getDepth() == 3, true);
	BOOST_CHECK_EQUAL(routenode4->getDepth() == 3, true);
	BOOST_CHECK_EQUAL(routenode5->getDepth() == -1, true);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace router
} // namespace torc
