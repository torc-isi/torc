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

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

//BOOST
#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>
#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/ConnectionHandler.hpp"
#include "torc/generic/util/Log.hpp"
#include "torc/generic/om/PortBundle.hpp"
#include "torc/generic/om/Net.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT( torc::generic::PortBundle )
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

void
PortBundle::setParent(
        const ViewSharedPtr &inParent ) throw() {
    ParentedObject<View>::setParent( inParent );
    applyOnAllChildren(
                boost::bind( boost::mem_fn(
                    &Port::setParent), _1, getParent() ));
}

/**
 * Connect a Net to this object.
 *
 * @note This metod can be overridden by derived classes. However, the method must call the on_connected() method after this. The sigConnected_ signal must also be invoked in the overriding method.
 *
 * @param[in] inNet A pointer to the Net object that eeds to be connected
 * @return A connection that has been established. This can be used later for disconnection.
 */
Connectable::Connection
PortBundle::connect(
        const NetSharedPtr & inNet) throw(Error) {
    if( !inNet )
    {
        Error e( eMessageIdErrorPointerToItemDoesNotExist,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Pointer to the Net object does not exists", inNet);
        throw e;
    }
    if( inNet->getSize() != getSize() )
    {
        Error e( eMessageIdErrorItemSizeMismatch,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Net Size", inNet->getSize());
        e.saveContextData("Port Size", getSize() );
        throw e;
    }
    Bundle<Port>::List children;
    BundleFlattener<Port, ScalarPort, VectorPort, 
                        VectorPortBit, PortBundle> flattener;
    accept( flattener );
    flattener.getChildren( children );

    Bundle<Net>::List netChildren;
    inNet->getChildren( netChildren );
    Bundle<Port>::List::iterator childPort = children.begin();
    Bundle<Port>::List::iterator childEnd = children.end();
    Bundle<Net>::List::iterator net = netChildren.begin();
    for(; childPort != childEnd; ++childPort, ++net )
    {
        try
        {
            (*childPort)->connect( *net );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
    Connectable::Connection newConnection;
    try
    {
        ConnectionHandler handler( inNet );
        handler.connectPortToNet( getSharedThis() );
        newConnection = Connectable::connect( inNet );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
    return newConnection;
}

/**
 * Disconnect a Net from this object.
 * @note This metod can be overridden by derived classes. However, the method must call the on_connected() method after this. The sigConnected_ signal must also be invoked in the overriding method.
 *
 * @param[in] connection A connection as returned by the connect() method
 * @exception Error Provided connection is invalid, because pointer to the Net does not exist
 */
void
PortBundle::disconnect(
    const Connectable::Connection &inConnection) throw(Error) {
    NetSharedPtr connNet = *inConnection;
    if( !connNet )
    {
        Error e( eMessageIdErrorConnectionInvalid,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Pointer to Net", connNet);
        throw e;
    }
    Bundle<Port>::List children;
    BundleFlattener<Port, ScalarPort, VectorPort, 
                        VectorPortBit, PortBundle> flattener;
    accept( flattener );
    flattener.getChildren( children );
    Bundle<Net>::List netChildren;
    connNet->getChildren( netChildren );
    Bundle<Port>::List::iterator childPort = children.begin();
    Bundle<Port>::List::iterator childEnd = children.end();
    Bundle<Net>::List::iterator net = netChildren.begin();
    //We find all bits from the connected net and remove them
    //from the children nets. This disconnect is a little slow
    //as lookup is performed for each net. Otherwise we
    //would have to save the iterators (Connection objects)
    //for each child connection
    for(; childPort != childEnd; ++childPort, ++net )
    {
        try
        {
            (*childPort)->disconnect( *net );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
    try
    {
        ConnectionHandler handler( connNet );
        handler.disconnectPortFromNet( getSharedThis() );
        Connectable::disconnect( inConnection );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
* Create a port bundle.
*
* @param[in] inName Name of the port bundle to be created.
* @param[in] inDirection Direction of port.
* @param[in] inViewPtr Pointer to parented(View) object.
* @param[in] inParentCollection Pointer to parent bundle.
* @param[in] inOriginalName Original name of the port bundle [optional].
*
* @return Pointer to created port bundle.
**/
PortBundleSharedPtr
PortBundle::Factory::newPortBundlePtr( const std::string &inName,
    const ViewSharedPtr &inViewPtr,
    const PortBundleSharedPtr &inParentCollection,
    const std::string &inOriginalName ) throw(Error) {
    try
    {
        PortBundleSharedPtr newPortBundle;
        create( newPortBundle );
        newPortBundle->setName(inName);
        newPortBundle->setOriginalName( inOriginalName );
        if( inParentCollection )
        {
            inParentCollection->addChild( newPortBundle );
        }
        else
        {
            inViewPtr->addPort( newPortBundle );
        }
        return newPortBundle;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
PortBundle::accept(
    BaseVisitor & inoutVisitor) throw(Error) {
    try
    {
        runVisitor( *this, inoutVisitor );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

PortBundle::PortBundle()
    :Port(),
    Bundle<Port>() {
}

PortBundle::~PortBundle() throw() {
    torc::generic::log("Port bundle destroyed\n");
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
PortBundle::serialize( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object< Port >( *this );
    ar & boost::serialization::base_object< Bundle<Port> >(*this);
}

//TO SATISFY THE LINKER
template void
PortBundle::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
PortBundle::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
