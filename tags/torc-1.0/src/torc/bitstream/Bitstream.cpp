// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
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
/// \brief Source for the Bitstream class.

#include "torc/bitstream/Bitstream.hpp"
#include <boost/filesystem.hpp>
#include <ostream>

namespace torc {
namespace bitstream {

	void Bitstream::writeSubfieldSettings(std::ostream& inStream, uint32_t inWord, 
		const Subfield* inSubfields) {
		// count the subfields
		int subfieldCount = 0;
		while(inSubfields[subfieldCount].mMask) subfieldCount++;
		// generate the output
		inStream << " (";
		for(int index = subfieldCount; index > 0; index--) {
			uint32_t field = index - 1;
			const Subfield& subfield = inSubfields[field];
			uint32_t value = (inWord & subfield.mMask) >> subfield.mShift;
			inStream << (*subfield.mBitgenName == 0 ? subfield.mConfigGuideName 
				: subfield.mBitgenName) << ":";
			const char** ptr = subfield.mValues;
			for(uint32_t i = 0; ; i++) {
				if(*ptr == 0) { inStream << "[UNKNOWN " << i << "]"; break; }
				if(value == i) { inStream << *ptr; break; }
				ptr++;
			}
			if(index > 1) inStream << ", ";
		}
		inStream << ")";
	}

	void Bitstream::writeSubfieldSettings(std::ostream& inStream, uint16_t inWord, 
		const Subfield* inSubfields) {
		// count the subfields
		int subfieldCount = 0;
		while(inSubfields[subfieldCount].mMask) subfieldCount++;
		// generate the output
		inStream << " (";
		for(int index = subfieldCount; index > 0; index--) {
			uint16_t field = index - 1;
			const Subfield& subfield = inSubfields[field];
			uint16_t value = (inWord & subfield.mMask) >> subfield.mShift;
			inStream << (*subfield.mBitgenName == 0 ? subfield.mConfigGuideName 
				: subfield.mBitgenName) << ":";
			const char** ptr = subfield.mValues;
			for(uint16_t i = 0; ; i++) {
				if(*ptr == 0) { inStream << "[UNKNOWN " << i << "]"; break; }
				if(value == i) { inStream << *ptr; break; }
				ptr++;
			}
			if(index > 1) inStream << ", ";
		}
		inStream << ")";
	}

	void Bitstream::writeDeviceInfo(std::ostream& inStream, 
		const std::string& inDeviceName) {
		// write the device column composition
		inStream << "\t// " << inDeviceName << " static device information." << std::endl;
		inStream << "\tboost::uint32_t " << inDeviceName << "Columns[] = { ";
		ColumnTypeVector::const_iterator p = mDeviceInfo.getColumnTypes().begin();
		ColumnTypeVector::const_iterator e = mDeviceInfo.getColumnTypes().end();
		while(p < e) inStream << mColumnDefs[*p++].getName() << ", ";
		inStream << "END };" << std::endl;
		// instantiate the device information
		inStream << "\tDeviceInfo " << inDeviceName << "(" << mDeviceInfo.getTileCount() << ", " 
			<< mDeviceInfo.getRowCount() << ", " << mDeviceInfo.getColCount() << ", " 
			<< inDeviceName << "Columns);" << std::endl;
	}

} // namespace bitstream
} // namespace torc
