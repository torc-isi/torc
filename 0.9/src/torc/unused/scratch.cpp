// TORC - Copyright 2010 University of Southern California.  All Rights Reserved.
// $HeadURL$
// $Id$

#if 0
#include "torc/physical/xdl/parser.h"
#include "torc/physical/Named.hpp"
#include "torc/physical/Config.hpp"
#include "torc/physical/ConfigMap.hpp"
#include "torc/physical/Routethrough.hpp"
#include "torc/physical/Net.hpp"
#include "torc/physical/InstancePin.hpp"
#include "torc/physical/Instance.hpp"
#include "torc/physical/Pip.hpp"
#include "torc/physical/Module.hpp"
#include "torc/physical/Design.hpp"

#include <boost/test/unit_test.hpp>
#include "OutputStreamHelpers.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>


namespace torc {
namespace physical {


BOOST_AUTO_TEST_SUITE(physical_database)

/// \brief Unit test of scratch work.
BOOST_AUTO_TEST_CASE(scratch) {
	// create a design and verify it
	const char* designName = "design";
	Design design(designName);
	BOOST_CHECK_EQUAL(designName, design.getName());

	// create a module and verify it
	const char* moduleName = "module";
	Module module(moduleName);
	BOOST_CHECK_EQUAL(moduleName, module.getName());

	// create an instance and verify it
	const char* instanceName = "instance";
	Instance instance(instanceName);
	BOOST_CHECK_EQUAL(instanceName, instance.getName());

	// create a net and verify it
	const char* netName = "net";
	Net net(netName, eNetTypePower);
	BOOST_CHECK_EQUAL(netName, net.getName());
	BOOST_CHECK_EQUAL(eNetTypePower, net.getNetType());

	// change the net type and verity it
	net.setNetType(eNetTypeNormal);
	BOOST_CHECK_EQUAL(eNetTypeNormal, net.getNetType());

	// create some instance pins
	InstancePin instancePin1(instance, "pin1");
	InstancePin instancePin2(instance, "pin2");

	// add the instance pins to a net
	net.addSource(instancePin1);
	net.addSink(instancePin2);
	BOOST_CHECK(net.hasAnySources());
	BOOST_CHECK(net.hasOneSource());
	BOOST_CHECK(net.hasMultipleSources() == false);
	BOOST_CHECK(net.hasAnySinks());
	BOOST_CHECK(net.hasOneSink());
	BOOST_CHECK(net.hasMultipleSinks() == false);
	// verify that the pins show up in the net
	InstancePinVector::const_iterator sourcesBegin = net.sourcesBegin();
	InstancePinVector::const_iterator sourcesEnd = net.sourcesEnd();
	InstancePinVector::const_iterator sinksBegin = net.sinksBegin();
	InstancePinVector::const_iterator sinksEnd = net.sinksEnd();
	BOOST_CHECK_EQUAL(*sourcesBegin, instancePin1);
	BOOST_CHECK_EQUAL(*sinksBegin, instancePin2);
	BOOST_CHECK(net.containsSource(instancePin1));
	BOOST_CHECK(net.containsSource(instancePin2) == false);
	BOOST_CHECK(net.containsSink(instancePin1) == false);
	BOOST_CHECK(net.containsSink(instancePin2));

	// remove the instance pins from the net
	BOOST_CHECK(net.removeSource(instancePin1)); // should be found and removed
	BOOST_CHECK(net.removeSource(instancePin2) == false); // should not be found
	BOOST_CHECK(net.removeSink(instancePin1) == false); // should not be found
	BOOST_CHECK(net.removeSink(instancePin2)); // should be found and removed
	sourcesBegin = net.sourcesBegin();
	sourcesEnd = net.sourcesEnd();
	sinksBegin = net.sinksBegin();
	sinksEnd = net.sinksEnd();
	BOOST_CHECK_EQUAL(sourcesBegin, sourcesEnd);
	BOOST_CHECK_EQUAL(sinksBegin, sinksEnd);
	BOOST_CHECK(net.hasAnySources() == false);
	BOOST_CHECK(net.hasOneSource() == false);
	BOOST_CHECK(net.hasMultipleSources() == false);
	BOOST_CHECK(net.hasAnySinks() == false);
	BOOST_CHECK(net.hasOneSink() == false);
	BOOST_CHECK(net.hasMultipleSinks() == false);

	// create some pips
	Pip pip1("tile1", "source1", "sink1", ePipUnidirectionalBuffered);
	Pip pip2("tile2", "source2", "sink2", ePipUnidirectionalBuffered);
	// create a routethrough
	Routethrough routethrough("setting", "name", "value", instance, "source", "sink");

	// add the pips to the net
	net.addPip(pip1);
	net.addPip(pip2, routethrough);
	BOOST_CHECK(net.containsPip(pip1));
	BOOST_CHECK(net.containsPip(pip2));
	
	BOOST_CHECK(pip1.isRoutethrough() == false);
	BOOST_CHECK(pip2.isRoutethrough() == false);
	PipVector::const_iterator pipsBegin = net.pipsBegin();
	PipVector::const_iterator pipsEnd = net.pipsEnd();
	BOOST_CHECK((pipsBegin++)->isRoutethrough() == false);
	BOOST_CHECK((pipsBegin++)->isRoutethrough());

	// remove the pips from the net
	BOOST_CHECK(net.removePip(pip1));
	BOOST_CHECK(net.removePip(pip2));
	BOOST_CHECK(net.containsPip(pip1) == false);
	BOOST_CHECK(net.containsPip(pip2) == false);
	pipsBegin = net.pipsBegin();
	pipsEnd = net.pipsEnd();
	BOOST_CHECK_EQUAL(pipsBegin, pipsEnd);

	// create some config entries
	Config config1("name1", "value1");
	Config config2("name2", "value2");
	Config config3("name3", "value3");
	Config config4("name4", "value4");
	// create a config map and add config entries to it
	ConfigMap configMap;
	configMap.setConfig("config", config1);
	configMap.setConfig("config", config2);
	configMap.setConfig("_config", config3);
	configMap.setConfig("_config", config4);
	BOOST_CHECK(configMap.hasConfig("config"));
	BOOST_CHECK(configMap.hasConfig("_config"));
	BOOST_CHECK_EQUAL((size_t) 1, configMap.getMultiConfigCount("config"));
	BOOST_CHECK_EQUAL((size_t) 2, configMap.getMultiConfigCount("_config"));
	Config config5;
	configMap.getConfig("config", config5);
	BOOST_CHECK_EQUAL(config2, config5);
	ConfigMap::const_iterator p = configMap.begin();
	ConfigMap::const_iterator e = configMap.end();
	BOOST_CHECK_EQUAL(config3, p->second); p++;
	BOOST_CHECK_EQUAL(config4, p->second); p++;
	BOOST_CHECK_EQUAL(config2, p->second); p++;
	std::pair<ConfigMap::const_iterator, ConfigMap::const_iterator> range 
		= configMap.getMultiConfigValues("_config");
	p = range.first;
	e = range.second;
	while(p != e) {
		std::cout << p->first << ":" << p->second.getName() << ":" << p->second.getValue() << std::endl;
		p++;
	}

	// add the net and the instance to the module
	module.addInstance(instance);
	module.addNet(net);
	// add the module to the design
	design.addModule(module);

	// ensure that the module exists in the design
	ModuleVector::const_iterator modulesBegin = design.modulesBegin();
	ModuleVector::const_iterator modulesEnd = design.modulesEnd();
	BOOST_CHECK_EQUAL(*modulesBegin, module);
	// ensure that the instance and net exist in the module
	InstanceVector::const_iterator instancesBegin = module.instancesBegin();
	InstanceVector::const_iterator instancesEnd = module.instancesEnd();
	NetVector::const_iterator netsBegin = module.netsBegin();
	NetVector::const_iterator netsEnd = module.netsEnd();
	BOOST_CHECK_EQUAL(*instancesBegin, instance);
	BOOST_CHECK_EQUAL(*netsBegin, net);
}

BOOST_AUTO_TEST_SUITE_END()


} // namespace physical
} // namespace torc

