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
/// \brief Header for the Spartan6Packet class.

#ifndef TORC_BITSTREAM_SPARTAN6PACKET_HPP
#define TORC_BITSTREAM_SPARTAN6PACKET_HPP

#include "torc/common/Endian.hpp"
#include <boost/smart_ptr.hpp>
#include <vector>
#include <istream>
#include <iostream>
#include <iomanip>

namespace torc {
namespace bitstream {

namespace bitstream { class Spartan6PacketUnitTest; }

	/// \brief Bitstream packet constants for Spartan 16 bit class architectures.
	class Spartan6PacketConstants {
	public:
	// enumerations
		/// \brief Packet type enumeration.
		/// \see packet type: UG380, v2.2, July 30, 2010, Table 5-24, 5-26 
		enum EPacketType { ePacketType1 = 1, ePacketType2, ePacketTypeCount = 8 };
		//
		/// \brief Packet opcode enumeration.
		/// \see Opcode Format: UG380, v2.2, July 30, 2010, Table 5-23 
		enum EOpcode { eOpcodeNOP = 0, eOpcodeRead, eOpcodeWrite, eOpcodeReserved, eOpcodeCount };
		/// \brief Packet subfields.
		/// \see type 1 packet format: UG380, v2.2, July 30, 2010, Table 5-24 
		/// \see type 2 packet format: UG380, v2.2, July 30, 2010, Table 5-26
		/// \see type 2 packet format: UG380, v2.2, July 30, 2010, Table 5-27
		enum EPacket {
			// generic packet subfields
			ePacketMaskType =			 0xe000,		ePacketShiftType =			  13,
			ePacketMaskOpcode =			 0x1800,		ePacketShiftOpcode =		  11,
			ePacketMaskTypeAddress =	 0x07e0,		ePacketShiftTypeAddress =	   5,
		  // type 1 packet subfields
			ePacketMaskType1Count =		 0x001f,		ePacketShiftType1Count =	   0,
			// type 2 packet subfields
			ePacketMaskType2Count0 =	 0xffff,		ePacketShiftType2Count0 =	   0,
			ePacketMaskType2Count1 =	 0xffff,		ePacketShiftType2Count1 =	   0
		};
		//
		/// \brief Synchronization words.
		/// \see Configuration Sequence: UG380, v2.2, July 30, 2010, Table 5-50.
		enum ESynchronization {
			eSynchronizationDummy =		      0xffff,
			eSynchronizationSync0 =			  0xaa99,
			eSynchronizationSync1 =           0x5566,
			eSynchronizationBusWidthSync0 =	  0x0000,
			eSynchronizationBusWidthSync1 =   0x00bb,
			eSynchronizationBusWidthDetect0 = 0x1122,
			eSynchronizationBusWidthDetect1 = 0x0044
		};
	};

