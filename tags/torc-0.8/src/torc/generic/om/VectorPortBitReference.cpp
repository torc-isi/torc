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
#include "torc/generic/om/VectorPortBitReference.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT( torc::generic::VectorPortBitReference )
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

void
VectorPortBitReference::accept(
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

Connectable::Connection
VectorPortBitReference::connect(
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
        e.saveContextData("Vector Net Size", getSize() );
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
VectorPortBitReference::disconnect(
        const Connection &inConnection) throw(Error) {
    NetSharedPtr connNet = *inConnection;
    if( !connNet )
    {
        Error e( eMessageIdErrorPointerToItemDoesNotExist,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Pointer to the Net object does not exist", connNet);
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

void
VectorPortBitReference::getConnectedNets(
    std::vector< NetSharedPtr > &outNets,
    bool inSkipChildConnections 
        ) const throw(Error) {
    //First get connections exclusive to me
    PortReference::getConnectedNets( outNets );

    if( !inSkipChildConnections )
    {
        //Append cousins from nets that have been connected to my
        //parent
        Composite<PortReference>::Pointer parent
                                    = getParentCollection();
        if( !parent )
        {
            Error e( eMessageIdErrorPointerToItemDoesNotExist,
                __FUNCTION__, __FILE__, __LINE__ );
            e.saveContextData("Pointer to parent collection does not exist", parent);
            throw e;
        }
        if( eCompositionTypeVector
                    != parent->getCompositionType() )
        {
            Error e( eMessageIdErrorCompositionTypeMismatch,
                __FUNCTION__, __FILE__, __LINE__ );
            e.saveContextData("Composition type mismatch", parent->getCompositionType());
            throw e;
        }
        VectorBit<Net>::List parentConnections;
        parent->getConnectedNets( parentConnections, true );
        VectorBit<Net>::List::iterator connNet
                                    = parentConnections.begin();
        VectorBit<Net>::List::iterator connNetEnd
                                    = parentConnections.end();
        const std::vector<VectorBit<Net>::SizeType> &myIndex = getIndices();
        for(; connNet != connNetEnd; ++connNet )
        {
            Composite<Net>::Pointer cousin;
            if( eCompositionTypeVector
                    == (*connNet)->getCompositionType() )
            {
                cousin = (*connNet)->get( myIndex );
            }
            else
            {
                VectorBit<Net>::List bChildren;
                (*connNet)->getChildren( bChildren );
                cousin = bChildren[ getAbsoluteIndex() ];
            }
            if( !cousin )
            {
                Error e( eMessageIdErrorPointerToItemDoesNotExist,
                    __FUNCTION__, __FILE__, __LINE__ );
                e.saveContextData("Pointer to item does not exist", cousin);
                throw e;
            }
            outNets.push_back( cousin );
        }
    }
    return;
}

VectorPortBitReference::VectorPortBitReference()
    :PortReference(),
    VectorBit<PortReference>() {
}

VectorPortBitReference::~VectorPortBitReference() throw() {
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
VectorPortBitReference::serialize( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object< PortReference >( *this );
    ar & boost::serialization::base_object<
                                VectorBit<PortReference> >(*this);
}

//TO SATISFY THE LINKER
template void
VectorPortBitReference::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
VectorPortBitReference::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
