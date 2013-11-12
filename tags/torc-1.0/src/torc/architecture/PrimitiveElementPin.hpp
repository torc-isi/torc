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
/// \brief Header for the PrimitiveElementPin class.

#ifndef TORC_ARCHITECTURE_PRIMITIVEELEMENTPIN_HPP
#define TORC_ARCHITECTURE_PRIMITIVEELEMENTPIN_HPP

#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/architecture/PrimitivePin.hpp"
#include "torc/architecture/Array.hpp"
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace torc {
namespace architecture {

	namespace architecture { class PrimitiveElementPinUnitTest; }

	// forward declarations
	class PrimitiveElement;
	class PrimitiveConn;
	typedef boost::shared_ptr<const PrimitiveConn> PrimitiveConnSharedPtr;

	/// \brief Encapsulation of a primitive element pin's name, flags, and element pointer.
	/// \detail Primitive element pins are sub-site inputs or outputs.
	class PrimitiveElementPin : public PinDirection {
	protected:
	// friends
		/// \brief The Sites class has access to our internals.
		friend class Sites;
		/// \brief Our unit test class has access to our internals.
		friend class torc::architecture::architecture::PrimitiveElementPinUnitTest;
	// types
		typedef xilinx::PinFlags PinFlags;				///< \brief Imported type name.
		typedef std::string string;						///< \brief Imported type name.
	// members
		/// \brief The primitive element that owns this pin.
		const PrimitiveElement* mElementPtr;
		/// \brief The pin name.
		string mName;
		/// \brief The pin direction flags.
		/// \see PinDirection.
		PinFlags mFlags;
		/// \brief The primitive connection that includes this pin.
		const PrimitiveConnSharedPtr mPrimitiveConn;
	// constructors
		/// \brief Protected constructor.
		/// \parameter inElementPtr The primitive element that owns this pin.
		/// \parameter inName The pin name.
		/// \parameter inFlags The pin direction flags.
		PrimitiveElementPin(const PrimitiveElement* inElementPtr, const std::string& inName, 
			PinFlags inFlags) : mElementPtr(inElementPtr), mName(inName), mFlags(inFlags) {}
	public:
	// accessors
		/// \brief Returns a pointer to the primitive element that owns this pin.
		const PrimitiveElement* getElementPtr(void) const { return mElementPtr; }
		/// \brief Returns the pin name.
		const string& getName(void) const { return mName; }
		/// \brief Returns the pin direction flags.
		PinFlags getFlags(void) const { return mFlags; }
		/// \brief Returns the primitive connection that includes this pin.
		const PrimitiveConnSharedPtr getPrimitiveConn(void) const { return mPrimitiveConn; }
		/// \brief Null constructor.
		/// \detail This constructor should only be used by containers.
		PrimitiveElementPin(void) : mElementPtr(0), mName(), mFlags() {};
		/// \brief Returns true if this pin is a primitive input.
		bool isInput(void) const { return (mFlags & ePinDirectionInput) != 0; }
		/// \brief Returns true if this pin is a primitive output.
		bool isOutput(void) const { return (mFlags & ePinDirectionOutput) != 0; }
	};

	/// \brief Array of constant PrimitiveElementPin objects.
	typedef Array<const PrimitiveElementPin> PrimitiveElementPinArray;

	/// \brief Vector of constant PrimitiveElementPin pointers.
	typedef std::vector<const PrimitiveElementPin*> PrimitiveElementPinPtrVector;

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_PRIMITIVEELEMENTPIN_HPP
