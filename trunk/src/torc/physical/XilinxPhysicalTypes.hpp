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
/// \brief Header for Xilinx physical types.

#ifndef TORC_PHYSICAL_XILINXPHYSICALTYPES_HPP
#define TORC_PHYSICAL_XILINXPHYSICALTYPES_HPP

#include <boost/cstdint.hpp>
#include <string>

namespace torc {
namespace physical {

	// --------------------------------------------------------------------------------------------
	// ------------------------------------------- Wires ------------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of a wire name.
	class WireName : public std::string {
		/// \brief Basic constructor.
		public: WireName(const std::string& rhs) : std::string(rhs) {}
	};

	// --------------------------------------------------------------------------------------------
	// ------------------------------------------- Tiles ------------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of a tile name.
	class TileName : public std::string {
		/// \brief Basic constructor.
		public: TileName(const std::string& rhs) : std::string(rhs) {}
	};

	// --------------------------------------------------------------------------------------------
	// ---------------------------------------- Tile types ----------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of a tile type name.
	class TileTypeName : public std::string {
		/// \brief Basic constructor.
		public: TileTypeName(const std::string& rhs) : std::string(rhs) {}
	};

	// --------------------------------------------------------------------------------------------
	// ----------------------------------------- Instances ----------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of an instance name.
	class InstanceName : public std::string {
		/// \brief Basic constructor.
		public: InstanceName(const std::string& rhs) : std::string(rhs) {}
	};

	/// \brief Enumeration of pad bonding types.
	enum EInstanceBonding {
		eInstanceBondingUnbonded = 0,
		eInstanceBondingBonded,
		eInstanceBondingUnknown,
		eInstanceBondingCount
	};

	// --------------------------------------------------------------------------------------------
	// ------------------------------------------- Sites ------------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of a site name.
	class SiteName : public std::string {
		/// \brief Basic constructor.
		public: SiteName(const std::string& rhs) : std::string(rhs) {}
	};

	// --------------------------------------------------------------------------------------------
	// ----------------------------------------- Site types ---------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of a site type name.
	class SiteTypeName : public std::string {
		/// \brief Basic constructor.
		public: SiteTypeName(const std::string& rhs) : std::string(rhs) {}
	};

	// --------------------------------------------------------------------------------------------
	// ------------------------------------------- Pins -------------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Encapsulation of a site pin name.
	class PinName : public std::string {
		/// \brief Basic constructor.
		public: PinName(const std::string& rhs) : std::string(rhs) {}
	};

	/// \brief Enumeration of instance pin directionality.
	enum EPinDirection {
		ePinDirectionInpin = 0,
		ePinDirectionOutpin,
		ePinDirectionUnknown,
		ePinDirectionCount
	};

	// --------------------------------------------------------------------------------------------
	// ------------------------------------------- Pips -------------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Enumeration of pip directionality.
	enum EPipDirection {
		ePipBidirectionalUnbuffered = 0,
		ePipBidirectionalUnidirectionallyBuffered,
		ePipBidirectionalBidirectionallyBuffered,
		ePipUnidirectionalBuffered,
		ePipDirectionCount
	};

	// --------------------------------------------------------------------------------------------
	// ------------------------------------------- Nets -------------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Enumeration of net power types.
	enum ENetType {
		eNetTypeNormal = 0,
		eNetTypePower,
		eNetTypeGround,
		eNetTypeCount
	};

	// --------------------------------------------------------------------------------------------
	// ----------------------------------------- Sequences ----------------------------------------
	// --------------------------------------------------------------------------------------------

	/// \brief Typedef for generic sequences.
	typedef boost::uint64_t SequenceIndex;
	/// \brief Enumeration of sequence indexes.
	/// \brief A sequence index is just a number, so we only need to capture the number to use when 
	///		there is no sequence defined, and the number with which to begin actual sequences.
	///		Note that this should probably be pulled into a higher-level part of the code, since it 
	///		may be used in multiple cases.
	enum ESequenceIndex {
		eSequenceIndexNone = 0,
		eSequenceIndexFirst
	};

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_XILINXPHYSICALTYPES_HPP
