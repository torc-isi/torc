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

#include "torc/generic/InterfaceAttributes.hpp"

namespace torc {
namespace generic {

void InterfaceAttributes::setDesignator(const std::string& inSource) {
	mDesignator = inSource;
}

/**
 * Set the pointer to the simulate.
 *
 * @param[in] inSource Pointer to the simulate
 */
void InterfaceAttributes::setSimulate(const SimulateSharedPtr& inSource) {
	mSimulate = inSource;
}

/**
 * Set the pointer to the timing object
 *
 * @param[in] inSource Pointer to the timing object
 */
void InterfaceAttributes::setTiming(const TimingSharedPtr& inSource) {
	mTiming = inSource;
}

InterfaceAttributes::InterfaceAttributes() : Commentable(), UserDataContainer(), mDesignator(),
	mSimulate(), mTiming() {}

InterfaceAttributes::~InterfaceAttributes() throw () {}

} // namespace generic
} // namespace torc
