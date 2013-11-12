// Torc - Copyright 2013-2013 University of Southern California.  All Rights Reserved.
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

/// \brief Implementation of class to encapsulate micro-bitstream library generation code

#include "AssemblerLibGen.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include "torc/Physical.hpp"
#include "torc/Bitstream.hpp"
#include "torc/packer/Virtex5PrimitiveStructure.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "../Assembler.hpp"

namespace torc {
namespace bitstream {

/// static const initialization
const char *AssemblerLibGen::sNameSeparator = "-";
const char *AssemblerLibGen::sConfigValueOff = "#OFF";
const char *AssemblerLibGen::sXdlExtension = ".xdl";
const char *AssemblerLibGen::sLibExtension = ".ldb";

/// \brief Initialie function called from constructor
void AssemblerLibGen::initialize() {
	std::cout << "Initializing AssemblerLibGen object for family " << mDB.getFamilyName() << "... " << std::endl;
	// Attach architecture family name to harness folder and check if it exists
	mHarnessFolder = mHarnessFolder / mDB.getFamilyName();
	if(!boost::filesystem::exists(mHarnessFolder)) {
		std::cerr << "ERROR: Folder " << mHarnessFolder.string() << " does not exist" << std::endl;
		throw -1;
	}

	// Create xdl generation folders
	mXdlGenerationReferenceFolder = mXdlGenerationFolder / "reference";
	boost::filesystem::create_directory(mXdlGenerationFolder);
	boost::filesystem::create_directory(mXdlGenerationReferenceFolder);
	
	mLibraryFolder = torc::common::DirectoryTree::getExecutablePath() / "torc/bitstream/assembler/libraries";

	mDeviceToPackageMap = getDeviceToPackageMap();

	std::cout << "Harness folder - " << mHarnessFolder.string() << std::endl;
	std::cout << "Xdl generation folder - " << mXdlGenerationFolder.string() << std::endl;
}

/// \brief Generate micro-Xdl for logic sites
void AssemblerLibGen::generateLogicXdls() {
	std::cout << "Generating micro Xdls for Logic element..." << std::endl;
	// Poplulate tile type to site types map
	TileTypeToSiteTypeSet tileTypeToSiteType = getTileTypeToSiteTypeMap();
	// Populate primitive name to structure map
	PrimitiveStructuresSharedPtrMap primitiveStructures = getPrimitiveNameToStructreMap();

	// Iterate through tile types and then site types
	TileTypeToSiteTypeSet::const_iterator pTileTypes = tileTypeToSiteType.begin();
	TileTypeToSiteTypeSet::const_iterator eTileTypes = tileTypeToSiteType.end();

	// For every tile type
	while(pTileTypes != eTileTypes) {
		//std::cout << "  Tile type: " << pTileTypes->first << std::endl;
		mCurTileType = pTileTypes->first;
		std::set<std::string>::iterator pSites = pTileTypes->second.begin();
		std::set<std::string>::iterator eSites = pTileTypes->second.end();
		pTileTypes++;
		while(pSites != eSites) {

			// look up the PrimitiveDef name and the PrimitiveStructureSharedPtr
			std::string primitiveDefName = *pSites;
			PrimitiveStructureSharedPtr primitiveStructurePtr =
				primitiveStructures[primitiveDefName];
			pSites++;
			//std::cout << "    Site type: " << primitiveDefName << std::endl;
			// If the site type is to be supported
			if(isSiteTypeSupported(primitiveDefName)) {

				// Get the designPtr for test harness for this site type
				torc::physical::DesignSharedPtr harnessDesignPtr = getHarnessDesignForSiteType(
					primitiveDefName);

				if(!harnessDesignPtr) {
					std::cout << "WARNING: No harness design for site type " << primitiveDefName << std::endl;
					continue;
				}

				std::cout << "Tile: " << mCurTileType << "  Site: " << primitiveDefName << std::endl;

				// Create the XDL file name (partial)
				// XDL file is in the form <Family>-<TileType>-<SiteType>-<Config>-<Value>.xdl
				std::stringstream ssXdlFileNamePartial;
				ssXdlFileNamePartial << mDB.getFamilyName() << sNameSeparator << mCurTileType << sNameSeparator << primitiveDefName;

				if(isRambSite(primitiveDefName)) {
					createRambBaseXdl(primitiveDefName, harnessDesignPtr);
				}

				// For Dsp site, MASK and PATTER config have hex config value.
				// The allowed values of these hex cofig are not present in Xdlrc and have to be created
				if(isDspSite(primitiveDefName)) {
					createHexConfigXdl("MASK", ssXdlFileNamePartial.str(), 12, harnessDesignPtr);
					createHexConfigXdl("PATTERN", ssXdlFileNamePartial.str(), 12, harnessDesignPtr);
				}

				// We know the design has only one instance. So, get the first one in the iterator.
				torc::physical::InstanceSharedPtr harnessInstancePtr = *(harnessDesignPtr->instancesBegin());

				const torc::architecture::PrimitiveDef* primitiveDefPtr =
					primitiveStructurePtr->getPrimitiveDefPtr();

				const PrimitiveElementArray& elements = primitiveDefPtr->getElements();
				PrimitiveElementArray::const_iterator pElements = elements.begin();
				PrimitiveElementArray::const_iterator eElements = elements.end();

				// Iterate over config settings/elements of the site
				while(pElements != eElements) {
					const torc::architecture::PrimitiveElement &element = *pElements++;
					//std::cout << "   " << element.getName() << std::endl << "      " ;
					// Cache the current setting of the element in harness design
					std::string harnessConfigName;
					std::string harnessConfigVal;
					// We know harness XDL design has only one instance
					harnessInstancePtr = *(harnessDesignPtr->instancesBegin());
					harnessInstancePtr->getConfig(element.getName(), harnessConfigName, harnessConfigVal);

					// Ignore the route through elements
					if(!primitiveStructurePtr->isRoutethrough(element)) {

						// Generate XDLs only if the element has a config to be set.
						if(element.getCfgs().size() > 0) {

							// ToDo: Not all config setting require reference Xdl with harness as base. E.g. LUT configs.
							// These configs can retain their setting and Xdls for them are not built up on harness.
							createReferenceXdlForConfigSetting(element.getName(), harnessDesignPtr,
								ssXdlFileNamePartial.str());

							// Generate XDL for each config of the element.
							createXdlsForConfigSetting(element, primitiveStructurePtr, ssXdlFileNamePartial.str(),
								harnessDesignPtr);
						}
					}

					// Store back the previous setting of the element in harness Design
					harnessInstancePtr->setConfig(element.getName(), harnessConfigName, harnessConfigVal);

				}
			}
		}
	}

}

/// \brief Generate XDL files for tiles supported for PIPs
void AssemblerLibGen::generateWireXdls() {
	std::cout << "Generating micro XDLs for routing PIPs..." << std::endl;
	TileCount tileCount = mTiles.getTileCount();

	// Go over tiles supported for PIPs
	std::vector<std::string> tilesSupported = getTilesSupportedForPips();
	std::vector<std::string>::const_iterator pTiles = tilesSupported.begin();
	std::vector<std::string>::const_iterator eTiles = tilesSupported.end();
    while(pTiles != eTiles) {
		
		std::string currTileName = *pTiles++;
		bool firstTile = true; //Torc doesn't handle first tile well. So ignore it.
		for(TileIndex i; i < tileCount; i++) {
			const TileInfo& tileInfo = mTiles.getTileInfo(i);
			TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
			std::string tileTypeName = mTiles.getTileTypeName(tileTypeIndex);
			if(tileTypeName.compare(currTileName) == 0) {
				if(firstTile) {
					firstTile = false;
					continue;
				} else {
					exportTileSinks(i);
					break;
				}
			}
		}
	}

}


/// \brief Export all pips within the specified tile
void AssemblerLibGen::exportTileSinks(TileIndex inTileIndex) {

	const TileInfo& tileInfo = mTiles.getTileInfo(inTileIndex);

	torc::architecture::xilinx::TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();

	std::cout << "   Tile name: " << tileInfo.getName() << ", Tile type: " 
		<< mTiles.getTileTypeName(tileTypeIndex) << std::endl;

	torc::architecture::xilinx::WireCount wireCount = mTiles.getWireCount(tileTypeIndex);

	// iterate through all the wires in the tile
	for(torc::architecture::xilinx::WireIndex wireIndex /* implicitly initialized to 0 */; wireIndex < wireCount; wireIndex++) {
		// join the tile index and the wire index into a "tilewire"
		Tilewire source(inTileIndex, wireIndex);
		// expand all of the sinks of interest in this tile
		torc::architecture::TilewireVector sinks;
		mDB.expandTilewireSinks(source, sinks, false, true, true, true);
		if(sinks.empty())
			continue;
		// iterate over all of the pips that we have found
		torc::architecture::TilewireVector::iterator p = sinks.begin();
		torc::architecture::TilewireVector::iterator e = sinks.end();
		while(p < e) {
			// look up the sink tilewire
			Tilewire sink = *p++;
			exportSink(source, sink);
		}
	}
}

/// \brief Export a single pip.
void AssemblerLibGen::exportSink(Tilewire& inSource, Tilewire& inSink) {

	ExtendedWireInfo source(mDB, inSource);
	ExtendedWireInfo sink(mDB, inSink);

	std::stringstream ss;
	ss << mDB.getFamilyName() << sNameSeparator << source.mTileTypeName << sNameSeparator << "routing" << sNameSeparator << source.mWireName << sNameSeparator << sink.mWireName;
	std::string designName(ss.str());

	// ToDo: Should load a design for routing from harness folder
	torc::physical::DesignSharedPtr designPtr = torc::physical::Factory::newDesignPtr(designName,
		mDB.getDeviceName(), mDeviceToPackageMap[mDB.getDeviceName()], "-2", "v3.2");

	string dummyInstName = "dummyInst";
	string dummyInstSiteType = "SLICEL";
	string dummyInstSiteLocation = "SLICE_X1Y1";
	torc::physical::InstanceSharedPtr instancePtr = torc::physical::Factory::newInstancePtr(
		dummyInstName, dummyInstSiteType, "DUMMY", dummyInstSiteLocation);

	designPtr->addInstance(instancePtr);

	NetSharedPtr net = torc::physical::Factory::newNetPtr(dummyInstName);
	torc::physical::Pip pip = torc::physical::Factory::newPip(source.mTileName, source.mWireName, sink.mWireName,
		torc::physical::ePipUnidirectionalBuffered);
	net->addPip(pip);

	torc::physical::InstancePinSharedPtr sourcePin = torc::physical::Factory::newInstancePinPtr(instancePtr,
		"BQ");
	torc::physical::InstancePinSharedPtr sinkPin = torc::physical::Factory::newInstancePinPtr(instancePtr,
		"B4");
	net->addSource(sourcePin);
	net->addSink(sinkPin);

	designPtr->addNet(net);

	// export the created design
	boost::filesystem::path xdlFilePath = mXdlGenerationFolder / designName.append(sXdlExtension);
	std::fstream xdlExport(xdlFilePath.string().c_str(), std::ios_base::out);
	torc::physical::XdlExporter fileExporter(xdlExport);
	fileExporter(designPtr);
	xdlExport.close();
}


/// \details A tile can contain more than one site. This functions returns a map
///	from tile type to set of contained site types
AssemblerLibGen::TileTypeToSiteTypeSet AssemblerLibGen::getTileTypeToSiteTypeMap() {

	TileTypeToSiteTypeSet tileTypeToSiteTypeMap;
	const torc::architecture::Array<const Site>& allSites = mSites.getSites();

	for(uint i = 0; i < allSites.getSize(); i++) {

		const torc::architecture::Site& site = allSites[i];
		std::string siteType = site.getPrimitiveDefPtr()->getName();

		// Some sites can be instantiated via multiple primitives
		// We are interested in one particular primitive only.
		if(isRambSite(siteType))
			siteType = getRambPrimaryPrimitive();

		TileIndex tileIndex = site.getTileIndex();
		const TileInfo& tileInfo = mTiles.getTileInfo(tileIndex);
		const std::string &tileType = mTiles.getTileTypeName(tileInfo.getTypeIndex());

		// There is no entry in map corresponding to this tile type
		if(tileTypeToSiteTypeMap.find(tileType) == tileTypeToSiteTypeMap.end()) {
			std::set<std::string> siteTypeSet;
			// There are two slices contained in a tile. Micro bitstream for both the slices should be 
			// stored in library.
			if(!isSlicelType(siteType)) {
				siteTypeSet.insert(siteType);
			} else {
				string sliceEven = siteType + "E";
				string sliceOdd = siteType + "O";
				siteTypeSet.insert(sliceEven);
				siteTypeSet.insert(sliceOdd);
			}
			tileTypeToSiteTypeMap.insert(std::pair<std::string, std::set<std::string> >(tileType,
				siteTypeSet));
		} else {
			// There are 2 slices in a tile. The two sites should be treated differently.
			if(!isSlicelType(siteType)) {
				tileTypeToSiteTypeMap[tileType].insert(siteType);
			} else {
				string sliceEven = siteType + "E";
				string sliceOdd = siteType + "O";
				tileTypeToSiteTypeMap[tileType].insert(sliceEven);
				tileTypeToSiteTypeMap[tileType].insert(sliceOdd);
			}
		}
	}
	return tileTypeToSiteTypeMap;
}

/// \details Prints list of tile types and the site types in each of the tile types
void AssemblerLibGen::printTileTypeToSiteTypeMapInfo(TileTypeToSiteTypeSet tileTypeToSiteType) {
	TileTypeToSiteTypeSet::const_iterator p = tileTypeToSiteType.begin();
	TileTypeToSiteTypeSet::const_iterator e = tileTypeToSiteType.end();
	std::cout << "Tile type to Site type info:" << std::endl;
	while(p != e) {
		std::cout << "  Tile: " << p->first << std::endl;
		std::set<std::string>::iterator pSites = p->second.begin();
		std::set<std::string>::iterator eSites = p->second.end();
		while(pSites != eSites) {
			std::cout << "\t" << *pSites;
			pSites++;
		}
		std::cout << std::endl;
		p++;
	}
}


/// \details Every site type has a harness design which serves as base generating Xdl
///	for all configurations of the site type
AssemblerLibGen::DesignSharedPtr AssemblerLibGen::getHarnessDesignForSiteType(const string &inSiteType) {

	// Harness file name is of format <tile_type>-<site_type>.xdl
	string harnessFileName = mCurTileType + sNameSeparator + inSiteType + sXdlExtension;
	path harnessFilePath = mHarnessFolder / harnessFileName;

	DesignSharedPtr harnessDesignPtr;
	std::ifstream fileStream(harnessFilePath.string().c_str());
	if(!fileStream.good()) {
		std::cerr << "Could not open file " << harnessFilePath.string() << std::endl;
		return harnessDesignPtr;
	}
	torc::physical::XdlImporter importer;
	importer(fileStream, harnessFilePath.string());

	fileStream.close();

	// Return the design pointer
	harnessDesignPtr = importer.getDesignPtr();
	return harnessDesignPtr;
}

/// \details Returns reference config value for give config setting.
AssemblerLibGen::string AssemblerLibGen::getReferenceConfigValue(const string &inConfigSetting) {
	// if element present in the map of Element->ReferenceConfig, return it.
	std::map<std::string, std::string>::const_iterator pElementConfigMap;
	std::map<std::string, std::string>::const_iterator eElementConfigMap =
		mReferenceConfigMap.end();
	pElementConfigMap = mReferenceConfigMap.find(inConfigSetting);
	if(pElementConfigMap != eElementConfigMap) {
		return pElementConfigMap->second;
	}
	return string(sConfigValueOff);
}

/// \details Return empty design with same metadata
AssemblerLibGen::DesignSharedPtr AssemblerLibGen::createEmptyDesignWithSameMetadata(DesignSharedPtr inDesignPtr) {
	DesignSharedPtr emptyDesign = torc::physical::Factory::newDesignPtr(inDesignPtr->getName(),
		inDesignPtr->getDevice(), inDesignPtr->getPackage(), inDesignPtr->getSpeedGrade(),
		inDesignPtr->getXdlVersion());

	return emptyDesign;
}

/// \brief Set config value and create Xdl
void AssemblerLibGen::createXdlForGeneralConfigSetting(const string &inConfigSetting, const string &inConfigVal,
	const string &inXdlFileNamePartial, DesignSharedPtr inHarnessDesignPtr) {

	std::string xdlFileName = inXdlFileNamePartial + sNameSeparator + inConfigVal + sXdlExtension;

	setConfigAndGenerateXdl(inConfigSetting, inConfigVal, xdlFileName, inHarnessDesignPtr);
}

/// \breif Create Xdls for each config value of the config setting
void AssemblerLibGen::createXdlsForConfigSetting(const torc::architecture::PrimitiveElement &inElement,
	PrimitiveStructureSharedPtr const inPrimitiveStructurePtr, const string &inXdlFileNamePartial,
	DesignSharedPtr inHarnessDesignPtr) {

	std::string configSetting = inElement.getName();

	if(isCompoundSettingHead(inElement.getName())) {
		createXdlForCompoundSettings(inElement, inXdlFileNamePartial, inPrimitiveStructurePtr, inHarnessDesignPtr);
//	} // The use of orphan elements is still debatable
//	else if(inPrimitiveStructurePtr->isOrphan(inElement) && inElement.getName() != "SYNC_ATTR" && 
//				inElement.getName().substr(1) != "5FFSR") {
//		createXdlForOrphanElements(inElement, inXdlFileNamePartial, inHarnessDesignPtr);
	} else {

		std::string xdlFileNameConfig = inXdlFileNamePartial + sNameSeparator + configSetting;

		const torc::architecture::PrimitiveElement::StringSet &configs = inElement.getCfgs();
		torc::architecture::PrimitiveElement::StringSet::const_iterator pConfigs = configs.begin();
		torc::architecture::PrimitiveElement::StringSet::const_iterator eConfigs = configs.end();

		// Iterate over configs of the primitive
		while(pConfigs != eConfigs) {

			if(inPrimitiveStructurePtr->isLUT(inElement, *pConfigs)) {
				createXdlForLut(inElement, inXdlFileNamePartial, inHarnessDesignPtr);
			} else {
				createXdlForGeneralConfigSetting(inElement.getName(), *pConfigs, xdlFileNameConfig, inHarnessDesignPtr);
			}

			pConfigs++;
		}
	}
}

/// \details Create Xdl with just Ramb site instantiated.
void AssemblerLibGen::createRambBaseXdl(const string &inSiteType, DesignSharedPtr inHarnessDesignPtr) {
	// Create XDL file name of the pattern <Family>-<TileType>-<SiteType>-<Element>-<Config>.xdl
	std::stringstream ssXDLFileName;
	ssXDLFileName << mDB.getFamilyName() << sNameSeparator << mCurTileType << sNameSeparator << inSiteType << sNameSeparator
		<< inSiteType << sNameSeparator << "BASE.xdl";
	boost::filesystem::path xdlFilePath = mXdlGenerationFolder / ssXDLFileName.str();
	std::fstream xdlExport(xdlFilePath.string().c_str(), std::ios::out);
	torc::physical::XdlExporter fileExporter(xdlExport);
	DesignSharedPtr emptyDesignPtr = createEmptyDesignWithSameMetadata(inHarnessDesignPtr);
	// add a bram instance
	// Add an empty instance of same site type and location
	InstanceSharedPtr instancePtr = *(inHarnessDesignPtr->instancesBegin());

	InstanceSharedPtr emptyInstance = torc::physical::Factory::newInstancePtr(instancePtr->getName(),
		instancePtr->getType(), instancePtr->getTile(), instancePtr->getSite(),
		instancePtr->getBonding());

	emptyDesignPtr->addInstance(emptyInstance);

	fileExporter(emptyDesignPtr);
	xdlExport.close();

}

/// \brief Create Xdl for configurations with values as hex string
void AssemblerLibGen::createHexConfigXdl(const string &inConfigSetting, const string &inXdlFileNamePartial, int inNumHexChars,
	    	DesignSharedPtr inHarnessDesignPtr) {
	// We know for Virtex 5 DSP's mask and pattern have 12 hex characters as config value.
	long long configValNumber = 1;

	std::string siteTypeAndElement = inXdlFileNamePartial + sNameSeparator + inConfigSetting;

	std::stringstream xdlReferenceFileName;
	xdlReferenceFileName << siteTypeAndElement << sXdlExtension;
	createReferenceXdlForConfigSetting(inConfigSetting, inHarnessDesignPtr, inXdlFileNamePartial);

	for(int bitPosition = 0; bitPosition < inNumHexChars*4; bitPosition++) {
		std::stringstream ssConfigVal;
		ssConfigVal << std::setfill('0') << std::setw(inNumHexChars) << std::hex << configValNumber;

		std::stringstream xdlFileName;
		xdlFileName << siteTypeAndElement << sNameSeparator << bitPosition << sXdlExtension;

		setConfigAndGenerateXdl(inConfigSetting, ssConfigVal.str(), xdlFileName.str(), inHarnessDesignPtr);

		configValNumber = configValNumber << 1;

	}

	// Set the config to default again
	InstanceSharedPtr instancePtr = *(inHarnessDesignPtr->instancesBegin());
	instancePtr->setConfig(inConfigSetting, "", getReferenceConfigValue(inConfigSetting));
}

/// \brief Create Xdls for compound settings
void AssemblerLibGen::createXdlForCompoundSettings(const PrimitiveElement &inElement1, const string &inSiteType, PrimitiveStructureSharedPtr inPrimitiveStructurePtr, DesignSharedPtr inHarnessDesignPtr) {
	std::vector<std::string> compoundElements = mCompoundSettingMap[inElement1.getName()];
	// For now assume there is only on element in the vector, i.e. there is a compound
	// setting with only two elements.
	if(compoundElements.size() != 1) {
		std::cerr << "WARNING: Compound setting with more than two elements not handled currently"
			<< std::endl;
	} else {

		// Generate XDLs with all permutations of configs of the two elements
		// The two element names will be concatenated to form a compound element name Element1Element2.
		// Similarly two config settings will be concatenated to form a compound config name.
		std::string element1Name = inElement1.getName();
		std::string element2Name = compoundElements[0];
		std::string compoundElementName = element1Name + element2Name;

		// Get the other element
		int elementIndex = getElementIndexFromName(element2Name, inPrimitiveStructurePtr);

		if(elementIndex == -1) {
			std::cerr << "WARNING: Compound setting " << element2Name << " not found" << std::endl;
			return;
		}

		const PrimitiveElement &element2 = inPrimitiveStructurePtr->getPrimitiveDefPtr()->getElements()[elementIndex];

		// Get the first instance
		InstanceSharedPtr instancePtr = *(inHarnessDesignPtr->instancesBegin());

		// Store the 2nd elements harness config
		std::string harnessConfig2Val, harnessConfig2Name;
		instancePtr->getConfig(element2Name, harnessConfig2Name, harnessConfig2Val);

		// Set element1 OFF and generate harness
		std::string referenceXdlFileName = inSiteType + sNameSeparator + compoundElementName + sXdlExtension;
		instancePtr->setConfig(element1Name, "", getReferenceConfigValue(element1Name));
		instancePtr->setConfig(element2Name, "", getReferenceConfigValue(element2Name));
		
		boost::filesystem::path referenceXDLFilePath = mXdlGenerationReferenceFolder / referenceXdlFileName;

		std::ofstream fileStream(referenceXDLFilePath.string().c_str());
		if(!fileStream.good()) {
			std::cerr << "Could not open file " << referenceXDLFilePath.string() << std::endl;
			return;
		}

		// Export design as XDL (create referene Xdl)
		torc::physical::XdlExporter exporter(fileStream);
		exporter(inHarnessDesignPtr);
		fileStream.close();

		// Iterate over configs of 1st element
		const PrimitiveElement::StringSet &configs1 = inElement1.getCfgs();
		PrimitiveElement::StringSet::const_iterator pConfigs1 = configs1.begin();
		PrimitiveElement::StringSet::const_iterator eConfigs1 = configs1.end();

		while(pConfigs1 != eConfigs1) {

			// Iterate over configs of 2nd element.
			const PrimitiveElement::StringSet &configs2 = element2.getCfgs();
			PrimitiveElement::StringSet::const_iterator pConfigs2 = configs2.begin();
			PrimitiveElement::StringSet::const_iterator eConfigs2 = configs2.end();

			while(pConfigs2 != eConfigs2) {

				// Form the XDL file name
				std::string compoundConfigName = *pConfigs1 + *pConfigs2;
				std::string xdlFileName = inSiteType + sNameSeparator + compoundElementName
					+ sNameSeparator + compoundConfigName + sXdlExtension;

				// Set element1 config and generate primary XDL
				instancePtr->setConfig(element1Name, "", *pConfigs1);
				setConfigAndGenerateXdl(element2Name, *pConfigs2, xdlFileName, inHarnessDesignPtr);
				pConfigs2++;
			}

			pConfigs1++;
		}

		// Set original value of element2
		instancePtr->setConfig(element2Name, harnessConfig2Name, harnessConfig2Val);
	}
}

/// \brief Create Xdls for orphan elements with empty reference Xdl
void AssemblerLibGen::createXdlForOrphanElements(const torc::architecture::PrimitiveElement &inElement, 
				const string &inXdlFileNamePartial, 
				DesignSharedPtr inHarnessDesignPtr) {
	
	std::string elementName = inElement.getName();
	std::string siteTypeAndElem;
	std::cout << "Found Orphan Element " << elementName << std::endl;
	// Get and empty design with same metadata
	DesignSharedPtr emptyDesign = createEmptyDesignWithSameMetadata(inHarnessDesignPtr);

	// Add an empty instance of same site type and location
	InstanceSharedPtr instancePtr = *(inHarnessDesignPtr->instancesBegin());

	InstanceSharedPtr emptyInstance = torc::physical::Factory::newInstancePtr(instancePtr->getName(),
		instancePtr->getType(), instancePtr->getTile(), instancePtr->getSite(),
		instancePtr->getBonding());

	emptyDesign->addInstance(emptyInstance);

	createReferenceXdlForConfigSetting(elementName, emptyDesign, inXdlFileNamePartial);

	// Go over all the configs
	const PrimitiveElement::StringSet &configs = inElement.getCfgs();
	PrimitiveElement::StringSet::const_iterator pConfigs = configs.begin();
	PrimitiveElement::StringSet::const_iterator eConfigs = configs.end();

	std::string xdlFileNameSetting = inXdlFileNamePartial + sNameSeparator + elementName;
	// Iterate over configs of the primitive
	while(pConfigs != eConfigs) {
		std::string xdlFileName = xdlFileNameSetting + sNameSeparator + *pConfigs + sXdlExtension;
		//std::cout << "\t\tConfig " << *pConfigs << ". File " << xdlFileName << std::endl;
		setConfigAndGenerateXdl(elementName, *pConfigs, xdlFileName, emptyDesign);
		pConfigs++;
	}
	
}

/// \brief Create Xdl for Lut
void AssemblerLibGen::createXdlForLut(const torc::architecture::PrimitiveElement &inElement, const string &inXdlFileNameSitetype,
	DesignSharedPtr inHarnessDesignPtr) {
	std::string configSetting = inElement.getName();

//	std::cout << "Found a LUT " << configSetting << std::endl;

	DesignSharedPtr emptyDesign = createEmptyDesignWithSameMetadata(inHarnessDesignPtr);
	InstanceSharedPtr instancePtr = *(inHarnessDesignPtr->instancesBegin());
	InstanceSharedPtr emptyInstance = torc::physical::Factory::newInstancePtr(instancePtr->getName(),
		instancePtr->getType(), instancePtr->getTile(), instancePtr->getSite(),
		instancePtr->getBonding());
	emptyDesign->addInstance(emptyInstance);

	// Generate reference XDL for LUT
	createReferenceXdlForConfigSetting(configSetting, emptyDesign, inXdlFileNameSitetype);
	string xdlFileNameSetting = inXdlFileNameSitetype + sNameSeparator + configSetting;

	const PrimitiveElementPinArray &pins = inElement.getPins();
	PrimitiveElementPinArray::const_iterator pPinsOut = pins.begin();
	PrimitiveElementPinArray::const_iterator ePinsOut = pins.end();
	//For every output-input pin combination
	while(pPinsOut != ePinsOut) {

		// Handly only input Ax assigned to output O5/O6
		if(pPinsOut->isOutput() && boost::iequals(pPinsOut->getName().substr(0, 1), "O")) {

			PrimitiveElementPinArray::const_iterator pPinsIn = pins.begin();
			PrimitiveElementPinArray::const_iterator ePinsIn = pins.end();

			while(pPinsIn != ePinsIn && boost::iequals(pPinsIn->getName().substr(0, 1), "A")) {
				if(pPinsIn->isInput()) {

					std::string configVal = "#LUT:" + pPinsOut->getName() + "=" + pPinsIn->getName();
					std::string xdlFileName = xdlFileNameSetting + sNameSeparator + configVal + sXdlExtension;
					setConfigAndGenerateXdl(configSetting, configVal, xdlFileName, emptyDesign);
				}
				pPinsIn++;
			}

			// Generate XDLs for output 1 also
			std::string configVal = "#LUT:" + pPinsOut->getName() + "=" + "1";
			std::stringstream xdlFileName1;
			xdlFileName1 << xdlFileNameSetting << sNameSeparator << configVal << sXdlExtension;
			setConfigAndGenerateXdl(configSetting, configVal, xdlFileName1.str(), emptyDesign);

			configVal = "#LUT:" + pPinsOut->getName() + "=" + "0";
			std::stringstream xdlFileName0;
			xdlFileName0 << xdlFileNameSetting << sNameSeparator << configVal << sXdlExtension;
			setConfigAndGenerateXdl(configSetting, configVal, xdlFileName0.str(), emptyDesign);
		}
		pPinsOut++;
	}
}

/// \brief Create reference Xdl for given config setting
void AssemblerLibGen::createReferenceXdlForConfigSetting(const string &inConfigSetting, DesignSharedPtr inHarnessDesignPtr,
		const string &inXdlFileNamePartial) {

	InstanceSharedPtr instancePtr = *(inHarnessDesignPtr->instancesBegin());
	// If no instance found, just return
	if(!instancePtr) {
		std::cerr << "No instance found in the harness " << inHarnessDesignPtr->getName()
			<< std::endl;
		return;
	}

	// Set config value to #OFF for the given element
	std::string referenceConfigValue = getReferenceConfigValue(inConfigSetting);
	instancePtr->setConfig(inConfigSetting, "", referenceConfigValue);

	// Generate an reference XDL file for this element's custom harness
	std::stringstream ssReferenceXdlFileName;
	ssReferenceXdlFileName << inXdlFileNamePartial << sNameSeparator
		<< inConfigSetting << sXdlExtension;
	boost::filesystem::path referenceXDLFilePath = mXdlGenerationReferenceFolder / ssReferenceXdlFileName.str();

	std::ofstream fileStream(referenceXDLFilePath.string().c_str());
	if(!fileStream.good()) {
		std::cerr << "Could not open file " << referenceXDLFilePath.string() << std::endl;
		return;
	}

	// Export design as XDL
	torc::physical::XdlExporter exporter(fileStream);
	exporter(inHarnessDesignPtr);
	fileStream.close();
}

/// \brief Set given config and generate Xdl
void AssemblerLibGen::setConfigAndGenerateXdl(const string &inConfigSetting, const string & inConfigValue,
	const string &inXdlFileName, DesignSharedPtr inOutDesignPtr) {

	InstanceSharedPtr instancePtr = *(inOutDesignPtr->instancesBegin());

	instancePtr->setConfig(inConfigSetting, "", inConfigValue);
	// create the appropriate file paths
	path xdlFilePath = mXdlGenerationFolder / inXdlFileName;

	// Export the design as XDL
	std::fstream xdlExport(xdlFilePath.string().c_str(), std::ios_base::out);
	torc::physical::XdlExporter fileExporter(xdlExport);
	fileExporter(inOutDesignPtr);
}

/// \details Generates bitstream for all Xdls be calling a perl script which iterates over Xdls and
///	calls Xilinxs tools to create bitstream
void AssemblerLibGen::generateBitstreams() {
	// Call perl script to convert Xdl to Bitstream.
	path buildFolder = torc::common::DirectoryTree::getExecutablePath() / "torc/bitstream/assembler/build";
	string systemCommand = "perl " + buildFolder.string() + "/xdl2bit.pl " + mXdlGenerationFolder.string();
	int systemReturn = system(systemCommand.c_str());
	if(systemReturn != 0) {
		std::cerr << "Error calling perl script xdl2bit.pl " << std::endl;
	}
	systemCommand = "perl " + buildFolder.string() + "/xdl2bit.pl " + mXdlGenerationReferenceFolder.string();
	systemReturn = system(systemCommand.c_str());
	if(systemReturn != 0) {
		std::cerr << "Error calling perl script xdl2bit.pl " << std::endl;
	}
}

/// \brief Compress all Xilinx bitfiles in mXdlGenerationFolder
void AssemblerLibGen::compressBitFiles() {
	// Go over all the .bit files
	// Go over all the files in the directory
	boost::filesystem::directory_iterator eFiles;
	boost::filesystem::directory_iterator pFiles(mXdlGenerationFolder);
	while(pFiles != eFiles) {
		// If it is a Xilinx bit file (.bit extension)
		path filePath = pFiles->path();
		pFiles++;
		if(filePath.extension() == ".bit") {

			// Check if bit file in newer then compressed bit file
			path cBitPath = filePath;
			cBitPath.replace_extension(".cbit");
			if(boost::filesystem::exists(cBitPath)) {
				std::time_t bitFileModTime = boost::filesystem::last_write_time(filePath);
				std::time_t cBitFileModTime = boost::filesystem::last_write_time(cBitPath);
				// If compressed bit file modification time later than bitgen bit, continue
				if(cBitFileModTime > bitFileModTime)
					continue;
			}
		
			// Check for reference file
			// (Note: Wrapping filename() as a path for compatibility with Boost.Filesystem V2)
			string referenceBitFileName 
				= boost::filesystem::path(filePath.filename()).string();
			path referenceBitFilePath = mXdlGenerationReferenceFolder / referenceBitFileName;
			if(!boost::filesystem::exists(referenceBitFilePath)) {
				// Remove last part of Xdl file name (config value)
				unsigned position = referenceBitFileName.find_last_of('-');
				referenceBitFileName.replace(position, string::npos, ".bit");
				referenceBitFilePath = mXdlGenerationReferenceFolder / referenceBitFileName;
				if(!boost::filesystem::exists(referenceBitFilePath)) {
					// Get null bitstream path
					referenceBitFilePath = mLibraryFolder / "null_bitstreams" / mDB.getDeviceName();
					referenceBitFilePath.replace_extension(".bit");
					
				}

			}
			compressBitFile(filePath, referenceBitFilePath);
			// std::cout << "Bitfile: " << filePath.filename() << ". Reference: " << referenceBitFilePath.filename() << std::endl;
		}
	}

	generateMemoryMicroBitstream();
}


/// \details Stitch micro-bitstreams in folder mXdlGenerationFolder
/// 	Go over all files with extension .cbit, split file name on '-' to get family, tile type,
///		site type, config setting, and config value. The vector of address and names are 
///		stored in map data structure and then stored in a file. File format is -
///		<<<<BITLIBDB>>>>
/// 	Number of tile types
/// 	Tile type name char count
/// 	Tile type
/// 	Number of sites
///	 	  Site type name char count
///		  Site type
///		  Config seting name char count
///			Config setting
///			Config value name char count
///			Config value
///			  Number of 32bit words in compressed bitstream
///				words
void AssemblerLibGen::stitchMicroBitstreams() {
	std::cout << "Stitching micro bitstream files..." << std::endl; 
	// Maps to store the information on tile type, elements and configs
	std::vector<ConfigValuesToBits> configMapVector;
	std::vector<ConfigSettingsToValues> settingsToValuesMap;
	std::vector<SiteTypeToConfigSettings> siteToSettingsMap;
	TileTypeToSiteType library;

	// Go over all the files in the directory
	boost::filesystem::directory_iterator eFiles;
	boost::filesystem::directory_iterator pFiles(mXdlGenerationFolder);
	while(pFiles != eFiles) {

		// If it is a compressed bit file (.cbit extension)
		if(pFiles->path().extension() == ".cbit") {

			std::ifstream libBitFile(pFiles->path().string().c_str(), std::ios::binary);
			if(!libBitFile.good()) {
				std::cerr << "ERROR: Could not open micro-bitstream file " << pFiles->path().string() << std::endl;
				return;
			}

			// Get number of bits set and put the bit addresses in a vector
			int32_t numSetBits, bitAddr;
			libBitFile.read((char *) &numSetBits, 4);
			std::cout << "\t" << pFiles->path().filename() <<  " opened. Bit count " << numSetBits << std::endl;
			std::vector<uint32_t> bitAddresses;
			for(int i = 0; i < numSetBits && !libBitFile.eof(); i++) {
				libBitFile.read((char *) &bitAddr, 4);
				bitAddresses.push_back(bitAddr);
			}

			// Get the file name without extension and split the name to get tile type, site type element/source wire and config/sink wire.
			boost::filesystem::path filePath(pFiles->path());
			// (Note: Wrapping filename() as a path for compatibility with Boost.Filesystem V2)
			std::string fileName(boost::filesystem::path(filePath.replace_extension().filename()).string());

			std::vector<std::string> splitVector;
			boost::algorithm::split(splitVector, fileName, boost::algorithm::is_any_of(sNameSeparator));

			// Get the tile type, element(source wire) and config (sink wire)from the split string
			string tileType(splitVector[1]);
			string siteType(splitVector[2]);
			string configSetting(splitVector[3]);
			string configValue(splitVector[4]);

			// if tile not found in map, a new tile type has come up. Add it to the map.
			// Create another map for Element-config and push it in vector so that it is not destroyed.
			if(library.find(tileType) == library.end()) {
				SiteTypeToConfigSettings siteToSettings;
				siteToSettingsMap.push_back(siteToSettings);
				library[tileType] = siteToSettingsMap.back();
			}

			// Check if site type is present in map
			if(library[tileType].find(siteType) == library[tileType].end()) {
				ConfigSettingsToValues settingToValues;
				settingsToValuesMap.push_back(settingToValues);
				library[tileType][siteType] = settingsToValuesMap.back();
			}

			// Check if config setting is present in library
			if(library[tileType][siteType].find(configSetting) == library[tileType][siteType].end()) {
				ConfigValuesToBits valuesToBits;
				configMapVector.push_back(valuesToBits);
				library[tileType][siteType][configSetting] = configMapVector.back();
			}

			// Mapping the config to bit addresses
			library[tileType][siteType][configSetting][configValue] = bitAddresses;

		}

		pFiles++;
	}

	saveLibraryMapToFile(library);

}

/// \brief Get word offset from Xdl
uint32_t AssemblerLibGen::getWordOffsetFromXdlFile(path inBitstreamPath, BitstreamSharedPtr inBitstreamPtr) {

	path xdlFilePath = inBitstreamPath.replace_extension(".xdl");
	std::ifstream xdlStream(xdlFilePath.string().c_str());
	if(!xdlStream.good()) {
		std::cout << "File " << xdlFilePath.string() << " not found" << std::endl;
		return 0;
	}
	torc::physical::XdlImporter xdlImporter;
	xdlImporter(xdlFilePath);

	// Get design pointer and related data base
	torc::physical::DesignSharedPtr designPtr = xdlImporter.getDesignPtr();

	torc::architecture::DDB ddb(designPtr->getDevice());
	const Tiles &tiles = ddb.getTiles();

	torc::architecture::xilinx::TileRow tileRow;
	torc::architecture::xilinx::TileCol tileCol;

	bool pipDesign = false;
	// We know this XDL has only one NET and one PIP
	if(designPtr->netsBegin() != designPtr->netsEnd()) {
		torc::physical::NetSharedPtr netPtr = *(designPtr->netsBegin());
		if(netPtr->pipsBegin() != netPtr->pipsEnd()) {
			std::cout << "\tProcessing NET" << std::endl;
			pipDesign = true;
			torc::physical::Pip pip = *(netPtr->pipsBegin());
			torc::architecture::xilinx::TileIndex tileIndex = tiles.findTileIndex(pip.getTileName());
			const torc::architecture::TileInfo& tileInfo = tiles.getTileInfo(tileIndex);
			tileRow = tileInfo.getRow();
			tileCol = tileInfo.getCol();
		}
	}

	// If there is no pip, it is an XDL for logic element.
	if(pipDesign == false) {
		const Sites &sites = ddb.getSites();
		torc::physical::InstanceSharedPtr instancePtr = *(designPtr->instancesBegin());
		SiteIndex siteIndex = sites.findSiteIndex(instancePtr->getSite());
		const Site& site = sites.getSite(siteIndex);

		// look up the site tile index
		TileIndex tileIndex = site.getTileIndex();
		const TileInfo& tileInfo = tiles.getTileInfo(tileIndex);
		tileRow = tileInfo.getRow();
		tileCol = tileInfo.getCol();
	}

	return getWordOffsetFromTileLocation(tileRow, tileCol, inBitstreamPtr);

}

/// \brief Get index in Element array
int AssemblerLibGen::getElementIndexFromName(const string inElementName, PrimitiveStructureSharedPtr inPrimitiveStructurePtr) {
	int elementIndex = -1, index = 0;
	const PrimitiveElementArray& elementsArray = inPrimitiveStructurePtr->getPrimitiveDefPtr()->getElements();
	PrimitiveElementArray::const_iterator pElements = elementsArray.begin();
	PrimitiveElementArray::const_iterator eElements = elementsArray.end();

	// For every primitive element (BEL) in the site
	while(pElements != eElements) {
		if(pElements->getName().compare(inElementName) == 0) {
			elementIndex = index;
			break;
		}
		pElements++; index++;
	}
	return elementIndex;
}

/// \details Saves the library map structure in database format
void AssemblerLibGen::saveLibraryMapToFile(TileTypeToSiteType &inLibrary) {

	std::string dbFileName = getParentFamilyName() + sLibExtension;
	path dbFilePath = mLibraryFolder / dbFileName;
	std::ofstream dbFile(dbFilePath.string().c_str(), std::ofstream::binary);
	if(!dbFile.is_open()) {
		std::cout << "Could not open file " << dbFilePath.string() << " to write" << std::endl;
		return;
	}

	std::cout << "Storing combined micro-bitstream in " << dbFilePath.string() << std::endl;
	dbFile.write("<<<<BITLIBDB>>>>", 16);
//	std::cout << "Tile map size " << sizeof(tileMap.size()) << std::endl;
	uint32_t dataSize;
	dataSize = inLibrary.size();
	dbFile.write((char *) (&dataSize), sizeof(dataSize));
	std::cout << "Total tiles " << dataSize << std::endl;

	// Now go over the map data structure and write it to a file
	// File format is given at the top of this file
	TileTypeToSiteType::const_iterator pTiletypes = inLibrary.begin();
	TileTypeToSiteType::const_iterator eTiletypes = inLibrary.end();
	while(pTiletypes != eTiletypes) {
		// Write the tile name char count, name string and element count
		dataSize = pTiletypes->first.size();
		dbFile.write((const char *) &dataSize, sizeof(dataSize));
		dbFile.write((const char *) pTiletypes->first.c_str(), pTiletypes->first.size());
		dataSize = pTiletypes->second.size();
		dbFile.write((const char*) &dataSize, sizeof(dataSize));
		std::cout << "Tile type " << pTiletypes->first << " has " << pTiletypes->second.size()
			<< " sites." << std::endl;

		// Iterate over elements
		SiteTypeToConfigSettings::const_iterator pSiteTypes = pTiletypes->second.begin();
		SiteTypeToConfigSettings::const_iterator eSiteTypes = pTiletypes->second.end();
		while(pSiteTypes != eSiteTypes) {

			// Write site type name char count, name string and config setting count
			dataSize = pSiteTypes->first.size();
			dbFile.write((const char *) &dataSize, sizeof(dataSize));
			dbFile.write((const char *) pSiteTypes->first.c_str(), pSiteTypes->first.size());
			dataSize = pSiteTypes->second.size();
			dbFile.write((const char *) &dataSize, sizeof(dataSize));
//			std::cout << "   Element type " << pSiteTypes->first << " has "
//				<< pSiteTypes->second.size() << " configs." << std::endl;

			// Itereate over configs
			ConfigSettingsToValues::const_iterator pConfigSettings = pSiteTypes->second.begin();
			ConfigSettingsToValues::const_iterator eConfigSettings = pSiteTypes->second.end();
			while(pConfigSettings != eConfigSettings) {
				// Write config namem char count, name string and count of bit addresses
				dataSize = pConfigSettings->first.size();
				dbFile.write((const char *) &dataSize, sizeof(dataSize));
				dbFile.write(pConfigSettings->first.c_str(), pConfigSettings->first.size());
				dataSize = pConfigSettings->second.size();
				dbFile.write((const char *) &dataSize, sizeof(dataSize));

				ConfigValuesToBits::const_iterator pConfigValues = pConfigSettings->second.begin();
				ConfigValuesToBits::const_iterator eConfigValues = pConfigSettings->second.end();

				while(pConfigValues != eConfigValues) {

					dataSize = pConfigValues->first.size();
					dbFile.write((const char *) &dataSize, sizeof(dataSize));
					dbFile.write(pConfigValues->first.c_str(), pConfigValues->first.size());
					dataSize = pConfigValues->second.size();
					dbFile.write((const char *) &dataSize, sizeof(dataSize));
					// Write the bit addresses
					for(std::vector<uint32_t>::const_iterator iter = pConfigValues->second.begin(); iter
						!= pConfigValues->second.end(); iter++) {
						dbFile.write((char *) &*iter, sizeof(uint32_t));
					}
//					std::cout << "\t" << pConfigSettings->first << " " << pConfigSettings->second.size();
					pConfigValues++;
				}
				pConfigSettings++;
			}
//			std::cout << std::endl;
			pSiteTypes++;
		}
		pTiletypes++;
	}

	// Close file
	dbFile.close();

}

} // namepsace bitstream
} // namespace torc
