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

#ifndef TORC_PACKER_ELEMENT_HPP
#define TORC_PACKER_ELEMENT_HPP

#include <string>
#include <map>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <functional>
#include <boost/smart_ptr.hpp>

#include "torc/packer/Component.hpp"
#include "torc/packer/Connection.hpp"

namespace torc {
namespace physical {
	
	using namespace std;
	
	typedef vector<string> ConfigVector;
	
		
	/// \brief Element composed of connections and pins.
	/// \details This class implements everything for XDLRC elements.
	class Element : public Component{
		// friends
         /// \brief The Factory class has direct access to our internals.
         friend class RcFactory;
	protected:
		
		ConnectionSharedPtrVector mConnections;
		ConfigVector mConfigs;
		
		/// \brief Constructor.  
		Element(const string& inName) : Component(inName){}		
	public:
		
		typedef ConnectionSharedPtrVector::const_iterator ConnectionSharedPtrConstIterator;
		typedef ConnectionSharedPtrVector::iterator ConnectionSharedPtrIterator;
		
		typedef ConfigVector::const_iterator ConfigConstIterator;
		typedef ConfigVector::iterator ConfigIterator;
	
			
		bool addConnection(ConnectionSharedPtr& inConnectionPtr) {
			/// \todo Acquire mutex.
			mConnections.push_back(inConnectionPtr);
			return true;
			/// \todo Release mutex.
		}
		bool removeConnection(ConnectionSharedPtr& inConnectionPtr) {
			/// \todo Acquire mutex.
			ConnectionSharedPtrIterator e = connectionsEnd();
			ConnectionSharedPtrIterator result = std::find(connectionsBegin(), e, inConnectionPtr);
			if(result == e) return false;
			mConnections.erase(result);
			/// \todo Release mutex.
			return true;
		}
		
		bool removeConnection(ConnectionSharedPtrIterator inConnectionIter) {
			/// \todo Acquire mutex.
			
			mConnections.erase(inConnectionIter);
			/// \todo Release mutex.
			return true;
		}
		
		// connection iterators
		ConnectionSharedPtrConstIterator connectionsBegin(void) const { return mConnections.begin(); }
		ConnectionSharedPtrConstIterator connectionsEnd(void) const { return mConnections.end(); }
		ConnectionSharedPtrIterator connectionsBegin(void) { return mConnections.begin(); }
		ConnectionSharedPtrIterator connectionsEnd(void) { return mConnections.end(); }
		size_t getConnectionCount(void) const { return mConnections.size(); }
		
		bool addConfig(string& inConfigPtr) {
			/// \todo Acquire mutex.
			ConfigIterator e = mConfigs.end();
			ConfigIterator result = std::find(mConfigs.begin(), e, inConfigPtr);
			if(result != e) return false;
			mConfigs.push_back(inConfigPtr);
			return true;
			/// \todo Release mutex.
		}
		bool removeConfig(string& inConfigPtr) {
			/// \todo Acquire mutex.
			ConfigIterator e = mConfigs.end();
			ConfigIterator result = std::find(mConfigs.begin(), e, inConfigPtr);
			if(result == e) return false;
			mConfigs.erase(result);
			/// \todo Release mutex.
			return true;
		}
		// connection iterators
		ConfigConstIterator confsBegin(void) const { return mConfigs.begin(); }
		ConfigConstIterator confsEnd(void) const { return mConfigs.end(); }
		ConfigIterator confsBegin(void)  { return mConfigs.begin(); }
		ConfigIterator confsEnd(void)  { return mConfigs.end(); }
		size_t getConfigCount(void) const { return mConfigs.size(); }
	};

	/// \brief Shared pointer encapsulation of a element.
	typedef boost::shared_ptr<Element> ElementSharedPtr;

	/// \brief Weak pointer encapsulation of a element.
	typedef boost::weak_ptr<Element> ElementWeakPtr;

	/// \brief Vector of element shared pointers.
	typedef std::vector<ElementSharedPtr> ElementSharedPtrVector;
		
	
} // namespace physical
} // namespace torc

#endif // TORC_PACKER_ELEMENT_HPP
