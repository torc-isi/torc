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
/// \brief Header for the PrimitiveDef class.

#ifndef TORC_ARCHITECTURE_PRIMITIVEDEF_HPP
#define TORC_ARCHITECTURE_PRIMITIVEDEF_HPP

#include "torc/architecture/PrimitiveConn.hpp"
#include "torc/architecture/PrimitiveElement.hpp"

namespace torc {
namespace architecture {

	/// \brief Encapsulation of primitive site definition, with associated connections, elements, 
	///		and pins.
	class PrimitiveDef {
	protected:
	// friends
		friend class Sites;
	// types
		typedef xilinx::PinIndex PinIndex;				///< \brief Imported type name.
		typedef std::string string;						///< \brief Imported type name.
		/// \brief Map of pin names to pin indexes for a primitive element.
		typedef std::map<std::string, PinIndex> PinNameToPinIndexMap;
	// elements
		/// \brief The primitive name.
		std::string mName;
		/// \brief The primitive pins.
		PrimitivePinArray mPins;
		/// \brief The primitive elements.
		PrimitiveElementArray mElements;
		/// \brief The primitive internal connections.
		PrimitiveConnSharedPtrArray mConnections;
		/// \brief The map of pin names to pin indexes.
		PinNameToPinIndexMap mPinNameToPinIndex;
	// functions
		/// \brief Returns a non-constant array of element pins.
		/// \detail This function should only be used by the Sites class during database 
		///		iniialization.
		PrimitivePinArray& getPins(void) { return mPins; }
	public:
	// constructors
		/// \brief Null constructor.
		/// \detail This constructor should only be used by containers.
		PrimitiveDef(void) : mName(), mPins(), mPinNameToPinIndex() {}
	// functions
		/// \brief Returns the pin index corresponding to the given pin name, or 
		///		PinIndex::undefined() if the pin name does not exist.
		/// \parameter inName The pin name to find.
		PinIndex findPinIndexByName(const std::string& inName) const {
			PinNameToPinIndexMap::const_iterator p = mPinNameToPinIndex.find(inName);
			return (p == mPinNameToPinIndex.end()) 
				? PinIndex(PinIndex::undefined()) : p->second;
		}
	// accessors
		/// \brief Returns the name of the primitive.
		const string& getName(void) const { return mName; }
		/// \brief Returns a constant array of primitive pins.
		const PrimitivePinArray& getPins(void) const { return mPins; }
		/// \brief Returns a constant array of primitive elements.
		const PrimitiveElementArray& getElements(void) const { return mElements; }
		/// \brief Returns a constant array of primitive connection shared pointers.
		const PrimitiveConnSharedPtrArray& getConnections(void) const { return mConnections; }
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_PRIMITIVEDEF_HPP
