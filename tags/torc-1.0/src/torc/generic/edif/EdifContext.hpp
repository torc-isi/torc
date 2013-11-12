// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
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

#ifndef TORC_GENERIC_EDIF_EDIFCONTEXT_HPP
#define TORC_GENERIC_EDIF_EDIFCONTEXT_HPP

#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/edif/ParserPointerTypes.hpp"
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
	inline RootSharedPtr getRoot() const;

	void setRoot(const RootSharedPtr& inRoot);

	inline LinkerSharedPtr getLinker() const;

	void setLinker(const LinkerSharedPtr& inLinker);

	inline LibrarySharedPtr getCurrentLibrary() const;

	void setCurrentLibrary(const LibrarySharedPtr& inLibrary);

	inline CellSharedPtr getCurrentCell() const;

	void setCurrentCell(const CellSharedPtr& inCell);

	inline DesignSharedPtr getCurrentDesign() const;

	void setCurrentDesign(const DesignSharedPtr& inDesign);

	inline ViewSharedPtr getCurrentView() const;

	void setCurrentView(const ViewSharedPtr& inView);

	inline InstanceSharedPtr getCurrentInstance() const;

	void setCurrentInstance(const InstanceSharedPtr& inInstance);

	inline PortSharedPtr getCurrentPort() const;

	void setCurrentPort(const PortSharedPtr& inPort);

	inline PortReferenceSharedPtr getCurrentPortRef() const;

	void setCurrentPortRef(const PortReferenceSharedPtr& inPortRef);

	void pushCurrentNet(const NetSharedPtr& inNet);

	NetSharedPtr getCurrentNet();

	void popCurrentNet();

	void setCurrentNet(const NetSharedPtr& inNet);

	inline StatusSharedPtr getCurrentStatus() const;

	void setCurrentStatus(const StatusSharedPtr& inStatus);

	inline WrittenSharedPtr getCurrentWritten() const;

	void setCurrentWritten(const WrittenSharedPtr& inWritten);

	//For SimulationInfo
	inline SimulationInfoSharedPtr getCurrentSimulationInfo() const;

	void setCurrentSimulationInfo(const SimulationInfoSharedPtr& inSimulationInfo);

	//For Simulate
	inline SimulateSharedPtr getCurrentSimulate() const;

	void setCurrentSimulate(const SimulateSharedPtr& inSimulate);

	//For Apply
	inline ApplySharedPtr getCurrentApply() const;

	void setCurrentApply(const ApplySharedPtr& inApply);

	//For LogicalResponse
	inline LogicalResponseSharedPtr getCurrentLogicalResponse() const;

	void setCurrentLogicalResponse(const LogicalResponseSharedPtr& inLogicalResponse);

	//For WaveValue
	inline WaveValueSharedPtr getCurrentWaveValue() const;

	void setCurrentWaveValue(const WaveValueSharedPtr& inWaveValue);

	//For LogicValue
	inline LogicValueSharedPtr getCurrentLogicValue() const;

	void setCurrentLogicValue(const LogicValueSharedPtr& inLogicValue);

	//For Timing
	inline TimingSharedPtr getCurrentTiming() const;

	void setCurrentTiming(const TimingSharedPtr& inTiming);

	//For PathDelay
	inline PathDelaySharedPtr getCurrentPathDelay() const;

	void setCurrentPathDelay(const PathDelaySharedPtr& inPathDelay);

	//For Event
	inline EventSharedPtr getCurrentEvent() const;

	void setCurrentEvent(const EventSharedPtr& inEvent);

	//For ForbiddenEvent
	inline ForbiddenEventSharedPtr getCurrentForbiddenEvent() const;

	void setCurrentForbiddenEvent(const ForbiddenEventSharedPtr& inForbiddenEvent);

	void popStatusContainer();

	void pushStatusContainer(const StatusContainerSharedPtr& inStatusContainer);

	StatusContainerSharedPtr getCurrentStatusContainer() const;

	PortBundleSharedPtr getCurrentPortBundleContext();

	void pushPortBundleContext(PortBundleSharedPtr inPortBundleContext);

	void popPortBundleContext();

	NetBundleSharedPtr getCurrentNetBundleContext();

	void pushNetBundleContext(NetBundleSharedPtr inNetBundleContext);

	void popNetBundleContext();

	inline bool getIsInInstance() const;

	void setIsInInstance(bool inIsInInstance);

	inline const ParserOptions& getOptions() const;

	inline ObjectFactorySharedPtr getFactory() const;

	void setFactory(const ObjectFactorySharedPtr& inFactory);

	inline bool getIsViewBeingLinked() const;

	void setIsViewBeingLinked(bool inIsViewBeingLinked);

	void pushProperty(const PropertySharedPtr& inProperty);

	void popProperty();

	PropertySharedPtr getCurrentProperty();

	void popPropertyContainer();

	void pushPropertyContainer(const PropertyContainerSharedPtr& inPropertyContainer);

	PropertyContainerSharedPtr getCurrentPropertyContainer() const;

	inline int32_t getPropertyDepth() const;

	void incrementPropertyDepth();

	void decrementPropertyDepth();

	//For Permutable
	void pushPermutable(const PermutableSharedPtr& inPermutable);

	void popPermutable();

	PermutableSharedPtr getCurrentPermutable();

	inline int32_t getPermutableDepth() const;

	void incrementPermutableDepth();

	void decrementPermutableDepth();

	//For InterfaceJoinedInfo
	void pushInterfaceJoinedInfo(const InterfaceJoinedInfoSharedPtr& inInterfaceJoinedInfo);

	void popInterfaceJoinedInfo();

	InterfaceJoinedInfoSharedPtr getCurrentInterfaceJoinedInfo();

	inline int32_t getInterfaceJoinedInfoDepth() const;

	void incrementInterfaceJoinedInfoDepth();

	void decrementInterfaceJoinedInfoDepth();

	//For LogicElement
	void pushLogicElement(const LogicElementSharedPtr& inLogicElement);

	void popLogicElement();

	LogicElementSharedPtr getCurrentLogicElement();

	inline int32_t getLogicElementDepth() const;

	void incrementLogicElementDepth();

	void decrementLogicElementDepth();

	EdifContext(const RootSharedPtr& inRoot, const LinkerSharedPtr& inLinker,
		const ObjectFactorySharedPtr& inFactory, const ParserOptions& inOptions);
	~EdifContext() throw ();

