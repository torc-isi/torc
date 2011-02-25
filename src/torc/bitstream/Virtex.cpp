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
/// \brief Source for the Virtex class.

#include "torc/bitstream/Virtex.hpp"
#include <iostream>

namespace torc {
namespace bitstream {

	const char* Virtex::sPacketTypeName[ePacketTypeCount] = {
		"[UNKNOWN TYPE 0]", "TYPE1", "TYPE2", "[UNKNOWN TYPE 3]", "[UNKNOWN TYPE 4]", 
		"[UNKNOWN TYPE 5]", "[UNKNOWN TYPE 6]", "[UNKNOWN TYPE 7]"
	};

		/// \see configuration data: XAPP151, v1.7, October 20, 2004, Figure 12.
	const char* Virtex::sOpcodeName[eOpcodeCount] = {
		"[UNKNOWN OP 0]", "READ", "WRITE", "[UNKNOWN OP 3]"
	};

	const char* Virtex::sRegisterName[eRegisterCount] = {
		"CRC", "FAR", "FDRI", "FDRO", "CMD", "CTL", "MASK", "STAT", "LOUT", "COR", "[UNKNOWN REG 0]", "FLR"
	};

	const char* Virtex::sCommandName[eCommandCount] = {
		"[UNKNOWN CMD 0]", "WCFG", "", "LFRM", "RCFG", "START", "RCAP", "RCRC", "AGHIGH", "SWITCH"
	};

#define VALUES (const char*[])

