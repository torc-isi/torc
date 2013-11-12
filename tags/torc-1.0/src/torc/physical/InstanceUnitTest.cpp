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
/// \brief Unit test for the Instance class.

#include <boost/test/unit_test.hpp>
#include "torc/physical/Factory.hpp"
#include "torc/physical/Instance.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical)

/// \brief Unit test for the Instance class.
BOOST_AUTO_TEST_CASE(InstanceUnitTest) {
	// create accessory instances and module
	std::string name = "name";
	std::string type = "type";
	std::string tile = "tile";
	std::string site = "site";
	ModuleSharedPtr modulePtr = Factory::newModulePtr("name", "anchor");
	InstanceSharedPtr instance2Ptr = Factory::newInstancePtr(name, "type", "tile", "site");
	InstanceReferenceSharedPtr instanceReferencePtr = Factory::newInstanceReferencePtr("name", 
		modulePtr, instance2Ptr);

	// functions tested:
	//		Instance(const string& inName, const string& inType, const string& inTile, 
	//			const string& inSite, EInstanceBonding inBonding, 
	//			InstanceReferenceSharedPtr& inInstanceReferencePtr);
	// create an instance
	EInstanceBonding bonding = eInstanceBondingBonded;
	InstanceSharedPtr instance1Ptr = Factory::newInstancePtr(name, "", "", "");
	BOOST_REQUIRE(instance1Ptr != 0);

	// functions tested:
	//		void setType(const string& inType);
	//		void setTile(const string& inTile);
	//		void setSite(const string& inSite);
	//		void setBonding(EInstanceBonding inBonding);
	//		void setInstanceReferencePtr(InstanceReferenceSharedPtr inInstanceReferenceSharedPtr);
	//		const string& getType(void) const;
	//		const string& getTile(void) const;
	//		const string& getSite(void) const;
	//		EInstanceBonding getBonding(void) const;
	//		InstanceReferenceSharedPtr getInstanceReferencePtr(void) const;
	BOOST_CHECK(instance1Ptr->getType().empty());
	BOOST_CHECK(instance1Ptr->getTile().empty());
	BOOST_CHECK(instance1Ptr->getSite().empty());
	BOOST_CHECK(instance1Ptr->getBonding() == eInstanceBondingUnknown);
	instance1Ptr->setType(type);
	instance1Ptr->setTile(tile);
	instance1Ptr->setSite(site);
	instance1Ptr->setBonding(bonding);
	instance1Ptr->setInstanceReferencePtr(instanceReferencePtr);
	BOOST_CHECK(instance1Ptr->getType() == type);
	BOOST_CHECK(instance1Ptr->getTile() == tile);
	BOOST_CHECK(instance1Ptr->getSite() == site);
	BOOST_CHECK(instance1Ptr->getBonding() == bonding);
	BOOST_CHECK(instance1Ptr->getInstanceReferencePtr() == instanceReferencePtr);

	// functions tested:
	//		bool operator ==(const Instance& rhs) const;
	BOOST_CHECK(*instance1Ptr == *instance2Ptr); // comparison based on instance names only

	// functions tested:
	//		void unplace(void);
	instance1Ptr->unplace();
	BOOST_CHECK(instance1Ptr->getTile().empty());
	BOOST_CHECK(instance1Ptr->getSite().empty());

	// functions tested:
	//		void addPin(const InstancePinWeakPtr& inInstancePinPtr);
	//		void removePin(const InstancePinWeakPtr& inInstancePinPtr);
	//		const InstancePinSharedPtrConstIterator findPin(const PinName& inPinName);
	PinName pinName1("name1");
	PinName pinName2("name2");
	InstancePinSharedPtr instancePin1Ptr = Factory::newInstancePinPtr(instance1Ptr, pinName1);
	InstancePinSharedPtr instancePin2Ptr = Factory::newInstancePinPtr(instance1Ptr, pinName1);
	InstancePinSharedPtr instancePin3Ptr = Factory::newInstancePinPtr(instance1Ptr, pinName2);
	NetSharedPtr netPtr = Factory::newNetPtr("name");
	netPtr->addSource(instancePin1Ptr);
	netPtr->addSource(instancePin2Ptr);
	netPtr->addSink(instancePin3Ptr);
	BOOST_CHECK_EQUAL(instance1Ptr->getPinCount(), 3u);
	BOOST_CHECK_EQUAL(instance1Ptr->getPinCount(pinName1), 2u);
	BOOST_CHECK_EQUAL(instance1Ptr->getPinCount(pinName2), 1u);
	Instance::InstancePinSharedPtrConstIterator ipp = instance1Ptr->findPin(pinName1);
	Instance::InstancePinSharedPtrConstIterator ipe = instance1Ptr->pinsEnd();
	BOOST_REQUIRE(ipp != ipe);
	BOOST_CHECK(ipp->second == instancePin1Ptr);
	BOOST_CHECK_EQUAL(ipp->second->getPinName(), pinName1);
	BOOST_CHECK_EQUAL(ipp->second->getPinName(), ipp->first);
	BOOST_CHECK(ipp->second->getParentWeakPtr().lock() == netPtr);
	std::pair<Instance::InstancePinSharedPtrConstIterator, 
		Instance::InstancePinSharedPtrConstIterator> range = instance1Ptr->findPinRange(pinName1);
	BOOST_REQUIRE(range.first != range.second);
	BOOST_CHECK_EQUAL(range.first->second->getPinName(), pinName1); range.first++;
	BOOST_CHECK_EQUAL(range.first->second->getPinName(), pinName1); range.first++;
	BOOST_CHECK(range.first == range.second);
	netPtr->removeSource(instancePin2Ptr);
	netPtr->removeSink(instancePin3Ptr);
	BOOST_CHECK_EQUAL(instance1Ptr->getPinCount(), 1u);
	BOOST_CHECK_EQUAL(instance1Ptr->getPinCount(pinName1), 1u);
	BOOST_CHECK_EQUAL(instance1Ptr->getPinCount(pinName2), 0u);
	ipp = instance1Ptr->findPin(pinName1);
	ipe = instance1Ptr->pinsEnd();
	BOOST_REQUIRE(ipp != ipe);
	BOOST_CHECK(ipp->second == instancePin1Ptr);
	BOOST_CHECK_EQUAL(ipp->second->getPinName(), pinName1);
	BOOST_CHECK_EQUAL(ipp->second->getPinName(), ipp->first);
	BOOST_CHECK(ipp->second->getParentWeakPtr().lock() == netPtr);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
