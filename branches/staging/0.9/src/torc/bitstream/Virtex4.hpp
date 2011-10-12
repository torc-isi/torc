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
/// \brief Header for the Virtex4 class.

#ifndef TORC_BITSTREAM_VIRTEX4_HPP
#define TORC_BITSTREAM_VIRTEX4_HPP

#include <boost/integer.hpp>
#include <boost/filesystem.hpp>
#include "torc/bitstream/VirtexBitstream.hpp"

namespace torc { namespace architecture { class DDB; } }

namespace torc {
namespace bitstream {

namespace bitstream { class bitstream_virtex4; }
namespace bitstream { class bitstream_virtex4_far; }
namespace bitstream { void testVirtex4Device(const std::string& inDeviceName, 
	const boost::filesystem::path& inWorkingPath); }

	/// \brief Virtex4 bitstream.
	class Virtex4 : public VirtexBitstream {
		friend class torc::bitstream::bitstream::bitstream_virtex4;
		friend class torc::bitstream::bitstream::bitstream_virtex4_far;
		friend void torc::bitstream::bitstream::testVirtex4Device(const std::string& inDeviceName, 
			const boost::filesystem::path& inWorkingPath);
	protected:
	// typedefs
		/// \brief Imported type name.
		typedef boost::uint32_t uint32_t;
	public:
	// enumerations
		//
		/// \brief Configuration register enumeration.
		/// \see configuration registers: UG071, v1.10, April 8, 2008, Table 7-5.
		enum ERegister { eRegisterCRC = 0, eRegisterFAR, eRegisterFDRI, eRegisterFDRO, 
			eRegisterCMD, eRegisterCTL, eRegisterMASK, eRegisterSTAT, eRegisterLOUT, eRegisterCOR, 
			eRegisterMFWR, eRegisterCBC, eRegisterIDCODE, eRegisterAXSS, eRegisterCount };
		//
		/// \brief Configuration command enumeration for eRegisterCMD.
		/// \see CMD register commands: UG071, v1.10, April 8, 2008, Table 7-6.
		enum ECommand { eCommandNULL = 0, eCommandWCFG, eCommandMFWR, eCommandLFRM, eCommandRCFG, 
			eCommandSTART, eCommandRCAP, eCommandRCRC, eCommandAGHIGH, eCommandSWITCH, 
			eCommandGRESTORE, eCommandSHUTDOWN, eCommandGCAPTURE, eCommandDESYNC, eCommandCount };
		//
		/// \brief Frame Address Register subfields.
		/// \see Frame Address Register Description: UG071, v1.10, April 8, 2008, Table 7-8.
		enum EFar {
			eFarMaskTopBottom =			0x00400000,		eFarShiftTopBottom =		22,
			eFarMaskBlockType =			0x00380000,		eFarShiftBlockType =		19,
			eFarMaskRow =				0x0007c000,		eFarShiftRow =				14,
			eFarMaskMajor =				0x00003fc0,		eFarShiftMajor =			 6,
			eFarMaskMinor =				0x0000003f,		eFarShiftMinor =			 0,
		};
		//
		/// \brief Frame Address Register top and bottom constants.
		enum EFarTopBottom { eFarTop = 0, eFarBottom = 1 };
		/// \brief Frame Address Register block type constants.
		enum EFarBlockType { eFarBlockType0 = 0, eFarBlockType1, eFarBlockType2, eFarBlockType3, 
			eFarBlockType4, eFarBlockType5, eFarBlockType6, eFarBlockType7, eFarBlockTypeCount };
		/// \brief Major column types.
		/// \details These are defined and used for internal purposes only, and are not derived 
		///		from any Xilinx documentation.
		enum EColumnType { eColumnTypeEmpty = 0, eColumnTypeBram, eColumnTypeClb, eColumnTypeClock,
			eColumnTypeDsp, eColumnTypeGtx, eColumnTypeIob, eColumnTypeCount };
		/// \brief Frame length.
		/// \details Constant frame length of 41 32-bit words for the entire Virtex5 family.
		/// \see Virtex-4 Frame Count, Frame Length, Overhead and Bitstream Size: UG071, v.1.10, 
		///		April 8, 2008, Table 7-1.
		enum { eFrameLength = 41 };
	protected:
	// members
//		/// \brief Configuration controller registers.
//		uint32_t mRegister[eRegisterCount];
		/// \brief Packet type names.
		static const char* sPacketTypeName[ePacketTypeCount];
		/// \brief Packet opcode names.
		static const char* sOpcodeName[eOpcodeCount];
		/// \brief Configuration register names.
		static const char* sRegisterName[eRegisterCount];
		/// \brief Configuration command names.
		static const char* sCommandName[eCommandCount];
		/// \brief Configuration Options Register (COR) subfields.
		static const Subfield sCOR[];
		/// \brief Status Register (STAT) subfields.
		static const Subfield sSTAT[];
		/// \brief Control Register (CTL) subfields.
		static const Subfield sCTL[];
		/// \brief Control Mask Register (MASK) subfields.
		static const Subfield sMASK[];
	public:
	// constructors
		/// \brief Basic constructor.
		Virtex4(void) : VirtexBitstream() {
//			for(int i = 0; i < eRegisterCount; i++) mRegister[i] = 0;
			// initialize the column type widths for this family
			mColumnDefs.resize(eColumnTypeCount);
			mColumnDefs[eColumnTypeEmpty] 	= ColumnDef("Empty",    0,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeBram]	= ColumnDef("Bram",     0, 20, 64,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeClb]		= ColumnDef("Clb",     22,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeClock] 	= ColumnDef("Clock",    3,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeDsp]		= ColumnDef("Dsp",     21,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeGtx]		= ColumnDef("Gtx",     20,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeIob]		= ColumnDef("Iob",     30,  0,  0,  0,  0,  0,  0,  0);
			// map type type names to column types
			mTileTypeNameToColumnType["BRAM"]		= eColumnTypeBram;
			mTileTypeNameToColumnType["CLB"]		= eColumnTypeClb;
			mTileTypeNameToColumnType["CLKV_DCM_T"]	= eColumnTypeClock;
			mTileTypeNameToColumnType["DSP"]		= eColumnTypeDsp;
			mTileTypeNameToColumnType["MGT_AL"]		= eColumnTypeGtx;
			mTileTypeNameToColumnType["MGT_BL"]		= eColumnTypeGtx;
			mTileTypeNameToColumnType["MGT_AR"]		= eColumnTypeGtx;
			mTileTypeNameToColumnType["MGT_BR"]		= eColumnTypeGtx;
			mTileTypeNameToColumnType["IOIS_LC"]	= eColumnTypeIob;
			mTileTypeNameToColumnType["IOIS_LC_L"]	= eColumnTypeIob;
			mTileTypeNameToColumnType["IOIS_NC"]	= eColumnTypeIob;
			mTileTypeNameToColumnType["IOIS_NC_L"]	= eColumnTypeIob;
		}
	// functions
		/// \brief Return the masked value for a subfield of the specified register.
		static uint32_t makeSubfield(ERegister inRegister, const std::string& inSubfield, 
			const std::string& inSetting);
		/// \brief Initialize the device information.
		virtual void initializeDeviceInfo(const std::string& inDeviceName);
		/// \brief Initialize the maps between frame indexes and frame addresses.
		/// \detail This is generally only useful for internal purposes.
		void initializeFrameMaps(void);
	// accessors
		/// \brief Return the frame length for the current device.
		virtual uint32_t getFrameLength(void) const { return eFrameLength; }
	// inserters
		/// \brief Insert the bitstream header into an output stream.
		friend std::ostream& operator<< (std::ostream& os, const Virtex4& rhs);
	// inner classes
		class FrameAddress {
		protected:
			void assign(uint32_t inAddress) {
				mTopBottom = EFarTopBottom((inAddress & eFarMaskTopBottom) >> eFarShiftTopBottom);
				mBlockType = EFarBlockType((inAddress & eFarMaskBlockType) >> eFarShiftBlockType);
				mRow = (inAddress & eFarMaskRow) >> eFarShiftRow;
				mMajor = (inAddress & eFarMaskMajor) >> eFarShiftMajor;
				mMinor = (inAddress & eFarMaskMinor) >> eFarShiftMinor;
			}
		public:
			FrameAddress(void) : mTopBottom(eFarTop), mBlockType(eFarBlockType0), mRow(0), 
				mMajor(0), mMinor(0) {}
			FrameAddress(EFarTopBottom inTopBottom, EFarBlockType inBlockType, uint32_t inRow, 
				uint32_t inMajor, uint32_t inMinor) : mTopBottom(inTopBottom), 
				mBlockType(inBlockType), mRow(inRow), mMajor(inMajor), mMinor(inMinor) {}
			FrameAddress(uint32_t inAddress) { assign(inAddress); }
			EFarTopBottom mTopBottom;
			EFarBlockType mBlockType;
			uint32_t mRow;
			uint32_t mMajor;
			uint32_t mMinor;
			bool operator== (const FrameAddress& rhs) const {
				return mTopBottom == rhs.mTopBottom && mBlockType == rhs.mBlockType 
					&& mRow == rhs.mRow && mMajor == rhs.mMajor && mMinor == rhs.mMinor;
			}
			bool operator< (const FrameAddress& rhs) const {
				int diffBlockType = mBlockType - rhs.mBlockType;
				if(diffBlockType) return diffBlockType < 0;
				int diffTopBottom = mTopBottom - rhs.mTopBottom;
				if(diffTopBottom) return diffTopBottom < 0;
				int diffRow = mRow - rhs.mRow;
				if(diffRow) return diffRow < 0;
				int diffMajor = mMajor - rhs.mMajor;
				if(diffMajor) return diffMajor < 0;
				return mMinor < rhs.mMinor;
			}
		private:
			//operator uint32_t (void) const {
			//	return 
			//		((mTopBottom << eFarShiftTopBottom) & eFarMaskTopBottom) | 
			//		((mBlockType << eFarShiftBlockType) & eFarMaskBlockType) |
			//		((mRow << eFarShiftRow) & eFarMaskRow) | 
			//		((mMajor << eFarShiftMajor) & eFarMaskMajor) | 
			//		((mMinor << eFarShiftMinor) & eFarMaskMinor);
			//}
		};
	protected:
	// typedefs
		/// \brief Map from frame index to frame address.
		typedef std::map<uint32_t, Virtex4::FrameAddress> FrameIndexToAddress;
		/// \brief Map from frame address to frame index.
		typedef std::map<Virtex4::FrameAddress, uint32_t> FrameAddressToIndex;
		/// \brief Array of vectors to store frame indexes of each block type
		typedef std::vector<uint32_t> ColumnIndexVector;
	// members
		/// \brief Map of frame indexes to frame addresses.
		FrameIndexToAddress mFrameIndexToAddress;
		/// \brief Map of frame addressee to frame indexes.
		FrameAddressToIndex mFrameAddressToIndex;
		/// \brief Vector to store frame indexes of XDL columns.
		ColumnIndexVector mBitColumnIndexes [Virtex4::eFarBlockTypeCount];
		/// \brief Vector to store frame indexes of Bitstream columns.
		ColumnIndexVector mXdlColumnIndexes [Virtex4::eFarBlockTypeCount];
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_VIRTEX4_HPP
