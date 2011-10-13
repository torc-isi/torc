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
/// \brief Header for the Progenitor class.

#ifndef TORC_PHYSICAL_PROGENITOR_HPP
#define TORC_PHYSICAL_PROGENITOR_HPP

#include "torc/physical/Renamable.hpp"
#include <string>
#include <boost/smart_ptr.hpp>

namespace torc {
namespace physical {

// forward declaration of our unit test class within its namespace
namespace physical { class ProgenitorUnitTest; }
namespace physical { class ProgenyUnitTest; }

	/// \brief Concept for any object that may have children.
	/// \todo Rename Progenitor to SelfReferent, since some childless objects need to refer to 
	///		themselves.  c.f. InstancePin.
	template <class T> class Progenitor {
	// friends
		/// \brief The Factory class has direct access to our internals.
		/// \details The Factory class needs access so that it can set this object's self weak 
		///		pointer after constructing it.  This object can in turn share its weak pointer with 
		///		its children, allowing them to link back.
		friend class Factory;
		/// \brief Our unit test has direct access to our internals.
		friend class torc::physical::physical::ProgenitorUnitTest;
		/// \brief The Progeny<T> unit test has direct access to our internals.
		friend class torc::physical::physical::ProgenyUnitTest;
	protected:
	// types
		/// \brief Weak pointer of our own type.
		typedef boost::weak_ptr<T> WeakPtrType;
		/// \brief Shared pointer of our own type.
		typedef boost::shared_ptr<T> SharedPtrType;
	// members
		/// \brief Weak pointer this object.
		/// \details This weak pointer is kept here to be passed along to our children, allowing 
		///		them to point back to us without the strong dependency of a shared pointer.
		WeakPtrType mSelfWeakPtr;
		/// \brief Sets the weak pointer to this object.
		void setSelfWeakPtr(WeakPtrType inSelfPtr) { mSelfWeakPtr = inSelfPtr; }
	// functions
//		/// \brief Rename a child element.
//		void renameChild(Renamable& inChild, const string& inName) {
//			inChild.setName(inName);
//		}
	public:
	// accessors
		/// \brief Returns a weak pointer to this object.
		const WeakPtrType& getSelfWeakPtr(void) const { return mSelfWeakPtr; }
	};

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_PROGENITOR_HPP
