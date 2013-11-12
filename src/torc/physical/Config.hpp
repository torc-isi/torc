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
/// \brief Header for the Config class.

#ifndef TORC_PHYSICAL_CONFIG_HPP
#define TORC_PHYSICAL_CONFIG_HPP

#include "torc/physical/XilinxPhysicalTypes.hpp"
#include "torc/physical/Named.hpp"

namespace torc {
namespace physical {

// forward declaration of our unit test class within its namespace
namespace physical { class ConfigUnitTest; }

	/// \brief Configuration.  A {name:value} pair.
	/// \details Config elements typically live in a ConfigMap, and by inheritance, in Design, 
	///		Module, Instance, and Net objects.  Within a ConfigMap, a setting name acts as a key 
	///		that points to a Config, together forming the standard {setting:name:value} triplet.
	/// \note The name mentioned here is a user-specified name stemming from the design.  It is not 
	///		the configuration setting name.  For example, in configuration "DFF:blink:#FF", "DFF" 
	///		is the specified setting, "blink" is the name that the design assigns to the 
	///		corresponding resource, and "#FF" is the value to which "DFF" is set.
	class Config : public Named {
	// friends
		/// \brief The ConfigMap class has direct access to our internals.
		friend class ConfigMap;
		/// \brief Our unit test has direct access to our internals.
		friend class torc::physical::physical::ConfigUnitTest;
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
	// static variables
		/// \brief Default configuration name.
		static const char* sConfigDefaultName;
		/// \brief Default configuration value.
		static const char* sConfigDefaultValue;
	// members
		/// \brief The configuration setting value.
		/// \todo Consider typing mValue to something like SettingValueString.
		string mValue;
		/// \brief The sequence in which the configuration was created.
		/// \details This is used by the XDL exporter to preserve the original order in cases where 
		///		multiple configurations exist for the same setting name.
		SequenceIndex mOrder;
	// accessors
		/// \brief Set the sequence index for this configuration.
		/// \param inOrder The new sequence index.
		void setOrder(SequenceIndex inOrder) { mOrder = inOrder; }
	// constructors
		/// \brief Protected copy constructor.  This constructor allows the caller to specify the 
		///		sequence index.
		/// \param inName The configuration name.  The is a user-specified name, not a setting name.
		/// \param inValue The configuration value.
		/// \param inOrder The sequence index.
		Config(const string& inName, const string& inValue, SequenceIndex inOrder) 
			: Named(inName), mValue(inValue), mOrder(inOrder) {}
	public:
	// constructors
		/// \brief Null constructor required by collections.
		Config(void) 
			: Named(sConfigDefaultName), mValue(sConfigDefaultValue), mOrder(eSequenceIndexNone) {}
		/// \brief Standard constructor.
		/// \param inName The configuration name.  The is a user-specified name, not a setting name.
		/// \param inValue The configuration value.
		Config(const string& inName, const string& inValue) 
			: Named(inName), mValue(inValue), mOrder(eSequenceIndexNone) {}
	// accessors
		/// \brief Return the configuration value.
		const string& getValue(void) const { return mValue; }
		/// \brief Return the configuration sequence index.
		SequenceIndex getOrder(void) const { return mOrder; }
		/// \brief Set the configuration value.
		void setValue(const string& inValue) { mValue = inValue; }
		/// \brief Sets the object name.
		/// \details Config names can be changed arbitrarily, but other Named subclasses require 
		///		more complex semantics to prevent name collisions.
		void setName(const string& inName) { mName = inName; }
	// operators
		/// \brief Comparison operator.
		/// \returns true if specified configuration matches this one in both name and value, or 
		///		false otherwise.
		bool operator ==(const Config& rhs) const 
			{ return mName == rhs.mName && mValue == rhs.mValue; }
	};

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_CONFIG_HPP
