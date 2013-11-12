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
/// \brief Unit test for the generic object model class.

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <sstream>
#include "torc/generic/ObjectFactory.hpp"
#include "torc/generic/ScalarNet.hpp"
#include "torc/generic/edif/Decompiler.hpp"
#include "torc/generic/edif/Linker.hpp"
#include "torc/generic/edif/ParserOptions.hpp"
#include "torc/generic/edif/EdifParser.hpp"
#include "torc/generic/MessageTable.hpp"
#include <boost/regex.hpp>

namespace torc {
namespace generic {

BOOST_AUTO_TEST_SUITE(generic)

	class VisitNet {
	public:
		typedef boost::shared_ptr<Net> NetSharedPtr;
		void operator ()(NetSharedPtr& inNetPtr) const {
			int sources = 0;
			int sinks = 0;
			// look for a single source and a single sink on the net
			// get the ports
			typedef boost::shared_ptr<Port> PortSharedPtr;
			std::vector<PortSharedPtr> ports;
			inNetPtr->getConnectedPorts(ports);
			std::vector<PortSharedPtr>::const_iterator pp = ports.begin();
			std::vector<PortSharedPtr>::const_iterator pe = ports.end();
			while(pp < pe) {
				const PortSharedPtr& portPtr = *pp++;
				EPortDirection direction = portPtr->getDirection();
				switch(direction) {
				case ePortDirectionIn: sources++; break;
				case ePortDirectionOut: sinks++; break;
				case ePortDirectionInOut: sources++; sinks++; break;
				default: break;
				}
			}
			// get the port refs
			typedef boost::shared_ptr<PortReference> PortReferenceSharedPtr;
			std::vector<PortReferenceSharedPtr> portReferences;
			inNetPtr->getConnectedPortRefs(portReferences);
			// convert the port refs to ports
			std::vector<PortReferenceSharedPtr>::const_iterator rp = portReferences.begin();
			std::vector<PortReferenceSharedPtr>::const_iterator re = portReferences.end();
			while(rp < re) {
				const PortReferenceSharedPtr& portRefPtr = *rp++;
				const PortSharedPtr& portPtr = portRefPtr->getMaster();
				EPortDirection direction = portPtr->getDirection();
				switch(direction) {
				case ePortDirectionIn: sinks++; break;
				case ePortDirectionOut: sources++; break;
				case ePortDirectionInOut: sources++; sinks++; break;
				default: break;
				}
			}
			BOOST_REQUIRE_EQUAL(sources, 1);
			std::cerr << "Net " << inNetPtr->getName() << ": " 
				<< ports.size() << " ports, "
				<< portReferences.size() << " portRefs, "
				<< sources << " sources, "
				<< sinks << " sinks"
				<< std::endl;
		}
	};

/// \brief Unit test for the generic object model.
BOOST_AUTO_TEST_CASE(Om1UnitTest) {

	// create string streams for both the generated and parsed EDIF
	std::stringstream generated;
	std::stringstream parsed;

	std::cout << "Generating " << "and.edf" << " ...  ";
	try {
		// create the object factory
		boost::shared_ptr<ObjectFactory> objectFactoryPtr(new ObjectFactory());

		// create the design root
		RootSharedPtr rootPtr = objectFactoryPtr->newRootPtr("and");
		rootPtr->setOriginalName("AND");

		// create the VIRTEX and work libraries
		LibrarySharedPtr virtexLibraryPtr = objectFactoryPtr->newLibraryPtr("VIRTEX", rootPtr);
		LibrarySharedPtr workLibraryPtr = objectFactoryPtr->newLibraryPtr("work", rootPtr);

		// create the LUT2 cell and its PRIM view
		CellSharedPtr lut2CellPtr = objectFactoryPtr->newCellPtr("LUT2", virtexLibraryPtr);
		ViewSharedPtr lut2ViewPtr = objectFactoryPtr->newViewPtr("PRIM", lut2CellPtr);
		ScalarPortSharedPtr lut2I0PortPtr = objectFactoryPtr->newScalarPortPtr("I0", 
			ePortDirectionIn, lut2ViewPtr);
		ScalarPortSharedPtr lut2I1PortPtr = objectFactoryPtr->newScalarPortPtr("I1", 
			ePortDirectionIn, lut2ViewPtr);
		ScalarPortSharedPtr lut2OPortPtr = objectFactoryPtr->newScalarPortPtr("O", 
			ePortDirectionOut, lut2ViewPtr);

		// create the and cell and its PRIM view
		CellSharedPtr andCellPtr = objectFactoryPtr->newCellPtr("and", workLibraryPtr, 
			Cell::eTypeGeneric, "AND");
		ViewSharedPtr andViewPtr = objectFactoryPtr->newViewPtr("verilog", andCellPtr);
		ScalarPortSharedPtr andI0PortPtr = objectFactoryPtr->newScalarPortPtr("I0", 
			ePortDirectionIn, andViewPtr);
		ScalarPortSharedPtr andI1PortPtr = objectFactoryPtr->newScalarPortPtr("I1", 
			ePortDirectionIn, andViewPtr);
		ScalarPortSharedPtr andOPortPtr = objectFactoryPtr->newScalarPortPtr("O", 
			ePortDirectionOut, andViewPtr);

		// create the oZ0 instance of type VIRTEX:LUT2:PRIM
		SingleInstanceSharedPtr oZ0InstancePtr = objectFactoryPtr->newSingleInstancePtr("oZ0", 
			andViewPtr, lut2ViewPtr, "o");
		ScalarPortReferenceSharedPtr oZ0I0PortReferencePtr 
			= objectFactoryPtr->newScalarPortReferencePtr(oZ0InstancePtr, lut2I0PortPtr);
		ScalarPortReferenceSharedPtr oZ0I1PortReferencePtr 
			= objectFactoryPtr->newScalarPortReferencePtr(oZ0InstancePtr, lut2I1PortPtr);
		ScalarPortReferenceSharedPtr oZ0OPortReferencePtr 
			= objectFactoryPtr->newScalarPortReferencePtr(oZ0InstancePtr, lut2OPortPtr);

		// create the i0 net
		ScalarNetSharedPtr i0NetPtr = objectFactoryPtr->newScalarNetPtr("i0", andViewPtr);
		andI0PortPtr->connect(i0NetPtr);
		oZ0I0PortReferencePtr->connect(i0NetPtr);
		ScalarNetSharedPtr i1NetPtr = objectFactoryPtr->newScalarNetPtr("i1", andViewPtr);
		andI1PortPtr->connect(i1NetPtr);
		oZ0I1PortReferencePtr->connect(i1NetPtr);
		ScalarNetSharedPtr oNetPtr = objectFactoryPtr->newScalarNetPtr("o", andViewPtr);
		andOPortPtr->connect(oNetPtr);
		oZ0OPortReferencePtr->connect(oNetPtr);

		// attach the INIT 8 property to the oZ0 instance
		PropertySharedPtr initPropertyPtr = objectFactoryPtr->newPropertyPtr("INIT", oZ0InstancePtr, 
			Value(Value::eValueTypeString, std::string("8")));
(void) initPropertyPtr;

		// instantiate the design
		DesignSharedPtr designPtr = objectFactoryPtr->newDesignPtr("and", rootPtr, "and", "work", 
			"AND");

		// attach the PART xc5vlx30ff324-1 property to the and design instance
		PropertySharedPtr partPropertyPtr = objectFactoryPtr->newPropertyPtr("PART", designPtr, 
			Value(Value::eValueTypeString, std::string("xc5vlx30ff324-1")));
		partPropertyPtr->setOwner("Xilinx");

		std::cout << "done." << std::endl;
//		PrintInfo info(generated);
//		rootPtr->accept(info);

		Decompiler decompiler(rootPtr);
		decompiler();

		// now insert inverter pairs on each net
//		VisitNet visitNet;
//		andViewPtr->applyOnAllNets(visitNet);

	} catch(Error& e) {
		std::cerr << MessageTable::instance()->getMessage(e.getErrorMessageId()) << std::endl;
		const std::vector<Error::StackFrameInfo>& stack = e.getStackTrace();
		for(std::vector<Error::StackFrameInfo>::const_iterator it = stack.begin(); 
			it != stack.end(); it++) {
			std::cerr << "    " << (*it).getFunction() << "() [" << (*it).getFile() << ":" 
				<< (*it).getLine() << "]" << std::endl;
		}
	}
/*
	try {
		std::cout << "Parsing " << "and.edf" << " ...  ";
		boost::shared_ptr<ObjectFactory> factoryPtr(new ObjectFactory());
		boost::shared_ptr<Root> rootPtr;
		factoryPtr->create(rootPtr);
		boost::shared_ptr<Linker> linkerPtr(new Linker(rootPtr));
		ParserOptions options;
		EdifParser parser;
		parser.parse("and.edf", rootPtr, linkerPtr, factoryPtr, options);
		std::cout << "done." << std::endl;
		//PrintInfo info(parsed);
		//rootPtr->accept(info);
//		Decompiler decompiler(rootPtr);
//		decompiler();
	} catch(Error& e) {
		std::cerr << MessageTable::instance()->getMessage(e.getErrorMessageId()) << std::endl;
		const std::vector<Error::StackFrameInfo>& stack = e.getStackTrace();
		for(std::vector<Error::StackFrameInfo>::const_iterator it = stack.begin(); 
			it != stack.end(); it++) {
			std::cerr << "    " << (*it).getFunction() << "() [" << (*it).getFile() << ":" 
				<< (*it).getLine() << "]" << std::endl;
		}
	}
*/
	// verify that the generated and parsed outputs are identical
	BOOST_CHECK_EQUAL(generated.str(), parsed.str());

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace generic
} // namespace torc


#if 0
(edif (rename and "AND")
  (edifVersion 2 0 0)
  (edifLevel 0)
  (keywordMap (keywordLevel 0))
  (status
    (written
      (timeStamp 2010 8 30 21 51 32)
      (author "Synplicity, Inc.")
      (program "Synplify Pro" (version "C-2009.06-SP1, mapper map450rcp1sp1, Build 037R"))
     )
   )
  (library VIRTEX
    (edifLevel 0)
    (technology (numberDefinition ))
    (cell LUT2 (cellType GENERIC)
       (view PRIM (viewType NETLIST)
         (interface
           (port I0 (direction INPUT))
           (port I1 (direction INPUT))
           (port O (direction OUTPUT))
         )
       )
    )
  )
  (library work
    (edifLevel 0)
    (technology (numberDefinition ))
    (cell (rename and "AND") (cellType GENERIC)
       (view verilog (viewType NETLIST)
         (interface
           (port i0 (direction INPUT))
           (port i1 (direction INPUT))
           (port o (direction OUTPUT))
         )
         (contents
          (instance (rename oZ0 "o") (viewRef PRIM (cellRef LUT2 (libraryRef VIRTEX)))
           (property INIT (string "8"))
          )
          (net i0 (joined
           (portRef i0)
           (portRef I0 (instanceRef oZ0))
          ))
          (net i1 (joined
           (portRef i1)
           (portRef I1 (instanceRef oZ0))
          ))
          (net o (joined
           (portRef O (instanceRef oZ0))
           (portRef o)
          ))
         )
       )
    )
  )
  (design (rename and "AND") (cellRef and (libraryRef work))
	(property PART (string "xc5vlx30ff324-1") (owner "Xilinx")))
)
#endif
