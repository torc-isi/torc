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

#ifndef TORC_GENERIC_NETDELAY_HPP
#define TORC_GENERIC_NETDELAY_HPP

#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/Derivation.hpp"
#include "torc/generic/Value.hpp"

namespace torc {
namespace generic {

/**
 * @brief Represents the netDelay attribute of Net
 * 
 * The NetDelay class stores the delay value and its derivation 
 * for the given set of transitions. 
 */
class NetDelay {
public:
	/**
	 * Get derivation
	 *
	 * @return Derivation value
	 */
	inline const Derivation getDerivation() const;

	/**
	 * Set derivation
	 *
	 * @param[in] value Derivation value
	 */
	void setDerivation(const Derivation& value);

	/**
	 * Get delay value
	 *
	 * @return Value containing Value::MiNoMax
	 */
	inline const Value::MiNoMax getDelay() const;

	/**
	 * Set delay value
	 *
	 * @param[in] value Delay value. Must be Value::MiNoMax
	 */
	void setDelay(const Value::MiNoMax& value);

	/**
	 * Get the pointer to logic state value(transition/becomes).
	 *
	 * @return Pointer to logic state value(transition/becomes).
	 */
	inline const LogicElementSharedPtr getTransition() const;

	/**
	 * Set the pointer to logic state value(transition/becomes).
	 *
	 * @param[in] inSource Pointer to logic state value(transition/becomes).
	 */
	void setTransition(const LogicElementSharedPtr& inSource);

	NetDelay();

	~NetDelay() throw ();

	NetDelay(const NetDelay& source);

	NetDelay& operator=(const NetDelay& source);

private:
	Derivation mDerivation;
	Value::MiNoMax mDelay;
	LogicElementSharedPtr mTransition;

};
/**
 * Get derivation
 * 
 * @return Derivation value
 */
inline const Derivation NetDelay::getDerivation() const {
	return mDerivation;
}

/**
 * Get delay value
 * 
 * @return Value containing Value::MiNoMax
 */
inline const Value::MiNoMax NetDelay::getDelay() const {
	return mDelay;
}

/**
 * Get the pointer to logic state value(transition/becomes).
 *
 * @return Pointer to logic state value(transition/becomes).
 */
inline const LogicElementSharedPtr NetDelay::getTransition() const {
	return mTransition;
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_NETDELAY_HPP
