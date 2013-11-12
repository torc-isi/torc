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

#ifndef TORC_GENERIC_OBJECTFACTORY_HPP
#define TORC_GENERIC_OBJECTFACTORY_HPP

#include "torc/generic/Cell.hpp"
#include "torc/generic/Design.hpp"
#include "torc/generic/Library.hpp"
#include "torc/generic/InstanceArray.hpp"
#include "torc/generic/InstanceArrayMember.hpp"
#include "torc/generic/Root.hpp"
#include "torc/generic/NetBundle.hpp"
#include "torc/generic/Property.hpp"
#include "torc/generic/ParameterArray.hpp"
#include "torc/generic/ParameterArrayElement.hpp"
#include "torc/generic/PortBundle.hpp"
#include "torc/generic/PortList.hpp"
#include "torc/generic/PortBundleReference.hpp"
#include "torc/generic/ScalarNet.hpp"
#include "torc/generic/ScalarPort.hpp"
#include "torc/generic/ScalarPortReference.hpp"
#include "torc/generic/SingleInstance.hpp"
#include "torc/generic/SingleParameter.hpp"
#include "torc/generic/Status.hpp"
#include "torc/generic/VectorNet.hpp"
#include "torc/generic/VectorNetBit.hpp"
#include "torc/generic/VectorPort.hpp"
#include "torc/generic/VectorPortBit.hpp"
#include "torc/generic/VectorPortReference.hpp"
#include "torc/generic/VectorPortBitReference.hpp"
#include "torc/generic/View.hpp"
#include "torc/generic/Written.hpp"
#include "torc/generic/Permutable.hpp"
#include "torc/generic/InterfaceJoinedInfo.hpp"
#include "torc/generic/LogicValue.hpp"
#include "torc/generic/SimulationInfo.hpp"
#include "torc/generic/Simulate.hpp"
#include "torc/generic/Apply.hpp"
#include "torc/generic/LogicalResponse.hpp"
#include "torc/generic/LogicElement.hpp"
#include "torc/generic/WaveValue.hpp"
#include "torc/generic/Timing.hpp"
#include "torc/generic/PathDelay.hpp"
#include "torc/generic/Event.hpp"
#include "torc/generic/ForbiddenEvent.hpp"

namespace torc {
namespace generic {

/**
 * Creates objects of different leaf types. Provides a series of create functions for creating leaf
 * type objects. This factory is used by the parser to generate the object model. This can also be
 * used by the clients to create inherited objects by overriding specific create methods.
 *
 * @note The create method should be called as: boost::shared_ptr<Port> port =
 * factory->create<VectorPort>();
 */
class ObjectFactory :
	public Cell::Factory,
	public Design::Factory,
	public Library::Factory,
	public InstanceArray::Factory,
	public InstanceArrayMember::Factory,
	public NetBundle::Factory,
	public ParameterArray::Factory,
	public PortBundle::Factory,
	public PortBundleReference::Factory,
	public Property::Factory,
	public ParameterArrayElement::Factory,
	public PortList::Factory,
	public PortListAlias::Factory,
	public Root::Factory,
	public ScalarNet::Factory,
	public ScalarPort::Factory,
	public ScalarPortReference::Factory,
	public SingleInstance::Factory,
	public SingleParameter::Factory,
	public Status::Factory,
	public VectorNet::Factory,
	public VectorNetBit::Factory,
	public VectorPort::Factory,
	public VectorPortBit::Factory,
	public VectorPortReference::Factory,
	public VectorPortBitReference::Factory,
	public View::Factory,
	public Written::Factory,
	public Permutable::Factory,
	public InterfaceJoinedInfo::Factory,
	public LogicValue::Factory,
	public SimulationInfo::Factory,
	public Simulate::Factory,
	public Apply::Factory,
	public LogicalResponse::Factory,
	public LogicElement::Factory,
	public WaveValue::Factory,
	public Timing::Factory,
	public PathDelay::Factory,
	public Event::Factory,
	public ForbiddenEvent::Factory
{
public:

	using Cell::Factory::create;
	using Design::Factory::create;
	using Library::Factory::create;
	using InstanceArray::Factory::create;
	using InstanceArrayMember::Factory::create;
	using NetBundle::Factory::create;
	using ParameterArray::Factory::create;
	using PortBundle::Factory::create;
	using PortBundleReference::Factory::create;
	using Property::Factory::create;
	using ParameterArrayElement::Factory::create;
	using PortList::Factory::create;
	using PortListAlias::Factory::create;
	using Root::Factory::create;
	using ScalarNet::Factory::create;
	using ScalarPort::Factory::create;
	using ScalarPortReference::Factory::create;
	using SingleInstance::Factory::create;
	using SingleParameter::Factory::create;
	using Status::Factory::create;
	using VectorNet::Factory::create;
	using VectorNetBit::Factory::create;
	using VectorPort::Factory::create;
	using VectorPortBit::Factory::create;
	using VectorPortReference::Factory::create;
	using VectorPortBitReference::Factory::create;
	using View::Factory::create;
	using Written::Factory::create;
	using Permutable::Factory::create;
	using InterfaceJoinedInfo::Factory::create;
	using LogicValue::Factory::create;
	using SimulationInfo::Factory::create;
	using Simulate::Factory::create;
	using Apply::Factory::create;
	using LogicalResponse::Factory::create;
	using LogicElement::Factory::create;
	using WaveValue::Factory::create;
	using Timing::Factory::create;
	using PathDelay::Factory::create;
	using Event::Factory::create;
	using ForbiddenEvent::Factory::create;

	using Root::Factory::newRootPtr;
	using Library::Factory::newLibraryPtr;
	using Cell::Factory::newCellPtr;
	using Design::Factory::newDesignPtr;
	using View::Factory::newViewPtr;
	using ScalarNet::Factory::newScalarNetPtr;
	using VectorNet::Factory::newVectorNetPtr;
	using NetBundle::Factory::newNetBundlePtr;
	using ScalarPort::Factory::newScalarPortPtr;
	using VectorPort::Factory::newVectorPortPtr;
	using PortBundle::Factory::newPortBundlePtr;
	using ScalarPortReference::Factory::newScalarPortReferencePtr;
	using VectorPortReference::Factory::newVectorPortReferencePtr;
	using PortBundleReference::Factory::newPortBundleReferencePtr;
	using SingleInstance::Factory::newSingleInstancePtr;
	using InstanceArray::Factory::newInstanceArrayPtr;
	using SingleParameter::Factory::newSingleParameterPtr;
	using ParameterArray::Factory::newParameterArrayPtr;
	using Property::Factory::newPropertyPtr;
	using PortList::Factory::newPortListPtr;
	using Written::Factory::newWrittenPtr;
	using Status::Factory::newStatusPtr;
	using Permutable::Factory::newPermutablePtr;
	using InterfaceJoinedInfo::Factory::newInterfaceJoinedInfoPtr;
	using LogicValue::Factory::newLogicValuePtr;
	using SimulationInfo::Factory::newSimulationInfoPtr;
	using Simulate::Factory::newSimulatePtr;
	using Apply::Factory::newApplyPtr;
	using LogicalResponse::Factory::newLogicalResponsePtr;
	using LogicElement::Factory::newLogicElementPtr;
	using WaveValue::Factory::newWaveValuePtr;
	using Timing::Factory::newTimingPtr;
	using PathDelay::Factory::newPathDelayPtr;
	using Event::Factory::newEventPtr;
	using ForbiddenEvent::Factory::newForbiddenEventPtr;

	ObjectFactory();

	virtual ~ObjectFactory() throw();

};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_OBJECTFACTORY_HPP
