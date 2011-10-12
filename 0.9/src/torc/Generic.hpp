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
/// \brief Main torc::generic namespace header.

/// \namespace torc::generic
/// \brief Namespace for the Torc generic netlist, including EDIF and BLIF importers and exporters.

#ifndef TORC_GENERIC_HPP
#define TORC_GENERIC_HPP

#include "torc/generic/decompiler/Decompiler.hpp"
#include "torc/generic/decompiler/ObjectVisitor.hpp"
#include "torc/generic/om/Apply.hpp"
#include "torc/generic/om/Bundle.hpp"
#include "torc/generic/om/BundleFlattener.hpp"
#include "torc/generic/om/Cell.hpp"
#include "torc/generic/om/Cloneable.hpp"
#include "torc/generic/om/Cloning.hpp"
#include "torc/generic/om/Commentable.hpp"
#include "torc/generic/om/Composite.hpp"
#include "torc/generic/om/CompositionType.hpp"
#include "torc/generic/om/Connectable.hpp"
#include "torc/generic/om/ConnectionHandler.hpp"
#include "torc/generic/om/Derivation.hpp"
#include "torc/generic/om/Design.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"
#include "torc/generic/om/DumpRestoreData.hpp"
#include "torc/generic/om/EdifLevel.hpp"
#include "torc/generic/om/EdifVersion.hpp"
#include "torc/generic/om/Event.hpp"
#include "torc/generic/om/Extern.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/Flattening.hpp"
#include "torc/generic/om/ForbiddenEvent.hpp"
#include "torc/generic/om/Instance.hpp"
#include "torc/generic/om/InstanceArray.hpp"
#include "torc/generic/om/InstanceArrayMember.hpp"
#include "torc/generic/om/InterfaceAttributes.hpp"
#include "torc/generic/om/InterfaceJoinedInfo.hpp"
#include "torc/generic/om/InternalUtilityFunctions.hpp"
#include "torc/generic/om/Library.hpp"
#include "torc/generic/om/LogicElement.hpp"
#include "torc/generic/om/LogicValue.hpp"
#include "torc/generic/om/LogicValueAttributes.hpp"
#include "torc/generic/om/LogicalResponse.hpp"
#include "torc/generic/om/Nameable.hpp"
#include "torc/generic/om/Net.hpp"
#include "torc/generic/om/NetAttributes.hpp"
#include "torc/generic/om/NetBundle.hpp"
#include "torc/generic/om/NetDelay.hpp"
#include "torc/generic/om/ObjectFactory.hpp"
#include "torc/generic/om/Parameter.hpp"
#include "torc/generic/om/ParameterArray.hpp"
#include "torc/generic/om/ParameterArrayElement.hpp"
#include "torc/generic/om/ParameterContext.hpp"
#include "torc/generic/om/ParameterMap.hpp"
#include "torc/generic/om/ParentedObject.hpp"
#include "torc/generic/om/PathDelay.hpp"
#include "torc/generic/om/Permutable.hpp"
#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/Port.hpp"
#include "torc/generic/om/PortAttributes.hpp"
#include "torc/generic/om/PortBundle.hpp"
#include "torc/generic/om/PortBundleReference.hpp"
#include "torc/generic/om/PortDelay.hpp"
#include "torc/generic/om/PortDirection.hpp"
#include "torc/generic/om/PortElement.hpp"
#include "torc/generic/om/PortList.hpp"
#include "torc/generic/om/PortListAlias.hpp"
#include "torc/generic/om/PortRefCreator.hpp"
#include "torc/generic/om/PortReference.hpp"
#include "torc/generic/om/Property.hpp"
#include "torc/generic/om/PropertyContainer.hpp"
#include "torc/generic/om/Renamable.hpp"
#include "torc/generic/om/Root.hpp"
#include "torc/generic/om/Scalar.hpp"
#include "torc/generic/om/ScalarNet.hpp"
#include "torc/generic/om/ScalarPort.hpp"
#include "torc/generic/om/ScalarPortReference.hpp"
#include "torc/generic/om/ScaleFactor.hpp"
#include "torc/generic/om/SelfReferencing.hpp"
#include "torc/generic/om/Simulate.hpp"
#include "torc/generic/om/SimulationInfo.hpp"
#include "torc/generic/om/SingleInstance.hpp"
#include "torc/generic/om/SingleParameter.hpp"
#include "torc/generic/om/Status.hpp"
#include "torc/generic/om/StatusContainer.hpp"
#include "torc/generic/om/SymTab.hpp"
#include "torc/generic/om/TimeStamp.hpp"
#include "torc/generic/om/Timing.hpp"
#include "torc/generic/om/Unit.hpp"
#include "torc/generic/om/UserDataContainer.hpp"
#include "torc/generic/om/Value.hpp"
#include "torc/generic/om/Vector.hpp"
#include "torc/generic/om/VectorBit.hpp"
#include "torc/generic/om/VectorNet.hpp"
#include "torc/generic/om/VectorNetBit.hpp"
#include "torc/generic/om/VectorPort.hpp"
#include "torc/generic/om/VectorPortBit.hpp"
#include "torc/generic/om/VectorPortBitReference.hpp"
#include "torc/generic/om/VectorPortReference.hpp"
#include "torc/generic/om/View.hpp"
#include "torc/generic/om/Visitable.hpp"
#include "torc/generic/om/VisitorApplier.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/WaveValue.hpp"
#include "torc/generic/om/Written.hpp"
#include "torc/generic/parser/Driver.hpp"
#include "torc/generic/parser/EdifContext.hpp"
#include "torc/generic/parser/EdifParser.hpp"
#include "torc/generic/parser/Linker.hpp"
#include "torc/generic/parser/ParserHelpers.hpp"
#include "torc/generic/parser/ParserOptions.hpp"
#include "torc/generic/parser/ParserPointerTypes.hpp"
#include "torc/generic/parser/Scanner.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/util/Log.hpp"
#include "torc/generic/util/Message.hpp"
#include "torc/generic/util/MessageId.hpp"
#include "torc/generic/util/MessageSeverity.hpp"
#include "torc/generic/util/MessageTable.hpp"

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
