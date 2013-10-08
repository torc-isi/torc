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
/// \brief Source for the DDB class.

#include "torc/architecture/DDB.hpp"
#include "torc/architecture/DigestStream.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/architecture/OutputStreamHelpers.hpp"
#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>

namespace torc {
namespace architecture {

	void DDB::initialize(const string& inDeviceName, const string& inPackageName) {

		// pull in symbols from other namespaces
		using std::string;
		using std::cout;
		using std::cerr;
		using std::clog;
		using std::endl;
		using std::fstream;
		using boost::uint32_t;

		try {

			// track statistics
			uint32_t familyDatabaseBytesRead = 0;
			uint32_t deviceDatabaseBytesRead = 0;

			// open the device database
			mDeviceName = inDeviceName;
			string deviceFilename = inDeviceName + ".db";
			boost::to_lower(deviceFilename);
			boost::filesystem::path devicePath 
				= torc::common::DirectoryTree::getDevicesPath() / deviceFilename;
			clog << "Reading device " << inDeviceName << " (" << devicePath << ")..." << endl;
			DigestStream deviceDatabaseStream(devicePath);
			deviceDatabaseStream.exceptions(fstream::eofbit | fstream::failbit | fstream::badbit);
			deviceDatabaseBytesRead += mDeviceVersion.readVersions(deviceDatabaseStream, true);
			deviceDatabaseBytesRead += readFamilyName(deviceDatabaseStream);

			// open the family database
			boost::filesystem::path familyPath = devicePath.parent_path() / (mFamilyName + ".db");
			cout << "\tReading family " << mFamilyName << " (" << familyPath << ")..." << endl;
			DigestStream familyDatabaseStream(familyPath);
			familyDatabaseStream.exceptions(fstream::eofbit | fstream::failbit | fstream::badbit);

			// populate the database from family and device data
			familyDatabaseBytesRead += mFamilyVersion.readVersions(familyDatabaseStream, false);
			familyDatabaseBytesRead += mTiles.readTileTypes(familyDatabaseStream);
			familyDatabaseBytesRead += mTiles.readTileWireInfo(familyDatabaseStream);
			deviceDatabaseBytesRead += readSpeedGrades(deviceDatabaseStream);
			deviceDatabaseBytesRead += mSites.readPackages(deviceDatabaseStream);
			deviceDatabaseBytesRead += mTiles.readTileMap(deviceDatabaseStream);
			deviceDatabaseBytesRead += mSegments.readTilewireSegments(deviceDatabaseStream);
			bool extendedAnchorTileCount 
				= mDeviceVersion.getFormat() == torc::common::DottedVersion(1, 0, 1);
			deviceDatabaseBytesRead 
				+= mSegments.readSegments(deviceDatabaseStream, extendedAnchorTileCount);
			deviceDatabaseBytesRead += mSegments.readIrregularArcs(deviceDatabaseStream);
			familyDatabaseBytesRead += mSites.readPrimitiveTypes(familyDatabaseStream);
			familyDatabaseBytesRead += mSites.readPrimitivePinMaps(familyDatabaseStream);
			deviceDatabaseBytesRead += mSites.readSites(deviceDatabaseStream);
			cout << "Read " << familyDatabaseBytesRead << " bytes from " << mFamilyName << endl;
			cout << "Read " << deviceDatabaseBytesRead << " bytes from " << mDeviceName << endl;
			// automatically size the wire and arc usage objects
			mWireUsage.autosize();
			mArcUsage.autosize();

			// activate the specified package if any
			if(inPackageName.length()) mSites.activatePackage(inPackageName);
		}

		catch(fstream::failure f) {
			cerr << "An unprocessed ifstream::failure exception was thrown: " << f.what() << endl;
			throw;
		}

	}

