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
/// \brief Header for the NetVectorRouterHeuristicBase class.

#ifndef TORC_ROUTER_NETVECTORROUTERHEURISTICBASE_HPP
#define TORC_ROUTER_NETVECTORROUTERHEURISTICBASE_HPP

#include "torc/router/RouterHeuristicBase.hpp"
#include "torc/router/RouteNet.hpp"

namespace torc {
namespace router {

	/// \brief Provides net routing based on the Nillson graphsearch algorithm.
	/// \details The router can either return a vector of nodes or directly populate DDB usage.
	class NetVectorRouterHeuristicBase : public RouterHeuristicBase {
	// types
		/// \brief Imported type names
		typedef architecture::DDB DDB;
		typedef boost::unordered_map<boost::uint32_t, boost::any> ParameterMap;
		
	public:
	// constructor
		/// \brief Public Constructor
		NetVectorRouterHeuristicBase(DDB& inDB) : RouterHeuristicBase(inDB) {}
		/// \brief Destructor.
		virtual ~NetVectorRouterHeuristicBase() {}

		/// \brief Prioritize nets for routing.
		virtual void prioritizeNets(RouteNetVector& inNets) {}
		/// \brief Heuristically update parameters related to the associated NetVector router.
		virtual void updateParameters(ParameterMap& inParameters) {}
		
	}; // class HeuristicBase
	

} // namespace router
} // namespace torc

#endif // TORC_ROUTER_NETVECTORROUTERHEURISTICBASE_HPP
