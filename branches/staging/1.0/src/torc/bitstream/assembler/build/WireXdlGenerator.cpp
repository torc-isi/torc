// Torc - Copyright 2012-2013 University of Southern California.  All Rights Reserved.
/// \file
/// \brief Open a device, go over all the pips of some tile types
//   	and create an XDL file for each pip

#include <iostream>
#include <fstream>
#include <sstream>
#include "torc/Architecture.hpp"
#include "torc/Physical.hpp"
#include "torc/Common.hpp"
#include "SharedFunctions.hpp"

using namespace torc::common;
using namespace torc::architecture;
using namespace torc::architecture::xilinx;
using namespace torc::physical;

const std::string dummyInstName = "dummyInst";
const std::string dummyInstSiteType = "SLICEL";
const std::string dummyInstSiteLcoation = "SLICE_X1Y1";

static boost::filesystem::path gXDLGenerationFolder;

static const std::string kSupportedTiles[] = {			"INT",
								"CLBLM",
								"CFG_HCLK_INTERFACE",
								"CLKV",
								"CLKV_MC",
								"CLK_BUFGMUX",
								"CLK_CMT_BOT",
								"CLK_CMT_BOT_MGT",
								"CLK_CMT_TOP",
								"CLK_CMT_TOP_MGT",
								"CLK_HROW",
								"CLK_HROW_MGT",
								"CLK_IOB_B",
								"CLK_IOB_T",
								"CLK_MGT_BOT",
								"CLK_MGT_BOT_MGT",
								"CLK_MGT_TOP",
								"CLK_MGT_TOP_MGT",
								"CLK_TERM_BOT",
								"CLK_TERM_TOP",
								"HCLK",
								"HCLK_BRAM",
								"HCLK_BRAM_MGT",
								"HCLK_BRAM_MGT_LEFT",
								"HCLK_CLB",
								"HCLK_CMT_CMT",
								"HCLK_CMT_CMT_MGT",
								"HCLK_CMT_IOI",
								"HCLK_DSP",
								"HCLK_GT3",
								"HCLK_GTX",
								"HCLK_GTX_LEFT",
								"HCLK_IOB",
								"HCLK_IOB_CMT_BOT",
								"HCLK_IOB_CMT_BOT_MGT",
								"HCLK_IOB_CMT_MID",
								"HCLK_IOB_CMT_MID_MGT",
								"HCLK_IOB_CMT_TOP",
								"HCLK_IOB_CMT_TOP_MGT",
								"HCLK_IOI",
								"HCLK_IOI_BOTCEN",
								"HCLK_IOI_BOTCEN_MGT",
								"HCLK_IOI_CENTER",
								"HCLK_IOI_CMT",
								"HCLK_IOI_CMT_MGT",
								"HCLK_IOI_TOPCEN",
								"HCLK_IOI_TOPCEN_MGT",
								"HCLK_LIOB",
								"HCLK_PCIE_BRAM",
								"HCLK_PPC",
								"HCLK_PPC_TERM",
								"HCLK_TERM_L",
								"HCLK_TERM_R",
								"HCLK_VBRK",
								"INT_HCLK_BUFS",
								"LAST_TILE"
							};

