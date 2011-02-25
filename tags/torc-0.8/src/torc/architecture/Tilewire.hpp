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
/// \brief Header for the Tilewire class.

#ifndef TORC_ARCHITECTURE_TILEWIRE_HPP
#define TORC_ARCHITECTURE_TILEWIRE_HPP

#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include <vector>

namespace torc {
namespace architecture {

	// Pack Tilewire objects tightly.
	/// \todo Have to justify the packing decision, and its impact on memory footprint versus 
	///		performance.
	#ifdef __GNUC__
		#pragma pack(push, 2)
	#endif

	/// \brief Encapsulation of a device tile and wire pair.
	/// \details A tilewire uniquely identifies a physical wire in the device, as defined by its 
	///		tile index and wire index.  Tilewires are used extensively by the device database, and 
	///		by the router and tracer classes.
	class Tilewire {
	protected:
	// types
		/// \brief Imported type name.
		typedef xilinx::TileIndex TileIndex;
		/// \brief Imported type name.
		typedef xilinx::WireIndex WireIndex;
	// members
		/// \brief The tile index.
		TileIndex mTileIndex;
		/// \brief The wire index.
		WireIndex mWireIndex;
	public:
	// constructors
		/// \brief Null constructor.
		/// \details The tilewire will be set to Tilewire::sInvalid.
		Tilewire(void) : mTileIndex(-1), mWireIndex(-1) {}
		/// \brief Public constructor.
		Tilewire(const TileIndex& inTileIndex, const WireIndex& inWireIndex) 
			: mTileIndex(inTileIndex), mWireIndex(inWireIndex) {}
		/// \brief Copy constructor.
		Tilewire(const Tilewire& inTilewire) 
			: mTileIndex(inTilewire.mTileIndex), mWireIndex(inTilewire.mWireIndex) {}
	// accessors
		/// \brief Returns the tile index.
		const TileIndex& getTileIndex(void) const { return mTileIndex; }
		/// \brief Returns the wire index.
		const WireIndex& getWireIndex(void) const { return mWireIndex; }
		/// \brief Sets the tile index.
		void setTileIndex(const TileIndex& inTileIndex) { mTileIndex = inTileIndex; }
		/// \brief Sets the wire index.
		void setWireIndex(const WireIndex& inWireIndex) { mWireIndex = inWireIndex; }
	// operators
		/// \brief Equality operator.
		bool operator ==(const Tilewire& rhs) const 
			{ return mTileIndex == rhs.mTileIndex && mWireIndex == rhs.mWireIndex; }
		/// \brief Assignment operator.
		const Tilewire& operator =(const Tilewire& rhs) 
			{ mTileIndex = rhs.mTileIndex; mWireIndex = rhs.mWireIndex; return *this; }
		/// \brief Comparison operator.
		/// \details This operator facilitates ordering in containers.
		bool operator <(const Tilewire& rhs) const {
			if(mTileIndex < rhs.mTileIndex) return true;
			if(mTileIndex == rhs.mTileIndex && mWireIndex < rhs.mWireIndex) return true;
			return false;
		}
	// functions
		bool isInvalid(void) const {
			return mTileIndex == TileIndex(-1) || mWireIndex == WireIndex(-1);
		}
	// friends
		/// \brief Return a hash value for the specified tilewire.
		friend std::size_t hash_value(const Tilewire& inTilewire);
	// static
		static const Tilewire sInvalid;
	};

	#ifdef __GNUC__
		#pragma pack(pop)
	#endif

	/// \brief Vector of Tilewire objects.
	typedef std::vector<Tilewire> TilewireVector;

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_TILEWIRE_HPP
