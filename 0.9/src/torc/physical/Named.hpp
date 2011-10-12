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
/// \brief Header for the Named class.

#ifndef TORC_PHYSICAL_NAMED_HPP
#define TORC_PHYSICAL_NAMED_HPP

#include <string>
#include <boost/smart_ptr.hpp>

namespace torc {
namespace physical {

	/// \brief Concept for any object that can be named.
	/// \details No support is provided at this level for renaming, because many subclasses may 
	///		may require oversight from their parent to avoid name collisions.
	/// \todo Add a setName() accessor to support renaming.  For many design elements, this may 
	///		require permission from the parent, to avoid name collisions.  We could make the 
	///		function virtual, but that would come at the cost of a vtable, and that's very 
	///		expensive for things like Config objects.  In general, the renaming should probably be 
	///		managed by the Progeny<T> template.
	class Named {
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
	// members
		/// \brief The name of the object.
		string mName;
	public:
	// constructors
		/// \brief Constructor which must specify the object name.
		/// \param inName The object name.
		Named(const string& inName) : mName(inName) {}
	// accessors
		/// \brief Returns the object name.
		const string& getName(void) const { return mName; }
	// operators
		/// \brief Equality operator.
		bool operator ==(const Named& rhs) const { return mName == rhs.mName; }
	};

	/// \brief Shared pointer encapsulation of a Named object.
	typedef boost::shared_ptr<Named> NamedSharedPtr;

	/// \brief Comparator class to serve as a predicate when searching for names.
	class NameComparator {
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
	// members
		/// \brief The name to compare against.
		const string& mName;
	public:
	// constructors
		/// \brief Constructor.
		/// \param inName The name to compare against.
		NameComparator(const string& inName) : mName(inName) {}
	// operators
		/// \brief Function operator taking a Named object.
		/// \returns true if the given object's name matches the desired name, or false 
		///		otherwise.
		bool operator() (const Named& inNamed) const { return inNamed.getName() == mName; }
		/// \brief Function operator taking a Named shared pointer.
		/// \returns true if the given object's name matches the desired name, or false 
		///		otherwise.
		bool operator() (const NamedSharedPtr& inNamedPtr) const 
			{ return inNamedPtr->getName() == mName; }
	};

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_NAMED_HPP