/*
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/indexed_by.hpp>

using namespace boost::multi_index;

struct name{};
struct order{};
typedef boost::multi_index::multi_index_container<
	std::string, 
	boost::multi_index::indexed_by<
		ordered_unique< tag<name>, identity<name> >,
		sequenced< tag<order> >
	>
> ConfigMap;

BOOST_AUTO_TEST_SUITE(physical_database)

BOOST_AUTO_TEST_CASE(multi_index) {
	ConfigMap configMap;
	BOOST_CHECK(configMap.size() == 0);
std::cout << "sizeof(boost::multi_index::multi_index_container): " << sizeof(torc::ConfigMap) << std::endl;
std::cout << "sizeof(std::map<std::string,std::string>): " << sizeof(std::map<std::string,std::string>) << std::endl;
std::cout << "sizeof(std::multimap<std::string,std::string>): " << sizeof(std::multimap<std::string,std::string>) << std::endl;
std::cout << "sizeof(std::vector<torc::InstancePin>): " << sizeof(std::vector<torc::InstancePin>) << std::endl;
std::cout << "sizeof(std::vector<torc::Pip>): " << sizeof(std::vector<torc::Pip>) << std::endl;
std::cout << "sizeof(std::vector<std::string>): " << sizeof(std::vector<std::string>) << std::endl;

}

BOOST_AUTO_TEST_SUITE_END()
*/
#endif