	/// \see Configuration Options Register Description: XAPP151, v1.7, October 20, 2004, Table 20.
	const Bitstream::Subfield Virtex::sCOR[] = { 
		{0x00000007,  0, "GSR_cycle", "GSR_CYCLE", 5,
			// bitgen: 6, 1, 2, 3, 4, 5, Done, Keep
			// config: 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x00000038,  3, "GWE_cycle", "GWE_CYCLE", 5,
			// bitgen: 6, 1, 2, 3, 4, 5, Done, Keep
			// config: 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x000001c0,  6, "GTS_cycle", "GTS_CYCLE", 4,
			// bitgen: 5, 1, 2, 3, 4, 6, Done, Keep
			// config: 000:"1", 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x00000e00,  9, "LCK_cycle", "LCK_CYCLE", 7,
			// bitgen: NoWait, 0, 1, 2, 3, 4, 5, 6
			// config: 000:"1", 001:"2", 010:"3", 011:"4", 100:"5", 101:"6", 111:"NO_WAIT"
			VALUES{"1", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "NoWait", 0}},
		{0x00007000, 12, "DONE_cycle", "DONE_CYCLE", 3,
			// bitgen: 4, 1, 2, 3, 5, 6
			// config: 001:"2", 010:"3", 011:"4", 100:"5", 101:"6"
			VALUES{"[UNDEFINED 0]", "2", "3", "4", "5", "6", "[UNDEFINED 6]", "[UNDEFINED 7]", 0}},
		{0x00008000, 15, "", "SHUTDOWN", 0,
			// config: 0:"Startup", 1:"Shutdown sequence"
			VALUES{"0", "1", 0}},
		{0x000f0000, 16, "", "LOCK_WAIT", 0,
			// config: 000:"bottom-right", 001:"bottom-left", 010:"top-right", 011:"top-left"
			VALUES{"1", "2", "3", "4", 0}},
		{0x00300000, 20, "StartupClk", "SSCLKSRC", 0,
			// bitgen: Cclk, UserClk, JtagClk
			// config: 00:"CCLK", 01:"UserClk", 1x:"JTAGClk"
			VALUES{"Cclk", "UserClk", "JtagClk", "JtagClk", 0}},
		{0x0fc00000, 22, "ConfigRate", "OSCFSEL", 2,
			// bitgen: 4, 5, 7, 8, 9, 10, 13, 15, 20, 26, 30, 34, 41, 45, 51, 55, 60
			// config: values undefined
			VALUES{
				"[UNKNOWN 0]", "[UNKNOWN 1]", "[UNKNOWN 2]", "[UNKNOWN 3]", 
				"[UNKNOWN 4]", "[UNKNOWN 5]", "[UNKNOWN 6]", "[UNKNOWN 7]", 
				"[UNKNOWN 8]", "[UNKNOWN 9]", "[UNKNOWN 10]", "[UNKNOWN 11]", 
				"[UNKNOWN 12]", "[UNKNOWN 13]", "[UNKNOWN 14]", "[UNKNOWN 15]", 
				"[UNKNOWN 16]", "[UNKNOWN 17]", "[UNKNOWN 18]", "[UNKNOWN 19]", 
				"[UNKNOWN 20]", "[UNKNOWN 21]", "[UNKNOWN 22]", "[UNKNOWN 23]", 
				"[UNKNOWN 24]", "[UNKNOWN 25]", "[UNKNOWN 26]", "[UNKNOWN 27]", 
				"[UNKNOWN 28]", "[UNKNOWN 29]", "[UNKNOWN 30]", "[UNKNOWN 31]", 
			0}},
		{0x10000000, 28, "Capture", "SINGLE", 0,
			// bitgen: n/a -- this comes from the CAPTURE site ONESHOT setting
			// config: 0:"Readback is not single-shot", 1:"Readback is single-shot"
			VALUES{"Continuous", "OneShot", 0}},
		{0x20000000, 29, "DriveDone", "DRIVE_DONE", 0,
			// bitgen: No, Yes
			// config: 0:"DONE pin is open drain", 1:"DONE is actively driven high"
			VALUES{"No", "Yes", 0}}, 
		{0x40000000, 30, "DonePipe", "DONE_PIPE", 0,
			// bitgen: No, Yes
			// config: 0:"No pipeline stage for DONEIN", 1:"Add pipeline stage for DONEIN"
			VALUES{"No", "Yes", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Status Register Description: XAPP151, v1.7, October 20, 2004, Table 25.
	/// \note The "bitgen" names attempt to mimic the general bitgen convention.
	const Bitstream::Subfield Virtex::sSTAT[] = { 
		{0x00000001,  0, "CRC_error", "CRC_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"No CRC error", 1:"CRC error"
			VALUES{"No", "Yes", 0}},
		{0x00000002,  1, "TR_locked", "TR_LOCK", 0, 
			// TR - top right
			// bitgen: n/a
			// config: 0:"TR not locked", 1:"TR locked"
			VALUES{"0", "1", 0}},
		{0x00000004,  2, "TL_locked", "TL_LOCK", 0, 
			// TL - top left
			// bitgen: n/a
			// config: 0:"TL not locked", 1:"TL locked"
			VALUES{"0", "1", 0}},
		{0x00000008,  3, "BR_locked", "BR_LOCK", 0, 
			// BR - bottom right
			// bitgen: n/a
			// config: 0:"BR locked", 1:"BR locked
			VALUES{"0", "1", 0}},
		{0x00000010,  4, "BL_locked", "BL_LOCK", 0,
			// BL - bottom left
			// bitgen: n/a
			// config: 0:"BL locked", 1:"BL locked"
			VALUES{"No", "Yes", 0}},
		{0x00000020,  5, "IN_error", "IN_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"Not In Error", 1:"In Error"
			VALUES{"0", "1", 0}},
		{0x00000040,  6, "GTS_cfg", "GTS_CFG", 0, 
			// bitgen: n/a
			// config: 0:"I/O's tri-stated", 1:"I/O's not tri-stated"
			VALUES{"0", "1", 0}},
		{0x00000080,  7, "GWE_B", "GWE_B", 0, 
			// bitgen: n/a
			// config: 0:"FF & BRAM are write enabled", 1:"FF & BRAM are write disabled"
			VALUES{"0", "1", 0}},
		{0x00000100,  8, "GSR_B", "GSR_B", 0, 
			// bitgen: n/a
			// config: 0:"all FFs are Reset/Set", 1:"all FFs are not Reset/set"
			VALUES{"0", "1", 0}},
		{0x00000200, 9, "GHIGH_B", "GHIGH_B", 0, 
			// bitgen: n/a
			// config: 0:"GHIGH_B asserted", 1:"GHIGH_B not asserted"
			VALUES{"0", "1", 0}},
		{0x000001c0, 10, "MODE", "MODE", 0, 
			// bitgen: n/a
			// config: Status of the MODE pins (M2:M0)
			VALUES{"MasterSerial", "SlaveSelectMap32", "[UNDEFINED 2]", "MasterSelectMap", 
				"[UNDEFINED 3]", "JTAG", "SlaveSelectMap8", "[UNDEFINED 6]", "SlaveSerial", 0}},
		{0x00002000, 13, "INIT", "INIT", 0, 
			// bitgen: n/a
			// config: Value on INIT pin
			VALUES{"Deasserted", "Asserted", 0}},
		{0x00004000, 14, "DONE", "DONE", 0, 
			// bitgen: n/a
			// config: Value on DONE pin
			VALUES{"NotDone", "Done", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Status Register Description: XAPP151, v1.7, October 20, 2004, Table 23.
	const Bitstream::Subfield Virtex::sCTL[] = { 
		{0x00000001,  0, "GTS_USER_B", "GTS_USER_B", 0, 
			// bitgen: n/a?
			// config: 0:"I/Os placed in high-Z state", 1:"I/Os active"
			VALUES{"IoDisabled", "IoActive", 0}},
		{0x00000040,  6, "Persist", "PERSIST", 0, 
			// bitgen: No, Yes
			// config: 0:"No (default)", 1:"Yes"
			VALUES{"No", "Yes", 0}},
		{0x00000180,  7, "Security", "SBITS", 0, 
			// bitgen: None, Level1, Level2
			// config: 00:"Read/Write OK (default)", 01:"Readback disabled", 1x:"Readback disabled, 
			//	writing disabled except CRC register."
			VALUES{"None", "Level1", "Level2", "Level2", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Control Mask Register Description: Inferred from Table 23.
	const Bitstream::Subfield Virtex::sMASK[] = { 
		{0x00000001,  0, "GTS_USER_B", "GTS_USER_B", 0, VALUES{"Protected", "Writable", 0}},
		{0x00000040,  6, "Persist", "PERSIST", 0, VALUES{"Protected", "Writable", 0}},
		{0x00000180,  7, "Security", "SBITS", 0, 
			VALUES{"Protected", "[UNKNOWN 1]", "[UNKNOWN 2]", "Writable", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \brief Return the masked value for a subfield of the specified register.
	uint32_t Virtex::makeSubfield(ERegister inRegister, const std::string& inSubfield, 
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
