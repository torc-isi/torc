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

#ifndef TORC_GENERIC_PARSER_EDIFCONTEXT_HPP
#define TORC_GENERIC_PARSER_EDIFCONTEXT_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/parser/ParserPointerTypes.hpp"
#include <stack>

//BOOST
#include <boost/shared_ptr.hpp>

namespace torc { namespace generic { class Cell; } }
namespace torc { namespace generic { class Design; } }
namespace torc { namespace generic { class Instance; } }
namespace torc { namespace generic { class Library; } }
namespace torc { namespace generic { class Linker; } }
namespace torc { namespace generic { class Net; } }
namespace torc { namespace generic { class NetBundle; } }
namespace torc { namespace generic { class ObjectFactory; } }
namespace torc { namespace generic { class ParserOptions; } }
namespace torc { namespace generic { class Port; } }
namespace torc { namespace generic { class PortReference; } }
namespace torc { namespace generic { class PortBundle; } }
namespace torc { namespace generic { class Property; } }
namespace torc { namespace generic { class PropertyContainer; } }
namespace torc { namespace generic { class Root; } }
namespace torc { namespace generic { class View; } }
namespace torc { namespace generic { class Instance; } }
namespace torc { namespace generic { class Status; } }
namespace torc { namespace generic { class StatusContainer; } }
namespace torc { namespace generic { class Permutable; } }
namespace torc { namespace generic { class InterfaceJoinedInfo; } }
namespace torc { namespace generic { class SimulationInfo; } }
namespace torc { namespace generic { class Simulate; } }
namespace torc { namespace generic { class LogicValue; } }
namespace torc { namespace generic { class Apply; } }
namespace torc { namespace generic { class LogicalResponse; } }
namespace torc { namespace generic { class LogicElement; } }
namespace torc { namespace generic { class WaveValue; } }
namespace torc { namespace generic { class Timing; } }
namespace torc { namespace generic { class PathDelay; } }
namespace torc { namespace generic { class Event; } }
namespace torc { namespace generic { class ForbiddenEvent; } }

namespace torc {
namespace generic {

class EdifContext {
  public:
    inline RootSharedPtr
    getRoot() const throw();

    void
    setRoot( const  RootSharedPtr &inRoot ) throw();

    inline LinkerSharedPtr
    getLinker() const throw();

    void
    setLinker( const  LinkerSharedPtr &inLinker ) throw();

    inline LibrarySharedPtr
    getCurrentLibrary() const throw();

    void
    setCurrentLibrary(
            const LibrarySharedPtr &inLibrary ) throw();

    inline CellSharedPtr
    getCurrentCell() const throw();

    void
    setCurrentCell( const  CellSharedPtr &inCell ) throw();

    inline DesignSharedPtr
    getCurrentDesign() const throw();

    void
    setCurrentDesign( const  DesignSharedPtr &inDesign ) throw();

    inline ViewSharedPtr
    getCurrentView() const throw();

    void
    setCurrentView( const ViewSharedPtr &inView ) throw();


    inline InstanceSharedPtr
    getCurrentInstance() const throw();

    void
    setCurrentInstance( const InstanceSharedPtr &inInstance ) throw();


    inline PortSharedPtr
    getCurrentPort() const throw();

    void
    setCurrentPort( const PortSharedPtr &inPort ) throw();

    inline PortReferenceSharedPtr
    getCurrentPortRef() const throw();

    void
    setCurrentPortRef(
        const PortReferenceSharedPtr &inPortRef ) throw();

    void
    pushCurrentNet( const NetSharedPtr &inNet ) throw(); 

    NetSharedPtr
    getCurrentNet() throw();

    void
    popCurrentNet() throw();

    void
    setCurrentNet( const NetSharedPtr &inNet ) throw();

    inline StatusSharedPtr
    getCurrentStatus() const throw();

    void
    setCurrentStatus( const StatusSharedPtr &inStatus ) throw();

    inline WrittenSharedPtr
    getCurrentWritten() const throw();

    void
    setCurrentWritten( const WrittenSharedPtr &inWritten ) throw();
    
    //For SimulationInfo
    inline SimulationInfoSharedPtr
    getCurrentSimulationInfo() const throw();

    void
    setCurrentSimulationInfo( const SimulationInfoSharedPtr &inSimulationInfo ) throw();

    //For Simulate
    inline SimulateSharedPtr
    getCurrentSimulate() const throw();

    void
    setCurrentSimulate( const SimulateSharedPtr &inSimulate ) throw();

    //For Apply
    inline ApplySharedPtr
    getCurrentApply() const throw();

    void
    setCurrentApply( const ApplySharedPtr &inApply ) throw();

    //For LogicalResponse
    inline LogicalResponseSharedPtr
    getCurrentLogicalResponse() const throw();

    void
    setCurrentLogicalResponse( const LogicalResponseSharedPtr & inLogicalResponse ) throw();

    //For WaveValue
    inline WaveValueSharedPtr
    getCurrentWaveValue() const throw();

    void
    setCurrentWaveValue( const WaveValueSharedPtr & inWaveValue ) throw();


