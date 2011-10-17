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
/// \brief Header for the DDBStreamHelper class.

#ifndef TORC_ARCHITECTURE_DDBSTREAMHELPER_HPP
#define TORC_ARCHITECTURE_DDBSTREAMHELPER_HPP

#include "torc/common/EncapsulatedInteger.hpp"
#include <ostream>
#include <map>

namespace torc {
namespace architecture {

	/// \brief Device database stream helper class.
	/// \details This helper class allows the caller to associate a device database with an output 
	///		stream.  This allows extremely lightweight classes like Tilewire to decorate their 
	///		output with additional information, without requiring their own database reference.
	/// \note The database reference only needs to be inserted one time into the stream.
	///	\details <p><pre>std::cout << ddb << ...;</pre>
	class DDBStreamHelper {
	protected:
	// types
		/// \brief Map of streams to iwords.
		typedef std::map<std::ostream*, int> OStreamToIntMap;
	// static variables
		/// \brief Map of streams to iwords.
		static OStreamToIntMap sIWordIndexes;
	public:
	// types
		/// \brief Encapsulation of a stream iword used as a pword index.
		typedef torc::common::EncapsulatedInteger<int> IWordIndex;
	// functions
		/// \brief Return the iword index for this stream, or allocate it if not defined.
		static IWordIndex getIWordIndex(std::ostream& os) {
			OStreamToIntMap::const_iterator index = sIWordIndexes.find(&os);
			if(index == sIWordIndexes.end()) {
				std::pair<OStreamToIntMap::iterator, bool> inserted 
					= sIWordIndexes.insert(std::make_pair(&os, os.xalloc()));
				index = inserted.first;
			}
			return index->second;
		}
		/// \brief Return the device database pointer associated with this stream.
		/// \returns the device database pointer, or 0 if no database has been associated with the 
		///		stream.
		static const class DDB* getDDBPtr(std::ostream& os) {
			return static_cast<DDB*>(os.pword(getIWordIndex(os)));
		}
	};

	/// \brief Insertion operator to associate the given device database with the given stream.
	/// \details This operator does not generate any stream output, but instead associates the 
	///		given device database with the stream.  This allows lightweight objects such as 
	///		tilewires to decorate their output with database information, without including a 
	///		database reference.
	std::ostream& operator <<(std::ostream& os, const class DDB& ddb);

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_DDBSTREAMHELPER_HPP
