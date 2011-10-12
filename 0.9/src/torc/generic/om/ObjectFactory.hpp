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

#ifndef TORC_GENERIC_OBJECT_FACTORY_HPP
#define TORC_GENERIC_OBJECT_FACTORY_HPP


#include "torc/generic/om/Cell.hpp"
#include "torc/generic/om/Design.hpp"
#include "torc/generic/om/Library.hpp"
#include "torc/generic/om/InstanceArray.hpp"
#include "torc/generic/om/InstanceArrayMember.hpp"
#include "torc/generic/om/Root.hpp"
#include "torc/generic/om/NetBundle.hpp"
#include "torc/generic/om/Property.hpp"
#include "torc/generic/om/ParameterArray.hpp"
#include "torc/generic/om/ParameterArrayElement.hpp"
#include "torc/generic/om/PortBundle.hpp"
#include "torc/generic/om/PortList.hpp"
#include "torc/generic/om/PortBundleReference.hpp"
#include "torc/generic/om/ScalarNet.hpp"
#include "torc/generic/om/ScalarPort.hpp"
#include "torc/generic/om/ScalarPortReference.hpp"
#include "torc/generic/om/SingleInstance.hpp"
#include "torc/generic/om/SingleParameter.hpp"
#include "torc/generic/om/VectorNet.hpp"
#include "torc/generic/om/VectorNetBit.hpp"
#include "torc/generic/om/VectorPort.hpp"
#include "torc/generic/om/VectorPortBit.hpp"
#include "torc/generic/om/VectorPortReference.hpp"
#include "torc/generic/om/VectorPortBitReference.hpp"
#include "torc/generic/om/View.hpp"

namespace torc {

namespace generic {

/**
 * Creates objects of different leaf types. Provids a series of create functions for creating leaf type objects. This factory is used by the parser to generate the object model. This can also be used by the clients to create inherited objects by overriding specifc create methods.
 *
 * @note The create method should be called as: boost::shared_ptr<Port> port = factory->create<VectorPort>();
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
    public Root::Factory,
    public ScalarNet::Factory,
    public ScalarPort::Factory,
    public ScalarPortReference::Factory,
    public SingleInstance::Factory,
    public SingleParameter::Factory,
    public VectorNet::Factory,
    public VectorNetBit::Factory,
    public VectorPort::Factory,
    public VectorPortBit::Factory,
    public VectorPortReference::Factory,
    public VectorPortBitReference::Factory,
    public View::Factory
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
    using Root::Factory::create;
    using ScalarNet::Factory::create;
    using ScalarPort::Factory::create;
    using ScalarPortReference::Factory::create;
    using SingleInstance::Factory::create;
    using SingleParameter::Factory::create;
    using VectorNet::Factory::create;
    using VectorNetBit::Factory::create;
    using VectorPort::Factory::create;
    using VectorPortBit::Factory::create;
    using VectorPortReference::Factory::create;
    using VectorPortBitReference::Factory::create;
    using View::Factory::create;

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
    
    ObjectFactory();

    virtual
    ~ObjectFactory() throw();

};

} // namespace torc::generic

} // namespace torc
#endif
