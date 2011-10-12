
#ifndef TORC_PLACER_NETLISTPIN_HPP
#define TORC_PLACER_NETLISTPIN_HPP

#include <string>
#include "torc/placer/NetlistInstance.hpp"
#include "torc/placer/NetlistNet.hpp"
#include "torc/architecture/Sites.hpp"

namespace torc {
namespace placer {
	class NetlistInstance;
	class NetlistNet;
	
	class NetlistPin {
	protected:
		std::string mName;
		NetlistInstance* mInstance;
		NetlistNet* mNet;
		bool mIsNetSource;
		architecture::Sites::SitePin* mSitePin;
		
	public:
		NetlistPin(std::string inName, bool inNetSource)
			: mName(inName), mInstance(NULL), mNet(NULL), mIsNetSource(inNetSource) {}
		~NetlistPin()
		{
		}
		std::string& getName() {
			return mName;
		}
		void setInstance(NetlistInstance* inInstance) {
			mInstance = inInstance;
		}
		void setNet(NetlistNet* inNet) {
			mNet = inNet;
		}
		NetlistInstance* getInstance() {
			return mInstance;
		}
		NetlistNet* getNet() {
			return mNet;
		}
		bool isNetSource() {
			return mIsNetSource;
		}
		architecture::Sites::SitePin* getSitePin() {
			return mSitePin;
		}
		void setSitePin(architecture::Sites::SitePin* inSitePin) {
			mSitePin = inSitePin;
		}
	}; //class NetlistPin
} // namespace placer
} // namespace torc
#endif // TORC_PLACER_NETLISTPIN_HPP



