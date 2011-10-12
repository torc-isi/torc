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
/// \brief Header for the Virtex6 class.

#ifndef TORC_BITSTREAM_VIRTEX6_HPP
#define TORC_BITSTREAM_VIRTEX6_HPP

#include <boost/cstdint.hpp>
#include <boost/filesystem.hpp>
#include "torc/bitstream/VirtexBitstream.hpp"
#include "torc/bitstream/VirtexFrameAddress.hpp"
#include <map>

namespace torc { namespace architecture { class DDB; } }

namespace torc {
namespace bitstream {

namespace bitstream { class Virtex6UnitTest; }
namespace bitstream { class Virtex6FarUnitTest; }
namespace bitstream { void testVirtex6Device(const std::string& inDeviceName, 
	const boost::filesystem::path& inWorkingPath); }
namespace bitstream { void testVirtex6FullMapping(const boost::filesystem::path& inWorkingPath); }
namespace bitstream { void testVirtex6PartialMapping(const boost::filesystem::path& inWorkingPath); }

	/// \brief Virtex6 bitstream.
	class Virtex6 : public VirtexBitstream {
		friend class torc::bitstream::bitstream::Virtex6UnitTest;
		friend class torc::bitstream::bitstream::Virtex6FarUnitTest;
		friend void torc::bitstream::bitstream::testVirtex6Device(const std::string& inDeviceName, 
			const boost::filesystem::path& inWorkingPath);
		friend void torc::bitstream::bitstream::testVirtex6FullMapping(const boost::filesystem::path& inWorkingPath);
		friend void torc::bitstream::bitstream::testVirtex6PartialMapping(const boost::filesystem::path& inWorkingPath);
	protected:
	// typedefs
		/// \brief Imported type name.
		typedef boost::uint32_t uint32_t;
	public:
	// enumerations
		/// \brief Configuration register enumeration.
		/// \see Type 1 Packet Registers: UG360, v3.2, November 1, 2010, Table 6-26.
		enum ERegister { eRegisterCRC = 0, eRegisterFAR, eRegisterFDRI, eRegisterFDRO, 
			eRegisterCMD, eRegisterCTL0, eRegisterMASK, eRegisterSTAT, eRegisterLOUT, 
			eRegisterCOR0, eRegisterMFWR, eRegisterCBC, eRegisterIDCODE, eRegisterAXSS, 
			eRegisterCOR1, eRegisterCSOB, eRegisterWBSTAR, eRegisterTIMER, eRegisterBOOTSTS = 22, 
			eRegisterCTL1 = 24, eRegisterDWC = 26, eRegisterCount };
		//
		/// \brief Configuration command enumeration for eRegisterCMD.
		/// \see Command Register Codes: UG360, v3.2, November 1, 2010, Table 6-27.
		enum ECommand { eCommandNULL = 0, eCommandWCFG, eCommandMFW, eCommandDGHIGH, 
			eCommandLFRM = eCommandDGHIGH, eCommandRCFG, eCommandSTART, eCommandRCAP, 
			eCommandRCRC, eCommandAGHIGH, eCommandSWITCH, eCommandGRESTORE, eCommandSHUTDOWN, 
			eCommandGCAPTURE, eCommandDESYNCH, eCommandReserved, eCommandIPROG, eCommandCRCC,  
			eCommandLTIMER, eCommandCount };
		//
		/// \brief Frame Address Register subfields.
		/// \see Frame Address Register Description: UG360, v3.2, November 1, 2010, Table 6-31.
		enum EFar {
			eFarMaskBlockType =		0x00e00000,		eFarShiftBlockType =	21,
			eFarMaskTopBottom =		0x00100000,		eFarShiftTopBottom =	20,
			eFarMaskRow =			0x000f8000,		eFarShiftRow =          15,
			eFarMaskMajor =			0x00007f80,		eFarShiftMajor =         7,
			eFarMaskMinor =			0x0000007f,		eFarShiftMinor =         0
		};
		//
		/// \brief Frame Address Register top and bottom constants.
		/// \see Frame Address Register Description: UG360, v3.2, November 1, 2010, Table 6-31.
		enum EFarTopBottom { eFarTop = 0, eFarBottom = 1 };
		//
		/// \brief Frame Address Register block type constants.
		enum EFarBlockType { eFarBlockType0 = 0, eFarBlockType1, eFarBlockType2, eFarBlockType3, 
			eFarBlockType4, eFarBlockType5, eFarBlockType6, eFarBlockType7, eFarBlockTypeCount };
		//
		/// \brief Assorted constants.
		/// \see WBSTAR Register Description: UG360, v3.2, November 1, 2010, Table 6-39.
		enum {
			eStartAddr =						0x03ffffff,
			eTimerValue =						0x00ffffff
		};
		/// \brief Major column types.
		/// \details These are defined and used for internal purposes only, and are not derived 
		///		from any Xilinx documentation.
		enum EColumnType { eColumnTypeEmpty = 0, eColumnTypeBram, eColumnTypeClb, eColumnTypeClock,
			eColumnTypeDsp, eColumnTypeGtx, eColumnTypeIob, eColumnTypeCount };
		/// \brief Frame length.
		/// \details Constant frame length of 81 32-bit words for the entire Virtex6 family.
		/// \see Virtex-6 FPGA Frame Count, Frame Length, and Bitstream Size: UG360, v.3.2, 
		///		November 1, 2010, Table 6-22.
		enum { eFrameLength = 81 };
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
		/// \brief Configuration Options Register 0 (COR) subfields.
		static const Subfield sCOR0[];
		/// \brief Configuration Options Register 1 (COR) subfields.
		static const Subfield sCOR1[];
		/// \brief Status Register (STAT) subfields.
		static const Subfield sSTAT[];
		/// \brief Control Register 0 (CTL) subfields.
		static const Subfield sCTL0[];
		/// \brief Control Register 1 (CTL) subfields.
		static const Subfield sCTL1[];
		/// \brief Control Mask Register (MASK) subfields.
		static const Subfield sMASK0[];
		/// \brief Warm Boot Start Address Register (WBSTAR) subfields.
		static const Subfield sWBSTAR[];
		/// \brief Watchdog Timer Register (TIMER) subfields.
		static const Subfield sTIMER[];
		/// \brief Boot History Status Register (BOOTSTS) subfields.
		static const Subfield sBOOTSTS[];
		/// \brief The number of top bitstream rows.
		uint32_t mTopRowCount;
		/// \brief The number of bottom bitstream rows.
		uint32_t mBottomRowCount;
	// functions
		/// \brief Set the number of top and bottom bitstream rows.
		void setRowCounts(void);
	public:
	// constructors
		/// \brief Basic constructor.
		Virtex6(void) : VirtexBitstream(), mTopRowCount(0), mBottomRowCount(0) {
//			for(int i = 0; i < eRegisterCount; i++) mRegister[i] = 0;
			// initialize the column type widths for this family
			mColumnDefs.resize(eColumnTypeCount);
			mColumnDefs[eColumnTypeEmpty] 	= ColumnDef("Empty",    0,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeBram]	= ColumnDef("Bram",    28,128,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeClb]	= ColumnDef("Clb",     36,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeClock] 	= ColumnDef("Clock",   38,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeDsp]	= ColumnDef("Dsp",     28,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeGtx]	= ColumnDef("Gtx",     30,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeIob]	= ColumnDef("Iob",     44,  0,  0,  0,  0,  0,  0,  0);
			// map type type names to column types
			mTileTypeNameToColumnType["BRAM"]		= eColumnTypeBram;
			mTileTypeNameToColumnType["PCIE_BRAM"]		= eColumnTypeBram;
			mTileTypeNameToColumnType["CLBLL"]		= eColumnTypeClb;
			mTileTypeNameToColumnType["CLBLM"]		= eColumnTypeClb;
			mTileTypeNameToColumnType["CMT_TOP"]		= eColumnTypeClock;
			mTileTypeNameToColumnType["DSP"]		= eColumnTypeDsp;
			mTileTypeNameToColumnType["GTX"]		= eColumnTypeGtx;
			mTileTypeNameToColumnType["GTX_LEFT"]		= eColumnTypeGtx;
			mTileTypeNameToColumnType["GT3"]		= eColumnTypeGtx;
			mTileTypeNameToColumnType["CIOB"]		= eColumnTypeIob;
			mTileTypeNameToColumnType["LIOB"]		= eColumnTypeIob;
			mTileTypeNameToColumnType["LIOB_FT"]		= eColumnTypeIob;
			mTileTypeNameToColumnType["RIOB"]		= eColumnTypeIob;
		}
	// functions
		/// \brief Return the masked value for a subfield of the specified register.
		static uint32_t makeSubfield(ERegister inRegister, const std::string& inSubfield, 
			const std::string& inSetting);
		/// \brief Initialize the device information.
		virtual void initializeDeviceInfo(const std::string& inDeviceName);
		/// \brief Initialize the maps between frame indexes and frame addresses.
		/// \detail This is generally only useful for internal purposes.
		virtual void initializeFrameMaps(void);
		/// \brief Loads full bitstream frames into block data structure.
		void initializeFullFrameBlocks(void);
		/// \brief Returns frames for queried bitstream co-ordinates
		VirtexFrameBlocks getBitstreamFrames(uint32_t inBlockCount, uint32_t inBitCol);
		/// \brief Returns frames for queried xdl co-ordinates
		VirtexFrameBlocks getXdlFrames(uint32_t inBlockCount, uint32_t inXdlCol);
	// accessors
		/// \brief Return the frame length for the current device.
		virtual uint32_t getFrameLength(void) const { return eFrameLength; }
	// inserters
		/// \brief Insert the bitstream header into an output stream.
		friend std::ostream& operator<< (std::ostream& os, const Virtex6& rhs);
	// inner classes
		class FrameAddress : public VirtexFrameAddress {
		protected:
			virtual void assign(uint32_t inAddress) {
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
			virtual bool operator== (const VirtexFrameAddress& vrhs) const {
			    const FrameAddress& rhs = reinterpret_cast<const FrameAddress&>(vrhs);
				return mTopBottom == rhs.mTopBottom && mBlockType == rhs.mBlockType 
					&& mRow == rhs.mRow && mMajor == rhs.mMajor && mMinor == rhs.mMinor;
			}
			virtual bool operator< (const VirtexFrameAddress& vrhs) const {
			    const FrameAddress& rhs = reinterpret_cast<const FrameAddress&>(vrhs);
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
		typedef std::map<uint32_t, Virtex6::FrameAddress> FrameIndexToAddress;
		/// \brief Map from frame address to frame index.
		typedef std::map<Virtex6::FrameAddress, uint32_t> FrameAddressToIndex;
		/// \brief Array of vectors to store frame indexes of each block type
		typedef std::vector<uint32_t> IndexVector;

	// members
		/// \brief Map of frame indexes to frame addresses.
		FrameIndexToAddress mFrameIndexToAddress;
		/// \brief Map of frame addressee to frame indexes.
		FrameAddressToIndex mFrameAddressToIndex;
		/// \brief Vector to store frame indexes of XDL columns.
		IndexVector mBitColumnIndexes [Virtex6::eFarBlockTypeCount];
		/// \brief Vector to store frame indexes of Bitstream columns.
		IndexVector mXdlColumnIndexes [Virtex6::eFarBlockTypeCount];
		/// \brief Array to hold frame index boundaries for blocks.
		uint32_t mBlockFrameIndexBounds [Virtex6::eFarBlockTypeCount];
		/// \brief Map of xdl columns to bit columns.
		std::map<uint32_t, uint32_t> mXdlIndexToBitIndex;
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_VIRTEX6_HPP
