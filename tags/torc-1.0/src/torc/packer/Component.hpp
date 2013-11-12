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

#ifndef TORC_PACKER_COMPONENT_HPP
#define TORC_PACKER_COMPONENT_HPP

#include "torc/physical/Named.hpp"        
#include "torc/physical/Progeny.hpp"
#include "torc/physical/Progenitor.hpp"
#include "torc/packer/PrimitivePin.hpp"

#include <string>

namespace torc {
namespace physical {

	/// \brief Hierarchical componenet.
	class Component : public Named, public Progeny<class Component>,
						         protected Progenitor<class Component> {
	protected:
		typedef std::string string;
		PrimitivePinSharedPtrVector mPrimitivePins;
		
		/// \brief Protected constructor.  
		Component(const string& inName) : Named(inName), Progenitor<class Component>(){}
	public:
		typedef PrimitivePinSharedPtrVector::const_iterator PrimitivePinSharedPtrConstIterator;
		typedef PrimitivePinSharedPtrVector::iterator PrimitivePinSharedPtrIterator;
		// PrimitivePin iterators
		size_t getPrimitivePinCount(void) const { return mPrimitivePins.size(); }
		PrimitivePinSharedPtrConstIterator PrimitivePinsBegin(void) const { return mPrimitivePins.begin(); }
		PrimitivePinSharedPtrConstIterator PrimitivePinsEnd(void) const { return mPrimitivePins.end(); }
		PrimitivePinSharedPtrIterator PrimitivePinsBegin(void) { return mPrimitivePins.begin(); }
		PrimitivePinSharedPtrIterator PrimitivePinsEnd(void) { return mPrimitivePins.end(); }
		// PrimitivePin population
		bool addPrimitivePin(PrimitivePinSharedPtr& inPrimitivePinPtr) {
			/// \todo Acquire mutex.
			PrimitivePinSharedPtrIterator e = PrimitivePinsEnd();
			PrimitivePinSharedPtrIterator result = std::find(PrimitivePinsEnd(), e, inPrimitivePinPtr);
			if(result != e) return false;
			mPrimitivePins.push_back(inPrimitivePinPtr);
			return true;
			/// \todo Release mutex.
		}
		bool removePrimitivePin(PrimitivePinSharedPtr& inPrimitivePinPtr) {
			/// \todo Acquire mutex.
			PrimitivePinSharedPtrIterator e = PrimitivePinsEnd();
			PrimitivePinSharedPtrIterator result = std::find(PrimitivePinsBegin(), e, inPrimitivePinPtr);
			if(result == e) return false;
			mPrimitivePins.erase(result);
			/// \todo Release mutex.
			return true;
		}
		PrimitivePinSharedPtrIterator findPrimitivePin(const string& inName) {
			NameComparator predicate(inName);
			return std::find_if(PrimitivePinsBegin(), PrimitivePinsEnd(), predicate);
		}
		// operators
		bool operator ==(const Component& rhs) const { return mName == rhs.mName; }
		
	};

	/// \brief Shared pointer encapsulation of a componenet
	typedef boost::shared_ptr<Component> ComponentSharedPtr;

	/// \brief Weak pointer encapsulation of a componenet
	typedef boost::weak_ptr<Component> ComponentWeakPtr;

	/// \brief Vector of componenet shared pointers.
	typedef std::vector<ComponentSharedPtr> ComponentSharedPtrVector;

} // namespace physical
} // namespace torc

#endif // TORC_PACKER_COMPONENT_HPP
