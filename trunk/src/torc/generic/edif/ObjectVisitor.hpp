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

#ifndef TORC_GENERIC_EDIF_OBJECTVISITOR_HPP
#define TORC_GENERIC_EDIF_OBJECTVISITOR_HPP

#include "torc/generic/Root.hpp"
#include "torc/generic/Library.hpp"
#include "torc/generic/Cell.hpp"
#include "torc/generic/Design.hpp"
#include "torc/generic/View.hpp"
#include "torc/generic/ScalarPort.hpp"
#include "torc/generic/VectorPort.hpp"
#include "torc/generic/VectorPortBit.hpp"
#include "torc/generic/PortBundle.hpp"
#include "torc/generic/ScalarNet.hpp"
#include "torc/generic/VectorNet.hpp"
#include "torc/generic/VectorNetBit.hpp"
#include "torc/generic/NetBundle.hpp"
#include "torc/generic/SingleInstance.hpp"
#include "torc/generic/InstanceArray.hpp"
#include "torc/generic/InstanceArrayMember.hpp"
#include "torc/generic/SingleParameter.hpp"
#include "torc/generic/ParameterArray.hpp"
#include "torc/generic/ParameterArrayElement.hpp"
#include "torc/generic/Property.hpp"
#include "torc/generic/ScalarPortReference.hpp"
#include "torc/generic/VectorPortReference.hpp"
#include "torc/generic/VectorPortBitReference.hpp"
#include "torc/generic/PortBundleReference.hpp"
#include "torc/generic/PortList.hpp"
#include "torc/generic/PortListAlias.hpp"
#include "torc/generic/Status.hpp"
#include "torc/generic/Permutable.hpp"
#include "torc/generic/InterfaceJoinedInfo.hpp"
#include "torc/generic/SimulationInfo.hpp"
#include "torc/generic/Simulate.hpp"
#include "torc/generic/LogicalResponse.hpp"
#include "torc/generic/LogicValue.hpp"
#include "torc/generic/LogicElement.hpp"
#include "torc/generic/WaveValue.hpp"
#include "torc/generic/Apply.hpp"
#include "torc/generic/Timing.hpp"
#include "torc/generic/Event.hpp"
#include "torc/generic/ForbiddenEvent.hpp"

namespace torc {
namespace generic {

/**
 * A inoutVisitor that can visit all types of leaf type elements in the library. All visit methods
 * will need to be overridden in the deriving class.
 */
class ObjectVisitor : public Root::Visitor, public Library::Visitor, public Cell::Visitor,
	public Design::Visitor, public View::Visitor, public ScalarPort::Visitor,
	public VectorPort::Visitor, public VectorPortBit::Visitor, public PortBundle::Visitor,
	public ScalarNet::Visitor, public VectorNet::Visitor, public VectorNetBit::Visitor,
	public NetBundle::Visitor, public SingleInstance::Visitor, public InstanceArray::Visitor,
	public InstanceArrayMember::Visitor, public Property::Visitor, public SingleParameter::Visitor,
	public ParameterArray::Visitor, public ParameterArrayElement::Visitor,
	public ScalarPortReference::Visitor, public VectorPortReference::Visitor,
	public VectorPortBitReference::Visitor, public PortBundleReference::Visitor,
	public PortList::Visitor, public PortListAlias::Visitor, public Status::Visitor,
	public Permutable::Visitor, public InterfaceJoinedInfo::Visitor, public SimulationInfo::Visitor,
	public Simulate::Visitor, public LogicalResponse::Visitor, public LogicValue::Visitor,
	public LogicElement::Visitor, public WaveValue::Visitor, public Apply::Visitor,
	public Timing::Visitor, public Event::Visitor, public ForbiddenEvent::Visitor {

protected:
	ObjectVisitor();
public:
	virtual
	~ObjectVisitor() throw ();
};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_EDIF_OBJECTVISITOR_HPP
