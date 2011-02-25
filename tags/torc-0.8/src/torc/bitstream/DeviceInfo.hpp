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
/// \brief Header for the DeviceInfo class.

#ifndef TORC_BITSTREAM_STATIC_DEVICE_INFO_HPP
#define TORC_BITSTREAM_STATIC_DEVICE_INFO_HPP

#include <boost/cstdint.hpp>
#include <string>
#include <vector>

namespace torc {
namespace bitstream {

namespace bitstream { class bitstream_static_device_info; }

	/// \brief Bitstream column definition for Xilinx bitstreams.
	/// \detail For every block type 0 through 8, this class provides the width of the specified 
	///		column in minor frames.  Note that in most families, block types beyond 3 or 4 are 
	///		unused, and hence of zero width.
	class ColumnDef {
	protected:
	// typedefs
		typedef boost::uint32_t uint32_t; ///< Imported type name.
		typedef std::string string; ///< Imported type name.
		/// \brief The column type name.
		/// \detail Note that this name is not used by Xilinx, and may not directly correspond to 
		///		that found in the device tile map.
		string mName;
		/// \brief The column width in minor frames for each of the eight block types.
		uint32_t mWidth[8];
	public:
	// constructors
		/// \brief Basic constructor.
		ColumnDef(const string& inName = string(), uint32_t in0 = 0, uint32_t in1 = 0, 
			uint32_t in2 = 0, uint32_t in3 = 0, uint32_t in4 = 0, uint32_t in5 = 0, 
			uint32_t in6 = 0, uint32_t in7 = 0) : mName(inName) {
			int i = 0;
			mWidth[i++] = in0; mWidth[i++] = in1; mWidth[i++] = in2; mWidth[i++] = in3;
			mWidth[i++] = in4; mWidth[i++] = in5; mWidth[i++] = in6; mWidth[i++] = in7;
		}
	// functions
		/// \brief Clears all column widths.
		void clear(void) { for(int i = 0; i < 8; i++) mWidth[i] = 0; }
	// accessors
		/// \brief Returns the specified column width.
		uint32_t operator[] (int inIndex) const { return mWidth[inIndex]; }
		/// \brief Returns the column type name.
		const string& getName(void) const { return mName; }
	};

	/// \brief Column definition vector.
	class ColumnDefVector : public std::vector<ColumnDef> {};

	/// \brief Column type vector.
	class ColumnTypeVector : public std::vector<boost::uint32_t> {};

	/// \brief Static device information class for Xilinx bitstreams.
	/// \detail This class facilitates the creation of frame address maps without dependence upon 
	///		torc::architecture.
	class DeviceInfo {
	protected:
	// typedefs
		typedef boost::uint32_t uint32_t; ///< Imported type name.
		typedef boost::uint16_t uint16_t; ///< Imported type name.
	// members
		/// \brief The tile count for this device.
		uint32_t mTileCount;
		/// \brief The row count for this device.
		/// \brief These are regular tile rows, not bitstream clock region rows.
		uint16_t mRowCount;
		/// \brief The column count for this device.
		uint16_t mColCount;
		/// \brief The column types as applicable for the bitstream.
		ColumnTypeVector mColumnTypes;
	public:
	// constructors
		/// \brief Basic constructor.
		DeviceInfo(uint32_t inTileCount, uint16_t inRowCount, uint16_t inColCount, 
			uint32_t* inColumns) : mTileCount(inTileCount), mRowCount(inRowCount), 
			mColCount(inColCount) {
			while(true) {
				uint32_t type = *inColumns++;
				if(type == static_cast<uint32_t>(-1)) break;
				mColumnTypes.push_back(type);
			}
		}
		/// \brief Null constructor.
		DeviceInfo(void) {}
	// accessors
		/// \brief Returns the tile count.
		uint32_t getTileCount(void) const { return mTileCount; }
		/// \brief Returns the row count.
		uint16_t getRowCount(void) const { return mRowCount; }
		/// \brief Returns the column count.
		uint16_t getColCount(void) const { return mColCount; }
		/// \brief Returns the column type vector.
		const ColumnTypeVector& getColumnTypes(void) const { return mColumnTypes; }
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_STATIC_DEVICE_INFO_HPP
