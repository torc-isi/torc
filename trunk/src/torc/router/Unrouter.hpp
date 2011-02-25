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

/// \file
/// \brief Header for the Tracer class.

#ifndef TORC_ROUTER_UNROUTER_HPP
#define TORC_ROUTER_UNROUTER_HPP

#include "torc/architecture/DDB.hpp"
#include "torc/router/RouteTreeNode.hpp"
#include <set>
#include <iostream>

#include "torc/architecture/OutputStreamHelpers.hpp"
#include "torc/common/TestHelpers.hpp"

namespace torc {
namespace router {

	/// \brief Unroutes connected resources in a DDB instance.
	/// \details The unrouter allows the user to deactivate pips and wires that are associated with
	///		one another as all or part of a net.
	class Unrouter {
	// types
		/// \brief Imported type name
		typedef architecture::DDB DDB;
		typedef architecture::ArcUsage ArcUsage;
		typedef architecture::WireUsage WireUsage;
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
		/// \brief WireUsage reference.
		WireUsage& mWireUsage;
		/// \brief Scratch segment storage.
		TilewireVector mSegmentBuf;
		/// \brief Scratch wire storage
		TilewireVector mSinksBuf;
		/// \brief Scratch wire storage
		TilewireVector mSourcesBuf;
		/// \brief Traced wires collection.
		std::set<Tilewire> mTracedWiresBuf;
		
		
		TilewireVector mWireQueue;
		
		enum { eTraceToSource = 0, eTraceToBranch = 1, eTraceFullNet = 2 };
	
	public:
	// constructor
		/// \brief Public Constructor
		Unrouter(DDB& inDB) : mDB(inDB), mArcUsage(inDB.getArcUsage()), 
			mWireUsage(inDB.getWireUsage()) {}
		/// \brief Destructor.
		~Unrouter() {}
		
		/// \brief 
		boost::int32_t unrouteToSinks(const Tilewire inTilewire) {
			//mTracedWiresBuf.clear();
			mWireQueue.clear();
			mWireQueue.push_back(inTilewire);
			Tilewire currentTilewire;
			boost::int32_t releasedArcs = 0;
			
			while (mWireQueue.size() != 0) {
				currentTilewire = mWireQueue.back();
				mWireQueue.pop_back();
				
				mSegmentBuf.clear();
				mDB.expandSegment(currentTilewire, mSegmentBuf);
				TilewireVector::iterator p;
				TilewireVector::iterator e = mSegmentBuf.end();
				for (p = mSegmentBuf.begin(); p < e; p++) {
					const Tilewire& segmentTilewire = *p;
					mSinksBuf.clear();
					mDB.expandTilewireSinks(segmentTilewire, mSinksBuf);
					TilewireVector::iterator q;
					TilewireVector::iterator f = mSinksBuf.end();
					for (q = mSinksBuf.begin(); q < f; q++) {
						const Tilewire& sinkTilewire = *q;
						if(mArcUsage.isUsed(segmentTilewire, sinkTilewire)) {
							mWireQueue.push_back(sinkTilewire);
							Arc arc(segmentTilewire, sinkTilewire);
							mDB.releaseArc(arc);
							releasedArcs++;
						}
					}
				}
			}
			return releasedArcs;
		}
		

