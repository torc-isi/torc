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
/// \brief Header for the Versions class.

#ifndef TORC_ARCHITECTURE_VERSIONS_HPP
#define TORC_ARCHITECTURE_VERSIONS_HPP

#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/architecture/DigestStream.hpp"
#include "torc/common/DottedVersion.hpp"

namespace torc {
namespace architecture {

	/// \brief Encapsulation of database version information.
	/// \details The format version information determines if and how DDB should read the database. 
	///		The vendor vesion information indicates the version of the vendor tools that the 
	///		database was generated from.
	class Versions {
	// friends
		/// \brief The database has access to our protected functions.
		friend class DDB;
	protected:
	// types
		typedef std::string string;						///< \brief Imported type name.
		typedef boost::uint8_t uint8_t;					///< \brief Imported type name.
		typedef boost::uint16_t uint16_t;				///< \brief Imported type name.
		typedef boost::uint32_t uint32_t;				///< \brief Imported type name.
		typedef torc::common::DottedVersion DottedVersion;	///< \brief Imported type name.
	// nested classes
	protected:
	// members
		/// \brief The database format version.
		DottedVersion mFormat;
		/// \brief The database build number.
		uint32_t mBuild;
		/// \brief The vendor data version.
		DottedVersion mVendor;
		/// \brief The vendor data version string.
		string mVendorString;
	// functions
		/// \brief Read the version information.
		size_t readVersions(DigestStream& inStream, bool inShowVersion);
	// constructors
		/// \brief Protected constructor.
		Versions(void) : mFormat(), mBuild(0), mVendor(), mVendorString() {}
	public:
	// accessors
		/// \brief Returns the database format version.
		const DottedVersion& getFormat(void) const { return mFormat; }
		/// \brief Returns the database build number.
		uint32_t getBuild(void) const { return mBuild; }
		/// \brief Returns the vendor data version.
		const DottedVersion& getVendor(void) const { return mVendor; }
		/// \brief Returns the database format version string.
		const string& getVendorString(void) const { return mVendorString; }
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_VERSIONS_HPP
