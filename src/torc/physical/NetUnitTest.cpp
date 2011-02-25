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
/// \brief Source for the Net unit test.

#include <boost/test/unit_test.hpp>
#include "torc/physical/Factory.hpp"
#include "torc/physical/Net.hpp"

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical_database)

/// \brief Unit test for the Net class.
BOOST_AUTO_TEST_CASE(physical_net) {
	// accessory instance and pins
	std::string name = "name";
	// create accessory instance, pins, and pips
	InstanceSharedPtr instancePtr = Factory::newInstancePtr("name", "type", "tile", "site");
	InstancePinSharedPtr pin1Ptr = Factory::newInstancePinPtr(instancePtr, "pin1");
	InstancePinSharedPtr pin2Ptr = Factory::newInstancePinPtr(instancePtr, "pin2");
	InstancePinSharedPtr pin3Ptr = Factory::newInstancePinPtr(instancePtr, "pin3");
	InstancePinSharedPtr pin4Ptr = Factory::newInstancePinPtr(instancePtr, "pin4");
	Pip pip1 = Factory::newPip("tile", "source1", "sink1", ePipUnidirectionalBuffered);
	Pip pip2 = Factory::newPip("tile", "source2", "sink2", ePipUnidirectionalBuffered);
	Pip pip3 = Factory::newPip("tile", "source3", "sink3", ePipUnidirectionalBuffered);

	// functions tested:
	//		Net(const string& inName, ENetType inNetType);
	ENetType normal = eNetTypeNormal;
	ENetType power = eNetTypePower;
	ENetType ground = eNetTypeGround;
	NetSharedPtr net1Ptr = Factory::newNetPtr(name);
	NetSharedPtr net2Ptr = Factory::newNetPtr(name, power);
	BOOST_REQUIRE(net1Ptr != 0);
	BOOST_REQUIRE(net2Ptr != 0);

	// functions tested:
	//		ENetType getNetType(void) const;
	//		void setNetType(ENetType inNetType);
	BOOST_CHECK(net1Ptr->getNetType() == normal);
	BOOST_CHECK(net2Ptr->getNetType() == power);
	net2Ptr->setNetType(ground);
	BOOST_CHECK(net2Ptr->getNetType() == ground);

	// functions tested:
	//		void addSource(const InstancePin& inInstancePin);
	//		void addSink(const InstancePin& inInstancePin);
	//		bool containsSource(const InstancePin& inInstancePin) const;
	//		bool containsSink(const InstancePin& inInstancePin) const;
	//		bool hasAnySources(void) const;
	//		bool hasOneSource(void) const;
	//		bool hasMultipleSources(void) const;
	//		size_t getSourceCount(void) const;
	//		bool hasAnySinks(void) const;
	//		bool hasOneSink(void) const;
	//		bool hasMultipleSinks(void) const;
	//		size_t getSinkCount(void) const;
	net1Ptr->addSource(pin1Ptr);
	net1Ptr->addSink(pin2Ptr);
	net1Ptr->addSink(pin3Ptr);
	net1Ptr->addSink(pin4Ptr);
	BOOST_CHECK_EQUAL(net1Ptr->containsSource(pin1Ptr), true);
	BOOST_CHECK_EQUAL(net1Ptr->containsSink(pin1Ptr), false);
	BOOST_CHECK_EQUAL(net1Ptr->containsSink(pin2Ptr), true);
	BOOST_CHECK_EQUAL(net1Ptr->containsSink(pin3Ptr), true);
	BOOST_CHECK_EQUAL(net1Ptr->containsSink(pin4Ptr), true);
	BOOST_CHECK_EQUAL(net1Ptr->hasAnySources(), true);
	BOOST_CHECK_EQUAL(net1Ptr->hasOneSource(), true);
	BOOST_CHECK_EQUAL(net1Ptr->hasMultipleSources(), false);
	BOOST_CHECK(net1Ptr->getSourceCount() == 1);
	BOOST_CHECK_EQUAL(net1Ptr->hasAnySinks(), true);
	BOOST_CHECK_EQUAL(net1Ptr->hasOneSink(), false);
	BOOST_CHECK_EQUAL(net1Ptr->hasMultipleSinks(), true);
	BOOST_CHECK(net1Ptr->getSinkCount() == 3);

	// functions tested:
	//		bool removeSource(const InstancePin& inInstancePin);
	//		bool removeSink(const InstancePin& inInstancePin);
	//		InstancePinConstIterator sourcesBegin(void) const;
	//		InstancePinConstIterator sourcesEnd(void) const;
	//		InstancePinConstIterator sinksBegin(void) const;
	//		InstancePinConstIterator sinksEnd(void) const;
	Net::InstancePinSharedPtrConstIterator op = net1Ptr->sourcesBegin();
	Net::InstancePinSharedPtrConstIterator oe = net1Ptr->sourcesEnd();
	BOOST_CHECK(*op++ == pin1Ptr);
	BOOST_CHECK(op == oe);
	BOOST_CHECK_EQUAL(net1Ptr->removeSource(pin1Ptr), true);
	BOOST_CHECK_EQUAL(net1Ptr->removeSource(pin2Ptr), false);
	BOOST_CHECK(net1Ptr->getSourceCount() == 0);
	Net::InstancePinSharedPtrConstIterator ip = net1Ptr->sinksBegin();
	Net::InstancePinSharedPtrConstIterator ie = net1Ptr->sinksEnd();
	BOOST_CHECK(*ip++ == pin2Ptr);
	BOOST_CHECK(*ip++ == pin3Ptr);
	BOOST_CHECK(*ip++ == pin4Ptr);
	BOOST_CHECK(ip == ie);
	BOOST_CHECK_EQUAL(net1Ptr->removeSink(pin1Ptr), false);
	BOOST_CHECK_EQUAL(net1Ptr->removeSink(pin2Ptr), true);
	BOOST_CHECK_EQUAL(net1Ptr->removeSink(pin3Ptr), true);
	BOOST_CHECK_EQUAL(net1Ptr->removeSink(pin4Ptr), true);
	BOOST_CHECK(net1Ptr->getSinkCount() == 0);

	// functions tested:
	//		void addPip(const Pip& inPip);
	//		bool containsPip(const Pip& inPip) const;
	//		bool hasAnyPips(void) const;
	//		size_t getPipCount(void) const;
	//		bool isRouted(void) const;
	//		bool isUnrouted(void) const;
	net1Ptr->addPip(pip1);
	net1Ptr->addPip(pip2);
	net1Ptr->addPip(pip3);
	BOOST_CHECK_EQUAL(net1Ptr->containsPip(pip1), true);
	BOOST_CHECK_EQUAL(net1Ptr->containsPip(pip2), true);
	BOOST_CHECK_EQUAL(net1Ptr->containsPip(pip3), true);
	BOOST_CHECK_EQUAL(net1Ptr->hasAnyPips(), true);
	BOOST_CHECK_EQUAL(net1Ptr->isRouted(), true);
	BOOST_CHECK_EQUAL(net1Ptr->isUnrouted(), false);
	BOOST_CHECK(net1Ptr->getPipCount() == 3);

	// functions tested:
	//		bool removePip(const Pip& inPip);
	//		void unroute(void);
	//		PipConstIterator pipsBegin(void) const;
	//		PipConstIterator pipsEnd(void) const;
	//		size_t getPipCount(void) const;
	//		bool isRouted(void) const;
	//		bool isUnrouted(void) const;
	Net::PipConstIterator pp = net1Ptr->pipsBegin();
	Net::PipConstIterator pe = net1Ptr->pipsEnd();
	BOOST_CHECK(*pp++ == pip1);
	BOOST_CHECK(*pp++ == pip2);
	BOOST_CHECK(*pp++ == pip3);
	BOOST_CHECK(pp == pe);
	BOOST_CHECK_EQUAL(net1Ptr->removePip(pip1), true);
	BOOST_CHECK_EQUAL(net1Ptr->removePip(pip1), false);
	BOOST_CHECK_EQUAL(net1Ptr->isRouted(), true);
	BOOST_CHECK_EQUAL(net1Ptr->isUnrouted(), false);
	BOOST_CHECK(net1Ptr->getPipCount() == 2);
	net1Ptr->unroute();
	BOOST_CHECK_EQUAL(net1Ptr->isRouted(), false);
	BOOST_CHECK_EQUAL(net1Ptr->isUnrouted(), true);
	BOOST_CHECK(net1Ptr->getPipCount() == 0);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
