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

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif 

#include "torc/generic/parser/EdifContext.hpp"
#include "torc/generic/util/Log.hpp"

namespace torc {
namespace generic {

void
EdifContext::setRoot(
        const  RootSharedPtr &inRoot ) throw() {
    mRoot = inRoot;
}

void
EdifContext::setLinker(
        const  LinkerSharedPtr &inLinker ) throw() {
    mLinker = inLinker;
}

void
EdifContext::setCurrentLibrary(
        const  LibrarySharedPtr &inLibrary ) throw() {
    mCurrentLibrary = inLibrary;
}

void
EdifContext::setCurrentDesign(
        const  DesignSharedPtr &inDesign ) throw() {
    mCurrentDesign = inDesign;
}

void
EdifContext::setCurrentCell(
        const  CellSharedPtr &inCell ) throw() {
    mCurrentCell = inCell;
}

void
EdifContext::setCurrentView(
        const  ViewSharedPtr &inView ) throw() {
    mCurrentView = inView;
}

void
EdifContext::setCurrentPort(
        const  PortSharedPtr &inPort ) throw() {
    mCurrentPort = inPort;
}

void
EdifContext::setCurrentPortRef(
    const PortReferenceSharedPtr &inPortRef ) throw() {
    mCurrentPortRef = inPortRef;
}

void
EdifContext::pushCurrentNet( const NetSharedPtr &inNet ) throw() {
    mNet.push( inNet );    
}

void
EdifContext::setCurrentNet(
        const  NetSharedPtr &inNet ) throw() {
    mCurrentNet = inNet;
}

void
EdifContext::popCurrentNet() throw() {
    assert( !mNet.empty() );
    mNet.pop();
}

NetSharedPtr
EdifContext::getCurrentNet() throw() {
    return ( !mNet.empty() )
                ? mNet.top() : NetSharedPtr();
}

PortBundleSharedPtr
EdifContext::getCurrentPortBundleContext() throw() {
    return ( !mPortBundleContext.empty() )
                    ? mPortBundleContext.top()
                    : PortBundleSharedPtr();
}

void
EdifContext::pushPortBundleContext(
        PortBundleSharedPtr inPortBundleContext ) throw() {
    mPortBundleContext.push( inPortBundleContext );
}

void
EdifContext::popPortBundleContext() throw() {
    assert( !mPortBundleContext.empty() );
    mPortBundleContext.pop();
}

NetBundleSharedPtr
EdifContext::getCurrentNetBundleContext() throw() {
   return ( !mNetBundleContext.empty() )
                    ? mNetBundleContext.top()
                    : NetBundleSharedPtr();
}

void
EdifContext::pushNetBundleContext(
        NetBundleSharedPtr inNetBundleContext ) throw() {
    mNetBundleContext.push( inNetBundleContext );
}

void
EdifContext::popNetBundleContext() throw() {
    assert( !mNetBundleContext.empty() );
    mNetBundleContext.pop();
}

void
EdifContext::setIsInInstance( bool inIsInInstance ) throw() {
    mIsInInstance = inIsInInstance;
}

void
EdifContext::setFactory(
    const ObjectFactorySharedPtr &inFactory) throw() {
    mFactory = inFactory;
}

void
EdifContext::setIsViewBeingLinked(
                    bool inIsViewBeingLinked ) throw() {
    mIsViewBeingLinked = inIsViewBeingLinked;
}

void
EdifContext::pushProperty(
        const PropertySharedPtr &inProperty) throw() {
    mProperty.push( inProperty );
}

void
EdifContext::popProperty() throw() {
    assert( !mProperty.empty() );
    mProperty.pop();
}

PropertySharedPtr
EdifContext::getCurrentProperty() throw() {
    return ( !mProperty.empty() )
                ? mProperty.top() : PropertySharedPtr();
}

void
EdifContext::pushPropertyContainer(
    const PropertyContainerSharedPtr &inPropertyContainer
                                    ) throw() {
    mPropertyContainer.push( inPropertyContainer );
}

void
EdifContext::popPropertyContainer() throw() {
    assert( !mPropertyContainer.empty() );
    mPropertyContainer.pop();
}

void
EdifContext::incrementPropertyDepth() throw() {
    mPropertyDepth++;
}

void
EdifContext::decrementPropertyDepth() throw() {
    mPropertyDepth--;
}



PropertyContainerSharedPtr
EdifContext::getCurrentPropertyContainer() const throw() {
    return ( !mPropertyContainer.empty() )
                ? mPropertyContainer.top()
                : PropertyContainerSharedPtr();
}

EdifContext::EdifContext( const RootSharedPtr &inRoot,
                const LinkerSharedPtr &inLinker,
                const ObjectFactorySharedPtr &inFactory,
                const ParserOptions &inOptions )
    :mRoot( inRoot ),
    mLinker( inLinker ),
    mFactory( inFactory ),
    mOptions( inOptions ),
    mCurrentLibrary(),
    mCurrentCell(),
    mCurrentView(),
    mCurrentPort(),
    mCurrentNet(),
    mPortBundleContext(),
    mNetBundleContext(),
    mIsInInstance(),
    mIsViewBeingLinked( false ),
    mProperty(),
    mPropertyContainer(),
	mPropertyDepth(0) {
    log("Context created\n");
}

EdifContext::~EdifContext() throw() {
    log("Context destroyed\n");
}

} //namespace generic
} //namespace torc

