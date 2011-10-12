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
/// \brief Source for torc::architecture output stream helpers.

#include "torc/architecture/OutputStreamHelpers.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/DDBStreamHelper.hpp"
#include "torc/architecture/Tiles.hpp"
#include "torc/architecture/Tilewire.hpp"
#include "torc/architecture/Arc.hpp"

namespace torc {
namespace architecture {

using namespace torc::architecture::xilinx;

	std::ostream& operator <<(std::ostream& os, const Tilewire& rhs) {
		const DDB* ddbPtr = DDBStreamHelper::getDDBPtr(os);
		if(ddbPtr) {
			// the database is available: look up the extended tilewire information
			os << ExtendedWireInfo(*ddbPtr, rhs);
		} else {
			// the database is not available: write out the basic tilewire information
			os << rhs.getWireIndex() << "@" << rhs.getTileIndex();
		}
		return os;
	}

	std::ostream& operator <<(std::ostream& os, const ExtendedWireInfo& rhs) {
		// handle uninitialized wire info
		if(rhs.mTileIndex.isUndefined()) return os << "[UNINITIALIZED]";

		// handle regular wire info
		return os << rhs.mWireName << "@[" << rhs.mTileRow << "," << rhs.mTileCol << "] " 
			<< rhs.mTileTypeName << " \"" << rhs.mTileName << "\" (" << rhs.mWireIndex << "@" 
			<< rhs.mTileIndex << ")"
			<< (WireInfo::isHidden(rhs.mWireFlags) ? " HIDDEN" : "")
			<< (WireInfo::isInput(rhs.mWireFlags) ? " INPUT" : "")
			<< (WireInfo::isOutput(rhs.mWireFlags) ? " OUTPUT" : "")
			<< (WireInfo::isRemote(rhs.mWireFlags) ? " REMOTE" : "")
			<< (WireInfo::isRemoteNodeCapable(rhs.mWireFlags) ? " REMOTE_NODE_CAPABLE" : "")
			<< (WireInfo::isRemoteArcCapable(rhs.mWireFlags) ? " REMOTE_ARC_CAPABLE" : "")
			;
	}

	std::ostream& operator <<(std::ostream& os, const Arc& rhs) {
		return os << rhs.getSourceTilewire() << " >> " << rhs.getSinkTilewire();
	}

} // namespace architecture
} // namespace torc
