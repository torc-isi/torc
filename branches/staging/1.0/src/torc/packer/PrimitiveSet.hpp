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
/// \brief Header for the PrimitiveSet class.

#ifndef TORC_PACKER_PRIMITIVESET_HPP
#define TORC_PACKER_PRIMITIVESET_HPP

#include "torc/packer/Primitive.hpp"
#include <string>

namespace torc {
namespace physical {

/// \brief PrimitiveSet.

class PrimitiveSet : public Named, public Progeny<class PrimitiveSet>,
						         protected Progenitor<class PrimitiveSet>  {
		// friends
/// \brief The Factory class has direct access to our internals.
friend class RcFactory;


protected:
// types
/// \brief Imported type name.
typedef std::string string;
// members
/// \brief Vector of primitive shared pointers.
PrimitiveSharedPtrVector mPrimitives;
				 
				 // constructors
PrimitiveSet(const string& inName) : Named(inName), Progenitor<class PrimitiveSet>(){}
public:

// types
/// \brief Constant iterator for Primitive shared pointers.
typedef PrimitiveSharedPtrVector::const_iterator PrimitiveSharedPtrConstIterator;
/// \brief Non-constant iterator for Primitive shared pointers.
typedef PrimitiveSharedPtrVector::iterator PrimitiveSharedPtrIterator;
// functions
/// \brief Find a PrimitiveSet primitive by name.
/// \param inName The primitive name to look for.
/// \returns an iterator for the specified primitive, or primitivesEnd() if the name was not 
///     found.
PrimitiveSharedPtrIterator findPrimitive(const string& inName) {
NameComparator predicate(inName);
return std::find_if(primitivesBegin(), primitivesEnd(), predicate);
}
/// \brief Add a primitive to the PrimitiveSet.
/// \param inPrimitivePtr The primitive to add.
/// \returns true if the primitive was added, or false if a primitive with the same name already 
///     exists in the PrimitiveSet.
bool addPrimitive(PrimitiveSharedPtr& inPrimitivePtr) {
/// \todo Acquire mutex.
PrimitiveSharedPtrVector::iterator e = mPrimitives.end();
PrimitiveSharedPtrVector::iterator result = findPrimitive(inPrimitivePtr->getName());
if(result == e) mPrimitives.push_back(inPrimitivePtr);
return result == e; // return true if we added the primitive
/// \todo Release mutex.
}
/// \brief Remove a primitive from the PrimitiveSet.
/// \param inPrimitivePtr The primitive to remove.
/// \returns true if the primitive was removed, or false if the primitive did not exist.
bool removePrimitive(PrimitiveSharedPtr& inPrimitivePtr) {
/// \todo Acquire mutex.
PrimitiveSharedPtrVector::iterator e = mPrimitives.end();
PrimitiveSharedPtrVector::iterator result = std::find(mPrimitives.begin(), e, inPrimitivePtr);
if(result == e) return false;
mPrimitives.erase(result);
/// \todo Release mutex.
return true;
}


// accessors

size_t getPrimitiveCount(void) const { return mPrimitives.size(); }
// iterators
/// \brief Returns the begin constant iterator for primitives.
PrimitiveSharedPtrConstIterator primitivesBegin(void) const { return mPrimitives.begin(); }
/// \brief Returns the end constant iterator for primitives.
PrimitiveSharedPtrConstIterator primitivesEnd(void) const { return mPrimitives.end(); }
/// \brief Returns the begin non-constant iterator for primitives.
PrimitiveSharedPtrIterator primitivesBegin(void) { return mPrimitives.begin(); }
/// \brief Returns the end non-constant iterator for primitives.
PrimitiveSharedPtrIterator primitivesEnd(void) { return mPrimitives.end(); }

};

/// \brief Shared pointer encapsulation of a PrimitiveSet.
typedef boost::shared_ptr<PrimitiveSet> PrimitiveSetSharedPtr;
	
	/// \brief Shared pointer encapsulation of a PrimitiveSet.
typedef boost::weak_ptr<PrimitiveSet> PrimitiveSetWeakPtr;

/// \brief Vector of PrimitiveSet shared pointers.
typedef std::vector<PrimitiveSetSharedPtr> PrimitiveSetSharedPtrVector;
		
} // namespace physical
} // namespace torc

#endif // TORC_PACKER_PRIMITIVESET_HPP
