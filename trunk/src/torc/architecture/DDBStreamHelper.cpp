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
/// \brief Source for the DDBStreamHelper class.

#include "torc/architecture/DDBStreamHelper.hpp"
#include "torc/architecture/DDB.hpp"

namespace torc {
namespace architecture {

	DDBStreamHelper::OStreamToDDBMap DDBStreamHelper::sStreamToDatabase;

	std::ostream& operator <<(std::ostream& os, const DDB& ddb) {
		// check to see whether there is an existing database association for this stream
		typedef std::map<std::ostream*, const class DDB*> OStreamToDDBMap;
		OStreamToDDBMap::const_iterator p = DDBStreamHelper::sStreamToDatabase.find(&os);		
		if(p == DDBStreamHelper::sStreamToDatabase.end()) {
			// if no association exists, create a new association between the stream and database
			std::pair<OStreamToDDBMap::iterator, bool> inserted
				= DDBStreamHelper::sStreamToDatabase.insert(std::make_pair(&os, &ddb));
			p = inserted.first; // implicitly assuming that insertion always succeeds
		}
		return os;
	}

} // namespace architecture
} // namespace torc

