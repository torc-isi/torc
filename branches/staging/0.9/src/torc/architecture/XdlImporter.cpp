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
/// \brief Source for the XdlImporter class.

#include "torc/architecture/XdlImporter.hpp"
#include "torc/architecture/DeviceDesignator.hpp"

namespace torc {
namespace architecture {

	void XdlImporter::initializeDatabase(void) {
		DeviceDesignator deviceDesignator(mDesignDevice + mDesignPackage + mDesignSpeedGrade);
		mDDBPtr = new DDB(deviceDesignator);
	}

	void XdlImporter::bind(torc::physical::InstancePinSharedPtr& inInstancePin) {
		if(!mDDBPtr) return;
		INSTANCE_PIN_PHYSICAL_TO_ARCHITECTURE(inInstancePin)->updateTilewire(*mDDBPtr);
	}

	void XdlImporter::bind(torc::physical::Pip& inPip, EPipType inPipType) {
		if(!mDDBPtr) return;
		/// \todo Add routethrough usage support.
		if(inPipType == ePipTypeRoutethrough) return; // we don't support routethroughs yet
		torc::physical::TileName tileName = inPip.getTileName();
		torc::physical::WireName sourceWireName = inPip.getSourceWireName();
		torc::physical::WireName sinkWireName = inPip.getSinkWireName();
		// the pip directions defined in the XDL seem not to follow XDLRC conventions
		switch(inPip.getDirection()) {
		case torc::physical::ePipBidirectionalUnbuffered: 
			//std::cerr << inPip.getDirectionString() << std::endl;
		case torc::physical::ePipBidirectionalUnidirectionallyBuffered: 
			//std::cerr << inPip.getDirectionString() << std::endl;
		case torc::physical::ePipBidirectionalBidirectionallyBuffered: 
			//std::cerr << inPip.getDirectionString() << std::endl;
			try {
				mDDBPtr->useArc(mDDBPtr->tilePipToArc(tileName, sinkWireName, sourceWireName));
			} catch(InvalidArcException& iae) {
				std::cerr << "WARNING: Arc " << iae.mArc << " is invalid." << std::endl;
			}
			// fall through to include opposite direction arc
		case torc::physical::ePipUnidirectionalBuffered: 
			//std::cerr << inPip.getDirectionString() << std::endl;
			try {
				mDDBPtr->useArc(mDDBPtr->tilePipToArc(tileName, sourceWireName, sinkWireName));
			} catch(InvalidArcException& iae) {
				std::cerr << "WARNING: Arc " << iae.mArc << " is invalid." << std::endl;
			}
		default: 
			break;
		}
	}

} // namespace architecture
} // namespace torc
