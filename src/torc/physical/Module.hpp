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
/// \brief Header for the Module class.

#ifndef TORC_PHYSICAL_MODULE_HPP
#define TORC_PHYSICAL_MODULE_HPP

#include "torc/physical/Circuit.hpp"
#include "torc/physical/Port.hpp"
#include <string>

namespace torc {
namespace physical {

	/// \brief Hierarchical module.
	/// \details Modules are used for hard macros in the Xilinx tool flow.  For most users, modules 
	///		never appear in XDL netlists, either because the design did not use hard macros, or 
	///		because those hard macros were flattened as instances and nets directly in the design.
	class Module : public Circuit, protected Progenitor<Module> {
	// friends
		/// \brief The Factory class has direct access to our internals.
		friend class Factory;
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
	// members
		/// \brief The module anchor instance name.
		string mAnchor;
		/// \brief Vector of Port shared pointers for the module
		PortSharedPtrVector mPorts;
	// constructors
		/// \brief Protected constructor.
		/// \param inName The module name.
		/// \param inAnchor The anchor instance name for the module.  The anchor designates the 
		///		instance in the module relative to which the module will be placed.
		Module(const string& inName, const string& inAnchor) : Circuit(inName), mAnchor(inAnchor) {}
	public:
	// types
		/// \brief Constant iterator to Port shared pointers.
		typedef PortSharedPtrVector::const_iterator PortSharedPtrConstIterator;
		/// \brief Non-constant iterator to Port shared pointers.
		typedef PortSharedPtrVector::iterator PortSharedPtrIterator;
	// functions
		/// \brief Find a port by name.
		/// \param inName The port name to look for.
		/// \returns an iterator for the specified port, or portsEnd() if the name was not found.
		PortSharedPtrIterator findPort(const string& inName) {
			NameComparator predicate(inName);
			return std::find_if(portsBegin(), portsEnd(), predicate);
		}
		/// \brief Add a port to the module.
		/// \param inPortPtr The port to add.
		/// \returns true if the instance was added, or false if an instance with the same name 
		///		already exists in the circuit.
		bool addPort(PortSharedPtr& inPortPtr) {
			/// \todo Acquire mutex.
			PortSharedPtrIterator e = portsEnd();
			PortSharedPtrIterator result = findPort(inPortPtr->getName());
			if(result != e) return false;
			mPorts.push_back(inPortPtr);
			inPortPtr->Progeny<Module>::setParentWeakPtr(Progenitor<Module>::mSelfWeakPtr);
			return true;
			/// \todo Release mutex.
		}
		bool removePort(PortSharedPtr& inPortPtr) {
			/// \todo Acquire mutex.
			PortSharedPtrIterator e = portsEnd();
			PortSharedPtrIterator result = findPort(inPortPtr->getName());
			if(result == e) return false;
			mPorts.erase(result);
			/// \todo Release mutex.
			return true;
		}
	// accessors
		/// \brief Returns the anchor instance name for this module.
		const string& getAnchor(void) const { return mAnchor; }
		/// \brief Sets the anchor instance name for this module.
		void setAnchor(const string& inAnchor) { mAnchor = inAnchor; }
	// iterators
		/// \brief Returns the begin constant iterator for ports.
		PortSharedPtrConstIterator portsBegin(void) const { return mPorts.begin(); }
		/// \brief Returns the end constant iterator for ports.
		PortSharedPtrConstIterator portsEnd(void) const { return mPorts.end(); }
		/// \brief Returns the begin non-constant iterator for ports.
		PortSharedPtrIterator portsBegin(void) { return mPorts.begin(); }
		/// \brief Returns the end non-constant iterator for ports.
		PortSharedPtrIterator portsEnd(void) { return mPorts.end(); }
		/// \brief Returns the number of ports in the module.
		size_t getPortCount(void) const { return mPorts.size(); }
	// operators
		/// \brief Equality operator.
		/// \details This function deems modules equal if their names are identical.
		/// \param rhs The module to compare against.
		/// \returns true if both module names are identical, or false otherwise.
		bool operator ==(const Module& rhs) const { return mName == rhs.mName; }
	};

	/// \brief Shared pointer encapsulation of a Module.
	typedef boost::shared_ptr<Module> ModuleSharedPtr;

	/// \brief Weak pointer encapsulation of a Module.
	typedef boost::weak_ptr<Module> ModuleWeakPtr;

	/// \brief Vector of Module shared pointers.
	typedef std::vector<ModuleSharedPtr> ModuleSharedPtrVector;

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_MODULE_HPP
