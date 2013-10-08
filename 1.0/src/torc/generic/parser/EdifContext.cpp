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
EdifContext::setCurrentInstance(
        const  InstanceSharedPtr &inInstance ) throw() {
    mCurrentInstance = inInstance;
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

void
EdifContext::setCurrentStatus(
        const  StatusSharedPtr &inStatus ) throw() {
    mCurrentStatus = inStatus;
}

void
EdifContext::setCurrentWritten(
        const  WrittenSharedPtr &inWritten ) throw() {
    mCurrentWritten = inWritten;
}

void
EdifContext::setCurrentSimulationInfo(
        const  SimulationInfoSharedPtr &inSimulationInfo ) throw() {
    mCurrentSimulationInfo = inSimulationInfo;
}

void
EdifContext::setCurrentSimulate(
        const  SimulateSharedPtr &inSimulate ) throw() {
    mCurrentSimulate = inSimulate;
}

void
EdifContext::setCurrentApply(
        const  ApplySharedPtr &inApply ) throw() {
    mCurrentApply = inApply;
}

void
EdifContext::setCurrentLogicalResponse(
        const  LogicalResponseSharedPtr &inLogicalResponse ) throw() {
    mCurrentLogicalResponse = inLogicalResponse;
}

void
EdifContext::setCurrentWaveValue(
        const  WaveValueSharedPtr &inWaveValue ) throw() {
    mCurrentWaveValue = inWaveValue;
}

void
EdifContext::setCurrentLogicValue(
        const  LogicValueSharedPtr &inLogicValue ) throw() {
    mCurrentLogicValue = inLogicValue;
}

void
EdifContext::setCurrentTiming(
        const  TimingSharedPtr &inTiming ) throw() {
    mCurrentTiming = inTiming;
}

void
EdifContext::setCurrentPathDelay(
        const  PathDelaySharedPtr &inPathDelay ) throw() {
    mCurrentPathDelay = inPathDelay;
}

void
EdifContext::setCurrentEvent(
        const  EventSharedPtr &inEvent ) throw() {
    mCurrentEvent = inEvent;
}

void
EdifContext::setCurrentForbiddenEvent(
        const  ForbiddenEventSharedPtr &inForbiddenEvent ) throw() {
    mCurrentForbiddenEvent = inForbiddenEvent;
}

void
EdifContext::pushStatusContainer(
    const StatusContainerSharedPtr &inStatusContainer
                                    ) throw() {
    mStatusContainer.push( inStatusContainer );
}

void
EdifContext::popStatusContainer() throw() {
    assert( !mStatusContainer.empty() );
    mStatusContainer.pop();
}

StatusContainerSharedPtr
EdifContext::getCurrentStatusContainer() const throw() {
    return ( !mStatusContainer.empty() )
                ? mStatusContainer.top()
                : StatusContainerSharedPtr();
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

//For Permutable
void
EdifContext::pushPermutable(
        const PermutableSharedPtr &inPermutable) throw() {
    mPermutable.push( inPermutable );
}

void
EdifContext::popPermutable() throw() {
    assert( !mPermutable.empty() );
    mPermutable.pop();
}

PermutableSharedPtr
EdifContext::getCurrentPermutable() throw() {
    return ( !mPermutable.empty() )
                ? mPermutable.top() : PermutableSharedPtr();
}

void
EdifContext::incrementPermutableDepth() throw() {
    mPermutableDepth++;
}

void
EdifContext::decrementPermutableDepth() throw() {
    mPermutableDepth--;
}

//For Interface joined info
void
EdifContext::pushInterfaceJoinedInfo(
        const InterfaceJoinedInfoSharedPtr &inInterfaceJoinedInfo) throw() {
    mInterfaceJoinedInfo.push( inInterfaceJoinedInfo );
}

void
EdifContext::popInterfaceJoinedInfo() throw() {
    assert( !mInterfaceJoinedInfo.empty() );
    mInterfaceJoinedInfo.pop();
}

InterfaceJoinedInfoSharedPtr
EdifContext::getCurrentInterfaceJoinedInfo() throw() {
    return ( !mInterfaceJoinedInfo.empty() )
                ? mInterfaceJoinedInfo.top() : InterfaceJoinedInfoSharedPtr();
}

void
EdifContext::incrementInterfaceJoinedInfoDepth() throw() {
    mInterfaceJoinedInfoDepth++;
}

void
EdifContext::decrementInterfaceJoinedInfoDepth() throw() {
    mInterfaceJoinedInfoDepth--;
}

//For LogicElement
void
EdifContext::pushLogicElement(
        const LogicElementSharedPtr &inLogicElement) throw() {
    mLogicElement.push( inLogicElement );
}

void
EdifContext::popLogicElement() throw() {
    assert( !mLogicElement.empty() );
    mLogicElement.pop();
}

LogicElementSharedPtr
EdifContext::getCurrentLogicElement() throw() {
    return ( !mLogicElement.empty() )
                ? mLogicElement.top() : LogicElementSharedPtr();
}

void
EdifContext::incrementLogicElementDepth() throw() {
    mLogicElementDepth++;
}

void
EdifContext::decrementLogicElementDepth() throw() {
    mLogicElementDepth--;
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
    mCurrentDesign(),
    mCurrentCell(),
    mCurrentView(),
    mCurrentInstance(),
    mCurrentPort(),
    mCurrentNet(),
    mCurrentStatus(),
    mCurrentWritten(),
    mStatusContainer(),
    mPortBundleContext(),
    mNetBundleContext(),
    mIsInInstance(),
    mIsViewBeingLinked( false ),
    mProperty(),
    mPropertyContainer(),
	mPropertyDepth(0),
    mPermutable(),
    mPermutableDepth(0),
    mInterfaceJoinedInfo(),
    mInterfaceJoinedInfoDepth(0),
    mCurrentSimulationInfo(),
    mCurrentSimulate(),
    mCurrentApply(),
    mCurrentLogicalResponse(),
    mCurrentWaveValue(),
    mCurrentLogicValue(),
    mCurrentTiming(),
    mCurrentEvent(),
    mCurrentForbiddenEvent(),
    mLogicElement(),
    mLogicElementDepth(0) {
    log("Context created\n");
}

EdifContext::~EdifContext() throw() {
    log("Context destroyed\n");
}

} //namespace generic
} //namespace torc

