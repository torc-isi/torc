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

/// \file
/// \brief Header for the Trace class.

#ifndef TORC_ROUTER_TRACE_HPP
#define TORC_ROUTER_TRACE_HPP

#include "torc/architecture/DDB.hpp"
#include "torc/router/TraceNode.hpp"
#include <set>
#include <iostream>

#include "torc/architecture/OutputStreamHelpers.hpp"
#include "torc/common/TestHelpers.hpp"

namespace torc {
namespace router {

	/// \brief Provides path extraction from usage information in a DDB instance..
	/// \details The tracer provides functions that recover a set of connected routing resources
	///		to build up complete or partial nets from the device usage information.
	class Trace {
	// types
		/// \brief Imported type name
		typedef architecture::DDB DDB;
		typedef architecture::ArcUsage ArcUsage;
		typedef architecture::Tilewire Tilewire;
		typedef architecture::Arc Arc;
		typedef architecture::TilewireVector TilewireVector;
		typedef architecture::ArcVector ArcVector;
	protected:
	// members
		/// \brief Database reference.
		DDB& mDB;
		/// \brief ArcUsage reference.
		ArcUsage& mArcUsage;
		
		/// \brief TraceNode representing the starting point for this trace.
		TraceNode* mInitialNode;
		/// \brief Vector of net sink nodes.
		TraceNodePtrVector mSinks;
		/// \brief Vector of net source nodes.
		TraceNodePtrVector mSources;
		/// \brief Vector of net branch nodes.
		TraceNodePtrVector mBranchPoints;
		
		/// \brief Vector of all TraceNode pointers.
		TraceNodePtrVector mAllNodes;
		/// \brief Map of Tilewires to owning TraceNode.
		std::map<Tilewire, TraceNode*> mTilewireToTraceNode;
		/// \brief Map of TraceNode pointers to Arc that connects them
		std::map<TraceNode*, std::map<TraceNode*, Arc> > mTraceNodesToArc;
		/// \brief Vector of all arcs, populated on a getArcs call.
		ArcVector mArcVector;
		
	public:
		/// \brief Enumeration for Trace modes.
		enum ETraceMode { eTraceFullNet = 0, eTraceToSinks, eTraceToBranch, 
			eTraceToSources, eTraceSinglePath };
	
