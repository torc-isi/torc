// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
// $HeadURL: https://svn.east.isi.edu/torc/trunk/src/torc/packer/Virtex5PrimitiveStructureUnitTest.cpp $
// $Id: Virtex5PrimitiveStructureUnitTest.cpp 768 2011-08-24 19:44:37Z nsteiner $

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
/// \brief XDL generator for supported logic sites. Generates an XDL for each config and also 
///		an harness XDL for each BEL or as required.

#include "torc/packer/Virtex5PrimitiveStructure.hpp"
#include "torc/architecture/DDB.hpp"
#include <iostream>
#include <fstream>
#include <map>
#include "iomanip"
#include "torc/common/DirectoryTree.hpp"
#include "torc/Physical.hpp"
#include "SharedFunctions.hpp"

using namespace torc::architecture;
using namespace torc::packer;
using namespace torc::physical;

// Constants
static const std::string kXDLReferenceFolder = "reference";

// Globals
static boost::filesystem::path gXDLGenerationFolder;
static boost::filesystem::path gXDLReferenceGenerationFolder;
static std::map<std::string, std::string> gElementReferenceConfigMap;

// Some elements generate 1 bit when configured to #OFF, which is not good for generating compressed bitstream.
// This map stores those configs which generate 0 bit in the bitstream.
void InitializeElementReferenceConfigMap() {

	gElementReferenceConfigMap["LFSR_EN_SET"] = "SET";
	gElementReferenceConfigMap["TEST_SET_P"] = "SET";
	gElementReferenceConfigMap["MASK"] = "000000000000";
	gElementReferenceConfigMap["PATTERN"] = "000000000000";
}

// For most element #OFF is the reference config. But for some elements, #OFF generates 1 bit in bitstream.
// For such elements, the reference config is one which generates 0 bit in bitstream.
std::string GetReferenceConfigValueForElement(std::string elementName) {
	std::string configValue;
	// if element present in the map of Element->ReferenceConfig, return it.
	std::map<std::string, std::string>::const_iterator pElementConfigMap;
	std::map<std::string, std::string>::const_iterator eElementConfigMap =
		gElementReferenceConfigMap.end();
	pElementConfigMap = gElementReferenceConfigMap.find(elementName);
	if(pElementConfigMap != eElementConfigMap) {
		return pElementConfigMap->second;
	}

	return kConfigOff;
}

// \brief Generates a custom reference XDL for the element.
void GenerateReferenceXDLForTheElement(const std::string &elementName, DesignSharedPtr harnessDesignPtr,
	const std::string &xdlFileName) {
	// We know harness XDL design has only one instance
	InstanceSharedPtr instancePtr = *(harnessDesignPtr->instancesBegin());
	// If no instance found, just return
	if(!instancePtr) {
		std::cerr << "No instance found in the harness " << harnessDesignPtr->getName() << std::endl;
		return;
	}

	// Set config value to #OFF for the given element
	std::string configValue = GetReferenceConfigValueForElement(elementName);
	instancePtr->setConfig(elementName, "", configValue);

	/* Generate an reference XDL file for this element's custom harness */
	boost::filesystem::path referenceXDLFilePath = gXDLReferenceGenerationFolder / xdlFileName;

	std::ofstream fileStream(referenceXDLFilePath.string().c_str());
	if(!fileStream.good()) {
		std::cerr << "Could not open file " << referenceXDLFilePath.string() << std::endl;
		return;
	}

	// Export design as XDL
	torc::physical::XdlExporter exporter(fileStream);
	exporter(harnessDesignPtr);
	fileStream.close();

	std::cout << "\tGenerated reference XDL " << referenceXDLFilePath.string() << std::endl;
}

// Given a site type and harness folder, return XDL design shared pointer for harness of the site type
torc::physical::DesignSharedPtr GetTestHarnessDesignForSiteType(std::string siteType,
	const boost::filesystem::path &harnessFolderPath) {

	boost::filesystem::path harnessFilePath(harnessFolderPath);

	torc::physical::DesignSharedPtr harnessDesignPtr;
	std::stringstream ssHarnessFileName;
	//	ssHarnessFileName << "Virtex5_" << siteType << ".xdl";
	ssHarnessFileName << kArchitectureName << kNameSeparator << siteType << ".xdl";

	harnessFilePath = harnessFilePath / ssHarnessFileName.str();

	std::ifstream fileStream(harnessFilePath.string().c_str());
	if(!fileStream.good()) {
		std::cerr << "Could not open file " << harnessFilePath.string() << std::endl;
		return harnessDesignPtr;
	}
	torc::physical::XdlImporter importer;
	importer(fileStream, ssHarnessFileName.str());

	fileStream.close();

	// Return the design pointer
	harnessDesignPtr = importer.getDesignPtr();
	return harnessDesignPtr;
}

