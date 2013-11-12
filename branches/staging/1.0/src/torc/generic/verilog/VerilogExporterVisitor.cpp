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
/// \brief Source for the VerilogExporterVisitor class.

#include "torc/generic/verilog/VerilogExporterVisitor.hpp"
#include "torc/generic/verilog/VerilogNames.hpp"
#include "torc/generic/verilog/TemporaryAssignment.hpp"
#include <sstream>

namespace torc {
namespace generic {

	using namespace std;

	string VerilogExporterVisitor::getRange(VectorPort& inVectorPort) {
		// first try to look for a range directly in the name
		string name = inVectorPort.getOriginalName();
		size_t pos = name.find("[");
		if(pos != string::npos) return name.substr(pos);

		// otherwise look up the vector range through its properties
		PropertySharedPtr msbPtr 
			= inVectorPort.getProperty(VerilogNames::getTorcRangeMSBPropertyName());
		PropertySharedPtr lsbPtr 
			= inVectorPort.getProperty(VerilogNames::getTorcRangeLSBPropertyName());
		// hopefully we found the properties
		if(msbPtr && lsbPtr) {
			stringstream ss;
			ss << "[" << msbPtr->getValue().get<Value::Integer>() << ":" 
				<< lsbPtr->getValue().get<Value::Integer>() << "]";
			return ss.str();
		}

		// apparently neither the name nor the properties were set
		return "[]";
	}

	/// \brief Visit the top-level netlist.
	void VerilogExporterVisitor::visit(Root& inRoot) throw (Error) {
//		cerr << "Generic root: " << inRoot.getName() << endl;
		inRoot.applyOnAllDesigns(mVisitor);
		inRoot.applyOnAllLibraries(mVisitor);
	}

	void VerilogExporterVisitor::visit(Design& inDesign) throw (Error) {
//		cerr << "Generic design: " << inDesign.getName() << endl;
	}

	void VerilogExporterVisitor::visit(Library& inLibrary) throw (Error) {
		// we output nothing for inferred black boxes or cell libraries
		if(inLibrary.getName() == VerilogNames::getInferredBlackBoxesLibraryName()) return; 
		if(inLibrary.getName() == VerilogNames::getImportedCellLibraryName()) return; 
		// process all other libraries
//		cerr << "Generic library: " << inLibrary.getName() << endl;
		inLibrary.applyOnAllCells(mVisitor);
	}

	void VerilogExporterVisitor::visit(Cell& inCell) throw (Error) {
		// declare the module
		string name = getName(inCell);
		mOut << "module " << name << " ";
		inCell.applyOnAllViews(mVisitor);
		mOut << "endmodule" << endl << endl;
	}

	void VerilogExporterVisitor::visit(View& inView) throw (Error) {
		// begin the port declaration
		mOut << "(" << endl;
		// look up the ports
		PortSharedPtrVector ports; 
		inView.getPorts(ports);
		// iterate through the ports
		PortSharedPtrVector::const_iterator pp = ports.begin();
		PortSharedPtrVector::const_iterator pe = ports.end();
		while(pp < pe) {
			// process each port
			PortSharedPtr portPtr = *pp++;
			mOut << mTab << portPtr->getName();
			if(pp < pe) mOut << ",";
			mOut << endl;
		}
		// end the port declaration
		mOut << ");" << endl;

		inView.applyOnAllPorts(mVisitor);
		inView.applyOnAllNets(mVisitor);
		inView.applyOnAllInstances(mVisitor);
		TemporaryAssignment<string> t(mPropertyContainerName, inView.getName());
		inView.applyOnAllProperties(mVisitor);
	}

	void VerilogExporterVisitor::visit(ScalarPort& inScalarPort) throw (Error) {
		inScalarPort.applyOnAllProperties(mVisitor);
		mOut << getDirection(inScalarPort) << " " << inScalarPort.getName() << ";" << endl;
	}

	void VerilogExporterVisitor::visit(VectorPort& inVectorPort) throw (Error) {
		inVectorPort.applyOnAllProperties(mVisitor);
		mOut << getDirection(inVectorPort) << " " << getRange(inVectorPort) << " "
			<< inVectorPort.getName() << ";" << endl;
	}

	void VerilogExporterVisitor::visit(VectorPortBit& inVectorPortBit) throw (Error) {
		/// \todo Throw an exception.
	}

