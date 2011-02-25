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
/// \brief Source for the DeviceDesignator class.

#include "torc/architecture/DeviceDesignator.hpp"
#include <boost/regex.hpp>

namespace torc {
namespace architecture {

	boost::regex DeviceDesignator::sSpartan2RegEx( 
		"(x?c?2s[0-9]+)" /* device */
		"((?:cs|fg|pq|tq|vq)[0-9]+)?" /* package */
		"(-[0-9]+Q?)?", /* speed */
		boost::regex_constants::icase
	);

	boost::regex DeviceDesignator::sSpartan2ERegEx( 
		"(x?c?2s[0-9]+e)" /* device */
		"((?:fg|ft|pq)[0-9]+)?" /* package */
		"(-[0-9]+Q?)?", /* speed */
		boost::regex_constants::icase
	);

	boost::regex DeviceDesignator::sSpartan3RegEx( 
		"(x?c?3s[0-9]+l?)" /* device */
		"((?:cp|fg|ft|pq|tq|vq)[0-9]+)?" /* package */
		"(-[0-9]+)?", /* speed */
		boost::regex_constants::icase
	);

	boost::regex DeviceDesignator::sSpartan3ARegEx( 
		"(x?c?3s[0-9]+an?)" /* device */
		"((?:fg|fgg|ft|tq|tqg)[0-9]+)?" /* package */
		"(-[0-9]+)?", /* speed */
		boost::regex_constants::icase
	);

	boost::regex DeviceDesignator::sSpartan3ERegEx( 
		"(x?c?3s[0-9]+e)" /* device */
		"((?:cp|fg|ft|pq|vq)[0-9]+)?" /* package */
		"(-[0-9]+)?", /* speed */
		boost::regex_constants::icase
	);

	boost::regex DeviceDesignator::sSpartan6RegEx(
		"(x?c?6s[l]x[0-9]+t?)" /* device */
		"((?:cpg|csg|fgg|ftg|tqg)[0-9]+)?" /* package */
		"(-[0-9]+)?", /* speed */
		boost::regex_constants::icase
	);

	boost::regex DeviceDesignator::sSpartan6LRegEx(
		"(x?c?6s[l]x[0-9]+t?l)" /* device */
		"((?:cpg|csg|fgg|ftg|tqg)[0-9]+)?" /* package */
		"(-[0-9]+)?", /* speed */
		boost::regex_constants::icase
	);

	boost::regex DeviceDesignator::sVirtexRegEx( 
		"(x?c?v[0-9]+)" /* device */
		"((?:bg|cs|fg|hq|pq|tq)[0-9]+)?" /* package */
		"(-[0-9]+)?", /* speed */
		boost::regex_constants::icase
	);

	boost::regex DeviceDesignator::sVirtexERegEx(
		"(x?c?v[0-9]+e)" /* device */
		"((?:bg|cs|fg|hq|pq)[0-9]+)?" /* package */
		"(-[0-9]+)?", /* speed */
		boost::regex_constants::icase
	);

	boost::regex DeviceDesignator::sVirtex2RegEx(
		"(x?c?2v[0-9]+)" /* device */
		"((?:bf|bg|cs|ff|fg)[0-9]+)?" /* package */
		"(-[0-9]+)?", /* speed */
		boost::regex_constants::icase
	);

	boost::regex DeviceDesignator::sVirtex2PRegEx(
		"(x?c?2vp[0-9]+)" /* device */
		"((?:ff|fg)[0-9]+)?" /* package */
		"(-[0-9]+)?", /* speed */
		boost::regex_constants::icase
	);

	boost::regex DeviceDesignator::sVirtex4RegEx(
		"(x?c?4v[fls]x[0-9]+)" /* device */
		"((?:ff|sf)[0-9]+)?" /* package */
		"(-[0-9]+)?", /* speed */
		boost::regex_constants::icase
	);

	boost::regex DeviceDesignator::sVirtex5RegEx(
		"(x?c?5v[fls]x[0-9]+)" /* device */
		"((?:ff)[0-9]+)?" /* package */
		"(-[0-9]+)?", /* speed */
		boost::regex_constants::icase
	);

	boost::regex DeviceDesignator::sVirtex6RegEx(
		"(x?c?6v[chls]x[0-9]+t?)" /* device */
		"((?:ff)[0-9]+)?" /* package */
		"(-[0-9]+)?", /* speed */
		boost::regex_constants::icase
	);

	boost::regex DeviceDesignator::sVirtex6LRegEx(
		"(x?c?6v[ls]x[0-9]+t?l)" /* device */
		"((?:ff)[0-9]+)?" /* package */
		"(-[0-9]+)?", /* speed */
		boost::regex_constants::icase
	);

	DeviceDesignator::DeviceDesignator(const string& inDeviceDesignator) {
		if(parse(inDeviceDesignator, sSpartan2RegEx)) { mFamily = eFamilySpartan2; } else 
		if(parse(inDeviceDesignator, sSpartan2ERegEx)) { mFamily = eFamilySpartan2E; } else 
		if(parse(inDeviceDesignator, sSpartan3RegEx)) { mFamily = eFamilySpartan3; } else 
		if(parse(inDeviceDesignator, sSpartan3ARegEx)) { mFamily = eFamilySpartan3A; } else 
		if(parse(inDeviceDesignator, sSpartan3ERegEx)) { mFamily = eFamilySpartan3E; } else 
		if(parse(inDeviceDesignator, sSpartan6RegEx)) { mFamily = eFamilySpartan6; } else 
		if(parse(inDeviceDesignator, sSpartan6LRegEx)) { mFamily = eFamilySpartan6L; } else 
		if(parse(inDeviceDesignator, sVirtexRegEx)) { mFamily = eFamilyVirtex; } else 
		if(parse(inDeviceDesignator, sVirtexERegEx)) { mFamily = eFamilyVirtexE; } else 
		if(parse(inDeviceDesignator, sVirtex2RegEx)) { mFamily = eFamilyVirtex2; } else 
		if(parse(inDeviceDesignator, sVirtex2PRegEx)) { mFamily = eFamilyVirtex2P; } else 
		if(parse(inDeviceDesignator, sVirtex4RegEx)) { mFamily = eFamilyVirtex4; } else 
		if(parse(inDeviceDesignator, sVirtex5RegEx)) { mFamily = eFamilyVirtex5; } else 
		if(parse(inDeviceDesignator, sVirtex6RegEx)) { mFamily = eFamilyVirtex6; } else 
		if(parse(inDeviceDesignator, sVirtex6LRegEx)) { mFamily = eFamilyVirtex6L; } else 
		{ mFamily = eFamilyUnknown; }
	}

	bool DeviceDesignator::parse(const string& inDeviceDesignator, const boost::regex& inRegEx) {
		boost::smatch what;
		if(boost::regex_match(inDeviceDesignator, what, inRegEx, boost::match_default)) {
			if(what[1].matched) mDeviceName = std::string(what[1].first, what[1].second);
			if(what[2].matched) mDevicePackage = std::string(what[2].first, what[2].second);
			if(what[3].matched) mDeviceSpeedGrade = std::string(what[3].first, what[3].second);
			return true;
		} else {
			return false;
		}
	}

} // namespace architecture
} // namespace torc
