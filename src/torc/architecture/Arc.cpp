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
/// \brief Source for the Arc class.

#include "torc/architecture/Arc.hpp"
#include <boost/functional/hash.hpp>

namespace torc {
namespace architecture {

	std::size_t hash_value(const Arc& inArc) {
		std::size_t seed = 0;
		boost::hash_combine(seed, inArc.getSourceTilewire().getTileIndex());
		boost::hash_combine(seed, inArc.getSourceTilewire().getWireIndex());
		boost::hash_combine(seed, inArc.getSinkTilewire().getTileIndex());
		boost::hash_combine(seed, inArc.getSinkTilewire().getWireIndex());
		return seed;
	}
} // namespace architecture
} // namespace torc
