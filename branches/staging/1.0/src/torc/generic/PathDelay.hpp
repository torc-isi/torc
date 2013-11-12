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

#ifndef TORC_GENERIC_PATHDELAY_HPP
#define TORC_GENERIC_PATHDELAY_HPP

#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/Value.hpp"
#include "torc/generic/SelfReferencing.hpp"
#include "torc/generic/FactoryType.hpp"

#include <list>

namespace torc { namespace generic { class Event; } }

namespace torc {
namespace generic {

/**
 * 
 * @brief This class associates a delay with a specified chain of events.
 * Delay contains the time from first event to final event.
 */
class PathDelay : public SelfReferencing<PathDelay> {
	friend class FactoryType<PathDelay> ;

public:
	/**
	 * Convenience class to create a PathDelay.
	 */
	class Factory : public FactoryType<PathDelay> {
	public:
		using FactoryType<PathDelay>::create;
		/**
		 * Create a PathDelay.
		 *
		 * @param[in] inDelay Value::MiNoMax Delay value.
		 * @param[in] inEvents List of events to be appended to
		 * @param[in] inTiming Pointer to parented object (Timing) [optional].
		 *            If not mentioned PathDelay will not be added to timing.
		 *
		 * @return Pointer to created PathDelay.
		 */
		PathDelaySharedPtr virtual newPathDelayPtr(const Value::MiNoMax& inDelay,
			const std::list<EventSharedPtr>& inEvents,
			const TimingSharedPtr& inTiming = TimingSharedPtr()) throw (Error);
	};

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
	 * Get the list of events associated with this path delay.
	 *
	 * @return outEvents List of events associated with this path delay
	 */
	inline void getEvents(std::list<EventSharedPtr>& outEvents) const;

	/**
	 * Set all the events of this forbiddenEvent.
	 *
	 * @param[in] inEvents List of events to be appended to
	 */
	void setEvents(const std::list<EventSharedPtr>& inEvents);

	/**
	 * Add an event to the list of events. Empty pointer is ignored.
	 *
	 * @param[in] inEvent Pointer to event to be added.
	 */
	void addEvent(const EventSharedPtr& inEvent) throw (Error);

	PathDelay();

	~PathDelay() throw ();

private:
	PathDelay(const PathDelay& inSource);

	PathDelay& operator =(const PathDelay& inSource);

	Value::MiNoMax mDelay;
	std::list<EventSharedPtr> mEvents;

};

inline const Value::MiNoMax PathDelay::getDelay() const {
	return mDelay;
}

inline void PathDelay::getEvents(std::list<EventSharedPtr>& outEvents) const {
	outEvents.insert(outEvents.end(), mEvents.begin(), mEvents.end());
}

} // namespace generic
} // namespace torc
#endif // TORC_GENERIC_PATHDELAY_HPP
