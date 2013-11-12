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

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif 

#include "torc/generic/edif/EdifContext.hpp"
#include "torc/generic/Log.hpp"

namespace torc {
namespace generic {

void EdifContext::setRoot(const RootSharedPtr& inRoot) {
	mRoot = inRoot;
}

void EdifContext::setLinker(const LinkerSharedPtr& inLinker) {
	mLinker = inLinker;
}

void EdifContext::setCurrentLibrary(const LibrarySharedPtr& inLibrary) {
	mCurrentLibrary = inLibrary;
}

void EdifContext::setCurrentDesign(const DesignSharedPtr& inDesign) {
	mCurrentDesign = inDesign;
}

void EdifContext::setCurrentCell(const CellSharedPtr& inCell) {
	mCurrentCell = inCell;
}

void EdifContext::setCurrentView(const ViewSharedPtr& inView) {
	mCurrentView = inView;
}

void EdifContext::setCurrentInstance(const InstanceSharedPtr& inInstance) {
	mCurrentInstance = inInstance;
}

void EdifContext::setCurrentPort(const PortSharedPtr& inPort) {
	mCurrentPort = inPort;
}

void EdifContext::setCurrentPortRef(const PortReferenceSharedPtr& inPortRef) {
	mCurrentPortRef = inPortRef;
}

void EdifContext::pushCurrentNet(const NetSharedPtr& inNet) {
	mNet.push(inNet);
}

void EdifContext::setCurrentNet(const NetSharedPtr& inNet) {
	mCurrentNet = inNet;
}

void EdifContext::popCurrentNet() {
	assert(!mNet.empty());
	mNet.pop();
}

NetSharedPtr EdifContext::getCurrentNet() {
	return (!mNet.empty()) ? mNet.top() : NetSharedPtr();
}

void EdifContext::setCurrentStatus(const StatusSharedPtr& inStatus) {
	mCurrentStatus = inStatus;
}

void EdifContext::setCurrentWritten(const WrittenSharedPtr& inWritten) {
	mCurrentWritten = inWritten;
}

void EdifContext::setCurrentSimulationInfo(const SimulationInfoSharedPtr& inSimulationInfo) {
	mCurrentSimulationInfo = inSimulationInfo;
}

void EdifContext::setCurrentSimulate(const SimulateSharedPtr& inSimulate) {
	mCurrentSimulate = inSimulate;
}

void EdifContext::setCurrentApply(const ApplySharedPtr& inApply) {
	mCurrentApply = inApply;
}

void EdifContext::setCurrentLogicalResponse(const LogicalResponseSharedPtr& inLogicalResponse) {
	mCurrentLogicalResponse = inLogicalResponse;
}

void EdifContext::setCurrentWaveValue(const WaveValueSharedPtr& inWaveValue) {
	mCurrentWaveValue = inWaveValue;
}

void EdifContext::setCurrentLogicValue(const LogicValueSharedPtr& inLogicValue) {
	mCurrentLogicValue = inLogicValue;
}

void EdifContext::setCurrentTiming(const TimingSharedPtr& inTiming) {
	mCurrentTiming = inTiming;
}

void EdifContext::setCurrentPathDelay(const PathDelaySharedPtr& inPathDelay) {
	mCurrentPathDelay = inPathDelay;
}

void EdifContext::setCurrentEvent(const EventSharedPtr& inEvent) {
	mCurrentEvent = inEvent;
}

void EdifContext::setCurrentForbiddenEvent(const ForbiddenEventSharedPtr& inForbiddenEvent) {
	mCurrentForbiddenEvent = inForbiddenEvent;
}

void EdifContext::pushStatusContainer(const StatusContainerSharedPtr& inStatusContainer) {
	mStatusContainer.push(inStatusContainer);
}

void EdifContext::popStatusContainer() {
	assert(!mStatusContainer.empty());
	mStatusContainer.pop();
}

StatusContainerSharedPtr EdifContext::getCurrentStatusContainer() const {
	return (!mStatusContainer.empty()) ? mStatusContainer.top() : StatusContainerSharedPtr();
}

PortBundleSharedPtr EdifContext::getCurrentPortBundleContext() {
	return (!mPortBundleContext.empty()) ? mPortBundleContext.top() : PortBundleSharedPtr();
}

void EdifContext::pushPortBundleContext(PortBundleSharedPtr inPortBundleContext) {
	mPortBundleContext.push(inPortBundleContext);
}

void EdifContext::popPortBundleContext() {
	assert(!mPortBundleContext.empty());
	mPortBundleContext.pop();
}

NetBundleSharedPtr EdifContext::getCurrentNetBundleContext() {
	return (!mNetBundleContext.empty()) ? mNetBundleContext.top() : NetBundleSharedPtr();
}

void EdifContext::pushNetBundleContext(NetBundleSharedPtr inNetBundleContext) {
	mNetBundleContext.push(inNetBundleContext);
}

void EdifContext::popNetBundleContext() {
	assert(!mNetBundleContext.empty());
	mNetBundleContext.pop();
}

void EdifContext::setIsInInstance(bool inIsInInstance) {
	mIsInInstance = inIsInInstance;
}

void EdifContext::setFactory(const ObjectFactorySharedPtr& inFactory) {
	mFactory = inFactory;
}

void EdifContext::setIsViewBeingLinked(bool inIsViewBeingLinked) {
	mIsViewBeingLinked = inIsViewBeingLinked;
}

void EdifContext::pushProperty(const PropertySharedPtr& inProperty) {
	mProperty.push(inProperty);
}

void EdifContext::popProperty() {
	assert(!mProperty.empty());
	mProperty.pop();
}

PropertySharedPtr EdifContext::getCurrentProperty() {
	return (!mProperty.empty()) ? mProperty.top() : PropertySharedPtr();
}

void EdifContext::pushPropertyContainer(const PropertyContainerSharedPtr& inPropertyContainer) {
	mPropertyContainer.push(inPropertyContainer);
}

void EdifContext::popPropertyContainer() {
	assert(!mPropertyContainer.empty());
	mPropertyContainer.pop();
}

void EdifContext::incrementPropertyDepth() {
	mPropertyDepth++;
}

void EdifContext::decrementPropertyDepth() {
	mPropertyDepth--;
}

PropertyContainerSharedPtr EdifContext::getCurrentPropertyContainer() const {
	return (!mPropertyContainer.empty()) ? mPropertyContainer.top() : PropertyContainerSharedPtr();
}

//For Permutable
void EdifContext::pushPermutable(const PermutableSharedPtr& inPermutable) {
	mPermutable.push(inPermutable);
}

void EdifContext::popPermutable() {
	assert(!mPermutable.empty());
	mPermutable.pop();
}

PermutableSharedPtr EdifContext::getCurrentPermutable() {
	return (!mPermutable.empty()) ? mPermutable.top() : PermutableSharedPtr();
}

void EdifContext::incrementPermutableDepth() {
	mPermutableDepth++;
}

void EdifContext::decrementPermutableDepth() {
	mPermutableDepth--;
}

//For Interface joined info
void EdifContext::pushInterfaceJoinedInfo(
	const InterfaceJoinedInfoSharedPtr& inInterfaceJoinedInfo) {
	mInterfaceJoinedInfo.push(inInterfaceJoinedInfo);
}

void EdifContext::popInterfaceJoinedInfo() {
	assert(!mInterfaceJoinedInfo.empty());
	mInterfaceJoinedInfo.pop();
}

InterfaceJoinedInfoSharedPtr EdifContext::getCurrentInterfaceJoinedInfo() {
	return (!mInterfaceJoinedInfo.empty())
		? mInterfaceJoinedInfo.top() : InterfaceJoinedInfoSharedPtr();
}

void EdifContext::incrementInterfaceJoinedInfoDepth() {
	mInterfaceJoinedInfoDepth++;
}

void EdifContext::decrementInterfaceJoinedInfoDepth() {
	mInterfaceJoinedInfoDepth--;
}

//For LogicElement
void EdifContext::pushLogicElement(const LogicElementSharedPtr& inLogicElement) {
	mLogicElement.push(inLogicElement);
}

void EdifContext::popLogicElement() {
	assert(!mLogicElement.empty());
	mLogicElement.pop();
}

LogicElementSharedPtr EdifContext::getCurrentLogicElement() {
	return (!mLogicElement.empty()) ? mLogicElement.top() : LogicElementSharedPtr();
}

void EdifContext::incrementLogicElementDepth() {
	mLogicElementDepth++;
}

void EdifContext::decrementLogicElementDepth() {
	mLogicElementDepth--;
}

EdifContext::EdifContext(const RootSharedPtr& inRoot, const LinkerSharedPtr& inLinker,
	const ObjectFactorySharedPtr& inFactory, const ParserOptions& inOptions) : mRoot(inRoot),
	mLinker(inLinker), mFactory(inFactory), mOptions(inOptions), mCurrentLibrary(),
	mCurrentDesign(), mCurrentCell(), mCurrentView(), mCurrentInstance(), mCurrentPort(),
	mCurrentNet(), mCurrentStatus(), mCurrentWritten(), mStatusContainer(), mPortBundleContext(),
	mNetBundleContext(), mIsInInstance(), mIsViewBeingLinked(false), mProperty(),
	mPropertyContainer(), mPropertyDepth(0), mPermutable(), mPermutableDepth(0),
	mInterfaceJoinedInfo(), mInterfaceJoinedInfoDepth(0), mCurrentSimulationInfo(),
	mCurrentSimulate(), mCurrentApply(), mCurrentLogicalResponse(), mCurrentWaveValue(),
	mCurrentLogicValue(), mCurrentTiming(), mCurrentEvent(), mCurrentForbiddenEvent(),
	mLogicElement(), mLogicElementDepth(0) {
	log("Context created\n");
}

EdifContext::~EdifContext() throw () {
	log("Context destroyed\n");
}

} //namespace generic
} //namespace torc

