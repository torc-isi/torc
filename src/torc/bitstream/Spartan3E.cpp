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
/// \brief Source for the Spartan3E class.

#include "torc/bitstream/Spartan3E.hpp"
#include <iostream>

namespace torc {
namespace bitstream {

	const char* Spartan3E::sPacketTypeName[ePacketTypeCount] = {
		"[UNKNOWN TYPE 0]", "TYPE1", "TYPE2", "[UNKNOWN TYPE 3]", "[UNKNOWN TYPE 4]", 
		"[UNKNOWN TYPE 5]", "[UNKNOWN TYPE 6]", "[UNKNOWN TYPE 7]"
	};

	const char* Spartan3E::sOpcodeName[eOpcodeCount] = {
		"NOP", "READ", "WRITE", "RESERVED"
	};

	const char* Spartan3E::sRegisterName[eRegisterCount] = {
		"CRC", "FAR", "FDRI", "FDRO", "CMD", "CTL", "MASK", "STAT", "LOUT", "COR", "MFWR", "FLR",
	 	"[UNKNOWN_REGISTER 12]", "[UNKNOWN_REGISTER 13]", "IDCODE"
	};

	const char* Spartan3E::sCommandName[eCommandCount] = {
		"NULL", "WCFG", "MFWR", "LFRM", "RCFG", "START", "RCAP", "RCRC", "AGHIGH", "SWITCH", 
		"GRESTORE", "SHUTDOWN", "GCAPTURE", "DESYNCH"
	};

#define VALUES (const char*[])