    //For LogicValue
    inline LogicValueSharedPtr
    getCurrentLogicValue() const throw();

    void
    setCurrentLogicValue( const LogicValueSharedPtr &inLogicValue ) throw();

    //For Timing
    inline TimingSharedPtr
    getCurrentTiming() const throw();

    void
    setCurrentTiming( const TimingSharedPtr &inTiming ) throw();

    //For PathDelay
    inline PathDelaySharedPtr
    getCurrentPathDelay() const throw();

    void
    setCurrentPathDelay( const PathDelaySharedPtr &inPathDelay ) throw();

    //For Event
    inline EventSharedPtr
    getCurrentEvent() const throw();

    void
    setCurrentEvent( const EventSharedPtr &inEvent ) throw();

    //For ForbiddenEvent
    inline ForbiddenEventSharedPtr
    getCurrentForbiddenEvent() const throw();

    void
    setCurrentForbiddenEvent( const ForbiddenEventSharedPtr &inForbiddenEvent ) throw();

    void
    popStatusContainer() throw();

    void
    pushStatusContainer(
        const StatusContainerSharedPtr &inStatusContainer ) throw();

    StatusContainerSharedPtr
    getCurrentStatusContainer() const throw();

    PortBundleSharedPtr
    getCurrentPortBundleContext() throw();

    void
    pushPortBundleContext( PortBundleSharedPtr inPortBundleContext ) throw();

    void
    popPortBundleContext() throw();

    NetBundleSharedPtr
    getCurrentNetBundleContext() throw();

    void
    pushNetBundleContext( NetBundleSharedPtr inNetBundleContext ) throw();

    void
    popNetBundleContext() throw();

    inline bool
    getIsInInstance() const throw();

    void
    setIsInInstance( bool inIsInInstance ) throw();

    inline const ParserOptions &
    getOptions() const throw();

    inline ObjectFactorySharedPtr
    getFactory() const throw();

    void
    setFactory(
        const ObjectFactorySharedPtr &inFactory) throw();

    inline bool
    getIsViewBeingLinked() const throw();    

    void
    setIsViewBeingLinked( bool inIsViewBeingLinked ) throw();

    void
    pushProperty(
            const PropertySharedPtr &inProperty) throw();

    void
    popProperty() throw();

    PropertySharedPtr
    getCurrentProperty() throw();

    void
    popPropertyContainer() throw();

    void
    pushPropertyContainer(
        const PropertyContainerSharedPtr &inPropertyContainer ) throw();

    PropertyContainerSharedPtr
    getCurrentPropertyContainer() const throw();

    inline int32_t
    getPropertyDepth() const throw();

    void
    incrementPropertyDepth() throw(); 

    void
    decrementPropertyDepth() throw(); 

    //For Permutable
    void
    pushPermutable(
            const PermutableSharedPtr &inPermutable) throw();

    void
    popPermutable() throw();

    PermutableSharedPtr
    getCurrentPermutable() throw();

    inline int32_t
    getPermutableDepth() const throw();

    void
    incrementPermutableDepth() throw(); 

    void
    decrementPermutableDepth() throw(); 

    //For InterfaceJoinedInfo
    void
    pushInterfaceJoinedInfo(
            const InterfaceJoinedInfoSharedPtr &inInterfaceJoinedInfo) throw();

    void
    popInterfaceJoinedInfo() throw();

    InterfaceJoinedInfoSharedPtr
    getCurrentInterfaceJoinedInfo() throw();

    inline int32_t
    getInterfaceJoinedInfoDepth() const throw();

    void
    incrementInterfaceJoinedInfoDepth() throw(); 

    void
    decrementInterfaceJoinedInfoDepth() throw(); 

    //For LogicElement
    void
    pushLogicElement(
            const LogicElementSharedPtr &inLogicElement) throw();

    void
    popLogicElement() throw();

    LogicElementSharedPtr
    getCurrentLogicElement() throw();

    inline int32_t
    getLogicElementDepth() const throw();

    void
    incrementLogicElementDepth() throw(); 

    void
    decrementLogicElementDepth() throw(); 

    EdifContext( const RootSharedPtr &inRoot,
                const LinkerSharedPtr &inLinker,
                const ObjectFactorySharedPtr &inFactory,
                const ParserOptions &inOptions );
    ~EdifContext() throw();

