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
/// \brief Header for the EncapsulatedInteger template.

#ifndef TORC_ENCAPSULATED_INTEGER_HPP
#define TORC_ENCAPSULATED_INTEGER_HPP

namespace torc {
namespace common {

	/// \brief Template base for encapsulated integers, to enforce strong typing
	template <typename T> class EncapsulatedInteger {
	protected:
	// members
		T m; ///< \brief Encapsulated integer.
	public:
	// types
		typedef EncapsulatedInteger<T> type; ///< \brief Alias for the instantiated class type.
		typedef T pod; ///< \brief Alias for the encapsulated Plain-Old-Data type.
	// constructors
		/// \brief Null constructor.
		EncapsulatedInteger(void) { m = 0; }
		/// \brief Copy constructor.
		EncapsulatedInteger(const type& rhs) { m = rhs.m; }
		/// \brief Copy constructor.
		EncapsulatedInteger(const T& rhs) { m = rhs; }
	// operators
		/// \brief Assignment operator (from encapsulated type).
		type& operator =(const type& rhs) { m = rhs.m; return *this; }
		/// \brief Assignment operator.
		type& operator =(const T& rhs) { m = rhs; return *this; }
		/// \brief Equality operator (against encapsulated type).
		bool operator ==(const type& rhs) const { return m == rhs.m; }
		/// \brief Equality operator.
		bool operator ==(const T& rhs) const { return m == rhs; }
		/// \brief Constant cast operator.
		operator const T&(void) const { return m; }
		/// \brief Non-constant cast operator.
		operator T&(void) { return m; }
	// functions
		static inline pod undefined(void) { return static_cast<pod>(-1); }
		inline bool isUndefined(void) const { return m == static_cast<pod>(-1); }
		/// \brief Cast operator.
		//operator const T(void) const { return m; }
		//operator T(void) { return m; }
	};

} // namespace common
} // namespace torc

#endif // TORC_ENCAPSULATED_INTEGER_HPP
