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
/// \brief Header for the DeviceSiteTypePin class.

#ifndef TORC_PLACER_DEVICESITETYPEPIN_HPP
#define TORC_PLACER_DEVICESITETYPEPIN_HPP

#include <string>

namespace torc {
namespace placer {

	class DeviceSiteTypePin {
	protected:
		std::string mName;
		
		int mRowOffset; // these offsets tell you where the pin actually lives relative to the tile index of the site
		int mColOffset;
		//int mTileOffset; // these offsets tell you where the pin actually lives relative to the tile index of the site
		
	public:
		DeviceSiteTypePin(std::string& inName, int inRowOffset, int inColOffset) : 
			mName(inName), mRowOffset(inRowOffset), mColOffset(inColOffset)
		//PlacerSiteTypePin(std::string& inName, int inTileOffset) : 
		//	mName(inName), mTileOffset(inTileOffset)
		{ }
		~DeviceSiteTypePin() {}
		std::string& getName() {
			return mName;
		}
		//int getOffset() {
		//	return mTileOffset;
		//}
		int getRowOffset() {
			return mRowOffset;
		}
		int getColOffset() {
			return mColOffset;
		}
		
	}; //class DeviceSiteTypePin
} // namespace placer
} // namespace torc
#endif // TORC_PLACER_DEVICESITETYPEPIN_HPP



