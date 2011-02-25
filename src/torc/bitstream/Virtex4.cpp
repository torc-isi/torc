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
/// \brief Source for the Virtex4 class.

#include "torc/bitstream/Virtex4.hpp"
#include <iostream>

namespace torc {
namespace bitstream {

	const char* Virtex4::sPacketTypeName[ePacketTypeCount] = {
		"[UNKNOWN TYPE 0]", "TYPE1", "TYPE2", "[UNKNOWN TYPE 3]", "[UNKNOWN TYPE 4]", 
		"[UNKNOWN TYPE 5]", "[UNKNOWN TYPE 6]", "[UNKNOWN TYPE 7]"
	};

	const char* Virtex4::sOpcodeName[eOpcodeCount] = {
		"NOP", "READ", "WRITE", "RESERVED"
	};

	const char* Virtex4::sRegisterName[eRegisterCount] = {
		"CRC", "FAR", "FDRI", "FDRO", "CMD", "CTL", "MASK", "STAT", "LOUT", "COR", "MFWR", "CBC", 
		"IDCODE", "AXSS"
	};

	const char* Virtex4::sCommandName[eCommandCount] = {
		"NULL", "WCFG", "MFWR", "LFRM", "RCFG", "START", "RCAP", "RCRC", "AGHIGH", "SWITCH", 
		"GRESTORE", "SHUTDOWN", "GCAPTURE", "DESYNC"
	};

#define VALUES (const char*[])

	/// \see Configuration Options Register Description: UG071, v1.10, April 8, 2008, Table 7-10.
	const Bitstream::Subfield Virtex4::sCOR[] = { 
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
		{0x007e0000, 17, "ConfigRate", "OSCFSEL", 0,
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
		{0x10000000, 28, "CRC", "CRC_BYPASS", 0,
			// bitgen: Enable, Disable
			// config: 0:"CRC enabled", 1:"CRC disabled"
			VALUES{"Enable", "Disable", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Status Register Description: UG071, v1.10, April 8, 2008, Table 7-9.
	/// \note The "bitgen" names attempt to mimic the general bitgen convention.
	const Bitstream::Subfield Virtex4::sSTAT[] = { 
		{0x00000001,  0, "CRC_error", "CRC_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"No CRC error", 1:"CRC error"
			VALUES{"No", "Yes", 0}},
		{0x00000002,  1, "DecryptorSecuritySet", "PART_SECURED", 0, 
			// bitgen: n/a
			// config: 0:"Decryptor security not set", 1:"Decryptor security set"
			VALUES{"No", "Yes", 0}},
		{0x00000004,  2, "DCM_locked", "DCM_LOCK", 0, 
			// bitgen: n/a
			// config: 0:"DCMs not locked", 1:"DCMs are locked"
			VALUES{"No", "Yes", 0}},
		{0x00000008,  3, "DCI_matched", "DCI_MATCH", 0, 
			// bitgen: n/a
			// config: 0:"DCI not matched", 1:"DCI matched
			VALUES{"No", "Yes", 0}},
		{0x00000010,  4, "StartupFinished", "EOS", 0, 
			// bitgen: n/a
			// config: 0:"Startup sequence has not finished", 1:"Startup sequence has finished"
			VALUES{"No", "Yes", 0}},
		{0x00000020,  5, "GTS_CFG_B", "GTS_CFG_B", 0, 
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
		{0x00000800, 11, "INIT_complete", "INIT_COMPLETE", 0, 
			// bitgen: n/a
			// config: 0:"Initializations has not finished", 1:"Initialization finished"
			VALUES{"No", "Yes", 0}},
		{0x00001000, 12, "INIT", "INIT", 0, 
			// bitgen: n/a
			// config: Value on INIT pin
			VALUES{"Deasserted", "Asserted", 0}},
		{0x00002000, 13, "DONE_released", "RELEASE_DONE", 0, 
			// bitgen: n/a
			// config: 0:"DONE signal not released", 1:"DONE signal released"
			VALUES{"DrivenLow", "Released", 0}},
		{0x00004000, 14, "DONE", "DONE", 0, 
			// bitgen: n/a
			// config: Value on DONE pin
			VALUES{"NotDone", "Done", 0}},
		{0x00008000, 15, "ID_error", "ID_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"No IE_ERROR", 1:"ID_ERROR"
			VALUES{"NoError", "Error", 0}}, 
		{0x00010000, 16, "Decrypt_error", "DEC_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"No DEC_ERROR", 1:"DEC_ERROR"
			VALUES{"NoError", "Error", 0}},/*,
		{0x00020000, 17, "Reserved", "Reserved", 0, 
			// bitgen: n/a
			// config: undefined
			VALUES{0}}*/
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Control Register Description: UG071, v1.10, April 8, 2008, Table 7-7.
	const Bitstream::Subfield Virtex4::sCTL[] = { 
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
		{0x00000100,  8, "GLUTMASK", "GLUTMASK", 0, 
			// bitgen: n/a
			// config: 0:"Readback all 0s from SRL16 and Distributed RAM. Use with active device 
			//	readback.", 1:"Readback dynamic values from SRL16 and Distributed RAM. Use with 
			//	shutdown readback."
			VALUES{"Masked", "Dynamic", 0}},
		{0x40000000, 30, "ICAP_sel", "ICAP_SEL", 0, 
			// bitgen: n/a
			// config: 0:"Top ICAP Port Enabled (default)", 1:"Bottom ICAP Port Enabled"
			VALUES{"Top", "Bottom", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Control Mask Register Description: Inferred from Table 7-7.
	const Bitstream::Subfield Virtex4::sMASK[] = { 
		{0x00000001,  0, "GTS_USER_B", "GTS_USER_B", 0, VALUES{"Protected", "Writable", 0}},
		{0x00000008,  3, "Persist", "PERSIST", 0, VALUES{"Protected", "Writable", 0}},
		{0x00000080,  4, "Security", "SBITS", 0, 
			VALUES{"Protected", "[UNKNOWN 1]", "[UNKNOWN 2]", "Writable", 0}},
		{0x00000100,  8, "GLUTMASK", "GLUTMASK", 0, VALUES{"Protected", "Writable", 0}},
		{0x40000000, 30, "ICAP_sel", "ICAP_SEL", 0, VALUES{"Protected", "Writable", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \brief Return the masked value for a subfield of the specified register.
	uint32_t Virtex4::makeSubfield(ERegister inRegister, const std::string& inSubfield, 
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


//	bool Virtex4::readPackets(std::istream& inStream) {
//		uint32_t bitstreamWordLength = mBitstreamByteLength >> 2;
//		uint32_t cumulativeWordLength = 0;
//		while(cumulativeWordLength < bitstreamWordLength) {
//			push_back(VirtexPacket::read(inStream));
//			cumulativeWordLength += back().getWordSize();
//		}
//		return true;
//	}

} // namespace bitstream
} // namespace torc