	public:
	// constructor
		/// \brief Public Constructor
		Trace(DDB& inDB, Tilewire inTilewire, ETraceMode inTraceMode = eTraceFullNet)
			: mDB(inDB), mArcUsage(inDB.getArcUsage()) {
			mInitialNode = createNode(inTilewire);
			switch (inTraceMode) {
				case eTraceFullNet:
				case eTraceToSinks:
				case eTraceToBranch:
				case eTraceToSources:
				case eTraceSinglePath:
					break;
				default:
					std::cout << "Invalid Trace mode setting." << std::endl;
					return;
			}
			traceWorker(mInitialNode, inTraceMode);
		}
		/// \brief Destructor.
		~Trace() {
			std::cout << "Destroying Trace!" << std::endl;
			TraceNodePtrVector::iterator p;
			TraceNodePtrVector::iterator e = mAllNodes.end();
			for (p = mAllNodes.begin(); p != e; p++) {
				delete *p;
			}
			mAllNodes.clear();
			mSinks.clear();
			mSources.clear();
			mBranchPoints.clear();
			//delete mInitialNode;
		}
		/// \brief Get trace sink nodes.
		TraceNodePtrVector& getSinks() {
			return mSinks;
		}
		/// \brief Get trace source nodes.
		TraceNodePtrVector& getSources() {
			return mSources;
		}
		/// \brief Get trace branch point nodes.
		TraceNodePtrVector& getBranchPoints() {
			return mBranchPoints;
		}
		/// \brief Get all Arcs found during the trace.
		ArcVector& getArcs() {
			mArcVector.clear();
			std::map<TraceNode*, std::map<TraceNode*, Arc> >::iterator outer_p;
			std::map<TraceNode*, std::map<TraceNode*, Arc> >::iterator outer_end;
			std::map<TraceNode*, Arc>::iterator inner_p;
			std::map<TraceNode*, Arc>::iterator inner_end;
			outer_end = mTraceNodesToArc.end();
			for (outer_p = mTraceNodesToArc.begin(); outer_p != outer_end; outer_p++) {
				std::map<TraceNode*, Arc> innermap = outer_p->second;
				inner_end = innermap.end();
				for (inner_p = innermap.begin(); inner_p != inner_end; inner_p++) {
					mArcVector.push_back(inner_p->second);
					//std::cout << mArcVector.back() << std::endl;
				}
			}
			//std::cout << mArcVector.size() << std::endl;
			return mArcVector;
		}
	protected:
		/// \brief Recursively traces from the specified TraceNode in the specified mode.
		void traceWorker(TraceNode* inNode, ETraceMode inMode) {
			TraceNodePtrVector activeArcs;
			
			//std::cout << "\t## traceWorker ## " << inNode->getTilewire() << std::endl;
			
			ArcVector outArcs;
			ArcVector inArcs;
			TraceNodePtrVector activeSinks;
			TraceNodePtrVector activeSources;
			Tilewire nodeTilewire = inNode->getTilewire();
			
			unsigned int childCount = 0;
			unsigned int parentCount = 0;
			mDB.expandSegmentSinks(nodeTilewire, outArcs);
			for (unsigned int i = 0 ; i < outArcs.size(); i++) {
				if (mArcUsage.isUsed(outArcs[i]))
					childCount++;
			}
			mDB.expandSegmentSources(nodeTilewire, inArcs);
			for (unsigned int i = 0; i < inArcs.size(); i++) {
				if (mArcUsage.isUsed(inArcs[i]))
					parentCount++;
			}
			bool isBranch;
			if (childCount == 0) {
				mSinks.push_back(inNode);
				std::cout << "FOUND A SINK NODE: " << nodeTilewire << std::endl;
			}
			if (parentCount == 0) {
				mSources.push_back(inNode);
				std::cout << "FOUND A SOURCE NODE: " << nodeTilewire << std::endl;
			}
			if (childCount > 1 || parentCount > 1) {
				mBranchPoints.push_back(inNode);
				std::cout << "FOUND A BRANCH NODE: " << nodeTilewire
					<< " children: " << childCount << " parents: " << parentCount << std::endl;
				isBranch = true;
			} else {
				isBranch = false;
			}
			if (childCount == 1 && parentCount == 1) {
				//std::cout << "NODE: " << inNode->getTilewire() << std::endl;
			}
			
			// mode selection
			bool traceSinks = inMode != eTraceToSources;
			bool traceSources = inMode != eTraceToSinks;
			if (inMode == eTraceToBranch || inMode == eTraceSinglePath) {
				traceSinks = !isBranch;
			}
			if (inMode == eTraceToBranch || inMode == eTraceSinglePath) {
				traceSources = !isBranch;
			}
			
			// node creation
			if (traceSinks) { // only trace to sources doesn't look at sinks
				for (unsigned int i = 0; i < outArcs.size(); i++) {
					const Tilewire& sinkTilewire = outArcs[i].getSinkTilewire();
					// if the arc is used we need to check this out
					if (mArcUsage.isUsed(outArcs[i])) {
						//std::cout << "\tOUT ARC USED " << outArcs[i] << std::endl;
						// get the node
						TraceNode* arcNode = getNode(sinkTilewire);
						if (arcNode == 0) {
							// node doesn't exist, create it
							arcNode = createNode(sinkTilewire);
							// store it for traversal
							activeSinks.push_back(arcNode);
						}
						if (findArc(inNode, arcNode) == Arc()) {
							// arc does not exist so add it
							inNode->addChild(arcNode);
							arcNode->addParent(inNode);
						} else {
							// arc already traversed, don't do it again
							continue;
						}
						// record this arc in the trace
						mTraceNodesToArc[inNode][arcNode] = outArcs[i];
					}
				}
			}
			if (traceSources) { // only trace to sinks doesn't look at sources
				for (unsigned int i = 0; i < inArcs.size(); i++) {
					const Tilewire& sourceTilewire = inArcs[i].getSourceTilewire();
					// if the arc is used we need to check this out
					if (mArcUsage.isUsed(inArcs[i])) {
						//std::cout << "\tIN ARC USED " << inArcs[i] << std::endl;
						// get the node
						TraceNode* arcNode = getNode(sourceTilewire);
						if (arcNode == 0) {
							// node doesn't exist, create it
							arcNode = createNode(sourceTilewire);
							// store it for traversal
							activeSources.push_back(arcNode);
						}
						if (findArc(arcNode, inNode) == Arc()) {
							// arc does not exist so add it
							arcNode->addChild(inNode);
							inNode->addParent(arcNode);
						} else {
							// arc already traversed, don't do it again
							continue;
						}
						// record this arc in the trace
						mTraceNodesToArc[arcNode][inNode] = inArcs[i];
					}
				}
			}
			
			// recursive calls
			if (traceSinks) {
				for (unsigned int i = 0; i < activeSinks.size(); i++) {
					if (inMode == eTraceToBranch)
						traceWorker(activeSinks[i], eTraceToSinks);
					else
						traceWorker(activeSinks[i], inMode);
				}
			}
			if (traceSources) { // only trace to sinks doesn't look at sources
				for (unsigned int i = 0; i < activeSources.size(); i++) {
					traceWorker(activeSources[i], inMode);
				}
			}
		}
		
		
	// functions
		/// \brief Normalize depth of nodes.
		void normalizeDepth(TraceNode* inNode) {
			TraceNode::TraceNodePtrVector parents;
			TraceNode::TraceNodePtrList wavefront;
			TraceNode* node;
			std::set<Tilewire> nodesVisited;
			
			// not needed anymore since we save sources
			//findTop(parents, inNode);
			
			for (unsigned int i = 0; i < parents.size(); i++) {
				parents[i]->setDepth(0);
				std::cout << "PARENT " << parents[i]->getTilewire() << std::endl;
				nodesVisited.insert(parents[i]->getTilewire());
				for (unsigned int j = 0; j < parents[i]->getNumChildren(); j++) {
					wavefront.push_back(parents[i]->getChild(j));
				}
			}
			while (wavefront.size() != 0) {
				node = wavefront.front();
				wavefront.pop_front();
				
				Tilewire tw = node->getTilewire();
				if (nodesVisited.find(tw) == nodesVisited.end()) {
					nodesVisited.insert(node->getTilewire());
				} else {
					std::cout << "Already visited " << tw << std::endl;
				}
				
				if (node->getNumParents() == 0) {
					std::cout << "SOURCE NODE" << std::endl;
					node->setDepth(0);
				} else {
					for (unsigned int i = 0; i < node->getNumParents(); i++) {
						std::cout << "PARENT DEPTH " << node->getParent(i)->getDepth() << std::endl;
						if (node->getParent(i)->getDepth() + 1 > node->getDepth()) {
							node->setDepth(node->getParent(i)->getDepth() + 1);
						}
					}
				}
				if (node->getNumChildren() == 0) {
					std::cout << "SINK NODE" << std::endl;
				}
				for (unsigned int i = 0; i < node->getNumChildren(); i++) {
						wavefront.push_back(node->getChild(i));
				}
			}
			
		}
		/// \brief Create a TraceNode and update auxiliary structures.
		TraceNode* createNode(Tilewire inTilewire) {
			std::map<Tilewire, TraceNode*>::iterator it;
			it = mTilewireToTraceNode.find(inTilewire);
			if (it != mTilewireToTraceNode.end()) {
				return 0;
			}
			TilewireVector segmentTilewires;
			mDB.expandSegment(inTilewire, segmentTilewires);
			TraceNode* newNode = new TraceNode(inTilewire);
			mAllNodes.push_back(newNode);
			for (unsigned int i = 0; i < segmentTilewires.size(); i++) {
				mTilewireToTraceNode.insert(
					std::pair<Tilewire, TraceNode*>(segmentTilewires[i], newNode));
			}
			return newNode;
		}
		/// \brief Get a TraceNode based on its owning Tilewire.
		TraceNode* getNode(Tilewire inTilewire) {
			std::map<Tilewire, TraceNode*>::iterator it;
			it = mTilewireToTraceNode.find(inTilewire);
			if (it == mTilewireToTraceNode.end()) {
				return 0;
			} else {
				return it->second;
			}
		}
		/// \brief Find a traced Arc from the nodes that it connects
		Arc findArc(TraceNode* source, TraceNode* sink) {
			// returns the Arc connecting the two nodes if the arc exists.
			std::map<TraceNode*, std::map<TraceNode*, Arc> >::iterator outer_p;
			std::map<TraceNode*, std::map<TraceNode*, Arc> >::iterator outer_end;
			std::map<TraceNode*, Arc>::iterator inner_p;
			std::map<TraceNode*, Arc>::iterator inner_end;
			outer_end = mTraceNodesToArc.end();
			outer_p = mTraceNodesToArc.find(source);
			if (outer_p != outer_end) {
				std::map<TraceNode*, Arc> innermap = outer_p->second;
				inner_end = innermap.end();
				inner_p = innermap.find(sink);
				if (inner_p != inner_end) {
					return inner_p->second;
				}
			}
			return Arc();
		}
	}; // class Trace
} // namespace router
} // namespace torc

#endif // TORC_ROUTER_TRACE_HPP
