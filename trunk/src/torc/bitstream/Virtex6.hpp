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

#include <boost/integer.hpp>
#include "torc/bitstream/VirtexBitstream.hpp"
#include <map>

namespace torc { namespace architecture { class DDB; } }

namespace torc {
namespace bitstream {

namespace bitstream { class bitstream_virtex6; }

	/// \brief Virtex6 bitstream.
	class Virtex6 : public VirtexBitstream {
		friend class torc::bitstream::bitstream::bitstream_virtex6;
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
			eFarMaskBlockType =	0x00e00000,		eFarShiftBlockType =	21,
			eFarMaskTopBottom =	0x00100000,		eFarShiftTopBottom =	20,
			eFarMaskRow =				0x000f8000,		eFarShiftRow =				15,
			eFarMaskMajor =			0x00007f80,		eFarShiftMajor =			 7,
			eFarMaskMinor =			0x0000007f,		eFarShiftMinor =			 0
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
	public:
//	// constructors
//		/// \brief Basic constructor.
//		Virtex6(void) : VirtexBitstream() {
//			for(int i = 0; i < eRegisterCount; i++) mRegister[i] = 0;
//		}
	// functions
		/// \brief Return the masked value for a subfield of the specified register.
		static uint32_t makeSubfield(ERegister inRegister, const std::string& inSubfield, 
			const std::string& inSetting);
	// inserters
		/// \brief Insert the bitstream header into an output stream.
		friend std::ostream& operator<< (std::ostream& os, const Virtex6& rhs);
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
		typedef std::map<uint32_t, Virtex6::FrameAddress> FrameIndexToAddress;
		/// \brief Map from frame address to frame index.
		typedef std::map<Virtex6::FrameAddress, uint32_t> FrameAddressToIndex;
	// members
		/// \brief Map of frame indexes to frame addresses.
		FrameIndexToAddress mFrameIndexToAddress;
		/// \brief Map of frame addressee to frame indexes.
		FrameAddressToIndex mFrameAddressToIndex;
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_VIRTEX6_HPP
