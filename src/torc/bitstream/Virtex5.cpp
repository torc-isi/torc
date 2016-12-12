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

#include "torc/bitstream/Virtex5.hpp"
#include <iostream>


/// \todo Warning: this will need to be moved elsewhere.
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <fstream>


namespace torc {
namespace bitstream {

	const char* Virtex5::sPacketTypeName[ePacketTypeCount] = {
		"[UNKNOWN TYPE 0]", "TYPE1", "TYPE2", "[UNKNOWN TYPE 3]", "[UNKNOWN TYPE 4]", 
		"[UNKNOWN TYPE 5]", "[UNKNOWN TYPE 6]", "[UNKNOWN TYPE 7]"
	};

	const char* Virtex5::sOpcodeName[eOpcodeCount] = {
		"NOP", "READ", "WRITE", "RESERVED"
	};

	const char* Virtex5::sRegisterName[eRegisterCount] = {
		"CRC", "FAR", "FDRI", "FDRO", "CMD", "CTL0", "MASK", "STAT", "LOUT", "COR0", "MFWR", "CBC", 
		"IDCODE", "AXSS", "COR1", "CSOB", "WBSTAR", "TIMER", "[UNKNOWN REG 18]", 
		"[UNKNOWN REG 19]", "[UNKNOWN REG 20]", "[UNKNOWN REG 21]", "BOOTSTS", "[UNKNOWN REG 23]", 
		"CTL1"
	};

	const char* Virtex5::sCommandName[eCommandCount] = {
		"NULL", "WCFG", "MFW", "DGHIGH/LFRM", "RCFG", "START", "RCAP", "RCRC", "AGHIGH", "SWITCH", 
		"GRESTORE", "SHUTDOWN", "GCAPTURE", "DESYNCH", "Reserved", "IPROG", "[UNKNOWN CMD 16]", 
		"LTIMER"
	};

#define VALUES (const char*[])

