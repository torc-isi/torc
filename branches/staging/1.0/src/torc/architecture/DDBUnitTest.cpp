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
/// \brief Unit test for the DDB class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/DDBStreamHelper.hpp"
#include "torc/architecture/OutputStreamHelpers.hpp"
#include "torc/common/Devices.hpp"
#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/architecture/Array.hpp"
#include "torc/architecture/Sites.hpp"
#include <iostream>

namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

using namespace torc::architecture::xilinx;

void testDevice(const std::string& inDeviceName);
void testDevice(const std::string& inDeviceName) {
	// functions not tested:
	// (note: some of these functions are tested in their respective classes)
	//		const string& getFamilyName(void) const;
	//		const Segments& getSegments(void) const;
	//		const WireUsage& getWireUsage(void) const;
	//		const ArcUsage& getArcUsage(void) const;
	//		void clearUsage(void);

	// functions tested:
	//		DDB(const string& inDeviceName);
	DDB ddb(inDeviceName);

	// functions tested:
	//		const string& getDeviceName(void) const;
	BOOST_CHECK_EQUAL(ddb.getDeviceName(), inDeviceName);

	// for now this test merely opens representative device databases; regression tests are 
	// performed elsewhere
	if(true) return;

	// functions tested:
	//		const Tiles& getTiles(void) const;
	std::map<TileTypeIndex, int> tileTypeMap;
	const Tiles& tiles = ddb.getTiles();
	TileCount tileCount = tiles.getTileCount();
	for(TileIndex tileIndex; tileIndex < tileCount; tileIndex++) {
		const TileInfo& tileInfo = tiles.getTileInfo(tileIndex);
		TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
		const std::string& tileTypeName = tiles.getTileTypeName(tileTypeIndex);
		tileTypeMap[tileTypeIndex] = 0;
		(void) tileTypeName;
	}
	std::cout << tileTypeMap.size() << " tile types" << std::endl;

	// functions tested
	//		const Sites& getSites(void) const;
	std::map<std::string, int> siteTypeMap;
	const Sites& sites = ddb.getSites();
	SiteCount siteCount = sites.getSiteCount();
	for(SiteIndex siteIndex; siteIndex < siteCount; siteIndex++) {
		const Site& site = sites.getSite(siteIndex);
		const std::string& siteTypeName = site.getPrimitiveDefPtr()->getName();
		siteTypeMap[siteTypeName] = 0;
	}
	std::cout << ddb;
	std::cout << siteTypeMap.size() << " site types" << std::endl;
}

/// \brief Unit test for the DDB class.
BOOST_AUTO_TEST_CASE(DDBUnitTest) {
	// iterate over the devices
	const torc::common::DeviceVector& devices = torc::common::Devices::getUnitTestDevices();
	torc::common::DeviceVector::const_iterator dp = devices.begin();
	torc::common::DeviceVector::const_iterator de = devices.end();
	while(dp < de) {
		const std::string& device = *dp++;
		if(device.empty()) break;
		testDevice(device);
	}
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
