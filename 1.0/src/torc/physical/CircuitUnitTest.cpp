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
/// \brief Unit test for the Circuit class.

#include <boost/test/unit_test.hpp>
#include "torc/physical/Factory.hpp"
#include "torc/physical/Circuit.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical)

typedef boost::shared_ptr<class RenamableInstance> RenamableInstanceSharedPtr;
class RenamableInstance : public Instance {
public:
	void setName(const string& inName) { mName = inName; }
	static RenamableInstanceSharedPtr& recast(InstanceSharedPtr& inInstancePtr) {
		return reinterpret_cast<RenamableInstanceSharedPtr&>(inInstancePtr);
	}
};

/// \brief Unit test for the Circuit class.
BOOST_AUTO_TEST_CASE(CircuitUnitTest) {
	// functions tested:
	//		Circuit(const string& inName);
	//		size_t getInstanceCount(void) const;
	//		size_t getNetCount(void) const;
	//		InstanceSharedPtrIterator instancesBegin(void);
	//		InstanceSharedPtrIterator instancesEnd(void);
	//		InstanceSharedPtrIterator netsBegin(void);
	//		InstanceSharedPtrIterator netsEnd(void);
	// create a module, since we cannot create a circuit directly
	ModuleSharedPtr circuitPtr = Factory::newModulePtr("module", "anchor");
	// verify that the circuit exists, and that it is empty
	BOOST_REQUIRE(circuitPtr.get() != 0);
	BOOST_CHECK(circuitPtr->getInstanceCount() == 0);
	BOOST_CHECK(circuitPtr->getNetCount() == 0);
	BOOST_CHECK(circuitPtr->instancesBegin() == circuitPtr->instancesEnd());
	BOOST_CHECK(circuitPtr->netsBegin() == circuitPtr->netsEnd());

	// create a few instances and nets, and add them to the circuit
	InstanceSharedPtr instance1aPtr = Factory::newInstancePtr("instance", "SLICEL", "CLBLL_X16Y59", 
		"SLICE_X27Y59");
	InstanceSharedPtr instance1bPtr = Factory::newInstancePtr("instance", "dummy", "dummy", 
		"dummy"); // deliberate reuse of instance name
	NetSharedPtr net1Ptr = Factory::newNetPtr("net1");
	net1Ptr->addPip(Factory::newPip("CLBLL_X16Y59", "L_DQ", "SITE_LOGIC_OUTS3", 
		ePipUnidirectionalBuffered));
	NetSharedPtr net2aPtr = Factory::newNetPtr("net2");
	NetSharedPtr net2bPtr = Factory::newNetPtr("net2");	// deliberate reuse of net name
	NetSharedPtr net3Ptr = Factory::newNetPtr("net3");
	//RenamableInstance::recast(instance1aPtr)->setName("goodbye");

	// functions tested:
	//		bool addInstance(InstanceSharedPtr& inInstancePtr);
	//		bool addNet(NetSharedPtr& inNetPtr);
	//		InstanceSharedPtrIterator findInstance(const string& inName);
	//		NetSharedPtrIterator findNet(const string& inName);
	//		size_t getInstanceCount(void) const;
	//		size_t getNetCount(void) const;
	BOOST_CHECK_EQUAL(circuitPtr->addInstance(instance1aPtr), true);
	BOOST_CHECK_EQUAL(circuitPtr->addInstance(instance1bPtr), false); // name already exists
	BOOST_CHECK_EQUAL(circuitPtr->addNet(net1Ptr), true);
	BOOST_CHECK_EQUAL(circuitPtr->addNet(net2aPtr), true);
	BOOST_CHECK_EQUAL(circuitPtr->addNet(net2bPtr), false); // name already exists
	BOOST_CHECK_EQUAL(circuitPtr->addNet(net3Ptr), true);
	BOOST_CHECK(circuitPtr->getInstanceCount() == 1);
	BOOST_CHECK(circuitPtr->getNetCount() == 3);

	// functions tested:
	//		void unplace(void);
	//		void unroute(void);
	// unplace and unroute the circuit
	BOOST_CHECK(instance1aPtr->getTile().length() != 0);
	BOOST_CHECK(instance1aPtr->getSite().length() != 0);
	BOOST_CHECK(net1Ptr->getPipCount() != 0);
	circuitPtr->unplace();
	circuitPtr->unroute();
	BOOST_CHECK(instance1aPtr->getTile().length() == 0);
	BOOST_CHECK(instance1aPtr->getSite().length() == 0);
	BOOST_CHECK(net1Ptr->getPipCount() == 0);

	// functions tested:
	//		bool removeInstance(InstanceSharedPtr& inInstancePtr);
	//		bool removeNet(NetSharedPtr& inNetPtr);
	//		size_t getInstanceCount(void) const;
	//		size_t getNetCount(void) const;
	// find and remove instances and nets
	BOOST_CHECK(circuitPtr->removeInstance(*circuitPtr->findInstance("instance")));
	BOOST_CHECK(circuitPtr->removeNet(*circuitPtr->findNet("net1")));
	BOOST_CHECK(circuitPtr->removeNet(*circuitPtr->findNet("net2")));
	BOOST_CHECK(circuitPtr->removeNet(*circuitPtr->findNet("net3")));
	BOOST_CHECK(circuitPtr->getInstanceCount() == 0);
	BOOST_CHECK(circuitPtr->getNetCount() == 0);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