	size_t DDB::readFamilyName(DigestStream& inStream) {
		// prepare to read from the stream
		size_t bytesReadOffset = inStream.getBytesRead();
		char scratch[1 << 10];			// scratch read buffer
		boost::uint16_t nameLength = 0;	// length of family name

		// read the section header
		string sectionName;
		inStream.readSectionHeader(sectionName);
		/// \todo Throw a proper exception.
		if(sectionName != ">>>> Family >>>>") throw -1;

		// read the family name
		inStream.read(nameLength);
		if(nameLength > sizeof(scratch)) throw -1;
		inStream.read(scratch, nameLength);
		scratch[nameLength] = 0;
		mFamilyName = scratch;

		// return the number of bytes read
		return inStream.getBytesRead() - bytesReadOffset;
	}

	size_t DDB::readSpeedGrades(DigestStream& inStream) {
		// prepare to read from the stream
		size_t bytesReadOffset = inStream.getBytesRead();
		char scratch[1 << 10];			// scratch read buffer
		uint16_t nameLength = 0;	// length of family name

		// read the section header
		string sectionName;
		inStream.readSectionHeader(sectionName);
		/// \todo Throw a proper exception.
		if(sectionName != ">>>> Speeds >>>>") throw -1;

		// read the speed grade count
		uint16_t speedGradeCount;
		inStream.read(speedGradeCount);
		std::cout << "\tReading " << speedGradeCount << " speed grade" 
			<< (speedGradeCount != 1 ? "s" : "") << " (";
		// loop through each speed grade
		for(uint16_t i = 0; i < speedGradeCount; i++) {
			// read the speed grade
			inStream.read(nameLength);
			if(nameLength > sizeof(scratch)) throw -1;
			inStream.read(scratch, nameLength);
			scratch[nameLength] = 0;
			mSpeedGrades.push_back(scratch);
			std::cout << scratch << (i + 1 < speedGradeCount ? ", " : "");
		}
		std::cout << ") ... " << std::endl;

		// return the number of bytes read
		return inStream.getBytesRead() - bytesReadOffset;
	}

	void DDB::expandSegment(const Tilewire& inTilewire, TilewireVector& outTilewires, 
		EExpandDirection inExpandDirection) {

		// look up the segment specified by this tile and wire
		WireIndex wireIndex = inTilewire.getWireIndex();
		TileIndex tileIndex = inTilewire.getTileIndex();
		const Segments::SegmentReference& compactSegmentReference 
			= mSegments.getTilewireSegment(inTilewire);
		// undefined segments are not real in the current device
		if(!compactSegmentReference.isDefined()) return;

		// initialize the wire array with the segment information or the trivial wire
		Array<Segments::CompactSegmentTilewire> trivialSegmentReference;
		if(compactSegmentReference.isTrivial()) {
			trivialSegmentReference.setSize(1);
			trivialSegmentReference[0] = Segments::CompactSegmentTilewire(wireIndex, 
				TileOffset(tileIndex));
		}
		const Array<Segments::CompactSegmentTilewire>& segmentReference 
			= compactSegmentReference.isTrivial() ? trivialSegmentReference 
				: mSegments.mCompactSegments[compactSegmentReference.getCompactSegmentIndex()];

		// iterate through each wire in the compact segment
		TileIndex anchorTileIndex = compactSegmentReference.getAnchorTileIndex();
		Array<Segments::CompactSegmentTilewire>::const_iterator csp 
			= segmentReference.begin();
		Array<Segments::CompactSegmentTilewire>::const_iterator cse 
			= segmentReference.end();
		bool sinkward = (inExpandDirection & eExpandDirectionSinkward) != 0;
		bool sourceward = (inExpandDirection & eExpandDirectionSourceward) != 0;
		while(csp < cse) {
			// get information about the current tilewire
			const Segments::CompactSegmentTilewire& compactSegmentTilewire = *csp++;
			WireIndex csWireIndex = compactSegmentTilewire.getWireIndex();
			TileOffset csTileOffset = compactSegmentTilewire.getTileOffset();
			TileIndex csTileIndex = csTileOffset + anchorTileIndex;
			// push each wire into the segment
			outTilewires.push_back(Tilewire(csTileIndex, csWireIndex));
			// look up tile and wire information
			const TileInfo& tileInfo = mTiles.getTileInfo(csTileIndex);
			const WireInfo& wireInfo = mTiles.getWireInfo(tileInfo.getTypeIndex(), csWireIndex);
			// include tied sinks
			if(sinkward) {
				const WireArray& tiedSinks = wireInfo.getTiedSinks();
				WireArray::const_iterator p = tiedSinks.begin();
				WireArray::const_iterator e = tiedSinks.end();
				while(p < e) expandSegment(Tilewire(csTileIndex, *p++), outTilewires, 
					inExpandDirection);
			}
			// include tied sources
			if(sourceward) {
				const WireArray& tiedSources = wireInfo.getTiedSources();
				WireArray::const_iterator p = tiedSources.begin();
				WireArray::const_iterator e = tiedSources.end();
				while(p < e) expandSegment(Tilewire(csTileIndex, *p++), outTilewires, 
					inExpandDirection);
			}
		}
	}

