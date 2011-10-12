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
/// \brief Source for the torc::physical output stream helpers.

#include <boost/test/unit_test.hpp>
#include "torc/physical/OutputStreamHelpers.hpp"
#include "torc/physical/Factory.hpp"
#include <sstream>

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical)

/// \brief Unit test for the output stream helpers.
/// \details It's not pretty, and it's incomplete, but it'll have to do for now.
BOOST_AUTO_TEST_CASE(OutputStreamHelpersUnitTest) {
	std::stringstream s;

	// functions tested:
	//		std::ostream& operator <<(std::ostream& os, const Design& rhs);
	std::string designName = "blinker";
	std::string deviceName = "xc5vlx30";
	std::string devicePackage = "ff324";
	std::string deviceSpeedGrade = "-1";
	std::string xdlVersion = "v3.2";
	DesignSharedPtr designPtr = Factory::newDesignPtr(designName, deviceName, devicePackage, 
		deviceSpeedGrade, xdlVersion);
	s << *designPtr;
	BOOST_CHECK_EQUAL(s.str(), "blinker [xc5vlx30ff324-1, v3.2]");
	s.str(std::string());

	// functions tested:
	//		std::ostream& operator <<(std::ostream& os, const Module& rhs);
	std::string moduleName = "osc7";
	std::string moduleAnchor = "x0y0";
	ModuleSharedPtr modulePtr = Factory::newModulePtr(moduleName, moduleAnchor);
	s << *modulePtr;
	BOOST_CHECK_EQUAL(s.str(), "osc7 [x0y0]");
	s.str(std::string());

	// functions tested:
	//		std::ostream& operator <<(std::ostream& os, const Instance& rhs);
	std::string instanceName = "blink";
	std::string instanceType = "SLICEL";
	std::string instanceTile = "CLBLL_X16Y59";
	std::string instanceSite = "SLICE_X27Y59";
	InstanceSharedPtr instancePtr = Factory::newInstancePtr(instanceName, instanceType, 
		instanceTile, instanceSite);
	s << *instancePtr;
	BOOST_CHECK_EQUAL(s.str(), "blink");
	s.str(std::string());

	// functions tested:
	//		std::ostream& operator <<(std::ostream& os, const Net& rhs);
	std::string netName = "blink";
	ENetType netType = eNetTypeNormal;
	NetSharedPtr netPtr = Factory::newNetPtr(netName, netType);
	s << *netPtr;
	BOOST_CHECK_EQUAL(s.str(), "blink");
	s.str(std::string());

	// functions tested:
	//		std::ostream& operator <<(std::ostream& os, const InstancePin& rhs);
	std::string pinName = "DQ";
	InstancePinSharedPtr instancePinPtr = Factory::newInstancePinPtr(instancePtr, pinName);
	s << *instancePinPtr;
	BOOST_CHECK_EQUAL(s.str(), "blink.DQ");
	s.str(std::string());

	// functions tested:
	//		std::ostream& operator <<(std::ostream& os, const Pip& rhs);
	std::string pipTile = "CLBLL_X16Y59";
	std::string pipSource = "L_DQ";
	std::string pipSink = "SITE_LOGIC_OUTS3";
	EPipDirection pipDirection = ePipUnidirectionalBuffered;
	Pip pip = Factory::newPip(pipTile, pipSource, pipSink, pipDirection);
	s << pip;
	BOOST_CHECK_EQUAL(s.str(), "CLBLL_X16Y59 L_DQ -> SITE_LOGIC_OUTS3");
	s.str(std::string());

	// functions tested:
	//		std::ostream& operator <<(std::ostream& os, const Config& rhs);
	std::string configName = "blink";
	std::string configValue = "#FF";
	Config config(configName, configValue);
	s << config;
	BOOST_CHECK_EQUAL(s.str(), "blink:#FF");
	s.str(std::string());
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
