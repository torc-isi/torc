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
/// \brief Source for the RouteNet unit test.

#include <boost/test/unit_test.hpp>
#include "torc/router/RouteNet.hpp"

namespace torc {
namespace router {

BOOST_AUTO_TEST_SUITE(router)

/// \brief Unit test for the Net class.
BOOST_AUTO_TEST_CASE(RouteNetT) {
	typedef architecture::Tilewire Tilewire;
	typedef architecture::TilewireVector TilewireVector;
	typedef architecture::Arc Arc;
	typedef architecture::ArcVector ArcVector;
	typedef std::string string;
	
	Tilewire tilewire1(architecture::xilinx::TileIndex(100), architecture::xilinx::WireIndex(2));
	Tilewire tilewire2(architecture::xilinx::TileIndex(100), architecture::xilinx::WireIndex(3));
	Tilewire tilewire3(architecture::xilinx::TileIndex(120), architecture::xilinx::WireIndex(2));
	Tilewire tilewire4(architecture::xilinx::TileIndex(120), architecture::xilinx::WireIndex(3));
	Arc arc1(tilewire1, tilewire2);
	Arc arc2(tilewire2, tilewire3);
	string name1 = "Name1";
	string name2 = "Name2";
	TilewireVector sources;
	TilewireVector sinks;
	sources.push_back(tilewire1);
	sinks.push_back(tilewire2);
	sinks.push_back(tilewire3);
	
	// features tested:
	//		sizeof(RouteNet);
	BOOST_CHECK_EQUAL(sizeof(RouteNet), sizeof(string) + sizeof(TilewireVector)
		+ sizeof(TilewireVector) + sizeof(ArcVector) + sizeof(RouteNodePtrVector)
		+ sizeof(boost::unordered_map<boost::uint32_t, boost::any>));
	//BOOST_CHECK_EQUAL(sizeof(RouteNet), 144u);
	
	// functions tested:
	//		RouteNet(string& inName);
	//		RouteNet(string& inName, const TilewireVector& inSources, const TilewireVector& inSinks)
	RouteNet rn1(name1);
	RouteNet rn2(name2, sources, sinks);

	// functions tested:
	//		bool containsSource(Tilewire inTilewire)
	//		bool containsSink(Tilewire inTilewire)
	//		bool containsArc(const Arc& inArc)
	//		void addSource(Tilewire inTilewire)
	//		void addSink(Tilewire inTilewire)
	//		bool removeSource(Tilewire inTilewire)
	//		bool removeSink(Tilewire inTilewire)
	//		void addArc(const Arc& inArc)
	//		bool removeArc(const Arc& inArc)
	//		void unroute(void)
	//		bool hasAnySources(void) const
	//		bool hasOneSource(void) const
	//		bool hasMultipleSources(void) const
	//		size_t getSourceCount(void) const
	//		bool hasAnySinks(void) const
	//		bool hasOneSink(void) const
	//		bool hasMultipleSinks(void) const
	//		size_t getSinkCount(void) const
	//		bool hasAnyArcs(void) const
	//		size_t getArcCount(void) const
	//		const string& const getName()
	BOOST_CHECK_EQUAL(rn1.getName(), name1);
	BOOST_CHECK_EQUAL(rn1.containsSource(tilewire4), false);
	BOOST_CHECK_EQUAL(rn1.containsSink(tilewire4), false);
	BOOST_CHECK_EQUAL(rn1.containsArc(arc1), false);
	BOOST_CHECK_EQUAL(rn1.hasAnySources(), false);
	BOOST_CHECK_EQUAL(rn1.hasOneSource(), false);
	BOOST_CHECK_EQUAL(rn1.hasAnySinks(), false);
	BOOST_CHECK_EQUAL(rn1.hasOneSink(), false);
	BOOST_CHECK_EQUAL(rn1.hasMultipleSources(), false);
	BOOST_CHECK_EQUAL(rn1.hasMultipleSinks(), false);
	BOOST_CHECK_EQUAL(rn1.hasAnyArcs(), false);
	BOOST_CHECK_EQUAL(rn1.removeSource(tilewire2), false);
	BOOST_CHECK_EQUAL(rn1.removeSink(tilewire2), false);
	BOOST_CHECK_EQUAL(rn1.removeArc(arc1), false);
	
	BOOST_CHECK_EQUAL(rn2.getName(), name2);
	BOOST_CHECK_EQUAL(rn2.hasAnySources(), true);
	BOOST_CHECK_EQUAL(rn2.hasOneSource(), true);
	BOOST_CHECK_EQUAL(rn2.hasAnySinks(), true);
	BOOST_CHECK_EQUAL(rn2.hasMultipleSinks(), true);
	BOOST_CHECK_EQUAL(rn2.containsSource(tilewire1), true);
	BOOST_CHECK_EQUAL(rn2.containsSink(tilewire3), true);
	BOOST_CHECK_EQUAL(rn2.containsArc(arc1), false);
	rn2.addArc(arc1);
	BOOST_CHECK_EQUAL(rn2.containsArc(arc1), true);
	BOOST_CHECK_EQUAL(rn2.containsSource(tilewire4), false);
	rn2.addSource(tilewire4);
	BOOST_CHECK_EQUAL(rn2.containsSource(tilewire4), true);
	BOOST_CHECK_EQUAL(rn2.removeSource(tilewire4), true);
	BOOST_CHECK_EQUAL(rn2.containsSource(tilewire4), false);
	BOOST_CHECK_EQUAL(rn2.containsSink(tilewire2), true);
	BOOST_CHECK_EQUAL(rn2.removeSink(tilewire2), true);
	BOOST_CHECK_EQUAL(rn2.containsSink(tilewire2), false);
	rn2.addSink(tilewire2);
	BOOST_CHECK_EQUAL(rn2.containsSink(tilewire2), true);
	BOOST_CHECK_EQUAL(rn2.getArcCount(), 1u);
	
	
	

	
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
