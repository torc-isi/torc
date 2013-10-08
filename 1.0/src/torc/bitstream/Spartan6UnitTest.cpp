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
/// \brief Unit test for the Spartan6 class.

#include <boost/test/unit_test.hpp>
#include "torc/bitstream/Spartan6.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/common/DeviceDesignator.hpp"
#include "torc/bitstream/OutputStreamHelpers.hpp"
#include "torc/common/TestHelpers.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <fstream>
#include <iostream>

namespace torc {
namespace bitstream {

BOOST_AUTO_TEST_SUITE(bitstream)


/// \brief Unit test for the Spartan6 class.
BOOST_AUTO_TEST_CASE(Spartan6UnitTest) {

	// enums tested:
	//		EPacket
	//		EFar
	boost::uint16_t mask;
	// type 1 packet subfield masks
	mask = Spartan6::ePacketMaskType + Spartan6::ePacketMaskOpcode 
		+ Spartan6::ePacketMaskTypeAddress + Spartan6::ePacketMaskType1Count;
	BOOST_CHECK_EQUAL(mask, 0xFFFFu);
	// type 2 packet subfield masks
	mask = Spartan6::ePacketMaskType + Spartan6::ePacketMaskOpcode 
		+ Spartan6::ePacketMaskTypeAddress;
	BOOST_CHECK_EQUAL(mask, 0xFFE0u);
	// frame major address register subfield masks
	mask = Spartan6::eFarMajMaskBlock + Spartan6::eFarMajMaskRow + Spartan6::eFarMajMaskMajor;
	BOOST_CHECK_EQUAL(mask, 0xFFFFu);
	// frame minor address register subfield masks
	mask = Spartan6::eFarMinMaskBlock + Spartan6::eFarMinMaskMinor;
	BOOST_CHECK_EQUAL(mask, 0xC1FFu);

	// members tested:
	//		Spartan6::sPacketTypeName and EPacketTypeName
	BOOST_CHECK_EQUAL(Spartan6::sPacketTypeName[0],							"[UNKNOWN TYPE 0]");
	BOOST_CHECK_EQUAL(Spartan6::sPacketTypeName[Spartan6::ePacketType1],		"TYPE1");
	BOOST_CHECK_EQUAL(Spartan6::sPacketTypeName[Spartan6::ePacketType2],		"TYPE2");
	BOOST_CHECK_EQUAL(Spartan6::sPacketTypeName[3],							"[UNKNOWN TYPE 3]");
	BOOST_CHECK_EQUAL(Spartan6::sPacketTypeName[4],							"[UNKNOWN TYPE 4]");
	BOOST_CHECK_EQUAL(Spartan6::sPacketTypeName[5],							"[UNKNOWN TYPE 5]");
	BOOST_CHECK_EQUAL(Spartan6::sPacketTypeName[6],							"[UNKNOWN TYPE 6]");
	BOOST_CHECK_EQUAL(Spartan6::sPacketTypeName[7],							"[UNKNOWN TYPE 7]");

	// members tested:
	//		Spartan6::sOpcodeName and EOpcode
	BOOST_CHECK_EQUAL(Spartan6::sOpcodeName[Spartan6::eOpcodeNOP],			"NOP");
	BOOST_CHECK_EQUAL(Spartan6::sOpcodeName[Spartan6::eOpcodeRead],			"READ");
	BOOST_CHECK_EQUAL(Spartan6::sOpcodeName[Spartan6::eOpcodeWrite],			"WRITE");
	BOOST_CHECK_EQUAL(Spartan6::sOpcodeName[Spartan6::eOpcodeReserved],		"RESERVED");

	// members tested:
	//		Spartan6::sRegisterName and ERegister
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterCRC],		"CRC");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterFARMAJ],		"FARMAJ");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterFARMIN],		"FARMIN");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterFDRI],		"FDRI");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterFDRO],		"FDRO");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterCMD],		"CMD");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterCTL],		"CTL");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterMASK],		"MASK");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterSTAT],		"STAT");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterLOUT],		"LOUT");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterCOR1],		"COR1");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterCOR2],		"COR2");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterPWRDN_REG],		"PWRDN_REG");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterFLR],		"FLR");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterIDCODE],		"IDCODE");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterCWDT],		"CWDT");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterHC_OPT_REG],		"HC_OPT_REG");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterCSBO],		"CSBO");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterGENERAL1],		"GENERAL1");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterGENERAL2],		"GENERAL2");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterGENERAL3],		"GENERAL3");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterGENERAL4],		"GENERAL4");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterGENERAL5],		"GENERAL5");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterMODE_REG],		"MODE_REG");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterPU_GWE],		"PU_GWE");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterPU_GTS],		"PU_GTS");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterMFWR],		"MFWR");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterCCLK_FREQ],		"CCLK_FREQ");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterSEU_OPT],		"SEU_OPT");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterEXP_SIGN],		"EXP_SIGN");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterRDBK_SIGN],		"RDBK_SIGN");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterBOOSTS],		"BOOSTS");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterEYE_MASK],		"EYE_MASK");
	BOOST_CHECK_EQUAL(Spartan6::sRegisterName[Spartan6::eRegisterCBC_REG],		"CBC_REG");

	// members tested:
	//		Spartan6::sCommandName and ECommand
	BOOST_CHECK_EQUAL(Spartan6::sCommandName[Spartan6::eCommandNULL],			"NULL");
	BOOST_CHECK_EQUAL(Spartan6::sCommandName[Spartan6::eCommandWCFG],			"WCFG");
	BOOST_CHECK_EQUAL(Spartan6::sCommandName[Spartan6::eCommandMFW],			"MFW");
	BOOST_CHECK_EQUAL(Spartan6::sCommandName[Spartan6::eCommandLFRM],			"LFRM");
	BOOST_CHECK_EQUAL(Spartan6::sCommandName[Spartan6::eCommandRCFG],			"RCFG");
	BOOST_CHECK_EQUAL(Spartan6::sCommandName[Spartan6::eCommandSTART],		"START");
	BOOST_CHECK_EQUAL(Spartan6::sCommandName[6],			"[UNKNOWN COMMAND 6]");
	BOOST_CHECK_EQUAL(Spartan6::sCommandName[Spartan6::eCommandRCRC],			"RCRC");
	BOOST_CHECK_EQUAL(Spartan6::sCommandName[Spartan6::eCommandAGHIGH],		"AGHIGH");
	BOOST_CHECK_EQUAL(Spartan6::sCommandName[9],		  "[UNKNOWN COMMAND 9]");
	BOOST_CHECK_EQUAL(Spartan6::sCommandName[Spartan6::eCommandGRESTORE],		"GRESTORE");
	BOOST_CHECK_EQUAL(Spartan6::sCommandName[Spartan6::eCommandSHUTDOWN],		"SHUTDOWN");
	BOOST_CHECK_EQUAL(Spartan6::sCommandName[Spartan6::eCommandDESYNC],		"DESYNC");
	BOOST_CHECK_EQUAL(Spartan6::sCommandName[Spartan6::eCommandIPROG],		"IPROG");

	// build the file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "Spartan6UnitTest.generated.bit";
	//boost::filesystem::path generatedPath = regressionPath / "xc6slx4.partial.even.bit";
	boost::filesystem::path referencePath = regressionPath / "Spartan6UnitTest.reference.bit";

	// read the bitstream
	std::fstream fileStream(referencePath.string().c_str(), std::ios::binary | std::ios::in);
	//std::fstream fileStream(generatedPath.string().c_str(), std::ios::binary | std::ios::in);
	BOOST_REQUIRE(fileStream.good());
	Spartan6 bitstream;
	bitstream.read(fileStream, false);
	// write the bitstream digest to the console
	//std::cout << bitstream << std::endl;

	std::string designName = bitstream.getDesignName();
	std::string deviceName = bitstream.getDeviceName();
	std::string designDate = bitstream.getDesignDate();
	std::string designTime = bitstream.getDesignTime();
	torc::common::DeviceDesignator deviceDesignator(deviceName);
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
