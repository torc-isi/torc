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
/// \brief Unit test for the PrimitiveStructure class.

#include <boost/test/unit_test.hpp>
#include "torc/packer/PrimitiveStructure.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/XdlImporter.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/physical/Instance.hpp"
#include "torc/physical/Net.hpp"
#include <iostream>
#include <fstream>

namespace torc {
namespace packer {

BOOST_AUTO_TEST_SUITE(packer)

/// \brief Unit test for the PrimitiveStructure class.
BOOST_AUTO_TEST_CASE(PrimitiveStructureUnitTest) {

	using namespace torc::architecture;
	// open and initialize a database
//	DDB ddb("xc7v285t");
//	DDB ddb("xc6vlx75t");
	DDB ddb("xc5vlx30");
//	DDB ddb("xc4vlx15");
//	DDB ddb("xc2vp4");
//	DDB ddb("xc2v40");
//	DDB ddb("xcv50e");
//	DDB ddb("xcv50");
	// look up the logic sites in the device
	const Sites& sites = ddb.getSites();
	// define a vector of PrimitiveStructure objects
	typedef std::map<std::string, PrimitiveStructureSharedPtr> PrimitiveStructuresSharedPtrMap;
	PrimitiveStructuresSharedPtrMap primitiveStructures;
	// look up the primitive def types
	typedef const Array<const PrimitiveDef> PrimitiveDefArray;
	PrimitiveDefArray& primitiveDefs = sites.getSiteTypes();
	PrimitiveDefArray::const_iterator p = primitiveDefs.begin();
	PrimitiveDefArray::const_iterator e = primitiveDefs.end();
	while(p < e) {
		// create a PrimitiveStructure object for this PrimitiveDef
		PrimitiveStructureSharedPtr primitiveStructurePtr(new PrimitiveStructure(&*p++));
		const torc::architecture::PrimitiveDef* primitiveDefPtr 
			= primitiveStructurePtr->getPrimitiveDefPtr();
		const std::string& primitiveDefName = primitiveDefPtr->getName();
		// insert the PrimitiveStructure into the map
		primitiveStructures[primitiveDefName] = primitiveStructurePtr;
	}

	// iterate through the PrimitiveStructure entries in the map
	// (just to make sure we can read out what we put in)
	PrimitiveStructuresSharedPtrMap::iterator psp = primitiveStructures.begin();
	PrimitiveStructuresSharedPtrMap::iterator pse = primitiveStructures.end();
	while(psp != pse) {
		// look up the PrimitiveDef name and the PrimitiveStructure object
		std::string primitiveDefName = psp->first;
		PrimitiveStructureSharedPtr primitiveStructurePtr = psp->second;
		(void) primitiveStructurePtr;
		// const torc::architecture::PrimitiveDef* primitiveDefPtr 
		// 	= primitiveStructurePtr->getPrimitiveDefPtr();
		// std::cout << "PrimitiveDef: " << primitiveDefName << std::endl;
		psp++;
	}

}



/// \brief Unit test for the PrimitiveStructure class.
BOOST_AUTO_TEST_CASE(PrimitiveStructureInstancePinsUnitTest) {

	using namespace torc::architecture;
	// create the appropriate file paths
	boost::filesystem::path referencePath = torc::common::DirectoryTree::getExecutablePath()
		/ "torc" / "physical" / "DesignUnitTest.reference.xdl";
	boost::filesystem::path generatedPath = torc::common::DirectoryTree::getExecutablePath()
		/ "regression" / "DesignUnitTest.generated.xdl";

	// import the XDL design
	std::fstream fileStream(referencePath.string().c_str());
	BOOST_REQUIRE(fileStream.good());
	XdlImporter importer;
	importer(fileStream, referencePath.string());

	// look up the design (and do something with it ...)
	torc::physical::DesignSharedPtr designPtr = importer.getDesignPtr();
	BOOST_REQUIRE(designPtr.get() != 0);

	// look up the device database (and do something with it ...)
	DDB* ddbPtr = importer.releaseDDBPtr();

	// look up the logic sites in the device
	const Sites& sites = ddbPtr->getSites();
	// define a vector of PrimitiveStructure objects
	typedef std::map<std::string, PrimitiveStructureSharedPtr> PrimitiveStructuresSharedPtrMap;
	PrimitiveStructuresSharedPtrMap primitiveStructures;
	// look up the primitive def types
	typedef const Array<const PrimitiveDef> PrimitiveDefArray;
	PrimitiveDefArray& primitiveDefs = sites.getSiteTypes();
	PrimitiveDefArray::const_iterator p = primitiveDefs.begin();
	PrimitiveDefArray::const_iterator e = primitiveDefs.end();
	while(p < e) {
		// create a PrimitiveStructure object for this PrimitiveDef
		PrimitiveStructureSharedPtr primitiveStructurePtr(new PrimitiveStructure(&*p++));
		const torc::architecture::PrimitiveDef* primitiveDefPtr 
			= primitiveStructurePtr->getPrimitiveDefPtr();
		const std::string& primitiveDefName = primitiveDefPtr->getName();
		// insert the PrimitiveStructure into the map
		primitiveStructures[primitiveDefName] = primitiveStructurePtr;
	}

	// look up a sample instance
	torc::physical::Circuit::InstanceSharedPtrIterator instancePtrIterator 
		= designPtr->findInstance("blink");
	if(instancePtrIterator == designPtr->instancesEnd()) return;
	torc::physical::InstanceSharedPtr instancePtr = *instancePtrIterator;
	PrimitiveStructureSharedPtr primitiveStructurePtr = primitiveStructures[instancePtr->getType()];
	const PrimitiveDef* primitiveDefPtr = primitiveStructurePtr->getPrimitiveDefPtr();
	if(primitiveDefPtr == 0) return; // this should not happen for valid instances
	const PrimitivePinArray& primitivePins = primitiveDefPtr->getPins();

	using torc::physical::InstancePinSharedPtr;
	using torc::physical::InstancePinSharedPtrVector;
	using torc::physical::Instance;
	using torc::physical::NetSharedPtr;
	using torc::architecture::xilinx::PinIndex;

	// iterate over the instance pins
	InstancePinSharedPtrVector inputPins;
	InstancePinSharedPtrVector outputPins;
	Instance::InstancePinSharedPtrConstIterator ipp = instancePtr->pinsBegin();
	Instance::InstancePinSharedPtrConstIterator ipe = instancePtr->pinsEnd();
	while(ipp != ipe) {
		// look up the instance pin
		InstancePinSharedPtr instancePinPtr = ipp++->second;
		const std::string& pinName = instancePinPtr->getPinName();
		NetSharedPtr netPtr = instancePinPtr->getParentWeakPtr().lock();
		std::cout << "    found " << pinName << " connected to net " << netPtr->getName() << ": ";
		// determine whether the pin is an output
		PinIndex pinIndex = primitiveDefPtr->findPinIndexByName(pinName);
		if(static_cast<boost::int32_t>(pinIndex) < 0) continue; // this shouldn't happen
		const PrimitivePin& primitivePin = primitivePins[pinIndex];
		if(primitivePin.isInput()) inputPins.push_back(instancePinPtr);
		if(primitivePin.isOutput()) outputPins.push_back(instancePinPtr);
		std::cout << (primitivePin.isInput() ? "INPUT" : (primitivePin.isOutput() ? "OUTPUT" : "")) 
			<< std::endl;
	}

	// iterate over the input pins
	InstancePinSharedPtrVector::const_iterator ip;
	InstancePinSharedPtrVector::const_iterator ie;
	ip = inputPins.begin();
	ie = inputPins.end();
	while(ip != ie) {
		InstancePinSharedPtr instancePinPtr = *ip++;
		//std::cout << "    found INPUT " << instancePinPtr->getPinName() << std::endl;
	}
	// iterate over the output pins
	ip = outputPins.begin();
	ie = outputPins.end();
	while(ip != ie) {
		InstancePinSharedPtr instancePinPtr = *ip++;
		//std::cout << "    found OUTPUT " << instancePinPtr->getPinName() << std::endl;
	}

}


BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
