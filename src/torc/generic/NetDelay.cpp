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

#include "torc/generic/NetDelay.hpp"

namespace torc {
namespace generic {

/**
 * Set derivation
 * 
 * @param[in] value Derivation value
 */
void NetDelay::setDerivation(const Derivation& value) {
	mDerivation = value;
}

/**
 * Set delay value
 * 
 * @param[in] value Delay value. Must be Value::MiNoMax
 */
void NetDelay::setDelay(const Value::MiNoMax& value) {
	mDelay = value;
}

/**
 * Set the pointer to logic state value(transition/becomes).
 *
 * @param[in] inSource Pointer to logic state value(transition/becomes).
 */
void NetDelay::setTransition(const LogicElementSharedPtr& inSource) {
	mTransition = inSource;
}

NetDelay::NetDelay() : mDerivation(eDerivationUndefined), mDelay(), mTransition() {}

NetDelay::~NetDelay() throw () {}

NetDelay::NetDelay(const NetDelay& source) :
	mDerivation(source.mDerivation), mDelay(source.mDelay), mTransition(source.mTransition) {}

NetDelay& NetDelay::operator=(const NetDelay& source) {
	if(this != &source) {
		mDerivation = source.mDerivation;
		mDelay = source.mDelay;
		mTransition = source.mTransition;
	}
	return *this;
}

} // namespace generic
} // namespace torc
