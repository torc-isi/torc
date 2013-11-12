/*
 * SharedFunctions.hpp
 *
 *  Created on: Aug 9, 2012
 *      Author: rsoni
 */

#ifndef SHAREDFUNCTIONS_HPP_
#define SHAREDFUNCTIONS_HPP_

#include <iostream>
#include <vector>
#include <map>
#include <stdint.h>

// Typedefs
typedef std::map<const std::string, std::vector<std::string> > CompoundSettingMap;
typedef std::map<const std::string, std::vector<uint32_t> > ConfigBitMap;
typedef std::map<const std::string, ConfigBitMap> ElementConfigMap;
typedef std::map<const std::string, ElementConfigMap> TiletypeElementMap;


// Shared constants
extern const std::string kNameSeparator;
extern const std::string kFamily;
extern const std::string kConfigOff;
extern const std::string kArchitectureName;
extern const std::string kXDLExtension;

// Shared global variables
extern CompoundSettingMap gCompoundSettingsMap;

bool ElementNeedsCompoundSetting(std::string elementName);
bool DSPMaskOrPatternConfig(const std::string &siteType, const std::string &elementName);
void InitializeCompoundSettingsMap();


#endif /* SHAREDFUNCTIONS_HPP_ */
