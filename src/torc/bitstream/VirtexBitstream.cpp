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

	// template instantiations
	template void VirtexBitstream::readFramePackets4567<Virtex7>(uint32_t inBlockFrameIndexBounds[],
		std::map<Virtex7::FrameAddress, uint32_t>& inFrameAddressToIndex, 
		std::map<uint32_t, Virtex7::FrameAddress>& inFrameIndexToAddress);
	template void VirtexBitstream::readFramePackets4567<Virtex6>(uint32_t inBlockFrameIndexBounds[],
		std::map<Virtex6::FrameAddress, uint32_t>& inFrameAddressToIndex, 
		std::map<uint32_t, Virtex6::FrameAddress>& inFrameIndexToAddress);
	template void VirtexBitstream::readFramePackets4567<Virtex5>(uint32_t inBlockFrameIndexBounds[],
		std::map<Virtex5::FrameAddress, uint32_t>& inFrameAddressToIndex, 
		std::map<uint32_t, Virtex5::FrameAddress>& inFrameIndexToAddress);
	template void VirtexBitstream::readFramePackets4567<Virtex4>(uint32_t inBlockFrameIndexBounds[],
		std::map<Virtex4::FrameAddress, uint32_t>& inFrameAddressToIndex, 
		std::map<uint32_t, Virtex4::FrameAddress>& inFrameIndexToAddress);
	// template definition
	template <class ARCH> void VirtexBitstream::readFramePackets4567(
		uint32_t inBlockFrameIndexBounds[], 
		std::map<typename ARCH::FrameAddress, uint32_t>& inFrameAddressToIndex, 
		std::map<uint32_t, typename ARCH::FrameAddress>& inFrameIndexToAddress) {
		// clean up the template parameters
		typedef typename ARCH::FrameAddress FrameAddress;
		typedef std::map<FrameAddress, uint32_t> FrameAddressToIndex;
		typedef std::map<uint32_t, FrameAddress> FrameIndexToAddress;
		// declare a shared word array
		boost::shared_array<uint32_t> frameWords;
		// precalculate the start indexes of all blocks, and create all possible frames
		uint32_t frameStart[eBlockTypeCount + 1];
		frameStart[0] = 0;
		VirtexFrame frame(ARCH::eFrameLength);
		for(int i = 0; i < eBlockTypeCount; i++) {
			frameStart[i + 1] = frameStart[i] + inBlockFrameIndexBounds[i];
			// clear any existing frames
			mFrameBlocks.mBlock[i].clear();
			// populate the block with the correct number of empty frames
			for(uint32_t j = 0; j < inBlockFrameIndexBounds[i]; j++) {
				mFrameBlocks.mBlock[i].push_back(VirtexFrameSet::FrameSharedPtr
					(new VirtexFrame(getFrameLength())));
				mFrameBlocks.mBlock[i].back()->setUsed(false);
			}
		}
		// walk the bitstream and extract all frames
		const uint32_t frameLength = getFrameLength();
		uint32_t frameIndex = 0;
		FrameAddress frameAddress;
		typename ARCH::ERegister lastAddress = typename ARCH::ERegister();
		typename ARCH::iterator p = begin();
		typename ARCH::iterator e = end();
		while(p < e) {
		    const VirtexPacket& packet = *p++;
			// process FAR write packets
			if(packet.isWrite() && packet.getAddress() == ARCH::eRegisterFAR) {
				// extract the new frame address
				frameAddress = typename ARCH::FrameAddress(packet[1]);
				// convert the frame address to the corresponding frame index
				typename FrameAddressToIndex::iterator ip 
					= inFrameAddressToIndex.find(frameAddress);
				if(ip != inFrameAddressToIndex.end()) frameIndex = ip->second;
			}
			// process FDRI write packets
			else if(packet.isWrite() 
				&& (
					// this is a Type 2 packet and the prior Type 1 address was FDRI
					(packet.isType2() && lastAddress == ARCH::eRegisterFDRI)
				||
					// this is a non-empty Type 1 packet and its address is FDRI
					(packet.isType1() && packet.getAddress() == ARCH::eRegisterFDRI 
					&& packet.getWordCount() > 0)
				)) {
				// determine the number of frames in the packet and look up the frame words
				const uint32_t numWords = packet.getWordCount();
				boost::shared_array<uint32_t> frameWords(packet.getWords());
				uint32_t position = 0;
				// iterate over every frame in the packet
				while(position + frameLength <= numWords) {
					// look up the current block type and the frame index into that block type
					typename ARCH::EFarBlockType blockType = frameAddress.mBlockType;
					uint32_t index = frameIndex - frameStart[blockType];
					mFrameBlocks.mBlock[blockType][index] = VirtexFrameSet::FrameSharedPtr
						(new VirtexFrame(frameLength, &frameWords[position]));
					mFrameBlocks.mBlock[blockType][index]->setUsed();
					position += frameLength;
					frameIndex++;
					typename FrameIndexToAddress::iterator ap 
						= inFrameIndexToAddress.find(frameIndex);
					if(ap != inFrameIndexToAddress.end()) frameAddress = ap->second;
					// sanity escape exit (which also breaks out of the higher level loop)
					if(position > numWords) {
						std::cerr << "Overflowed expected frame counts for device." << std::endl;
						break;
					}
				}
				if(frameIndex != frameStart[frameAddress.mBlockType + 1]) {
					// if we ended on a pad frame, where the current index has no corresponding 
					// frame address, we need to advance to the next valid frame address
					if(inFrameIndexToAddress.find(frameIndex) == inFrameIndexToAddress.end()) 
						frameIndex++;
					if(inFrameIndexToAddress.find(frameIndex) == inFrameIndexToAddress.end()) 
						frameIndex++;
					// at this point we should again be on a valid frame
					typename FrameIndexToAddress::iterator ap 
						= inFrameIndexToAddress.find(frameIndex);
					if(ap != inFrameIndexToAddress.end()) frameAddress = ap->second;
					else if(frameIndex == frameStart[frameAddress.mBlockType + 1]) /* at end */;
					else {
						std::cerr << "Failed to find the next valid frame address at the end of a "
							"packet." << std::endl;
					}
				}
			}
			// remember the last register address for Type 2 packets
			if(packet.isType1()) lastAddress = typename ARCH::ERegister(packet.getAddress());
			// process MFWR write packets
			/// \todo
		}
	}

	VirtexPacketVector::iterator VirtexBitstream::deleteFramePackets(void) {
		unimplemented("deleteFramePackets()");
		return end();
	}

	// template instantiations
	template VirtexPacketVector::iterator VirtexBitstream::deleteFramePackets4567<Virtex7>(void);
	template VirtexPacketVector::iterator VirtexBitstream::deleteFramePackets4567<Virtex6>(void);
	template VirtexPacketVector::iterator VirtexBitstream::deleteFramePackets4567<Virtex5>(void);
	template VirtexPacketVector::iterator VirtexBitstream::deleteFramePackets4567<Virtex4>(void);
	// template definition
	template <class ARCH> VirtexPacketVector::iterator VirtexBitstream::deleteFramePackets4567(void)
		{
		// walk the bitstream and look for frame packet boundaries
		// we begin with the first FAR write, and end before the first CRC write after an FDRI write
		iterator b = begin();
		iterator p = b;
		iterator e = end();
		iterator start = e;
		iterator stop = b;
		iterator fdri = b;
		while(p < e) {
			// look up the current packet
		    const VirtexPacket& packet = *p;
			// start from the first FAR write
			if(start == e && packet.isWrite() && packet.getAddress() == ARCH::eRegisterFAR) {
				start = p;
			}
			// remember the last FDRI write (including a trailing Type 2 write if present)
			if(packet.isWrite() && packet.getAddress() == ARCH::eRegisterFDRI) {
				fdri = p;
				if((p+1)->isWrite() && (p+1)->isType2()) fdri++;
			}
			// stop on the first CRC after the last FDRI write
			// (Beware: the zero "address" of a Type 2 packet looks like the CRC register)
		    if(stop < fdri && packet.isWrite() && packet.isType1() 
				&& packet.getAddress() == ARCH::eRegisterCRC) {
				stop = p;
			}
			p++;
		}
		// look for anything unexpected
		p = start;
		while(p < stop) {
			// look up the current packet
		    const VirtexPacket& packet = *p++;
			// abort if we find anything unexpected
			if(start < e && stop > b && !(
				packet.isNop() || (
				packet.isWrite() && (
					packet.getAddress() == ARCH::eRegisterFAR ||
					packet.getAddress() == ARCH::eRegisterLOUT ||
					packet.getAddress() == ARCH::eRegisterFDRI ||
					packet.getAddress() == ARCH::eRegisterCRC ||
					(packet.getAddress() == ARCH::eRegisterCMD && packet[1] == ARCH::eCommandWCFG)
			)))) {
				/// \todo Throw some meaningful exception
				std::cerr << "Unable to safely distinguish frame packets." << std::endl;
				return e;
			}
		}
		// if we found the expected FAR -> FDRI -> CRC structure, delete the frame packets
		if(start < stop && b < stop) {
			erase(start, stop);
			return start;
		}
		// if we didn't find frame packets or delete anything, return the end iterator
		return e;
	}

	VirtexPacketVector VirtexBitstream::generateFullBitstreamPackets(void) {
		unimplemented("generateFullBitstreamPackets()");
		return VirtexPacketVector();
	}

	// template instantiations
	template VirtexPacketVector VirtexBitstream::generateFullBitstreamPackets4567<Virtex7>(
		uint32_t inBlockFrameIndexBounds[]);
	template VirtexPacketVector VirtexBitstream::generateFullBitstreamPackets4567<Virtex6>(
		uint32_t inBlockFrameIndexBounds[]);
	template VirtexPacketVector VirtexBitstream::generateFullBitstreamPackets4567<Virtex5>(
		uint32_t inBlockFrameIndexBounds[]);
	template VirtexPacketVector VirtexBitstream::generateFullBitstreamPackets4567<Virtex4>(
		uint32_t inBlockFrameIndexBounds[]);
	// template definition
	template <class ARCH> VirtexPacketVector VirtexBitstream::generateFullBitstreamPackets4567(
		uint32_t inBlockFrameIndexBounds[]) {

		// declare the packet vector and define a NOP packet
		typedef VirtexFrame::word_t word_t;
		VirtexPacketVector packets;
		VirtexPacket nop(VirtexPacket::makeHeader(ePacketType1, eOpcodeNOP, 0, 0));

		// determine the total size of the frames to write
		size_t size = 0;
		for(int i = 0; i < eBlockTypeCount; i++) 
			size += inBlockFrameIndexBounds[i] * getFrameLength();
		word_t* frameContents = new VirtexFrameSet::word_t[size];
		word_t* pos = frameContents;
		// gather the frames to write
		for(int i = 0; i < eBlockTypeCount; i++) {
			// look up the frame set for this block type
			VirtexFrameSet& frameSet = mFrameBlocks.mBlock[i];
			// iterate through the frames in the set and collect the frame words
			VirtexFrameSet::iterator p = frameSet.begin();
			VirtexFrameSet::iterator e = frameSet.end();
			while(p < e) {
				// look up the current frame
				VirtexFrameSharedPtr framePtr = *p++;
				// set up the pointers
				const word_t* wp = framePtr->getWords();
				const word_t* we = wp + framePtr->getLength();
				// and copy the frame words into the new memory
				if(wp) do { *pos++ = *wp++; } while(wp < we); // frames with words allocated
				else do { *pos++ = 0; wp++; } while(wp < we); // frames with no words allocated
			}
		}
		// write the starting frame address
		packets.push_back(VirtexPacket::makeType1Write(ARCH::eRegisterFAR, 0));
		// write the write configuration register command
		packets.push_back(VirtexPacket::makeType1Write(ARCH::eRegisterCMD, ARCH::eCommandWCFG));
		packets.push_back(nop);
		// write 0 bytes to FDRI (in preparation for type 2 write packet)
		packets.push_back(VirtexPacket::makeNullType1Write(ARCH::eRegisterFDRI));
		// write all frames to FDRI
		packets.push_back(VirtexPacket::makeType2Write(size, frameContents));

		// return the packet vector
		return packets;
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

	// template instantiations
	template VirtexPacketVector VirtexBitstream::generatePartialBitstreamPackets4567<Virtex7>(
		EFrameInclude inFrameInclusion,
		std::map<Virtex7::FrameAddress, uint32_t>& inFrameAddressToIndex, 
		std::map<uint32_t, Virtex7::FrameAddress>& inFrameIndexToAddress);
	template VirtexPacketVector VirtexBitstream::generatePartialBitstreamPackets4567<Virtex6>(
		EFrameInclude inFrameInclusion,
		std::map<Virtex6::FrameAddress, uint32_t>& inFrameAddressToIndex, 
		std::map<uint32_t, Virtex6::FrameAddress>& inFrameIndexToAddress);
	template VirtexPacketVector VirtexBitstream::generatePartialBitstreamPackets4567<Virtex5>(
		EFrameInclude inFrameInclusion,
		std::map<Virtex5::FrameAddress, uint32_t>& inFrameAddressToIndex, 
		std::map<uint32_t, Virtex5::FrameAddress>& inFrameIndexToAddress);
	template VirtexPacketVector VirtexBitstream::generatePartialBitstreamPackets4567<Virtex4>(
		EFrameInclude inFrameInclusion,
		std::map<Virtex4::FrameAddress, uint32_t>& inFrameAddressToIndex, 
		std::map<uint32_t, Virtex4::FrameAddress>& inFrameIndexToAddress);
	// template definition
	template <class ARCH> VirtexPacketVector VirtexBitstream::generatePartialBitstreamPackets4567(
		EFrameInclude inFrameInclusion, 
		std::map<typename ARCH::FrameAddress, uint32_t>& inFrameAddressToIndex, 
		std::map<uint32_t, typename ARCH::FrameAddress>& inFrameIndexToAddress) {

		// declare the packet vector and define a NOP packet
		typedef VirtexFrame::word_t word_t;
		VirtexPacketVector packets;
		VirtexPacket nop(VirtexPacket::makeHeader(ePacketType1, eOpcodeNOP, 0, 0));

		// write the starting frame address
		packets.push_back(VirtexPacket::makeType1Write(ARCH::eRegisterFAR, 0));
		// write the write configuration register command
		packets.push_back(VirtexPacket::makeType1Write(ARCH::eRegisterCMD, ARCH::eCommandWCFG));
		packets.push_back(nop);

		// iterate through the frame blocks looking for groups of contiguous frames that are in use
		bool empty = true;
		uint32_t index = 0;
		uint32_t blockStart = 0;
		for(int i = 0; i < eBlockTypeCount; i++) {
			blockStart += index;
			index = 0;
			bool started = false;
			uint32_t startIndex = 0;
			// look up the frame set for this block type
			VirtexFrameSet& frameSet = mFrameBlocks.mBlock[i];
			// iterate through the frames in the set and add up their lengths
			VirtexFrameSet::iterator p = frameSet.begin();
			VirtexFrameSet::iterator e = frameSet.end();
			while(p < e) {
				// look up the current frame
				VirtexFrameSharedPtr framePtr = *p++;
				// determine whether the frame fits inclusion criteria
					// we include dirty frames, we include clean frames if permitted by the caller, 
					// and if we are collecting frames and we encounter pad frames, we include them,
					// but we stop collecting if we reach the last frame in the set
				/// \todo mFrameIndexToAddress.size() is too short because excludes pad frames
				bool include = p < e 
					&& (
						framePtr->isDirty() 
						|| (inFrameInclusion == eFrameIncludeAllUsedFrames && framePtr->isUsed()) 
						|| (started && blockStart + index < inFrameIndexToAddress.size() && 
							inFrameIndexToAddress.find(blockStart + index) 
								== inFrameIndexToAddress.end())
					);
				// if we are accumulating frames and this frame doesn't meet the criteria, process 
				// the collection of frames and stop collecting
				if((started && !include)) {
					started = false;
					uint32_t stopIndex = index + (p == e ? 1 : 0);
					uint32_t currentIndex = startIndex;
					// std::cerr << "    stopped at: " << stopIndex << std::endl;
					// include two trailing pad frames if appropriate
					if(inFrameIndexToAddress.find(blockStart + stopIndex + 1) 
						== inFrameIndexToAddress.end()) {
						stopIndex++;
						if(inFrameIndexToAddress.find(blockStart + stopIndex + 1) 
							== inFrameIndexToAddress.end())
							stopIndex++;
					}
					// determine the length of the contiguous block
					size_t size = (stopIndex - startIndex) * getFrameLength();
					// allocate a word array
					word_t* frameContents = new VirtexFrameSet::word_t[size];
					word_t* pos = frameContents;
					while(currentIndex < stopIndex) {
						// look up the current frame
						VirtexFrameSharedPtr framePtr = mFrameBlocks.mBlock[i][currentIndex];
						// set up the pointers
						const word_t* wp = framePtr->getWords();
						const word_t* we = wp + framePtr->getLength();
						// and copy the frame words into the new memory
						if(wp) do { *pos++ = *wp++; } while(wp < we); // frames with words
						else do { *pos++ = 0; wp++; } while(wp < we); // frames with no words
						currentIndex++;
					}
					// write the starting frame address
					packets.push_back(VirtexPacket::makeType1Write(ARCH::eRegisterFAR, 
						inFrameIndexToAddress[blockStart + startIndex]));
					packets.push_back(nop);
					// if the size is more than 2048 words, we have to use a Type 2 write
					if(size > 2048) {
						// write 0 bytes to FDRI (in preparation for type 2 write packet)
						packets.push_back(VirtexPacket::makeNullType1Write(ARCH::eRegisterFDRI));
						// write all frames to FDRI
						packets.push_back(VirtexPacket::makeType2Write(size, frameContents));
					} else {
						// write all frames to FDRI
						packets.push_back(VirtexPacket::makeType1Write(ARCH::eRegisterFDRI, size, 
							frameContents));
					}
					if(size) empty = false;
				// if we are not collecting, but we encounter a frame to include, begin collecting
				} else if(!started && include) {
					// std::cerr << "    started at: " << index << std::endl;
					startIndex = index;
					started = true;
				}
				// increment the frame index
				index++;
			}
		}

		// if we generated no frame contents, discard the wrapper packets
		if(empty) packets.clear();
		// return the packet vector
		return packets;
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