	/// \brief Bitstream packet for Spartan 16 bit class architectures.
	class Spartan6Packet : public Spartan6PacketConstants {
		friend class torc::bitstream::bitstream::Spartan6PacketUnitTest;
	protected:
	// typedefs
		/// \brief Imported type name.
		typedef boost::uint16_t uint16_t;
		typedef boost::uint32_t uint32_t;
		/// \brief Word shared array type.
		typedef boost::shared_array<uint16_t> WordSharedArray;
	// functions
		void initialize(void) {
			mType = EPacketType((mHeader & ePacketMaskType) >> ePacketShiftType);
			mOpcode = EOpcode((mHeader & ePacketMaskOpcode) >> ePacketShiftOpcode);
			mAddress = (mHeader & ePacketMaskTypeAddress) >> ePacketShiftTypeAddress;
		}
	public:
	//protected:
	// members
		uint16_t mHeader;
		uint32_t mCount;
		uint16_t mWord;
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
		Spartan6Packet(void) : mHeader(0), mCount(0), mWord(0), mWords(0), mType(EPacketType(0)), 
			mOpcode(eOpcodeNOP), mAddress(0) {}
		/// \brief Full constructor.
		Spartan6Packet(uint16_t inHeader, uint32_t inCount, uint16_t inWord, 
			uint16_t* inWords) : mHeader(inHeader), mCount(inCount), mWord(inWord),
	 		mWords(WordSharedArray(inWords)), mType(EPacketType(0)), mOpcode(eOpcodeNOP), mAddress(0) {
			initialize();
		}
		/// \brief Header plus single word constructor.
		Spartan6Packet(uint16_t inHeader, uint16_t inWord) : mHeader(inHeader), mCount(1), 
			mWord(inWord), mWords(0), mType(EPacketType(0)), mOpcode(eOpcodeNOP), mAddress(0) {
			initialize();
		}
		/// \brief Header only constructor.
		Spartan6Packet(uint16_t inHeader) : mHeader(inHeader), mCount(0), mWord(0), mWords(0), 
			mType(EPacketType(0)), mOpcode(eOpcodeNOP), mAddress(0) {
			initialize();
		}
		/// \brief Copy constructor.
		Spartan6Packet(const Spartan6Packet& rhs) : mHeader(rhs.mHeader), mCount(rhs.mCount),
	 		mWord(rhs.mWord), mWords(rhs.mWords), mType(EPacketType(0)), mOpcode(eOpcodeNOP), mAddress(0) {
			initialize();
		}
	// functions
		/// \brief Read a packet.
		static Spartan6Packet read(std::istream& inStream) {
			uint32_t count = 0;
			uint16_t count1 = 0; 
			uint16_t count2 = 0; 
			uint16_t header = 0;
			uint16_t word = 0;
			uint16_t* raw_words = 0;
			// read the header
			inStream.read((char*) &header, sizeof(header));
			header = ntohs(header);
			// process synchronization packets
			if(header == eSynchronizationDummy || header == eSynchronizationSync0
				|| header == eSynchronizationSync1 || header == eSynchronizationBusWidthSync0
			  || header == eSynchronizationBusWidthSync1 || header == eSynchronizationBusWidthDetect0
				|| header == eSynchronizationBusWidthDetect1) {
				return Spartan6Packet(header, 0, 0, 0);
			}
			// determine the payload length
			EPacketType type = EPacketType((header & ePacketMaskType) >> ePacketShiftType);
			switch(type) {
			case ePacketType1:
				count = (header & ePacketMaskType1Count) >> ePacketShiftType1Count;
				break;
			case ePacketType2:
				inStream.read((char*) &count2, sizeof(count2));
				inStream.read((char*) &count1, sizeof(count1));
				count2 = ntohs(count2);
				count1 = ntohs(count1);
				count = (count2 << 16) | count1;
				break;
			default:
				/// \todo we should throw an exception on invalid packet types
				count = 0;
				count1 = 0;
				count2 = 0;
				break;
			}
			// read the packet payload
			// Type1 Packet
			if (type == ePacketType1) {
				if(count == 1) {
					inStream.read((char*) &word, sizeof(word));
					word = ntohs(word);
				} else if(count > 1) {
					raw_words = new uint16_t[count];
					inStream.read((char*) raw_words, count << 1);
					uint16_t* wordPtr = raw_words;
					for(uint32_t i = 0; i < count; i++, wordPtr++) *wordPtr = ntohs(*wordPtr);
				}
			}
			// Type2 Packet
			else if (type == ePacketType2) {
				if(count == 1) {
					inStream.read((char*) &word, sizeof(word));
					word = ntohs(word);
				} else if(count > 1) {
					raw_words = new uint16_t[count];
					inStream.read((char*) raw_words, count << 1);
					uint16_t* wordPtr = raw_words;
					for(uint32_t i = 0; i < count; i++, wordPtr++) *wordPtr = ntohs(*wordPtr);
				}
			}

			// create and return the packet
			return Spartan6Packet(header, count, word, raw_words);
		}
		/// \brief Write a packet.
		void write(std::ostream& inStream) const {
			uint32_t size = getWordSize();
			for(uint32_t i = 0; i < size; i++) {
				uint16_t word = htons(operator[](i));
				inStream.write((char*) &word, sizeof(word));
				// type 2 packets need to insert two count words right after the header
				// added count information in check for non-Type 2 headers such as Sync Word 2
				if(i == 0 && isType2() && !isSyncWord1()) {
					word = htons(mCount >> 16);
					inStream.write((char*) &word, sizeof(word));
					word = htons(mCount & 0x0000ffff);
					inStream.write((char*) &word, sizeof(word));
					// if this was a Type 2, we've just accounted for the two extra length words, 
					// so don't try to extract them form the packet payload
					size -= 2;
				}
			}
		}
		/// \brief Construct a null type 1 write packet.
		/// \details A type 2 write packet to the FDRI register is generally only useful to specify 
		///		the current register in preparation for a type 2 write.  The type 2 write header 
		///		includes a size but specifies no target register, hence the prior null type 2 
		///		packet.
		static Spartan6Packet makeNullType2Write(uint16_t inAddress) {
			return Spartan6Packet(makeHeader(ePacketType2, eOpcodeWrite, inAddress, 0), 0, 0, 0);
		}
		/// \brief Construct a type 1 write packet.
		static Spartan6Packet makeType1Write(uint16_t inAddress, uint16_t inWord) {
			return Spartan6Packet(makeHeader(ePacketType1, eOpcodeWrite, inAddress, 1), 1, inWord, 0);
		}
		/// \brief Construct a type 1 write packet for two words.
		static Spartan6Packet makeType1Write32(uint16_t inAddress, uint32_t inWord) {
			// used to send two packets after header for packets like IDCODE, RDBK_SIGN, FARMAJ, and CRC
			uint16_t* words = new uint16_t[2];
			words[0] = inWord >> 16;
			words[1] = inWord & 0x0000ffff;
			return Spartan6Packet(makeHeader(ePacketType1, eOpcodeWrite, inAddress, 2), 2, 0, words);
		}
		/// \brief Construct a type 1 write packet for two words.
		static Spartan6Packet makeType1Write(uint16_t inAddress, uint16_t inWord1, uint16_t inWord2) {
			// used to send two packets after header for packets like IDCODE, RDBK_SIGN, FARMAJ, and CRC
			uint16_t* words = new uint16_t[2];
			words[0] = inWord1;
			words[1] = inWord2;
			return Spartan6Packet(makeHeader(ePacketType1, eOpcodeWrite, inAddress, 2), 2, 0, words);
		}
		/// \brief Construct a type 1 write packet for multiple words.
		static Spartan6Packet makeType1Write(uint32_t inCount, uint16_t inAddress, uint16_t* inWords) {
			return Spartan6Packet(makeHeader(ePacketType1, eOpcodeWrite, inAddress, inCount), inCount, 0, inWords);
		}
		/// \brief Construct a type 2 write packet.
		static Spartan6Packet makeType2Write(uint32_t inCount, uint16_t inAddress, uint16_t* inWords) {
			return Spartan6Packet(makeHeader(ePacketType2, eOpcodeWrite, inAddress, 0), inCount, 0, inWords);
		}
//		/// \brief Construct a type 1 write packet header.
//		static uint16_t makeType1WriteHeader(uint16_t inAddress, uint32_t inCount) {
//			return makeHeader(ePacketType1, eOpcodeWrite, inAddress, inCount);
//		}
//		/// \brief Construct a type 2 write packet header.
//		static uint16_t makeType2WriteHeader(uint32_t inCount) {
//			return makeHeader(ePacketType2, eOpcodeWrite, 0, inCount);
//		}
		/// \brief Construct a packet header.
		static uint16_t makeHeader(EPacketType inType, EOpcode inOpcode, uint16_t inAddress, 
			uint32_t inCount) {
			// type 1 packets
			if(inType == ePacketType1) return 
				((inType << ePacketShiftType) & ePacketMaskType) |
				((inOpcode << ePacketShiftOpcode) & ePacketMaskOpcode) |
				((inAddress << ePacketShiftTypeAddress) & ePacketMaskTypeAddress) |
				((inCount << ePacketShiftType1Count) & ePacketMaskType1Count);
			// type 2 packets
			else if(inType == ePacketType2) return
				((inType << ePacketShiftType) & ePacketMaskType) |
				((inOpcode << ePacketShiftOpcode) & ePacketMaskOpcode) |
				((inAddress << ePacketShiftTypeAddress) & ePacketMaskTypeAddress);
			// undefined packet types
			else
				/// \todo Generate an error or throw an exception on invalid packet types?
				return 0;
		}
	// operators
		uint16_t operator[] (size_t inIndex) const {
			if(inIndex == 0) return mHeader;
			if(inIndex == 1 && mCount == 1) return mWord;
			if(inIndex <= mCount) return mWords[inIndex-1];
			return 0;
		}
	// accessors
		EPacketType getType(void) const { return mType; }
		EOpcode getOpcode(void) const { return mOpcode; }
		int getAddress(void) const { return mAddress; }
		uint16_t getHeader(void) const { return mHeader; }
		/// \brief Returns the number of payload words in the packet, excluding the header word.
		uint32_t getWordCount(void) const { return mCount; }
		/// \brief Returns the total number of words in the packet, including the header word.
		/// \detail In the case of Type 2 packets, we also include the two implicity length words.
		uint32_t getWordSize(void) const { 
			if(isType2() && !isSyncWord1()) return mCount + 1 + 2;
			return mCount + 1;
		}
		/// brief Returns the raw packet words, including the header word.
		const WordSharedArray getWords(void) const { return mWords; }
	  // tests
		bool isType1(void) const { return mType == ePacketType1; }
		bool isType2(void) const { return mType == ePacketType2; }
		bool isNop(void) const { return mOpcode == eOpcodeNOP; }
		bool isReserved(void) const { return mOpcode == eOpcodeReserved; }
		bool isRead(void) const { return mOpcode == eOpcodeRead; }
		bool isWrite(void) const { return mOpcode == eOpcodeWrite; }
		bool isDummyWord(void) const { return mHeader == eSynchronizationDummy; }
		bool isSyncWord0(void) const { return mHeader == eSynchronizationSync0; }
		bool isSyncWord1(void) const { return mHeader == eSynchronizationSync1; }
		bool isBusWidthSyncWord0(void) const { return mHeader == eSynchronizationBusWidthSync0; }
		bool isBusWidthSyncWord1(void) const { return mHeader == eSynchronizationBusWidthSync1; }
		bool isBusWidthDetectWord0(void) const { return mHeader == eSynchronizationBusWidthDetect0; }
		bool isBusWidthDetectWord1(void) const { return mHeader == eSynchronizationBusWidthDetect1; }
	};

	/// \brief Vector of Spartan 16 bit packets.
	typedef std::vector<Spartan6Packet> Spartan6PacketVector;

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_SPARTAN6PACKET_HPP
