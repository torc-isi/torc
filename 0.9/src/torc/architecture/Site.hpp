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
/// \brief Header for the Site class.

#ifndef TORC_ARCHITECTURE_SITE_HPP
#define TORC_ARCHITECTURE_SITE_HPP

#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/architecture/PrimitiveDef.hpp"
#include "torc/architecture/Tilewire.hpp"

namespace torc {
namespace architecture {

	/// \brief Encapsulation of a device logic site.
	class Site {
	protected:
	// friends
		/// \brief The Sites class has access to our internals.
		friend class Sites;
	// types
		typedef std::string string;						///< \brief Imported type name.
		typedef xilinx::PinIndex PinIndex;				///< \brief Imported type name.
		typedef xilinx::TileIndex TileIndex;			///< \brief Imported type name.
		typedef xilinx::WireIndex WireIndex;			///< \brief Imported type name.
		typedef xilinx::SiteFlags SiteFlags;			///< \brief Imported type name.
	public:
	// types
		/// \brief Array of constant wire indexes.
		typedef Array<const WireIndex> WireIndexArray;
	protected:
	// members
		/// \brief The logic site name.
		string mName;
		/// \brief Pointer to the associated primitive definition.
		const PrimitiveDef* mPrimitiveDefPtr;
		/// \brief The index of the containing tile.
		TileIndex mTileIndex;
		/// \brief The flags associated with this site.
		SiteFlags mFlags;
		/// \brief The pin to wire map for this site.
		const WireIndexArray* mPinMapPtr;
	// constructors
		/// \brief Protected constructor.
		Site(const string& inName, const PrimitiveDef& inPrimitiveDef, 
			TileIndex inTileIndex, SiteFlags inFlags, const WireIndexArray& inPinMap) 
			: mName(inName), mPrimitiveDefPtr(&inPrimitiveDef), mTileIndex(inTileIndex), 
			mFlags(inFlags), mPinMapPtr(&inPinMap) {
		}
	public:
	// constructors
		/// \brief Null constructor (for use in containers only).
		Site(void) : mName(), mPrimitiveDefPtr(0), mTileIndex(), mFlags(), mPinMapPtr(0) {}
	// functions
		/// \brief Returns the Tilewire associated with the specified pin name.
		const Tilewire getPinTilewire(const std::string& inName) const {
			if(mPrimitiveDefPtr == 0) return Tilewire::sInvalid;
			PinIndex pinIndex = mPrimitiveDefPtr->findPinIndexByName(inName);
			if(pinIndex.isUndefined()) return Tilewire::sInvalid;
			if(mPinMapPtr == 0) return Tilewire::sInvalid;
			return Tilewire(mTileIndex, (*mPinMapPtr)[pinIndex]);
		}
	// accessors
		/// \brief Returns the site name.
		const string& getName(void) const { return mName; }
		/// \brief Returns a pointer to the associated primitive definition.
		const PrimitiveDef* getPrimitiveDefPtr(void) const { return mPrimitiveDefPtr; }
		/// \brief Returns the index of the containing tile.
		TileIndex getTileIndex(void) const { return mTileIndex; }
		/// \brief Returns the flags associated with this site.
		SiteFlags getFlags(void) const { return mFlags; }
		/// \brief Returns the pin to wire map for this site.
		const WireIndexArray* getPinMapPtr(void) const { return mPinMapPtr; }
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_SITE_HPP
