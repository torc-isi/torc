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
/// \brief Unit test for the ArchitectureBrowser class.

#include <sstream>
#include <boost/test/unit_test.hpp>
#include "torc/utils/ArchitectureBrowser.hpp"


// for special testing only
#include <iostream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <set>


namespace torc {

BOOST_AUTO_TEST_SUITE(utils)

/// \brief Unit test for the design diff function.
BOOST_AUTO_TEST_CASE(ArchitectureBrowserUnitTest) {
	/*
	// setup a database for testing
	//architecture::DDB db("torc/devices/xc5vlx30");
	architecture::DDB db("devices/xc5vlx30");
	
	ArchitectureBrowser ab(db);
	
	ab.browse();
	throw;
	
	boost::regex tileLine("\\s*\\(tile\\s\\d+\\s\\d+\\s(\\S+)\\s(\\S+)\\s\\d+\\s*");
	boost::regex wireLine("\\s*\\(wire\\s(\\S+)\\s(\\d+)\\s*");
	boost::regex connLine("\\s*\\(conn\\s(\\S+)\\s(\\S+)\\)\\s*");
	
	std::ifstream xdlrcFile("xc5vlx30.xdlrc", std::ifstream::in);
	std::string inString;
	int z = 0;
	int numwires = 0;
	char buf[16384];
	
	std::string currentTile("emptyTile");
	std::string currentTileType("emptyTileType");
	std::string currentWire("emptyWire");
	std::string tilewirestring("emptyTilewire");
	architecture::Tilewire tw;
	architecture::Tilewire tw2;
	
	std::set<architecture::Tilewire> allwires;
	std::map<architecture::Tilewire, int> currentsegment;
	
	boost::smatch matches;
	
	while (xdlrcFile.getline(buf, 16384)) {
		inString = buf;
		if (regex_match(inString, matches, tileLine)) {
			currentTile = matches[1];
			currentTileType = matches[2];
			std::cout << "TILE: " << currentTile << " " << currentTileType << std::endl;
		}
		else if (regex_match(inString, matches, wireLine)) {
			numwires++;
			
			currentWire = matches[1];
			int segsize = boost::lexical_cast<int>(matches[2]);

			tilewirestring = currentWire + "@" + currentTile;
			tw = ab.stringToTilewire(tilewirestring);
			if (tw == architecture::Tilewire::sInvalid) {
				std::cout << "Recovered bad tilewire from " << tilewirestring << std::endl;
				throw;
			}
			
			std::cout << "\tWIRE: " << tilewirestring << " (" 
				<< currentTileType << ") " << tw << std::endl;
			
			if (allwires.find(tw) != allwires.end()) {
				std::cout << "WIRE ALREADY EXISTS! " << tw << " " << tilewirestring << std::endl;
				throw;
			} else {
				allwires.insert(tw);
			}
			
			for (int i = 0; i < segsize; i++) {
//std::cout << " READING CONN " << i << std::endl;
				xdlrcFile.getline(buf, 16384);
				inString = buf;
				if (regex_match(inString, matches, connLine)) {
					tilewirestring = matches[2] + "@" + matches[1];
//std::cout << "  " << tilewirestring << std::endl;
					tw2 = ab.stringToTilewire(tilewirestring);
//std::cout << "   " << tw2 << std::endl;
					if (tw2 == architecture::Tilewire::sInvalid) {
						std::cout << "Bad tilewire as part fo segment " << tilewirestring << std::endl;
						throw;
					}
					std::cout << "\t\tCONN: " << tilewirestring << " " << tw2 << std::endl;
					
				} else {
					std::cout << "ERROR, did not match a connection when expected "
						<< segsize << " " << i << " " << inString << std::endl;
					throw;
				}
			}
			
		} else {
			std::cout << "UNUSED: " << inString << std::endl;
		}
		z++;
	}
	std::cout << "Processed " << z << " lines" << std::endl;
	std::cout << "Found " << numwires << std::endl;
	std::cout << "DONE" << std::endl;
	
	*/
	
	

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace torc
