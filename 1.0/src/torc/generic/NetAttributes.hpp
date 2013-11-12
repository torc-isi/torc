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

#ifndef TORC_GENERIC_NETATTRIBUTES_HPP
#define TORC_GENERIC_NETATTRIBUTES_HPP

#include "torc/generic/NetDelay.hpp"

namespace torc {
namespace generic {

/**
 * @brief Represents attributes of a Net object
 * 
 * The NetAttributes class stores some optional attributes of a Net. 
 */
class NetAttributes {
public:
	/**
	 * Get the criticality info of a net for routing.
	 *
	 * @return Integer value representing criticality info of a net.
	 */
	inline const int32_t getCriticality() const;

	/**
	 * Set the criticality info of a net for routing.
	 *
	 * @param[in] value Integer value representing criticality info of a net.
	 */
	void setCriticality(const int32_t& value);

	/**
	 * Get delay information of net
	 *
	 * @return NetDelay
	 */
	inline const NetDelay getNetDelay() const;

	/**
	 * Set delay information of net
	 *
	 * @param[in] inSource NetDelay, the delay information of a net.
	 */
	void setNetDelay(const NetDelay& inSource);

	inline const bool getIsNetDelaySet() const;

	inline const bool getIsCriticalitySet() const;

	NetAttributes();

	~NetAttributes() throw ();

	NetAttributes(const NetAttributes& source);

	NetAttributes& operator=(const NetAttributes& source);

private:
	/**
	 * This attribute is used to describe the relative importance of the net
	 * compared to other nets, for routing purpose.
	 */
	int32_t mCriticality;

	/**
	 * This is an attributes of a net to specify a delay
	 * and its derivation for the given set of transitions.
	 */
	NetDelay mNetDelay;
	bool mCriticalitySet;
	bool mNetDelaySet;

};

inline const int32_t NetAttributes::getCriticality() const {
	return mCriticality;
}

inline const NetDelay NetAttributes::getNetDelay() const {
	return mNetDelay;
}

inline const bool NetAttributes::getIsCriticalitySet() const {
	return mCriticalitySet;
}

inline const bool NetAttributes::getIsNetDelaySet() const {
	return mNetDelaySet;
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_NETATTRIBUTES_HPP
