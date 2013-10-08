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
/// \brief Source for the DeviceInfoHelper class.

#include "torc/bitstream/build/DeviceInfoHelper.hpp"
#include "torc/bitstream/DeviceInfo.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

namespace torc {
namespace bitstream {

	void DeviceInfoHelper::buildFamilyDeviceInfo(const std::string& inFamilyName, 
		const std::string& inTemplateName, const std::string& inOutputName, 
		const torc::common::DeviceVector& inDeviceNames, Bitstream& inBitstream) {

		// preflight and open the output file
		boost::filesystem::path outputPath 
			= torc::common::DirectoryTree::getWorkingPath() / "torc" / "bitstream" / inOutputName;
/// \todo Reinstate check for existing static device information file.
//		if(boost::filesystem::exists(outputPath)) {
//			std::cerr << "Existing static device information file will not be replaced: " 
//				<< outputPath.string() << std::endl;
//			return;
//		}
		std::ofstream outputStream(outputPath.string().c_str(), std::ios::out | std::ios::binary);
		if(!outputStream.is_open()) {
			std::cerr << "Output static device information file could not be opened: " 
				<< outputPath.string() << std::endl;
			return;
		}

		// read the device info template
		boost::filesystem::path templatePath = torc::common::DirectoryTree::getWorkingPath() 
			/ "torc" / "bitstream" / "build" / inTemplateName;
		std::ifstream templateStream(templatePath.string().c_str(), 
			std::ios::in | std::ios::binary | std::ios::ate);
		if(!templateStream.is_open()) {
			std::cerr << "Input static device information template could not be opened: " 
				<< outputPath.string() << std::endl;
			return;
		}
		std::string templateString;
		// resize the string according to the file length
		std::ifstream::pos_type size = templateStream.tellg();
		templateString.resize(size);
		// rewind to the beginning and read the entire file directly into the string
		templateStream.seekg(0, std::ios::beg);
		templateStream.read(const_cast<char*>(templateString.data()), size);
		// close close file
		templateStream.close();

		// iterate over all devices
		std::stringstream allDeviceInfo;
		torc::common::DeviceVector::const_iterator dp = inDeviceNames.begin();
		torc::common::DeviceVector::const_iterator de = inDeviceNames.end();
		while(dp < de) {
			const std::string& device = *dp++;
			if(device.empty()) break;
			inBitstream.setDevice(device);
			inBitstream.initializeDeviceInfo(device);
			inBitstream.writeDeviceInfo(allDeviceInfo, device);
			if(dp < de) allDeviceInfo << std::endl;
		}

		// define the substitutions
		typedef std::map<std::string, std::string> SubstitutionMap;
		SubstitutionMap substitutions;
		substitutions["%%ARCHITECTURE%%"] = inFamilyName;
		substitutions["%%DEVICES%%"] = allDeviceInfo.str();
		substitutions["%%GENERATED%%"] = __FILE__;

		// replace all substitution strings in the template
		SubstitutionMap::iterator p = substitutions.begin();
		SubstitutionMap::iterator e = substitutions.end();
		while(p != e) {
			// create the regular expression, and prepare to replace
			boost::regex re(p->first);
			std::stringstream stringStream;
			std::ostream_iterator<char, char> stringIterator(stringStream);
			// replace all occurrences of the current substitution pattern
			boost::regex_replace(stringIterator, templateString.begin(), templateString.end(), re, 
				p->second, boost::match_default);
			// copy the results back into place
			templateString = stringStream.str();
			p++;
		}

		// write the output file
		outputStream << templateString;
		outputStream.close();

	}

} // namespace bitstream
} // namespace torc
