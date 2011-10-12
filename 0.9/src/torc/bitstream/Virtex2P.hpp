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
/// \brief Header for the Virtex2P class.

#ifndef TORC_BITSTREAM_VIRTEX2P_HPP
#define TORC_BITSTREAM_VIRTEX2P_HPP

#include <boost/cstdint.hpp>
#include "torc/bitstream/Virtex2.hpp"

namespace torc { namespace architecture { class DDB; } }

namespace torc {
namespace bitstream {

namespace bitstream { class Virtex2PUnitTest; }
namespace bitstream { class Virtex2PFarUnitTest; }
namespace bitstream { void testVirtex2PDevice(const std::string& inDeviceName, 
	const boost::filesystem::path& inWorkingPath); }
namespace bitstream { void testVirtex2PFullMapping(const boost::filesystem::path& inWorkingPath); }

	/// \brief Virtex2P bitstream inherited from Virtex2 bitstream.
	class Virtex2P : public Virtex2 {
		friend class torc::bitstream::bitstream::Virtex2PUnitTest;
		friend class torc::bitstream::bitstream::Virtex2PFarUnitTest;
		friend void torc::bitstream::bitstream::testVirtex2PDevice(const std::string& inDeviceName, 
			const boost::filesystem::path& inWorkingPath);
		friend void torc::bitstream::bitstream::testVirtex2PFullMapping(const boost::filesystem::path& inWorkingPath);
	  public:
	// functions
		/// \brief Initialize the device information.
		virtual void initializeDeviceInfo(const std::string& inDeviceName);
	// accessors
		virtual uint32_t getFrameLength(void) const {
			using namespace torc::common;
			// Frame Length Register Value: UG002, v2.2, November 5, 2007, Table 4-15.
			switch(mDevice) {
				case eXC2VP2: return 46;
				case eXC2VP4: return 106;
				case eXC2VP7: return 106;
				case eXC2VP20: return 146;
				case eXC2VPX20: return 146;
				case eXC2VP30: return 206;
				case eXC2VP40: return 226;
				case eXC2VP50: return 226;
				case eXC2VP70: return 266;
				case eXC2VPX70: return 266;
				case eXC2VP100: return 306;
				default: return 0;
			}
		}
	};

} // namespace bitstream
} // namespace torc // namespace torc

#endif // TORC_BITSTREAM_VIRTEX2P_HPP
