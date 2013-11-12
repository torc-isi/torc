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
/// \brief Header for the Design class.

#ifndef TORC_PHYSICAL_DESIGN_HPP
#define TORC_PHYSICAL_DESIGN_HPP

#include "torc/physical/Circuit.hpp"
#include "torc/physical/Module.hpp"
#include <string>

namespace torc {
namespace physical {

	/// \brief Physical netlist design.
	/// \details The design is the top-level entity.  It consists of an arbitrary number of 
	///		modules, instances, and nets.
	/// \details Design objects must be created by the Factory class.
	class Design : public Circuit {
	// friends
		/// \brief The Factory class has direct access to our internals.
		friend class Factory;
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
	// members
		/// \brief Vector of module shared pointers.
		ModuleSharedPtrVector mModules;
		/// \brief The target device specified for this design.
		string mDevice;
		/// \brief The device package specified for this design.
		string mPackage;
		/// \brief The device speed grade specified for this design.
		string mSpeedGrade;
		/// \brief The XDL version specified for this design.
		string mXdlVersion;
	// constructors
		/// \brief Protected constructor.  Designs must be created by the Factory.
		/// \param inName The design name.
		/// \param inDevice The design device.
		/// \param inPackage The device package.
		/// \param inSpeedGrade The device speed grade.
		/// \param inXdlVersion The design XDL version.
		Design(const string& inName, const string& inDevice, const string& inPackage, 
			const string& inSpeedGrade, const string& inXdlVersion) : Circuit(inName), 
			mDevice(inDevice), mPackage(inPackage), mSpeedGrade(inSpeedGrade), 
			mXdlVersion(inXdlVersion) {}
	public:
	// types
		/// \brief Constant iterator for Module shared pointers.
		typedef ModuleSharedPtrVector::const_iterator ModuleSharedPtrConstIterator;
		/// \brief Non-constant iterator for Module shared pointers.
		typedef ModuleSharedPtrVector::iterator ModuleSharedPtrIterator;
	// functions
		/// \brief Find a design module by name.
		/// \param inName The module name to look for.
		/// \returns an iterator for the specified module, or modulesEnd() if the name was not 
		///		found.
		ModuleSharedPtrIterator findModule(const string& inName) {
			NameComparator predicate(inName);
			return std::find_if(modulesBegin(), modulesEnd(), predicate);
		}
		/// \brief Add a module to the design.
		/// \param inModulePtr The module to add.
		/// \returns true if the module was added, or false if a module with the same name already 
		///		exists in the design.
		bool addModule(ModuleSharedPtr& inModulePtr) {
			/// \todo Acquire mutex.
			ModuleSharedPtrVector::iterator e = mModules.end();
			ModuleSharedPtrVector::iterator result = findModule(inModulePtr->getName());
			if(result == e) mModules.push_back(inModulePtr);
			inModulePtr->setParentWeakPtr(mSelfWeakPtr);
			return result == e; // return true if we added the module
			/// \todo Release mutex.
		}
		/// \brief Remove a module from the design.
		/// \param inModulePtr The module to remove.
		/// \returns true if the module was removed, or false if the module did not exist.
		bool removeModule(ModuleSharedPtr& inModulePtr) {
			/// \todo Acquire mutex.
			ModuleSharedPtrVector::iterator e = mModules.end();
			ModuleSharedPtrVector::iterator result = std::find(mModules.begin(), e, inModulePtr);
			if(result == e) return false;
			mModules.erase(result);
			/// \todo Release mutex.
			return true;
		}
	// accessors
		/// \brief Returns the target device for this design.
		const string& getDevice(void) const { return mDevice; }
		/// \brief Returns the device package for this design.
		const string& getPackage(void) const { return mPackage; }
		/// \brief Returns the device speed grade for this design.
		const string& getSpeedGrade(void) const { return mSpeedGrade; }
		/// \brief Returns the XDL version for this design.
		const string& getXdlVersion(void) const { return mXdlVersion; }
		/// \brief Sets the target device for this design.
		void setDevice(const string& inDevice) { mDevice = inDevice; }
		/// \brief Sets the device package for this design.
		void setPackage(const string& inPackage) { mPackage = inPackage; }
		/// \brief Sets the device speed grade for this design.
		void setSpeedGrade(const string& inSpeedGrade) { mSpeedGrade = inSpeedGrade; }
		/// \brief Sets the XDL version for this design.
		void setXdlVersion(const string& inXdlVersion) { mXdlVersion = inXdlVersion; }
		/// \brief Returns the number of modules in the design.
		size_t getModuleCount(void) const { return mModules.size(); }
	// iterators
		/// \brief Returns the begin constant iterator for modules.
		ModuleSharedPtrConstIterator modulesBegin(void) const { return mModules.begin(); }
		/// \brief Returns the end constant iterator for modules.
		ModuleSharedPtrConstIterator modulesEnd(void) const { return mModules.end(); }
		/// \brief Returns the begin non-constant iterator for modules.
		ModuleSharedPtrIterator modulesBegin(void) { return mModules.begin(); }
		/// \brief Returns the end non-constant iterator for modules.
		ModuleSharedPtrIterator modulesEnd(void) { return mModules.end(); }
	};

	/// \brief Shared pointer encapsulation of a Design.
	typedef boost::shared_ptr<Design> DesignSharedPtr;

	/// \brief Vector of Design shared pointers.
	typedef std::vector<DesignSharedPtr> DesignSharedPtrVector;

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_DESIGN_HPP
