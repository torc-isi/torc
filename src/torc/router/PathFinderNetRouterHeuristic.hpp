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
/// \brief Header for the Heuristic class.

#ifndef TORC_ROUTER_PATHFINDERNETROUTERHEURISTIC_HPP
#define TORC_ROUTER_PATHFINDERNETROUTERHEURISTIC_HPP

#include "torc/architecture/DDB.hpp"
#include "torc/router/RouteNode.hpp"
#include "torc/router/NetRouterHeuristicBase.hpp"
#include "torc/router/PathFinder.hpp"
#include "torc/architecture/OutputStreamHelpers.hpp"
#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include <set>
#include <iostream>
#include <algorithm>
#include <queue>
#include <stdlib.h>
#include <boost/cstdint.hpp>
#include <boost/timer.hpp>
#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>
#include <boost/integer_traits.hpp>

namespace torc {
namespace router {

	/// \brief Provides net routing based on the Nillson graphsearch algorithm.
	/// \details The router can either return a vector of nodes or directly populate DDB usage.
	class PathFinderNetRouterHeuristic : public NetRouterHeuristicBase {
	// types
		/// \brief Imported type names
		typedef architecture::DDB DDB;
		typedef architecture::Tiles Tiles;
		typedef architecture::ArcUsage ArcUsage;
		typedef architecture::WireUsage WireUsage;
		typedef architecture::Tilewire Tilewire;
		typedef architecture::TileInfo TileInfo;
		typedef architecture::Arc Arc;
		typedef architecture::TilewireVector TilewireVector;
		typedef architecture::ArcVector ArcVector;
		typedef architecture::xilinx::TileRow TileRow;
		typedef architecture::xilinx::TileCol TileCol;
		
		typedef boost::unordered_map<Tilewire, TilewireData> PathFinderSharingMap;
				
	protected:
	// members
		/// \brief ArcUsage reference.
		ArcUsage& mArcUsage;
		/// \brief Tiles reference.
		const Tiles& mTiles;
		/// \brief Target sink tilewire.
		Tilewire mTargetSink;		
		/// \brief Target sink tile information.
		const TileInfo* mSinkTileInfo;
		/// \brief Target row coordinate
		boost::int32_t mRow;
		/// \brief Target column coordinate
		boost::int32_t mCol;
		
		/// \brief Segment tilewire buffer.
		TilewireVector mSegmentBuf;
		/// \brief Arc buffer.
		ArcVector mArcsBuf;
		
		/// \brief PathFinder sharing information.
		PathFinderSharingMap* mConflictMap;
		
		
	public:
	// constructor
		/// \brief Public Constructor
		PathFinderNetRouterHeuristic(DDB& inDB) : NetRouterHeuristicBase(inDB), 
			mArcUsage(inDB.getArcUsage()), mTiles(inDB.getTiles()) {}
		/// \brief Destructor.
		~PathFinderNetRouterHeuristic() {}

		void processParameters() {
			boost::any map = getParameter(0);
			mConflictMap = boost::any_cast<PathFinderSharingMap*>(map);
			std::cout<< "SETUP CONFLICT MAP" << std::endl;
		}
		
