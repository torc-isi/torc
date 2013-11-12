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
/// \brief Header for the Pad class.

#ifndef TORC_ARCHITECTURE_PAD_HPP
#define TORC_ARCHITECTURE_PAD_HPP

#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/architecture/Array.hpp"
#include <string>

namespace torc {
namespace architecture {

	namespace architecture { class PadUnitTest; }
	namespace architecture { class PackageUnitTest; }

	/// \brief Encapsulation of the site index, pin name, and pin flags for a package.
	class Pad {
	protected:
		/// \brief The Sites class has access to our internals.
		friend class Sites;
		/// \brief Our unit test class has access to our internals.
		friend class torc::architecture::architecture::PadUnitTest;
		/// \brief The Package unit test class has access to our internals.
		friend class torc::architecture::architecture::PackageUnitTest;
	// types
		typedef std::string string;						///< \brief Imported type name.
		typedef xilinx::SiteIndex SiteIndex;			///< \brief Imported type name.
		typedef xilinx::SiteFlags SiteFlags;			///< \brief Imported type name.
	// members
		/// \brief The index of the site corresponding to this pad.
		SiteIndex mSiteIndex;
		/// \brief The name of the pad in its physical package.
		string mName;
		/// \brief The site flags for this pad, specifically including bonding.
		SiteFlags mFlags;
	// constructors
		/// \brief Protected constructor.
		Pad(SiteIndex inSiteIndex, const string& inName, SiteFlags inFlags) 
			: mSiteIndex(inSiteIndex), mName(inName), mFlags(inFlags) {}
	public:
	// constructors
		/// \brief Null constructor.
		Pad(void) : mSiteIndex(), mName(), mFlags() {};
	// accessors
		/// \brief Returns the index of the site corresponding to this pad.
		SiteIndex getSiteIndex(void) const { return mSiteIndex; }
		/// \brief Returns the name of the pad in its physical package.
		const string& getName(void) const { return mName; }
		/// \brief Returns the site flags for this pad, specifically including bonding.
		SiteFlags getFlags(void) const { return mFlags; }
	};

	/// \brief Array of constant Pad objects.
	typedef Array<const Pad> PadArray;

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_PAD_HPP
