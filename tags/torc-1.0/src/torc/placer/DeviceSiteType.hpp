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
/// \brief Header for the DeviceSiteType class.

#ifndef TORC_PLACER_DEVICESITETYPE_HPP
#define TORC_PLACER_DEVICESITETYPE_HPP

#include "DeviceSite.hpp"
#include "DeviceSiteTypePin.hpp"
#include <vector>

namespace torc {
namespace placer {
	class DeviceSiteType
	{
		typedef std::vector<DeviceSiteTypePin> DeviceSiteTypePinVector;
		
	protected:
		std::string mName;
		DeviceSiteTypePinVector mPins;
		int mSiteTypeIndex;
		
	public:
		DeviceSiteType() {
			mName = "UNINITIALIZED!";
			mSiteTypeIndex = -1;
		}
		DeviceSiteType(std::string inName, int inSiteTypeIndex) : mName(inName), 
			mSiteTypeIndex(inSiteTypeIndex) {}
		~DeviceSiteType() {}
		const std::string& getName() const {
			return mName;
		}
		void setName(std::string inName) {
			mName = inName;
		}
		int getSiteTypeIndex() {
			return mSiteTypeIndex;
		}
		void setSiteTypeIndex(int inSiteTypeIndex) {
			mSiteTypeIndex = inSiteTypeIndex;
		}
		//void addPin(std::string inName, int inTileOffset)
		void addPin(std::string inName, int inRowOffset, int inColOffset) {
			//PlacerSiteTypePin pin(inName, inTileOffset)
			DeviceSiteTypePin pin(inName, inRowOffset, inColOffset);
			mPins.push_back(pin); // this is a nasty thing, copying this around, but probably don't matter, only to initialize the thing
		}
		int getNumPins() {
			return mPins.size();
		}
		DeviceSiteTypePin& getPin(int i) {
			return mPins[i];
		}
	}; //class DeviceSiteType
} // namespace placer
} // namespace torc
#endif // TORC_PLACER_DEVICESITETYPE_HPP
