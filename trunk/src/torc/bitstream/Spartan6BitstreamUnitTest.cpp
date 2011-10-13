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
/// \brief Unit test for the SpartanBitstream class.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/Spartan6Bitstream.hpp"
#include "torc/bitstream/Spartan6Packet.hpp"
#include "torc/bitstream/Spartan6.hpp"
#include "torc/common/DeviceDesignator.hpp"
#include "torc/bitstream/OutputStreamHelpers.hpp"
#include "torc/common/TestHelpers.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <fstream>
#include <iostream>

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)

/// \brief Unit test for the Spartan6Bitstream class.
BOOST_AUTO_TEST_CASE(Spartan6BitstreamCrcUnitTest) {
	std::fstream fileStream("Spartan6UnitTest.reference.bit", std::ios::binary | std::ios::in);
	Spartan6 bitstream;
	bitstream.read(fileStream, false);
	std::cout << bitstream << std::endl;
	bitstream.preflightPackets();
	BOOST_REQUIRE(true);
}
/// \brief Unit test for the Spartan6Bitstream class.
BOOST_AUTO_TEST_CASE(Spartan6BistreamUnitTest) {

	// build the file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "Spartan6UnitTest.generated.bit";
	boost::filesystem::path referencePath = regressionPath / "Spartan6UnitTest.reference.bit";

	// read the reference bitstream
	std::fstream fileStream1(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream1.good());
	Spartan6 bitstream1;
	bitstream1.read(fileStream1, false);
	// find the type 2 write packet
	Spartan6Packet frameContents;
	Spartan6::const_iterator p = bitstream1.begin();
	Spartan6::const_iterator e = bitstream1.end();
	while(p < e) { 
		if(p->isType2() && !p->isSyncWord1() && p->isWrite()) { frameContents = *p; break; }
		p++;
	}

	// declare a bitstream of unspecified architectures
	Spartan6Bitstream bitstream;
	// prepare the bitstream header
	bitstream.setDesignName("Spartan6UnitTest.reference.ncd;UserID=0xFFFFFFFF");
	bitstream.setDeviceName("6slx25csg324");
	bitstream.setDesignDate("2011/02/09");
	bitstream.setDesignTime("13:28:37");

	// construct and add packets to the bitstream
	Spartan6Packet dummy(Spartan6::eSynchronizationDummy);
	Spartan6Packet nop(Spartan6Packet::makeHeader(Spartan6::ePacketType1, Spartan6::eOpcodeNOP, 
		0, 0));
	// dummy words
	bitstream.insert(bitstream.end(), 8, dummy);
	// sync
	bitstream.push_back(Spartan6Packet(Spartan6::eSynchronizationSync0));
	bitstream.push_back(Spartan6Packet(Spartan6::eSynchronizationSync1));
	// reset CRC command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterCMD, Spartan6::eCommandRCRC));
	// nop command
	bitstream.push_back(nop);
	// FLR command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterFLR, 0x0488));
	// configuration options register 1 
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterCOR1, 
			Spartan6::makeSubfield(Spartan6::eRegisterCOR1, "Drive_Awake", "0") |
			Spartan6::makeSubfield(Spartan6::eRegisterCOR1, "CRC_Bypass", "0") |
			Spartan6::makeSubfield(Spartan6::eRegisterCOR1, "DonePipe", "No") |
			Spartan6::makeSubfield(Spartan6::eRegisterCOR1, "DriveDone", "No") |
			Spartan6::makeSubfield(Spartan6::eRegisterCOR1, "StartupClk", "Cclk") |
		0x3d00)); // No documentation for this Value
	// configuration options register 2
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterCOR2, 
			Spartan6::makeSubfield(Spartan6::eRegisterCOR2, "Reset_on_err", "Disable") |
			Spartan6::makeSubfield(Spartan6::eRegisterCOR2, "DONE_cycle", "5") |
			Spartan6::makeSubfield(Spartan6::eRegisterCOR2, "LCK_cycle", "NoWait") |
			Spartan6::makeSubfield(Spartan6::eRegisterCOR2, "GTS_cycle", "6") |
			Spartan6::makeSubfield(Spartan6::eRegisterCOR2, "GWE_cycle", "[UNDEFINED 6]") |
		0));
	// write the ID code
	bitstream.push_back(Spartan6Packet::makeType1Write32(Spartan6::eRegisterIDCODE, 0x04004093));
	// control register mask
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterMASK, 
			Spartan6::makeSubfield(Spartan6::eRegisterMASK, "DEC", "Writable") |
			Spartan6::makeSubfield(Spartan6::eRegisterMASK, "Security", "Protected") |
			Spartan6::makeSubfield(Spartan6::eRegisterMASK, "Persist", "Writable") |
			Spartan6::makeSubfield(Spartan6::eRegisterMASK, "USE_EFUSE_KEY", "Writable") |
			Spartan6::makeSubfield(Spartan6::eRegisterMASK, "CRC_EXTSTAT_DISABLE", "Writable") |
			Spartan6::makeSubfield(Spartan6::eRegisterMASK, "RESERVED", "Writable") |
		0x80)); // No documentation for this Value
	// control register 
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterCTL, 
			Spartan6::makeSubfield(Spartan6::eRegisterCTL, "DEC", "No") |
			Spartan6::makeSubfield(Spartan6::eRegisterCTL, "Security", "None") |
			Spartan6::makeSubfield(Spartan6::eRegisterCTL, "Persist", "No") |
			Spartan6::makeSubfield(Spartan6::eRegisterCTL, "USE_EFUSE_KEY", "0") |
			Spartan6::makeSubfield(Spartan6::eRegisterCTL, "CRC_EXTSTAT_DISABLE", "0") |
		0x0081)); // No documentation for this Value
	bitstream.insert(bitstream.end(), 17, nop);
	// SEU_OPT register 
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterSEU_OPT, 
			Spartan6::makeSubfield(Spartan6::eRegisterSEU_OPT, "SEU_FREQ", "0x01be") |
			Spartan6::makeSubfield(Spartan6::eRegisterSEU_OPT, "SEU_RUN_ON_ERR", "Run") |
			Spartan6::makeSubfield(Spartan6::eRegisterSEU_OPT, "GLUT_MASK", "Unmask") |
			Spartan6::makeSubfield(Spartan6::eRegisterSEU_OPT, "SEU_ENABLE", "DISABLE") |
		0x3cc0)); // No documentation for this Value
	// PWRDN_REG register 
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterPWRDN_REG, 
			Spartan6::makeSubfield(Spartan6::eRegisterPWRDN_REG, "EN_EYES", "Disable") |
			Spartan6::makeSubfield(Spartan6::eRegisterPWRDN_REG, "FILTER", "0") |
			Spartan6::makeSubfield(Spartan6::eRegisterPWRDN_REG, "EN_PGSR", "No") |
			Spartan6::makeSubfield(Spartan6::eRegisterPWRDN_REG, "EN_PWRDN", "0") |
			Spartan6::makeSubfield(Spartan6::eRegisterPWRDN_REG, "KEEP_SCLK", "1") |
		0x0880)); // No documentation for this Value
	// CBC write command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterCBC_REG, 0));
	// HC_OPT_REG register 
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterHC_OPT_REG, 
			Spartan6::makeSubfield(Spartan6::eRegisterHC_OPT_REG, "INIT", "0") |
		0x1f)); // No documentation for this Value
	// CWDT write command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterCWDT, 0xffff));
	// PU_GTS write command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterPU_GTS, 0x0005));
	// MFWR write command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterMFWR, 0x0004));
	// PU_GWE write command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterPU_GWE, 0x0100));
	// GENERAL2 write command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterGENERAL2, 0x0000));
	// GENERAL3 write command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterGENERAL3, 0x0000));
	// GENERAL4 write command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterGENERAL4, 0x0000));
	// GENERAL5 write command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterGENERAL5, 0x0000));
	// MODE_REG register 
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterMODE_REG, 
			Spartan6::makeSubfield(Spartan6::eRegisterMODE_REG, "NEW_MODE", "No") |
			Spartan6::makeSubfield(Spartan6::eRegisterMODE_REG, "BUSWIDTH", "0") |
			Spartan6::makeSubfield(Spartan6::eRegisterMODE_REG, "BOOTMODE", "0") |
			Spartan6::makeSubfield(Spartan6::eRegisterMODE_REG, "BOOTVSEL", "[READ ONLY]") |
		0));
	// EXP_SIGN write command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterEXP_SIGN, 0x1be2));
	// EXP_SIGN write command
	bitstream.push_back(Spartan6Packet::makeType1Write32(Spartan6::eRegisterRDBK_SIGN, 0x00000000));
	// nop command
	bitstream.push_back(nop);
	bitstream.push_back(nop);
	// frame address register
	bitstream.push_back(Spartan6Packet::makeType1Write32(Spartan6::eRegisterFARMAJ, 0x00000000));
	// enable configuration write command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterCMD, Spartan6::eCommandWCFG));
	// write 0 bytes to FDRI (in preparation for type 2 write packet)
	//bitstream.push_back(Spartan6Packet::makeNullType2Write(Spartan6::eRegisterFDRI));
	// note that we do not release the buffer as it will be taken over by the packet
	//size_t length = 0x00061cca;
	//uint16_t* buffer = new uint16_t[length];
	//uint16_t* ptr = buffer + length - 1;
	//while(ptr >= buffer) *ptr-- = 0;
  //*(buffer + 0) = 0xabcd;
	//*(buffer + length - 1) = 0x0123;
	//bitstream.push_back(Spartan6Packet::makeType2Write(0x00061cca, Spartan6::eRegisterFDRI, buffer));
	bitstream.push_back(frameContents);
	// CRC Word? 
	uint16_t crcEndWord = 0x0015;
	bitstream.push_back(Spartan6Packet(crcEndWord));
	// CWDT write command
	size_t length = 15;
	uint16_t* buffer = new uint16_t[length];
	uint16_t* ptr = buffer + length - 1;
	while(ptr >= buffer) *ptr-- = 0x2000;
	bitstream.push_back(Spartan6Packet::makeType1Write(length, Spartan6::eRegisterCWDT, buffer));
	// 9 NOPs
	bitstream.insert(bitstream.end(), 9, nop);
	// restore command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterCMD, 
		Spartan6::eCommandGRESTORE));
	// last frame command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterCMD, 
		Spartan6::eCommandLFRM));
	// 4 NOPs
	bitstream.insert(bitstream.end(), 4, nop);
	// restore command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterCMD, 
		Spartan6::eCommandGRESTORE));
	// start command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterCMD, 
		Spartan6::eCommandSTART));
	// control register mask
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterMASK, 
			Spartan6::makeSubfield(Spartan6::eRegisterMASK, "DEC", "Writable") |
			Spartan6::makeSubfield(Spartan6::eRegisterMASK, "Security", "Writable") |
			Spartan6::makeSubfield(Spartan6::eRegisterMASK, "Persist", "Writable") |
			Spartan6::makeSubfield(Spartan6::eRegisterMASK, "USE_EFUSE_KEY", "Writable") |
			Spartan6::makeSubfield(Spartan6::eRegisterMASK, "CRC_EXTSTAT_DISABLE", "Writable") |
			Spartan6::makeSubfield(Spartan6::eRegisterMASK, "RESERVED", "Writable") |
		0x80)); // No documentation for this Value
	// control register 
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterCTL, 
			Spartan6::makeSubfield(Spartan6::eRegisterCTL, "DEC", "No") |
			Spartan6::makeSubfield(Spartan6::eRegisterCTL, "Security", "None") |
			Spartan6::makeSubfield(Spartan6::eRegisterCTL, "Persist", "No") |
			Spartan6::makeSubfield(Spartan6::eRegisterCTL, "USE_EFUSE_KEY", "0") |
			Spartan6::makeSubfield(Spartan6::eRegisterCTL, "CRC_EXTSTAT_DISABLE", "0") |
		0x81)); // No documentation for this Value
	// write the CRC value
	bitstream.push_back(Spartan6Packet::makeType1Write32(Spartan6::eRegisterCRC, 0x0002109d));
	// desync command
	bitstream.push_back(Spartan6Packet::makeType1Write(Spartan6::eRegisterCMD, 
		Spartan6::eCommandDESYNC));
	bitstream.insert(bitstream.end(), 14, nop);

	// write the bitstream back out
	std::fstream outputStream(generatedPath.string().c_str(), std::ios::binary | std::ios::out);
	BOOST_REQUIRE(outputStream.good());
	bitstream.write(outputStream);
	outputStream.flush();

	// read the bitstream
	std::fstream fileStream2(generatedPath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream2.good());
	Spartan6 bitstream2;
	bitstream2.read(fileStream2, false);
	// write the bitstream digest to the console
	std::cout << bitstream2 << std::endl;
	std::cerr << "TYPEOF BITSTREAM: " << typeid(bitstream2).name() << std::endl;

	// compare the reference and generated XDL
	//BOOST_CHECK(fileContentsAreEqual(generatedPath, referencePath));

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc

