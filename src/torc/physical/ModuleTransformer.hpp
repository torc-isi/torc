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
/// \brief ModuleTransformer class to modularize/flatten designs.

#ifndef TORC_PHYSICAL_MODULETRANSFORMER_HPP
#define TORC_PHYSICAL_MODULETRANSFORMER_HPP

#include "torc/Architecture.hpp"
#include "torc/physical/Factory.hpp"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace torc::architecture;
using namespace torc::architecture::xilinx;

namespace torc {
namespace physical {

	/// \brief Utility class to modularize/flatten designs.
	class ModuleTransformer {

	protected:
	// typedefs
		/// \brief Imported type.
		typedef Design::InstanceSharedPtrConstIterator InstanceSharedPtrConstIterator;
		/// \brief Imported type.
		typedef Design::NetSharedPtrIterator NetSharedPtrIterator;
		/// \brief Imported type.
		typedef Design::ModuleSharedPtrIterator ModuleSharedPtrIterator;
		/// \brief Imported type.
		typedef Design::InstanceSharedPtrIterator InstanceSharedPtrIterator;
		/// \brief Imported type.
		typedef Module::PortSharedPtrConstIterator PortSharedPtrConstIterator;
		/// \brief Imported type.
		typedef Net::InstancePinSharedPtrIterator InstancePinSharedPtrIterator;
		/// \brief Imported type.
		typedef Net::InstancePinSharedPtrConstIterator InstancePinSharedPtrConstIterator;
		/// \brief Imported type.
		typedef Net::PipConstIterator PipConstIterator;
	// members
		/// \brief Design pointer.
		DesignSharedPtr mDesignPtr;
		/// \brief Hierarchy separator.
		static const string sHierarchySeparator;
		/// \brief Port index separator.
		static const string sPortIndexSeparator;
		/// \brief Valid characters in a pin name.
		static const boost::regex sValidPinNameCharactersRegEx;
		/// \brief Invalid first characters in a pin name.
		static const boost::regex sInvalidPinNameFirstCharactersRegEx;
	// types
		/// \brief Net connectivity with respect to module instance, either not connected
		///		to module, internal to module, or external to module.
		enum ENetConnectivity {
			eNetConnectivityNotConnected = 0, eNetConnectivityInternal, eNetConnectivityExternal
		};

	public:
		// constructors
		/// \brief Public constructor.
		/// \param inDesignPtr Pointer to the design.
		ModuleTransformer(DesignSharedPtr& inDesignPtr) : mDesignPtr(inDesignPtr) {}

		/// \brief Group a set of instances into a module.
		/// \param inInstances The vector of instances to group.
		/// \param inModuleDefinitionName The module definition name.
		/// \param inModuleInstanceName The module instance name.
		/// \param inKeepPlacement A flag specifying whether placement should be retained after 
		///		modularization.  Default to false.
		/// \param inKeepRouting A flag specifying whether routing should be retained afer 
		///		modularization.  Defaults to false.
		/*
		 * Function pseudocode
		 *
		 * bool modularize(InstanceSharedPtrVector inInstances,
		 *                 string inModuleDefinitionName,
		 *                 string inModuleInstanceNamebool,
		 *                 bool inKeepPlacement = false,
		 *                 bool inKeepRouting = false)
		 * {
		 *   if(inInstances size == 0)
		 *   {
		 *     warn user that inInstances is empty;
		 *     return false;
		 *   }
		 *   create module definition, anchor point is first instance in inInstances;
		 *   create module instance, placed where anchor point is placed;
		 *   //handle instances
		 *   iterate over inInstances
		 *   {
		 *     if(instance not found)
		 *     {
		 *       warn user that instance not found;
		 *       return false;
		 *     }
		 *     if(!inKeepPlacement) unplace instance;
		 *     move instance from design to module definition;
		 *   }
		 *   //handle nets
		 *   iterate over design nets
		 *   {
		 *     if(!inKeepRouting) unroute net;
		 *     if(net is internal to module)
		 *     {
		 *       move net from design to module definition;
		 *     }
		 *     else if(net is external to module)
		 *     {
		 *       //handle net sources
		 *       iterate over net sources
		 *       {
		 *         if(source connects to instance in module definition)
		 *         {
		 *           create port for module definition;
		 *           update net outpin;
		 *         }
		 *       }
		 *       //handle net sinks
		 *       iterate over net sinks
		 *       {
		 *         if(sink connects to instance in module definition)
		 *         {
		 *           create port for module definition;
		 *           update net inpin;
		 *         }
		 *       }
		 *     }
		 *   }
		 *   add module definition and instance to design;
		 *   return true;
		 * }
		 */
		bool modularize(const InstanceSharedPtrVector& inInstances, 
			const string& inModuleDefinitionName, const string& inModuleInstanceName, 
			bool inKeepPlacement = false, bool inKeepRouting = false) {

			// Get a current iterator to inInstances
			InstanceSharedPtrConstIterator inInstanceSharedPtrConstIter = inInstances.begin();
			// Get an end iterator to inInstances
			InstanceSharedPtrConstIterator inInstanceSharedPtrConstEnd = inInstances.end();

			// Check if inInstances is empty
			if(inInstanceSharedPtrConstIter == inInstanceSharedPtrConstEnd) {

				// Warn the user that none of the instances specified are found in the design
				std::clog << "WARNING: There were no instances specified to be modularized. "
					<< std::endl;

				// Operation did not complete successfully
				return false;

			}

			// Set the first returned instance as a reference point
			InstanceSharedPtr moduleReferenceInst = *inInstanceSharedPtrConstIter;
			// Create module definition
			ModuleSharedPtr moduleDefinition = Factory::newModulePtr(inModuleDefinitionName,
				moduleReferenceInst->getName());
			// Create module instance
			InstanceSharedPtr moduleInst = Factory::newInstancePtr(inModuleInstanceName,
				inModuleDefinitionName, moduleReferenceInst->getTile(),
				moduleReferenceInst->getSite());

			// Unplace module instance if so requested
			if(!inKeepPlacement) moduleInst->unplace();

			// Begin handle instances
			// Iterate over all instances in inInstances
			while(inInstanceSharedPtrConstIter != inInstanceSharedPtrConstEnd) {

				// Get a pointer to the current instance
				InstanceSharedPtr instPtr = *inInstanceSharedPtrConstIter;

				// Look for the current instance in the design
				InstanceSharedPtrConstIterator instPtrIter 
					= mDesignPtr->findInstance(instPtr->getName());

				// Warn if the instance was not found
				if(instPtrIter == mDesignPtr->instancesEnd()) {

					// Warn the user that the instance was not found in the design.
					std::clog << "WARNING: Instance " << instPtr->getName()
						<< "was not found in design " << mDesignPtr->getName() << std::endl;
					// Operation did not complete successfully
					return false;

				}

				// Get a pointer to the looked up instance
				InstanceSharedPtr designInstPtr = *instPtrIter;

				// Unplace instance if so requested
				if(!inKeepPlacement) designInstPtr->unplace();

				// Add instance to module
				moduleDefinition->addInstance(designInstPtr);
				// Remove instance from design
				mDesignPtr->removeInstance(designInstPtr);

				// Move to next instance
				inInstanceSharedPtrConstIter++;

			} // while(inInstanceSharedPtrConstIter != inInstanceSharedPtrConstEnd)
			// End handle instances

			// Begin handle connectivity
			// Get a begin iterator to design nets
			NetSharedPtrIterator designNetsIter = mDesignPtr->netsBegin();
			// Get an end iterator to design nets
			NetSharedPtrIterator designNetsEnd = mDesignPtr->netsEnd();
			// Get a begin iterator to module hosted instances
			InstanceSharedPtrConstIterator modHostedInstPtrConstBegin 
				= moduleDefinition->instancesBegin();
			// Get an end iterator to module hosted instances
			InstanceSharedPtrConstIterator modHostedInstPtrConstEnd 
				= moduleDefinition->instancesEnd();
			// Module instance source pins
			InstancePinSharedPtrVector modInstSourcePinPtrVector;
			// Module instance sink pins
			InstancePinSharedPtrVector modInstSinkPinPtrVector;

			// Iterate over all design nets
			while(designNetsIter != designNetsEnd) {

				// Get a pointer to the current net
				NetSharedPtr designNetPtr = *designNetsIter;
				// Compute net connectivity type
				ENetConnectivity eNet = getNetConnectivity(modHostedInstPtrConstBegin,
					modHostedInstPtrConstEnd, designNetPtr);

				// For internal nets, move net from design to module
				if(eNet == eNetConnectivityInternal) {

					// Unroute the net if so requested
					if(!inKeepRouting) designNetPtr->unroute();

					// Add net to module
					moduleDefinition->addNet(designNetPtr);
					// Remove net from design
					mDesignPtr->removeNet(designNetPtr);
					// Update designNetsEnd iterator
					designNetsEnd = mDesignPtr->netsEnd();

				} else {

					// For unconnected nets, increment the iterator and continue looping
					if(eNet == eNetConnectivityNotConnected) {

						// Move to next net
						designNetsIter++;
						continue;

					}

					// For external nets, expose corresponding pins and update connectivity
					// Unroute the net if so requested
					if(!inKeepRouting) designNetPtr->unroute();

					// Get a begin iterator to net sources
					InstancePinSharedPtrIterator designNetSourcesIter 
						= designNetPtr->sourcesBegin();
					// Get an end iterator to net sources
					InstancePinSharedPtrIterator designNetSourcesEnd = designNetPtr->sourcesEnd();
					// Wire index used when creating ports
					u_int32_t portIndex = 0;

					// Iterate over all net sources
					while(designNetSourcesIter != designNetSourcesEnd) {

						// Get a pointer to the current net source pin
						InstancePinSharedPtr instPinPtr = *designNetSourcesIter;
						// Get a pointer to the instance hosting the net source pin
						InstanceSharedPtr pinInstPtr = instPinPtr->getInstancePtr().lock();

						// Look for the instance in the vector of instances
						if((std::find(modHostedInstPtrConstBegin, modHostedInstPtrConstEnd,
							pinInstPtr)) != modHostedInstPtrConstEnd) {

							// Build port name: netName_portIndex
							string portName = sanitizePinName(designNetPtr->getName())
								+ sPortIndexSeparator 
								+ boost::lexical_cast<std::string>(portIndex++);
							// Create port
							PortSharedPtr portPtr = Factory::newPortPtr(portName, *(std::find(
								modHostedInstPtrConstBegin, modHostedInstPtrConstEnd,
								pinInstPtr)), instPinPtr->getPinName());
							// Add port to module
							moduleDefinition->addPort(portPtr);
							// Create source pin from the module instance
							modInstSourcePinPtrVector.push_back(
								Factory::newInstancePinPtr(moduleInst, portName));
							// Remove old source pin from net
							designNetPtr->removeSource(instPinPtr);
							// Update designNetSourcesEnd iterator
							designNetSourcesEnd = designNetPtr->sourcesEnd();

						} else {

							// Move to next source
							designNetSourcesIter++;

						}
					} // while(designNetSourcesIter != designNetSourcesEnd)

					// Get a begin iterator to module source pins
					InstancePinSharedPtrIterator modInstSourcesIter =
						modInstSourcePinPtrVector.begin();
					// Get an end iterator to module source pins
					InstancePinSharedPtrIterator modInstSourcesEnd =
						modInstSourcePinPtrVector.end();

					// Update net sources with new source pins
					while(modInstSourcesIter != modInstSourcesEnd) {

						// Add source pin to net
						designNetPtr->addSource(*modInstSourcesIter);
						// Move to next source
						modInstSourcesIter++;

					}

					// Reset module instance source pins vector
					modInstSourcePinPtrVector.clear();
					// Get a begin iterator to net sinks
					InstancePinSharedPtrIterator designNetSinksIter = designNetPtr->sinksBegin();
					// Get an end iterator to net sinks
					InstancePinSharedPtrIterator designNetSinksEnd = designNetPtr->sinksEnd();

					// Iterate over all net sinks
					while(designNetSinksIter != designNetSinksEnd) {

						// Get a pointer to the current net sink pin
						InstancePinSharedPtr instPinPtr = *designNetSinksIter;
						// Get a pointer to the instance hosting the net source pin
						InstanceSharedPtr pinInstPtr = instPinPtr->getInstancePtr().lock();

						// Look for the instance in the vector of instances
						if((std::find(modHostedInstPtrConstBegin, modHostedInstPtrConstEnd,
							pinInstPtr)) != modHostedInstPtrConstEnd) {

							// Build port name: netName_portIndex
							string portName = sanitizePinName(designNetPtr->getName())
								+ sPortIndexSeparator + boost::lexical_cast<std::string>(
								portIndex++);
							// Create port
							PortSharedPtr portPtr = Factory::newPortPtr(portName, *(std::find(
								modHostedInstPtrConstBegin, modHostedInstPtrConstEnd,
								pinInstPtr)), instPinPtr->getPinName());
							// Add port to module
							moduleDefinition->addPort(portPtr);
							// Create sink pin to the module instance
							modInstSinkPinPtrVector.push_back(Factory::newInstancePinPtr(
								moduleInst, portName));
							// Remove old sink pin from net
							designNetPtr->removeSink(instPinPtr);
							// Update designNetSinksEnd iterator
							designNetSinksEnd = designNetPtr->sinksEnd();

						} else {

							// Move to next sink
							designNetSinksIter++;

						}
					} // while(designNetSinksIter != designNetSinksEnd)

					// Get a begin iterator to module sink pins
					InstancePinSharedPtrIterator modInstSinksIter =
						modInstSinkPinPtrVector.begin();
					// Get an end iterator to module sink pins
					InstancePinSharedPtrIterator modInstSinksEnd =
						modInstSinkPinPtrVector.end();

					// Update net sources with new source pins
					while(modInstSinksIter != modInstSinksEnd) {

						// Add sink pin to net
						designNetPtr->addSink(*modInstSinksIter);
						// Move to next sink
						modInstSinksIter++;

					}

					// Reset module instance sink pins vector
					modInstSinkPinPtrVector.clear();

					// Move to next net
					designNetsIter++;

				} // else
			} // while(designNetsIter != designNetsEnd)
			// End handle connectivity

			// Add module to design
			mDesignPtr->addModule(moduleDefinition);
			// Add module instance to design
			mDesignPtr->addInstance(moduleInst);

			// Operation successfully completed
			return true;
		}

