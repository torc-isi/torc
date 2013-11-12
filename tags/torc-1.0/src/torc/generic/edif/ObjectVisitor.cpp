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

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif 

#include "torc/generic/edif/ObjectVisitor.hpp"

namespace torc {
namespace generic {

ObjectVisitor::ObjectVisitor() :
    Root::Visitor(),
    Library::Visitor(),
    Cell::Visitor(),
    Design::Visitor(),
    View::Visitor(),
    ScalarPort::Visitor(),
    VectorPort::Visitor(),
    VectorPortBit::Visitor(),
    PortBundle::Visitor(),
    ScalarNet::Visitor(),
    VectorNet::Visitor(),
    VectorNetBit::Visitor(),
    NetBundle::Visitor(),
    SingleInstance::Visitor(),
    InstanceArray::Visitor(),
    InstanceArrayMember::Visitor(),
    Property::Visitor(),
    SingleParameter::Visitor(),
    ParameterArray::Visitor(),
    ParameterArrayElement::Visitor(),
    ScalarPortReference::Visitor(),
    VectorPortReference::Visitor(),
    VectorPortBitReference::Visitor(),
    PortBundleReference::Visitor(),
    PortList::Visitor(),
    PortListAlias::Visitor(),
    Status::Visitor(),
    Permutable::Visitor(),
    InterfaceJoinedInfo::Visitor(),
    SimulationInfo::Visitor(),
    Simulate::Visitor(),
    LogicalResponse::Visitor(),
    LogicValue::Visitor(),
    LogicElement::Visitor(),
    WaveValue::Visitor(),
    Apply::Visitor(),
    Timing::Visitor(),
    Event::Visitor(),
    ForbiddenEvent::Visitor() {}

ObjectVisitor::~ObjectVisitor() throw() {}

} // namespace generic
} // namespace torc
