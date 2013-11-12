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
/// \brief Header for the Renamable class.

#ifndef TORC_PHYSICAL_RENAMABLE_HPP
#define TORC_PHYSICAL_RENAMABLE_HPP

#include "torc/physical/Named.hpp"
#include <string>
#include <boost/smart_ptr.hpp>

namespace torc {
namespace physical {

	// forward declarations

	/// \brief Forward declaration of Progenitor template (a parent of the Renamable object).
	template <class T> class Progenitor;

	/// \brief Concept for any object that can be renamed.
//	/// \details No support is provided at this level for renaming, because many subclasses may 
//	///		may require oversight from their parent to avoid name collisions.
//	/// \todo Add a setName() accessor to support renaming.  For many design elements, this may 
//	///		require permission from the parent, to avoid name collisions.  We could make the 
//	///		function virtual, but that would come at the cost of a vtable, and that's very 
//	///		expensive for things like Config objects.  In general, the renaming should probably be 
//	///		managed by the Progeny<T> template.
	template <class T> class Renamable : public Named {
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
	// friends
		/// \brief The Progenitor class has access to our internals.
		friend class Progenitor<T>;
	// accessors
		/// \brief Sets the object name.
		void setName(const string& inName) { mName = inName; }
	public:
	// constructors
		/// \brief Constructor which must specify the object name.
		/// \param inName The object name.
		Renamable(const string& inName) : Named(inName) {}
	// accessors
		/// \brief Illegally sets the object name.
		/// \deprecated.
		void deprecatedSetName(const string& inName) { mName = inName; }
	};

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_RENAMABLE_HPP
