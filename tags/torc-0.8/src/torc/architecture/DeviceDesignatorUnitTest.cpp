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
/// \brief Source for the DeviceDesignator unit test.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/DeviceDesignator.hpp"

namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the DeviceDesignator class.
BOOST_AUTO_TEST_CASE(architecture_device_name) {

	// functions tested:
	//		DeviceDesignator(const string& inDeviceDesignator);
	//		bool parse(const string& inDeviceDesignator, const boost::regex& inRegEx);
	DeviceDesignator deviceName0("xc3s1600efg484-5");
	BOOST_CHECK_EQUAL(deviceName0.getFamily(), DeviceDesignator::eFamilySpartan3E);
	BOOST_CHECK_EQUAL(deviceName0.getDeviceName(), "xc3s1600e");
	BOOST_CHECK_EQUAL(deviceName0.getDevicePackage(), "fg484");
	BOOST_CHECK_EQUAL(deviceName0.getDeviceSpeedGrade(), "-5");
	DeviceDesignator deviceName1("xcv3200efg1156-8");
	BOOST_CHECK_EQUAL(deviceName1.getFamily(), DeviceDesignator::eFamilyVirtexE);
	BOOST_CHECK_EQUAL(deviceName1.getDeviceName(), "xcv3200e");
	BOOST_CHECK_EQUAL(deviceName1.getDevicePackage(), "fg1156");
	BOOST_CHECK_EQUAL(deviceName1.getDeviceSpeedGrade(), "-8");
	DeviceDesignator deviceName2("xc2vp30ff896-7");
	BOOST_CHECK_EQUAL(deviceName2.getFamily(), DeviceDesignator::eFamilyVirtex2P);
	BOOST_CHECK_EQUAL(deviceName2.getDeviceName(), "xc2vp30");
	BOOST_CHECK_EQUAL(deviceName2.getDevicePackage(), "ff896");
	BOOST_CHECK_EQUAL(deviceName2.getDeviceSpeedGrade(), "-7");
	DeviceDesignator deviceName3("xc4vfx60ff1152-12");
	BOOST_CHECK_EQUAL(deviceName3.getFamily(), DeviceDesignator::eFamilyVirtex4);
	BOOST_CHECK_EQUAL(deviceName3.getDeviceName(), "xc4vfx60");
	BOOST_CHECK_EQUAL(deviceName3.getDevicePackage(), "ff1152");
	BOOST_CHECK_EQUAL(deviceName3.getDeviceSpeedGrade(), "-12");

	// the above tests do not check the recently added devices
	BOOST_CHECK(false);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
