// Torc - Copyright 2011 University of Southern California.  All Rights Reserved.
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
/// \brief Header for the InstancePin class.

#ifndef TORC_PHYSICAL_INSTANCE_PIN_HPP
#define TORC_PHYSICAL_INSTANCE_PIN_HPP

#include "torc/physical/Progeny.hpp"
#include "torc/physical/Instance.hpp"
#include "torc/physical/TilewirePlaceholder.hpp"
#include <boost/smart_ptr.hpp>

namespace torc {
namespace physical {

	/// \brief Physical design instance-pin pair, suitable for specifying a net endpoint.
	/// \details This class specifies a pin on a physical design instance.
	///	\sa SitePin.
	class InstancePinBase : public Progeny<class Net>, protected Progenitor<InstancePin> {
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
	// members
		/// \brief The pin weak instance pointer.
		InstanceWeakPtr mInstancePtr;
		/// \brief The pin name.
		PinName mPinName;
	// functions
		/// \brief Asks the associated instance to add this pin from its pin map.
		void addToInstance(void) { mInstancePtr.lock()->addPin(mSelfWeakPtr); }
		/// \brief Asks the associated instance to remove this pin from its pin map.
		void removeFromInstance(void) { mInstancePtr.lock()->removePin(mSelfWeakPtr); }
	// constructors
		/// \brief Protected constructor.
		/// \param inInstancePtr The pin instance pointer.
		/// \param inPinName The pin name.
		InstancePinBase(InstanceSharedPtr inInstancePtr, const string& inPinName) 
			: mInstancePtr(inInstancePtr), mPinName(inPinName) {}
		/// \brief Disabled copy constructor.
		InstancePinBase(const InstancePin&) : mInstancePtr(), mPinName("") {}
	// operators
		/// \brief Disabled assignment operator.
		void operator =(const InstancePin&) {}
	public:
	// constructors
		/// \brief Non-virtual destructor.
		~InstancePinBase(void) { if(!mInstancePtr.expired()) removeFromInstance(); }
	// accessors
		/// \brief Returns the weak instance pointer.
		const InstanceWeakPtr& getInstancePtr(void) const { return mInstancePtr; }
		/// \brief Returns the pin name.
		const PinName& getPinName(void) const { return mPinName; }
		/// \brief Sets the weak instance pointer.
		void setInstancePtr(const InstanceWeakPtr& inInstancePtr) { mInstancePtr = inInstancePtr; }
		/// \brief Sets the pin name.
		void setPinName(const string& inPinName) { mPinName = inPinName; }
	// operators
		/// \brief Equality operator.
		bool operator ==(const InstancePinBase& rhs) const { 
			return !(mInstancePtr < rhs.mInstancePtr) && !(rhs.mInstancePtr < mInstancePtr) 
				&& mPinName == rhs.mPinName; 
		}
	};

	/// \brief Physical design instance-pin pair, suitable for specifying a net endpoint.
	/// \details This class specifies a pin on a physical design instance.  Note that this class 
	///		has a TilewirePlaceholder as a base class, and is directly analogous to its 
	///		Tilewire-aware counterpart, torc::architecture::InstancePin.  This approach is used to 
	///		avoid importing torc::architecture dependencies into torc::physical for users who do 
	///		not require device database functionality.
	///	\sa SitePin.
	class InstancePin : public InstancePinBase, public TilewirePlaceholder {
	// friends
		/// \brief The Factory class has direct access to our internals.
		friend class Factory;
		/// \brief The Net class has direct access to our internals.
		/// \details The parent Net can use this to direct the InstancePin to register itself with 
		///		its related Instance.
		friend class Net;
	protected:
	// constructors
		/// \brief Protected constructor.
		/// \param inInstancePtr The pin instance pointer.
		/// \param inPinName The pin name.
		InstancePin(InstanceSharedPtr inInstancePtr, const string& inPinName) 
			: InstancePinBase(inInstancePtr, inPinName), TilewirePlaceholder() {}
	private:
	// constructors
		/// \brief Disabled copy constructor.
		InstancePin(const InstancePin& rhs) 
			: InstancePinBase(rhs), TilewirePlaceholder() {}
	};

	/// \brief Shared pointer encapsulation of an InstancePin.
	typedef boost::shared_ptr<InstancePin> InstancePinSharedPtr;

	/// \brief Weak pointer encapsulation of an InstancePin.
	typedef boost::weak_ptr<InstancePin> InstancePinWeakPtr;

	/// \brief Vector of InstancePin shared pointers.
	typedef std::vector<InstancePinSharedPtr> InstancePinSharedPtrVector;

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_INSTANCE_PIN_HPP
