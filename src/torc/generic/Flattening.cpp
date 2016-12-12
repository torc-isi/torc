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

#ifdef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif //HAVE_CONFIG_H
#include <algorithm>
#include <iterator>
#include <ostream>
#include <sstream>

#include "torc/generic/Flattening.hpp"

#include "torc/generic/View.hpp"
#include "torc/generic/SingleInstance.hpp"
#include "torc/generic/InstanceArray.hpp"
#include "torc/generic/InstanceArrayMember.hpp"
#include "torc/generic/Port.hpp"
#include "torc/generic/ScalarPortReference.hpp"
#include "torc/generic/VectorPortReference.hpp"
#include "torc/generic/VectorPortBitReference.hpp"
#include "torc/generic/PortBundleReference.hpp"
#include "torc/generic/PortList.hpp"
#include "torc/generic/Net.hpp"
#include "torc/generic/Log.hpp"
#include "torc/generic/Cloning.hpp"

namespace {

using namespace torc::generic;

class FlatteningHelpers {
public:

	static FlatteningHelpers* instance();

	inline NetNamingFunction getNetNamingFunction();

	inline void setNetNamingFunction(const NetNamingFunction& inNamingFunction);

	inline NetRenamingFunction getNetRenamingFunction();

	inline void setNetRenamingFunction(const NetRenamingFunction& inRenamingFunction);

	inline InstanceNamingFunction getInstanceNamingFunction();

	inline void setInstanceNamingFunction(const InstanceNamingFunction& inNamingFunction);

	inline InstanceRenamingFunction getInstanceRenamingFunction();

	inline void setInstanceRenamingFunction(const InstanceRenamingFunction& inRenamingFunction);

	inline std::string getSeparator();

	inline void setSeparator(const std::string& inSeparator);

	inline void reset();

private:
	FlatteningHelpers();

	FlatteningHelpers(const FlatteningHelpers&);
	FlatteningHelpers& operator =(const FlatteningHelpers&);

