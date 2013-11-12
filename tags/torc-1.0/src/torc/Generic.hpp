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
/// \brief Main torc::generic namespace header.

/// \namespace torc::generic
/// \brief Namespace for the Torc generic netlist, including EDIF and BLIF importers and exporters.

#ifndef TORC_GENERIC_HPP
#define TORC_GENERIC_HPP

#include "torc/generic/Apply.hpp"
#include "torc/generic/Bundle.hpp"
#include "torc/generic/BundleFlattener.hpp"
#include "torc/generic/Cell.hpp"
#include "torc/generic/Cloneable.hpp"
#include "torc/generic/Cloning.hpp"
#include "torc/generic/Commentable.hpp"
#include "torc/generic/Composite.hpp"
#include "torc/generic/CompositionType.hpp"
#include "torc/generic/Connectable.hpp"
#include "torc/generic/ConnectionHandler.hpp"
#include "torc/generic/Derivation.hpp"
#include "torc/generic/Design.hpp"
#include "torc/generic/DumpRestoreConfig.hpp"
#include "torc/generic/DumpRestoreData.hpp"
#include "torc/generic/EdifLevel.hpp"
#include "torc/generic/EdifVersion.hpp"
#include "torc/generic/Event.hpp"
#include "torc/generic/Extern.hpp"
#include "torc/generic/FactoryType.hpp"
#include "torc/generic/Flattening.hpp"
#include "torc/generic/ForbiddenEvent.hpp"
#include "torc/generic/Instance.hpp"
#include "torc/generic/InstanceArray.hpp"
#include "torc/generic/InstanceArrayMember.hpp"
#include "torc/generic/InterfaceAttributes.hpp"
#include "torc/generic/InterfaceJoinedInfo.hpp"
#include "torc/generic/InternalUtilityFunctions.hpp"
#include "torc/generic/Library.hpp"
#include "torc/generic/LogicElement.hpp"
#include "torc/generic/LogicValue.hpp"
#include "torc/generic/LogicValueAttributes.hpp"
#include "torc/generic/LogicalResponse.hpp"
#include "torc/generic/Nameable.hpp"
#include "torc/generic/Net.hpp"
#include "torc/generic/NetAttributes.hpp"
#include "torc/generic/NetBundle.hpp"
#include "torc/generic/NetDelay.hpp"
#include "torc/generic/ObjectFactory.hpp"
#include "torc/generic/Parameter.hpp"
#include "torc/generic/ParameterArray.hpp"
#include "torc/generic/ParameterArrayElement.hpp"
#include "torc/generic/ParameterContext.hpp"
#include "torc/generic/ParameterMap.hpp"
#include "torc/generic/ParentedObject.hpp"
#include "torc/generic/PathDelay.hpp"
#include "torc/generic/Permutable.hpp"
#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/Port.hpp"
#include "torc/generic/PortAttributes.hpp"
#include "torc/generic/PortBundle.hpp"
#include "torc/generic/PortBundleReference.hpp"
#include "torc/generic/PortDelay.hpp"
#include "torc/generic/PortDirection.hpp"
#include "torc/generic/PortElement.hpp"
#include "torc/generic/PortList.hpp"
#include "torc/generic/PortListAlias.hpp"
#include "torc/generic/PortRefCreator.hpp"
#include "torc/generic/PortReference.hpp"
#include "torc/generic/Property.hpp"
#include "torc/generic/PropertyContainer.hpp"
#include "torc/generic/Pruning.hpp"
#include "torc/generic/Renamable.hpp"
#include "torc/generic/Root.hpp"
#include "torc/generic/Scalar.hpp"
#include "torc/generic/ScalarNet.hpp"
#include "torc/generic/ScalarPort.hpp"
#include "torc/generic/ScalarPortReference.hpp"
#include "torc/generic/ScaleFactor.hpp"
#include "torc/generic/SelfReferencing.hpp"
#include "torc/generic/Simulate.hpp"
#include "torc/generic/SimulationInfo.hpp"
#include "torc/generic/SingleInstance.hpp"
#include "torc/generic/SingleParameter.hpp"
#include "torc/generic/Status.hpp"
#include "torc/generic/StatusContainer.hpp"
#include "torc/generic/SymTab.hpp"
#include "torc/generic/TimeStamp.hpp"
#include "torc/generic/Timing.hpp"
#include "torc/generic/Unit.hpp"
#include "torc/generic/UserDataContainer.hpp"
#include "torc/generic/Value.hpp"
#include "torc/generic/Vector.hpp"
#include "torc/generic/VectorBit.hpp"
#include "torc/generic/VectorNet.hpp"
#include "torc/generic/VectorNetBit.hpp"
#include "torc/generic/VectorPort.hpp"
#include "torc/generic/VectorPortBit.hpp"
#include "torc/generic/VectorPortBitReference.hpp"
#include "torc/generic/VectorPortReference.hpp"
#include "torc/generic/View.hpp"
#include "torc/generic/Visitable.hpp"
#include "torc/generic/VisitorApplier.hpp"
#include "torc/generic/VisitorType.hpp"
#include "torc/generic/WaveValue.hpp"
#include "torc/generic/Written.hpp"
#include "torc/generic/Error.hpp"
#include "torc/generic/Log.hpp"
#include "torc/generic/Message.hpp"
#include "torc/generic/MessageId.hpp"
#include "torc/generic/MessageSeverity.hpp"
#include "torc/generic/MessageTable.hpp"
#include "torc/generic/edif/Decompiler.hpp"
#include "torc/generic/edif/Driver.hpp"
#include "torc/generic/edif/EdifContext.hpp"
#include "torc/generic/edif/EdifParser.hpp"
#include "torc/generic/edif/Linker.hpp"
#include "torc/generic/edif/ObjectVisitor.hpp"
#include "torc/generic/edif/ParserHelpers.hpp"
#include "torc/generic/edif/ParserOptions.hpp"
#include "torc/generic/edif/ParserPointerTypes.hpp"
#include "torc/generic/edif/Scanner.hpp"
#include "torc/generic/verilog/VerilogImporter.hpp"
// #including this would require that sources be compiled with -Wno-overloaded-virtual.
//#include "torc/generic/verilog/VerilogImporterVisitor.hpp"
#include "torc/generic/verilog/VerilogExporter.hpp"
#include "torc/generic/verilog/VerilogExporterVisitor.hpp"
#include "torc/generic/verilog/VerilogNames.hpp"

