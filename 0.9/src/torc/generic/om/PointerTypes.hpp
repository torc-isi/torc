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

#ifndef POINTER_TYPES_HPP
#define POINTER_TYPES_HPP

//BOOST
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace torc {
namespace generic {

class Cell;
typedef boost::shared_ptr<Cell> CellSharedPtr;
typedef boost::weak_ptr<Cell> CellWeakPtr;

class Design;
typedef boost::shared_ptr<Design> DesignSharedPtr;
typedef boost::weak_ptr<Design> DesignWeakPtr;

class View;
typedef boost::shared_ptr<View> ViewSharedPtr;
typedef boost::weak_ptr<View> ViewWeakPtr;

class Root;
typedef boost::shared_ptr<Root> RootSharedPtr;
typedef boost::weak_ptr<Root> RootWeakPtr;

class Library;
typedef boost::shared_ptr<Library> LibrarySharedPtr;
typedef boost::weak_ptr<Library> LibraryWeakPtr;

class Instance;
typedef boost::shared_ptr<Instance> InstanceSharedPtr;
typedef boost::weak_ptr<Instance> InstanceWeakPtr;

class SingleInstance;
typedef boost::shared_ptr<SingleInstance> SingleInstanceSharedPtr;
typedef boost::weak_ptr<SingleInstance> SingleInstanceWeakPtr;

class InstanceArray;
typedef boost::shared_ptr<InstanceArray> InstanceArraySharedPtr;
typedef boost::weak_ptr<InstanceArray> InstanceArrayWeakPtr;

class InstanceArrayMember;
typedef boost::shared_ptr<InstanceArrayMember> InstanceArrayMemberSharedPtr;
typedef boost::weak_ptr<InstanceArrayMember> InstanceArrayMemberWeakPtr;

class Net;
typedef boost::shared_ptr<Net> NetSharedPtr;
typedef boost::weak_ptr<Net> NetWeakPtr;

class ScalarNet;
typedef boost::shared_ptr<ScalarNet> ScalarNetSharedPtr;
typedef boost::weak_ptr<ScalarNet> ScalarNetWeakPtr;

class VectorNet;
typedef boost::shared_ptr<VectorNet> VectorNetSharedPtr;
typedef boost::weak_ptr<VectorNet> VectorNetWeakPtr;

class VectorNetBit;
typedef boost::shared_ptr<VectorNetBit> VectorNetBitSharedPtr;
typedef boost::weak_ptr<VectorNetBit> VectorNetBitWeakPtr;

class NetBundle;
typedef boost::shared_ptr<NetBundle> NetBundleSharedPtr;
typedef boost::weak_ptr<NetBundle> NetBundleWeakPtr;

class ObjectFactory;
typedef boost::shared_ptr<ObjectFactory> ObjectFactorySharedPtr;
typedef boost::weak_ptr<ObjectFactory> ObjectFactoryWeakPtr;

class Port;
typedef boost::shared_ptr<Port> PortSharedPtr;
typedef boost::weak_ptr<Port> PortWeakPtr;

class PortList;
typedef boost::shared_ptr<PortList> PortListSharedPtr;
typedef boost::weak_ptr<PortList> PortListWeakPtr;

class PortAttributes;
typedef boost::shared_ptr<PortAttributes> PortAttributesSharedPtr;
typedef boost::weak_ptr<PortAttributes> PortAttributesWeakPtr;

class PortBundle;
typedef boost::shared_ptr<PortBundle> PortBundleSharedPtr;
typedef boost::weak_ptr<PortBundle> PortBundleWeakPtr;

class PortBundleReference;
typedef boost::shared_ptr<PortBundleReference> PortBundleReferenceSharedPtr;
typedef boost::weak_ptr<PortBundleReference> PortBundleReferenceWeakPtr;

class PortReference;
typedef boost::shared_ptr<PortReference> PortReferenceSharedPtr;
typedef boost::weak_ptr<PortReference> PortReferenceWeakPtr;

class Parameter;
typedef boost::shared_ptr<Parameter> ParameterSharedPtr;
typedef boost::weak_ptr<Parameter> ParameterWeakPtr;

class SingleParameter;
typedef boost::shared_ptr<SingleParameter> SingleParameterSharedPtr;
typedef boost::weak_ptr<SingleParameter> SingleParameterWeakPtr;

class ParameterArray;
typedef boost::shared_ptr<ParameterArray> ParameterArraySharedPtr;
typedef boost::weak_ptr<ParameterArray> ParameterArrayWeakPtr;

class ParameterArrayElement;
typedef boost::shared_ptr<ParameterArrayElement> ParameterArrayElementSharedPtr;
typedef boost::weak_ptr<ParameterArrayElement> ParameterArrayElementWeakPtr;

class ParameterMap;
typedef boost::shared_ptr<ParameterMap> ParameterMapSharedPtr;
typedef boost::weak_ptr<ParameterMap> ParameterMapWeakPtr;

class Property;
typedef boost::shared_ptr<Property> PropertySharedPtr;
typedef boost::weak_ptr<Property> PropertyWeakPtr;

class PropertyContainer;
typedef boost::shared_ptr<PropertyContainer> PropertyContainerSharedPtr;
typedef boost::weak_ptr<PropertyContainer> PropertyContainerWeakPtr;

class ScalarNet;
typedef boost::shared_ptr<ScalarNet> ScalarNetSharedPtr;
typedef boost::weak_ptr<ScalarNet> ScalarNetWeakPtr;

class ScalarPort;
typedef boost::shared_ptr<ScalarPort> ScalarPortSharedPtr;
typedef boost::weak_ptr<ScalarPort> ScalarPortWeakPtr;

class ScalarPortReference;
typedef boost::shared_ptr<ScalarPortReference> ScalarPortReferenceSharedPtr;
typedef boost::weak_ptr<ScalarPortReference> ScalarPortReferenceWeakPtr;

class SingleParameter;
typedef boost::shared_ptr<SingleParameter> SingleParameterSharedPtr;
typedef boost::weak_ptr<SingleParameter> SingleParameterWeakPtr;

class SingleInstance;
typedef boost::shared_ptr<SingleInstance> SingleInstanceSharedPtr;
typedef boost::weak_ptr<SingleInstance> SingleInstanceWeakPtr;

class VectorNet;
typedef boost::shared_ptr<VectorNet> VectorNetSharedPtr;
typedef boost::weak_ptr<VectorNet> VectorNetWeakPtr;

class VectorPort;
typedef boost::shared_ptr<VectorPort> VectorPortSharedPtr;
typedef boost::weak_ptr<VectorPort> VectorPortWeakPtr;

class VectorPortReference;
typedef boost::shared_ptr<VectorPortReference> VectorPortReferenceSharedPtr;
typedef boost::weak_ptr<VectorPortReference> VectorPortReferenceWeakPtr;

class ParameterArray;
typedef boost::shared_ptr<ParameterArray> ParameterArraySharedPtr;
typedef boost::weak_ptr<ParameterArray> ParameterArrayWeakPtr;

class InstanceArray;
typedef boost::shared_ptr<InstanceArray> InstanceArraySharedPtr;
typedef boost::weak_ptr<InstanceArray> InstanceArrayWeakPtr;

class VectorPortBit;
typedef boost::shared_ptr<VectorPortBit> VectorPortBitSharedPtr;
typedef boost::shared_ptr<VectorPortBit> VectorPortBitWeakPtr;

class VectorPortBitReference;
typedef boost::shared_ptr<VectorPortBitReference> VectorPortBitReferenceSharedPtr;
typedef boost::shared_ptr<VectorPortBitReference> VectorPortBitReferenceWeakPtr;

class Error;
typedef boost::shared_ptr<Error> ErrorSharedPtr;
typedef boost::weak_ptr<Error> ErrorWeakPtr;

} // namespace torc::generic

} // namespace torc
#endif

