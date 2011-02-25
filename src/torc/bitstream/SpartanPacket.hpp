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
/// \brief Header for the SpartanPacket class.

#ifndef TORC_BITSTREAM_SPARTAN_PACKET_HPP
#define TORC_BITSTREAM_SPARTAN_PACKET_HPP

#include "torc/common/Endian.hpp"
#include <boost/smart_ptr.hpp>
#include <vector>
#include <istream>

#include <iostream>

namespace torc {
namespace bitstream {

namespace bitstream { class bitstream_spartan_packet; }

	/// \brief Bitstream packet constants for Spartan class architectures.
	class SpartanPacketConstants {
	public:
	// enumerations
		/// \brief Packet type enumeration.
		/// \see packet type: XAPP452, v1.1, June 25, 2008, Figure 6, 7.
		enum EPacketType { ePacketType1 = 1, ePacketType2, ePacketTypeCount = 8 };
		//
		/// \brief Packet opcode enumeration.
		enum EOpcode { eOpcodeNOP = 0, eOpcodeRead, eOpcodeWrite, eOpcodeReserved, eOpcodeCount };
		/// \brief Packet subfields.
		/// \see type 1 packet format: XAPP452, v1.1, June 25, 2008, Figure 6.
		/// \see type 2 packet format: XAPP452, v1.1, June 25, 2008, Figure 7.
		enum EPacket {
			// generic packet subfields
			ePacketMaskType =			     0xe0000000,		ePacketShiftType =			    29,
			ePacketMaskOpcode =			   0x18000000,		ePacketShiftOpcode =		    27,
			// type 1 packet subfields
			ePacketMaskType1Address =	 0x07ffe000,		ePacketShiftType1Address =	13,
			ePacketMaskType1Reserved = 0x00001800,		ePacketShiftType1Reserved =	11,
			ePacketMaskType1Count =		 0x000007ff,		ePacketShiftType1Count =	   0,
			// type 2 packet subfields
			ePacketMaskType2Count =		 0x07ffffff,		ePacketShiftType2Count =	   0
		};
		//
		/// \brief Synchronization words.
		/// \see Configuration Sequence: XAPP452, v1.1, June 25, 2008, Table 9.
		enum ESynchronization {
			eSynchronizationDummy =				    0xffffffff,
			eSynchronizationSync =				    0xaa995566,
			eSynchronizationBusWidthSync =		0x000000bb,
			eSynchronizationBusWidthDetect =	0x11220044
		};
	};

