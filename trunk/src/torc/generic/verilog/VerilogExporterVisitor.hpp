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
/// \brief Header for the VerilogExporterVisitor class.

#ifndef TORC_GENERIC_VERILOG_VERILOGEXPORTERVISITOR_HPP
#define TORC_GENERIC_VERILOG_VERILOGEXPORTERVISITOR_HPP

#include "torc/generic/edif/ObjectVisitor.hpp"
#include <string>
#include <vector>

namespace torc {
namespace generic {

namespace generic { class VerilogExporterUnitTest; }

	/// \brief Generic netlist object visitor for output as structural Verilog.
	class VerilogExporterVisitor : 
		public Root::Visitor, public Design::Visitor, public Library::Visitor, 
		public Cell::Visitor, public View::Visitor, public ScalarPort::Visitor, 
		public VectorPort::Visitor, public VectorPortBit::Visitor, public PortBundle::Visitor, 
		public ScalarNet::Visitor, public VectorNet::Visitor, public VectorNetBit::Visitor, 
		public NetBundle::Visitor, public SingleInstance::Visitor, public InstanceArray::Visitor, 
		public InstanceArrayMember::Visitor, public ScalarPortReference::Visitor, 
		public VectorPortReference::Visitor, public VectorPortBitReference::Visitor, 
		public PortBundleReference::Visitor, public SingleParameter::Visitor, 
		public ParameterArray::Visitor, public ParameterArrayElement::Visitor, 
		public Property::Visitor, public PortList::Visitor, public PortListAlias::Visitor, 
		public Status::Visitor, public Permutable::Visitor, public InterfaceJoinedInfo::Visitor, 
		public SimulationInfo::Visitor, public Simulate::Visitor, public Apply::Visitor, 
		public LogicalResponse::Visitor, public LogicValue::Visitor, public LogicElement::Visitor, 
		public WaveValue::Visitor, public Timing::Visitor, public Event::Visitor, 
		public ForbiddenEvent::Visitor
		{
	protected:
	// friends
		/// \brief The unit test class has access to our internals.
		friend class torc::generic::generic::VerilogExporterUnitTest;
	// typedefs
		/// \brief Imported type name.
		typedef std::string string;
		/// \brief Vector of port shared pointers.
		typedef std::vector<PortSharedPtr> PortSharedPtrVector;
		/// \brief Vector of port reference shared pointers.
		typedef std::vector<PortReferenceSharedPtr> PortReferenceSharedPtrVector;
		/// \brief Vector of net shared pointers.
		typedef std::vector<NetSharedPtr> NetSharedPtrVector;
	// members
		/// \brief The root shared pointer.
		RootSharedPtr mRootPtr;
		/// \brief Generic netlist visitor.
		VisitorApplier<VerilogExporterVisitor> mVisitor;
		/// \brief The output stream.
		std::ostream& mOut;
		/// \brief The tab character.
		string mTab;
		/// \brief The current property container name.
		string mPropertyContainerName;
	// functions
		void visit(Root& inroot) throw (Error);
		void visit(Design& inDesign) throw (Error);
		void visit(Library& inLibrary) throw (Error);
		void visit(Cell& inCell) throw (Error);
		void visit(View& inView) throw (Error);
		void visit(ScalarPort& inScalarPort) throw (Error);
		void visit(VectorPort& inVectorPort) throw (Error);
		void visit(VectorPortBit& inVectorPortBit) throw (Error);
		void visit(PortBundle& inPortBundle) throw (Error);
		void visit(ScalarNet& inScalarNet) throw (Error);
		void visit(VectorNet& inVectorNet) throw (Error);
		void visit(VectorNetBit& inVectorNetBit) throw (Error);
		void visit(NetBundle& inNetBundle) throw (Error);
		void visit(SingleInstance& inSingleInstance) throw (Error);
		void visit(InstanceArray& inInstanceArray) throw (Error);
		void visit(InstanceArrayMember& inInstanceArrayMember) throw (Error);
		void visit(ScalarPortReference& inScalarPortRef) throw (Error);
		void visit(VectorPortReference& inVectorPortRef) throw (Error);
		void visit(VectorPortBitReference& inVectorPortBitRef) throw (Error);
		void visit(PortBundleReference& inPortBundleRef) throw (Error);
		void visit(SingleParameter& inSingleParameter) throw (Error);
		void visit(ParameterArray& inParameterArray) throw (Error);
		void visit(ParameterArrayElement& inParameterArrayElement) throw (Error);
		void visit(Property& inProperty) throw (Error);
		void visit(PortList& inPortList) throw (Error);
		void visit(PortListAlias& inPortListAlias) throw (Error);
		void visit(Status& inStatus) throw (Error);
		void visit(Permutable& inPermutable) throw (Error);
		void visit(InterfaceJoinedInfo& inInterfaceJoinedInfo) throw (Error);
		void visit(SimulationInfo& inSimulationInfo) throw (Error);
		void visit(Simulate& inSimulate) throw (Error);
		void visit(Apply& inApply) throw (Error);
		void visit(LogicalResponse& inLogicalResponse) throw (Error);
		void visit(LogicValue& inLogicValue) throw (Error);
		void visit(LogicElement& inLogicElement) throw (Error);
		void visit(WaveValue& inWaveValue) throw (Error);
		void visit(Timing& inTiming) throw (Error);
		void visit(Event& inEvent) throw (Error);
		void visit(ForbiddenEvent& inForbiddenEvent) throw (Error);
		/// \brief Return the original or renamed name of the given netlist object.
		/// \param inNameable A Nameable or Renamable object to obtain the name of.
		string getName(Nameable& inNameable) {
			string name;
			try {
				// try to obtain the original name, if appropriate and available
				Renamable& renamable = dynamic_cast<Renamable&>(inNameable);
				name = renamable.getOriginalName();
				// if we found an original name, we're done
				if(name.length()) return name;
			} catch(std::bad_cast bc) {
				/* don't need to do anything here */
			}
			// return the object name
			return inNameable.getName();
		}
		/// \brief Return a string indicating the direction of a port.
		/// \param inPort The port of interest.
		/// \return "input", "output", or "inout".
		string getDirection(Port& inPort) {
			switch(inPort.getDirection()) {
			case ePortDirectionIn: return "input";
			case ePortDirectionOut: return "output";
			case ePortDirectionInOut: return "inout";
			default: break;
			}
			/// \todo Throw an exception.
			return "";
		}
		/// \brief Return a string describing the range of a vector port.  The range is expressed 
		///		in the form "[start:end]".
		/// \param inVectorPort The vector port of interest.
		string getRange(VectorPort& inVectorPort);
	public:
	// constructors
		/// \brief Public constructor.
		VerilogExporterVisitor(RootSharedPtr inRootPtr, std::ostream& inStream = std::cout) 
			: mRootPtr(inRootPtr), mVisitor(*this), mOut(inStream), mTab("\t"), 
			mPropertyContainerName() {}
		/// \brief Virtual destructor.
		virtual ~VerilogExporterVisitor(void) throw() {}
	};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_VERILOG_VERILOGEXPORTERVISITOR_HPP