	void DDB::expandTilewireSinks(const Tilewire& inTilewire, TilewireVector& outSinks, 
		bool inUseTied, bool inUseRegular, bool inUseIrregular, bool inUseRoutethrough) {
		// get information about the starting tile and wire
		WireIndex wireIndex = inTilewire.getWireIndex();
		TileIndex tileIndex = inTilewire.getTileIndex();
		const TileInfo& tileInfo = mTiles.getTileInfo(tileIndex);
		const WireInfo& wireInfo = mTiles.getWireInfo(tileInfo.getTypeIndex(), wireIndex);

		// add the regular sinks
		if(inUseRegular) {
			const WireArray& sinks = wireInfo.getSinks();
			WireArray::const_iterator sp = sinks.begin();
			WireArray::const_iterator se = sinks.end();
			while(sp < se) outSinks.push_back(Tilewire(tileIndex, *sp++));
		}

		// add any irregular sinks
		if(inUseIrregular) {
			const WireArray& irregularSinks = wireInfo.getIrregularSinks();
			WireArray::const_iterator isp = irregularSinks.begin();
			WireArray::const_iterator ise = irregularSinks.end();
			while(isp < ise) {
				WireIndex sinkWireIndex = *isp++;
				// arcs to undefined segments are likewise undefined
				const Segments::SegmentReference& segment 
					= mSegments.getTilewireSegment(Tilewire(tileIndex, sinkWireIndex));
				if(!segment.isDefined()) continue;
				// determine whether this irregular arc exists
				const Segments::IrregularArc* irregularArc = mSegments.getIrregularArc(tileIndex, 
					wireIndex, sinkWireIndex);
				if(irregularArc != 0 && !segment.isDefined()) {
					std::cerr << "WARNING: Irregular arc " << inTilewire << " >> " 
						<< Tilewire(tileIndex, sinkWireIndex) 
						<< " is defined, but the sink segment is undefined." << std::endl;
				}
				if(irregularArc == 0) continue;
				// the arc exists, so add it to the list
				outSinks.push_back(Tilewire(tileIndex, sinkWireIndex));
			}
		}

		// add the routethrough sinks
		if(inUseRoutethrough) {
			const WireArray& routethroughSinks = wireInfo.getRoutethroughSinks();
			WireArray::const_iterator rsp = routethroughSinks.begin();
			WireArray::const_iterator rse = routethroughSinks.end();
			while(rsp < rse) outSinks.push_back(Tilewire(tileIndex, *rsp++));
		}
	}

