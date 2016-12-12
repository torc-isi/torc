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

#include "torc/bitstream/Virtex7.hpp"
#include <iostream>

/// \todo Warning: this will need to be moved elsewhere.
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <fstream>
#include <iomanip>

namespace torc {
namespace bitstream{

	const char* Virtex7::sPacketTypeName[ePacketTypeCount] = {
		"[UNKNOWN TYPE 0]", "TYPE1", "TYPE2", "[UNKNOWN TYPE 3]", "[UNKNOWN TYPE 4]", 
		"[UNKNOWN TYPE 5]", "[UNKNOWN TYPE 6]", "[UNKNOWN TYPE 7]"
	};

	const char* Virtex7::sOpcodeName[eOpcodeCount] = {
		"NOP", "READ", "WRITE", "RESERVED"
	};

	const char* Virtex7::sRegisterName[eRegisterCount] = {
		"CRC", "FAR", "FDRI", "FDRO", "CMD", "CTL0", "MASK", "STAT", "LOUT", "COR0", 
		"MFWR", "CBC", "IDCODE", "AXSS", "COR1", "[UNKNOWN REG 15]","WBSTAR", "TIMER",
		"[UNKNOWN REG 18]","[UNKNOWN REG 19]", "[UNKNOWN REG 20]", "[UNKNOWN REG 21]", 
		"BOOTSTS", "[UNKNOWN REG 23]", "CTL1"
	};

	const char* Virtex7::sCommandName[eCommandCount] = {
		"NULL", "WCFG", "MFW", "DGHIGH/LFRM", "RCFG", "START", "RCAP", "RCRC", 
		"AGHIGH", "SWITCH", "GRESTORE", "SHUTDOWN", "GCAPTURE", "DESYNCH", "Reserved", 
		"IPROG", "CRCC", "LTIMER"
	};

#define VALUES (const char*[])

