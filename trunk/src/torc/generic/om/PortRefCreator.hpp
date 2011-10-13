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

#ifndef TORC_GENERIC_OM_PORTREFCREATOR_HPP
#define TORC_GENERIC_OM_PORTREFCREATOR_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

#include "torc/generic/om/ScalarPort.hpp"
#include "torc/generic/om/VectorPort.hpp"
#include "torc/generic/om/PortBundle.hpp"
#include "torc/generic/om/ObjectFactory.hpp"
#include "torc/generic/om/VisitorApplier.hpp"

namespace torc {

namespace generic {

/**
 * @brief Create port reference 
 *
 * The PortRefCreator class is used to create scalar, 
 * vector and bundle port reference 
 */
template<typename _ReturnType>
class PortRefCreator :
         public ScalarPort::Visitor,
         public VectorPort::Visitor,
         public PortBundle::Visitor {
  public:
    void
    visit( ScalarPort &port ) throw(Error);

    void
    visit( VectorPort &port ) throw(Error);

    void
    visit( PortBundle &port ) throw(Error);

    inline _ReturnType
    getReturnValue() const throw();

    PortRefCreator(
        const ObjectFactorySharedPtr &inFactory,
        const InstanceSharedPtr &inInstance,
        const PortBundleReferenceSharedPtr &inBundle
                       = PortBundleReferenceSharedPtr() );
    ~PortRefCreator() throw(); 

   private:
    void
    setupCreatedPort( const PortSharedPtr &port,
            const PortReferenceSharedPtr &inPortRef ) throw(Error);

    ObjectFactorySharedPtr mFactory;
    InstanceSharedPtr mInstance;
    PortBundleReferenceSharedPtr mBundle;
    _ReturnType mReturnValue;    
};

template<typename _ReturnType>
void
PortRefCreator<_ReturnType>::visit( ScalarPort &port ) throw(Error) {
    try
    {
        ScalarPortReferenceSharedPtr scalarPortRef;
        mFactory->create( scalarPortRef );
        setupCreatedPort( port.getSharedThis(), scalarPortRef );
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<typename _ReturnType>
void
PortRefCreator<_ReturnType>::visit( VectorPort &port ) throw(Error) {
    try
    {
        VectorPortReferenceSharedPtr vectorPortRef;
        mFactory->create( vectorPortRef );
        std::vector< size_t > limits;
        port.getLimits( limits );
        vectorPortRef->constructChildren( mFactory, limits );
        setupCreatedPort( port.getSharedThis(), vectorPortRef );
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<typename _ReturnType>
void
PortRefCreator<_ReturnType>::visit( PortBundle &port ) throw(Error) {
    try
    {
        PortBundleReferenceSharedPtr portBundleRef;
        mFactory->create( portBundleRef );
        PortRefCreator<PortReferenceSharedPtr> creator( 
                            mFactory, mInstance, portBundleRef );
        VisitorApplier< PortRefCreator<PortReferenceSharedPtr> > 
                                            applier( creator );
        port.applyOnAllChildren( applier );
        setupCreatedPort( port.getSharedThis(), portBundleRef );
        mReturnValue = portBundleRef;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<typename _ReturnType>
inline _ReturnType
PortRefCreator<_ReturnType>::getReturnValue() const throw() {
    return mReturnValue;
}

template<typename _ReturnType>
PortRefCreator<_ReturnType>::PortRefCreator(
    const ObjectFactorySharedPtr &inFactory,
    const InstanceSharedPtr &inInstance,
    const PortBundleReferenceSharedPtr &inBundle )
    :mFactory( inFactory ),
    mInstance( inInstance ),
    mBundle( inBundle ),
    mReturnValue() {
}

template<typename _ReturnType>
PortRefCreator<_ReturnType>::~PortRefCreator() throw() {
}

template<typename _ReturnType>
void
PortRefCreator<_ReturnType>::setupCreatedPort( const PortSharedPtr &port,
            const PortReferenceSharedPtr &inPortRef ) throw(Error) {
    try
    {
        inPortRef->bindToMasterPort( port );
        if( mBundle )
        {
            inPortRef->setParentCollection( mBundle );
            mBundle->addChild( inPortRef );
        }
        else
        {
            inPortRef->setParent( mInstance );
            mInstance->addPortReference( inPortRef );
        }
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_PORTREFCREATOR_HPP
