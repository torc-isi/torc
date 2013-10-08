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
/// \brief Header for the Placer class.

#ifndef TORC_PLACER_PLACERHEURISTICBASE_HPP
#define TORC_PLACER_PLACERHEURISTICBASE_HPP

#include "torc/physical/Design.hpp"
#include "torc/placer/DeviceWrapper.hpp"
#include <boost/timer.hpp>

namespace torc {
namespace placer {

	/// \brief Simulated annealing algorithm class.
	class PlacerHeuristicBase {
	protected:
	//types
		typedef architecture::DDB DDB;
		typedef boost::uint32_t uint32;
		typedef torc::physical::DesignSharedPtr DesignSharedPtr;
		
		DeviceWrapper& mDevice;
		DesignSharedPtr mDesign;
		
		uint32 mMovesPerTemperature;
		uint32 mInitialTemperature;
		//COOLING SCHEDULE
		//MOVE SELECTION PROBABILTY FOR TYPES

	public:
		// the heuristic may be the site mapping structure...
		// heuristic should contain most of the numeric constants here
		// probably functions for cooling schedule and such as well
		// in fact, the heuristic may wrap the placement entirely... as we do in routing
		PlacerHeuristicBase(DeviceWrapper& inDevice, DesignSharedPtr inDesign)
			: mDevice(inDevice), mDesign(inDesign),
			mMovesPerTemperature(1000), mInitialTemperature(10000) {
			
			// crawl the design to extract RPMS and create corresponding site types.
			/*InstanceSharedPtrVector::iterator p = mDesign->instancesBegin();
			InstanceSharedPtrVector::iterator e = mDesign->instancesEnd();
			for ( ; p != e; p++) {
				InstanceSharedPtr instance = *p;
				uint32 typeIndex = mDevice.mTypeMapping.getTypeIndex(instance->getType());
				instance->setAnnotation(ePlacerInstanceTypeIndex, typeIndex);
				
				mAllInstances.push_back(instance);
				mAllInstancesByType[typeIndex].push_back(instance);
			}*/
			
			
			
		}
		
		~PlacerHeuristicBase() {}
		
		uint32 getMovesPerTemperature() { return mMovesPerTemperature; }
		uint32 getInitialTemperature() { return mInitialTemperature; }
		uint32 updateTemperature() { return 0; }
		
		void updateCostFull() {}
		void updateCostRemovePair() {} // remove a pair of instance from cost
		void updateCostAddPair() {} // add a pair of instances from cost
		
	}; // class PlacerHeuristicBase
} // namespace placer
} // namespace torc

#endif // TORC_PLACER_PLACERHEURISTICBASE_HPP
