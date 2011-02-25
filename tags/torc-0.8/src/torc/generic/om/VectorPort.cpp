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

#include <algorithm>
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
#include "torc/generic/om/Net.hpp"
#include "torc/generic/om/VectorPort.hpp"
#include "torc/generic/om/PortBundle.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT( torc::generic::VectorPort )
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

/**
 * Create a vector port.
 *
 * @param[in] inName Name of the vector port to be created.
 * @param[in] inDirection Direction of port.
 * @param[in] inViewPtr Pointer to parented(View) object.
 * @param[in] inSize Size of the port array.
 * @param[in] inFactory Factory for the child.
 * @param[in] inOriginalName Original name of the vector port [optional].
 *
 * @return Pointer to created vector port.
 **/
VectorPortSharedPtr
VectorPort::Factory::newVectorPortPtr( const std::string &inName,
    const PortDirection &inDirection,
    const ViewSharedPtr &inViewPtr,
    const size_t &inSize,
    const PortBundleSharedPtr &inParentCollection,    
    const ChildFactorySharedPtr &inFactory,
    const std::string &inOriginalName ) throw(Error) {
    try
    {
        std::vector<size_t> limits;
        limits.push_back( inSize );
        return newVectorPortPtr(inName, inDirection, 
                    inViewPtr, limits, inParentCollection, inFactory, inOriginalName);
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
 * Create a vector port.
 *
 * @param[in] inName Name of the vector port to be created.
 * @param[in] inDirection Direction of port.
 * @param[in] inViewPtr Pointer to parented(View) object.
 * @param[in] inLimits Dimensions of the vector.
 * @param[in] inFactory Factory for the child.
 * @param[in] inOriginalName Original name of the vector port [optional].
 *
 * @return Pointer to created vector port.
 **/
VectorPortSharedPtr
VectorPort::Factory::newVectorPortPtr( const std::string &inName,
    const PortDirection &inDirection,
    const ViewSharedPtr &inViewPtr,
    const std::vector<size_t> &inLimits,
    const PortBundleSharedPtr &inParentCollection,
    const ChildFactorySharedPtr &inFactory,
    const std::string &inOriginalName ) throw(Error) {
    try
    {
        VectorPortSharedPtr newVectorPort;
        create( newVectorPort );
        newVectorPort->setName(inName);
        newVectorPort->setDirection( inDirection ),
        newVectorPort->constructChildren( inFactory, inLimits);
        newVectorPort->setOriginalName( inOriginalName );
        if( inParentCollection )
        {
            inParentCollection->addChild( newVectorPort );
        }
        else
        {
            inViewPtr->addPort( newVectorPort );
        }
        return newVectorPort;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}
void
VectorPort::accept(BaseVisitor & inoutVisitor) throw(Error) {
    try {
        runVisitor( *this, inoutVisitor );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
VectorPort::setParent(
        const ViewSharedPtr &inParent ) throw() {
    ParentedObject<View>::setParent( inParent );
    BaseVectorType::List children;
    getCreatedChildren( children );
    std::for_each( children.begin(), children.end(),
            boost::bind( boost::mem_fn(
                    &Port::setParent), _1, getParent() ));
}

/**
 * Connect a Net to this object.
 *
 * @note This metod can be overridden by derived classes. However, the method must call the on_connected() method after this. The sigConnected_ signal must also be invoked in the overriding method.
 *
 * @param[in] net A pointer to the Net object that eeds to be connected
 * @return A connection that has been established. This can be used later for disconnection.
 */
Connectable::Connection
VectorPort::connect(const NetSharedPtr &inNet) throw(Error) {
    if( !inNet )
    {
        Error e( eMessageIdErrorPointerToItemDoesNotExist,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Pointer to the Net object does not exist", inNet);
        throw e;
    }
    if( inNet->getSize() != getSize() )
    {
        Error e( eMessageIdErrorItemSizeMismatch,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Net Size", inNet->getSize());
        e.saveContextData("Vector Net Size", getSize() );
        throw e;
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
 *
 * Disconnect a Net from this object.
 * @note This metod can be overridden by derived classes. However, the method must call the on_connected() method after this. The sigConnected_ signal must also be invoked in the overriding method.
 *
 * @param[in] connection A connection as returned by the connect() method
 * @exception Error Provided connection is invalid
 */

void
VectorPort::disconnect(
    const Connectable::Connection &inConnection) throw(Error) {
    NetSharedPtr net = *inConnection;
    if( !net )
    {
        Error e( eMessageIdErrorPointerToItemDoesNotExist,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Pointer to the Net object does not exist", net);
        throw e;
    }
    try
    {
        ConnectionHandler handler( net );
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

void
VectorPort::getConnectedNets(
    std::vector< NetSharedPtr > &outNets,
    bool inSkipChildConnections
        ) const throw(Error) {
    Port::getConnectedNets( outNets );
    if( !inSkipChildConnections )
    {
        BaseVectorType::List children;
        getCreatedChildren( children ); //We need only the
                        // bits that have been created
        BaseVectorType::List::iterator port = children.begin();
        BaseVectorType::List::iterator pEnd = children.end();
        for(; port != pEnd; ++port )
        {
            (*port)->getConnectedNets ( outNets, true );
        }
    }
    return;
}

void
VectorPort::onChildCreate( const boost::shared_ptr<BaseVectorType::ChildType> &inCreatedChild
            ) const throw(Error) {
   inCreatedChild->setName( getName() ); 
}

VectorPort::VectorPort()
    : Port(),
    BaseVectorType() {
}

VectorPort::~VectorPort() throw() {
    torc::generic::log("Vector Port destroyed\n");
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
VectorPort::serialize( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object< Port >( *this );
    ar & boost::serialization::base_object< BaseVectorType >( *this );
}

//TO SATISFY THE LINKER
template void
VectorPort::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
VectorPort::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION


} // namespace torc::generic

} // namespace torc
