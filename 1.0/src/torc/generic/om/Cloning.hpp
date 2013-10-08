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

#ifndef TORC_GENERIC_OM_CLONING_HPP
#define TORC_GENERIC_OM_CLONING_HPP

#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/util/Error.hpp"

#include "torc/generic/om/Root.hpp"
#include "torc/generic/om/Library.hpp"
#include "torc/generic/om/Cell.hpp"
#include "torc/generic/om/View.hpp"
#include "torc/generic/om/ScalarPort.hpp"
#include "torc/generic/om/VectorPort.hpp"
#include "torc/generic/om/PortBundle.hpp"
#include "torc/generic/om/ScalarNet.hpp"
#include "torc/generic/om/VectorNet.hpp"
#include "torc/generic/om/NetBundle.hpp"
#include "torc/generic/om/SingleInstance.hpp"
#include "torc/generic/om/InstanceArray.hpp"
#include "torc/generic/om/SingleParameter.hpp"
#include "torc/generic/om/ParameterArray.hpp"
#include "torc/generic/om/Property.hpp"
#include "torc/generic/om/ScalarPortReference.hpp"
#include "torc/generic/om/VectorPortReference.hpp"
#include "torc/generic/om/PortBundleReference.hpp"
#include "torc/generic/om/ObjectFactory.hpp"
#include "torc/generic/om/InternalUtilityFunctions.hpp"

