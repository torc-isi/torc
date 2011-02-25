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
/// \brief Source for the DDB unit test.

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

/// \brief Unit test for the DDB class.
BOOST_AUTO_TEST_CASE(architecture_ddb) {
	// iterate over the devices
	const torc::common::DeviceVector& devices = torc::common::Devices::getAllDevices();
	torc::common::DeviceVector::const_iterator dp = devices.begin();
	torc::common::DeviceVector::const_iterator de = devices.end();
	while(dp < de) {
		const std::string& device = *dp++;
		if(device.empty()) break;
		testDevice(device);
//break;
	}
}


void testDevice(const std::string& inDeviceName) {
	// functions not tested:
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

std::map<TileTypeIndex, int> tileTypeMap;
	// functions tested:
	//		const Tiles& getTiles(void) const;
	const Tiles& tiles = ddb.getTiles();
	TileCount tileCount = tiles.getTileCount();
	for(TileIndex tileIndex; tileIndex < tileCount; tileIndex++) {
		const TileInfo& tileInfo = tiles.getTileInfo(tileIndex);
		TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
		const std::string& tileTypeName = tiles.getTileTypeName(tileTypeIndex);
tileTypeMap[tileTypeIndex] = 0;
//		std::cout << "\t" << tileIndex << ": " << tileInfo.getName() << " (" << tileTypeName 
//			<< ") [" << tileInfo.getRow() << "," << tileInfo.getCol() << "]  ";
//		const Array<const WireInfo>& wires = tiles.getWireInfo(tileTypeIndex);
//		WireCount wireCount(wires.getSize());
//		for(WireIndex wireIndex; wireIndex < wireCount; wireIndex++) {
//			std::cout << ".";
//			const WireInfo& wireInfo = wires[wireIndex];
//			std::cout << "\t\t" << wireIndex << ": " << wireInfo.getName() 
//				<< std::endl;
//		}
//		std::cout << std::endl;
(void) tileTypeName;
//		TileIndex tileIndex = tiles.findTileIndex(tileInfo.getName());
//		std::cout << "\t" << tileInfo.getName() << ": " << tileIndex << std::endl;
	}
std::cout << tileTypeMap.size() << " tile types" << std::endl;

std::map<std::string, int> siteTypeMap;
	// functions tested
	//		const Sites& getSites(void) const;
	const Sites& sites = ddb.getSites();
	SiteCount siteCount = sites.getSiteCount();
	for(SiteIndex siteIndex; siteIndex < siteCount; siteIndex++) {
//		std::cout << "\t" << siteIndex << std::endl;
		const Sites::Site& site = sites.getSite(siteIndex);
		const std::string& siteTypeName = site.getPrimitiveDefPtr()->getName();
		siteTypeMap[siteTypeName] = 0;
	}
	std::cout << ddb;
std::cout << siteTypeMap.size() << " site types" << std::endl;
//	const Array<const Sites::Site>& siteArray = sites.getSites();
//	Array<const Sites::Site>::const_iterator ssp = siteArray.begin();
//	Array<const Sites::Site>::const_iterator sse = siteArray.end();
//	while(ssp < sse) {
//		const Sites::Site& site = *ssp++;
//		std::cout << site.getName() << " (" << site.getSiteDefinitionPtr()->getName() << ")" << std::endl;
//		const Array<const Sites::SitePin>& sitePinArray = site.getSiteDefinitionPtr()->getPins();
//		Array<const Sites::SitePin>::const_iterator spp = sitePinArray.begin();
//		Array<const Sites::SitePin>::const_iterator spe = sitePinArray.end();
//		while(spp < spe) {
//			std::string pinName = spp++->getName();
//			std::cout << "\t" << pinName << ": " << site.getPinTilewire(pinName) << std::endl;
//		}
//	}
return;
	xilinx::SiteIndex siteIndex = sites.findSiteIndex("TBUF_X38Y26");
	const Sites::Site& site = sites.getSite(siteIndex);
	std::cout << "Found site " << site.getName() << std::endl;
	Tilewire pinTilewire1 = site.getPinTilewire("T");
	std::cout << "Found " << site.getName() << ".T" << " tilewire: " << pinTilewire1 << std::endl;
	Tilewire pinTilewire2 = ddb.sitePinToTilewire("TBUF_X38Y26", "T");
	std::cout << "Found TBUF_X38Y26.T tilewire: " << pinTilewire2 << std::endl;

	TilewireVector segment;
	Tilewire tilewire(TileIndex(14), WireIndex(1));
	ddb.expandSegment(tilewire, segment, DDB::eExpandDirectionSinkward);
	BOOST_CHECK_EQUAL(segment.size(), 23u);
//	std::cerr << ddb << tilewire << std::endl;
//	TilewireVector::const_iterator sep = segment.begin();
//	TilewireVector::const_iterator see = segment.end();
//	while(sep < see) {
//		const Tilewire& tw = *sep++;
//		//std::cerr << "\t" << tw << std::endl;
//		TilewireVector sinks;
//		ddb.expandTilewireSinks(tw, sinks);
//		TilewireVector::const_iterator sip = sinks.begin();
//		TilewireVector::const_iterator sie = sinks.end();
//		while(sip < sie) std::cerr << "\t\t" << *sip++ << std::endl;
//	}
//	std::cerr << ddb << tilewire << std::endl;
//	TilewireVector::const_iterator sep = segment.begin();
//	TilewireVector::const_iterator see = segment.end();
//	while(sep < see) {
//		const Tilewire& tw = *sep++;
//		std::cerr << "\t" << tw << std::endl;
//		TilewireVector sources;
//		ddb.expandTilewireSources(tw, sources);
//		TilewireVector::const_iterator sop = sources.begin();
//		TilewireVector::const_iterator soe = sources.end();
//		while(sop < soe) std::cerr << "\t\t" << *sop++ << std::endl;
//	}
//	std::cerr << ddb << tilewire << std::endl;
//	ArcVector sinks;
//	ddb.expandSegmentSinks(tilewire, sinks);
//	ArcVector::const_iterator sip = sinks.begin();
//	ArcVector::const_iterator sie = sinks.end();
//	while(sip < sie) {
//		const Tilewire& source = sip->getSourceTilewire();
//		const Tilewire& sink = sip->getSinkTilewire();
//		std::cerr << "\t" << source << " >> " << sink << std::endl;
//		sip++;
//	}
//	std::cerr << ddb << tilewire << std::endl;
//	ArcVector sources;
//	ddb.expandSegmentSources(tilewire, sources);
//	ArcVector::const_iterator sip = sources.begin();
//	ArcVector::const_iterator sie = sources.end();
//	while(sip < sie) {
//		const Tilewire& source = sip->getSourceTilewire();
//		const Tilewire& sink = sip->getSinkTilewire();
//		std::cerr << "\t" << sink << " << " << source << std::endl;
//		sip++;
//	}
//	ExtendedWireInfo ewi(ddb, tilewire);
//	std::cerr << ddb;
//	std::cerr << ewi << std::endl;
//	std::cerr << tilewire << std::endl;

	ArcVector sinks;
	ddb.expandSegmentSinks(tilewire, sinks);
	ArcVector::const_iterator sip = sinks.begin();
	const Tilewire& source1 = sip->getSourceTilewire();
	const Tilewire& sink1 = sip->getSinkTilewire();
	sip++;
	const Tilewire& source2 = sip->getSourceTilewire();
	const Tilewire& sink2 = sip->getSinkTilewire();
	ArcUsage& arcUsage = ddb.getArcUsage();
	WireUsage& wireUsage = ddb.getWireUsage();
	BOOST_CHECK_EQUAL(arcUsage.isUsed(source1, sink1), false);
	arcUsage.use(source1, sink1);
	arcUsage.release(source2, sink2);
	BOOST_CHECK_EQUAL(arcUsage.isUsed(source1, sink1), true);
	BOOST_CHECK_EQUAL(arcUsage.isUsed(source2, sink2), false);
	ddb.useArc(Arc(source2,sink2));
	BOOST_CHECK_EQUAL(wireUsage.isUsed(source2), true);
	BOOST_CHECK_EQUAL(wireUsage.isUsed(sink2), true);
	arcUsage.clear();
	BOOST_CHECK_EQUAL(arcUsage.isUsed(source1, sink1), false);
	BOOST_CHECK_EQUAL(arcUsage.isUsed(source2, sink2), false);

	
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
