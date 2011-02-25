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
#include "torc/bitstream/VirtexBitstream.hpp"

namespace torc {
namespace bitstream {

namespace bitstream { class bitstream_virtex; }

	/// \brief Virtex bitstream.
	class Virtex : public VirtexBitstream {
		friend class torc::bitstream::bitstream::bitstream_virtex;
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
			eFarMaskBlockType =		0x06000000,		eFarShiftBlockType =	25,
			eFarMaskMajor =				0x01FE0000,		eFarShiftMajor =			17,
			eFarMaskMinor =				0x0001FE00,		eFarShiftMinor =			 9,
		};

		/// \brief Frame Address Register block type constants.
		enum EFarBlockType { eFarBlockType0 = 0, eFarBlockType1, eFarBlockType2, eFarBlockType3, 
			eFarBlockType4, eFarBlockType5, eFarBlockType6, eFarBlockType7, eFarBlockTypeCount };
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
		static uint32_t makeSubfield(ERegister inRegister, const std::string& inSubfield, 
			const std::string& inSetting);
//	// constructors
//		/// \brief Basic constructor.
//		Virtex(void) : VirtexBitstream() {
//			for(int i = 0; i < eRegisterCount; i++) mRegister[i] = 0;
//		}
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
			FrameAddress(void) : mBlockType(eFarBlockType0),
				mMajor(0), mMinor(0) {}
			FrameAddress(EFarBlockType inBlockType,
				uint32_t inMajor, uint32_t inMinor) : 
				mBlockType(inBlockType), mMajor(inMajor), mMinor(inMinor) {}
			FrameAddress(uint32_t inAddress) { assign(inAddress); }
			EFarBlockType mBlockType;
			uint32_t mMajor;
			uint32_t mMinor;
			bool operator== (const FrameAddress& rhs) const {
				return mBlockType == rhs.mBlockType 
					&& mMajor == rhs.mMajor && mMinor == rhs.mMinor;
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
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_VIRTEX_HPP
