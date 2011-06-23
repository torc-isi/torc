
#ifndef TORC_PLACER_NETLISTNET_HPP
#define TORC_PLACER_NETLISTNET_HPP

#include <vector>
#include <string>
#include "torc/placer/NetlistPin.hpp"
#include "torc/physical/XilinxPhysicalTypes.hpp"

namespace torc {
namespace placer {
	class NetlistPin;

	class NetlistNet {
	// types
		typedef physical::ENetType ENetType;
		typedef boost::uint32_t uint32;
	protected:
		uint32 mIndex;
		std::string mName;
		ENetType mType;
		std::vector<NetlistPin*> mSources;
		std::vector<NetlistPin*> mSinks;
		
	public:
		NetlistNet(std::string inName, ENetType inType, uint32 inIndex) : 
			mIndex(inIndex), mName(inName), mType(inType) {}
		~NetlistNet() {}
		uint32 addSource(NetlistPin* newPin) {
			uint32 index = mSources.size();
			mSources.push_back(newPin);
			return index;
		}
		uint32 addSink(NetlistPin* newPin) {
			uint32 index = mSinks.size();
			mSinks.push_back(newPin);
			return index;
		}
		uint32 getNumSources() {
			return mSources.size();
		}
		uint32 getNumSinks() {
			return mSinks.size();
		}
		NetlistPin* getSource(uint32 i) {
			return mSources[i];
		}
		NetlistPin* getSink(uint32 i) {
			return mSinks[i];
		}
		std::string& getName() {
			return mName;
		}
		ENetType getType() {
			return mType;
		}
		uint32 getIndex() {
			return mIndex;
		}
	}; //class NetlistNet
	
	typedef std::vector<NetlistNet*> NetlistNetPtrVector;
} // namespace placer
} // namespace torc
#endif // TORC_PLACER_NETLISTNET_HPP



