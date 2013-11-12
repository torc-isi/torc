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
/// \brief Header for the PrimitiveElement class.

#ifndef TORC_ARCHITECTURE_PRIMITIVEELEMENT_HPP
#define TORC_ARCHITECTURE_PRIMITIVEELEMENT_HPP

#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/architecture/PrimitiveElementPin.hpp"
#include "torc/architecture/Array.hpp"
#include <boost/cstdint.hpp>
#include <map>
#include <set>

namespace torc {
namespace architecture {

	namespace architecture { class PrimitiveElementUnitTest; }

	/// \brief Encapsulation of a primitive site element.
	/// \detail Primitive elements are subcomponents of logic primitive sites.
	class PrimitiveElement {
	public:
	// types
		/// \brief A set of configuration values.
		typedef std::set<std::string> StringSet;
	protected:
	// friends
		/// \brief The Sites class has access to our internals.
		friend class Sites;
		/// \brief Our unit test class has access to our internals.
		friend class torc::architecture::architecture::PrimitiveElementUnitTest;
	// types
		typedef xilinx::PinIndex PinIndex;				///< \brief Imported type name.
		typedef std::string string;						///< \brief Imported type name.
		/// \brief Map of pin names to pin indexes for a primitive element.
		typedef std::map<std::string, PinIndex> PinNameToPinIndexMap;
	// members
		/// \brief The element name.
		string mName;
		/// \brief The array of pins.
		PrimitiveElementPinArray mPins;
		/// \brief The set of allowable configuration values.
		StringSet mCfgs;
		/// \brief A flag indicating whether or not this element is a BEL (Basic ELement).
		bool mIsBel;
		/// \brief The map of pin names to pin indexes.
		PinNameToPinIndexMap mPinNameToPinIndex;
	// accessors
		/// \brief Returns a non-constant array of element pins.
		/// \detail This function should only be used by the Sites class during database 
		///		iniialization.
		PrimitiveElementPinArray& getPins(void) { return mPins; }
	public:
	// constructors
		/// \brief Null constructor.
		/// \detail This constructor should only be used by containers.
		PrimitiveElement(void) : mName(), mPins(), mCfgs(), mPinNameToPinIndex() {};
	// functions
		/// \brief Returns the pin index corresponding to the given pin name, or 
		///		PinIndex::undefined() if the pin name does not exist.
		/// \parameter inName The pin name to find.
		PinIndex findPinIndexByName(const string& inName) const {
			PinNameToPinIndexMap::const_iterator p = mPinNameToPinIndex.find(inName);
			return (p == mPinNameToPinIndex.end()) 
				? PinIndex(PinIndex::undefined()) : p->second;
		}
	// accessors
		/// \brief Returns the name of the element.
		const string& getName(void) const { return mName; }
		/// \brief Returns a constant array of element pins.
		const PrimitiveElementPinArray& getPins(void) const { return mPins; }
		/// \brief Returns the set of allowable configuration values.
		const StringSet& getCfgs(void) const { return mCfgs; }
		/// \brief Returns true if this element is a BEL (Basic ELement).
        bool isBel(void) const { return mIsBel; }
	};

	/// \brief Array of constant PrimitiveElement objects.
	typedef Array<const PrimitiveElement> PrimitiveElementArray;

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_PRIMITIVEELEMENT_HPP
