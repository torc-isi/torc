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

#ifndef TORC_PLACER_DEVICESITEPIN_HPP
#define TORC_PLACER_DEVICESITEPIN_HPP

#include "torc/architecture/Tilewire.hpp"
#include <string>

namespace torc {
namespace placer {
	class DeviceSitePin
	{
	// types
		typedef architecture::Tilewire Tilewire;
	protected:
		std::string& mName;
		
		Tilewire mTilewire;
		int mRow; // for the sitepin, we have actual row and column locations for distance calculation
		int mCol;
		//int mTileOffset; // these offsets tell you where the pin actually lives relative to the tile index of the site
		
	public:
		DeviceSitePin(std::string& inName, int inRow, int inCol) : 
			mName(inName), mRow(inRow), mCol(inCol) {}
		~DeviceSitePin() {}
		std::string& getName() {
			return mName;
		}
		int getRow() {
			return mRow;
		}
		int getCol() {
			return mCol;
		}
		Tilewire getTilewire() {
			return mTilewire;
		}
		
	}; //class DeviceSitePin
} // namespace placer
} // namespace torc
#endif // TORC_PLACER_DEVICESITEPIN_HPP
