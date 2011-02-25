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
/// \brief Source for the Spartan3E unit test.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/Spartan3E.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/DeviceDesignator.hpp"
#include "torc/bitstream/OutputStreamHelpers.hpp"
#include "torc/common/TestHelpers.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <fstream>
#include <iostream>

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)


/// \brief Unit test for the Spartan3E class.
BOOST_AUTO_TEST_CASE(bitstream_spartan3e) {

	// enums tested:
	//		EPacket
	//		EFar
	boost::uint32_t mask;
	// type 1 packet subfield masks
	mask = Spartan3E::ePacketMaskType + Spartan3E::ePacketMaskOpcode 
		+ Spartan3E::ePacketMaskType1Address + Spartan3E::ePacketMaskType1Reserved 
		+ Spartan3E::ePacketMaskType1Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// type 2 packet subfield masks
	mask = Spartan3E::ePacketMaskType + Spartan3E::ePacketMaskOpcode 
		+ Spartan3E::ePacketMaskType2Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFFFFFu);
	// frame address register subfield masks
	mask = Spartan3E::eFarMaskColumn + Spartan3E::eFarMaskMajor + Spartan3E::eFarMaskMinor;
	BOOST_CHECK_EQUAL(mask, 0x07FFFE00u);

	// members tested:
	//		Spartan3E::sPacketTypeName and EPacketTypeName
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[0],							"[UNKNOWN TYPE 0]");
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[Spartan3E::ePacketType1],		"TYPE1");
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[Spartan3E::ePacketType2],		"TYPE2");
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[3],							"[UNKNOWN TYPE 3]");
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[4],							"[UNKNOWN TYPE 4]");
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[5],							"[UNKNOWN TYPE 5]");
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[6],							"[UNKNOWN TYPE 6]");
	BOOST_CHECK_EQUAL(Spartan3E::sPacketTypeName[7],							"[UNKNOWN TYPE 7]");

	// members tested:
	//		Spartan3E::sOpcodeName and EOpcode
	BOOST_CHECK_EQUAL(Spartan3E::sOpcodeName[Spartan3E::eOpcodeNOP],			"NOP");
	BOOST_CHECK_EQUAL(Spartan3E::sOpcodeName[Spartan3E::eOpcodeRead],			"READ");
	BOOST_CHECK_EQUAL(Spartan3E::sOpcodeName[Spartan3E::eOpcodeWrite],			"WRITE");
	BOOST_CHECK_EQUAL(Spartan3E::sOpcodeName[Spartan3E::eOpcodeReserved],		"RESERVED");

	// members tested:
	//		Spartan3E::sRegisterName and ERegister
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterCRC],		"CRC");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterFAR],		"FAR");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterFDRI],		"FDRI");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterFDRO],		"FDRO");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterCMD],		"CMD");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterCTL],		"CTL");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterMASK],		"MASK");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterSTAT],		"STAT");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterLOUT],		"LOUT");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterCOR],		"COR");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterMFWR],		"MFWR");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterFLR],		"FLR");
	BOOST_CHECK_EQUAL(Spartan3E::sRegisterName[Spartan3E::eRegisterIDCODE],		"IDCODE");

	// members tested:
	//		Spartan3E::sCommandName and ECommand
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandWCFG],			"WCFG");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandMFWR],			"MFWR");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandLFRM],			"LFRM");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandRCFG],			"RCFG");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandSTART],		"START");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandRCAP],			"RCAP");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandRCRC],			"RCRC");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandAGHIGH],		"AGHIGH");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandSWITCH],		"SWITCH");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandGRESTORE],		"GRESTORE");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandSHUTDOWN],		"SHUTDOWN");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandGCAPTURE],		"GCAPTURE");
	BOOST_CHECK_EQUAL(Spartan3E::sCommandName[Spartan3E::eCommandDESYNCH],		"DESYNCH");

	// build the file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "Spartan3EUnitTest.generated.bit";
	boost::filesystem::path referencePath = regressionPath / "Spartan3EUnitTest.reference.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	Spartan3E bitstream;
	bitstream.read(fileStream, false);
	// write the bitstream digest to the console
	std::cout << bitstream << std::endl;

	std::string designName = bitstream.getDesignName();
	std::string deviceName = bitstream.getDeviceName();
	std::string designDate = bitstream.getDesignDate();
	std::string designTime = bitstream.getDesignTime();
	torc::architecture::DeviceDesignator deviceDesignator(deviceName);
	std::cout << "family of " << deviceName << " is " << deviceDesignator.getFamily() << std::endl;

	// write the bitstream back out
	std::fstream outputStream(generatedPath.string().c_str(), std::ios::binary | std::ios::out);
	BOOST_REQUIRE(outputStream.good());
	bitstream.write(outputStream);
	outputStream.flush();

	// compare the reference and generated XDL
	BOOST_CHECK(torc::common::fileContentsAreEqual(generatedPath, referencePath));
}
BOOST_AUTO_TEST_SUITE_END()

} // namespace bitstream
} // namespace torc