		/// \brief Flatten a module instance in a design.
		/// \param inModuleInstanceName The name of the module instance to be flattened.
		/// \param inKeepPlacement A flag specifying whether placement should be retained after 
		///		flattening.  Default to false.
		/// \param inKeepRouting A flag specifying whether routing should be retained afer 
		///		flattening.  Defaults to false.
		/*
		 * Function pseudocode
		 *
		 * bool flatten(const string& inModuleInstanceName, bool inKeepPlacement = false,
		 * 				bool inKeepRouting = false)
		 * {
		 *   locate module instance in design;
		 *   if(module instance not found)
		 *   {
		 *     warn user that module instance was not found;
		 *     return false;
		 *   }
		 *   locate module definition corresponding to module instance in design;
		 *   if(module definition not found)
		 *   {
		 *     warn user that module definition was not found;
		 *     return false;
		 *   }
		 *   //handle instances
		 *   iterate over module definition instances
		 *   {
		 *     clone every instance and set its name to moduleInstance/instanceName;
		 *     if(!inKeepPlacement) unplace instance;
		 *     add instance clone to design;
		 *   }
		 *   //handle internal nets
		 *   iterate over module definition internal nets
		 *   {
		 *     clone every net and set its name to moduleInstance/netName;
		 *     if(!inKeepRouting) unroute net;
		 *     add net clone to design;
		 *   }
		 *   //handle external nets
		 *   iterate over all design nets
		 *   {
		 *     if(!inKeepRouting) unroute net;
		 *     //handle net sources
		 *     iterate over all sources of a net
		 *     {
		 *       if(source is a port of the module)
		 *       {
		 *         update net outpin to reflect the flattened instance and the corresponding pin 
		 *           that matches the port;
		 *       }
		 *     }
		 *     //handle net sinks
		 *     iterate over all sinks of a net
		 *     {
		 *       if(sink is a port of the module)
		 *       {
		 *         update net inpin to reflect the flattened instance and the corresponding pin 
		 *           that matches the port;
		 *       }
		 *     }
		 *   }
		 *   remove module instance from design;
		 * }
		 */
		bool flatten(const string& inModuleInstanceName, bool inKeepPlacement = false,
			bool inKeepRouting = false) {

			// Flattened module source pins
			InstancePinSharedPtrVector flattenedModuleSourcePinPtrVector;
			// Flattened module sink pins
			InstancePinSharedPtrVector flattenedModuleSinkPinPtrVector;
			// Look for the module instance in the design
			InstanceSharedPtrConstIterator designModInstPtrConstIter 
				= mDesignPtr->findInstance(inModuleInstanceName);

			// Check for a non-existent module instance
			if(designModInstPtrConstIter == mDesignPtr->instancesEnd()) {

				// Warn the user that the module instance was not found in the design
				std::clog << "WARNING: Module instance of name " << inModuleInstanceName
					<< " was not found in design " << mDesignPtr->getName() << "." << std::endl;

				// Operation did not complete successfully
				return false;

			}

			// Get a pointer to the instance
			InstanceSharedPtr instPtr = *designModInstPtrConstIter;
			// Look for the module in the design
			ModuleSharedPtrIterator designModPtrConstIter 
				= mDesignPtr->findModule(instPtr->getType());

			// Check for a non-existent module
			if(designModPtrConstIter == mDesignPtr->modulesEnd()) {

				// Warn the user that the module was not found in the design
				std::clog << "WARNING: Module of name " << instPtr->getType()
					<< " was not found in design " << mDesignPtr->getName() << "." << std::endl;

				// Operation did not complete successfully
				return false;

			}

			// Current design module
			ModuleSharedPtr module = *designModPtrConstIter;

			// Check whether tile/site for module instance is same as tile/site for module def.
			// In case they don't match, the flattened module instance will be unplaced and any
			// external/internal nets to the module instance will be unrouted.
			InstanceSharedPtrIterator moduleAnchorInstIterator 
				= module->findInstance(module->getAnchor());
			InstanceSharedPtr moduleAnchorInst = *moduleAnchorInstIterator;
			// Begin handle instances inside module
			// Get a begin iterator to module instances
			InstanceSharedPtrIterator moduleInstancesIter = module->instancesBegin();
			// Get an end iterator to module instances
			InstanceSharedPtrIterator moduleInstancesEnd = module->instancesEnd();

			// Iterate over all instances hosted by the module
			while(moduleInstancesIter != moduleInstancesEnd) {

				// Get a pointer to the current instance hosted by the module
				InstanceSharedPtr modInstPtr = *moduleInstancesIter;
				// Clone the instance
				InstanceSharedPtr modInstPtrClone = cloneInstance(modInstPtr,
					inModuleInstanceName + sHierarchySeparator + modInstPtr->getName());
				// Set clone instance reference
				modInstPtrClone->setInstanceReferencePtr(Factory::newInstanceReferencePtr(
					inModuleInstanceName, module, modInstPtr));

				// Unplace the instance if so requested
				if(!inKeepPlacement) modInstPtrClone->unplace();

				// Add clone of instance to design
				mDesignPtr->addInstance(modInstPtrClone);

				// Next instance
				moduleInstancesIter++;

			} // while(moduleInstancesIter != moduleInstancesEnd)
			// End handle instances inside module

			// Begin handle internal module nets
			// Get a begin iterator to module nets
			NetSharedPtrIterator moduleNetsIter = module->netsBegin();
			// Get an end iterator to module nets
			NetSharedPtrIterator moduleNetsEnd = module->netsEnd();

			// Iterate over all nets inside module
			while(moduleNetsIter != moduleNetsEnd) {

				// Get a pointer to the current net
				NetSharedPtr moduleNetPtr = *moduleNetsIter;
				// Clone the net
				NetSharedPtr moduleNetPtrClone = cloneNet(moduleNetPtr, inModuleInstanceName
					+ sHierarchySeparator + moduleNetPtr->getName(), inModuleInstanceName);

				// Unroute the net if so requested
				if(!inKeepRouting) moduleNetPtrClone->unroute();

				// Add net to design
				mDesignPtr->addNet(moduleNetPtrClone);
				// Next net
				moduleNetsIter++;

			} // End handle internal module nets

			// Begin handle external module nets
			// Get a begin iterator to design nets
			NetSharedPtrIterator designNetsIter = mDesignPtr->netsBegin();
			// Get an end iterator to design nets
			NetSharedPtrIterator designNetsEnd = mDesignPtr->netsEnd();

			// Iterate over all nets connected to module and other instances outside module
			while(designNetsIter != designNetsEnd) {

				// Get a pointer to the current net
				NetSharedPtr designNetPtr = *designNetsIter;

				// Get a begin iterator to net sources
				InstancePinSharedPtrIterator designNetSourcesIter = designNetPtr->sourcesBegin();
				// Get an end iterator to net sources
				InstancePinSharedPtrIterator designNetSourcesEnd = designNetPtr->sourcesEnd();

				// Iterate over all net sources
				while(designNetSourcesIter != designNetSourcesEnd) {

					// Net pin
					InstancePinSharedPtr instPinPtr = *designNetSourcesIter;
					// Pin instance
					InstanceSharedPtr pinInstPtr = instPinPtr->getInstancePtr().lock();

					// Net connects to module instance
					if(pinInstPtr == instPtr) {

						// Unroute the net if so requested
						if(!inKeepRouting) designNetPtr->unroute();

						// Look for the port in the module
						PortSharedPtrConstIterator modulePortConstIter 
							= module->findPort(instPinPtr->getPinName());

						// Check if port was found
						if(modulePortConstIter != module->portsEnd()) {

							// Get a pointer to the current port
							PortSharedPtr modPortPtr = *modulePortConstIter;
							// Get module port reference instance
							InstanceSharedPtr portInstPtr = modPortPtr->getInstancePtr().lock();
							// Find the cloned instance corresponding to the pin reference instance
							InstanceSharedPtrIterator pinInstPtrCloneItr 
								= mDesignPtr->findInstance(inModuleInstanceName
								+ sHierarchySeparator + portInstPtr->getName());
							// Create new pin referencing the port reference instance
							flattenedModuleSourcePinPtrVector.push_back(
								Factory::newInstancePinPtr(*pinInstPtrCloneItr,
								modPortPtr->getPinName()));
							// Remove pin reference module instance
							designNetPtr->removeSource(instPinPtr);
							// Update designNetSourcesEnd iterator
							designNetSourcesEnd = designNetPtr->sourcesEnd();

						}

					} else {

						// Move to next source
						designNetSourcesIter++;

					}
				} // while(designNetSourcesIter != designNetSourcesEnd)

				// Get a begin iterator to flattened module source pins
				InstancePinSharedPtrIterator flattenedModuleSourcesIter 
					= flattenedModuleSourcePinPtrVector.begin();
				// Get an end iterator to flattened module source pins
				InstancePinSharedPtrIterator flattenedModuleSourcesEnd 
					= flattenedModuleSourcePinPtrVector.end();

				// Update net sources with new source pins
				while(flattenedModuleSourcesIter != flattenedModuleSourcesEnd) {

					// Add source pin to net
					designNetPtr->addSource(*flattenedModuleSourcesIter);
					// Move to next source
					flattenedModuleSourcesIter++;
				}

				// Reset vector
				flattenedModuleSourcePinPtrVector.clear();
				// Get a begin iterator to net sinks
				InstancePinSharedPtrIterator designNetSinksIter = designNetPtr->sinksBegin();
				// Get an end iterator to net sinks
				InstancePinSharedPtrIterator designNetSinksEnd = designNetPtr->sinksEnd();

				// Iterate over all net sinks
				while(designNetSinksIter != designNetSinksEnd) {

					// Net pin
					InstancePinSharedPtr instPinPtr = *designNetSinksIter;
					// Pin instance
					InstanceSharedPtr pinInstPtr = instPinPtr->getInstancePtr().lock();

					// Net connets to module instance
					if(pinInstPtr == instPtr) {

						// Unroute the net if so requested
						if(!inKeepRouting) designNetPtr->unroute();

						// Look for the port in the module
						PortSharedPtrConstIterator modulePortConstIter 
							= module->findPort(instPinPtr->getPinName());

						// Check if port was found
						if(modulePortConstIter != module->portsEnd()) {

							// Get a pointer to the current port
							PortSharedPtr modPortPtr = *modulePortConstIter;
							// Get module port reference instance
							InstanceSharedPtr portInstPtr = modPortPtr->getInstancePtr().lock();
							// Find the cloned instance corresponding to the pin reference instance
							InstanceSharedPtrIterator pinInstPtrCloneItr 
								= mDesignPtr->findInstance(inModuleInstanceName
								+ sHierarchySeparator + portInstPtr->getName());
							// Create new pin referencing the port reference instance
							flattenedModuleSinkPinPtrVector.push_back(
								Factory::newInstancePinPtr(*pinInstPtrCloneItr,
								modPortPtr->getPinName()));
							// Remove pin reference module instance
							designNetPtr->removeSink(instPinPtr);
							// Update designNetSinksEnd iterator
							designNetSinksEnd = designNetPtr->sinksEnd();
						}

					} else {

						// Move to next source
						designNetSinksIter++;

					}
				} // while(designNetSinksIter != designNetSinksEnd)

				// Get a begin iterator to flattened module sink pins
				InstancePinSharedPtrIterator flattenedModuleSinksBegin 
					= flattenedModuleSinkPinPtrVector.begin();
				// Get an end iterator to flattened module sink pins
				InstancePinSharedPtrIterator flattenedModuleSinksEnd 
					= flattenedModuleSinkPinPtrVector.end();

				// Update net sources with new source pins
				while(flattenedModuleSinksBegin != flattenedModuleSinksEnd) {

					// Add sink pin to net
					designNetPtr->addSink(*flattenedModuleSinksBegin);
					// Move to next sink
					flattenedModuleSinksBegin++;

				}

				// Reset vector
				flattenedModuleSinkPinPtrVector.clear();

				// Move to next net
				designNetsIter++;

			} // while(designNetsIter != designNetsEnd)
			// End handle external module nets

			// Remove module instance from design
			mDesignPtr->removeInstance(instPtr);

			// Operation successfully completed
			return true;
		}