  private:
    RootSharedPtr mRoot;
    LinkerSharedPtr mLinker;
    ObjectFactorySharedPtr mFactory;
    const ParserOptions &mOptions;
    LibrarySharedPtr mCurrentLibrary;
    DesignSharedPtr mCurrentDesign;
    CellSharedPtr mCurrentCell;
    ViewSharedPtr mCurrentView;
    InstanceSharedPtr mCurrentInstance;
    PortSharedPtr mCurrentPort;
    PortReferenceSharedPtr mCurrentPortRef;
    NetSharedPtr mCurrentNet;
    StatusSharedPtr mCurrentStatus;
    WrittenSharedPtr mCurrentWritten;
    std::stack< StatusContainerSharedPtr > mStatusContainer;
    std::stack< PortBundleSharedPtr > mPortBundleContext;
    std::stack< NetBundleSharedPtr > mNetBundleContext;
    bool mIsInInstance;
    bool mIsViewBeingLinked;
    std::stack< PropertySharedPtr > mProperty;
    std::stack< PropertyContainerSharedPtr > mPropertyContainer;
    std::stack< NetSharedPtr > mNet;
    int32_t mPropertyDepth;
    std::stack< PermutableSharedPtr > mPermutable;
    int32_t mPermutableDepth;
    std::stack< InterfaceJoinedInfoSharedPtr > mInterfaceJoinedInfo;
    int32_t mInterfaceJoinedInfoDepth;
    SimulationInfoSharedPtr mCurrentSimulationInfo;
    SimulateSharedPtr mCurrentSimulate;
    ApplySharedPtr mCurrentApply;
    LogicalResponseSharedPtr mCurrentLogicalResponse;
    WaveValueSharedPtr mCurrentWaveValue;
    LogicValueSharedPtr mCurrentLogicValue;
    TimingSharedPtr mCurrentTiming;
    PathDelaySharedPtr mCurrentPathDelay;
    EventSharedPtr mCurrentEvent;
    ForbiddenEventSharedPtr mCurrentForbiddenEvent;
    std::stack< LogicElementSharedPtr > mLogicElement;
    int32_t mLogicElementDepth;
}; 

inline RootSharedPtr
EdifContext::getRoot() const throw() {
    return mRoot;
}

inline LinkerSharedPtr
EdifContext::getLinker() const throw() {
    return mLinker;
}

inline LibrarySharedPtr
EdifContext::getCurrentLibrary() const throw() {
    return mCurrentLibrary;
}

inline DesignSharedPtr
EdifContext::getCurrentDesign() const throw() {
    return mCurrentDesign;
}

inline CellSharedPtr
EdifContext::getCurrentCell() const throw() {
    return mCurrentCell;
}

inline ViewSharedPtr
EdifContext::getCurrentView() const throw() {
    return mCurrentView;
}

inline InstanceSharedPtr
EdifContext::getCurrentInstance() const throw() {
    return mCurrentInstance;
}

inline PortSharedPtr
EdifContext::getCurrentPort() const throw() {
    return mCurrentPort;
}

inline PortReferenceSharedPtr
EdifContext::getCurrentPortRef() const throw() {
    return mCurrentPortRef;
}

inline bool
EdifContext::getIsInInstance() const throw() {
    return mIsInInstance;
}

inline StatusSharedPtr
EdifContext::getCurrentStatus() const throw() {
    return mCurrentStatus;
}

inline WrittenSharedPtr
EdifContext::getCurrentWritten() const throw() {
    return mCurrentWritten;
}

inline SimulationInfoSharedPtr
EdifContext::getCurrentSimulationInfo() const throw() {
    return mCurrentSimulationInfo;
}

inline SimulateSharedPtr
EdifContext::getCurrentSimulate() const throw() {
    return mCurrentSimulate;
}

inline ApplySharedPtr
EdifContext::getCurrentApply() const throw() {
    return mCurrentApply;
}

inline LogicalResponseSharedPtr
EdifContext::getCurrentLogicalResponse() const throw() {
    return mCurrentLogicalResponse;
}

inline WaveValueSharedPtr
EdifContext::getCurrentWaveValue() const throw() {
    return mCurrentWaveValue;
}

inline LogicValueSharedPtr
EdifContext::getCurrentLogicValue() const throw() {
    return mCurrentLogicValue;
}

inline TimingSharedPtr
EdifContext::getCurrentTiming() const throw() {
    return mCurrentTiming;
}

inline PathDelaySharedPtr
EdifContext::getCurrentPathDelay() const throw() {
    return mCurrentPathDelay;
}

inline EventSharedPtr
EdifContext::getCurrentEvent() const throw() {
    return mCurrentEvent;
}

inline ForbiddenEventSharedPtr
EdifContext::getCurrentForbiddenEvent() const throw() {
    return mCurrentForbiddenEvent;
}

inline const ParserOptions &
EdifContext::getOptions() const throw() {
    return mOptions;
}

inline ObjectFactorySharedPtr
EdifContext::getFactory() const throw() {
    return mFactory;
}

inline bool
EdifContext::getIsViewBeingLinked() const throw() {
    return mIsViewBeingLinked;
}


inline int32_t
EdifContext::getPropertyDepth() const throw() {
    return mPropertyDepth;
}

inline int32_t
EdifContext::getPermutableDepth() const throw() {
    return mPermutableDepth;
}

inline int32_t
EdifContext::getInterfaceJoinedInfoDepth() const throw() {
    return mInterfaceJoinedInfoDepth;
}

inline int32_t
EdifContext::getLogicElementDepth() const throw() {
    return mLogicElementDepth;
}

} //namespace generic
} //namespace torc

#endif // TORC_GENERIC_PARSER_EDIFCONTEXT_HPP
