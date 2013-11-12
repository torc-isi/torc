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
/// \brief Unit test for the SpartanBitstream class.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/SpartanBitstream.hpp"
#include "torc/bitstream/SpartanPacket.hpp"
#include "torc/bitstream/Spartan3E.hpp"
#include "torc/bitstream/TestHelpers.hpp"
#include "torc/common/DeviceDesignator.hpp"
#include "torc/bitstream/OutputStreamHelpers.hpp"
#include "torc/common/TestHelpers.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <fstream>
#include <iostream>

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)

/// \brief Unit test for the SpartanBitstream class.
BOOST_AUTO_TEST_CASE(SpartanBitstreamCrcUnitTest) {
	// check the CRC generation
	boost::filesystem::path regressionPath = torc::common::DirectoryTree::getExecutablePath() 
		/ "torc" / "bitstream" / "Spartan3EUnitTest.reference.bit";
	BOOST_CHECK(checkCRC<Spartan3E>(regressionPath, true));
}

/// \brief Unit test for the SpartanBitstream class.
BOOST_AUTO_TEST_CASE(SpartanBitstreamUnitTest) {

	// build the file paths
	boost::filesystem::path referencePath = torc::common::DirectoryTree::getExecutablePath()
		/ "torc" / "bitstream" / "Spartan3EUnitTest.reference.bit";
	boost::filesystem::path generatedPath = torc::common::DirectoryTree::getExecutablePath()
		/ "regression" / "SpartanBitstreamUnitTest.generated.bit";

/*
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
	{
		uint32_t length = frameContents.getWordCount();
		for(uint32_t i = 1; i < length; i++) {
			if(frameContents[i] != 0) printf("buffer[0x%8.8x] = 0x%8.8x; // byte address 0x%8.8x\n", i - 1, frameContents[i], (i - 1) << 2); 
		}
	}
*/

	// declare a bitstream of unspecified architectures
	SpartanBitstream bitstream;
	// prepare the bitstream header
	bitstream.setDesignName("Spartan3EUnitTest.reference.ncd");
	bitstream.setDeviceName("3s500ecp132");
	bitstream.setDesignDate("2011/ 2/ 1");
	bitstream.setDesignTime(" 9: 6:48");

	// construct and add packets to the bitstream
	SpartanPacket dummy(Spartan3E::eSynchronizationDummy);
	SpartanPacket nop(SpartanPacket::makeHeader(Spartan3E::ePacketType1, Spartan3E::eOpcodeNOP, 
		0, 0));
	// dummy words
	bitstream.insert(bitstream.end(), 1, dummy);
	// sync
	bitstream.push_back(SpartanPacket(Spartan3E::eSynchronizationSync));
	//bitstream.push_back(nop);
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
	// kludge: the "null" reference bitstream in question isn't completely empty
	buffer[0x00000124] = 0x05500000; // byte address 0x00000490
	buffer[0x000001e3] = 0x00002a80; // byte address 0x0000078c
	buffer[0x000001e4] = 0x18000000; // byte address 0x00000790
	buffer[0x0000cbda] = 0x0000000a; // byte address 0x00032f68
	buffer[0x0000cbdb] = 0xa0000000; // byte address 0x00032f6c
	buffer[0x0000cc39] = 0x00140000; // byte address 0x000330e4
	buffer[0x0000cc3a] = 0x05000000; // byte address 0x000330e8
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
	//std::fstream fileStream2(generatedPath.string().c_str(), std::ios::binary | std::ios::in);
	//BOOST_REQUIRE(fileStream2.good());
	//Spartan3E bitstream2;
	//bitstream2.read(fileStream2, false);
	// write the bitstream digest to the console
	//std::cout << bitstream2 << std::endl;

	// compare the reference and generated XDL
	BOOST_CHECK(torc::common::fileContentsAreEqual(generatedPath, referencePath));

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc

