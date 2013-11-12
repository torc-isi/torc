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
/// \brief Header for the PrimitivePin class.

#ifndef TORC_ARCHITECTURE_PRIMITIVEPIN_HPP
#define TORC_ARCHITECTURE_PRIMITIVEPIN_HPP

#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/architecture/Array.hpp"
#include <string>

namespace torc {
namespace architecture {

	namespace architecture { class PrimitivePinUnitTest; }

	/// \bried Encapsulation of pin directionality.
	/// \detail These direction enumerations are obtained from the database build code, and are 
	///		used by the PrimitivePin and PrimitivElementPin classes.
	class PinDirection {
	public:
	// enumerations
		enum EPinDirection { ePinDirectionNone = 0, ePinDirectionInput = 2, 
			ePinDirectionOutput = 4 };
	};

	/// \brief Encapsulation of a primitive pin's name and flags.
	/// \detail Primitive pins are logic site inputs or outputs.
	class PrimitivePin : public PinDirection {
	protected:
	// friends
		/// \brief The Sites class has access to our internals.
		friend class Sites;
		/// \brief Our unit test class has access to our internals.
		friend class torc::architecture::architecture::PrimitivePinUnitTest;
	// types
		typedef xilinx::PinFlags PinFlags;				///< \brief Imported type name.
		typedef std::string string;						///< \brief Imported type name.
	// members
		/// \brief The pin name.
		string mName;
		/// \brief The pin direction flags.
		/// \see PinDirection.
		PinFlags mFlags;
	// constructors
		/// \brief Protected constructor.
		/// \parameter inName The pin name.
		/// \parameter inFlags The pin direction flags.
		PrimitivePin(const string& inName, PinFlags inFlags) : mName(inName), mFlags(inFlags) 
			{}
	public:
	// accessors
		/// \brief Returns the pin name.
		const string& getName(void) const { return mName; }
		/// \brief Returns the pin direction flags.
		PinFlags getFlags(void) const { return mFlags; }
		/// \brief Null constructor.
		/// \detail This constructor should only be used by containers.
		PrimitivePin(void) : mName(), mFlags() {};
		/// \brief Returns true if this pin is a primitive input.
		bool isInput(void) const { return (mFlags & ePinDirectionInput) != 0; }
		/// \brief Returns true if this pin is a primitive output.
		bool isOutput(void) const { return (mFlags & ePinDirectionOutput) != 0; }
	};

	/// \brief Array of constant PrimitivePin objects.
	typedef Array<const PrimitivePin> PrimitivePinArray;

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_PRIMITIVEPIN_HPP
