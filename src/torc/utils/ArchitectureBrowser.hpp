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
/// \brief ArchitectureBrowser class for exploring database contents.

#ifndef TORC_UTILS_ARCHITECTUREBROWSER_HPP
#define TORC_UTILS_ARCHITECTUREBROWSER_HPP

#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/DDBStreamHelper.hpp"
#include "torc/architecture/OutputStreamHelpers.hpp"
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>


namespace torc {

/// \brief Utility class for exploring database contents.
class ArchitectureBrowser {
	
protected:
	/// \brief Imported type name.
	typedef std::string string;
	/// \brief Imported type name.
	typedef architecture::DDB DDB;
	/// \brief Imported type name.
	typedef architecture::PrimitiveDef PrimitiveDef;
	/// \brief Imported type name.
	typedef architecture::Segments Segments;
	/// \brief Imported type name.
	typedef architecture::Sites Sites;
	/// \brief Imported type name.
	typedef architecture::Tiles Tiles;
	/// \brief Imported type name.
	typedef architecture::ArcUsage ArcUsage;
	/// \brief Imported type name.
	typedef architecture::WireUsage WireUsage;
	/// \brief Imported type name.
	typedef architecture::Tilewire Tilewire;
	/// \brief Imported type name.
	typedef architecture::TilewireVector TilewireVector;
	/// \brief Imported type name.
	typedef architecture::WireInfo WireInfo;
	/// \brief Imported type name.
	typedef architecture::xilinx::TileIndex TileIndex;
	/// \brief Imported type name.
	typedef architecture::xilinx::TileCount TileCount;
	/// \brief Imported type name.
	typedef architecture::xilinx::WireIndex WireIndex;
	/// \brief Imported type name.
	typedef architecture::xilinx::TileTypeIndex TileTypeIndex;
	/// \brief Imported type name.
	typedef architecture::xilinx::SiteIndex SiteIndex;
	/// \brief Type for member function pointers.
	typedef void (ArchitectureBrowser::*BrowserFunction)(const std::string& x);
	
	/// \brief Database Reference.
	DDB& mDB;
	/// \brief Segments reference from the database.
	const Segments& mSegments;
	/// \brief Site data reference from the database.
	const Sites& mSites;
	/// \brief Tile data reference from the database.
	const Tiles& mTiles;
	/// \brief Pip usage information from the database.
	ArcUsage& mArcUsage;
	/// \brief Wire usage information from the database.
	WireUsage& mWireUsage;
	
	/// \brief Regular expression for Tilewire form.
	static boost::regex sTilewireRegEx;
	/// \brief Regular expression for nonnegative integers.
	static boost::regex sIndexRegEx;
	/// \brief Regular expression for a name.
	static boost::regex sNameRegEx;
	
	/// \brief Enumeration of menu selection options.
	enum EFunctionMap {
		eNotDefined, eQuit, eBit, eClear, eDetail, eFind, eGoto, eSinks,
		eInstances, eRoute, eSources, eWires, eTypes, eEquations, eeXtent, eMap, 
		eSites, eSitePins, eSegment, eSiteTypes, eDeviceSummary, eTiles, eSiteDetail, eSitesOfType
	};

