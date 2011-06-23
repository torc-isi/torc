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

#include "torc/bitstream/Spartan6Bitstream.hpp"
#include <boost/crc.hpp>
#include <stdio.h>

namespace torc {
namespace bitstream {

	void Spartan6Bitstream::readPackets(std::istream& inStream) {
		uint16_t bitstreamWordLength = mBitstreamByteLength >> 1;
		uint16_t cumulativeWordLength = 0;
		while(cumulativeWordLength < bitstreamWordLength) {
			push_back(Spartan6Packet::read(inStream));
			cumulativeWordLength += back().getWordSize();
		}
	}

	void Spartan6Bitstream::writePackets(std::ostream& inStream) {
		const_iterator p = begin();
		const_iterator e = end();
		while(p < e) p++->write(inStream);
	}

	void Spartan6Bitstream::preflightPackets(void) {
return;
		uint32_t crc;
		uint32_t address = 0;
		const_iterator p = begin();
		const_iterator e = end();
		boost::crc_basic<32> crc32(0x1EDC6F41, 0, 0, false, false);
		while(p < e) {
			// read packet in
		  	const Spartan6Packet& packet = *p++;
		  	if(!packet.isWrite()) continue;
			address = packet.getAddress();
			uint32_t wordCount = packet.getWordCount();
			if(wordCount == 0) continue;
			// CRC Command
			if(address == 0) {
				printf("\n\nExpected CRC: %X%X\n", packet[1],packet[2]);
				crc = crc32.checksum();
				printf("Calculated CRC: %X\n\n\n", crc);
				crc = 0;
				crc32.reset();
			}
		  	// Reset CRC
			else if(address == 5 && wordCount >= 1 && packet[1] == 7) {
				crc = 0;
				crc32.reset();
				//printf("Reset\n");
		  	}
			else {
				for(uint32_t i = 1; i <= wordCount; i++) {
					uint16_t word = packet[i];
					printf("Address: %2.2X\n", address);
					printf("Word (%d): %4.4X\n", i, word);
					uint16_t j;
					uint16_t mask;
					for(j = 0, mask = 1; j < 16; j++, mask <<= 1) {
					  crc32.process_bits((word & mask) ? 1 : 0, 1);
					  printf("\t%8.8X\n", crc32.checksum());
					}
					for(j = 0, mask = 1; j < 6; j++, mask <<= 1) {
					  crc32.process_bits((address & mask) ? 1 : 0, 1);
					  printf("\t\t%8.8X\n", crc32.checksum());
					}
					printf("\t\t\t%8.8X\n", crc32.checksum());
				}
				// Auto CRC Called
				if (packet.isType2()){
				  crc = crc32.checksum();
  				  printf("Calculated Auto CRC: %X\n", crc);
				  crc = 0;
				  crc32.reset();
				}
			}
		}
	}

	void Spartan6Bitstream::updatePacketLength(void) {
		uint32_t totalWordCount = 0;
		iterator p = begin();
		iterator e = end();
		while(p < e) totalWordCount += (p++)->getWordSize();
		mBitstreamByteLength = totalWordCount << 1;
	}

} // namespace bitstream
} // namespace torc
