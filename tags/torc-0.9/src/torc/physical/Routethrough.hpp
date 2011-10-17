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
/// \brief Header for the Routethrough class.

#ifndef TORC_PHYSICAL_ROUTETHROUGH_HPP
#define TORC_PHYSICAL_ROUTETHROUGH_HPP

#include "torc/physical/Config.hpp"
#include "torc/physical/Instance.hpp"
#include <string>

namespace torc {
namespace physical {

	/// \brief Pip routethrough. 
	/// \details A routethrough defines how a signal passes through logic resources on its way to 
	///		wiring resources.
	class Routethrough : public Config {
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
	// members
		/// \brief The _ROUTETHROUGH setting in the associated instance.
		string mSetting;
		/// \brief The instance in which the routethrough is implemented.
		InstanceWeakPtr mInstancePtr;
		/// \brief The routethrough instance source wire.
		WireName mSourceWireName;
		/// \brief The routethrough instance sink wire.
		WireName mSinkWireName;
	public:
	// constructors
		/// \brief Public constructor.
		/// \param inSetting The _ROUTETHROUGH setting in the associated instance.
		/// \param inName The _ROUTETHROUGH name in the associated instance.
		/// \param inValue The _ROUTETHROUGH value in the associated instance.
		/// \param inInstancePtr The instance weak pointer.
		/// \param inSourceWireName The instance source wire name.
		/// \param inSinkWireName The instance sink wire name.
		Routethrough(const string& inSetting, const string& inName, const string& inValue, 
			const InstanceWeakPtr& inInstancePtr, const string& inSourceWireName, 
			const string& inSinkWireName) 
			: Config(inName, inValue), mSetting(inSetting), mInstancePtr(inInstancePtr), 
			mSourceWireName(inSourceWireName), mSinkWireName(inSinkWireName) {}
	// accessors
		/// \brief Returns the _ROUTETHROUGH setting.
		const string& getSetting(void) const { return mSetting; }
		/// \brief Returns the instance weak pointer.
		const InstanceWeakPtr& getInstancePtr(void) const { return mInstancePtr; }
		/// \brief Returns the instance source wire name.
		const WireName& getSourceWireName(void) const { return mSourceWireName; }
		/// \brief Returns the instance sink wire name.
		const WireName& getSinkWireName(void) const { return mSinkWireName; }
	};

	/// \brief Shared pointer encapsulation of a Routethrough.
	typedef boost::shared_ptr<Routethrough> RoutethroughSharedPtr;

	/// \brief Weak pointer encapsulation of a Routethrough.
	typedef boost::weak_ptr<Routethrough> RoutethroughWeakPtr;

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_ROUTETHROUGH_HPP
