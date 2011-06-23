
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



