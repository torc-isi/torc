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
/// \brief Header for the XdlImporter class.

#ifndef TORC_ARCHITECTURE_XDL_IMPORTER_HPP
#define TORC_ARCHITECTURE_XDL_IMPORTER_HPP

#include "torc/physical/XdlImporter.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/InstancePin.hpp"
#include <iostream>
#include "torc/architecture/OutputStreamHelpers.hpp"
#include "torc/architecture/DDBStreamHelper.hpp"

namespace torc {
namespace architecture {

	/// \brief Architecture aware importer from XDL format into a physical design.
	class XdlImporter : public torc::physical::XdlImporter {
	protected:
	// typedefs
		/// \brief Type of the superclass.
		typedef torc::physical::XdlImporter super;
	// members
		/// \brief The device database pointer.
		DDB* mDDBPtr;
	// functions
		/// \brief Initialize the device database.
		virtual void initializeDatabase(void);
		/// \brief Bind the given instance pin to its database Tilewire if applicable.
		virtual void bind(torc::physical::InstancePinSharedPtr& inInstancePin);
		/// \brief Bind the given pip to the database arc and wire usage if applicable.
		virtual void bind(torc::physical::Pip& inPip, EPipType inPipType);
	public:
	// constructors
		/// \brief Basic constructor.
		XdlImporter(void) : super(), mDDBPtr(0) {}
		/// \brief Destructor.
		~XdlImporter(void) {
			if(mDDBPtr) { delete mDDBPtr; mDDBPtr = 0; }
		}
	// accessors
		/// \brief Returns the pointer for the device database.
		DDB* getDDBPtr(void) { return mDDBPtr; }
	// functions
		/// \brief Releases ownership of the device database.  The caller is now responsible for 
		///		deleting it.
		DDB* releaseDDBPtr(void) { DDB* ddbPtr = mDDBPtr; mDDBPtr = 0; return ddbPtr; }
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_XDL_IMPORTER_HPP