	///\see Control Register 0 (CTL0): UG470, v1.1, March 28, 2011, Table 5-23
	const Bitstream::Subfield Virtex7::sCTL0[] = {
		{0x00000001, 0, "GTS_USER_B", "GTS_USER_B", 0,
			// bitgen: n/a
			// config: 0:"I/Os 3 stated, 1:"I/Os active"
			VALUES{"IoDisabled", "IoActive", 0}},
		{0x00000008, 3, "Persist", "PERSIST", 0,
			// bitgen: No, Yes
			// config: 0:"No (default)", 1:"Yes"
			VALUES{"No", "Yes", 0}},
		{0x00000030,  4, "Security", "SBITS", 0, 
			// bitgen: None, Level1, Level2
			// config: 00:"Read/Write OK (default)", 01:"Readback disabled", 1x:"Both writes and 
			//		read disabled."
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
			//		readback.", 1:"Readback dynamic values from SRL16 and Distributed RAM. Use with 
			//		shutdown readback."
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

	/// \see Control Register 0 (CTL0): UG470, v1.1, March 28, 2011, Table 5-22.
	const Bitstream::Subfield Virtex7::sMASK0[] = {
		{0x00000001,  0, "GTS_USER_B", "GTS_USER_B", 0, VALUES{"Protected", "Writable", 0}},
		{0x00000008,  3, "Persist", "PERSIST", 0,  VALUES{"Protected", "Writable", 0}},
		{0x00000030,  4, "Security", "SBITS", 0, 
			VALUES{"Protected", "[UNKNOWN 1]", "[UNKNOWN 2]", "Writable", 0}},
		{0x00000040,  6, "Encrypt", "DEC", 0,  VALUES{"Protected", "Writable", 0}},
		{0x00000080,  7, "FARSRC", "FARSRC", 0,  VALUES{"Protected", "Writable", 0}},
		{0x00000100,  8, "GLUTMASK", "GLUTMASK", 0,  VALUES{"Protected", "Writable", 0}},
		{0x00001000, 12, "OverTempPowerDown", "OverTempPowerDown", 0, 
			VALUES{"Protected", "Writable", 0}},
		{0x40000000, 30, "ICAP_sel", "ICAP_SEL", 0, VALUES{"Protected", "Writable", 0}},
		{0x80000000, 31, "EFUSE_key", "EFUSE_KEY", 0, VALUES{"Protected", "Writable", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Control Register 1 (CTL1): UG470, v1.1, March 28, 2011, Table 5-36
	const Bitstream::Subfield Virtex7::sCTL1[] = {
		{0, 0, 0, 0, 0, 0}
	};


	/// \see Configurations Options Register 0 (COR0): UG470, v1.1, March 28, 2011, Table 5-27.
	const Bitstream::Subfield Virtex7::sCOR0[] = {
		{0x00000007,  0, "GWE_cycle", "GWE_CYCLE", 5,
			// bitgen: 6, 1, 2, 3, 4, 5, Done, Keep
			// config: 000:"1", 001:"2", 010:"3", 011:"4", 100:"5", 101:"6", 110:"GTS tracks DONE 
			//		pin.  BitGen option -g GTS_cycle:Done", 111:"Keep"
			VALUES{"1", "2", "3", "4", "5", "6", "Done", "Keep", 0}},
		{0x00000038,  3, "GTS_cycle", "GTS_CYCLE", 4,
			// bitgen: 5, 1, 2, 3, 4, 6, Done, Keep
			// config: 000:"1", 001:"2", 010:"3", 011:"4", 100:"5", 101:"6", 110:"GTS tracks DONE 
			//		pin.  BitGen option -g GTS_cycle:Done", 001:"Keep" 
			VALUES{"1", "2", "3", "4", "5", "6", "Done", "Keep", 0}},
		{0x000001C0, 6, "LCK_cycle", "LOCK_CYCLE", 0,
			// bitgen: NoWait, 0, 1, 2, 3, 4, 5, 6
			// config: 000:"0", 001:"1", 010:"2", 011:"3", 100:"4", 101:"5", 110:"6", 111:"No Wait"
			VALUES{"0", "1", "2", "3", "4", "5", "6", "No Wait", 0}},
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
			// bitgen: 2, 4, 6, 10, 12, 16, 22, 26, 33, 40, 50, 66
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
			// bitgen: Yes, No
			// config: 0:"No pipeline stage for DONEIN", 1:"Add pipeline stage for DONEIN"
			VALUES{"No", "Yes", 0}},
		{0x08000000, 27, "DONE_status", "PWRDWN_STAT", 0,
			// bitgen: n/a?
			// config: 0:"DONE pin", 1:"Powerdown pin"
			VALUES{"DonePin", "PowerdownPin", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	///\see Configurations Options Register 1 (COR1): UG470 v1.1, March 28, 2011, Table 5-29 .
	const Bitstream::Subfield Virtex7::sCOR1[] = {
		{0x00000003, 0, "BPI_page_size", "BPI_PAGE_SIZE", 0,
			// bitgen: 1, 4, 8
			// config: 00:"1 byte/word", 01:"4 bytes/words", 10:"8 bytes/words", 11:"Reserved"
			VALUES{"1", "4", "8", "Reserved", 0}},
		{0x0000000C, 2, "BPI_1st_read_cycle", "BPI_1ST_READ_CYCLES", 0,
			// bitgen: 1, 2, 3, 4
			// config: 00:"1", 01:"2", 10:"3", 11:"4"
			VALUES{"1", "2", "3", "4", 0}},
			//VALUES{"1", "4", "8", "Reserved", 0}}, // WHY ?!
		{0x00000100,  8, "ContinuousReadbackCRC", "RBCRC_EN", 0, 
			// bitgen: n/a?
			// config: Continuous readback CRC enable
			VALUES{"Enabled", "disabled", 0}},
			//VALUES{"Disabled", "Enabled", 0}}, // WHY?!
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

	/// \see WBSTAR Register Descrption: UG470, v1.1, March 28, 2011, Table 5-31.
	const Bitstream::Subfield Virtex7::sWBSTAR[] = {
		{0x40000000, 30, "NextRevisionSelect", "RS[1:0]", 0,
			// config: RS[1:0] pin value on next warm boot
			VALUES{"00", "01", "10", "11", 0}},
		{0x20000000, 29, "RevisionSelectTristate", "RS_TS_B", 0,
			// config: 0:"Disabled", 1:"Enabled"
			VALUES{"Disabled", "Enabled", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see TIMER Register Description: UG470, v1.1, March 28, 2011, Table 5-33.
	const Bitstream::Subfield Virtex7::sTIMER[] = {
		{0x40000000, 30, "TimerForConfig", "TIMER_CFG_MON", 0,
			// config: 0:"Disabled", 1:"Enabled"
			VALUES{"Disabled", "Enabled", 0}},
		{0x80000000, 31, "TimerForUser", "TIMER_USR_MON", 0,
			// config: 0:"Disabled", 1:"Enabled"
			VALUES{"Disabled", "Enabled", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see BOOTSTS Register Description: UG470, v1.1, March 28, 2011, Table 5-35.
	const Bitstream::Subfield Virtex7::sBOOTSTS[] = {
		{0x00000001,  0, "RegisterStatus0", "VALID_0", 0,
			// config: Status valid
			VALUES{"Valid", "Invalid", 0}},
		{0x00000002,  1, "FallbackStatus0", "FALLBACK_0", 0,
			// config: 0:"Normal configuration", 1:"Fallback to default reconfiguration, RS[1:0]
			//		actively drives 2'b00"
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
			//		actively drives 2'b00"
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

	/// \see Status Register Description: UG470, v1.1, March 28, 2011, Table 5-25.
	const Bitstream::Subfield Virtex7::sSTAT[] = {
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
			//		enabled"
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
		{0x001c0000, 18, "StartupState", "STARTUP_STATE", 0, 
			// bitgen: n/a
			// config: 000:"0", 001:"1", 010:"3", 011:"2", 100:"7", 101:"6", 110:"4", 111:"5"
			VALUES{"0", "1", "3", "2", "7", "6", "4", "5", 0}},
		{0x06000000, 25, "BusWidth", "BUS_WIDTH", 0, 
			// bitgen: n/a
			// config: 00:"x1", 01:"x8", 10:"x16", 11:"x32"
			VALUES{"1", "8", "16", "32", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \brief Return the masked value for a subfield of the specified register.
	uint32_t Virtex7::makeSubfield(ERegister inRegister, const std::string& inSubfield, 
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
		for(uint32_t field = 0; subfields[field].mMask != 0; field++){ 
			const Subfield& subfield = subfields[field]; 
			if(inSubfield != subfield.mBitgenName && inSubfield != subfield.mConfigGuideName)
				continue; 
			const char** ptr = subfield.mValues; 
			for(uint32_t i = 0; *ptr != 0; i++, ptr++){
				if(inSetting == *ptr) return (i << subfield.mShift) & subfield.mMask; 
			}
		} 
		return 0;
	}

//#define GENERATE_STATIC_DEVICE_INFO
#ifndef GENERATE_STATIC_DEVICE_INFO

	extern DeviceInfo xc7a100t;
	extern DeviceInfo xc7a200t;
	extern DeviceInfo xc7k70t;
	extern DeviceInfo xc7k160t;
	extern DeviceInfo xc7k325t;
	extern DeviceInfo xc7k355t;
	extern DeviceInfo xc7k410t;
	extern DeviceInfo xc7k420t;
	extern DeviceInfo xc7k480t;
	extern DeviceInfo xc7v585t;
	extern DeviceInfo xc7v2000t;
	extern DeviceInfo xc7vh580t;
	extern DeviceInfo xc7vh870t;
	extern DeviceInfo xc7vx330t;
	extern DeviceInfo xc7vx415t;
	extern DeviceInfo xc7vx485t;
	extern DeviceInfo xc7vx550t;
	extern DeviceInfo xc7vx690t;
	extern DeviceInfo xc7vx980t;
	extern DeviceInfo xc7vx1140t;
	extern DeviceInfo xc7z010;
	extern DeviceInfo xc7z020;
	extern DeviceInfo xc7z030;
	extern DeviceInfo xc7z045;

	void Virtex7::initializeDeviceInfo(const std::string& inDeviceName) {
		using namespace torc::common;
		switch(mDevice) {
			case eXC7A100T: setDeviceInfo(xc7a100t); break;
			case eXC7A200T: setDeviceInfo(xc7a200t); break;
			case eXC7K70T: setDeviceInfo(xc7k70t); break;
			case eXC7K160T: setDeviceInfo(xc7k160t); break;
			case eXC7K325T: setDeviceInfo(xc7k325t); break;
			case eXC7K355T: setDeviceInfo(xc7k355t); break;
			case eXC7K410T: setDeviceInfo(xc7k410t); break;
			case eXC7K420T: setDeviceInfo(xc7k420t); break;
			case eXC7K480T: setDeviceInfo(xc7k480t); break;
			case eXC7V585T: setDeviceInfo(xc7v585t); break;
			case eXC7V2000T: setDeviceInfo(xc7v2000t); break;
			case eXC7VH580T: setDeviceInfo(xc7vh580t); break;
			case eXC7VH870T: setDeviceInfo(xc7vh870t); break;
			case eXC7VX330T: setDeviceInfo(xc7vx330t); break;
			case eXC7VX415T: setDeviceInfo(xc7vx415t); break;
			case eXC7VX485T: setDeviceInfo(xc7vx485t); break;
			case eXC7VX550T: setDeviceInfo(xc7vx550t); break;
			case eXC7VX690T: setDeviceInfo(xc7vx690t); break;
			case eXC7VX980T: setDeviceInfo(xc7vx980t); break;
			case eXC7VX1140T: setDeviceInfo(xc7vx1140t); break;
			case eXC7Z010: setDeviceInfo(xc7z010); break;
			case eXC7Z020: setDeviceInfo(xc7z020); break;
			case eXC7Z030: setDeviceInfo(xc7z030); break;
			case eXC7Z045: setDeviceInfo(xc7z045); break;
			default: break;
		}

		// update the top and bottom bitstream row counts as appropriate for the device
		setRowCounts(inDeviceName);
	}

#else

	void Virtex7::initializeDeviceInfo(const std::string& inDeviceName) {

		typedef torc::architecture::xilinx::TileCount TileCount;
		typedef torc::architecture::xilinx::TileRow TileRow;
		typedef torc::architecture::xilinx::TileCol TileCol;
		typedef torc::architecture::xilinx::TileTypeIndex TileTypeIndex;
		typedef torc::architecture::xilinx::TileTypeCount TileTypeCount;

//		ColumnDefVector::iterator p = mColumnDefs.begin();
//		ColumnDefVector::iterator e = mColumnDefs.end();
//		std::cerr << "========== ColumnDefs ==========" << std::endl;
//		while(p < e) {
//			ColumnDef& columnDef = *p++;
//			std::cerr << columnDef.getName() << ": " 
//				<< columnDef[0] << ", " << columnDef[1] << ", " << columnDef[2] << ", "
//				<< columnDef[3] << ", " << columnDef[4] << ", " << columnDef[5] << ", "
//				<< columnDef[6] << ", " << columnDef[7] << std::endl;
//		}
//		std::cerr << "================================" << std::endl;

		// clear variables before starting
		// this is in part important because the various 7-Series families do not share the same 
		// family databases, and consequently may have different mTileTypeIndexToColumnType mappings
		mFrameRowCount = mTopRowCount = mBottomRowCount = 0;
		mFrameIndexToAddress.clear();
		mFrameAddressToIndex.clear();
		for(int i = 0; i < eBlockTypeCount; i++) {
			mBitColumnIndexes[i].clear();
			mXdlColumnIndexes[i].clear();
			mBlockFrameIndexBounds[i] = 0;
		}
		mBitColumnToXdlColumn.clear();
		mXdlRowToFrameRowDesignator.clear();
		mTileTypeIndexToColumnType.clear();

		// look up the device tile map
		torc::architecture::DDB ddb(inDeviceName);
		const torc::architecture::Tiles& tiles = ddb.getTiles();
		uint32_t tileCount = tiles.getTileCount();
		uint16_t rowCount = tiles.getRowCount();
		uint16_t colCount = tiles.getColCount();
		ColumnTypeVector columnTypes;
//std::cerr << "tile count: " << tileCount << ", row count: " << rowCount << ", col count " << colCount << std::endl;

		// set up the tile index and name mappings, and the index to column def mapping
		typedef std::map<TileTypeIndex, std::string> TileTypeIndexToName;
		typedef std::map<std::string, TileTypeIndex> TileTypeNameToIndex;
		TileTypeIndexToName tileTypeIndexToName;
		TileTypeNameToIndex tileTypeNameToIndex;
		TileTypeCount tileTypeCount = tiles.getTileTypeCount();
		for(TileTypeIndex tileTypeIndex(0); tileTypeIndex < tileTypeCount; tileTypeIndex++) {
			const std::string tileTypeName = tiles.getTileTypeName(tileTypeIndex);
			tileTypeIndexToName[tileTypeIndex] = tileTypeName;
			tileTypeNameToIndex[tileTypeName] = tileTypeIndex;
			TileTypeNameToColumnType::iterator ttwp = mTileTypeNameToColumnType.find(tileTypeName);
			TileTypeNameToColumnType::iterator ttwe = mTileTypeNameToColumnType.end();
			if(ttwp != ttwe) mTileTypeIndexToColumnType[tileTypeIndex] = EColumnType(ttwp->second);
//if(ttwp != ttwe) {
//	std::cout << "Tile type index " << tileTypeIndex << "\tmapped " << tileTypeName 
//		<< " to " << ttwp->second << std::endl;
//}
		}

		// map XDL tiles to top/bottom halves and frame rows
		setRowCounts(inDeviceName);
		mFrameRowCount = mBottomRowCount + mTopRowCount;
		columnTypes.resize(colCount * mFrameRowCount);
		bool isArtix200t = mDevice == common::eXC7A200T;
		bool isZynq010 = mDevice == common::eXC7Z010;

		// initialize the XDL rows and columns
		uint32_t xdlCenterRow = mTopRowCount * eClockRegionRows;
		uint32_t baseCol = 0;
		// iterate over both halves of the device
		for(uint8_t half = 0; half < 2; half++) {
			EFarTopBottom topBottom(half == eFarTop ? eFarTop : eFarBottom);
			// determine the row count and XDL row offset
			uint32_t farRowCount = topBottom == eFarTop ? mTopRowCount : mBottomRowCount;
			// iterate over the frame rows
			for(uint32_t farRow = 0; farRow < farRowCount; farRow++) {
				// determine the XDL base row for this frame row
				TileRow xdlBaseRow(topBottom == eFarTop
					? (xdlCenterRow - (farRow + 1) * eClockRegionRows)
					: (xdlCenterRow + (farRow + 0) * eClockRegionRows)
					);
				// iterate over the XDL columns
				uint32_t bitCol = 0;
				for(TileCol xdlCol(0); xdlCol < colCount; xdlCol++) {
					// special case for the xc7a200t: we make all rows like bottom row 0
					TileRow xdlRow(isArtix200t ? xdlCenterRow : xdlBaseRow);
					// try to identify the column type
					columnTypes[baseCol + xdlCol] = eColumnTypeEmpty;
					TileTypeIndexToColumnType::iterator ttwe = mTileTypeIndexToColumnType.end();
					TileTypeIndexToColumnType::iterator ttwp = ttwe;
					// iterate over the XDL rows within the frame row
					for(int rowOffset = 0; rowOffset < eClockRegionRows; rowOffset++, xdlRow++) {
						// look up the tile info
						const torc::architecture::TileInfo& tileInfo 
							= tiles.getTileInfo(tiles.getTileIndex(xdlRow, xdlCol));
						TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
						// determine whether the tile type widths are defined
						ttwp = mTileTypeIndexToColumnType.find(tileTypeIndex);
						if(ttwp != ttwe) {
							// what an ugly way to handle the PSS macro-column
							if(EColumnType(ttwp->second) == eColumnTypePss) {
								// this is for the xc7z010
								if(isZynq010) {
									// the density of this layout is purely conjectural
									// column types match frame addresses and other Zynq devices
									columnTypes[baseCol + xdlCol - 12] = eColumnTypeIoi;
									columnTypes[baseCol + xdlCol - 11] = eColumnTypeCmt;
									columnTypes[baseCol + xdlCol - 10] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol -  9] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol -  8] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol -  7] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol -  6] = eColumnTypeBram;
									columnTypes[baseCol + xdlCol -  5] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol -  4] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol -  3] = eColumnTypeDsp;
									columnTypes[baseCol + xdlCol -  2] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol -  1] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol +  0] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol +  1] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol +  2] = eColumnTypeDsp;
									columnTypes[baseCol + xdlCol +  3] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol +  4] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol +  5] = eColumnTypeBram;
									columnTypes[baseCol + xdlCol +  6] = eColumnTypeClb;
									xdlCol += 6;
									bitCol += 19;
									break;
								// this is for all other Zynq devices
								} else  {
									// this layout is based on bottom row 1 of xc7z020 and others
									columnTypes[baseCol + xdlCol - 31] = eColumnTypeIoi;
									columnTypes[baseCol + xdlCol - 27] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol - 28] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol - 25] = eColumnTypeCmt;
									columnTypes[baseCol + xdlCol - 22] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol - 21] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol - 20] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol - 19] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol - 18] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol - 17] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol - 16] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol - 15] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol - 13] = eColumnTypeBram;
									columnTypes[baseCol + xdlCol - 11] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol - 10] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol -  9] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol -  8] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol -  7] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol -  6] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol -  4] = eColumnTypeDsp;
									columnTypes[baseCol + xdlCol -  2] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol -  1] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol +  0] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol +  1] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol +  2] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol +  3] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol +  4] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol +  5] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol +  7] = eColumnTypeDsp;
									columnTypes[baseCol + xdlCol +  9] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol + 10] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol + 11] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol + 12] = eColumnTypeClb;
									columnTypes[baseCol + xdlCol + 13] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol + 14] = eColumnTypeInt;
									columnTypes[baseCol + xdlCol + 16] = eColumnTypeBram;
									columnTypes[baseCol + xdlCol + 18] = eColumnTypeClb;
									xdlCol += 18;
									bitCol += 37;
									break;
								}
							}
