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
/// \brief Unit Test for ModuleTransformer class.

#include <boost/test/unit_test.hpp>
#include "torc/physical/ModuleTransformer.hpp"
#include "torc/physical/XdlImporter.hpp"
#include "torc/physical/XdlExporter.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/physical/Factory.hpp"
#include "torc/physical/Design.hpp"
#include <fstream>

namespace torc {
namespace physical {

BOOST_AUTO_TEST_SUITE(physical)

BOOST_AUTO_TEST_SUITE(moduletransformer)

static const string sInstanceOneName = "c1";
static const string sInstanceTwoName = "c2";
static const string sModuleDefinition = "aModule";
static const string sModuleInstanceName = "aModuleInstance";
static const string sHierarchySeparator = "/";

DesignSharedPtr verifyDesignOpenedSuccessfully(const string & inDesignFileName);
InstanceSharedPtrVector getInstancesToModularize(DesignSharedPtr inDesignPtr);
void preModularizationVerifications(DesignSharedPtr inDesignPtr);
void postModularizationVerifications(DesignSharedPtr inDesignPtr,
	InstanceSharedPtrVector inInstanceVector, bool inKeepPlacement = false,
	bool inKeepRouting = false);
void preFlatteningVerifications(DesignSharedPtr inDesignPtr);
void postFlatteningVerifications(DesignSharedPtr inDesignPtr, bool inKeepPlacement = false,
	bool inKeepRouting = false);

/// \brief Verify that the design file opened successfully.
/// \param inDesignFileName The design file name.
DesignSharedPtr verifyDesignOpenedSuccessfully(const string & inDesignFileName) {
	// Design shared pointer
	DesignSharedPtr designPtr;
	// Create the appropriate file paths
	boost::filesystem::path physicalPath =
	torc::common::DirectoryTree::getExecutablePath() / "torc" / "physical";
	// Original xdl
	boost::filesystem::path originalPath = physicalPath / inDesignFileName;
	// Import the XDL design
	std::fstream fileStream(originalPath.string().c_str());
	// Verify the file is good
	BOOST_REQUIRE(fileStream.good());
	// Import the xdl design
	XdlImporter importer;
	importer(fileStream, originalPath.string());
	// Get a pointer to the design
	designPtr = importer.getDesignPtr();
	// Verify the design pointer
	BOOST_REQUIRE(designPtr.get() != 0);
	return designPtr;
}

/// \brief Retrieve the instances to modularize from the design and return them in a vector.
/// \param inDesignPtr The design shared pointer.
InstanceSharedPtrVector getInstancesToModularize(DesignSharedPtr inDesignPtr) {
	// Create instances vector
	InstanceSharedPtrVector instPtrVector;
	// Select instance sInstanceOneName to modularize
	Design::InstanceSharedPtrIterator instIter = inDesignPtr->findInstance(sInstanceOneName);
	// Verify design has instance sInstanceOneName
	BOOST_REQUIRE(instIter != inDesignPtr->instancesEnd());
	// Add instance pointer to instance vector
	instPtrVector.push_back(*instIter);
	// Select instance sInstanceTwoName to modularize
	instIter = inDesignPtr->findInstance(sInstanceTwoName);
	// Verify design has instance sInstanceTwoName
	BOOST_REQUIRE(instIter != inDesignPtr->instancesEnd());
	instPtrVector.push_back(*instIter);
	// Return instance vector
	return instPtrVector;
}

/// \brief Perform pre-modularization tests on a design.
/// \param inDesignPtr The design shared pointer.
/// \details The pre-modularization tests consist of the following:
/// 	1 - Verify module definition does not exist in design.
/// 	2 - Verify module instance does not exist in design.
void preModularizationVerifications(DesignSharedPtr inDesignPtr) {
	// Search design for sModuleDefinition
	Design::ModuleSharedPtrIterator modIter = inDesignPtr->findModule(sModuleDefinition);
	// Verify design does not have module sModuleDefinition
	BOOST_REQUIRE(modIter == inDesignPtr->modulesEnd());
	// Search design for sModuleInstanceName
	Design::InstanceSharedPtrIterator instIter = inDesignPtr->findInstance(sModuleInstanceName);
	// Verify design does not have sModuleInstanceName
	BOOST_REQUIRE(instIter == inDesignPtr->instancesEnd());
}

/// \brief Perform post-modularization tests on a design.
/// \param inDesignPtr The design shared pointer.
/// \param inInstanceVector The vector of the instances that were modularized
/// \param inKeepPlacement The flag to keep placement when modularizing, defaults to false.
/// \param inKeepRouting The flag to keep routing when modularizing, defaults to false.
/// \details The post-modularization tests consist of the following:
///		1 - Verify module definition exists in design.
///		2 - Verify module instance exists in design.
///		3 - Verify module instance placement.
///		4 - Verify instances migration & placement.
///		5 - Verify intra net migration & routing.
///		6 - Verify inter net routing.
///		7 - Verify module definition port count.
void postModularizationVerifications(DesignSharedPtr inDesignPtr,
	InstanceSharedPtrVector inInstanceVector, bool inKeepPlacement, bool inKeepRouting) {
	// Begin verify module definition and module instance creation
	// Search design for sModuleDefinition
	Design::ModuleSharedPtrIterator modIter = inDesignPtr->findModule(sModuleDefinition);
	// Verify design has module sModuleDefinition
	BOOST_REQUIRE(modIter != inDesignPtr->modulesEnd());
	// Search design for sModuleInstanceName
	Design::InstanceSharedPtrIterator instIter = inDesignPtr->findInstance(sModuleInstanceName);
	// Verify design has sModuleInstanceName
	BOOST_REQUIRE(instIter != inDesignPtr->instancesEnd());
	// Get a pointer to the module definition
	ModuleSharedPtr modPtr = *modIter;
	// Get a pointer to the module instance
	InstanceSharedPtr instPtr = *instIter;
	// Verify placement
	if(!inKeepPlacement) {
		// Verify module instance is not placed
		BOOST_REQUIRE_EQUAL(instPtr->getTile().compare(""), 0);
		BOOST_REQUIRE_EQUAL(instPtr->getSite().compare(""), 0);
	}
	// End verify module definition and module instance creation

	// Begin verify instance migration and placement
	// Get a begin iterator to inInstanceVector
	Design::InstanceSharedPtrConstIterator inInstanceVectorBegin = inInstanceVector.begin();
	// Get an end iterator to inInstanceVector
	Design::InstanceSharedPtrConstIterator inInstanceVectorEnd = inInstanceVector.end();
	// Iterate over all instances in inInstanceVector
	while(inInstanceVectorBegin != inInstanceVectorEnd) {
		// Get a pointer to the current instance
		InstanceSharedPtr instPtr = *inInstanceVectorBegin;
		// Search design for instance
		instIter = inDesignPtr->findInstance(instPtr->getName());
		// Verify design does not have instance
		BOOST_REQUIRE(instIter == inDesignPtr->instancesEnd());
		// Search module definition for instance
		instIter = modPtr->findInstance(instPtr->getName());
		// Verify module definition has instance
		BOOST_REQUIRE(instIter != modPtr->instancesEnd());
		// Get a pointer to the looked up instance
		InstanceSharedPtr modInstPtr = *instIter;
		// Verify placement
		if(!inKeepPlacement) {
			// Verify instance is not placed
			BOOST_REQUIRE_EQUAL(modInstPtr->getTile().compare(""), 0);
			BOOST_REQUIRE_EQUAL(modInstPtr->getSite().compare(""), 0);
		}
		// Move to next instance
		inInstanceVectorBegin++;
	}
	// End verify instance migration and placement

	// Begin verify intra net migration and routing
	// Get a begin iterator to module nets
	Design::NetSharedPtrIterator moduleNetsBegin = modPtr->netsBegin();
	// Get an end iterator to module nets
	Design::NetSharedPtrIterator moduleNetsEnd = modPtr->netsEnd();
	// Iterate over all module nets
	while(moduleNetsBegin != moduleNetsEnd) {
		// Get a pointer to the current net
		NetSharedPtr moduleNetPtr = *moduleNetsBegin;
		// Verify routing
		if(!inKeepRouting) {
			BOOST_REQUIRE(moduleNetPtr->pipsBegin() == moduleNetPtr->pipsEnd());
		}
		// Search for net in design
		Design::NetSharedPtrIterator designNetItr = inDesignPtr->findNet(moduleNetPtr->getName());
		// Verify net does not exist in design
		BOOST_REQUIRE(designNetItr == inDesignPtr->netsEnd());
		// Move to next net
		moduleNetsBegin++;
	}
	// End verify intra net migration and routing

	// Begin verify module port count and inter net routing
	// Initialize port count
	u_int32_t portCount = 0;
	// Search design for sModuleInstanceName
	instIter = inDesignPtr->findInstance(sModuleInstanceName);
	// Get a pointer to the module instance
	instPtr = *instIter;
	// Get a begin iterator to design nets
	Design::NetSharedPtrIterator designNetsBegin = inDesignPtr->netsBegin();
	// Get an end iterator to design nets
	Design::NetSharedPtrIterator designNetsEnd = inDesignPtr->netsEnd();
	// Iterate over all design nets
	while(designNetsBegin != designNetsEnd) {
		// Get a pointer to the current net
		NetSharedPtr designNetPtr = *designNetsBegin;
		// Get a begin iterator to net sources
		Net::InstancePinSharedPtrIterator designNetSourcesBegin = designNetPtr->sourcesBegin();
		// Get an end iterator to net sources
		Net::InstancePinSharedPtrIterator designNetSourcesEnd = designNetPtr->sourcesEnd();
		// Iterate over all net source
		while(designNetSourcesBegin != designNetSourcesEnd) {
			// Get a pointer to the current net source pin
			InstancePinSharedPtr instPinPtr = *designNetSourcesBegin;
			// Get a pointer to the instance hosting the net source pin
			InstanceSharedPtr pinInstPtr = instPinPtr->getInstancePtr().lock();
			// Is this the module instance?
			if(pinInstPtr == instPtr) {
				// Verify routing
				if(!inKeepRouting) {
					BOOST_REQUIRE(designNetPtr->pipsBegin() == designNetPtr->pipsEnd());
				}
				portCount++;
			}
			// Move to next source
			designNetSourcesBegin++;
		}
		// Get a begin iterator to net sinks
		Net::InstancePinSharedPtrIterator designNetSinksBegin = designNetPtr->sinksBegin();
		// Get an end iterator to net sinks
		Net::InstancePinSharedPtrIterator designNetSinksEnd = designNetPtr->sinksEnd();
		// Iterate over all net source
		while(designNetSinksBegin != designNetSinksEnd) {
			// Get a pointer to the current net source pin
			InstancePinSharedPtr instPinPtr = *designNetSinksBegin;
			// Get a pointer to the instance hosting the net source pin
			InstanceSharedPtr pinInstPtr = instPinPtr->getInstancePtr().lock();
			// Is this the module instance?
			if(pinInstPtr == instPtr) {
				// Verify routing
				if(!inKeepRouting) {
					BOOST_REQUIRE(designNetPtr->pipsBegin() == designNetPtr->pipsEnd());
				}
				portCount++;
			}
			// Move to next sink
			designNetSinksBegin++;
		}
		// Move to next net
		designNetsBegin++;
	}

	// Verify that module definition has portCount ports
	BOOST_REQUIRE_EQUAL(modPtr->getPortCount(), portCount);
	// End verify module port count and inter net routing
}

/// \brief Perform pre-flattening tests on a design.
/// \param inDesignPtr The design shared pointer.
/// \details The pre-flattening tests consist of the following:
/// 	1 - Verify module definition exists in design.
/// 	2 - Verify module instance exists in design.
void preFlatteningVerifications(DesignSharedPtr inDesignPtr) {
	// Search design for sModuleDefinition
	Design::ModuleSharedPtrIterator modIter = inDesignPtr->findModule(sModuleDefinition);
	// Verify design has module sModuleDefinition
	BOOST_REQUIRE(modIter != inDesignPtr->modulesEnd());
	// Search design for sModuleInstanceName
	Design::InstanceSharedPtrIterator instIter = inDesignPtr->findInstance(sModuleInstanceName);
	// Verify design has sModuleInstanceName
	BOOST_REQUIRE(instIter != inDesignPtr->instancesEnd());
}

/// \brief Perform post-flattening tests on a design.
/// \param inDesignPtr The design shared pointer.
/// \param inKeepPlacement The flag to keep placement when flattening, defaults to false.
/// \param inKeepRouting The flag to keep routing when flattening, defaults to false.
/// \details The post-flattening tests consist of the following:
///		1 - Verify module definition still exists in design.
///		2 - Verify module instance does not exist in design.
///		3 - Verify module instances cloning & placement.
///		4 - Verify module intra nets cloning & routing.
///		5 - Verify inter nets do not reference module instance & inter net routing.
void postFlatteningVerifications(DesignSharedPtr inDesignPtr, bool inKeepPlacement,
	bool inKeepRouting) {
	// Begin verify module definition existence and removal of module instance
	// Search design for sModuleDefinition
	Design::ModuleSharedPtrIterator modIter = inDesignPtr->findModule(sModuleDefinition);
	// Verify design has module sModuleDefinition
	BOOST_REQUIRE(modIter != inDesignPtr->modulesEnd());
	// Search design for sModuleInstanceName
	Design::InstanceSharedPtrIterator designInstIter 
		= inDesignPtr->findInstance(sModuleInstanceName);
	// Verify design has sModuleInstanceName
	BOOST_REQUIRE(designInstIter == inDesignPtr->instancesEnd());
	// End verify module definition existence and removal of module instance

	// Begin verify module instances cloning and placement
	// Get a pointer to the module definition
	ModuleSharedPtr modPtr = *modIter;
	// Get a begin iterator to module definition instances
	Design::InstanceSharedPtrConstIterator modInstancesBegin = modPtr->instancesBegin();
	// Get an end iterator to module definition instances
	Design::InstanceSharedPtrConstIterator modInstancesEnd = modPtr->instancesEnd();
	// Iterate over all module definition instances
	while(modInstancesBegin != modInstancesEnd) {
		// Get a pointer to the current instance
		InstanceSharedPtr modInstPtr = *modInstancesBegin;
		// Search for corresponding flattened instance in design
		designInstIter = inDesignPtr->findInstance(sModuleInstanceName +
			sHierarchySeparator + modInstPtr->getName());
		// Verify instance is cloned and moved to design
		BOOST_REQUIRE(designInstIter != inDesignPtr->instancesEnd());
		// Verify placement
		if(!inKeepPlacement) {
			// Get a pointer to the instance clone
			InstanceSharedPtr instPtr = *designInstIter;
			// Verify it is not placed
			BOOST_REQUIRE_EQUAL(instPtr->getTile().compare(""), 0);
			BOOST_REQUIRE_EQUAL(instPtr->getSite().compare(""), 0);
		}
		// Move to next instance
		modInstancesBegin++;
	}
	// End verify module instances cloning and placement

	// Begin verify intra net cloning and routing
	// Get a begin iterator to module nets
	Design::NetSharedPtrIterator moduleNetsBegin = modPtr->netsBegin();
	// Get an end iterator to module nets
	Design::NetSharedPtrIterator moduleNetsEnd = modPtr->netsEnd();
	// Iterate over all module nets
	while(moduleNetsBegin != moduleNetsEnd) {
		// Get a pointer to the current net
		NetSharedPtr moduleNetPtr = *moduleNetsBegin;
		// Search for corresponding flattened net in design
		Design::NetSharedPtrIterator designNetIter = inDesignPtr->findNet(sModuleInstanceName +
			sHierarchySeparator + moduleNetPtr->getName());
		// Verify net is cloned and moved to design
		BOOST_REQUIRE(designNetIter != inDesignPtr->netsEnd());
		// Get a pointer to the cloned design net
		NetSharedPtr designNetPtr = *designNetIter;
		// Verify routing
		if(!inKeepRouting) {
			BOOST_REQUIRE(designNetPtr->pipsBegin() == designNetPtr->pipsEnd());
		}
		// Move to next net
		moduleNetsBegin++;
	}
	// End verify intra net migration and routing

	// Begin verify inter nets does not reference module instance & routing
	// Get a begin iterator to design nets
	Design::NetSharedPtrIterator designNetsBegin = inDesignPtr->netsBegin();
	// Get an end iterator to design nets
	Design::NetSharedPtrIterator designNetsEnd = inDesignPtr->netsEnd();
	// Iterate over all design nets
	while(designNetsBegin != designNetsEnd) {
		// Get a pointer to the current net
		NetSharedPtr designNetPtr = *designNetsBegin;
		// Get a begin iterator to net sources
		Net::InstancePinSharedPtrIterator designNetSourcesBegin = designNetPtr->sourcesBegin();
		// Get an end iterator to net sources
		Net::InstancePinSharedPtrIterator designNetSourcesEnd = designNetPtr->sourcesEnd();
		// Iterate over all net source
		while(designNetSourcesBegin != designNetSourcesEnd) {
			// Get a pointer to the current net source pin
			InstancePinSharedPtr instPinPtr = *designNetSourcesBegin;
			// Get a pointer to the instance hosting the net source pin
			InstanceSharedPtr pinInstPtr = instPinPtr->getInstancePtr().lock();
			// Verify net does not point to flattened module instance
			BOOST_REQUIRE_NE(pinInstPtr->getName().compare(sModuleInstanceName), 0);

			// Was this an inter net once?
			if((pinInstPtr->getName()).find(sModuleInstanceName + sHierarchySeparator) 
				!= string::npos) {
				// Verify routing
				if(!inKeepRouting) {
					BOOST_REQUIRE(designNetPtr->pipsBegin() == designNetPtr->pipsEnd());
				}
			}
			// Move to next source
			designNetSourcesBegin++;
		}
		// Get a begin iterator to net sinks
		Net::InstancePinSharedPtrIterator designNetSinksBegin = designNetPtr->sinksBegin();
		// Get an end iterator to net sinks
		Net::InstancePinSharedPtrIterator designNetSinksEnd = designNetPtr->sinksEnd();
		// Iterate over all net source
		while(designNetSinksBegin != designNetSinksEnd) {
			// Get a pointer to the current net source pin
			InstancePinSharedPtr instPinPtr = *designNetSinksBegin;
			// Get a pointer to the instance hosting the net source pin
			InstanceSharedPtr pinInstPtr = instPinPtr->getInstancePtr().lock();
			// Verify net does not point to flattened module instance
			BOOST_REQUIRE_NE(pinInstPtr->getName().compare(sModuleInstanceName), 0);
			// Was this an inter net once?
			if((pinInstPtr->getName()).find(sModuleInstanceName + sHierarchySeparator) 
				!= string::npos) {
				// Verify routing
				if(!inKeepRouting) {
					BOOST_REQUIRE(designNetPtr->pipsBegin() == designNetPtr->pipsEnd());
				}
			}
			// Move to next sink
			designNetSinksBegin++;
		}
		// Move to next net
		designNetsBegin++;
	}
	// End verify inter nets does not reference module instance & routing
}

/// \brief Unit test for modularizing two instances with intra net.
BOOST_AUTO_TEST_CASE(ModularizeWithIntraNetUnitTest) {
	// Verify file opened successfully
	DesignSharedPtr designPtr 
		= verifyDesignOpenedSuccessfully("ModuleTransformerUnitTest.Scenario.01.Modularize.xdl");
	// Get instances to modularize
	InstanceSharedPtrVector instPtrVector = getInstancesToModularize(designPtr);
	// Pre modularization verifications
	preModularizationVerifications(designPtr);
	// Create a ModuleTransformer object
	ModuleTransformer moduleTransfomer(designPtr);
	// Modularize instances in instPtrVector
	moduleTransfomer.modularize(instPtrVector, sModuleDefinition, sModuleInstanceName);
	// Post modularization verifications
	postModularizationVerifications(designPtr, instPtrVector);
}

/// \brief Unit test for modularizing two instances with input inter net.
BOOST_AUTO_TEST_CASE(ModularizeWithInputInterNetUnitTest) {
	// Verify file opened successfully
	DesignSharedPtr designPtr 
		= verifyDesignOpenedSuccessfully("ModuleTransformerUnitTest.Scenario.02.Modularize.xdl");
	// Get instances to modularize
	InstanceSharedPtrVector instPtrVector = getInstancesToModularize(designPtr);
	// Pre modularization verifications
	preModularizationVerifications(designPtr);
	// Create a ModuleTransformer object
	ModuleTransformer moduleTransfomer(designPtr);
	// Modularize instances in instPtrVector
	moduleTransfomer.modularize(instPtrVector, sModuleDefinition, sModuleInstanceName);
	// Post modularization verifications
	postModularizationVerifications(designPtr, instPtrVector);
}

/// \brief Unit test for modularizing two instances with input fanout input net.
BOOST_AUTO_TEST_CASE(ModularizeWithInputFanoutInterNetUnitTest) {
	// Verify file opened successfully
	DesignSharedPtr designPtr 
		= verifyDesignOpenedSuccessfully("ModuleTransformerUnitTest.Scenario.03.Modularize.xdl");
	// Get instances to modularize
	InstanceSharedPtrVector instPtrVector = getInstancesToModularize(designPtr);
	// Pre modularization verifications
	preModularizationVerifications(designPtr);
	// Create a ModuleTransformer object
	ModuleTransformer moduleTransfomer(designPtr);
	// Modularize instances in instPtrVector
	moduleTransfomer.modularize(instPtrVector, sModuleDefinition, sModuleInstanceName);
	// Post modularization verifications
	postModularizationVerifications(designPtr, instPtrVector);
}

/// \brief Unit test for modularizing two instances with output inter net.
BOOST_AUTO_TEST_CASE(ModularizeWithOutputInterNetUnitTest) {
	// Verify file opened successfully
	DesignSharedPtr designPtr 
		= verifyDesignOpenedSuccessfully("ModuleTransformerUnitTest.Scenario.04.Modularize.xdl");
	// Get instances to modularize
	InstanceSharedPtrVector instPtrVector = getInstancesToModularize(designPtr);
	// Pre modularization verifications
	preModularizationVerifications(designPtr);
	// Create a ModuleTransformer object
	ModuleTransformer moduleTransfomer(designPtr);
	// Modularize instances in instPtrVector
	moduleTransfomer.modularize(instPtrVector, sModuleDefinition, sModuleInstanceName);
	// Post modularization verifications
	postModularizationVerifications(designPtr, instPtrVector);
}

/// \brief Unit test for modularizing two instances with output fanout inter net.
BOOST_AUTO_TEST_CASE(ModularizeWithOutputFanoutInterNetUnitTest) {
	// Verify file opened successfully
	DesignSharedPtr designPtr 
		= verifyDesignOpenedSuccessfully("ModuleTransformerUnitTest.Scenario.05.Modularize.xdl");
	// Get instances to modularize
	InstanceSharedPtrVector instPtrVector = getInstancesToModularize(designPtr);
	// Pre modularization verifications
	preModularizationVerifications(designPtr);
	// Create a ModuleTransformer object
	ModuleTransformer moduleTransfomer(designPtr);
	// Modularize instances in instPtrVector
	moduleTransfomer.modularize(instPtrVector, sModuleDefinition, sModuleInstanceName);
	// Post modularization verifications
	postModularizationVerifications(designPtr, instPtrVector);
}

/// \brief Unit test for modularizing two instances with fanout intra and inter net.
BOOST_AUTO_TEST_CASE(ModularizeWithIntraAndInterNetUnitTest) {
	// Verify file opened successfully
	DesignSharedPtr designPtr 
		= verifyDesignOpenedSuccessfully("ModuleTransformerUnitTest.Scenario.06.Modularize.xdl");
	// Get instances to modularize
	InstanceSharedPtrVector instPtrVector = getInstancesToModularize(designPtr);
	// Pre modularization verifications
	preModularizationVerifications(designPtr);
	// Create a ModuleTransformer object
	ModuleTransformer moduleTransfomer(designPtr);
	// Modularize instances in instPtrVector
	moduleTransfomer.modularize(instPtrVector, sModuleDefinition, sModuleInstanceName);
	// Post modularization verifications
	postModularizationVerifications(designPtr, instPtrVector);
}
// End modularization tests

// Begin flattening tests
/// \brief Unit test for flattening a module with two instances and an intra net.
BOOST_AUTO_TEST_CASE(FlattenWithIntraNetUnitTest) {
	// Verify file opened successfully
	DesignSharedPtr designPtr 
		= verifyDesignOpenedSuccessfully("ModuleTransformerUnitTest.Scenario.01.Flatten.xdl");
	// Pre flattening verifications
	preFlatteningVerifications(designPtr);
	// Create a ModuleTransformer object
	ModuleTransformer moduleTransfomer(designPtr);
	// Flatten sModuleInstanceName
	moduleTransfomer.flatten(sModuleInstanceName);
	// Post flattening verifications
	postFlatteningVerifications(designPtr);
}

/// \brief Unit test for flattening two instances with input inter net.
BOOST_AUTO_TEST_CASE(FlattenWithInputInterNetUnitTest) {
	// Verify file opened successfully
	DesignSharedPtr designPtr 
		= verifyDesignOpenedSuccessfully("ModuleTransformerUnitTest.Scenario.02.Flatten.xdl");
	// Pre flattening verifications
	preFlatteningVerifications(designPtr);
	// Create a ModuleTransformer object
	ModuleTransformer moduleTransfomer(designPtr);
	// Flatten sModuleInstanceName
	moduleTransfomer.flatten(sModuleInstanceName);
	// Post flattening verifications
	postFlatteningVerifications(designPtr);
}

/// \brief Unit test for flattening two instances with input fanout input net.
BOOST_AUTO_TEST_CASE(FlattenWithInputFanoutInterNetUnitTest) {
	// Verify file opened successfully
	DesignSharedPtr designPtr 
		= verifyDesignOpenedSuccessfully("ModuleTransformerUnitTest.Scenario.03.Flatten.xdl");
	// Pre flattening verifications
	preFlatteningVerifications(designPtr);
	// Create a ModuleTransformer object
	ModuleTransformer moduleTransfomer(designPtr);
	// Flatten sModuleInstanceName
	moduleTransfomer.flatten(sModuleInstanceName);
	// Post flattening verifications
	postFlatteningVerifications(designPtr);
}

/// \brief Unit test for flattening two instances with output inter net.
BOOST_AUTO_TEST_CASE(FlattenWithOutputInterNetUnitTest) {
	// Verify file opened successfully
	DesignSharedPtr designPtr 
		= verifyDesignOpenedSuccessfully("ModuleTransformerUnitTest.Scenario.04.Flatten.xdl");
	// Pre flattening verifications
	preFlatteningVerifications(designPtr);
	// Create a ModuleTransformer object
	ModuleTransformer moduleTransfomer(designPtr);
	// Flatten sModuleInstanceName
	moduleTransfomer.flatten(sModuleInstanceName);
	// Post flattening verifications
	postFlatteningVerifications(designPtr);
}

/// \brief Unit test for flattening two instances with output fanout inter net.
BOOST_AUTO_TEST_CASE(FlattenWithOutputFanoutInterNetUnitTest) {
	// Verify file opened successfully
	DesignSharedPtr designPtr 
		= verifyDesignOpenedSuccessfully("ModuleTransformerUnitTest.Scenario.05.Flatten.xdl");
	// Pre flattening verifications
	preFlatteningVerifications(designPtr);
	// Create a ModuleTransformer object
	ModuleTransformer moduleTransfomer(designPtr);
	// Flatten sModuleInstanceName
	moduleTransfomer.flatten(sModuleInstanceName);
	// Post flattening verifications
	postFlatteningVerifications(designPtr);
}

/// \brief Unit test for flattening two instances with fanout intra and inter net.
BOOST_AUTO_TEST_CASE(FlattenWithIntraAndInterNetUnitTest) {
	// Verify file opened successfully
	DesignSharedPtr designPtr 
		= verifyDesignOpenedSuccessfully("ModuleTransformerUnitTest.Scenario.06.Flatten.xdl");
	// Pre flattening verifications
	preFlatteningVerifications(designPtr);
	// Create a ModuleTransformer object
	ModuleTransformer moduleTransfomer(designPtr);
	// Flatten sModuleInstanceName
	moduleTransfomer.flatten(sModuleInstanceName);
	// Post flattening verifications
	postFlatteningVerifications(designPtr);
}

// End flattening tests

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

} // namespace physical
} // namespace torc
