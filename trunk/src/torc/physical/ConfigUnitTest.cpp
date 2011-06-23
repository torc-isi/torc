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
/// \brief Source for the Config unit test.

#include <boost/test/unit_test.hpp>
#include "torc/physical/Config.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical)

/// \brief Unit test for the Config class.
BOOST_AUTO_TEST_CASE(ConfigUnitTest) {
	// functions tested:
	//		Config(const string& inName, const string& inValue);
	//		Config(const string& inName, const string& inValue, SequenceIndex inOrder);
	//		Config(void);
	// create a config
	std::string name1 = "name1";
	std::string name2 = "name2";
	std::string name3 = "name3";
	std::string value1 = "value1";
	std::string value2 = "value2";
	std::string value3 = "value3";
	SequenceIndex order1(0);
	SequenceIndex order2(9);
	SequenceIndex order3(10);
	Config config1(name1, value1);
	Config config2("name3", "value3", order2);
	Config config3;

	// functions tested:
	//		const string& Named::getName(void) const;
	//		const string& getValue(void) const;
	//		SequenceIndex getOrder(void) const;
	//		void setOrder(SequenceIndex inOrder);
	BOOST_CHECK_EQUAL(config1.getName(), name1);
	BOOST_CHECK_EQUAL(config1.getValue(), value1);
	config3.setOrder(order3);
	BOOST_CHECK(config1.getOrder() == order1);
	BOOST_CHECK(config2.getOrder() == order2);
	BOOST_CHECK(config3.getOrder() == order3);

	// functions tested:
	//		void setValue(const string& inValue);
	//		void setName(const string& inName);
	//		const string& Named::getName(void) const;
	//		const string& getValue(void) const;
	config1.setName(name2);
	config1.setValue(value2);
	BOOST_CHECK_EQUAL(config1.getName(), name2);
	BOOST_CHECK_EQUAL(config1.getValue(), value2);

	// functions tested:
	//		bool operator ==(const Config& rhs) const;
	BOOST_CHECK((config1 == config2) == false);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
