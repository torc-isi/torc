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
/// \brief Header for the ExtendedWireInfo class.

#ifndef TORC_ARCHITECTURE_EXTENDEDWIREINFO_HPP
#define TORC_ARCHITECTURE_EXTENDEDWIREINFO_HPP

#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/architecture/Tilewire.hpp"

namespace torc {
namespace architecture {

	class DDB;

	/// \brief Verbose encapsulation of a wire's information.
	class ExtendedWireInfo {
	protected:
	// types
		typedef xilinx::WireIndex WireIndex; ///< \brief Imported type name.
		typedef xilinx::WireFlags WireFlags; ///< \brief Imported type name.
		typedef xilinx::TileIndex TileIndex; ///< \brief Imported type name.
		typedef xilinx::TileTypeIndex TileTypeIndex; ///< \brief Imported type name.
		typedef xilinx::TileRow TileRow; ///< \brief Imported type name.
		typedef xilinx::TileCol TileCol; ///< \brief Imported type name.
	// members
		/// \brief The device database reference.
		const DDB& mDDB;
	public:
	// members
		/// \brief The wire index.
		WireIndex mWireIndex;
		/// \brief The wire name.
		const char* mWireName;
		/// \brief The wire flags.
		WireFlags mWireFlags;
		/// \brief The tile index.
		TileIndex mTileIndex;
		/// \brief The tile name.
		const char* mTileName;
		/// \brief The tile type index.
		TileTypeIndex mTileTypeIndex;
		/// \brief The tile type name.
		const char* mTileTypeName;
		/// \brief The tile row.
		TileRow mTileRow;
		/// \brief The tile column.
		TileCol mTileCol;
	// constructors
		/// \brief Basic constructor.
		ExtendedWireInfo(const DDB& inDDB) : mDDB(inDDB), 
			mWireIndex(WireIndex(WireIndex::undefined())), mWireName(0), mWireFlags(0), 
			mTileIndex(TileIndex(TileIndex::undefined())), mTileName(0), 
			mTileTypeIndex(TileTypeIndex(TileTypeIndex::undefined())), mTileTypeName(0), 
			mTileRow(0), mTileCol(0) {}
		/// \brief Tilewire constructor.
		ExtendedWireInfo(const DDB& inDDB, const Tilewire& inTilewire) : mDDB(inDDB) {
			set(inTilewire);
		}
	// functions
		/// \brief Configure the extended info fields.
		void set(const Tilewire& inTilewire);
	// operators
		/// \brief Assignment operator.
		const Tilewire& operator =(const Tilewire& rhs) { set(rhs); return rhs; }
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_EXTENDEDWIREINFO_HPP
