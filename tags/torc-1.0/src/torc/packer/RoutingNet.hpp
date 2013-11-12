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
/// \brief Header for the RoutingNet class.

#ifndef TORC_PACKER_ROUTINGNET_HPP
#define TORC_PACKER_ROUTINGNET_HPP

#include "torc/physical/Net.hpp"
#include <vector>

namespace torc {
namespace physical {

	/// \brief Routing net.
	
	class RoutingNet:public Named {
	// friends
		/// \brief The Factory class has direct access to our internals.
		friend class RcFactory;
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
		
	// members
		/// \brief The original net
		NetSharedPtr superNet;
		/// \brife combinational path count
		std::vector<size_t> pathCounts;

	public:
		
		// constructors
		/// \brief Constructor.
		/// \param original net
		RoutingNet(NetSharedPtr snet) : Named(snet->getName()) {
			pathCounts.resize(snet->getSourceCount() + snet->getSinkCount(),0);
			superNet=snet;
		}
	
	// functions
		/// \brief Returns net supernet
		NetSharedPtr getSuperNet(void){
			return superNet;
		}
		
		/// \brief Sets super net
		NetSharedPtr setSuperNet(NetSharedPtr snet){
			return (superNet=snet);
		}
		
		/// \brief Returns path count for pin index
		size_t getPathCount(size_t index){
			return pathCounts[index];	
		}
		
		/// \brief Sets path count for pin index
		bool setPathCount(size_t index, size_t pCount){
			pathCounts[index] = pCount;
			return true;	
		}
		
		/// \brief Returns path count for pin 
		size_t getPathCount(InstancePinSharedPtr pinPtr){
			size_t index =0;
			Net::InstancePinSharedPtrIterator sip = superNet->sourcesBegin();
			Net::InstancePinSharedPtrIterator sie = superNet->sourcesEnd();
			while(sip != sie){
				if(**sip == *pinPtr)
					return getPathCount(index);;
				++index;
				++sip;
			}
			
			sip = superNet->sinksBegin();
			sie = superNet->sinksEnd();
			while(sip != sie){
				if(**sip == *pinPtr)
					return getPathCount(index);;
				++index;
				++sip;
			}
			return getPathCount(index);			
		}
		
		/// \brief Sets path count for pin 
		bool setPathCount(InstancePinSharedPtr pinPtr, size_t pCount){
			size_t index =0;
			Net::InstancePinSharedPtrIterator sip = superNet->sourcesBegin();
			Net::InstancePinSharedPtrIterator sie = superNet->sourcesEnd();
			while(sip != sie){
				if(**sip == *pinPtr)
					return setPathCount(index, pCount);
				++index;
				++sip;
			}
			sip = superNet->sinksBegin();
			sie = superNet->sinksEnd();
			while(sip != sie){
				if(**sip == *pinPtr)
					return setPathCount(index, pCount);
				++index;
				++sip;
			}
			return setPathCount(index, pCount);	
		}
		
		// operators
		/// \brief Equality operator.
		/// \details This function deems nets equal if their names are identical.
		/// \param rhs The net to compare against.
		/// \returns true if both net names are identical, or false otherwise.
		bool operator ==(const RoutingNet& rhs) const { return getName() == rhs.getName(); }	
	};

	/// \brief Shared pointer encapsulation of a RoutingNet.
	typedef boost::shared_ptr<RoutingNet> RoutingNetSharedPtr;

	/// \brief Vector of RoutingNet shared pointers.
	typedef std::vector<RoutingNetSharedPtr> RoutingNetSharedPtrVector;

} // namespace physical
} // namespace torc

#endif // TORC_PACKER_ROUTINGNET_HPP
