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

/// \file
/// \brief Header for the InstancePin class.

#ifndef TORC_ARCHITECTURE_INSTANCEPIN_HPP
#define TORC_ARCHITECTURE_INSTANCEPIN_HPP

#include "torc/physical/InstancePin.hpp"
#include "torc/architecture/Tilewire.hpp"

namespace torc {
namespace architecture {

	/// \brief Physical design instance-pin pair, suitable for specifying a net endpoint.
	/// \details This class is a Tilewire-aware version of torc::physical::InstancePin.  It 
	///		functions as an encapsulation of an InstancePin and its corresponding Tilewire.
	class InstancePin : public torc::physical::InstancePinBase, public Tilewire {
	protected:
	// constructors
		/// \brief Protected constructor.
		/// \param inInstancePtr The pin instance pointer.
		/// \param inPinName The pin name.
		InstancePin(torc::physical::InstanceSharedPtr inInstancePtr, const string& inPinName) 
			: InstancePinBase(inInstancePtr, inPinName), Tilewire() {}
	private:
	// constructors
		/// \brief Disabled copy constructor.
		InstancePin(const InstancePin& rhs) : InstancePinBase(rhs), Tilewire() {}
	public:
	// typedefs
		typedef boost::shared_ptr<class ::torc::architecture::InstancePin> ArchitectureInstancePinPtr;
		typedef boost::shared_ptr<class ::torc::physical::InstancePin> PhysicalInstancePinPtr;
	// statics
		/// \brief Typecasts a torc::physical::InstancePin to a torc::architecture::InstancePin.
		inline static ArchitectureInstancePinPtr& physicalToArchitecture(
			PhysicalInstancePinPtr& inInstancePinSharedPtr) {
			return reinterpret_cast<ArchitectureInstancePinPtr&>(inInstancePinSharedPtr);
		}
		inline static PhysicalInstancePinPtr& architectureToPhysical(
			ArchitectureInstancePinPtr& inInstancePinSharedPtr) {
			return reinterpret_cast<PhysicalInstancePinPtr&>(inInstancePinSharedPtr);
		}
	// functions
		/// \brief Updates the Tilewire from the instance and pin names.
		void updateTilewire(class DDB& inDDB);
	// accessors
		/// \brief Returns the Tilewire designated by this instance pin.
		Tilewire& getTilewire(void) { return static_cast<Tilewire&>(*this); }
	};

	/// \brief Shared pointer encapsulation of an InstancePin.
	typedef boost::shared_ptr<InstancePin> InstancePinSharedPtr;

	/// \brief Weak pointer encapsulation of an InstancePin.
	typedef boost::weak_ptr<InstancePin> InstancePinWeakPtr;

	/// \brief Vector of InstancePin shared pointers.
	typedef std::vector<InstancePinSharedPtr> InstancePinSharedPtrVector;

	/// \brief Reinterpret the given torc::physical::InstancePinSharedPtr as a 
	///		torc::architecture::InstancePinSharedPtr.
	#define INSTANCE_PIN_PHYSICAL_TO_ARCHITECTURE(x) \
		::torc::architecture::InstancePin::physicalToArchitecture(x)

	/// \brief Reinterpret the given torc::architecture::InstancePinSharedPtr as a 
	///		torc::physical::InstancePinSharedPtr.
	#define INSTANCE_PIN_ARCHITECTURE_TO_PHYSICAL(x) \
		::torc::architecture::InstancePin::architectureToPhysical(x)

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_INSTANCEPIN_HPP
