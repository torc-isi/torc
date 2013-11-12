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
/// \brief Source for the Instance class.

#include "torc/physical/Instance.hpp"
#include "torc/physical/InstancePin.hpp"

namespace torc {
namespace physical {

	void Instance::addPin(const InstancePinWeakPtr& inInstancePinPtr) {
		if(!inInstancePinPtr.expired()) {
			InstancePinSharedPtr instancePinSharedPtr = inInstancePinPtr.lock();
			mInstancePins.insert(InstancePinMap::value_type(instancePinSharedPtr->getPinName(), 
				instancePinSharedPtr));
		}
	}

	void Instance::removePin(const InstancePinWeakPtr& inInstancePinPtr) {
		if(!inInstancePinPtr.expired()) {
			// find all InstancePin objects for the requested pin name
			InstancePinSharedPtr instancePinSharedPtr = inInstancePinPtr.lock();
			std::pair<InstancePinSharedPtrIterator, InstancePinSharedPtrIterator> range 
				= mInstancePins.equal_range(instancePinSharedPtr->getPinName());
			// delete the one entry that matches our InstancePin
			while(range.first != range.second) {
				if(range.first->second == instancePinSharedPtr) {
					mInstancePins.erase(range.first);
					break;
				}
				range.first++;
			}
		}
	}

} // namespace physical
} // namespace torc
