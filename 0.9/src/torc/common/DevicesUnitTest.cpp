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
/// \brief Unit test for Devices class.

#include <boost/test/unit_test.hpp>
#include "torc/common/Devices.hpp"

namespace torc {
namespace common {

BOOST_AUTO_TEST_SUITE(common)

/// \brief Unit test for the Devices class.
BOOST_AUTO_TEST_CASE(DevicesUnitTest) {
	// members untested
	//		DeviceVector mVirtexDevices;
	//		DeviceVector mVirtexEDevices;
	//		DeviceVector mVirtex2Devices;
	//		DeviceVector mVirtex2PDevices;
	//		DeviceVector mVirtex4Devices;
	//		DeviceVector mVirtex5Devices;
	//		DeviceVector mVirtex6Devices;
	//		DeviceVector mVirtex6LDevices;
	//		DeviceVector mVirtex7Devices;
	//		DeviceVector mVirtex7LDevices;
	//		DeviceVector mKintex7Devices;
	//		DeviceVector mKintex7LDevices;
	//		DeviceVector mSpartan3EDevices;
	//		DeviceVector mSpartan6Devices;
	//		DeviceVector mSpartan6LDevices;
	// functions untested:
	//		static const DeviceVector& getVirtexDevices(void);
	//		static const DeviceVector& getVirtexEDevices(void);
	//		static const DeviceVector& getVirtex2Devices(void);
	//		static const DeviceVector& getVirtex2PDevices(void);
	//		static const DeviceVector& getVirtex4Devices(void);
	//		static const DeviceVector& getVirtex5Devices(void);
	//		static const DeviceVector& getVirtex6Devices(void);
	//		static const DeviceVector& getVirtex6LDevices(void);
	//		static const DeviceVector& getVirtex7Devices(void);
	//		static const DeviceVector& getVirtex7LDevices(void);
	//		static const DeviceVector& getKintex7Devices(void);
	//		static const DeviceVector& getKintex7LDevices(void);
	//		static const DeviceVector& getSpartan3EDevices(void);
	//		static const DeviceVector& getSpartan6Devices(void);
	//		static const DeviceVector& getSpartan6LDevices(void);

	// members tested:
	//		static Devices sDevices;
	//		DeviceVector mSupportedDevices;
	// functions tested:
	//		Devices(void);
	//		void addToSupportedDevices(const DeviceVector& inDeviceVector);
	//		static const DeviceVector& getSupportedDevices(void);
	BOOST_CHECK(Devices::getSupportedDevices().size() > 0);

	// members tested:
	//		enum EDevice;
	//		DeviceNameMap mDeviceNameMap;
	// functions tested: 
	//		static EDevice getDeviceEnum(const string& inName);
	//		static const DeviceNameMap& getDeviceNameMap(void);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv50"), eXCV50);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv100"), eXCV100);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv150"), eXCV150);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv200"), eXCV200);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv300"), eXCV300);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv400"), eXCV400);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv600"), eXCV600);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv800"), eXCV800);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv1000"), eXCV1000);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv50e"), eXCV50E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv100e"), eXCV100E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv200e"), eXCV200E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv300e"), eXCV300E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv400e"), eXCV400E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv405e"), eXCV405E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv600e"), eXCV600E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv812e"), eXCV812E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv1000e"), eXCV1000E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv1600e"), eXCV1600E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv2000e"), eXCV2000E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv2600e"), eXCV2600E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xcv3200e"), eXCV3200E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2v40"), eXC2V40);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2v80"), eXC2V80);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2v250"), eXC2V250);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2v500"), eXC2V500);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2v1000"), eXC2V1000);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2v1500"), eXC2V1500);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2v2000"), eXC2V2000);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2v3000"), eXC2V3000);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2v4000"), eXC2V4000);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2v6000"), eXC2V6000);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2v8000"), eXC2V8000);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2vp2"), eXC2VP2);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2vp4"), eXC2VP4);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2vp7"), eXC2VP7);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2vp20"), eXC2VP20);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2vp30"), eXC2VP30);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2vp40"), eXC2VP40);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2vp50"), eXC2VP50);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2vp70"), eXC2VP70);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2vp100"), eXC2VP100);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2vpx20"), eXC2VPX20);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc2vpx70"), eXC2VPX70);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vfx12"), eXC4VFX12);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vfx20"), eXC4VFX20);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vfx40"), eXC4VFX40);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vfx60"), eXC4VFX60);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vfx100"), eXC4VFX100);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vfx140"), eXC4VFX140);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vlx15"), eXC4VLX15);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vlx25"), eXC4VLX25);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vlx40"), eXC4VLX40);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vlx60"), eXC4VLX60);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vlx80"), eXC4VLX80);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vlx100"), eXC4VLX100);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vlx160"), eXC4VLX160);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vlx200"), eXC4VLX200);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vsx25"), eXC4VSX25);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vsx35"), eXC4VSX35);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc4vsx55"), eXC4VSX55);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vfx30t"), eXC5VFX30T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vfx70t"), eXC5VFX70T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vfx100t"), eXC5VFX100T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vfx130t"), eXC5VFX130T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vfx200t"), eXC5VFX200T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vlx30"), eXC5VLX30);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vlx50"), eXC5VLX50);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vlx85"), eXC5VLX85);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vlx110"), eXC5VLX110);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vlx155"), eXC5VLX155);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vlx220"), eXC5VLX220);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vlx330"), eXC5VLX330);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vlx20t"), eXC5VLX20T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vlx30t"), eXC5VLX30T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vlx50t"), eXC5VLX50T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vlx85t"), eXC5VLX85T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vlx110t"), eXC5VLX110T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vlx155t"), eXC5VLX155T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vlx220t"), eXC5VLX220T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vlx330t"), eXC5VLX330T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vsx35t"), eXC5VSX35T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vsx50t"), eXC5VSX50T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vsx95t"), eXC5VSX95T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vsx240t"), eXC5VSX240T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vtx150t"), eXC5VTX150T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc5vtx240t"), eXC5VTX240T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vcx75t"), eXC6VCX75T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vcx130t"), eXC6VCX130T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vcx195t"), eXC6VCX195T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vcx240t"), eXC6VCX240T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vhx250t"), eXC6VHX250T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vhx255t"), eXC6VHX255T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vhx380t"), eXC6VHX380T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vhx565t"), eXC6VHX565T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vlx75t"), eXC6VLX75T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vlx130t"), eXC6VLX130T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vlx195t"), eXC6VLX195T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vlx240t"), eXC6VLX240T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vlx365t"), eXC6VLX365T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vlx550t"), eXC6VLX550T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vlx760"), eXC6VLX760);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vsx315t"), eXC6VSX315T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vsx475t"), eXC6VSX475T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vlx75tl"), eXC6VLX75TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vlx130tl"), eXC6VLX130TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vlx195tl"), eXC6VLX195TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vlx240tl"), eXC6VLX240TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vlx365tl"), eXC6VLX365TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vlx550tl"), eXC6VLX550TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vlx760l"), eXC6VLX760L);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vsx315tl"), eXC6VSX315TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6vsx475tl"), eXC6VSX475TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7v285t"), eXC7V285T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7v450t"), eXC7V450T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7v585t"), eXC7V585T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7v855t"), eXC7V855T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7v1500t"), eXC7V1500T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7v2000t"), eXC7V2000T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7vx485t"), eXC7VX485T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7v285tl"), eXC7V285TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7v450tl"), eXC7V450TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7v585tl"), eXC7V585TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7v855tl"), eXC7V855TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7v1500tl"), eXC7V1500TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7v2000tl"), eXC7V2000TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7k30t"), eXC7K30T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7k70t"), eXC7K70T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7k160t"), eXC7K160T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7k325t"), eXC7K325T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7k410t"), eXC7K410T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7k30tl"), eXC7K30TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7k70tl"), eXC7K70TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7k160tl"), eXC7K160TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7k325tl"), eXC7K325TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc7k410tl"), eXC7K410TL);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc3s100e"), eXC3S100E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc3s250e"), eXC3S250E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc3s500e"), eXC3S500E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc3s1200e"), eXC3S1200E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc3s1600e"), eXC3S1600E);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx4"), eXC6SLX4);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx9"), eXC6SLX9);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx16"), eXC6SLX16);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx25"), eXC6SLX25);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx45"), eXC6SLX45);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx75"), eXC6SLX75);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx100"), eXC6SLX100);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx150"), eXC6SLX150);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx25t"), eXC6SLX25T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx45t"), eXC6SLX45T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx75t"), eXC6SLX75T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx100t"), eXC6SLX100T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx150t"), eXC6SLX150T);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx4l"), eXC6SLX4L);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx9l"), eXC6SLX9L);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx16l"), eXC6SLX16L);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx25l"), eXC6SLX25L);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx45l"), eXC6SLX45L);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx75l"), eXC6SLX75L);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx100l"), eXC6SLX100L);
	BOOST_CHECK_EQUAL(Devices::getDeviceEnum("xc6slx150l"), eXC6SLX150L);

	// members tested:
	//		DeviceVector mDeviceNames;
	// functions tested: 
	//		static const string& getDeviceName(EDevice inEnum);
	//		static const DeviceVector& getDeviceNames(void);
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV50), "xcv50");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV100), "xcv100");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV150), "xcv150");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV200), "xcv200");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV300), "xcv300");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV400), "xcv400");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV600), "xcv600");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV800), "xcv800");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV1000), "xcv1000");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV50E), "xcv50e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV100E), "xcv100e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV200E), "xcv200e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV300E), "xcv300e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV400E), "xcv400e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV405E), "xcv405e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV600E), "xcv600e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV812E), "xcv812e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV1000E), "xcv1000e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV1600E), "xcv1600e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV2000E), "xcv2000e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV2600E), "xcv2600e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXCV3200E), "xcv3200e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2V40), "xc2v40");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2V80), "xc2v80");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2V250), "xc2v250");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2V500), "xc2v500");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2V1000), "xc2v1000");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2V1500), "xc2v1500");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2V2000), "xc2v2000");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2V3000), "xc2v3000");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2V4000), "xc2v4000");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2V6000), "xc2v6000");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2V8000), "xc2v8000");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2VP2), "xc2vp2");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2VP4), "xc2vp4");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2VP7), "xc2vp7");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2VP20), "xc2vp20");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2VP30), "xc2vp30");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2VP40), "xc2vp40");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2VP50), "xc2vp50");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2VP70), "xc2vp70");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2VP100), "xc2vp100");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2VPX20), "xc2vpx20");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC2VPX70), "xc2vpx70");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VFX12), "xc4vfx12");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VFX20), "xc4vfx20");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VFX40), "xc4vfx40");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VFX60), "xc4vfx60");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VFX100), "xc4vfx100");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VFX140), "xc4vfx140");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VLX15), "xc4vlx15");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VLX25), "xc4vlx25");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VLX40), "xc4vlx40");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VLX60), "xc4vlx60");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VLX80), "xc4vlx80");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VLX100), "xc4vlx100");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VLX160), "xc4vlx160");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VLX200), "xc4vlx200");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VSX25), "xc4vsx25");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VSX35), "xc4vsx35");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC4VSX55), "xc4vsx55");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VFX30T), "xc5vfx30t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VFX70T), "xc5vfx70t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VFX100T), "xc5vfx100t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VFX130T), "xc5vfx130t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VFX200T), "xc5vfx200t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VLX30), "xc5vlx30");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VLX50), "xc5vlx50");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VLX85), "xc5vlx85");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VLX110), "xc5vlx110");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VLX155), "xc5vlx155");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VLX220), "xc5vlx220");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VLX330), "xc5vlx330");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VLX20T), "xc5vlx20t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VLX30T), "xc5vlx30t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VLX50T), "xc5vlx50t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VLX85T), "xc5vlx85t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VLX110T), "xc5vlx110t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VLX155T), "xc5vlx155t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VLX220T), "xc5vlx220t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VLX330T), "xc5vlx330t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VSX35T), "xc5vsx35t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VSX50T), "xc5vsx50t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VSX95T), "xc5vsx95t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VSX240T), "xc5vsx240t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VTX150T), "xc5vtx150t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC5VTX240T), "xc5vtx240t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VCX75T), "xc6vcx75t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VCX130T), "xc6vcx130t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VCX195T), "xc6vcx195t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VCX240T), "xc6vcx240t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VHX250T), "xc6vhx250t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VHX255T), "xc6vhx255t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VHX380T), "xc6vhx380t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VHX565T), "xc6vhx565t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VLX75T), "xc6vlx75t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VLX130T), "xc6vlx130t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VLX195T), "xc6vlx195t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VLX240T), "xc6vlx240t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VLX365T), "xc6vlx365t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VLX550T), "xc6vlx550t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VLX760), "xc6vlx760");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VSX315T), "xc6vsx315t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VSX475T), "xc6vsx475t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VLX75TL), "xc6vlx75tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VLX130TL), "xc6vlx130tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VLX195TL), "xc6vlx195tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VLX240TL), "xc6vlx240tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VLX365TL), "xc6vlx365tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VLX550TL), "xc6vlx550tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VLX760L), "xc6vlx760l");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VSX315TL), "xc6vsx315tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6VSX475TL), "xc6vsx475tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7V285T), "xc7v285t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7V450T), "xc7v450t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7V585T), "xc7v585t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7V855T), "xc7v855t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7V1500T), "xc7v1500t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7V2000T), "xc7v2000t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7VX485T), "xc7vx485t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7V285TL), "xc7v285tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7V450TL), "xc7v450tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7V585TL), "xc7v585tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7V855TL), "xc7v855tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7V1500TL), "xc7v1500tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7V2000TL), "xc7v2000tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7K30T), "xc7k30t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7K70T), "xc7k70t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7K160T), "xc7k160t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7K325T), "xc7k325t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7K410T), "xc7k410t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7K30TL), "xc7k30tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7K70TL), "xc7k70tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7K160TL), "xc7k160tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7K325TL), "xc7k325tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC7K410TL), "xc7k410tl");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC3S100E), "xc3s100e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC3S250E), "xc3s250e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC3S500E), "xc3s500e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC3S1200E), "xc3s1200e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC3S1600E), "xc3s1600e");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX4), "xc6slx4");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX9), "xc6slx9");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX16), "xc6slx16");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX25), "xc6slx25");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX45), "xc6slx45");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX75), "xc6slx75");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX100), "xc6slx100");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX150), "xc6slx150");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX25T), "xc6slx25t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX45T), "xc6slx45t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX75T), "xc6slx75t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX100T), "xc6slx100t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX150T), "xc6slx150t");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX4L), "xc6slx4l");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX9L), "xc6slx9l");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX16L), "xc6slx16l");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX25L), "xc6slx25l");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX45L), "xc6slx45l");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX75L), "xc6slx75l");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX100L), "xc6slx100l");
	BOOST_CHECK_EQUAL(Devices::getDeviceName(eXC6SLX150L), "xc6slx150l");

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace common
} // namespace torc
