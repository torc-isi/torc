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
/// \brief Source for the ArchitectureBrowser class.

#include "torc/utils/ArchitectureBrowser.hpp"

namespace torc {
	boost::regex ArchitectureBrowser::sTilewireRegEx("\\s*(\\S+)@(\\S+)\\s*");
	boost::regex ArchitectureBrowser::sIndexRegEx("(\\d+)");
	boost::regex ArchitectureBrowser::sNameRegEx("(\\S+)");


	void ArchitectureBrowser::printFullMenu() {
		using std::cout;
		using std::endl;
		cout << endl;
		cout << "########## Device Database Browser ##########" << endl;
		cout << "Select a command:" << endl
			<< "\t0) Device Summary - Display basic information about the device." << endl
			<< "\t1) Tile list - Search a list of all tiles in the device." << endl
			<< "\t2) Wire List - Display a list of all wires in a given tile type." << endl
			<< "\t3) Tilewire Detail - Display detail of a tilewire." << endl
			<< "\t4) Segment Information" << endl
			<< "\t5) Sources - Display segment sources." << endl
			<< "\t6) Sinks - Display segment sinks." << endl
			<< "\t7) Site List - Search a list of all sites in the device." << endl
			<< "\t8) Site Detail - Display detail of a site." << endl
			<< "\t9) Site Types - Display a list of all site types in the device family." << endl
			<< "\t10) Sites of Type - Display a list of all sites of a specified type." << endl
			<< "\tq) Quit" << endl 
			<< "> ";
	}

	void ArchitectureBrowser::setupFunctionMap() {
		mFunctionMap["q"] = eQuit;
		mFunctionMap["Q"] = eQuit;
		mFunctionMap["exit"] = eQuit;
		mFunctionMap["quit"] = eQuit;
		
		mFunctionMap["0"] = eDeviceSummary;
		mFunctionMap["1"] = eTiles;
		mFunctionMap["2"] = eWires;
		mFunctionMap["3"] = eDetail;
		mFunctionMap["4"] = eSegment;
		mFunctionMap["5"] = eSources;
		mFunctionMap["6"] = eSinks;
		mFunctionMap["7"] = eSites;
		mFunctionMap["8"] = eSiteDetail;
		mFunctionMap["9"] = eSiteTypes;
		mFunctionMap["10"] = eSitesOfType;
		mFunctionMap["d"] = eDetail;
		mFunctionMap["D"] = eDetail;
	}

} // namespace torc