		/// \see Configuration Options Register Description: XAPP452, v1.1, June 25, 2008, Table 5.
	const Bitstream::Subfield Spartan3E::sCOR[] = { 
		{0x00000007,  0, "GWE_cycle", "GWE_CYCLE", 5,
			// bitgen: 6, 1, 2, 3, 4, 5, Done, Keep
			// config: 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x00000038,  3, "GTS_cycle", "GTS_CYCLE", 4,
			// bitgen: 5, 1, 2, 3, 4, 6, Done, Keep
			// config: 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x000001c0,  6, "LCK_cycle", "LOCK_CYCLE", 7,
			// bitgen: NoWait, 0, 1, 2, 3, 4, 5, 6
			// config: 000:"1", 001:"2", 010:"3", 011:"4", 100:"5", 101:"6", 111:"NO_WAIT"
			VALUES{"1", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "NoWait", 0}},
		{0x00000E00,  9, "Match_cycle", "MATCH_CYCLE", 7,
			// bitgen: Auto, NoWait, 0, 1, 2, 3, 4, 5, 6
			// config: 000:"1", 001:"2", 010:"3", 011:"4", 100:"5", 101:"6", 111:"NO_WAIT"
			VALUES{"1", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "NoWait", 0}},
		{0x00007000, 12, "DONE_cycle", "DONE_CYCLE", 3,
			// bitgen: 4, 1, 2, 3, 5, 6
			// config: 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x00018000, 15, "StartupClk", "SSCLKSRC", 0,
			// bitgen: Cclk, UserClk, JtagClk
			// config: 00:"CCLK", 01:"UserClk", 1x:"JTAGClk"
			VALUES{"Cclk", "UserClk", "JtagClk", "JtagClk", 0}},
		{0x00780000, 19, "ConfigRate", "OSCFSEL", 0,
			// bitgen: 1, 3, 6, 12, 25, 50
			// config: values undefined
			VALUES{
				"[UNKNOWN 0]", "[UNKNOWN 1]", "[UNKNOWN 2]", "[UNKNOWN 3]", 
				"[UNKNOWN 4]", "[UNKNOWN 5]", "[UNKNOWN 6]", "[UNKNOWN 7]", 
				"[UNKNOWN 8]", "[UNKNOWN 9]", "[UNKNOWN 10]", "[UNKNOWN 11]", 
				"[UNKNOWN 12]", "[UNKNOWN 13]", "[UNKNOWN 14]", "[UNKNOWN 15]", 
			0}},
		{0x00800000, 23, "Capture", "SINGLE", 0,
			// bitgen: n/a -- this comes from the CAPTURE site ONESHOT setting
			// config: 0:"Readback is not single-shot", 1:"Readback is single-shot"
			VALUES{"Continuous", "OneShot", 0}},
		{0x01000000, 24, "DriveDone", "DRIVE_DONE", 0,
			// bitgen: No, Yes
			// config: 0:"DONE pin is open drain", 1:"DONE is actively driven high"
			VALUES{"No", "Yes", 0}}, 
		{0x02000000, 25, "DonePipe", "DONE_PIPE", 0,
			// bitgen: No, Yes
			// config: 0:"No pipeline stage for DONEIN", 1:"Add pipeline stage for DONEIN"
			VALUES{"No", "Yes", 0}},
		{0x20000000, 29, "CRC", "CRC_BYPASS", 0,
			// bitgen: Enable, Disable
			// config: 0:"CRC enabled", 1:"CRC disabled"
			VALUES{"Enable", "Disable", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Status Register Description: XAPP452, v1.1, June 25, 2008, Table 4.
	/// \note The "bitgen" names attempt to mimic the general bitgen convention.
	const Bitstream::Subfield Spartan3E::sSTAT[] = { 
		{0x00000001,  0, "CRC_error", "CRC_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"No CRC error", 1:"CRC error"
			VALUES{"No", "Yes", 0}},
		{0x00000004,  2, "DCM_locked", "DCM_LOCK", 0, 
			// bitgen: n/a
			// config: 0:"DCMs not locked", 1:"DCMs are locked"
			VALUES{"No", "Yes", 0}},
		{0x00000008,  3, "DCI_matched", "DCI_MATCH", 0, 
			// bitgen: n/a
			// config: 0:"DCI not matched", 1:"DCI matched
			VALUES{"No", "Yes", 0}},
		{0x00000010,  4, "IN_error", "IN_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"No legacy input error", 1:"Legacy input error"
			VALUES{"No", "Yes", 0}},
		{0x00000020,  5, "GTS_CFG", "GTS_CFG_B", 0, 
			// bitgen: n/a
			// config: 0:"All I/Os are placed in high-Z state", 1:"All I/Os behave as configured"
			VALUES{"IoDisabled", "IoEnabled", 0}},
		{0x00000040,  6, "GWE", "GWE", 0, 
			// bitgen: n/a
			// config: 0:"FFs and block RAM are write disabled", 1:"FFs and block RAM are write 
			//	enabled"
			VALUES{"WriteDisabled", "WriteEnabled", 0}},
		{0x00000080,  7, "GHIGH_B", "GHIGH_B", 0, 
			// bitgen: n/a
			// config: 0:"GHIGH_B asserted", 1:"GHIGH_B deasserted"
			VALUES{"InterconnectDisabled", "InterconnectEnabled", 0}},
		{0x00000700,  8, "Mode", "MODE", 0, 
			// bitgen: n/a
			// config: Status of the MODE pins (M2:M0)
			VALUES{"MasterSerial", "SlaveSelectMap32", "[UNDEFINED 2]", "MasterSelectMap", 
				"[UNDEFINED 3]", "JTAG", "SlaveSelectMap8", "[UNDEFINED 6]", "SlaveSerial", 0}},
		{0x00000800, 11, "INIT", "INIT", 0, 
			// bitgen: n/a
			// config: Value on INIT pin
			VALUES{"Deasserted", "Asserted", 0}},
		{0x00001000, 12, "Done", "DONE", 0, 
			// bitgen: n/a
			// config: Value on DONE pin
			VALUES{"Deasserted", "Asserted", 0}},
		{0x00002000, 13, "ID_error", "ID_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"No ID Error", 1:"ID Error"
			VALUES{"No", "Yes", 0}},
		{0, 0, 0, 0, 0, 0}
	};

		/// \see Control Register Description: XAPP452, v1.1, June 25, 2008, Table 3.
	const Bitstream::Subfield Spartan3E::sCTL[] = { 
		{0x00000001,  0, "GTS_USER_B", "GTS_USER_B", 0, 
			// bitgen: n/a?
			// config: 0:"I/Os placed in high-Z state", 1:"I/Os active"
			VALUES{"IoDisabled", "IoActive", 0}},
		{0x00000008,  3, "Persist", "PERSIST", 0, 
			// bitgen: No, Yes
			// config: 0:"No (default)", 1:"Yes"
			VALUES{"No", "Yes", 0}},
		{0x00000030,  4, "Security", "SBITS", 0, 
			// bitgen: None, Level1, Level2
			// config: 00:"Read/Write OK (default)", 01:"Readback disabled", 1x:"Readback disabled, 
			//	writing disabled except CRC register."
			VALUES{"None", "Level1", "Level2", "Level2", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Control Mask Register Description: Inferred from Table 3.
	const Bitstream::Subfield Spartan3E::sMASK[] = { 
		{0x00000001,  0, "GTS_USER_B", "GTS_USER_B", 0, VALUES{"Protected", "Writable", 0}},
		{0x00000008,  3, "Persist", "PERSIST", 0, VALUES{"Protected", "Writable", 0}},
		{0x00000080,  4, "Security", "SBITS", 0, 
			VALUES{"Protected", "[UNKNOWN 1]", "[UNKNOWN 2]", "Writable", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \brief Return the masked value for a subfield of the specified register.
	uint32_t Spartan3E::makeSubfield(ERegister inRegister, const std::string& inSubfield, 
		const std::string& inSetting) {
		const Subfield* subfields;
		switch(inRegister) {
		case eRegisterCOR: subfields = sCOR; break;
		case eRegisterSTAT: subfields = sSTAT; break;
		case eRegisterCTL: subfields = sCTL; break;
		case eRegisterMASK: subfields = sMASK; break;
		default: return 0;
		}
		for(uint32_t field = 0; subfields[field].mMask != 0; field++) {
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