	void VerilogExporterVisitor::visit(PortBundle& inPortBundle) throw (Error) {
		/// \todo Throw an exception.
	}

	void VerilogExporterVisitor::visit(ScalarNet& inScalarNet) throw (Error) {
		// declare the net as a wire, unless it connects to a port
		/// \todo Shouldn't we declare a wire unless the net contains an *input* port?
		vector<PortSharedPtr> ports;
		inScalarNet.getConnectedPorts(ports);
		if(ports.size() == 0) {
			PropertySharedPtr assignPtr 
				= inScalarNet.getProperty(VerilogNames::getTorcAssignRHSPropertyName());
			mOut << "wire " << inScalarNet.getName();
			if(assignPtr) mOut << " = " << assignPtr->getValue().get<Value::String>();
			mOut << ";" << endl;
		}
		inScalarNet.applyOnAllProperties(mVisitor);
	}

	void VerilogExporterVisitor::visit(VectorNet& inVectorNet) throw (Error) {}
	void VerilogExporterVisitor::visit(VectorNetBit& inVectorNetBit) throw (Error) {}
	void VerilogExporterVisitor::visit(NetBundle& inNetBundle) throw (Error) {}

	void VerilogExporterVisitor::visit(SingleInstance& inSingleInstance) throw (Error) {
		// look up and export the instance information
		CellSharedPtr cellPtr = inSingleInstance.getMaster()->getParent();
		mOut << getName(*cellPtr) << " " << getName(inSingleInstance);

		// look up the port references
		PortReferenceSharedPtrVector portReferences;
		inSingleInstance.getPortReferences(portReferences);
		bool hasPortReferences = portReferences.size() != 0;
		PortReferenceSharedPtrVector::const_iterator prp = portReferences.begin();
		PortReferenceSharedPtrVector::const_iterator pre = portReferences.end();
		if(hasPortReferences) mOut << "(";
		while(prp < pre) {
			// process each port reference
			PortReferenceSharedPtr portReferencePtr = *prp++;
			//PortAttributesSharedPtr portAttributesPtr = portReferencePtr->getAttributes();
			(portReferencePtr)->accept(*this);
			if(prp < pre) mOut << ", ";
		}
		// end the port declaration
		if(hasPortReferences) mOut << ")";
		mOut << ";" << endl;
		TemporaryAssignment<string> t(mPropertyContainerName, inSingleInstance.getName());
		inSingleInstance.applyOnAllProperties(mVisitor);
	}

	void VerilogExporterVisitor::visit(InstanceArray& inInstanceArray) throw (Error) {
		/// \todo Throw an exception.
	}

	void VerilogExporterVisitor::visit(InstanceArrayMember& inInstanceArrayMember) throw (Error) {
		/// \todo Throw an exception.
	}

	void VerilogExporterVisitor::visit(ScalarPortReference& inScalarPortRef) throw (Error) {
//mOut << "@";
		// look up the master port name
		string name = getName(*(inScalarPortRef.getMaster()));
		mOut << "." << name << "(";
		NetSharedPtrVector nets;
		inScalarPortRef.getConnectedNets(nets);
		NetSharedPtrVector::const_iterator np = nets.begin();
		NetSharedPtrVector::const_iterator ne = nets.end();
		if(nets.size() > 1) {} ///< \todo Throw an exception.
		while(np < ne) {
			NetSharedPtr netPtr = *np++;
			mOut << getName(*netPtr);
			break;
		}
		mOut << ")";
		inScalarPortRef.applyOnAllProperties(mVisitor);
	}

	void VerilogExporterVisitor::visit(VectorPortReference& inVectorPortRef) throw (Error) {
mOut << "[VECTORPORTREFERENCE]";
/*
		NetSharedPtrVector nets;
		inVectorPortRef.getConnectedNets(nets);
		NetSharedPtrVector::const_iterator np = nets.begin();
		NetSharedPtrVector::const_iterator ne = nets.end();
		if(nets.size() > 1) {} ///< \todo Throw an exception.
		while(np < ne) {
			NetSharedPtr netPtr = *np++;
			mOut << getName(*netPtr);
			break;
		}
*/
	}

