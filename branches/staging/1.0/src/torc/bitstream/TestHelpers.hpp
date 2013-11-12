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
/// \brief Header for Boost.Test helper functions.

#ifndef TORC_BITSTREAM_TESTHELPERS_HPP
#define TORC_BITSTREAM_TESTHELPERS_HPP

#include <boost/filesystem.hpp>
#include <iostream>

namespace torc {
namespace bitstream {

/// \brief Template function to compare the contents of two packet vectors.
template <class V, class P> bool packetVectorsAreIdentical(const V& inVector1, const V& inVector2);
template <class V, class P> bool packetVectorsAreIdentical(const V& inVector1, const V& inVector2) {
	// the vectors are not identical if their lengths differ
	if(inVector1.size() != inVector2.size()) return false;
	// iterate through both vectors
	typename V::const_iterator p1 = inVector1.begin();
	typename V::const_iterator e1 = inVector1.end();
	typename V::const_iterator p2 = inVector2.begin();
	while(p1 < e1) {
		// look up the current packets
		const P& packet1 = *p1++;
		const P& packet2 = *p2++;
		// the packets are not identical if their lengths differ
		uint32_t size1 = packet1.getWordSize();
		uint32_t size2 = packet2.getWordSize();
		if(size1 != size2) return false;
		// iterate through every packet word
		for(uint32_t i = 0; i < size1; i++) if(packet1[i] != packet2[i]) return false;
	}
	// the vectors are identical if we got to here
	return true;
}

/// \brief Template function to recalculate bitstream CRC checksums and compare to reference values.
/// \param inPath The path to the reference bitstream.
template <class V> bool checkCRC(boost::filesystem::path inPath, 
	bool inDebug = false) {
	std::fstream fileStream(inPath.string().c_str(), std::ios::binary | std::ios::in);
	// read packets into the first bitstream and copy them into the second bitstream
	V bitstream1;
	bitstream1.read(fileStream, false);
	V bitstream2(bitstream1);
	// recalculate the CRC checksums in the second bitstream
	if(inDebug) std::cout << bitstream1 << std::endl;
	bitstream2.preflightPackets();
	if(inDebug) std::cout << bitstream2 << std::endl;
	// compare the packet vectors and return the result to the caller
	return packetVectorsAreIdentical<V, typename V::value_type>(bitstream1, bitstream2);
};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_TESTHELPERS_HPP