	private:

		/// \brief Generate net connectivity type based on module boundary.
		/// \param inInstanceSharedPtrConstBegin A begin iterator to instances contained in module.
		/// \param inInstanceSharedPtrConstEnd An end iterator to instances contained in module.
		/// \param inNetPtr A pointer to a net.
		ENetConnectivity getNetConnectivity(
			InstanceSharedPtrConstIterator inInstanceSharedPtrConstBegin,
			InstanceSharedPtrConstIterator inInstanceSharedPtrConstEnd, NetSharedPtr inNetPtr) {

			// Net connectivity type
			ENetConnectivity netConnectivity = eNetConnectivityNotConnected;
			// Is net connected to module?
			bool isNetConnectedToModule = false;
			// Is net crossing module boundary?
			bool isNetInterToModule = false;
			// Get a begin iterator to net sources
			InstancePinSharedPtrIterator sourcesBegin = inNetPtr->sourcesBegin();
			// Get an end iterator to net sources
			InstancePinSharedPtrIterator sourcesEnd = inNetPtr->sourcesEnd();

			// Iterate over all sources
			while(sourcesBegin != sourcesEnd) {

				// Get a pointer to the current source pin
				InstancePinSharedPtr instPinPtr = *sourcesBegin;
				// Get a pointer to the instance that hosts the pin
				InstanceSharedPtr pinInst = instPinPtr->getInstancePtr().lock();

				// Look for the instance in the vector of instances
				if((std::find(inInstanceSharedPtrConstBegin, inInstanceSharedPtrConstEnd, pinInst))
					!= inInstanceSharedPtrConstEnd) {

					// Net connects to instance inside module boundary
					isNetConnectedToModule = true;

				} else {

					// Net connects to instance outside module boundary
					isNetInterToModule = true;

				}

				// Move to next source
				sourcesBegin++;
			}

			// Get a begin iterator to net sinks
			InstancePinSharedPtrIterator sinksBegin = inNetPtr->sinksBegin();
			// Get an end iterator to net sinks
			InstancePinSharedPtrIterator sinksEnd = inNetPtr->sinksEnd();

			// Iterate over all sinks
			while(sinksBegin != sinksEnd) {

				// Get a pointer to the current sink pin
				InstancePinSharedPtr instPinPtr = *sinksBegin;
				// Get a pointer to the instance that hosts the pin
				InstanceSharedPtr pinInst = instPinPtr->getInstancePtr().lock();
				// Look for the instance in the vector of instances
				if((std::find(inInstanceSharedPtrConstBegin, inInstanceSharedPtrConstEnd, pinInst))
					!= inInstanceSharedPtrConstEnd) {
					// Net connects to instance inside module boundary
					isNetConnectedToModule = true;
				} else {
					// Net connects to instance outside module boundary
					isNetInterToModule = true;
				}
				// Move to the next sink
				sinksBegin++;
			}

			if(isNetConnectedToModule) { // Net connects to module
				if(isNetInterToModule) {
					netConnectivity = eNetConnectivityExternal; // Net crosses module boundary
				} else {
					netConnectivity = eNetConnectivityInternal; // Net lies within module boundary
				}
			}

			// Return net connectivity type
			return netConnectivity;
		}

