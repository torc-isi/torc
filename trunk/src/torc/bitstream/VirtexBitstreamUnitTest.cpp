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
/// \brief Source for the VirtexBitstream unit test.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/VirtexBitstream.hpp"
#include "torc/bitstream/Virtex5.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/architecture/DeviceDesignator.hpp"
#include "torc/bitstream/OutputStreamHelpers.hpp"
#include "torc/common/TestHelpers.hpp"
#include <fstream>
#include <iostream>

/// \todo Remove this #include
#include "torc/bitstream/Virtex.hpp"

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)


/// \brief Unit test for the VirtexBitstream class.
BOOST_AUTO_TEST_CASE(crc) {
	std::fstream fileStream("VirtexTest.bit", std::ios::binary | std::ios::in);
	Virtex bitstream;
	bitstream.read(fileStream, false);
	std::cout << bitstream << std::endl;
	bitstream.preflightPackets();
	BOOST_REQUIRE(true);
}


/// \brief Unit test for the VirtexBitstream class.
BOOST_AUTO_TEST_CASE(bitstream_virtex_bitstream) {

	// build the file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "VirtexBitstreamUnitTest.generated.bit";
	boost::filesystem::path referencePath = regressionPath / "Virtex5UnitTest.reference.bit";

	// read the reference bitstream
	std::fstream fileStream1(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream1.good());
	Virtex5 bitstream1;
	bitstream1.read(fileStream1, false);
	// find the type 2 write packet
	VirtexPacket frameContents;
	Virtex5::const_iterator p = bitstream1.begin();
	Virtex5::const_iterator e = bitstream1.end();
	while(p < e) { 
		if(p->isType2() && p->isWrite()) { frameContents = *p; break; }
		p++;
	}

	// declare a bitstream of unspecified architectures
	VirtexBitstream bitstream;
	// prepare the bitstream header
	bitstream.setDesignName("Virtex5UnitTest.reference.ncd;UserID=0xFFFFFFFF");
	bitstream.setDeviceName("5vlx20tff323");
	bitstream.setDesignDate("2010/10/08");
	bitstream.setDesignTime("15:06:11");

	// construct and add packets to the bitstream
	VirtexPacket dummy(Virtex5::eSynchronizationDummy);
	VirtexPacket nop(VirtexPacket::makeHeader(Virtex5::ePacketType1, Virtex5::eOpcodeNOP, 
		0, 0));
	// dummy words
	bitstream.insert(bitstream.end(), 8, dummy);
	// bus width detect
	bitstream.push_back(VirtexPacket(Virtex5::eSynchronizationBusWidthSync));
	bitstream.push_back(VirtexPacket(Virtex5::eSynchronizationBusWidthDetect));
	bitstream.push_back(dummy);
	bitstream.push_back(dummy);
	// sync
	bitstream.push_back(VirtexPacket(Virtex5::eSynchronizationSync));
	bitstream.push_back(nop);
	// warm boot register
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterWBSTAR, 0));
	// NULL command
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCMD, Virtex5::eCommandNULL));
	bitstream.push_back(nop);
	// reset CRC command
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCMD, Virtex5::eCommandRCRC));
	bitstream.push_back(nop);
	bitstream.push_back(nop);
	// watchdog timer
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterTIMER, 0));
	// undocumented register 19
	bitstream.push_back(VirtexPacket::makeType1Write(19, 0));
	// configuration options register 0
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCOR0, 
			Virtex5::makeSubfield(Virtex5::eRegisterCOR0, "CRC", "Enable") |
			Virtex5::makeSubfield(Virtex5::eRegisterCOR0, "DONE_status", "DonePin") |
			Virtex5::makeSubfield(Virtex5::eRegisterCOR0, "DonePipe", "No") |
			Virtex5::makeSubfield(Virtex5::eRegisterCOR0, "DriveDone", "No") |
			Virtex5::makeSubfield(Virtex5::eRegisterCOR0, "Capture", "Continuous") |
			Virtex5::makeSubfield(Virtex5::eRegisterCOR0, "ConfigRate", "[UNKNOWN 0]") |
			Virtex5::makeSubfield(Virtex5::eRegisterCOR0, "StartupClk", "Cclk") |
			Virtex5::makeSubfield(Virtex5::eRegisterCOR0, "DONE_cycle", "4") |
			Virtex5::makeSubfield(Virtex5::eRegisterCOR0, "Match_cycle", "NoWait") |
			Virtex5::makeSubfield(Virtex5::eRegisterCOR0, "LCK_cycle", "NoWait") |
			Virtex5::makeSubfield(Virtex5::eRegisterCOR0, "GTS_cycle", "5") |
			Virtex5::makeSubfield(Virtex5::eRegisterCOR0, "GWE_cycle", "6") |
		0));
	// configuration options register 1
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCOR1, 
			Virtex5::makeSubfield(Virtex5::eRegisterCOR1, "PersistDeassertAtDesynch", "Disabled") |
			Virtex5::makeSubfield(Virtex5::eRegisterCOR1, "InitAsCRCErrorPin", "Disabled") |
			Virtex5::makeSubfield(Virtex5::eRegisterCOR1, "ContinuousReadbackCRC", "Disabled") |
			Virtex5::makeSubfield(Virtex5::eRegisterCOR1, "BPI_1st_read_cycle", "1") |
			Virtex5::makeSubfield(Virtex5::eRegisterCOR1, "BPI_page_size", "1") |
		0));
	// write the ID code
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterIDCODE, 0x02a56093));
	// clock and rate switch command
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCMD, 
		Virtex5::eCommandSWITCH));
	bitstream.push_back(nop);
	// control register 0 mask
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterMASK, 
			Virtex5::makeSubfield(Virtex5::eRegisterMASK, "ICAP_sel", "Protected") |
			Virtex5::makeSubfield(Virtex5::eRegisterMASK, "OverTempPowerDown", "Protected") |
			Virtex5::makeSubfield(Virtex5::eRegisterMASK, "GLUTMASK", "Protected") |
			Virtex5::makeSubfield(Virtex5::eRegisterMASK, "Encrypt", "Protected") |
			Virtex5::makeSubfield(Virtex5::eRegisterMASK, "Security", "Protected") |
			Virtex5::makeSubfield(Virtex5::eRegisterMASK, "Persist", "Protected") |
			Virtex5::makeSubfield(Virtex5::eRegisterMASK, "GTS_USER_B", "Protected") |
			0x00400000 | // no documentation available for this subfield
		0));
	// control register 0
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCTL0, 
			Virtex5::makeSubfield(Virtex5::eRegisterCTL0, "ICAP_sel", "Top") |
			Virtex5::makeSubfield(Virtex5::eRegisterCTL0, "OverTempPowerDown", "Disable") |
			Virtex5::makeSubfield(Virtex5::eRegisterCTL0, "GLUTMASK", "Masked") |
			Virtex5::makeSubfield(Virtex5::eRegisterCTL0, "Encrypt", "No") |
			Virtex5::makeSubfield(Virtex5::eRegisterCTL0, "Security", "None") |
			Virtex5::makeSubfield(Virtex5::eRegisterCTL0, "Persist", "No") |
			Virtex5::makeSubfield(Virtex5::eRegisterCTL0, "GTS_USER_B", "IoDisabled") |
			0x00400000 | // no documentation available for this subfield
		0));
	// control register 1 mask
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterMASK, 0));
	// control register 1
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCTL1, 0));
	// more NOPs
	bitstream.insert(bitstream.end(), 8, nop);
	// frame address register
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterFAR, 0));
	// enable configuration write command
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCMD, Virtex5::eCommandWCFG));
	bitstream.push_back(nop);
	// write 0 bytes to FDRI (in preparation for type 2 write packet)
	bitstream.push_back(VirtexPacket::makeNullType1Write(Virtex5::eRegisterFDRI));
	// write 0 bytes to FDRI (in preparation for type 2 write packet)
