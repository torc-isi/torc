// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
// $HeadURL: https://svn.east.isi.edu/torc/trunk/src/torc/bitstream/assembler/lut/parser.yy $
// $Id: parser.yy 1303 2013-02-25 23:18:16Z nsteiner $

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

#include "SharedFunctions.hpp"

CompoundSettingMap gCompoundSettingsMap;
const std::string kNameSeparator = "-";
const std::string kFamily = "Virtex5";
const std::string kConfigOff = "#OFF";
const std::string kArchitectureName = "Virtex5";
const std::string kXDLExtension = ".xdl";

// Some elements affect the bitstream together with other element settings.
bool elementNeedsCompoundSetting(std::string elementName) {

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
