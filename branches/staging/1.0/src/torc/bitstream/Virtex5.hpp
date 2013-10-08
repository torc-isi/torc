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
/// \brief Header for the Virtex5 class.

#ifndef TORC_BITSTREAM_VIRTEX5_HPP
#define TORC_BITSTREAM_VIRTEX5_HPP

#include <boost/cstdint.hpp>
#include <boost/filesystem.hpp>
#include "torc/bitstream/VirtexBitstream.hpp"
#include "torc/bitstream/VirtexFrameAddress.hpp"
#include <map>

namespace torc { namespace architecture { class DDB; } }

namespace torc {
namespace bitstream {

namespace bitstream { class Virtex5UnitTest; }
namespace bitstream { class Virtex5FarUnitTest; }
namespace bitstream { void testVirtex5Device(const std::string& inDeviceName, 
	const boost::filesystem::path& inWorkingPath); }
namespace bitstream { void testVirtex5FullMapping(const boost::filesystem::path& inWorkingPath); }
namespace bitstream { void testVirtex5PartialMapping(const boost::filesystem::path& inWorkingPath); }

	/// \brief Virtex5 bitstream.
	class Virtex5 : public VirtexBitstream {
		friend class torc::bitstream::bitstream::Virtex5UnitTest;
		friend class torc::bitstream::bitstream::Virtex5FarUnitTest;
		friend void torc::bitstream::bitstream::testVirtex5Device(const std::string& inDeviceName, 
			const boost::filesystem::path& inWorkingPath);
		friend void torc::bitstream::bitstream::testVirtex5FullMapping(const boost::filesystem::path& inWorkingPath);
		friend void torc::bitstream::bitstream::testVirtex5PartialMapping(const boost::filesystem::path& inWorkingPath);
	protected:
	// typedefs
		/// \brief Imported type name.
		typedef boost::uint32_t uint32_t;
	public:
	// enumerations
		/// \brief Configuration register enumeration.
		/// \see Type 1 Packet Registers: UG191, v3.7, June 24, 2009, Table 6-5.
		enum ERegister { eRegisterCRC = 0, eRegisterFAR, eRegisterFDRI, eRegisterFDRO, 
			eRegisterCMD, eRegisterCTL0, eRegisterMASK, eRegisterSTAT, eRegisterLOUT, 
			eRegisterCOR0, eRegisterMFWR, eRegisterCBC, eRegisterIDCODE, eRegisterAXSS, 
			eRegisterCOR1, eRegisterCSOB, eRegisterWBSTAR, eRegisterTIMER, eRegisterBOOTSTS = 22, 
			eRegisterCTL1 = 24, eRegisterCount };
		//
		/// \brief Configuration command enumeration for eRegisterCMD.
		/// \see Command Register Codes: UG191, v3.7, June 24, 2009, Table 6-6.
		enum ECommand { eCommandNULL = 0, eCommandWCFG, eCommandMFW, eCommandDGHIGH, 
			eCommandLFRM = eCommandDGHIGH, eCommandRCFG, eCommandSTART, eCommandRCAP, 
			eCommandRCRC, eCommandAGHIGH, eCommandSWITCH, eCommandGRESTORE, eCommandSHUTDOWN, 
			eCommandGCAPTURE, eCommandDESYNCH, eCommandReserved, eCommandIPROG, 
			eCommandLTIMER = 17, eCommandCount };
		//
		/// \brief Frame Address Register subfields.
		/// \see Frame Address Register Description: UG191, v3.7, June 24, 2009, Table 6-8.
		enum EFar {
			eFarMaskBlockType =			0x00e00000,		eFarShiftBlockType =		21,
			eFarMaskTopBottom =			0x00100000,		eFarShiftTopBottom =		20,
			eFarMaskRow =				0x000f8000,		eFarShiftRow =				15,
			eFarMaskMajor =				0x00007f80,		eFarShiftMajor =			 7,
			eFarMaskMinor =				0x0000007f,		eFarShiftMinor =			 0
		};
		//
		/// \brief Frame Address Register top and bottom constants.
		/// \see Frame Address Register Description: UG191, v3.7, June 24, 2009, Table 6-8.
		enum EFarTopBottom { eFarTop = 0, eFarBottom = 1 };
		//
		/// \brief Frame Address Register block type constants.
		enum EFarBlockType { eFarBlockType0 = 0, eFarBlockType1, eFarBlockType2, eFarBlockType3, 
			eFarBlockType4, eFarBlockType5, eFarBlockType6, eFarBlockType7, eFarBlockTypeCount };
		//
		/// \brief Assorted constants.
		/// \see WBSTAR Register Description: UG191, v3.7, June 24, 2009, Table 6-12.
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
		/// \details Constant frame length of 41 32-bit words for the entire Virtex5 family.
		/// \see Virtex-5 Frame Count, Frame Length, Overhead, and Bitstream Size: UG191, v.3.7, 
		///		June 24, 2009, Table 6-1.
		enum { eFrameLength = 41 };
	protected:
	// enumerations
	// static variables
//		static const ColumnWidths sColumnTypeWidths[];
//		static const std::map<std::string, EColumnType> sTileNameToColumnType;
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
	public:
	// constructors
		/// \brief Basic constructor.
		Virtex5(void) : VirtexBitstream() {
//			for(int i = 0; i < eRegisterCount; i++) mRegister[i] = 0;
			// initialize the column type widths for this family
			mColumnDefs.resize(eColumnTypeCount);
			mColumnDefs[eColumnTypeEmpty] 	= ColumnDef("Empty",    0,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeBram]	= ColumnDef("Bram",    30,128,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeClb]		= ColumnDef("Clb",     36,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeClock] 	= ColumnDef("Clock",    4,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeDsp]		= ColumnDef("Dsp",     28,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeGtx]		= ColumnDef("Gtx",     32,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeIob]		= ColumnDef("Iob",     54,  0,  0,  0,  0,  0,  0,  0);
			// map type type names to column types
			mTileTypeNameToColumnType["BRAM"]		= eColumnTypeBram;
			mTileTypeNameToColumnType["PCIE_BRAM"]	= eColumnTypeBram;
			mTileTypeNameToColumnType["CLBLL"]		= eColumnTypeClb;
			mTileTypeNameToColumnType["CLBLM"]		= eColumnTypeClb;
			mTileTypeNameToColumnType["CLKV"]		= eColumnTypeClock;
			mTileTypeNameToColumnType["DSP"]		= eColumnTypeDsp;
			mTileTypeNameToColumnType["GTX"]		= eColumnTypeGtx;
			mTileTypeNameToColumnType["GTX_LEFT"]	= eColumnTypeGtx;
			mTileTypeNameToColumnType["GT3"]		= eColumnTypeGtx;
			mTileTypeNameToColumnType["CIOB"]		= eColumnTypeIob;
			mTileTypeNameToColumnType["LIOB"]		= eColumnTypeIob;
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
		/// \brief Loads partial bitstream frames into block data structure.
		//void initializePartialFrameBlocks(void);
		/// \brief Returns frames for queried bitstream co-ordinates
		VirtexFrameBlocks getBitstreamFrames(uint32_t inBlockCount, uint32_t inBitCol);
		/// \brief Returns frames for queried xdl co-ordinates
		VirtexFrameBlocks getXdlFrames(uint32_t inBlockCount, uint32_t inXdlCol);
	// accessors
		/// \brief Return the frame length for the current device.
		virtual uint32_t getFrameLength(void) const { return eFrameLength; }
	// inserters
		/// \brief Insert the bitstream header into an output stream.
		friend std::ostream& operator<< (std::ostream& os, const Virtex5& rhs);

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
			operator uint32_t (void) const {
				return uint32_t(((mTopBottom << eFarShiftTopBottom) & eFarMaskTopBottom) 
					| ((mBlockType << eFarShiftBlockType) & eFarMaskBlockType)
					| ((mRow << eFarShiftRow) & eFarMaskRow) 
					| ((mMajor << eFarShiftMajor) & eFarMaskMajor)
					| ((mMinor << eFarShiftMinor) & eFarMaskMinor));
			}
			friend std::ostream& operator<< (std::ostream& os, const Virtex5::FrameAddress& rhs) {
				return os << (rhs.mTopBottom == Virtex5::eFarTop ? 'T' : 'B')
					<< "" << rhs.mBlockType << "(" << rhs.mRow << "," << rhs.mMajor << "." 
					<< rhs.mMinor << ")";
			}
		private:
			//
		};
	protected:
	// typedefs
		/// \brief Map from frame index to frame address.
		typedef std::map<uint32_t, Virtex5::FrameAddress> FrameIndexToAddress;
		/// \brief Map from frame address to frame index.
		typedef std::map<Virtex5::FrameAddress, uint32_t> FrameAddressToIndex;
		/// \brief Array of vectors to store frame indexes of each block type
		typedef std::vector<uint32_t> IndexVector;

	// members
		/// \brief Map of frame indexes to frame addresses.
		FrameIndexToAddress mFrameIndexToAddress;
		/// \brief Map of frame addressee to frame indexes.
		FrameAddressToIndex mFrameAddressToIndex;
		/// \brief Vector to store frame indexes of XDL columns.
		IndexVector mBitColumnIndexes [Virtex5::eFarBlockTypeCount];
		/// \brief Vector to store frame indexes of Bitstream columns.
		IndexVector mXdlColumnIndexes [Virtex5::eFarBlockTypeCount];
		/// \brief Array to hold frame index boundaries for blocks.
		uint32_t mBlockFrameIndexBounds [Virtex5::eFarBlockTypeCount];
		/// \brief Map of xdl columns to bit columns.
		std::map<uint32_t, uint32_t> mXdlIndexToBitIndex;
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_VIRTEX5_HPP
