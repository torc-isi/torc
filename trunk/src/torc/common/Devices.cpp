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
/// \brief Source for the Devices class.

#include "torc/common/Devices.hpp"


#if 0
	#include <boost/assign/list_of.hpp>
	#include <boost/assign/list_inserter.hpp>
	#include <boost/bimap/bimap.hpp>
	#include <boost/bimap/multiset_of.hpp>
	#include <boost/bimap/list_of.hpp>

	using namespace boost::bimaps;
	using namespace boost;

	// an alternate approach based on Boost bimaps, but much heavier on object code
	void populate(void);
	void populate(void) {
		typedef bimap< multiset_of< EDevice >, list_of< std::string > > bm_type;
		bm_type bm = assign::list_of< bm_type::relation >
			(eXCV50, "xcv50") (eXCV100, "xcv100") (eXCV150, "xcv150") (eXCV200, "xcv200") 
			(eXCV300, "xcv300") (eXCV400, "xcv400") (eXCV600, "xcv600") (eXCV800, "xcv800") 
			(eXCV1000, "xcv1000");
	}
#endif


namespace torc {
namespace common {

	/// \brief Helper class to initialize device map and list information.
	class DeviceFamilyHelper {
	protected:
	// friends
		/// \brief Only the Devices class has access to our internals.
		friend class Devices;
	// typedefs
		/// \brief Imported name.
		typedef std::string string;
	// members
		/// \brief The map from device name to device enumeration.
		DeviceNameMap& mDeviceMap;
		/// \brief The list of devices for a particular family.
		DeviceVector& mDeviceVector;
		/// \brief The list of devices for all families.
		DeviceVector& mCombinedDeviceVector;
		/// \brief Convenience storage for the last inserted device name.
		static string sDevice;
	// constructors
		/// \brief Protected constructor.
		DeviceFamilyHelper(DeviceNameMap& inDeviceMap, DeviceVector& inDeviceVector, 
			DeviceVector& inCombinedDeviceVector) : mDeviceMap(inDeviceMap), 
			mDeviceVector(inDeviceVector), mCombinedDeviceVector(inCombinedDeviceVector) {}
	// operators
		/// \brief Convenience operator to retain the last device name inserted.
		DeviceFamilyHelper& operator<< (const string& rhs) { sDevice = rhs; return *this; }
		/// \brief Operator to propagate an enumeration and the previously inserted device name to 
		///		the device map and lists.
		DeviceFamilyHelper& operator<< (enum EDevice rhs) {
			mDeviceMap[sDevice] = rhs;
			mDeviceVector.push_back(sDevice);
			mCombinedDeviceVector[rhs] = sDevice;
			return *this;
		}
	};

	std::string DeviceFamilyHelper::sDevice;
	Devices Devices::sDevices;

