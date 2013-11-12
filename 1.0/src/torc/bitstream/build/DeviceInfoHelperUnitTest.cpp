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
/// \brief Unit test for the DeviceInfoHelper class.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/build/DeviceInfoHelper.hpp"
#include "torc/bitstream/Spartan3E.hpp"
#include "torc/bitstream/Spartan6.hpp"
#include "torc/bitstream/Virtex.hpp"
#include "torc/bitstream/VirtexE.hpp"
#include "torc/bitstream/Virtex2.hpp"
#include "torc/bitstream/Virtex2P.hpp"
#include "torc/bitstream/Virtex4.hpp"
#include "torc/bitstream/Virtex5.hpp"
#include "torc/bitstream/Virtex6.hpp"
#include "torc/bitstream/Virtex7.hpp"

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)

/// \brief Unit test for the DeviceInfoHelper class.
BOOST_AUTO_TEST_CASE(DeviceInfoHelperUnitTest) {

	BOOST_REQUIRE(true);

}

//#define GENERATE_STATIC_DEVICE_INFO
#ifdef GENERATE_STATIC_DEVICE_INFO

/// \brief Unit test for the Spartan3E static device info generation.
BOOST_AUTO_TEST_CASE(Spartan3EGenerateUnitTest) {
	Spartan3E bitstream;
	DeviceInfoHelper::buildFamilyDeviceInfo("Spartan3E", "Spartan3EDeviceInfo.template", 
		"Spartan3EDeviceInfo.cpp", torc::common::Devices::getSpartan3EDevices(), bitstream);
}

/// \brief Unit test for the Virtex static device info generation.
BOOST_AUTO_TEST_CASE(VirtexGenerateUnitTest) {
	Virtex bitstream;
	DeviceInfoHelper::buildFamilyDeviceInfo("Virtex", "VirtexDeviceInfo.template", 
		"VirtexDeviceInfo.cpp", torc::common::Devices::getVirtexDevices(), bitstream);
}

/// \brief Unit test for the VirtexE static device info generation.
BOOST_AUTO_TEST_CASE(VirtexEGenerateUnitTest) {
	VirtexE bitstream;
	DeviceInfoHelper::buildFamilyDeviceInfo("VirtexE", "VirtexEDeviceInfo.template", 
		"VirtexEDeviceInfo.cpp", torc::common::Devices::getVirtexEDevices(), bitstream);
}

/// \brief Unit test for the Virtex2 static device info generation.
BOOST_AUTO_TEST_CASE(Virtex2GenerateUnitTest) {
	Virtex2 bitstream;
	DeviceInfoHelper::buildFamilyDeviceInfo("Virtex2", "Virtex2DeviceInfo.template", 
		"Virtex2DeviceInfo.cpp", torc::common::Devices::getVirtex2Devices(), bitstream);
}

/// \brief Unit test for the Virtex2P static device info generation.
BOOST_AUTO_TEST_CASE(Virtex2PGenerateUnitTest) {
	Virtex2P bitstream;
	DeviceInfoHelper::buildFamilyDeviceInfo("Virtex2P", "Virtex2PDeviceInfo.template", 
		"Virtex2PDeviceInfo.cpp", torc::common::Devices::getVirtex2PDevices(), bitstream);
}

/// \brief Unit test for the Virtex4 static device info generation.
BOOST_AUTO_TEST_CASE(Virtex4GenerateUnitTest) {
	Virtex4 bitstream;
	DeviceInfoHelper::buildFamilyDeviceInfo("Virtex4", "Virtex4DeviceInfo.template", 
		"Virtex4DeviceInfo.cpp", torc::common::Devices::getVirtex4Devices(), bitstream);
}

/// \brief Unit test for the Virtex5 static device info generation.
BOOST_AUTO_TEST_CASE(Virtex5GenerateUnitTest) {
	Virtex5 bitstream;
	DeviceInfoHelper::buildFamilyDeviceInfo("Virtex5", "Virtex5DeviceInfo.template", 
		"Virtex5DeviceInfo.cpp", torc::common::Devices::getVirtex5Devices(), bitstream);
}

/// \brief Unit test for the Virtex6 static device info generation.
BOOST_AUTO_TEST_CASE(Virtex6GenerateUnitTest) {
	Virtex6 bitstream;
	DeviceInfoHelper::buildFamilyDeviceInfo("Virtex6", "Virtex6DeviceInfo.template", 
		"Virtex6DeviceInfo.cpp", torc::common::Devices::getVirtex6Devices(), bitstream);
}

/// \brief Unit test for the Virtex7 static device info generation.
BOOST_AUTO_TEST_CASE(Virtex7GenerateUnitTest) {
	Virtex7 bitstream;
	const torc::common::DeviceVector& artix7Devices = torc::common::Devices::getArtix7Devices();
	const torc::common::DeviceVector& virtex7Devices = torc::common::Devices::getVirtex7Devices();
	const torc::common::DeviceVector& kintex7Devices = torc::common::Devices::getKintex7Devices();
	const torc::common::DeviceVector& zynq7000Devices = torc::common::Devices::getZynq7000Devices();
	torc::common::DeviceVector devices;
	devices.insert(devices.end(), artix7Devices.begin(), artix7Devices.end());
	devices.insert(devices.end(), kintex7Devices.begin(), kintex7Devices.end());
	devices.insert(devices.end(), virtex7Devices.begin(), virtex7Devices.end());
	devices.insert(devices.end(), zynq7000Devices.begin(), zynq7000Devices.end());
(void) artix7Devices;
(void) kintex7Devices;
(void) virtex7Devices;
(void) zynq7000Devices;
	DeviceInfoHelper::buildFamilyDeviceInfo("Virtex7", "Virtex7DeviceInfo.template", 
		"Virtex7DeviceInfo.cpp", devices, bitstream);
}

#endif

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
