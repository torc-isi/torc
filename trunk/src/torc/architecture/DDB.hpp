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
/// \brief Header for the DDB class.

#ifndef TORC_ARCHITECTURE_DDB_HPP
#define TORC_ARCHITECTURE_DDB_HPP

#include "torc/architecture/Versions.hpp"
#include "torc/architecture/Segments.hpp"
#include "torc/architecture/Sites.hpp"
#include "torc/architecture/Tiles.hpp"
#include "torc/architecture/Arc.hpp"
#include "torc/architecture/ArcUsage.hpp"
#include "torc/architecture/WireUsage.hpp"
#include "torc/architecture/ExtendedWireInfo.hpp"
#include "torc/architecture/DeviceDesignator.hpp"
#include "torc/architecture/DDBStreamHelper.hpp"
#include <string>
#include <map>

namespace torc {
namespace architecture {

	/// \brief Device database, including complete wiring and logic support.
	class DDB {
	protected:
	// types
		typedef std::string string; ///< \brief Imported type name.
		typedef std::vector<string> StringVector; ///< \brief Imported type name.
		typedef xilinx::SiteIndex SiteIndex; ///< \brief Imported type name.
		typedef xilinx::WireIndex WireIndex; ///< \brief Imported type name.
		typedef xilinx::TileIndex TileIndex; ///< \brief Imported type name.
		typedef xilinx::TileOffset TileOffset; ///< \brief Imported type name.
		typedef xilinx::TileTypeIndex TileTypeIndex; ///< \brief Imported type name.
	// members
		/// \brief The name of the device.
		string mDeviceName;
		/// \brief The name of the family.
		string mFamilyName;
		/// \brief The device database version.
		Versions mDeviceVersion;
		/// \brief The family database version.
		Versions mFamilyVersion;
		/// \brief The supported speed grades.
		StringVector mSpeedGrades;
		/// \brief The device segment data.
		Segments mSegments;
		/// \brief The family and device site data.
		Sites mSites;
		/// \brief The family and device tile data.
		Tiles mTiles;
		/// \brief The design arc usage.
		ArcUsage mArcUsage;
		/// \brief The design wire usage.
		WireUsage mWireUsage;
	// functions
		/// \brief Reads the family name from a device DigestStream.
		size_t readFamilyName(class DigestStream& inStream);
		/// \brief Reads the supported speed grades from a device DigestStream.
		size_t readSpeedGrades(class DigestStream& inStream);
		/// \biref Initializes the database.
		void initialize(const string& inDeviceName, const string& inPackageName);
	public:
	// enumerations
		/// \brief Enumeration to indicate in which directions segments should be expanded.
		/// \details Segment expansion direction only makes sense when device databases include 
		///		tied wires (or "fake pips").  When tied wires are present, the caller can request 
		///		that segment expansions include no tied wires, include tied wires in the sinkward 
		///		direction, include tied wires in the sourceward direction, or include tied wires in 
		///		both directions.
		enum EExpandDirection { 
			eExpandDirectionNone = 0, eExpandDirectionSinkward = 1, eExpandDirectionSourceward = 2, 
			eExpandDirectionBidirectional = eExpandDirectionSinkward | eExpandDirectionSourceward
		};
	// types
	// constructors
		/// \brief Public constructor.
		/// \param inDeviceName The name of the device to open.  This name should not include any 
		///		package or speed grade information, nor any extension.  Examples are xc2vp100, 
		///		xc4vfx60, xc5vlx30, et cetera.
		DDB(const string& inDeviceName, const string& inPackageName = "") : mArcUsage(mTiles), 
			mWireUsage(mTiles) { initialize(inDeviceName, inPackageName); }
		/// \brief Public constructor.
		DDB(const DeviceDesignator& inDeviceDesignator) : mArcUsage(mTiles), mWireUsage(mTiles) {
			initialize(inDeviceDesignator.getDeviceName(), inDeviceDesignator.getDevicePackage());
		}
	// iterators
	// accessors
		/// \brief Returns the device name.
		const string& getDeviceName(void) const { return mDeviceName; }
		/// \brief Returns the family name.
		const string& getFamilyName(void) const { return mFamilyName; }
		/// \brief Returns a constant reference to a vector of speed grades.
 		const StringVector& getSpeedGrades(void) const { return mSpeedGrades; }
		/// \brief Returns a constant reference to the device segment data.
		const Segments& getSegments(void) const { return mSegments; }
		/// \brief Returns a constant reference to the family and device site data.
		const Sites& getSites(void) const { return mSites; }
		/// \brief Returns a constant reference to the family and device tile data.
		const Tiles& getTiles(void) const { return mTiles; }
		/// \brief Returns a constant reference to the design arc usage.
		ArcUsage& getArcUsage(void) { return mArcUsage; }
		/// \brief Returns a constant reference to the design wire usage.
		WireUsage& getWireUsage(void) { return mWireUsage; }
	// functions
		/// \brief Clears all arc and wire usage, effectively restaring with an empty device.
		void clearUsage(void) {
			mWireUsage.clear();
			mArcUsage.clear();
		}
		/// \brief Marks the arc and all of its source and sink segment wires as used.
		void useArc(const Arc& inArc) {
			mArcUsage.use(inArc);
			useSegment(inArc.getSourceTilewire());
			useSegment(inArc.getSinkTilewire());
		}
		/// \brief Marks the arc and all of its source and sink segment wires as unused.
		void releaseArc(const Arc& inArc, bool releaseSource = true, bool releaseSink = true) {
			mArcUsage.release(inArc);
			if(releaseSource) releaseSegment(inArc.getSourceTilewire());
			if(releaseSink) releaseSegment(inArc.getSinkTilewire());
		}
		/// \brief Marks all wires on the segment as used.
		void useSegment(const Tilewire& inTilewire) {
			TilewireVector segment;
			expandSegment(inTilewire, segment);
			TilewireVector::const_iterator p = segment.begin();
			TilewireVector::const_iterator e = segment.end();
			while(p < e) mWireUsage.use(*p++);
		}
		/// \brief Marks all wires on the segment as unused.
		void releaseSegment(const Tilewire& inTilewire) {
			TilewireVector segment;
			expandSegment(inTilewire, segment);
			TilewireVector::const_iterator p = segment.begin();
			TilewireVector::const_iterator e = segment.end();
			while(p < e) mWireUsage.release(*p++);
		}
		/// \brief Expands the given tilewire's segment.
		/// \details Given a tilewire belonging to a segment, this function populates the supplied 
		///		vector with all of the tilewires on the segment.  An optional parameter allows the 
		///		expansion to continue in the sinkward or sourceward directions in cases where tied 
		///		sources or sinks are available.
		void expandSegment(const Tilewire& inTilewire, TilewireVector& outTilewires, 
			EExpandDirection inExpandDirection = eExpandDirectionNone);
		/// \brief Expands the given tilewire's arc sinks.
		/// \details The optional inUseTied, inUseRegular, inUseIrregular, and inUseRoutethrough 
		///		flags determine whether the corresponding arc types will be included.  Note that 
		///		tied arcs (or "fake pips") are not currently available in the databases.
		void expandTilewireSinks(const Tilewire& inTilewire, TilewireVector& outSinks, 
			bool inUseTied = true, bool inUseRegular = true, bool inUseIrregular = true, 
			bool inUseRoutethrough = true);
		/// \brief Expands the given tilewire's arc sources.
		/// \details The optional inUseTied, inUseRegular, inUseIrregular, and inUseRoutethrough 
		///		flags determine whether the corresponding arc types will be included.  Note that 
		///		tied arcs (or "fake pips") are not currently available in the databases.
		void expandTilewireSources(const Tilewire& inTilewire, TilewireVector& outSources, 
			bool inUseTied = true, bool inUseRegular = true, bool inUseIrregular = true, 
			bool inUseRoutethrough = true);
		/// \brief Expands all sink arcs for the given tilewire's segment.
		/// \details The optional inUseTied, inUseRegular, inUseIrregular, and inUseRoutethrough 
		///		flags determine whether the corresponding arc types will be included.  Note that 
		///		tied arcs (or "fake pips") are not currently available in the databases.
		void expandSegmentSinks(const Tilewire& inTilewire, ArcVector& outSinks, 
			EExpandDirection inExpandDirection = eExpandDirectionNone, bool inUseTied = true, 
			bool inUseRegular = true, bool inUseIrregular = true, bool inUseRoutethrough = true);
		/// \brief Expands all source arcs for the given tilewire's segment.
		/// \details The optional inUseTied, inUseRegular, inUseIrregular, and inUseRoutethrough 
		///		flags determine whether the corresponding arc types will be included.  Note that 
		///		tied arcs (or "fake pips") are not currently available in the databases.
		void expandSegmentSources(const Tilewire& inTilewire, ArcVector& outSources, 
			EExpandDirection inExpandDirection = eExpandDirectionNone, bool inUseTied = true, 
			bool inUseRegular = true, bool inUseIrregular = true, bool inUseRoutethrough = true);
		/// \brief Returns the tilewire for the specified site and pin name pair.
		Tilewire sitePinToTilewire(const std::string& inSiteName, 
			const std::string& inPinName) {
			SiteIndex siteIndex = mSites.findSiteIndex(inSiteName);
			if(siteIndex.isUndefined()) return Tilewire::sInvalid;
			return mSites.getSite(siteIndex).getPinTilewire(inPinName);
		}
		/// \brief Returns the arc for the specified pip tile and wires.
		Arc tilePipToArc(const std::string& inTileName, const std::string& inSourceWireName, 
			const std::string& inSinkWireName) {
			TileIndex tileIndex = mTiles.findTileIndex(inTileName);
			if(tileIndex.isUndefined()) return Arc();
			const TileInfo& tileInfo = mTiles.getTileInfo(tileIndex);
			TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
			WireIndex sourceWireIndex = mTiles.findWireIndex(tileTypeIndex, inSourceWireName);
			if(sourceWireIndex.isUndefined()) return Arc();
			WireIndex sinkWireIndex = mTiles.findWireIndex(tileTypeIndex, inSinkWireName);
			if(sinkWireIndex.isUndefined()) return Arc();
			return Arc(Tilewire(tileIndex, sourceWireIndex), Tilewire(tileIndex, sinkWireIndex));
		}
		/// \brief Returns the tilewire for the specified tile and wire names.
		/// \todo Add to unit tests.
		Tilewire lookupTilewire(const std::string& inTileName, const std::string& inWireName) {
			TileIndex tileIndex = mTiles.findTileIndex(inTileName);
			if(tileIndex.isUndefined()) return Tilewire::sInvalid;
			const TileInfo& tileInfo = mTiles.getTileInfo(tileIndex);
			TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
			WireIndex wireIndex = mTiles.findWireIndex(tileTypeIndex, inWireName);
			if(wireIndex.isUndefined()) return Tilewire::sInvalid;
			return Tilewire(tileIndex, wireIndex);
		}
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_DDB_HPP
