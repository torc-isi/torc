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
/// \brief Source for the Module unit test.

#include <boost/test/unit_test.hpp>
#include "torc/physical/Factory.hpp"
#include "torc/physical/Module.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical_database)

/// \brief Unit test for the Module class.
BOOST_AUTO_TEST_CASE(physical_module) {
	std::string port = "name1";
	// create accessory instance and ports
	InstanceSharedPtr instancePtr = Factory::newInstancePtr("name", "type", "tile", "site");
	PortSharedPtr port1Ptr = Factory::newPortPtr(port, instancePtr, "pin");
	PortSharedPtr port2aPtr = Factory::newPortPtr("name2", instancePtr, "pin");
	PortSharedPtr port2bPtr = Factory::newPortPtr("name2", instancePtr, "pin");
	// functions tested:
	//		Module(const string& inName, const string& inAnchor);
	// create two modules
	std::string name = "name";
	std::string anchor = "anchor";
	ModuleSharedPtr module1Ptr = Factory::newModulePtr(name, "");
	ModuleSharedPtr module2Ptr = Factory::newModulePtr(name, anchor);
	BOOST_REQUIRE(module1Ptr != 0);
	BOOST_REQUIRE(module2Ptr != 0);

	// functions tested:
	//		const string& getAnchor(void) const;
	//		void setAnchor(const string& inAnchor);
	BOOST_CHECK(module1Ptr->getAnchor().empty());
	module1Ptr->setAnchor(anchor);
	BOOST_CHECK(module1Ptr->getAnchor() == anchor);

	// functions tested:
	//		PortSharedPtrIterator findPort(const string& inName);
	//		bool addPort(PortSharedPtr& inPortPtr);
	//		size_t getPortCount(void) const;
	BOOST_CHECK_EQUAL(module1Ptr->addPort(port1Ptr), true);
	BOOST_CHECK_EQUAL(module1Ptr->addPort(port2aPtr), true);
	BOOST_CHECK_EQUAL(module1Ptr->addPort(port2bPtr), false); // port name already exists
	BOOST_CHECK(module1Ptr->getPortCount() == 2);

	// functions tested:
	//		PortSharedPtrConstIterator portsBegin(void) const;
	//		PortSharedPtrConstIterator portsEnd(void) const;
	//		PortSharedPtrIterator portsBegin(void);
	//		PortSharedPtrIterator portsEnd(void);
	Module::PortSharedPtrConstIterator p = module1Ptr->portsBegin();
	Module::PortSharedPtrConstIterator e = module1Ptr->portsEnd();
	BOOST_CHECK(*p++ == port1Ptr);
	BOOST_CHECK(*p++ == port2aPtr);
	BOOST_CHECK(p == e);
	BOOST_CHECK(*(module1Ptr->findPort(port)) == port1Ptr);

	// functions tested:
	//		PortSharedPtrIterator findPort(const string& inName);
	//		bool removePort(PortSharedPtr& inPortPtr);
	//		size_t getPortCount(void) const;
	BOOST_CHECK_EQUAL(module1Ptr->removePort(port1Ptr), true);
	BOOST_CHECK_EQUAL(module1Ptr->removePort(port2aPtr), true);
	BOOST_CHECK_EQUAL(module1Ptr->removePort(port2bPtr), false);
	BOOST_CHECK(module1Ptr->getPortCount() == 0);

	// functions tested:
	//		bool operator ==(const Module& rhs) const;
	BOOST_CHECK(*module1Ptr == *module2Ptr); // comparison based on module names only
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
