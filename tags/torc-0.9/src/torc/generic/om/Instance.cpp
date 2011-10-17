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

#ifdef GENOM_SERIALIZATION
#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/is_abstract.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/shared_ptr.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Instance.hpp"
#include "torc/generic/om/ParameterMap.hpp"
#include "torc/generic/om/Parameter.hpp"
#include "torc/generic/om/Port.hpp"
#include "torc/generic/om/PortReference.hpp"
#ifdef GENOM_SERIALIZATION
#include "torc/generic/om/Library.hpp"
#include "torc/generic/om/Cell.hpp"
#endif //GENOM_SERIALIZATION
#include "torc/generic/om/View.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_IS_ABSTRACT(
        torc::generic::Composite<torc::generic::Instance> )
#endif //GENOM_SERIALIZATION

#ifdef GENOM_SERIALIZATION
namespace {
}
#endif //GENOM_SERIALIZATION

namespace {

class PortMapper {
  public:
    void
    operator() (
        const torc::generic::PortReferenceSharedPtr &pRef
                        ) const throw(torc::generic::Error) {
        torc::generic::PortSharedPtr port
                            = mMaster->findPort( pRef->getName() );
        if( !port
            || port->getCompositionType()
                                != pRef->getCompositionType()
            || port->getSize() != pRef->getSize() )
        {
            //TBD::ERROR
        }
        try
        {
            pRef->bindToMasterPort( port );
        }
        catch( torc::generic::Error &e )
        {
            e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    PortMapper(
        const torc::generic::ViewSharedPtr &inMaster )
        : mMaster( inMaster ) {
        }

  private:
    torc::generic::ViewSharedPtr mMaster;
};

}

namespace torc {

namespace generic {

ParameterContext
Instance::getParameterContext() const throw() {
    return mMyContext;
}

ParameterMapSharedPtr
Instance::getParameters() const throw(Error) {
    if( !getMaster() )
    {
        return ParameterMapSharedPtr();
    }
    ParameterMapSharedPtr params = mMaster->getParameters();
    if( !params->isContextRegistered( mMyContext ) )
    {
        params->registerContext( mMyContext,
                                   mMaster->getParameterContext() );
    }
    return params;
}

/**
 * Bind an instance to it's master. Instance binding consists of two steps:
 * <ul>
 *    <li> Bind instance to cell </li>
 *    <li> Bind port references to ports of the cell </li>
 * </ul>
 *
 * @param[in] inMaster Source Master view for this instance.
 * @exception Error If there is any kind of mismatch between ports etc.
 */
void
Instance::bindToMasterView(
    const ViewSharedPtr &inMaster,
    bool inMapPortReferences ) throw(Error) {
    if( !mMaster )
    {
        //TDB::ERROR MASTER REBINDING NOT ALLOWED
    }
    ViewSharedPtr oldMaster = getMaster();
    std::map<std::string, ParameterSharedPtr> myParams;
    if( oldMaster )
    {
        ParameterMapSharedPtr oldParams
                                = oldMaster->getParameters();
        oldParams->getOverriddenParameters( mMyContext, myParams );
        oldParams->unregisterContext( mMyContext );
    }
    setMaster( inMaster );
    ParameterMapSharedPtr paramMap = mMaster->getParameters();
    mMyContext = paramMap->getNewContext();
    //We do not create params here, they will be created only when
    //someone asks for params
    //For rebinding, however, if some params were there
    //.. we have to override them on the new master
    if( !myParams.empty() )
    {
        try
        {
            paramMap->registerContext( mMyContext,
                                   mMaster->getParameterContext() );
            for( std::map<std::string, ParameterSharedPtr>::iterator
                it = myParams.begin(); it != myParams.end(); ++it )
            {
                paramMap->set(
                        mMyContext, (*it).first, (*it).second );
            }
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
    if( inMaster->getIsExtern() )
    {
        return; //No port binding for extern
    }
    if( inMapPortReferences )
    {
        PortMapper mapper( inMaster );
        try
        {
            mPortReferences.applyOnAll( mapper );
        }
        catch(Error &e)
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
}

void
Instance::setMaster(
            const ViewSharedPtr &inMaster ) throw() {
    mMaster = inMaster;
}

/**
 * Add a port reference to this master. When a master is set using the set_master(), the list of port references is not created. It has to be extrinsically set.
 *
 * @param[in] inPortRef A port reference.
 *
 * @exception Error The port ref could not be added.
 */
void
Instance::addPortReference(
    const PortReferenceSharedPtr & inPortRef) throw(Error) {
    std::string name = inPortRef->getName();
    if( name.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("PortReference name", name);
        throw e;
    }
    if( false == mPortReferences.set( name, inPortRef ) )
    {
        Error e( eMessageIdErrorItemAlreadyExists,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("PortReference name", name);
        throw e;
    }
	inPortRef->setParent( getSharedThis() );
    return;
}

/**
 * Find a port reference.
 *
 * @param[in] inName String containing the name of the port.
 */
PortReferenceSharedPtr
Instance::findPortReference(
            const std::string &inName) throw(Error) {
    PortReferenceSharedPtr portRef;
    mPortReferences.get( inName, portRef );
    return portRef;
}

/**
 * Find a Net reference.
 *
 * @param[in] inName String containing the name of the Net.
 */
NetReferenceSharedPtr
Instance::findNetReference(
            const std::string &inName) throw(Error) {
    NetReferenceSharedPtr netRef;
    mNetReferences.get( inName, netRef );
    return netRef;
}

/**
 * Remove a given port reference.
 *
 * @param inName Name of the object to be delete
 *
 * @exception Error PortReference name is empty 
 * @exception Error PortReference name could not be found
 */
void
Instance::removePortReference(
        const std::string &inName) throw(Error) {
    if( inName.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("PortReference name", inName);
        throw e;
    }
    if( false == mPortReferences.remove( inName ) )
    {
        Error e( eMessageIdErrorItemNotFound,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("PortReference name", inName);
        throw e;
    }
    return;
}

/**
 * Get list of all port references.
 *
 * @param[out] outPortRefs List of port references
 */
void
Instance::getPortReferences(
    std::vector< PortReferenceSharedPtr > &outPortRefs
        ) const throw() {
  mPortReferences.getValues( outPortRefs );
}

/**
 * Set list of all port references.
 *
 * @param[in] inSource List of port references
 */
void
Instance::setPortReferences(
    const std::vector< PortReferenceSharedPtr > &inSource
    ) throw( Error ) {
    std::vector< PortReferenceSharedPtr >::const_iterator portRef = inSource.begin();
    std::vector< PortReferenceSharedPtr >::const_iterator endP = inSource.end();
    for( ; portRef != endP; ++endP )
    {
        try
        {
            addPortReference( *portRef );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
    return;
}

void
Instance::setDesignator(const std::string & inSource) throw() {
    mDesignator = inSource;
}

/**
 * Set the pointer to the timing object
 *
 * @param[in] inSource Pointer to the timing object
 */
void
Instance::setTiming(const TimingSharedPtr & inSource ) throw() {
    mTiming = inSource;
}

void
Instance::flatten() throw(Error) {
}

Instance::Instance()
    : Composite<Instance>(),
    Commentable(),
    Nameable(),
    PropertyContainer(),
    Renamable(),
    Visitable(),
    ParentedObject<View>(),
    UserDataContainer(),
#ifdef GENOM_SERIALIZATION
    mMasterData( NULL ),
#endif //GENOM_SERIALIZATION
    mMaster(),
    mPortReferences(),
    mMyContext(),
    mDesignator() {

}

Instance::~Instance() throw() {
#ifdef GENOM_SERIALIZATION
    delete mMasterData;
#endif //GENOM_SERIALIZATION
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
Instance::load( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object<
                                Composite<Instance> >( *this );
    ar & boost::serialization::base_object<Commentable>( *this );
    ar & boost::serialization::base_object<Nameable>( *this );
    ar & boost::serialization::base_object<PropertyContainer>(*this);
    ar & boost::serialization::base_object<Renamable>( *this );
    ar & boost::serialization::base_object<Visitable>( *this );
    ar & mPortReferences;
    ar & mMasterData; //We restore master from view
    mPortReferences.applyOnAll( boost::bind( boost::mem_fn(
                &PortReference::setParent ), _1, getSharedThis() ) );
}

template<class Archive> void
Instance::save ( Archive &ar, unsigned int ) const {
    ar & boost::serialization::base_object<
                                Composite<Instance> >( *this );
    ar & boost::serialization::base_object<Commentable>( *this );
    ar & boost::serialization::base_object<Nameable>( *this );
    ar & boost::serialization::base_object<PropertyContainer>(*this);
    ar & boost::serialization::base_object<Renamable>( *this );
    ar & boost::serialization::base_object<Visitable>( *this );
    ar & mPortReferences;
    mMasterData = new MasterData();
    mMasterData->mLibrary = mMaster->getParent()
                                   ->getParent()->getName();
    mMasterData->mCell = mMaster->getParent()->getName();
    mMasterData->mView = mMaster->getName();
    //We save only our overridden params
    getParameters()->getOverriddenParameters(
                    getParameterContext(), mMasterData->mParams );
    ar & mMasterData;
}

//TO SATISFY THE LINKER
template void
Instance::save<boost::archive::binary_oarchive>(
    boost::archive::binary_oarchive & ar, const unsigned int) const;

template void
Instance::load<boost::archive::binary_iarchive>(
    boost::archive::binary_iarchive & ar, const unsigned int);

void
Instance::restoreMaster() throw(Error)
try
{
    if( !mMasterData )
    {
        //TBD::ERROR
    }
    ViewSharedPtr view = getParent();
    if( !view )
    {
        //TBD::ERROR
    }
    CellSharedPtr cell = view->getParent();
    if( !cell )
    {
        //TBD::ERROR
    }
    LibrarySharedPtr lib = cell->getParent();
    if( !lib )
    {
        //TBD::ERROR
    }
    RootSharedPtr root = lib->getParent();
    if( !root )
    {
        //TBD::ERROR
    }
    LibrarySharedPtr targetLib
                    = root->findLibrary( mMasterData->mLibrary ); 
    if( !targetLib )
    {
        //TBD::ERROR
    }
    CellSharedPtr targetCell
                    = targetLib->findCell( mMasterData->mCell ); 
    if( !targetCell )
    {
        //TBD::ERROR
    }
    ViewSharedPtr targetView
                    = targetCell->findView( mMasterData->mView );
    if( !targetView )
    {
        //TBD::ERROR
    }
    bindToMasterView( targetView );
    //Restore params
    std::map< std::string,ParameterSharedPtr >::iterator
                                it = mMasterData->mParams.begin();
    std::map< std::string,ParameterSharedPtr >::iterator
                                end = mMasterData->mParams.end();
    for(; it !=end; ++it )
    {
        getParameters()->set(
                        mMyContext, (*it).first, (*it).second );
    }
    delete mMasterData;
    mMasterData = NULL;
}
catch( Error &e )
{
    e.setCurrentLocation(
        __FUNCTION__, __FILE__, __LINE__ );
    throw;
}
#endif //GENOM_SERIALIZATION


} // namespace torc::generic

} // namespace torc
