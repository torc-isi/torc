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
/// \brief Header for the Sites class.

#ifndef TORC_ARCHITECTURE_SITES_HPP
#define TORC_ARCHITECTURE_SITES_HPP

#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/architecture/DigestStream.hpp"
#include "torc/architecture/Array.hpp"
#include "torc/architecture/Tilewire.hpp"
#include "torc/architecture/Package.hpp"
#include "torc/architecture/PrimitivePin.hpp"
#include "torc/architecture/PrimitiveElementPin.hpp"
#include "torc/architecture/PrimitiveElement.hpp"
#include "torc/architecture/PrimitiveConn.hpp"
#include "torc/architecture/PrimitiveDef.hpp"
#include "torc/architecture/Site.hpp"
#include "torc/architecture/DDBConsoleStreams.hpp"
#include <boost/cstdint.hpp>
#include <map>
#include <set>

namespace torc {
namespace architecture {

	/// \brief Site type and population data for the family and the device.
	/// \details Each device has a collection of logic sites.  Those sites are instantiations of 
	///		family defined site types, with each instance also including a mapping from site pin 
	///		to Tilewire.
	class Sites : DDBConsoleStreams {
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
		typedef std::map<string, SiteIndex> SiteNameToSiteIndexMap;
		/// \brief Map from package name to package index.
		typedef std::map<string, PackageIndex> PackageNameToPackageIndexMap;
	// members
		/// \brief The site types for this family.
		Array<const PrimitiveDef> mSiteTypes;
		/// \brief The site pin maps for this family.
		Array2D<const WireIndex> mPrimitivePinMaps;
		/// \brief The logic sites for this device.
		Array<const Site> mSites;
		/// \brief The mapping from site name to site index for this device.
		SiteNameToSiteIndexMap mSiteNameToSiteIndex;
		/// \brief The packages for this device
		Array<const Package> mPackages;
		/// \brief The mapping from package name to package index for this device.
		PackageNameToPackageIndexMap mPackageNameToPackageIndex;
	// functions
		/// \brief Read the primitive types for the family.
		size_t readPrimitiveTypes(DigestStream& inStream);
		/// \brief Read the site pin mappings for the family.
		size_t readPrimitivePinMaps(DigestStream& inStream);
		/// \brief Read the sites for the device.
		size_t readSites(DigestStream& inStream);
		/// \brief Read the packages for the device.
		size_t readPackages(DigestStream& inStream);
		/// \brief Activate the specified device package.
		void activatePackage(const string& inName) {
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
			for(SiteIndex i; i < getSiteCount(); i++) {
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
		SiteIndex findSiteIndex(const string& inName) const {
			SiteNameToSiteIndexMap::const_iterator p = mSiteNameToSiteIndex.find(inName);
			return (p == mSiteNameToSiteIndex.end()) 
				? SiteIndex(SiteIndex::undefined()) : p->second;
		}
		/// \brief Returns the package index for the given package name.
		PackageIndex findPackageIndex(const string& inName) const {
			PackageNameToPackageIndexMap::const_iterator p 
				= mPackageNameToPackageIndex.find(inName);
			return (p == mPackageNameToPackageIndex.end()) 
				? PackageIndex(PackageIndex::undefined()) : p->second;
		}
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_SITES_HPP