//std::cout << "" << (topBottom == eFarTop ? "top" : "bottom") << ", far row: " << farRow 
//	<< ", xdl column: " << xdlCol << " (" << baseCol + xdlCol << "), bit column: " 
//	<< bitCol << ", xdl base row: " << xdlBaseRow;
							//std::cout << "	" << tiles.getTileTypeName(tileInfo.getTypeIndex()) 
							//	<< std::endl;
							columnTypes[baseCol + xdlCol] = static_cast<EColumnType>(ttwp->second);
							bitCol++;
							break;
						}
					}
				}
				baseCol += colCount;
			}
		}

		// write out the tile map as a spreadsheet
		boost::filesystem::path workingPath = torc::common::DirectoryTree::getWorkingPath();
		boost::filesystem::path generatedMap = workingPath / (inDeviceName + ".map.csv");
		std::fstream tilemapStream(generatedMap.string().c_str(), std::ios::out);
		for(TileRow row; row < rowCount; row++) {
			for(TileCol col; col < colCount; col++) {
				const torc::architecture::TileInfo& tileInfo 
					= tiles.getTileInfo(tiles.getTileIndex(row, col));
				TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
				tilemapStream << tiles.getTileTypeName(tileTypeIndex);
				if(col + 1 < colCount) tilemapStream << ",";
			}
			tilemapStream << std::endl;
		}
		tilemapStream.close();

		// update bitstream device information
//std::cerr << "tile count: " << tileCount << ", row count: " << rowCount << ", col count " << colCount << std::endl;
//std::cout << "invoking setDeviceInfo()" << std::endl;
		setDeviceInfo(DeviceInfo(tileCount, rowCount, colCount, columnTypes));
//std::cout << "finished setDeviceInfo()" << std::endl;
//		setRowCounts(inDeviceName);

	}

