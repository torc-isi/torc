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
#include "torc/generic/om/Net.hpp"
#include "torc/generic/om/Port.hpp"
#include "torc/generic/om/VectorPortReference.hpp"
#include "torc/generic/om/PortBundleReference.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT( torc::generic::VectorPortReference )
#endif //GENOM_SERIALIZATION

namespace {

class PortBinder
        : public torc::generic::VectorPortBitReference::Visitor {
  public:    
    void
    visit( torc::generic::VectorPortBitReference &portBit
                                    ) throw(torc::generic::Error) {
        torc::generic::PortSharedPtr childPort
                    = mMasterVector->get( portBit.getIndices() );
        try
        {
            portBit.bindToMasterPort( childPort );
        }
        catch(torc::generic::Error &e)
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
        return;
    }

    PortBinder(
        const torc::generic::PortSharedPtr &inMasterVector )
        : mMasterVector( inMasterVector ) {
    }

    ~PortBinder() throw() {
    }

  private:
    torc::generic::PortSharedPtr mMasterVector;
};


}
namespace torc {

namespace generic {

/**
 * Create a vector port ref.
 *
 * @param[in] inInstancePtr Pointer to parented(Instance) object.
 * @param[in] inPortPtr Pointer to master(Port) object.
 * @param[in] inSize Size of the net array.
 * @param[in] inFactory Factory for the child.
 *
 * @return Pointer to created scalar port ref.
 **/
VectorPortReferenceSharedPtr
VectorPortReference::Factory::newVectorPortReferencePtr( const InstanceSharedPtr &inInstancePtr,
    const PortSharedPtr &inPortPtr,
    const size_t &inSize,
    const PortBundleReferenceSharedPtr &inParentCollection, 
    const ChildFactorySharedPtr &inFactory ) throw(Error) {
    try
    {
        std::vector<size_t> limits;
        limits.push_back( inSize );
        return newVectorPortReferencePtr( inInstancePtr, inPortPtr, limits, inParentCollection, inFactory );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
 * Create a vector port ref.
 *
 * @param[in] inInstancePtr Pointer to parented(Instance) object.
 * @param[in] inPortPtr Pointer to master(Port) object.
 * @param[in] inLimits Dimensions of the vector.
 * @param[in] inFactory Factory for the child.
 *
 * @return Pointer to created scalar port ref.
 **/
VectorPortReferenceSharedPtr
VectorPortReference::Factory::newVectorPortReferencePtr( const InstanceSharedPtr &inInstancePtr,
    const PortSharedPtr &inPortPtr,
    const std::vector<size_t> &inLimits,
    const PortBundleReferenceSharedPtr &inParentCollection, 
    const ChildFactorySharedPtr &inFactory ) throw(Error) {
    try
    {
        VectorPortReferenceSharedPtr newVectorPortReference;
        create( newVectorPortReference );
        newVectorPortReference->constructChildren( inFactory, inLimits);
        newVectorPortReference->bindToMasterPort( inPortPtr );
        if( inParentCollection )
        {
            inParentCollection->addChild( newVectorPortReference );
        }
        else
        {
            inInstancePtr->addPortReference( newVectorPortReference );
        }
        return newVectorPortReference;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
VectorPortReference::accept(
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

void
VectorPortReference::setParent(
        const InstanceSharedPtr &inParent ) throw() {
    ParentedObject<Instance>::setParent( inParent );
    BaseType::List children;
    getCreatedChildren( children );
    std::for_each( children.begin(), children.end(),
            boost::bind( boost::mem_fn(
                    &PortReference::setParent), _1, getParent() ));
}


/**
 * Connect a Net to this object.
 *
 * @note This metod can be overridden by derived classes. However, the method must call the on_connected() method after this. The sigConnected_ signal must also be invoked in the overriding method.
 *
 * @param[in] inNet A pointer to the Net object that eeds to be connected
 * @return A connection that has been established. This can be used later for disconnection.
 *
 * @exception Error Pointer to the Net object does not exist
 * @exception Error Net size does not match with VectorPortReference size 
 */
Connectable::Connection
VectorPortReference::connect(const NetSharedPtr &inNet) throw(Error) {
    if( !inNet )
    {
        Error e( eMessageIdErrorPointerToItemDoesNotExist,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Pointer to Net", inNet);
        throw e;
    }
    if( inNet->getSize() != getSize() )
    {
        Error e( eMessageIdErrorItemSizeMismatch,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Net Size", inNet->getSize());
        e.saveContextData("VectorPortReference Size", getSize() );
        throw e;
    }
    Connectable::Connection newConnection;
    try
    {
        ConnectionHandler handler( inNet );
        handler.connectPortRefToNet( getSharedThis() );
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
 * @exception Error Provided connection is invalid
 *
 */
void
VectorPortReference::disconnect(
    const Connectable::Connection &inConnection) throw(Error) {
    NetSharedPtr connNet = *inConnection;
    if( !connNet )
    {
        Error e( eMessageIdErrorPointerToItemDoesNotExist,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Pointer to Net", connNet);
        throw e;
    }
    try
    {
        BaseType::List children;
        getCreatedChildren( children );
        for_each( children.begin(), children.end(),
                    boost::bind( boost::mem_fn(
                        &Connectable::disconnect), _1));
        ConnectionHandler handler( connNet );
        handler.disconnectPortRefFromNet( getSharedThis() );
        Connectable::disconnect( inConnection );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
    return;
}

void
VectorPortReference::getConnectedNets(
    std::vector< NetSharedPtr > &outNets,
    bool inSkipChildConnections 
        ) const throw(Error) {
    PortReference::getConnectedNets( outNets );
    if( !inSkipChildConnections )
    {
        BaseType::List children;
        getCreatedChildren( children ); //We need only the
                        // bits that have been created
        BaseType::List::iterator pRef = children.begin();
        BaseType::List::iterator nEnd = children.end();
        for(; pRef != nEnd; ++pRef )
        {
            (*pRef)->getConnectedNets ( outNets, true );
        }
    }
    return;
}

/**
 * Set master port.
 *
 * @param[in] inSource Set the master port.
 */
void
VectorPortReference::bindToMasterPort(
    const PortSharedPtr &inMaster) throw(Error) {
    PortReference::bindToMasterPort( inMaster );

}

void
VectorPortReference::onChildCreate(
    const VectorPortBitReferenceSharedPtr &inCreatedChild
            ) const throw(Error) {
    PortSharedPtr masterPort = getMaster();
    if( masterPort )
    {
        PortBinder binder( masterPort );
        //We have now a handle to a bit whose parent's master is set
        //But it does not have a master itself. So we get the
        //corresponding bit from master
        try
        {
            inCreatedChild->accept( binder );
        }
        catch( Error &e )
        {
            e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
}

VectorPortReference::VectorPortReference()
    :PortReference(),
    BaseType()
{
}

VectorPortReference::~VectorPortReference() throw()
{
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
VectorPortReference::serialize( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object< PortReference >( *this );
    ar & boost::serialization::base_object< BaseType >(*this);
}

//TO SATISFY THE LINKER
template void
VectorPortReference::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
VectorPortReference::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