		/// \brief Trace from given Tilewire in a sinkwards direction only.
		/// \details Traces from the specified source Tilewire in a sinkwards direction.
		///		If the Tilewire is a logic site	output, then the result will be a full net.
		RouteTreeNode* traceToSinks(Tilewire inTilewire) {
			mTracedWiresBuf.clear();
			RouteTreeNode* node = new RouteTreeNode(inTilewire, inTilewire, 0, 0);
			traceDownstream(node);
			// returned node is a dummy node
			return node;
		}
		/// \brief Trace from given Tilewire sinkwards and sourcewards to source or branch.
		/// \details Traces from the specified source Tilewire.  All downstream sinks are found
		///		and the closest branch point in the net or source is found.
		RouteTreeNode* traceBranch(Tilewire inTilewire) {
			mTracedWiresBuf.clear();
			RouteTreeNode* node = new RouteTreeNode(inTilewire, inTilewire, 0, 0);
			traceUpstream(node, eTraceToBranch);
			// returned node is a dummy node
			return node;
		}
		/// \brief Trace from given Tilewire sourcewards to the source of the net.
		/// \details Traces from the specified source Tilewire.  All downstream sinks are found
		///		and the source of the net is found.  Branch points are ignored.
		RouteTreeNode* traceToSource(Tilewire inTilewire) {
			mTracedWiresBuf.clear();
			RouteTreeNode* node = new RouteTreeNode(inTilewire, inTilewire, 0, 0);
			traceUpstream(node, eTraceToSource);
			// returned node is a dummy node
			return node;
		}
		/// \brief Trace from given Tilewire and recover the entire net.
		/// \details Traces from the specified Tilewire.  All net sinks and the source are
		///		recovered fully reconstructing the net to which the Tilewire belongs.
		RouteTreeNode* traceFull(Tilewire inTilewire) {
			mTracedWiresBuf.clear();
			RouteTreeNode* node = new RouteTreeNode(inTilewire, inTilewire, 0, 0);
			traceUpstream(node, eTraceFullNet);
			// returned node is a dummy node
			return node;
		}
		/// \brief Remove the dummy node if possible.
		/// \details Removes the dummy node if it is at an end of the trace.
		/// 	If it is in the middle, it removes it if there is only one child.
		/// 	Otherwise no change is made.
		/*RouteTreeNode* removeDummy(RouteTreeNode* inNode) {
			std::cout << "THIS FUNCTION MAY BE CUT AND IS INCOMPELETE" << std::endl;
			RouteTreeNode* node;
			if (inNode->getParent() == 0) { // node has no parent
				if (inNode->getNumChildren() == 0) { // Single node, no trace
					return inNode;
				} else if (inNode->getNumChildren() == 1) { // Source
					node = inNode->getChild(0);
					// remove parent of node
					delete inNode;
					return node;
				} else { // multiple children of node with no parents
					// no modification
					return inNode;
				}
			} else { // node has a parent
				if (inNode->getNumChildren() == 0) { // Sink
					node = (RouteTreeNode*)inNode->getParent();
					// remove child of node
					delete inNode;
					return node;
				} else if (inNode->getNumChildren() == 1) { // Middle with one on either end
					// move node to parent
					node = (RouteTreeNode*)(inNode->getParent());
					// remove child of node
					// add child of child to node
					// set child to new parent
					delete inNode;
					return node;
				} else {
					return inNode;
				}
			}
		}*/
		/// \brief Find the source of net that owns the given Tilewire.
		/*Tilewire findSource(Tilewire inTilewire) {
			Arc parentArc; 
			bool foundParent = false;
			mSegmentBuf.clear();
			mDB.expandSegment(inTilewire, mSegmentBuf);
			TilewireVector::iterator p;
			TilewireVector::iterator e = mSegmentBuf.end();
			for (p = mSegmentBuf.begin(); p < e; p++) {
				Tilewire segmentTilewire = *p;
				mTracedWiresBuf.insert(segmentTilewire);
				mSourcesBuf.clear();
				mDB.expandTilewireSources(segmentTilewire, mSourcesBuf);
				TilewireVector::iterator q;
				TilewireVector::iterator f = mSourcesBuf.end();
				for (q = mSourcesBuf.begin(); q < f; q++) {
					Tilewire sourceTilewire = *q;
					if (mTracedWiresBuf.count(sourceTilewire) == 1) {
						std::cout << "TRACER ERROR: REVISITING SOURCE, POSSIBLE LOOP" << std::endl;
						exit(1);
					}
					if (mArcUsage.isUsed(sourceTilewire, segmentTilewire)) {
						if (foundParent) {
							std::cout << "TRACER ERROR: MULTIPLE PARENTS" << std::endl;
							exit(1);
						}
						parentArc = Arc(sourceTilewire, segmentTilewire);
						foundParent = true;
					}
				}
			}
			if (foundParent) {
				return findSource(parentArc.getSourceTilewire());
			} else {
				return inTilewire;
			}
		}
		/// \brief Find all sinks downstream from given Tilewire.
		void findBranchSinks(Tilewire inTilewire, TilewireVector& outSinks) {
			std::cout << "findBranchSinks not yet implemented." << std::endl;
			exit(1);
		}
		/// \brief Find all sinks attached to the net that includes input Tilewire.
		void findSinks(Tilewire inTilewire, TilewireVector& outSinks) {
			std::cout << "findSinks not yet implemented." << std::endl;
			exit(1);
		}*/

