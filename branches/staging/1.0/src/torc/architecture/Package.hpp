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
/// \brief Header for the Package class.

#ifndef TORC_ARCHITECTURE_PACKAGE_HPP
#define TORC_ARCHITECTURE_PACKAGE_HPP

#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/architecture/Array.hpp"
#include "torc/architecture/Pad.hpp"
#include <map>

namespace torc {
namespace architecture {

	namespace architecture { class PackageUnitTest; }

	/// \brief Encapsulation of a physical device package and its pins.
	class Package {
	protected:
	// friends
		/// \brief The Sites class has access to our internals.
		friend class Sites;
		/// \brief Our unit test class has access to our internals.
		friend class torc::architecture::architecture::PackageUnitTest;
	// types
		typedef std::string string;						///< \brief Imported type name.
		typedef xilinx::PadIndex PadIndex;				///< \brief Imported type name.
		/// \brief A map from pad names to pad indexes.
		typedef std::map<std::string, PadIndex> PadNameToPadIndexMap;
	// members
		/// \brief The package name.
		string mName;
		/// \brief The array of pads for this package.
		PadArray mPads;
		/// \brief The map from pad names to corresponding map indexes.
		PadNameToPadIndexMap mPadNameToPadIndex;
	// constructors
		/// \brief Protected constructor
		Package(const string& inName) : mName(inName), mPads() {}
	// accessors
		/// \brief Returns a non-constant array of package pads
		PadArray& getPads(void) { return mPads; }
	public:
	// constructors
		/// \brief Null constructor.
		Package(void) : mName(), mPads() {}
	// accessors
		/// \brief Returns the package name.
		const string& getName(void) const { return mName; }
		/// \brief Returns a constant array of package pads
		const PadArray& getPads(void) const { return mPads; }
	// functions
		/// \brief Returns the pad index corresponding to the given pad name.
		PadIndex findPadIndexByName(const string& inName) const {
			PadNameToPadIndexMap::const_iterator p = mPadNameToPadIndex.find(inName);
			return (p == mPadNameToPadIndex.end()) ? PadIndex(-1) : p->second;
		}
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_PACKAGE_HPP
