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
#include <iomanip>

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
/*
		// prepare CRC variables for use
		uint32_t crc = 0;
		bool c[16];
		bool d[36];
		bool x[16];
		// prepare other variables for use
		uint32_t address = 0;
//		uint32_t loutAddress = 8;
		int j = 0;
		uint32_t mask = 0;
		for(j = 0; j < 16; j++) x[j] = c[j] = false;
		for(j = 0; j < 36; j++) d[j] = false;
		// iterate through the packets
		const_iterator p = begin();
		const_iterator e = end();
		while(p < e) {
			const VirtexPacket& packet = *p++;
//			bool isType1 = packet.isType1();
//			bool isType2 = packet.isType2();
//			bool isRead = packet.isRead();
//			bool isWrite = packet.isWrite();
			if(!packet.isWrite()) continue;
			if(packet.isType1()) address = packet.getAddress();
//			if(address == loutAddress) continue; // LOUT register is excluded from CRC calculations
			uint32_t wordCount = packet.getWordCount();
			if(wordCount == 0) continue;
			if(address == 0) {
std::cout << "Write CRC " << std::hex << std::setfill('0') << std::setw(8) << crc << std::dec << std::endl;
		for(j = 0; j < 16; j++) x[j] = c[j] = false;
				crc = 0;	// write to CRC
			}
			if(address == 4 && wordCount >= 1 && packet[1] == 7) {
std::cout << "Reset CRC " << std::hex << std::setfill('0') << std::setw(8) << crc << std::dec << std::endl;
		for(j = 0; j < 16; j++) x[j] = c[j] = false;
				crc = 0; // RCRC command
			}
			// vectorize the current CRC
			for(j = 0, mask = 1; j < 16; j++, mask <<= 1) c[j] = (crc & mask) != 0;
			// iterate over the data words
			for(uint32_t i = 0; i < wordCount; i++) {
				// vectorize the data word
				uint32_t word = packet[i];
				for(j = 0, mask = 1; j < 32; j++, mask <<= 1) d[j] = (word & mask) != 0;
				for(j = 32, mask = 1; j < 36; j++, mask <<= 1) d[j] = (address & mask) != 0;
				// XOR the CRC with the data
				for(j = 0; j < 16; j++) x[j] = c[j] ^ d[j];
				// compute the new CRC in parallel
				c[0] = x[0]^x[1]^x[2]^x[3]^x[4]^x[5]^x[6]^x[9]^x[10]^x[11]^x[12]^x[13]^x[14]^x[15]
					^d[16]^d[17]^d[18]^d[19]^d[20]^d[21]^d[23]^d[24]^d[25]^d[26]^d[27]^d[28]^d[29]
					^d[30]^d[31]^d[32]^d[33]^d[34]^d[35];
				c[1] = x[7]^x[9]^d[22]^d[23];
				c[2] = x[8]^x[10]^d[23]^d[24];
				c[3] = x[9]^x[11]^d[24]^d[25];
				c[4] = x[10]^x[12]^d[25]^d[26];
				c[5] = x[11]^x[13]^d[26]^d[27];
				c[6] = x[0]^x[12]^x[14]^d[27]^d[28];
				c[7] = x[0]^x[1]^x[13]^x[15]^d[28]^d[29];
				c[8] = x[0]^x[1]^x[2]^x[14]^d[16]^d[29]^d[30];
				c[9] = x[0]^x[1]^x[2]^x[3]^x[15]^d[17]^d[30]^d[31];
				c[10] = x[1]^x[2]^x[3]^x[4]^d[16]^d[18]^d[31]^d[32];
				c[11] = x[2]^x[3]^x[4]^x[5]^d[17]^d[19]^d[32]^d[33];
				c[12] = x[3]^x[4]^x[5]^x[6]^d[18]^d[20]^d[33]^d[34];
				c[13] = x[4]^x[5]^x[6]^x[7]^d[19]^d[21]^d[34]^d[35];
				c[14] = x[0]^x[1]^x[2]^x[3]^x[4]^x[7]^x[8]^x[9]^x[10]^x[11]^x[12]^x[13]^x[14]^x[15]
					^d[16]^d[17]^d[18]^d[19]^d[21]^d[22]^d[23]^d[24]^d[25]^d[26]^d[27]^d[28]^d[29]
					^d[30]^d[31]^d[32]^d[33]^d[34];
				c[15] = x[0]^x[1]^x[2]^x[3]^x[4]^x[5]^x[8]^x[9]^x[10]^x[11]^x[12]^x[13]^x[14]^x[15]
					^d[16]^d[17]^d[18]^d[19]^d[20]^d[22]^d[23]^d[24]^d[25]^d[26]^d[27]^d[28]^d[29]
					^d[30]^d[31]^d[32]^d[33]^d[34]^d[35];
			}
			// convert the vectorized CRC back into an integer
			crc = 0;
			for(j = 0, mask = 1;j < 16;j++, mask <<= 1) if(c[j]) crc |= mask;
std::cout << "CRC " << std::hex << std::setfill('0') << std::setw(8) << crc << std::dec << std::endl;
		}
*/
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
