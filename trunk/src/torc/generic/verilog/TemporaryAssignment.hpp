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
/// \brief Header for the TemporaryAssignment class.

#ifndef TORC_GENERIC_VERILOG_TEMPORARYASSIGNMENT_HPP
#define TORC_GENERIC_VERILOG_TEMPORARYASSIGNMENT_HPP

namespace torc {
namespace generic {

	/// \brief Template class that stores the current value of a variable and restores that value 
	///		when this object goes out of scope.
	/// \note This class inspired a StackOverflow question, and there may be more general ways to 
	///		solve the problem.
	/// \see http://stackoverflow.com/questions/13383683/reverting-temporary-assignment-in-c
	template <typename TYPE> class TemporaryAssignment {
	protected:
	// members
		/// \brief Reference to the variable.
		TYPE& mVariable;
		/// \brief Original value of the variable
		TYPE mOriginalValue;
	public:
	// constructors
		/// \brief Public constructor that does not assign a new value.
		/// \param inVariable The variable to save and restore.
		TemporaryAssignment(TYPE& inVariable) 
			: mVariable(inVariable), mOriginalValue(inVariable) {
		}
		/// \brief Public constructor that assigns a new value
		/// \param inVariable The variable to save and restore.
		/// \param inValue The new value to use.
		TemporaryAssignment(TYPE& inVariable, TYPE inValue) 
			: mVariable(inVariable), mOriginalValue(inVariable) {
			mVariable = inValue;
		}
		/// \brief Public destructor to restore the original value.
		~TemporaryAssignment(void) {
			mVariable = mOriginalValue;
		}
	};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_VERILOG_TEMPORARYASSIGNMENT_HPP