// Given element name, return the index of the element in PrimitiveElementArray
int FindPrimitiveElementByName(std::string elementName, PrimitiveStructureSharedPtr const primitiveStructurePtr) {

	int elementIndex = -1, index = 0;
	const PrimitiveElementArray& elementsArray = primitiveStructurePtr->getPrimitiveDefPtr()->getElements();
	PrimitiveElementArray::const_iterator pElements = elementsArray.begin();
	PrimitiveElementArray::const_iterator eElements = elementsArray.end();

	// For every primitive element (BEL) in the site
	while(pElements != eElements) {
		if(pElements->getName().compare(elementName) == 0) {
			elementIndex = index;
			break;
		}
		pElements++; index++;
	}
	return elementIndex;
}

// Sets given elemnt to given config and generates XDL
void SetConfigAndGenerateXDL(const std::string &elementName, const std::string & configVal,
	const std::string & xdlFileName, torc::physical::DesignSharedPtr designPtr) {

	torc::physical::InstanceSharedPtr instancePtr = *(designPtr->instancesBegin());

	instancePtr->setConfig(elementName, "", configVal);
	// create the appropriate file paths
	boost::filesystem::path xdlFilePath = gXDLGenerationFolder / xdlFileName.c_str();

	// Export the design as XDL
	std::fstream xdlExport(xdlFilePath.string().c_str(), std::ios_base::out);
	torc::physical::XdlExporter fileExporter(xdlExport);
	fileExporter(designPtr);
	std::cout << "\tGenerated XDL file " << xdlFileName << std::endl;
}

// Some elements do not require harness. Use empty design for such elements, but same metadata.
DesignSharedPtr GetEmptyDesignWithSameMetadata(torc::physical::DesignSharedPtr designPtr) {
	DesignSharedPtr emptyDesign = Factory::newDesignPtr(designPtr->getName(),
		designPtr->getDevice(), designPtr->getPackage(), designPtr->getSpeedGrade(),
		designPtr->getXdlVersion());

	return emptyDesign;
}

// Some elements together affect the bitstream. Their combined settings decide which bits should be set in bitstream.
// Eg. In DSP48E, ACASCREG and AREG together affect the bitstream.
static void GenerateXdlForCompoundSettings(const PrimitiveElement &element1, std::string siteType,
	PrimitiveStructureSharedPtr const primitiveStructurePtr, torc::physical::DesignSharedPtr designPtr) {

	std::vector<std::string> compoundElements = gCompoundSettingsMap[element1.getName()];
	// For now assume there is only on element in the vector, i.e. there is a compound
	// setting with only two elements.
	if(compoundElements.size() != 1) {
		std::cout << "Compound setting with more than two elements not handled currently"
			<< std::endl;
	} else {

		// Generate XDLs with all permutations of configs of the two elements
		// The two element names will be concatenated to form a compound element name Element1Element2.
		// Similarly two config settings will be concatenated to form a compound config name.
		std::string element1Name = element1.getName();
		std::string element2Name = compoundElements[0];
		std::string compoundElementName = element1Name + element2Name;

		// Get the other element
		int elementIndex = FindPrimitiveElementByName(element2Name, primitiveStructurePtr);

		if(elementIndex == -1) {
			std::cout << "Primitive element " << element2Name << " not found" << std::endl;
			return;
		}

		const PrimitiveElement &element2 = primitiveStructurePtr->getPrimitiveDefPtr()->getElements()[elementIndex];

		// Get the first instance
		InstanceSharedPtr instancePtr = *(designPtr->instancesBegin());

		// Store the 2nd elements harness config
		std::string harnessConfig2Val, harnessConfig2Name;
		instancePtr->getConfig(element2Name, harnessConfig2Name, harnessConfig2Val);

		// Set element1 OFF and generate harness
		std::string referenceXdlFileName = siteType + kNameSeparator + compoundElementName + kXDLExtension;
		instancePtr->setConfig(element1Name, "", kConfigOff);
		GenerateReferenceXDLForTheElement(element2Name, designPtr, referenceXdlFileName);

		// Iterate over configs of 1st element
		const PrimitiveElement::StringSet &configs1 = element1.getCfgs();
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
				std::string xdlFileName = siteType + kNameSeparator + compoundElementName
					+ kNameSeparator + compoundConfigName + kXDLExtension;

				// Set element1 config and generate primary XDL
				instancePtr->setConfig(element1Name, "", *pConfigs1);
				SetConfigAndGenerateXDL(element2Name, *pConfigs2, xdlFileName, designPtr);
				pConfigs2++;
			}

			pConfigs1++;
		}

		// Set original value of element2
		instancePtr->setConfig(element2Name, harnessConfig2Name, harnessConfig2Val);
	}
}

