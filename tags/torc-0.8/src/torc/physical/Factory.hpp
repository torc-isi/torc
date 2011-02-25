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
/// \brief Header for the Factory class.

#ifndef TORC_PHYSICAL_FACTORY_HPP
#define TORC_PHYSICAL_FACTORY_HPP

#include "torc/physical/Design.hpp"
#include <string>
#include <boost/smart_ptr.hpp>

namespace torc {
namespace physical {

	/// \brief Factory class for physical netlist elements.
	/// \details Physical netlist elements must be created by this factory.
	class Factory {
	protected:
	// friends
		/// \brief Imported type name.
		typedef std::string string;
	public:
	// functions
		/// \brief Create and return a new Design shared pointer.
		/// \param inName The design name.
		/// \param inDevice The design device.
		/// \param inPackage The device package.
		/// \param inSpeedGrade The device speed grade.
		/// \param inXdlVersion The design XDL version.
		static DesignSharedPtr newDesignPtr(const string& inName, const string& inDevice, 
			const string& inPackage, const string& inSpeedGrade, const string& inXdlVersion) {
			DesignSharedPtr designPtr(new Design(inName, inDevice, inPackage, inSpeedGrade, 
				inXdlVersion));
			designPtr->setSelfWeakPtr(designPtr);
			return designPtr;
		}
		/// \brief Create and return a new Module shared pointer.
		/// \param inName The module name.
		/// \param inAnchor The anchor instance name for the module.  The anchor designates the 
		///		instance in the module relative to which the module will be placed.
		static ModuleSharedPtr newModulePtr(const string& inName, const string& inAnchor) {
			ModuleSharedPtr modulePtr(new Module(inName, inAnchor));
			modulePtr->Progenitor<Module>::setSelfWeakPtr(modulePtr);
			return modulePtr;

		}
		/// \brief Create and return a new Port shared pointer.
		/// \param inName The port name.
		/// \param inInstancePtr The port instance pointer.
		/// \param inPinName The port pin name.
		static PortSharedPtr newPortPtr(const string& inName, InstanceSharedPtr inInstancePtr, 
			const string& inPinName) {
			PortSharedPtr portPtr(new Port(inName, inInstancePtr, inPinName));
			return portPtr;
		}
		/// \brief Construct and return a new Instance shared pointer.
		/// \param inName The instance name.
		/// \param inType The instance type.
		/// \param inTile The instance tile, or an empty string if unplaced.
		/// \param inSite The instance site, or an empty string if unplaced.
		/// \param inBonding The specified bonding for unplaced IO, or eInstanceBondingUnknown for 
		///		placed IO instances and non-IO instances.  The default value is almost always 
		///		correct.
		/// \param inInstanceReferencePtr The module instance reference, if applicable.  The 
		///		default value is almost always correct.  Note that instance references are never 
		///		required for XDL, and are only supported to faithfully replicate XDL designs.
		static InstanceSharedPtr newInstancePtr(const string& inName, const string& inType, 
			const string& inTile, const string& inSite, 
			EInstanceBonding inBonding = eInstanceBondingUnknown, 
			InstanceReferenceSharedPtr inInstanceReferencePtr = InstanceReferenceSharedPtr()) {
			InstanceSharedPtr instancePtr(new Instance(inName, inType, inTile, inSite, inBonding, 
				inInstanceReferencePtr));
			instancePtr->setSelfWeakPtr(instancePtr);
			return instancePtr;
		}
		/// \brief Create and return a new Net share pointer.
		/// \param inName The net name.
		/// \param inNetType The net power type.
		static NetSharedPtr newNetPtr(const string& inName, ENetType inNetType = eNetTypeNormal) {
			NetSharedPtr netPtr(new Net(inName, inNetType));
			netPtr->setSelfWeakPtr(netPtr);
			return netPtr;
		}
		/// \brief Create and return a new InstanceReference shared pointer.
		/// \param inInstantiationName The name under which the reference module was instantiated.
		/// \param inModulePtr The module containing the referenced instance.
		/// \param inInstancePtr The instance being referenced.
		static InstanceReferenceSharedPtr newInstanceReferencePtr(const string& inInstantiationName,
			ModuleSharedPtr inModulePtr, InstanceSharedPtr inInstancePtr) {
			InstanceReferenceSharedPtr instanceReferencePtr(
				new InstanceReference(inInstantiationName, inModulePtr, inInstancePtr));
			return instanceReferencePtr;
		}
		/// \brief Construct and return a new InstancePin shared pointer.
		/// \param inInstancePtr The pin instance pointer.
		/// \param inPinName The pin name.
		static InstancePinSharedPtr newInstancePinPtr(InstanceSharedPtr inInstancePtr, 
			const string& inPinName) {
			InstancePinSharedPtr instancePinPtr(new InstancePin(inInstancePtr, inPinName));
			instancePinPtr->setSelfWeakPtr(instancePinPtr);
			return instancePinPtr;
		}
		/// \brief Construct a pip and return it.
		/// \param inTileName The containing tile.
		/// \param inSourceWireName The source wire.
		/// \param inSinkWireName The sink wire.
		/// \param inPipDirection The pip directionality.  See EPipDirection.  The most common 
		///		value is ePipUnidirectionalBuffered, which represents a unidirectional buffered 
		///		connection from source to sink.
		/// \param inRoutethroughPtr The routethrough shared pointer.  The default value creates 
		///		no routethrough.
		static torc::physical::Pip newPip(const string& inTileName, const string& inSourceWireName, 
			const string& inSinkWireName, EPipDirection inPipDirection, 
			RoutethroughSharedPtr inRoutethroughPtr = RoutethroughSharedPtr()) {
			return Pip::Pip(inTileName, inSourceWireName, inSinkWireName, inPipDirection, 
				inRoutethroughPtr);
		}
		/// \brief Construct and return a new Routethrough shared pointer.
		static RoutethroughSharedPtr newRoutethroughPtr(const string& inSetting, 
			const string& inName, const string& inValue, const InstanceWeakPtr& inInstancePtr, 
			const string& inSourceWireName, const string& inSinkWireName) {
			RoutethroughSharedPtr routethroughPtr(new Routethrough(inSetting, inName, inValue, 
				inInstancePtr, inSourceWireName, inSinkWireName));
			return routethroughPtr;
		}
	};

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_FACTORY_HPP
