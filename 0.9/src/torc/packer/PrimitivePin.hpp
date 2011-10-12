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

#ifndef TORC_PHYSICAL_PRIMITIVE_PIN_HPP
#define TORC_PHYSICAL_PRIMITIVE_PIN_HPP

#include "torc/physical/XilinxPhysicalTypes.hpp"
#include "torc/physical/Named.hpp"
#include <boost/smart_ptr.hpp>
#include <string>
#include <vector>

namespace torc {
namespace physical {

	using namespace std;
	
	//pin types
	typedef enum { Unknown, InputP, OutputP } PinType;
	
	/// \brief Physical design primitive-pin
	/// \details This class specifies a pin on a physical design primitive.
	
	class PrimitivePin: public Named {
		// friends
         /// \brief The Factory class has direct access to our internals.
         friend class RcFactory;
	protected:
		typedef std::string string;
		/// \brief Element Name
		string mElementName;
		/// \brief Type of pin
		PinType mType;
		
		/// \brief Construct from primitive reference and pin name.
		PrimitivePin(string inElementName, string inPinName, const PinType inType) 
			: Named(inPinName), mElementName(inElementName), mType(inType) {}
	public:
		
		/// \brief Returns the element pointer or NULL.
		const string& getElementName(void) const { return mElementName; }
		/// \brief Returns pin type.
		const PinType getType(void) const { return mType; }
		
		/// \brief Returns the type string.
		const string getTypeString(void) const { 
			
			string sTmp = (mType==Unknown) ? "Unknown" : (mType==InputP) ? "input":"output";
			return sTmp;
		}
		
		/// \brief Equality operator.
		bool operator ==(const PrimitivePin& rhs) const { return mElementName == rhs.mElementName 
			 && mName == rhs.mName && mType == mType; }
	};

	/// \brief Vector of primitive pins.
	typedef std::vector<PrimitivePin> PrimitivePinVector;
	
	/// \brief Shared pointer encapsulation of a componenet
	typedef boost::shared_ptr<PrimitivePin> PrimitivePinSharedPtr;

	/// \brief Weak pointer encapsulation of a componenet
	typedef boost::weak_ptr<PrimitivePin> PrimitivePinWeakPtr;

	/// \brief Vector of componenet shared pointers.
	typedef std::vector<PrimitivePinSharedPtr> PrimitivePinSharedPtrVector;
		
	
} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_PRIMITIVE_PIN_HPP
