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
/// \brief Unit test for the DeviceDesignator class.

#include <boost/test/unit_test.hpp>
#include "torc/common/DeviceDesignator.hpp"
#include <sstream>

namespace torc {
namespace common {

BOOST_AUTO_TEST_SUITE(common)

/// \brief Unit test for the DeviceDesignator class.
BOOST_AUTO_TEST_CASE(DeviceDesignatorUnitTest) {

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
	DeviceDesignator deviceName4("xc5vfx130tff1738-3");
	BOOST_CHECK_EQUAL(deviceName4.getFamily(), DeviceDesignator::eFamilyVirtex5);
	BOOST_CHECK_EQUAL(deviceName4.getDeviceName(), "xc5vfx130t");
	BOOST_CHECK_EQUAL(deviceName4.getDevicePackage(), "ff1738");
	BOOST_CHECK_EQUAL(deviceName4.getDeviceSpeedGrade(), "-3");
	DeviceDesignator deviceName5("xc6vhx380tff1924-3");
	BOOST_CHECK_EQUAL(deviceName5.getFamily(), DeviceDesignator::eFamilyVirtex6);
	BOOST_CHECK_EQUAL(deviceName5.getDeviceName(), "xc6vhx380t");
	BOOST_CHECK_EQUAL(deviceName5.getDevicePackage(), "ff1924");
	BOOST_CHECK_EQUAL(deviceName5.getDeviceSpeedGrade(), "-3");
	DeviceDesignator deviceName6("xc7vx485tffg1929-3");
	BOOST_CHECK_EQUAL(deviceName6.getFamily(), DeviceDesignator::eFamilyVirtex7);
	BOOST_CHECK_EQUAL(deviceName6.getDeviceName(), "xc7vx485t");
	BOOST_CHECK_EQUAL(deviceName6.getDevicePackage(), "ffg1929");
	BOOST_CHECK_EQUAL(deviceName6.getDeviceSpeedGrade(), "-3");

	// functions tested:
	//		std::ostream& operator<< (std::ostream& os);
	std::stringstream ss;
	ss << deviceName6;
	BOOST_CHECK_EQUAL(ss.str(), "xc7vx485tffg1929-3");

	// the above tests are not exhaustive
	//BOOST_CHECK(false);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace common
} // namespace torc
