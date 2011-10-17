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
/// \brief Header for the Virtex2PrimitiveStructure class.

#ifndef TORC_PACKER_VIRTEX2PRIMITIVESTRUCTURE_HPP
#define TORC_PACKER_VIRTEX2PRIMITIVESTRUCTURE_HPP

#include "torc/packer/PrimitiveStructure.hpp"
#include <iostream>

namespace torc {
namespace packer {

	using namespace torc::architecture;
	namespace packer { class Virtex2PrimitiveStructureUnitTest; }

	/// \brief Subclass of PrimitiveStructure for Virtex2 and Virtex2P.
	class Virtex2PrimitiveStructure : public PrimitiveStructure {
	protected:
		/// \brief Our unit test class has access to our internals.
		friend class torc::packer::packer::Virtex2PrimitiveStructureUnitTest;
	// functions
		/// \brief Initialize this object based on the PrimitiveDef information.
		virtual void initialize(void);
	public:
	// constructors
		/// \brief Default constructor.
		Virtex2PrimitiveStructure(const PrimitiveDef* inPrimitiveDefPtr) 
			: PrimitiveStructure(inPrimitiveDefPtr) {
			initialize();
		}
		/// \brief Null constructor.
		Virtex2PrimitiveStructure(void) : PrimitiveStructure(0) {};
	};

} // namespace architecture
} // namespace torc

#endif // TORC_PACKER_VIRTEX2PRIMITIVESTRUCTURE_HPP
