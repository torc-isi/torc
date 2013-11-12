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

#ifndef TORC_GENERIC_PORTDELAY_HPP
#define TORC_GENERIC_PORTDELAY_HPP

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/Derivation.hpp"
#include "torc/generic/Value.hpp"

namespace torc {
namespace generic {

/**
 * @brief Represents the portDelay attribute on Port or PortReference
 *
 * The Portdelay class stores the delay inSource for a port. Currently it supports storing only
 * static delay values. Load dependent delay inSource support (using the loadDelay construct) is
 * not present.
 */
class PortDelay {
public:
	/**
	 * @enum DelayType
	 *
	 * Types of possible delay
	 */
	enum Type {
		eTypeDelay, eTypeLoadDelay
	};
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION
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
	 * @param[in] inSource Derivation inSource
	 */
	void setDerivation(const Derivation& inSource);

	/**
	 * Get whether value is delay or loadDelay
	 *
	 * @return Type
	 */
	inline const PortDelay::Type getType() const;

	/**
	 * Set whether value is delay or loadDelay
	 *
	 * @param[in] inSource Type of data
	 */
	void setType(Type inSource);

	/**
	 * Get delay
	 *
	 * @return MiNoMax value
	 */
	inline const Value::MiNoMax getDelay() const;

	/**
	 * Set delay
	 *
	 * @param[in] inSource Delay value.
	 */
	void setDelay(const Value::MiNoMax& inSource);

	/**
	 * Get the AcLoad value. This is valid when value type is loadDelay
	 *
	 * @return MiNoMax value
	 */
	inline const Value::MiNoMax getAcLoad() const;

	/**
	 * Set AC Load
	 *
	 * @param[in] inSource Load value
	 */
	void setAcLoad(const Value::MiNoMax& inSource);

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

	PortDelay();

	~PortDelay() throw ();

	PortDelay(const PortDelay& source);

	PortDelay&
	operator=(const PortDelay& source);

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
	Derivation mDerivation;
	Type mType;
	Value::MiNoMax mDelay;
	Value::MiNoMax mAcLoad;
	LogicElementSharedPtr mTransition;
};

inline const Derivation PortDelay::getDerivation() const {
	return mDerivation;
}

inline const PortDelay::Type PortDelay::getType() const {
	return mType;
}

inline const Value::MiNoMax PortDelay::getDelay() const {
	return mDelay;
}

inline const Value::MiNoMax PortDelay::getAcLoad() const {
	return mAcLoad;
}

/**
 * Get the pointer to logic state value(transition/becomes).
 *
 * @return Pointer to logic state value(transition/becomes).
 */
inline const LogicElementSharedPtr PortDelay::getTransition() const {
	return mTransition;
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_PORTDELAY_HPP