	void DDB::expandTilewireSources(const Tilewire& inTilewire, TilewireVector& outSources, 
		bool inUseTied, bool inUseRegular, bool inUseIrregular, bool inUseRoutethrough) {
		// get information about the starting tile and wire
		WireIndex wireIndex = inTilewire.getWireIndex();
		TileIndex tileIndex = inTilewire.getTileIndex();
		const TileInfo& tileInfo = mTiles.getTileInfo(tileIndex);
		const WireInfo& wireInfo = mTiles.getWireInfo(tileInfo.getTypeIndex(), wireIndex);

		// add the regular sources
		if(inUseRegular) {
			const WireArray& sources = wireInfo.getSources();
			WireArray::const_iterator sp = sources.begin();
			WireArray::const_iterator se = sources.end();
			while(sp < se) outSources.push_back(Tilewire(tileIndex, *sp++));
		}

		// add any irregular sources
		if(inUseIrregular) {
			const WireArray& irregularSources = wireInfo.getIrregularSources();
			WireArray::const_iterator isp = irregularSources.begin();
			WireArray::const_iterator ise = irregularSources.end();
			while(isp < ise) {
				WireIndex sourceWireIndex = *isp++;
				// arcs to undefined segments are likewise undefined
				const Segments::SegmentReference& segment 
					= mSegments.getTilewireSegment(Tilewire(tileIndex, sourceWireIndex));
				if(!segment.isDefined()) continue;
				// determine whether this irregular arc exists
				const Segments::IrregularArc* irregularArc = mSegments.getIrregularArc(tileIndex, 
					sourceWireIndex, wireIndex);
				if(irregularArc != 0 && !segment.isDefined()) {
					std::cerr << "WARNING: Irregular arc " << inTilewire << " >> " 
						<< Tilewire(tileIndex, sourceWireIndex) 
						<< " is defined, but the source segment is undefined." << std::endl;
				}
				if(irregularArc == 0) continue;
				// the arc exists, so add it to the list
				outSources.push_back(Tilewire(tileIndex, sourceWireIndex));
			}
		}

		// add the routethrough sinks
		if(inUseRoutethrough) {
			const WireArray& routethroughSources = wireInfo.getRoutethroughSources();
			WireArray::const_iterator rsp = routethroughSources.begin();
			WireArray::const_iterator rse = routethroughSources.end();
			while(rsp < rse) outSources.push_back(Tilewire(tileIndex, *rsp++));
		}
	}

	void DDB::expandSegmentSinks(const Tilewire& inTilewire, ArcVector& outArcs, 
			EExpandDirection inExpandDirection, bool inUseTied, bool inUseRegular, 
			bool inUseIrregular, bool inUseRoutethrough) {
		// expand the segment
		TilewireVector segment;
		expandSegment(inTilewire, segment, inExpandDirection);
		// expand all of the arcs
		TilewireVector::const_iterator sep = segment.begin();
		TilewireVector::const_iterator see = segment.end();
		while(sep < see) {
			// expand the tilewire sinks
			const Tilewire& tilewire = *sep++;
			TilewireVector sinks;
			expandTilewireSinks(tilewire, sinks, inUseTied, inUseRegular, inUseIrregular, 
				inUseRoutethrough);
			// rewrite the sinks as arcs
			TilewireVector::const_iterator sip = sinks.begin();
			TilewireVector::const_iterator sie = sinks.end();
			while(sip < sie) outArcs.push_back(Arc(tilewire, *sip++));
		}
	}

	void DDB::expandSegmentSources(const Tilewire& inTilewire, ArcVector& outArcs, 
			EExpandDirection inExpandDirection, bool inUseTied, bool inUseRegular, 
			bool inUseIrregular, bool inUseRoutethrough) {
		// expand the segment
		TilewireVector segment;
		expandSegment(inTilewire, segment, inExpandDirection);
		// expand all of the arcs
		TilewireVector::const_iterator sep = segment.begin();
		TilewireVector::const_iterator see = segment.end();
		while(sep < see) {
			// expand the tilewire sources
			const Tilewire& tilewire = *sep++;
			TilewireVector sources;
			expandTilewireSources(tilewire, sources, inUseTied, inUseRegular, inUseIrregular, 
				inUseRoutethrough);
			// rewrite the sources as arcs
			TilewireVector::const_iterator sop = sources.begin();
			TilewireVector::const_iterator soe = sources.end();
			while(sop < soe) outArcs.push_back(Arc(*sop++, tilewire));
		}
	}

} // namespace architecture
} // namespace torc