namespace torc {
namespace generic {

	// temporary patch for FCCM paper compatibility
	class EdifImporter : public EdifParser {
	protected:
		ObjectFactorySharedPtr mObjectFactoryPtr;
		RootSharedPtr mRootPtr;
	public:
		EdifImporter(ObjectFactorySharedPtr& inObjectFactoryPtr) 
			: mObjectFactoryPtr(inObjectFactoryPtr), mRootPtr(inObjectFactoryPtr->newRootPtr("new")) {}
		virtual ~EdifImporter(void) throw () {}
		void operator() (std::ostream& inStream, std::string& inName) {
			mRootPtr->setName(inName);
			LinkerSharedPtr linkerPtr(new Linker(mRootPtr));
			ParserOptions options;
			parse(inName, mRootPtr, linkerPtr, mObjectFactoryPtr, options);
		}
		RootSharedPtr getRootPtr(void) { return mRootPtr; }
	};

	// temporary patch for FCCM paper compatibility
	class EdifExporter {
	protected:
		std::ostream& mStream;
	public:
		EdifExporter(std::ostream& inStream) : mStream(inStream) {}
		void operator() (RootSharedPtr& inRootPtr) {
			Decompiler decompiler(inRootPtr, mStream);
			decompiler();
		}
	};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_HPP