	/// \brief Mapping from function to index.
	std::map<string, EFunctionMap> mFunctionMap;
	
	
	
public:
	/// \brief Constructor.
	ArchitectureBrowser(DDB& inDB) : mDB(inDB), mSegments(inDB.getSegments()), 
		mSites(inDB.getSites()), mTiles(inDB.getTiles()), 
		mArcUsage(inDB.getArcUsage()), mWireUsage(inDB.getWireUsage()) {
		setupFunctionMap();
	}
	/// \brief Destructor.
	~ArchitectureBrowser() {}
public:
	/// \brief Function to begin an interactive browsing session.
	void browse() {
		using std::cout;
		using std::cin;
		using std::endl;
		
		BrowserFunction getFilteredTileListRef = &ArchitectureBrowser::getFilteredTileList;
		BrowserFunction getTilewireDetailRef = &ArchitectureBrowser::getTilewireDetail;
		BrowserFunction displayFullSegmentRef = &ArchitectureBrowser::displayFullSegment;
		BrowserFunction getWireListForTileTypeRef = &ArchitectureBrowser::getWireListForTileType;
		BrowserFunction displaySegmentSourcesRef = &ArchitectureBrowser::displaySegmentSources;
		BrowserFunction displaySegmentSinksRef = &ArchitectureBrowser::displaySegmentSinks;
		BrowserFunction getFilteredSiteListRef = &ArchitectureBrowser::getFilteredSiteList;
		BrowserFunction getSiteDetailRef = &ArchitectureBrowser::getSiteDetail;
		BrowserFunction displaySitesOfTypeRef = &ArchitectureBrowser::displaySitesOfType;
		
		string function;
		string userinput;
		Tilewire tw;
		while (true) {
			printFullMenu();
			cin >> function;
			switch (mFunctionMap[function]) {
				case eQuit:
					cout << "Bye!" << endl << endl;
					return;
					break;
				case eDeviceSummary:
					cout << "Device Summary." << endl;
					deviceSummary();
					break;
				case eTiles:
					//cout << "Tile List." << endl;
					functionPrompt("Enter regexp filter (\".*\" to match all tiles), q to quit",
						getFilteredTileListRef);
					break;
				case eWires:
					functionPrompt("Enter tile type index, tile type name or tile name, q to quit", 
						getWireListForTileTypeRef);
					break;
				case eDetail:
					functionPrompt("Enter wire@tile, q to quit", getTilewireDetailRef);
					break;	
				case eSegment:
					functionPrompt("Enter wire@tile, q to quit", displayFullSegmentRef);
					break;
				case eSources:
					functionPrompt("Enter wire@tile, q to quit", displaySegmentSourcesRef);
					break;
				case eSinks:
					functionPrompt("Enter wire@tile, q to quit", displaySegmentSinksRef);
					break;
				case eSites:
					//cout << "Site List." << endl;
					functionPrompt("Enter regexp filter (\".*\" to match all sites), q to quit",
						getFilteredSiteListRef);
					break;
				case eSiteDetail:
					functionPrompt("Enter site name or index, q to quit", getSiteDetailRef);
					break;
				case eSiteTypes:
					cout << "List site types: " << endl;
					enumerateSiteTypes();
					break;
				case eSitesOfType:
					functionPrompt("Enter site type name, q to quit", displaySitesOfTypeRef);
					break;
					
					
					
				case eBit:
					cout << "Bit - Nothing here yet." << endl;
					break;
				case eClear:
					cout << "Clear - Nothing here yet." << endl;
					break;
				case eFind:
					cout << "Enter tile index: ";
					cin >> userinput;
					//findIndexInformation(userinput);
					break;
				case eGoto:
					cout << "Enter wire@tile: " << endl;
					cin >> userinput;
					//enumerateTileWireSinks(userinput);
					break;
				case eInstances:
					cout << "Enter tile type: ";
					cin >> userinput;
					//enumerateTileInstances(userinput);
					break;
				case eRoute:
					cout << "Route - Not yet supported" << endl;
					//cout << "Route - Supports routing to a single sink." << endl;
					//routeToSingleSink();
					break;
				case eTypes:
					cout << "Available tile types:" << endl;
					//enumerateTileTypes();
					break;
				case eEquations:
					cout << "Equations - Nothing here yet." << endl;
					break;
				case eeXtent:
					cout << "Enter wire@tile: ";
					cin >> userinput;
					//enumerateTilewireReach(userinput);
					break;
				case eMap:
					cout << "Displaying tilemap:" << endl;
					//displayTilemap();
					break;
				case eSitePins:
					cout << "Enter site: ";
					cin >> userinput;
					//enumerateSitePins(userinput);
					break;
				default:
					cout << "Invalid Function: " << function << endl;
			}
		}
	}
	/// \brief Secondary menu loop to remain in a particular function.
	void functionPrompt(const char* inPrompt, BrowserFunction f) {
		using std::cout;
		using std::endl;
		using std::cin;
		string userinput;
		while (true) {
			cout << endl << inPrompt << "> ";
			cin >> userinput;
			switch (mFunctionMap[userinput]) {
				case eQuit:
					return;
				default:
					(this->*f)(userinput);
					break;
			}
		}
	}
	/// \brief A brief summary of device information.
	void deviceSummary() {
		using std::cout;
		using std::endl;
		cout << "Name: " << mDB.getDeviceName() << endl;
		cout << "Family: " << mDB.getFamilyName() << endl;
		cout << "Speed grades:";
		for (unsigned int i =0; i < mDB.getSpeedGrades().size(); i++) {
			cout << " " << mDB.getSpeedGrades()[i];
		}
		cout << endl;
		
		cout << "Total tiles: " << mTiles.getTileCount() << endl;
		cout << "Family tile type count: " << mTiles.getTileTypeCount() << endl;
		cout << "Tilemap dimensions: " << mTiles.getRowCount() << " " 
			<< mTiles.getColCount() << endl;
		cout << "Wire use: " << mWireUsage.getWireUsageCount() << endl;
		cout << "Arc use: " << mArcUsage.getArcUsageCount() << endl;
	}
	/// \brief List all tiles.
	void getTileList() {
		using std::cout;
		using std::endl;
		std::ios_base::fmtflags saveflags = cout.flags();
		
		for (TileIndex i(0); i < mTiles.getTileCount(); i++) {
			const architecture::TileInfo& ti = mTiles.getTileInfo(i);
			cout << std::setw(6) << i << " type " << std::setw(3) << ti.getTypeIndex() << " " 
				<< "(" << std::setw(3) << ti.getRow() << "," << std::setw(3) << ti.getCol() 
				<< ")  " << ti.getName() << endl;
		}
		cout.flags(saveflags);
	}
	/// \brief Filtered list of all tiles.
	void getFilteredTileList(const string& inString) {
		using std::cout;
		using std::endl;
		std::ios_base::fmtflags saveflags = cout.flags();
		
		// Replace all asterisks in the input string with regex wildcards (".*")
		std::string userPattern = regex_replace(inString, boost::regex("(?<!\\.)\\*"), ".*");
		
		// Build regex
		boost::regex sTileNameRegEx(userPattern, boost::regex::perl | boost::regex::icase
													| boost::regex::no_except);
		// Check regex syntax
		if (sTileNameRegEx.empty()) {
			cout << "Bad regular expression. Try again." << endl;
			return;
		}
		
		// Print matching sites
		boost::smatch smatches;
		int matchCount = 0;
		for (TileIndex i(0); i < mTiles.getTileCount(); i++) {
			const architecture::TileInfo& ti = mTiles.getTileInfo(i);
			if (regex_match(std::string(ti.getName()), smatches, sTileNameRegEx)) {
				matchCount++;
				cout << std::setw(6) << i << " type " << std::setw(3) << ti.getTypeIndex() << " " 
					<< "(" << std::setw(3) << ti.getRow() << "," << std::setw(3) << ti.getCol() 
					<< ")  " << ti.getName() << endl;
			}
		}
		
		// Output # matches
		cout << endl << matchCount << " match" << (matchCount == 1 ? "." : "es.") << endl;
		
		cout.flags(saveflags);
	}
	/// \brief List of all sites.
	void getSiteList() {
		using std::cout;
		using std::endl;
		std::ios_base::fmtflags saveflags = cout.flags();
		
		for (SiteIndex i(0); i < mSites.getSiteCount(); i++) {
			const architecture::Site& site = mSites.getSite(i);
			cout << std::setw(7) << i << " " << site.getName() << endl;
		}
		cout.flags(saveflags);
	}
	/// \brief Filtered list of all sites.
	void getFilteredSiteList(const string& inString) {
		using std::cout;
		using std::endl;
		std::ios_base::fmtflags saveflags = cout.flags();
		
		// Replace all asterisks in the input string with regex wildcards (".*")
		std::string userPattern = regex_replace(inString, boost::regex("(?<!\\.)\\*"), ".*");
		
		// Build regex
		boost::regex sSiteNameRegEx(userPattern, boost::regex::perl | boost::regex::icase
													| boost::regex::no_except);
		// Check regex syntax
		if (sSiteNameRegEx.empty()) {
			cout << "Bad regular expression. Try again." << endl;
			return;
		}
		
		// Print matching sites
		boost::smatch smatches;
		int matchCount = 0;
		for (SiteIndex i(0); i < mSites.getSiteCount(); i++) {
			const architecture::Site& site = mSites.getSite(i);
			if (regex_match(site.getName(), smatches, sSiteNameRegEx)) {
				matchCount++;
				cout << std::setw(7) << i << " " << site.getName() << endl;
			}
		}
		
		// Output # matches
		cout << endl << matchCount << " match" << (matchCount == 1 ? "." : "es.") << endl;
		
		cout.flags(saveflags);
	}
	/// brief Site detail function.
	void getSiteDetail(const string& inString) {
		using std::cout;
		using std::endl;
		std::ios_base::fmtflags saveflags = cout.flags();
		SiteIndex siteIndex(0);
		std::string siteName;
		boost::smatch smatches;
		if (regex_match(inString, smatches, ArchitectureBrowser::sIndexRegEx)) {
			siteIndex = boost::lexical_cast<SiteIndex>(smatches[1]);
			if (siteIndex >= mSites.getSiteCount()) {
				cout << "Site index " << siteIndex << " out of range! " 
					<< mSites.getSiteCount() << endl;
				return;
			}
		} else if (regex_match(inString, smatches, ArchitectureBrowser::sNameRegEx)) {
			siteIndex = mSites.findSiteIndex(inString);
			if (siteIndex == SiteIndex(-1)) {
				cout << "Site not found: " << inString << endl;
				return;
			}
		} else {
			cout << "Input invalid: " << inString << endl;
			return;
		}
		const architecture::Site& site = mSites.getSite(siteIndex);
		cout << "Site: " << site.getName() << endl;
		cout << "\tTile: " << mTiles.getTileInfo(site.getTileIndex()).getName()
			<< " (" << site.getTileIndex() << ") " << endl;
		cout << "\tFlags: " << std::hex << site.getFlags() << endl;
		cout << std::dec << "\tPinMapPtr" << endl << "\t\t";
		for (unsigned int i = 0; i < (site.getPinMapPtr())->getSize(); i++) {
			cout << " " << (*site.getPinMapPtr())[i];
		}
		cout << endl;
		const PrimitiveDef& primDef = *site.getPrimitiveDefPtr();
		cout << "\tPrimitive Definition: " << primDef.getName() << endl;
		cout.flags(saveflags);
		
	}
	/// brief Sites of a particular type function.
	void displaySitesOfType(const string& inString) {
		using std::cout;
		using std::endl;
		std::ios_base::fmtflags saveflags = cout.flags();
		
		for (SiteIndex i(0); i < mSites.getSiteCount(); i++) {
			const architecture::Site& site = mSites.getSite(i);
			const PrimitiveDef* siteTypePtr = site.getPrimitiveDefPtr();
			if (siteTypePtr->getName() == inString)
				cout << std::setw(7) << i << " " << site.getName() << endl;
		}
		
		cout.flags(saveflags);
	}
	/// \brief Tilewire detail function.
	void getTilewireDetail(const string& inString) {
		using std::cout;
		using std::endl;
		Tilewire tw = stringToTilewire(inString);
		if (tw == Tilewire::sInvalid) {
			cout << "Specified tilewire not found: " << inString << endl;
			return;
		}
		TileTypeIndex tti = mTiles.getTileInfo(tw.getTileIndex()).getTypeIndex();
		architecture::ExtendedWireInfo ewi(mDB, tw);
		cout << ewi << endl;
		const architecture::WireInfo& wireinfo = mTiles.getWireInfo(tti, tw.getWireIndex());
		cout << "\toffset: " << wireinfo.getArcOffset() << endl;
		cout << "\tattributes: ";
		cout << (wireinfo.isHidden() ? "HIDDEN " : "");
		cout << (wireinfo.isInput() ? "INPUT " : "");
		cout << (wireinfo.isOutput() ? "OUTPUT " : "");
		cout << (wireinfo.isRemote() ? "REMOTE " : "");
		cout << (wireinfo.isGlobal() ? "GLOBAL " : "");
		cout << (wireinfo.isRemoteNodeCapable() ? "REMOTE_NODE_CAPABLE " : "");
		cout << (wireinfo.isRemoteArcCapable() ? "REMOTE_ARC_CAPABLE " : "") << endl;
		const architecture::WireArray& sinks = wireinfo.getSinks();
		cout << "\tsinks: ";
		for (uint32_t i = 0; i < sinks.getSize(); i++) cout << sinks[i] << " ";
		cout << endl;
		
		const architecture::WireArray& sources = wireinfo.getSources();
		cout << "\tsources: ";
		for (uint32_t i = 0; i < sources.getSize(); i++) cout << sources[i] << " ";
		cout << endl;
		
		const architecture::WireArray& isinks = wireinfo.getIrregularSinks();
		cout << "\tirregular sinks: ";
		for (uint32_t i = 0; i < isinks.getSize(); i++) cout << isinks[i] << " ";
		cout << endl;
		
		const architecture::WireArray& isources = wireinfo.getIrregularSources();
		cout << "\tirregular sources: ";
		for (uint32_t i = 0; i < isources.getSize(); i++) cout << isources[i] << " ";
		cout << endl;
		
		const architecture::WireArray& tsinks = wireinfo.getTiedSinks();
		cout << "\ttied sinks: ";
		for (uint32_t i = 0; i < tsinks.getSize(); i++) cout << tsinks[i] << " ";
		cout << endl;
		
		const architecture::WireArray& tsources = wireinfo.getTiedSources();
		cout << "\ttied sources: ";
		for (uint32_t i = 0; i < tsources.getSize(); i++) cout << tsources[i] << " ";
		cout << endl;

		/// \todo SegmentReference with internal segment data?
		
		architecture::TilewireVector segmentwires;
		mDB.expandSegment(tw, segmentwires);
		cout << "\t";
		for (uint32_t i = 0; i < segmentwires.size(); i++) cout << segmentwires[i] << " ";
		cout << endl;
	}
	/// \brief Display source Tilewires of a segment.
	void displaySegmentSources(const string& inString) {
		using std::cout;
		using std::endl;
		
		Tilewire tw = stringToTilewire(inString);
		if (tw == Tilewire::sInvalid) {
			cout << inString << " is not a valid Tilewire" << endl;
			return;
		}
		architecture::TilewireVector segmentWires;
		architecture::TilewireVector sourceWires;
		cout << "Sources for segment containing: " << inString << endl;
		mDB.expandSegment(tw, segmentWires);
		for (uint32_t i = 0; i < segmentWires.size(); i++) {
			architecture::ExtendedWireInfo ewi(mDB, segmentWires[i]);
			cout << "|\t" << ewi << endl;
			sourceWires.clear();
			mDB.expandTilewireSources(segmentWires[i], sourceWires, false, true, false, false);
			for (uint32_t j = 0; j < sourceWires.size(); j++) {
				architecture::ExtendedWireInfo ewi2(mDB, sourceWires[j]);
				cout << "|<--\t\t" << ewi2 << endl;
			}
			sourceWires.clear();
			mDB.expandTilewireSources(segmentWires[i], sourceWires, false, false, true, false);
			for (uint32_t j = 0; j < sourceWires.size(); j++) {
				architecture::ExtendedWireInfo ewi2(mDB, sourceWires[j]);
				cout << "|<--\t\t" << ewi2 << " - IRREGULAR" << endl;
			}
			sourceWires.clear();
			mDB.expandTilewireSources(segmentWires[i], sourceWires, false, false, false, true);
			for (uint32_t j = 0; j < sourceWires.size(); j++) {
				architecture::ExtendedWireInfo ewi2(mDB, sourceWires[j]);
				cout << "|<--\t\t" << ewi2 << " - ROUTETHROUGH" << endl;
			}
		}
	}
	/// \brief Display sink Tilewires of a segment.
	void displaySegmentSinks(const string& inString) {
		using std::cout;
		using std::endl;
		
		Tilewire tw = stringToTilewire(inString);
		if (tw == Tilewire::sInvalid) {
			cout << inString << " is not a valid Tilewire" << endl;
			return;
		}
		architecture::TilewireVector segmentWires;
		architecture::TilewireVector sinkWires;
		cout << "Sinks for segment containing: " << inString << endl;
		mDB.expandSegment(tw, segmentWires);
		for (uint32_t i = 0; i < segmentWires.size(); i++) {
			architecture::ExtendedWireInfo ewi(mDB, segmentWires[i]);
			cout << "|\t" << ewi << endl;
			sinkWires.clear();
			mDB.expandTilewireSinks(segmentWires[i], sinkWires, false, true, false, false);
			for (uint32_t j = 0; j < sinkWires.size(); j++) {
				architecture::ExtendedWireInfo ewi2(mDB, sinkWires[j]);
				cout << "|-->\t\t" << ewi2 << endl;
			}
			sinkWires.clear();
			mDB.expandTilewireSinks(segmentWires[i], sinkWires, false, false, true, false);
			for (uint32_t j = 0; j < sinkWires.size(); j++) {
				architecture::ExtendedWireInfo ewi2(mDB, sinkWires[j]);
				cout << "|-->\t\t" << ewi2 << " - IRREGULAR" << endl;
			}
			sinkWires.clear();
			mDB.expandTilewireSinks(segmentWires[i], sinkWires, false, false, false, true);
			for (uint32_t j = 0; j < sinkWires.size(); j++) {
				architecture::ExtendedWireInfo ewi2(mDB, sinkWires[j]);
				cout << "|-->\t\t" << ewi2 << " - ROUTETHROUGH" << endl;
			}
		}
	}
	/// \brief Display segment connectivity.
	void displayFullSegment(const string& inString) {
		using std::cout;
		using std::endl;
		
		std::map<string, std::vector<string> > sourcemap;
		std::map<string, std::vector<string> > sinkmap;
		std::vector<string> segmentstrings;
		
		Tilewire tw = stringToTilewire(inString);
		if (tw == Tilewire::sInvalid) {
			cout << inString << " is not a valid Tilewire" << std::endl;
			return;
		}
		architecture::TilewireVector segmentwires;
		architecture::TilewireVector arcwires;
		mDB.expandSegment(tw, segmentwires);
		for (uint32_t i = 0; i < segmentwires.size(); i++) {
			architecture::ExtendedWireInfo ewi(mDB, segmentwires[i]);
			std::stringstream segwire;
			segwire << ewi;
			string segwirestring = segwire.str();
			segmentstrings.push_back(segwirestring);
			
			sinkmap[segwirestring] = std::vector<string>();
			sourcemap[segwirestring] = std::vector<string>();

			arcwires.clear();
			mDB.expandTilewireSinks(segmentwires[i], arcwires);
			for (uint32_t j = 0; j < arcwires.size(); j++) {
				architecture::ExtendedWireInfo ewi2(mDB, arcwires[j]);
				std::stringstream arcwire;
				arcwire << ewi2;
				
				sinkmap[segwirestring].push_back(arcwire.str());
			}
			arcwires.clear();
			mDB.expandTilewireSources(segmentwires[i], arcwires);
			for (uint32_t j = 0; j < arcwires.size(); j++) {
				architecture::ExtendedWireInfo ewi2(mDB, arcwires[j]);
				std::stringstream arcwire;
				arcwire << ewi2;
				
				sourcemap[segwirestring].push_back(arcwire.str());
			}
		}
		cout << "Displaying segment containing: " << inString << endl;
		/*for (uint32_t i = 0; i < segmentstrings.size(); i++) {
			//cout << "\t\t" << segmentstrings[i] << endl;
			for (uint32_t j = 0; j < sourcemap[segmentstrings[i]].size(); j++) {
				cout << "|<--\t" << sourcemap[segmentstrings[i]][j] << endl;
			}
			cout << "|\t\t" << segmentstrings[i] << endl;
			for (uint32_t j = 0; j < sinkmap[segmentstrings[i]].size(); j++) {
				cout << "|-->\t\t\t" << sinkmap[segmentstrings[i]][j] << endl;
			}
		}*/
		for (uint32_t i = 0; i < segmentstrings.size(); i++) {
			cout << "|" << endl << "| " << segmentstrings[i] << endl;
			
			for (uint32_t j = 0; j < sourcemap[segmentstrings[i]].size(); j++) {
				cout << "|	<--- " << sourcemap[segmentstrings[i]][j] << endl;
			}
			for (uint32_t j = 0; j < sinkmap[segmentstrings[i]].size(); j++) {
				cout << "|	---> " << sinkmap[segmentstrings[i]][j] << endl;
			}
		}
		
	}
	/// \brief Display segment connectivity.
	void displayFullSegmentFANCY(const string& inString) {
		using std::cout;
		using std::endl;
		
		std::map<string, std::vector<string> > sourcemap;
		std::map<string, std::vector<string> > sinkmap;
		std::vector<string> segmentstrings;
		
		Tilewire tw = stringToTilewire(inString);
		architecture::TilewireVector segmentwires;
		architecture::TilewireVector arcwires;
		mDB.expandSegment(tw, segmentwires);
		for (uint32_t i = 0; i < segmentwires.size(); i++) {
			architecture::ExtendedWireInfo ewi(mDB, segmentwires[i]);
			std::stringstream segwire;
			
			segwire << ewi;
			string segwirestring = segwire.str();
			segmentstrings.push_back(segwirestring);
			
			sinkmap[segwirestring] = std::vector<string>();
			sourcemap[segwirestring] = std::vector<string>();

			arcwires.clear();
			mDB.expandTilewireSinks(segmentwires[i], arcwires);
			for (uint32_t j = 0; j < arcwires.size(); j++) {
				architecture::ExtendedWireInfo ewi2(mDB, arcwires[j]);
				std::stringstream arcwire;
				
				arcwire << ewi2;
				
				sinkmap[segwirestring].push_back(arcwire.str());
			}
			arcwires.clear();
			mDB.expandTilewireSources(segmentwires[i], arcwires);
			for (uint32_t j = 0; j < arcwires.size(); j++) {
				architecture::ExtendedWireInfo ewi2(mDB, arcwires[j]);
				std::stringstream arcwire;
				
				arcwire << ewi2;
				
				sourcemap[segwirestring].push_back(arcwire.str());
			}
		}
		
		uint32_t sourcewidth = 0;
		uint32_t segwidth = 0;
		uint32_t sinkwidth = 0;
		
		std::map<string, std::vector<string> >::iterator e;
		std::map<string, std::vector<string> >::iterator p;
		
		e = sourcemap.end();
		for (p = sourcemap.begin(); p != e; p++) {
			// get length of longest string
			for (uint32_t j = 0; j < p->second.size(); j++) {
				if (p->second[j].size() > sourcewidth) sourcewidth = p->second[j].size();
			}
		}
		e = sinkmap.end();
		for (p = sinkmap.begin(); p != e; p++) {
			// get length of longest string
			for (uint32_t j = 0; j < p->second.size(); j++) {
				if (p->second[j].size() > sinkwidth) sinkwidth = p->second[j].size();
			}
		}
		for (uint32_t i = 0; i < segmentstrings.size(); i++) {
			if (segmentstrings[i].size() > segwidth) segwidth = segmentstrings[i].size();
		}
		sourcewidth += 2;
		segwidth += 2;
		sinkwidth += 2;
		
		for (uint32_t i = 0; i < segmentstrings.size(); i++) {
			std::map<string, std::vector<string> >::iterator p;
			p = sourcemap.find(segmentstrings[i]);
			if (p != sourcemap.end() && p->second.size() > 0) {
				for (uint32_t j = 0; j < p->second.size() - 1; j++) {
					cout << " " << p->second[j];
					for (uint32_t k = p->second[j].size(); k < sourcewidth; k++) cout << " ";
					cout << "|" << endl;
				}
				cout << " " << p->second[p->second.size() - 1];
				for (uint32_t k = p->second[p->second.size() - 1].size();
					k < sourcewidth; k++) cout << " ";
				cout << "+ ";
			} else {
				for (uint32_t k = 0; k < sourcewidth + 3; k++) cout << " "; 
			}
			
			cout << segmentstrings[i];
			for (uint32_t k = segmentstrings[i].size(); k < segwidth; k++) cout << " ";
			p = sinkmap.find(segmentstrings[i]);
			if (p != sinkmap.end()) {
				if (p->second.size() > 0) cout << " + " << p->second[0] << endl;
				else cout << endl;
				for (uint32_t j = 1; j < p->second.size(); j++) {
					for (uint32_t k = 0; k < sourcewidth + segwidth + 4; k++) cout << " ";
					cout << "| " << p->second[j] << endl;
				}
			}
		}
		
	}
	/// \brief List all wires for a given tile type.
	void getWireListForTileType(const string& inString) {
		using std::cout;
		using std::endl;
		
		TileTypeIndex tileType(0);
		TileIndex tileIndex(0);
		string tileTypeName;
		string tileName;
		
		boost::smatch smatches;
		if (regex_match(inString, smatches, ArchitectureBrowser::sIndexRegEx)) {
			tileType = boost::lexical_cast<TileTypeIndex>(smatches[1]);
			if (tileType >= mTiles.getTileTypeCount()) {
				cout << "Type index " << tileType << " out of range! " 
					<< mTiles.getTileTypeCount() << endl;
				return;
			}
			tileTypeName = mTiles.getTileTypeName(tileType);
		} else if (regex_match(inString, smatches, ArchitectureBrowser::sNameRegEx)) {
			bool flag = false;
			tileTypeName = smatches[1];
			for ( ; tileType < mTiles.getTileTypeCount(); tileType++) {
				if (tileTypeName == mTiles.getTileTypeName(tileType)) {
					flag = true;
					break;
				}
			}
			tileName = tileTypeName;
			for ( ; tileIndex < mTiles.getTileCount(); tileIndex++) {
				const architecture::TileInfo& tileInfo = mTiles.getTileInfo(tileIndex);
				if (tileTypeName == tileInfo.getName()) {
					tileType = tileInfo.getTypeIndex();
					tileTypeName = mTiles.getTileTypeName(tileType);
					flag = true;
					break;
				}
			}
			if (!flag) {
				cout << inString << " not found." << endl;
				return;
			}
		} else {
			cout << "Input invalid: " << inString << endl;
			return;
		}
		
		const architecture::Array<const architecture::WireInfo>& wireInfoArray = 
			mTiles.getWireInfo(tileType);
		
		cout << "Wires for tile type: " << tileTypeName << " (" << tileType << ")" << endl;
		for (uint32_t i = 0; i < wireInfoArray.getSize(); i++) {
			const WireInfo& wireInfo = wireInfoArray[i];
			cout << "\t" << i << ": " << wireInfo.getName();
			cout << (wireInfo.isHidden() ? "  (HIDDEN) " : "");
			cout << (wireInfo.isInput() ? "  (INPUT) " : "");
			cout << (wireInfo.isOutput() ? "  (OUTPUT) " : "");
			cout << (wireInfo.isRemote() ? "  (REMOTE) " : "");
			cout << (wireInfo.isGlobal() ? "  (GLOBAL) " : "");
			cout << (wireInfo.isRemoteNodeCapable() ? "  (REMOTE_NODE_CAPABLE) " : "");
			cout << (wireInfo.isRemoteArcCapable() ? "  (REMOTE_ARC_CAPABLE) " : "");
			cout << endl;
		}
	}
	void enumerateSiteTypes() {
		using std::cout;
		using std::endl;
		cout << "Device Site Types:" << endl;
		
		const architecture::Array<const PrimitiveDef>& siteTypes = mSites.getSiteTypes();
		for (uint32_t i = 0; i < siteTypes.getSize(); i++) {
			cout << siteTypes[i].getName() << endl;
		}
	}
public:
	/// \brief Convert user input to a Tilewire.
	Tilewire stringToTilewire(const string& inString) {
		TileIndex tile;
		WireIndex wire;
		
		boost::smatch smatches;
		boost::smatch smatches2;
		if (regex_match(inString, smatches, ArchitectureBrowser::sTilewireRegEx)) {
			// wire@tile form
			string tilePart = smatches[2];
			boost::to_upper(tilePart);
			if (regex_match(tilePart, smatches2, ArchitectureBrowser::sIndexRegEx)) {
				tile = boost::lexical_cast<TileIndex>(smatches2[1]);
			} else if (regex_match(tilePart, smatches2, ArchitectureBrowser::sNameRegEx)) {
				tile = mTiles.findTileIndex(tilePart);
			} else {
				return Tilewire::sInvalid;
			}
			if (tile >= mTiles.getTileCount()) return Tilewire::sInvalid;
			TileTypeIndex tti = mTiles.getTileInfo(tile).getTypeIndex();
			string wirePart = smatches[1];
			boost::to_upper(wirePart);
			if (regex_match(wirePart, smatches2, ArchitectureBrowser::sIndexRegEx)) {
				wire = boost::lexical_cast<WireIndex>(smatches2[1]);
			} else if (regex_match(wirePart, smatches2, ArchitectureBrowser::sNameRegEx)) {
				wire = mTiles.findWireIndex(tti, wirePart);
			} else {
				return Tilewire::sInvalid;
			}
			if (wire >= mTiles.getWireInfo(tti).getSize()) return Tilewire::sInvalid;
			return Tilewire(tile, wire);
		}
		else {
			return Tilewire::sInvalid;
		}
	}
	/// \brief Print the full menu of browser functions.
	void printFullMenu();
	/// \brief Prepare function map to lookup what the user asks for.
	void setupFunctionMap();
	
};

} // namespace torc

#endif // TORC_UTILS_ARCHITECTUREBROWSER_HPP
