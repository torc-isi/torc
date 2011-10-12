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
/// \brief Header for the NetRouter class.

#ifndef TORC_ROUTER_NETROUTER_HPP
#define TORC_ROUTER_NETROUTER_HPP

#include "torc/router/NetRouterBase.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/router/RouteNode.hpp"
#include "torc/router/RouteNet.hpp"
#include "torc/router/NetRouterHeuristicBase.hpp"
#include "torc/architecture/OutputStreamHelpers.hpp"
#include <set>
#include <iostream>
#include <algorithm>
#include <queue>
#include <boost/cstdint.hpp>
#include <boost/timer.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/functional/hash.hpp>

namespace torc {
namespace router {
	/// \brief Provides net routing based on the Nilsson graphsearch algorithm.
	/// \details The router can either return a vector of nodes or directly populate DDB usage.
	class NetRouter : public NetRouterBase {
	// types
		/// \brief Imported type names
		typedef architecture::DDB DDB;
		typedef architecture::ArcUsage ArcUsage;
		typedef architecture::WireUsage WireUsage;
		typedef architecture::Tilewire Tilewire;
		typedef architecture::Arc Arc;
		typedef architecture::TilewireVector TilewireVector;
		typedef architecture::ArcVector ArcVector;
		
		typedef std::priority_queue<RouteNode*, std::vector<RouteNode*>, 
			RouteNodePtrCostCompare> RouteNodePtrQueue;
		typedef boost::unordered_map<Tilewire, RouteNode*, 
			boost::hash<architecture::Tilewire> > RouteNodePtrMap;
		typedef boost::unordered_set<Tilewire, boost::hash<architecture::Tilewire> > TilewireSet;
		
	protected:
	// members
		/// \brief Database reference.
		//DDB& mDB;
		/// \brief ArcUsage reference.
		ArcUsage& mArcUsage;
		/// \brief WireUsage reference.
		WireUsage& mWireUsage;
		
		/// \brief Pointer to the heuristic for making routing decisions.
		//NetRouterHeuristicBase* mHeuristic;
		
		/// \brief Priority queue of nodes to explore during routing, mirrors mOpen.
		RouteNodePtrQueue mOrder;
		/// \brief Collection of candidate nodes the router can expand.
		RouteNodePtrMap mOpen;
		/// \brief Collection of nodes that have been expanded.
		RouteNodePtrMap mClosed;
		/// \brief Collection of nodes skipped due to heuristic constraints.
		RouteNodePtrMap mSkipped;
		/// \brief Vector of nodes created during pre-routing.
		RouteNodePtrVector mPreRoute;
		/// \brief Set of Closed Tilewires to avoid issues with bidir resources.
		TilewireSet mAuxClosed;
		
		/// \brief Scratch segment storage.
		TilewireVector mSegmentBuf;
		/// \brief Scratch arc storage
		ArcVector mArcsBuf;
		
		/// \brief Maximum number of nodes to explore before failing.
		boost::uint32_t mSearchLimit;
		/// \brief Number of nets routed since construction.
		boost::uint32_t mStatNets;
		/// \brief Number of passes through the main loop since construction.
		boost::uint32_t mStatLoopPasses;
		/// \brief Number of nodes created from expansion.
		boost::uint32_t mStatExpanded;
		/// \brief Number of net sources seen since construction.
		boost::uint32_t mStatSources;
		/// \brief Number of net sinks seen since construction.
		boost::uint32_t mStatSinks;
		
	public:
	// constructor
		/// \brief Public Constructor
		NetRouter(DDB& inDB, NetRouterHeuristicBase* inHeuristic) : 
			NetRouterBase(inDB, inHeuristic), mArcUsage(inDB.getArcUsage()), 
			mWireUsage(inDB.getWireUsage()) {
			
			mSearchLimit = 0x001FFFFF;
			mStatNets = 0u;
			mStatLoopPasses = 0u;
			mStatExpanded = 0u;
			mStatSources = 0u;
			mStatSinks = 0u;

			mTotalRouteTime = 0;
		}
		/// \brief Destructor.
		~NetRouter() {}