private:
	RootSharedPtr mRoot;
	LinkerSharedPtr mLinker;
	ObjectFactorySharedPtr mFactory;
	const ParserOptions& mOptions;
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
	std::stack<StatusContainerSharedPtr> mStatusContainer;
	std::stack<PortBundleSharedPtr> mPortBundleContext;
	std::stack<NetBundleSharedPtr> mNetBundleContext;
	bool mIsInInstance;
	bool mIsViewBeingLinked;
	std::stack<PropertySharedPtr> mProperty;
	std::stack<PropertyContainerSharedPtr> mPropertyContainer;
	std::stack<NetSharedPtr> mNet;
	int32_t mPropertyDepth;
	std::stack<PermutableSharedPtr> mPermutable;
	int32_t mPermutableDepth;
	std::stack<InterfaceJoinedInfoSharedPtr> mInterfaceJoinedInfo;
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
	std::stack<LogicElementSharedPtr> mLogicElement;
	int32_t mLogicElementDepth;
};

inline RootSharedPtr EdifContext::getRoot() const {
	return mRoot;
}

inline LinkerSharedPtr EdifContext::getLinker() const {
	return mLinker;
}

inline LibrarySharedPtr EdifContext::getCurrentLibrary() const {
	return mCurrentLibrary;
}

inline DesignSharedPtr EdifContext::getCurrentDesign() const {
	return mCurrentDesign;
}

inline CellSharedPtr EdifContext::getCurrentCell() const {
	return mCurrentCell;
}

inline ViewSharedPtr EdifContext::getCurrentView() const {
	return mCurrentView;
}

inline InstanceSharedPtr EdifContext::getCurrentInstance() const {
	return mCurrentInstance;
}

inline PortSharedPtr EdifContext::getCurrentPort() const {
	return mCurrentPort;
}

inline PortReferenceSharedPtr EdifContext::getCurrentPortRef() const {
	return mCurrentPortRef;
}

inline bool EdifContext::getIsInInstance() const {
	return mIsInInstance;
}

inline StatusSharedPtr EdifContext::getCurrentStatus() const {
	return mCurrentStatus;
}

inline WrittenSharedPtr EdifContext::getCurrentWritten() const {
	return mCurrentWritten;
}

inline SimulationInfoSharedPtr EdifContext::getCurrentSimulationInfo() const {
	return mCurrentSimulationInfo;
}

inline SimulateSharedPtr EdifContext::getCurrentSimulate() const {
	return mCurrentSimulate;
}

inline ApplySharedPtr EdifContext::getCurrentApply() const {
	return mCurrentApply;
}

inline LogicalResponseSharedPtr EdifContext::getCurrentLogicalResponse() const {
	return mCurrentLogicalResponse;
}

inline WaveValueSharedPtr EdifContext::getCurrentWaveValue() const {
	return mCurrentWaveValue;
}

inline LogicValueSharedPtr EdifContext::getCurrentLogicValue() const {
	return mCurrentLogicValue;
}

inline TimingSharedPtr EdifContext::getCurrentTiming() const {
	return mCurrentTiming;
}

inline PathDelaySharedPtr EdifContext::getCurrentPathDelay() const {
	return mCurrentPathDelay;
}

inline EventSharedPtr EdifContext::getCurrentEvent() const {
	return mCurrentEvent;
}

inline ForbiddenEventSharedPtr EdifContext::getCurrentForbiddenEvent() const {
	return mCurrentForbiddenEvent;
}

inline const ParserOptions& EdifContext::getOptions() const {
	return mOptions;
}

inline ObjectFactorySharedPtr EdifContext::getFactory() const {
	return mFactory;
}

inline bool EdifContext::getIsViewBeingLinked() const {
	return mIsViewBeingLinked;
}

inline int32_t EdifContext::getPropertyDepth() const {
	return mPropertyDepth;
}

inline int32_t EdifContext::getPermutableDepth() const {
	return mPermutableDepth;
}

inline int32_t EdifContext::getInterfaceJoinedInfoDepth() const {
	return mInterfaceJoinedInfoDepth;
}

inline int32_t EdifContext::getLogicElementDepth() const {
	return mLogicElementDepth;
}

} //namespace generic
} //namespace torc

#endif // TORC_GENERIC_EDIF_EDIFCONTEXT_HPP
