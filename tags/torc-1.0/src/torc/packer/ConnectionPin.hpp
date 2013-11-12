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

#ifndef TORC_PACKER_CONNECTIONPIN_HPP
#define TORC_PACKER_CONNECTIONPIN_HPP

#include "torc/physical/XilinxPhysicalTypes.hpp"
#include <boost/smart_ptr.hpp>
#include <string>
#include <vector>

namespace torc {
namespace physical {

	using namespace std;	
	/// \brief Physical design connection-pin pair, suitable for specifying a net endpoint.
	/// \details This class specifies a pin on a physical design connection.
	
	class ConnectionPin {
		// friends
         /// \brief The Factory class has direct access to our internals.
         friend class RcFactory;
	protected:
		typedef std::string string;
		/// \brief Element Name.
		
		string mElementName;
		/// \brief Name of the pin.
		PinName mPinName;
		
		/// \brief Construct from connection reference and pin name.
		ConnectionPin(string inElementName, const string& inPinName) 
			: mElementName(inElementName), mPinName(inPinName) {}
	public:
		/// \brief NULL connection pin
		ConnectionPin() 
			: mElementName("NULL"), mPinName("NULL") {}
		/// \brief Returns the element pointer or NULL.
		const string& getElementName(void) const { return mElementName; }
		/// \brief Returns the pin name.
		const PinName& getPinName(void) const { return mPinName; }
		/// \brief Equality operator.
		bool operator ==(const ConnectionPin& rhs) const { return mElementName == rhs.mElementName 
			 && mPinName == rhs.mPinName; }
	};

	/// \brief Vector of connection pins.
	typedef std::vector<ConnectionPin> ConnectionPinVector;
		
	
} // namespace physical
} // namespace torc

#endif // TORC_PACKER_CONNECTIONPIN_HPP