void GenerateXdlForDSPMaskAndPattern(const std::string &elementName, std::string siteType,
		torc::physical::DesignSharedPtr designPtr) {

	// We know for Virtex 5 DSP's mask and pattern have 12 hex characters as config value.
	long long configValNumber = 1;

	std::string siteTypeAndElement = siteType + kNameSeparator + elementName;

	std::stringstream xdlReferenceFileName;
	xdlReferenceFileName << siteTypeAndElement << kXDLExtension;
	GenerateReferenceXDLForTheElement(elementName, designPtr, xdlReferenceFileName.str());

	for(int bitPosition = 0; bitPosition < 48; bitPosition++) {
		std::stringstream ssConfigVal;
		ssConfigVal << std::setfill('0') << std::setw(12) << std::hex << configValNumber;

		std::stringstream xdlFileName;
		xdlFileName << siteTypeAndElement << kNameSeparator << bitPosition << kXDLExtension;

		SetConfigAndGenerateXDL(elementName, ssConfigVal.str(), xdlFileName.str(), designPtr);

		configValNumber = configValNumber << 1;

	}

	// Set the config to default again
	InstanceSharedPtr instancePtr = *(designPtr->instancesBegin());
	instancePtr->setConfig(elementName, "", GetReferenceConfigValueForElement(elementName));
}

void GenerateXdlForLUT(const PrimitiveElement &elem, const std::string &xdlFileNamePartial,
	torc::physical::DesignSharedPtr designPtr) {

	std::string elementName = elem.getName();

	std::cout << "Found a LUT " << elementName << std::endl;

	DesignSharedPtr emptyDesign = GetEmptyDesignWithSameMetadata(designPtr);
	InstanceSharedPtr instancePtr = *(designPtr->instancesBegin());
	InstanceSharedPtr emptyInstance = Factory::newInstancePtr(instancePtr->getName(),
		instancePtr->getType(), instancePtr->getTile(), instancePtr->getSite(),
		instancePtr->getBonding());
	emptyDesign->addInstance(emptyInstance);

	const PrimitiveElementPinArray &pins = elem.getPins();
	PrimitiveElementPinArray::const_iterator pPins = pins.begin();
	PrimitiveElementPinArray::const_iterator ePins = pins.end();

	// Generate reference XDL for LUT
	std::string referenceXdlFileName = xdlFileNamePartial + kXDLExtension;
	GenerateReferenceXDLForTheElement(elementName, emptyDesign, referenceXdlFileName);

	//For every output-input pin combination
	while(pPins != ePins) {

		if(pPins->isOutput()) {

			PrimitiveElementPinArray::const_iterator pPinsIn = pins.begin();
			PrimitiveElementPinArray::const_iterator ePinsIn = pins.end();

			while(pPinsIn != ePinsIn) {
				if(pPinsIn->isInput()) {

					std::string configVal = "#LUT:" + pPins->getName() + "=" + pPinsIn->getName();
					std::stringstream xdlFileName;
					xdlFileName << xdlFileNamePartial << kNameSeparator << configVal << ".xdl";
//					GenerateReferenceXDLForTheElement(elementName, emptyDesign, xdlFileName.str());
					SetConfigAndGenerateXDL(elementName, configVal, xdlFileName.str(), emptyDesign);
				}
				pPinsIn++;
			}

			// Generate XDLs for output 1 also
			std::string configVal = "#LUT:" + pPins->getName() + "=" + "1";
			std::stringstream xdlFileName1;
			xdlFileName1 << xdlFileNamePartial << kNameSeparator << configVal << ".xdl";
//			GenerateReferenceXDLForTheElement(elementName, emptyDesign, xdlFileName1.str());
			SetConfigAndGenerateXDL(elementName, configVal, xdlFileName1.str(), emptyDesign);

			configVal = "#LUT:" + pPins->getName() + "=" + "0";
			std::stringstream xdlFileName0;
			xdlFileName0 << xdlFileNamePartial << kNameSeparator << configVal << ".xdl";
//			GenerateReferenceXDLForTheElement(elementName, emptyDesign, xdlFileName0.str());
			SetConfigAndGenerateXDL(elementName, configVal, xdlFileName0.str(), emptyDesign);
		}
		pPins++;
	}

}

