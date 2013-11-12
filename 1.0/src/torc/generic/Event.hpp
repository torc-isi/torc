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

#ifndef TORC_GENERIC_EVENT_HPP
#define TORC_GENERIC_EVENT_HPP

#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/Visitable.hpp"
#include "torc/generic/SelfReferencing.hpp"
#include "torc/generic/FactoryType.hpp"
#include "torc/generic/VisitorType.hpp"
#include "torc/generic/Value.hpp"
#include "torc/generic/PortElement.hpp"

#include <list>

namespace torc { namespace generic { class PortElement; } }
namespace torc { namespace generic { class PortList; } }
namespace torc { namespace generic { class Net; } }

namespace torc {
namespace generic {

/**
 * @brief Event is used to describe an event on a port or a net using
 * logic state transitions. Events can also be described for unordered groups
 * of ports or nets using portGroup or netGroup. An ordered list of ports may 
 * also be used using a portList. 
 *
 * This class also models edif OffsetEvent construct,
 * which is used to define a time relative to an Event.
 */
class Event : public Visitable, public SelfReferencing<Event> {
	friend class FactoryType<Event> ;

public:
	/**
	 * @enum Type
	 * Event type
	 */
	enum Type {
		eTypeEvent = 0, eTypeOffsetEvent
	};

	/**
	 * Convenience class to visit an Event.
	 */
	typedef VisitorType<Event> Visitor;

	virtual void accept(BaseVisitor& inoutVisitor) throw (Error);

	/**
	 * Convenience class to create an Event.
	 */
	class Factory : public FactoryType<Event> {
	public:
		using FactoryType<Event>::create;
		/**
		 * Create an event.
		 *
		 * @param[in] inEventType Event type.
		 * @param[in] inPorts List of ports to this composition.
		 * @param[in] inPortReferences List of port references to this composition.
		 * @param[in] inNets List of nets to this composition.
		 * @param[in] inTransition Pointer to logic state value(transition/becomes) [optional].
		 *            If no transition are specified, then the default meaning is any logic state
		 *            change.
		 * @param[in] inPortList Connected Port list to this event [optional].
		 * @param[in] inOffsetTime Offset time retative to an event [optional].
		 *            This is must for OffsetEvent, but optional for normal Event.
		 *
		 * @return Pointer to created event.
		 */
		virtual EventSharedPtr newEventPtr(const Event::Type& inEventType,
			const std::list<PortSharedPtr>& inPorts,
			const std::list<PortReferenceSharedPtr>& inPortReferences,
			const std::list<NetSharedPtr>& inNets,
			const LogicElementSharedPtr& inTransition = LogicElementSharedPtr(),
			const PortListSharedPtr& inPortList = PortListSharedPtr(),
			const Value& inOffsetTime = Value()) throw (Error);
	};

	/**
	 * Get the event type
	 *
	 * @return Event type
	 */
	inline const Type getType() const;

	/**
	 * Set the event type
	 *
	 * @param[in] inSource Event type
	 */
	void setType(const Type& inSource);

	/**
	 * Get the list of port elements of an Event.
	 * When port element list size is more than one, it is portGroup.
	 *
	 * @return outPortElements List of port elements to be appended to
	 */
	inline void getPortElements(std::list<PortElement>& outPortElements) const;

	/**
	 * Add a port to the port list.
	 *
	 * @param[in] inPort Pointer to port to be added.
	 */
	void addPort(const PortSharedPtr& inPort);

	/**
	 * Add a port reference to the port list.
	 *
	 * @param[in] inPortRef Pointer to port reference to be added.
	 */
	void addPortReference(const PortReferenceSharedPtr& inPortRef);

	/**
	 * Get the pointer to portList of an Event.
	 *
	 * @return Pointer to portList
	 */
	inline const PortListSharedPtr getPortList() const;

	/**
	 * Set the pointer to portList
	 *
	 * @param[in] inPortList Pointer to portList
	 */
	void setPortList(const PortListSharedPtr& inPortList);

	/**
	 * Get the list of Net of an Event.
	 * When Net element list size is more than one, it is NetGroup.
	 *
	 * @return outNets List of nets to be appended to
	 */
	inline void getNets(std::list<NetSharedPtr>& outNets) const;

	/**
	 * Add a Net to the Net list.
	 *
	 * @param[in] inNet Pointer to Net to be added.
	 */
	void addNet(const NetSharedPtr& inNet);

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

	/**
	 * Get the offset time retative to an event.
	 * @return Value offset time retative to an event.
	 */
	inline const Value getOffsetTime() const;

	/**
	 * Set the offset time retative to an event.
	 * @param[in] value Offset time retative to an event.
	 */
	void setOffsetTime(const Value& value);

	virtual ~Event() throw ();

protected:
	Event();

private:
	Event(const Event& source);

	Event& operator=(const Event& source);

	Type mType;
	std::list<PortElement> mPortElements;
	PortListSharedPtr mPortList;
	std::list<NetSharedPtr> mNets;
	LogicElementSharedPtr mTransition;
	Value mOffsetTime;

};

/**
 * Get the event type
 *
 * @return Event type
 */
inline const Event::Type Event::getType() const {
	return mType;
}

/**
 * Get the list of port elements of an Event.
 * When port element list size is more than one, it is portGroup.
 *
 * @return outPortElements List of port elements to be appended to
 */
inline void Event::getPortElements(std::list<PortElement>& outPortElements) const {
	outPortElements.insert(outPortElements.end(), mPortElements.begin(), mPortElements.end());
}

/**
 * Get the pointer to portList of an Event.
 *
 * @return Pointer to portList
 */
inline const PortListSharedPtr Event::getPortList() const {
	return mPortList;
}

/**
 * Get the list of nets of an Event.
 * When Net element list size is more than one, it is NetGroup.
 *
 * @return outNets List of nets to be appended to
 */
inline void Event::getNets(std::list<NetSharedPtr>& outNets) const {
	outNets.insert(outNets.end(), mNets.begin(), mNets.end());
}

/**
 * Get the pointer to logic state value(transition/becomes).
 *
 * @return Pointer to logic state value(transition/becomes).
 */
inline const LogicElementSharedPtr Event::getTransition() const {
	return mTransition;
}

/**
 * Get the offset time retative to an event.
 * @return Value offset time retative to an event.
 */
inline const Value Event::getOffsetTime() const {
	return mOffsetTime;
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_EVENT_HPP
