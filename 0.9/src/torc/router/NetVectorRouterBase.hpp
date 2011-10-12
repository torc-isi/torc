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
/// \brief Header for the NetVectorRouterBase class.

#ifndef TORC_ROUTER_NETVECTORROUTERBASE_HPP
#define TORC_ROUTER_NETVECTORROUTERBASE_HPP

#include "torc/architecture/DDB.hpp"
#include "torc/router/RouteNode.hpp"
#include "torc/router/NetVectorRouterHeuristicBase.hpp"
#include "torc/router/RouteNet.hpp"

#include <algorithm>
#include <queue>
#include <boost/cstdint.hpp>
#include <boost/timer.hpp>
#include <boost/unordered_map.hpp>
#include "NetVectorRouterHeuristicBase.hpp"
#include "NetRouterBase.hpp"

namespace torc {
namespace router {

	/// \brief Abstract class for a net router.
	/// \details This base class provides a virtual route function.
	class NetVectorRouterBase {
	// types
		/// \brief Imported type names
		typedef architecture::DDB DDB;
		typedef architecture::WireUsage WireUsage;
		typedef architecture::Tilewire Tilewire;
		
	protected:
	// members
		/// \brief Database reference.
		DDB& mDB;
		/// \brief Pointer to the heuristic for making routing decisions.
		NetVectorRouterHeuristicBase* mHeuristic;
		/// \brief Pointer to the underlying net router.
		NetRouterBase* mNetRouter;
		/// \brief Timer object for performance analysis.
		boost::timer mRouteTimer;
		/// \brief Total routing time since construction.
		double mTotalRouteTime;

	public:
	// constructor
		/// \brief Public Constructor
		NetVectorRouterBase(DDB& inDB, NetVectorRouterHeuristicBase* inHeuristic,
			NetRouterBase* inNetRouter) : mDB(inDB), mHeuristic(inHeuristic), 
			mNetRouter(inNetRouter) {
			mTotalRouteTime = 0;
		}
		/// \brief Destructor.
		virtual ~NetVectorRouterBase() {}

		/// \brief Primary route call
		void route(RouteNetVector& inNets) {
			mRouteTimer.restart();
			routeNets(inNets);
			mTotalRouteTime += mRouteTimer.elapsed();
		}
		/// \brief Accessor for the heuristic.
		NetVectorRouterHeuristicBase* getHeuristic() { return mHeuristic; }
		/// \brief Set method for the heuristic.
		void setHeuristic(NetVectorRouterHeuristicBase* inHeuristic) { mHeuristic = inHeuristic; }
		/// \brief Accessor for the underlying Net Router.
		NetRouterBase* getNetRouter() { return mNetRouter; }
		/// \brief Set method for the net router.
		void setNetRouter(NetRouterBase* inNetRouter) { mNetRouter = inNetRouter; }
		
	protected:
		/// brief protected routing call 
		virtual void routeNets(RouteNetVector& inNets) = 0;

	}; // class SignalRouterBase
	

} // namespace router
} // namespace torc

#endif // TORC_ROUTER_NETVECTORROUTERBASE_HPP
