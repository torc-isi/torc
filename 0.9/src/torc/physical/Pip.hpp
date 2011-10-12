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
/// \brief Header for the Pip class.

#ifndef TORC_PHYSICAL_PIP_HPP
#define TORC_PHYSICAL_PIP_HPP

#include "torc/physical/Progeny.hpp"
#include "torc/physical/Routethrough.hpp"
#include <string>
#include <vector>
#include <boost/smart_ptr.hpp>

namespace torc {
namespace physical {

	/// \brief Physical design programmable interconnect point.
	/// \details This class represents a pip (programmable interconnect point) on a physical design 
	//		net.
	class Pip : public Progeny<class Net> {
	// friends
		/// \brief The Factory class has direct access to our internals.
		friend class Factory;
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
	// members
		/// \brief The containing tile for this pip.
		TileName mTileName;
		/// \brief The source wire for this pip.
		WireName mSourceWireName;
		/// \brief The sink wire for this pip.
		WireName mSinkWireName;
		/// \brief The connection directionality for this pip.  See EPipDirection.
		EPipDirection mDirection;
		/// \brief The routethrough shared pointer.
		RoutethroughSharedPtr mRoutethroughPtr;
		/// \brief String representation of pip directions.
		static const char* sPipDirectionStrings[];
	// constructors
		/// \brief Protected constructor.
		/// \param inTileName The containing tile.
		/// \param inSourceWireName The source wire.
		/// \param inSinkWireName The sink wire.
		/// \param inPipDirection The pip directionality.  See EPipDirection.
		/// \param inRoutethroughPtr The routethrough shared pointer.
		Pip(const string& inTileName, const string& inSourceWireName, const string& inSinkWireName, 
			EPipDirection inPipDirection, RoutethroughSharedPtr inRoutethroughPtr) 
			: mTileName(inTileName), mSourceWireName(inSourceWireName), 
			mSinkWireName(inSinkWireName), mDirection(inPipDirection), 
			mRoutethroughPtr(inRoutethroughPtr) {}
	public:
	// static
		/// \brief Returns the pip directionality as a string.
		static const char* getDirectionString(EPipDirection inPipDirection);
	// accessors
		/// \brief Returns the pip tile.
		const TileName& getTileName(void) const { return mTileName; }
		/// \brief Returns the pip source wire.
		const WireName& getSourceWireName(void) const { return mSourceWireName; }
		/// \brief Returns the pip sink wire.
		const WireName& getSinkWireName(void) const { return mSinkWireName; }
		/// \brief Returns the pip directionality.
		EPipDirection getDirection(void) const { return mDirection; }
		/// \brief Returns the pip directionality as a string.
		const char* getDirectionString(void) const;
		/// \brief Returns the pip's routethrough pointer.
		RoutethroughSharedPtr getRoutethroughPtr(void) const { return mRoutethroughPtr; }
	// tests
		/// \brief Indicates whether or not the pip has an associated routethrough
		bool isRoutethrough(void) const { return mRoutethroughPtr.get() != 0; }
	// operators
		/// \brief Equality operator.
		bool operator ==(const Pip& rhs) const 
			{ return mTileName == rhs.mTileName && mSourceWireName == rhs.mSourceWireName 
			&& mSinkWireName == rhs.mSinkWireName && mDirection == rhs.mDirection; }
	};

	/// \brief Vector of pips.
	typedef std::vector<Pip> PipVector;

	/// \brief Shared pointer encapsulation of a Pip.
	typedef boost::shared_ptr<Pip> PipSharedPtr;

	/// \brief Weak pointer encapsulation of a Pip.
	typedef boost::weak_ptr<Pip> PipWeakPtr;

	/// \brief Vector of Pip shared pointers.
	typedef std::vector<PipSharedPtr> PipSharedPtrVector;

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_PIP_HPP
