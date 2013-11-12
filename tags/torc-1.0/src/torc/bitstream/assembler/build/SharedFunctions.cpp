/*
 * SharedFunctions.cpp
 *
 *  Created on: Aug 9, 2012
 *      Author: rsoni
 */

#include "SharedFunctions.hpp"

CompoundSettingMap gCompoundSettingsMap;
const std::string kNameSeparator = "-";
const std::string kFamily = "Virtex5";
const std::string kConfigOff = "#OFF";
const std::string kArchitectureName = "Virtex5";
const std::string kXDLExtension = ".xdl";

// Some elements affect the bitstream together with other element settings.
bool ElementNeedsCompoundSetting(std::string elementName) {

	CompoundSettingMap::const_iterator pElement = gCompoundSettingsMap.find(elementName);
	// If element found in the compound settings map
	if(pElement != gCompoundSettingsMap.end()) {
		return true;
	}
	return false;
}

bool DSPMaskOrPatternConfig(const std::string &siteType, const std::string &elementName) {

	if(siteType.compare("DSP48E") == 0) {
		if(elementName.compare("PATTERN") == 0 || elementName.compare("MASK") == 0) {
			return true;
		}
	}
	return false;
}

void InitializeCompoundSettingsMap() {

	std::vector<std::string> acascregCompound;
	acascregCompound.push_back("AREG");
	gCompoundSettingsMap["ACASCREG"] = acascregCompound;

	std::vector<std::string> bcascregCompound;
	bcascregCompound.push_back("BREG");
	gCompoundSettingsMap["BCASCREG"] = bcascregCompound;
}
