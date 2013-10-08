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
/// \brief Header for the PrimitiveStructure class.

#ifndef TORC_PACKER_PRIMITIVESTRUCTURE_HPP
#define TORC_PACKER_PRIMITIVESTRUCTURE_HPP

#include "torc/architecture/PrimitiveDef.hpp"
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>

namespace torc {
namespace packer {

	//namespace packer { class PrimitiveStructureUnitTest; }
	namespace architecture { class PackageUnitTest; }
	namespace packer { class PrimitiveStructureUnitTest; }
	
	using namespace architecture;
	
	/// \brief Enumeration of logic types
	enum ELogicType {
		eLogicTypeUnknown = 0, 
		eLogicTypeLUT, 
		eLogicTypeFlop, 
		eLogicTypeInv, 
		eLogicTypeAnd, 
		eLogicTypeXor, 
		eLogicTypeMux,
		eLogicTypeCount
	};

	/// \brief Encapsulation of the site index, pin name, and pin flags for a package.
	class PrimitiveStructure {
	protected:
		/// \brief Our unit test class has access to our internals.
		friend class torc::packer::packer::PrimitiveStructureUnitTest;
		friend class Unpacker;
		
	// types
		/// \brief Imported type name.
		typedef std::string string;
		/// \brief Imported type name.
		typedef torc::architecture::PrimitiveDef PrimitiveDef;
		/// \brief Imported type name.
		typedef torc::architecture::PrimitiveElement PrimitiveElement;
		/// \brief Mapping from element name to element pointer.
		typedef std::map<string, const PrimitiveElement*> NameToElementPtrMap;
		/// \brief Mapping from principal element name to orphan element pointer.
		typedef std::map<string, std::vector<const PrimitiveElement*> > PrincipalToOrphanPtrMap;
		/// \brief A set of configuration values.
		typedef std::set<const torc::architecture::PrimitiveElementPin*> ElementPinPtrSet;
	// members
		/// \brief Pointer to the associated primitive definition.
		const PrimitiveDef* mPrimitiveDefPtr;
	// members (type-independent maps)
		/// \brief Set of inverted element input pins.
		ElementPinPtrSet mInvertedInputs;
		/// \brief Map of unprocessed elements.
		NameToElementPtrMap mUnprocessed;
		/// \brief Map of pre-classified elements (typically by a subclass).
		NameToElementPtrMap mPreclassified;
		/// \brief Map of all elements.
		NameToElementPtrMap mElements;
		/// \brief Map of all principals.
		NameToElementPtrMap mPrincipals;
		/// \brief Map of all terminals.
		NameToElementPtrMap mTerminals;
		/// \brief Map of all configurable muxes (including switches and inverters).
		NameToElementPtrMap mMuxes;
		/// \brief Map of all Orphans.
		NameToElementPtrMap mOrphans;
	// members (secondary classification)
		/// \brief Map of all switches.
		NameToElementPtrMap mSwitches;
// we probably want to remember which mux pins are inverted, and not merely which muxes include inversion
//		/// \brief Map of all inverters.
//		NameToElementPtrMap mInverters;
	// members (type-dependent maps)
		/// \brief Map of all LUTs.
		NameToElementPtrMap mLUTs;
		/// \brief Map of all flops.
		NameToElementPtrMap mFlops;
// do we actually need this?  how about XORs?
//		/// \brief Map of all AND gates.
//		NameToElementPtrMap mANDs;
		/// \brief Map of all power sources.
		NameToElementPtrMap mPower;
		/// \brief Map of all ground sources.
		NameToElementPtrMap mGround;
		/// \brief Map of all routethroughs.
		NameToElementPtrMap mRoutethroughs;
                /// \brief Map of principals to orphans .
		PrincipalToOrphanPtrMap mPrincipalstoOrphans;
	// statics
		/// \brief Regular expression for routethroughs.
		static boost::regex sRoutethroughRegEx;
		/// \brief Regular expression for additional principal elements.
		static boost::regex sPrincipalRegEx;
		/// \brief Regular expression for LUTs.
		static boost::regex sLUTRegEx;
		/// \brief Regular expression for flops.
		static boost::regex sFlopRegEx;
		/// \brief Regular expression for power sources.
		static boost::regex sPowerRegEx;
		/// \brief Regular expression for ground sources.
		static boost::regex sGroundRegEx;
		/// \brief Regular expression for inverting input pins.
		static boost::regex sInvertingInputRegEx;
	// functions
		/// \brief Initialize this object based on the PrimitiveDef information.
		virtual void initialize(void);
		/// \brief Return true if the elemtn has been preclassified (typically by a subclass).
		virtual bool isPreclassified(const PrimitiveElement& inElement);
		/// \brief Return true if the element is the principal element (a parent to the orphans).
		virtual bool isPrincipal(const PrimitiveElement& inElement);
		/// \brief Return true if the element is a terminal.
		virtual bool isTerminal(const PrimitiveElement& inElement);
		/// \brief Return true if the element is an orphan.
		virtual bool isOrphan(const PrimitiveElement& inElement);
		/// \brief Return true if the element is a configurable mux.
		virtual bool isMux(const PrimitiveElement& inElement, bool& outIsSwitch);
		/// \brief Return true if the element is a power source.
		virtual bool isPower(const PrimitiveElement& inElement);
		/// \brief Return true if the element is a ground source.
		virtual bool isGround(const PrimitiveElement& inElement);
		/// \brief Return true if the element is a LUT.
		virtual bool isLUT(const PrimitiveElement& inElement, const string& inConfig);
		/// \brief Return true if the element is a flop.
		virtual bool isFlop(const PrimitiveElement& inElement, const string& inConfig);
		/// \brief Return true if the element is a routethrough.
		virtual bool isRoutethrough(const PrimitiveElement& inElementPtr);
	public:
	// constructors
		/// \brief Default constructor.
		PrimitiveStructure(const PrimitiveDef* inPrimitiveDefPtr) 
			: mPrimitiveDefPtr(inPrimitiveDefPtr) {
			initialize();
		}
		/// \brief Null constructor.
		PrimitiveStructure(void) : mPrimitiveDefPtr(0) {};
		/// \brief Virtual destructor.
		virtual ~PrimitiveStructure(void) {}
	// functions
		/// \brief Prints out debugging information for the specified element.
		void debug(const PrimitiveElement& inPrimitiveElement);
	// accessors
		/// \brief Returns a pointer to the associated primitive definition.
		const PrimitiveDef* getPrimitiveDefPtr(void) const { return mPrimitiveDefPtr; }
	};

	/// \brief Shared pointer encapsulation of a PrimitiveStructure.
	typedef boost::shared_ptr<PrimitiveStructure> PrimitiveStructureSharedPtr;

} // namespace architecture
} // namespace torc

#endif // TORC_PACKER_PRIMITIVESTRUCTURE_HPP
