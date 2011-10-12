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
/// \brief Source for the SpartanBitstream unit test.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/SpartanBitstream.hpp"
#include "torc/bitstream/SpartanPacket.hpp"
#include "torc/bitstream/Spartan3E.hpp"
#include "torc/architecture/DeviceDesignator.hpp"
#include "torc/bitstream/OutputStreamHelpers.hpp"
#include "torc/common/TestHelpers.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <fstream>
#include <iostream>

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)

/// \brief Unit test for the SpartanBitstream class.
BOOST_AUTO_TEST_CASE(crc_spartan) {
	std::fstream fileStream("Spartan3EUnitTest.reference.bit", std::ios::binary | std::ios::in);
	Spartan3E bitstream;
	bitstream.read(fileStream, false);
	std::cout << bitstream << std::endl;
	bitstream.preflightPackets();
	BOOST_REQUIRE(true);
}
/// \brief Unit test for the SpartanBitstream class.
BOOST_AUTO_TEST_CASE(bitstream_spartan_bitstream) {

	// build the file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "Spartan3EBitstreamUnitTest.generated.bit";
	boost::filesystem::path referencePath = regressionPath / "Spartan3EUnitTest.reference.bit";

	// read the reference bitstream
	std::fstream fileStream1(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream1.good());
	Spartan3E bitstream1;
	bitstream1.read(fileStream1, false);
	// find the type 2 write packet
	SpartanPacket frameContents;
	Spartan3E::const_iterator p = bitstream1.begin();
	Spartan3E::const_iterator e = bitstream1.end();
	while(p < e) { 
		if(p->isType2() && p->isWrite()) { frameContents = *p; break; }
		p++;
	}

	// declare a bitstream of unspecified architectures
	SpartanBitstream bitstream;
	// prepare the bitstream header
	bitstream.setDesignName("Spartan3EUnitTest.reference.ncd;UserID=0xFFFFFFFF");
	bitstream.setDeviceName("xc3s500e4cp132");
	bitstream.setDesignDate("2010/10/08");
	bitstream.setDesignTime("15:06:11");

	// construct and add packets to the bitstream
	SpartanPacket dummy(Spartan3E::eSynchronizationDummy);
	SpartanPacket nop(SpartanPacket::makeHeader(Spartan3E::ePacketType1, Spartan3E::eOpcodeNOP, 
		0, 0));
	// dummy words
	bitstream.insert(bitstream.end(), 1, dummy);
	// sync
	bitstream.push_back(SpartanPacket(Spartan3E::eSynchronizationSync));
	bitstream.push_back(nop);
	// reset CRC command
	bitstream.push_back(SpartanPacket::makeType1Write(Spartan3E::eRegisterCMD, Spartan3E::eCommandRCRC));
	// FLR command
	bitstream.push_back(SpartanPacket::makeType1Write(Spartan3E::eRegisterFLR, 0x00000060));
	// configuration options register 
	bitstream.push_back(SpartanPacket::makeType1Write(Spartan3E::eRegisterCOR, 
			Spartan3E::makeSubfield(Spartan3E::eRegisterCOR, "CRC", "Enable") |
			Spartan3E::makeSubfield(Spartan3E::eRegisterCOR, "DonePipe", "No") |
			Spartan3E::makeSubfield(Spartan3E::eRegisterCOR, "DriveDone", "No") |
			Spartan3E::makeSubfield(Spartan3E::eRegisterCOR, "Capture", "Continuous") |
			Spartan3E::makeSubfield(Spartan3E::eRegisterCOR, "ConfigRate", "[UNKNOWN 0]") |
			Spartan3E::makeSubfield(Spartan3E::eRegisterCOR, "StartupClk", "Cclk") |
			Spartan3E::makeSubfield(Spartan3E::eRegisterCOR, "DONE_cycle", "4") |
			Spartan3E::makeSubfield(Spartan3E::eRegisterCOR, "Match_cycle", "1") |
			Spartan3E::makeSubfield(Spartan3E::eRegisterCOR, "LCK_cycle", "NoWait") |
			Spartan3E::makeSubfield(Spartan3E::eRegisterCOR, "GTS_cycle", "5") |
			Spartan3E::makeSubfield(Spartan3E::eRegisterCOR, "GWE_cycle", "6") |
		0));
	// write the ID code
	bitstream.push_back(SpartanPacket::makeType1Write(Spartan3E::eRegisterIDCODE, 0x01c22093));
	// control register mask
	bitstream.push_back(SpartanPacket::makeType1Write(Spartan3E::eRegisterMASK, 
			Spartan3E::makeSubfield(Spartan3E::eRegisterMASK, "Security", "Protected") |
			Spartan3E::makeSubfield(Spartan3E::eRegisterMASK, "Persist", "Protected") |
			Spartan3E::makeSubfield(Spartan3E::eRegisterMASK, "GTS_USER_B", "Protected") |
			0x00000000 | // no documentation available for this subfield
		0));
	// switch command
	bitstream.push_back(SpartanPacket::makeType1Write(Spartan3E::eRegisterCMD, Spartan3E::eCommandSWITCH));
	// frame address register
	bitstream.push_back(SpartanPacket::makeType1Write(Spartan3E::eRegisterFAR, 0));
	// enable configuration write command
	bitstream.push_back(SpartanPacket::makeType1Write(Spartan3E::eRegisterCMD, Spartan3E::eCommandWCFG));
	// write 0 bytes to FDRI (in preparation for type 2 write packet)
	bitstream.push_back(SpartanPacket::makeNullType1Write(Spartan3E::eRegisterFDRI));
	// write 0x0001149a words to FDRI
	//bitstream.push_back(SpartanPacket::makeType2Write(Spartan3E::eRegisterFDRI,0x0001149a));
	// note that we do not release the buffer as it will be taken over by the packet
	size_t length = 0x0001149a;
	uint32_t* buffer = new uint32_t[length];
	uint32_t* ptr = buffer + length - 1;
	while(ptr >= buffer) *ptr-- = 0;
	bitstream.push_back(SpartanPacket::makeType2Write(0x0001149a, buffer));
	// NOP
	bitstream.push_back(nop);
	// restore command
	bitstream.push_back(SpartanPacket::makeType1Write(Spartan3E::eRegisterCMD, 
		Spartan3E::eCommandGRESTORE));
	// last frame command
	bitstream.push_back(SpartanPacket::makeType1Write(Spartan3E::eRegisterCMD, 
		Spartan3E::eCommandLFRM));
	// 97 NOPs
	bitstream.insert(bitstream.end(), 97, nop);
	// start command
	bitstream.push_back(SpartanPacket::makeType1Write(Spartan3E::eRegisterCMD, 
		Spartan3E::eCommandSTART));
	// control register 
	bitstream.push_back(SpartanPacket::makeType1Write(Spartan3E::eRegisterCTL, 
			Spartan3E::makeSubfield(Spartan3E::eRegisterCTL, "Security", "None") |
			Spartan3E::makeSubfield(Spartan3E::eRegisterCTL, "Persist", "No") |
			Spartan3E::makeSubfield(Spartan3E::eRegisterCTL, "GTS_USER_B", "IoDisabled") |
			0x00000000 | // no documentation available for this subfield
		0));
	// write the CRC value
	bitstream.push_back(SpartanPacket::makeType1Write(Spartan3E::eRegisterCRC, 0x00005f57));
	// desynch command
	bitstream.push_back(SpartanPacket::makeType1Write(Spartan3E::eRegisterCMD, 
		Spartan3E::eCommandDESYNCH));
	bitstream.insert(bitstream.end(), 4, nop);

	// write the bitstream back out
	std::fstream outputStream(generatedPath.string().c_str(), std::ios::binary | std::ios::out);
	BOOST_REQUIRE(outputStream.good());
	bitstream.write(outputStream);
	outputStream.flush();

	// read the bitstream
	std::fstream fileStream2(generatedPath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream2.good());
	Spartan3E bitstream2;
	bitstream2.read(fileStream2, false);
	// write the bitstream digest to the console
	std::cout << bitstream2 << std::endl;

	// compare the reference and generated XDL
	BOOST_CHECK(torc::common::fileContentsAreEqual(generatedPath, referencePath));

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc

