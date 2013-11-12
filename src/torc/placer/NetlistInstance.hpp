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

#ifndef TORC_PLACER_NETLISTINSTANCE_HPP
#define TORC_PLACER_NETLISTINSTANCE_HPP

#include "torc/physical/Instance.hpp"
#include "torc/placer/DeviceSite.hpp"
#include "torc/placer/NetlistPin.hpp"
#include <vector>

namespace torc {
namespace placer {
	class DeviceSite;
	
	class NetlistInstance {
	//types
		typedef boost::uint32_t uint32;
		typedef physical::InstanceSharedPtr InstanceSharedPtr;
		
	protected:
		DeviceSite* mSite; // this is the site on which this instance is placed
		
		InstanceSharedPtr mPhysicalInstance;
		uint32 mInstanceType; // the type index for sites that this instance can be placed on
			// type indexes
		//std::vector<int> m
		
	public:
		NetlistInstance(InstanceSharedPtr inInstance, uint32 inType) : mSite(NULL), 
			mPhysicalInstance(inInstance), mInstanceType(inType) {}
		~NetlistInstance() {}
		void setSite(DeviceSite* siteptr) {
			mSite = siteptr;
		}
		DeviceSite* getSite() {
			return mSite;
		}
		InstanceSharedPtr getInstance() {
			return mPhysicalInstance;
		}
		uint32 getType() {
			return mInstanceType;
		}
		void addPin(NetlistPin* pin) {
		}
		uint32 getNumPins() {
			return 0;
		}
		NetlistPin* getPin(uint32 index) {
			return 0;
		}
	}; // class NetlistInstance
	
	typedef std::vector<NetlistInstance*> NetlistInstancePtrVector;
} // namespace placer
} // namespace torc
#endif // TORC_PLACER_NETLISTINSTANCE_HPP
