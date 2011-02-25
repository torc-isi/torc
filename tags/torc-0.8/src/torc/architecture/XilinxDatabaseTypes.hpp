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
/// \brief Device database types for Xilinx architectures.

#ifndef TORC_ARCHITECTURE_XILINX_XILINX_DATABASE_TYPES_HPP
#define TORC_ARCHITECTURE_XILINX_XILINX_DATABASE_TYPES_HPP

#include "torc/common/EncapsulatedInteger.hpp"
#include <boost/cstdint.hpp>

namespace torc {
namespace architecture {
namespace xilinx {

	// --------------------------------------------------------------------------------------------
	// ------------------------------------------- Wires ------------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of a wire count in an unsigned 16-bit integer.
	/// \details This type supports counts between 0 and 65,535 inclusive.
	struct WireCount : public torc::common::EncapsulatedInteger<boost::uint16_t> {
		explicit WireCount(void) : type() {} ///< \brief Null constructor.
		explicit WireCount(const pod& rhs) : type(rhs) {} ///< \brief Data type constructor.
		explicit WireCount(const type& rhs) : type(rhs) {} ///< \brief Copy constructor.
	};
	/// \brief Encapsulation of a wire index in an unsigned 16-bit integer.
	/// \details This type supports indexes between 0 and 65,535 inclusive.
	struct WireIndex : public WireCount {
		explicit WireIndex(void) : WireCount() {} ///< \brief Null constructor.
		explicit WireIndex(const pod& rhs) : WireCount(rhs) {} ///< \brief Data type constructor.
		explicit WireIndex(const type& rhs) : WireCount(rhs) {} ///< \brief Copy constructor.
	};
	/// \brief Encapsulation of wire attribute flags in an unsigned 16-bit integer.
	/// \details This type supports a maximum of sixteen unique one-bit flags.
	struct WireFlags : public torc::common::EncapsulatedInteger<boost::uint16_t> {
		explicit WireFlags(void) : type() {} ///< \brief Null constructor.
		explicit WireFlags(const pod& rhs) : type(rhs) {} ///< \brief Data type constructor.
		explicit WireFlags(const type& rhs) : type(rhs) {} ///< \brief Copy constructor.
	};

	// --------------------------------------------------------------------------------------------
	// ------------------------------------------- Tiles ------------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of a tile count in an unsigned 32-bit integer.
	/// \details This type supports counts between 0 and 4,294,967,295 inclusive.
	struct TileCount : public torc::common::EncapsulatedInteger<boost::uint32_t> {
		explicit TileCount(void) : type() {} ///< \brief Null constructor.
		explicit TileCount(const pod& rhs) : type(rhs) {} ///< \brief Data type constructor.
		explicit TileCount(const type& rhs) : type(rhs) {} ///< \brief Copy constructor.
	};
	/// \brief Encapsulation of a tile index in an unsigned 32-bit integer.
	/// \details This type supports indexes between 0 and 4,294,967,295 inclusive.
	struct TileIndex : public TileCount {
		explicit TileIndex(void) : TileCount() {} ///< \brief Null constructor.
		explicit TileIndex(const pod& rhs) : TileCount(rhs) {} ///< \brief Data type constructor.
		explicit TileIndex(const type& rhs) : TileCount(rhs) {} ///< \brief Copy constructor.
	};
	/// \brief Encapsulation of a tile offset in an unsigned 32-bit integer.
	/// \details This type supports offsets between 0 and 4,294,967,295 inclusive.  Note that 
	///		although an offset would normally be a signed data type, this type is only used in 
	///		explicit compact segments, to express positive offsets from an anchor tile index.
	struct TileOffset : public torc::common::EncapsulatedInteger<boost::uint32_t> {
		explicit TileOffset(void) : type() {} ///< \brief Null constructor.
		explicit TileOffset(const pod& rhs) : type(rhs) {} ///< \brief Data type constructor.
		explicit TileOffset(const type& rhs) : type(rhs) {} ///< \brief Copy constructor.
		explicit TileOffset(const TileIndex& rhs) : type(rhs) {} ///< \brief Conversion constructor.
	};
	/// \brief Addition operator to add an anchor tile index to a relative tile offset.
	TileIndex operator +(const TileOffset& inTileOffset, const TileIndex& inTileIndex);
	/// \brief Encapsulation of a tile row in an unsigned 16-bit integer.
	/// \details This type supports row positions between 0 and 65,535 inclusive.
	struct TileRow : public torc::common::EncapsulatedInteger<boost::uint16_t> {
		explicit TileRow(void) : type() {} ///< \brief Null constructor.
		explicit TileRow(const pod& rhs) : type(rhs) {} ///< \brief Data type constructor.
		explicit TileRow(const type& rhs) : type(rhs) {} ///< \brief Copy constructor.
	};
	/// \brief Encapsulation of a tile column in an unsigned 16-bit integer.
	/// \details This type supports column positions between 0 and 65,535 inclusive.
	struct TileCol : public torc::common::EncapsulatedInteger<boost::uint16_t> {
		explicit TileCol(void) : type() {} ///< \brief Null constructor.
		explicit TileCol(const pod& rhs) : type(rhs) {} ///< \brief Data type constructor.
		explicit TileCol(const type& rhs) : type(rhs) {} ///< \brief Copy constructor.
	};

