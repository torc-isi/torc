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
/// \brief Header for the Spartan6 class.

#ifndef TORC_BITSTREAM_SPARTAN6_HPP
#define TORC_BITSTREAM_SPARTAN6_HPP

#include <boost/cstdint.hpp>
#include "torc/bitstream/Spartan6Bitstream.hpp"

namespace torc {
namespace bitstream {

namespace bitstream { class Spartan6UnitTest; }

	/// \brief Spartan6 bitstream.
	class Spartan6 : public Spartan6Bitstream {
		friend class torc::bitstream::bitstream::Spartan6UnitTest;
	protected:
	// typedefs
		/// \brief Imported type name.
		typedef boost::uint16_t uint16_t;
		typedef boost::uint32_t uint32_t;
	public:
	// enumerations
		//
		/// \brief Configuration register enumeration.
		/// \see configuration registers: UG380, v2.2, July 30, 2010 Table 5-30
		enum ERegister { eRegisterCRC = 0, eRegisterFARMAJ, eRegisterFARMIN, eRegisterFDRI, eRegisterFDRO, 
			eRegisterCMD, eRegisterCTL, eRegisterMASK, eRegisterSTAT, eRegisterLOUT, eRegisterCOR1, 
			eRegisterCOR2, eRegisterPWRDN_REG, eRegisterFLR, eRegisterIDCODE, eRegisterCWDT, eRegisterHC_OPT_REG,
			eRegisterCSBO, eRegisterGENERAL1, eRegisterGENERAL2, eRegisterGENERAL3, eRegisterGENERAL4,
		 	eRegisterGENERAL5, eRegisterMODE_REG, eRegisterPU_GWE, eRegisterPU_GTS, eRegisterMFWR, eRegisterCCLK_FREQ,
			eRegisterSEU_OPT, eRegisterEXP_SIGN, eRegisterRDBK_SIGN, eRegisterBOOSTS, eRegisterEYE_MASK,
			eRegisterCBC_REG, eRegisterCount };
		//
		/// \brief Configuration command enumeration for eRegisterCMD.
		/// \see CMD register commands: UG380, v2.2, July 30, 2010 Table 5-33
		enum ECommand { eCommandNULL = 0, eCommandWCFG, eCommandMFW, eCommandLFRM, eCommandRCFG, 
			eCommandSTART, eCommandRCRC = 7, eCommandAGHIGH, eCommandGRESTORE = 10, eCommandSHUTDOWN, eCommandDESYNC = 13,
		  eCommandIPROG, eCommandCount };
		//
		/// \brief Frame Address Register Major subfields.
		/// \see Frame Address Register Major Description: UG380, v2.2, July 30, 2010 Table 5-31
		enum EFarMaj {
			eFarMajMaskBlock =	0xf000,		eFarMajShiftBlock =	12,
			eFarMajMaskRow =	0x0f00,		eFarMajShiftRow =	 8,
			eFarMajMaskMajor =	0x00ff,		eFarMajShiftMajor =	 0
		};
		//
		/// \brief Frame Address Register Minor subfields.
		/// \see Frame Address Register Minor Description: UG380, v2.2, July 30, 2010 Table 5-32
		enum EFarMin {
			eFarMinMaskBlock =	0xc000,		eFarMinShiftBlock =	12,
			eFarMinMaskMinor =	0x01ff,		eFarMinShiftMinor =	 0
		};
		//
		/// \brief Frame Address Register top and bottom constants.
		//enum EFarTopBottom { eFarTop = 0, eFarBottom = 1 };
		/// \brief Frame Address Register block type constants.
		enum EFarBlockType { eFarBlockType0 = 0, eFarBlockType1, eFarBlockType2, eFarBlockType3, 
			eFarBlockType4, eFarBlockType5, eFarBlockType6, eFarBlockType7, eFarBlockTypeCount };
		/// \brief Frame length.
		/// \details Constant frame length of 41 32-bit words for the entire Spartan6 family.  Note 
		///		that this length applies only to non-IOB frames.  IOB frame lengths are defined by 
		///		the bitstream FLR register plus one.
		enum { eFrameLength = 41 };
	protected:
	// members
//		/// \brief Configuration controller registers.
//		uint16_t mRegister[eRegisterCount];
		/// \brief Packet type names.
		static const char* sPacketTypeName[ePacketTypeCount];
		/// \brief Packet opcode names.
		static const char* sOpcodeName[eOpcodeCount];
		/// \brief Configuration register names.
		static const char* sRegisterName[eRegisterCount];
		/// \brief Configuration command names.
		static const char* sCommandName[eCommandCount];
		/// \brief Configuration Options Register 1 (COR1) subfields.
		static const Subfield sCOR1[];
		/// \brief Configuration Options Register 2 (COR2) subfields.
		static const Subfield sCOR2[];
		/// \brief Status Register (STAT) subfields.
		static const Subfield sSTAT[];
		/// \brief Control Register 0 (CTL0) subfields.
		static const Subfield sCTL[];
		/// \brief Control Mask Register (MASK) subfields.
		static const Subfield sMASK[];
		/// \brief Suspend Register (PWRDN_REG) subfields.
		static const Subfield sPWRDN_REG[];
		/// \brief HC_OPT_REG Register (PWRDN_REG) subfields.
		static const Subfield sHC_OPT_REG[];
		/// \brief Mode Register (Mode) subfields.
		static const Subfield sMODE_REG[];
		/// \brief BOOSTS Register (BOOSTS) subfields.
		static const Subfield sBOOSTS[];
		/// \brief SEU_OPT Register (SEU_OPT) subfields.
		static const Subfield sSEU_OPT[];
	public:
		static uint16_t makeSubfield(ERegister inRegister, const std::string& inSubfield, 
			const std::string& inSetting);
	// inserters
		/// \brief Insert the bitstream header into an output stream.
		friend std::ostream& operator<< (std::ostream& os, const Spartan6& rhs);
	// inner classes
		class FrameMajorAddress {
		protected:
			void assign(uint16_t inMajAddress) {
				mMajBlock = (inMajAddress & eFarMajMaskBlock) >> eFarMajShiftBlock;
				mMajRow = (inMajAddress & eFarMajMaskRow) >> eFarMajShiftRow;
				mMajMajor = (inMajAddress & eFarMajMaskMajor) >> eFarMajShiftMajor;
			}
		public:
			FrameMajorAddress(void) : mMajBlock(0), mMajRow(0), mMajMajor(0) {}
			FrameMajorAddress(uint16_t inMajBlock, uint16_t inMajRow, uint16_t inMajMajor)
			 						: mMajBlock(inMajBlock), mMajRow(inMajRow), mMajMajor(inMajMajor) {}
			FrameMajorAddress(uint16_t inMajAddress) { assign(inMajAddress); }
			uint16_t mMajBlock;
			uint16_t mMajRow;
			uint16_t mMajMajor;

