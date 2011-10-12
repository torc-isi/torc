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

#ifndef TORC_PHYSICAL_CONNECTION_HPP
#define TORC_PHYSICAL_CONNECTION_HPP

#include "torc/physical/Named.hpp"
#include "torc/physical/Progeny.hpp"
#include "torc/packer/ConnectionPin.hpp"

#include <string>

namespace torc {
namespace physical {

	/// \brief Hierarchical componenet. 
	class Connection : public Named { 
		// friends 
		/// \brief The Factory class has direct access to our internals. 
		friend class RcFactory; protected: typedef std::string string; 
		ConnectionPinVector mConnectionPins;
	

	public:			
		/// \brief Constructor.  
		Connection(const string& inName) : Named(inName){}
			
		typedef ConnectionPinVector::const_iterator ConnectionPinSharedPtrConstIterator;
		typedef ConnectionPinVector::iterator ConnectionPinSharedPtrIterator;
		// connectionPin iterators
		ConnectionPinSharedPtrConstIterator connectionPinsBegin(void) const { return mConnectionPins.begin(); }
		ConnectionPinSharedPtrConstIterator connectionPinsEnd(void) const { return mConnectionPins.end(); }
		ConnectionPinSharedPtrIterator connectionPinsBegin(void) { return mConnectionPins.begin(); }
		ConnectionPinSharedPtrIterator connectionPinsEnd(void) { return mConnectionPins.end(); }
		ConnectionPinSharedPtrConstIterator getSource(void) const { return mConnectionPins.begin(); }
		ConnectionPinSharedPtrConstIterator getSink(void) const { ConnectionPinSharedPtrConstIterator cp = mConnectionPins.begin();; ++cp; return cp; }
		size_t getConnectionPinCount(void) const { return mConnectionPins.size(); }
		// connectionPin population
		bool addConnectionPin(ConnectionPin& inConnectionPinPtr) {
			/// \todo Acquire mutex.
			ConnectionPinSharedPtrIterator e = connectionPinsEnd();
			ConnectionPinSharedPtrIterator result = std::find(connectionPinsEnd(), e, inConnectionPinPtr);
			if(result != e) return false;
			mConnectionPins.push_back(inConnectionPinPtr);
			return true;
			/// \todo Release mutex.
		}
		
		// checks if the elemnt is a mux that needs to be removed
		bool removeConnectionPin(ConnectionPin& inConnectionPinPtr) {
			/// \todo Acquire mutex.
			ConnectionPinSharedPtrIterator e = connectionPinsEnd();
			ConnectionPinSharedPtrIterator result = std::find(connectionPinsBegin(), e, inConnectionPinPtr);
			if(result == e) return false;
			mConnectionPins.erase(result);
			/// \todo Release mutex.
			return true;
		}
		
		
		
		
		
		// operators
		bool operator ==(const Connection& rhs) const { return mName == rhs.mName; }
		
	};

	/// \brief Shared pointer encapsulation of a componenet
	typedef boost::shared_ptr<Connection> ConnectionSharedPtr;

	/// \brief Weak pointer encapsulation of a componenet
	typedef boost::weak_ptr<Connection> ConnectionWeakPtr;

	/// \brief Vector of componenet shared pointers.
	typedef std::vector<ConnectionSharedPtr> ConnectionSharedPtrVector;
	
} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_CONNECTION_HPP
