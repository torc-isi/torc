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
/// \brief Header for the VirtexE class.

#ifndef TORC_BITSTREAM_VIRTEXE_HPP
#define TORC_BITSTREAM_VIRTEXE_HPP

#include <boost/cstdint.hpp>
#include "torc/bitstream/Virtex.hpp"

namespace torc { namespace architecture { class DDB; } }

namespace torc {
namespace bitstream {

namespace bitstream { class VirtexEUnitTest; }
namespace bitstream { class VirtexEFarUnitTest; }
namespace bitstream { void testVirtexEDevice(const std::string& inDeviceName, 
	const boost::filesystem::path& inWorkingPath); }
namespace bitstream { void testVirtexEFullMapping(const boost::filesystem::path& inWorkingPath); }

	/// \brief VirtexE bitstream inherited from Virtex bitstream.
	class VirtexE : public Virtex {
		friend class torc::bitstream::bitstream::VirtexEUnitTest;
		friend class torc::bitstream::bitstream::VirtexEFarUnitTest;
		friend void torc::bitstream::bitstream::testVirtexEDevice(const std::string& inDeviceName, 
			const boost::filesystem::path& inWorkingPath);
		friend void torc::bitstream::bitstream::testVirtexEFullMapping(const boost::filesystem::path& inWorkingPath);
	  public:
	// functions
		/// \brief Initialize the device information.
		virtual void initializeDeviceInfo(const std::string& inDeviceName);
		/// \brief Initialize the maps between frame indexes and frame addresses.
		/// \detail This is generally only useful for internal purposes.
		virtual void initializeFrameMaps(void);
	// accessors
		virtual uint32_t getFrameLength(void) const {
			using namespace torc::common;
			// Frame Length Register Value: XAPP151, v.1.7, October 20, 2004, Table 24.
			switch(mDevice) {
				case eXCV50E: return 12;
				case eXCV100E: return 14;
				case eXCV200E: return 18;
				case eXCV300E: return 21;
				case eXCV400E: return 25;
				case eXCV405E: return 25;
				case eXCV600E: return 30;
				case eXCV812E: return 34;
				case eXCV1000E: return 39;
				case eXCV1600E: return 43;
				case eXCV2000E: return 48;
				case eXCV2600E: return 54;
				case eXCV3200E: return 61;
				default: return 0;
			}
		}
		/// \brief Return the number of frame rows for the current device.
		virtual uint32_t getFrameRowCount(void) const { return 0; }
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_VIRTEXE_HPP
