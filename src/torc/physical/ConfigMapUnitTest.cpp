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
/// \brief Source for the ConfigMap unit test.

#include <boost/test/unit_test.hpp>
#include "torc/physical/ConfigMap.hpp"
#include "torc/physical/OutputStreamHelpers.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical_database)

/// \brief Unit test for the ConfigMap class.
BOOST_AUTO_TEST_CASE(physical_config_map) {
	// create some configuration entries
	Config config1("name1", "value1");
	Config config2("name2", "value2");
	Config config3("name3", "value3");
	Config config4("name4", "value4");

	// functions tested:
	//		ConfigMap(void);
	//		void setConfig(const string& inSetting, const Config& inConfig);
	//		static bool allowConfigDuplicates(const string& inSetting);
	// create a configuration map and add the configuration entries to it
	ConfigMap configMap1;
	configMap1.setConfig("config", config1);
	configMap1.setConfig("config", config2);
	configMap1.setConfig("_config", config3);
	configMap1.setConfig("_config", config4);
	ConfigMap configMap2;

	// functions tested:
	//		bool getConfig(const string& inSetting, string& outName, string& outValue);
	//		void setConfig(const string& inSetting, const string& inName, const string& inValue);
	std::string name;
	std::string value;
	configMap1.getConfig("config", name, value);
	configMap2.setConfig("ending", name, value);

	// functions tested:
	//		bool hasConfig(const string& inSetting) const;
	//		size_type getMultiConfigCount(const string& inSetting);
	//		size_t getConfigCount(void) const;
	//		bool configIsEmpty(void) const;
	// verify that the configuration entries exist in the proper quantities
	// (settings prefixed with an underscore can exist multiple times, while other settings cannot)
	BOOST_CHECK(configMap1.hasConfig("config"));
	BOOST_CHECK(configMap1.hasConfig("_config"));
	BOOST_CHECK_EQUAL((size_t) 1, configMap1.getMultiConfigCount("config"));
	BOOST_CHECK_EQUAL((size_t) 2, configMap1.getMultiConfigCount("_config"));
	BOOST_CHECK(configMap1.getConfigCount() == 3);
	BOOST_CHECK(configMap1.configIsEmpty() == false);

	// functions tested:
	//		bool getConfig(const string& inSetting, Config& outConfig);
	//		const_iterator configBegin(void) const;
	//		const_iterator configEnd(void) const;
	// read out and check expected settings
	Config config5;
	configMap1.getConfig("config", config5);
	BOOST_CHECK_EQUAL(config2, config5);
	// verify the expected iterator behavior
	ConfigMap::const_iterator p = configMap1.configBegin();
	ConfigMap::const_iterator e = configMap1.configEnd();
	BOOST_CHECK_EQUAL(config3, p->second); p++;
	BOOST_CHECK_EQUAL(config4, p->second); p++;
	BOOST_CHECK_EQUAL(config2, p->second); p++;

	// functions tested:
	//		std::pair<iterator, iterator> getMultiConfigValues(const string& inSetting);
	std::pair<ConfigMap::const_iterator, ConfigMap::const_iterator> range 
		= configMap1.getMultiConfigValues("_config");
	p = range.first;
	e = range.second;
	BOOST_CHECK_EQUAL(config3, p->second); p++;
	BOOST_CHECK_EQUAL(config4, p->second); p++;
	BOOST_CHECK(p == e);
	//	p = configMap1.configBegin();
	//	e = configMap1.configEnd();
	//	while(p != e) {
	//		std::cout << p->first << ":" << p->second.getName() << ":" << p->second.getValue() << std::endl;
	//		p++;
	//	}

	// functions tested:
	//		void clearConfig(void);
	//		void addConfigs(const ConfigMap& inConfigMap);
	//		size_t getConfigCount(void) const;
	//		bool configIsEmpty(void) const;
	configMap2.addConfigs(configMap1);
	configMap1.clearConfig();
	BOOST_CHECK(configMap1.getConfigCount() == 0);
	BOOST_CHECK(configMap1.configIsEmpty() == true);
	BOOST_CHECK(configMap2.getConfigCount() == 4);
	BOOST_CHECK(configMap2.configIsEmpty() == false);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
