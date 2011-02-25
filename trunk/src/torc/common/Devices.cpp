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
#include <iostream>

namespace torc {
namespace common {

	Devices Devices::sDevices;

	Devices::Devices(void) {
		mVirtexDevices 
			// Virtex
			<< "xcv50" << "xcv100" << "xcv150" << "xcv200" << "xcv300" << "xcv400" << "xcv600" 
			<< "xcv800" << "xcv1000";
		mVirtexEDevices
			// VirtexE
			<< "xcv50e" << "xcv100e" << "xcv200e" << "xcv300e" << "xcv400e" << "xcv405e" 
			<< "xcv600e" << "xcv812e" << "xcv1000e" << "xcv1600e" << "xcv2000e" << "xcv2600e" 
			<< "xcv3200e";
		mVirtex2Devices
			// Virtex2
			<< "xc2v40" << "xc2v80" << "xc2v250" << "xc2v500" << "xc2v1000" << "xc2v1500" 
			<< "xc2v2000" << "xc2v3000" << "xc2v4000" << "xc2v6000" << "xc2v8000";
		mVirtex2PDevices
			// Virtex2P
			<< "xc2vp2" << "xc2vp4" << "xc2vp7" << "xc2vp20" << "xc2vp30" << "xc2vp40" << "xc2vp50" 
			<< "xc2vp70" << "xc2vp100"
			// Virtex2P X
			<< "xc2vpx20" << "xc2vpx70";
		mVirtex4Devices
			// Virtex4 FX
			<< "xc4vfx12" << "xc4vfx20" << "xc4vfx40" << "xc4vfx60" << "xc4vfx100" << "xc4vfx140" 
			// Virtex4 LX
			<< "xc4vlx15" << "xc4vlx25" << "xc4vlx40" << "xc4vlx60" << "xc4vlx80" << "xc4vlx100" 
			<< "xc4vlx160" << "xc4vlx200"
			// Virtex4 SX
			<< "xc4vsx25" << "xc4vsx35" << "xc4vsx55";
		mVirtex5Devices
			// Virtex5 FXT
			<< "xc5vfx30t" << "xc5vfx70t" << "xc5vfx100t" << "xc5vfx130t" << "xc5vfx200t"
			// Virtex5 LX
			<< "xc5vlx30" << "xc5vlx50" << "xc5vlx85" << "xc5vlx110" << "xc5vlx155" << "xc5vlx220" 
			<< "xc5vlx330"
			// Virtex5 LXT
			<< "xc5vlx20t" << "xc5vlx30t" << "xc5vlx50t" << "xc5vlx85t" << "xc5vlx110t" 
			<< "xc5vlx155t" << "xc5vlx220t" << "xc5vlx330t"
			// Virtex5 SXT
			<< "xc5vsx35t" << "xc5vsx50t" << "xc5vsx95t" << "xc5vsx240t"
			// Virtex5 TXT
			<< "xc5vtx150t" << "xc5vtx240t";
		mVirtex6Devices
			// Virtex6 CXT
			<< "xc6vcx75t" << "xc6vcx130t" << "xc6vcx195t" << "xc6vcx240t"
			// Virtex6 HXT
			<< "xc6vhx250t" << "xc6vhx255t" << "xc6vhx380t" << "xc6vhx565t"
			// Virtex6 LXT
			<< "xc6vlx75t" << "xc6vlx130t" << "xc6vlx195t" << "xc6vlx240t" << "xc6vlx365t" 
			<< "xc6vlx550t" << "xc6vlx760"
			// Virtex6 SXT
			<< "xc6vsx315t" << "xc6vsx475t";
		mVirtex6LDevices
			// Virtex6L LXTL
			<< "xc6vlx75tl" << "xc6vlx130tl" << "xc6vlx195tl" << "xc6vlx240tl" << "xc6vlx365tl" 
			<< "xc6vlx550tl" << "xc6vlx760l"
			// Virtex6L SXTL
			<< "xc6vsx315tl" << "xc6vsx475tl";
		mSpartan3EDevices
			// Spartan3E
			<< "xc3s100e" << "xc3s250e" << "xc3s500e" << "xc3s1200e" << "xc3s1600e";
		mSpartan6Devices
			// Spartan6 LX
			<< "xc6slx4" << "xc6slx9" << "xc6slx16" << "xc6slx25" << "xc6slx45" << "xc6slx75" 
			<< "xc6slx100" << "xc6slx150"
			// Spartan6 LXT
			<< "xc6slx25t" << "xc6slx45t" << "xc6slx75t" << "xc6slx100t" << "xc6slx150t";
		mSpartan6LDevices
			// Spartan6L LXL
			<< "xc6slx4l" << "xc6slx9l" << "xc6slx16l" << "xc6slx25l" << "xc6slx45l" << "xc6slx75l" 
			<< "xc6slx100l" << "xc6slx150l";

		addToAllDevices(mVirtexDevices);
		addToAllDevices(mVirtexEDevices);
		addToAllDevices(mVirtex2Devices);
		addToAllDevices(mVirtex2PDevices);
		addToAllDevices(mVirtex4Devices);
		addToAllDevices(mVirtex5Devices);
		addToAllDevices(mVirtex6Devices);
		addToAllDevices(mVirtex6LDevices);
		addToAllDevices(mSpartan3EDevices);
		addToAllDevices(mSpartan6Devices);
		addToAllDevices(mSpartan6LDevices);
	}

} // namespace common
} // namespace torc
