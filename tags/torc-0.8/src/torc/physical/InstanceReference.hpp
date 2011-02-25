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
/// \brief Header for the InstanceReference class.

#ifndef TORC_PHYSICAL_INSTANCE_REFERENCE_HPP
#define TORC_PHYSICAL_INSTANCE_REFERENCE_HPP

#include <string>
#include <boost/smart_ptr.hpp>

namespace torc {
namespace physical {

	/// \brief Instantiation of a module instance.
	/// \details This class references a module instance.  It is used in cases where a module has 
	///		been instantiated in a design, and instances appearing in that design point back to 
	///		the module instances they were instantiated from.
	class InstanceReference {
	// friends
		/// \brief The Factory class has direct access to our internals.
		friend class Factory;
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
		/// \brief Local forward declaration of InstanceSharedPtr to avoid dependency loops.
		typedef boost::shared_ptr<class Instance> InstanceSharedPtr;
		/// \brief Local forward declaration of InstanceWeakPtr to avoid dependency loops.
		typedef boost::weak_ptr<class Instance> InstanceWeakPtr;
		/// \brief Local forward declaration of ModuleSharedPtr to avoid dependency loops.
		typedef boost::shared_ptr<class Module> ModuleSharedPtr;
		/// \brief Local forward declaration of ModuleWeakPtr to avoid dependency loops.
		typedef boost::weak_ptr<class Module> ModuleWeakPtr;
	// members
		/// \brief The module instantiation name.
		/// \details The instantiation name is the name under which the module was instantiated.
		string mInstantiationName;
		/// \brief The module weak pointer.
		ModuleWeakPtr mModulePtr;
		/// \brief The instance weak pointer.
		/// \details The instance pointer points to the module instance that this object references.
		InstanceWeakPtr mInstancePtr;
	// constructors
		/// \brief Protected constructor.
		/// \param inInstantiationName The name under which the reference module was instantiated.
		/// \param inModulePtr The module containing the referenced instance.
		/// \param inInstancePtr The instance being referenced.
		InstanceReference(const string& inInstantiationName, ModuleSharedPtr inModulePtr, 
			InstanceSharedPtr inInstancePtr) : mInstantiationName(inInstantiationName), 
			mModulePtr(inModulePtr), mInstancePtr(inInstancePtr) {}
	public:
	// accessors
		/// \brief Returns the weak module pointer.
		const ModuleWeakPtr& getModulePtr(void) const { return mModulePtr; }
		/// \brief Returns the weak instance pointer.
		const InstanceWeakPtr& getInstancePtr(void) const { return mInstancePtr; }
		/// \brief Returns the instantiation name.
		const string& getInstantiationName(void) const { return mInstantiationName; }
		// Setting accessors will be provided if necessary.
	// operators
		/// \brief Equality operator.
		bool operator ==(const InstanceReference& rhs) const { 
			return !(mInstancePtr < rhs.mInstancePtr) && !(rhs.mInstancePtr < mInstancePtr) 
				&& !(mModulePtr < rhs.mModulePtr) && !(rhs.mModulePtr < mModulePtr) 
				&& mInstantiationName == rhs.mInstantiationName; 
		}
	};

	/// \brief Shared pointer encapsulation of an InstanceReference.
	typedef boost::shared_ptr<InstanceReference> InstanceReferenceSharedPtr;

	/// \brief Weak pointer encapsulation of an InstanceReference.
	typedef boost::weak_ptr<InstanceReference> InstanceReferenceWeakPtr;

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_INSTANCE_REFERENCE_HPP
