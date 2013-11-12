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
/// \brief Unit test for the Routethrough class.

#include <boost/test/unit_test.hpp>
#include "torc/physical/Factory.hpp"
#include "torc/physical/Pip.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical)

/// \brief Unit test for the Routethrough class.
BOOST_AUTO_TEST_CASE(RouteThroughUnitTest) {
	// create an accessory instance
	InstanceSharedPtr instancePtr = Factory::newInstancePtr("name", "type", "tile", "site", 
		eInstanceBondingUnknown);
	BOOST_REQUIRE(instancePtr.get() != 0);

	// functions tested:
	//		Routethrough(const string& inSetting, const string& inName, const string& inValue, 
	//			const InstanceWeakPtr& inInstancePtr, const string& inSourceWireName, 
	//			const string& inSinkWireName);
	std::string setting = "_ROUTETHROUGH";
	std::string name = "name";
	std::string value = "value";
	std::string source = "source";
	std::string sink = "sink";
	Routethrough routethrough(setting, name, value, instancePtr, source, sink);

	// functions tested:
	//		const string& getSetting(void) const;
	//		const InstanceWeakPtr& getInstancePtr(void) const;
	//		const WireName& getSourceWireName(void) const;
	//		const WireName& getSinkWireName(void) const;
	BOOST_CHECK_EQUAL(routethrough.getSetting(), setting);
	BOOST_CHECK_EQUAL(routethrough.getSourceWireName(), source);
	BOOST_CHECK_EQUAL(routethrough.getSinkWireName(), sink);
	BOOST_CHECK(routethrough.getInstancePtr().lock() == instancePtr);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
