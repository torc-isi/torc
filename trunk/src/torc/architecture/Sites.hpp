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
/// \brief Header for the Sites class.

#ifndef TORC_ARCHITECTURE_SITES_HPP
#define TORC_ARCHITECTURE_SITES_HPP

#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/architecture/DigestStream.hpp"
#include "torc/architecture/Array.hpp"
#include "torc/architecture/Tilewire.hpp"
#include <boost/cstdint.hpp>
#include <map>
#include <set>

/// \todo remove this #include
#include <iostream>

namespace torc {
namespace architecture {

	/// \brief Site type and population data for the family and the device.
	/// \details Each device has a collection of logic sites.  Those sites are instantiations of 
	///		family defined site types, with each instance also including a mapping from site pin 
	///		to Tilewire.
	class Sites {
	// friends
		/// \brief The database has access to our protected functions.
		friend class DDB;
	protected:
	// types
		typedef std::string string;						///< \brief Imported type name.
		typedef xilinx::TileIndex TileIndex;			///< \brief Imported type name.
		typedef xilinx::WireIndex WireIndex;			///< \brief Imported type name.
		typedef xilinx::PinFlags PinFlags;				///< \brief Imported type name.
		typedef xilinx::PinCount PinCount;				///< \brief Imported type name.
		typedef xilinx::SiteCount SiteCount;			///< \brief Imported type name.
		typedef xilinx::SiteIndex SiteIndex;			///< \brief Imported type name.
		typedef xilinx::SiteFlags SiteFlags;			///< \brief Imported type name.
		typedef xilinx::SiteTypeCount SiteTypeCount;	///< \brief Imported type name.
		typedef xilinx::SiteTypeIndex SiteTypeIndex;	///< \brief Imported type name.
		typedef xilinx::PackageCount PackageCount;		///< \brief Imported type name.
		typedef xilinx::PackageIndex PackageIndex;		///< \brief Imported type name.
		typedef xilinx::PadCount PadCount;				///< \brief Imported type name.
		typedef xilinx::PadIndex PadIndex;				///< \brief Imported type name.
		/// \brief Map from site name to site index.
		typedef std::map<std::string, SiteIndex> SiteNameToSiteIndexMap;
		/// \brief Map from package name to package index.
		typedef std::map<std::string, PackageIndex> PackageNameToPackageIndexMap;
	// nested classes
	public:
		/// \brief Encapsulation of the site index, pin name, and pin flags for a package.
		class Pad {
			friend class Sites;
			SiteIndex mSiteIndex;
			std::string mName;
			SiteFlags mFlags;
			Pad(SiteIndex inSiteIndex, const std::string& inName, SiteFlags inFlags) 
				: mSiteIndex(inSiteIndex), mName(inName), mFlags(inFlags) {}
		public:
			SiteIndex getSiteIndex(void) const { return mSiteIndex; }
			const std::string& getName(void) const { return mName; }
			SiteFlags getFlags(void) const { return mFlags; }
			Pad(void) : mSiteIndex(), mName(), mFlags() {};
		};
		/// \brief Encapsulation of the packages for a device.
		class Package {
			friend class Sites;
			typedef std::map<std::string, PadIndex> PadNameToPadIndexMap;
			std::string mName;
			Array<const Pad> mPads;
			PadNameToPadIndexMap mPadNameToPadIndex;
			Package(const std::string& inName) : mName(inName), mPads() {}
			Array<const Pad>& getPads(void) { return mPads; }
		public:
			const std::string& getName(void) const { return mName; }
			const Array<const Pad>& getPads(void) const { return mPads; }
			PadIndex findPadIndexByName(const std::string& inName) const {
				PadNameToPadIndexMap::const_iterator p = mPadNameToPadIndex.find(inName);
				return (p == mPadNameToPadIndex.end()) 
					? PadIndex(PadIndex::undefined()) : p->second;
			}
			Package(void) : mName(), mPads() {}
		};
		/// \bried Encapsulation of pin directionality
		class PinDirection {
		public:
			enum { ePinDirectionNone = 0, ePinDirectionInput = 2, ePinDirectionOutput = 4 };
		};
		/// \brief Encapsulation of a site pin's name and flags.
		class SitePin : public PinDirection {
			friend class Sites;
			std::string mName;
			PinFlags mFlags;
			SitePin(const std::string& inName, PinFlags inFlags) : mName(inName), mFlags(inFlags) {}
		public:
			const std::string& getName(void) const { return mName; }
			PinFlags getFlags(void) const { return mFlags; }
			SitePin(void) : mName(), mFlags() {};
			bool isInput(void) const { return (mFlags & ePinDirectionInput) != 0; }
			bool isOutput(void) const { return (mFlags & ePinDirectionOutput) != 0; }
		};
		/// \brief Encapsulation of an element pin's name and flags.
		class PrimitiveElement;
		class ElementPin : public PinDirection {
			friend class Sites;
			const PrimitiveElement* mElementPtr;
			std::string mName;
			PinFlags mFlags;
			ElementPin(const PrimitiveElement* inElementPtr, const std::string& inName, 
				PinFlags inFlags) : mElementPtr(inElementPtr), mName(inName), mFlags(inFlags) {}
		public:
			const PrimitiveElement* getElementPtr(void) const { return mElementPtr; }
			const std::string& getName(void) const { return mName; }
			PinFlags getFlags(void) const { return mFlags; }
			ElementPin(void) : mElementPtr(0), mName(), mFlags() {};
			bool isInput(void) const { return (mFlags & ePinDirectionInput) != 0; }
			bool isOutput(void) const { return (mFlags & ePinDirectionOutput) != 0; }
		};
		/// \brief Encapsulation of a PrimitiveDef element.
		class PrimitiveElement {
		public:
			typedef Array<const ElementPin> ElementPinArray;
			typedef std::set<std::string> StringSet;
		protected:
			friend class Sites;
			typedef xilinx::PinIndex PinIndex;
			typedef std::map<std::string, PinIndex> PinNameToPinIndexMap;
			std::string mName;
			ElementPinArray mPins;
			StringSet mCfg;
			bool mIsBel;
			PinNameToPinIndexMap mPinNameToPinIndex;
			Array<const ElementPin>& getPins(void) { return mPins; }
		public:
			const std::string& getName(void) const { return mName; }
			const ElementPinArray& getPins(void) const { return mPins; }
		        const StringSet& getCfgs(void) const { return mCfg; }
		        bool isBel(void) const { return mIsBel; }
			PinIndex findPinIndexByName(const std::string& inName) const {
				PinNameToPinIndexMap::const_iterator p = mPinNameToPinIndex.find(inName);
				return (p == mPinNameToPinIndex.end()) 
					? PinIndex(PinIndex::undefined()) : p->second;
			}
			PrimitiveElement(void) : mName(), mPins(), mPinNameToPinIndex() {}
		};
		/// \brief Encapsulation of a PrimitiveDef internal connection.
		class PrimitiveConn {
		protected:
			friend class Sites;
			typedef std::vector<const ElementPin*> ElementPinPtrVector;
			const ElementPin* mSourcePtr;
			ElementPinPtrVector mSinks;
		public:
			const ElementPin* getSourcePtr(void) const { return mSourcePtr; }
			const ElementPinPtrVector& getSinks(void) const { return mSinks; }
		};
		/// \brief Encapsulation of a site type's name and pins.
		class PrimitiveDef {
		public:
			typedef Array<const SitePin> SitePinArray;
			typedef Array<const PrimitiveElement> PrimitiveElementArray;
			typedef Array<const PrimitiveConn> PrimitiveConnArray;
		protected:
			friend class Sites;
			typedef xilinx::PinIndex PinIndex;
			typedef std::map<std::string, PinIndex> PinNameToPinIndexMap;
			std::string mName;
			SitePinArray mPins;
			PrimitiveElementArray mElements;
			PrimitiveConnArray mConnections;
			PinNameToPinIndexMap mPinNameToPinIndex;
			Array<const SitePin>& getPins(void) { return mPins; }
		public:
			const std::string& getName(void) const { return mName; }
			const SitePinArray& getPins(void) const { return mPins; }
			const PrimitiveElementArray& getElements(void) const { return mElements; }
			const PrimitiveConnArray& getConnections(void) const { return mConnections; }
			PinIndex findPinIndexByName(const std::string& inName) const {
				PinNameToPinIndexMap::const_iterator p = mPinNameToPinIndex.find(inName);
				return (p == mPinNameToPinIndex.end()) 
					? PinIndex(PinIndex::undefined()) : p->second;
			}
			PrimitiveDef(void) : mName(), mPins(), mPinNameToPinIndex() {}
		};
		/// \brief Encapsulation of a logic site.
		class Site {
			friend class Sites;
		public:
			typedef Array<const WireIndex> WireIndexArray;
		protected:
			typedef xilinx::PinIndex PinIndex;
			std::string mName;
			const PrimitiveDef* mPrimitiveDefPtr;
			TileIndex mTileIndex;
			SiteFlags mFlags;
			const WireIndexArray* mPinMapPtr;
			Site(const std::string& inName, const PrimitiveDef& inPrimitiveDef, 
				TileIndex inTileIndex, SiteFlags inFlags, const WireIndexArray& inPinMap) 
				: mName(inName), mPrimitiveDefPtr(&inPrimitiveDef), mTileIndex(inTileIndex), 
				mFlags(inFlags), mPinMapPtr(&inPinMap) {
			}
		public:
			const std::string& getName(void) const { return mName; }
			const PrimitiveDef* getPrimitiveDefPtr(void) const { return mPrimitiveDefPtr; }
			TileIndex getTileIndex(void) const { return mTileIndex; }
			SiteFlags getFlags(void) const { return mFlags; }
			const WireIndexArray* getPinMapPtr(void) const { return mPinMapPtr; }
			Site(void) : mName(), mPrimitiveDefPtr(0), mTileIndex(), mFlags(), mPinMapPtr(0) {}
			const Tilewire getPinTilewire(const std::string& inName) const {
				if(mPrimitiveDefPtr == 0) return Tilewire::sInvalid;
				PinIndex pinIndex = mPrimitiveDefPtr->findPinIndexByName(inName);
				if(pinIndex.isUndefined()) return Tilewire::sInvalid;
				if(mPinMapPtr == 0) return Tilewire::sInvalid;
				return Tilewire(mTileIndex, (*mPinMapPtr)[pinIndex]);
			}
		};
	protected:
	// members
		/// \brief The site types for this family.
		Array<const PrimitiveDef> mSiteTypes;
		/// \brief The site pin maps for this family.
		Array2D<const WireIndex> mSitePinMaps;
		/// \brief The logic sites for this device.
		Array<const Site> mSites;
		/// \brief The mapping from site name to site index for this device.
		SiteNameToSiteIndexMap mSiteNameToSiteIndex;
		/// \brief The packages for this device
		Array<const Package> mPackages;
		/// \brief The mapping from package name to package index for this device.
		PackageNameToPackageIndexMap mPackageNameToPackageIndex;
	// functions
		/// \brief Read the site types for the family.
		size_t readSiteTypes(DigestStream& inStream);
		/// \brief Read the site pin mappings for the family.
		size_t readSitePinMaps(DigestStream& inStream);
		/// \brief Read the sites for the device.
		size_t readSites(DigestStream& inStream);
		/// \brief Read the packages for the device.
		size_t readPackages(DigestStream& inStream);
		/// \brief Activate the specified device package.
		void activatePackage(const std::string& inName) {
			activatePackage(findPackageIndex(inName));
		}
		/// \brief Activate the specified device package.
		void activatePackage(PackageIndex inPackageIndex) {
			// look up the package
			if(inPackageIndex >= mPackages.getSize()) return;
			const Package& package = mPackages[inPackageIndex];
			// iterate through the pads
			const Array<const Pad>& pads = package.getPads();
			Array<const Pad>::const_iterator p = pads.begin();
			Array<const Pad>::const_iterator e = pads.end();
			while(p < e) {
				// update the site name and flags
				Site& site = const_cast<Site&>(mSites[p->mSiteIndex]);
				site.mName = p->mName;
				site.mFlags = p->mFlags;
				p++;
			}
			// update the name to index map
			mSiteNameToSiteIndex.clear();
			for (SiteIndex i; i < getSiteCount(); i++) {
				Site& site = const_cast<Site&>(mSites[i]);
				mSiteNameToSiteIndex[site.mName] = i;
			}
		}
	public:
	// accessors
		/// \brief Returns the site count for this device.
		SiteCount getSiteCount(void) const { return SiteCount(mSites.getSize()); }
		/// \brief Returns the site type count for this family.
		SiteTypeCount getSiteTypeCount(void) const { return SiteTypeCount(mSiteTypes.getSize()); }
		/// \brief Returns the site types for this family.
		const Array<const PrimitiveDef>& getSiteTypes(void) const { return mSiteTypes; }
		/// \brief Returns the sites for this family.
		const Array<const Site>& getSites(void) const { return mSites; }
		/// \brief Returns the site for the specified index.
		const Site& getSite(SiteIndex inSiteIndex) const { return mSites[inSiteIndex]; }
		/// \brief Returns the packages for this family.
		const Array<const Package>& getPackages(void) const { return mPackages; }
	// functions
		/// \brief Returns the site index for the given site name.
		SiteIndex findSiteIndex(const std::string& inName) const {
			SiteNameToSiteIndexMap::const_iterator p = mSiteNameToSiteIndex.find(inName);
			return (p == mSiteNameToSiteIndex.end()) 
				? SiteIndex(SiteIndex::undefined()) : p->second;
		}
		/// \brief Returns the package index for the given package name.
		PackageIndex findPackageIndex(const std::string& inName) const {
			PackageNameToPackageIndexMap::const_iterator p 
				= mPackageNameToPackageIndex.find(inName);
			return (p == mPackageNameToPackageIndex.end()) 
				? PackageIndex(PackageIndex::undefined()) : p->second;
		}
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_SITES_HPP
