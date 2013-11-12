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

#ifndef TORC_GENERIC_TIMING_HPP
#define TORC_GENERIC_TIMING_HPP

#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/Visitable.hpp"
#include "torc/generic/SelfReferencing.hpp"
#include "torc/generic/Commentable.hpp"
#include "torc/generic/UserDataContainer.hpp"
#include "torc/generic/Error.hpp"
#include "torc/generic/Derivation.hpp"
#include "torc/generic/FactoryType.hpp"
#include "torc/generic/VisitorType.hpp"

namespace torc { namespace generic { class BaseVisitor; } }
namespace torc { namespace generic { class PathDelay; } }
namespace torc { namespace generic { class ForbiddenEvent; } }

namespace torc {
namespace generic {

/**
 * @brief This class is used to provide a set of path delays or 
 * timing constrains (forbidden events)
 */
class Timing : public Visitable, public SelfReferencing<Timing>, public Commentable,
	public UserDataContainer {
	friend class FactoryType<Timing> ;

public:
	/**
	 * Convenience class to visit a Timing.
	 */
	typedef VisitorType<Timing> Visitor;

	virtual void accept(BaseVisitor& inoutVisitor) throw (Error);

	/**
	 * Convenience class to create a Timing.
	 */
	class Factory : public FactoryType<Timing> {
	public:
		using FactoryType<Timing>::create;
		/**
		 * Create a Timing.
		 *
		 * @param[in] inDerivation Derivation value
		 * @param[in] inPathDelays List of path delays of this timing to be appended to.
		 * @param[in] inForbiddentEvents List of forbidden events to be appended to
		 * @param[in] inView Pointer to parented (view) object.
		 * @param[in] inInterfaceAttributes Pointer to
		 *            parented (InterfaceAttributes) object [optional].
		 *            If mentioned then this will decompile within (contents ...) construct.
		 * @param[in] inInstance Pointer to parented object(Instance) [optional].
		 *            If mentioned then this will decompile within (instance ...) construct.
		 *
		 * @return Pointer to created Timing.
		 */
		virtual TimingSharedPtr newTimingPtr(const Derivation& inDerivation,
			const std::list<PathDelaySharedPtr>& inPathDelays,
			const std::list<ForbiddenEventSharedPtr>& inForbiddentEvents,
			const ViewSharedPtr& inView, const InterfaceAttributesSharedPtr& inInterfaceAttributes =
				InterfaceAttributesSharedPtr(), const InstanceSharedPtr& inInstance =
				InstanceSharedPtr()) throw (Error);
	};

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
	 * Get all the path delays of this timing.
	 *
	 * @param[out] outPathDelays List of path delays of this timing to be appended to
	 */
	inline void getPathDelays(std::list<PathDelaySharedPtr>& outPathDelays) const;

	/**
	 * Set all the path delays of this timing.
	 *
	 * @param[in] inPathDelays List of path delays of this timing to be appended to
	 */
	void setPathDelays(const std::list<PathDelaySharedPtr>& inPathDelays);

	/**
	 * Add a path delay to the list of path delays.
	 *
	 * @param[in] inSource Pointer to path delay to be appended to
	 */
	void addPathDelay(const PathDelaySharedPtr& inSource);

	/**
	 * Get all the forbidden events of this timing.
	 *
	 * @param[out] outForbiddentEvents List of events to be appended to
	 */
	inline void getForbiddentEvents(std::list<ForbiddenEventSharedPtr>& outForbiddentEvents) const;

	/**
	 * Set all the forbidden events of this timing.
	 *
	 * @param[in] inForbiddentEvents List of events to be appended to
	 */
	void setForbiddentEvents(const std::list<ForbiddenEventSharedPtr>& inForbiddentEvents);

	/**
	 * Add a forbidden event of the list of forbidden events.
	 *
	 * @param[in] inSource Pointer to forbidden event to be appended to
	 */
	void addForbiddenEvent(const ForbiddenEventSharedPtr& inSource);

	~Timing() throw ();

protected:
	Timing();

private:
	Timing(const Timing& source);

	Timing& operator=(const Timing& source);

	Derivation mDerivation;
	std::list<PathDelaySharedPtr> mPathDelays;
	std::list<ForbiddenEventSharedPtr> mForbiddentEvents;

};

/**
 * Get derivation
 *
 * @return Derivation value
 */
inline const Derivation Timing::getDerivation() const {
	return mDerivation;
}

/**
 * Get all the path delays of this timing.
 *
 * @param[out] outPathDelays List of path delays of this timing to be appended to
 */
inline void Timing::getPathDelays(std::list<PathDelaySharedPtr>& outPathDelays) const {
	outPathDelays.insert(outPathDelays.end(), mPathDelays.begin(), mPathDelays.end());
}

/**
 * Get all the forbidden events of this timing.
 *
 * @param[out] outForbiddentEvents List of events to be appended to
 */
inline void Timing::getForbiddentEvents(std::list<ForbiddenEventSharedPtr>& outForbiddentEvents)
	const {
	outForbiddentEvents.insert(outForbiddentEvents.end(), mForbiddentEvents.begin(),
		mForbiddentEvents.end());
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_TIMING_HPP
