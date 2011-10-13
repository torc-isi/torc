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

#include "torc/bitstream/VirtexBitstream.hpp"
#include <boost/crc.hpp>
#include <stdio.h>

namespace torc {
namespace bitstream {

	void VirtexBitstream::readPackets(std::istream& inStream) {
		uint32_t bitstreamWordLength = mBitstreamByteLength >> 2;
		uint32_t cumulativeWordLength = 0;
		while(cumulativeWordLength < bitstreamWordLength) {
			push_back(VirtexPacket::read(inStream));
			cumulativeWordLength += back().getWordSize();
		}
	}

	void VirtexBitstream::writePackets(std::ostream& inStream) {
		const_iterator p = begin();
		const_iterator e = end();
		while(p < e) p++->write(inStream);
	}

	void VirtexBitstream::preflightPackets(void) {
		uint32_t crc;
		uint32_t address = 0;
		uint32_t addressLength = 0;
		bool crctype; //true for 32 bit crc
		string DeviceName = getDeviceName();
		switch (DeviceName[0])
		{
		  //Virtex 2, 2P
		  case '2':
			addressLength = 5;
			crctype = false;
			break;
		  //Virtex 4, 5, 6
		  case '4':
		  case '5':
		  case '6':
			addressLength = 5;
			crctype = true;
			break;
		  //Virtex, E
		  case 'v':
			addressLength = 4;
			crctype = false;
			break;
		  default:
			addressLength = 5;
			crctype = true;
			break;
		}
		const_iterator p = begin();
		const_iterator e = end();
		boost::crc_basic<16> crc16(0x8005, 0, 0, false, true);
		boost::crc_basic<32> crc32(0x1EDC6F41, 0, 0, false, true);
		while(p < e) {
			// read packet in
		  	const VirtexPacket& packet = *p++;
		  	if(!packet.isWrite()) continue;
			if(packet.isType1()) address = packet.getAddress();
			uint32_t wordCount = packet.getWordCount();
			if(wordCount == 0) continue;
			// CRC Command
			if(address == 0) {
				printf("Expected CRC: %x\n", packet[1]);
				if (crctype) {
				  crc = crc32.checksum();
				  printf("Calculated CRC32: %x\n", crc);
				}
				else {
				  crc = crc16.checksum();
  				  //printf("Calculated CRC16: %x\n", crc);
				}
				crc = 0;
				crc16.reset();
				crc32.reset();
			}
		  	// Reset CRC
			else if(address == 4 && wordCount >= 1 && packet[1] == 7) {
				crc = 0;
				crc16.reset();
				crc32.reset();
		  	}
			else {
			  	uint32_t i;
				for(i = 1; i <= wordCount; i++) {
					uint32_t word = packet[i];
					//printf("Address: %4.4x\n", address);
					//printf("Word: %8.8x\n", word);
					uint32_t j;
					uint32_t mask;
					for(j = 0, mask = 1; j < 32; j++, mask <<= 1) {
					  crc16.process_bits((word & mask) ? 1 : 0, 1);
					  //printf("\t%4.4X\n", crc16.checksum());
					  crc32.process_bits((word & mask) ? 1 : 0, 1);
					  //printf("\t%8.8X\n", crc32.checksum());
					}
					for(j = 0, mask = 1; j < addressLength; j++, mask <<= 1) {
					  crc16.process_bits((address & mask) ? 1 : 0, 1);
					  //printf("\t%4.4X\n", crc16.checksum());
					  crc32.process_bits((address & mask) ? 1 : 0, 1);
					  //printf("\t%8.8X\n", crc32.checksum());
					}
					//printf("\t%4.4x\n", crc16.checksum());
				}
				// Auto CRC called only in Virtex 2, 2P
				if (packet.isType2() && DeviceName[0] == '2') {
				  crc = crc16.checksum();
  				  printf("Calculated Auto CRC16: %x\n", crc);
				  crc = 0;
				  crc16.reset();
				}
			}
		}
	}

	void VirtexBitstream::updatePacketLength(void) {
		uint32_t totalWordCount = 0;
		iterator p = begin();
		iterator e = end();
		while(p < e) totalWordCount += (p++)->getWordSize();
		mBitstreamByteLength = totalWordCount << 2;
	}
	
} // namespace bitstream
} // namespace torc