#endif

	void Virtex7::setRowCounts(const string& inDeviceName) {
		// The division between top and bottom rows can be determined by the locations of the 
		// CLK_HROW_TOP_R and CLK_HROW_BOT_R tiles in the clock column.  The number of clock 
		// regions above and including the CLK_HROW_TOP_R tile determine the number of top rows in 
		// the bitstream.  The number of clock regions below and including the CLK_HROW_BOT_R tile 
		// determine the number of bottom rows in the bitstream.
		using namespace torc::common;
		switch(mDevice) {
			// Artix
			case eXC7A100T: mTopRowCount = 2; mBottomRowCount = 2; break;
			case eXC7A200T: mTopRowCount = 2; mBottomRowCount = 3; break;
			// Kintex
			case eXC7K70T: mTopRowCount = 2; mBottomRowCount = 2; break;
			case eXC7K160T: mTopRowCount = 2; mBottomRowCount = 3; break;
			case eXC7K325T: mTopRowCount = 4; mBottomRowCount = 3; break;
			case eXC7K355T: mTopRowCount = 3; mBottomRowCount = 3; break;
			case eXC7K410T: mTopRowCount = 4; mBottomRowCount = 3; break;
			case eXC7K420T: mTopRowCount = 4; mBottomRowCount = 4; break;
			case eXC7K480T: mTopRowCount = 4; mBottomRowCount = 4; break;
			// Virtex
			case eXC7V585T: mTopRowCount = 5; mBottomRowCount = 4; break;
			case eXC7VX330T: mTopRowCount = 5; mBottomRowCount = 2; break;
			case eXC7VX415T: mTopRowCount = 5; mBottomRowCount = 1; break;
			case eXC7VX485T: mTopRowCount = 5; mBottomRowCount = 2; break;
			case eXC7VX550T: mTopRowCount = 5; mBottomRowCount = 5; break;
			case eXC7VX690T: mTopRowCount = 5; mBottomRowCount = 5; break;
			case eXC7VX980T: mTopRowCount = 4; mBottomRowCount = 5; break;
			// Zynq
			case eXC7Z010: mTopRowCount = 1; mBottomRowCount = 1; break;
			case eXC7Z020: mTopRowCount = 1; mBottomRowCount = 2; break;
			case eXC7Z030: mTopRowCount = 1; mBottomRowCount = 3; break;
			case eXC7Z045: mTopRowCount = 1; mBottomRowCount = 6; break;
			default: 
				std::cerr << "Bitstream row dimensions are undefined for " << mDevice << "." 
					<< std::endl;
				mTopRowCount = 0; mBottomRowCount = 0;
				break;
		}
	}

	void Virtex7::initializeFrameMaps(void) {

			bool debug = false;
			setRowCounts(mDeviceName);
			mFrameRowCount = mTopRowCount + mBottomRowCount;
			if(mFrameRowCount == 0) return;
			uint32_t frameCount = 0;

//std::cerr << "row count: " << mDeviceInfo.getRowCount() << ", far row count: " << mFrameRowCount 
//	<< std::endl;

			// map XDL tiles to top/bottom halves and frame rows
			mXdlRowToFrameRowDesignator.resize(mDeviceInfo.getRowCount());
			uint32_t xdlRow = 0;
			uint32_t xdlBaseRow = xdlRow;
			uint32_t baseCol = 0;
			uint32_t serialFrame = mTopRowCount - 1;
			baseCol = serialFrame * mDeviceInfo.getColCount();
			for(uint32_t frameRow = mTopRowCount - 1; ; frameRow--) {
				for(int i = 0; i < eClockRegionRows; i++, xdlRow++) {
//std::cout << "mapping XDL row " << xdlRow << " to top frame row " << frameRow << " (serial " 
//	<< serialFrame << ")" << " (base " << xdlBaseRow << ") from " << baseCol << std::endl;
					mXdlRowToFrameRowDesignator[xdlRow] 
						= FrameRowDesignator(eFarTop, serialFrame, xdlBaseRow, baseCol);
				}
				xdlBaseRow += eClockRegionRows;
				baseCol -= mDeviceInfo.getColCount();
				serialFrame--;
				if(frameRow == 0) break;
			}
			// bottom half rows
			serialFrame = mTopRowCount;
			baseCol = serialFrame * mDeviceInfo.getColCount();
			for(uint32_t frameRow = 0; frameRow < mBottomRowCount; frameRow++) {
				for(int i = 0; i < eClockRegionRows; i++, xdlRow++) {
//std::cout << "mapping XDL row " << xdlRow << " to bottom frame row " << frameRow << " (serial " 
//	<< serialFrame << ")" << " (base " << xdlBaseRow << ") from " << baseCol << std::endl;
					mXdlRowToFrameRowDesignator[xdlRow] 
						= FrameRowDesignator(eFarBottom, serialFrame, xdlBaseRow, baseCol);
				}
				xdlBaseRow += eClockRegionRows;
				baseCol += mDeviceInfo.getColCount();
				serialFrame++;
			}

			// generate the columns and mappings
			uint32_t frameIndex = 0;
			for(uint32_t i = 0; i < eFarBlockTypeCount; i++) {
				EFarBlockType blockType = EFarBlockType(i);
				uint32_t blockFrameIndexBounds = 0;
				// set first frame index to 0
				uint32_t bitIndex = 0;
				uint32_t xdlIndex = 0;
				mBitColumnIndexes[i].push_back(bitIndex);
				mXdlColumnIndexes[i].push_back(xdlIndex);
				bool blockUsed = false;
				uint32_t baseCol = 0;
				for(uint32_t half = 0; half < 2; half++) {
					EFarTopBottom topBottom(half == eFarTop ? eFarTop : eFarBottom);
					uint32_t farRowCount = (topBottom == eFarTop) ? mTopRowCount : mBottomRowCount;
					for(uint32_t farRow = 0; farRow < farRowCount; farRow++) {
						// build the columns
						uint32_t farMajor = 0;
						typedef torc::common::EncapsulatedInteger<uint16_t> ColumnIndex;
						uint16_t finalColumn = mDeviceInfo.getColCount() - 1;
						uint32_t xdlColumnCount = 0;
						uint32_t bitColumnCount = 0;
						for(ColumnIndex col; col < mDeviceInfo.getColCount(); col++) {
							uint32_t realCol = col + baseCol;
							uint32_t width 
								= mColumnDefs[mDeviceInfo.getColumnTypes()[col + baseCol]][i];
//std::cerr << "block: " << i << ", xdl col: " << col << " => " << realCol << ": " 
//	<< mDeviceInfo.getColumnTypes()[realCol] << std::endl;
							// allocate the frame maps
							for(uint32_t farMinor = 0; farMinor < width; farMinor++) {
								FrameAddress far(EFarTopBottom(topBottom), 
									blockType, farRow, farMajor, farMinor);
								mFrameIndexToAddress[frameIndex] = far;
								mFrameAddressToIndex[far] = frameIndex;
								frameIndex++;
								blockFrameIndexBounds++;
							}
//if(width) std::cerr << "CALCULATED: " << (topBottom ? "B" : "T") << blockType << "(" << farRow 
//	<< "," << farMajor << "." << (width - 1) << "): " 
// << mColumnDefs[mDeviceInfo.getColumnTypes()[col + baseCol]].getName() << std::endl;
							if(width > 0) {
								farMajor++;
								blockUsed = true;
							}
							frameCount += width;
							// indexes for Bitstream Columns, only stores non-empty tile types
							if(mDeviceInfo.getColumnTypes()[realCol] != eColumnTypeEmpty && 
								mDeviceInfo.getColumnTypes()[realCol] != eColumnTypeInt) {
								mBitColumnToXdlColumn[bitColumnCount] = xdlColumnCount;
								bitColumnCount++;
								bitIndex += width;
								mBitColumnIndexes[i].push_back(bitIndex);
								if(blockUsed && col == finalColumn) {
									bitIndex += getRowPadFrames();
								}
							}
							// indexes for XDL Columns
							xdlIndex += width;
							mXdlColumnIndexes[i].push_back(xdlIndex);
							xdlColumnCount++;
							if(blockUsed && col == finalColumn) {
								xdlIndex += getRowPadFrames();
							}
						}
						baseCol += mDeviceInfo.getColCount();
						// account for two pad frames after each frame row
						if(blockUsed) {
							frameIndex += getRowPadFrames();
							blockFrameIndexBounds += getRowPadFrames();
						}
						if(debug) std::cout << "Last frame index:   [" << i << ", " << frameIndex 
							<< "]" << std::endl;
					}
				}
				// stores frame index bounds for each block type
				mBlockFrameIndexBounds[i] = blockFrameIndexBounds;
				if(debug) std::cout << "***Block frame index bounds: " << mBlockFrameIndexBounds[i] 
					<< std::endl;
			}
			// test to check proper indexing
			if(debug) {
				for(uint32_t i = 0; i < eFarBlockTypeCount; i++) {
					for(uint32_t j = 0; j < mBitColumnIndexes[i].size(); j++) 
						std::cout << "Bit Value at index: (" << i << ", " << j << ") : "
							<< mBitColumnIndexes[i][j] << std::endl;
					for(uint32_t k = 0; k < mXdlColumnIndexes[i].size(); k++)
						std::cout << "Xdl Value at index: (" << i << ", " << k << ") : "
							<< mXdlColumnIndexes[i][k] << std::endl;
				}
			}
	}

	VirtexPacketVector Virtex7::generateFullBitstreamPrefix(void) {
		//	0000005d: DUMMY
		//	00000061: DUMMY
		//	00000065: DUMMY
		//	00000069: DUMMY
		//	0000006d: DUMMY
		//	00000071: DUMMY
		//	00000075: DUMMY
		//	00000079: DUMMY
		//	0000007d: BUS WIDTH SYNC
		//	00000081: BUS WIDTH DETECT
		//	00000085: DUMMY
		//	00000089: DUMMY
		//	0000008d: SYNC
		//	00000091: NOP x 1
		//	00000095: TYPE1 WRITE TIMER: 00000000 (TimerForUser:Disabled, TimerForConfig:Disabled)
		//	0000009d: TYPE1 WRITE WBSTAR: 00000000 (RevisionSelectTristate:Disabled, 
		//				NextRevisionSelect:00)
		//	000000a5: TYPE1 WRITE CMD NULL
		//	000000ad: NOP x 1
		//	000000b1: TYPE1 WRITE CMD RCRC
		//	000000b9: NOP x 2
		//	000000c1: TYPE1 WRITE [UNKNOWN REG 19]: 00000000
		//	000000c9: TYPE1 WRITE COR0: 02003fe5 (DONE_status:DonePin, DonePipe:Yes, DriveDone:No, 
		//				Capture:Continuous, ConfigRate:[UNKNOWN 0], StartupClk:Cclk, DONE_cycle:4, 
		//				Match_cycle:NoWait, LCK_cycle:No Wait, GTS_cycle:5, GWE_cycle:6)
		//	000000d1: TYPE1 WRITE COR1: 00000000 (PersistDeassertAtDesynch:Disabled, 
		//				ActionReadbackCRC:Continue, InitAsCRCErrorPin:Disabled, 
		//				ContinuousReadbackCRC:Enabled, BPI_1st_read_cycle:1, BPI_page_size:1)
		//	000000d9: TYPE1 WRITE IDCODE: 03651093
		//	000000e1: TYPE1 WRITE CMD SWITCH
		//	000000e9: NOP x 1
		//	000000ed: TYPE1 WRITE MASK: 00000401 (EFUSE_key:Protected, ICAP_sel:Protected, 
		//				OverTempPowerDown:Protected, GLUTMASK:Protected, FARSRC:Protected, 
		//				Encrypt:Protected, Security:Protected, Persist:Protected, 
		//				GTS_USER_B:Writable)
		//	000000f5: TYPE1 WRITE CTL0: 00000501 (EFUSE_key:No, ICAP_sel:Top, 
		//				OverTempPowerDown:Disable, GLUTMASK:Dynamic, FARSRC:FAR, Encrypt:No, 
		//				Security:None, Persist:No, GTS_USER_B:IoActive)
		//	000000fd: TYPE1 WRITE MASK: 00000000 ()
		//	00000105: TYPE1 WRITE CTL1: 00000000 ()
		//	0000010d: NOP x 8

		// declare the packet vector and define a NOP packet
		VirtexPacketVector packets;
		VirtexPacket nop(VirtexPacket::makeHeader(ePacketType1, eOpcodeNOP, 0, 0));
		VirtexPacket dummy(eSynchronizationDummy);
		// dummy words
		packets.insert(packets.end(), 8, dummy);
		// bus width detect
		packets.push_back(VirtexPacket(eSynchronizationBusWidthSync));
		packets.push_back(VirtexPacket(eSynchronizationBusWidthDetect));
		packets.push_back(dummy);
		packets.push_back(dummy);
		// sync
		packets.push_back(VirtexPacket(eSynchronizationSync));
		packets.push_back(nop);
		// watchdog timer
		packets.push_back(VirtexPacket::makeType1Write(eRegisterTIMER, 0));
		// warm boot register
		packets.push_back(VirtexPacket::makeType1Write(eRegisterWBSTAR, 0));
		// NULL command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandNULL));
		packets.push_back(nop);
		// reset CRC command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandRCRC));
		packets.push_back(nop);
		packets.push_back(nop);
		// undocumented register 19
		packets.push_back(VirtexPacket::makeType1Write(19, 0));
		// configuration options register 0
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCOR0, 
				makeSubfield(eRegisterCOR0, "DONE_status", "DonePin") |
				makeSubfield(eRegisterCOR0, "DonePipe", "Yes") |
				makeSubfield(eRegisterCOR0, "DriveDone", "No") |
				makeSubfield(eRegisterCOR0, "Capture", "Continuous") |
				makeSubfield(eRegisterCOR0, "ConfigRate", "[UNKNOWN 0]") |
				makeSubfield(eRegisterCOR0, "StartupClk", "Cclk") |
				makeSubfield(eRegisterCOR0, "DONE_cycle", "4") |
				makeSubfield(eRegisterCOR0, "Match_cycle", "NoWait") |
				makeSubfield(eRegisterCOR0, "LCK_cycle", "NoWait") |
				makeSubfield(eRegisterCOR0, "GTS_cycle", "5") |
				makeSubfield(eRegisterCOR0, "GWE_cycle", "6") |
			0));
		// configuration options register 1
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCOR1, 
				makeSubfield(eRegisterCOR1, "PersistDeassertAtDesynch", "Disabled") |
				makeSubfield(eRegisterCOR1, "ActionReadbackCRC", "Continue") |
				makeSubfield(eRegisterCOR1, "InitAsCRCErrorPin", "Disabled") |
				makeSubfield(eRegisterCOR1, "ContinuousReadbackCRC", "Enabled") |
				makeSubfield(eRegisterCOR1, "BPI_1st_read_cycle", "1") |
				makeSubfield(eRegisterCOR1, "BPI_page_size", "1") |
			0));
		// write the ID code
		packets.push_back(VirtexPacket::makeType1Write(eRegisterIDCODE, 0x00000000));
		// clock and rate switch command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandSWITCH));
		packets.push_back(nop);
		// control register 0 mask
		packets.push_back(VirtexPacket::makeType1Write(eRegisterMASK, 
				makeSubfield(eRegisterMASK, "EFUSE_key", "Protected") |
				makeSubfield(eRegisterMASK, "ICAP_sel", "Protected") |
				makeSubfield(eRegisterMASK, "OverTempPowerDown", "Protected") |
				makeSubfield(eRegisterMASK, "GLUTMASK", "Protected") |
				makeSubfield(eRegisterMASK, "FARSRC", "Protected") |
				makeSubfield(eRegisterMASK, "Encrypt", "Protected") |
				makeSubfield(eRegisterMASK, "Security", "Protected") |
				makeSubfield(eRegisterMASK, "Persist", "Protected") |
				makeSubfield(eRegisterMASK, "GTS_USER_B", "Writable") |
			0));
		// control register 0
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCTL0, 
				makeSubfield(eRegisterCTL0, "EFUSE_key", "NO") |
				makeSubfield(eRegisterCTL0, "ICAP_sel", "Top") |
				makeSubfield(eRegisterCTL0, "OverTempPowerDown", "Disable") |
				makeSubfield(eRegisterCTL0, "GLUTMASK", "Dynamic") |
				makeSubfield(eRegisterCTL0, "FARSRC", "FAR") |
				makeSubfield(eRegisterCTL0, "Encrypt", "No") |
				makeSubfield(eRegisterCTL0, "Security", "None") |
				makeSubfield(eRegisterCTL0, "Persist", "No") |
				makeSubfield(eRegisterCTL0, "GTS_USER_B", "IoActive") |
			0));
		// control register 1 mask
		packets.push_back(VirtexPacket::makeType1Write(eRegisterMASK, 0));
		// control register 1
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCTL1, 0));
		// more NOPs
		packets.insert(packets.end(), 8, nop);
		// return the packet vector
		return packets;
	}

	VirtexPacketVector Virtex7::generateFullBitstreamSuffix(void) {
		//	00ae95cd: TYPE1 WRITE CRC: 9f18b580
		//	00ae95d1: NOP x 2
		//	00ae95d9: TYPE1 WRITE CMD GRESTORE
		//	00ae95e1: NOP x 1
		//	00ae95e5: TYPE1 WRITE CMD DGHIGH/LFRM
		//	00ae95ed: NOP x 100
		//	00ae977d: TYPE1 WRITE CMD START
		//	00ae9785: NOP x 1
		//	00ae9789: TYPE1 WRITE FAR: 03be0000
		//	00ae9791: TYPE1 WRITE MASK: 00000501 (EFUSE_key:Protected, ICAP_sel:Protected, 
		//				OverTempPowerDown:Protected, GLUTMASK:Writable, FARSRC:Protected, 
		//				Encrypt:Protected, Security:Protected, Persist:Protected, 
		//				GTS_USER_B:Writable)
		//	00ae9799: TYPE1 WRITE CTL0: 00000501 (EFUSE_key:No, ICAP_sel:Top, 
		//				OverTempPowerDown:Disable, GLUTMASK:Dynamic, FARSRC:FAR, Encrypt:No, 
		//				Security:None, Persist:No, GTS_USER_B:IoActive)
		//	00ae97a1: TYPE1 WRITE CRC: e3ad7ea5
		//	00ae97a9: NOP x 2
		//	00ae97b1: TYPE1 WRITE CMD DESYNCH
		//	00ae97b9: NOP x 400

		// declare the packet vector and define a NOP packet
		VirtexPacketVector packets;
		VirtexPacket nop(VirtexPacket::makeHeader(ePacketType1, eOpcodeNOP, 0, 0));
		// write a placeholder CRC value
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCRC, 0));
		packets.push_back(nop);
		packets.push_back(nop);
		// restore command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandGRESTORE));
		packets.push_back(nop);
		// last frame command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandLFRM));
		packets.insert(packets.end(), 100, nop);
		// start command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandSTART));
		packets.push_back(nop);
		// frame address register
		packets.push_back(VirtexPacket::makeType1Write(eRegisterFAR, 
			eFarMaskBlockType | eFarMaskRow)); // is this what the configuration controller wants?
		// control register 0 mask
		packets.push_back(VirtexPacket::makeType1Write(eRegisterMASK, 
				makeSubfield(eRegisterMASK, "EFUSE_key", "Protected") |
				makeSubfield(eRegisterMASK, "ICAP_sel", "Protected") |
				makeSubfield(eRegisterMASK, "OverTempPowerDown", "Protected") |
				makeSubfield(eRegisterMASK, "GLUTMASK", "Protected") |
				makeSubfield(eRegisterMASK, "FARSRC", "Protected") |
				makeSubfield(eRegisterMASK, "Encrypt", "Protected") |
				makeSubfield(eRegisterMASK, "Security", "Protected") |
				makeSubfield(eRegisterMASK, "Persist", "Protected") |
				makeSubfield(eRegisterMASK, "GTS_USER_B", "Writable") |
			0));
		// control register 0
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCTL0, 
				makeSubfield(eRegisterCTL0, "EFUSE_key", "NO") |
				makeSubfield(eRegisterCTL0, "ICAP_sel", "Top") |
				makeSubfield(eRegisterCTL0, "OverTempPowerDown", "Disable") |
				makeSubfield(eRegisterCTL0, "GLUTMASK", "Dynamic") |
				makeSubfield(eRegisterCTL0, "FARSRC", "FAR") |
				makeSubfield(eRegisterCTL0, "Encrypt", "No") |
				makeSubfield(eRegisterCTL0, "Security", "None") |
				makeSubfield(eRegisterCTL0, "Persist", "No") |
				makeSubfield(eRegisterCTL0, "GTS_USER_B", "IoActive") |
			0));
		// write the CRC value
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCRC, 0x00000000));
		packets.push_back(nop);
		packets.push_back(nop);
		// desynch command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandDESYNCH));
		packets.insert(packets.end(), 400, nop);
		// return the packet vector
		return packets;
	}

	VirtexPacketVector Virtex7::generatePartialBitstreamPrefix(EBitstreamType inBitstreamType) {
		//	Packets marked S pertain to shutdown bitstreams only
		//		0000005d: DUMMY
		//		00000061: DUMMY
		//		00000065: DUMMY
		//		00000069: DUMMY
		//		0000006d: DUMMY
		//		00000071: DUMMY
		//		00000075: DUMMY
		//		00000079: DUMMY
		//		0000007d: BUS WIDTH SYNC
		//		00000081: BUS WIDTH DETECT
		//		00000085: DUMMY
		//		00000089: DUMMY
		//		0000008d: SYNC
		//		00000091: NOP x 1
		//		00000095: TYPE1 WRITE CMD RCRC
		//		0000009d: NOP x 2
		//		000000a5: TYPE1 WRITE IDCODE: 03651093
		//	S	000000ad: TYPE1 WRITE COR0: 02003fe5 (DONE_status:DonePin, DonePipe:Yes, 
		//					DriveDone:No, Capture:Continuous, ConfigRate:[UNKNOWN 0], 
		//					StartupClk:Cclk, DONE_cycle:4, Match_cycle:NoWait, LCK_cycle:No Wait, 
		//					GTS_cycle:5, GWE_cycle:6)
		//	S	000000b5: TYPE1 WRITE CMD SHUTDOWN
		//	S	000000bd: NOP x 1
		//	S	000000c1: TYPE1 WRITE CRC: 6f322c8f
		//	S	000000c9: NOP x 4
		//	S	000000d9: TYPE1 WRITE CMD AGHIGH
		//	S	000000e1: NOP x 1
		//		000000e5: TYPE1 WRITE CMD NULL

		// declare the packet vector and define a NOP packet
		VirtexPacketVector packets;
		VirtexPacket nop(VirtexPacket::makeHeader(ePacketType1, eOpcodeNOP, 0, 0));
		VirtexPacket dummy(eSynchronizationDummy);
		// dummy words
		packets.insert(packets.end(), 8, dummy);
		// bus width detect
		packets.push_back(VirtexPacket(eSynchronizationBusWidthSync));
		packets.push_back(VirtexPacket(eSynchronizationBusWidthDetect));
		packets.push_back(dummy);
		packets.push_back(dummy);
		// sync
		packets.push_back(VirtexPacket(eSynchronizationSync));
		packets.push_back(nop);
		// reset CRC command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandRCRC));
		packets.push_back(nop);
		packets.push_back(nop);
		// write the ID code
		packets.push_back(VirtexPacket::makeType1Write(eRegisterIDCODE, 0x00000000));
		// extra for shutdown bitstreams
		if(inBitstreamType == eBitstreamTypePartialShutdown) {
			// configuration options register 0
			packets.push_back(VirtexPacket::makeType1Write(eRegisterCOR0, 
					makeSubfield(eRegisterCOR0, "DONE_status", "DonePin") |
					makeSubfield(eRegisterCOR0, "DonePipe", "Yes") |
					makeSubfield(eRegisterCOR0, "DriveDone", "No") |
					makeSubfield(eRegisterCOR0, "Capture", "Continuous") |
					makeSubfield(eRegisterCOR0, "ConfigRate", "[UNKNOWN 0]") |
					makeSubfield(eRegisterCOR0, "StartupClk", "Cclk") |
					makeSubfield(eRegisterCOR0, "DONE_cycle", "4") |
					makeSubfield(eRegisterCOR0, "Match_cycle", "NoWait") |
					makeSubfield(eRegisterCOR0, "LCK_cycle", "NoWait") |
					makeSubfield(eRegisterCOR0, "GTS_cycle", "5") |
					makeSubfield(eRegisterCOR0, "GWE_cycle", "6") |
				0));
			// shutdown command
			packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandSHUTDOWN));
			packets.push_back(nop);
			// write the CRC value
			packets.push_back(VirtexPacket::makeType1Write(eRegisterCRC, 0x00000000));
			packets.insert(packets.end(), 4, nop);
			// aghigh command
			packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandAGHIGH));
			packets.push_back(nop);
		}
		// null command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandNULL));
		// return the packet vector
		return packets;
	}

	VirtexPacketVector Virtex7::generatePartialBitstreamSuffix(EBitstreamType inBitstreamType) {
		//	Packets marked S pertain to shutdown bitstreams only
		//	S	000008ed: TYPE1 WRITE CMD GRESTORE
		//	S	000008f5: NOP x 1
		//		000008f9: TYPE1 WRITE CMD DGHIGH/LFRM
		//		00000901: NOP x 100
		//	S	00000a91: TYPE1 WRITE CMD GRESTORE
		//	S	00000a99: NOP x 1
		//	S	00000a9d: TYPE1 WRITE CMD START
		//	S	00000aa5: NOP x 1
		//		00000aa9: TYPE1 WRITE FAR: 03be0000
		//		00000ab1: TYPE1 WRITE CRC: a658dd48
		//		00000ab9: TYPE1 WRITE CMD DESYNCH
		//		00000ac1: NOP x 16

		// declare the packet vector and define a NOP packet
		VirtexPacketVector packets;
		VirtexPacket nop(VirtexPacket::makeHeader(ePacketType1, eOpcodeNOP, 0, 0));
		// extra for shutdown bitstreams
		if(inBitstreamType == eBitstreamTypePartialShutdown) {
			// restore command
			packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandGRESTORE));
			packets.push_back(nop);
		}
		// last frame command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandLFRM));
		packets.insert(packets.end(), 100, nop);
		// extra for shutdown bitstreams
		if(inBitstreamType == eBitstreamTypePartialShutdown) {
			// restore command
			packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandGRESTORE));
			packets.push_back(nop);
			// start command
			packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandSTART));
			packets.push_back(nop);
		}
		// frame address register
		packets.push_back(VirtexPacket::makeType1Write(eRegisterFAR, 
			eFarMaskBlockType | eFarMaskRow)); // is this what the configuration controller wants?
		// write the CRC value
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCRC, 0x00000000));
		// desynch command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandDESYNCH));
		packets.insert(packets.end(), 16, nop);
		// return the packet vector
		return packets;
	}

	void Virtex7::initializeFullFrameBlocks(void) {
		boost::shared_array<uint32_t> frameWords;
		// walk the bitstream and extract all frames
		Virtex7::iterator p = begin();
		Virtex7::iterator e = end();
		while(p < e) {
		    const VirtexPacket& packet = *p++;
		    if(packet.isType2() && packet.isWrite()) 
  				frameWords = packet.getWords();
		}
		uint32_t index = 0;
		for(uint32_t i = 0; i < Bitstream::eBlockTypeCount; i++) {
			// all frames of block type are extracted
			for(uint32_t j = 0; j < mBlockFrameIndexBounds[i]; j++) {
				mFrameBlocks.mBlock[i].push_back(VirtexFrameSet::FrameSharedPtr
					(new VirtexFrame(getFrameLength(), &frameWords[index])));
				index += getFrameLength();
			}
		}
	}

	void Virtex7::updateFullFrameBlocks(void) {
		/// \todo If the packet size differs from the frame data size, we need to replace the 
		///		entire packet.
		uint32_t frameLength = getFrameLength();
		typedef boost::shared_array<uint32_t> WordSharedArray;
		// walk the bitstream and find the (full) FDRI write packet
		Virtex7::iterator p = begin();
		Virtex7::iterator e = end();
		while(p < e) {
		    const VirtexPacket& packet = *p++;
		    if(packet.isType2() && packet.isWrite()) {
				WordSharedArray words = packet.getWords();
				uint32_t* ptr = words.get();
				for(uint32_t block = 0; block < 8; block++) {
					for(uint32_t frame = 0; frame < mBlockFrameIndexBounds[block]; frame++) {
						const VirtexFrameBlocks::word_t* words 
							= mFrameBlocks.mBlock[block][frame]->getWords();
						for(uint32_t index = 0; index < frameLength; index++) *ptr++ = *words++;
					}
				}
			}
		}
	}

	VirtexFrameBlocks Virtex7::getBitstreamFrames(EFarTopBottom inTopBottom, uint32_t inFrameRow, 
		uint32_t inBitCol, uint32_t inBlockCount) {
		return getXdlFrames(inFrameRow + (inTopBottom == eFarBottom ? (mTopRowCount) : 0), 
			mBitColumnToXdlColumn[inBitCol], inBlockCount);
	}

	VirtexFrameBlocks Virtex7::getBitstreamFrames(uint32_t inBitRow, uint32_t inBitCol, 
		uint32_t& outBeginBit, uint32_t& outEndBit, uint32_t inBlockCount) {
		return getXdlFrames(inBitRow, mBitColumnToXdlColumn[inBitCol], outBeginBit, outEndBit, 
			inBlockCount);
	}

	VirtexFrameBlocks Virtex7::getBitstreamFrames(uint32_t inSerialFrameRow, uint32_t inBitCol, 
		uint32_t inBlockCount) {
		return getXdlFrames(inSerialFrameRow, mBitColumnToXdlColumn[inBitCol], inBlockCount);
	}

	VirtexFrameBlocks Virtex7::getXdlFrames(uint32_t inXdlRow, uint32_t inXdlCol, 
		uint32_t& outBeginBit, uint32_t& outEndBit, uint32_t inBlockCount) {

		// look up the frame row designator for this XDL row
		FrameRowDesignator designator = mXdlRowToFrameRowDesignator[inXdlRow];

		// by default we have no bit position information
		outBeginBit = outEndBit = 0;

		// look up the relative XDL row
		uint32_t relativeXdlRow = inXdlRow - designator.mXdlBaseRow;
		//std::cout << "inXdlRow: " << inXdlRow << std::endl;
		//std::cout << "inXdlCol: " << inXdlCol << std::endl;
		//std::cout << "    designator.mTopBottom: " << designator.mTopBottom << std::endl;
		//std::cout << "    designator.mFrameRow: " << designator.mFrameRow << std::endl;
		//std::cout << "    designator.mXdlBaseRow: " << designator.mXdlBaseRow << std::endl;
		//std::cout << "    designator.mColumnVectorBase: " << designator.mColumnVectorBase 
		//	<< std::endl;

		// provide bit information for the columns that we know
		uint32_t index = relativeXdlRow;
		uint32_t columnVectorBase = designator.mColumnVectorBase;
		//std::cout << "    relativeXdlRow: " << relativeXdlRow << std::endl;
		//std::cout << "    mDeviceInfo.getColumnTypes()[inXdlCol]: " 
		//	<< mDeviceInfo.getColumnTypes()[inXdlCol + columnVectorBase] << std::endl;
		switch(mDeviceInfo.getColumnTypes()[inXdlCol + columnVectorBase]) {
		case eColumnTypeClb:
		case eColumnTypeInt:
			if(index == 0 || index == 26) break; // these tiles have no bits
			index--; // tile 0 has no bits
			outBeginBit = index * 64; // regular tiles are 64 bits long
			outEndBit = outBeginBit + 64;
			if(index > 26) {
				outBeginBit -= 32; // the clock word is 32 bits long
				outEndBit -= 32;   // the clock word is 32 bits long
			}
			break;
		case eColumnTypeDsp:
		case eColumnTypeBram:
			if(index != 5 && index != 10 && index != 15 && index != 20 && index != 25
				&& index != 31 && index != 36 && index != 41 && index != 46 && index != 51) break;
			index -= 5;
			outBeginBit = index * 64; // regular tiles are 64 bits long
			outEndBit = outBeginBit + 320;
			if(index > 25) {
				outBeginBit -= 32; // the clock word is 32 bits long
				outEndBit -= 32;   // the clock word is 32 bits long
			}
			break;
		default:
			break;
		}

		// invoke the function that really does the work
		return getXdlFrames(designator.mFrameRow, designator.mColumnVectorBase, inXdlCol);
	}

	VirtexFrameBlocks Virtex7::getXdlFrames(uint32_t inSerialFrameRow, 
		uint32_t inColumnVectorBase, uint32_t inXdlCol, uint32_t inBlockCount) {
		if(inSerialFrameRow > getFrameRowCount()) return VirtexFrameBlocks();

		// index and extract frames
		int32_t xdlColumnIndex[inBlockCount];
		int32_t xdlColumnBound[inBlockCount];
		for(uint32_t i = 0; i < inBlockCount; i++) {
			// column index of given frame index
			xdlColumnIndex[i] = mXdlColumnIndexes[i][inColumnVectorBase + inXdlCol];
			// frame bounds for given column type
			uint32_t col = inColumnVectorBase + inXdlCol;
			xdlColumnBound[i] = mColumnDefs[mDeviceInfo.getColumnTypes()[col]][i];
			//std::cout << "    i: " << i << std::endl;
			//std::cout << "    length: " << mXdlColumnIndexes[i].size() << std::endl;
			//std::cout << "    xdlColumnBound[i]: " << xdlColumnBound[i] << std::endl;
			//std::cout << "    xdlColumnIndex[i]: " << xdlColumnIndex[i] << std::endl;
			//std::cout << "    col: " << col << std::endl;
		}
		// extract the tile frames for the specified FAR 
		VirtexFrameBlocks frameBlocks;
		for(uint32_t i = 0; i < inBlockCount; i++) {
		    int startIndex = xdlColumnIndex[i];
		    for(int j = 0; j < xdlColumnBound[i]; j++) {
				frameBlocks.mBlock[i].push_back(mFrameBlocks.mBlock[i][startIndex+j]);
				if(!xdlColumnBound[i]) continue;
				int offset = i > 0 ? mXdlColumnIndexes[i-1].back() + 2 : 0;
				std::cout << "Virtex7::getXdlFrames(): pushing mBlock[" << i << "][" 
					<< (startIndex + offset + j) << "]: " 
					<< mFrameIndexToAddress[startIndex + offset + j] << std::endl;
				//std::cout << "    pushing mBlock[" << i << "][" << (startIndex + offset) << "]-[" 
				//	<< (startIndex + offset + xdlColumnBound[i] - 1) << "]" << ": " 
				//	<< mFrameIndexToAddress[startIndex + offset] << " - " 
				//	<< mFrameIndexToAddress[startIndex + offset + xdlColumnBound[i] - 1] 
				//	<< std::endl;
			}
		}
		return frameBlocks;
	}

	uint32_t Virtex7::getPrimaryXdlColumn(uint32_t inXdlRow, uint32_t inXdlCol) {
		// INT columns "belong" with their corresponding primary column in the bitstream, so if the 
		// caller hands us an INT column, we look for the next primary column;
		// if the caller passes in a primary column, we return it as is
		const ColumnTypeVector& columns = mDeviceInfo.getColumnTypes();
		// look up the column vector base index for this XDL row
		uint32_t columnVectorBase = mXdlRowToFrameRowDesignator[inXdlRow].mColumnVectorBase;
		// determine the bounds for the current column
		uint32_t col = inXdlCol;
		ColumnTypeVector::const_iterator b = columnVectorBase + columns.begin();
		ColumnTypeVector::const_iterator p = columnVectorBase + columns.begin() + col;
		ColumnTypeVector::const_iterator e = columnVectorBase + columns.end();
		// if this is anything other than an INT_L or INT_R column, we return it unchanged
		if(*p != eColumnTypeInt) return inXdlCol;
		// if column to the left is also INT, then this is an INT_R, otherwise it's an INT_L
		if(p > b && *(p-1) == eColumnTypeInt) {
			// this is an INT_R
			while(p < e) {
				col++; p++; if(p == e) break;
				if(*p != eColumnTypeEmpty && *p != eColumnTypeVframe && *p != eColumnTypeCfg 
					&& *p != eColumnTypeClock) return col;
			}
		} else {
			// this is an INT_L
			while(p > b) {
				col--; p--;
				if(*p != eColumnTypeEmpty && *p != eColumnTypeVframe && *p != eColumnTypeCfg 
					&& *p != eColumnTypeClock) return col;
			}
		}
		// we shouldn't get here, but if we do, the best thing is to return the original column
		return inXdlCol;
	}

}// namespace bitstream
}// namespace torc

