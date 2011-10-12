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

#ifndef TORC_PHYSICAL_XDLUNPACK_HPP
#define TORC_PHYSICAL_XDLUNPACK_HPP

#include "torc/physical/Design.hpp"
#include "torc/physical/Factory.hpp"
#include "ParsePrimitive.hpp"
#include "WritePrimitive.hpp"

#include <boost/smart_ptr.hpp>
#include <iostream>
#include <string>

namespace torc {
namespace physical {

// Elements need to be removed
const string elementNames = "SRUSED REVUSED PRECYINIT DUSED CUSED COUTUSED CEUSED BUSED AUSED DCY0 CCY0 BCY0 ACY0 END";

// Elements need to be removed based on their configuration
const string extraMuxNames = "CY0G CY0F GYMUX DYMUX FXMUX DXMUX CYINIT DIG_MUX DIF_MUX XBMUX DOUTMUX COUTMUX BOUTMUX AOUTMUX DFFMUX CFFMUX BFFMUX AFFMUX ACY0 BCY0 CCY0 DCY0 PRECYINIT CDI1MUX BDI1MUX ADI1MUX WEMUX TMUX OMUX IDELMUX D2OBYP_SEL IFFDELMUX IFFMUX END";

//Inverters
const string extraInvNames = "CLKINV T1INV T2INV D1INV D2INV END";

class XdlUnpack {
	
public:
	
	DesignSharedPtr operator() (const DesignSharedPtr& inDesignPtr);
	void unpack(const Circuit& circuit, CircuitSharedPtr newCircuit);
	DesignSharedPtr unpack(const Design& design);
	ModuleSharedPtr unpack(const Module& module);
	PortSharedPtr 	unpack(const Port& port);
	void unpack(Instance& instance, CircuitSharedPtr newCircuit);
	NetSharedPtr unpack(const Net& net);
	InstancePinSharedPtr unpack(const InstancePinSharedPtr instancePin, EPinDirection pinDirection, bool comma);
	Pip unpack(const Pip& pip, bool comma);
	RoutethroughSharedPtr unpack(const Routethrough& routethrough);
	
	/// \brief removes extra elements that are needed for unpacking
	void removeExtraElements(PrimitiveSharedPtr primPtr);
	
	// read primitives.
	void readPrimitives(const char *fileName) { 
		ParsePrimitive parser;
		primitives = parser.ParsePrimitiveFile(fileName);
		return;
	}
	
	// checks if the elemnt is a configured multiplexer that needs to be removed
	bool isExtraMux(string elem){
		char names[1024] ;
		strcpy(names,extraMuxNames.c_str());
		string pch;
		pch = strtok (names," ");
		while (pch!="END")
		{
			if(pch == elem){
				return true;
			}
			pch = strtok (NULL," ");
		}
		return false;
	}
	//void unpack(const ConfigMap& configMap);
protected:
	typedef boost::shared_ptr<torc::physical::Named> NamedSharedPtr;
	typedef boost::shared_ptr<torc::physical::ConfigMap> ConfigMapSharedPtr;
									 
	PrimitiveSetSharedPtr primitives;
	
};

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_XDLUNPACK_HPP


