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
/// \brief Header for the WireInfo class.

#ifndef TORC_ARCHITECTURE_WIRE_INFO_HPP
#define TORC_ARCHITECTURE_WIRE_INFO_HPP

#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/architecture/Array.hpp"
#include <cstring>
#include <cstdlib>

namespace torc {
namespace architecture {

	/// \brief Array of wire indexes.
	class WireArray : public Array<const xilinx::WireIndex> {};

	/// \brief Encapsulation of a wire within a tile type.
	/// \details The wire information includes the wire name, attribute flags, and arrays of all 
	///		tied, regular, and irregular sources and sinks.
	class WireInfo {
	// friends
		/// \brief The Tiles class has access to our protected members.
		friend class Tiles;
	protected:
	// types
		/// \brief Imported type name.
		typedef boost::uint16_t uint16_t;
		/// \brief Imported type name.
		typedef xilinx::WireFlags WireFlags;
	// members
		/// \brief This wire's offset into the arc usage bitset.
		uint16_t mArcOffset;
		/// \brief The attribute flags for this wire.
		WireFlags mFlags;
		/// \brief The name for this wire.
		const char* mName;
		/// \brief The tied sink array for this wire.
		WireArray mTiedSinks;
		/// \brief The tied source array for this wire.
		WireArray mTiedSources;
		/// \brief The sink array for this wire.
		WireArray mSinks;
		/// \brief The source array for this wire.
		WireArray mSources;
		/// \brief The irregular sink array for this wire.
		WireArray mIrregularSinks;
		/// \brief The irregular source array for this wire.
		WireArray mIrregularSources;
		/// \brief The routethrough sink array for this wire.
		WireArray mRoutethroughSinks;
		/// \brief The routethrough source array for this wire.
		WireArray mRoutethroughSources;
	// constructors
		/// \brief Protected constructor.
		WireInfo(uint16_t inOffset, WireFlags inFlags, const char* inName) : mArcOffset(inOffset), 
			mFlags(inFlags), mTiedSinks(), mTiedSources(), mSinks(), mSources(), mIrregularSinks(), 
			mIrregularSources(), mRoutethroughSinks(), mRoutethroughSources() {
			// make a private copy of the wire name
			mName = strdup(inName);
		}
	// constructors
		/// \brief Disabled copy constructor.
		WireInfo(const WireInfo& /*rhs*/) : mArcOffset(), mFlags(), mName(0), mTiedSinks(), 
			mTiedSources(), mSinks(), mSources(), mIrregularSinks(), mIrregularSources(), 
			mRoutethroughSinks(), mRoutethroughSources() {}
	public:
	// enumerations
		/// \brief Enumeration of wire attribute flags.
		enum EWireFlag {
			eWireFlagNone = 0, eWireFlagHidden = 1, eWireFlagInput = 2, eWireFlagOutput = 4, 
			eWireFlagRemote = 8, eWireFlagGlobal = 16, eWireFlagRemoteNodeCapable = 64, 
			eWireFlagRemoteArcCapable = 128
		};
	// constructors
		/// \brief Null constructor.
		WireInfo(void) : mArcOffset(0), mFlags(eWireFlagNone), mName(0), mTiedSinks(), 
			mTiedSources(), mSinks(), mSources(), mIrregularSinks(), mIrregularSources(),
			mRoutethroughSinks(), mRoutethroughSources() {}
		/// \brief Non-virtual destructor.
		~WireInfo(void) {
			// release the wire name if allocated
			if(mName != 0) { free(const_cast<char*>(mName)); mName = 0; }
		}
	// accessors
		/// \brief Returns this wire's offset into the arc usage bitset.
		/// \details Every wire in a tile type has a wire index, as well as zero or more arcs.  
		///		The offset for this wire is the total number of arcs for all wires seen so far 
		///		(i.e. wires of lower index).
		const uint16_t getArcOffset(void) const { return mArcOffset; }
		/// \brief Returns the attribute flags for this wire.
		const WireFlags getFlags(void) const { return mFlags; }
		/// \brief Returns the name for this wire.
		const char* getName(void) const { return mName; }
		/// \brief Returns the tied sink array for this wire.
		const WireArray& getTiedSinks(void) const { return mTiedSinks; }
		/// \brief Returns the tied source array for this wire.
		const WireArray& getTiedSources(void) const { return mTiedSources; }
		/// \brief Returns the sink array for this wire.
		const WireArray& getSinks(void) const { return mSinks; }
		/// \brief Returns the source array for this wire.
		const WireArray& getSources(void) const { return mSources; }
		/// \brief Returns the irregular sink array for this wire.
		const WireArray& getIrregularSinks(void) const { return mIrregularSinks; }
		/// \brief Returns the irregular source array for this wire.
		const WireArray& getIrregularSources(void) const { return mIrregularSources; }
		/// \brief Returns the routethrough sink array for this wire.
		const WireArray& getRoutethroughSinks(void) const { return mRoutethroughSinks; }
		/// \brief Returns the routethrough source array for this wire.
		const WireArray& getRoutethroughSources(void) const { return mRoutethroughSources; }
	// tests
		/// \brief Returns true if the wire is hidden in traces.
		bool isHidden(void) const { return isHidden(mFlags); }
		/// \brief Returns true if the wire is a logic input.
		bool isInput(void) const { return isInput(mFlags); }
		/// \brief Returns true if the wire is a logic output.
		bool isOutput(void) const { return isOutput(mFlags); }
		/// \brief Returns true if the wire is a remote wire.
		bool isRemote(void) const { return isRemote(mFlags); }
		/// \brief Returns true if the wire is a global wire (a what!?).
		bool isGlobal(void) const { return isGlobal(mFlags); }
		/// \brief Returns true if the wire may be referenced by a remote wire.
		bool isRemoteNodeCapable(void) const { return isRemoteNodeCapable(mFlags); }
		/// \brief Returns true if the wire may be referenced by a remote arc.
		bool isRemoteArcCapable(void) const { return isRemoteArcCapable(mFlags); }
	// static
		/// \brief Returns true if the wire is hidden in traces.
		static bool isHidden(WireFlags inWireFlags) 
			{ return (inWireFlags & eWireFlagHidden) != 0; }
		/// \brief Returns true if the wire is a logic input.
		static bool isInput(WireFlags inWireFlags) 
			{ return (inWireFlags & eWireFlagInput) != 0; }
		/// \brief Returns true if the wire is a logic output.
		static bool isOutput(WireFlags inWireFlags) 
			{ return (inWireFlags & eWireFlagOutput) != 0; }
		/// \brief Returns true if the wire is a remote wire.
		static bool isRemote(WireFlags inWireFlags) 
			{ return (inWireFlags & eWireFlagRemote) != 0; }
		/// \brief Returns true if the wire is a global wire (a what!?).
		static bool isGlobal(WireFlags inWireFlags) 
			{ return (inWireFlags & eWireFlagGlobal) != 0; }
		/// \brief Returns true if the wire may be referenced by a remote wire.
		static bool isRemoteNodeCapable(WireFlags inWireFlags) 
			{ return (inWireFlags & eWireFlagRemoteNodeCapable) != 0; }
		/// \brief Returns true if the wire may be referenced by a remote arc.
		static bool isRemoteArcCapable(WireFlags inWireFlags) 
			{ return (inWireFlags & eWireFlagRemoteArcCapable) != 0; }
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_WIRE_INFO_HPP