#if 0
	void Virtex7::readFramePackets(void) {
		boost::shared_array<uint32_t> frameWords;
		// precalculate the start indexes of all blocks, and create all possible frames
		uint32_t frameStart[eBlockTypeCount + 1];
		frameStart[0] = 0;
		VirtexFrame frame(eFrameLength);
		for(int i = 0; i < eBlockTypeCount; i++) {
			frameStart[i + 1] = frameStart[i] + mBlockFrameIndexBounds[i];
			// clear any existing frames
			mFrameBlocks.mBlock[i].clear();
			// populate the block with the correct number of empty frames
			for(uint32_t j = 0; j < mBlockFrameIndexBounds[i]; j++) {
				mFrameBlocks.mBlock[i].push_back(VirtexFrameSet::FrameSharedPtr
					(new VirtexFrame(getFrameLength())));
				mFrameBlocks.mBlock[i].back()->setUsed(false);
			}
		}
		// walk the bitstream and extract all frames
		const uint32_t frameLength = getFrameLength();
		uint32_t frameIndex = 0;
		FrameAddress frameAddress;
		ERegister lastAddress = ERegister();
		Virtex7::iterator p = begin();
		Virtex7::iterator e = end();
		while(p < e) {
		    const VirtexPacket& packet = *p++;
			// process FAR write packets
			if(packet.isWrite() && packet.getAddress() == eRegisterFAR) {
				// extract the new frame address
				frameAddress = FrameAddress(packet[1]);
				// convert the frame address to the corresponding frame index
				FrameAddressToIndex::iterator ip = mFrameAddressToIndex.find(frameAddress);
				if(ip != mFrameAddressToIndex.end()) frameIndex = ip->second;
			}
			// process FDRI write packets
			else if(packet.isWrite() 
				&& (
					// this is a Type 2 packet and the prior Type 1 address was FDRI
					(packet.isType2() && lastAddress == eRegisterFDRI)
				||
					// this is a non-empty Type 1 packet and its address is FDRI
					(packet.isType1() && packet.getAddress() == eRegisterFDRI 
					&& packet.getWordCount() > 0)
				)) {
				// determine the number of frames in the packet and look up the frame words
				const uint32_t numWords = packet.getWordCount();
				boost::shared_array<uint32_t> frameWords(packet.getWords());
				uint32_t position = 0;
				// iterate over every frame in the packet
				while(position + frameLength <= numWords) {
					// look up the current block type and the frame index into that block type
					EFarBlockType blockType = frameAddress.mBlockType;
					uint32_t index = frameIndex - frameStart[blockType];
					mFrameBlocks.mBlock[blockType][index] = VirtexFrameSet::FrameSharedPtr
						(new VirtexFrame(frameLength, &frameWords[position]));
					mFrameBlocks.mBlock[blockType][index]->setUsed();
					position += frameLength;
					frameIndex++;
					FrameIndexToAddress::iterator ap = mFrameIndexToAddress.find(frameIndex);
					if(ap != mFrameIndexToAddress.end()) frameAddress = ap->second;
					// sanity escape exit (which also breaks out of the higher level loop)
					if(position > numWords) {
						std::cerr << "Overflowed expected frame counts for device." << std::endl;
						break;
					}
				}
				if(frameIndex != frameStart[frameAddress.mBlockType + 1]) {
					// if we ended on a pad frame, where the current index has no corresponding 
					// frame address, we need to advance to the next valid frame address
					if(mFrameIndexToAddress.find(frameIndex) == mFrameIndexToAddress.end()) 
						frameIndex++;
					if(mFrameIndexToAddress.find(frameIndex) == mFrameIndexToAddress.end()) 
						frameIndex++;
					// at this point we should again be on a valid frame
					FrameIndexToAddress::iterator ap = mFrameIndexToAddress.find(frameIndex);
					if(ap != mFrameIndexToAddress.end()) frameAddress = ap->second;
					else if(frameIndex == frameStart[frameAddress.mBlockType + 1]) /* at end */;
					else {
						std::cerr << "Failed to find the next valid frame address at the end of a "
							"packet." << std::endl;
					}
				}
			}
			// remember the last register address for Type 2 packets
			if(packet.isType1()) lastAddress = ERegister(packet.getAddress());
			// process MFWR write packets
			/// \todo
		}
	}

	VirtexPacketVector::iterator Virtex7::deleteFramePackets(void) {
		// walk the bitstream and look for frame packet boundaries
		// we begin with the first FAR write, and end before the first CRC write after an FDRI write
		Virtex7::iterator b = begin();
		Virtex7::iterator p = b;
		Virtex7::iterator e = end();
		Virtex7::iterator start = e;
		Virtex7::iterator stop = b;
		Virtex7::iterator fdri = b;
		while(p < e) {
			// look up the current packet
		    const VirtexPacket& packet = *p;
			// start from the first FAR write
			if(start == e && packet.isWrite() && packet.getAddress() == eRegisterFAR) {
				start = p;
			}
			// remember the last FDRI write (including a trailing Type 2 write if present)
			if(packet.isWrite() && packet.getAddress() == eRegisterFDRI) {
				fdri = p;
				if((p+1)->isWrite() && (p+1)->isType2()) fdri++;
			}
			// stop on the first CRC after the last FDRI write
			// (Beware: the zero "address" of a Type 2 packet looks like the CRC register)
		    if(stop < fdri && packet.isWrite() && packet.isType1() 
				&& packet.getAddress() == eRegisterCRC) {
				stop = p;
			}
			p++;
		}
		// look for anything unexpected
		p = start;
		while(p < stop) {
			// look up the current packet
		    const VirtexPacket& packet = *p++;
			// abort if we find anything unexpected
			if(start < e && stop > b && !(
				packet.isNop() || (
				packet.isWrite() && (
					packet.getAddress() == eRegisterFAR ||
					packet.getAddress() == eRegisterLOUT ||
					packet.getAddress() == eRegisterFDRI ||
					packet.getAddress() == eRegisterCRC ||
					(packet.getAddress() == eRegisterCMD && packet[1] == eCommandWCFG)
			)))) {
				/// \todo Throw some meaningful exception
				std::cerr << "Unable to safely distinguish frame packets." << std::endl;
				return e;
			}
		}
		// if we found the expected FAR -> FDRI -> CRC structure, delete the frame packets
		if(start < stop && b < stop) {
			erase(start, stop);
			return start;
		}
		// if we didn't find frame packets or delete anything, return the end iterator
		return e;
	}

	VirtexPacketVector Virtex7::generateFullBitstreamPackets(void) {

		// declare the packet vector and define a NOP packet
		typedef VirtexFrame::word_t word_t;
		VirtexPacketVector packets;
		VirtexPacket nop(VirtexPacket::makeHeader(ePacketType1, eOpcodeNOP, 0, 0));

		// determine the total size of the frames to write
		size_t size = 0;
		for(int i = 0; i < eBlockTypeCount; i++) 
			size += mBlockFrameIndexBounds[i] * getFrameLength();
		word_t* frameContents = new VirtexFrameSet::word_t[size];
		word_t* pos = frameContents;
		// gather the frames to write
		for(int i = 0; i < eBlockTypeCount; i++) {
			// look up the frame set for this block type
			VirtexFrameSet& frameSet = mFrameBlocks.mBlock[i];
			// iterate through the frames in the set and collect the frame words
			VirtexFrameSet::iterator p = frameSet.begin();
			VirtexFrameSet::iterator e = frameSet.end();
			while(p < e) {
				// look up the current frame
				VirtexFrameSharedPtr framePtr = *p++;
				// set up the pointers
				const word_t* wp = framePtr->getWords();
				const word_t* we = wp + framePtr->getLength();
				// and copy the frame words into the new memory
				if(wp) do { *pos++ = *wp++; } while(wp < we); // frames with words allocated
				else do { *pos++ = 0; wp++; } while(wp < we); // frames with no words allocated
			}
		}
		// write the starting frame address
		packets.push_back(VirtexPacket::makeType1Write(eRegisterFAR, 0));
		// write the write configuration register command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandWCFG));
		packets.push_back(nop);
		// write 0 bytes to FDRI (in preparation for type 2 write packet)
		packets.push_back(VirtexPacket::makeNullType1Write(eRegisterFDRI));
		// write all frames to FDRI
		packets.push_back(VirtexPacket::makeType2Write(size, frameContents));

		// return the packet vector
		return packets;
	}

	VirtexPacketVector Virtex7::generatePartialBitstreamPackets(EFrameInclude inFrameInclusion) {

		// declare the packet vector and define a NOP packet
		typedef VirtexFrame::word_t word_t;
		VirtexPacketVector packets;
		VirtexPacket nop(VirtexPacket::makeHeader(ePacketType1, eOpcodeNOP, 0, 0));

		// write the starting frame address
		packets.push_back(VirtexPacket::makeType1Write(eRegisterFAR, 0));
		// write the write configuration register command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandWCFG));
		packets.push_back(nop);

		// iterate through the frame blocks looking for groups of contiguous frames that are in use
		bool empty = true;
		uint32_t index = 0;
		uint32_t blockStart = 0;
		for(int i = 0; i < eBlockTypeCount; i++) {
			blockStart += index;
			index = 0;
			bool started = false;
			uint32_t startIndex = 0;
			// look up the frame set for this block type
			VirtexFrameSet& frameSet = mFrameBlocks.mBlock[i];
			// iterate through the frames in the set and add up their lengths
			VirtexFrameSet::iterator p = frameSet.begin();
			VirtexFrameSet::iterator e = frameSet.end();
			while(p < e) {
				// look up the current frame
				VirtexFrameSharedPtr framePtr = *p++;
				// determine whether the frame fits inclusion criteria
					// we include dirty frames, we include clean frames if permitted by the caller, 
					// and if we are collecting frames and we encounter pad frames, we include them,
					// but we stop collecting if we reach the last frame in the set
				/// \todo mFrameIndexToAddress.size() is too short because excludes pad frames
				bool include = p < e 
					&& (
						framePtr->isDirty() 
						|| (inFrameInclusion == eFrameIncludeAllUsedFrames && framePtr->isUsed()) 
						|| (started && blockStart + index < mFrameIndexToAddress.size() && 
							mFrameIndexToAddress.find(blockStart + index) 
								== mFrameIndexToAddress.end())
					);
				// if we are accumulating frames and this frame doesn't meet the criteria, process 
				// the collection of frames and stop collecting
				if((started && !include)) {
					started = false;
					uint32_t stopIndex = index + (p == e ? 1 : 0);
					uint32_t currentIndex = startIndex;
					// std::cerr << "    stopped at: " << stopIndex << std::endl;
					// include two trailing pad frames if appropriate
					if(mFrameIndexToAddress.find(blockStart + stopIndex + 1) 
						== mFrameIndexToAddress.end()) {
						stopIndex++;
						if(mFrameIndexToAddress.find(blockStart + stopIndex + 1) 
							== mFrameIndexToAddress.end())
							stopIndex++;
					}
					// determine the length of the contiguous block
					size_t size = (stopIndex - startIndex) * getFrameLength();
					// allocate a word array
					word_t* frameContents = new VirtexFrameSet::word_t[size];
					word_t* pos = frameContents;
					while(currentIndex < stopIndex) {
						// look up the current frame
						VirtexFrameSharedPtr framePtr = mFrameBlocks.mBlock[i][currentIndex];
						// set up the pointers
						const word_t* wp = framePtr->getWords();
						const word_t* we = wp + framePtr->getLength();
						// and copy the frame words into the new memory
						if(wp) do { *pos++ = *wp++; } while(wp < we); // frames with words
						else do { *pos++ = 0; wp++; } while(wp < we); // frames with no words
						currentIndex++;
					}
					// write the starting frame address
					packets.push_back(VirtexPacket::makeType1Write(eRegisterFAR, 
						mFrameIndexToAddress[blockStart + startIndex]));
					packets.push_back(nop);
					// if the size is more than 2048 words, we have to use a Type 2 write
					if(size > 2048) {
						// write 0 bytes to FDRI (in preparation for type 2 write packet)
						packets.push_back(VirtexPacket::makeNullType1Write(eRegisterFDRI));
						// write all frames to FDRI
						packets.push_back(VirtexPacket::makeType2Write(size, frameContents));
					} else {
						// write all frames to FDRI
						packets.push_back(VirtexPacket::makeType1Write(eRegisterFDRI, size, 
							frameContents));
					}
					if(size) empty = false;
				// if we are not collecting, but we encounter a frame to include, begin collecting
				} else if(!started && include) {
					// std::cerr << "    started at: " << index << std::endl;
					startIndex = index;
					started = true;
				}
				// increment the frame index
				index++;
			}
		}

		// if we generated no frame contents, discard the wrapper packets
		if(empty) packets.clear();
		// return the packet vector
		return packets;
	}
#endif
