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

#include "torc/generic/ObjectFactory.hpp"

namespace torc {
namespace generic {

ObjectFactory::ObjectFactory() :
    Cell::Factory(),
    Design::Factory(),
    Library::Factory(),
    InstanceArray::Factory(),
    InstanceArrayMember::Factory(),
    NetBundle::Factory(),
    ParameterArray::Factory(),
    PortBundle::Factory(),
    PortBundleReference::Factory(),
    Property::Factory(),
    ParameterArrayElement::Factory(),
    PortList::Factory(),
    PortListAlias::Factory(),
    Root::Factory(),
    ScalarNet::Factory(),
    ScalarPort::Factory(),
    ScalarPortReference::Factory(),
    SingleInstance::Factory(),
    SingleParameter::Factory(),
    Status::Factory(),
    VectorNet::Factory(),
    VectorNetBit::Factory(),
    VectorPort::Factory(),
    VectorPortBit::Factory(),
    VectorPortReference::Factory(),
    VectorPortBitReference::Factory(),
    View::Factory(),
    Written::Factory(),
    Permutable::Factory(),
    InterfaceJoinedInfo::Factory(),
    LogicValue::Factory(), 
    SimulationInfo::Factory(),
    Simulate::Factory(),
    Apply::Factory(),
    LogicalResponse::Factory(),
    LogicElement::Factory(),
    WaveValue::Factory(),
    Timing::Factory(),
    PathDelay::Factory(),
    Event::Factory(),
    ForbiddenEvent::Factory() {}

ObjectFactory::~ObjectFactory() throw() {}

} // namespace generic
} // namespace torc
