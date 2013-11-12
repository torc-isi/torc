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

#include "torc/bitstream/Spartan6Bitstream.hpp"
#include "torc/bitstream/Spartan6.hpp"
#include "torc/common/DeviceDesignator.hpp"
#include <boost/crc.hpp>
#include <stdio.h>

using namespace torc::common;

namespace torc {
namespace bitstream {

	void Spartan6Bitstream::readPackets(std::istream& inStream) {
		uint16_t bitstreamWordLength = mBitstreamByteLength >> 1;
		uint16_t cumulativeWordLength = 0;
		while(cumulativeWordLength < bitstreamWordLength) {
			push_back(Spartan6Packet::read(inStream));
			uint32_t wordSize = back().getWordSize();
			cumulativeWordLength += wordSize;
			// infer Auto CRCs for writes equal to or longer than one frame (not rigorously correct)
			if(wordSize <= getFrameLength()) continue;
			uint16_t autoCrcHi = 0;
			uint16_t autoCrcLo = 0;
			inStream.read((char*) &autoCrcHi, sizeof(autoCrcHi));
			inStream.read((char*) &autoCrcLo, sizeof(autoCrcLo));
			autoCrcHi = ntohs(autoCrcHi);
			autoCrcLo = ntohs(autoCrcLo);
			push_back(Spartan6Packet(autoCrcHi, autoCrcLo));
			cumulativeWordLength += 2;
		}
	}

	void Spartan6Bitstream::writePackets(std::ostream& inStream) {
		const_iterator p = begin();
		const_iterator e = end();
		while(p < e) p++->write(inStream);
	}

	void Spartan6Bitstream::preflightPackets(void) {
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
		case DeviceDesignator::eFamilySpartan6: 
			cmdRegister = Spartan6::eRegisterCMD; rcrcCommand = Spartan6::eCommandRCRC; 
			fdriRegister = Spartan6::eRegisterFDRI; crcRegister = Spartan6::eRegisterCRC; 
			/*addressLength = 6*/ (void) addressLength; autoCrc = true; 
			break;
		default:
			std::cerr << "Unsupported architecture in Spartan6Bitstream::preflightPackets()."
				<< std::endl;
			break;
		}
		// begin CRC calculation: calculation is derived from one of the following simulation files:
		//		ISE/verilog/src/unisims/SIM_CONFIG_S6.v
		//		ISE/vhdl/src/unisims/primitive/SIM_CONFIG_S6.vhd
		CRC crc;
		iterator p = begin();
		iterator e = end();
		while(p < e) {
			// look up the current packet
		  	const Spartan6Packet& packet = *p++;
			// only process write packets with non-zero payload length
		  	if(!packet.isWrite()) continue;
			uint32_t address = packet.getAddress();
			uint32_t wordCount = packet.getWordCount();
			if(wordCount == 0) continue;
			// CRC register write (this is what compares the expected and supplied CRC values)
			if(address == crcRegister) {
				//printf("Expected CRC16:   %8.8x\n", uint32_t(packet[1]) << 16 | packet[2]);
				//printf("Calculated CRC16: %8.8x\n", uint32_t(crc));
				*(p-1) = Spartan6Packet::makeType1Write32(crcRegister, crc);
				crc.reset();
		  	// reset CRC command
			} else if(address == cmdRegister && wordCount >= 1 && packet[1] == rcrcCommand) {
				crc.reset();
			// registers that are omitted from CRC calculation
			} else if(address == Spartan6::eRegisterBOOSTS || address == Spartan6::eRegisterSTAT 
				|| address == Spartan6::eRegisterLOUT || address == Spartan6::eRegisterCSBO 
				|| address == Spartan6::eRegisterRDBK_SIGN || address == Spartan6::eRegisterFDRO) {
			// process packet contents
		  	} else {
				for(uint32_t i = 1; i <= wordCount; i++) {
					uint32_t word = packet[i];
					//printf("Address: %4.4x\n", address);
					//printf("Word: %8.8x\n", word);
					crc.update(address, word);
					// debugging output
					//for(int32_t c = sizeof(crc_curr) - 1; c >= 0; c--) printf("%d", crc_curr[c]);
					//printf("\n");
				}
				// process the Auto CRC
				if(autoCrc && address == fdriRegister) {
					//printf("Expected Auto CRC16:   %8.8x\n", (uint32_t((*p)[0]) << 16) | (*p)[1]);
					//printf("Calculated Auto CRC16: %8.8x\n", uint32_t(crc));
					// current packet is FDRI, next is Auto CRC
					*p = Spartan6Packet(crc >> 16, crc & 0xffff);
					// unlike the Spartan3E, the CRC is not reset after an Auto CRC
					//crc.reset();
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
