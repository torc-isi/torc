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

#ifndef TORC_GENERIC_CLONING_HPP
#define TORC_GENERIC_CLONING_HPP

#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/Error.hpp"

#include "torc/generic/Root.hpp"
#include "torc/generic/Library.hpp"
#include "torc/generic/Design.hpp"
#include "torc/generic/Cell.hpp"
#include "torc/generic/View.hpp"
#include "torc/generic/ScalarPort.hpp"
#include "torc/generic/VectorPort.hpp"
#include "torc/generic/PortBundle.hpp"
#include "torc/generic/ScalarNet.hpp"
#include "torc/generic/VectorNet.hpp"
#include "torc/generic/NetBundle.hpp"
#include "torc/generic/SingleInstance.hpp"
#include "torc/generic/InstanceArray.hpp"
#include "torc/generic/SingleParameter.hpp"
#include "torc/generic/ParameterArray.hpp"
#include "torc/generic/Property.hpp"
#include "torc/generic/ScalarPortReference.hpp"
#include "torc/generic/VectorPortReference.hpp"
#include "torc/generic/PortBundleReference.hpp"
#include "torc/generic/ObjectFactory.hpp"
#include "torc/generic/InternalUtilityFunctions.hpp"
#include "torc/generic/InterfaceAttributes.hpp"
#include "torc/generic/LogicValueAttributes.hpp"

