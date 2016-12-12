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
/// \brief Regression test for Tracer class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/XdlImporter.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/router/Trace.hpp"
#include <map>

//#include <boost/filesystem/convenience.hpp>
#include "torc/common/DirectoryTree.hpp"
//#include "torc/physical/Factory.hpp"
//#include "torc/physical/XdlExporter.hpp"
#include "torc/physical/OutputStreamHelpers.hpp"
#include "torc/common/TestHelpers.hpp"
#include <fstream>

namespace torc {
namespace router {

struct TraceUnitTestFixture {

	architecture::DDB ddb;
	architecture::Tilewire twSource;
	architecture::Tilewire twSink1;
	architecture::Tilewire twSink2;
	architecture::Tilewire twMid;
	
	TraceUnitTestFixture() : ddb("xc5vlx30") {
		//tracer(*ddbPtr);
		BOOST_TEST_MESSAGE("CONSTRUCT");
		BOOST_TEST_MESSAGE(ddb);
		ddb.useArc(ddb.tilePipToArc("CLBLL_X16Y42", "SITE_CTRL_B0", "L_CE"));
		ddb.useArc(ddb.tilePipToArc("CLBLL_X16Y42", "SITE_IMUX_B5", "L_A1"));
		ddb.useArc(ddb.tilePipToArc("INT_X16Y42", "CTRL0", "CTRL_B0"));
		ddb.useArc(ddb.tilePipToArc("INT_X16Y42", "FAN6", "FAN_BOUNCE6"));
		ddb.useArc(ddb.tilePipToArc("INT_X16Y42", "FAN_BOUNCE6", "GFAN1"));
		ddb.useArc(ddb.tilePipToArc("INT_X16Y42", "GFAN1", "CTRL0"));
		ddb.useArc(ddb.tilePipToArc("INT_X16Y42", "NL2END2", "FAN6"));
		ddb.useArc(ddb.tilePipToArc("INT_X16Y42", "NL2END2", "IMUX_B5"));
		ddb.useArc(ddb.tilePipToArc("INT_X16Y40", "NW2MID1", "NL2BEG2"));
		
		twSource = ddb.lookupTilewire("INT_X16Y40", "NW2MID1");
		twSink1 = ddb.lookupTilewire("CLBLL_X16Y42", "L_CE");
		twSink2 = ddb.lookupTilewire("CLBLL_X16Y42", "L_A1");
		twMid = ddb.lookupTilewire("INT_X16Y42", "FAN_BOUNCE6");
		
		BOOST_CHECK_EQUAL(ddb.getArcUsage().getArcUsageCount(), 9u);
	}
	~TraceUnitTestFixture() {}
};

BOOST_AUTO_TEST_SUITE(router)

BOOST_FIXTURE_TEST_SUITE(TraceT, TraceUnitTestFixture)

BOOST_AUTO_TEST_CASE(trace_full_net) {
	Trace trace(ddb, twMid, Trace::eTraceFullNet);
	BOOST_CHECK_EQUAL(trace.getSources().size(), 1u);
	BOOST_CHECK_EQUAL(trace.getSinks().size(), 2u);
	BOOST_CHECK_EQUAL(trace.getBranchPoints().size(), 1u);
	BOOST_CHECK_EQUAL(trace.getArcs().size(), 9u);
}

BOOST_AUTO_TEST_CASE(trace_to_sinks) {
	Trace trace(ddb, twMid, Trace::eTraceToSinks);
	BOOST_CHECK_EQUAL(trace.getSources().size(), 0u);
	BOOST_CHECK_EQUAL(trace.getSinks().size(), 1u);
	BOOST_CHECK_EQUAL(trace.getBranchPoints().size(), 0u);
	BOOST_CHECK_EQUAL(trace.getArcs().size(), 4u);
}

BOOST_AUTO_TEST_CASE(trace_to_branch) {
	Trace trace(ddb, twMid, Trace::eTraceToBranch);
	BOOST_CHECK_EQUAL(trace.getSources().size(), 0u);
	BOOST_CHECK_EQUAL(trace.getSinks().size(), 1u);
	BOOST_CHECK_EQUAL(trace.getBranchPoints().size(), 1u);
	BOOST_CHECK_EQUAL(trace.getArcs().size(), 6u);
}

BOOST_AUTO_TEST_CASE(trace_to_sources) {
	Trace trace(ddb, twMid, Trace::eTraceToSources);
	BOOST_CHECK_EQUAL(trace.getSources().size(), 1u);
	BOOST_CHECK_EQUAL(trace.getSinks().size(), 0u);
	BOOST_CHECK_EQUAL(trace.getBranchPoints().size(), 1u);
	BOOST_CHECK_EQUAL(trace.getArcs().size(), 3u);
}

BOOST_AUTO_TEST_CASE(trace_single_path) {
	Trace trace(ddb, twMid, Trace::eTraceSinglePath);
	BOOST_CHECK_EQUAL(trace.getSources().size(), 0u);
	BOOST_CHECK_EQUAL(trace.getSinks().size(), 1u);
	BOOST_CHECK_EQUAL(trace.getBranchPoints().size(), 1u);
	BOOST_CHECK_EQUAL(trace.getArcs().size(), 6u);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

} // namespace router
} // namespace torc
