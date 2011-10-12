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

#include "torc/generic/om/Port.hpp"
#include "torc/generic/om/PortReference.hpp"
#include "torc/generic/om/VectorNetBit.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT( torc::generic::VectorNetBit )
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

void
VectorNetBit::accept(
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
VectorNetBit::connect(
    const NetSharedPtr &inNet ) throw(Error) {
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
        e.saveContextData("VectorNetBit Size", getSize() );
        throw e;
    }
    return Connectable::connect( inNet );
}

void
VectorNetBit::disconnect(
    const Connection &inConnection) throw(Error) {
    Connectable::disconnect( inConnection );
}

void
VectorNetBit::getConnectedNets(
        std::vector< NetSharedPtr > &outNets,
        bool inSkipChildConnections ) const throw(Error) {
    Composite<Net>::Pointer parent = getParentCollection();
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
    //First get connections exclusive to me
    Net::getConnectedNets( outNets );

    if( !inSkipChildConnections )
    {
        //Append cousins from nets that have been connected to my
        //parent
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
                //We don't want to mess up preservation
                //by calling getChildren
            }
            else //Must be a bundle
            {
                VectorBit<Net>::List bChildren;
                (*connNet)->getChildren( bChildren );
                cousin = bChildren[ getAbsoluteIndex() ];
                            //We assume List is a vector
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
        //MAINTAINER_NOTE::Also check getConnectedPorts
        // getConnectedPortReferences as they follow the same logic
    }
    return;
}

void
VectorNetBit::getConnectedPorts(
    std::vector< PortSharedPtr > &outPorts,
    bool inSkipChildConnections
                ) const throw() {
    Composite<Net>::Pointer parent = getParentCollection();
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
    //First get connections exclusive to me
    Net::getConnectedPorts( outPorts );

    if( !inSkipChildConnections )
    {
        //Append cousins from nets that have been connected to my
        //parent
        VectorBit<Port>::List parentConnections;
        parent->getConnectedPorts( parentConnections, true );
        VectorBit<Port>::List::iterator connPort
                                    = parentConnections.begin();
        VectorBit<Port>::List::iterator connPortEnd
                                    = parentConnections.end();
        const std::vector<VectorBit<Net>::SizeType> &myIndex = getIndices();
        for(; connPort != connPortEnd; ++connPort )
        {
            Composite<Port>::Pointer cousin;
            //Maintainer Note:: Check comments in getConnectedNets
            if( eCompositionTypeVector
                    == (*connPort)->getCompositionType() )
            {
                cousin = (*connPort)->get( myIndex );
            }
            else
            {
                VectorBit<Port>::List bChildren;
                (*connPort)->getChildren( bChildren );
                cousin = bChildren[ getAbsoluteIndex() ];
            }
            if( !cousin )
            {
                Error e( eMessageIdErrorPointerToItemDoesNotExist,
                    __FUNCTION__, __FILE__, __LINE__ );
                e.saveContextData("Pointer to item does not exist", cousin);
                throw e;
            }
            outPorts.push_back( cousin );
        }
        //MAINTAINER_NOTE::Also check getConnectedNets
        // getConnectedPortReferences as they follow the same logic
    }
    return;
}

void
VectorNetBit::getConnectedPortRefs(
        std::vector< PortReferenceSharedPtr > &outPortRefs,
        bool inSkipChildConnections
    ) const throw() {
    Composite<Net>::Pointer parent = getParentCollection();
    if( !parent )
    {
        Error e( eMessageIdErrorPointerToItemDoesNotExist,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Pointer to parent collection does not exist", parent);
        throw e;
    }
    //Maintainer Note:: Check comments in getConnectedNets
    if( eCompositionTypeVector
                != parent->getCompositionType() )
    {
        Error e( eMessageIdErrorCompositionTypeMismatch,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Composition type mismatch", parent->getCompositionType());
        throw e;
    }
    //First get connections exclusive to me
    Net::getConnectedPortRefs ( outPortRefs );

    if( !inSkipChildConnections )
    {
        //Append cousins from nets that have been connected to my
        //parent
        VectorBit<PortReference>::List parentConnections;
        parent->getConnectedPortRefs( parentConnections, true );
        VectorBit<PortReference>::List::iterator connPortRef
                                    = parentConnections.begin();
        VectorBit<PortReference>::List::iterator connPortRefEnd
                                    = parentConnections.end();
        const std::vector<VectorBit<Net>::SizeType> &myIndex = getIndices();
        for(; connPortRef != connPortRefEnd; ++connPortRef )
        {
            Composite<PortReference>::Pointer cousin;
            //Maintainer note:: Check comments in getConnectedNets
            if( eCompositionTypeVector
                    == (*connPortRef)->getCompositionType() )
            {
                cousin = (*connPortRef)->get( myIndex );
            }
            else
            {
                VectorBit<PortReference>::List bChildren;
                (*connPortRef)->getChildren( bChildren );
                cousin = bChildren[ getAbsoluteIndex() ];
            }
            if( !cousin )
            {
                Error e( eMessageIdErrorPointerToItemDoesNotExist,
                    __FUNCTION__, __FILE__, __LINE__ );
                e.saveContextData("Pointer to item does not exist", cousin);
                throw e;
            }
            outPortRefs.push_back( cousin );
        }
        //MAINTAINER_NOTE::Also check getConnectedPorts
        // getConnectedNets as they follow the same logic
    }
    return;
}

VectorNetBit::VectorNetBit()
    : Net(),
    VectorBit<Net>() {
}

VectorNetBit::~VectorNetBit() throw() {
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
VectorNetBit::serialize( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object< Net >( *this );
    ar & boost::serialization::base_object< VectorBit<Net> >(*this);
}

//TO SATISFY THE LINKER
template void
VectorNetBit::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
VectorNetBit::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
