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
#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/ConnectionHandler.hpp"
#include "torc/generic/om/Net.hpp"
#include "torc/generic/om/ScalarPortReference.hpp"
#include "torc/generic/om/PortBundleReference.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT( torc::generic::ScalarPortReference )
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

/**
 * Create a scalar port ref.
 *
 * @param[in] inInstancePtr Pointer to parented(Instance) object.
 * @param[in] inPortPtr Pointer to master(Port) object. 
 *
 * @return Pointer to created scalar port ref.
 **/
ScalarPortReferenceSharedPtr
ScalarPortReference::Factory::newScalarPortReferencePtr( const InstanceSharedPtr &inInstancePtr,
    const PortSharedPtr &inPortPtr,
    const PortBundleReferenceSharedPtr &inParentCollection ) throw(Error) {
    try
    {
        ScalarPortReferenceSharedPtr newScalarPortReference;
        create( newScalarPortReference );
        newScalarPortReference->bindToMasterPort( inPortPtr );
        if( inParentCollection )
        {
            inParentCollection->addChild( newScalarPortReference );
        }
        else
        {
            inInstancePtr->addPortReference( newScalarPortReference );
        }
        return newScalarPortReference;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
ScalarPortReference::accept(BaseVisitor & inoutVisitor) throw(Error) {
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

Connectable::Connection
ScalarPortReference::connect(
        const NetSharedPtr &inNet) throw(Error) {
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
        e.saveContextData("Scalar port reference Size", getSize() );
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

void
ScalarPortReference::disconnect(
    const Connection & inConnection) throw(Error) {
    NetSharedPtr connNet = *inConnection;
    if( !connNet )
    {
        Error e( eMessageIdErrorConnectionInvalid,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Net is invalid", connNet);
        throw e;
    }
    try
    {
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

ScalarPortReference::ScalarPortReference()
    :PortReference(),
    Scalar<PortReference>() {
}

ScalarPortReference::~ScalarPortReference() throw() {
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
ScalarPortReference::serialize( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object< PortReference >( *this );
    ar & boost::serialization::base_object<
                                    Scalar<PortReference> >(*this);
}

//TO SATISFY THE LINKER
template void
ScalarPortReference::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
ScalarPortReference::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