namespace torc {
namespace generic {

namespace _impl {
template <typename _Tp> class Copier;
}

extern void copyParams(const Instance& inOriginal, const InstanceSharedPtr& outCloned,
	const ObjectFactorySharedPtr& inFactory);

template <typename _Tp> boost::shared_ptr<_Tp> clone(const boost::shared_ptr<_Tp>& inPointer,
	const ObjectFactorySharedPtr& inFactory) throw (Error) {
	try {
		return _impl::Copier<_Tp>()(inPointer, inFactory);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <typename _Tp> boost::shared_ptr<_Tp> cloneJoinedInfo(
	const boost::shared_ptr<_Tp>& inPointer, const ObjectFactorySharedPtr& inFactory,
	const ViewSharedPtr& inView) throw (Error) {
	try {
		return _impl::Copier<_Tp>()(inPointer, inFactory, inView);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <typename _Tp> boost::shared_ptr<_Tp> clonePermutable(
	const boost::shared_ptr<_Tp>& inPointer, const ObjectFactorySharedPtr& inFactory,
	const ViewSharedPtr& inView) throw (Error) {
	try {
		return _impl::Copier<_Tp>()(inPointer, inFactory, inView);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

namespace _impl {

template <typename _Tp> class Copier {
public:
	typedef _Tp Type;
	typedef boost::shared_ptr<Type> Pointer;

public:

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		throw Error(eMessageIdErrorUnsupoortedOperation, __FUNCTION__, __FILE__, __LINE__);
		return mReturnVal;
	}

	Copier() : mFactory(), mReturnVal(), mView() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
	ViewSharedPtr mView;
};

// For Root
template <> class Copier<class Root> : public Root::Visitor {
public:
	typedef Root Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(Root& inRoot) throw (Error) {
		try {
			RootSharedPtr rootPtr;
			mFactory->create(rootPtr);

			rootPtr->setComments(inRoot.getComments());
			rootPtr->setName(inRoot.getName());
			rootPtr->setOriginalName(inRoot.getOriginalName());
			rootPtr->setLevel(inRoot.getLevel());
			rootPtr->setVersion(inRoot.getVersion());

			std::vector<StatusSharedPtr> outStatus;
			inRoot.getStatuses(outStatus);
			for(std::vector<StatusSharedPtr>::iterator it = outStatus.begin();
				it != outStatus.end(); ++it) {
				StatusSharedPtr clonedStatus = clone(*it, mFactory);
				rootPtr->addStatus(clonedStatus);
			}

			std::vector<LibrarySharedPtr> libraries;
			inRoot.getLibraries(libraries);
			std::vector<LibrarySharedPtr>::iterator libIt = libraries.begin();
			for(; libIt != libraries.end(); ++libIt) {
				LibrarySharedPtr newLib = clone(*libIt, mFactory);
				rootPtr->addLibrary(newLib);
			}

			std::vector<DesignSharedPtr> outDesigns;
			inRoot.getDesigns(outDesigns);
			for(std::vector<DesignSharedPtr>::iterator it = outDesigns.begin();
				it != outDesigns.end(); ++it) {
				DesignSharedPtr newDesign = clone(*it, mFactory);
				rootPtr->addDesign(newDesign);
			}

			mReturnVal = rootPtr;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For Design
template <> class Copier<class Design> : public Design::Visitor {
public:
	typedef Design Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(Design& inDesign) throw (Error)
	try {
		DesignSharedPtr designPtr;
		mFactory->create(designPtr);

		designPtr->setComments(inDesign.getComments());
		designPtr->setName(inDesign.getName());
		designPtr->setOriginalName(inDesign.getOriginalName());
		designPtr->setParent(inDesign.getParent());
		designPtr->setCellRefName(inDesign.getCellRefName());
		designPtr->setLibraryRefName(inDesign.getLibraryRefName());

		std::list < std::string > userData;
		inDesign.getUserData(userData);
		designPtr->setUserData(userData);

		std::vector<StatusSharedPtr> outStatus;
		inDesign.getStatuses(outStatus);
		for(std::vector<StatusSharedPtr>::iterator it = outStatus.begin(); it != outStatus.end();
			++it) {
			StatusSharedPtr clonedStatus = clone(*it, mFactory);
			designPtr->addStatus(clonedStatus);
		}

		std::map<std::string, PropertySharedPtr> outProperties;
		std::vector<PropertySharedPtr> properties;
		inDesign.getProperties(outProperties);
		for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
			it != outProperties.end(); ++it) {
			PropertySharedPtr clonedProperty = clone((*it).second, mFactory);
			properties.push_back(clonedProperty);
		}
		for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
			it != properties.end(); ++it) {
			designPtr->setProperty((*it)->getName(), *it);
		}

		mReturnVal = designPtr;
	}
	catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For Library
template <> class Copier<class Library> : public Library::Visitor {
public:
	typedef Library Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(Library& inLibrary) throw (Error)
	try {
		LibrarySharedPtr libPtr;
		mFactory->create(libPtr);

		libPtr->setComments(inLibrary.getComments());
		libPtr->setIsExtern(inLibrary.getIsExtern());
		libPtr->setName(inLibrary.getName());
		libPtr->setParent(inLibrary.getParent());

		std::list < std::string > userData;
		inLibrary.getUserData(userData);
		libPtr->setUserData(userData);

		std::vector<StatusSharedPtr> outStatus;
		inLibrary.getStatuses(outStatus);
		for(std::vector<StatusSharedPtr>::iterator it = outStatus.begin(); it != outStatus.end();
			++it) {
			StatusSharedPtr clonedStatus = clone(*it, mFactory);
			libPtr->addStatus(clonedStatus);
		}

		std::map<Unit, ScaleFactor> outScaleFactors;
		outScaleFactors = inLibrary.getScaleFactors();
		std::map<Unit, ScaleFactor>::iterator it = outScaleFactors.begin();
		for(; it != outScaleFactors.end(); ++it) {
			libPtr->setScaleFactor((*it).first, (*it).second);
		}
		libPtr->setLevel(inLibrary.getLevel());
		libPtr->setOriginalName(inLibrary.getOriginalName());
		std::vector<CellSharedPtr> outCells;
		inLibrary.getCells(outCells);
		std::vector<CellSharedPtr>::iterator cellIt = outCells.begin();
		for(; cellIt != outCells.end(); ++cellIt) {
			CellSharedPtr newCell = clone(*cellIt, mFactory);
			libPtr->addCell(newCell);
		}

		SimulationInfoSharedPtr simuInfo = inLibrary.getSimulationInfo();
		if(simuInfo) {
			SimulationInfoSharedPtr clonedSimuInfo = clone(simuInfo, mFactory);
			libPtr->setSimulationInfo(clonedSimuInfo);
		}

		mReturnVal = libPtr;
	}
	catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For Cell
template <> class Copier<class Cell> : public Cell::Visitor {
public:
	typedef Cell Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(Cell& inCell) throw (Error) {
		try {
			CellSharedPtr cellPtr;
			mFactory->create(cellPtr);
			cellPtr->setComments(inCell.getComments());

			std::list < std::string > userData;
			inCell.getUserData(userData);
			cellPtr->setUserData(userData);

			std::vector<StatusSharedPtr> outStatus;
			inCell.getStatuses(outStatus);
			for(std::vector<StatusSharedPtr>::iterator it = outStatus.begin();
				it != outStatus.end(); ++it) {
				StatusSharedPtr clonedStatus = clone(*it, mFactory);
				cellPtr->addStatus(clonedStatus);
			}

			cellPtr->setIsExtern(inCell.getIsExtern());
			cellPtr->setName(inCell.getName());
			cellPtr->setParent(inCell.getParent());

			std::map<std::string, PropertySharedPtr> outProperties;
			std::vector<PropertySharedPtr> properties;
			inCell.getProperties(outProperties);
			for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
				it != outProperties.end(); ++it) {
				PropertySharedPtr clonedProperty = clone((*it).second, mFactory);
				properties.push_back(clonedProperty);
			}
			for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
				it != properties.end(); ++it) {
				cellPtr->setProperty((*it)->getName(), *it);
			}

			cellPtr->setType(inCell.getType());
			cellPtr->setOriginalName(inCell.getOriginalName());
			std::vector<ViewSharedPtr> outViews;
			inCell.getViews(outViews);
			std::vector<ViewSharedPtr>::iterator viewIt = outViews.begin();
			for(; viewIt != outViews.end(); ++viewIt) {
				ViewSharedPtr newView = clone(*viewIt, mFactory);
				cellPtr->addView(newView);
			}
			mReturnVal = cellPtr;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For Status
template <> class Copier<class Status> : public Status::Visitor {
public:
	typedef Status Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(Status& inStatus) throw (Error) {
		try {
			StatusSharedPtr statusPtr;
			mFactory->create(statusPtr);
			statusPtr->setComments(inStatus.getComments());

			std::list < std::string > userData;
			inStatus.getUserData(userData);
			statusPtr->setUserData(userData);

			std::vector<WrittenSharedPtr> outWrittens;
			inStatus.getWrittens(outWrittens);
			for(std::vector<WrittenSharedPtr>::iterator it = outWrittens.begin();
				it != outWrittens.end(); ++it) {
				WrittenSharedPtr written = *it;
				WrittenSharedPtr newWritten;
				mFactory->create(newWritten);
				newWritten->setComments(written->getComments());

				std::list < std::string > userData;
				written->getUserData(userData);
				newWritten->setUserData(userData);

				std::map<std::string, PropertySharedPtr> outProperties;
				std::vector<PropertySharedPtr> properties;
				written->getProperties(outProperties);
				for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
					it != outProperties.end(); ++it) {
					PropertySharedPtr clonedProperty = clone((*it).second, mFactory);
					properties.push_back(clonedProperty);
				}
				for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
					it != properties.end(); ++it) {
					newWritten->setProperty((*it)->getName(), *it);
				}

				newWritten->setTimeStamp(written->getTimeStamp());
				newWritten->setAuthorName(written->getAuthorName());
				newWritten->setProgramName(written->getProgramName());
				newWritten->setProgramVersion(written->getProgramVersion());
				newWritten->setDataOriginLocationName(written->getDataOriginLocationName());
				newWritten->setDataOriginVersion(written->getDataOriginVersion());
				statusPtr->addWritten(newWritten);
			}

			mReturnVal = statusPtr;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For SimulationInfo
template <> class Copier<class SimulationInfo> : public SimulationInfo::Visitor {
public:
	typedef SimulationInfo Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(SimulationInfo& inSimulationInfo) throw (Error) {
		try {
			SimulationInfoSharedPtr simuInfoPtr;
			mFactory->create(simuInfoPtr);
			simuInfoPtr->setComments(inSimulationInfo.getComments());

			std::list < std::string > userData;
			inSimulationInfo.getUserData(userData);
			simuInfoPtr->setUserData(userData);

			simuInfoPtr->setParent(inSimulationInfo.getParent());

			std::vector<LogicValueSharedPtr> outLogicValues;
			inSimulationInfo.getLogicValues(outLogicValues);
			for(std::vector<LogicValueSharedPtr>::iterator it = outLogicValues.begin();
				it != outLogicValues.end(); ++it) {
				LogicValueSharedPtr newLogicVal = clone(*it, mFactory);
				simuInfoPtr->addLogicValue(newLogicVal);
			}

			mReturnVal = simuInfoPtr;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For LogicValue
template <> class Copier<class LogicValue> : public LogicValue::Visitor {
public:
	typedef LogicValue Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(LogicValue& inLogicValue) throw (Error) {
		try {
			LogicValueSharedPtr logicValPtr;
			mFactory->create(logicValPtr);
			logicValPtr->setName(inLogicValue.getName());
			logicValPtr->setOriginalName(inLogicValue.getOriginalName());
			logicValPtr->setParent(inLogicValue.getParent());
			logicValPtr->setComments(inLogicValue.getComments());

			std::map<std::string, PropertySharedPtr> outProperties;
			std::vector<PropertySharedPtr> properties;
			inLogicValue.getProperties(outProperties);
			for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
				it != outProperties.end(); ++it) {
				PropertySharedPtr clonedProperty = clone((*it).second, mFactory);
				properties.push_back(clonedProperty);
			}
			for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
				it != properties.end(); ++it) {
				logicValPtr->setProperty((*it)->getName(), *it);
			}

			std::list < std::string > userData;
			inLogicValue.getUserData(userData);
			logicValPtr->setUserData(userData);

			LogicValueAttributesSharedPtr attrib = inLogicValue.getAttributes();
			LogicValueAttributesSharedPtr newAttrib(new LogicValueAttributes());
			if(attrib->getIsVoltageMapSet()) {
				newAttrib->setVoltageMap(attrib->getVoltageMap());
			}
			if(attrib->getIsCurrentMapSet()) {
				newAttrib->setCurrentMap(attrib->getCurrentMap());
			}
			if(attrib->getIsBooleanMapSet()) {
				newAttrib->setBooleanMap(attrib->getBooleanMap());
			}
			SimulationInfoSharedPtr simuInfo = logicValPtr->getParent();
			if(attrib->getWeakLogicValue()) {
				LogicValueSharedPtr logicVal = simuInfo->findLogicValue(
					attrib->getWeakLogicValue()->getName());
				newAttrib->setWeakLogicValue(logicVal);
			}
			if(attrib->getStrongLogicValue()) {
				LogicValueSharedPtr logicVal = simuInfo->findLogicValue(
					attrib->getStrongLogicValue()->getName());
				newAttrib->setStrongLogicValue(logicVal);
			}
			{
				std::list<LogicValueSharedPtr> outLogicValues;
				attrib->getCompoundLogicValues(outLogicValues);
				std::list<LogicValueSharedPtr> logicValues;
				for(std::list<LogicValueSharedPtr>::iterator it = outLogicValues.begin();
					it != outLogicValues.end(); ++it) {
					LogicValueSharedPtr logicVal = simuInfo->findLogicValue((*it)->getName());
					logicValues.push_back(logicVal);
				}
				newAttrib->setCompoundLogicValues(logicValues);
			}
			{
				std::list<LogicValueSharedPtr> outLogicValues;
				attrib->getDominatedLogicValues(outLogicValues);
				std::list<LogicValueSharedPtr> logicValues;
				for(std::list<LogicValueSharedPtr>::iterator it = outLogicValues.begin();
					it != outLogicValues.end(); ++it) {
					LogicValueSharedPtr logicVal = simuInfo->findLogicValue((*it)->getName());
					logicValues.push_back(logicVal);
				}
				newAttrib->setDominatedLogicValues(logicValues);
			}
			{
				std::list<LogicValueSharedPtr> outLogicValues;
				attrib->getResolvedLogicValues(outLogicValues);
				std::list<LogicValueSharedPtr> logicValues;
				for(std::list<LogicValueSharedPtr>::iterator it = outLogicValues.begin();
					it != outLogicValues.end(); ++it) {
					LogicValueSharedPtr logicVal = simuInfo->findLogicValue((*it)->getName());
					logicValues.push_back(logicVal);
				}
				newAttrib->setResolvedLogicValues(logicValues);
			}
			if(attrib->getIsIsolated()) {
				newAttrib->setIsIsolated(attrib->getIsIsolated());
			}
			{
				typedef LogicValueAttributes::LogicMap LogicMap;
				std::list<LogicMap*> outLogicMaps;
				attrib->getLogicMaps(outLogicMaps);
				for(std::list<LogicMap*>::iterator it = outLogicMaps.begin();
					it != outLogicMaps.end(); ++it) {
					LogicValueAttributes::LogicMap *logicMap = new LogicValueAttributes::LogicMap();
					logicMap->mLogicRef = (*it)->mLogicRef;
					logicMap->mLibraryRef = (*it)->mLibraryRef;
					logicMap->mLogicMapType = (*it)->mLogicMapType;
					newAttrib->addLogicMap(logicMap);
				}
			}
			logicValPtr->setAttributes(newAttrib);

			mReturnVal = logicValPtr;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For View
template <> class Copier<class View> : public View::Visitor {
public:
	typedef View Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(View& inView) throw (Error) {
		try {
			ViewSharedPtr viewPtr;
			mFactory->create(viewPtr);
			viewPtr->setComments(inView.getComments());

			std::list < std::string > userData;
			inView.getUserData(userData);
			viewPtr->setUserData(userData);

			std::vector<StatusSharedPtr> outStatus;
			inView.getStatuses(outStatus);
			for(std::vector<StatusSharedPtr>::iterator it = outStatus.begin();
				it != outStatus.end(); ++it) {
				StatusSharedPtr clonedStatus = clone(*it, mFactory);
				viewPtr->addStatus(clonedStatus);
			}

			viewPtr->setIsExtern(inView.getIsExtern());
			viewPtr->setName(inView.getName());
			viewPtr->setParent(inView.getParent());
			viewPtr->setType(inView.getType());

			std::map<std::string, PropertySharedPtr> outProperties;
			std::vector<PropertySharedPtr> properties;
			inView.getProperties(outProperties);
			for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
				it != outProperties.end(); ++it) {
				PropertySharedPtr clonedProperty = clone((*it).second, mFactory);
				properties.push_back(clonedProperty);
			}
			for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
				it != properties.end(); ++it) {
				viewPtr->setProperty((*it)->getName(), *it);
			}

			viewPtr->setOriginalName(inView.getOriginalName());

			ParameterMapSharedPtr oldParams = inView.getParameters();
			ParameterMapSharedPtr newParams = viewPtr->getParameters();
			ParameterContext oldCtx = inView.getParameterContext();
			ParameterContext newCtx = viewPtr->getParameterContext();
			std::map<std::string, ParameterSharedPtr> params;
			oldParams->getAllParameters(oldCtx, params);
			for(std::map<std::string, ParameterSharedPtr>::iterator it = params.begin();
				it != params.end(); ++it) {
				ParameterSharedPtr clonedParam = clone((*it).second, mFactory);
				newParams->set(newCtx, (*it).first, clonedParam);
			}

			std::vector<InstanceSharedPtr> outInstances;
			inView.getInstances(outInstances);
			std::vector<InstanceSharedPtr>::iterator instIt = outInstances.begin();
			for(; instIt != outInstances.end(); ++instIt) {
				InstanceSharedPtr newInst = clone(*instIt, mFactory);
				viewPtr->addInstance(newInst);
			}

			std::vector<PortSharedPtr> outPorts;
			inView.getPorts(outPorts);
			std::vector<PortSharedPtr>::iterator portIt = outPorts.begin();
			for(; portIt != outPorts.end(); ++portIt) {
				PortSharedPtr newPort = clone(*portIt, mFactory);
				viewPtr->addPort(newPort);
			}

			std::vector<PermutableSharedPtr> outPermutables;
			inView.getPermutables(outPermutables);
			for(std::vector<PermutableSharedPtr>::iterator it = outPermutables.begin();
				it != outPermutables.end(); ++it) {
				PermutableSharedPtr clonedPermutable = clonePermutable(*it, mFactory, viewPtr);
				viewPtr->addPermutable(clonedPermutable);
			}

			std::vector<InterfaceJoinedInfoSharedPtr> outJoinedInfos;
			inView.getInterfaceJoinedInfos(outJoinedInfos);
			for(std::vector<InterfaceJoinedInfoSharedPtr>::iterator it = outJoinedInfos.begin();
				it != outJoinedInfos.end(); ++it) {
				InterfaceJoinedInfoSharedPtr clonedJoinedInfo = cloneJoinedInfo(*it, mFactory,
					viewPtr);
				viewPtr->addInterfaceJoinedInfo(clonedJoinedInfo);
			}

			std::vector<NetSharedPtr> outNets;
			inView.getNets(outNets);
			std::vector<NetSharedPtr>::iterator netIt = outNets.begin();
			for(; netIt != outNets.end(); ++netIt) {
				NetSharedPtr netPtr = clone(*netIt, mFactory);
				viewPtr->addNet(netPtr);
				connectNets(*netIt, netPtr, viewPtr);
			}
			if(NULL != inView.getTiming()) {
				TimingSharedPtr clonedTiming = clone(inView.getTiming(), mFactory);
				viewPtr->setTiming(clonedTiming);
			}
			if(NULL != inView.getInterfaceAttributes()) {
				InterfaceAttributesSharedPtr attrib = inView.getInterfaceAttributes();
				InterfaceAttributesSharedPtr newAttrib(new InterfaceAttributes());
				if(!attrib->getDesignator().empty()) {
					newAttrib->setDesignator(attrib->getDesignator());
				}
				if(NULL != attrib->getSimulate()) {
					SimulateSharedPtr clonedSimulate = clone(attrib->getSimulate(), mFactory);
					newAttrib->setSimulate(clonedSimulate);
				}
				if(NULL != attrib->getTiming()) {
					TimingSharedPtr clonedTiming = clone(attrib->getTiming(), mFactory);
					newAttrib->setTiming(clonedTiming);
				}
				newAttrib->setComments(attrib->getComments());

				std::list < std::string > userData;
				attrib->getUserData(userData);
				newAttrib->setUserData(userData);

				viewPtr->setInterfaceAttributes(newAttrib);
			}

			if(NULL != inView.getSimulate()) {
				SimulateSharedPtr clonedSimulate = clone(inView.getSimulate(), mFactory);
				viewPtr->setSimulate(clonedSimulate);
			}

			mReturnVal = viewPtr;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	void connectNets(const NetSharedPtr& inNet, const NetSharedPtr& outNet,
		const ViewSharedPtr& inView) {
		if(eCompositionTypeBundle == inNet->getCompositionType()) {
			std::vector<NetSharedPtr> inChildren;
			std::vector<NetSharedPtr> outChildren;
			inNet->getChildren(inChildren);
			outNet->getChildren(outChildren);
			std::vector<NetSharedPtr>::iterator inIt = inChildren.begin();
			std::vector<NetSharedPtr>::iterator outIt = outChildren.begin();
			for(; inIt != inChildren.end(); ++inIt, ++outIt) {
				connectNets(*inIt, *outIt, inView);
			}
		}
		cloneConnection(inNet, outNet, inView);
	}

	void cloneConnection(const NetSharedPtr& inNet, const NetSharedPtr& outNet,
		const ViewSharedPtr& inView) {
		try {
			std::vector<PortSharedPtr> connectedPorts;
			inNet->getConnectedPorts(connectedPorts, true);
			for(std::vector<PortSharedPtr>::iterator it = connectedPorts.begin();
				it != connectedPorts.end(); ++it) {
				std::vector < std::string > nestedNames;
				PortSharedPtr actualPort = *it;
				PortSharedPtr port = actualPort;
				while(port) {
					if(eCompositionTypeVectorBit != port->getCompositionType())
						nestedNames.push_back(port->getName());
					port = port->getParentCollection();
				}
				std::string portName = *nestedNames.rbegin();
				PortSharedPtr targetPort = inView->findPort(portName);
				if(!targetPort) {
					Error e(eMessageIdErrorPointerToItemDoesNotExist, __FUNCTION__, __FILE__,
						__LINE__);
					e.saveContextData("Pointer to the target port does not exist", targetPort);
					throw e;
				}
				if(nestedNames.size() > 1) {
					findLeafConnectable(nestedNames, targetPort);
				}
				std::vector < size_t > indices;
				if(eCompositionTypeVectorBit == actualPort->getCompositionType()) {
					indices = IndexFinder<Port, VectorPortBit>()(actualPort);
				}
				connectNetToElement(indices, targetPort, outNet);
			}
			std::vector<PortReferenceSharedPtr> connectedPortRefs;
			inNet->getConnectedPortRefs(connectedPortRefs, true);
			for(std::vector<PortReferenceSharedPtr>::iterator it = connectedPortRefs.begin();
				it != connectedPortRefs.end(); ++it) {
				std::vector < std::string > nestedNames;
				PortReferenceSharedPtr actualPort = *it;
				PortReferenceSharedPtr port = *it;
				InstanceSharedPtr instance;
				while(port) {
					if(eCompositionTypeVectorBit != port->getCompositionType()) {
						nestedNames.push_back(port->getName());
						instance = port->getParent();
					}
					port = port->getParentCollection();
				}
				std::vector < size_t > indices;
				if(eCompositionTypeVectorBit == instance->getCompositionType()) {
					indices = IndexFinder<Instance, InstanceArrayMember>()(instance);
					instance = instance->getParentCollection();
				}
				InstanceSharedPtr targetInst = inView->findInstance(instance->getName());
				if(!indices.empty()) {
					targetInst = targetInst->get(indices);
				}
				std::string portName = *nestedNames.rbegin();
				PortReferenceSharedPtr targetPort = targetInst->findPortReference(portName);
				if(!targetPort) {
					Error e(eMessageIdErrorPointerToItemDoesNotExist, __FUNCTION__, __FILE__,
						__LINE__);
					e.saveContextData("Pointer to the target port does not exist", targetPort);
					throw e;
				}
				if(nestedNames.size() > 1) {
					findLeafConnectable(nestedNames, targetPort);
				}
				std::vector < size_t > portIndices;
				if(eCompositionTypeVectorBit == actualPort->getCompositionType()) {
					portIndices = IndexFinder<PortReference, VectorPortBitReference>()(actualPort);
				}
				connectNetToElement(portIndices, targetPort, outNet);
			}
			std::vector<PortListSharedPtr> connectedPortLists;
			inNet->getConnectedPortLists(connectedPortLists);
			for(std::vector<PortListSharedPtr>::iterator it = connectedPortLists.begin();
				it != connectedPortLists.end(); ++it) {
				// PortList Clone has not been called directly because, its only
				// add child port/portRef to portList
				PortListSharedPtr cloned;
				mFactory->create(cloned);
				std::list<PortList::PortListElement> elements;
				(*it)->getChildren(elements);
				for(std::list<PortList::PortListElement>::iterator element = elements.begin();
					element != elements.end(); ++element) {
					switch((*element).getType()) {
					case PortList::PortListElement::eElementTypePort: {
						std::vector < std::string > nestedNames;
						PortSharedPtr actualPort = (*element).getPort();
						PortSharedPtr port = actualPort;
						while(port) {
							if(eCompositionTypeVectorBit != port->getCompositionType())
								nestedNames.push_back(port->getName());
							port = port->getParentCollection();
						}
						std::vector<std::string>::reverse_iterator name = nestedNames.rbegin();
						std::string portName = *name;
						PortSharedPtr targetPort = inView->findPort(portName);
						if(nestedNames.size() > 1) {
							findLeafConnectable(nestedNames, targetPort);
						}
						if(eCompositionTypeVectorBit == actualPort->getCompositionType()) {
							targetPort = targetPort->get(
								IndexFinder<Port, VectorPortBit>()(actualPort));
						}
						cloned->addChildPort(targetPort);
						break;
					}
					case PortList::PortListElement::eElementTypePortReference: {
						std::vector < std::string > nestedNames;
						PortReferenceSharedPtr actualPort = (*element).getPortReference();
						PortReferenceSharedPtr port = actualPort;
						InstanceSharedPtr instance;
						while(port) {
							if(eCompositionTypeVectorBit != port->getCompositionType()) {
								nestedNames.push_back(port->getName());
								instance = port->getParent();
							}
							port = port->getParentCollection();
						}
						std::vector < size_t > indices;
						if(eCompositionTypeVectorBit == instance->getCompositionType()) {
							indices = IndexFinder<Instance, InstanceArrayMember>()(instance);
							instance = instance->getParentCollection();
						}
						InstanceSharedPtr targetInst = inView->findInstance(instance->getName());
						if(!indices.empty()) {
							targetInst = targetInst->get(indices);
						}
						std::string portName = *nestedNames.rbegin();
						PortReferenceSharedPtr targetPort = targetInst->findPortReference(portName);
						if(!targetPort) {
							Error e(eMessageIdErrorPointerToItemDoesNotExist, __FUNCTION__,
								__FILE__, __LINE__);
							e.saveContextData("Pointer to the target port does not exist",
								targetPort);
							throw e;
						}
						if(nestedNames.size() > 1) {
							findLeafConnectable(nestedNames, targetPort);
						}
						if(eCompositionTypeVectorBit == actualPort->getCompositionType()) {
							targetPort = targetPort->get(
								IndexFinder<PortReference, VectorPortBitReference>()(actualPort));
						}
						cloned->addChildPortReference(targetPort);
						break;
					}
					}
				}
				cloned->connect(outNet);
			}
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For ScalarPort
template <typename _PointerType> void copyObject(ScalarPort& inPort,
	ObjectFactorySharedPtr& inFactory, _PointerType& outPointer) throw (Error) {
	try {
		ScalarPortSharedPtr scalarPort;
		inFactory->create(scalarPort);
		scalarPort->setComments(inPort.getComments());
		scalarPort->setName(inPort.getName());
		scalarPort->setDirection(inPort.getDirection());
		scalarPort->setOriginalName(inPort.getOriginalName());
		scalarPort->setAttributes(inPort.getAttributes());
		scalarPort->setIsExtern(inPort.getIsExtern());
		scalarPort->setParent(inPort.getParent());
		scalarPort->setParentCollection(inPort.getParentCollection());

		std::list < std::string > userData;
		inPort.getUserData(userData);
		scalarPort->setUserData(userData);

		std::map<std::string, PropertySharedPtr> outProperties;
		std::vector<PropertySharedPtr> properties;
		inPort.getProperties(outProperties);
		for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
			it != outProperties.end(); ++it) {
			PropertySharedPtr clonedProperty = clone((*it).second, inFactory);
			properties.push_back(clonedProperty);
		}
		for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
			it != properties.end(); ++it) {
			scalarPort->setProperty((*it)->getName(), *it);
		}

		outPointer = scalarPort;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <> class Copier<class ScalarPort> : public ScalarPort::Visitor {
public:
	typedef ScalarPort Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(ScalarPort& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For ScalarPortReference
template <typename _PointerType> void copyObject(ScalarPortReference& inPortRef,
	ObjectFactorySharedPtr& inFactory, _PointerType& outPointer) throw (Error) {
	try {
		ScalarPortReferenceSharedPtr portRefPtr;
		inFactory->create(portRefPtr);
		portRefPtr->setParent(inPortRef.getParent());
		portRefPtr->setParentCollection(inPortRef.getParentCollection());
		portRefPtr->setParent(inPortRef.getParent());
		portRefPtr->bindToMasterPort(inPortRef.getMaster());
		outPointer = portRefPtr;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <> class Copier<class ScalarPortReference> : public ScalarPortReference::Visitor {
public:
	typedef ScalarPortReference Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(ScalarPortReference& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For VectorPort
template <typename _PointerType> void copyObject(VectorPort& inPort,
	ObjectFactorySharedPtr& inFactory, _PointerType& outPointer) throw (Error) {
	try {
		VectorPortSharedPtr vectorPortPtr;
		inFactory->create(vectorPortPtr);
		vectorPortPtr->setComments(inPort.getComments());
		vectorPortPtr->setName(inPort.getName());
		vectorPortPtr->setDirection(inPort.getDirection());
		vectorPortPtr->setOriginalName(inPort.getOriginalName());
		vectorPortPtr->setAttributes(inPort.getAttributes());
		vectorPortPtr->setIsExtern(inPort.getIsExtern());
		vectorPortPtr->setParent(inPort.getParent());
		vectorPortPtr->setParentCollection(inPort.getParentCollection());

		std::list < std::string > userData;
		inPort.getUserData(userData);
		vectorPortPtr->setUserData(userData);

		std::map<std::string, PropertySharedPtr> outProperties;
		std::vector<PropertySharedPtr> properties;
		inPort.getProperties(outProperties);
		for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
			it != outProperties.end(); ++it) {
			PropertySharedPtr clonedProperty = clone((*it).second, inFactory);
			properties.push_back(clonedProperty);
		}
		for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
			it != properties.end(); ++it) {
			vectorPortPtr->setProperty((*it)->getName(), *it);
		}

		//CREATE AND POPULATE CHILDREN
		std::vector < size_t > outLimits;
		inPort.getLimits(outLimits);
		vectorPortPtr->constructChildren(inFactory, outLimits);
		VectorPort::BaseType::List children;
		inPort.getCreatedChildren(children);
		for(VectorPort::BaseType::List::iterator it = children.begin(); it != children.end();
			++it) {
			PortSharedPtr child = *it;
			PortSharedPtr clonedChild = vectorPortPtr->get(
				IndexFinder<Port, VectorPortBit>()(child));
			clonedChild->setComments(child->getComments());
			clonedChild->setName(child->getName());
			clonedChild->setDirection(child->getDirection());
			clonedChild->setOriginalName(child->getOriginalName());
			clonedChild->setAttributes(child->getAttributes());
			clonedChild->setIsExtern(child->getIsExtern());
			clonedChild->setParent(child->getParent());

			std::map < std::string, PropertySharedPtr > outProperties;
			std::vector < PropertySharedPtr > properties;
			child->getProperties(outProperties);
			for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
				it != outProperties.end(); ++it) {
				PropertySharedPtr clonedProperty = clone((*it).second, inFactory);
				properties.push_back(clonedProperty);
			}
			for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
				it != properties.end(); ++it) {
				clonedChild->setProperty((*it)->getName(), *it);
			}
		}
		//CONNECTIONS ARE NOT COPIED
		outPointer = vectorPortPtr;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <> class Copier<class VectorPort> : public VectorPort::Visitor {
public:
	typedef VectorPort Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(VectorPort& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For VectorPortReference
template <typename _PointerType> void copyObject(VectorPortReference& inPortRef,
	ObjectFactorySharedPtr& inFactory, _PointerType& outPointer) throw (Error) {
	try {
		VectorPortReferenceSharedPtr vectorPortRefPtr;
		inFactory->create(vectorPortRefPtr);
		//CREATE AND POPULATE CHILDREN
		std::vector < size_t > outLimits;
		inPortRef.getLimits(outLimits);
		vectorPortRefPtr->constructChildren(inFactory, outLimits);
		vectorPortRefPtr->setParent(inPortRef.getParent());
		vectorPortRefPtr->setParentCollection(inPortRef.getParentCollection());
		vectorPortRefPtr->bindToMasterPort(inPortRef.getMaster());
		VectorPortReference::BaseType::List children;
		inPortRef.getCreatedChildren(children);
		for(VectorPortReference::BaseType::List::iterator it = children.begin();
			it != children.end(); ++it) {
			PortReferenceSharedPtr child = *it;
			PortReferenceSharedPtr clonedChild = vectorPortRefPtr->get(
				IndexFinder<PortReference, VectorPortBitReference>()(child));
			clonedChild->setParent(child->getParent());
			//        clonedChild->bindToMasterPort(child->getMaster());
		}
		//CONNECTIONS ARE NOT COPIED
		outPointer = vectorPortRefPtr;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <> class Copier<class VectorPortReference> : public VectorPortReference::Visitor {
public:
	typedef VectorPortReference Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(VectorPortReference& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For PortBundle
template <typename _PointerType> void copyObject(PortBundle& inPort,
	ObjectFactorySharedPtr& inFactory, _PointerType& outPointer) throw (Error) {
	try {
		PortBundleSharedPtr portBundlePtr;
		inFactory->create(portBundlePtr);
		portBundlePtr->setComments(inPort.getComments());
		portBundlePtr->setName(inPort.getName());
		portBundlePtr->setIsExtern(inPort.getIsExtern());
		portBundlePtr->setParent(inPort.getParent());
		portBundlePtr->setParentCollection(inPort.getParentCollection());

		std::list < std::string > userData;
		inPort.getUserData(userData);
		portBundlePtr->setUserData(userData);

		std::map<std::string, PropertySharedPtr> outProperties;
		std::vector<PropertySharedPtr> properties;
		inPort.getProperties(outProperties);
		for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
			it != outProperties.end(); ++it) {
			PropertySharedPtr clonedProperty = clone((*it).second, inFactory);
			properties.push_back(clonedProperty);
		}
		for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
			it != properties.end(); ++it) {
			portBundlePtr->setProperty((*it)->getName(), *it);
		}

		portBundlePtr->setDirection(inPort.getDirection());
		portBundlePtr->setAttributes(inPort.getAttributes());
		portBundlePtr->setOriginalName(inPort.getOriginalName());
		std::vector<PortSharedPtr> children;
		inPort.getChildren(children);
		for(std::vector<PortSharedPtr>::iterator it = children.begin(); it != children.end();
			++it) {
			PortSharedPtr clonedPort = clone(*it, inFactory);
			portBundlePtr->addChild(clonedPort);
		}
		//CONNECTIONS ARE NOT COPIED
		outPointer = portBundlePtr;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <> class Copier<class PortBundle> : public PortBundle::Visitor {
public:
	typedef PortBundle Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(PortBundle& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For PortBundleReference
template <typename _PointerType> void copyObject(PortBundleReference& inPortRef,
	ObjectFactorySharedPtr& inFactory, _PointerType& outPointer) throw (Error) {
	try {
		PortBundleReferenceSharedPtr portBundleRefPtr;
		inFactory->create(portBundleRefPtr);
		portBundleRefPtr->setParent(inPortRef.getParent());
		portBundleRefPtr->setParentCollection(inPortRef.getParentCollection());
		std::vector<PortReferenceSharedPtr> children;
		inPortRef.getChildren(children);
		for(std::vector<PortReferenceSharedPtr>::iterator it = children.begin();
			it != children.end(); ++it) {
			PortReferenceSharedPtr clonedPortRef = clone(*it, inFactory);
			portBundleRefPtr->addChild(clonedPortRef);
		}
		portBundleRefPtr->bindToMasterPort(inPortRef.getMaster());

		//CONNECTIONS ARE NOT COPIED
		outPointer = portBundleRefPtr;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <> class Copier<class PortBundleReference> : public PortBundleReference::Visitor {
public:
	typedef PortBundleReference Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(PortBundleReference& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For SingleInstance
template <typename _PointerType> void copyObject(SingleInstance& inInstance,
	ObjectFactorySharedPtr& inFactory, _PointerType& outPointer) throw (Error) {
	try {
		SingleInstanceSharedPtr instPtr;
		inFactory->create(instPtr);
		instPtr->setComments(inInstance.getComments());
		instPtr->setName(inInstance.getName());
		instPtr->setOriginalName(inInstance.getOriginalName());
		instPtr->setParent(inInstance.getParent());

		std::vector<PortReferenceSharedPtr> outPortRefs;
		inInstance.getPortReferences(outPortRefs);
		for(std::vector<PortReferenceSharedPtr>::iterator it = outPortRefs.begin();
			it != outPortRefs.end(); ++it) {
			PortReferenceSharedPtr clonedRef = clone(*it, inFactory);
			instPtr->addPortReference(clonedRef);
		}
		instPtr->bindToMasterView(inInstance.getMaster());
		//THIS SHOULD BIND ALL THE CLONED PORTREFS TO THE MASTERS
		copyParams(inInstance, instPtr, inFactory);

		std::list < std::string > userData;
		inInstance.getUserData(userData);
		instPtr->setUserData(userData);

		std::map<std::string, PropertySharedPtr> outProperties;
		std::vector<PropertySharedPtr> properties;
		inInstance.getProperties(outProperties);
		for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
			it != outProperties.end(); ++it) {
			PropertySharedPtr clonedProperty = clone((*it).second, inFactory);
			properties.push_back(clonedProperty);
		}
		for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
			it != properties.end(); ++it) {
			instPtr->setProperty((*it)->getName(), *it);
		}

		if(NULL != inInstance.getTiming()) {
			TimingSharedPtr clonedTiming = clone(inInstance.getTiming(), inFactory);
			instPtr->setTiming(clonedTiming);
		}

		outPointer = instPtr;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

//For SingleInstance
template <> class Copier<class SingleInstance> : public SingleInstance::Visitor {
public:
	typedef SingleInstance Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(SingleInstance& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For ScalarNet
template <typename _PointerType> void copyObject(ScalarNet& inNet,
	ObjectFactorySharedPtr& inFactory, _PointerType& outPointer) throw (Error) {
	try {
		ScalarNetSharedPtr netPtr;
		inFactory->create(netPtr);
		netPtr->setComments(inNet.getComments());
		netPtr->setName(inNet.getName());
		netPtr->setParent(inNet.getParent());
		netPtr->setOriginalName(inNet.getOriginalName());

		std::list < std::string > userData;
		inNet.getUserData(userData);
		netPtr->setUserData(userData);

		std::map<std::string, PropertySharedPtr> outProperties;
		std::vector<PropertySharedPtr> properties;
		inNet.getProperties(outProperties);
		for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
			it != outProperties.end(); ++it) {
			PropertySharedPtr clonedProperty = clone((*it).second, inFactory);
			properties.push_back(clonedProperty);
		}
		for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
			it != properties.end(); ++it) {
			netPtr->setProperty((*it)->getName(), *it);
		}

		std::vector<NetSharedPtr> outNets;
		inNet.getSubnets(outNets);
		std::vector<NetSharedPtr>::iterator netIt = outNets.begin();
		for(; netIt != outNets.end(); ++netIt) {
			NetSharedPtr childNet = clone(*netIt, inFactory);
			netPtr->addSubnet(childNet);
		}
		//DO NOT CONNECT
		outPointer = netPtr;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <> class Copier<class ScalarNet> : public ScalarNet::Visitor {
public:
	typedef ScalarNet Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(ScalarNet& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For NetBundle
template <typename _PointerType> void copyObject(NetBundle& inNet,
	ObjectFactorySharedPtr& inFactory, _PointerType& outPointer) throw (Error) {
	try {
		NetBundleSharedPtr netBundlePtr;
		inFactory->create(netBundlePtr);
		netBundlePtr->setComments(inNet.getComments());
		netBundlePtr->setName(inNet.getName());
		netBundlePtr->setParent(inNet.getParent());
		netBundlePtr->setOriginalName(inNet.getOriginalName());

		std::list < std::string > userData;
		inNet.getUserData(userData);
		netBundlePtr->setUserData(userData);

		std::map<std::string, PropertySharedPtr> outProperties;
		std::vector<PropertySharedPtr> properties;
		inNet.getProperties(outProperties);
		for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
			it != outProperties.end(); ++it) {
			PropertySharedPtr clonedProperty = clone((*it).second, inFactory);
			properties.push_back(clonedProperty);
		}
		for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
			it != properties.end(); ++it) {
			netBundlePtr->setProperty((*it)->getName(), *it);
		}

		std::vector<NetSharedPtr> outChildren;
		inNet.getChildren(outChildren);
		std::vector<NetSharedPtr>::iterator it = outChildren.begin();
		for(; it != outChildren.end(); ++it) {
			NetSharedPtr clonedChild = clone(*it, inFactory);
			netBundlePtr->addChild(clonedChild);
		}
		std::vector<NetSharedPtr> outNets;
		inNet.getSubnets(outNets);
		std::vector<NetSharedPtr>::iterator netIt = outNets.begin();
		for(; netIt != outNets.end(); ++netIt) {
			NetSharedPtr childNet = clone(*netIt, inFactory);
			netBundlePtr->addSubnet(childNet);
		}
		outPointer = netBundlePtr;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <> class Copier<class NetBundle> : public NetBundle::Visitor {
public:
	typedef NetBundle Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(NetBundle& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For VectorNet
template <typename _PointerType> void copyObject(VectorNet& inNet,
	ObjectFactorySharedPtr& inFactory, _PointerType& outPointer) throw (Error) {
	try {
		VectorNetSharedPtr vectorNetPtr;
		inFactory->create(vectorNetPtr);
		vectorNetPtr->setComments(inNet.getComments());
		vectorNetPtr->setName(inNet.getName());
		vectorNetPtr->setParent(inNet.getParent());
		vectorNetPtr->setOriginalName(inNet.getOriginalName());

		std::list < std::string > userData;
		inNet.getUserData(userData);
		vectorNetPtr->setUserData(userData);

		std::map<std::string, PropertySharedPtr> outProperties;
		std::vector<PropertySharedPtr> properties;
		inNet.getProperties(outProperties);
		for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
			it != outProperties.end(); ++it) {
			PropertySharedPtr clonedProperty = clone((*it).second, inFactory);
			properties.push_back(clonedProperty);
		}
		for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
			it != properties.end(); ++it) {
			vectorNetPtr->setProperty((*it)->getName(), *it);
		}

		//CREATE AND POPULATE CHILDREN
		std::vector < size_t > outLimits;
		inNet.getLimits(outLimits);
		vectorNetPtr->constructChildren(inFactory, outLimits);
		VectorNet::BaseType::List children;
		inNet.getCreatedChildren(children);
		for(VectorNet::BaseType::List::iterator it = children.begin(); it != children.end(); ++it) {
			NetSharedPtr child = *it;
			NetSharedPtr clonedChild = vectorNetPtr->get(IndexFinder<Net, VectorNetBit>()(child));
			clonedChild->setComments(child->getComments());
			clonedChild->setName(child->getName());
			clonedChild->setParent(child->getParent());
			clonedChild->setOriginalName(child->getOriginalName());

			std::map < std::string, PropertySharedPtr > outProperties;
			std::vector < PropertySharedPtr > properties;
			child->getProperties(outProperties);
			for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
				it != outProperties.end(); ++it) {
				PropertySharedPtr clonedProperty = clone((*it).second, inFactory);
				properties.push_back(clonedProperty);
			}
			for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
				it != properties.end(); ++it) {
				clonedChild->setProperty((*it)->getName(), *it);
			}

			std::vector<NetSharedPtr> outNets;
			child->getSubnets(outNets);
			std::vector<NetSharedPtr>::iterator netIt = outNets.begin();
			for(; netIt != outNets.end(); ++netIt) {
				NetSharedPtr clonedChildNet = clone(*netIt, inFactory);
				clonedChild->addSubnet(clonedChildNet);
			}
		}
		//CONNECTIONS ARE NOT COPIED
		std::vector<NetSharedPtr> outNets;
		inNet.getSubnets(outNets);
		std::vector<NetSharedPtr>::iterator netIt = outNets.begin();
		for(; netIt != outNets.end(); ++netIt) {
			NetSharedPtr childNet = clone(*netIt, inFactory);
			vectorNetPtr->addSubnet(childNet);
		}

		outPointer = vectorNetPtr;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <> class Copier<class VectorNet> : public VectorNet::Visitor {
public:
	typedef VectorNet Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(VectorNet& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For InstanceArray
template <typename _PointerType> void copyObject(InstanceArray& inInstanceArray,
	ObjectFactorySharedPtr& inFactory, _PointerType& outPointer) throw (Error) {
	try {
		InstanceArraySharedPtr instArrayPtr;
		inFactory->create(instArrayPtr);
		instArrayPtr->setComments(inInstanceArray.getComments());
		instArrayPtr->setName(inInstanceArray.getName());
		instArrayPtr->setParent(inInstanceArray.getParent());
		instArrayPtr->setOriginalName(inInstanceArray.getOriginalName());

		std::list < std::string > userData;
		inInstanceArray.getUserData(userData);
		instArrayPtr->setUserData(userData);

		std::map<std::string, PropertySharedPtr> outProperties;
		std::vector<PropertySharedPtr> properties;
		inInstanceArray.getProperties(outProperties);
		for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
			it != outProperties.end(); ++it) {
			PropertySharedPtr clonedProperty = clone((*it).second, inFactory);
			properties.push_back(clonedProperty);
		}
		for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
			it != properties.end(); ++it) {
			instArrayPtr->setProperty((*it)->getName(), *it);
		}

		//WE intentionally bind here
		//This will help when children are bound to different masters
		//The actual child binding will be done later
		instArrayPtr->bindToMasterView(inInstanceArray.getMaster());
		copyParams(inInstanceArray, instArrayPtr, inFactory);

		//CREATE AND POPULATE CHILDREN
		std::vector < size_t > outLimits;
		inInstanceArray.getLimits(outLimits);
		instArrayPtr->constructChildren(inFactory, outLimits);
		InstanceArray::BaseVectorType::List children;
		inInstanceArray.getChildren(children);
		for(InstanceArray::BaseVectorType::List::iterator it = children.begin();
			it != children.end(); ++it) {
			InstanceSharedPtr orig = *it;
			InstanceSharedPtr cloned = instArrayPtr->get(
				IndexFinder<Instance, InstanceArrayMember>()(orig));
			cloned->setComments(orig->getComments());
			cloned->setName(orig->getName());
			cloned->setOriginalName(orig->getOriginalName());
			cloned->setParent(orig->getParent());

			std::list < std::string > userData;
			orig->getUserData(userData);
			cloned->setUserData(userData);

			std::map < std::string, PropertySharedPtr > outProperties;
			std::vector < PropertySharedPtr > properties;
			orig->getProperties(outProperties);
			for(std::map<std::string, PropertySharedPtr>::iterator it = outProperties.begin();
				it != outProperties.end(); ++it) {
				PropertySharedPtr clonedProperty = clone((*it).second, inFactory);
				properties.push_back(clonedProperty);
			}
			for(std::vector<PropertySharedPtr>::iterator it = properties.begin();
				it != properties.end(); ++it) {
				cloned->setProperty((*it)->getName(), *it);
			}

			std::vector<PortReferenceSharedPtr> outPortRefs;
			orig->getPortReferences(outPortRefs);
			for(std::vector<PortReferenceSharedPtr>::iterator pit = outPortRefs.begin();
				pit != outPortRefs.end(); ++pit) {
				PortReferenceSharedPtr clonedRef = clone(*pit, inFactory);
				cloned->addPortReference(clonedRef);
			}
			cloned->bindToMasterView(orig->getMaster());
			copyParams(*orig, cloned, inFactory);
		}
		outPointer = instArrayPtr;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <> class Copier<class InstanceArray> : public InstanceArray::Visitor {
public:
	typedef InstanceArray Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(InstanceArray& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For SingleParameter
template <typename _PointerType> void copyObject(SingleParameter& inSingleParameter,
	ObjectFactorySharedPtr& inFactory, _PointerType& outPointer) throw (Error) {
	try {
		SingleParameterSharedPtr singleParameterPtr;
		inFactory->create(singleParameterPtr);
		singleParameterPtr->setName(inSingleParameter.getName());
		singleParameterPtr->setUnit(inSingleParameter.getUnit());
		singleParameterPtr->setValue(inSingleParameter.getValue());
		singleParameterPtr->setOriginalName(inSingleParameter.getOriginalName());
		outPointer = singleParameterPtr;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <> class Copier<class SingleParameter> : public SingleParameter::Visitor {
public:
	typedef SingleParameter Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(SingleParameter& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For ParameterArray
template <typename _PointerType> void copyObject(ParameterArray& inParamArray,
	ObjectFactorySharedPtr& inFactory, _PointerType& outPointer) throw (Error) {
	try {
		ParameterArraySharedPtr paramArrayPtr;
		inFactory->create(paramArrayPtr);
		paramArrayPtr->setName(inParamArray.getName());
		paramArrayPtr->setOriginalName(inParamArray.getOriginalName());

		std::vector < size_t > outLimits;
		inParamArray.getLimits(outLimits);
		paramArrayPtr->constructChildren(inFactory, outLimits);
		ParameterArray::BaseVectorType::List children;
		inParamArray.getChildren(children);
		ParameterArray::BaseVectorType::List::iterator it = children.begin();
		for(; it != children.end(); ++it) {
			ParameterSharedPtr orig = *it;
			ParameterSharedPtr cloned = paramArrayPtr->get(
				IndexFinder<Parameter, ParameterArrayElement>()(orig));
			cloned->setName(orig->getName());
			cloned->setUnit(orig->getUnit());
			cloned->setValue(orig->getValue());
		}
		outPointer = paramArrayPtr;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

template <> class Copier<class ParameterArray> : public ParameterArray::Visitor {
public:
	typedef ParameterArray Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(ParameterArray& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For Property
template <> class Copier<class Property> : public Property::Visitor {
public:
	typedef Property Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(Property& inProperty) throw (Error) {
		try {
			PropertySharedPtr propertyPtr;
			mFactory->create(propertyPtr);
			propertyPtr->setComments(inProperty.getComments());
			propertyPtr->setName(inProperty.getName());
			propertyPtr->setOriginalName(inProperty.getOriginalName());
			propertyPtr->setOwner(inProperty.getOwner());
			propertyPtr->setUnit(inProperty.getUnit());
			propertyPtr->setValue(inProperty.getValue());

			std::map<std::string, PropertySharedPtr> outValues;
			inProperty.getChildren(outValues);
			for(std::map<std::string, PropertySharedPtr>::iterator it = outValues.begin();
				it != outValues.end(); ++it) {
				PropertySharedPtr childProperty = clone((*it).second, mFactory);
				propertyPtr->addChildProperty((*it).first, childProperty);
			}
			mReturnVal = propertyPtr;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

template <> class Copier<class Port> : public ScalarPort::Visitor, public VectorPort::Visitor,
	public PortBundle::Visitor {
public:
	typedef Port Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(ScalarPort& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	void visit(VectorPort& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	void visit(PortBundle& inPort) throw (Error) {
		try {
			copyObject(inPort, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : ScalarPort::Visitor(), VectorPort::Visitor(), PortBundle::Visitor(), mFactory(),
		mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

template <> class Copier<class PortReference> : public ScalarPortReference::Visitor,
	public VectorPortReference::Visitor, public PortBundleReference::Visitor {
public:
	typedef PortReference Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(ScalarPortReference& inPortReference) throw (Error) {
		try {
			copyObject(inPortReference, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	void visit(VectorPortReference& inPortReference) throw (Error) {
		try {
			copyObject(inPortReference, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	void visit(PortBundleReference& inPortReference) throw (Error) {
		try {
			copyObject(inPortReference, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : ScalarPortReference::Visitor(), VectorPortReference::Visitor(),
		PortBundleReference::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

template <> class Copier<class Net> : public ScalarNet::Visitor, public VectorNet::Visitor,
	public NetBundle::Visitor {
public:
	typedef Net Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(ScalarNet& inNet) throw (Error) {
		try {
			copyObject(inNet, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	void visit(VectorNet& inNet) throw (Error) {
		try {
			copyObject(inNet, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	void visit(NetBundle& inNet) throw (Error) {
		try {
			copyObject(inNet, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : ScalarNet::Visitor(), VectorNet::Visitor(), NetBundle::Visitor(), mFactory(),
		mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

//For Instance
template <> class Copier<class Instance> : public SingleInstance::Visitor,
	public InstanceArray::Visitor {
public:
	typedef Instance Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(SingleInstance& inInstance) throw (Error) {
		try {
			copyObject(inInstance, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	void visit(InstanceArray& inInstance) throw (Error) {
		try {
			copyObject(inInstance, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : SingleInstance::Visitor(), InstanceArray::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

//For Parameter
template <> class Copier<class Parameter> : public SingleParameter::Visitor,
	public ParameterArray::Visitor {
public:
	typedef Parameter Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(SingleParameter& inParameter) throw (Error) {
		try {
			copyObject(inParameter, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	void visit(ParameterArray& inParameter) throw (Error) {
		try {
			copyObject(inParameter, mFactory, mReturnVal);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : SingleParameter::Visitor(), ParameterArray::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For Permutable
template <> class Copier<class Permutable> : public Permutable::Visitor {
public:
	typedef Permutable Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory,
		const ViewSharedPtr& inView) throw (Error) {
		mFactory = inFactory;
		mView = inView;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(Permutable& inPermutable) throw (Error) {
		try {
			PermutableSharedPtr permutablePtr;
			mFactory->create(permutablePtr);
			permutablePtr->setPermutableType(inPermutable.getPermutableType());
			permutablePtr->setIsNonPermutable(inPermutable.getIsNonPermutable());

			std::vector<PortSharedPtr> outPorts;
			inPermutable.getPorts(outPorts);
			std::vector<PortSharedPtr>::iterator portIt = outPorts.begin();
			for(; portIt != outPorts.end(); ++portIt) {
				PortSharedPtr actualPort = *portIt;
				PortSharedPtr port = actualPort;
				std::vector < std::string > nestedNames;
				while(port) {
					if(eCompositionTypeVectorBit != port->getCompositionType())
						nestedNames.push_back(port->getName());
					port = port->getParentCollection();
				}
				std::string portName = *nestedNames.rbegin();
				PortSharedPtr targetPort = mView->findPort(portName);
				if(nestedNames.size() > 1) {
					findLeafConnectable(nestedNames, targetPort);
				}
				std::vector < size_t > indices;
				if(eCompositionTypeVectorBit == actualPort->getCompositionType()) {
					targetPort = targetPort->get(IndexFinder<Port, VectorPortBit>()(actualPort));
					indices = IndexFinder<Port, VectorNetBit>()(actualPort);
				}
				if(!indices.empty()) {
					targetPort = targetPort->get(indices);
				}
				permutablePtr->addPort(targetPort);
			}

			std::vector<PermutableSharedPtr> outPermutables;
			inPermutable.getChildren(outPermutables);
			for(std::vector<PermutableSharedPtr>::iterator it = outPermutables.begin();
				it != outPermutables.end(); ++it) {
				PermutableSharedPtr childPermutable = clonePermutable(*it, mFactory, mView);
				permutablePtr->addChildPermutable(childPermutable);
			}

			mReturnVal = permutablePtr;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}

	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal(), mView() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
	ViewSharedPtr mView;
};

// For InterfaceJoinedInfo
template <> class Copier<class InterfaceJoinedInfo> : public InterfaceJoinedInfo::Visitor {
public:
	typedef InterfaceJoinedInfo Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory,
		const ViewSharedPtr& inView) throw (Error) {
		mFactory = inFactory;
		mView = inView;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(InterfaceJoinedInfo& inInterfaceJoinedInfo) throw (Error) {
		try {
			InterfaceJoinedInfoSharedPtr joinedInfoPtr;
			mFactory->create(joinedInfoPtr);
			joinedInfoPtr->setJoinedType(inInterfaceJoinedInfo.getJoinedType());

			std::list<PortSharedPtr> outPorts;
			inInterfaceJoinedInfo.getPorts(outPorts);
			std::list<PortSharedPtr>::iterator portIt = outPorts.begin();
			for(; portIt != outPorts.end(); ++portIt) {
				PortSharedPtr actualPort = *portIt;
				PortSharedPtr port = actualPort;
				std::vector < std::string > nestedNames;
				while(port) {
					if(eCompositionTypeVectorBit != port->getCompositionType())
						nestedNames.push_back(port->getName());
					port = port->getParentCollection();
				}
				std::string portName = *nestedNames.rbegin();
				PortSharedPtr targetPort = mView->findPort(portName);
				if(nestedNames.size() > 1) {
					findLeafConnectable(nestedNames, targetPort);
				}
				std::vector < size_t > indices;
				if(eCompositionTypeVectorBit == actualPort->getCompositionType()) {
					targetPort = targetPort->get(IndexFinder<Port, VectorPortBit>()(actualPort));
					indices = IndexFinder<Port, VectorNetBit>()(actualPort);
				}
				if(!indices.empty()) {
					targetPort = targetPort->get(indices);
				}

				joinedInfoPtr->addPort(targetPort);
			}

			std::list<PortListSharedPtr> outPortLists;
			inInterfaceJoinedInfo.getPortLists(outPortLists);
			std::list<PortListSharedPtr>::iterator portListIt = outPortLists.begin();
			for(; portListIt != outPortLists.end(); ++portListIt) {
				PortListSharedPtr newPortList = clone(*portListIt, mFactory);
				joinedInfoPtr->addPortList(newPortList);
			}

			std::vector<InterfaceJoinedInfoSharedPtr> outJoinedInfos;
			inInterfaceJoinedInfo.getChildren(outJoinedInfos);
			for(std::vector<InterfaceJoinedInfoSharedPtr>::iterator it = outJoinedInfos.begin();
				it != outJoinedInfos.end(); ++it) {
				InterfaceJoinedInfoSharedPtr childJoinedInfo = cloneJoinedInfo(*it, mFactory,
					mView);
				joinedInfoPtr->addChildJoinedInfo(childJoinedInfo);
			}

			mReturnVal = joinedInfoPtr;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal(), mView() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
	ViewSharedPtr mView;
};

// For PortList
template <> class Copier<class PortList> : public PortList::Visitor {
public:
	typedef PortList Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(PortList& inPortList) throw (Error) {
		try {
			PortListSharedPtr portList;
			mFactory->create(portList);
			std::list<PortList::PortListElement> elements;
			inPortList.getChildren(elements);
			for(std::list<PortList::PortListElement>::iterator it = elements.begin();
				it != elements.end(); ++it) {
				switch((*it).getType()) {
				case PortList::PortListElement::eElementTypePort: {
					portList->addChildPort((*it).getPort());
					break;
				}
				case PortList::PortListElement::eElementTypePortReference: {
					portList->addChildPortReference((*it).getPortReference());
					break;
				}
				}
			}

			mReturnVal = portList;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For Simulate
template <> class Copier<class Simulate> : public Simulate::Visitor {
public:
	typedef Simulate Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(Simulate& inSimulate) throw (Error) {
		try {
			SimulateSharedPtr simulatePtr;
			mFactory->create(simulatePtr);
			simulatePtr->setComments(inSimulate.getComments());

			std::list < std::string > userData;
			inSimulate.getUserData(userData);
			simulatePtr->setUserData(userData);

			simulatePtr->setName(inSimulate.getName());
			simulatePtr->setOriginalName(inSimulate.getOriginalName());

			std::vector<PortListAliasSharedPtr> outPortListAliases;
			inSimulate.getPortListAliases(outPortListAliases);
			for(std::vector<PortListAliasSharedPtr>::iterator it = outPortListAliases.begin();
				it != outPortListAliases.end(); ++it) {
				PortListAliasSharedPtr newPortListAlias;
				mFactory->create(newPortListAlias);
				newPortListAlias->setName((*it)->getName());
				PortListSharedPtr clonedPortList = clone((*it)->getPortList(), mFactory);
				newPortListAlias->setPortList(clonedPortList);
				simulatePtr->addPortListAlias(newPortListAlias);
			}

			std::vector<WaveValueSharedPtr> outWaveValues;
			inSimulate.getWaveValues(outWaveValues);
			for(std::vector<WaveValueSharedPtr>::iterator it = outWaveValues.begin();
				it != outWaveValues.end(); ++it) {
				WaveValueSharedPtr newWaveValue;
				mFactory->create(newWaveValue);
				newWaveValue->setName((*it)->getName());
				newWaveValue->setDeltaTimeDuration((*it)->getDeltaTimeDuration());
				if(NULL != (*it)->getLogicWaveform()) {
					LogicElementSharedPtr clonedLogicElem = clone((*it)->getLogicWaveform(),
						mFactory);
					newWaveValue->setLogicWaveform(clonedLogicElem);
				}
				simulatePtr->addWaveValue(newWaveValue);
			}

			std::vector<ApplySharedPtr> outAllApply;
			inSimulate.getAllApply(outAllApply);
			for(std::vector<ApplySharedPtr>::iterator it = outAllApply.begin();
				it != outAllApply.end(); ++it) {
				ApplySharedPtr newApply;
				mFactory->create(newApply);
				newApply->setComments((*it)->getComments());

				std::list < std::string > userData;
				(*it)->getUserData(userData);
				newApply->setUserData(userData);

				newApply->setNoOfCycle((*it)->getNoOfCycle());
				newApply->setCycleDuration((*it)->getCycleDuration());

				std::list<LogicalResponseSharedPtr> outLogicResponces;
				(*it)->getLogicResponses(outLogicResponces);
				for(std::list<LogicalResponseSharedPtr>::iterator it = outLogicResponces.begin();
					it != outLogicResponces.end(); ++it) {
					LogicalResponseSharedPtr clonedLogicalResponse = clone((*it), mFactory);
					newApply->addLogicResponse(clonedLogicalResponse);
				}
				simulatePtr->addApply(newApply);
			}

			mReturnVal = simulatePtr;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For LogicalResponse
template <> class Copier<class LogicalResponse> : public LogicalResponse::Visitor {
public:
	typedef LogicalResponse Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(LogicalResponse& inLogicalResponse) throw (Error) {
		try {
			LogicalResponseSharedPtr logicalResponsePtr;
			mFactory->create(logicalResponsePtr);
			logicalResponsePtr->setResponseType(inLogicalResponse.getResponseType());
			if(NULL != inLogicalResponse.getConnectedPort()) {

				PortSharedPtr clonedPort = clone(inLogicalResponse.getConnectedPort(), mFactory);
				logicalResponsePtr->setConnectedPort(clonedPort);
			}
			if(NULL != inLogicalResponse.getConnectedPortListAlias()) {
				PortListAliasSharedPtr newPortListAlias;
				mFactory->create(newPortListAlias);
				newPortListAlias->setName(inLogicalResponse.getConnectedPortListAlias()->getName());
				PortListSharedPtr clonedPortList = clone(
					inLogicalResponse.getConnectedPortListAlias()->getPortList(), mFactory);
				newPortListAlias->setPortList(clonedPortList);
				logicalResponsePtr->setConnectedPortListAlias(newPortListAlias);
			}
			if(NULL != inLogicalResponse.getConnectedPortList()) {
				PortListSharedPtr clonedPortList = clone(inLogicalResponse.getConnectedPortList(),
					mFactory);
				logicalResponsePtr->setConnectedPortList(clonedPortList);
			}

			LogicElementSharedPtr clonedLogicElem = clone(inLogicalResponse.getLogicWaveForm(),
				mFactory);
			logicalResponsePtr->setLogicWaveForm(clonedLogicElem);

			mReturnVal = logicalResponsePtr;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For Timing
template <> class Copier<class Timing> : public Timing::Visitor {
public:
	typedef Timing Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(Timing& inTiming) throw (Error) {
		try {
			TimingSharedPtr timingPtr;
			mFactory->create(timingPtr);
			timingPtr->setComments(inTiming.getComments());

			std::list < std::string > userData;
			inTiming.getUserData(userData);
			timingPtr->setUserData(userData);

			timingPtr->setDerivation(inTiming.getDerivation());
			std::list<PathDelaySharedPtr> outPathDelays;
			inTiming.getPathDelays(outPathDelays);
			std::list<PathDelaySharedPtr>::iterator pathDelayIt = outPathDelays.begin();
			for(; pathDelayIt != outPathDelays.end(); pathDelayIt++) {
				PathDelaySharedPtr pathDelay = *pathDelayIt;
				PathDelaySharedPtr newPathDelay;
				mFactory->create(newPathDelay);
				newPathDelay->setDelay(pathDelay->getDelay());

				std::list<EventSharedPtr> outEvents;
				pathDelay->getEvents(outEvents);
				std::list<EventSharedPtr>::iterator eventIt = outEvents.begin();
				for(; eventIt != outEvents.end(); eventIt++) {
					EventSharedPtr clonedEvent = clone(*eventIt, mFactory);
					newPathDelay->addEvent(clonedEvent);
				}

				timingPtr->addPathDelay(newPathDelay);
			}

			std::list<ForbiddenEventSharedPtr> outForbiddentEvents;
			inTiming.getForbiddentEvents(outForbiddentEvents);
			std::list<ForbiddenEventSharedPtr>::iterator forbiddentEventIt =
				outForbiddentEvents.begin();
			for(; forbiddentEventIt != outForbiddentEvents.end(); forbiddentEventIt++) {
				ForbiddenEventSharedPtr clonedForbiddenEvent = clone(*forbiddentEventIt, mFactory);
				timingPtr->addForbiddenEvent(clonedForbiddenEvent);
			}

			mReturnVal = timingPtr;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For ForbiddenEvent
template <> class Copier<class ForbiddenEvent> : public ForbiddenEvent::Visitor {
public:
	typedef ForbiddenEvent Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(ForbiddenEvent& inForbiddenEvent) throw (Error) {
		try {
			ForbiddenEventSharedPtr forbiddenEventPtr;
			mFactory->create(forbiddenEventPtr);
			if(NULL != inForbiddenEvent.getStartTimeInterval()) {
				EventSharedPtr clonedEvent = clone(inForbiddenEvent.getStartTimeInterval(),
					mFactory);
				forbiddenEventPtr->setStartTimeInterval(clonedEvent);
			}
			if(NULL != inForbiddenEvent.getEndTimeInterval()) {
				EventSharedPtr clonedEvent = clone(inForbiddenEvent.getEndTimeInterval(), mFactory);
				forbiddenEventPtr->setEndTimeInterval(clonedEvent);
			} else {
				forbiddenEventPtr->setDuration(inForbiddenEvent.getDuration());
			}
			std::list<EventSharedPtr> outEvents;
			inForbiddenEvent.getEvents(outEvents);
			std::list<EventSharedPtr>::iterator eventIt = outEvents.begin();
			for(; eventIt != outEvents.end(); eventIt++) {
				EventSharedPtr clonedEvent = clone(*eventIt, mFactory);
				forbiddenEventPtr->addEvent(clonedEvent);
			}

			mReturnVal = forbiddenEventPtr;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For Event
template <> class Copier<class Event> : public Event::Visitor {
public:
	typedef Event Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(Event& inEvent) throw (Error) {
		try {
			EventSharedPtr eventPtr;
			mFactory->create(eventPtr);
			eventPtr->setType(inEvent.getType());
			if(Event::eTypeOffsetEvent == inEvent.getType()) {
				eventPtr->setOffsetTime(inEvent.getOffsetTime());
			}

			std::list<PortElement> outPortElements;
			inEvent.getPortElements(outPortElements);
			std::list<PortElement>::iterator portElem = outPortElements.begin();
			std::list<PortElement>::iterator end = outPortElements.end();
			for(; portElem != end; ++portElem) {
				switch((*portElem).getType()) {
				case PortElement::eElementTypePort: {
					PortSharedPtr newPort = clone((*portElem).getPort(), mFactory);
					eventPtr->addPort(newPort);
					break;
				}
				case PortElement::eElementTypePortReference: {
					PortReferenceSharedPtr clonedRef = clone((*portElem).getPortReference(),
						mFactory);
					eventPtr->addPortReference(clonedRef);
					break;
				}
				}
			}

			if(NULL != inEvent.getPortList()) {
				PortListSharedPtr clonedPortList = clone(inEvent.getPortList(), mFactory);
				eventPtr->setPortList(clonedPortList);
			}

			std::list<NetSharedPtr> outNets;
			inEvent.getNets(outNets);
			std::list<NetSharedPtr>::iterator netIt = outNets.begin();

			for(; netIt != outNets.end(); ++netIt) {
				NetSharedPtr netPtr = clone(*netIt, mFactory);
				eventPtr->addNet(netPtr);
			}

			if(NULL != inEvent.getTransition()) {
				LogicElementSharedPtr clonedLogicElem = clone(inEvent.getTransition(), mFactory);
				eventPtr->setTransition(clonedLogicElem);
			}

			mReturnVal = eventPtr;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

// For LogicElement
template <> class Copier<class LogicElement> : public LogicElement::Visitor {
public:
	typedef LogicElement Type;
	typedef boost::shared_ptr<Type> Pointer;

	Pointer operator()(const Pointer& inSource, const ObjectFactorySharedPtr& inFactory)
		throw (Error) {
		mFactory = inFactory;
		inSource->accept(*this);
		return mReturnVal;
	}

	void visit(LogicElement& inLogicElement) throw (Error) {
		try {
			LogicElementSharedPtr logicElementPtr;
			mFactory->create(logicElementPtr);
			logicElementPtr->setType(inLogicElement.getType());
			if(LogicElement::eTypeSingle == inLogicElement.getType()) {
				logicElementPtr->setName(inLogicElement.getName());
			}
			std::vector<LogicElementSharedPtr> outLogicElements;
			inLogicElement.getChildren(outLogicElements);
			for(std::vector<LogicElementSharedPtr>::iterator it = outLogicElements.begin();
				it != outLogicElements.end(); ++it) {
				LogicElementSharedPtr clonedLogicElement = clone(*it, mFactory);
				logicElementPtr->addChildLogicElement(clonedLogicElement);
			}

			mReturnVal = logicElementPtr;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	Copier() : Type::Visitor(), mFactory(), mReturnVal() {
	}

	~Copier() throw () {
	}

private:
	ObjectFactorySharedPtr mFactory;
	Pointer mReturnVal;
};

} //namespace _impl

} // namespace generic
} // namespace torc
#endif // TORC_GENERIC_CLONING_HPP
