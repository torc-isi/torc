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
/// \brief Header for the Port class.

#ifndef TORC_PHYSICAL_PORT_HPP
#define TORC_PHYSICAL_PORT_HPP

#include "torc/physical/Named.hpp"
#include "torc/physical/Progeny.hpp"
#include "torc/physical/Instance.hpp"
#include <boost/smart_ptr.hpp>

// forward declaration of XdlParser within its namespace
namespace torc {
	class XdlParser;
} // namespace torc

namespace torc {
namespace physical {

	/// \brief Module input or output port.
	/// \details This class declares a named port for the enclosing module.
	class Port : public Named, public Progeny<class Module> {
	// friends
		/// \brief The Factory class has direct access to our internals.
		friend class Factory;
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
	// members
		/// \brief The port instance pointer.
		InstanceWeakPtr mInstancePtr;
		/// \brief The port pin name.
		PinName mPinName;
	// constructors
		/// \brief Protected constructor.
		/// \param inName The port name.
		/// \param inInstancePtr The port instance pointer.
		/// \param inPinName The port pin name.
		Port(const string& inName, InstanceSharedPtr inInstancePtr, const string& inPinName) 
			: Named(inName), mInstancePtr(inInstancePtr), mPinName(inPinName) {}
	public:
	// accessors
		/// \brief Returns a weak instance pointer.
		const InstanceWeakPtr& getInstancePtr(void) const { return mInstancePtr; }
		/// \brief Returns the pin name.
		const PinName& getPinName(void) const { return mPinName; }
	// operators
		/// \brief Equality operator.
		/// \details This function deems ports equal if their names are identical.
		/// \returns true if both port names are identical, or false otherwise.
		bool operator ==(const Port& rhs) const { return mName == rhs.mName; }
	};

	/// \brief Shared pointer encapsulation of a Port.
	typedef boost::shared_ptr<Port> PortSharedPtr;

	/// \brief Weak pointer encapsulation of a Port.
	typedef boost::weak_ptr<Port> PortWeakPtr;

	/// \brief Vector of Port shared pointers.
	typedef std::vector<PortSharedPtr> PortSharedPtrVector;

	/// \brief Temporary module port.
	/// \details This class should only be used by XdlParser to remember the port while awaiting 
	///		the definition of the port's instance.
	class PortTemp {
		/// \brief The XdlParser class has direct access to our internals.
		friend class torc::XdlParser;
	// types
		/// \brief Imported type name.
		typedef std::string string;
	protected:
	// members
		/// \brief The port name.
		string mName;
		/// \brief The port instance.
		string mInstance;
		/// \brief The port pin.
		string mPin;
	// constructors
		/// \param inName The port name.
		/// \param inInstance The port instance.
		/// \param inPin The port pin.
		PortTemp(const string& inName, const string& inInstance, const string& inPin) 
			: mName(inName), mInstance(inInstance), mPin(inPin) {}
	// accessors
		/// \brief Returns the port name.
		const string& getName(void) const { return mName; }
		/// \brief Returns the port instance.
		const string& getInstance(void) const { return mInstance; }
		/// \brief Returns the port instance pin.
		const string& getPin(void) const { return mPin; }
	};

	/// \brief Vector 
	typedef std::vector<PortTemp> PortTempVector;


} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_PORT_HPP
