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

#include "torc/bitstream/SpartanBitstream.hpp"
#include "torc/bitstream/Spartan3E.hpp"
#include "torc/common/DeviceDesignator.hpp"
#include <boost/crc.hpp>
#include <stdio.h>

using namespace torc::common;

namespace torc {
namespace bitstream {

	void SpartanBitstream::readPackets(std::istream& inStream) {
		uint32_t bitstreamWordLength = mBitstreamByteLength >> 2;
		uint32_t cumulativeWordLength = 0;
		while(cumulativeWordLength < bitstreamWordLength) {
			push_back(SpartanPacket::read(inStream));
			uint32_t wordSize = back().getWordSize();
			cumulativeWordLength += wordSize;
			// infer Auto CRCs for writes equal to or longer than one frame (not rigorously correct)
			if(wordSize <= getFrameLength()) continue;
			uint32_t autoCrc = 0;
			inStream.read((char*) &autoCrc, sizeof(autoCrc));
			autoCrc = ntohl(autoCrc);
			push_back(SpartanPacket(autoCrc));
			cumulativeWordLength++;
		}
	}

	void SpartanBitstream::writePackets(std::ostream& inStream) {
		const_iterator p = begin();
		const_iterator e = end();
		while(p < e) p++->write(inStream);
	}

	void SpartanBitstream::preflightPackets(void) {
		// determine which architecture this is
		DeviceDesignator deviceDesignator(getDeviceName());
		DeviceDesignator::EFamily family = deviceDesignator.getFamily();
		// set up family-specific variables
		uint32_t crcRegister = 0;
		uint32_t cmdRegister = 0;
		uint32_t fdriRegister = 0;
		uint32_t rcrcCommand = 0;
		uint32_t addressLength = 0;
		bool autoCrc = false;
		switch(family) {
		case DeviceDesignator::eFamilySpartan3E: 
			cmdRegister = Spartan3E::eRegisterCMD; rcrcCommand = Spartan3E::eCommandRCRC; 
			fdriRegister = Spartan3E::eRegisterFDRI; crcRegister = Spartan3E::eRegisterCRC; 
			addressLength = 5; autoCrc = true; 
			break;
		default:
			std::cerr << "Unsupported architecture in SpartanBitstream::preflightPackets()."
				<< std::endl;
			break;
		}
		// begin CRC calculation
		uint32_t address = 0;
		iterator p = begin();
		iterator e = end();
		// CRC-16-IBM polynomial for Spartan3/6 and Virtex/E/2/2P families
		boost::crc_basic<16> crc16(0x8005, 0, 0, false, true);
		while(p < e) {
			// look up the current packet
		  	const SpartanPacket& packet = *p++;
			// only process write packets with non-zero payload length
		  	if(!packet.isWrite()) continue;
			if(packet.isType1()) address = packet.getAddress();
			uint32_t wordCount = packet.getWordCount();
			if(wordCount == 0) continue;
			// CRC register write (this is what compares the expected and supplied CRC values)
			if(address == crcRegister) {
				//printf("Expected CRC16:   %4.4x\n", packet[1]);
				//printf("Calculated CRC16: %4.4x\n", crc16.checksum());
				*(p-1) = SpartanPacket::makeType1Write(crcRegister, crc16.checksum());
				crc16.reset();
		  	// reset CRC command
			} else if(address == cmdRegister && wordCount >= 1 && packet[1] == rcrcCommand) {
				crc16.reset();
			// process packet contents
		  	} else {
				uint32_t j;
				uint32_t mask;
				for(uint32_t i = 1; i <= wordCount; i++) {
					uint32_t word = packet[i];
					//printf("Address: %4.4x\n", address);
					//printf("Word: %8.8x\n", word);
					for(j = 0, mask = 1; j < 32; j++, mask <<= 1) {
						crc16.process_bit((word & mask) ? 1 : 0);
					}
					for(j = 0, mask = 1; j < addressLength; j++, mask <<= 1) {
						crc16.process_bit((address & mask) ? 1 : 0);
					}
				}
				// process the Auto CRC
				if(autoCrc && address == fdriRegister) {
					//printf("Expected Auto CRC16:   %4.4x\n", (*p)[0]);
					//printf("Calculated Auto CRC16: %4.4x\n", crc16.checksum());
					*p = SpartanPacket(crc16.checksum()); // current word is FDRI, next is Auto CRC
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
