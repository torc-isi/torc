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

#ifndef TORC_GENERIC_OM_CONNECTIONHANDLER_HPP
#define TORC_GENERIC_OM_CONNECTIONHANDLER_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/util/Error.hpp"

namespace torc {
namespace generic {

class ConnectionHandler {

  public:	
	void
	connectPortToNet( const PortSharedPtr &inPort ) throw(Error);

	void
	disconnectPortFromNet( const PortSharedPtr &inPort ) throw(Error);

	void
	connectPortRefToNet(
			const PortReferenceSharedPtr &inPort ) throw(Error);

	void
	disconnectPortRefFromNet(
			const PortReferenceSharedPtr &inPort ) throw(Error);

	void
	connectPortListToNet(
			const PortListSharedPtr &inPortList ) throw(Error);

	void
	disconnectPortListFromNet(
			const PortListSharedPtr &inPortList ) throw(Error);

	ConnectionHandler( const NetSharedPtr &inNet )
		:mNet( inNet ) {
	}

  private:	
	NetSharedPtr mNet;
};

} //namespace generic
} //namespace torc

#endif // TORC_GENERIC_OM_CONNECTIONHANDLER_HPP
