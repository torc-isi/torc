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
/// \brief Source for the Spartan6 class.

#include "torc/bitstream/Spartan6.hpp"
#include <iostream>

namespace torc {
namespace bitstream {

	const char* Spartan6::sPacketTypeName[ePacketTypeCount] = {
		"[UNKNOWN TYPE 0]", "TYPE1", "TYPE2", "[UNKNOWN TYPE 3]", "[UNKNOWN TYPE 4]", 
		"[UNKNOWN TYPE 5]", "[UNKNOWN TYPE 6]", "[UNKNOWN TYPE 7]"
	};

	const char* Spartan6::sOpcodeName[eOpcodeCount] = {
		"NOP", "READ", "WRITE", "RESERVED"
	};

	const char* Spartan6::sRegisterName[eRegisterCount] = {
		"CRC", "FARMAJ", "FARMIN", "FDRI", "FDRO", "CMD", "CTL", "MASK", "STAT", "LOUT", "COR1", 
		"COR2", "PWRDN_REG", "FLR", "IDCODE", "CWDT", "HC_OPT_REG", "[UNUSED REGISTER 17]", "CSBO", 
		"GENERAL1", "GENERAL2", "GENERAL3", "GENERAL4", "GENERAL5", "MODE_REG", "PU_GWE", "PU_GTS", 
		"MFWR", "CCLK_FREQ", "SEU_OPT", "EXP_SIGN", "RDBK_SIGN", "BOOSTS", "EYE_MASK", "CBC_REG"
	};

	const char* Spartan6::sCommandName[eCommandCount] = {
		"NULL", "WCFG", "MFW", "LFRM", "RCFG", "START", "[UNKNOWN COMMAND 6]", "RCRC", "AGHIGH", 
		"[UNKNOWN COMMAND 9]", "GRESTORE", "SHUTDOWN", "[UNKNOWN COMMAND 12]", "DESYNC", "IPROG"
	};

#define VALUES (const char*[])

		/// \see Configuration Options Register Description: UG380, v2.2, July 30, 2010, Table 5-36
	const Bitstream::Subfield Spartan6::sCOR1[] = {
		{0x0003, 0, "StartupClk", "SSCLKSRC", 0,
			// bitgen: Cclk, UserClk, JtagClk
			// config: 00:"CCLK", 01:"UserClk", 1x:"TCK"
			VALUES{"Cclk", "UserClk", "JtagClk", "TCK", 0}},
		{0x0004, 2, "DriveDone", "DRIVE_DONE", 0,
			// bitgen: No, Yes
			// config: 0:"DONE pin is open drain", 1:"DONE is actively driven high"
			VALUES{"No", "Yes", 0}}, 
		{0x0008, 3, "DonePipe", "DONE_PIPE", 0,
			// bitgen: No, Yes
			// config: 0:"No pipeline stage for DONEIN", 1:"Add pipeline stage for DONEIN"
			VALUES{"No", "Yes", 0}},
		{0x0010,  4, "CRC_Bypass", "CRC_BYPASS", 0, 
			// bitgen: n/a?
			// config: Does not check against updated CRC
			VALUES{"0", "1", 0}},
		{0x1000, 15, "Drive_Awake", "DRIVE_AWAKE", 0,
			// bitgen: n/a?
			// config: 0:"Does not drive the awake pin, 1:"Actively drives the awake pin"
			VALUES{"0", "1", 0}},
		{0, 0, 0, 0, 0, 0}
	};


