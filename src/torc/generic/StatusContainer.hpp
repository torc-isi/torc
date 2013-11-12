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

#ifndef TORC_GENERIC_STATUSCONTAINER_HPP
#define TORC_GENERIC_STATUSCONTAINER_HPP

#include <iostream>
#include <vector>

#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/Error.hpp"

namespace torc { namespace generic { class Status; } }

namespace torc {
namespace generic {

/**
 * @brief Represents objects that have status
 * 
 * The StatusContainer interface is generalized by classes that need to hold status.
 */
class StatusContainer {
public:
	/**
	 * Get a vector of pointer to Status object
	 *
	 * @param[out] outStatus The vector of pointer to Status object
	 */
	inline void getStatuses(std::vector<StatusSharedPtr>& outStatus) const;

	/**
	 * Set a vector of pointer to Status object
	 *
	 * @param[in] inStatus The vector of pointer to Status object
	 */
	void setStatuses(const std::vector<StatusSharedPtr>& inStatus);

	/**
	 * Add a status to the vector of statuses. If an empty pointer is supplied,
	 * it returns without doing anything.
	 *
	 * @param[in] inStatus A pointer to a status object.
	 *
	 * @exception Error Status could not be added.
	 */
	void addStatus(const StatusSharedPtr& inStatus) throw (Error);

	/**
	 * Apply action on all statuses.
	 * @param[in] action Action to be applied
	 *
	 */
	template <typename _Action> inline void applyOnAllStatuses(const _Action& action) throw (Error);

	~StatusContainer() throw ();

protected:
	StatusContainer();

private:
	StatusContainer(const StatusContainer& source);

	StatusContainer& operator=(const StatusContainer& source);

	std::vector<StatusSharedPtr> mStatuses;

};
/**
 * Get a vector of pointer to Status object
 * 
 * @param[out] outStatus The vector of pointer to Status object
 */
inline void StatusContainer::getStatuses(std::vector<StatusSharedPtr>& outStatus) const {
	outStatus.insert(outStatus.end(), mStatuses.begin(), mStatuses.end());
}

/**
 * Apply action on all statuses.
 * @param[in] action Action to be applied
 *
 */
template <typename _Action> inline void StatusContainer::applyOnAllStatuses(const _Action& action)
	throw (Error) {
	try {
		std::vector<StatusSharedPtr>::iterator it = mStatuses.begin();
		for(; it != mStatuses.end(); ++it) {
			action(*it);
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_STATUSCONTAINER_HPP
