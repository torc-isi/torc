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

#include "torc/bitstream/Virtex6.hpp"
#include <iostream>

namespace torc {
namespace bitstream {

	const char* Virtex6::sPacketTypeName[ePacketTypeCount] = {
		"[UNKNOWN TYPE 0]", "TYPE1", "TYPE2", "[UNKNOWN TYPE 3]", "[UNKNOWN TYPE 4]", 
		"[UNKNOWN TYPE 5]", "[UNKNOWN TYPE 6]", "[UNKNOWN TYPE 7]"
	};

	const char* Virtex6::sOpcodeName[eOpcodeCount] = {
		"NOP", "READ", "WRITE", "RESERVED"
	};

	const char* Virtex6::sRegisterName[eRegisterCount] = {
		"CRC", "FAR", "FDRI", "FDRO", "CMD", "CTL0", "MASK", "STAT", "LOUT", "COR0", "MFWR", "CBC", 
		"IDCODE", "AXSS", "COR1", "CSOB", "WBSTAR", "TIMER", "[UNKNOWN REG 18]", 
		"[UNKNOWN REG 19]", "[UNKNOWN REG 20]", "[UNKNOWN REG 21]", "BOOTSTS", "[UNKNOWN REG 23]", 
		"CTL1", "[UNKNOWN REG 25]", "DWC"
	};

	const char* Virtex6::sCommandName[eCommandCount] = {
		"NULL", "WCFG", "MFW", "DGHIGH/LFRM", "RCFG", "START", "RCAP", "RCRC", "AGHIGH", "SWITCH", 
		"GRESTORE", "SHUTDOWN", "GCAPTURE", "DESYNCH", "Reserved", "IPROG", "CRCC", 
		"LTIMER"
	};

#define VALUES (const char*[])

