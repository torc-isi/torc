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

#include "torc/bitstream/VirtexBitstream.hpp"
#include "torc/bitstream/Virtex.hpp"
#include "torc/bitstream/Virtex2.hpp"
#include "torc/bitstream/Virtex4.hpp"
#include "torc/bitstream/Virtex5.hpp"
#include "torc/bitstream/Virtex6.hpp"
#include "torc/bitstream/Virtex7.hpp"
#include <boost/crc.hpp>
#include <stdio.h>

using namespace torc::common;

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

	void VirtexBitstream::updateFramePackets(EBitstreamType inBitstreamType, 
		EFrameInclude inFrameInclusion) {
		// delete the existing frame packets
		iterator position = deleteFramePackets();
		// insert the new frame packets in place of the old ones
		if(inBitstreamType == eBitstreamTypeFull) {
			insert(position, generateFullBitstreamPackets());
		} else {
			insert(position, generatePartialBitstreamPackets(inFrameInclusion));
		}
	}

	void VirtexBitstream::generateBitstream(EBitstreamType inBitstreamType, 
		EFrameInclude inFrameInclusion) {
		// discard all packets
		clear();
		if(inBitstreamType == eBitstreamTypeFull) {
			// generate the prefix
			append(generateFullBitstreamPrefix());
			// generate the frame packets
			append(generateFullBitstreamPackets());
			// generate the suffix
			append(generateFullBitstreamSuffix());
		} else {
			// generate the prefix
			append(generatePartialBitstreamPrefix(inBitstreamType));
			// generate the frame packets
			append(generatePartialBitstreamPackets(inFrameInclusion));
			// generate the suffix
			append(generatePartialBitstreamSuffix(inBitstreamType));
		}
	}

	void VirtexBitstream::unimplemented(std::string inName) {
		std::cerr << inName << " is not yet implemented for this architecture." << std::endl;
	}

	void VirtexBitstream::readFramePackets(void) {
		unimplemented("readFramePackets(void)");
	}

	VirtexPacketVector::iterator VirtexBitstream::deleteFramePackets(void) {
		unimplemented("deleteFramePackets()");
		return end();
	}

	VirtexPacketVector VirtexBitstream::generateFullBitstreamPackets(void) {
		unimplemented("generateFullBitstreamPackets()");
		return VirtexPacketVector();
	}

	VirtexPacketVector VirtexBitstream::generateFullBitstreamPrefix(void) {
		unimplemented("generateFullBitstreamPrefix()");
		return VirtexPacketVector();
	}

	VirtexPacketVector VirtexBitstream::generateFullBitstreamSuffix(void) {
		unimplemented("generateFullBitstreamSuffix()");
		return VirtexPacketVector();
	}

	VirtexPacketVector VirtexBitstream::generatePartialBitstreamPackets(
		EFrameInclude inFrameInclusion) {
		unimplemented("generatePartialBitstreamPackets()");
		return VirtexPacketVector();
	}

	VirtexPacketVector VirtexBitstream::generatePartialBitstreamPrefix(
		EBitstreamType inBitstreamType) {
		unimplemented("generatePartialBitstreamPrefix()");
		return VirtexPacketVector();
	}

	VirtexPacketVector VirtexBitstream::generatePartialBitstreamSuffix(
		EBitstreamType inBitstreamType) {
		unimplemented("generatePartialBitstreamSuffix()");
		return VirtexPacketVector();
	}

	void VirtexBitstream::preflightPackets(void) {
		// determine which architecture this is
		DeviceDesignator deviceDesignator(getDeviceName());
		DeviceDesignator::EFamily family = deviceDesignator.getFamily();
		switch(family) {
			case DeviceDesignator::eFamilyVirtex: case DeviceDesignator::eFamilyVirtexE:
			case DeviceDesignator::eFamilyVirtex2: case DeviceDesignator::eFamilyVirtex2P:
				updateCrc16(family); break;
			case DeviceDesignator::eFamilyVirtex4: case DeviceDesignator::eFamilyVirtex5: 
			case DeviceDesignator::eFamilyVirtex6: case DeviceDesignator::eFamilyArtix7:
			case DeviceDesignator::eFamilyKintex7: case DeviceDesignator::eFamilyVirtex7: 
			case DeviceDesignator::eFamilyZynq7000: default:
				updateCrc32(family); break;
		}
	}

	void VirtexBitstream::updateCrc16(DeviceDesignator::EFamily inFamily) {
		// set up family-specific variables
		uint32_t crcRegister = 0;
		uint32_t cmdRegister = 0;
		uint32_t fdriRegister = 0;
		uint32_t rcrcCommand = 0;
		uint32_t addressLength = 0;
		bool autoCrc = false;
		switch(inFamily) {
		case DeviceDesignator::eFamilyVirtex: case DeviceDesignator::eFamilyVirtexE: 
			cmdRegister = Virtex::eRegisterCMD; rcrcCommand = Virtex::eCommandRCRC; 
			fdriRegister = Virtex::eRegisterFDRI; crcRegister = Virtex::eRegisterCRC; 
			addressLength = 4; autoCrc = false; 
			break;
		case DeviceDesignator::eFamilyVirtex2: case DeviceDesignator::eFamilyVirtex2P: 
			cmdRegister = Virtex2::eRegisterCMD; rcrcCommand = Virtex2::eCommandRCRC; 
			fdriRegister = Virtex2::eRegisterFDRI; crcRegister = Virtex::eRegisterCRC; 
			addressLength = 5; autoCrc = true; 
			break;
		default:
			std::cerr << "Unsupported architecture in VirtexBitstream::updateCrc16()." << std::endl;
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
		  	const VirtexPacket& packet = *p++;
			// only process write packets with non-zero payload length
		  	if(!packet.isWrite()) continue;
			if(packet.isType1()) address = packet.getAddress();
			uint32_t wordCount = packet.getWordCount();
			if(wordCount == 0) continue;
			// CRC register write (this is what compares the expected and supplied CRC values)
			if(address == crcRegister) {
				//printf("Expected CRC16:   %4.4x\n", packet[1]);
				//printf("Calculated CRC16: %4.4x\n", crc16.checksum());
				*(p-1) = VirtexPacket::makeType1Write(crcRegister, crc16.checksum());
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
				// process the Auto CRC on Virtex2 and Virtex2P
				if(autoCrc && address == fdriRegister) {
					//printf("Expected Auto CRC16:   %4.4x\n", packet[1]);
					//printf("Calculated Auto CRC16: %4.4x\n", crc16.checksum());
					*p = VirtexPacket(crc16.checksum()); // current word is FDRI, next is Auto CRC
					crc16.reset();
				}
			}
		}
	}

	void VirtexBitstream::updateCrc32(DeviceDesignator::EFamily inFamily) {
		// set up family-specific variables
		uint32_t crcRegister = 0;
		uint32_t cmdRegister = 0;
		uint32_t rcrcCommand = 0;
		uint32_t addressLength = 5;
		switch(inFamily) {
		case DeviceDesignator::eFamilyVirtex4: 
			cmdRegister = Virtex4::eRegisterCMD; rcrcCommand = Virtex4::eCommandRCRC; 
			crcRegister = Virtex4::eRegisterCRC; 
			break;
		case DeviceDesignator::eFamilyVirtex5: 
			cmdRegister = Virtex5::eRegisterCMD; rcrcCommand = Virtex5::eCommandRCRC; 
			crcRegister = Virtex5::eRegisterCRC; 
			break;
		case DeviceDesignator::eFamilyVirtex6: 
			cmdRegister = Virtex6::eRegisterCMD; rcrcCommand = Virtex6::eCommandRCRC; 
			crcRegister = Virtex6::eRegisterCRC; 
			break;
		case DeviceDesignator::eFamilyArtix7: case DeviceDesignator::eFamilyKintex7: 
		case DeviceDesignator::eFamilyVirtex7: case DeviceDesignator::eFamilyZynq7000: 
			cmdRegister = Virtex7::eRegisterCMD; rcrcCommand = Virtex7::eCommandRCRC; 
			crcRegister = Virtex7::eRegisterCRC; 
			break;
		default:
			std::cerr << "Unsupported architecture in VirtexBitstream::updateCrc32()." << std::endl;
			break;
		}
		// begin CRC calculation
		uint32_t address = 0;
		iterator p = begin();
		iterator e = end();
		// CRC-32C (Castagnoli) polynomial for Virtex4/5/6/7 families
		boost::crc_basic<32> crc32(0x1edc6f41, 0, 0, false, true);
		while(p < e) {
			// look up the current packet
		  	const VirtexPacket& packet = *p++;
			// only process write packets with non-zero payload length
		  	if(!packet.isWrite()) continue;
			if(packet.isType1()) address = packet.getAddress();
			uint32_t wordCount = packet.getWordCount();
			if(wordCount == 0) continue;
			// CRC register write (this is what compares the expected and supplied CRC values)
			if(address == crcRegister) {
				//printf("Expected CRC32:   %8.8x\n", packet[1]);
				//printf("Calculated CRC32: %8.8x\n", crc32.checksum());
				*(p-1) = VirtexPacket::makeType1Write(crcRegister, crc32.checksum());
				crc32.reset();
		  	// reset CRC command
			} else if(address == cmdRegister && wordCount >= 1 && packet[1] == rcrcCommand) {
				crc32.reset();
			// process packet contents
		  	} else {
				uint32_t j;
				uint32_t mask;
				for(uint32_t i = 1; i <= wordCount; i++) {
					uint32_t word = packet[i];
					//printf("Address: %4.4x\n", address);
					//printf("Word: %8.8x\n", word);
					for(j = 0, mask = 1; j < 32; j++, mask <<= 1) {
						crc32.process_bit((word & mask) ? 1 : 0);
					}
					for(j = 0, mask = 1; j < addressLength; j++, mask <<= 1) {
						crc32.process_bit((address & mask) ? 1 : 0);
					}
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
