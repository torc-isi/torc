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
