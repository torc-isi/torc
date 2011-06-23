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

#include "torc/bitstream/SpartanBitstream.hpp"
#include <boost/crc.hpp>
#include <stdio.h>

namespace torc {
namespace bitstream {

	void SpartanBitstream::readPackets(std::istream& inStream) {
		uint32_t bitstreamWordLength = mBitstreamByteLength >> 2;
		uint32_t cumulativeWordLength = 0;
		while(cumulativeWordLength < bitstreamWordLength) {
			push_back(SpartanPacket::read(inStream));
			cumulativeWordLength += back().getWordSize();
		}
	}

	void SpartanBitstream::writePackets(std::ostream& inStream) {
		const_iterator p = begin();
		const_iterator e = end();
		while(p < e) p++->write(inStream);
	}

	void SpartanBitstream::preflightPackets(void) {
		uint32_t crc;
		uint32_t address = 0;
		const_iterator p = begin();
		const_iterator e = end();
		boost::crc_basic<16> crc16(0x8005, 0, 0, false, true);
		while(p < e) {
			// read packet in
		  	const SpartanPacket& packet = *p++;
		  	if(!packet.isWrite()) continue;
			if(packet.isType1()) address = packet.getAddress();
			uint32_t wordCount = packet.getWordCount();
			if(wordCount == 0) continue;
			// CRC Command
			if(address == 0) {
				printf("Expected CRC: %x\n", packet[1]);
				crc = crc16.checksum();
				printf("Calculated CRC16: %x\n", crc);;
				crc = 0;
				crc16.reset();
			}
		  	// Reset CRC
			else if(address == 4 && wordCount >= 1 && packet[1] == 7) {
				crc = 0;
				crc16.reset();
		  	}
			else {
				for(uint32_t i = 1; i <= wordCount; i++) {
					uint32_t word = packet[i];
					//printf("Address: %4.4x\n", address);
					//printf("Word: %8.8x\n", word);
					uint32_t j;
					uint32_t mask;
					for(j = 0, mask = 1; j < 32; j++, mask <<= 1) {
					  crc16.process_bits((word & mask) ? 1 : 0, 1);
					}
					for(j = 0, mask = 1; j < 5; j++, mask <<= 1) {
					  crc16.process_bits((address & mask) ? 1 : 0, 1);
					}
					//printf("\t%4.4x\n", crc16.checksum());
				}
				// Auto CRC Called
				if (packet.isType2()){
				  crc = crc16.checksum();
  				  printf("Calculated Auto CRC16: %x\n", crc);
				  crc = 0;
				  crc16.reset();
				}
			}
		}
	}

	void SpartanBitstream::updatePacketLength(void) {
		uint32_t totalWordCount = 0;
		iterator p = begin();
		iterator e = end();
		while(p < e) totalWordCount += (p++)->getWordSize();
		mBitstreamByteLength = totalWordCount << 2;
	}

} // namespace bitstream
} // namespace torc
