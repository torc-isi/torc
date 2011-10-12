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
/// \brief Header for the Devices class.

#ifndef TORC_DEVICES_HPP
#define TORC_DEVICES_HPP

#include "torc/architecture/DeviceDesignator.hpp"
#include <boost/algorithm/string.hpp>    
#include <string>
#include <vector>
#include <map>

namespace torc {
namespace common {

	/// \brief Enumeration of all supported devices.
	enum EDevice {
		eDeviceInvalid = 0,
	// Spartan3E
		// Spartan3E
		eXC3S100E, eXC3S250E, eXC3S500E, eXC3S1200E, eXC3S1600E, 
	// Spartan6
		// Spartan6 LX
		eXC6SLX4, eXC6SLX9, eXC6SLX16, eXC6SLX25, eXC6SLX45, eXC6SLX75, 
		eXC6SLX100, eXC6SLX150, 
		// Spartan6 LXT
		eXC6SLX25T, eXC6SLX45T, eXC6SLX75T, eXC6SLX100T, eXC6SLX150T, 
		// Spartan6 LXL
		eXC6SLX4L, eXC6SLX9L, eXC6SLX16L, eXC6SLX25L, eXC6SLX45L, eXC6SLX75L, 
		eXC6SLX100L, eXC6SLX150L, 
	// Virtex
		// Virtex
		eXCV50, eXCV100, eXCV150, eXCV200, eXCV300, eXCV400, eXCV600, eXCV800, eXCV1000, 
	// VirtexE
		// VirtexE
		eXCV50E, eXCV100E, eXCV200E, eXCV300E, eXCV400E, eXCV600E, eXCV1000E, eXCV1600E, 
		eXCV2000E, eXCV2600E, eXCV3200E, 
		// VirtexEM
		eXCV405E, eXCV812E, 
	// Virtex2
		// Virtex2
		eXC2V40, eXC2V80, eXC2V250, eXC2V500, eXC2V1000, eXC2V1500, eXC2V2000, eXC2V3000, 
		eXC2V4000, eXC2V6000, eXC2V8000, 
	// Virtex2P
		// Virtex2P
		eXC2VP2, eXC2VP4, eXC2VP7, eXC2VP20, eXC2VP30, eXC2VP40, eXC2VP50, eXC2VP70, eXC2VP100, 
		// Virtex2P X
		eXC2VPX20, eXC2VPX70, 
	// Virtex4
		// Virtex4 FX
		eXC4VFX12, eXC4VFX20, eXC4VFX40, eXC4VFX60, eXC4VFX100, eXC4VFX140, 
		// Virtex4 LX
		eXC4VLX15, eXC4VLX25, eXC4VLX40, eXC4VLX60, eXC4VLX80, eXC4VLX100, eXC4VLX160, 
		eXC4VLX200, 
		// Virtex4 SX
		eXC4VSX25, eXC4VSX35, eXC4VSX55, 
	// Virtex5
		// Virtex5 FXT
		eXC5VFX30T, eXC5VFX70T, eXC5VFX100T, eXC5VFX130T, eXC5VFX200T, 
		// Virtex5 LX
		eXC5VLX30, eXC5VLX50, eXC5VLX85, eXC5VLX110, eXC5VLX155, eXC5VLX220, eXC5VLX330, 
		// Virtex5 LXT
		eXC5VLX20T, eXC5VLX30T, eXC5VLX50T, eXC5VLX85T, eXC5VLX110T, eXC5VLX155T, eXC5VLX220T, 
		eXC5VLX330T, 
		// Virtex5 SXT
		eXC5VSX35T, eXC5VSX50T, eXC5VSX95T, eXC5VSX240T, 
		// Virtex5 TXT
		eXC5VTX150T, eXC5VTX240T, 
	// Virtex6
		// Virtex6 CXT
		eXC6VCX75T, eXC6VCX130T, eXC6VCX195T, eXC6VCX240T, 
		// Virtex6 HXT
		eXC6VHX250T, eXC6VHX255T, eXC6VHX380T, eXC6VHX565T, 
		// Virtex6 LXT
		eXC6VLX75T, eXC6VLX130T, eXC6VLX195T, eXC6VLX240T, eXC6VLX365T, eXC6VLX550T, 
		eXC6VLX760, 
		// Virtex6 SXT
		eXC6VSX315T, eXC6VSX475T, 
	// Virtex6L
		// Virtex6 LXTL
		eXC6VLX75TL, eXC6VLX130TL, eXC6VLX195TL, eXC6VLX240TL, eXC6VLX365TL, eXC6VLX550TL, 
		eXC6VLX760L, 
		// Virtex6 SXTL
		eXC6VSX315TL, eXC6VSX475TL, 
	// Virtex7
		// Virtex7 T
		eXC7V285T, eXC7V450T, eXC7V585T, eXC7V855T, eXC7V1500T, eXC7V2000T,
		// Virtex7 XT
		eXC7VX485T, 
	// Virtex7L
		// Virtex7 TL
		eXC7V285TL, eXC7V450TL, eXC7V585TL, eXC7V855TL, eXC7V1500TL, 
		eXC7V2000TL, 
	// Kintex7
		// Kintex7 T
		eXC7K30T, eXC7K70T, eXC7K160T, eXC7K325T, eXC7K410T, 
	// Kintex7L
		// Kintex7 TL
		eXC7K30TL, eXC7K70TL, eXC7K160TL, eXC7K325TL, eXC7K410TL, 
		// end
		eDeviceCount
	};

	/// \brief Vector of device names.
	typedef std::vector<std::string> DeviceVector;

	/// \brief Map of device names.
	typedef std::map<std::string, enum EDevice> DeviceNameMap;