	/// \see Control Register 0 (CTL0): UG191, v3.7, June 24, 2009, Figure 6-1.
	const Bitstream::Subfield Virtex5::sCTL0[] = {
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
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Control Register 0 (CTL0): UG191, v3.7, June 24, 2009, Figure 6-1.
	const Bitstream::Subfield Virtex5::sMASK0[] = {
		{0x00000001,  0, "GTS_USER_B", "GTS_USER_B", 0, VALUES{"Protected", "Writable", 0}},
		{0x00000008,  3, "Persist", "PERSIST", 0,  VALUES{"Protected", "Writable", 0}},
		{0x00000030,  4, "Security", "SBITS", 0, 
			VALUES{"Protected", "[UNKNOWN 1]", "[UNKNOWN 2]", "Writable", 0}},
		{0x00000040,  6, "Encrypt", "DEC", 0,  VALUES{"Protected", "Writable", 0}},
		{0x00000100,  8, "GLUTMASK", "GLUTMASK", 0,  VALUES{"Protected", "Writable", 0}},
		{0x00001000, 12, "OverTempPowerDown", "OverTempPowerDown", 0, VALUES{"Protected", 
			"Writable", 0}},
		{0x40000000, 30, "ICAP_sel", "ICAP_SEL", 0, VALUES{"Protected", "Writable", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Control Register 1 (CTL1): UG191, v3.7, June 24, 2009, Figure 6-2.
	const Bitstream::Subfield Virtex5::sCTL1[] = {
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Configuration Options Register 0 (COR0): UG191, v3.7, June 24, 2009, Table 6-10.
	const Bitstream::Subfield Virtex5::sCOR0[] = {
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
		{0x000001c0,  6, "LCK_cycle", "LOCK_CYCLE", 0,
			// bitgen: NoWait, 0, 1, 2, 3, 4, 5, 6
			// config: 000:"0", 001:"1", 010:"2", 011:"3", 100:"4", 101:"5", 110:"6", 111:"KEEP"
			VALUES{"0", "1", "2", "3", "4", "5", "6", "NoWait", 0}},
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
		{0x10000000, 28, "CRC", "CRC_BYPASS", 0,
			// bitgen: Enable, Disable
			// config: 0:"CRC enabled", 1:"CRC disabled"
			VALUES{"Enable", "Disable", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Configuration Options Register 1 (COR1): UG191, v3.7, June 24, 2009, Table 6-11.
	const Bitstream::Subfield Virtex5::sCOR1[] = {
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
		{0x00020000, 17, "PersistDeassertAtDesynch", "PERSIST_DEASSERT_AT_DESYNCH", 0,
			// bitgen: n/a?
			// config: Enables deassertion of PERSIST with the DESYNCH command
			VALUES{"Disabled", "Enabled", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see WBSTAR Register Description: UG191, v3.7, June 24, 2009, Table 6-12.
	const Bitstream::Subfield Virtex5::sWBSTAR[] = {
		{0x18000000, 27, "NextRevisionSelect", "RS[1:0]", 0,
			// config: RS[1:0] pin value on next warm boot
			VALUES{"00", "01", "10", "11", 0}},
		{0x04000000, 26, "RevisionSelectTristate", "RS_TS_B", 0, 
			// config: 0:"Disabled", 1:"Enabled"
			VALUES{"Disabled", "Enabled", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see TIMER Register Description: UG191, v3.7, June 24, 2009, Table 6-13.
	const Bitstream::Subfield Virtex5::sTIMER[] = {
		{0x01000000, 24, "TimerForConfig", "TIMER_CFG_MON", 0, 
			// config: 0:"Disabled", 1:"Enabled"
			VALUES{"Disabled", "Enabled", 0}},
		{0x02000000, 25, "TimerForUser", "TIMER_USR_MON", 0, 
			// config: 0:"Disabled", 1:"Enabled"
			VALUES{"Disabled", "Enabled", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see BOOTSTS Register Description: UG191, v3.7, June 24, 2009, Table 6-14.
	const Bitstream::Subfield Virtex5::sBOOTSTS[] = {
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
		{0x00000080,  7, "ReadbackCRC_error0", "RBCRC_ERROR_0", 0, 
			// config: RBCRC error cause [sic] reconfiguration
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
		{0x00008000, 15, "ReadbackCRC_error1", "RBCRC_ERROR_1", 0, 
			// config: RBCRC error cause [sic] reconfiguration
			VALUES{"NoError", "Error", 0}},
		{0, 0, 0, 0, 0, 0}
	};

	/// \see Status Register Description: UG191, v3.7, June 24, 2009, Table 6-9.
	const Bitstream::Subfield Virtex5::sSTAT[] = { 
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
		{0, 0, 0, 0, 0, 0}
	};

	/// \brief Return the masked value for a subfield of the specified register.
	uint32_t Virtex5::makeSubfield(ERegister inRegister, const std::string& inSubfield, 
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

//#define GENERATE_STATIC_DEVICE_INFO
#ifndef GENERATE_STATIC_DEVICE_INFO

	extern DeviceInfo xc5vfx30t;
	extern DeviceInfo xc5vfx70t;
	extern DeviceInfo xc5vfx100t;
	extern DeviceInfo xc5vfx130t;
	extern DeviceInfo xc5vfx200t;
	extern DeviceInfo xc5vlx30;
	extern DeviceInfo xc5vlx50;
	extern DeviceInfo xc5vlx85;
	extern DeviceInfo xc5vlx110;
	extern DeviceInfo xc5vlx155;
	extern DeviceInfo xc5vlx220;
	extern DeviceInfo xc5vlx330;
	extern DeviceInfo xc5vlx20t;
	extern DeviceInfo xc5vlx30t;
	extern DeviceInfo xc5vlx50t;
	extern DeviceInfo xc5vlx85t;
	extern DeviceInfo xc5vlx110t;
	extern DeviceInfo xc5vlx155t;
	extern DeviceInfo xc5vlx220t;
	extern DeviceInfo xc5vlx330t;
	extern DeviceInfo xc5vsx35t;
	extern DeviceInfo xc5vsx50t;
	extern DeviceInfo xc5vsx95t;
	extern DeviceInfo xc5vsx240t;
	extern DeviceInfo xc5vtx150t;
	extern DeviceInfo xc5vtx240t;

	void Virtex5::initializeDeviceInfo(const std::string& inDeviceName) {
		using namespace torc::common;
		switch(mDevice) {
			case eXC5VFX30T: setDeviceInfo(xc5vfx30t); break;
			case eXC5VFX70T: setDeviceInfo(xc5vfx70t); break;
			case eXC5VFX100T: setDeviceInfo(xc5vfx100t); break;
			case eXC5VFX130T: setDeviceInfo(xc5vfx130t); break;
			case eXC5VFX200T: setDeviceInfo(xc5vfx200t); break;
			case eXC5VLX30: setDeviceInfo(xc5vlx30); break;
			case eXC5VLX50: setDeviceInfo(xc5vlx50); break;
			case eXC5VLX85: setDeviceInfo(xc5vlx85); break;
			case eXC5VLX110: setDeviceInfo(xc5vlx110); break;
			case eXC5VLX155: setDeviceInfo(xc5vlx155); break;
			case eXC5VLX220: setDeviceInfo(xc5vlx220); break;
			case eXC5VLX330: setDeviceInfo(xc5vlx330); break;
			case eXC5VLX20T: setDeviceInfo(xc5vlx20t); break;
			case eXC5VLX30T: setDeviceInfo(xc5vlx30t); break;
			case eXC5VLX50T: setDeviceInfo(xc5vlx50t); break;
			case eXC5VLX85T: setDeviceInfo(xc5vlx85t); break;
			case eXC5VLX110T: setDeviceInfo(xc5vlx110t); break;
			case eXC5VLX155T: setDeviceInfo(xc5vlx155t); break;
			case eXC5VLX220T: setDeviceInfo(xc5vlx220t); break;
			case eXC5VLX330T: setDeviceInfo(xc5vlx330t); break;
			case eXC5VSX35T: setDeviceInfo(xc5vsx35t); break;
			case eXC5VSX50T: setDeviceInfo(xc5vsx50t); break;
			case eXC5VSX95T: setDeviceInfo(xc5vsx95t); break;
			case eXC5VSX240T: setDeviceInfo(xc5vsx240t); break;
			case eXC5VTX150T: setDeviceInfo(xc5vtx150t); break;
			case eXC5VTX240T: setDeviceInfo(xc5vtx240t); break;
			default: break;
		}
	}

#else

	void Virtex5::initializeDeviceInfo(const std::string& inDeviceName) {

		typedef torc::architecture::xilinx::TileCount TileCount;
		typedef torc::architecture::xilinx::TileRow TileRow;
		typedef torc::architecture::xilinx::TileCol TileCol;
		typedef torc::architecture::xilinx::TileTypeIndex TileTypeIndex;
		typedef torc::architecture::xilinx::TileTypeCount TileTypeCount;

		// look up the device tile map
		torc::architecture::DDB ddb(inDeviceName);
		const torc::architecture::Tiles& tiles = ddb.getTiles();
		uint32_t tileCount = tiles.getTileCount();
		uint16_t rowCount = tiles.getRowCount();
		uint16_t colCount = tiles.getColCount();
		ColumnTypeVector columnTypes;

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
		}

		// identify every column that contains known frames
		columnTypes.resize(colCount);
		uint32_t frameRowCount = 0;
		for(uint32_t blockType = 0; blockType < eFarBlockTypeCount; blockType++) {
			for(TileCol col; col < colCount; col++) {
				columnTypes[col] = eColumnTypeEmpty;
				TileTypeIndexToColumnType::iterator ttwe = mTileTypeIndexToColumnType.end();
				TileTypeIndexToColumnType::iterator ttwp = ttwe;
				for(TileRow row; row < rowCount; row++) {
					// look up the tile info
					const torc::architecture::TileInfo& tileInfo 
						= tiles.getTileInfo(tiles.getTileIndex(row, col));
					TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
					// determine whether the tile type widths are defined
					ttwp = mTileTypeIndexToColumnType.find(tileTypeIndex);
					if(ttwp != ttwe) {
						uint32_t width = mColumnDefs[ttwp->second][blockType];
						frameRowCount += width;
						//std::cout << "    " << tiles.getTileTypeName(tileInfo.getTypeIndex()) 
						// << ": " << width << " (" << frameCount << ")" << std::endl;
						columnTypes[col] = static_cast<EColumnType>(ttwp->second);
						break;
					}
				}
			}
			//std::cout << std::endl;
			//if(blockType == 2) break;
		}

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
		setDeviceInfo(DeviceInfo(tileCount, rowCount, colCount, columnTypes));
	}

#endif

	void Virtex5::initializeFrameMaps(void) {

		bool debug = false;
		mFrameRowCount = 0;
		uint32_t frameCount = 0;
		uint32_t farRowCount = (mDeviceInfo.getRowCount() / 22) >> 1;
		// the xc5vlx20t has 3 total FAR rows, 1 in the bottom half, and 2 in the top half
		bool shortBottomHalf = (farRowCount * 22 << 1) + 1 < mDeviceInfo.getRowCount();
		if(shortBottomHalf) farRowCount++;

		// map XDL tiles to top/bottom halves and frame rows
		uint32_t bottomRowCount = farRowCount - (shortBottomHalf ? 1 : 0);
		uint32_t topRowCount = farRowCount;
		// top half rows
		uint32_t xdlRow = 0;
		uint32_t xdlBaseRow = xdlRow;
		for(uint32_t frameRow = topRowCount - 1; ; frameRow--) {
			for(int i = 0; i < 22; i++, xdlRow++) {
//std::cout << "mapping XDL row " << xdlRow << " to top frame row " << frameRow << " (base " << xdlBaseRow << ")" << std::endl;
				mXdlRowToFrameRowDesignator[xdlRow] 
					= FrameRowDesignator(eFarTop, frameRow, xdlBaseRow);
			}
			xdlBaseRow += 22;
			if(frameRow == 0) break;
		}
		// bottom half rows
		for(uint32_t frameRow = 0; frameRow < bottomRowCount; frameRow++) {
			for(int i = 0; i < 22; i++, xdlRow++) {
//std::cout << "mapping XDL row " << xdlRow << " to bottom frame row " << frameRow << " (base " << xdlBaseRow << ")" << std::endl;
				mXdlRowToFrameRowDesignator[xdlRow] 
					= FrameRowDesignator(eFarBottom, frameRow, xdlBaseRow);
			}
			xdlBaseRow += 22;
		}

		// generate the columns and mappings
		uint32_t frameIndex = 0;
		for(uint32_t i = 0; i < Virtex5::eFarBlockTypeCount; i++) {
			Virtex5::EFarBlockType blockType = Virtex5::EFarBlockType(i);
			uint32_t blockFrameIndexBounds = 0;
			//Set first frame index to 0
			uint32_t bitIndex = 0;
			uint32_t xdlIndex = 0;
			mBitColumnIndexes[i].push_back(bitIndex);
			mXdlColumnIndexes[i].push_back(xdlIndex);
			bool blockUsed = false;
			for(uint32_t half = 0; half < 2; half++) {
				for(uint32_t farRow = 0; farRow < farRowCount; farRow++) {
					// fix short bottom half on xc5vlx20t
					if(shortBottomHalf && half == 0 && (farRow + 1 == farRowCount)) continue;
					// count the number of frame rows
					if(i == 0) mFrameRowCount++;
					// build the columns
					uint32_t farMajor = 0;
					typedef torc::common::EncapsulatedInteger<uint16_t> ColumnIndex;
					uint16_t finalColumn = mDeviceInfo.getColCount()-1;
					uint32_t xdlColumnCount = 0;
					uint32_t bitColumnCount = 0;
					for(ColumnIndex col; col < mDeviceInfo.getColCount(); col++) {
						uint32_t width = mColumnDefs[mDeviceInfo.getColumnTypes()[col]][i];
						//Allocate the frame maps
						for(uint32_t farMinor = 0; farMinor < width; farMinor++) {
							Virtex5::FrameAddress far(Virtex5::EFarTopBottom(half), blockType, 
								farRow, farMajor, farMinor);
							mFrameIndexToAddress[frameIndex] = far;
							mFrameAddressToIndex[far] = frameIndex;
							frameIndex++;
							blockFrameIndexBounds++;
						}
						if(width > 0) {
							farMajor++;
							blockUsed = true;
						}
						frameCount += width;

						//Extract frame indexes for 1 row
						if(farRow == 0 && half == 0) {
						    //Indexes for Bitstream Columns, only stores non-empty tile types
						    if(mDeviceInfo.getColumnTypes()[col] != Virtex5::eColumnTypeEmpty
								&& mDeviceInfo.getColumnTypes()[col] != Virtex5::eColumnTypeInt ) {
								mBitColumnToXdlColumn[bitColumnCount] = xdlColumnCount;
							    bitColumnCount++;
							    bitIndex += width;
							    mBitColumnIndexes[i].push_back(bitIndex);
								if(col == finalColumn) {
									bitIndex += getRowPadFrames();
							        mBitColumnIndexes[i].push_back(bitIndex);
							    }
						    }
						    //Indexes for XDL Columns, stores interconnect and tile indexes for
						    //non-empty tiles
						    xdlIndex += width;
						    mXdlColumnIndexes[i].push_back(xdlIndex);
							xdlColumnCount++;
							if(col == finalColumn) {
								xdlIndex += getRowPadFrames();
								mXdlColumnIndexes[i].push_back(xdlIndex);
							}
						}
					}
					// account for two pad frames after each frame row
					if(blockUsed) {
						frameIndex += getRowPadFrames();
						blockFrameIndexBounds += getRowPadFrames();
					}
					if(debug) std::cout << "Last frame index:   [" << i << ", " << frameIndex 
						<< "]" << std::endl;
				}
			}
			//stores frame index bounds for each block type
			mBlockFrameIndexBounds[i] = blockFrameIndexBounds;
			if(debug) std::cout << "***Block frame index bounds: " << mBlockFrameIndexBounds[i] 
				<< std::endl;
		}
		//Test to check proper indexing
		if(debug) {
  		  for(uint32_t i = 0; i < Virtex5::eFarBlockTypeCount; i++) {
			  for(uint32_t j = 0; j < mBitColumnIndexes[i].size(); j++) 
				  std::cout << "Bit Value at index: (" << i << ", " << j << ") : "
					  << mBitColumnIndexes[i][j] << std::endl;
			  for(uint32_t k = 0; k < mXdlColumnIndexes[i].size(); k++)
				  std::cout << "Xdl Value at index: (" << i << ", " << k << ") : "
					  << mXdlColumnIndexes[i][k] << std::endl;
		    }
		}
	}

	VirtexPacketVector Virtex5::generateFullBitstreamPrefix(void) {
		//	0000005f: DUMMY
		//	00000063: DUMMY
		//	00000067: DUMMY
		//	0000006b: DUMMY
		//	0000006f: DUMMY
		//	00000073: DUMMY
		//	00000077: DUMMY
		//	0000007b: DUMMY
		//	0000007f: BUS WIDTH SYNC
		//	00000083: BUS WIDTH DETECT
		//	00000087: DUMMY
		//	0000008b: DUMMY
		//	0000008f: SYNC
		//	00000093: NOP x 1
		//	00000097: TYPE1 WRITE WBSTAR: 00000000 (RevisionSelectTristate:Disabled, 
		//				NextRevisionSelect:00)
		//	0000009f: TYPE1 WRITE CMD NULL
		//	000000a7: NOP x 1
		//	000000ab: TYPE1 WRITE CMD RCRC
		//	000000b3: NOP x 2
		//	000000bb: TYPE1 WRITE TIMER: 00000000 (TimerForUser:Disabled, TimerForConfig:Disabled)
		//	000000c3: TYPE1 WRITE [UNKNOWN REG 19]: 00000000
		//	000000cb: TYPE1 WRITE COR0: 00003fe5 (DONE_status:DonePin, DonePipe:No, DriveDone:No, 
		//				Capture:Continuous, ConfigRate:[UNKNOWN 0], StartupClk:Cclk, DONE_cycle:4, 
		//				Match_cycle:NoWait, GTS_cycle:5, GWE_cycle:6)
		//	000000d3: TYPE1 WRITE COR1: 00000000 (PersistDeassertAtDesynch:Disabled, 
		//				ActionReadbackCRC:Continue, InitAsCRCErrorPin:Disabled, 
		//				ContinuousReadbackCRC:Disabled, BPI_1st_read_cycle:1, BPI_page_size:1)
		//	000000db: TYPE1 WRITE IDCODE: 02ad6093
		//	000000e3: TYPE1 WRITE CMD SWITCH
		//	000000eb: NOP x 1
		//	000000ef: TYPE1 WRITE MASK: 00400000 (ICAP_sel:Protected, OverTempPowerDown:Protected, 
		//				GLUTMASK:Protected, Encrypt:Protected, Security:Protected, 
		//				Persist:Protected, GTS_USER_B:Protected)
		//	000000f7: TYPE1 WRITE CTL0: 00400000 (ICAP_sel:Top, OverTempPowerDown:Disable, 
		//				GLUTMASK:Masked, Encrypt:No, Security:None, Persist:No, 
		//				GTS_USER_B:IoDisabled)
		//	000000ff: TYPE1 WRITE MASK: 00000000 ()
		//	00000107: TYPE1 WRITE CTL1: 00000000 ()
		//	0000010f: NOP x 8

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
		// warm boot register
		packets.push_back(VirtexPacket::makeType1Write(eRegisterWBSTAR, 0));
		// NULL command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandNULL));
		packets.push_back(nop);
		// reset CRC command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandRCRC));
		packets.push_back(nop);
		packets.push_back(nop);
		// watchdog timer
		packets.push_back(VirtexPacket::makeType1Write(eRegisterTIMER, 0));
		// undocumented register 19
		packets.push_back(VirtexPacket::makeType1Write(19, 0));
		// configuration options register 0
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCOR0, 
				makeSubfield(eRegisterCOR0, "DONE_status", "DonePin") |
				makeSubfield(eRegisterCOR0, "DonePipe", "No") |
				makeSubfield(eRegisterCOR0, "DriveDone", "No") |
				makeSubfield(eRegisterCOR0, "Capture", "Continuous") |
				makeSubfield(eRegisterCOR0, "ConfigRate", "[UNKNOWN 0]") |
				makeSubfield(eRegisterCOR0, "StartupClk", "Cclk") |
				makeSubfield(eRegisterCOR0, "DONE_cycle", "4") |
				makeSubfield(eRegisterCOR0, "Match_cycle", "NoWait") |
				makeSubfield(eRegisterCOR0, "GTS_cycle", "5") |
				makeSubfield(eRegisterCOR0, "GWE_cycle", "6") |
			0));
		// configuration options register 1
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCOR1, 
				makeSubfield(eRegisterCOR1, "PersistDeassertAtDesynch", "Disabled") |
				makeSubfield(eRegisterCOR1, "ActionReadbackCRC", "Continue") |
				makeSubfield(eRegisterCOR1, "InitAsCRCErrorPin", "Disabled") |
				makeSubfield(eRegisterCOR1, "ContinuousReadbackCRC", "Disabled") |
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
				makeSubfield(eRegisterMASK, "ICAP_sel", "Protected") |
				makeSubfield(eRegisterMASK, "OverTempPowerDown", "Protected") |
				makeSubfield(eRegisterMASK, "GLUTMASK", "Protected") |
				makeSubfield(eRegisterMASK, "FARSRC", "Protected") |
				makeSubfield(eRegisterMASK, "Encrypt", "Protected") |
				makeSubfield(eRegisterMASK, "Security", "Protected") |
				makeSubfield(eRegisterMASK, "Persist", "Protected") |
				makeSubfield(eRegisterMASK, "GTS_USER_B", "Protected") |
				0x00400000 /* this bit is not defined in the configuration guide */ |
			0));
		// control register 0
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCTL0, 
				makeSubfield(eRegisterCTL0, "ICAP_sel", "Top") |
				makeSubfield(eRegisterCTL0, "OverTempPowerDown", "Disable") |
				makeSubfield(eRegisterCTL0, "GLUTMASK", "Dynamic") |
				makeSubfield(eRegisterCTL0, "FARSRC", "FAR") |
				makeSubfield(eRegisterCTL0, "Encrypt", "No") |
				makeSubfield(eRegisterCTL0, "Security", "None") |
				makeSubfield(eRegisterCTL0, "Persist", "No") |
				makeSubfield(eRegisterCTL0, "GTS_USER_B", "IoDisabled") |
				0x00400000 /* this bit is not defined in the configuration guide */ |
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

	VirtexPacketVector Virtex5::generateFullBitstreamSuffix(void) {
		//	003b57cb: TYPE1 WRITE CRC: 7db34bdb
		//	003b57d3: TYPE1 WRITE CMD GRESTORE
		//	003b57db: NOP x 1
		//	003b57df: TYPE1 WRITE CMD DGHIGH/LFRM
		//	003b57e7: NOP x 100
		//	003b5977: TYPE1 WRITE CMD GRESTORE
		//	003b597f: NOP x 30
		//	003b59f7: TYPE1 WRITE CMD START
		//	003b59ff: NOP x 1
		//	003b5a03: TYPE1 WRITE FAR: 00ef8000
		//	003b5a0b: TYPE1 WRITE MASK: 00400000 (ICAP_sel:Protected, OverTempPowerDown:Protected, 
		//				GLUTMASK:Protected, Encrypt:Protected, Security:Protected, 
		//				Persist:Protected, GTS_USER_B:Protected)
		//	003b5a13: TYPE1 WRITE CTL0: 00400000 (ICAP_sel:Top, OverTempPowerDown:Disable, 
		//				GLUTMASK:Masked, Encrypt:No, Security:None, Persist:No, 
		//				GTS_USER_B:IoDisabled)
		//	003b5a1b: TYPE1 WRITE CRC: 0c90449e
		//	003b5a23: TYPE1 WRITE CMD DESYNCH
		//	003b5a2b: NOP x 61

		// declare the packet vector and define a NOP packet
		VirtexPacketVector packets;
		VirtexPacket nop(VirtexPacket::makeHeader(ePacketType1, eOpcodeNOP, 0, 0));
		// write a placeholder CRC value
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCRC, 0));
		// restore command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandGRESTORE));
		packets.push_back(nop);
		// last frame command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandLFRM));
		packets.insert(packets.end(), 100, nop);
		// restore command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandGRESTORE));
		packets.insert(packets.end(), 30, nop);
		// start command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandSTART));
		packets.push_back(nop);
		// frame address register
		packets.push_back(VirtexPacket::makeType1Write(eRegisterFAR, 
			eFarMaskBlockType | eFarMaskRow)); // is this what the configuration controller wants?
		// control register 0 mask
		packets.push_back(VirtexPacket::makeType1Write(eRegisterMASK, 
				makeSubfield(eRegisterMASK, "ICAP_sel", "Protected") |
				makeSubfield(eRegisterMASK, "OverTempPowerDown", "Protected") |
				makeSubfield(eRegisterMASK, "GLUTMASK", "Protected") |
				makeSubfield(eRegisterMASK, "Encrypt", "Protected") |
				makeSubfield(eRegisterMASK, "Security", "Protected") |
				makeSubfield(eRegisterMASK, "Persist", "Protected") |
				makeSubfield(eRegisterMASK, "GTS_USER_B", "Protected") |
				0x00400000 /* this bit is not defined in the configuration guide */ |
			0));
		// control register 0
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCTL0, 
				makeSubfield(eRegisterCTL0, "ICAP_sel", "Top") |
				makeSubfield(eRegisterCTL0, "OverTempPowerDown", "Disable") |
				makeSubfield(eRegisterCTL0, "GLUTMASK", "Masked") |
				makeSubfield(eRegisterCTL0, "Encrypt", "No") |
				makeSubfield(eRegisterCTL0, "Security", "None") |
				makeSubfield(eRegisterCTL0, "Persist", "No") |
				makeSubfield(eRegisterCTL0, "GTS_USER_B", "IoDisabled") |
				0x00400000 /* this bit is not defined in the configuration guide */ |
			0));
		// write the CRC value
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCRC, 0x00000000));
		// desynch command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandDESYNCH));
		packets.insert(packets.end(), 61, nop);
		// return the packet vector
		return packets;
	}

	VirtexPacketVector Virtex5::generatePartialBitstreamPrefix(EBitstreamType inBitstreamType) {
		//	Packets marked S pertain to shutdown bitstreams only
		//		0000005f: DUMMY
		//		00000063: DUMMY
		//		00000067: DUMMY
		//		0000006b: DUMMY
		//		0000006f: DUMMY
		//		00000073: DUMMY
		//		00000077: DUMMY
		//		0000007b: DUMMY
		//		0000007f: BUS WIDTH SYNC
		//		00000083: BUS WIDTH DETECT
		//		00000087: DUMMY
		//		0000008b: DUMMY
		//		0000008f: SYNC
		//		00000093: NOP x 1
		//		00000097: TYPE1 WRITE CMD RCRC
		//		0000009f: NOP x 2
		//		000000a7: TYPE1 WRITE IDCODE: 02ad6093
		//	S	000000af: TYPE1 WRITE COR0: 00003fe5 (CRC:Enable, DONE_status:DonePin, DonePipe:No, 
		//					DriveDone:No, Capture:Continuous, ConfigRate:[UNKNOWN 0], 
		//					StartupClk:Cclk, DONE_cycle:4, Match_cycle:NoWait, LCK_cycle:NoWait, 
		//					GTS_cycle:5, GWE_cycle:6)
		//	S	000000b7: TYPE1 WRITE CMD SHUTDOWN
		//	S	000000bf: NOP x 1
		//	S	000000c3: TYPE1 WRITE CRC: e48ff1d0
		//	S	000000cb: NOP x 4
		//	S	000000db: TYPE1 WRITE CMD AGHIGH
		//	S	000000e3: NOP x 1

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
					makeSubfield(eRegisterCOR0, "CRC", "Enable") |
					makeSubfield(eRegisterCOR0, "DONE_status", "DonePin") |
					makeSubfield(eRegisterCOR0, "DonePipe", "No") |
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
		// return the packet vector
		return packets;
	}

	VirtexPacketVector Virtex5::generatePartialBitstreamSuffix(EBitstreamType inBitstreamType) {
		//	Packets marked S pertain to shutdown bitstreams only
		//	S	0000d8b7: TYPE1 WRITE CMD GRESTORE
		//	S	0000d8bf: NOP x 1
		//		0000d8c3: TYPE1 WRITE MASK: 00001000 ()
		//		0000d8cb: TYPE1 WRITE CTL1: 00000000 ()
		//		0000d8d3: TYPE1 WRITE CMD DGHIGH/LFRM
		//		0000d8db: NOP x 101
		//	S	0000da6f: TYPE1 WRITE CMD GRESTORE
		//	S	0000da77: NOP x 1
		//	S	0000da7b: TYPE1 WRITE CMD START
		//	S	0000da83: NOP x 1
		//		0000da87: TYPE1 WRITE FAR: 00ef8000
		//		0000da8f: TYPE1 WRITE CRC: 3b8df9dc
		//		0000da97: TYPE1 WRITE CMD DESYNCH
		//		0000da9f: NOP x 4

		// declare the packet vector and define a NOP packet
		VirtexPacketVector packets;
		VirtexPacket nop(VirtexPacket::makeHeader(ePacketType1, eOpcodeNOP, 0, 0));
		// extra for shutdown bitstreams
		if(inBitstreamType == eBitstreamTypePartialShutdown) {
			// restore command
			packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandGRESTORE));
			packets.push_back(nop);
		}
		// control register 1 mask
		packets.push_back(VirtexPacket::makeType1Write(eRegisterMASK, 
				0x00001000 /* this value is undocumented for CTL1 in the configuration guide */ |
			0));
		// control register 1
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCTL1, 
				0x00000000 /* this value is undocumented for CTL1 in the configuration guide */ |
			0));
		// last frame command
		packets.push_back(VirtexPacket::makeType1Write(eRegisterCMD, eCommandLFRM));
		packets.insert(packets.end(), 101, nop);
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
		if(inBitstreamType == eBitstreamTypePartialShutdown) {
			packets.insert(packets.end(), 4, nop);
		} else {
			packets.insert(packets.end(), 1, nop);
		}
		// return the packet vector
		return packets;
	}

	void Virtex5::initializeFullFrameBlocks(void) {
		boost::shared_array<uint32_t> frameWords;
		// walk the bitstream and extract all frames
		Virtex5::iterator p = begin();
		Virtex5::iterator e = end();
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

	void Virtex5::updateFullFrameBlocks(void) {
		/// \todo If the packet size differs from the frame data size, we need to replace the 
		///		entire packet.
		uint32_t frameLength = getFrameLength();
		typedef boost::shared_array<uint32_t> WordSharedArray;
		// walk the bitstream and find the (full) FDRI write packet
		Virtex5::iterator p = begin();
		Virtex5::iterator e = end();
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

	/*
	void Virtex5::initializePartialFrameBlocks (void) {
		boost::shared_array<uint32_t> frameWords;
		//Walk the bitstream and extract all FAR and FDRI 
		Virtex5::iterator p = begin();
		Virtex5::iterator e = end();
		Virtex5::FrameAddress far;
		uint32_t address;
		while(p < e) {
		    const VirtexPacket& packet = *p++;
		    if(packet.isType1() && packet.isWrite() && packet.getAddress() == eRegisterFAR) {
			    address = packet[1];
				far.assign (address);
				//std::cout << "Far: " << far << std::endl;
			}
		    if(packet.isType1() && packet.isWrite() && packet.getAddress() == eRegisterFDRI) {
  				frameWords = packet.getWords();
				//uint32_t numFrames = packet.getWordCount() % getFrameLength();
				//mFrameBlocks.mBlock[far.mBlockType].push_back(VirtexFrameSet
				//	(getFrameLength(), numFrames, frameWords, far));
			}
		}
	}
	*/

	VirtexFrameBlocks Virtex5::getBitstreamFrames(EFarTopBottom inTopBottom, uint32_t inFrameRow, 
		uint32_t inBitCol, uint32_t inBlockCount) {
		return getXdlFrames(inFrameRow + (inTopBottom == eFarBottom ? (mFrameRowCount >> 1) : 0), 
			mBitColumnToXdlColumn[inBitCol], inBlockCount);
	}

	VirtexFrameBlocks Virtex5::getBitstreamFrames(uint32_t inBitRow, uint32_t inBitCol, 
		uint32_t& outBeginBit, uint32_t& outEndBit, uint32_t inBlockCount) {
		return getXdlFrames(inBitRow, mBitColumnToXdlColumn[inBitCol], outBeginBit, outEndBit, 
			inBlockCount);
	}

	VirtexFrameBlocks Virtex5::getBitstreamFrames(uint32_t inFrameRow, uint32_t inBitCol, 
		uint32_t inBlockCount) {
		return getXdlFrames(inFrameRow, mBitColumnToXdlColumn[inBitCol], inBlockCount);
	}

	VirtexFrameBlocks Virtex5::getXdlFrames(uint32_t inXdlRow, uint32_t inXdlCol, 
		uint32_t& outBeginBit, uint32_t& outEndBit, uint32_t inBlockCount) {

		// look up the frame row designator for this XDL row
		FrameRowDesignator designator = mXdlRowToFrameRowDesignator[inXdlRow];

		// by default we have no bit position information
		outBeginBit = outEndBit = 0;
		// look up the absolute frame row
		uint32_t absoluteFrameRow = designator.mFrameRow;
		if(designator.mTopBottom == eFarBottom) absoluteFrameRow += (mFrameRowCount >> 1);

		// look up the relative XDL row
		uint32_t relativeXdlRow = inXdlRow - designator.mXdlBaseRow;
		//std::cout << "inXdlRow: " << inXdlRow << std::endl;
		//std::cout << "inXdlCol: " << inXdlCol << std::endl;
		//std::cout << "    designator.mTopBottom: " << designator.mTopBottom << std::endl;
		//std::cout << "    designator.mFrameRow: " << designator.mFrameRow << std::endl;
		//std::cout << "    designator.mXdlBaseRow: " << designator.mXdlBaseRow << std::endl;
		//std::cout << "    absoluteFrameRow: " << absoluteFrameRow << std::endl;
		//std::cout << "    relativeXdlRow: " << relativeXdlRow << std::endl;

		// provide bit information for the columns that we know
		uint32_t index = relativeXdlRow;
		//std::cout << "    index: " << index << std::endl;
		//std::cout << "    mDeviceInfo.getColumnTypes()[inXdlCol]: " 
		//	<< mDeviceInfo.getColumnTypes()[inXdlCol] << std::endl;
		switch(mDeviceInfo.getColumnTypes()[inXdlCol]) {
		case eColumnTypeClb:
		case eColumnTypeInt:
			if(index == 0 || index == 11) break; // these tiles have no bits
			index--; // tile 0 has no bits
			outBeginBit = index * 64; // regular tiles are 64 bits long
			outEndBit = outBeginBit + 64;
			if(index > 11) {
				outBeginBit -= 32; // the clock word is 32 bits long
				outEndBit -= 32;   // the clock word is 32 bits long
			}
			break;
		case eColumnTypeBram:
			if(index != 5 && index != 10 && index != 16 && index != 21) break;
			index -= 5;
			outBeginBit = index * 64; // regular tiles are 64 bits long
			outEndBit = outBeginBit + 320;
			if(index > 10) {
				outBeginBit -= 32; // the clock word is 32 bits long
				outEndBit -= 32;   // the clock word is 32 bits long
			}
			break;
		default:
			break;
		}

		// invoke the function that really does the work
		return getXdlFrames(absoluteFrameRow, inXdlCol);
	}

	VirtexFrameBlocks Virtex5::getXdlFrames(uint32_t inFrameRow, uint32_t inXdlCol, 
		uint32_t inBlockCount) {
		if(inFrameRow > getFrameRowCount()) return VirtexFrameBlocks();

		// index and extract frames
		int32_t xdlColumnIndex[inBlockCount];
		int32_t xdlColumnBound[inBlockCount];
		for(uint32_t i = 0; i < inBlockCount; i++) {
			// adjust for the frame row
			uint32_t indexOffset = inFrameRow * mXdlColumnIndexes[i].back();
			// column Index of given frame index
			xdlColumnIndex[i] = mXdlColumnIndexes[i][inXdlCol] + indexOffset;
			// frame bounds for given column type
			xdlColumnBound[i] = mColumnDefs[mDeviceInfo.getColumnTypes()[inXdlCol]][i];
			//std::cout << "    block: " << i << ", start: " << xdlColumnIndex[i] << ", width: " 
			//	<< xdlColumnBound[i] << " ==== XDL column: " << inXdlCol << ", bit column: " 
			//	<< mXdlColumnToBitColumn[inXdlCol] << std::endl;
		}
		// extract the tile frames for the specified FAR 
		VirtexFrameBlocks frameBlocks;
		for(uint32_t i = 0; i < inBlockCount; i++) {
		    int startIndex = xdlColumnIndex[i];
		    for(int j = 0; j < xdlColumnBound[i]; j++) 
				frameBlocks.mBlock[i].push_back(mFrameBlocks.mBlock[i][startIndex+j]);
		}
		return frameBlocks;
	}

	uint32_t Virtex5::getPrimaryXdlColumn(uint32_t inXdlCol) {
		// INT columns "belong" with their corresponding primary column in the bitstream, so if the 
		// caller hands us an INT column, we look for the next primary column to the right
		// if the caller passes in a primary column, we return it as is
		const ColumnTypeVector& columns = mDeviceInfo.getColumnTypes();
		if(columns[inXdlCol] != eColumnTypeInt) return inXdlCol;
		// otherwise loop and increment until we find a primary column
		uint32_t col = inXdlCol + 1;
		ColumnTypeVector::const_iterator p = columns.begin() + col;
		ColumnTypeVector::const_iterator e = columns.end();
		while(p < e) {
			if(*p != eColumnTypeEmpty) return col;
			col++; p++;
		}
		// we shouldn't get here, but if we do, the best thing is to return the original column
		return inXdlCol;
	}

} // namespace bitstream
} // namespace torc
