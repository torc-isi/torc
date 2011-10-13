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
/// \brief Header for the PrimitiveConn class.

#ifndef TORC_ARCHITECTURE_PRIMITIVECONN_HPP
#define TORC_ARCHITECTURE_PRIMITIVECONN_HPP

#include "torc/architecture/PrimitiveElementPin.hpp"

namespace torc {
namespace architecture {

	namespace architecture { class PrimitiveConnUnitTest; }

	/// \brief Encapsulation of a PrimitiveDef internal connection.
	/// \detail This class is analogous to a permanent net with one and only source pin.  All 
	///		PrimitiveConn objects are initialized from a family database by the Sites class.
	class PrimitiveConn {
	protected:
	// friends
		/// \brief The Sites class has access to our internals.
		friend class Sites;
		/// \brief Our unit test class has access to our internals.
		friend class torc::architecture::architecture::PrimitiveConnUnitTest;
	// members
		/// \brief Pointer to the source primitive element pin.
		const PrimitiveElementPin* mSourcePtr;
		/// \brief Vector of pointers to the sink primitive element pins.
		PrimitiveElementPinPtrVector mSinks;
	public:
	// accessors
		/// \brief Returns a pointer to the source primitive element pin.
		const PrimitiveElementPin* getSourcePtr(void) const { return mSourcePtr; }
		/// \brief Returns a vector of pointers to the sink primitive element pins.
		const PrimitiveElementPinPtrVector& getSinks(void) const { return mSinks; }
	};

	/// \brief Array of shared pointers to PrimitiveConn objects.
	typedef Array<PrimitiveConnSharedPtr> PrimitiveConnSharedPtrArray;

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_PRIMITIVECONN_HPP
