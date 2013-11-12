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
/// \brief Header for the Spartan3E class.

#ifndef TORC_BITSTREAM_SPARTAN3E_HPP
#define TORC_BITSTREAM_SPARTAN3E_HPP

#include <boost/cstdint.hpp>
#include <boost/filesystem.hpp>
#include "torc/bitstream/SpartanBitstream.hpp"
#include <map>

namespace torc { namespace architecture { class DDB; } }

namespace torc {
namespace bitstream {

namespace bitstream { class Spartan3EUnitTest; }
namespace bitstream { class Spartan3EFarUnitTest; }
namespace bitstream { void testSpartan3EDevice(const std::string& inDeviceName, 
	const boost::filesystem::path& inWorkingPath); }

	/// \brief Spartan3E bitstream.
	class Spartan3E : public SpartanBitstream {
		friend class torc::bitstream::bitstream::Spartan3EUnitTest;
		friend class torc::bitstream::bitstream::Spartan3EFarUnitTest;
		friend void torc::bitstream::bitstream::testSpartan3EDevice(const std::string& inDeviceName, 
			const boost::filesystem::path& inWorkingPath);
	protected:
	// typedefs
		/// \brief Imported type name.
		typedef boost::uint32_t uint32_t;
	public:
	// enumerations
		//
		/// \brief Configuration register enumeration.
		/// \see configuration registers: XAPP452, v1.1, June 25, 2008, Table 1.
		enum ERegister { eRegisterCRC = 0, eRegisterFAR, eRegisterFDRI, eRegisterFDRO, 
			eRegisterCMD, eRegisterCTL, eRegisterMASK, eRegisterSTAT, eRegisterLOUT, eRegisterCOR, 
			eRegisterMFWR, eRegisterFLR, eRegisterIDCODE = 14, eRegisterCount };
		//
		/// \brief Configuration command enumeration for eRegisterCMD.
		/// \see CMD register commands: XAPP452, v1.1, June 25, 2008, Table 2.
		enum ECommand { eCommandNULL = 0, eCommandWCFG, eCommandMFWR, eCommandLFRM, eCommandRCFG, 
			eCommandSTART, eCommandRCAP, eCommandRCRC, eCommandAGHIGH, eCommandSWITCH, 
			eCommandGRESTORE, eCommandSHUTDOWN, eCommandGCAPTURE, eCommandDESYNCH, eCommandCount };
		//
		/// \brief Frame Address Register subfields.
		/// \see Frame Address Register Description: XAPP452, v1.1, June 25, 2008, Figure 2. 
		enum EFar {
			eFarMaskColumn =	0x06000000,		eFarShiftColumn =	25,
			eFarMaskMajor =		0x01fe0000,		eFarShiftMajor  =	17,
			eFarMaskMinor =		0x0001fe00,		eFarShiftMinor  =	 9
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
		enum EColumnType { eColumnTypeEmpty = 0, eColumnTypeBram, eColumnTypeBramInt, eColumnTypeClb,
		 	 eColumnTypeClock, eColumnTypeClockLL, eColumnTypeTerm, eColumnTypeIob, eColumnTypeCount };
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
		Spartan3E(void) : SpartanBitstream() {
//			for(int i = 0; i < eRegisterCount; i++) mRegister[i] = 0;
			// initialize the column type widths for this family
			mColumnDefs.resize(eColumnTypeCount);
			mColumnDefs[eColumnTypeEmpty] 	= ColumnDef("Empty",    0,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeBram]	= ColumnDef("Bram",     0, 76,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeBramInt]	= ColumnDef("BramInt",  0,  0, 19,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeClb]		= ColumnDef("Clb",     19,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeClock] 	= ColumnDef("Clock",    3,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeClockLL] = ColumnDef("ClockLL",  4,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeIob]		= ColumnDef("Iob",     19,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeTerm]	= ColumnDef("Term",     2,  0,  0,  0,  0,  0,  0,  0);
			// map type type names to column types
			mTileTypeNameToColumnType["BRAMSITE2"]		= eColumnTypeBram;
			mTileTypeNameToColumnType["BRAM3_SMALL"]	= eColumnTypeBramInt;
			mTileTypeNameToColumnType["CENTER_SMALL"]	= eColumnTypeClb;
			mTileTypeNameToColumnType["CLKV"]			= eColumnTypeClock;
			mTileTypeNameToColumnType["CLKV_LL"]		= eColumnTypeClockLL;
			mTileTypeNameToColumnType["LIOIS"]			= eColumnTypeIob;
			mTileTypeNameToColumnType["RIOIS"]			= eColumnTypeIob;
			mTileTypeNameToColumnType["LTERM"]			= eColumnTypeTerm;
			mTileTypeNameToColumnType["RTERM"]			= eColumnTypeTerm;
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
	// inserters
		/// \brief Insert the bitstream header into an output stream.
		friend std::ostream& operator<< (std::ostream& os, const Spartan3E& rhs);
	// inner classes
		class FrameAddress {
		protected:
			void assign(uint32_t inAddress) {
				mColumn = (inAddress & eFarMaskColumn) >> eFarShiftColumn;
				mMajor = (inAddress & eFarMaskMajor) >> eFarShiftMajor;
				mMinor = (inAddress & eFarMaskMinor) >> eFarShiftMinor;
			}
		public:
			FrameAddress(void) : mColumn(0), mMajor(0), mMinor(0) {}
			FrameAddress(uint32_t inColumn,	uint32_t inMajor, uint32_t inMinor) : 
				mColumn(inColumn), mMajor(inMajor), mMinor(inMinor) {}
			FrameAddress(uint32_t inAddress) { assign(inAddress); }
			uint32_t mColumn;
			uint32_t mMajor;
			uint32_t mMinor;
			bool operator== (const FrameAddress& rhs) const {
				return  mColumn == rhs.mColumn && mMajor == rhs.mMajor && mMinor == rhs.mMinor;
			}
			bool operator< (const FrameAddress& rhs) const {
				int diffColumn = mColumn - rhs.mColumn;
				if(diffColumn) return diffColumn < 0;
				int diffMajor = mMajor - rhs.mMajor;
				if(diffMajor) return diffMajor < 0;
				return mMinor < rhs.mMinor;
			}
		};
	// accessors
		virtual uint32_t getFrameLength(void) const {
			using namespace torc::common;
			// lengths obtained directly from bitstream Frame Length Register
			switch(mDevice) {
				case eXC3S100E: return 49;
				case eXC3S250E: return 73;
				case eXC3S500E: return 97;
				case eXC3S1200E: return 125;
				case eXC3S1600E: return 157;
				default: return 0;
			}
		}
	protected:
	// typedefs
		/// \brief Map from frame index to frame address.
		typedef std::map<uint32_t, Spartan3E::FrameAddress> FrameIndexToAddress;
		/// \brief Map from frame address to frame index.
		typedef std::map<Spartan3E::FrameAddress, uint32_t> FrameAddressToIndex;
		/// \brief Array of vectors to store frame indexes of each block type
		typedef std::vector<uint32_t> ColumnIndexVector;

	// members
		/// \brief Map of frame indexes to frame addresses.
		FrameIndexToAddress mFrameIndexToAddress;
		/// \brief Map of frame addressee to frame indexes.
		FrameAddressToIndex mFrameAddressToIndex;
		/// \brief Vector to store frame indexes of XDL columns.
		ColumnIndexVector mBitColumnIndexes[Spartan3E::eFarBlockTypeCount];
		/// \brief Vector to store frame indexes of Bitstream columns.
		ColumnIndexVector mXdlColumnIndexes[Spartan3E::eFarBlockTypeCount];
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_SPARTAN3E_HPP
