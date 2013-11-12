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
/// \brief Header for the DeviceDesignator class.

#ifndef TORC_COMMON_DEVICEDESIGNATOR_HPP
#define TORC_COMMON_DEVICEDESIGNATOR_HPP

#include <boost/regex.hpp>
#include <ostream>
#include <string>

namespace torc {
namespace common {

	/// \brief Encapsulation of a device designator and its constituent elements.
	class DeviceDesignator {
	public:
	// enums
		enum EFamily { 
			// unknown families
			eFamilyUnknown = 0,
			// Spartan families
			eFamilySpartan2, eFamilySpartan2E, eFamilySpartan3, eFamilySpartan3A, eFamilySpartan3E, 
				eFamilySpartan6, 
			// Virtex families
			eFamilyVirtex, eFamilyVirtexE, eFamilyVirtex2, eFamilyVirtex2P, eFamilyVirtex4, 
				eFamilyVirtex5, eFamilyVirtex6, eFamilyArtix7, eFamilyKintex7, eFamilyVirtex7, 
				eFamilyZynq7000, 
			// number of defined families
			eFamilyCount
		};
	protected:
	// typedefs
		typedef std::string string; ///< \brief Imported type name.
	// members
		/// \brief The full device designator.
		string mDeviceDesignator;
		/// \brief The family type.
		EFamily mFamily;
		/// \brief The device name.
		string mDeviceName;
		/// \brief The device package.
		string mDevicePackage;
		/// \brief The device speed grade.
		string mDeviceSpeedGrade;
	// functions
		/// \brief Parses the device name into constituent device, package, and speed components.
		bool parse(const string& inDeviceDesignator, const boost::regex& inRegEx);
	// statics
		static boost::regex sSpartan2RegEx; ///< \brief Regular expression for Spartan2 devices.
		static boost::regex sSpartan2ERegEx; ///< \brief Regular expression for Spartan2E devices.
		static boost::regex sSpartan3RegEx; ///< \brief Regular expression for Spartan3 devices.
		static boost::regex sSpartan3ARegEx; ///< \brief Regular expression for Spartan3A devices.
		static boost::regex sSpartan3ERegEx; ///< \brief Regular expression for Spartan3E devices.
		static boost::regex sSpartan6RegEx; ///< \brief Regular expression for Spartan3E devices.
		static boost::regex sVirtexRegEx; ///< \brief Regular expression for Virtex devices.
		static boost::regex sVirtexERegEx; ///< \brief Regular expression for VirtexE devices.
		static boost::regex sVirtex2RegEx; ///< \brief Regular expression for Virtex2 devices.
		static boost::regex sVirtex2PRegEx; ///< \brief Regular expression for Virtex2P devices.
		static boost::regex sVirtex4RegEx; ///< \brief Regular expression for Virtex4 devices.
		static boost::regex sVirtex5RegEx; ///< \brief Regular expression for Virtex5 devices.
		static boost::regex sVirtex6RegEx; ///< \brief Regular expression for Virtex6 devices.
		static boost::regex sArtix7RegEx; ///< \brief Regular expression for Artix7 devices.
		static boost::regex sKintex7RegEx; ///< \brief Regular expression for Kintex7 devices.
		static boost::regex sVirtex7RegEx; ///< \brief Regular expression for Virtex7 devices.
		static boost::regex sZynq7000RegEx; ///< \brief Regular expression for Zynq7000 devices.
	public:
	// constructors
		/// \brief Basic constructor.
		DeviceDesignator(const string& inDeviceDesignator);
	// operators
		/// \brief Stream insertion operator.
		friend std::ostream& operator<< (std::ostream& os, const DeviceDesignator& rhs);
	// accessors
		/// \brief Returns the device designator.
		const string& getDeviceDesignator(void) const { return mDeviceDesignator; }
		/// \brief Returns the device family.
		const EFamily& getFamily(void) const { return mFamily; }
		/// \brief Returns the device name.
		const string& getDeviceName(void) const { return mDeviceName; }
		/// \brief Returns the device package.
		const string& getDevicePackage(void) const { return mDevicePackage; }
		/// \brief Returns the device speed grade.
		const string& getDeviceSpeedGrade(void) const { return mDeviceSpeedGrade; }
	};

} // namespace common
} // namespace torc

#endif // TORC_COMMON_DEVICEDESIGNATOR_HPP
