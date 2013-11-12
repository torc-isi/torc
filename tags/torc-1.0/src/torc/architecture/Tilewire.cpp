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
/// \brief Source for the Tilewire class.

#include "torc/architecture/Tilewire.hpp"
#include <boost/functional/hash.hpp>

namespace torc {
namespace architecture {

	const Tilewire Tilewire::sInvalid = Tilewire(TileIndex(-1), WireIndex(-1));

	std::size_t hash_value(const Tilewire& inTilewire) {
		std::size_t seed = 0;
		boost::hash_combine(seed, inTilewire.getTileIndex());
		boost::hash_combine(seed, inTilewire.getWireIndex());
		return seed;
	}

} // namespace architecture
} // namespace torc