namespace torc {

namespace generic {

namespace _impl { template<typename _Tp> class Copier; }    

extern void
copyParams( const Instance &inOriginal,
            const InstanceSharedPtr &outCloned,
            const ObjectFactorySharedPtr &inFactory );

template<typename _Tp>
boost::shared_ptr<_Tp>
clone( const boost::shared_ptr<_Tp> &inPointer,
        const ObjectFactorySharedPtr &inFactory ) throw(Error)
{
    try
    {
        return _impl::Copier<_Tp>()( inPointer, inFactory );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

namespace _impl {

template<typename _Tp>
class Copier {
  public:    
      typedef _Tp Type;
      typedef boost::shared_ptr<Type> Pointer;

  public:

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        throw Error( eMessageIdErrorUnsupoortedOperation,
                        __FUNCTION__, __FILE__, __LINE__ );
        return mReturnVal;
    }

    Copier()
        :mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

#if 0

// For Root
template<>
class Copier<class Root>
    :  public Root::Visitor {
  public:    
    typedef Root Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( Root &inRoot ) throw(Error)
    {
        try
        {
            RootSharedPtr rootPtr;
            mFactory->create( rootPtr );

            rootPtr->setComments( inRoot.getComments() );
            rootPtr->setName( inRoot.getName() );
            rootPtr->setLevel( inRoot.getLevel() );
            rootPtr->setVersion( inRoot.getVersion() );

            std::vector< LibrarySharedPtr > libraries;
            inRoot.getLibraries( libraries );
            std::vector< LibrarySharedPtr >::iterator libIt
                                                    = libraries.begin();
            for(; libIt != libraries.end(); ++libIt )
            {
                LibrarySharedPtr newLib = clone( *libIt, mFactory );
                rootPtr->addLibrary( newLib );
            }
            mReturnVal = rootPtr;
        }
        catch( Error &e )
        {
            e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

// For Library
template<>
class Copier<class Library>
    :  public Library::Visitor {
  public:    
    typedef Library Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( Library &inLibrary ) throw(Error)
    try
    {
        LibrarySharedPtr libPtr;
        mFactory->create( libPtr );

        libPtr->setComments ( inLibrary.getComments() );
        libPtr->setIsExtern ( inLibrary.getIsExtern() );
        libPtr->setName( inLibrary.getName() );
        libPtr->setParent ( inLibrary.getParent() );
        std::map<Unit,ScaleFactor> outScaleFactors;
        outScaleFactors = inLibrary.getScaleFactors();
        std::map<Unit,ScaleFactor>::iterator it = outScaleFactors.begin();
        for(; it != outScaleFactors.end(); ++it )
        {
            libPtr->setScaleFactor( (*it).first, (*it).second );
        }
        libPtr->setLevel( inLibrary.getLevel() );
        libPtr->setOriginalName( inLibrary.getOriginalName() );
        std::vector< CellSharedPtr > outCells;
        inLibrary.getCells( outCells );
        std::vector< CellSharedPtr >::iterator cellIt = outCells.begin();
        for(; cellIt != outCells.end(); ++cellIt )
        {
            CellSharedPtr newCell = clone( *cellIt, mFactory );
            libPtr->addCell( newCell );
        }  
        mReturnVal = libPtr;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

#endif

// For Cell
template<>
class Copier<class Cell>
    :  public Cell::Visitor {
  public:    
    typedef Cell Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( Cell &inCell ) throw(Error)
    {
        try
        {
            CellSharedPtr cellPtr;
            mFactory->create( cellPtr );
            cellPtr->setComments ( inCell.getComments() );

            std::list< std::string > userData;
            inCell.getUserData( userData );
            cellPtr->setUserData ( userData );

            std::vector< StatusSharedPtr > outStatus;
            inCell.getStatuses( outStatus );
            cellPtr->setStatuses ( outStatus );

            cellPtr->setIsExtern ( inCell.getIsExtern() );
            cellPtr->setName( inCell.getName() );
            cellPtr->setParent ( inCell.getParent() );

            std::map< std::string, PropertySharedPtr > outProperties;
            inCell.getProperties( outProperties );
            cellPtr->setProperties( outProperties );

            cellPtr->setType( inCell.getType() );
            cellPtr->setOriginalName( inCell.getOriginalName() );
            std::vector< ViewSharedPtr > outViews;
            inCell.getViews( outViews );
            std::vector< ViewSharedPtr >::iterator viewIt
                                                = outViews.begin();
            for(; viewIt != outViews.end(); ++viewIt )
            {
                ViewSharedPtr newView = clone( *viewIt, mFactory );
                cellPtr->addView( newView );
            }
            mReturnVal = cellPtr;
        }
        catch( Error &e )
        {
            e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

// For View
template<>
class Copier<class View>
    :  public View::Visitor {
  public:    
    typedef View Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( View &inView ) throw(Error)
    {
        try
        {
            ViewSharedPtr viewPtr;
            mFactory->create( viewPtr );
            viewPtr->setComments ( inView.getComments() );
        
            std::list< std::string > userData;
            inView.getUserData( userData );
            viewPtr->setUserData ( userData );

            std::vector< StatusSharedPtr > outStatus;
            inView.getStatuses( outStatus );
            viewPtr->setStatuses ( outStatus );

            viewPtr->setIsExtern ( inView.getIsExtern() );
            viewPtr->setName( inView.getName() );
            viewPtr->setParent ( inView.getParent() );
            viewPtr->setType( inView.getType() );

            std::map< std::string, PropertySharedPtr > outProperties;
            inView.getProperties( outProperties );
            viewPtr->setProperties( outProperties );
            viewPtr->setOriginalName( inView.getOriginalName() );

            ParameterMapSharedPtr oldParams = inView.getParameters();
            ParameterMapSharedPtr newParams = viewPtr->getParameters();
            ParameterContext oldCtx = inView.getParameterContext();
            ParameterContext newCtx = viewPtr->getParameterContext();
            std::map< std::string,ParameterSharedPtr > params;
            oldParams->getAllParameters( oldCtx, params );
            for( std::map< std::string,ParameterSharedPtr >::iterator it
                    = params.begin(); it != params.end(); ++it )
            {
                ParameterSharedPtr clonedParam
                                    = clone( (*it).second, mFactory );
                newParams->set( newCtx, (*it).first, clonedParam );
            }

            std::vector< InstanceSharedPtr > outInstances;
            inView.getInstances( outInstances );
            std::vector< InstanceSharedPtr >::iterator instIt
                                                = outInstances.begin();
            for(; instIt != outInstances.end(); ++instIt )
            {
                InstanceSharedPtr newInst = clone( *instIt, mFactory );
                viewPtr->addInstance( newInst );
            }

            std::vector< PortSharedPtr > outPorts;
            inView.getPorts( outPorts );
            std::vector< PortSharedPtr >::iterator portIt
                                                    = outPorts.begin();
            for(; portIt != outPorts.end(); ++portIt )
            {
                PortSharedPtr newPort = clone( *portIt, mFactory );
                viewPtr->addPort( newPort );
            }
            
            std::vector< PermutableSharedPtr > outPermutables;
            inView.getPermutables( outPermutables );
            viewPtr->setPermutables( outPermutables );

            std::vector< InterfaceJoinedInfoSharedPtr > outJoinedInfos;
            inView.getInterfaceJoinedInfos( outJoinedInfos );
            viewPtr->setInterfaceJoinedInfos( outJoinedInfos );

            std::vector< NetSharedPtr > outNets;
            inView.getNets( outNets );
            std::vector< NetSharedPtr >::iterator netIt
                                            = outNets.begin();
            for(; netIt != outNets.end(); ++netIt )
            {
                NetSharedPtr netPtr = clone( *netIt, mFactory );
                viewPtr->addNet( netPtr );
                connectNets( *netIt, netPtr, viewPtr );
            }

            mReturnVal = viewPtr;
        }
        catch( Error &e )
        {
            e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    void
    connectNets( const NetSharedPtr &inNet,
            const NetSharedPtr &outNet,
            const ViewSharedPtr &inView    )
    {
        if( eCompositionTypeBundle
                        == inNet->getCompositionType() )
        {
            std::vector< NetSharedPtr > inChildren;
            std::vector< NetSharedPtr > outChildren;
            inNet->getChildren( inChildren );
            outNet->getChildren( outChildren );
            std::vector< NetSharedPtr >::iterator inIt
                                            = inChildren.begin();
            std::vector< NetSharedPtr >::iterator outIt
                                            = outChildren.begin();
            for( ; inIt != inChildren.end(); ++inIt, ++outIt )
            {
                connectNets( *inIt, *outIt, inView );
            }
        }
        cloneConnection( inNet, outNet, inView );
    }

    void
    cloneConnection( const NetSharedPtr &inNet,
            const NetSharedPtr &outNet,
            const ViewSharedPtr &inView    ) 
    {
        try
        {
            std::vector< PortSharedPtr > connectedPorts;
            inNet->getConnectedPorts( connectedPorts, true );
            for( std::vector< PortSharedPtr >::iterator it
                    = connectedPorts.begin(); it != connectedPorts.end();
                    ++it )
            {
                std::vector<std::string> nestedNames;
                PortSharedPtr actualPort = *it;
                PortSharedPtr port = actualPort;
                while( port )
                {
                    if( eCompositionTypeVectorBit
                                    != port->getCompositionType())
                        nestedNames.push_back( port->getName() );
                    port = port->getParentCollection();
                }
                std::string portName = *nestedNames.rbegin();
                PortSharedPtr targetPort = inView->findPort( portName );
                if( !targetPort )
                {
                    //TBD::ERROR
                }
                if( nestedNames.size() > 1 )
                {
                    findLeafConnectable( nestedNames, targetPort );
                }
                std::vector<size_t> indices;
                if( eCompositionTypeVectorBit
                        == actualPort->getCompositionType() )
                {
                    indices = IndexFinder<Port, VectorPortBit>()(
                                                            actualPort);
                }
                connectNetToElement( indices, targetPort, outNet );
            }
            std::vector< PortReferenceSharedPtr > connectedPortRefs;
            inNet->getConnectedPortRefs(
                                    connectedPortRefs, true );
            for( std::vector< PortReferenceSharedPtr >::iterator it
                    = connectedPortRefs.begin();
                    it != connectedPortRefs.end(); ++it )
            {
                std::vector<std::string> nestedNames;
                PortReferenceSharedPtr actualPort = *it;
                PortReferenceSharedPtr port = *it;
                InstanceSharedPtr instance;
                while( port )
                {
                    if( eCompositionTypeVectorBit
                                    != port->getCompositionType())
                    {
                        nestedNames.push_back( port->getName() );
                        instance = port->getParent();
                    }
                    port = port->getParentCollection();
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
                InstanceSharedPtr targetInst = inView->findInstance(
                                                instance->getName() );
                if( !indices.empty() )
                {
                    targetInst = targetInst->get( indices );
                }
                std::string portName = *nestedNames.rbegin();
                PortReferenceSharedPtr targetPort
                            = targetInst->findPortReference( portName );
                if( !targetPort )
                {
                    //TBD::ERROR
                }
                if( nestedNames.size() > 1 )
                {
                    findLeafConnectable( nestedNames, targetPort );
                }
                std::vector<size_t> portIndices;
                if( eCompositionTypeVectorBit
                        == actualPort->getCompositionType() )
                {
                    portIndices = IndexFinder<PortReference,
                        VectorPortBitReference>()( actualPort );
                }
                connectNetToElement( portIndices, targetPort, outNet );
            }
            std::vector< PortListSharedPtr > connectedPortLists;
            inNet->getConnectedPortLists( connectedPortLists );
            for( std::vector< PortListSharedPtr >::iterator it
                    = connectedPortLists.begin();
                    it != connectedPortLists.end(); ++it )
            {
                PortListSharedPtr cloned
                                    = clone( *it, mFactory );
                std::list< PortList::PortListElement > elements;
                (*it)->getChildren( elements );
                for( std::list< PortList::PortListElement >::iterator element
                            = elements.begin(); element != elements.end();
                            ++element )
                {
                    switch( (*element).getType() )
                    {
                        case PortList::PortListElement::eElementTypePort:
                        {
                            std::vector<std::string> nestedNames;
                            PortSharedPtr actualPort = (*element).getPort();
                            PortSharedPtr port = actualPort;
                            while( port )
                            {
                                if( eCompositionTypeVectorBit
                                                != port->getCompositionType())
                                    nestedNames.push_back( port->getName() );
                                port = port->getParentCollection();
                            }
                            std::vector<std::string>::reverse_iterator name
                                                    = nestedNames.rbegin();
                            std::string portName = *name;
                            PortSharedPtr targetPort = inView->findPort( portName );
                            if( nestedNames.size() > 1 )
                            {
                                findLeafConnectable( nestedNames, targetPort );
                            }
                            if( eCompositionTypeVectorBit
                                    == actualPort->getCompositionType() )
                            {
                                targetPort = targetPort->get(
                                    IndexFinder<Port, VectorPortBit>()(
                                                                        actualPort ) );
                            }
                            cloned->addChildPort( targetPort );
                            break;
                        }
                        case PortList::PortListElement::eElementTypePortReference:
                        {
                            std::vector<std::string> nestedNames;
                            PortReferenceSharedPtr actualPort = (*element).getPortReference();
                            PortReferenceSharedPtr port = actualPort;
                            InstanceSharedPtr instance;
                            while( port )
                            {
                                if( eCompositionTypeVectorBit
                                                != port->getCompositionType())
                                {
                                    nestedNames.push_back( port->getName() );
                                    instance = port->getParent();
                                }
                                port = port->getParentCollection();
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
                            InstanceSharedPtr targetInst = inView->findInstance(
                                                            instance->getName() );
                            if( !indices.empty() )
                            {
                                targetInst = targetInst->get( indices );
                            }
                            std::string portName = *nestedNames.rbegin();
                            PortReferenceSharedPtr targetPort
                                        = targetInst->findPortReference( portName );
                            if( !targetPort )
                            {
                                //TBD::ERROR
                            }
                            if( nestedNames.size() > 1 )
                            {
                                findLeafConnectable( nestedNames, targetPort );
                            }
                            if( eCompositionTypeVectorBit
                                    == actualPort->getCompositionType() )
                            {
                                targetPort = targetPort->get(
                                    IndexFinder<PortReference, VectorPortBitReference>()(
                                                                        actualPort ) );
                            }
                            cloned->addChildPortReference( targetPort );
                            break;
                        }
                    }
                }
                cloned->connect( outNet );
            }
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

// For ScalarPort
template<typename _PointerType>
void
copyObject( ScalarPort &inPort,
        ObjectFactorySharedPtr &inFactory,
        _PointerType &outPointer ) throw(Error)
{
    try
    {
        ScalarPortSharedPtr scalarPort;
        inFactory->create( scalarPort );
        scalarPort->setComments( inPort.getComments() );
        scalarPort->setName( inPort.getName() );
        scalarPort->setDirection( inPort.getDirection() );
        scalarPort->setOriginalName( inPort.getOriginalName() );
        scalarPort->setAttributes( inPort.getAttributes() );
        scalarPort->setIsExtern( inPort.getIsExtern() ); 
        scalarPort->setParent( inPort.getParent() );
        scalarPort->setParentCollection( inPort.getParentCollection() );

        std::list< std::string > userData;
        inPort.getUserData( userData );
        scalarPort->setUserData ( userData );

        std::map< std::string, PropertySharedPtr > outProperties;
        inPort.getProperties( outProperties );
        scalarPort->setProperties( outProperties );
        outPointer = scalarPort;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<>
class Copier<class ScalarPort>
    :  public ScalarPort::Visitor {
  public:    
    typedef ScalarPort Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( ScalarPort &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

// For ScalarPortReference
template<typename _PointerType>
void
copyObject( ScalarPortReference &inPortRef,
        ObjectFactorySharedPtr &inFactory,
        _PointerType &outPointer ) throw(Error)
{
    try
    {
        ScalarPortReferenceSharedPtr portRefPtr;
        inFactory->create( portRefPtr );
        portRefPtr->setParent ( inPortRef.getParent() );  
        portRefPtr->setParentCollection(
                            inPortRef.getParentCollection() );  
        portRefPtr->setParent ( inPortRef.getParent() );
        portRefPtr->bindToMasterPort( inPortRef.getMaster() );
        outPointer = portRefPtr;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<>
class Copier<class ScalarPortReference>
    :  public ScalarPortReference::Visitor {
  public:    
    typedef ScalarPortReference Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( ScalarPortReference &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

// For VectorPort
template<typename _PointerType>
void
copyObject( VectorPort &inPort,
        ObjectFactorySharedPtr &inFactory,
       _PointerType &outPointer    ) throw(Error)
{
    try
    {
        VectorPortSharedPtr vectorPortPtr;
        inFactory->create( vectorPortPtr );
        vectorPortPtr->setComments ( inPort.getComments() );    
        vectorPortPtr->setName( inPort.getName() );    
        vectorPortPtr->setDirection( inPort.getDirection() );    
        vectorPortPtr->setOriginalName( inPort.getOriginalName() );
        vectorPortPtr->setAttributes( inPort.getAttributes() );   
        vectorPortPtr->setIsExtern ( inPort.getIsExtern() );    
        vectorPortPtr->setParent ( inPort.getParent() );
        vectorPortPtr->setParentCollection(inPort.getParentCollection());

        std::list< std::string > userData;
        inPort.getUserData( userData );
        vectorPortPtr->setUserData ( userData );

        std::map< std::string, PropertySharedPtr > outProperties;
        inPort.getProperties( outProperties );
        vectorPortPtr->setProperties( outProperties );    

        //CREATE AND POPULATE CHILDREN
        std::vector< size_t > outLimits;
        inPort.getLimits( outLimits );
        vectorPortPtr->constructChildren( inFactory, outLimits );
        VectorPort::BaseType::List children;
        inPort.getCreatedChildren( children );
        for( VectorPort::BaseType::List::iterator it = children.begin();
                        it != children.end(); ++it )
        {
            PortSharedPtr child = *it;
            PortSharedPtr clonedChild
                    = vectorPortPtr->get(
                        IndexFinder<Port, VectorPortBit>()( child ) );
            clonedChild->setComments( child->getComments() );
            clonedChild->setName( child->getName() ); 
            clonedChild->setDirection( child->getDirection() );
            clonedChild->setOriginalName( child->getOriginalName() );
            clonedChild->setAttributes( child->getAttributes() );
            clonedChild->setIsExtern ( child->getIsExtern() ); 
            clonedChild->setParent ( child->getParent() ); 

            std::map< std::string, PropertySharedPtr > outProperties;
            child->getProperties( outProperties ); 
            clonedChild->setProperties( outProperties );
        }
        //CONNECTIONS ARE NOT COPIED
        outPointer = vectorPortPtr;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }  
} 

template<>
class Copier<class VectorPort>
    :  public VectorPort::Visitor {
  public:    
    typedef VectorPort Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( VectorPort &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

// For VectorPortReference
template<typename _PointerType>
void
copyObject( VectorPortReference &inPortRef,
        ObjectFactorySharedPtr &inFactory,
       _PointerType &outPointer ) throw(Error)
{
    try
    {
        VectorPortReferenceSharedPtr vectorPortRefPtr;
        inFactory->create( vectorPortRefPtr );
        //CREATE AND POPULATE CHILDREN
        std::vector< size_t > outLimits;
        inPortRef.getLimits( outLimits );
        vectorPortRefPtr->constructChildren( inFactory, outLimits );
        vectorPortRefPtr->setParent ( inPortRef.getParent() );
        vectorPortRefPtr->setParentCollection(
                                    inPortRef.getParentCollection() );
        vectorPortRefPtr->bindToMasterPort( inPortRef.getMaster() );
        VectorPortReference::BaseType::List children;
        inPortRef.getCreatedChildren( children );
        for( VectorPortReference::BaseType::List::iterator it = children.begin();
                        it != children.end(); ++it )
        {
            PortReferenceSharedPtr child = *it;
            PortReferenceSharedPtr clonedChild
                    = vectorPortRefPtr->get(
                        IndexFinder<PortReference,
                            VectorPortBitReference>()( child ) );
            clonedChild->setParent ( child->getParent() );   
    //        clonedChild->bindToMasterPort( child->getMaster() );
        }
        //CONNECTIONS ARE NOT COPIED
        outPointer = vectorPortRefPtr;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<>
class Copier<class VectorPortReference>
    :  public VectorPortReference::Visitor {
  public:    
    typedef VectorPortReference Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( VectorPortReference &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

// For PortBundle
template<typename _PointerType>
void
copyObject( PortBundle &inPort,
        ObjectFactorySharedPtr &inFactory,
       _PointerType &outPointer ) throw(Error)
{
    try
    {
        PortBundleSharedPtr portBundlePtr;
        inFactory->create( portBundlePtr );
        portBundlePtr->setComments ( inPort.getComments() );
        portBundlePtr->setName( inPort.getName() );
        portBundlePtr->setIsExtern( inPort.getIsExtern() );
        portBundlePtr->setParent ( inPort.getParent() );
        portBundlePtr->setParentCollection( inPort.getParentCollection() );       
        
        std::list< std::string > userData;
        inPort.getUserData( userData );
        portBundlePtr->setUserData ( userData );

        std::map< std::string, PropertySharedPtr > outProperties;
        inPort.getProperties( outProperties );
        portBundlePtr->setProperties( outProperties );        

        portBundlePtr->setDirection( inPort.getDirection() );
        portBundlePtr->setAttributes( inPort.getAttributes() );
        portBundlePtr->setOriginalName( inPort.getOriginalName() );
        std::vector<PortSharedPtr> children;
        inPort.getChildren( children );
        for( std::vector<PortSharedPtr>::iterator it = children.begin();
                    it != children.end(); ++it )
        {
            PortSharedPtr clonedPort = clone( *it, inFactory );
            portBundlePtr->addChild( clonedPort );
        }
        //CONNECTIONS ARE NOT COPIED
        outPointer = portBundlePtr;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<>
class Copier<class PortBundle>
    :  public PortBundle::Visitor {
  public:    
    typedef PortBundle Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( PortBundle &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

// For PortBundleReference
template<typename _PointerType>
void
copyObject( PortBundleReference &inPortRef,
        ObjectFactorySharedPtr &inFactory,
       _PointerType &outPointer ) throw(Error)
{
    try
    {
        PortBundleReferenceSharedPtr portBundleRefPtr;
        inFactory->create( portBundleRefPtr );
        portBundleRefPtr->setParent ( inPortRef.getParent() );
        portBundleRefPtr->setParentCollection(
                                inPortRef.getParentCollection() );
        std::vector<PortReferenceSharedPtr> children;
        inPortRef.getChildren( children );
        for( std::vector<PortReferenceSharedPtr>::iterator it = children.begin();
                    it != children.end(); ++it )
        {
            PortReferenceSharedPtr clonedPortRef = clone( *it, inFactory );
            portBundleRefPtr->addChild( clonedPortRef );
        }
        portBundleRefPtr->bindToMasterPort( inPortRef.getMaster() );

        //CONNECTIONS ARE NOT COPIED
        outPointer = portBundleRefPtr;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<>
class Copier<class PortBundleReference>
    :  public PortBundleReference::Visitor {
  public:    
    typedef PortBundleReference Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( PortBundleReference &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

// For SingleInstance
template<typename _PointerType>
void
copyObject( SingleInstance &inInstance,
        ObjectFactorySharedPtr &inFactory,
       _PointerType &outPointer ) throw(Error)
{
    try
    {
        SingleInstanceSharedPtr instPtr;
        inFactory->create( instPtr );
        instPtr->setComments( inInstance.getComments() );
        instPtr->setName( inInstance.getName() );
        instPtr->setOriginalName( inInstance.getOriginalName() );
        instPtr->setParent ( inInstance.getParent() );
        
        std::vector< PortReferenceSharedPtr > outPortRefs;
        inInstance.getPortReferences( outPortRefs );  
        for( std::vector< PortReferenceSharedPtr >::iterator it
                = outPortRefs.begin(); it != outPortRefs.end(); ++it )
        {
            PortReferenceSharedPtr clonedRef = clone( *it, inFactory );
            instPtr->addPortReference( clonedRef );
        }
        instPtr->bindToMasterView( inInstance.getMaster() );
        //THIS SHOULD BIND ALL THE CLONED PORTREFS TO THE MASTERS
        copyParams( inInstance, instPtr, inFactory );

        std::list< std::string > userData;
        inInstance.getUserData( userData );
        instPtr->setUserData ( userData );

        std::map< std::string, PropertySharedPtr > outProperties;
        inInstance.getProperties( outProperties );
        instPtr->setProperties( outProperties );

        outPointer = instPtr;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<>
class Copier<class SingleInstance>
    :  public SingleInstance::Visitor {
  public:    
    typedef SingleInstance Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( SingleInstance &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

// For ScalarNet
template<typename _PointerType>
void
copyObject( ScalarNet &inNet,
        ObjectFactorySharedPtr &inFactory,
       _PointerType &outPointer ) throw(Error)
{
    try
    {
        ScalarNetSharedPtr netPtr;
        inFactory->create( netPtr );
        netPtr->setComments ( inNet.getComments() );
        netPtr->setName( inNet.getName() );
        netPtr->setParent ( inNet.getParent() );
        netPtr->setOriginalName( inNet.getOriginalName() );

        std::list< std::string > userData;
        inNet.getUserData( userData );
        netPtr->setUserData ( userData );

        std::map< std::string, PropertySharedPtr > outProperties;
        inNet.getProperties( outProperties );
        netPtr->setProperties( outProperties );

        std::vector< NetSharedPtr > outNets;
        inNet.getSubnets( outNets );
        std::vector< NetSharedPtr >::iterator netIt = outNets.begin();
        for(; netIt != outNets.end(); ++netIt )
        {
            NetSharedPtr childNet = clone( *netIt, inFactory );
            netPtr->addSubnet( childNet );
        }
        //DO NOT CONNECT
        outPointer = netPtr;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<>
class Copier<class ScalarNet>
    :  public ScalarNet::Visitor {
  public:    
    typedef ScalarNet Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( ScalarNet &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};


// For NetBundle
template<typename _PointerType>
void
copyObject( NetBundle &inNet,
        ObjectFactorySharedPtr &inFactory,
       _PointerType &outPointer ) throw(Error)
{
    try
    {
        NetBundleSharedPtr netBundlePtr;
        inFactory->create( netBundlePtr );
        netBundlePtr->setComments ( inNet.getComments() );
        netBundlePtr->setName( inNet.getName() );
        netBundlePtr->setParent ( inNet.getParent() );
        netBundlePtr->setOriginalName( inNet.getOriginalName() );

        std::list< std::string > userData;
        inNet.getUserData( userData );
        netBundlePtr->setUserData ( userData );

        std::map< std::string, PropertySharedPtr > outProperties;
        inNet.getProperties( outProperties );
        netBundlePtr->setProperties( outProperties );

        std::vector<NetSharedPtr> outChildren;
        inNet.getChildren( outChildren );
        std::vector<NetSharedPtr>::iterator it = outChildren.begin();
        for(; it != outChildren.end(); ++it )
        {
            NetSharedPtr clonedChild = clone( *it, inFactory );
            netBundlePtr->addChild( clonedChild );
        }
        std::vector< NetSharedPtr > outNets;
        inNet.getSubnets( outNets );
        std::vector< NetSharedPtr >::iterator netIt = outNets.begin();
        for(; netIt != outNets.end(); ++netIt )
        {
            NetSharedPtr childNet = clone( *netIt, inFactory );
            netBundlePtr->addSubnet( childNet );
        }
        outPointer = netBundlePtr;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<>
class Copier<class NetBundle>
    :  public NetBundle::Visitor {
  public:    
    typedef NetBundle Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( NetBundle &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

// For VectorNet
template<typename _PointerType>
void
copyObject( VectorNet &inNet,
        ObjectFactorySharedPtr &inFactory,
       _PointerType &outPointer ) throw(Error)
{
    try
    {
        VectorNetSharedPtr vectorNetPtr;
        inFactory->create( vectorNetPtr );
        vectorNetPtr->setComments ( inNet.getComments() );
        vectorNetPtr->setName( inNet.getName() );
        vectorNetPtr->setParent ( inNet.getParent() );
        vectorNetPtr->setOriginalName( inNet.getOriginalName() );

        std::list< std::string > userData;
        inNet.getUserData( userData );
        vectorNetPtr->setUserData ( userData );

        std::map< std::string, PropertySharedPtr > outProperties;
        inNet.getProperties( outProperties );
        vectorNetPtr->setProperties( outProperties );
        //CREATE AND POPULATE CHILDREN
        std::vector< size_t > outLimits;
        inNet.getLimits( outLimits );
        vectorNetPtr->constructChildren( inFactory, outLimits );
        VectorNet::BaseType::List children;
        inNet.getCreatedChildren( children );
        for( VectorNet::BaseType::List::iterator it = children.begin();
                        it != children.end(); ++it )
        {
            NetSharedPtr child = *it;
            NetSharedPtr clonedChild
                    = vectorNetPtr->get(
                        IndexFinder<Net, VectorNetBit>()( child ) );
            clonedChild->setComments ( child->getComments() );
            clonedChild->setName( child->getName() );
            clonedChild->setParent ( child->getParent() );
            clonedChild->setOriginalName( child->getOriginalName() );

            std::map< std::string, PropertySharedPtr > outProperties;
            child->getProperties( outProperties );
            clonedChild->setProperties( outProperties );
            std::vector< NetSharedPtr > outNets;
            child->getSubnets( outNets );
            std::vector< NetSharedPtr >::iterator netIt
                                                    = outNets.begin();
            for(; netIt != outNets.end(); ++netIt )
            {
                NetSharedPtr clonedChildNet = clone( *netIt, inFactory );
                clonedChild->addSubnet( clonedChildNet );
            }
        }
        //CONNECTIONS ARE NOT COPIED
        std::vector< NetSharedPtr > outNets;
        inNet.getSubnets( outNets );
        std::vector< NetSharedPtr >::iterator netIt = outNets.begin();
        for(; netIt != outNets.end(); ++netIt )
        {
            NetSharedPtr childNet = clone( *netIt, inFactory );
            vectorNetPtr->addSubnet( childNet );
        }

        outPointer = vectorNetPtr;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<>
class Copier<class VectorNet>
    :  public VectorNet::Visitor {
  public:    
    typedef VectorNet Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( VectorNet &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

// For InstanceArray
template<typename _PointerType>
void
copyObject( InstanceArray &inInstanceArray,
        ObjectFactorySharedPtr &inFactory,
       _PointerType &outPointer ) throw(Error)
{
    try
    {
        InstanceArraySharedPtr instArrayPtr;
        inFactory->create( instArrayPtr );
        instArrayPtr->setComments( inInstanceArray.getComments() );
        instArrayPtr->setName( inInstanceArray.getName() );
        instArrayPtr->setParent ( inInstanceArray.getParent() );
        instArrayPtr->setOriginalName(
                                inInstanceArray.getOriginalName() );

        std::list< std::string > userData;
        inInstanceArray.getUserData( userData );
        instArrayPtr->setUserData ( userData );

        std::map< std::string, PropertySharedPtr > outProperties;
        inInstanceArray.getProperties( outProperties );
        instArrayPtr->setProperties( outProperties );

        //WE intentionally bind here
        //This will help when children are bound to different masters
        //The actual child binding will be done later
        instArrayPtr->bindToMasterView( inInstanceArray.getMaster() );
        copyParams( inInstanceArray, instArrayPtr, inFactory );

        //CREATE AND POPULATE CHILDREN
        std::vector< size_t > outLimits;
        inInstanceArray.getLimits( outLimits );
        instArrayPtr->constructChildren( inFactory, outLimits );
        InstanceArray::BaseVectorType::List children;
        inInstanceArray.getChildren( children );
        for( InstanceArray::BaseVectorType::List::iterator it
                        = children.begin(); it != children.end(); ++it )
        {
            InstanceSharedPtr orig = *it;
            InstanceSharedPtr cloned
                    = instArrayPtr->get(
                        IndexFinder<Instance,
                            InstanceArrayMember>()( orig ) );
            cloned->setComments( orig->getComments() );
            cloned->setName( orig->getName() );
            cloned->setOriginalName( orig->getOriginalName() );
            cloned->setParent ( orig->getParent() );

            std::list< std::string > userData;
            orig->getUserData( userData );
            cloned->setUserData ( userData );

            std::map< std::string, PropertySharedPtr > outProperties;
            orig->getProperties( outProperties );
            cloned->setProperties( outProperties );

            std::vector< PortReferenceSharedPtr > outPortRefs;
            orig->getPortReferences( outPortRefs );  
            for( std::vector< PortReferenceSharedPtr >::iterator pit
                = outPortRefs.begin(); pit != outPortRefs.end(); ++pit )
            {
                PortReferenceSharedPtr clonedRef = clone( *pit, inFactory );
                cloned->addPortReference( clonedRef );
            }
            cloned->bindToMasterView( orig->getMaster() );
            copyParams( *orig, cloned, inFactory );
        }
        outPointer = instArrayPtr;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<>
class Copier<class InstanceArray>
    :  public InstanceArray::Visitor {
  public:    
    typedef InstanceArray Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( InstanceArray &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

// For SingleParameter
template<typename _PointerType>
void
copyObject(SingleParameter &inSingleParameter,
        ObjectFactorySharedPtr &inFactory,
       _PointerType &outPointer ) throw(Error)
{
    try
    {
        SingleParameterSharedPtr singleParameterPtr;
        inFactory->create( singleParameterPtr );
        singleParameterPtr->setName( inSingleParameter.getName() );
        singleParameterPtr->setUnit( inSingleParameter.getUnit() );
        singleParameterPtr->setValue( inSingleParameter.getValue() );
        singleParameterPtr->setOriginalName( inSingleParameter.getOriginalName() );
        outPointer = singleParameterPtr;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<>
class Copier<class SingleParameter>
    :  public SingleParameter::Visitor {
  public:    
    typedef SingleParameter Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( SingleParameter &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};


// For ParameterArray
template<typename _PointerType>
void
copyObject(ParameterArray &inParamArray,
        ObjectFactorySharedPtr &inFactory,
       _PointerType &outPointer ) throw(Error)
{
    try
    {
        ParameterArraySharedPtr paramArrayPtr;
        inFactory->create( paramArrayPtr );
        paramArrayPtr->setName( inParamArray.getName() );
        paramArrayPtr->setOriginalName( inParamArray.getOriginalName() );

        std::vector< size_t > outLimits;
        inParamArray.getLimits( outLimits );
        paramArrayPtr->constructChildren( inFactory, outLimits );
        ParameterArray::BaseVectorType::List children;
        inParamArray.getChildren( children );
        ParameterArray::BaseVectorType::List::iterator it
                                                    = children.begin();
        for(; it != children.end(); ++it )
        {
            ParameterSharedPtr orig = *it;
            ParameterSharedPtr cloned
                = paramArrayPtr->get(
                    IndexFinder<Parameter, ParameterArrayElement>()( orig ) );
            cloned->setName( orig->getName() );
            cloned->setUnit( orig->getUnit() );
            cloned->setValue( orig->getValue() );
        }
        outPointer = paramArrayPtr;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<>
class Copier<class ParameterArray>
    :  public ParameterArray::Visitor {
  public:    
    typedef ParameterArray Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( ParameterArray &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

// For Property
template<>
class Copier<class Property>
    :  public Property::Visitor {
  public:    
    typedef Property Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( Property &inProperty ) throw(Error)
    {
        try
        {
            PropertySharedPtr propertyPtr;
            mFactory->create( propertyPtr );
            propertyPtr->setComments ( inProperty.getComments() );
            propertyPtr->setName( inProperty.getName() );
            propertyPtr->setOriginalName( inProperty.getOriginalName() );
            propertyPtr->setOwner( inProperty.getOwner() );
            propertyPtr->setUnit( inProperty.getUnit() );
            propertyPtr->setValue( inProperty.getValue() );

            std::map< std::string, PropertySharedPtr > outValues;
            inProperty.getChildren( outValues );
            for( std::map< std::string, PropertySharedPtr >::iterator it
                     = outValues.begin(); it != outValues.end(); ++it )
            {
                PropertySharedPtr childProperty
                                        = clone( (*it).second, mFactory );
                propertyPtr->addChildProperty( (*it).first, childProperty );
            }
            mReturnVal = propertyPtr;
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

template<>
class Copier<class Port>
    :  public ScalarPort::Visitor,
    public VectorPort::Visitor,
    public PortBundle::Visitor {
  public:    
    typedef Port Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( ScalarPort &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    void
    visit( VectorPort &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    void
    visit( PortBundle &inPort ) throw(Error)
    {
        try
        {
            copyObject( inPort, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : ScalarPort::Visitor(),
        VectorPort::Visitor(),
        PortBundle::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

template<>
class Copier<class PortReference>
    :  public ScalarPortReference::Visitor,
    public VectorPortReference::Visitor,
    public PortBundleReference::Visitor {
  public:    
    typedef PortReference Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( ScalarPortReference &inPortReference ) throw(Error)
    {
        try
        {
            copyObject( inPortReference, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    void
    visit( VectorPortReference &inPortReference ) throw(Error)
    {
        try
        {
            copyObject( inPortReference, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    void
    visit( PortBundleReference &inPortReference ) throw(Error)
    {
        try
        {
            copyObject( inPortReference, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : ScalarPortReference::Visitor(),
        VectorPortReference::Visitor(),
        PortBundleReference::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

template<>
class Copier<class Net>
    :  public ScalarNet::Visitor,
    public VectorNet::Visitor,
    public NetBundle::Visitor {
  public:    
    typedef Net Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( ScalarNet &inNet ) throw(Error)
    {
        try
        {
            copyObject( inNet, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    void
    visit( VectorNet &inNet ) throw(Error)
    {
        try
        {
            copyObject( inNet, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    void
    visit( NetBundle &inNet ) throw(Error)
    {
        try
        {
            copyObject( inNet, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : ScalarNet::Visitor(),
        VectorNet::Visitor(),
        NetBundle::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

template<>
class Copier<class Instance>
    :  public SingleInstance::Visitor,
    public InstanceArray::Visitor {
  public:    
    typedef Instance Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( SingleInstance &inInstance ) throw(Error)
    {
        try
        {
            copyObject( inInstance, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    void
    visit( InstanceArray &inInstance ) throw(Error)
    {
        try
        {
            copyObject( inInstance, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : SingleInstance::Visitor(),
        InstanceArray::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

template<>
class Copier<class Parameter>
    :  public SingleParameter::Visitor,
    public ParameterArray::Visitor {
  public:    
    typedef Parameter Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( SingleParameter &inParameter ) throw(Error)
    {
        try
        {
            copyObject( inParameter, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    void
    visit( ParameterArray &inParameter ) throw(Error)
    {
        try
        {
            copyObject( inParameter, mFactory, mReturnVal );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : SingleParameter::Visitor(),
        ParameterArray::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

template<>
class Copier<class PortList>
    :  public PortList::Visitor {
  public:    
    typedef PortList Type;
    typedef boost::shared_ptr<Type> Pointer;

    Pointer
    operator()( const Pointer &inSource,
            const ObjectFactorySharedPtr &inFactory ) throw(Error) {
        mFactory = inFactory;
        inSource->accept( *this );
        return mReturnVal;
    }

    void
    visit( PortList &inPort ) throw(Error)
    {
        try
        {
            PortListSharedPtr portList;
            mFactory->create( portList );
#if 0
            std::list< PortList::PortListElement > elements;
            portList->getChildren( elements );
            for( std::list< PortList::PortListElement >::iterator it
                        = elements.begin(); it != elements.end();
                        ++it )
            {
                switch( (*it).getType() )
                {
                    case PortList::PortListElement::eElementTypePort:
                    {
                        portList->addChildPort( (*it).getPort() );
                        break;
                    }
                    case PortList::PortListElement::eElementTypePortReference:
                    {
                        portList->addChildPortReference(
                                    (*it).getPortReference() );
                        break;
                    }
                }
            }
#endif
            mReturnVal = portList;
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    Copier()
        : Type::Visitor(),
        mFactory(),
        mReturnVal() {
    }

    ~Copier() throw() {
    }

  private:
    ObjectFactorySharedPtr mFactory;
    Pointer mReturnVal;
};

} //namespace _impl

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_CLONING_HPP
