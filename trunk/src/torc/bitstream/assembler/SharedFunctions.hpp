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

#ifndef TORC_BITSTREAM_ASSEMBLER_SHAREDFUNCTIONS_HPP
#define TORC_BITSTREAM_ASSEMBLER_SHAREDFUNCTIONS_HPP

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

bool elementNeedsCompoundSetting(std::string elementName);
bool DSPMaskOrPatternConfig(const std::string &siteType, const std::string &elementName);
void InitializeCompoundSettingsMap();


#endif // TORC_BITSTREAM_ASSEMBLER_SHAREDFUNCTIONS_HPP