		/// \brief Set the current routing target
		void setSink(const Tilewire& inSink) {
			mSinkTileInfo = &mTiles.getTileInfo(inSink.getTileIndex());
			mTargetSink = inSink;
			mRow = mSinkTileInfo->getRow();
			mCol = mSinkTileInfo->getCol();
		}
		/// \brief Calculate the node cost based on distance to the sink and path length
		void nodeCost(RouteNode& inNode) {
			Tilewire sinkTilewire = inNode.getSinkTilewire();
			boost::int32_t bestDistance = boost::integer_traits<boost::int32_t>::max();
			boost::int32_t distance = boost::integer_traits<boost::int32_t>::max();
			boost::int32_t cost = 0;
			if (sinkTilewire == mTargetSink) {
				inNode.setCost(0);
				return;
			}
			mArcsBuf.clear();
			mDB.expandSegmentSinks(sinkTilewire, mArcsBuf);
			if (mArcsBuf.size() == 0) {
				inNode.setCost(bestDistance);
				return;
			}
			ArcVector::iterator p;
			ArcVector::iterator e = mArcsBuf.end();
			for (p = mArcsBuf.begin(); p != e; p++) {
				if (mArcUsage.isUsed(*p)) continue;
				if (sinkTilewire == mTargetSink) {
					inNode.setCost(0);
					return;
				}
				distance = distanceToSink(p->getSinkTilewire());
				if (distance < bestDistance) bestDistance = distance;
			}
			if (bestDistance == boost::integer_traits<boost::int32_t>::max()) {
				inNode.setCost(bestDistance);
				return;
			}
			cost += bestDistance; // heuristic cost
			/// \todo provide an additional field in RouteNode to store path cost
			if (inNode.getParent() == NULL)
				std::cout << "NULL PARENT" << std::endl;
			else
				cost += inNode.getParent()->getCost(); //add cost to parent;
			
			if (mConflictMap->find(sinkTilewire) != mConflictMap->end()) {
				cost = (cost + (*mConflictMap)[sinkTilewire].mHistorySharing) 
					* (*mConflictMap)[sinkTilewire].mPresentSharing;
			}
			
			inNode.setCost(cost);	
		}
		/// \brief 
		void nodeCostInitial(RouteNode& inNode) {
			Tilewire sinkTilewire = inNode.getSinkTilewire();
			boost::int32_t bestDistance = boost::integer_traits<boost::int32_t>::max();
			boost::int32_t distance = boost::integer_traits<boost::int32_t>::max();
			boost::int32_t cost = 0;
			if (sinkTilewire == mTargetSink) {
				inNode.setCost(0);
				return;
			}
			mArcsBuf.clear();
			mDB.expandSegmentSinks(sinkTilewire, mArcsBuf);
			if (mArcsBuf.size() == 0) {
				inNode.setCost(bestDistance);
				return;
			}
			ArcVector::iterator p;
			ArcVector::iterator e = mArcsBuf.end();
			for (p = mArcsBuf.begin(); p != e; p++) {
				if (mArcUsage.isUsed(*p)) continue;
				if (sinkTilewire == mTargetSink) {
					inNode.setCost(0);
					return;
				}
				distance = distanceToSink(p->getSinkTilewire());
				if (distance < bestDistance) bestDistance = distance;
			}
			if (bestDistance == boost::integer_traits<boost::int32_t>::max()) {
				inNode.setCost(bestDistance);
				return;
			}
			cost += bestDistance; // heuristic cost
			inNode.setCost(cost);	
		}
		/// \brief Reorder the Sinks based on this heuristic.
		virtual void reorderSinks(const Tilewire& inSource, TilewireVector& inSinks) {
			// do nothing for now
		}
		/// \brief Heuristic handling of expansion of a node.
		void expandSegmentSinks(const Tilewire& inTilewire, ArcVector& outArcs) {
			ArcVector tempArcs;
			
			const architecture::TileInfo& tileInfo = mTiles.getTileInfo(inTilewire.getTileIndex());
			const architecture::WireInfo& wireInfo = mTiles.getWireInfo(
				tileInfo.getTypeIndex(), inTilewire.getWireIndex());
			if (wireInfo.isInput()) return;
			
			mDB.expandSegmentSinks(inTilewire, tempArcs, DDB::eExpandDirectionNone,
				true, true, true, false);
			unsigned int s = tempArcs.size();
			if (s == 0) return;
			unsigned int t = inTilewire.getTileIndex() % s;
			for (unsigned int i = t; i < s; i++) {
				outArcs.push_back(tempArcs[i]);
			}
			for (unsigned int i = 0; i < t; i++) {
				outArcs.push_back(tempArcs[i]);
			}
		}

	protected:
		virtual boost::int32_t distanceToSink(const Tilewire& inTilewire) {
			const TileInfo* tileInfo = &mTiles.getTileInfo(inTilewire.getTileIndex());
			boost::int32_t distance = 0;
			boost::int32_t iRow = tileInfo->getRow();
			boost::int32_t iCol = tileInfo->getCol();
			distance += iRow < mRow ? mRow - iRow : iRow - mRow;
			distance += iCol < mCol ? mCol - iCol : iCol - mCol;
			return distance;
		}
		
		virtual boost::int32_t clkDistanceToSink(const Tilewire& inTilewire) {
			return -100;
		}
	}; // class Heuristic
	

} // namespace router
} // namespace torc

#endif // TORC_ROUTER_PATHFINDERNETROUTERHEURISTIC_HPP
