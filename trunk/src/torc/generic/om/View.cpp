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
#include <boost/serialization/is_abstract.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/shared_ptr.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Instance.hpp"
#include "torc/generic/util/Log.hpp"
#include "torc/generic/om/Net.hpp"
#include "torc/generic/om/Parameter.hpp"
#include "torc/generic/om/ParameterMap.hpp"
#include "torc/generic/om/Port.hpp"
#include "torc/generic/om/PortReference.hpp"
#include "torc/generic/om/View.hpp"
#include "torc/generic/om/Cell.hpp"

namespace torc {

namespace generic {

#ifdef GENOM_SERIALIZATION
class RestoredInstanceUpdater {
  public:
    void
    operator()( const InstanceSharedPtr &inInst
                                        ) const throw(Error) {
        try
        {
            inInst->setParent( mView );
            inInst->restoreMaster();
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    RestoredInstanceUpdater( const ViewSharedPtr &inView )
        : mView( inView ) {
    }    
  private:
    ViewSharedPtr mView;
};
#endif //GENOM_SERIALIZATION

/**
 * Create a view
 *
 * @param[in] inName Name of the view to be created.
 * @param[in] inCellPtr Pointer to parented(Cell) object.
 * @param[in] inViewType View Type.
 * @param[in] inOriginalName Original name of the view [optional].
 *
 * @return Pointer to created view.
 **/
ViewSharedPtr
View::Factory::newViewPtr( const std::string &inName,
    const CellSharedPtr &inCellPtr,
    const View::Type &inViewType,
    const std::string &inOriginalName ) throw(Error) {
    try
    {
        ViewSharedPtr newView;
        create( newView ); 
        newView->setName(inName);
        newView->setParent( inCellPtr );
        newView->setType( inViewType );
        newView->setOriginalName( inOriginalName );
        inCellPtr->addView( newView );
        return newView;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}
    
void
View::accept(BaseVisitor & inoutVisitor) throw(Error) {
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

ParameterMapSharedPtr
View::getParameters() throw(Error) {
    if( !mParameters->isContextRegistered( mMyContext ) )
    {
        mParameters->registerContext( mMyContext );
    }
    return mParameters;
}

/**
 * Set the type of view
 *
 * @param[in] inSource Type of view
 */
void
View::setType(const View::Type & inSource) throw() {
  mType = inSource;
}

/**
 * Set the list of instances to this view.
 *
 * @param[in] inSource List of instances.
 */
void
View::setInstances(
    const std::vector< InstanceSharedPtr > & inSource) throw(Error) {
    std::vector< InstanceSharedPtr >::const_iterator instance = inSource.begin();
    std::vector< InstanceSharedPtr >::const_iterator end = inSource.end();
    for(; instance != end; ++instance )
    {
        try
        {
            addInstance( *instance );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
}

/**
 * Add an instance to the list of instances. Empty pointer is ignored.
 *
 * @param[in] inInstance Pointer to instance to be added.
 *
 * @exception Error Could not add instance because pointer to the Instance does not exist
 * @exception Error Could not add instance because Instance name is empty
 * @exception Error Could not add instance because Instance name already exists 
 */
void
View::addInstance(
    const InstanceSharedPtr & inInstance) throw(Error) {
    if( !inInstance )
    {
        Error e( eMessageIdErrorPointerToItemDoesNotExist,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Pointer to the instance object does not exist", inInstance);
        throw e;
    }
    std::string name = inInstance->getName();
    if( name.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Empty Instance name", name);
        throw e;
    }
    if( false == mInstanceSymTab.set( name, inInstance ) )
    {
        Error e( eMessageIdErrorItemAlreadyExists,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Instance name", name);
        throw e;
    }
    inInstance->setParent( getSharedThis() );
}

/**
 * Find an instance by name, in the list of instances.
 *
 * @param[in] inName String inSource specifying the name of the instance.
 *
 * @return A pointer to the instance if found, an empty pointer otherwise.
 *
 * @exception Error Empty Instance name 
 */
InstanceSharedPtr
View::findInstance(const std::string &inName) throw() {
    if( inName.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Instance name", inName);
        throw e;
    }
    InstanceSharedPtr instance;
    mInstanceSymTab.get( inName, instance );
    return instance;
}

/**
 * Remove the specified instance from the list of cells. If an empty pointer is passed, it returns without doing anything
 *
 * @param inInstance Pointer to an instance object.
 *
 * @exception Error Empty Instance name
 * @exception Error Instance name not preset in collection.
 */
void
View::removeInstance(const std::string &inName) throw(Error) {
    if( inName.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Instance name", inName);
        throw e;
    }
    InstanceSharedPtr instance = findInstance( inName );
    if( instance )
    {
        std::vector<PortReferenceSharedPtr> refs;
        instance->getPortReferences( refs );
        for_each( refs.begin(), refs.end(),
                    boost::bind( boost::mem_fn(
                        &Connectable::disconnect), _1));
        if( false == mInstanceSymTab.remove( inName ) )
        {
            Error e( eMessageIdErrorItemNotFound,
                __FUNCTION__, __FILE__, __LINE__ );
            e.saveContextData("Instance name", inName);
            throw e;
        }
    }
    else
    {
        Error e( eMessageIdErrorItemNotFound,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Instance name", inName);
        throw e;
    }
}

/**
 * Set the list of nets to this view.
 *
 * @param[in] inSource List of nets.
 */
void
View::setNets(
    const std::vector< NetSharedPtr > & inSource) throw(Error) {
    std::vector< NetSharedPtr >::const_iterator net
                                        = inSource.begin();
    std::vector< NetSharedPtr >::const_iterator end
                                        = inSource.end();
    for(; net != end; ++net )
    {
        try
        {
            addNet( *net );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
}

/**
 * Add a net to the list of nets. Empty pointer is ignored.
 *
 * @param[in] inNet Pointer to net to be added.
 *
 * @exception Error Could not add Net, because Net name is empty
 * @exception Error Could not add Net, because Net name already exists
 */
void
View::addNet(const NetSharedPtr & inNet) throw(Error) {
    if( !inNet )
    {
        return;
    }
    std::string name = inNet->getName();
    if( name.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Net name", name);
        throw e;
    }
    if( false == mNetSymTab.set( name, inNet ) )
    {
        Error e( eMessageIdErrorItemAlreadyExists,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Net name", name);
        throw e;
    }
    inNet->setParent( getSharedThis() );
}

/**
 * Find a net by name, in the list of net.
 *
 * @param[in] inName String inSource specifying the name of the Net
 *
 * @return A pointer to the net if found, an empty pointer otherwise.
 * @exception Error Empty Net name
 */
NetSharedPtr
View::findNet(const std::string &inName) throw() {
    if( inName.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Net name not found", inName);
        throw e;
    }
    NetSharedPtr net;
    mNetSymTab.get( inName, net );
    return net;
}

/**
 * Remove the specified net from the list of nets. If an empty pointer is passed, it returns without doing anything
 *
 * @param inNet Pointer to a net object.
 *
 * @exception Error Empty Net name
 * @exception Error Net not preset in collection.
 */
void
View::removeNet(const std::string &inName) throw(Error) {
    if( inName.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Net name", inName);
        throw e;
    }
    if( false == mNetSymTab.remove( inName ) )
    {
        Error e( eMessageIdErrorItemNotFound,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Net name", inName);
        throw e;
    }
}

/**
 * Set the list of ports to this view.
 *
 * @param[in] inSource List of ports
 */
void
View::setPorts(const std::vector< PortSharedPtr > &inSource) throw(Error) {
    std::vector< PortSharedPtr >::const_iterator port = inSource.begin();
    std::vector< PortSharedPtr >::const_iterator end = inSource.end();
    for(;port != end; ++port)
    {
        try
        {
            addPort( *port );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

}

/**
 * Add a port to the list of ports. Empty pointer is ignored.
 *
 * @param[in] inPort Pointer to port to be added.
 *
 * @exception Error Could not add port, because Port name is empty
 */
void
View::addPort(
    const PortSharedPtr &inPort) throw(Error) {
    if( !inPort )
    {
        return;
    }
    std::string name = inPort->getName();
    if( name.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Port name", name);
        throw e;
    }
    if( false == mPortSymTab.set( name, inPort ) )
    {
        Error e( eMessageIdErrorItemAlreadyExists,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Port name", name);
        throw e;
    }
    inPort->setParent( getSharedThis() );
}

/**
 * Find a port by name, in the list of ports.
 *
 * @param[in] inName String inSource specifying the name of the port.
 *
 * @return A pointer to the port if found, an empty pointer otherwise.
 *
 * @exception Error Empty Port name
 */
PortSharedPtr
View::findPort(const std::string &inName) throw() {
    if( inName.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Port name", inName);
        throw e;
    }
    PortSharedPtr port;
    mPortSymTab.get( inName, port );
    return port;
}

/**
 * Remove the specified port from the list of ports. If an empty pointer is passed, it returns without doing anything
 *
 * @param inName Name of the port to be removed
 *
 * @exception Error Empty Port name 
 * @exception Error Port not preset in collection.
 */
void
View::removePort(const std::string &inName) throw(Error)
{
    if( inName.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Port name", inName);
        throw e;
    }
    if( false == mPortSymTab.remove( inName ) )
    {
        Error e( eMessageIdErrorItemNotFound,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Port name", inName);
        throw e;
    }
}

void
View::setNonNetlistViewData( const std::string &inData) throw() {
    mNonNetlistViewData = inData;
}

/**
 * Set the vector of permutables to this view. It will lead to a linear traversal on the list. So usage of this API is not recommended.
 *
 * @param[in] inSource Vector of permutables to this view.
 * @exception Error Could not add permutable because pointer to the permutable does not exist
 */
void
View::setPermutables(
        const std::vector< PermutableSharedPtr > & inSource) throw(Error) {
    std::vector< PermutableSharedPtr >::const_iterator entry = inSource.begin();
    std::vector< PermutableSharedPtr >::const_iterator end = inSource.end();
    for(; entry != end; ++entry )
    {
        try
        {
            addPermutable( *entry );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
}

/**
 * Add a permutable to this view.
 *
 * @param[in] inPermutable Pointer to permutable to be added.
 * @exception Error Could not add permutable because pointer to the permutable does not exist
 */
bool
View::addPermutable(
        const PermutableSharedPtr & inPermutable ) throw(Error) {
    if( !inPermutable )
    {
        Error e( eMessageIdErrorPointerToItemDoesNotExist,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Pointer to the permutable object does not exist", inPermutable);
        throw e;
    }
    if( inPermutable ) {
        mPermutables.push_back( inPermutable );
        return true;
    }
    else {
        return false;
    }
}

/**
 * Set the vector of joined info for this view.
 *
 * @param[in] inSource Vector of joined info to this view.
 * @exception Error Could not add joined info because pointer to the joined info does not exist
 */
void
View::setInterfaceJoinedInfos( const std::vector< InterfaceJoinedInfoSharedPtr >
     & inSource ) throw(Error) {
    std::vector< InterfaceJoinedInfoSharedPtr >::const_iterator entry = inSource.begin();
    std::vector< InterfaceJoinedInfoSharedPtr >::const_iterator end = inSource.end();
    for(; entry != end; ++entry )
    {
        try
        {
            addInterfaceJoinedInfo( *entry );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
}

/**
 * Add a joined info to this view.
 *
 * @param[in] inJoinedInfo Pointer to joined info to be added.
 * @exception Error Could not add joined info because pointer to the joined info does not exist
 */
bool
View::addInterfaceJoinedInfo(
    const InterfaceJoinedInfoSharedPtr & inJoinedInfo ) throw(Error) {
    if( !inJoinedInfo )
    {
        Error e( eMessageIdErrorPointerToItemDoesNotExist,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Pointer to the joined info object does not exist", inJoinedInfo);
        throw e;
    }
    if( inJoinedInfo ) {
        mInterfaceJoinedInfos.push_back( inJoinedInfo );
        return true;
    }
    else {
        return false;
    }
}

/**
 * Set the attributes of a view interface. 
 * Attributes include designator, simulate, timing, comments, userdata etc.
 * This will decompile within (contents ...) construct.
 *
 * @param[in] inSource Pointer to InterfaceAttributes object.
 */
void
View::setInterfaceAttributes(
        const InterfaceAttributesSharedPtr & inSource) throw() {
    mAttributes = inSource;
}

/**
 * Set the pointer to the simulate.
 *
 * @param[in] inSource Pointer to the simulate
 */
void
View::setSimulate(const SimulateSharedPtr & inSource ) throw() {
    mSimulate = inSource;
}

/**
 * Set the pointer to the timing object
 *
 * @param[in] inSource Pointer to the timing object
 */
void
View::setTiming(const TimingSharedPtr & inSource ) throw() {
    mTiming = inSource;
}


View::View()
    : Commentable(),
    Extern(),
    Nameable(),
    ParentedObject<Cell>(),
    PropertyContainer(),
    Renamable(),
    SelfReferencing<View>(),
    Visitable(),
    UserDataContainer(),
    StatusContainer(),
    mParameters( new ParameterMap() ),
    mMyContext( mParameters->getNewContext() ),
    mInstanceSymTab(),
    mNetSymTab(),
    mPortSymTab(),
    mType(eTypeNetlist),
    mPermutables(),
    mInterfaceJoinedInfos(),
    mAttributes(),
    mSimulate(),
    mTiming() {
}

View::~View() throw() {
    if( !getName().empty() )
    {
        log("View %s is being destroyed.\n",
                getName().c_str());
    }
    mNetSymTab.applyOnAll( boost::bind( boost::mem_fn(
                                   &Net::disconnect), _1 ));
    mNetSymTab.clear();
    mPortSymTab.clear();
    mInstanceSymTab.clear();
    mPermutables.clear();
    mInterfaceJoinedInfos.clear();
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
View::load( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object<Extern>( *this );
    ar & boost::serialization::base_object<Nameable>( *this );
    ar & boost::serialization::base_object<PropertyContainer>(*this);
    ar & boost::serialization::base_object<Renamable>(*this);
    ar & boost::serialization::base_object<
                                    SelfReferencing<View> >(*this);
    ar & boost::serialization::base_object<Visitable>(*this);
    ar & mInstanceSymTab;
    ar & mNetSymTab;
    ar & mPortSymTab;
    ar & mType;
    std::map< std::string,ParameterSharedPtr > params;
    ar & params;
    mParameters->registerContext( mMyContext );
    std::map< std::string,ParameterSharedPtr >::iterator
                                                it = params.begin();
    std::map< std::string,ParameterSharedPtr >::iterator
                                                end = params.end();
    for(; it != end; ++it )
    {
        mParameters->set( mMyContext, (*it).first, (*it).second );
    }
}

template<class Archive> void
View::save( Archive &ar, unsigned int ) const {
    ar & boost::serialization::base_object<Extern>( *this );
    ar & boost::serialization::base_object<Nameable>( *this );
    ar & boost::serialization::base_object<PropertyContainer>(*this);
    ar & boost::serialization::base_object<Renamable>(*this);
    ar & boost::serialization::base_object<
                                    SelfReferencing<View> >(*this);
    ar & boost::serialization::base_object<Visitable>(*this);
    ar & mInstanceSymTab;
    ar & mNetSymTab;
    ar & mPortSymTab;
    ar & mType;
    std::map< std::string,ParameterSharedPtr > params;
    mParameters->getAllParameters( mMyContext, params );
    ar & params;
}

void
View::restoreActions() throw(Error) {
    try
    {
        mInstanceSymTab.applyOnAll(
                RestoredInstanceUpdater( getSharedThis() ) );
        mInstanceSymTab.applyOnAll( boost::bind( boost::mem_fn(
                &Net::setParent ), _1, getSharedThis()));
        mInstanceSymTab.applyOnAll( boost::bind( boost::mem_fn(
                &Port::setParent ), _1, getSharedThis()));
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

//TO SATISFY THE LINKER
template void
View::load<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
View::save<boost::archive::binary_oarchive>(
    boost::archive::binary_oarchive & ar, const unsigned int) const;

#endif //GENOM_SERIALIZATION


} // namespace torc::generic

} // namespace torc
