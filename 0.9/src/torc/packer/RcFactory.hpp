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
/// \brief Header for the RcFactory class.

#ifndef TORC_PACKER_RCFACTORY_HPP
#define TORC_PACKER_RCFACTORY_HPP

#include "torc/physical/Design.hpp"
#include "torc/packer/PrimitiveSet.hpp"
#include "torc/packer/RoutingNet.hpp"
#include "torc/packer/CombinationalPath.hpp"
#include <string>
#include <boost/smart_ptr.hpp>

namespace torc {
namespace physical {

    /// \brief RcFactory class for physical netlist elements.
    /// \details Physical netlist elements must be created by this RcFactory.
    class RcFactory {
    protected:
    // friends
        /// \brief Imported type name.
        typedef std::string string;
    public:
    // functions
        		/// \brief Create and return a new PrimitiveSet object.
				static PrimitiveSetSharedPtr newPrimitiveSetPtr(const string& inName) {
					PrimitiveSetSharedPtr PrimitiveSetPtr(new PrimitiveSet(inName));
					PrimitiveSetPtr->setSelfWeakPtr(PrimitiveSetPtr);
					return PrimitiveSetPtr;
				}
				
				/// \brief Create and return a new Element object.
				static ElementSharedPtr newElementPtr(const string& inName) {
					ElementSharedPtr elementPtr(new Element(inName));
					elementPtr->setSelfWeakPtr(elementPtr);
					return elementPtr;
				}
				
				/// \brief Create and return a new RoutingNet object.
				static RoutingNetSharedPtr newRoutingNePtr(NetSharedPtr snet) {
					RoutingNetSharedPtr routingNetPtr(new RoutingNet(snet));
					return routingNetPtr;
				}
				
				/// \brief Create and return a new CombinationalPath object.
				static CombinationalPathSharedPtr newCombinationalPathPtr() {
					CombinationalPathSharedPtr combinationalPathPtr(new CombinationalPath());
					return combinationalPathPtr;
				}
				
				/// \brief Create and return a new Element object.
				static PrimitiveSharedPtr newPrimitivePtr(const string& inName) {
					PrimitiveSharedPtr primitivePtr(new Primitive(inName));
					primitivePtr->setSelfWeakPtr(primitivePtr);
					return primitivePtr;
				}
				
				/// \brief Construct an ConnectionPin and return a constant reference.
			  	/// \param inConnectionPtr The pin instance pointer.
			  	/// \param inPinName The pin name.
			  	static ConnectionPin newConnectionPin(string inElementName, 
			  		const string& inPinName) {
				  	return ConnectionPin(inElementName, inPinName);
			  	}
			  
			  	/// \brief Create and return a new Element object.
				static ConnectionSharedPtr newConnectionPtr(const string& inName) {
					ConnectionSharedPtr connectionPtr(new Connection(inName));
					return connectionPtr;
				}
				
				/// \brief Create and return a new PrimitivePin shared pointer.
         		/// \param inName The PrimitivePin name.
         		/// \param inInstancePtr The PrimitivePin instance pointer.
         		/// \param inPinName The PrimitivePin pin name.
         		static PrimitivePinSharedPtr newPrimitivePinPtr(string inElementName, string inPinName, const PinType inType) {
            		PrimitivePinSharedPtr PrimitivePinPtr(new PrimitivePin(inElementName, inPinName, inType));
             		return PrimitivePinPtr;
         		}

    };

} // namespace physical
} // namespace torc

#endif // TORC_PACKER_RCFACTORY_HPP