	// --------------------------------------------------------------------------------------------
	// ----------------------------------------- Tile types ---------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of a tile type count in an unsigned 16-bit integer.
	/// \details This type supports tile type counts between 0 and 65,535 inclusive.
	struct TileTypeCount : public torc::common::EncapsulatedInteger<boost::uint16_t> {
		explicit TileTypeCount(void) : type() {} ///< \brief Null constructor.
		explicit TileTypeCount(const pod& rhs) : type(rhs) {} ///< \brief Data type constructor.
		explicit TileTypeCount(const type& rhs) : type(rhs) {} ///< \brief Copy constructor.
	};
	/// \brief Encapsulation of a tile type index in an unsigned 16-bit integer.
	/// \details This type supports tile type indexes between 0 and 65,535 inclusive.
	struct TileTypeIndex : public TileTypeCount {
		explicit TileTypeIndex(void) : TileTypeCount() {} 
			///< \brief Null constructor.
		explicit TileTypeIndex(const pod& rhs) : TileTypeCount(rhs) {} 
			///< \brief Data type constructor.
		explicit TileTypeIndex(const type& rhs) : TileTypeCount(rhs) {} 
			///< \brief Copy constructor.
	};

	// --------------------------------------------------------------------------------------------
	// ------------------------------------- Compact segments -------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of a compact segment count in an unsigned 32-bit integer.
	/// \details This type supports compact segment counts between 0 and 4,294,967,295 inclusive.
	struct CompactSegmentCount : public torc::common::EncapsulatedInteger<boost::uint32_t> {
		explicit CompactSegmentCount(void) : type() {} 
			///< \brief Null constructor.
		explicit CompactSegmentCount(const pod& rhs) : type(rhs) {} 
			///< \brief Data type constructor.
		explicit CompactSegmentCount(const type& rhs) : type(rhs) {} 
			///< \brief Copy constructor.
	};
	/// \brief Encapsulation of a compact segment index in an unsigned 32-bit integer.
	/// \details This type supports compact segment indexes between 0 and 4,294,967,295 inclusive.
	struct CompactSegmentIndex : public CompactSegmentCount {
		explicit CompactSegmentIndex(void) : CompactSegmentCount() {} 
			///< \brief Null constructor.
		explicit CompactSegmentIndex(const pod& rhs) : CompactSegmentCount(rhs) {} 
			///< \brief Data type constructor.
		explicit CompactSegmentIndex(const type& rhs) : CompactSegmentCount(rhs) {} 
			///< \brief Copy constructor.
	};

	// --------------------------------------------------------------------------------------------
	// ------------------------------------------- Sites ------------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of a site count in an unsigned 32-bit integer.
	/// \details This type supports counts between 0 and 4,294,967,295 inclusive.
	struct SiteCount : public torc::common::EncapsulatedInteger<boost::uint32_t> {
		explicit SiteCount(void) : type() {} ///< \brief Null constructor.
		explicit SiteCount(const pod& rhs) : type(rhs) {} ///< \brief Data type constructor.
		explicit SiteCount(const type& rhs) : type(rhs) {} ///< \brief Copy constructor.
	};
	/// \brief Encapsulation of a site index in an unsigned 32-bit integer.
	/// \details This type supports indexes between 0 and 4,294,967,295 inclusive.
	struct SiteIndex : public SiteCount {
		explicit SiteIndex(void) : SiteCount() {} ///< \brief Null constructor.
		explicit SiteIndex(const pod& rhs) : SiteCount(rhs) {} ///< \brief Data type constructor.
		explicit SiteIndex(const type& rhs) : SiteCount(rhs) {} ///< \brief Copy constructor.
	};
	/// \brief Encapsulation of site attribute flags in an unsigned 16-bit integer.
	/// \details This type supports a maximum of sixteen unique one-bit flags.
	struct SiteFlags : public torc::common::EncapsulatedInteger<boost::uint16_t> {
		explicit SiteFlags(void) : type() {} ///< \brief Null constructor.
		explicit SiteFlags(const pod& rhs) : type(rhs) {} ///< \brief Data type constructor.
		explicit SiteFlags(const type& rhs) : type(rhs) {} ///< \brief Copy constructor.
	};

