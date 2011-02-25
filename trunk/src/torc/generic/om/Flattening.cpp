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

#ifdef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif //HAVE_CONFIG_H

#include <algorithm>
#include <iterator>
#include <ostream>
#include <sstream>

#include "torc/generic/om/Flattening.hpp"

#include "torc/generic/om/View.hpp"
#include "torc/generic/om/SingleInstance.hpp"
#include "torc/generic/om/InstanceArray.hpp"
#include "torc/generic/om/InstanceArrayMember.hpp"
#include "torc/generic/om/Port.hpp"
#include "torc/generic/om/ScalarPortReference.hpp"
#include "torc/generic/om/VectorPortReference.hpp"
#include "torc/generic/om/VectorPortBitReference.hpp"
#include "torc/generic/om/PortBundleReference.hpp"
#include "torc/generic/om/PortList.hpp"
#include "torc/generic/om/Net.hpp"
#include "torc/generic/util/Log.hpp"
#include "torc/generic/om/Cloning.hpp"

namespace {

using namespace torc::generic;

template<typename _Pointer>
std::string
getModifiedName( const std::string &inInstName,
        const _Pointer &inNameable )
{
    std::string name = inInstName + "_";
    name += inNameable->getName();
    return name;
}

void
replicatePortRefConnections( const NetSharedPtr &inOrigNet,
        const NetSharedPtr &outTargetNet,
        const ViewSharedPtr &inCurrentView,
        const std::string &inInstName ) {
    log("Replicating portRef connections for %s to %s\n",
            inOrigNet->getName().c_str(),
            outTargetNet->getName().c_str());
    std::vector<PortReferenceSharedPtr> portRefs;
    bool isBit =    eCompositionTypeVectorBit
            == inOrigNet->getCompositionType();
    inOrigNet->getConnectedPortRefs( portRefs, !isBit );
    for( std::vector<PortReferenceSharedPtr>::iterator ref
            = portRefs.begin(); ref != portRefs.end(); ++ref )
    {
        std::vector<std::string> nestedNames;
        PortReferenceSharedPtr actualPortRef = *ref;
        PortReferenceSharedPtr portRef = actualPortRef;
        InstanceSharedPtr instance;
        while( portRef )
        {
            if( eCompositionTypeVectorBit
                            != portRef->getCompositionType())
            {
                nestedNames.push_back( portRef->getName() );
                instance = portRef->getParent();
            }
            portRef = portRef->getParentCollection();
        }
        std::vector<size_t> indices;
        if( eCompositionTypeVectorBit
                == instance->getCompositionType() )
        {
            indices
                = IndexFinder<Instance, InstanceArrayMember>()(
                                                    instance );
            instance = instance->getParentCollection();
        }
        std::string newInstName
                    = getModifiedName( inInstName, instance );
        InstanceSharedPtr targetInst
                = inCurrentView->findInstance( newInstName );
        if( !indices.empty() )
        {
            targetInst = targetInst->get( indices );
        }
        std::string portName = *nestedNames.rbegin();
        PortReferenceSharedPtr targetPortRef
                    = targetInst->findPortReference( portName );
        if( !targetPortRef )
        {
            //TBD::ERROR
        }
        if( nestedNames.size() > 1 )
        {
            findLeafConnectable( nestedNames, targetPortRef );
        }
        if( eCompositionTypeVectorBit
                == actualPortRef->getCompositionType() )
        {
            std::vector<size_t> portRefIndices
                            = IndexFinder<PortReference,
                    VectorPortBitReference>()(actualPortRef);
            targetPortRef = targetPortRef->get( portRefIndices );
            //copy( portRefIndices.begin(), portRefIndices.end(),
            //        std::ostream_iterator<size_t>(std::cout, " "));
        }
        log("\tConnecting %s to net %s\n",
            targetPortRef->getName().c_str(),
            outTargetNet->getName().c_str());
        log("\tIndices: ");
        targetPortRef->connect( outTargetNet );
    }
}

class PortRefConnectionReplicator
    : ScalarNet::Visitor,
    VectorNet::Visitor,
    VectorNetBit::Visitor,
    NetBundle::Visitor {
  private:

  public:

    void
    visit( ScalarNet &inScalarNet ) throw(Error)
    try
    {
        replicatePortRefConnections(
            inScalarNet.getSharedThis(), mTargetNet,
            mCurrentView, mInstName );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

    void
    visit( VectorNet &inVectorNet ) throw(Error)
    try
    {
        replicatePortRefConnections(
            inVectorNet.getSharedThis(), mTargetNet,
            mCurrentView, mInstName );
#if 0
        std::vector< NetSharedPtr > children;
        inVectorNet.getCreatedChildren( children );
        for( std::vector< NetSharedPtr >::iterator it
                = children.begin(); it != children.end(); ++it )
        {
            NetSharedPtr childNet = *it;
            PortRefConnectionReplicator()( childNet,
                mTargetNet->get(
                    IndexFinder<PortReference,
                        VectorPortBitReference>()( childNet )),
                mCurrentView, mInstName );
        }
#endif
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

    void
    visit( VectorNetBit &inVectorNetBit ) throw(Error)
    try
    {
        replicatePortRefConnections(
            inVectorNetBit.getSharedThis(), mTargetNet,
            mCurrentView, mInstName );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

    void
    visit( NetBundle &inNetBundle ) throw(Error)
    try
    {
        replicatePortRefConnections(
            inNetBundle.getSharedThis(), mTargetNet,
            mCurrentView, mInstName );
#if 0
        std::vector< NetSharedPtr > children;
        inNetBundle.getChildren( children );
        std::vector< NetSharedPtr > targetChildren;
        mTargetNet->getChildren( targetChildren );
        std::vector< NetSharedPtr >::iterator target
                                = targetChildren.begin();
        for( std::vector< NetSharedPtr >::iterator it
                = children.begin(); it != children.end();
                ++it, ++target )
        {
            PortRefConnectionReplicator()( *it, *target,
                mCurrentView, mInstName );
        }
#endif
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

    void
    operator ()( const NetSharedPtr &inOrigNet,
            const NetSharedPtr &outTargetNet,
            const ViewSharedPtr &inCurrentView,
            const std::string &inInstName ) throw(Error)
    try
    {
        mTargetNet = outTargetNet;
        mCurrentView = inCurrentView;
        mInstName = inInstName;
        inOrigNet->accept( *this );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

    ~PortRefConnectionReplicator() throw() {
    }

  private:
    NetSharedPtr mTargetNet;
    ViewSharedPtr mCurrentView;
    std::string mInstName;

};

void
replicateNetConnections( const NetSharedPtr &inOrigNet,
        const ViewSharedPtr &inCurrentView,
        const InstanceSharedPtr &inInstance,
        const ObjectFactorySharedPtr &inFactory,
        const std::string &inInstName,
        NetSharedPtr &outClonedNet,
        const NetSharedPtr &inTargetNet ) throw(Error) 
try
{

    std::vector<PortSharedPtr> ports;
    inOrigNet->getConnectedPorts( ports, true );
    if( ports.empty() )
    {
        NetSharedPtr targetNet;
        if( inTargetNet )
        {
            targetNet = inTargetNet;
        }
        else
        {
            NetSharedPtr clonedNet
                        = clone( inOrigNet, inFactory );
            clonedNet->setName( getModifiedName(
                                inInstName, inOrigNet) );
            inCurrentView->addNet( clonedNet );
            outClonedNet = clonedNet;
            targetNet = clonedNet;
        }
        PortRefConnectionReplicator()( inOrigNet, targetNet,
            inCurrentView, inInstName);
    }
    else
    {
        for( std::vector<PortSharedPtr>::iterator port
                = ports.begin(); port != ports.end(); ++port )
        {
            std::vector<std::string> nestedNames;
            PortSharedPtr actualPortRef = *port;
            PortSharedPtr portRef = actualPortRef;
            while( portRef )
            {
                if( eCompositionTypeVectorBit
                            != portRef->getCompositionType())
                {
                    nestedNames.push_back(
                                    portRef->getName() );
                }
                portRef = portRef->getParentCollection();
            }
            std::string portName = *nestedNames.rbegin();
            PortReferenceSharedPtr origPortRef
                = inInstance->findPortReference( portName );
            if( nestedNames.size() > 1 )
            {
                findLeafConnectable(
                            nestedNames, origPortRef );
            }
            std::vector<NetSharedPtr> nets;
            origPortRef->getConnectedNets( nets );
            for( std::vector<NetSharedPtr>::iterator
                myNet = nets.begin();
                myNet != nets.end(); ++myNet )
            {
                NetSharedPtr connectedNet = *myNet;
                PortRefConnectionReplicator()(
                    inOrigNet, connectedNet, inCurrentView,
                    inInstName);
                origPortRef->disconnect( connectedNet );
            }
        }
    }
}
catch( Error &e )
{
    e.setCurrentLocation(
        __FUNCTION__, __FILE__, __LINE__ );
    throw;
}

class NetConnectionReplicator
    : ScalarNet::Visitor,
    VectorNet::Visitor,
    VectorNetBit::Visitor,
    NetBundle::Visitor {
  public:

    void
    visit( ScalarNet &inScalarNet ) throw(Error)
    try
    {
        replicateNetConnections(
            inScalarNet.getSharedThis(), mCurrentView,
            mInstance, mFactory, mInstName, mClonedNet,
            mTargetNet);
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
    void
    visit( VectorNet &inVectorNet ) throw(Error)
    try
    {
        std::vector<NetSharedPtr> children;
        inVectorNet.getCreatedChildren( children );
        NetSharedPtr targetVector = mClonedNet;
        for( std::vector<NetSharedPtr>::iterator it
            = children.begin(); it != children.end();
            ++it)
        {
            NetConnectionReplicator()( *it, mCurrentView,
                mInstance, mFactory,
                mClonedNet, targetVector, mInstName);
        }
        replicateNetConnections(
            inVectorNet.getSharedThis(), mCurrentView,
            mInstance, mFactory, mInstName, mClonedNet,
            mTargetNet);
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
    void
    visit( VectorNetBit &inVectorNetBit ) throw(Error)
    try
    {
#if 0
        replicateNetConnections(
            inVectorNetBit.getSharedThis(), mCurrentView,
            mInstance, mFactory, mInstName, mClonedNet,
            mTargetNet->get(inVectorNetBit.getIndices()));
#endif
        NetSharedPtr origNet = inVectorNetBit.getSharedThis();
        std::vector<PortSharedPtr> ports;
        inVectorNetBit.getConnectedPorts( ports );

        //We now create scalar net and move my connections
        //to this one
        ScalarNetSharedPtr newScalarNet;
        mFactory->create( newScalarNet );
        std::ostringstream sout;
        sout<<mInstName<<"_"<<origNet->getName()<<"_";
        std::vector<size_t> indices
            = IndexFinder<Net, VectorNetBit>()(
                                            origNet );
        copy( indices.begin(), indices.end(),
            std::ostream_iterator<size_t>( sout, "_"));
        newScalarNet->setName( sout.str() );
        mCurrentView->addNet( newScalarNet );
        PortRefConnectionReplicator()(
            origNet, newScalarNet, mCurrentView, mInstName);

        for( std::vector<PortSharedPtr>::iterator port
                = ports.begin(); port != ports.end(); ++port )
        {
            std::vector<std::string> nestedNames;
            PortSharedPtr actualPort = *port;
            PortSharedPtr portRef = actualPort;
            while( portRef )
            {
                if( eCompositionTypeVectorBit
                            != portRef->getCompositionType())
                {
                    nestedNames.push_back(
                                    portRef->getName() );
                }
                portRef = portRef->getParentCollection();
            }
            std::string portName = *nestedNames.rbegin();
            PortReferenceSharedPtr origPortRef
                = mInstance->findPortReference( portName );
            if( nestedNames.size() > 1 )
            {
                findLeafConnectable(
                            nestedNames, origPortRef );
            }
            if( eCompositionTypeVectorBit
                    == actualPort->getCompositionType() )
            {
                origPortRef = origPortRef->get(
                        IndexFinder<Port,VectorPortBit>()(
                                        actualPort ));
            }
            //Now have the port ref for this bit
            //find all ports and refs that are connected to
            //this ref and connect them to the newly created
            //net
            std::vector<NetSharedPtr> nets;
            origPortRef->getConnectedNets( nets );
            for( std::vector<NetSharedPtr>::iterator
                    myNet = nets.begin();
                    myNet != nets.end(); myNet++ )
            {
                std::vector<PortSharedPtr> cPorts;
                (*myNet)->getConnectedPorts( cPorts );
                for( std::vector<PortSharedPtr>::iterator it
                        = cPorts.begin(); it != cPorts.end();
                        ++it )
                {
                    (*it)->connect( newScalarNet );
                }
                std::vector<PortReferenceSharedPtr> cPortReferences;
                (*myNet)->getConnectedPortRefs(
                                            cPortReferences );
                for( std::vector<PortReferenceSharedPtr>::iterator it
                        = cPortReferences.begin(); it != cPortReferences.end();
                        ++it )
                {
                    if( *it == origPortRef )
                        continue;
                    (*it)->connect( newScalarNet );
                }
                bool toDisconnect = true;
                if( eCompositionTypeVectorBit == (*myNet)->getCompositionType()
                    && eCompositionTypeVectorBit == origPortRef->getCompositionType() )
                {
                    std::vector<size_t> netIdx
                            = IndexFinder<Net, VectorNetBit>()( *myNet );
                    std::vector<size_t> portIdx
                            = IndexFinder<PortReference, VectorPortBitReference>()( origPortRef );
                    toDisconnect = !( netIdx == portIdx );
                }
            }
        }
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

    void
    visit( NetBundle &inNetBundle ) throw(Error)
    try
    {
        replicateNetConnections(
            inNetBundle.getSharedThis(), mCurrentView,
            mInstance, mFactory, mInstName, mClonedNet,
            mTargetNet);
        std::vector<NetSharedPtr> children;
        inNetBundle.getChildren( children );
        std::vector<NetSharedPtr> tChildren;
        mClonedNet->getChildren( tChildren );
        std::vector<NetSharedPtr>::iterator tNet
                                        = tChildren.begin();
        for( std::vector<NetSharedPtr>::iterator it
            = children.begin(); it != children.end();
            ++it, ++tNet)
        {
            NetConnectionReplicator()( *it, mCurrentView,
                mInstance, mFactory,
                mClonedNet, *tNet, mInstName);
        }
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

    void
    operator ()( const NetSharedPtr &inOrigNet,
            const ViewSharedPtr &inCurrentView,
            const InstanceSharedPtr &inInstance,
            const ObjectFactorySharedPtr &inFactory,
            NetSharedPtr &outClonedNet,
            NetSharedPtr &inTargetNet,
            const std::string &inInstName = std::string() ) throw(Error)
    try
    {
        mCurrentView = inCurrentView;
        mInstance = inInstance;
        mFactory = inFactory;
        mInstName = (inInstName.empty())
                    ? inInstance->getName()
                    : inInstName;
        mTargetNet = inTargetNet;
        inOrigNet->accept( *this );
        outClonedNet = mClonedNet;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

    ~NetConnectionReplicator() throw() {
    }

  private:
    ViewSharedPtr mCurrentView;
    InstanceSharedPtr mInstance;
    ObjectFactorySharedPtr mFactory;
    std::string mInstName;
    NetSharedPtr mClonedNet;
    NetSharedPtr mTargetNet;
};

void
flattenInstance(const ViewSharedPtr &inParentView,
        const InstanceSharedPtr &inInstance,
        const ObjectFactorySharedPtr &inFactory,
        const std::string &inName = std::string() ) throw(Error)
{
    std::string name = (!inName.empty())
                        ? inName
                        : inInstance->getName();
    ViewSharedPtr masterView = inInstance->getMaster();
    log( "Flattining instance with name %s\n", name.c_str());

    log("Copying instantiations... ");
    std::vector<InstanceSharedPtr> childInstances;
    masterView->getInstances( childInstances );
    std::vector<PortReferenceSharedPtr> portRefs;
    inInstance->getPortReferences( portRefs );
    for( std::vector<InstanceSharedPtr>::iterator it
            = childInstances.begin(); it != childInstances.end();
            ++it)
    {
        InstanceSharedPtr inst = clone( *it, inFactory );
        std::string instName = getModifiedName(name,inst);
        inst->setName( instName );
        inParentView->addInstance( inst );
    }
    log("Done\n");
    //First we copy nets in the instance that are not connected
    //to it's ports
    log("Copying internal nets... ");
    std::vector<NetSharedPtr> allNets;
    masterView->getNets( allNets );
    for( std::vector<NetSharedPtr>::iterator it = allNets.begin();
                it != allNets.end(); ++it )
    {
        NetSharedPtr origNet = *it;
        NetSharedPtr clonedNet;
        NetSharedPtr dummy;
        NetConnectionReplicator()(
                origNet, inParentView, inInstance,
                inFactory, clonedNet, dummy );
    }
    log("Done\n");
}

}

namespace torc {
namespace generic {

void
flatten( const ViewSharedPtr &inParentView,
        const InstanceSharedPtr &inInstance,
        const ObjectFactorySharedPtr &inFactory ) throw(Error)
{
    if( !inParentView || !inInstance
            || !inParentView->findInstance( inInstance->getName() ))
    {
        //TBD::ERROR
    }
    switch( inInstance->getCompositionType() )
    {
        case eCompositionTypeScalar:
        {
            try
            {
                flattenInstance(
                    inParentView, inInstance, inFactory );
                inParentView->removeInstance(
                                inInstance->getName());
                break;
            }
            catch( Error &e )
            {
                e.setCurrentLocation(
                    __FUNCTION__, __FILE__, __LINE__ );
                throw;
            }

        }
        case eCompositionTypeVector:
        {
            try
            {
                std::vector<InstanceSharedPtr> children;
                inInstance->getChildren( children );
                for( std::vector<InstanceSharedPtr>::iterator it
                        = children.begin(); it != children.end(); ++it )
                {
                    flattenInstance(
                        inParentView, (*it), inFactory );
                }
                inParentView->removeInstance(
                                inInstance->getName());
            }
            catch( Error &e )
            {
                e.setCurrentLocation(
                    __FUNCTION__, __FILE__, __LINE__ );
                throw;
            }
            break;
        }
        default:
        {
            throw Error( eMessageIdErrorUnsupoortedOperation,
                    __FUNCTION__, __FILE__, __LINE__ );
        }
    }
}

} //namespace generic
} //namespace torc
