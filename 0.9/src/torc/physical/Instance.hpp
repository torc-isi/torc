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
/// \brief Header for the Instance class.

#ifndef TORC_PHYSICAL_INSTANCE_HPP
#define TORC_PHYSICAL_INSTANCE_HPP

#include "torc/physical/Named.hpp"
#include "torc/physical/Renamable.hpp"
#include "torc/physical/Progeny.hpp"
#include "torc/physical/Progenitor.hpp"
#include "torc/physical/ConfigMap.hpp"
#include "torc/physical/InstanceReference.hpp"
#include "torc/physical/Annotated.hpp"
#include <boost/smart_ptr.hpp>
#include <vector>
#include <map>

namespace torc {
namespace physical {

	// forward declarations

	/// \brief Shared pointer encapsulation of an InstancePin.
	typedef boost::shared_ptr<class InstancePin> InstancePinSharedPtr;

	/// \brief Weak pointer encapsulation of an InstancePin.
	typedef boost::weak_ptr<class InstancePin> InstancePinWeakPtr;

	/// \brief Vector of InstancePin shared pointers.
	typedef std::vector<InstancePinSharedPtr> InstancePinSharedPtrVector;

	/// \brief Physical design instance.
	/// \details This class describes a physical instance in the design.
	/// \todo Need a good way of finding the direction of each instance pin.
	class Instance : public Renamable<class Circuit>, public Progeny<class Circuit>, 
		public ConfigMap, public Annotated, protected Progenitor<Instance> {
	// friends
		/// \brief The Factory class has direct access to our internals.
		friend class Factory;
		/// \brief The InstancePin class has direct access to our internals.
		/// \details This is provided to allow notification when instance pins are added or removed 
		///		for this object.
		friend class InstancePinBase;
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
		/// \brief Map from instance pin name to instance pin
		/// \todo This needs to be made a multimap, to account for multiple InstancePin objects 
		///		with the same pin name being unintentionally created by the user.
		typedef std::multimap<PinName, InstancePinSharedPtr> InstancePinMap;
	// members
		/// \brief The instance logic type.
		/// \details The type should be a valid logic type for the target architecture, or the name 
		///		of a module defined in this design.
		string mType;
		/// \brief The instance placement tile, or an empty string if unplaced.
		string mTile;
		/// \brief The instance placement site, or an empty string if unplaced.
		string mSite;
		/// \brief The instance bonding.
		/// \details The bonding must be EInstanceBonding::eInstanceBondingUnknown unless this 
		///		instance is an unplaced IO.  For an unplaced IO, the bonding setting determines 
		///		whether the instance will be placed on a bonded pad or an unbonded pad.
		EInstanceBonding mBonding;
		/// \brief The module instance reference, if any.
		/// \details If this instance is the instantiation of a module instance, the 
		///		InstanceReference points back to the original instance in the instantiation module. 
		///		Instance refrences are rarely used, and almost never needed, and are only supported 
		///		to faithfully replicate XDL designs.
		InstanceReferenceSharedPtr mInstanceReferencePtr;
		/// \brief The map of pin names to InstancePin weak pointers.
		InstancePinMap mInstancePins;
	// functions
		/// \brief Add the referenced InstancePin to our pin map.
		void addPin(const InstancePinWeakPtr& inInstancePinPtr);
		/// \brief Remove the referenced InstancePin from our pin map.
		void removePin(const InstancePinWeakPtr& inInstancePinPtr);
	// constructors
		/// \brief Protected constructor.
		/// \param inName The instance name.
		/// \param inType The instance type.
		/// \param inTile The instance tile, or an empty string if unplaced.
		/// \param inSite The instance site, or an empty string if unplaced.
		/// \param inBonding The specified bonding for unplaced IO, or eInstanceBondingUnknown for 
		///		placed IO instances and non-IO instances.
		/// \param inInstanceReferencePtr The module instance reference, if applicable.  The 
		///		default value is almost always correct.  Note that instance references are never 
		///		required for XDL, and are only supported to completely replicated the original 
		///		design.
		Instance(const string& inName, const string& inType, const string& inTile, 
			const string& inSite, EInstanceBonding inBonding, 
			InstanceReferenceSharedPtr& inInstanceReferencePtr) : Renamable<class Circuit>(inName), 
			mType(inType), mTile(inTile), mSite(inSite), mBonding(inBonding), 
			mInstanceReferencePtr(inInstanceReferencePtr) {}
	public:
	// types
		/// \brief Constant iterator to InstancePin shared pointers.
		typedef InstancePinMap::const_iterator InstancePinSharedPtrConstIterator;
		/// \brief Non-constant iterator to InstancePin shared pointers.
		typedef InstancePinMap::iterator InstancePinSharedPtrIterator;
	// operators
		/// \brief Equality operator.
		/// \details This function deems instances equal if their names are identical.
		/// \param rhs The instance to compare against.
		/// \returns true if both instance names are identical, or false otherwise.
		bool operator ==(const Instance& rhs) const { return mName == rhs.mName; }
		/// \brief Unplace the instance.
		/// \details The instance is unplaced by clearing its site and tile.
	// functions
		/// \brief Unplace this instance by clearing its site and tile fields.
		void unplace(void) {
			mSite.clear();
			mTile.clear();
		}
		/// \brief Returns an InstancePin iterator for the requested pin.
		const InstancePinSharedPtrConstIterator findPin(const PinName& inPinName) const {
			InstancePinSharedPtrConstIterator p = mInstancePins.find(inPinName);
			return p;
		}
		/// \brief Returns a range that encompasses all instance pins for the given pin.
		/// \returns A constance iterator pair that encompasses all instance pins for the given 
		///		pins.  Refer to std::pair to determine how to extract the iterators.
		std::pair<InstancePinSharedPtrConstIterator, InstancePinSharedPtrConstIterator> 
			findPinRange(const PinName& inPinName) const {
			return mInstancePins.equal_range(inPinName);
		}
		/// \brief Returns the number of instance pins associated with the given pin.
		size_t getPinCount(const string& inPinName) const 
			{ return mInstancePins.count(inPinName); }
	// accessors
		/// \brief Returns the logic type for this instance.
		const string& getType(void) const { return mType; }
		/// \brief Returns the placement tile for this instance.
		const string& getTile(void) const { return mTile; }
		/// \brief Returns the placement site for this instance.
		const string& getSite(void) const { return mSite; }
		/// \brief Returns the IO bonding for this instance.
		EInstanceBonding getBonding(void) const { return mBonding; }
		/// \brief Returns the instance reference for this instance, if any.
		InstanceReferenceSharedPtr getInstanceReferencePtr(void) const 
			{ return mInstanceReferencePtr; }
		/// \brief Sets the logic type for this instance.
		void setType(const string& inType) { mType = inType; }
		/// \brief Sets the placement tile for this instance.
		void setTile(const string& inTile) { mTile = inTile; }
		/// \brief Sets the placement site for this instance.
		void setSite(const string& inSite) { mSite = inSite; }
		/// \brief Sets the IO bonding for this instance.
		/// \details The bonding must be EInstanceBonding::eInstanceBondingUnknown unless this 
		///		instance is an unplaced IO.  For an unplaced IO, the bonding setting determines 
		///		whether the instance will be placed on a bonded pad or an unbonded pad.
		void setBonding(EInstanceBonding inBonding) { mBonding = inBonding; }
		/// \brief Sets the instance reference for this instance.
		/// \details If this instance is the instantiation of a module instance, the 
		///		InstanceReference points back to the original instance in the instantiation module. 
		///		Instance refrences are rarely used, and almost never needed, and are only supported 
		///		to faithfully replicate XDL designs.
		void setInstanceReferencePtr(InstanceReferenceSharedPtr inInstanceReferenceSharedPtr) 
			{ mInstanceReferencePtr = inInstanceReferenceSharedPtr; }
	// iterators
		/// \brief Returns the begin constant iterator for instance pins.
		InstancePinSharedPtrConstIterator pinsBegin(void) const { return mInstancePins.begin(); }
		/// \brief Returns the end constant iterator for instance pins.
		InstancePinSharedPtrConstIterator pinsEnd(void) const { return mInstancePins.end(); }
		/// \brief Returns the begin non-constant iterator for instance pins.
		InstancePinSharedPtrIterator pinsBegin(void) { return mInstancePins.begin(); }
		/// \brief Returns the end non-constant iterator for instance pins.
		InstancePinSharedPtrIterator pinsEnd(void) { return mInstancePins.end(); }
		/// \brief Returns the number of instance pins in the instance.
		size_t getPinCount(void) const { return mInstancePins.size(); }
	};

	/// \brief Shared pointer encapsulation of an Instance.
	typedef boost::shared_ptr<Instance> InstanceSharedPtr;

	/// \brief Weak pointer encapsulation of an Instance.
	typedef boost::weak_ptr<Instance> InstanceWeakPtr;

	/// \brief Vector of Instance shared pointers.
	typedef std::vector<InstanceSharedPtr> InstanceSharedPtrVector;

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_INSTANCE_HPP