	// --------------------------------------------------------------------------------------------
	// ----------------------------------------- Site defs ----------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of a site type count in an unsigned 16-bit integer.
	/// \details This type supports site type counts between 0 and 65,535 inclusive.
	struct SiteTypeCount : public torc::common::EncapsulatedInteger<boost::uint16_t> {
		explicit SiteTypeCount(void) : type() {} ///< \brief Null constructor.
		explicit SiteTypeCount(const pod& rhs) : type(rhs) {} ///< \brief Data type constructor.
		explicit SiteTypeCount(const type& rhs) : type(rhs) {} ///< \brief Copy constructor.
	};
	/// \brief Encapsulation of a site type index in an unsigned 16-bit integer.
	/// \details This type supports site type indexes between 0 and 65,535 inclusive.
	struct SiteTypeIndex : public SiteTypeCount {
		explicit SiteTypeIndex(void) : SiteTypeCount() {} 
			///< \brief Null constructor.
		explicit SiteTypeIndex(const pod& rhs) : SiteTypeCount(rhs) {} 
			///< \brief Data type constructor.
		explicit SiteTypeIndex(const type& rhs) : SiteTypeCount(rhs) {} 
			///< \brief Copy constructor.
	};

	// --------------------------------------------------------------------------------------------
	// ------------------------------------------- Pins -------------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of a pin count in an unsigned 32-bit integer.
	/// \details This type supports counts between 0 and 4,294,967,295 inclusive.
	struct PinCount : public torc::common::EncapsulatedInteger<boost::uint32_t> {
		explicit PinCount(void) : type() {} ///< \brief Null constructor.
		explicit PinCount(const pod& rhs) : type(rhs) {} ///< \brief Data type constructor.
		explicit PinCount(const type& rhs) : type(rhs) {} ///< \brief Copy constructor.
	};
	/// \brief Encapsulation of a pin index in an unsigned 32-bit integer.
	/// \details This type supports indexes between 0 and 4,294,967,295 inclusive.
	struct PinIndex : public PinCount {
		explicit PinIndex(void) : PinCount() {} ///< \brief Null constructor.
		explicit PinIndex(const pod& rhs) : PinCount(rhs) {} ///< \brief Data type constructor.
		explicit PinIndex(const type& rhs) : PinCount(rhs) {} ///< \brief Copy constructor.
	};
	/// \brief Encapsulation of pin attribute flags in an unsigned 16-bit integer.
	/// \details This type supports a maximum of sixteen unique one-bit flags.
	struct PinFlags : public torc::common::EncapsulatedInteger<boost::uint16_t> {
		explicit PinFlags(void) : type() {} ///< \brief Null constructor.
		explicit PinFlags(const pod& rhs) : type(rhs) {} ///< \brief Data type constructor.
		explicit PinFlags(const type& rhs) : type(rhs) {} ///< \brief Copy constructor.
	};

	// --------------------------------------------------------------------------------------------
	// ----------------------------------------- Packages -----------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of a package count in an unsigned 16-bit integer.
	/// \details This type supports counts between 0 and 65,535 inclusive.
	struct PackageCount : public torc::common::EncapsulatedInteger<boost::uint16_t> {
		explicit PackageCount(void) : type() {} ///< \brief Null constructor.
		explicit PackageCount(const pod& rhs) : type(rhs) {} ///< \brief Data type constructor.
		explicit PackageCount(const type& rhs) : type(rhs) {} ///< \brief Copy constructor.
	};

	/// \brief Encapsulation of a package index in an unsigned 16-bit integer.
	/// \details This type supports package indexes between 0 and 65,535 inclusive.
	struct PackageIndex : public PackageCount {
		explicit PackageIndex(void) : PackageCount() {} ///< \brief Null constructor.
		explicit PackageIndex(const pod& rhs) : PackageCount(rhs) {} ///< \brief Data type constructor.
		explicit PackageIndex(const type& rhs) : PackageCount(rhs) {} ///< \brief Copy constructor.
	};

	// --------------------------------------------------------------------------------------------
	// ------------------------------------------- Pads -------------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of a pad count in an unsigned 32-bit integer.
	/// \details This type supports counts between 0 and 4,294,967,295 inclusive.
	struct PadCount : public torc::common::EncapsulatedInteger<boost::uint32_t> {
		explicit PadCount(void) : type() {} ///< \brief Null constructor.
		explicit PadCount(const pod& rhs) : type(rhs) {} ///< \brief Data type constructor.
		explicit PadCount(const type& rhs) : type(rhs) {} ///< \brief Copy constructor.
	};
	/// \brief Encapsulation of a pad index in an unsigned 32-bit integer.
	/// \details This type supports indexes between 0 and 4,294,967,295 inclusive.
	struct PadIndex : public PadCount {
		explicit PadIndex(void) : PadCount() {} ///< \brief Null constructor.
		explicit PadIndex(const pod& rhs) : PadCount(rhs) {} ///< \brief Data type constructor.
		explicit PadIndex(const type& rhs) : PadCount(rhs) {} ///< \brief Copy constructor.
	};

} // namespace xilinx
} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_XILINX_XILINX_DATABASE_TYPES_HPP