void GenerateXdlForGenericElement(const PrimitiveElement &elem, const std::string &configVal,
	const std::string &xdlFileNamePartial, torc::physical::DesignSharedPtr designPtr) {

	std::stringstream xdlFileName;
	xdlFileName << xdlFileNamePartial << kNameSeparator << configVal << ".xdl";

//	GenerateReferenceXDLForTheElement(elem.getName(), designPtr, xdlFileName.str());

	SetConfigAndGenerateXDL(elem.getName(), configVal, xdlFileName.str(), designPtr);

}

void GenerateXdlForOrphanElements(const PrimitiveElement &elem,
	const std::string &siteType, torc::physical::DesignSharedPtr designPtr) {

	std::cout << "  Found a Orphan element " << elem.getName() << std::endl;

	std::string elementName = elem.getName();
	std::string siteTypeAndElem = siteType + kNameSeparator + elementName;

	// Get and empty design with same metadata
	DesignSharedPtr emptyDesign = GetEmptyDesignWithSameMetadata(designPtr);

	// Add an empty instance of same site type and location
	InstanceSharedPtr instancePtr = *(designPtr->instancesBegin());

	InstanceSharedPtr emptyInstance = Factory::newInstancePtr(instancePtr->getName(),
		instancePtr->getType(), instancePtr->getTile(), instancePtr->getSite(),
		instancePtr->getBonding());

	emptyDesign->addInstance(emptyInstance);

	std::string referenceXdlFileName = siteTypeAndElem + kXDLExtension;
	GenerateReferenceXDLForTheElement(elementName, emptyDesign, referenceXdlFileName);

	// Go over all the configs
	const PrimitiveElement::StringSet &configs = elem.getCfgs();
	PrimitiveElement::StringSet::const_iterator pConfigs = configs.begin();
	PrimitiveElement::StringSet::const_iterator eConfigs = configs.end();

	// Iterate over configs of the primitive
	while(pConfigs != eConfigs) {
			GenerateXdlForGenericElement(elem, *pConfigs, siteTypeAndElem, emptyDesign);
		pConfigs++;
	}
}

void GenerateXdlForElement(const PrimitiveElement &elem,
	PrimitiveStructureSharedPtr const primitiveStructurePtr, const std::string &siteType,
	DesignSharedPtr designPtr) {

	std::cout << "Found a " << elem.getName() << std::endl;

	std::string elementName = elem.getName();

	if(ElementNeedsCompoundSetting(elem.getName())) {
		GenerateXdlForCompoundSettings(elem, siteType, primitiveStructurePtr, designPtr);
	} else if(primitiveStructurePtr->isOrphan(elem) && elem.getName() != "SYNC_ATTR") {
		GenerateXdlForOrphanElements(elem, siteType, designPtr);
	} else {

		std::string siteTypeAndElem = siteType + kNameSeparator + elementName;

		const PrimitiveElement::StringSet &configs = elem.getCfgs();
		PrimitiveElement::StringSet::const_iterator pConfigs = configs.begin();
		PrimitiveElement::StringSet::const_iterator eConfigs = configs.end();

		// Iterate over configs of the primitive
		while(pConfigs != eConfigs) {

			if(primitiveStructurePtr->isLUT(elem, *pConfigs)) {
				GenerateXdlForLUT(elem, siteTypeAndElem, designPtr);
			} else {
				GenerateXdlForGenericElement(elem, *pConfigs, siteTypeAndElem, designPtr);
			}

			pConfigs++;
		}
	}
}