	/// \brief Encapsulation of filesystem paths that are used by the library.
	class Devices {
	protected:
	// typedefs
		/// \brief Imported name.
		typedef std::string string;
	// members
		/// \brief Virtex devices.
		DeviceVector mVirtexDevices;
		/// \brief VirtexE devices.
		DeviceVector mVirtexEDevices;
		/// \brief Virtex2 devices.
		DeviceVector mVirtex2Devices;
		/// \brief Virtex2P devices.
		DeviceVector mVirtex2PDevices;
		/// \brief Virtex4 devices.
		DeviceVector mVirtex4Devices;
		/// \brief Virtex5 devices.
		DeviceVector mVirtex5Devices;
		/// \brief Virtex6 devices.
		DeviceVector mVirtex6Devices;
		/// \brief Virtex6L devices.
		DeviceVector mVirtex6LDevices;
		/// \brief Virtex7 devices.
		DeviceVector mVirtex7Devices;
		/// \brief Virtex7L devices.
		DeviceVector mVirtex7LDevices;
		/// \brief Kintex7 devices.
		DeviceVector mKintex7Devices;
		/// \brief Kintex7L devices.
		DeviceVector mKintex7LDevices;
		/// \brief Spartan3E devices.
		DeviceVector mSpartan3EDevices;
		/// \brief Spartan6 devices.
		DeviceVector mSpartan6Devices;
		/// \brief Spartan6L devices.
		DeviceVector mSpartan6LDevices;
		/// \brief All supported devices.
		DeviceVector mSupportedDevices;
		/// \brief Device names.
		DeviceVector mDeviceNames;
		/// \brief Device name to enum map.
		DeviceNameMap mDeviceNameMap;
		/// \brief Static instance of this class.
		static Devices sDevices;
	// functions
		/// \brief Add the specified devices to the list of all devices.
		void addToSupportedDevices(const DeviceVector& inDeviceVector) {
			mSupportedDevices.insert(mSupportedDevices.end(), inDeviceVector.begin(), 
				inDeviceVector.end());
		}
	public:
	// constructors
		/// Default constructor.
		Devices(void);
	// static accessors
		/// \brief Returns the Virtex devices.
		static const DeviceVector& getVirtexDevices(void) { return sDevices.mVirtexDevices; }
		/// \brief Returns the VirtexE devices.
		static const DeviceVector& getVirtexEDevices(void) { return sDevices.mVirtexEDevices; }
		/// \brief Returns the Virtex2 devices.
		static const DeviceVector& getVirtex2Devices(void) { return sDevices.mVirtex2Devices; }
		/// \brief Returns the Virtex2P devices.
		static const DeviceVector& getVirtex2PDevices(void) { return sDevices.mVirtex2PDevices; }
		/// \brief Returns the Virtex4 devices.
		static const DeviceVector& getVirtex4Devices(void) { return sDevices.mVirtex4Devices; }
		/// \brief Returns the Virtex5 devices.
		static const DeviceVector& getVirtex5Devices(void) { return sDevices.mVirtex5Devices; }
		/// \brief Returns the Virtex6 devices.
		static const DeviceVector& getVirtex6Devices(void) { return sDevices.mVirtex6Devices; }
		/// \brief Returns the Virtex6L devices.
		static const DeviceVector& getVirtex6LDevices(void) { return sDevices.mVirtex6LDevices; }
		/// \brief Returns the Virtex7 devices.
		static const DeviceVector& getVirtex7Devices(void) { return sDevices.mVirtex7Devices; }
		/// \brief Returns the Virtex7L devices.
		static const DeviceVector& getVirtex7LDevices(void) { return sDevices.mVirtex7LDevices; }
		/// \brief Returns the Kintex7 devices.
		static const DeviceVector& getKintex7Devices(void) { return sDevices.mKintex7Devices; }
		/// \brief Returns the Kintex7L devices.
		static const DeviceVector& getKintex7LDevices(void) { return sDevices.mKintex7LDevices; }
		/// \brief Returns the Spartan3E devices.
		static const DeviceVector& getSpartan3EDevices(void) { return sDevices.mSpartan3EDevices; }
		/// \brief Returns the Spartan6 devices.
		static const DeviceVector& getSpartan6Devices(void) { return sDevices.mSpartan6Devices; }
		/// \brief Returns the Spartan6L devices.
		static const DeviceVector& getSpartan6LDevices(void) { return sDevices.mSpartan6LDevices; }
		/// \brief Returns all devices.
		static const DeviceVector& getSupportedDevices(void) { return sDevices.mSupportedDevices; }
		/// \brief Returns all devices.
		static const DeviceVector& getDeviceNames(void) { return sDevices.mDeviceNames; }
		/// \brief Returns the device map.
		static const DeviceNameMap& getDeviceNameMap(void) { return sDevices.mDeviceNameMap; }
	// accessors
		/// \brief Returns the device enumeration corresponding to the given device name.
		static EDevice getDeviceEnum(const string& inName) {
			torc::architecture::DeviceDesignator designator(inName);
			std::string name = designator.getDeviceName();
			const DeviceNameMap& deviceNameMap = getDeviceNameMap();
			DeviceNameMap::const_iterator p = deviceNameMap.find(name);
			DeviceNameMap::const_iterator e = deviceNameMap.end();
			if(p == e) return eDeviceInvalid;
			return p->second;
		}
		/// \brief Returns the device name corresponding to the given enumeration constant.
		static const string& getDeviceName(EDevice inEnum) {
			//if(inEnum >= eDeviceCount) return 0;
			return getDeviceNames()[inEnum];
		}
	};

} // namespace common
} // namespace torc

#endif // TORC_DEVICES_HPP