		/// \brief Route a net.
		void routeNet(RouteNet& net) {
//std::cout << "### ROUTING " << net.getName() << " ###" << std::endl;
			if (!net.hasOneSource()) {
				std::cout << "NetRouter does not support multiple sources" << std::endl;
				throw;
			}
			Tilewire source = *net.sourcesBegin();
			TilewireVector sinks;
			TilewireVector::iterator p;
			TilewireVector::iterator e = net.sinksEnd();
			for (p = net.sinksBegin(); p != e; p++) {
				sinks.push_back(*p);
			}
			
			// test resources in use
			if (source == Tilewire::sInvalid) {
				std::cout << net.getName() << ": source is INVALID" << std::endl;
				return;
			}
			if (mWireUsage.isUsed(source)) {
				/// \todo Throw appropriate exception.
				std::cout << "!Source already in use! " << source << std::endl;
				throw;
			}
			e = sinks.end();
			for (p = sinks.begin(); p != e; p++) {
				if (*p == Tilewire::sInvalid) {
					std::cout << net.getName() << ": sink is INVALID" << std::endl;
					return;
				}
				if (mWireUsage.isUsed(*p)) {
					/// \todo Throw appropriate exception.
					std::cout << "!Sink already in use! " << *p << std::endl;
					throw;
				}
			}
			
			// pre-routing
			
			if (sinks.size() == 1 && testDedicatedPath(source, sinks[0], mPreRoute)) {
				RouteNodePtrVector::iterator p;
				RouteNodePtrVector::iterator e;
				e = mPreRoute.end();
				// transfer preroute to the net.
				for (p = mPreRoute.begin(); p != e; p++) {
					net.routeNodes().push_back(*p);
				}
				mPreRoute.clear();
			} else {
				if (!expandNetTerminals(source, sinks, mPreRoute)) {
					std::cout << "DISCARDED in expand function: " << net.getName() << std::endl;
					throw;
				}
			
				graphSearch(source, sinks, net.routeNodes());
			}
			
			mStatNets++;
			mStatSources++;
			mStatSinks += sinks.size();
		}
		void resetStats() {
			mStatNets = 0u;
			mStatLoopPasses = 0u;
			mStatExpanded = 0u;
			mStatSources = 0u;
			mStatSinks = 0u;
		}
		void printStats(std::ostream& out) {
			out << "Nets routed: " << mStatNets
				<< " Sources: " << mStatSources
				<< " Sinks : " << mStatSinks
				<< " Arcs expanded: " << mStatExpanded
				<< " Loop passes: " << mStatLoopPasses
				<< " Total routing time: " << mTotalRouteTime
				<< std::endl;
		}
	protected:
		/// \brief Internal top level route function.
		void graphSearch(const Tilewire& inSource, TilewireVector& inSinks,
			RouteNodePtrVector& outRoute) {
			
			clearContainers();
			
			mWireUsage.release(inSource);
			TilewireVector::const_iterator p;
			TilewireVector::const_iterator e = inSinks.end();
			for (p = inSinks.begin(); p != e; p++) {
				mWireUsage.release(*p);

			}
			mHeuristic->reorderSinks(inSource, inSinks);
			/// \todo Count new RouteNodes?
			RouteNode* s = new RouteNode(inSource, inSource, 0, 0, 0, 0);

			mOpen.insert(std::pair<Tilewire, RouteNode*>(inSource, s));
			mOrder.push(s);
			e = inSinks.end();
			for (p = inSinks.begin(); p != e; p++) {

				RouteNodePtrVector::iterator q;
				RouteNodePtrVector::iterator f = outRoute.end();
				for (q = outRoute.begin(); q != f; q++) {
					/// \todo Set initial cost
					mOpen.insert(std::pair<Tilewire, RouteNode*>((*q)->getSinkTilewire(), (*q)));
					mOrder.push(*q);
				}
				mHeuristic->setSink(*p);
				////////////////
				graphSearchLoop(inSource, *p, outRoute);
				////////////////
				f = outRoute.end();
				for (q = outRoute.begin(); q != f; q++) {
					Tilewire tw = (*q)->getSinkTilewire();
					mOpen.erase(tw);
					mClosed.erase(tw);
					mSkipped.erase(tw);
				}
				clearContainers();
			}
			
			mWireUsage.use(inSource);
			e = inSinks.end();
			for (p = inSinks.begin(); p != e; p++) {
				mWireUsage.use(*p);
			}
		}
		/// \brief Routing loop function.
		void graphSearchLoop(const Tilewire& inSource, const Tilewire& inSink, 
			RouteNodePtrVector& outRoute) {

			boost::uint32_t expanded = 0;
			boost::uint32_t processed = 0;
			while (true) {
				mStatLoopPasses++;
				if (mOrder.size() == 0) {
					std::cout << "ROUTING FAILED!" << std::endl;
					clearContainers();
					/// \todo Throw route failed exception.
					throw;
				}
				
				if (++processed > mSearchLimit) {
					std::cout << "ROUTE LIMIT!" << std::endl;
					/// \todo Throw route limited exception.
					throw;
				}
				RouteNode* n = mOrder.top();
				Tilewire key = n->getSinkTilewire();
				RouteNodePtrMap::iterator pk = mOpen.find(key);
				mClosed.insert(std::pair<Tilewire, RouteNode*>(key, n));
				mOpen.erase(pk);
				mOrder.pop();

//std::cout << key << "    " << mOrder.size() << std::endl;
				/// \todo adjust the node cost to be just the path cost?
				
				mSegmentBuf.clear();
				mDB.expandSegment(key, mSegmentBuf);
				TilewireVector::iterator p;
				TilewireVector::iterator e = mSegmentBuf.end();
				for (p = mSegmentBuf.begin(); p != e; p++) {
					if (*p == inSink) {
						recordPath(n, key, inSource, outRoute);
						mStatExpanded += expanded;
						return;
					}
					mAuxClosed.insert(*p);
				}
				
				mArcsBuf.clear();

				mHeuristic->expandSegmentSinks(key, mArcsBuf);
//std::cout << "\t" << mArcsBuf.size() << std::endl;
				ArcVector::iterator q;
				ArcVector::iterator f = mArcsBuf.end();
				for (q = mArcsBuf.begin(); q != f; q++) {
					expanded++;
					graphSearchFilter(n, *q, outRoute);
				}
			}
		}
		/// \brief Create a new node and put it into the appropriate structure
		void graphSearchFilter(RouteNode* inParent, const Arc& inArc, 
			RouteNodePtrVector& outRoute) {

			const Tilewire& arcSink = inArc.getSinkTilewire();
			/// \todo Need to check to see if we should be adjusting pointers in here.
			if (mWireUsage.isUsed(arcSink)) return; // check if resources used 
			if (mOpen.find(arcSink) != mOpen.end()) return;
			if (mClosed.find(arcSink) != mClosed.end()) return;
			if (mSkipped.find(arcSink) != mSkipped.end()) return;
			
			if (mAuxClosed.find(arcSink) != mAuxClosed.end()) {
//				std::cout << "FOUND SPECIAL CASE: " << arcSink << std::endl;
				return;
			}

			RouteNode* node = new RouteNode(inArc, 0, 0, inParent->getDepth() + 1, inParent);
			/// \todo Heuristic check if node is filtered
			/// \todo Heuristic cost of node
			mHeuristic->nodeCost(*node);
			/// \todo Cases for different places the node can go.
			mOpen.insert(std::pair<Tilewire, RouteNode*>(arcSink, node));
			mOrder.push(node);
		}
		/// \brief Add the newly found route to the outRoute vector
		void recordPath(RouteNode* node, Tilewire key, Tilewire inSource, 
			RouteNodePtrVector& outRoute) {
			
			RouteNodePtrVector::iterator p;
			RouteNodePtrVector::iterator e;
			e = mPreRoute.end();
			// transfer preroute to the outRoute;
			for (p = mPreRoute.begin(); p != e; p++) {
				outRoute.push_back(*p);
			}
			mPreRoute.clear();
			while (true) {
				node->setCost(0); // new wavefront?
				e = outRoute.end();
				for (p = outRoute.begin(); p != e; p++) {
					if ((*p)->getSinkTilewire() == node->getSinkTilewire()) return;
				}
				outRoute.push_back(node);
				if (key == inSource) return; // maybe a redundant check?
				node = node->getParent();
				if (node == 0) return;
				key = node->getSinkTilewire();
			}
		}
		/// \brief Delete nodes in containers and clear them.
		void clearContainers() {
			/// \todo Report statistics on nodes deleted?
			RouteNodePtrMap::iterator p;
			RouteNodePtrMap::iterator e = mOpen.end();
			for (p = mOpen.begin(); p != e; p++) { delete (*p).second; }
			e = mClosed.end();
			for (p = mClosed.begin(); p != e; p++) { delete (*p).second; }
			e = mSkipped.end();
			for (p = mSkipped.begin(); p != e; p++) { delete (*p).second; }
			while (!mOrder.empty()) { mOrder.pop(); }
			mOpen.clear();
			mClosed.clear();
			mSkipped.clear();
			mAuxClosed.clear();
		}
		/// \brief Move net terminals out into the INT tiles.
		bool expandNetTerminals(Tilewire& inSource, TilewireVector& inSinks, 
			RouteNodePtrVector& outRoute) {
			
			if (inSource == Tilewire::sInvalid) return false;
			inSource = expandSourceTerminal(inSource, outRoute);
			TilewireVector::iterator e = inSinks.end();
			for (TilewireVector::iterator p = inSinks.begin(); p != e; p++) {
				if (*p == Tilewire::sInvalid) return false;
				*p = expandSinkTerminal(*p, outRoute);
			}
			return true;
		}
		/// \brief Test for a dedicated routing path.
		bool testDedicatedPath(Tilewire inSource, Tilewire inSink, RouteNodePtrVector& outRoute) {
			ArcVector arcs;
			RouteNodePtrVector tempRoute;
			Tilewire tempsink = inSink;
			mDB.expandSegmentSources(inSink, arcs);
			while (arcs.size() == 1) {
				RouteNode* node = new RouteNode(arcs[0], 0, 0, 0, 0);
				tempRoute.push_back(node);
				mDB.useArc(arcs[0]);
				tempsink = arcs[0].getSourceTilewire();
				if (tempsink == inSource) {
//					std::cout << "FOUND DEDICATED ROUTE!!! " << inSource << " to " << inSink 
//						<< std::endl;
					for (unsigned int i = 0; i < tempRoute.size(); i++) {
						outRoute.push_back(tempRoute[i]);
					}
					return true;
				}
				arcs.clear();
				mDB.expandSegmentSources(tempsink, arcs);
			}
			return false;
		}
		/// \brief Move the source terminal of a net in the sinkwards direction.
		Tilewire expandSourceTerminal(Tilewire inSource, RouteNodePtrVector& outRoute) {
			ArcVector arcs;
			bool movingsource = false;
			Tilewire tempsource = inSource;
			mDB.expandSegmentSinks(inSource, arcs);
			while (arcs.size() == 1) {
				movingsource = true;
				RouteNode* node = new RouteNode(arcs[0], 0, 0, 0, 0);
				outRoute.push_back(node);
				mDB.useArc(arcs[0]);
				tempsource = arcs[0].getSinkTilewire();
				arcs.clear();
				mDB.expandSegmentSinks(tempsource, arcs);
			}
			return tempsource;
			//return arcs[0].getSourceTilewire();
		}
		/// \brief Move the sink terminal of a net in the sourcewards direction.
		Tilewire expandSinkTerminal(Tilewire inSink, RouteNodePtrVector& outRoute) {
			ArcVector arcs;
			bool movingsink = false;
			Tilewire tempsink = inSink;
			mDB.expandSegmentSources(inSink, arcs);
			while (arcs.size() == 1) {
				movingsink = true;
				RouteNode* node = new RouteNode(arcs[0], 0, 0, 0, 0);
				outRoute.push_back(node);
				mDB.useArc(arcs[0]);
				tempsink = arcs[0].getSourceTilewire();
				arcs.clear();
				mDB.expandSegmentSources(tempsink, arcs);
			}
			return arcs[0].getSinkTilewire();
		}
	}; // class NetRouter
} // namespace router
} // namespace torc

#endif // TORC_ROUTER_NETROUTER_HPP
