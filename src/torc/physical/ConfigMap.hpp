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
/// \brief Header for the ConfigMap class.

#ifndef TORC_PHYSICAL_CONFIGMAP_HPP
#define TORC_PHYSICAL_CONFIGMAP_HPP

#include "torc/physical/Config.hpp"
#include <map>

namespace torc {
namespace physical {

	/// \brief Configuration setting map.
	/// \details A configuration map is a collection of {setting:name:value} triplets, used to 
	///		represent additional information for netlist design elements.  Every physical netlist 
	///		element that can be configured inherits from this class:  Design, Module, Instance, Net.
	/// \details Special Xilinx settings beginning with underscores are permitted to exist multiple 
	///		times in the same ConfigMap.  This is consistent with XDL usage, particularly in the 
	///		case of XDL design statements.
	/// \note The name mentioned here is a user-specified name stemming from the design.  It is not 
	///		the configuration setting name.  For example, in configuration "DFF:blink:#FF", "DFF" 
	///		is the specified setting, "blink" is the name that the design assigns to the 
	///		corresponding resource, and "#FF" is the value to which "DFF" is set.
	class ConfigMap : private std::multimap<std::string, Config> {
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
		/// \brief Convenience typedef to represent our superclass.
		typedef std::multimap<std::string, Config> super;
	// members
		/// \brief Sequence index to use for the next configuration added to this map.
		SequenceIndex mNextSequenceIndex;
	public:
	// types
		/// \brief Constant iterator to {setting,Config} pairs.
		typedef const_iterator const_iterator;
	// constructors
		/// \brief Null constructor.
		ConfigMap(void) : super(), mNextSequenceIndex(eSequenceIndexFirst) {}
	// iterators
		/// \brief Returns the begin constant iterator for configurations.
		const_iterator configBegin(void) const { return super::begin(); }
		/// \brief Returns the end constant iterator for configurations.
		const_iterator configEnd(void) const { return super::end(); }
	// functions
		/// \brief Returns the number of configurations in the map.
		size_t getConfigCount(void) const { return super::size(); }
		/// \brief Returns true if the configuration map is empty.
		bool configIsEmpty(void) const { return super::empty(); }
		/// \brief Clears the configuration map.
		void clearConfig(void) { ConfigMap::clear(); }
		/// \brief Returns true if the specified setting exists in the map.
		bool hasConfig(const string& inSetting) const { 
			/// \todo Acquire mutex.
			const_iterator result = find(inSetting);
			return result != end();
			/// \todo Release mutex.
		}
		/// \brief Looks up the specified setting in the map.
		/// \param inSetting The setting to query.
		/// \param outConfig Reference to a configuration to be populated if the setting exists in 
		///		the map.  Default values are used if the setting does not exist.
		/// \note If this is a special setting with multiple configuration entries, only the first 
		///		one will be placed in outConfig.
		/// \returns true if the settings exists in the map, or false otherwise.
		bool getConfig(const string& inSetting, Config& outConfig) {
			// employ friendship status to reach in and reference the config name and value members
			return getConfig(inSetting, outConfig.mName, outConfig.mValue);
		}
		/// \brief Looks up the specified setting in the map.
		/// \param inSetting The setting to query.
		/// \param outName Reference to a string to accept the configuration name, or the default 
		///		name if the setting does not exist.
		/// \param outValue Reference to a string to accept the configuration value, or the default 
		///		value if the setting does not exist.
		/// \note If this is a special setting with multiple configuration entries, only the first 
		///		one will be placed in outName and outValue.
		/// \returns true if the settings exists in the map, or false otherwise.
		bool getConfig(const string& inSetting, string& outName, string& outValue) {
			/// \todo Acquire mutex.
			iterator result = find(inSetting);
			if(result == end()) {
				// if the key doesn't exist, return default values
				outName = Config::sConfigDefaultName;
				outValue = Config::sConfigDefaultValue;
				return false;
			} else {
				// otherwise return the config data
				Config& config = result->second;
				outName = config.getName();
				outValue = config.getValue();
				return true;
			}
			/// \todo Release mutex.
		}
		/// \brief Sets the configuration for the given setting.
		/// \details If this is a regular setting, then any existing configuration for the setting 
		///		will be replaced, but if this is a special setting for which multiple 
		///		configurations are allowed, it will be added to the map alongside the existing 
		///		configurations.
		/// \param inSetting The setting of interest.
		/// \param inConfig The configuration to set.
		void setConfig(const string& inSetting, const Config& inConfig) {
			setConfig(inSetting, inConfig.getName(), inConfig.getValue());
		}
		/// \brief Sets the configuration for the given setting.
		/// \details If this is a regular setting, then any existing configuration for the setting 
		///		will be replaced, but if this is a special setting for which multiple 
		///		configurations are allowed, it will be added to the map alongside the existing 
		///		configurations.
		/// \param inSetting The setting of interest.
		/// \param inName The configuration name to set.
		/// \param inValue The configuration value to set.
		void setConfig(const string& inSetting, const string& inName, const string& inValue) {
			/// \todo Acquire mutex.
			// if duplicates are disallowed for this setting, erase any matching entries
			if(!allowConfigDuplicates(inSetting)) {
				std::pair<iterator, iterator> range = equal_range(inSetting);
				if(range.first != range.second) erase(range.first, range.second);
			}
			// insert the config entry
			insert(make_pair(inSetting, Config(inName, inValue, mNextSequenceIndex++)));
			/// \todo Release mutex.
		}
		/// \brief Merges the configurations from the given ConfigMap into this one.
		/// \details For each setting, if the setting does not already exist in the map, it is 
		///		added.  If the setting does exist, then the incoming configuration either replaces 
		///		the existing setting (in the case of regular settings), or is added to the map (in 
		///		the case of special settings for which multiple configurations are allowed).
		void addConfigs(const ConfigMap& inConfigMap) {
			if(inConfigMap.empty()) return;
			/// \todo Acquire mutex.
			const_iterator p = inConfigMap.begin();
			const_iterator e = inConfigMap.end();
			while(p != e) {
				// look up the config information
				const string& setting = p->first;
				const Config& config = p->second;
				// try to insert the setting into the map (while respecting our special semantics)
				setConfig(setting, config);
				p++;
			}
			/// \todo Release mutex.
		}
		/// \brief Returns true if multiple configurations are allowed for the given setting.
		/// \details Special Xilinx settings prefixed with an underscore may have multiple 
		///		configurations in the map.
		static bool allowConfigDuplicates(const string& inSetting) {
			return inSetting.size() >= 1 && inSetting[0] == '_';
		}
		/// \brief Returns a range that encompasses all of the configurations for the given setting.
		/// \returns An iterator pair that encompasses all configurations for the setting.  Refer 
		///		to std::pair to determine how to extract the iterators.
		std::pair<iterator, iterator> getMultiConfigValues(const string& inSetting) 
			{ return equal_range(inSetting); }
		/// \brief Returns the number of configurations for the given setting.
		size_type getMultiConfigCount(const string& inSetting) { return count(inSetting); }
	};

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_CONFIGMAP_HPP
