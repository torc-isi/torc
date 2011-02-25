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
/// \brief Regression test for XDL Design class.

#include <boost/test/unit_test.hpp>
#include <boost/filesystem/convenience.hpp>
#include "torc/common/DirectoryTree.hpp"
#include "torc/physical/Factory.hpp"
#include "torc/physical/XdlExporter.hpp"
#include "torc/physical/OutputStreamHelpers.hpp"
#include "torc/common/TestHelpers.hpp"
#include <fstream>

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical_database)

/// \brief Basic regression test for the entire XDL infrastructure.
/// \details Note: This is neither a pleasant nor recommended way to create a design.
BOOST_AUTO_TEST_CASE(physical_xdl_regression) {

	// determine whether or not to output additional debugging information; this serves primarily 
	// to show the user how certain design items can be accessed
	bool debug = false;

	// create a design
	DesignSharedPtr designPtr = Factory::newDesignPtr("blinker", "xc5vlx30", "ff324", "-1", "v3.2");
	designPtr->setConfig("_DESIGN_PROP", "P3_PLACE_OPTIONS", "EFFORT_LEVEL:high");
	designPtr->setConfig("_DESIGN_PROP", "", "P3_PLACED:");
	designPtr->setConfig("_DESIGN_PROP", "", "P3_PLACE_OPTIONS:");
	designPtr->setConfig("_DESIGN_PROP", "", "PK_NGMTIMESTAMP:1278102371");
	BOOST_CHECK_EQUAL(designPtr->getConfigCount(), 4u);
	if(debug) {
		ConfigMap::const_iterator p = designPtr->configBegin();
		ConfigMap::const_iterator e = designPtr->configEnd();
		while(p != e) {
			const std::string& setting = p->first;
			const Config& config = p->second;
			std::clog << "\t" << setting << ":" << config.getName() << ":" << config.getValue() 
				<< std::endl;
			p++;
		}
	}

	// create the modules (this design has no modules)
	// verify the expected module count
	BOOST_CHECK_EQUAL(designPtr->getModuleCount(), 0u);
	if(debug) {
		Design::ModuleSharedPtrConstIterator p = designPtr->modulesBegin();
		Design::ModuleSharedPtrConstIterator e = designPtr->modulesEnd();
		while(p != e) {
			const ModuleSharedPtr& modulePtr = *p++;
			std::clog << "\tmodule " << modulePtr->getName() << std::endl;
		}
	}

	// create the instances
	// instance clk
	InstanceSharedPtr clkInstPtr 
		= Factory::newInstancePtr("clk", "IOB", "CIOB_X17Y61", "B6", eInstanceBondingBonded);
	clkInstPtr->setConfig("IMUX", "", "I");
	clkInstPtr->setConfig("INBUF", "clk_ibuf/IBUFG", "");
	clkInstPtr->setConfig("PAD", "clk", "");
	clkInstPtr->setConfig("ISTANDARD", "", "LVCMOS25");
	designPtr->addInstance(clkInstPtr);
	// instance reset
	InstanceSharedPtr resetInstPtr 
		= Factory::newInstancePtr("reset", "IOB", "CIOB_X17Y59", "F11", eInstanceBondingBonded);
	resetInstPtr->setConfig("IMUX", "", "I");
	resetInstPtr->setConfig("INBUF", "reset_ibuf", "");
	resetInstPtr->setConfig("PAD", "reset", "");
	resetInstPtr->setConfig("ISTANDARD", "", "LVCMOS25");
	designPtr->addInstance(resetInstPtr);
	// instance out
	InstanceSharedPtr outInstPtr 
		= Factory::newInstancePtr("out", "IOB", "CIOB_X17Y59", "G11", eInstanceBondingBonded);
	outInstPtr->setConfig("OUSED", "", "0");
	outInstPtr->setConfig("OUTBUF", "out_obuf", "");
	outInstPtr->setConfig("PAD", "out", "");
	outInstPtr->setConfig("DRIVE", "", "12");
	outInstPtr->setConfig("OSTANDARD", "", "LVCMOS25");
	outInstPtr->setConfig("SLEW", "", "SLOW");
	designPtr->addInstance(outInstPtr);
	// instance clk_ibuf/BUFG
	InstanceSharedPtr clkIbufInstPtr 
		= Factory::newInstancePtr("clk_ibuf/BUFG", "BUFG", "CLK_BUFGMUX_X47Y44", "BUFGCTRL_X0Y30");
	clkIbufInstPtr->setConfig("BUFG", "clk_ibuf/BUFG", "");
	designPtr->addInstance(clkIbufInstPtr);
	// instance blink
	InstanceSharedPtr blinkInstPtr 
		= Factory::newInstancePtr("blink", "SLICEL", "CLBLL_X16Y59", "SLICE_X27Y59");
	blinkInstPtr->setConfig("CLKINV", "", "CLK");
	blinkInstPtr->setConfig("D6LUT", "blink_i", "#LUT:O6=~A6");
	blinkInstPtr->setConfig("DFF", "blink", "#FF");
	blinkInstPtr->setConfig("DFFINIT", "", "INIT0");
	blinkInstPtr->setConfig("DFFMUX", "", "O6");
	blinkInstPtr->setConfig("DFFSR", "", "SRLOW");
	blinkInstPtr->setConfig("SRUSED", "", "0");
	blinkInstPtr->setConfig("SYNC_ATTR", "", "SYNC");
	designPtr->addInstance(blinkInstPtr);
	// instance XDL_DUMMY_IOI_X17Y59_OLOGIC_X1Y118
	InstanceSharedPtr OLOGIC_X1Y118InstPtr 
		= Factory::newInstancePtr("XDL_DUMMY_IOI_X17Y59_OLOGIC_X1Y118", "OLOGIC", "IOI_X17Y59", 
		"OLOGIC_X1Y118");
	OLOGIC_X1Y118InstPtr->setConfig("_NO_USER_LOGIC", "", "");
	OLOGIC_X1Y118InstPtr->setConfig("_ROUTETHROUGH", "D1", "OQ");
	designPtr->addInstance(OLOGIC_X1Y118InstPtr);
	// instance XDL_DUMMY_IOI_X17Y59_ILOGIC_X1Y119
	InstanceSharedPtr ILOGIC_X1Y119InstPtr 
		= Factory::newInstancePtr("XDL_DUMMY_IOI_X17Y59_ILOGIC_X1Y119", "ILOGIC", "IOI_X17Y59", 
		"ILOGIC_X1Y119");
	ILOGIC_X1Y119InstPtr->setConfig("_NO_USER_LOGIC", "", "");
	ILOGIC_X1Y119InstPtr->setConfig("_ROUTETHROUGH", "D", "O");
	designPtr->addInstance(ILOGIC_X1Y119InstPtr);
	// instance XDL_DUMMY_IOI_X17Y61_ILOGIC_X1Y123
	InstanceSharedPtr ILOGIC_X1Y123InstPtr 
		= Factory::newInstancePtr("XDL_DUMMY_IOI_X17Y61_ILOGIC_X1Y123", "ILOGIC", "IOI_X17Y61", 
		"ILOGIC_X1Y123");
	ILOGIC_X1Y123InstPtr->setConfig("_NO_USER_LOGIC", "", "");
	ILOGIC_X1Y123InstPtr->setConfig("_ROUTETHROUGH", "D", "O");
	designPtr->addInstance(ILOGIC_X1Y123InstPtr);
	// verify that expected instance count
	BOOST_CHECK_EQUAL(designPtr->getInstanceCount(), 8u);
	if(debug) {
		Design::InstanceSharedPtrConstIterator p = designPtr->instancesBegin();
		Design::InstanceSharedPtrConstIterator e = designPtr->instancesEnd();
		while(p != e) {
			const InstanceSharedPtr& instancePtr = *p++;
			std::clog << "\tinstance " << instancePtr->getName() << std::endl;
		}
	}

	// create the nets
	// net blink
	NetSharedPtr blinkNetPtr = Factory::newNetPtr("blink");
	InstancePinSharedPtr blinkNetPin1Ptr = Factory::newInstancePinPtr(blinkInstPtr, "DQ");
	blinkNetPtr->addSource(blinkNetPin1Ptr);
	InstancePinSharedPtr blinkNetPin2Ptr = Factory::newInstancePinPtr(blinkInstPtr, "D6");
	blinkNetPtr->addSink(blinkNetPin2Ptr);
	InstancePinSharedPtr blinkNetPin3Ptr = Factory::newInstancePinPtr(outInstPtr, "O");
	blinkNetPtr->addSink(blinkNetPin3Ptr);
	blinkNetPtr->addPip(Factory::newPip("CLBLL_X16Y59", "L_DQ", "SITE_LOGIC_OUTS3", 
		ePipBidirectionalUnidirectionallyBuffered));
	blinkNetPtr->addPip(Factory::newPip("CLBLL_X16Y59", "SITE_IMUX_B47", "L_D6", 
		ePipBidirectionalUnidirectionallyBuffered));
	blinkNetPtr->addPip(Factory::newPip("INT_X16Y59", "EL2BEG2", "IMUX_B47", 
		ePipBidirectionalUnidirectionallyBuffered));
	blinkNetPtr->addPip(Factory::newPip("INT_X16Y59", "LOGIC_OUTS3", "EL2BEG2", 
		ePipBidirectionalUnidirectionallyBuffered));
	blinkNetPtr->addPip(Factory::newPip("INT_X17Y59", "EL2MID2", "IMUX_B41", 
		ePipBidirectionalUnidirectionallyBuffered));
	blinkNetPtr->addPip(Factory::newPip("IOI_X17Y59", "IOI_IMUX_B41", "IOI_O11", 
		ePipBidirectionalUnidirectionallyBuffered));
	blinkNetPtr->addPip(Factory::newPip("IOI_X17Y59", "IOI_O11", "IOI_O_PINWIRE1", 
		ePipBidirectionalUnidirectionallyBuffered, Factory::newRoutethroughPtr("_ROUTETHROUGH", 
		"D1", "OQ", OLOGIC_X1Y118InstPtr, "D1", "OQ")));
	blinkNetPtr->addPip(Factory::newPip("IOI_X17Y59", "IOI_O_PINWIRE1", "IOI_O1", 
		ePipBidirectionalUnidirectionallyBuffered));
	designPtr->addNet(blinkNetPtr);
	// net clk
	NetSharedPtr clkNetPtr = Factory::newNetPtr("clk");
	clkNetPtr->setConfig("_BELSIG", "PAD,PAD,clk", "clk");
	designPtr->addNet(clkNetPtr);
	// net clk_c
	NetSharedPtr clkCNetPtr = Factory::newNetPtr("clk_c");
	InstancePinSharedPtr clkCNetPin1Ptr = Factory::newInstancePinPtr(clkIbufInstPtr, "O");
	clkCNetPtr->addSource(clkCNetPin1Ptr);
	InstancePinSharedPtr clkCNetPin2Ptr = Factory::newInstancePinPtr(blinkInstPtr, "CLK");
	clkCNetPtr->addSink(clkCNetPin2Ptr);
	clkCNetPtr->addPip(Factory::newPip("CLBLL_X16Y59", "SITE_CLK_B0", "L_CLK", 
		ePipBidirectionalUnidirectionallyBuffered));
	clkCNetPtr->addPip(Factory::newPip("CLK_BUFGMUX_X47Y44", "CLK_BUFGMUX_POSTMUX_GCLKP30", 
		"CLK_BUFGMUX_GCLKP30", ePipBidirectionalUnidirectionallyBuffered));
	clkCNetPtr->addPip(Factory::newPip("CLK_HROW_X17Y49", "CLK_HROW_GCLK_BUF30", 
		"CLK_HROW_HCLKL_P5", ePipBidirectionalUnidirectionallyBuffered));
	clkCNetPtr->addPip(Factory::newPip("HCLK_X16Y49", "HCLK_G_HCLK_P5", "HCLK_LEAF_GCLK5", 
		ePipBidirectionalUnidirectionallyBuffered));
	clkCNetPtr->addPip(Factory::newPip("INT_X16Y59", "GCLK5", "CLK_B0", 
		ePipBidirectionalUnidirectionallyBuffered));
	designPtr->addNet(clkCNetPtr);
	// net clk_ibuf/IBUFG
	NetSharedPtr clkIibufIbufgNetPtr = Factory::newNetPtr("clk_ibuf/IBUFG");
	InstancePinSharedPtr clkIibufIbufgNetPin1Ptr = Factory::newInstancePinPtr(clkInstPtr, "I");
	clkIibufIbufgNetPtr->addSource(clkIibufIbufgNetPin1Ptr);
	InstancePinSharedPtr clkIibufIbufgNetPin2Ptr = Factory::newInstancePinPtr(clkIbufInstPtr, "I0");
	clkIibufIbufgNetPtr->addSink(clkIibufIbufgNetPin2Ptr);
	clkIibufIbufgNetPtr->addPip(Factory::newPip("CLK_BUFGMUX_X47Y44", 
		"CLK_BUFGMUX_MUXED_IN_CLKT_P28", "CLK_BUFGMUX_PREMUX0_CLK30", 
		ePipBidirectionalUnidirectionallyBuffered));
	clkIibufIbufgNetPtr->addPip(Factory::newPip("CLK_BUFGMUX_X47Y44", "CLK_BUFGMUX_PREMUX0_CLK30", 
		"CLK_BUFGMUX_CLKP0_30", ePipBidirectionalUnidirectionallyBuffered));
	clkIibufIbufgNetPtr->addPip(Factory::newPip("CLK_IOB_T_X17Y60", "CLK_IOB_CLK_BUF1", 
		"CLK_IOB_MUXED_CLKOUT28", ePipBidirectionalUnidirectionallyBuffered));
	clkIibufIbufgNetPtr->addPip(Factory::newPip("CLK_IOB_T_X17Y60", "CLK_IOB_PAD_CLK1", 
		"CLK_IOB_CLK_BUF1", ePipBidirectionalUnidirectionallyBuffered));
	clkIibufIbufgNetPtr->addPip(Factory::newPip("IOI_X17Y61", "IOI_D0", "IOI_I0", 
		ePipBidirectionalUnidirectionallyBuffered, Factory::newRoutethroughPtr("_ROUTETHROUGH", 
		"D", "O", ILOGIC_X1Y123InstPtr, "D", "O")));
	clkIibufIbufgNetPtr->addPip(Factory::newPip("IOI_X17Y61", "IOI_I0", "IOI_I_2GCLK0", 
		ePipBidirectionalUnidirectionallyBuffered));
	clkIibufIbufgNetPtr->addPip(Factory::newPip("IOI_X17Y61", "IOI_IBUF0", "IOI_D0", 
		ePipBidirectionalUnidirectionallyBuffered));
	designPtr->addNet(clkIibufIbufgNetPtr);
	// net out
	NetSharedPtr outNetPtr = Factory::newNetPtr("out");
	outNetPtr->setConfig("_BELSIG", "PAD,PAD,out", "out");
	designPtr->addNet(outNetPtr);
	// net reset
	NetSharedPtr resetNetPtr = Factory::newNetPtr("reset");
	resetNetPtr->setConfig("_BELSIG", "PAD,PAD,reset", "reset");
	designPtr->addNet(resetNetPtr);
	// net reset_c
	NetSharedPtr resetCNetPtr = Factory::newNetPtr("reset_c");
	InstancePinSharedPtr resetCNetPin1Ptr = Factory::newInstancePinPtr(resetInstPtr, "I");
	resetCNetPtr->addSource(resetCNetPin1Ptr);
	InstancePinSharedPtr resetCNetPin2Ptr = Factory::newInstancePinPtr(blinkInstPtr, "SR");
	resetCNetPtr->addSink(resetCNetPin2Ptr);
	resetCNetPtr->addPip(Factory::newPip("CLBLL_X16Y59", "SITE_CTRL_B2", "L_SR", 
		ePipBidirectionalUnidirectionallyBuffered));
	resetCNetPtr->addPip(Factory::newPip("INT_INTERFACE_X17Y59", "INT_INTERFACE_LOGIC_OUTS_B11", 
		"INT_INTERFACE_LOGIC_OUTS11", ePipBidirectionalUnidirectionallyBuffered));
	resetCNetPtr->addPip(Factory::newPip("INT_X16Y59", "CTRL2", "CTRL_B2", 
		ePipBidirectionalUnidirectionallyBuffered));
	resetCNetPtr->addPip(Factory::newPip("INT_X16Y59", "WS2MID2", "CTRL2", 
		ePipBidirectionalUnidirectionallyBuffered));
	resetCNetPtr->addPip(Factory::newPip("INT_X17Y59", "LOGIC_OUTS11", "WS2BEG2", 
		ePipBidirectionalUnidirectionallyBuffered));
	resetCNetPtr->addPip(Factory::newPip("IOI_X17Y59", "IOI_D0", "IOI_I0", 
		ePipBidirectionalUnidirectionallyBuffered, Factory::newRoutethroughPtr("_ROUTETHROUGH", 
		"D", "O", ILOGIC_X1Y119InstPtr, "D", "O")));
	resetCNetPtr->addPip(Factory::newPip("IOI_X17Y59", "IOI_I0", "IOI_LOGIC_OUTS11", 
		ePipBidirectionalUnidirectionallyBuffered));
	resetCNetPtr->addPip(Factory::newPip("IOI_X17Y59", "IOI_IBUF0", "IOI_D0", 
		ePipBidirectionalUnidirectionallyBuffered));
	designPtr->addNet(resetCNetPtr);
	// verify the expected net count
	BOOST_CHECK_EQUAL(designPtr->getNetCount(), 7u);
	if(debug) {
		Design::NetSharedPtrConstIterator p = designPtr->netsBegin();
		Design::NetSharedPtrConstIterator e = designPtr->netsEnd();
		while(p != e) {
			const NetSharedPtr& netPtr = *p++;
			std::clog << "\tnet " << netPtr->getName() << std::endl;
		}
	}

	// create the appropriate file paths
	boost::filesystem::path regressionPath 
		= torc::common::DirectoryTree::getExecutablePath() / "regression";
	boost::filesystem::path generatedPath = regressionPath / "DesignUnitTest.generated.xdl";
	boost::filesystem::path referencePath = regressionPath / "DesignUnitTest.reference.xdl";
	// export the created design
	std::fstream xdlExport(generatedPath.string().c_str(), std::ios_base::out);
	XdlExporter fileExporter(xdlExport);
	fileExporter(designPtr);

	// compare the reference and generated XDL
	BOOST_CHECK(torc::common::fileContentsAreEqual(generatedPath, referencePath));

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc

#if 0
/*

# =======================================================
# XDL NCD CONVERSION MODE $Revision: 1.01$
# time: Fri Jul  2 18:17:55 2010

# =======================================================


# =======================================================
# The syntax for the design statement is:                
# design <design_name> <part> <ncd version>;             
# or                                                     
# design <design_name> <device> <package> <speed> <ncd_version>
# =======================================================
design "blinker" xc5vlx30ff324-1 v3.2 ,
  cfg "
       _DESIGN_PROP:P3_PLACE_OPTIONS:EFFORT_LEVEL:high
       _DESIGN_PROP::P3_PLACED:
       _DESIGN_PROP::P3_PLACE_OPTIONS:
       _DESIGN_PROP::PK_NGMTIMESTAMP:1278102371";


#  =======================================================
#  The syntax for instances is:
#      instance <name> <sitedef>, placed <tile> <site>, cfg <string> ;
#  or
#      instance <name> <sitedef>, unplaced, cfg <string> ;
# 
#  For typing convenience you can abbreviate instance to inst.
# 
#  For IOs there are two special keywords: bonded and unbonded
#  that can be used to designate whether the PAD of an unplaced IO is
#  bonded out. If neither keyword is specified, bonded is assumed.
# 
#  The bonding of placed IOs is determined by the site they are placed in.
# 
#  If you specify bonded or unbonded for an instance that is not an
#  IOB it is ignored.
# 
#  Shown below are three examples for IOs. 
#     instance IO1 IOB, unplaced ;          # This will be bonded
#     instance IO1 IOB, unplaced bonded ;   # This will be bonded
#     instance IO1 IOB, unplaced unbonded ; # This will be unbonded
#  =======================================================
inst "clk" "IOB",placed CIOB_X17Y61 B6  ,
  cfg " DIFFI_INUSED::#OFF DIFF_TERM::#OFF IMUX::I OUSED::#OFF PADOUTUSED::#OFF
       PULLTYPE::#OFF TUSED::#OFF INBUF:clk_ibuf/IBUFG: PAD:clk:
         ISTANDARD::LVCMOS25 "
  ;
inst "reset" "IOB",placed CIOB_X17Y59 F11  ,
  cfg " DIFFI_INUSED::#OFF DIFF_TERM::#OFF IMUX::I OUSED::#OFF PADOUTUSED::#OFF
       PULLTYPE::#OFF TUSED::#OFF INBUF:reset_ibuf: PAD:reset:
         ISTANDARD::LVCMOS25 "
  ;
inst "out" "IOB",placed CIOB_X17Y59 G11  ,
  cfg " DIFFI_INUSED::#OFF DIFF_TERM::#OFF IMUX::#OFF OUSED::0 PADOUTUSED::#OFF
       PULLTYPE::#OFF TUSED::#OFF OUTBUF:out_obuf: PAD:out:
         DRIVE::12  OSTANDARD::LVCMOS25  SLEW::SLOW "
  ;
inst "clk_ibuf/BUFG" "BUFG",placed CLK_BUFGMUX_X47Y44 BUFGCTRL_X0Y30  ,
  cfg " BUFG:clk_ibuf/BUFG: "
  ;
inst "blink" "SLICEL",placed CLBLL_X16Y59 SLICE_X27Y59  ,
  cfg " A5LUT::#OFF A6LUT::#OFF ACY0::#OFF AFF::#OFF AFFINIT::#OFF AFFMUX::#OFF
       AFFSR::#OFF AOUTMUX::#OFF AUSED::#OFF B5LUT::#OFF B6LUT::#OFF BCY0::#OFF
       BFF::#OFF BFFINIT::#OFF BFFMUX::#OFF BFFSR::#OFF BOUTMUX::#OFF BUSED::#OFF
       C5LUT::#OFF C6LUT::#OFF CCY0::#OFF CEUSED::#OFF CFF::#OFF CFFINIT::#OFF
       CFFMUX::#OFF CFFSR::#OFF CLKINV::CLK COUTMUX::#OFF COUTUSED::#OFF
       CUSED::#OFF D5LUT::#OFF D6LUT:blink_i:#LUT:O6=~A6 DCY0::#OFF DFF:blink:#FF
       DFFINIT::INIT0 DFFMUX::O6 DFFSR::SRLOW DOUTMUX::#OFF DUSED::#OFF
       PRECYINIT::#OFF REVUSED::#OFF SRUSED::0 SYNC_ATTR::SYNC "
  ;
inst "XDL_DUMMY_IOI_X17Y59_OLOGIC_X1Y118" "OLOGIC",placed IOI_X17Y59 OLOGIC_X1Y118  ,
  cfg "_NO_USER_LOGIC::  _ROUTETHROUGH:D1:OQ "
  ;
inst "XDL_DUMMY_IOI_X17Y59_ILOGIC_X1Y119" "ILOGIC",placed IOI_X17Y59 ILOGIC_X1Y119  ,
  cfg "_NO_USER_LOGIC::  _ROUTETHROUGH:D:O "
  ;
inst "XDL_DUMMY_IOI_X17Y61_ILOGIC_X1Y123" "ILOGIC",placed IOI_X17Y61 ILOGIC_X1Y123  ,
  cfg "_NO_USER_LOGIC::  _ROUTETHROUGH:D:O "
  ;

#  ================================================
#  The syntax for nets is:
#     net <name> <type>,
#       outpin <inst_name> <inst_pin>,
#       .
#       .
#       inpin <inst_name> <inst_pin>,
#       .
#       .
#       pip <tile> <wire0> <dir> <wire1> , # [<rt>]
#       .
#       .
#       ;
# 
#  There are three available wire types: wire, power and ground.
#  If no type is specified, wire is assumed.
# 
#  Wire indicates that this a normal wire.
#  Power indicates that this net is tied to a DC power source.
#  You can use "power", "vcc" or "vdd" to specify a power net.
# 
#  Ground indicates that this net is tied to ground.
#  You can use "ground", or "gnd" to specify a ground net.
# 
#  The <dir> token will be one of the following:
# 
#     Symbol Description
#     ====== ==========================================
#       ==   Bidirectional, unbuffered.
#       =>   Bidirectional, buffered in one direction.
#       =-   Bidirectional, buffered in both directions.
#       ->   Directional, buffered.
# 
#  No pips exist for unrouted nets.
#  ================================================
net "blink" , 
  outpin "blink" DQ ,
  inpin "blink" D6 ,
  inpin "out" O ,
  pip CLBLL_X16Y59 L_DQ -> SITE_LOGIC_OUTS3 , 
  pip CLBLL_X16Y59 SITE_IMUX_B47 -> L_D6 , 
  pip INT_X16Y59 EL2BEG2 -> IMUX_B47 , 
  pip INT_X16Y59 LOGIC_OUTS3 -> EL2BEG2 , 
  pip INT_X17Y59 EL2MID2 -> IMUX_B41 , 
  pip IOI_X17Y59 IOI_IMUX_B41 -> IOI_O11 , 
  pip IOI_X17Y59 IOI_O11 -> IOI_O_PINWIRE1 ,  #  _ROUTETHROUGH:D1:OQ "XDL_DUMMY_IOI_X17Y59_OLOGIC_X1Y118" D1 -> OQ
  pip IOI_X17Y59 IOI_O_PINWIRE1 -> IOI_O1 , 
  ;
net "clk" , cfg " _BELSIG:PAD,PAD,clk:clk",
  ;
net "clk_c" , 
  outpin "clk_ibuf/BUFG" O ,
  inpin "blink" CLK ,
  pip CLBLL_X16Y59 SITE_CLK_B0 -> L_CLK , 
  pip CLK_BUFGMUX_X47Y44 CLK_BUFGMUX_POSTMUX_GCLKP30 -> CLK_BUFGMUX_GCLKP30 , 
  pip CLK_HROW_X17Y49 CLK_HROW_GCLK_BUF30 -> CLK_HROW_HCLKL_P5 , 
  pip HCLK_X16Y49 HCLK_G_HCLK_P5 -> HCLK_LEAF_GCLK5 , 
  pip INT_X16Y59 GCLK5 -> CLK_B0 , 
  ;
net "clk_ibuf/IBUFG" , 
  outpin "clk" I ,
  inpin "clk_ibuf/BUFG" I0 ,
  pip CLK_BUFGMUX_X47Y44 CLK_BUFGMUX_MUXED_IN_CLKT_P28 -> CLK_BUFGMUX_PREMUX0_CLK30 , 
  pip CLK_BUFGMUX_X47Y44 CLK_BUFGMUX_PREMUX0_CLK30 -> CLK_BUFGMUX_CLKP0_30 , 
  pip CLK_IOB_T_X17Y60 CLK_IOB_CLK_BUF1 -> CLK_IOB_MUXED_CLKOUT28 , 
  pip CLK_IOB_T_X17Y60 CLK_IOB_PAD_CLK1 -> CLK_IOB_CLK_BUF1 , 
  pip IOI_X17Y61 IOI_D0 -> IOI_I0 ,  #  _ROUTETHROUGH:D:O "XDL_DUMMY_IOI_X17Y61_ILOGIC_X1Y123" D -> O
  pip IOI_X17Y61 IOI_I0 -> IOI_I_2GCLK0 , 
  pip IOI_X17Y61 IOI_IBUF0 -> IOI_D0 , 
  ;
net "out" , cfg " _BELSIG:PAD,PAD,out:out",
  ;
net "reset" , cfg " _BELSIG:PAD,PAD,reset:reset",
  ;
net "reset_c" , 
  outpin "reset" I ,
  inpin "blink" SR ,
  pip CLBLL_X16Y59 SITE_CTRL_B2 -> L_SR , 
  pip INT_INTERFACE_X17Y59 INT_INTERFACE_LOGIC_OUTS_B11 -> INT_INTERFACE_LOGIC_OUTS11 , 
  pip INT_X16Y59 CTRL2 -> CTRL_B2 , 
  pip INT_X16Y59 WS2MID2 -> CTRL2 , 
  pip INT_X17Y59 LOGIC_OUTS11 -> WS2BEG2 , 
  pip IOI_X17Y59 IOI_D0 -> IOI_I0 ,  #  _ROUTETHROUGH:D:O "XDL_DUMMY_IOI_X17Y59_ILOGIC_X1Y119" D -> O
  pip IOI_X17Y59 IOI_I0 -> IOI_LOGIC_OUTS11 , 
  pip IOI_X17Y59 IOI_IBUF0 -> IOI_D0 , 
  ;

# =======================================================
# SUMMARY
# Number of Module Defs: 0
# Number of Module Insts: 0
# Number of Primitive Insts: 5
# Number of Nets: 7
# =======================================================
*/
#endif