		/// \brief Sanitize a pin name.
		/// \param inPinName The pin name to be sanitized.
		string sanitizePinName(string inPinName) {

			// Sanitized pin name
			string sanitizedPinName;

			// Iterate over all characters in inString
			for(u_int32_t i = 0; i < inPinName.length(); i++) {

				// Match character against valid characters
				if(boost::regex_match(inPinName.substr(i, 1), sValidPinNameCharactersRegEx)) {
					sanitizedPinName += inPinName.substr(i, 1);
				}
			}

			// Make sure sanitizedPinName does not start with an invalid first character
			while(sanitizedPinName.size() > 0 && boost::regex_match(sanitizedPinName.substr(0, 1),
				sInvalidPinNameFirstCharactersRegEx)) {

				// Remove invalid character
				sanitizedPinName = sanitizedPinName.substr(1, sanitizedPinName.size() - 1);

			}

			// Return sanitizedPinName
			return sanitizedPinName;
		}

		/// \brief Clone an instance.
		/// \param inIntancePtr A pointer to the instance to be cloned.
		/// \param inCloneInstanceName The name of the clone.
		/// \details Clone an instance and its configuration. This function does not clone the 
		/// 	instance pins. Instance pins are handled when cloning nets, in function cloneNet.
		InstanceSharedPtr cloneInstance(InstanceSharedPtr inIntancePtr,
			const string& inCloneInstanceName) {

			// Clone inInstancePtr
			InstanceSharedPtr inInstanceClonePtr = Factory::newInstancePtr(inCloneInstanceName,
				inIntancePtr->getType(), inIntancePtr->getTile(), inIntancePtr->getSite(),
				inIntancePtr->getBonding());

			// Begin clone inIntance configuration
			// Get a begin iterator to inIntance config
			ConfigMap::const_iterator instanceConfigBegin = inIntancePtr->configBegin();
			// Get an end iterator to inIntance config
			ConfigMap::const_iterator instanceConfigEnd = inIntancePtr->configEnd();
			// Configuration map
			ConfigMap configMap;

			// Iterate over all configuration
			while(instanceConfigBegin != instanceConfigEnd) {

				// Get current setting
				const string setting = instanceConfigBegin->first;
				// Get current configuration
				const Config config = instanceConfigBegin->second;
				// Add (setting,configuration) to configuration map
				configMap.setConfig(setting, config);
				// Move to the next configuration
				instanceConfigBegin++;

			}

			// Add configurations to instance clone
			inInstanceClonePtr->addConfigs(configMap);
			// End clone inInstance configuration

			// Return cloned instance
			return inInstanceClonePtr;
		}