/// \brief Export a single pip.
void exportSink(DDB& inDDB, Tilewire& inSource, Tilewire& inSink) {

	ExtendedWireInfo source(inDDB, inSource);
	ExtendedWireInfo sink(inDDB, inSink);

	std::stringstream ss;
	ss << kFamily << kNameSeparator << source.mTileTypeName << kNameSeparator << source.mWireName << kNameSeparator << sink.mWireName;
	std::string designName(ss.str());

	torc::physical::DesignSharedPtr designPtr = torc::physical::Factory::newDesignPtr(designName,
		"xc5vfx130t", "ff1738", "-2", "v3.2");

	torc::physical::InstanceSharedPtr instancePtr = torc::physical::Factory::newInstancePtr(
		dummyInstName, dummyInstSiteType, "DUMMY", dummyInstSiteLcoation);

	designPtr->addInstance(instancePtr);

	NetSharedPtr net = Factory::newNetPtr(dummyInstName);
	Pip pip = Factory::newPip(source.mTileName, source.mWireName, sink.mWireName,
		ePipUnidirectionalBuffered);
	net->addPip(pip);

	torc::physical::InstancePinSharedPtr sourcePin = Factory::newInstancePinPtr(instancePtr,
		"BQ");
	torc::physical::InstancePinSharedPtr sinkPin = Factory::newInstancePinPtr(instancePtr,
		"B4");
	net->addSource(sourcePin);
	net->addSink(sinkPin);

	designPtr->addNet(net);

	// export the created design
	boost::filesystem::path xdlFilePath = gXDLGenerationFolder / designName.append(".xdl");
	std::fstream xdlExport(xdlFilePath.string().c_str(), std::ios_base::out);
	torc::physical::XdlExporter fileExporter(xdlExport);
	fileExporter(designPtr);
}

/// \brief Export all pips within the specified tile
void exportTileSinks(DDB& inDDB, TileIndex inTileIndex) {
	const Tiles& tiles = inDDB.getTiles();
	const TileInfo& tileInfo = tiles.getTileInfo(inTileIndex);

	TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();

	std::cout << "Tile name: " << tileInfo.getName() << ", Tile type name: " 
		<< tiles.getTileTypeName(tileTypeIndex) << std::endl;

	WireCount wireCount = tiles.getWireCount(tileTypeIndex);

	// iterate through all the wires in the tile
	for(WireIndex wireIndex /* implicitly initialized to 0 */; wireIndex < wireCount; wireIndex++) {
		// join the tile index and the wire index into a "tilewire"
		Tilewire source(inTileIndex, wireIndex);
		// expand all of the sinks of interest in this tile
		TilewireVector sinks;
		inDDB.expandTilewireSinks(source, sinks, false, true, true, true);
		if(sinks.empty())
			continue;
		// iterate over all of the pips that we have found
		TilewireVector::iterator p = sinks.begin();
		TilewireVector::iterator e = sinks.end();
		while(p < e) {
			// look up the sink tilewire
			Tilewire sink = *p++;
			exportSink(inDDB, source, sink);
		}
	}
}

/// \brief Export pips from arbitrary tiles for the given device.
int main(int argc, char* argv[]) {

	if(argc != 2) {
		std::cerr << "Invalid arguments. Usage: " << argv[0] <<
			" <xdl_generation_folder>" << std::endl;
		exit (-1);
	}

	// Create the xdl generation folder if it doesn't exits
	gXDLGenerationFolder = argv[1];
	if(!boost::filesystem::exists(gXDLGenerationFolder))
		boost::filesystem::create_directory(gXDLGenerationFolder);

	// construct and initialize the device database
	DirectoryTree directoryTree(argv[0]);


	DeviceDesignator designator("xc5vfx130tff1738-2");
	DDB ddb(designator);
	std::cout << ddb; // this allows the database to annotate stream output

	// look up all tile types
	const Tiles& tiles = ddb.getTiles();
	tiles.getTileTypeCount();
	TileCount tileCount = tiles.getTileCount();

	// Go over the supported tile types, find the first tile of the type in "all types" list
	uint32_t supportedTileIndex = 0;
	while(kSupportedTiles[supportedTileIndex].compare("LAST_TILE") != 0) {

		for(TileIndex i; i < tileCount; i++) {
		    const TileInfo& tileInfo = tiles.getTileInfo(i);
		    TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
		    std::string tileTypeName = tiles.getTileTypeName(tileTypeIndex);
		    if(tileTypeName.compare(kSupportedTiles[supportedTileIndex]) == 0) {
		    	exportTileSinks(ddb, i);
		    	break;
		    }
		}
		supportedTileIndex++;
	}


	return 0;
}