	protected:
		/// \brief Recursively traces from the specified RouteTreeNode.
		void traceDownstream(RouteTreeNode* inNode) {
			const Tilewire& nodeTilewire = inNode->getSinkTilewire();
			std::vector<RouteTreeNode*> activeSinks;

			mSegmentBuf.clear();
			mDB.expandSegment(nodeTilewire, mSegmentBuf);
			TilewireVector::iterator p;
			TilewireVector::iterator e = mSegmentBuf.end();
			for (p = mSegmentBuf.begin(); p < e; p++) {
				const Tilewire& segmentTilewire = *p;
				mTracedWiresBuf.insert(segmentTilewire);
				mSinksBuf.clear();
				mDB.expandTilewireSinks(segmentTilewire, mSinksBuf);
				TilewireVector::iterator q;
				TilewireVector::iterator f = mSinksBuf.end();
				for (q = mSinksBuf.begin(); q < f; q++) {
					const Tilewire& sinkTilewire = *q;
					if(mTracedWiresBuf.count(sinkTilewire) == 1) continue;
					if(mArcUsage.isUsed(segmentTilewire, sinkTilewire)) {
						activeSinks.push_back(new RouteTreeNode(
							segmentTilewire, sinkTilewire, 0, inNode));
					}
				}
			}
			unsigned int activeSinksSize = activeSinks.size();
			if(activeSinksSize == 0) return;

			inNode->addChildren(activeSinks);
			for(unsigned int i = 0; i < activeSinksSize; i++) {
				RouteTreeNode* childNode = inNode->getChild(i);
				traceDownstream(childNode);
			}
		}
		/// \brief Recursively traces from the specified RouteTreeNode in one of three modes.
		void traceUpstream(RouteTreeNode* inNode, boost::int32_t inMode) {
//std::cout << mDB << "UP CALL " << inNode->getArc() << std::endl;
			const Tilewire& nodeSourceTilewire = inNode->getSourceTilewire();
			const Tilewire& nodeSinkTilewire = inNode->getSinkTilewire();
			Arc parentArc; 
			bool foundParent = false;
			std::vector<RouteTreeNode*> activeSinks;
			mSegmentBuf.clear();
			mDB.expandSegment(nodeSourceTilewire, mSegmentBuf);
			TilewireVector::iterator p;
			TilewireVector::iterator e = mSegmentBuf.end();
			for (p = mSegmentBuf.begin(); p != e; p++) {
				const Tilewire& segmentTilewire = *p;
				mTracedWiresBuf.insert(segmentTilewire);
				mSourcesBuf.clear();
				mDB.expandTilewireSources(segmentTilewire, mSourcesBuf);
				TilewireVector::iterator q;
				TilewireVector::iterator f = mSourcesBuf.end();
				for (q = mSourcesBuf.begin(); q != f; q++) {
					const Tilewire& sourceTilewire = *q;
					if (mTracedWiresBuf.count(sourceTilewire) == 1) {
						std::cout << "ALREADY SEEN THIS TILEWIRE" << std::endl;
						continue;
					}
					if (mArcUsage.isUsed(sourceTilewire, segmentTilewire)) {
						if (foundParent) {
							std::cout << "TRACER ERROR: MULTIPLE PARENTS" << std::endl;
							exit(1);
						}
						parentArc = Arc(sourceTilewire, segmentTilewire);
						foundParent = true;
					}
				}
			}
			// now look at the sinks unless the caller is only interested in top level source
			if (inMode != eTraceToSource) {
				mSegmentBuf.clear();
				mDB.expandSegment(nodeSinkTilewire, mSegmentBuf);
				e = mSegmentBuf.end();
				for (p = mSegmentBuf.begin(); p < e; p++) {
					Tilewire segmentTilewire = *p;
					mTracedWiresBuf.insert(segmentTilewire);
					mSinksBuf.clear();
					mDB.expandTilewireSinks(segmentTilewire, mSinksBuf);
					TilewireVector::iterator q;
					TilewireVector::iterator f = mSinksBuf.end();
					for (q = mSinksBuf.begin(); q < f; q++) {
						Tilewire sinkTilewire = *q;
						if (mTracedWiresBuf.count(sinkTilewire) == 1) {
							std::cout << "ALREADY SEEN THIS TILEWIRE" << std::endl;
							continue;
						}
						if (mArcUsage.isUsed(segmentTilewire, sinkTilewire)) {
							activeSinks.push_back(new RouteTreeNode(
								segmentTilewire, sinkTilewire, 0, inNode));
						}
					}
				}
				unsigned int activeSinksSize = activeSinks.size();
				std::cout << "ACTIVE SINKS SIZE: " << activeSinksSize << std::endl;
				std::cout << inMode << " " << eTraceToBranch << std::endl;
				if (inMode == eTraceToBranch && activeSinksSize > 0) return;
				if (activeSinksSize > 1) {
					inNode->addChildren(activeSinks);
					activeSinksSize = inNode->getNumChildren(); //WHY??
					std::cout << activeSinksSize << std::endl;
					for (unsigned int i = 0; i < activeSinksSize; i++) {
						RouteTreeNode* newChildNode = inNode->getChild(i);
						traceDownstream(newChildNode);
					}
				}
			}
			
			if (foundParent) {
//std::cout << mDB << "\tFOUND PARENT " << parentArc << std::endl;
				inNode->makeParent(parentArc.getSourceTilewire(), parentArc.getSinkTilewire());
				RouteTreeNode* parentNode = (RouteTreeNode*)inNode->getParent();
				traceUpstream(parentNode, inMode);
			}
		}

/*		


	public:
		bool hasSource(CTraceNode* childNode)
		{

			// get the child's tilewire
			CTilewire tilewire = childNode->getTilewire();

			// expand the given wire into its full segment
			mWiresBuf.clear();
			mDB.expandSegmentSources(mWiresBuf, tilewire);

			// loop through each wire in the segment and look for arcs
			for(uint32 i = 0; i < mWiresBuf.size(); i++)
			{
				// get the wire
				CTilewire currentTilewire = mWiresBuf[i];
				// look for this wire's sources
				mSourcesBuf.clear();
				mDB.expandWireSources(mSourcesBuf, currentTilewire);
				// loop through the source wires
				for(uint32 j = 0; j < mSourcesBuf.size(); j++)
				{
					// get the source information
					CTilewire sourceTilewire = mSourcesBuf[j];
					// ensure that the source and sink wires live in the same tile,
					// even though we're really not expecting any surprises
					if(currentTilewire.getTileIndex() != sourceTilewire.getTileIndex())
					{
						std::cerr << "Oh yeah, it's pretty bad, the tile indices are not the same!" << std::endl;
					}
					// check to see if the arc is turned on
					if(mDB.arcIsOn(sourceTilewire, currentTilewire))
					{
						return true;
					}
				}
			}

			// if we got to here, no sources were found
			return false;

		}
*/

	}; // class Tracer


} // namespace router
} // namespace torc

#endif // TORC_ROUTER_UNROUTER_HPP
