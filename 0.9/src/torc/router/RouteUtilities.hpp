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
/// \brief Header for the Utilities class.

#ifndef TORC_ROUTER_ROUTUTILITIES_HPP
#define TORC_ROUTER_ROUTUTILITIES_HPP

#include "torc/router/RouteNet.hpp"
#include "torc/Physical.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/Tilewire.hpp"
#include "torc/architecture/XdlImporter.hpp"

#include <string>
#include <map>
#include <boost/unordered_map.hpp>
#include <boost/any.hpp>

namespace torc {
namespace router {


	/// \brief Router net.
	/// \details The RouteNet class contains architecture specific sources and sinks.
	class RouteUtilities {
	// types
		/// Imported type names
		typedef std::string string;
		typedef architecture::Tilewire Tilewire;
		typedef architecture::TilewireVector TilewireVector;
		typedef architecture::Arc Arc;
		typedef architecture::ArcVector ArcVector;
		
		typedef torc::physical::NetSharedPtr NetSharedPtr;
		typedef std::vector<NetSharedPtr> NetSharedPtrVector;
		typedef torc::physical::Circuit::NetSharedPtrConstIterator NetSharedPtrConstIterator;
		typedef torc::physical::Circuit::NetSharedPtrIterator NetSharedPtrIterator;
		typedef torc::physical::Net::InstancePinSharedPtrConstIterator 
		InstancePinSharedPtrConstIterator;
		typedef torc::physical::Net::InstancePinSharedPtrIterator 
		InstancePinSharedPtrIterator;
		typedef torc::physical::Pip Pip;
		
	public:
		typedef ArcVector::const_iterator ArcConstIterator;
		typedef ArcVector::iterator ArcIterator;
		typedef TilewireVector::const_iterator TilewireConstIterator;
		typedef TilewireVector::iterator TilewireIterator;
		typedef RouteNodePtrVector::const_iterator RouteNodePtrConstIterator;
		typedef RouteNodePtrVector::iterator RouteNodePtrIterator;
		
	protected:
	// members
		/// \brief Name of the net.
		string mName;
		/// \brief Vector of net source Tilewires.
		TilewireVector mSources;
		/// \brief Vector of net sink Tilewires.
		TilewireVector mSinks;
		/// \brief Vector of arcs representing net connectivity.
		ArcVector mArcs;
		/// \brief Vector of RouteNodes representing net connectivity.
		RouteNodePtrVector mRouteNodes;
		/// \brief Net annotation structure.
	public:
		boost::unordered_map<boost::uint32_t, boost::any> mProperties;

	public:
	// constructors
		/// \brief Constructor.
		RouteUtilities() {}
		
		void design2routenets(NetSharedPtrIterator netsbegin, NetSharedPtrIterator netsend,
			RouteNetVector& routenets) {
			NetSharedPtrIterator p = netsbegin;
			for (p = netsbegin; p != netsend; p++) {
				TilewireVector sources;
				TilewireVector sinks;
//std::cout << "Converting: " << (*p)->getName() << std::endl;
				//for (InstancePinSharedPtrConstIterator pinPtr = (*p)->sourcesBegin(); 
				for (InstancePinSharedPtrIterator pinPtr = (*p)->sourcesBegin(); 
					pinPtr != (*p)->sourcesEnd(); pinPtr++) {
					//sources.push_back((((architecture::InstancePin)(*pinPtr)).getTilewire());
					sources.push_back(INSTANCE_PIN_PHYSICAL_TO_ARCHITECTURE(*pinPtr)->getTilewire());
//std::cout << "Source: " << sources.back() << std::endl;
					if (sources.back() == Tilewire::sInvalid) {
						std::cout << "BAD IMPORT: " << (*p)->getName() << " - "
							<< (*pinPtr)->getPinName() << std::endl;
					}
				}
				for (InstancePinSharedPtrIterator pinPtr = (*p)->sinksBegin(); 
					pinPtr != (*p)->sinksEnd(); pinPtr++) {
					sinks.push_back(INSTANCE_PIN_PHYSICAL_TO_ARCHITECTURE(*pinPtr)->getTilewire());
//std::cout << "Sink: " << sinks.back() << std::endl;
					if (sinks.back() == Tilewire::sInvalid) {
						std::cout << "BAD IMPORT: " << (*p)->getName() << " - "
							<< (*pinPtr)->getPinName() << std::endl;
					}
				}
				
				
				std::string name = (*p)->getName();
				routenets.push_back(RouteNet(name, sources, sinks));
			}
		}
		void routenets2design(RouteNetVector& routenets, NetSharedPtrIterator netsbegin, 
			NetSharedPtrIterator netsend, architecture::DDB& ddb) {
			NetSharedPtrIterator p = netsbegin;
			architecture::ExtendedWireInfo ewisource(ddb);
			architecture::ExtendedWireInfo ewisink(ddb);
			unsigned int i = 0;
			for (p = netsbegin; p != netsend; p++) {
				if (!((*p)->getName() == routenets[i].getName())) {
					std::cout << "Name mismatch for corresponding entry: " 
						<< (*p)->getName() << " " << routenets[i].getName() << std::endl;
					throw;
				}
				// convert route nodes to pips
				RouteNodePtrVector& routeNodes = routenets[i].routeNodes();
				for (unsigned int j = 0; j < routeNodes.size(); j++) {
					Arc arc = routeNodes[j]->getArc();
					
					if (arc.getSourceTilewire() == arc.getSinkTilewire()) continue;
					
					ewisource = arc.getSourceTilewire();
					ewisink = arc.getSinkTilewire();
					
					Pip pip = physical::Factory::newPip(ewisource.mTileName, ewisource.mWireName,
						ewisink.mWireName, physical::ePipUnidirectionalBuffered);
						
					(*p)->addPip(pip);
				}
				
				
				
				i++;
			}
		}
	};


} // namespace router
} // namespace torc

#endif // TORC_ROUTER_ROUTEUTILITIES_HPP
