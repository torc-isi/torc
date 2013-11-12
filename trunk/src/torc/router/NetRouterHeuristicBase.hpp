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
/// \brief Header for the HeuristicBase class.

#ifndef TORC_ROUTER_NETROUTERHEURISTICBASE_HPP
#define TORC_ROUTER_NETROUTERHEURISTICBASE_HPP

#include "torc/router/RouterHeuristicBase.hpp"
#include "torc/architecture/Tilewire.hpp"
#include "torc/router/RouteNode.hpp"

namespace torc {
namespace router {

	/// \brief Provides the interface for net routers.
	class NetRouterHeuristicBase : public RouterHeuristicBase {
	// types
		/// \brief Imported type names
		typedef architecture::DDB DDB;
		typedef architecture::Tilewire Tilewire;
		typedef architecture::TilewireVector TilewireVector;
		typedef architecture::ArcVector ArcVector;
	
	public:
	// constructor
		/// \brief Public Constructor
		NetRouterHeuristicBase(DDB& inDB) : RouterHeuristicBase(inDB) {}
		/// \brief Destructor.
		virtual ~NetRouterHeuristicBase() {}
		
		/// \brief Set the current routing target
		virtual void setSink(const Tilewire& inSink) = 0;
		/// \brief Calculate the node cost based on distance to the sink and path length
		virtual void nodeCost(RouteNode& inNode) = 0;
		/// \brief Reorder the Sinks based on this heuristic
		virtual void reorderSinks(const Tilewire& inSource, TilewireVector& inSinks) {}
		/// \brief Heuristically expand a segment.
		virtual void expandSegmentSinks(const Tilewire& inTilewire, ArcVector& outArcs) {
			mDB.expandSegmentSinks(inTilewire, outArcs);
		}
		
	}; // class HeuristicBase
	

} // namespace router
} // namespace torc

#endif // TORC_ROUTER_NETROUTERHEURISTICBASE_HPP