	void VerilogExporterVisitor::visit(VectorPortBitReference& inVectorPortBitRef) throw (Error) {
mOut << "[VECTORPORTBITREFERENCE]";
/*
		NetSharedPtrVector nets;
		inVectorPortBitRef.getConnectedNets(nets);
		NetSharedPtrVector::const_iterator np = nets.begin();
		NetSharedPtrVector::const_iterator ne = nets.end();
		if(nets.size() > 1) {} ///< \todo Throw an exception.
		while(np < ne) {
			NetSharedPtr netPtr = *np++;
			mOut << getName(*netPtr) << "[";
			copy(inVectorPortBitRef.getIndices().begin(), inVectorPortBitRef.getIndices().end(), 
				ostream_iterator<size_t>(mOut, ","));
			mOut << "]";
			break;
		}
*/
	}

	void VerilogExporterVisitor::visit(PortBundleReference& inPortBundleRef) throw (Error) {
		/// \todo Throw an exception.
mOut << "[PORTBUNDLEREFERENCE]";
	}

	void VerilogExporterVisitor::visit(SingleParameter& inSingleParameter) throw (Error) {}
	void VerilogExporterVisitor::visit(ParameterArray& inParamArray) throw (Error) {}
	void VerilogExporterVisitor::visit(ParameterArrayElement& inParamArrayElement) throw (Error) {}

	void VerilogExporterVisitor::visit(Property& inProperty) throw (Error) {
		/// \todo Handle properties.
		const string& name = inProperty.getName();
		const string torcPrefix = "torc_";
		stringstream value;
		Value propertyValue = inProperty.getValue();
		switch(propertyValue.getType()) {
		case Value::eValueTypeBoolean: 
			value << (propertyValue.get<Value::Boolean>() ? "true" : "false");
			break;
		case Value::eValueTypeInteger:
			value << propertyValue.get<Value::Integer>();
			break;
		case Value::eValueTypeMiNoMax:
			value << "[MiNoMax]";
			break;
		case Value::eValueTypeNumber:
			value << propertyValue.get<Value::Number>().eval();
			break;
		case Value::eValueTypePoint:
			value << "[point]";
			break;
		case Value::eValueTypeString:
			value << '"' << propertyValue.get<Value::String>() << '"';
			break;
		default:
			value << "[unknown]";
			break;
		}
		if(name.compare(0, torcPrefix.length(), torcPrefix) == 0) {
			if(name != VerilogNames::getTorcAssignRHSPropertyName() 
				&& name != VerilogNames::getTorcRangeMSBPropertyName() 
				&& name != VerilogNames::getTorcRangeLSBPropertyName())
				mOut << "// property " << name << endl;
		} else {
			mOut << "defparam " << mPropertyContainerName << "." << name << " = " 
				<< value.str() << ";" << endl;
		}
	}

	void VerilogExporterVisitor::visit(PortList& inPortList) throw (Error) {}
	void VerilogExporterVisitor::visit(PortListAlias& inPortListAlias) throw (Error) {}
	void VerilogExporterVisitor::visit(Status& inStatus) throw (Error) {}
	void VerilogExporterVisitor::visit(Permutable& inPermutable) throw (Error) {}
	void VerilogExporterVisitor::visit(InterfaceJoinedInfo& inInterfaceJoinedInfo) throw (Error) {}
	void VerilogExporterVisitor::visit(SimulationInfo& inSimulationInfo) throw (Error) {}
	void VerilogExporterVisitor::visit(Simulate& inSimulate) throw (Error) {}
	void VerilogExporterVisitor::visit(Apply& inApply) throw (Error) {}
	void VerilogExporterVisitor::visit(LogicalResponse& inLogicalResponse) throw (Error) {}
	void VerilogExporterVisitor::visit(LogicValue& inLogicValue) throw (Error) {}
	void VerilogExporterVisitor::visit(LogicElement& inLogicElement) throw (Error) {}
	void VerilogExporterVisitor::visit(WaveValue& inWaveValue) throw (Error) {}
	void VerilogExporterVisitor::visit(Timing& inTiming) throw (Error) {}
	void VerilogExporterVisitor::visit(Event& inEvent) throw (Error) {}
	void VerilogExporterVisitor::visit(ForbiddenEvent& inForbiddenEvent) throw (Error) {}


} // namespace generic
} // namespace torc
