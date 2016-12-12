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
/// \brief Source for the TraceNode unit test.

#include <boost/test/unit_test.hpp>
#include "torc/router/TraceNode.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/OutputStreamHelpers.hpp"

namespace torc {
namespace router {

BOOST_AUTO_TEST_SUITE(router)

/// \brief Unit test for the RouteNode class.
BOOST_AUTO_TEST_CASE(TraceNodeT) {
	typedef architecture::Tilewire Tilewire;
	
	// create accessory wires and tiles
	Tilewire tilewire1(architecture::xilinx::TileIndex(100), architecture::xilinx::WireIndex(1));
	Tilewire tilewire2(architecture::xilinx::TileIndex(100), architecture::xilinx::WireIndex(2));
	Tilewire tilewire3(architecture::xilinx::TileIndex(120), architecture::xilinx::WireIndex(3));
	Tilewire tilewire4(architecture::xilinx::TileIndex(120), architecture::xilinx::WireIndex(4));
	Tilewire tilewire5(architecture::xilinx::TileIndex(140), architecture::xilinx::WireIndex(5));
	Tilewire tilewire6(architecture::xilinx::TileIndex(140), architecture::xilinx::WireIndex(6));
	
	// features tested:
	//		sizeof(TraceNode)
	BOOST_CHECK_EQUAL(sizeof(TraceNode), sizeof(boost::int32_t)
		+ sizeof(Tilewire) + sizeof(std::vector<TraceNode*>)
		+ sizeof(std::vector<std::pair<Tilewire, TraceNode*> >));
	BOOST_CHECK_EQUAL(sizeof(std::vector<TraceNode*>), 24u);
	BOOST_CHECK_EQUAL(sizeof(std::vector<std::pair<Tilewire, TraceNode*> >), 24u);
	BOOST_CHECK_EQUAL(sizeof(Tilewire), 6u);
	BOOST_CHECK_EQUAL(sizeof(TraceNode), 58u);
	BOOST_CHECK_EQUAL(sizeof(TraceNode*), 8u);
	BOOST_CHECK_EQUAL(sizeof(boost::int32_t), 4u);

	// functions tested:
	//		TraceNode(Tilewire inTilewire) 
	TraceNode* tracenode1 = new TraceNode(tilewire1);
	TraceNode* tracenode2 = new TraceNode(tilewire2);
	TraceNode* tracenode3 = new TraceNode(tilewire3);
	TraceNode* tracenode4 = new TraceNode(tilewire4);
	TraceNode* tracenode5 = new TraceNode(tilewire5);
	TraceNode* tracenode6 = new TraceNode(tilewire6);
	
	BOOST_CHECK(TraceNode::sLiveNodes == 6);
	
	//functions tested:
	//		getTilewire()
	//		getDepth()
	//		getNumChildren()
	//		getNumParents()
	//		getChild()
	//		getParent()
	//		removeChild()
	//		removeParent()
	BOOST_CHECK(tracenode1 != 0);
	BOOST_CHECK(tracenode1->getDepth() == -1);
	BOOST_CHECK(tracenode1->getNumChildren() == 0);
	BOOST_CHECK(tracenode1->getNumParents() == 0);
	BOOST_CHECK(tracenode1->getTilewire() == tilewire1);
	BOOST_CHECK(tracenode1->getChild(0) == 0);
	BOOST_CHECK(tracenode1->getParent(0) == 0);
	BOOST_CHECK(tracenode1->removeChild(0) == 0);
	BOOST_CHECK(tracenode1->removeParent(0) == 0);
	
	// attach a bunch of nodes together
	tracenode1->addChild(tracenode3);
	BOOST_CHECK(tracenode1->getNumChildren() == 1);
	BOOST_CHECK(tracenode1->getChild(0)->getTilewire() == tilewire3);
	tracenode2->addChild(tracenode4);
	BOOST_CHECK(tracenode2->getNumChildren() == 1);
	BOOST_CHECK(tracenode2->getChild(0)->getTilewire() == tilewire4);
	tracenode3->addParent(tracenode1);
	BOOST_CHECK(tracenode3->getNumParents() == 1);
	BOOST_CHECK(tracenode3->getParent(0)->getTilewire() == tilewire1);
	tracenode3->addChild(tracenode4);
	BOOST_CHECK(tracenode3->getNumChildren() == 1);
	BOOST_CHECK(tracenode3->getChild(0)->getTilewire() == tilewire4);
	tracenode4->addParent(tracenode2);
	BOOST_CHECK(tracenode4->getNumParents() == 1);
	tracenode4->addParent(tracenode3);
	BOOST_CHECK(tracenode4->getNumParents() == 2);
	std::vector<TraceNode*> testvec;
	testvec.push_back(tracenode5);
	testvec.push_back(tracenode6);
	tracenode4->addChildren(testvec);
	BOOST_CHECK(tracenode4->getNumChildren() == 2);
	tracenode5->addParent(tracenode4);
	BOOST_CHECK(tracenode5->getNumParents() == 1);
	tracenode6->addParent(tracenode4);
	BOOST_CHECK(tracenode6->getNumParents() == 1);
	
	// remove all node connections
	BOOST_CHECK(tracenode1->removeChild(0)->getTilewire() == tilewire3);
	BOOST_CHECK(tracenode1->getNumChildren() == 0);
	BOOST_CHECK(tracenode2->removeChild(0)->getTilewire() == tilewire4);
	BOOST_CHECK(tracenode2->getNumChildren() == 0);
	BOOST_CHECK(tracenode3->removeParent(0) == tracenode1);
	BOOST_CHECK(tracenode3->getNumParents() == 0);
	BOOST_CHECK(tracenode3->removeChild(0)->getTilewire() == tilewire4);
	BOOST_CHECK(tracenode3->getNumChildren() == 0);
	BOOST_CHECK(tracenode4->removeParent(1) == tracenode3);
	BOOST_CHECK(tracenode4->getNumParents() == 1);
	BOOST_CHECK(tracenode4->removeParent(0) == tracenode2);
	BOOST_CHECK(tracenode4->getNumParents() == 0);
	BOOST_CHECK(tracenode4->removeChild(0)->getTilewire() == tilewire5);
	BOOST_CHECK(tracenode4->getNumChildren() == 1);
	BOOST_CHECK(tracenode4->removeChild(0)->getTilewire() == tilewire6);
	BOOST_CHECK(tracenode4->getNumChildren() == 0);
	BOOST_CHECK(tracenode5->removeParent(0) == tracenode4);
	BOOST_CHECK(tracenode5->getNumParents() == 0);
	BOOST_CHECK(tracenode6->removeParent(0)->getTilewire() == tilewire4);
	BOOST_CHECK(tracenode6->getNumParents() == 0);
	
	// clean up
	delete tracenode1;
	delete tracenode2;
	delete tracenode3;
	delete tracenode4;
	delete tracenode5;
	delete tracenode6;
	
	BOOST_CHECK(TraceNode::sLiveNodes == 0);
	
	tracenode1 = new TraceNode(tilewire1);
	tracenode2 = new TraceNode(tilewire2);
	tracenode3 = new TraceNode(tilewire3);
	tracenode4 = new TraceNode(tilewire4);
	tracenode5 = new TraceNode(tilewire5);
	tracenode6 = new TraceNode(tilewire6);
	
	// attach the nodes together again
	tracenode1->addChild(tracenode3);
	tracenode2->addChild(tracenode4);
	tracenode3->addParent(tracenode1);
	tracenode3->addChild(tracenode4);
	tracenode4->addParent(tracenode2);
	tracenode4->addParent(tracenode3);
	testvec.clear();
	testvec.push_back(tracenode5);
	testvec.push_back(tracenode6);
	tracenode4->addChildren(testvec);
	tracenode5->addParent(tracenode4);
	tracenode6->addParent(tracenode4);
	
	BOOST_CHECK(TraceNode::sLiveNodes == 6);
	
	// testing the destructor, but no way to verfiy that it is working???
	delete tracenode1;
	delete tracenode2;
	delete tracenode3;
	delete tracenode4;
	delete tracenode5;
	delete tracenode6;
	
	BOOST_CHECK(TraceNode::sLiveNodes == 0);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace router
} // namespace torc