	NetNamingFunction mNetNamingFunction;
	NetRenamingFunction mNetRenamingFunction;
	InstanceNamingFunction mInstanceNamingFunction;
	InstanceRenamingFunction mInstanceRenamingFunction;
	std::string mSeparator;
};

FlatteningHelpers* FlatteningHelpers::instance() {
	static FlatteningHelpers obj;
	return &obj;
}

inline NetNamingFunction FlatteningHelpers::getNetNamingFunction() {
	return mNetNamingFunction;
}

inline void FlatteningHelpers::setNetNamingFunction(const NetNamingFunction& inNetNamingFunction) {
	mNetNamingFunction = inNetNamingFunction;
}

inline NetRenamingFunction FlatteningHelpers::getNetRenamingFunction() {
	return mNetRenamingFunction;
}

inline void FlatteningHelpers::setNetRenamingFunction(
	const NetRenamingFunction& inNetRenamingFunction) {
	mNetRenamingFunction = inNetRenamingFunction;
}

inline InstanceNamingFunction FlatteningHelpers::getInstanceNamingFunction() {
	return mInstanceNamingFunction;
}

inline void FlatteningHelpers::setInstanceNamingFunction(
	const InstanceNamingFunction& inInstanceNamingFunction) {
	mInstanceNamingFunction = inInstanceNamingFunction;
}

inline InstanceRenamingFunction FlatteningHelpers::getInstanceRenamingFunction() {
	return mInstanceRenamingFunction;
}

inline void FlatteningHelpers::setInstanceRenamingFunction(
	const InstanceRenamingFunction& inInstanceRenamingFunction) {
	mInstanceRenamingFunction = inInstanceRenamingFunction;
}

inline std::string FlatteningHelpers::getSeparator() {
	return mSeparator;
}

inline void FlatteningHelpers::setSeparator(const std::string& inSeparator) {
	mSeparator = inSeparator;
}

inline void FlatteningHelpers::reset() {
	mInstanceRenamingFunction = InstanceRenamingFunction(getRenamedName<InstanceSharedPtr>);
	mNetRenamingFunction = NetRenamingFunction(getRenamedName<NetSharedPtr>);
	mInstanceNamingFunction = InstanceNamingFunction(getModifiedName<InstanceSharedPtr>);
	mNetNamingFunction = NetNamingFunction(getModifiedName<NetSharedPtr>);
	mSeparator = "/";
}

FlatteningHelpers::FlatteningHelpers() : mNetNamingFunction(), mNetRenamingFunction(),
	mInstanceNamingFunction(), mInstanceRenamingFunction(), mSeparator() {}

template <typename _Pointer> struct NameModifier {
public:
	static NameModifier* instance() {
		static NameModifier obj;
		return &obj;
	}
	std::string operator()(const std::string& inInstName, const _Pointer& inNameable,
		const std::vector<size_t>& inIndicesVector = std::vector<size_t>()) {
		std::string name;
		name = inInstName + "_";
		if(inIndicesVector.empty()) {
			name += inNameable->getName();
		} else {
			std::ostringstream indices;
			copy(inIndicesVector.begin(), inIndicesVector.end(),
				std::ostream_iterator < size_t > (indices, "_"));
			name += indices.str();
			name += inNameable->getName();
		}
		if(name.length() > 255) {
			std::map<std::string, std::string>::iterator it = mNameMap.find(name);
			if(it != mNameMap.end()) {
				return (*it).second;
			}

			std::ostringstream sout;
			sout << "N";
			sout.width(8);
			sout.fill('0');
			sout << mNumber++;
			std::string numericName = sout.str();
			mNameMap.insert(std::make_pair(name, numericName));
			return numericName;
		}
		return name;
	}
	NameModifier() :
		mNameMap(), mNumber(1) {
	}

	~NameModifier() {
		mNameMap.clear();
	}
private:
	std::map<std::string, std::string> mNameMap;
	uint64_t mNumber;

};

void replicatePortRefConnections(const NetSharedPtr& inOrigNet, const NetSharedPtr& outTargetNet,
	const ViewSharedPtr& inCurrentView, const InstanceSharedPtr& inInstance) {
	log("Replicating portRef connections for %s to %s\n", inOrigNet->getName().c_str(),
		outTargetNet->getName().c_str());
	std::vector<PortReferenceSharedPtr> portRefs;
	bool isBit = eCompositionTypeVectorBit == inOrigNet->getCompositionType();
	inOrigNet->getConnectedPortRefs(portRefs, !isBit);
	for(std::vector<PortReferenceSharedPtr>::iterator ref = portRefs.begin(); ref != portRefs.end();
		++ref) {
		std::vector < std::string > nestedNames;
		PortReferenceSharedPtr actualPortRef = *ref;
		PortReferenceSharedPtr portRef = actualPortRef;
		InstanceSharedPtr instance;
		while(portRef) {
			if(eCompositionTypeVectorBit != portRef->getCompositionType()) {
				nestedNames.push_back(portRef->getName());
				instance = portRef->getParent();
			}
			portRef = portRef->getParentCollection();
		}
		std::vector < size_t > indices;
		if(eCompositionTypeVectorBit == instance->getCompositionType()) {
			indices = IndexFinder<Instance, InstanceArrayMember>()(instance);
			instance = instance->getParentCollection();
		}
		std::vector < size_t > originalInstanceIndex;
		if(eCompositionTypeVectorBit == inInstance->getCompositionType()) {
			originalInstanceIndex = IndexFinder<Instance, InstanceArrayMember>()(inInstance);
		}
		std::string newInstName = FlatteningHelpers::instance()->getInstanceNamingFunction()(
			inInstance->getName(), instance, originalInstanceIndex);
		InstanceSharedPtr targetInst = inCurrentView->findInstance(newInstName);
		if(!indices.empty()) {
			targetInst = targetInst->get(indices);
		}
		std::string portName = *nestedNames.rbegin();
		PortReferenceSharedPtr targetPortRef = targetInst->findPortReference(portName);
		if(!targetPortRef) {
			//TBD::ERROR
		}
		if(nestedNames.size() > 1) {
			findLeafConnectable(nestedNames, targetPortRef);
		}
		if(eCompositionTypeVectorBit == actualPortRef->getCompositionType()) {
			std::vector < size_t > portRefIndices = IndexFinder<PortReference,
				VectorPortBitReference>()(actualPortRef);
			targetPortRef = targetPortRef->get(portRefIndices);
		}
		log("\tConnecting %s to net %s\n", targetPortRef->getName().c_str(),
			outTargetNet->getName().c_str());
		log("\tIndices: ");
		targetPortRef->connect(outTargetNet);
	}
}

class PortRefConnectionReplicator : ScalarNet::Visitor, VectorNet::Visitor, VectorNetBit::Visitor,
	NetBundle::Visitor {
private:

public:

	void visit(ScalarNet& inScalarNet) throw (Error) {
		try {
			replicatePortRefConnections(inScalarNet.getSharedThis(), mTargetNet, mCurrentView,
				mOriginalInstance);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	void visit(VectorNet& inVectorNet) throw (Error) {
		try {
			replicatePortRefConnections(inVectorNet.getSharedThis(), mTargetNet, mCurrentView,
				mOriginalInstance);
#if 0
			std::vector<NetSharedPtr> children;
			inVectorNet.getCreatedChildren(children);
			for(std::vector<NetSharedPtr>::iterator it = children.begin(); it != children.end(); ++it) {
				NetSharedPtr childNet = *it;
				PortRefConnectionReplicator()(childNet,
					mTargetNet->get(IndexFinder<PortReference, VectorPortBitReference>()(childNet)),
					mCurrentView, mInstName);
			}
#endif
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	void visit(VectorNetBit& inVectorNetBit) throw (Error) {
		try {
			replicatePortRefConnections(inVectorNetBit.getSharedThis(), mTargetNet, mCurrentView,
				mOriginalInstance);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	void visit(NetBundle& inNetBundle) throw (Error) {
		try {
			replicatePortRefConnections(inNetBundle.getSharedThis(), mTargetNet, mCurrentView,
				mOriginalInstance);
#if 0
			std::vector< NetSharedPtr > children;
			inNetBundle.getChildren(children);
			std::vector< NetSharedPtr > targetChildren;
			mTargetNet->getChildren(targetChildren);
			std::vector< NetSharedPtr >::iterator target
			= targetChildren.begin();
			for( std::vector< NetSharedPtr >::iterator it
				= children.begin(); it != children.end();
				++it, ++target )
			{
				PortRefConnectionReplicator()( *it, *target,
					mCurrentView, mInstName );
			}
#endif
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	void operator ()(const NetSharedPtr& inOrigNet, const NetSharedPtr& outTargetNet,
		const ViewSharedPtr& inCurrentView, const InstanceSharedPtr& inOriginalInstance)
			throw (Error) {
		try {
			mTargetNet = outTargetNet;
			mCurrentView = inCurrentView;
			mOriginalInstance = inOriginalInstance;
			inOrigNet->accept(*this);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	~PortRefConnectionReplicator() throw () {
	}

private:
	NetSharedPtr mTargetNet;
	ViewSharedPtr mCurrentView;
	InstanceSharedPtr mOriginalInstance;
};

void replicateNetConnections(const NetSharedPtr& inOrigNet, const ViewSharedPtr& inCurrentView,
	const InstanceSharedPtr& inInstance, const ObjectFactorySharedPtr& inFactory,
	const std::string& inInstName, NetSharedPtr& outClonedNet, const NetSharedPtr& inTargetNet)
		throw (Error) {
	try {

		std::vector<PortSharedPtr> ports;
		inOrigNet->getConnectedPorts(ports, true);
		if(ports.empty()) {
			NetSharedPtr targetNet;
			if(inTargetNet) {
				targetNet = inTargetNet;
			} else {
				NetSharedPtr clonedNet = clone(inOrigNet, inFactory);
				std::vector < size_t > indices;
				if(eCompositionTypeVectorBit == inInstance->getCompositionType()) {
					indices = IndexFinder<Instance, InstanceArrayMember>()(inInstance);
				}
				clonedNet->setName(
					FlatteningHelpers::instance()->getNetNamingFunction()(inInstName, inOrigNet,
						indices));
				Renamable::Name renamedName =
					FlatteningHelpers::instance()->getNetRenamingFunction()(inInstance, inOrigNet,
						indices);
				clonedNet->setOriginalName(renamedName);
				inCurrentView->addNet(clonedNet);
				outClonedNet = clonedNet;
				targetNet = clonedNet;
			}
			PortRefConnectionReplicator()(inOrigNet, targetNet, inCurrentView, inInstance);
		} else {
			for(std::vector<PortSharedPtr>::iterator port = ports.begin(); port != ports.end();
				++port) {
				std::vector < std::string > nestedNames;
				PortSharedPtr actualPortRef = *port;
				PortSharedPtr portRef = actualPortRef;
				while(portRef) {
					if(eCompositionTypeVectorBit != portRef->getCompositionType()) {
						nestedNames.push_back(portRef->getName());
					}
					portRef = portRef->getParentCollection();
				}
				std::string portName = *nestedNames.rbegin();
				PortReferenceSharedPtr origPortRef = inInstance->findPortReference(portName);
				if(nestedNames.size() > 1) {
					findLeafConnectable(nestedNames, origPortRef);
				}
				if(eCompositionTypeVectorBit == actualPortRef->getCompositionType()) {
					origPortRef = origPortRef->get(
						IndexFinder<Port, VectorPortBit>()(actualPortRef));
				}
				std::vector<NetSharedPtr> nets;
				origPortRef->getConnectedNets(nets);
				for(std::vector<NetSharedPtr>::iterator myNet = nets.begin(); myNet != nets.end();
					++myNet) {
					NetSharedPtr connectedNet = *myNet;
					PortRefConnectionReplicator()(inOrigNet, connectedNet, inCurrentView,
						inInstance);
					origPortRef->disconnect(connectedNet);
				}
			}
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

class NetConnectionReplicator : ScalarNet::Visitor, VectorNet::Visitor, VectorNetBit::Visitor,
	NetBundle::Visitor {
public:

	void visit(ScalarNet& inScalarNet) throw (Error) {
		try {
			replicateNetConnections(inScalarNet.getSharedThis(), mCurrentView, mInstance, mFactory,
				mInstName, mClonedNet, mTargetNet);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
	void visit(VectorNet& inVectorNet) throw (Error) {
		try {
			std::vector<NetSharedPtr> children;
			inVectorNet.getCreatedChildren(children);
			NetSharedPtr targetVector = mClonedNet;
			for(std::vector<NetSharedPtr>::iterator it = children.begin(); it != children.end();
				++it) {
				NetConnectionReplicator()(*it, mCurrentView, mInstance, mFactory, mClonedNet,
					targetVector, mInstName);
			}
			replicateNetConnections(inVectorNet.getSharedThis(), mCurrentView, mInstance, mFactory,
				mInstName, mClonedNet, mTargetNet);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}
	void visit(VectorNetBit& inVectorNetBit) throw (Error) {
		try {
#if 0
			replicateNetConnections(inVectorNetBit.getSharedThis(), mCurrentView, mInstance, mFactory,
				mInstName, mClonedNet, mTargetNet->get(inVectorNetBit.getIndices()));
#endif
			NetSharedPtr origNet = inVectorNetBit.getSharedThis();
			std::vector<PortSharedPtr> ports;
			inVectorNetBit.getConnectedPorts(ports);

			//We now create scalar net and move my connections
			//to this one
			ScalarNetSharedPtr newScalarNet;
			mFactory->create(newScalarNet);
			std::vector < size_t > indices = IndexFinder<Net, VectorNetBit>()(origNet);
			/*
			 std::ostringstream sout;
			 sout<<mInstName<<"_"<<origNet->getName()<<"_";
			 copy(indices.begin(), indices.end(),
			 std::ostream_iterator<size_t>(sout, "_"));
			 */
			std::string newName = FlatteningHelpers::instance()->getNetNamingFunction()(mInstName,
				origNet, indices);
			Renamable::Name renamedName = FlatteningHelpers::instance()->getNetRenamingFunction()(
				mInstance, origNet, indices);
			newScalarNet->setName(newName);
			newScalarNet->setOriginalName(renamedName);
			mCurrentView->addNet(newScalarNet);
			PortRefConnectionReplicator()(origNet, newScalarNet, mCurrentView, mInstance);

			for(std::vector<PortSharedPtr>::iterator port = ports.begin(); port != ports.end();
				++port) {
				std::vector < std::string > nestedNames;
				PortSharedPtr actualPort = *port;
				PortSharedPtr portRef = actualPort;
				while(portRef) {
					if(eCompositionTypeVectorBit != portRef->getCompositionType()) {
						nestedNames.push_back(portRef->getName());
					}
					portRef = portRef->getParentCollection();
				}
				std::string portName = *nestedNames.rbegin();
				PortReferenceSharedPtr origPortRef = mInstance->findPortReference(portName);
				if(nestedNames.size() > 1) {
					findLeafConnectable(nestedNames, origPortRef);
				}
				if(eCompositionTypeVectorBit == actualPort->getCompositionType()) {
					origPortRef = origPortRef->get(IndexFinder<Port, VectorPortBit>()(actualPort));
				}
				//Now have the port ref for this bit
				//find all ports and refs that are connected to
				//this ref and connect them to the newly created
				//net
				std::vector<NetSharedPtr> nets;
				origPortRef->getConnectedNets(nets);
				for(std::vector<NetSharedPtr>::iterator myNet = nets.begin(); myNet != nets.end();
					myNet++) {
					std::vector<PortSharedPtr> cPorts;
					(*myNet)->getConnectedPorts(cPorts);
					for(std::vector<PortSharedPtr>::iterator it = cPorts.begin();
						it != cPorts.end(); ++it) {
						(*it)->connect(newScalarNet);
					}
					std::vector<PortReferenceSharedPtr> cPortReferences;
					(*myNet)->getConnectedPortRefs(cPortReferences);
					for(std::vector<PortReferenceSharedPtr>::iterator it = cPortReferences.begin();
						it != cPortReferences.end(); ++it) {
						if(*it == origPortRef)
							continue;
						(*it)->connect(newScalarNet);
					}
					/*bool toDisconnect = true;*/
					if(eCompositionTypeVectorBit == (*myNet)->getCompositionType()
						&& eCompositionTypeVectorBit == origPortRef->getCompositionType()) {
						std::vector < size_t > netIdx = IndexFinder<Net, VectorNetBit>()(*myNet);
						std::vector < size_t > portIdx = IndexFinder<PortReference,
							VectorPortBitReference>()(origPortRef);
						/*toDisconnect = !(netIdx == portIdx);*/
					}
				}
			}
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	void visit(NetBundle& inNetBundle) throw (Error) {
		try {
			replicateNetConnections(inNetBundle.getSharedThis(), mCurrentView, mInstance, mFactory,
				mInstName, mClonedNet, mTargetNet);
			std::vector<NetSharedPtr> children;
			inNetBundle.getChildren(children);
			std::vector<NetSharedPtr> tChildren;
			mClonedNet->getChildren(tChildren);
			std::vector<NetSharedPtr>::iterator tNet = tChildren.begin();
			for(std::vector<NetSharedPtr>::iterator it = children.begin(); it != children.end();
				++it, ++tNet) {
				NetConnectionReplicator()(*it, mCurrentView, mInstance, mFactory, mClonedNet, *tNet,
					mInstName);
			}
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	void operator ()(const NetSharedPtr& inOrigNet, const ViewSharedPtr& inCurrentView,
		const InstanceSharedPtr& inInstance, const ObjectFactorySharedPtr& inFactory,
		NetSharedPtr& outClonedNet, NetSharedPtr& inTargetNet, const std::string& inInstName =
			std::string()) throw (Error) {
		try {
			mCurrentView = inCurrentView;
			mInstance = inInstance;
			mFactory = inFactory;
			mInstName = (inInstName.empty()) ? inInstance->getName() : inInstName;
			mTargetNet = inTargetNet;
			inOrigNet->accept(*this);
			outClonedNet = mClonedNet;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	~NetConnectionReplicator() throw () {
	}

private:
	ViewSharedPtr mCurrentView;
	InstanceSharedPtr mInstance;
	ObjectFactorySharedPtr mFactory;
	std::string mInstName;
	NetSharedPtr mClonedNet;
	NetSharedPtr mTargetNet;
};

// Add all flatten instances of a target instance
bool addFlattenInstances(const ViewSharedPtr& inParentView, const InstanceSharedPtr& inInstance,
	const ObjectFactorySharedPtr& inFactory, std::list<InstanceSharedPtr>& outAddedInstances)
		throw (Error) {
	std::string name = inInstance->getName();
	ViewSharedPtr masterView = inInstance->getMaster();
	log("Flattening instance with name %s\n", name.c_str());

	log("Copying instantiations... ");
	std::vector<InstanceSharedPtr> childInstances;
	masterView->getInstances(childInstances);
	if(childInstances.empty()) {
		log("Leaf node.. cannot flatten\n");
		return false;
	}
	std::vector<PortReferenceSharedPtr> portRefs;
	inInstance->getPortReferences(portRefs);
	std::string flatInstName;
	std::vector < size_t > indices;
	if(eCompositionTypeVectorBit == inInstance->getCompositionType()) {
		indices = IndexFinder<Instance, InstanceArrayMember>()(inInstance);
	}

	for(std::vector<InstanceSharedPtr>::iterator it = childInstances.begin();
		it != childInstances.end(); ++it) {
		InstanceSharedPtr inst = clone(*it, inFactory);
		std::string instName;
		instName = FlatteningHelpers::instance()->getInstanceNamingFunction()(name, inst, indices);
		Renamable::Name renamedName = FlatteningHelpers::instance()->getInstanceRenamingFunction()(
			inInstance, inst, indices);
		log("Added Instance Name :: %s\n", instName.c_str());
		inst->setName(instName);
		inst->setOriginalName(renamedName);
		inParentView->addInstance(inst);
		outAddedInstances.push_back(inst);
	}
	log("Done\n");
	return true;
}

void flattenInstance(const ViewSharedPtr& inParentView, const InstanceSharedPtr& inInstance,
	const ObjectFactorySharedPtr& inFactory, const std::string& inName = std::string())
		throw (Error) {
	std::string name = (!inName.empty()) ? inName : inInstance->getName();
	ViewSharedPtr masterView = inInstance->getMaster();
	std::vector<InstanceSharedPtr> childInstances;
	masterView->getInstances(childInstances);
	std::vector<PortReferenceSharedPtr> portRefs;
	inInstance->getPortReferences(portRefs);

	//First we copy nets in the instance that are not connected
	//to it's ports
	log("Copying internal nets... ");
	std::vector<NetSharedPtr> allNets;
	masterView->getNets(allNets);
	for(std::vector<NetSharedPtr>::iterator it = allNets.begin(); it != allNets.end(); ++it) {
		NetSharedPtr origNet = *it;
		NetSharedPtr clonedNet;
		NetSharedPtr dummy;
		NetConnectionReplicator()(origNet, inParentView, inInstance, inFactory, clonedNet, dummy);
	}
	log("Done\n");
}

bool flatten_impl(const InstanceSharedPtr& inInstance, const ObjectFactorySharedPtr& inFactory,
	std::list<InstanceSharedPtr>& outAddedInstances) throw (Error) {
	if(!inInstance) {
		//TBD::ERROR
		return false;
	}
	ViewSharedPtr parentView = inInstance->getParent();
	if(!parentView || !parentView->findInstance(inInstance->getName())) {
		//TBD::ERROR
		return false;
	}
	switch(inInstance->getCompositionType()) {
	case eCompositionTypeScalar: {
		try {
			bool added = addFlattenInstances(parentView, inInstance, inFactory, outAddedInstances);
			if(added) {
				flattenInstance(parentView, inInstance, inFactory);
				parentView->removeInstance(inInstance->getName());
			} else {
				return false;
			}
			break;
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}

	}
	case eCompositionTypeVector: {
		try {
			std::vector<InstanceSharedPtr> children;
			inInstance->getChildren(children);
			bool added = false;
			for(std::vector<InstanceSharedPtr>::iterator it = children.begin();
				it != children.end(); ++it) {
				added = addFlattenInstances(parentView, (*it), inFactory, outAddedInstances);
				if(!added) {
					break;
				}
			}
			if(added) {
				for(std::vector<InstanceSharedPtr>::iterator it = children.begin();
					it != children.end(); ++it) {
					flattenInstance(parentView, (*it), inFactory);
				}
				parentView->removeInstance(inInstance->getName());
			} else {
				return false;
			}
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
		break;
	}
	default: {
		throw Error(eMessageIdErrorUnsupoortedOperation, __FUNCTION__, __FILE__, __LINE__);
	}
	}
	return true;
}

}

namespace torc {
namespace generic {

template <typename _Pointer> std::string getModifiedName(const std::string& inInstName,
	const _Pointer& inNameable, const std::vector<size_t>& inIndicesVector) {
	return (*NameModifier<_Pointer>::instance())(inInstName, inNameable, inIndicesVector);
}

template <typename _Pointer> std::string getRenamedName(const InstanceSharedPtr& inParentInstance,
	const _Pointer& inRenamable, const std::vector<size_t>& inIndicesVector) {
	std::string sep = FlatteningHelpers::instance()->getSeparator();
	Renamable::Name originalName = inRenamable->getOriginalName();
	if(originalName.empty()) {
		originalName = inRenamable->getName();
	}
	std::string parentInstanceName = inParentInstance->getOriginalName();
	if(parentInstanceName.empty()) {
		parentInstanceName = inParentInstance->getName();
	}
	if(!inIndicesVector.empty()) {
		parentInstanceName += "[ ";
		std::ostringstream sout;
		copy(inIndicesVector.begin(), inIndicesVector.end(),
			std::ostream_iterator < size_t > (sout, " "));
		parentInstanceName += sout.str();
		parentInstanceName += "]";
	}
	std::ostringstream sout;
	sout << ((parentInstanceName.find_first_of(sep)) ? sep : "") << parentInstanceName << sep
		<< originalName;
	return sout.str();
}

void flatten(const InstanceSharedPtr& inInstance, const ObjectFactorySharedPtr& inFactory,
	bool inRecursive, const std::string& inSeparator,
	const InstanceRenamingFunction& inInstanceRenameFunc,
	const NetRenamingFunction& inNetRenameFunc, const InstanceNamingFunction& inInstanceNameFunc,
	const NetNamingFunction& inNetNameFunc) throw (Error) {
	FlatteningHelpers *pHelper = FlatteningHelpers::instance();
	pHelper->setSeparator(inSeparator);
	pHelper->setInstanceRenamingFunction(inInstanceRenameFunc);
	pHelper->setNetRenamingFunction(inNetRenameFunc);
	pHelper->setInstanceNamingFunction(inInstanceNameFunc);
	pHelper->setNetNamingFunction(inNetNameFunc);
	if(inRecursive) {
		std::list<InstanceSharedPtr> newInstances;
		if(!flatten_impl(inInstance, inFactory, newInstances)) {
			return;
		}
		while(!newInstances.empty()) {
			std::list<InstanceSharedPtr>::iterator top = newInstances.begin();
			InstanceSharedPtr inst = *top;
			newInstances.erase(top);
			flatten_impl(inst, inFactory, newInstances);
		}
	} else {
		std::list < InstanceSharedPtr > dummy;
		flatten_impl(inInstance, inFactory, dummy);
	}
	pHelper->reset();
	return;
}

void flatten(const ViewSharedPtr& inView, const ObjectFactorySharedPtr& inFactory, bool inRecursive,
	const std::string& inSeparator, const InstanceRenamingFunction& inInstanceRenameFunc,
	const NetRenamingFunction& inNetRenameFunc, const InstanceNamingFunction& inInstanceNameFunc,
	const NetNamingFunction& inNetNameFunc) throw (Error) {
	if(!inView || !inFactory) {
		return;
	}
	std::vector<InstanceSharedPtr> outInstances;
	inView->getInstances(outInstances);
	std::vector<InstanceSharedPtr>::iterator instIt = outInstances.begin();
	for(; instIt != outInstances.end(); instIt++) {
		InstanceSharedPtr inst = *instIt;
		if(inst) {
			flatten(inst, inFactory, inRecursive, inSeparator, inInstanceRenameFunc,
				inNetRenameFunc, inInstanceNameFunc, inNetNameFunc);
		}
	}
	return;
}

void flatten(const DesignSharedPtr& inDesign, const ObjectFactorySharedPtr& inFactory,
	bool inRecursive, const std::string& inSeparator,
	const InstanceRenamingFunction& inInstanceRenameFunc,
	const NetRenamingFunction& inNetRenameFunc, const InstanceNamingFunction& inInstanceNameFunc,
	const NetNamingFunction& inNetNameFunc) throw (Error) {
	if(!inDesign || !inFactory) {
		return;
	}
	std::string cellName = inDesign->getCellRefName();
	std::string libName = inDesign->getLibraryRefName();
	RootSharedPtr root = inDesign->getParent();
	if(!root) {
		Error e(eMessageIdErrorPointerToItemDoesNotExist, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Pointer to Root", root);
		throw e;
	}
	LibrarySharedPtr library = root->findLibrary(libName);
	if(!library) {
		Error e(eMessageIdErrorPointerToItemDoesNotExist, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Pointer to Library", library);
		throw e;
	}
	CellSharedPtr cell = library->findCell(cellName);
	if(!cell) {
		Error e(eMessageIdErrorPointerToItemDoesNotExist, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Pointer to Cell", cell);
		throw e;
	}

	std::vector<ViewSharedPtr> outViews;
	cell->getViews(outViews);
	std::vector<ViewSharedPtr>::iterator viewIt = outViews.begin();
	for(; viewIt != outViews.end(); viewIt++) {
		ViewSharedPtr view = *viewIt;
		if(View::eTypeNetlist != view->getType()) {
			continue;
		}
		flatten(view, inFactory, inRecursive, inSeparator, inInstanceRenameFunc, inNetRenameFunc,
			inInstanceNameFunc, inNetNameFunc);
	}
}

template std::string getModifiedName(const std::string& inInstName, const NetSharedPtr& inNameable,
	const std::vector<size_t>& inIndicesVector);

template std::string getModifiedName(const std::string& inInstName,
	const InstanceSharedPtr& inNameable, const std::vector<size_t>& inIndicesVector);

template std::string getRenamedName(const InstanceSharedPtr& inParentInstance,
	const NetSharedPtr& inRenamable, const std::vector<size_t>& inIndicesVector);

template std::string getRenamedName(const InstanceSharedPtr& inParentInstance,
	const InstanceSharedPtr& inRenamable, const std::vector<size_t>& inIndicesVector);

} //namespace generic
} //namespace torc
