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
/// \brief Header for the BasicRouter class.

#ifndef TORC_ROUTER_NETROUTERBASE_HPP
#define TORC_ROUTER_NETROUTERBASE_HPP

#include "torc/architecture/DDB.hpp"
#include "torc/router/RouteNode.hpp"
#include "torc/router/NetRouterHeuristicBase.hpp"
#include "torc/router/RouteNet.hpp"

#include "torc/architecture/OutputStreamHelpers.hpp"
#include <set>
#include <iostream>
#include <algorithm>
#include <queue>
#include <boost/cstdint.hpp>
#include <boost/timer.hpp>
#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>
#include "NetRouterHeuristicBase.hpp"

namespace torc {
namespace router {

	/// \brief Abstract class for a net router.
	/// \details This base class provides a virtual route function.
	class NetRouterBase {
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
		NetRouterHeuristicBase* mHeuristic;
		/// \brief Timer object for performance analysis.
		boost::timer mRouteTimer;
		/// \brief Total routing time since construction.
		double mTotalRouteTime;

	public:
	// constructor
		/// \brief Public Constructor
		NetRouterBase(DDB& inDB, NetRouterHeuristicBase* inHeuristic) : mDB(inDB), 
			mHeuristic(inHeuristic) {
			mTotalRouteTime = 0;
		}
		/// \brief Destructor.
		virtual ~NetRouterBase() {}

		/// \brief Primary route call.
		void route(RouteNet& inNet) {
			mRouteTimer.restart();
			routeNet(inNet);
			double routeTime = mRouteTimer.elapsed();
			mTotalRouteTime += routeTime;
			inNet.mProperties[eRouteTime] = routeTime;
		}
		
		/// \brief Get total route time.
		double getTotalRouteTime() { return mTotalRouteTime; }
		/// \brief Get the current heuristic.
		NetRouterHeuristicBase* getHeuristic() { return mHeuristic; }
		/// \brief Set the current heuristic.
		void setHeuristic(NetRouterHeuristicBase* inHeuristic) { mHeuristic = inHeuristic; }
		
	protected:
		/// \brief Virtual route call.
		virtual void routeNet(RouteNet& inNet) = 0;
	}; // class SignalRouterBase
	

} // namespace router
} // namespace torc

#endif // TORC_ROUTER_NETROUTERBASE_HPP
