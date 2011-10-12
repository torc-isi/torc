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
/// \brief Header for endian conversion.

#ifndef TORC_ENDIAN_HPP
#define TORC_ENDIAN_HPP

#include <boost/integer.hpp>
#include <boost/detail/endian.hpp>

// pull in ntohs(), ntohl(), htons(), and ntonl()
#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <netinet/in.h>
#endif
//#if !defined(ntohs) || !defined(ntohl) || !defined(htons) || !defined(htonl)
//	#error Byte swapping macros have not been imported for this platform.
//#endif

namespace torc {
namespace common {

/// \brief Convert 64-bit types from network to host byte order.
/// \details Complements ntohs and ntohl, both found in <netinet/in.h> or <winsock2.h>.
boost::uint64_t inline ntohll(const boost::uint64_t& x) {
#ifdef BOOST_BIG_ENDIAN
	return x;
#else
	return
		(x>>56) | 
		((x<<40) & 0x00FF000000000000ull) |
		((x<<24) & 0x0000FF0000000000ull) |
		((x<<8)  & 0x000000FF00000000ull) |
		((x>>8)  & 0x00000000FF000000ull) |
		((x>>24) & 0x0000000000FF0000ull) |
		((x>>40) & 0x000000000000FF00ull) |
		(x<<56);
#endif
}

/// \brief Convert 64-bit types from host to network byte order.
/// \details Complements htons and htonl, both found in <netinet/in.h> or <winsock2.h>.  
// I'm sure there's a better way to swap the byte order for both ntohll() and htonll() without 
// replicating the code, but it'll be inlined anyway, and with this approach both ntohll() and 
// htonll() will show up symmetrically in doxygen.
boost::uint64_t inline htonll(const boost::uint64_t& x) {
#ifdef BOOST_BIG_ENDIAN
	return x;
#else
	return
		(x>>56) | 
		((x<<40) & 0x00FF000000000000ull) |
		((x<<24) & 0x0000FF0000000000ull) |
		((x<<8)  & 0x000000FF00000000ull) |
		((x>>8)  & 0x00000000FF000000ull) |
		((x>>24) & 0x0000000000FF0000ull) |
		((x>>40) & 0x000000000000FF00ull) |
		(x<<56);
#endif
}

} // namespace common
} // namespace torc

#endif // TORC_ENDIAN_HPP