	Devices::Devices(void) {
		// preinitialize the device vector size
		mDeviceNames.resize(eDeviceCount);
		// Virtex
		DeviceFamilyHelper virtexHelper(mDeviceNameMap, mVirtexDevices, mDeviceNames);
		virtexHelper
			// Virtex
			<< "xcv50" << eXCV50 << "xcv100" << eXCV100 << "xcv150" << eXCV150 
			<< "xcv200" << eXCV200 << "xcv300" << eXCV300 << "xcv400" << eXCV400 
			<< "xcv600" << eXCV600 << "xcv800" << eXCV800 << "xcv1000" << eXCV1000;
		// VirtexE
		DeviceFamilyHelper virtexEHelper(mDeviceNameMap, mVirtexEDevices, mDeviceNames);
		virtexEHelper
			// VirtexE
			<< "xcv50e" << eXCV50E << "xcv100e" << eXCV100E << "xcv200e" << eXCV200E 
			<< "xcv300e" << eXCV300E << "xcv400e" << eXCV400E << "xcv405e" << eXCV405E 
			<< "xcv600e" << eXCV600E << "xcv812e" << eXCV812E << "xcv1000e" << eXCV1000E 
			<< "xcv1600e" << eXCV1600E << "xcv2000e" << eXCV2000E << "xcv2600e" << eXCV2600E 
			<< "xcv3200e" << eXCV3200E;
		// Virtex2
		DeviceFamilyHelper virtex2Helper(mDeviceNameMap, mVirtex2Devices, mDeviceNames);
		virtex2Helper
			// Virtex2
			<< "xc2v40" << eXC2V40 << "xc2v80" << eXC2V80 << "xc2v250" << eXC2V250 
			<< "xc2v500" << eXC2V500 << "xc2v1000" << eXC2V1000 << "xc2v1500" << eXC2V1500 
			<< "xc2v2000" << eXC2V2000 << "xc2v3000" << eXC2V3000 << "xc2v4000" << eXC2V4000 
			<< "xc2v6000" << eXC2V6000 << "xc2v8000" << eXC2V8000;
		// Virtex2P
		DeviceFamilyHelper virtex2PHelper(mDeviceNameMap, mVirtex2PDevices, mDeviceNames);
		virtex2PHelper
			// Virtex2P
			<< "xc2vp2" << eXC2VP2 << "xc2vp4" << eXC2VP4 << "xc2vp7" << eXC2VP7 
			<< "xc2vp20" << eXC2VP20 << "xc2vp30" << eXC2VP30 << "xc2vp40" << eXC2VP40 
			<< "xc2vp50" << eXC2VP50 << "xc2vp70" << eXC2VP70 << "xc2vp100" << eXC2VP100 
			// Virtex2P X
			<< "xc2vpx20" << eXC2VPX20 << "xc2vpx70" << eXC2VPX70;
		// Virtex4
		DeviceFamilyHelper virtex4Helper(mDeviceNameMap, mVirtex4Devices, mDeviceNames);
		virtex4Helper
			// Virtex4 FX
			<< "xc4vfx12" << eXC4VFX12 << "xc4vfx20" << eXC4VFX20 << "xc4vfx40" << eXC4VFX40 
			<< "xc4vfx60" << eXC4VFX60 << "xc4vfx100" << eXC4VFX100 << "xc4vfx140" << eXC4VFX140 
			// Virtex4 LX
			<< "xc4vlx15" << eXC4VLX15 << "xc4vlx25" << eXC4VLX25 << "xc4vlx40" << eXC4VLX40 
			<< "xc4vlx60" << eXC4VLX60 << "xc4vlx80" << eXC4VLX80 << "xc4vlx100" << eXC4VLX100 
			<< "xc4vlx160" << eXC4VLX160 << "xc4vlx200" << eXC4VLX200
			// Virtex4 SX
			<< "xc4vsx25" << eXC4VSX25 << "xc4vsx35" << eXC4VSX35 << "xc4vsx55" << eXC4VSX55;
		// Virtex5
		DeviceFamilyHelper virtex5Helper(mDeviceNameMap, mVirtex5Devices, mDeviceNames);
		virtex5Helper
			// Virtex5 FXT
			<< "xc5vfx30t" << eXC5VFX30T << "xc5vfx70t" << eXC5VFX70T 
			<< "xc5vfx100t" << eXC5VFX100T << "xc5vfx130t" << eXC5VFX130T 
			<< "xc5vfx200t" << eXC5VFX200T
			// Virtex5 LX
			<< "xc5vlx30" << eXC5VLX30 << "xc5vlx50" << eXC5VLX50 << "xc5vlx85" << eXC5VLX85 
			<< "xc5vlx110" << eXC5VLX110 << "xc5vlx155" << eXC5VLX155 << "xc5vlx220" << eXC5VLX220 
			<< "xc5vlx330" << eXC5VLX330
			// Virtex5 LXT
			<< "xc5vlx20t" << eXC5VLX20T << "xc5vlx30t" << eXC5VLX30T << "xc5vlx50t" << eXC5VLX50T 
			<< "xc5vlx85t" << eXC5VLX85T << "xc5vlx110t" << eXC5VLX110T 
			<< "xc5vlx155t" << eXC5VLX155T << "xc5vlx220t" << eXC5VLX220T 
			<< "xc5vlx330t" << eXC5VLX330T
			// Virtex5 SXT
			<< "xc5vsx35t" << eXC5VSX35T << "xc5vsx50t" << eXC5VSX50T << "xc5vsx95t" << eXC5VSX95T 
			<< "xc5vsx240t" << eXC5VSX240T
			// Virtex5 TXT
			<< "xc5vtx150t" << eXC5VTX150T << "xc5vtx240t" << eXC5VTX240T;
		// Virtex6
		DeviceFamilyHelper virtex6Helper(mDeviceNameMap, mVirtex6Devices, mDeviceNames);
		virtex6Helper
			// Virtex6 CXT
			<< "xc6vcx75t" << eXC6VCX75T << "xc6vcx130t" << eXC6VCX130T 
			<< "xc6vcx195t" << eXC6VCX195T << "xc6vcx240t" << eXC6VCX240T
			// Virtex6 HXT
			<< "xc6vhx250t" << eXC6VHX250T << "xc6vhx255t" << eXC6VHX255T 
			<< "xc6vhx380t" << eXC6VHX380T << "xc6vhx565t" << eXC6VHX565T
			// Virtex6 LXT
			<< "xc6vlx75t" << eXC6VLX75T << "xc6vlx130t" << eXC6VLX130T 
			<< "xc6vlx195t" << eXC6VLX195T << "xc6vlx240t" << eXC6VLX240T 
			<< "xc6vlx365t" << eXC6VLX365T << "xc6vlx550t" << eXC6VLX550T 
			<< "xc6vlx760" << eXC6VLX760
			// Virtex6 SXT
			<< "xc6vsx315t" << eXC6VSX315T << "xc6vsx475t" << eXC6VSX475T;
		// Virtex6L
		DeviceFamilyHelper virtex6LHelper(mDeviceNameMap, mVirtex6Devices, mDeviceNames);
		virtex6LHelper
			// Virtex6L LXTL
			<< "xc6vlx75tl" << eXC6VLX75TL << "xc6vlx130tl" << eXC6VLX130TL 
			<< "xc6vlx195tl" << eXC6VLX195TL << "xc6vlx240tl" << eXC6VLX240TL 
			<< "xc6vlx365tl" << eXC6VLX365TL << "xc6vlx550tl" << eXC6VLX550TL 
			<< "xc6vlx760l" << eXC6VLX760L
			// Virtex6L SXTL
			<< "xc6vsx315tl" << eXC6VSX315TL << "xc6vsx475tl" << eXC6VSX475TL;
		// Virtex7
		DeviceFamilyHelper virtex7Helper(mDeviceNameMap, mVirtex7Devices, mDeviceNames);
		virtex7Helper
			// Virtex7 T
			<< "xc7v285t" << eXC7V285T << "xc7v450t" << eXC7V450T << "xc7v585t" << eXC7V585T 
			<< "xc7v855t" << eXC7V855T << "xc7v1500t" << eXC7V1500T << "xc7v2000t" << eXC7V2000T
			// Virtex7 XT
			<< "xc7vx485t" << eXC7VX485T;
		// Virtex7
		DeviceFamilyHelper virtex7LHelper(mDeviceNameMap, mVirtex7LDevices, mDeviceNames);
		virtex7LHelper
			// Virtex7 TL
			<< "xc7v285tl" << eXC7V285TL << "xc7v450tl" << eXC7V450TL << "xc7v585tl" << eXC7V585TL 
			<< "xc7v855tl" << eXC7V855TL << "xc7v1500tl" << eXC7V1500TL 
			<< "xc7v2000tl" << eXC7V2000TL;
		// Kintex7
		DeviceFamilyHelper kintex7Helper(mDeviceNameMap, mKintex7Devices, mDeviceNames);
		kintex7Helper
			// Kintex7 T
			<< "xc7k30t" << eXC7K30T << "xc7k70t" << eXC7K70T << "xc7k160t" << eXC7K160T 
			<< "xc7k325t" << eXC7K325T << "xc7k410t" << eXC7K410T;
		// Kintex7L
		DeviceFamilyHelper kintex7LHelper(mDeviceNameMap, mKintex7LDevices, mDeviceNames);
		kintex7LHelper
			// Kintex7 TL
			<< "xc7k30tl" << eXC7K30TL << "xc7k70tl" << eXC7K70TL << "xc7k160tl" << eXC7K160TL 
			<< "xc7k325tl" << eXC7K325TL << "xc7k410tl" << eXC7K410TL;
		// Spartan3E
		DeviceFamilyHelper spartan3EHelper(mDeviceNameMap, mSpartan3EDevices, mDeviceNames);
		spartan3EHelper
			// Spartan3E
			<< "xc3s100e" << eXC3S100E << "xc3s250e" << eXC3S250E << "xc3s500e" << eXC3S500E 
			<< "xc3s1200e" << eXC3S1200E << "xc3s1600e" << eXC3S1600E;
		// Spartan6
		DeviceFamilyHelper spartan6Helper(mDeviceNameMap, mSpartan6Devices, mDeviceNames);
		spartan6Helper
			// Spartan6 LX
			<< "xc6slx4" << eXC6SLX4 << "xc6slx9" << eXC6SLX9 << "xc6slx16" << eXC6SLX16 
			<< "xc6slx25" << eXC6SLX25 << "xc6slx45" << eXC6SLX45 << "xc6slx75" << eXC6SLX75 
			<< "xc6slx100" << eXC6SLX100 << "xc6slx150" << eXC6SLX150
			// Spartan6 LXT
			<< "xc6slx25t" << eXC6SLX25T << "xc6slx45t" << eXC6SLX45T << "xc6slx75t" << eXC6SLX75T 
			<< "xc6slx100t" << eXC6SLX100T << "xc6slx150t" << eXC6SLX150T;
		// Spartan6L
		DeviceFamilyHelper spartan6LHelper(mDeviceNameMap, mSpartan6LDevices, mDeviceNames);
		spartan6LHelper
			// Spartan6L LXL
			<< "xc6slx4l" << eXC6SLX4L << "xc6slx9l" << eXC6SLX9L << "xc6slx16l" << eXC6SLX16L 
			<< "xc6slx25l" << eXC6SLX25L << "xc6slx45l" << eXC6SLX45L << "xc6slx75l" << eXC6SLX75L 
			<< "xc6slx100l" << eXC6SLX100L << "xc6slx150l" << eXC6SLX150L;

		// add all of the devices that are currently supported
		addToSupportedDevices(mVirtexDevices);
		addToSupportedDevices(mVirtexEDevices);
		addToSupportedDevices(mVirtex2Devices);
		addToSupportedDevices(mVirtex2PDevices);
		addToSupportedDevices(mVirtex4Devices);
		addToSupportedDevices(mVirtex5Devices);
		addToSupportedDevices(mVirtex6Devices);
		addToSupportedDevices(mVirtex6LDevices);
		addToSupportedDevices(mVirtex7Devices);
//		addToSupportedDevices(mVirtex7LDevices);
		addToSupportedDevices(mKintex7Devices);
//		addToSupportedDevices(mKintex7LDevices);
		addToSupportedDevices(mSpartan3EDevices);
		addToSupportedDevices(mSpartan6Devices);
		addToSupportedDevices(mSpartan6LDevices);

		// add a subset of devices suitable for basic unit tests
		mUnitTestDevices.push_back(mVirtexDevices.front());
		mUnitTestDevices.push_back(mVirtexEDevices.front());
		mUnitTestDevices.push_back(mVirtex2Devices.front());
		mUnitTestDevices.push_back(mVirtex2PDevices.front());
		mUnitTestDevices.push_back(mVirtex4Devices.front());
		mUnitTestDevices.push_back(mVirtex5Devices.front());
		mUnitTestDevices.push_back(mVirtex6Devices.front());
		//mUnitTestDevices.push_back(mVirtex6LDevices.front()); // included in Virtex6
		mUnitTestDevices.push_back(mVirtex7Devices.front());
		//mUnitTestDevices.push_back(mVirtex7LDevices.front()); // included in Virtex7
		mUnitTestDevices.push_back(mKintex7Devices.front());
		//mUnitTestDevices.push_back(mKintex7LDevices.front()); // included in Kintex7
		mUnitTestDevices.push_back(mSpartan3EDevices.front());
		mUnitTestDevices.push_back(mSpartan6Devices.front());
		//mUnitTestDevices.push_back(mSpartan6LDevices.front()); // included in Spartan6
	}

} // namespace common
} // namespace torc
