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

#ifndef TORC_PACKER_UNPACKER_HPP
#define TORC_PACKER_UNPACKER_HPP

#include "torc/physical/Design.hpp"
#include "torc/physical/Instance.hpp"
#include "torc/physical/ConfigMap.hpp"
#include "torc/architecture/DDB.hpp"
#include "PrimitiveStructure.hpp"
#include <boost/smart_ptr.hpp>
#include <iostream>
#include <string>
#include <map>

namespace torc {
namespace packer {

	namespace packer { class UnpackerUnitTest; }

	class Unpacker {
		
	protected:
		/// \brief Our unit test class has access to our internals.            
                friend class torc::packer::packer::UnpackerUnitTest;

		// types
		/// \brief Imported type name
		typedef std::string string;
		/// \brief Imported type name 
		typedef torc::physical::DesignSharedPtr DesignSharedPtr;
		typedef	torc::physical::InstanceSharedPtrVector InstanceSharedPtrVector;
		typedef	torc::physical::Circuit::InstanceSharedPtrConstIterator	
		InstanceSharedPtrConstIterator;	
		typedef torc::architecture::DDB DDB;
		typedef torc::physical::ConfigMap ConfigMap;
		/// \brief Map of PrimitiveStructure Shared Pointers.               
		typedef std::map<std::string, PrimitiveStructureSharedPtr> 
		PrimitiveStructuresSharedPtrMap;
                /// \brief Vector of Primitive Element Pin Pointers.               
		typedef std::vector<const torc::architecture::PrimitiveElementPin*>
                PrimitiveElementPinPtrVector;
		/// \brief Map of Candidate Nets
		typedef std::map<const torc::architecture::PrimitiveElementPin*,
				 PrimitiveElementPinPtrVector> CandidateNet;
		/// \brief Map of Primitive Element Name to Primitive Element.               
		typedef std::map<std::string, const PrimitiveStructure::PrimitiveElement*> 
		NameToElementPtrMap;
		/// \brief Map of Primitive Element Pointer to Instance Shared Pointer.
		typedef std::map<const torc::architecture::PrimitiveElement*,
				 torc::physical::InstanceSharedPtr> ElementPtrToInstancePtrMap;

                //members
		PrimitiveStructuresSharedPtrMap mPrimitiveStructures;
		InstanceSharedPtrVector mInstances;
		DesignSharedPtr mDesignPtr;
		DDB* mDDBPtr;

                /// \brief Initialize this object based on the PrimitiveDef information.     
                virtual void initialize(void);
		/// \brief Default unpack function.         
		void unpack(void);				
                /// \brief Unpack function that takes Instance iterators as input.          
		void unpack(InstanceSharedPtrConstIterator inBegin, InstanceSharedPtrConstIterator inEnd);
		/// \brief Function to find the next element.
		bool findNextElement(const PrimitiveStructure& inPrimitiveStructure,
				     const torc::architecture::PrimitiveElement& element,
				     torc::physical::InstanceSharedPtr& inInstancePtr,
				     NameToElementPtrMap& inUsedElements, 
				     std::vector<std::string> inInstanceInputPins, 
				     CandidateNet& inElementPinsForNet,
				     const torc::architecture::PrimitiveElementPin*
				     &outNetSourcePin,
				     std::vector<torc::physical::InstanceSharedPtr>
				     &outNewInstances,
				     ElementPtrToInstancePtrMap &elementToInstanceMap);
	
	public:
		//constructors
		Unpacker(DesignSharedPtr& inDesignPtr) : mDesignPtr(inDesignPtr), mDDBPtr(0) {
                        initialize();
                }

                /// \brief Virtual destructor.                                       
                virtual ~Unpacker(void) {}
	
};

} // namespace packer
} // namespace torc

#endif // TORC_PACKER_UNPACKER_HPP