			bool operator== (const FrameMajorAddress& rhs) const {
				return mMajBlock == rhs.mMajBlock && mMajRow == rhs.mMajRow && mMajMajor == rhs.mMajMajor;
			}
			bool operator< (const FrameMajorAddress& rhs) const {
				int diffMajBlock = mMajBlock - rhs.mMajBlock;
				if(diffMajBlock) return diffMajBlock < 0;
				int diffMajRow = mMajRow - rhs.mMajRow;
				if(diffMajRow) return diffMajRow < 0;
				return mMajMajor < rhs.mMajMajor;
			}
		private:
		};

		class FrameMinorAddress {
		protected:
			void assign(uint16_t inMinAddress) {
				mMinBlock = (inMinAddress & eFarMinMaskBlock) >> eFarMinShiftBlock;
				mMinMinor = (inMinAddress & eFarMinMaskMinor) >> eFarMinShiftMinor;
			}
		public:
			FrameMinorAddress(void) : mMinBlock(0), mMinMinor(0) {}
			FrameMinorAddress(uint16_t inMinBlock, uint16_t inMinMinor) : mMinBlock(inMinBlock), mMinMinor(inMinMinor) {}
			FrameMinorAddress(uint16_t inMinAddress) { assign(inMinAddress); }
			uint16_t mMinBlock;
			uint16_t mMinMinor;

			bool operator== (const FrameMinorAddress& rhs) const {
				return mMinBlock == rhs.mMinBlock && mMinMinor == rhs.mMinMinor;
			}
			bool operator< (const FrameMinorAddress& rhs) const {
				int diffMinBlock = mMinBlock - rhs.mMinBlock;
				if(diffMinBlock) return diffMinBlock < 0;
				return mMinMinor < rhs.mMinMinor;
			}
		private:
		};

	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_SPARTAN6_HPP
