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
/// \brief Source for torc::physical output stream helpers.

#include "torc/physical/OutputStreamHelpers.hpp"
#include "torc/physical/Factory.hpp"
#include <iostream>

namespace torc {
namespace physical {

	std::ostream& operator << (std::ostream& os, const Design& rhs) {
		return os << rhs.getName() << " [" << rhs.getDevice() << rhs.getPackage() 
			<< rhs.getSpeedGrade() << ", " << rhs.getXdlVersion() << "]";
	}

	std::ostream& operator << (std::ostream& os, const Module& rhs) {
		return os << rhs.getName() << " [" << rhs.getAnchor() << "]";
	}

	std::ostream& operator <<(std::ostream& os, const Instance& rhs) {
		return os << rhs.getName();
	}

	std::ostream& operator << (std::ostream& os, const Net& rhs) {
		return os << rhs.getName();
	}

	std::ostream& operator <<(std::ostream& os, const InstancePin& rhs) {
		InstanceWeakPtr instancePtr = rhs.getInstancePtr();
		return os << (instancePtr.expired() ? "[unnamed]" : instancePtr.lock()->getName()) << '.' 
			<< rhs.getPinName();
	}

	std::ostream& operator <<(std::ostream& os, const Pip& rhs) {
		return os << rhs.getTileName() << ' ' << rhs.getSourceWireName() << ' '
			<< rhs.getDirectionString() << ' ' << rhs.getSinkWireName();
	}

	std::ostream& operator <<(std::ostream& os, const Config& rhs) {
		return os << rhs.getName() << ':' << rhs.getValue();
	}

} // namespace physical
} // namespace torc
