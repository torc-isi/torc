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
/// \brief Source for the Design unit test.

#include <boost/test/unit_test.hpp>
#include "torc/physical/Factory.hpp"
#include "torc/physical/Design.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical)

/// \brief Unit test for the Design class.
/// \todo Verify that all children can trace their lineage all the way up to the design.
BOOST_AUTO_TEST_CASE(DesignUnitTest) {
	// functions tested:
	//		Design(const string& inName, const string& inDevice, const string& inPackage, 
	//			const string& inSpeedGrade, const string& inXdlVersion);
	// create a design
	std::string designName = "design";
	std::string deviceName = "device";
	std::string devicePackage = "package";
	std::string deviceSpeedGrade = "speed_grade";
	std::string xdlVersion = "xdl_version";
	std::string moduleName = "module1";
	DesignSharedPtr designPtr = Factory::newDesignPtr(designName, "", "", "", "");
	BOOST_REQUIRE(designPtr.get() != 0);

	// functions tested:
	//		const string& getDevice(void) const;
	//		const string& getPackage(void) const;
	//		const string& getSpeedGrade(void) const;
	//		const string& getXdlVersion(void) const;
	//		void setDevice(const string& inDevice);
	//		void setPackage(const string& inPackage);
	//		void setSpeedGrade(const string& inSpeedGrade);
	//		void setXdlVersion(const string& inXdlVersion);
	BOOST_CHECK(designPtr->getDevice().empty());
	BOOST_CHECK(designPtr->getPackage().empty());
	BOOST_CHECK(designPtr->getSpeedGrade().empty());
	BOOST_CHECK(designPtr->getXdlVersion().empty());
	designPtr->setDevice(deviceName);
	designPtr->setPackage(devicePackage);
	designPtr->setSpeedGrade(deviceSpeedGrade);
	designPtr->setXdlVersion(xdlVersion);
	BOOST_CHECK_EQUAL(designPtr->getName(), designName);
	BOOST_CHECK_EQUAL(designPtr->getDevice(), deviceName);
	BOOST_CHECK_EQUAL(designPtr->getPackage(), devicePackage);
	BOOST_CHECK_EQUAL(designPtr->getSpeedGrade(), deviceSpeedGrade);
	BOOST_CHECK_EQUAL(designPtr->getXdlVersion(), xdlVersion);

	// functions tested:
	//		ModuleSharedPtrIterator findModule(const string& inName);
	//		bool addModule(ModuleSharedPtr& inModulePtr);
	//		size_t getModuleCount(void) const;
	// create and add some modules to the design
	ModuleSharedPtr module1Ptr = Factory::newModulePtr(moduleName, "anchor1");
	ModuleSharedPtr module2aPtr = Factory::newModulePtr("module2", "anchor2");
	ModuleSharedPtr module2bPtr = Factory::newModulePtr("module2", "anchor2");
	BOOST_CHECK_EQUAL(designPtr->addModule(module1Ptr), true);
	BOOST_CHECK_EQUAL(designPtr->addModule(module2aPtr), true);
	BOOST_CHECK_EQUAL(designPtr->addModule(module2bPtr), false);
	BOOST_CHECK(designPtr->getModuleCount() == 2);

	// functions tested:
	//		ModuleSharedPtrIterator findModule(const string& inName);
	//		ModuleSharedPtrConstIterator modulesBegin(void) const;
	//		ModuleSharedPtrConstIterator modulesEnd(void) const;
	//		ModuleSharedPtrIterator modulesBegin(void);
	//		ModuleSharedPtrIterator modulesEnd(void);
	Design::ModuleSharedPtrConstIterator p = designPtr->modulesBegin();
	Design::ModuleSharedPtrConstIterator e = designPtr->modulesEnd();
	BOOST_CHECK(*p++ == module1Ptr);
	BOOST_CHECK(*p++ == module2aPtr);
	BOOST_CHECK(p == e);
	BOOST_CHECK(*(designPtr->findModule(moduleName)) == module1Ptr);

	// functions tested:
	//		ModuleSharedPtrIterator findModule(const string& inName);
	//		bool removeModule(ModuleSharedPtr& inModulePtr);
	//		size_t getModuleCount(void) const;
	// remove the modules
	BOOST_CHECK_EQUAL(designPtr->removeModule(module1Ptr), true);
	BOOST_CHECK_EQUAL(designPtr->removeModule(module2aPtr), true);
	BOOST_CHECK_EQUAL(designPtr->removeModule(module2bPtr), false);
	BOOST_CHECK(designPtr->getModuleCount() == 0);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
