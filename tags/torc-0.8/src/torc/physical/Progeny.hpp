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
/// \brief Header for the Progeny class.

#ifndef TORC_PHYSICAL_PROGENY_HPP
#define TORC_PHYSICAL_PROGENY_HPP

#include <string>
#include <boost/smart_ptr.hpp>

namespace torc {
namespace physical {

	/// \brief Concept for any object that may have a parent.
	template <class T> class Progeny {
	protected:
	// types
		/// \brief Weak pointer of our own type.
		typedef boost::weak_ptr<T> WeakPtrType;
		/// \brief Shared pointer of our own type.
		typedef boost::shared_ptr<T> SharedPtrType;
	// members
		/// \brief Weak pointer to the parent.
		WeakPtrType mParentWeakPtr;
	public:
	// constructors
		/// \brief Null constructor.
		Progeny(void) {}
		/// \brief Constructor that specifies a parent.
		Progeny(const WeakPtrType& inParentPtr) : mParentWeakPtr(inParentPtr) {}
	// accessors
		/// \brief Returns a weak pointer to the parent.
		const WeakPtrType& getParentWeakPtr(void) const { return mParentWeakPtr; }
		/// \brief Sets the weak pointer to the parent.
		void setParentWeakPtr(WeakPtrType inParentPtr) { mParentWeakPtr = inParentPtr; }
		/// \brief Method to reset and orphan this object.
		void resetParentWeakPtr(void) { mParentWeakPtr.reset(); }
	// broken
		/// \brief Returns a shared pointer to the parent (WARNING: Does not work right).
		/// \details Always seems to generate a "Returning reference to temporary" warning, and I 
		///		cannot figure out why.
		/// \details It is the caller's responsibility to reset the shared pointer when done.
		/// \todo Figure out why Progeny<T>::getParentSharedPtr() yields a "Returning reference to 
		///		temporary" warning.
		const SharedPtrType& getParentSharedPtr(void) const { return mParentWeakPtr.lock(); }
	};

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_PROGENY_HPP