/// \brief Returns true if the library supports the site type.
bool LibrarySupportForSiteType(const std::string& siteType) {
	std::cout << "Site type: " << siteType << std::endl;
	return (	siteType.compare("SLICEM") == 0 ||
			siteType.compare("SLICEL") == 0 ||
			siteType.compare("RAMB36_EXP") == 0 ||
			siteType.compare("DSP48E") == 0 ||
			siteType.compare("BUFGCTRL") == 0 ||
			siteType.compare("BUFDS") == 0 ||
			siteType.compare("BUFR") == 0 ||
			siteType.compare("BUFG") == 0);
	//	return 1; //Generate library files for all types just for information.
}

// Returns true if the site is some form of RAMB site.
bool IsRAMBSite(const std::string &siteType) {
	if(siteType.length() > 3) {
		return siteType.substr(0, 4).compare("RAMB") == 0;
	}
	return false;
}

// Returns true if it DSP site
bool IsDSPSite(const std::string &siteType) {
	if(siteType.length() > 3) {
		return siteType.substr(0, 3).compare("DSP") == 0;
	}
	return false;
}

// This function generates an XDL with designPtr in libraryPath and another empty design in tempFolderPath
void GenerateRAMBBaseFile(const std::string &primitiveDefName,
	torc::physical::DesignSharedPtr designPtr) {

	/* Generate XDL for designPtr in libraryPath */

	// Create XDL file name of the pattern <Architecture>-<SiteType>-<Element>-<Config>.xdl
	std::stringstream ssXDLFileName;
	ssXDLFileName << kArchitectureName << kNameSeparator << primitiveDefName << kNameSeparator
		<< primitiveDefName << kNameSeparator << "BASE.xdl";
	boost::filesystem::path xdlFilePath = gXDLGenerationFolder / ssXDLFileName.str();
	std::fstream xdlExport(xdlFilePath.string().c_str(), std::ios::out);
	torc::physical::XdlExporter fileExporter(xdlExport);
	fileExporter(designPtr);
	xdlExport.close();

	/* Generate XDL for an empty design in tempFolderPath */

	// Get empty design
	DesignSharedPtr emptyDesign = GetEmptyDesignWithSameMetadata(designPtr);
	// Get empty design XDL path and open the file
	boost::filesystem::path emptyDesignPath = gXDLReferenceGenerationFolder / ssXDLFileName.str();
	std::fstream emptyDesignStream(emptyDesignPath.string().c_str(), std::ios::out);
	// Get XDL exporter and export the empty design
	torc::physical::XdlExporter emptyDesignExporter(emptyDesignStream);
	emptyDesignExporter(emptyDesign);
	emptyDesignStream.close();

	return;

}