		/// \see Configuration Options Register Description: UG380, v2.2, July 30, 2010, Table 5-36
	const Bitstream::Subfield Spartan6::sCOR2[] = { 
		{0x0007,  0, "GWE_cycle", "GWE_CYCLE", 6,
			// bitgen: 6, 1, 2, 3, 4, 5, Done, Keep
			// config: 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x0038,  3, "GTS_cycle", "GTS_CYCLE", 5,
			// bitgen: 5, 1, 2, 3, 4, 6, Done, Keep
			// config: 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x01c0,  6, "LCK_cycle", "LOCK_CYCLE", 7,
			// bitgen: NoWait, 0, 1, 2, 3, 4, 5, 6
			// config: 000:"1", 001:"2", 010:"3", 011:"4", 100:"5", 101:"6", 111:"NO_WAIT"
			VALUES{"1", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "NoWait", 0}},
		{0x0E00,  9, "DONE_cycle", "DONE_CYCLE", 4,
			// bitgen: 4, 1, 2, 3, 5, 6
			// config: 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x8000, 15, "Reset_on_err", "RESET_ON_ERROR", 0,
			// bitgen: No, Yes
			// config: 0:"Disable reset on error", 1:"Enable reset on error"
			VALUES{"Disable", "Enable", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Status Register Description: UG380, v2.2, July 30, 2010, Table 5-35
	/// \note The "bitgen" names attempt to mimic the general bitgen convention.
	const Bitstream::Subfield Spartan6::sSTAT[] = { 
		{0x0001,  0, "CRC_error", "CRC_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"No CRC error", 1:"CRC error"
			VALUES{"No", "Yes", 0}},
		{0x0002,  1, "ID_error", "ID_ERROR", 0, 
			// bitgen: n/a
			// config: IDCODE not validated while trying to write FDRI
			VALUES{"No", "Yes", 0}},
		{0x0004,  2, "DCM_locked", "DCM_LOCK", 0, 
			// bitgen: n/a
			// config: 0:"DCMs not locked", 1:"DCMs are locked"
			VALUES{"No", "Yes", 0}},
		{0x0008,  3, "GTS_CFG", "GTS_CFG_B", 0, 
			// bitgen: n/a
			// config: 0:"All I/Os are placed in high-Z state", 1:"All I/Os behave as configured"
			VALUES{"No", "Yes", 0}},
		{0x0010,  4, "GWE", "GWE", 0, 
			// bitgen: n/a
			// config: Status of Global Write Enable
			VALUES{"No", "Yes", 0}},
		{0x0020,  5, "GWHIGH", "GWHIGH_B", 0, 
			// bitgen: n/a
			// config: 0:"GHIGH_B asserted", 1:"GHIGH_B deasserted"
			VALUES{"No", "Yes", 0}},
		{0x0040,  6, "DEC_ERROR", "DEC_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"No DEC Error", 1:"DEC Error"
			VALUES{"No", "Yes", 0}},
		{0x0080,  7, "PART_SECURED", "PART_SECURED", 0, 
			// bitgen: n/a
			// config: 0:"Decryption security not set", 1:"Decyrption security set"
			VALUES{"No", "Yes", 0}},
		{0x0100,  8, "HSWAPEN", "HSWAPEN", 0, 
			// bitgen: n/a
			// config: HSWAPEN Status
			VALUES{"No", "Yes", 0}},
		{0x0800,  9, "Mode", "MODE", 0, 
			// bitgen: n/a
			// config: Status of the MODE pins (M1, M0, 0)
			VALUES{"MasterSerial", "SlaveSelectMap32", "[UNDEFINED 2]", "MasterSelectMap", 
				"[UNDEFINED 3]", "JTAG", "SlaveSelectMap8", "[UNDEFINED 6]", "SlaveSerial", 0}},
		{0x1000, 12, "INIT", "INIT_B", 0, 
			// bitgen: n/a
			// config: Value on INIT pin
			VALUES{"Deasserted", "Asserted", 0}},
		{0x2000, 13, "Done", "DONE", 0, 
			// bitgen: n/a
			// config: Value on DONE pin
			VALUES{"Deasserted", "Asserted", 0}},
		{0x4000, 14, "IN_PWRDN", "IN_PWRDN", 0, 
			// bitgen: n/a
			// config: Suspend status
			VALUES{"No", "Yes", 0}},
		{0x4000, 15, "SWWD_Strikeout", "SWWD_strikeout", 0, 
			// bitgen: n/a
			// config: Indicates error to configure for reasons of failure to find
			// the sync word within the Configuration WatchDog timer
			// (CWDT) count, invalid IDCODE, or CRC error. See the
			// BOOTSTS register for the specific cause of failure. INIT
			// is pulled Low and SWWD_strikeout goes High.
			VALUES{"No", "Yes", 0}},
		{0, 0, 0, 0, 0, 0}
	};

		/// \see Control Register Description: UG380, v2.2, July 30, 2010, Table 5-34
	const Bitstream::Subfield Spartan6::sCTL[] = { 
		{0x0002,  1, "CRC_EXTSTAT_DISABLE", "CRC_EXTSTAT_DISABLE", 0, 
			// bitgen: n/a?
			// config: 0:"CRC indicator disabled", 1:"CRC Indicator enabled"
			VALUES{"0", "1", 0}},
		{0x0004,  2, "USE_EFUSE_KEY", "USE_EFUSE_KEY", 0, 
			// bitgen: n/a?
			// config: 0:"Use battery backed ram", 1:"Use eFuse key"
			VALUES{"0", "1", 0}},
		{0x0008,  3, "Persist", "PERSIST", 0, 
			// bitgen: No, Yes
			// config: 0:"No (default)", 1:"Yes"
			VALUES{"No", "Yes", 0}},
		{0x0030,  4, "Security", "SBITS", 0, 
			// bitgen: None, Level1, Level2
			// config: 00:"Read/Write OK (default)", 01:"Readback disabled", 1x:"Readback disabled, 
			//	writing disabled except CRC register."
			VALUES{"None", "Level1", "Level2", "Level2", 0}},
		{0x0040,  6, "DEC", "DEC", 0, 
			// bitgen: No, Yes
			// config: 0:"No Decryption (default)", 1:"Yes Decryption Used"
			VALUES{"No", "Yes", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Control Mask Register Description: Inferred from Table 5-34
	const Bitstream::Subfield Spartan6::sMASK[] = { 
		{0x0001,  0, "RESERVED", "RESERVED", 0, VALUES{"Protected", "Writable", 0}},
		{0x0002,  1, "CRC_EXTSTAT_DISABLE", "CRC_EXTSTAT_DISABLE", 0, 
			VALUES{"Protected", "Writable", 0}},
		{0x0004,  2, "USE_EFUSE_KEY", "USE_EFUSE_KEY", 0, VALUES{"Protected", "Writable", 0}},
		{0x0008,  3, "Persist", "PERSIST", 0, VALUES{"Protected", "Writable", 0}},
		{0x0030,  4, "Security", "SBITS", 0, 
			VALUES{"Protected", "[UNKNOWN 1]", "[UNKNOWN 2]", "Writable", 0}},
		{0x0040,  6, "DEC", "DEC", 0, VALUES{"Protected", "Writable", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Suspend Register Description: UG380, v2.2, July 30, 2010, Table 5-37
	const Bitstream::Subfield Spartan6::sPWRDN_REG[] = { 
		{0x0001,  0, "KEEP_SCLK", "KEEP_SCLK", 1, 
			// bitgen: n/a?
			// config: 0:"Use MCCLK for startup sequence initiated by power-up", 1:"Use SSCLKSRC for
			// startup sequence initiated by power-up"
			VALUES{"0", "1", 0}},
		{0x0004,  2, "EN_PWRDN", "EN_PWRDN", 0, 
			// bitgen: n/a?
			// config: 0:"Suspend is disabled", 1:"Suspend is enabled"
			VALUES{"0", "1", 0}},
		{0x0010,  4, "EN_PGSR", "EN_PGSR", 0, 
			// bitgen: No, Yes
			// config: 0:"No GSR pulse during return from suspend", 1:"Generate GSR pulse during 
			// return from suspend"
			VALUES{"No", "Yes", 0}},
		{0x0020,  5, "FILTER", "FILTER_B", 0, 
			// bitgen: n/a
			// config: 0:"Suspend filter (300ns) on, 1:"Filter off"
			VALUES{"0", "1", 0}},
		{0x4000,  14, "EN_EYES", "EN_EYES", 0, 
			// bitgen: No, Yes
			// config: 0:"Disable Multi-Pin Wake-up
			VALUES{"Disable", "Enable", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see HC_OPT_REG Register Description: UG380, v2.2, July 30, 2010, Table 5-40
	const Bitstream::Subfield Spartan6::sHC_OPT_REG[] = { 
		{0x0040,  6, "INIT_SKIP", "INIT_SKIP", 0, 
			// bitgen: No, Yes
			// config: 0:"Do not skip initialization", 1:"Skip initialization"
			VALUES{"0", "1", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Mode Register Description: UG380, v2.2, July 30, 2010, Table 5-44
	const Bitstream::Subfield Spartan6::sMODE_REG[] = { 
		{0x0080,  0, "BOOTVSEL", "BOOTVSEL", 0, 
			// bitgen: n/a?
			// config: Read only
			VALUES{"[READ ONLY]", 0}},
		{0x0400,  8, "BOOTMODE", "BOOTMODE", 1, 
			// bitgen: n/a?
			// config: x00: BOOTMODE<0>, x10: BOOTMODE<1>
			VALUES{"0", "1", 0}},
		{0x1000,  11, "BUSWIDTH", "BUSWIDTH", 0, 
			// bitgen: n/a?
			// config: Buswidth setting to reboot
			VALUES{"0", "1", "2", 0}},
		{0x2000,  13, "NEW_MODE", "NEW_MODE", 0, 
			// bitgen: n/a
			// config: 0:"Physical Mode", 1:"Bitstream mode"
			VALUES{"No", "Yes", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see BOOSTS Register Description: UG380, v2.2, July 30, 2010, Table 5-48
	const Bitstream::Subfield Spartan6::sBOOSTS[] = { 
		{0x0001,  0, "VALID_0", "VALID_O", 0, 
			// bitgen: n/a?
			// config: Status valid
			VALUES{"0", "1", 0}},
		{0x0002,  1, "FALLBACK_O", "FALLBACK_0", 0, 
			// bitgen: n/a?
			// config: 0: Normal Configuration, 1: Fallback to golden bit stream address.
			VALUES{"0", "1", 0}},
		{0x0008,  3, "WTO_ERROR_0", "WTO_ERROR_0", 0, 
			// bitgen: n/a?
			// config: Watch dog time-out error.
			VALUES{"0", "1", 0}},
		{0x0010,  4, "ID_ERROR_0", "ID_ERROR_0", 0, 
			// bitgen: n/a?
			// config: IDCODE error.
			VALUES{"0", "1", 0}},
		{0x0020,  5, "CRC_ERROR_0", "CRC_ERROR_0", 0, 
			// bitgen: n/a?
			// config: CRC error.
			VALUES{"0", "1", 0}},
		{0x0040,  6, "VALID_1", "VALID_1", 0, 
			// bitgen: n/a?
			// config: Status valid
			VALUES{"0", "1", 0}},
		{0x0080,  7, "FALLBACK_1", "FALLBACK_1", 0, 
			// bitgen: n/a?
			// config: 0: Normal Configuration, 1: Fallback to 00 address.
			VALUES{"0", "1", 0}},
		{0x0200,  9, "WTO_ERROR_1", "WTO_ERROR_1", 0, 
			// bitgen: n/a?
			// config: Watch dog time-out error.
			VALUES{"0", "1", 0}},
		{0x0400,  10, "ID_ERROR_1", "ID_ERROR_1", 0, 
			// bitgen: n/a?
			// config: IDCODE error.
			VALUES{"0", "1", 0}},
		{0x0800,  11, "CRC_ERROR_1", "CRC_ERROR_1", 0, 
			// bitgen: n/a?
			// config: CRC error.
			VALUES{"0", "1", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see SEU_OPT Register Description: UG380, v2.2, July 30, 2010, Table 5-49
	const Bitstream::Subfield Spartan6::sSEU_OPT[] = { 
		{0x0001,  0, "SEU_ENABLE", "SEU_ENABLE", 0, 
			// bitgen: n/a?
			// config: SEU Detection Enable/Disable
			VALUES{"DISABLE", "ENABLE", 0}},
		{0x0002,  1, "GLUT_MASK", "GLUT_MASK", 1, 
			// bitgen: n/a?
			// config: 0: Unmask, 1: Mask out LUTRAM/SRL
			VALUES{"Unmask", "Mask LUTRAM/SRL", 0}},
		{0x0008,  3, "SEU_RUN_ON_ERR", "SEU_RUN_ON_ERR", 0, 
			// bitgen: n/a?
			// config: If SEU_ERR detected, keep running
			VALUES{"Halt", "Run", 0}},
		{0x0010,  4, "SEU_FREQ", "SEU_FREQ", 0, 
			// bitgen: n/a?
			// config: BUS Clock frequency during SEU
			VALUES{"1be",  0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \brief Return the masked value for a subfield of the specified register.
	uint16_t Spartan6::makeSubfield(ERegister inRegister, const std::string& inSubfield, 
		const std::string& inSetting) {
		const Subfield* subfields;
		switch(inRegister) {
		case eRegisterCOR1: subfields = sCOR1; break;
		case eRegisterCOR2: subfields = sCOR2; break;
		case eRegisterSTAT: subfields = sSTAT; break;
		case eRegisterCTL: subfields = sCTL; break;
		case eRegisterMASK: subfields = sMASK; break;
		case eRegisterPWRDN_REG: subfields = sPWRDN_REG; break;
		case eRegisterHC_OPT_REG: subfields = sHC_OPT_REG; break;
		case eRegisterMODE_REG: subfields = sMODE_REG; break;
		case eRegisterBOOSTS: subfields = sBOOSTS; break;
		case eRegisterSEU_OPT: subfields = sSEU_OPT; break;
		default: return 0;
		}
		for(uint16_t field = 0; subfields[field].mMask != 0; field++) {
			const Subfield& subfield = subfields[field];
			if(inSubfield != subfield.mBitgenName && inSubfield != subfield.mConfigGuideName) 
				continue;
			const char** ptr = subfield.mValues;
			for(uint32_t i = 0; *ptr != 0; i++, ptr++) {
				if(inSetting == *ptr) return (i << subfield.mShift) & subfield.mMask;
			}
		}
		return 0;
	}


} // namespace bitstream
} // namespace torc