//	// note that we do not release the buffer as it will be taken over by the packet
//	size_t length = 0x0002fa06;
//	uint32_t* buffer = new uint32_t[length];
//	uint32_t* ptr = buffer + length - 1;
//	while(ptr >= buffer) *ptr-- = 0;
//	bitstream.push_back(VirtexPacket::makeType2Write(0x0002fa06, buffer));
	bitstream.push_back(frameContents);
	// write the CRC value
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCRC, 0x2d63e9d));
	// restore command
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCMD, 
		Virtex5::eCommandGRESTORE));
	bitstream.push_back(nop);
	// last frame command
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCMD, 
		Virtex5::eCommandLFRM));
	bitstream.insert(bitstream.end(), 100, nop);
	// another restore command
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCMD, 
		Virtex5::eCommandGRESTORE));
	bitstream.insert(bitstream.end(), 30, nop);
	// start command
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCMD, 
		Virtex5::eCommandSTART));
	bitstream.push_back(nop);
	// frame address register
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterFAR, 0x00ef8000));
	// control register 0 mask
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterMASK, 
			Virtex5::makeSubfield(Virtex5::eRegisterMASK, "ICAP_sel", "Protected") |
			Virtex5::makeSubfield(Virtex5::eRegisterMASK, "OverTempPowerDown", "Protected") |
			Virtex5::makeSubfield(Virtex5::eRegisterMASK, "GLUTMASK", "Protected") |
			Virtex5::makeSubfield(Virtex5::eRegisterMASK, "Encrypt", "Protected") |
			Virtex5::makeSubfield(Virtex5::eRegisterMASK, "Security", "Protected") |
			Virtex5::makeSubfield(Virtex5::eRegisterMASK, "Persist", "Protected") |
			Virtex5::makeSubfield(Virtex5::eRegisterMASK, "GTS_USER_B", "Protected") |
			0x00400000 | // no documentation available for this subfield
		0));
	// control register 0
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCTL0, 
			Virtex5::makeSubfield(Virtex5::eRegisterCTL0, "ICAP_sel", "Top") |
			Virtex5::makeSubfield(Virtex5::eRegisterCTL0, "OverTempPowerDown", "Disable") |
			Virtex5::makeSubfield(Virtex5::eRegisterCTL0, "GLUTMASK", "Masked") |
			Virtex5::makeSubfield(Virtex5::eRegisterCTL0, "Encrypt", "No") |
			Virtex5::makeSubfield(Virtex5::eRegisterCTL0, "Security", "None") |
			Virtex5::makeSubfield(Virtex5::eRegisterCTL0, "Persist", "No") |
			Virtex5::makeSubfield(Virtex5::eRegisterCTL0, "GTS_USER_B", "IoDisabled") |
			0x00400000 | // no documentation available for this subfield
		0));
	// write the CRC value
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCRC, 0x0c90449e));
	// desynch command
	bitstream.push_back(VirtexPacket::makeType1Write(Virtex5::eRegisterCMD, 
		Virtex5::eCommandDESYNCH));
	bitstream.insert(bitstream.end(), 61, nop);

	// write the bitstream back out
	std::fstream outputStream(generatedPath.string().c_str(), std::ios::binary | std::ios::out);
	BOOST_REQUIRE(outputStream.good());
	bitstream.write(outputStream);
	outputStream.flush();

	// read the bitstream
	std::fstream fileStream2(generatedPath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream2.good());
	Virtex5 bitstream2;
	bitstream2.read(fileStream2, false);
	// write the bitstream digest to the console
	std::cout << bitstream2 << std::endl;

	// compare the reference and generated XDL
	BOOST_CHECK(torc::common::fileContentsAreEqual(generatedPath, referencePath));

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc

/*
Design Virtex5UnitTest.reference.ncd;UserID=0xFFFFFFFF (5vlx20tff323) @ 2010/10/08 15:06:11: 781400 bytes (195350 words)
    00000070: DUMMY
    00000074: DUMMY
    00000078: DUMMY
    0000007c: DUMMY
    00000080: DUMMY
    00000084: DUMMY
    00000088: DUMMY
    0000008c: DUMMY
    00000090: BUS WIDTH SYNC
    00000094: BUS WIDTH DETECT
    00000098: DUMMY
    0000009c: DUMMY
    000000a0: SYNC
    000000a4: NOP x 1
    000000a8: TYPE1 WRITE WBSTAR: 00000000 (RevisionSelectTristate:Disabled, NextRevisionSelect:00)
    000000b0: TYPE1 WRITE CMD NULL
    000000b8: NOP x 1
    000000bc: TYPE1 WRITE CMD RCRC
    000000c4: NOP x 2
    000000cc: TYPE1 WRITE TIMER: 00000000 (TimerForUser:Disabled, TimerForConfig:Disabled)
    000000d4: TYPE1 WRITE [UNKNOWN REG 19]: 00000000
    000000dc: TYPE1 WRITE COR0: 00003fe5 (CRC:Enable, DONE_status:DonePin, DonePipe:No, DriveDone:No, Capture:Continuous, ConfigRate:[UNKNOWN 0], StartupClk:Cclk, DONE_cycle:4, Match_cycle:NoWait, LCK_cycle:NoWait, GTS_cycle:5, GWE_cycle:6)
    000000e4: TYPE1 WRITE COR1: 00000000 (PersistDeassertAtDesynch:Disabled, InitAsCRCErrorPin:Disabled, ContinuousReadbackCRC:Disabled, BPI_1st_read_cycle:1, BPI_page_size:1)
    000000ec: TYPE1 WRITE IDCODE: 02a56093
    000000f4: TYPE1 WRITE CMD SWITCH
    000000fc: NOP x 1
    00000100: TYPE1 WRITE MASK: 00400000 (ICAP_sel:Protected, OverTempPowerDown:Protected, GLUTMASK:Protected, Encrypt:Protected, Security:Protected, Persist:Protected, GTS_USER_B:Protected)
    00000108: TYPE1 WRITE CTL0: 00400000 (ICAP_sel:Top, OverTempPowerDown:Disable, GLUTMASK:Masked, Encrypt:No, Security:None, Persist:No, GTS_USER_B:IoDisabled)
    00000110: TYPE1 WRITE MASK: 00000000 ()
    00000118: TYPE1 WRITE CTL1: 00000000 ()
    00000120: NOP x 8
    00000140: TYPE1 WRITE FAR: 00000000
    00000148: TYPE1 WRITE CMD WCFG
    00000150: NOP x 1
    00000154: TYPE1 WRITE FDRI: 00000000 words
    00000158: TYPE2 WRITE FDRI: 0002fa06 words
    000be974: TYPE1 WRITE CRC: 02d63e9d
    000be97c: TYPE1 WRITE CMD GRESTORE
    000be984: NOP x 1
    000be988: TYPE1 WRITE CMD DGHIGH/LFRM
    000be990: NOP x 100
    000beb20: TYPE1 WRITE CMD GRESTORE
    000beb28: NOP x 30
    000beba0: TYPE1 WRITE CMD START
    000beba8: NOP x 1
    000bebac: TYPE1 WRITE FAR: 00ef8000
    000bebb4: TYPE1 WRITE MASK: 00400000 (ICAP_sel:Protected, OverTempPowerDown:Protected, GLUTMASK:Protected, Encrypt:Protected, Security:Protected, Persist:Protected, GTS_USER_B:Protected)
    000bebbc: TYPE1 WRITE CTL0: 00400000 (ICAP_sel:Top, OverTempPowerDown:Disable, GLUTMASK:Masked, Encrypt:No, Security:None, Persist:No, GTS_USER_B:IoDisabled)
    000bebc4: TYPE1 WRITE CRC: 0c90449e
    000bebcc: TYPE1 WRITE CMD DESYNCH
    000bebd4: NOP x 61
*/
