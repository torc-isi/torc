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
/// \brief Header for the NullOutputStream class.
/// \details Credit for this code goes to Dietmar Kuehl for a July 22, 2005 response to a post on
///		http://bytes.com/topic/c/answers/127843-null-output-stream#post444998.

#ifndef TORC_COMMON_NULLOUTPUTSTREAM_HPP
#define TORC_COMMON_NULLOUTPUTSTREAM_HPP

#include <ostream>

namespace torc {
namespace common {

	/// \brief Null stream buffer used by NullOutputStream.
	/// \details Note that this stream buffer is not necessary for the NullOutputStream if we do 
	///		not care about the stream's badbit being set.
	class NullStreamBuffer : public std::streambuf {
		/// \brief Responds that we never overflow our buffer, and can thus keep writing 
		///		indefinitely.
		int overflow(int c) { return traits_type::not_eof(c); }
	};

	/// \brief Output stream that discards everything it receives.
	class NullOutputStream : public std::ostream {
	protected:
	// members
		/// \brief The null stream buffer that supports our operations.
		NullStreamBuffer mNullStreamBuf;
	public:
	// constructors
		/// \brief Public constructor.
		/// \details Constructs this output stream with a null stream buffer.
		NullOutputStream(void) : std::ios(&mNullStreamBuf), std::ostream(&mNullStreamBuf) {}
	};

	extern NullOutputStream cnull;

} // namespace common
} // namespace torc

#endif // TORC_COMMON_NULLOUTPUTSTREAM_HPP