	/// \see Control Register 0 (CTL0): UG360, v3.2, November 1, 2010, Table 6-28.
	const Bitstream::Subfield Virtex6::sCTL0[] = {
		{0x00000001,  0, "GTS_USER_B", "GTS_USER_B", 0, 
			// bitgen: n/a?
			// config: 0:"I/Os 3-stated", 1:"I/Os active"
			VALUES{"IoDisabled", "IoActive", 0}},
		{0x00000008,  3, "Persist", "PERSIST", 0, 
			// bitgen: No, Yes
			// config: 0:"No (default)", 1:"Yes"
			VALUES{"No", "Yes", 0}},
		{0x00000030,  4, "Security", "SBITS", 0, 
			// bitgen: None, Level1, Level2
			// config: 00:"Read/Write OK (default)", 01:"Readback disabled", 1x:"Both writes and 
			//	read disabled."
			VALUES{"None", "Level1", "Level2", "Level2", 0}},
		{0x00000040,  6, "Encrypt", "DEC", 0, 
			// bitgen: No, Yes
			// config: AES Decryptor enable bit
			VALUES{"No", "Yes", 0}},
		{0x00000080,  7, "FARSRC", "FARSRC", 0, 
			// bitgen: n/a
			// config: 0: FAR, address of RBCRC, 1: EFAR, address of ECC error frame
			VALUES{"FAR", "EFAR", 0}},
		{0x00000100,  8, "GLUTMASK", "GLUTMASK", 0, 
			// bitgen: n/a
			// config: 0:"Readback all 0s from SRL16 and Distributed RAM. Use with active device 
			//	readback.", 1:"Readback dynamic values from SRL16 and Distributed RAM. Use with 
			//	shutdown readback."
			VALUES{"Masked", "Dynamic", 0}},
		{0x00001000, 12, "OverTempPowerDown", "OverTempPowerDown", 0, 
			// bitgen: Disable, Enable
			// config: Enables the System Monitor Over-Temperature power down.
			VALUES{"Disable", "Enable", 0}},
		{0x40000000, 30, "ICAP_sel", "ICAP_SEL", 0, 
			// bitgen: n/a
			// config: 0:"Top ICAP Port Enabled (default)", 1:"Bottom ICAP Port Enabled"
			VALUES{"Top", "Bottom", 0}},
		{0x80000000, 31, "EFUSE_key", "EFUSE_KEY", 0, 
			// bitgen: n/a
			// config: 0:"Battery-back RAM", 1:"eFUSE"
			VALUES{"No", "Yes", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Control Register 0 (CTL0): UG360, v3.2, November 1, 2010, Table 6-28.
	const Bitstream::Subfield Virtex6::sMASK0[] = {
		{0x00000001,  0, "GTS_USER_B", "GTS_USER_B", 0, VALUES{"Protected", "Writable", 0}},
		{0x00000008,  3, "Persist", "PERSIST", 0,  VALUES{"Protected", "Writable", 0}},
		{0x00000030,  4, "Security", "SBITS", 0, 
			VALUES{"Protected", "[UNKNOWN 1]", "[UNKNOWN 2]", "Writable", 0}},
		{0x00000040,  6, "Encrypt", "DEC", 0,  VALUES{"Protected", "Writable", 0}},
		{0x00000080,  7, "FARSRC", "FARSRC", 0,  VALUES{"Protected", "Writable", 0}},
		{0x00000100,  8, "GLUTMASK", "GLUTMASK", 0,  VALUES{"Protected", "Writable", 0}},
		{0x00001000, 12, "OverTempPowerDown", "OverTempPowerDown", 0, VALUES{"Protected", 
			"Writable", 0}},
		{0x40000000, 30, "ICAP_sel", "ICAP_SEL", 0, VALUES{"Protected", "Writable", 0}},
		{0x80000000, 31, "EFUSE_key", "EFUSE_KEY", 0, VALUES{"Protected", "Writable", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Control Register 0 (CTL0): UG360, v3.2, November 1, 2010, Table 6-30.
	const Bitstream::Subfield Virtex6::sCTL1[] = {
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Control Register 0 (CTL0): UG360, v3.2, November 1, 2010, Table 6-35.
	const Bitstream::Subfield Virtex6::sCOR0[] = {
		{0x00000007,  0, "GWE_cycle", "GWE_CYCLE", 5,
			// bitgen: 6, 1, 2, 3, 4, 5, Done, Keep
			// config: 000:"1", 001:"2", 010:"3", 011:"4", 100:"5", 101:"6", 110:"GTS tracks DONE 
			//	pin.  BitGen option -g GTS_cycle:Done", 111:"Keep"
			VALUES{"1", "2", "3", "4", "5", "6", "Done", "Keep", 0}},
		{0x00000038,  3, "GTS_cycle", "GTS_CYCLE", 4,
			// bitgen: 5, 1, 2, 3, 4, 6, Done, Keep
			// config: 000:"1", 001:"2", 010:"3", 011:"4", 100:"5", 101:"6", 110:"GTS tracks DONE 
			//	pin.  BitGen option -g GTS_cycle:Done", 001[sic]:"Keep" but assuming 111:"Keep"
			VALUES{"1", "2", "3", "4", "5", "6", "Done", "Keep", 0}},
		{0x00000E00,  9, "Match_cycle", "MATCH_CYCLE", 0,
			// bitgen: Auto, NoWait, 0, 1, 2, 3, 4, 5, 6
			// config: 000:"0", 001:"1", 010:"2", 011:"3", 100:"4", 101:"5", 110:"6", 111:"KEEP"
			VALUES{"0", "1", "2", "3", "4", "5", "6", "NoWait", 0}},
		{0x00007000, 12, "DONE_cycle", "DONE_CYCLE", 3,
			// bitgen: 4, 1, 2, 3, 5, 6
			// config: 000:"1", 001:"2", 010:"3", 011:"4", 100:"5", 101:"6", 110:"7", 111:"KEEP"
			VALUES{"1", "2", "3", "4", "5", "6", "7", "KEEP", 0}},
		{0x00018000, 15, "StartupClk", "SSCLKSRC", 0,
			// bitgen: Cclk, UserClk, JtagClk
			// config: 00:"CCLK", 01:"UserClk", 1x:"JTAGClk"
			VALUES{"Cclk", "UserClk", "JtagClk", "JtagClk", 0}},
		{0x007e0000, 17, "ConfigRate", "OSCFSEL", 0,
			// bitgen: 2, 6, 9, 13, 17, 20, 24, 27, 31, 35, 38, 42, 46, 49, 53, 56, 60
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
		{0x08000000, 27, "DONE_status", "PWRDWN_STAT", 0,
			// bitgen: n/a?
			// config: 0:"DONE pin", 1:"Powerdown pin"
			VALUES{"DonePin", "PowerdownPin", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Configuration Options Register 1 (COR1): UG360, v3.2, November 1, 2010, Table 6-37.
	const Bitstream::Subfield Virtex6::sCOR1[] = {
		{0x00000003,  0, "BPI_page_size", "BPI_PAGE_SIZE", 0,
			// bitgen: 1, 4, 8
			// config: 00:"1 byte/word", 01:"4 bytes/words", 10:"8 bytes/words", 11:"Reserved"
			VALUES{"1", "4", "8", "Reserved", 0}},
		{0x0000000C,  2, "BPI_1st_read_cycle", "BPI_1ST_READ_CYCLES", 0,
			// bitgen: 1, 2, 3, 4
			// config: 00:"1", 01:"2", 10:"3", 11:"4"
			VALUES{"1", "4", "8", "Reserved", 0}},
		{0x00000100,  8, "ContinuousReadbackCRC", "RBCRC_EN", 0, 
			// bitgen: n/a?
			// config: Continuous readback CRC enable
			VALUES{"Disabled", "Enabled", 0}},
		{0x00000200,  9, "InitAsCRCErrorPin", "RBCRC_NO_PIN", 0, 
			// bitgen: n/a?
			// config: Disables INIT_B as read back CRC error status output pin
			VALUES{"Disabled", "Enabled", 0}},
		{0x00018000, 15, "ActionReadbackCRC", "RBCRC_ACTION", 0,
			// bitgen: n/a?
			// config: Action for readback CRC 00:"1", 01:"2", 10:"3", 11:"4"
			VALUES{"Continue", "Halt", "CorrectAndHalt", "CorrectAndContinue", 0}},
		{0x00020000, 17, "PersistDeassertAtDesynch", "PERSIST_DEASSERT_AT_DESYNCH", 0,
			// bitgen: n/a?
			// config: Enables deassertion of PERSIST with the DESYNCH command
			VALUES{"Disabled", "Enabled", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see WBSTAR Register Description: UG360, v3.2, November 1, 2010, Table 6-39.
	const Bitstream::Subfield Virtex6::sWBSTAR[] = {
		{0x18000000, 27, "NextRevisionSelect", "RS[1:0]", 0,
			// config: RS[1:0] pin value on next warm boot
			VALUES{"00", "01", "10", "11", 0}},
		{0x04000000, 26, "RevisionSelectTristate", "RS_TS_B", 0, 
			// config: 0:"Disabled", 1:"Enabled"
			VALUES{"Disabled", "Enabled", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see TIMER Register Description: UG360, v3.2, November 1, 2010, Table 6-41.
	const Bitstream::Subfield Virtex6::sTIMER[] = {
		{0x01000000, 24, "TimerForConfig", "TIMER_CFG_MON", 0, 
			// config: 0:"Disabled", 1:"Enabled"
			VALUES{"Disabled", "Enabled", 0}},
		{0x02000000, 25, "TimerForUser", "TIMER_USR_MON", 0, 
			// config: 0:"Disabled", 1:"Enabled"
			VALUES{"Disabled", "Enabled", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see BOOTSTS Register Description: UG360, v3.2, November 1, 2010, Table 6-43.
	const Bitstream::Subfield Virtex6::sBOOTSTS[] = {
		{0x00000001,  0, "RegisterStatus0", "VALID_0", 0, 
			// config: Status valid
			VALUES{"Valid", "Invalid", 0}},
		{0x00000002,  1, "FallbackStatus0", "FALLBACK_0", 0, 
			// config: 0:"Normal configuration", 1:"Fallback to default reconfiguration, RS[1:0] 
			//	actively drives 2'b00"
			VALUES{"Normal", "Fallback", 0}},
		{0x00000004,  2, "InternalTrigger0", "IPROG_0", 0, 
			// config: Internal PROG triggered configuration
			VALUES{"External", "Internal", 0}},
		{0x00000008,  3, "WatchdogTimeout0", "WTO_ERROR_0", 0, 
			// config: Watchdog time-out error
			VALUES{"Valid", "Invalid", 0}},
		{0x00000010,  4, "ID_error0", "ID_ERROR_0", 0, 
			// config: ID error
			VALUES{"NoError", "Error", 0}},
		{0x00000020,  5, "CRC_error0", "CRC_ERROR_0", 0, 
			// config: CRC error
			VALUES{"NoError", "Error", 0}},
		{0x00000040,  6, "BPI_wraparound_error0", "WRAP_ERROR_0", 0, 
			// config: BPI address counter wraparound error
			VALUES{"NoError", "Error", 0}},
		{0x00000100,  8, "RegisterStatus1", "VALID_1", 0, 
			// config: Status valid
			VALUES{"Valid", "Invalid", 0}},
		{0x00000200,  9, "FallbackStatus1", "FALLBACK_1", 0, 
			// config: 0:"Normal configuration", 1:"Fallback to default reconfiguration, RS[1:0] 
			//	actively drives 2'b00"
			VALUES{"Normal", "Fallback", 0}},
		{0x00000400, 10, "InternalTrigger1", "IPROG_1", 0, 
			// config: Internal PROG triggered configuration
			VALUES{"External", "Internal", 0}},
		{0x00000800, 11, "WatchdogTimeout1", "WTO_ERROR_1", 0, 
			// config: Watchdog time-out error
			VALUES{"Valid", "Invalid", 0}},
		{0x00001000, 12, "ID_error1", "ID_ERROR_1", 0, 
			// config: ID error
			VALUES{"NoError", "Error", 0}},
		{0x00002000, 13, "CRC_error1", "CRC_ERROR_1", 0, 
			// config: CRC error
			VALUES{"NoError", "Error", 0}},
		{0x00004000, 14, "BPI_wraparound_error1", "WRAP_ERROR_1", 0, 
			// config: BPI address counter wraparound error
			VALUES{"NoError", "Error", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Status Register Description: UG360, v3.2, November 1, 2010, Table 6-33.
	const Bitstream::Subfield Virtex6::sSTAT[] = { 
		{0x00000001,  0, "CRC_error", "CRC_ERROR", 0, 
			// bitgen: n/a
			// config: 0:"No CRC error", 1:"CRC error"
			VALUES{"No", "Yes", 0}},
		{0x00000002,  1, "DecryptorSecuritySet", "PART_SECURED", 0, 
			// bitgen: n/a
			// config: 0:"Decryptor security not set", 1:"Decryptor security set"
			VALUES{"No", "Yes", 0}},
		{0x00000004,  2, "MMCM_locked", "MMCM_LOCK", 0, 
			// bitgen: n/a
			// config: 0:"MMCMs not locked", 1:"MMCMs are locked"
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
			VALUES{"MasterSerial", "MasterSPI", "MasterBPI-Up", "MasterBPI-Down", 
				"MasterSelectMap", "JTAG", "SlaveSelectMap", "SlaveSerial", 0}},
		{0x00000800, 11, "INIT_complete", "INIT_COMPLETE", 0, 
			// bitgen: n/a
			// config: 0:"Initializations has not finished", 1:"Initialization finished"
			VALUES{"No", "Yes", 0}},
		{0x00001000, 12, "INIT_B", "INIT_B", 0, 
			// bitgen: n/a
			// config: Value on INIT_B pin
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
			VALUES{"NoError", "Error", 0}}, 
		{0x00020000, 17, "Decrypt_error", "SYSMON_OVER_TEMP", 0, 
			// bitgen: n/a
			// config: 0:"No DEC_ERROR", 1:"DEC_ERROR"
			VALUES{"NoError", "Error", 0}},
		{0x001c0000, 18, "StartupState", "STARTUP_STATE", 0, 
			// bitgen: n/a
			// config: 000:"0", 001:"1", 010:"3", 011:"2", 100:"7", 101:"6", 110:"4", 111:"5"
			VALUES{"0", "1", "3", "2", "7", "6", "4", "5", 0}},
		{0x01c00000, 22, "FlashTypeSelect", "FS", 0, 
			// bitgen: n/a
			// config: SPI Flash type select
			VALUES{0}},
		{0x06000000, 25, "BusWidth", "BUS_WIDTH", 0, 
			// bitgen: n/a
			// config: 00:"x1", 01:"x8", 10:"x16", 11:"x32"
			VALUES{"1", "8", "16", "32", 0}},
		{0x06000000, 28, "HswapenPin", "HSWAPEN_B", 0, 
			// bitgen: Pullup, Pulldown, Pullnone
			// config: 0:"0", 1:"1"
			VALUES{"0", "1", "16", "32", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \brief Return the masked value for a subfield of the specified register.
	uint32_t Virtex6::makeSubfield(ERegister inRegister, const std::string& inSubfield, 
		const std::string& inSetting) {
		const Subfield* subfields;
		switch(inRegister) {
		case eRegisterCOR0: subfields = sCOR0; break;
		case eRegisterCOR1: subfields = sCOR1; break;
		case eRegisterSTAT: subfields = sSTAT; break;
		case eRegisterCTL0: subfields = sCTL0; break;
		case eRegisterCTL1: subfields = sCTL1; break;
		case eRegisterMASK: subfields = sMASK0; break;
		case eRegisterWBSTAR: subfields = sWBSTAR; break;
		case eRegisterTIMER: subfields = sTIMER; break;
		case eRegisterBOOTSTS: subfields = sBOOTSTS; break;
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