	/// \brief Bitstream packet for Spartan class architectures.
	class SpartanPacket : public SpartanPacketConstants {
		friend class torc::bitstream::bitstream::bitstream_spartan_packet;
	protected:
	// typedefs
		/// \brief Imported type name.
		typedef boost::uint32_t uint32_t;
		/// \brief Word shared array type.
		typedef boost::shared_array<uint32_t> WordSharedArray;
	// functions
		void initialize(void) {
			mType = EPacketType((mHeader & ePacketMaskType) >> ePacketShiftType);
			mOpcode = EOpcode((mHeader & ePacketMaskOpcode) >> ePacketShiftOpcode);
			if(mType == ePacketType1) 
				mAddress = (mHeader & ePacketMaskType1Address) >> ePacketShiftType1Address;
		}
	public:
	//protected:
	// members
		uint32_t mHeader;
		uint32_t mCount;
		uint32_t mWord;
		WordSharedArray mWords;
		EPacketType mType;
		EOpcode mOpcode;
		int mAddress;
		/// \brief Packet type names.
		static const char* sPacketTypeName[ePacketTypeCount];
		/// \brief Packet opcode names.
		static const char* sOpcodeName[eOpcodeCount];
	public:
	// constructors
		/// \brief Null constructor.
		SpartanPacket(void) : mHeader(0), mCount(0), mWord(0), mWords(0), mType(EPacketType(0)), 
			mOpcode(eOpcodeNOP), mAddress(0) {}
		/// \brief Full constructor.
		SpartanPacket(uint32_t inHeader, uint32_t inCount, uint32_t inWord, uint32_t* inWords) 
			: mHeader(inHeader), mCount(inCount), mWord(inWord), mWords(WordSharedArray(inWords)), 
			mType(EPacketType(0)), mOpcode(eOpcodeNOP), mAddress(0) {
			initialize();
		}
		/// \brief Header plus single word constructor.
		SpartanPacket(uint32_t inHeader, uint32_t inWord) : mHeader(inHeader), mCount(1), 
			mWord(inWord), mWords(0), mType(EPacketType(0)), mOpcode(eOpcodeNOP), mAddress(0) {
			initialize();
		}
		/// \brief Header only constructor.
		SpartanPacket(uint32_t inHeader) : mHeader(inHeader), mCount(0), mWord(0), mWords(0), 
			mType(EPacketType(0)), mOpcode(eOpcodeNOP), mAddress(0) {
			initialize();
		}
		/// \brief Copy constructor.
		SpartanPacket(const SpartanPacket& rhs) : mHeader(rhs.mHeader), mCount(rhs.mCount), 
			mWord(rhs.mWord), mWords(rhs.mWords), mType(EPacketType(0)), mOpcode(eOpcodeNOP), 
			mAddress(0) {
			initialize();
		}
	// functions
		/// \brief Read a packet.
		static SpartanPacket read(std::istream& inStream) {
			uint32_t count = 0;
			uint32_t header = 0;
			uint32_t word = 0;
			uint32_t* raw_words = 0;
			bool debug = true;
			// read the header
			inStream.read((char*) &header, sizeof(header));
			header = ntohl(header);
			// process synchronization packets
			if(header == eSynchronizationDummy || header == eSynchronizationSync
				|| header == eSynchronizationBusWidthSync 
				|| header == eSynchronizationBusWidthDetect) {
				//if (debug) printf("Header in sync: %x\n", header) ;
				return SpartanPacket(header, 0, 0, 0);
			}
			// determine the payload length
			EPacketType type = EPacketType((header & ePacketMaskType) >> ePacketShiftType);
			switch(type) {
			case ePacketType1:
				count = (header & ePacketMaskType1Count) >> ePacketShiftType1Count;
				//if (debug) printf("Type 1: Packet Header: %x\n", header);
				//if (debug) std::cout << "Type 1: Packet Count: " << count << std::endl;
				break;
			case ePacketType2:
				count = (header & ePacketMaskType2Count) >> ePacketShiftType2Count;
				//if (debug) std::cout << "Type 2: Packet Count: " << count << std::endl;
				break;
			default:
				/// \todo we should throw an exception on invalid packet types
				count = 0;
				break;
			}
			// read the packet payload
			if(count == 1) {
				inStream.read((char*) &word, sizeof(word));
				word = ntohl(word);
			} else if(count > 1) {
				raw_words = new uint32_t[count];
				inStream.read((char*) raw_words, count << 2);
				if (debug) printf("Packet Count: %x\n", count);
				uint32_t* wordPtr = raw_words;
				for(uint32_t i = 0; i < count; i++, wordPtr++) *wordPtr = ntohl(*wordPtr);
			}
			// create and return the packet
			return SpartanPacket(header, count, word, raw_words);
		}
		/// \brief Write a packet.
		void write(std::ostream& inStream) const {
			uint32_t size = getWordSize();
			for(uint32_t i = 0; i < size; i++) {
				uint32_t word = htonl(operator[](i));
				inStream.write((char*) &word, sizeof(word));
			}
		}
		/// \brief Construct a null type 1 write packet.
		/// \details A type 1 write packet to the FDRI register is generally only useful to specify 
		///		the current register in preparation for a type 2 write.  The type 2 write header 
		///		includes a size but specifies no target register, hence the prior null type 1 
		///		packet.
		static SpartanPacket makeNullType1Write(uint32_t inAddress) {
			return SpartanPacket(makeHeader(ePacketType1, eOpcodeWrite, inAddress, 0), 0, 0, 0);
		}
		/// \brief Construct a type 1 write packet.
		static SpartanPacket makeType1Write(uint32_t inAddress, uint32_t inWord) {
			return SpartanPacket(makeHeader(ePacketType1, eOpcodeWrite, inAddress, 1), 1, inWord, 0);
		}
		/// \brief Construct a type 2 write packet.
		static SpartanPacket makeType2Write(uint32_t inCount, uint32_t* inWords) {
			return SpartanPacket(makeHeader(ePacketType2, eOpcodeWrite, 0, inCount), inCount, 0, 
				inWords);
		}
//		/// \brief Construct a type 1 write packet header.
//		static uint32_t makeType1WriteHeader(uint32_t inAddress, uint32_t inCount) {
//			return makeHeader(ePacketType1, eOpcodeWrite, inAddress, inCount);
//		}
//		/// \brief Construct a type 2 write packet header.
//		static uint32_t makeType2WriteHeader(uint32_t inCount) {
//			return makeHeader(ePacketType2, eOpcodeWrite, 0, inCount);
//		}
		/// \brief Construct a packet header.
		static uint32_t makeHeader(EPacketType inType, EOpcode inOpcode, uint32_t inAddress, 
			uint32_t inCount) {
			// type 1 packets
			if(inType == ePacketType1) return 
				((inType << ePacketShiftType) & ePacketMaskType) |
				((inOpcode << ePacketShiftOpcode) & ePacketMaskOpcode) |
				((inAddress << ePacketShiftType1Address) & ePacketMaskType1Address) |
				((inCount << ePacketShiftType1Count) & ePacketMaskType1Count);
			// type 2 packets
			else if(inType == ePacketType2) return
				((inType << ePacketShiftType) & ePacketMaskType) |
				((inOpcode << ePacketShiftOpcode) & ePacketMaskOpcode) |
				((inCount << ePacketShiftType2Count) & ePacketMaskType2Count);
			// undefined packet types
			else
				/// \todo Generate an error or throw an exception on invalid packet types?
				return 0;
		}
	// operators
		uint32_t operator[] (size_t inIndex) const {
			if(inIndex == 0) return mHeader;
			if(inIndex == 1 && mCount == 1) return mWord;
			if(inIndex <= mCount) return mWords[inIndex-1];
			return 0;
		}
	// accessors
		EPacketType getType(void) const { return mType; }
		EOpcode getOpcode(void) const { return mOpcode; }
		int getAddress(void) const { return mAddress; }
		uint32_t getHeader(void) const { return mHeader; }
		/// \brief Returns the number of payload words in the packet, excluding the header word.
		uint32_t getWordCount(void) const { return mCount; }
		/// \brief Returns the total number of words in the packet, including the header word.
		uint32_t getWordSize(void) const { return mCount + 1; }
//		const uint32_t* getWords(void) const { return mWords; }
	// tests
		bool isType1(void) const { return mType == ePacketType1; }
		bool isType2(void) const { return mType == ePacketType2; }
		bool isNop(void) const { return mOpcode == eOpcodeNOP; }
		bool isReserved(void) const { return mOpcode == eOpcodeReserved; }
		bool isRead(void) const { return mOpcode == eOpcodeRead; }
		bool isWrite(void) const { return mOpcode == eOpcodeWrite; }
		bool isDummyWord(void) const { return mHeader == eSynchronizationDummy; }
		bool isSyncWord(void) const { return mHeader == eSynchronizationSync; }
		bool isBusWidthSyncWord(void) const { return mHeader == eSynchronizationBusWidthSync; }
		bool isBusWidthDetectWord(void) const { return mHeader == eSynchronizationBusWidthDetect; }
	};

	/// \brief Vector of Spartan packets.
	typedef std::vector<SpartanPacket> SpartanPacketVector;

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_SPARTAN_PACKET_HPP
