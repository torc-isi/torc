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
/// \brief Header for the PathfinderHeuristic class.

#ifndef TORC_ROUTER_PATHFINDERHEURISTIC_HPP
#define TORC_ROUTER_PATHFINDERHEURISTICHPP

#include "torc/architecture/DDB.hpp"
#include "torc/router/NetVectorRouterHeuristicBase.hpp"
#include "torc/router/RouteNet.hpp"
#include <boost/unordered_map.hpp>
#include <boost/any.hpp>

namespace torc {
namespace router {

	/// \brief Provides net routing based on the Nillson graphsearch algorithm.
	/// \details The router can either return a vector of nodes or directly populate DDB usage.
	class PathFinderHeuristic : public NetVectorRouterHeuristicBase {
	// types
		/// \brief Imported type names
		typedef architecture::DDB DDB;
		typedef architecture::TilewireVector TilewireVector;
		
		typedef boost::unordered_map<boost::uint32_t, boost::any> ParameterMap;
				
	protected:
	// members
		
	public:
	// constructor
		/// \brief Public Constructor
		PathFinderHeuristic(DDB& inDB) : NetVectorRouterHeuristicBase(inDB) {}
		/// \brief Destructor.
		virtual ~PathFinderHeuristic() {}

		/// \brief Prioritize nets for routing.
		virtual void prioritizeNets(RouteNetVector& inNets) {}
		/// \brief Heuristically update parameters related to the associated NetVector router.
		virtual void updateParameters(ParameterMap& inParameters) {}
		
	}; // class PathFinderHeuristic
	

} // namespace router
} // namespace torc

#endif // TORC_ROUTER_PATHFINDERHEURISTIC_HPP
