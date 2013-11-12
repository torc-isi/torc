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
/// \brief Source for the Versions class.

#include "torc/architecture/Versions.hpp"
#include <iostream>

namespace torc {
namespace architecture {

	size_t Versions::readVersions(DigestStream& inStream, bool inShowVersion) {
		// prepare to read from the stream
		size_t bytesReadOffset = inStream.getBytesRead();
		char scratch[1 << 10];			// scratch read buffer
		uint16_t nameLength = 0;		// length of tile type name

		// read the "TORC" magic
		std::string torc("TORC");
		inStream.read(scratch, 4);
		scratch[4] = 0;
		if(!(torc == scratch)) {
			mErr() << "This is not a valid Torc database.  Magic \"" << scratch 
				<< "\" does not match expected \"" << torc << "\"." << std::endl;
			return 0;
		}

		// read the database Versions information
		inStream.read(mFormat.mMajor);
		inStream.read(mFormat.mMinor);
		inStream.read(mFormat.mRevision);
		inStream.read(mFormat.mReserved);
		// read the database build number
		inStream.read(mBuild);

		// if the database version we just read isn't one that we support, we should throw an 
		// exception and/or return now

		// read the section header
		std::string versionHeader;
		inStream.readSectionHeader(versionHeader);
		/// \todo Throw a proper exception.
		if(versionHeader != ">>>>Version >>>>") throw -1;

		// read the vendor Versions information
		inStream.read(mVendor.mMajor);
		inStream.read(mVendor.mMinor);
		inStream.read(mVendor.mRevision);
		inStream.read(mVendor.mReserved);
		// read the vendor Versions string
		inStream.read(nameLength);
		/// \todo Throw a proper exception.
		if(nameLength > sizeof(scratch)) throw -1;
		inStream.read(scratch, nameLength);
		scratch[nameLength] = 0;
		mVendorString = scratch;
		
		if(inShowVersion) mOut() << "\tDatabase " << static_cast<uint32_t>(mFormat.mMajor) 
			<< "." << static_cast<uint32_t>(mFormat.mMinor) << "." 
			<< static_cast<uint32_t>(mFormat.mRevision) << " build " << mBuild << ", Vendor "
			//<< static_cast<uint32_t>(mVendor.mMajor) << "." 
			//<< static_cast<uint32_t>(mVendor.mMinor) << "." 
			//<< static_cast<uint32_t>(mVendor.mRevision) 
			<< '"' << mVendorString << '"' << std::endl;

		// return the number of bytes read
		return inStream.getBytesRead() - bytesReadOffset;
	}

} // namespace architecture
} // namespace torc
