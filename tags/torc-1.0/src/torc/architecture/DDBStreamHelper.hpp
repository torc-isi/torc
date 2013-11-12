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
	// friends
		/// \brief Insertion operator to associate the given device database with the given stream.
		friend std::ostream& operator <<(std::ostream& os, const class DDB& ddb);
	// types
		/// \brief Map of streams to database pointers.
		typedef std::map<std::ostream*, const class DDB*> OStreamToDDBMap;
	// static variables
		/// \brief Map of streams to databases.
		static OStreamToDDBMap sStreamToDatabase;
	public:
		/// \brief Return the device database pointer associated with this stream.
		/// \returns the device database pointer, or 0 if no database has been associated with the 
		///		stream.
		static const class DDB* getDDBPtr(std::ostream& os) {
			OStreamToDDBMap::const_iterator p = sStreamToDatabase.find(&os);
			if(p == sStreamToDatabase.end()) return 0;
			return p->second;
		}
		/// \brief Dissociate the given device database from any stream.
		static void dissociate(const class DDB& ddb) {
			const DDB* ddbPtr = &ddb;
			// iterate through all streams that have an association
			OStreamToDDBMap::iterator p = sStreamToDatabase.begin();
			OStreamToDDBMap::iterator e = sStreamToDatabase.end();
			while(p != e) {
				// if we find a stream associated with this database, remove the association
				if(p->second == ddbPtr) sStreamToDatabase.erase(p);
				p++;
			}
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