		/// \brief Clone a net.
		/// \param inNetPtr A pointer to the net to be cloned.
		/// \param inNetCloneName The name of the clone.
		/// \param inModuleInstanceName The module instance name.
		/// \details Clone a net, its configuration, its sources, sinks and pips. For pips, the
		/// 	routethrough instances are droppped and not cloned.
		NetSharedPtr cloneNet(NetSharedPtr inNetPtr, const string& inNetCloneName,
			const string& inModuleInstanceName) {

			// Clone inNetPtr
			NetSharedPtr inNetClonePtr = Factory::newNetPtr(inNetCloneName, inNetPtr->getNetType());

			// Begin clone inNetPtr configuration
			// Get a begin iterator to net config
			ConfigMap::const_iterator netConfigBegin = inNetPtr->configBegin();
			// Get an end iterator to net config
			ConfigMap::const_iterator netConfigEnd = inNetPtr->configEnd();
			// Configuration map
			ConfigMap configMap;

			// Iterate over all configuration
			while(netConfigBegin != netConfigEnd) {

				// Get current setting
				const string setting = netConfigBegin->first;
				// Get current configuration
				const Config config = netConfigBegin->second;
				// Add (setting,configuration) to configuration map
				configMap.setConfig(setting, config);
				// Move to the next configuration
				netConfigBegin++;

			}

			// Add configurations to net clone
			inNetClonePtr->addConfigs(configMap);
			// End clone inNetPtr configuration

			// Begin clone inNetPtr sources
			// Get a begin iterator to net sources
			InstancePinSharedPtrConstIterator inNetSourcesBegin = inNetPtr->sourcesBegin();
			// Get an end iterator to net sources
			InstancePinSharedPtrConstIterator inNetSourcesEnd = inNetPtr->sourcesEnd();

			// Iterate over all source pins
			while(inNetSourcesBegin != inNetSourcesEnd) {

				// Get pointer to current net pin
				InstancePinSharedPtr instPinPtr = *inNetSourcesBegin;
				// Get net pin reference instance
				InstanceSharedPtr pinInstPtr = instPinPtr->getInstancePtr().lock();
				// Find the cloned instance that correspond to the pin reference instance
				InstanceSharedPtrIterator pinInstPtrCloneItr = mDesignPtr->findInstance(
					inModuleInstanceName + sHierarchySeparator + pinInstPtr->getName());
				// Clone source pin
				InstancePinSharedPtr instPinPtrClone 
					= Factory::newInstancePinPtr(*pinInstPtrCloneItr, instPinPtr->getPinName());
				// Add pin clone to the net clone
				inNetClonePtr->addSource(instPinPtrClone);
				// Move to next source
				inNetSourcesBegin++;

			}
			// End clone inNetPtr sources

			// Begin clone inNetPtr sinks
			// Get a begin iterator to net sinks
			InstancePinSharedPtrConstIterator inNetSinksBegin = inNetPtr->sinksBegin();
			// Get an end iterator to net sinks
			InstancePinSharedPtrConstIterator inNetSinksEnd = inNetPtr->sinksEnd();

			// Iterate over all sink pins
			while(inNetSinksBegin != inNetSinksEnd) {

				// Get pointer to current net pin
				InstancePinSharedPtr instPinPtr = *inNetSinksBegin;
				// Get net pin reference instance
				InstanceSharedPtr pinInstPtr = instPinPtr->getInstancePtr().lock();
				// Find the cloned instance that correspond to the pin reference instance
				InstanceSharedPtrIterator pinInstPtrCloneItr = mDesignPtr->findInstance(
					inModuleInstanceName + sHierarchySeparator + pinInstPtr->getName());
				// Clone sink pin
				InstancePinSharedPtr instPinPtrClone 
					= Factory::newInstancePinPtr(*pinInstPtrCloneItr, instPinPtr->getPinName());
				// Add pin clone to the net clone
				inNetClonePtr->addSink(instPinPtrClone);
				// Move to next sink
				inNetSinksBegin++;

			}
			// End clone inNetPtr sinks

			// Begin clone inNetPtr pips
			// Get a begin iterator to net pips
			PipConstIterator inNetPipsBegin = inNetPtr->pipsBegin();
			// Get an end iterator to net pips
			PipConstIterator inNetPipsEnd = inNetPtr->pipsEnd();

			// Iterate over all pips
			while(inNetPipsBegin != inNetPipsEnd) {

				// Get a pointer to the current pip
				Pip pipPtr = *inNetPipsBegin;
				/// \todo We are dropping routethrough instances.
				// Clone the pip
				Pip pipClone = Factory::newPip(pipPtr.getTileName(), pipPtr.getSourceWireName(),
					pipPtr.getSinkWireName(), pipPtr.getDirection());
				// Add pip clone to cloned net
				inNetClonePtr->addPip(pipClone);
				// Move to next pip
				inNetPipsBegin++;

			}
			// End clone inNetPtr pips

			// Return cloned net
			return inNetClonePtr;
		}
	};

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_MODULETRANSFORMER_HPP
