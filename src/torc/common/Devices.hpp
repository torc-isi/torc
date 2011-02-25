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

#include <string>
#include <vector>

namespace torc {
namespace common {

	/// \brief Vector of defice names.
//	typedef std::vector<std::string> DeviceVector;
	class DeviceVector : public std::vector<std::string> {
	public:
		DeviceVector& operator<< (const std::string& rhs) { push_back(rhs); return *this; }
	};

	/// \brief Encapsulation of filesystem paths that are used by the library.
	class Devices {
	protected:
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
		/// \brief Spartan3E devices.
		DeviceVector mSpartan3EDevices;
		/// \brief Spartan6 devices.
		DeviceVector mSpartan6Devices;
		/// \brief Spartan6L devices.
		DeviceVector mSpartan6LDevices;
		/// \brief All supported devices.
		DeviceVector mAllDevices;
		/// \brief Static instance of this class.
		static Devices sDevices;
	// functions
		void addToAllDevices(const DeviceVector& inDeviceVector) {
			mAllDevices.insert(mAllDevices.end(), inDeviceVector.begin(), inDeviceVector.end());
		}
	public:
	// constructors
		/// Default constructor.
		Devices(void);
	// static accessors
		/// brief Returns the Virtex devices.
		static const DeviceVector& getVirtexDevices(void) { return sDevices.mVirtexDevices; }
		/// brief Returns the VirtexE devices.
		static const DeviceVector& getVirtexEDevices(void) { return sDevices.mVirtexEDevices; }
		/// brief Returns the Virtex2 devices.
		static const DeviceVector& getVirtex2Devices(void) { return sDevices.mVirtex2Devices; }
		/// brief Returns the Virtex2P devices.
		static const DeviceVector& getVirtex2PDevices(void) { return sDevices.mVirtex2PDevices; }
		/// brief Returns the Virtex4 devices.
		static const DeviceVector& getVirtex4Devices(void) { return sDevices.mVirtex4Devices; }
		/// brief Returns the Virtex5 devices.
		static const DeviceVector& getVirtex5Devices(void) { return sDevices.mVirtex5Devices; }
		/// brief Returns the Virtex6 devices.
		static const DeviceVector& getVirtex6Devices(void) { return sDevices.mVirtex6Devices; }
		/// brief Returns the Virtex6L devices.
		static const DeviceVector& getVirtex6LDevices(void) { return sDevices.mVirtex6LDevices; }
		/// brief Returns the Spartan3E devices.
		static const DeviceVector& getSpartan3EDevices(void) { return sDevices.mSpartan3EDevices; }
		/// brief Returns the Spartan6 devices.
		static const DeviceVector& getSpartan6Devices(void) { return sDevices.mSpartan6Devices; }
		/// brief Returns the Spartan6L devices.
		static const DeviceVector& getSpartan6LDevices(void) { return sDevices.mSpartan6LDevices; }
		/// brief Returns all devices.
		static const DeviceVector& getAllDevices(void) { return sDevices.mAllDevices; }

	};

} // namespace common
} // namespace torc

#endif // TORC_DEVICES_HPP
