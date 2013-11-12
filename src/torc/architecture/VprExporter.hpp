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
/// \brief Header for the DDB class.

#ifndef TORC_ARCHITECTURE_VPREXPORTER_HPP
#define TORC_ARCHITECTURE_VPREXPORTER_HPP

#include "torc/architecture/DDB.hpp"
#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include <fstream>

namespace torc {
namespace architecture {

	/// \brief Device database exporter for the University of Toronto's VPR.
	/// \note This is still experimental and under development.
	class VprExporter {
	protected:
	// types
		typedef std::fstream fstream; ///< \brief Imported type name.
		typedef std::string string; ///< \brief Imported type name.
		typedef std::vector<string> StringVector; ///< \brief Vector type.
		typedef std::vector<uint32_t> Uint32Vector; ///< \brief Vector type.
		typedef xilinx::WireFlags WireFlags; ///< \brief Imported type name.
		typedef xilinx::WireCount WireCount; ///< \brief Imported type name.
		typedef xilinx::WireIndex WireIndex; ///< \brief Imported type name.
		typedef xilinx::TileCount TileCount; ///< \brief Imported type name.
		typedef xilinx::TileIndex TileIndex; ///< \brief Imported type name.
		typedef xilinx::TileCol TileCol; ///< \brief Imported type name.
		typedef xilinx::TileRow TileRow; ///< \brief Imported type name.
		typedef xilinx::TileTypeIndex TileTypeIndex; ///< \brief Imported type name.
	// classes
		/// \brief VPR node temporary class.
		class Node {
		public:
			Tilewire mTilewire;
			uint32_t mIndex;
			TileCol mMinCol;
			TileRow mMinRow;
			TileCol mMaxCol;
			TileRow mMaxRow;
			uint32_t mColRange;
			uint32_t mRowRange;
			WireFlags mFlags;
			Uint32Vector mEdges;
			Node(void) : mTilewire(), mIndex(0), mMinCol(-1), mMinRow(-1), mMaxCol(0), mMaxRow(0)
				{}
		};
	// members
		/// \brief Reference to the database object.
		DDB& mDDB;
		/// \brief Reference to the database Tiles object.
		const Tiles& mTiles;
		/// \brief Reference to the database Segments object.
		const Segments& mSegments;
		/// \brief Minimum row bounds.
		TileRow mMinTileRow;
		/// \brief Minimum column bounds.
		TileCol mMinTileCol;
		/// \brief Maximum row bounds.
		TileRow mMaxTileRow;
		/// \brief Maximum column bounds.
		TileCol mMaxTileCol;
		/// \brief Output file stream for VPR data.
		fstream mStream;
	public:
	// constructors
		/// \brief Public constructor.
		VprExporter(DDB& inDDB) : mDDB(inDDB), mTiles(mDDB.getTiles()), 
			mSegments(mDDB.getSegments()), 
			mMinTileRow(10), mMinTileCol(10), 
			mMaxTileRow(10), mMaxTileCol(10) {}
	// functions
		/// \brief Exports routing graph information for VPR v6.0.
		void operator()(void);
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_VPREXPORTER_HPP
