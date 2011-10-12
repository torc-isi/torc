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
/// \brief Header for the Virtex class.

#ifndef TORC_BITSTREAM_VIRTEX_HPP
#define TORC_BITSTREAM_VIRTEX_HPP

#include <boost/integer.hpp>
#include <boost/filesystem.hpp>
#include "torc/bitstream/VirtexBitstream.hpp"
#include <map>

namespace torc { namespace architecture { class DDB; } }

namespace torc {
namespace bitstream {

namespace bitstream { class bitstream_virtex; }
namespace bitstream { class bitstream_virtex_far; }
namespace bitstream { void testVirtexDevice(const std::string& inDeviceName, 
	const boost::filesystem::path& inWorkingPath); }

	/// \brief Virtex bitstream.
	class Virtex : public VirtexBitstream {
		friend class torc::bitstream::bitstream::bitstream_virtex;
		friend class torc::bitstream::bitstream::bitstream_virtex_far;
		friend void torc::bitstream::bitstream::testVirtexDevice(const std::string& inDeviceName, 
			const boost::filesystem::path& inWorkingPath);
	protected:
	// typedefs
		/// \brief Imported type name.
		typedef boost::uint32_t uint32_t;
	public:
	// enumerations
		//
		/// \brief Configuration register enumeration.
		/// \see configuration registers: XAPP151, v1.7, October 20, 2004, Table 18.
		enum ERegister { eRegisterCRC = 0, eRegisterFAR, eRegisterFDRI, eRegisterFDRO, 
			eRegisterCMD, eRegisterCTL, eRegisterMASK, eRegisterSTAT, eRegisterLOUT, eRegisterCOR, 
			eRegisterFLR=11, eRegisterCount};
		//
		/// \brief Configuration command enumeration for eRegisterCMD.
		/// \see CMD register commands: XAPP151, v1.7, October 20, 2004, Table 19.
		enum ECommand { eCommandWCFG = 1, eCommandLFRM = 3, eCommandRCFG, 
			eCommandSTART, eCommandRCAP, eCommandRCRC, eCommandAGHIGH, eCommandSWITCH, eCommandCount};
		//
		/// \brief Frame Address Register subfields.
		/// \see Frame Address Register Description: XAPP151, v1.7, October 20, 2004, Figure 19.
		enum EFar {
			eFarMaskBlockType =			0x06000000,		eFarShiftBlockType =		25,
			eFarMaskMajor =				0x01FE0000,		eFarShiftMajor =			17,
			eFarMaskMinor =				0x0001FE00,		eFarShiftMinor =			 9,
		};
		//
		/// \brief Frame Address Register block type constants.
		enum EFarBlockType { eFarBlockType0 = 0, eFarBlockType1, eFarBlockType2, eFarBlockType3, 
			eFarBlockType4, eFarBlockType5, eFarBlockType6, eFarBlockType7, eFarBlockTypeCount };
		/// \brief Major column types.
		/// \details These are defined and used for internal purposes only, and are not derived 
		///		from any Xilinx documentation.
		enum EColumnType { eColumnTypeEmpty = 0, eColumnTypeBram, eColumnTypeClb, eColumnTypeClock,
			eColumnTypeDsp, eColumnTypeGtx, eColumnTypeIob, eColumnTypeCount };
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
		string mPrivateDeviceName;
	public:
	// constructors
		/// \brief Basic constructor.
		Virtex(void) : VirtexBitstream() {
//			for(int i = 0; i < eRegisterCount; i++) mRegister[i] = 0;
			// initialize the column type widths for this family
			mColumnDefs.resize(eColumnTypeCount);
			mColumnDefs[eColumnTypeEmpty] 	= ColumnDef("Empty",    0,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeBram]	= ColumnDef("Bram",    27, 64,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeClb]		= ColumnDef("Clb",     48,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeClock] 	= ColumnDef("Clock",    8,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeIob]		= ColumnDef("Iob",     54,  0,  0,  0,  0,  0,  0,  0);
			// map type type names to column types
			mTileTypeNameToColumnType["LBRAM"]		= eColumnTypeBram;
			mTileTypeNameToColumnType["RBRAM"]		= eColumnTypeBram;
			mTileTypeNameToColumnType["MBRAM"]		= eColumnTypeBram;
			mTileTypeNameToColumnType["CENTER"]		= eColumnTypeClb;
			mTileTypeNameToColumnType["CLKV"]		= eColumnTypeClock;
			mTileTypeNameToColumnType["LEFT"]		= eColumnTypeIob;
			mTileTypeNameToColumnType["RIGHT"]		= eColumnTypeIob;
		}
	// functions
		/// \brief Return the masked value for a subfield of the specified register.
		static uint32_t makeSubfield(ERegister inRegister, const std::string& inSubfield, 
			const std::string& inSetting);
	// functions
		/// \brief Initialize the device information.
		virtual void initializeDeviceInfo(const std::string& inDeviceName);
		/// \brief Initialize the maps between frame indexes and frame addresses.
		/// \detail This is generally only useful for internal purposes.
		virtual void initializeFrameMaps(void);
		/// \brief This function takes care of the Virtex Frame Mapping
		typedef torc::common::EncapsulatedInteger<uint16_t> ColumnIndex;
		virtual void prepareFrames(ColumnIndex &inCol, int &inFrameCount, int &inFrameIndex, EFarBlockType &inBlockType, int &inFarMajor, int &inWidth);
	// inserters
		/// \brief Insert the bitstream header into an output stream.
		friend std::ostream& operator<< (std::ostream& os, const Virtex& rhs);
	// inner classes
		class FrameAddress {
		protected:
			void assign(uint32_t inAddress) {
				mBlockType = EFarBlockType((inAddress & eFarMaskBlockType) >> eFarShiftBlockType);
				mMajor = (inAddress & eFarMaskMajor) >> eFarShiftMajor;
				mMinor = (inAddress & eFarMaskMinor) >> eFarShiftMinor;
			}
		public:
			FrameAddress(void) : mBlockType(eFarBlockType0), mMajor(0), mMinor(0) {}
			FrameAddress(EFarBlockType inBlockType,	uint32_t inMajor, uint32_t inMinor) :  
				mBlockType(inBlockType), mMajor(inMajor), mMinor(inMinor) {}
			FrameAddress(uint32_t inAddress) { assign(inAddress); }
			EFarBlockType mBlockType;
			uint32_t mMajor;
			uint32_t mMinor;
			bool operator== (const FrameAddress& rhs) const {
				return mBlockType == rhs.mBlockType	&& mMajor == rhs.mMajor && mMinor == rhs.mMinor;
			}
			bool operator< (const FrameAddress& rhs) const {
				int diffBlockType = mBlockType - rhs.mBlockType;
				if(diffBlockType) return diffBlockType < 0;
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
	// accessors
		/// \brief Return the frame length for the current device.
		virtual uint32_t getFrameLength(void) const {
			using namespace torc::common;
			// Frame Length Register Value: XAPP151, v.1.7, October 20, 2004, Table 24.
			switch(mDevice) {
				case eXCV50: return 12;
				case eXCV100: return 14;
				case eXCV150: return 16;
				case eXCV200: return 18;
				case eXCV300: return 21;
				case eXCV400: return 25;
				case eXCV600: return 30;
				case eXCV800: return 34;
				case eXCV1000: return 39;
				default: return 0;
			}
		}
	protected:
	// typedefs
		/// \brief Map from frame index to frame address.
		typedef std::map<uint32_t, Virtex::FrameAddress> FrameIndexToAddress;
		/// \brief Map from frame address to frame index.
		typedef std::map<Virtex::FrameAddress, uint32_t> FrameAddressToIndex;
		/// \brief Array of vectors to store frame indexes of each block type
		typedef std::vector<uint32_t> ColumnIndexVector;
	// members
		/// \brief Map of frame indexes to frame addresses.
		FrameIndexToAddress mFrameIndexToAddress;
		/// \brief Map of frame addressee to frame indexes.
		FrameAddressToIndex mFrameAddressToIndex;
		/// \brief Vector to store frame indexes of XDL columns.
		ColumnIndexVector mBitColumnIndexes [Virtex::eFarBlockTypeCount];
		/// \brief Vector to store frame indexes of Bitstream columns.
		ColumnIndexVector mXdlColumnIndexes [Virtex::eFarBlockTypeCount];
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_VIRTEX_HPP
