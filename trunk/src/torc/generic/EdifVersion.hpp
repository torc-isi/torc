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

#ifndef TORC_GENERIC_EDIFVERSION_HPP
#define TORC_GENERIC_EDIFVERSION_HPP

//BOOST
#include<boost/cstdint.hpp>

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

namespace torc {
namespace generic {

/**
 * Version of EDIF being parsed. This release supports only EDIF 2 0 0.
 */
struct EdifVersion {

#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION
public:
	EdifVersion();

	~EdifVersion() throw ();

	EdifVersion(const EdifVersion& source);

	EdifVersion& operator=(const EdifVersion& source);

public:
	uint8_t mFirst;
	uint8_t mSecond;
	uint8_t mThird;

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_EDIFVERSION_HPP