/// \brief Generates XDLs for various site types. An XDL is generated for each
//  	config setting of each element. All the XDLs are generated with reference to
//		a 'harness' XDL. Two sets of XDLs are generated for each config value - one with the config OFF
//		and other with the config value set.
int main(int argc, char **argv) {

	if(argc != 3) {
		std::cerr << "Invalid arguments. Usage: " << argv[0] <<
			" <harness_folder> <xdl_generation_folder>" << std::endl;
		exit (-1);
	}
	// get the part number
	std::string partNumber = "xc5vfx130t";
	// Initialize the DirectoryTree class
	torc::common::DirectoryTree directoryTree(argv[0]);

	// Check if Harness folder path exists.
	boost::filesystem::path harnessFolderPath = argv[1];
	if(!boost::filesystem::exists(harnessFolderPath)) {
		std::cout << "Folder " << harnessFolderPath.string() << " does not exist" << std::endl;
		exit (-1);
	}

	// Create the xdl generation folder if it doesn't exits
	gXDLGenerationFolder = argv[2];
	if(!boost::filesystem::exists(gXDLGenerationFolder))
		boost::filesystem::create_directory(gXDLGenerationFolder);
	gXDLReferenceGenerationFolder = gXDLGenerationFolder / kXDLReferenceFolder;
	if(!boost::filesystem::exists(gXDLReferenceGenerationFolder))
		boost::filesystem::create_directory(gXDLReferenceGenerationFolder);

	// Initialize the map of element to reference config map
	InitializeElementReferenceConfigMap();
	InitializeCompoundSettingsMap();

	// open and initialize a database
	DDB ddb(partNumber);
	// look up the logic sites in the device
	const Sites& sites = ddb.getSites();
	// define a vector of PrimitiveStructure objects
	typedef std::map<std::string, PrimitiveStructureSharedPtr> PrimitiveStructuresSharedPtrMap;
	PrimitiveStructuresSharedPtrMap primitiveStructures;
	// look up the primitive def types
	typedef const Array<const PrimitiveDef> PrimitiveDefArray;
	PrimitiveDefArray& primitiveDefs = sites.getSiteTypes();
	PrimitiveDefArray::const_iterator p = primitiveDefs.begin();
	PrimitiveDefArray::const_iterator e = primitiveDefs.end();
	// Store the corresponding primitive structures in a map.
	while(p < e) {
		// create a PrimitiveStructure object for this PrimitiveDef
		PrimitiveStructureSharedPtr primitiveStructurePtr(new Virtex5PrimitiveStructure(&*p++));
		const torc::architecture::PrimitiveDef* primitiveDefPtr =
			primitiveStructurePtr->getPrimitiveDefPtr();
		const std::string& primitiveDefName = primitiveDefPtr->getName();
		// insert the PrimitiveStructure into the map
		primitiveStructures[primitiveDefName] = primitiveStructurePtr;
	}

	// iterate through the PrimitiveStructure entries in the map
	PrimitiveStructuresSharedPtrMap::iterator psp = primitiveStructures.begin();
	PrimitiveStructuresSharedPtrMap::iterator pse = primitiveStructures.end();

	// For every site type
	while(psp != pse) {

		// look up the PrimitiveDef name and the PrimitiveStructureSharedPtr
		std::string primitiveDefName = psp->first;
		PrimitiveStructureSharedPtr primitiveStructurePtr = psp->second;

		psp++;
		// If the site type is to be supported
		if(LibrarySupportForSiteType(primitiveDefName)) {

			// Get the designPtr for test harness for this site type
			torc::physical::DesignSharedPtr designPtr = GetTestHarnessDesignForSiteType(
				primitiveDefName, harnessFolderPath);

			if(!designPtr) {
				std::cout << "No test harness for site type " << primitiveDefName << std::endl;
				continue;
			}

			std::cout << "Working on site type " << primitiveDefName << std::endl;

			// Create the XDL file name (partial)
			std::stringstream ssXdlFileName;
			ssXdlFileName << kArchitectureName << kNameSeparator << primitiveDefName;

			if(IsRAMBSite(primitiveDefName)) {
				GenerateRAMBBaseFile(primitiveDefName, designPtr);
			}

			if(IsDSPSite(primitiveDefName)) {
				GenerateXdlForDSPMaskAndPattern("MASK", ssXdlFileName.str(), designPtr);
				GenerateXdlForDSPMaskAndPattern("PATTERN", ssXdlFileName.str(), designPtr);
			}

			// We know the design has only one instance. So, get the first one in the iterator.
			torc::physical::InstanceSharedPtr instancePtr = *(designPtr->instancesBegin());

			const torc::architecture::PrimitiveDef* primitiveDefPtr =
				primitiveStructurePtr->getPrimitiveDefPtr();

			const PrimitiveElementArray& elements = primitiveDefPtr->getElements();

			PrimitiveElementArray::const_iterator pElements = elements.begin();
			PrimitiveElementArray::const_iterator eElements = elements.end();

			// For every primitive element (BEL) in the site
			while(pElements != eElements) {
				const PrimitiveElement &elem = *pElements;
				// Ignore the route through elements
				if(!primitiveStructurePtr->isRoutethrough(elem)) {

					// Cache the current setting of the element
					std::string harnessConfigName;
					std::string harnessConfigVal;
					instancePtr->getConfig(elem.getName(), harnessConfigName, harnessConfigVal);


					// Generate XDLs only if the element has a config to be set.
					if(elem.getCfgs().size() > 0) {
						// First generate a generic reference XDL for all configs of the element, so that the special elements have option of over writing this harness
						std::stringstream ssReferenceXdlFileName;
						ssReferenceXdlFileName << ssXdlFileName.str() << kNameSeparator << elem.getName() << kXDLExtension;
						GenerateReferenceXDLForTheElement(elem.getName(), designPtr, ssReferenceXdlFileName.str());

						// Generate XDL for each config of the element.
						GenerateXdlForElement(elem, primitiveStructurePtr, ssXdlFileName.str(),
												designPtr);
					}

					// Store back the previous setting of the element
					instancePtr->setConfig(elem.getName(), harnessConfigName, harnessConfigVal);
				}

				pElements++;
			}
		}
	}
	return 0;
}

