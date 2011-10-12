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
/// \brief Header for the RouterStatistics class.

#ifndef TORC_ROUTER_ROUTERSTATISTICS_HPP
#define TORC_ROUTER_ROUTERSTATISTICS_HPP

#include <boost/cstdint.hpp>

namespace torc {
namespace router {

	/// \brief Router Statistics net.
	/// \details The RouteNet class contains architecture specific sources and sinks.
	struct RouterStatistics {

	// members

		/// \brief Time spent routing this net.
		double mRouteTime;
		/// \brief Number of passes through the main loop routing this net.
		boost::uint32_t mStatLoopPasses;
		/// \brief Number of nodes created from expansion.
		boost::uint32_t mStatExpanded;

	// constructors
		/// \brief Default constructor.
		RouterStatistics() : mRouteTime(0), mStatLoopPasses(0), mStatExpanded(0) {}
	};

} // namespace router
} // namespace torc

#endif // TORC_ROUTER_ROUTERSTATISTICS_HPP
