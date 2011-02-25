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
/// \brief Example program to open a device database and do something with it.

#include "torc/Architecture.hpp"
#include "torc/Common.hpp"
#include <iostream>

using namespace torc::architecture;
using namespace torc::architecture::xilinx;

int main(int argc, char* argv[]) {

	// construct and initialize the device database
	(void) argc;
	torc::common::DirectoryTree directoryTree(argv[0]);
	DeviceDesignator designator("xc6vlx75tff484-1");
	DDB ddb(designator);

	// look up a site output and convert it to a tilewire
	const Sites& sites = ddb.getSites();
	SiteIndex index = sites.findSiteIndex("SLICE_X0Y119");
	const Sites::Site& site = sites.getSite(index);
	Tilewire pinTilewire = site.getPinTilewire("A");
	std::cout << ddb << pinTilewire << std::endl;

	// look up arcs that connect from this tilewire
	ArcVector sinks;
	ddb.expandSegmentSinks(pinTilewire, sinks);
	ArcVector::const_iterator pos = sinks.begin();
	ArcVector::const_iterator end = sinks.end();
	while(pos < end) 
		std::cout << "\t" << *pos++ << std::endl;

	return 0;
}
