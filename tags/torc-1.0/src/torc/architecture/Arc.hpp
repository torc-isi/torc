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
/// \brief Header for the Arc class.

#ifndef TORC_ARCHITECTURE_ARC_HPP
#define TORC_ARCHITECTURE_ARC_HPP

#include "torc/architecture/Tilewire.hpp"

namespace torc {
namespace architecture {

	/// \brief Encapsulation of an arc between two tilewires.
	class Arc {
	protected:
	// members
		/// \brief The source tilewire.
		Tilewire mSourceTilewire;
		/// \brief The sink tilewire.
		Tilewire mSinkTilewire;
	public:
	// constructors
		/// \brief Null constructor.
		/// \details The source and sink tilewires will be set to Tilewire::sInvalid.
		Arc(void) : mSourceTilewire(), mSinkTilewire() {}
		/// \brief Public constructor.
		Arc(const Tilewire& inSourceTilewire, const Tilewire& inSinkTilewire) 
			: mSourceTilewire(inSourceTilewire), mSinkTilewire(inSinkTilewire) {}
	// accessors
		/// \brief Returns the source tilewire.
		const Tilewire& getSourceTilewire(void) const { return mSourceTilewire; }
		/// \brief Returns the sink tilewire.
		const Tilewire& getSinkTilewire(void) const { return mSinkTilewire; }
	// operators
		/// \brief Equality operator.
		bool operator ==(const Arc& rhs) const {
			return mSourceTilewire == rhs.mSourceTilewire && mSinkTilewire == rhs.mSinkTilewire;
		}
		/// \brief Comparison operator.
		bool operator <(const Arc& rhs) const {
			if (mSinkTilewire < rhs.mSinkTilewire) return true;
			if (mSinkTilewire == rhs.mSinkTilewire
				&& mSourceTilewire < rhs.mSourceTilewire) return true;
			return false;
		}
	// functions
		bool isUndefined(void) const {
			return mSourceTilewire.isUndefined() || mSinkTilewire.isUndefined();
		}
	// friends
		/// \brief Return a hash value for the specified arc.
		friend std::size_t hash_value(const Arc& inArc);
	};

	class InvalidArcException {
	public:
	// members
		Arc mArc;
	// constructors
		InvalidArcException(const Arc& inArc) : mArc(inArc) {}
	};

	/// \brief Vector of Arc objects.
	typedef std::vector<Arc> ArcVector;

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_ARC_HPP
