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
/// \brief Source for torc::bitstream output stream helpers.

#include "torc/bitstream/OutputStreamHelpers.hpp"
#include "torc/bitstream/Bitstream.hpp"
#include "torc/bitstream/Virtex.hpp"
#include "torc/bitstream/Virtex2.hpp"
#include "torc/bitstream/Virtex4.hpp"
#include "torc/bitstream/Virtex5.hpp"
#include "torc/bitstream/Virtex6.hpp"
#include "torc/bitstream/Virtex7.hpp"
#include "torc/bitstream/VirtexPacket.hpp"
#include "torc/bitstream/Spartan3E.hpp"
#include "torc/bitstream/Spartan6.hpp"
#include "torc/bitstream/SpartanPacket.hpp"
#include "torc/bitstream/Spartan6Packet.hpp"
#include "torc/bitstream/Spartan6Bitstream.hpp"
#include <iostream>
#include <iomanip>

namespace torc {
namespace bitstream {

	std::ostream& operator <<(std::ostream& os, const Hex16& inWord) {
		return os << std::hex << std::setfill('0') << std::setw(8) << inWord.mWord << std::dec;
	}

	std::ostream& operator <<(std::ostream& os, const Hex32& inWord) {
		return os << std::hex << std::setfill('0') << std::setw(8) << inWord.mWord << std::dec;
	}

	std::ostream& operator <<(std::ostream& os, const Bitstream& rhs) {
		return os << "Design " << rhs.mDesignName << " (" << rhs.mDeviceName << ") @ " 
			<< rhs.mDesignDate << " " << rhs.mDesignTime << ": " << rhs.mBitstreamByteLength 
			<< " bytes (" << (rhs.mBitstreamByteLength >> 2) << " words)";
	}

	std::ostream& operator <<(std::ostream& os, const Spartan6Bitstream& rhs) {
		return os << "Design " << rhs.mDesignName << " (" << rhs.mDeviceName << ") @ " 
			<< rhs.mDesignDate << " " << rhs.mDesignTime << ": " << rhs.mBitstreamByteLength 
			<< " bytes (" << (rhs.mBitstreamByteLength >> 1) << " words)";
	}

	// Spartan3E Output Stream
	std::ostream& operator <<(std::ostream& os, const Spartan3E& rhs) {
		// insert the bitstream header
		os << static_cast<const Bitstream>(rhs) << std::endl;
		uint32_t cumulativeWordLength = 0;
		bool newColumn = false;
		bool newBlock = false;
		uint32_t oldColumnValue = 0;
		uint32_t oldBlockValue = 0;
		uint32_t currentColumnValue = 0;
		uint32_t currentBlockValue = 0;

		// iterate over the packets
		Spartan3E::ERegister address = Spartan3E::ERegister();
		//SpartanPacketVector::const_iterator p = rhs.begin();
		std::vector<SpartanPacket>::const_iterator p = rhs.begin();
		//SpartanPacketVector::const_iterator e = rhs.end();
		std::vector<SpartanPacket>::const_iterator e = rhs.end();
		while(p < e) {

			// insert the byte address
			os << "    " << Hex32(rhs.getHeaderByteLength() + (cumulativeWordLength << 2)) << ": ";
			// look up the packet
			const SpartanPacket& packet = *p++;
			cumulativeWordLength += packet.getWordSize();

			// handle dummy words
			if(packet.isDummyWord()) {
				os << "DUMMY" << std::endl;

			// handle sync words
			} else if(packet.isSyncWord()) {
				os << "SYNC" << std::endl;

			// handle reserved packets
			} else if(packet.isReserved()) {
				os << Spartan3E::sOpcodeName[packet.getOpcode()] << std::endl;
			
			// handle NOP packets
			} else if(packet.isNop()) {
				int nops = 1;
				while(p < e && p->isNop()) { nops++; p++; }
				cumulativeWordLength += nops - 1;
				os << "NOP x " << nops << std::endl;

			// handle regular type 1 or type 2 packets
			} else {

				// look up the packet details
				Spartan3E::EPacketType type = packet.getType();
				Spartan3E::EOpcode opcode = packet.getOpcode();
				uint32_t wordCount = packet.getWordCount();
				const uint32_t word = packet[1];

				// account for the packet type
				os << Spartan3E::sPacketTypeName[type];
				switch(type) {
				case Spartan3E::ePacketType1: 
					address = Spartan3E::ERegister(packet.getAddress());
					break;
				case Spartan3E::ePacketType2: 
					break;
				default: 
					os << std::endl;
					continue;
				}

				// handle read packets
				if(opcode == packet.isRead()) {
					os << " READ " << Spartan3E::sRegisterName[address];

				// handle write packets
				} else if(opcode == Spartan3E::eOpcodeWrite) {
					os << " WRITE " << Spartan3E::sRegisterName[address];
					// process according to register address
					switch(address) {
					case Spartan3E::eRegisterFDRI:
						os << ": " << Hex32(wordCount) << " words";
						break;
					case Spartan3E::eRegisterCMD: 
						os << " " << Spartan3E::sCommandName[word];
						break;
					case Spartan3E::eRegisterCOR:
						os << ": " << Hex32(word);
						Spartan3E::writeSubfieldSettings(os, uint32_t(word), Spartan3E::sCOR);
						break;
					case Spartan3E::eRegisterSTAT:
						os << ": " << Hex32(word);
						Spartan3E::writeSubfieldSettings(os, uint32_t(word), Spartan3E::sSTAT);
						break;
					case Spartan3E::eRegisterCTL:
						os << ": " << Hex32(word);
						Spartan3E::writeSubfieldSettings(os, uint32_t(word), Spartan3E::sCTL);
						break;
					case Spartan3E::eRegisterMASK:
						os << ": " << Hex32(word);
						Spartan3E::writeSubfieldSettings(os, uint32_t(word), Spartan3E::sMASK);
						break;
					// Added to make frame mapping debug easier
					case Spartan3E::eRegisterLOUT:
						oldColumnValue = currentColumnValue;
						oldBlockValue = currentBlockValue;
						currentColumnValue = (word & Virtex::eFarMaskMajor);
						currentBlockValue = (word & Virtex::eFarMaskBlockType);
						newColumn = (currentColumnValue != oldColumnValue);
						newBlock = (currentBlockValue != oldBlockValue);
						os << ": " << Hex32(word);
						if(newColumn) std::cout << "\t\t\t!!!New Column!!!";
						if(newBlock) std::cout << "\t\t\t\t\t***New Block***" << Hex32(currentBlockValue);
						break;
					default:
						// output the register contents
						os << ": " << Hex32(word);
						break;
					}
					os << std::endl;
				}

			}

		}

		// return the stream reference
		return os;
	}


	// Spartan6 Output Stream
	std::ostream& operator <<(std::ostream& os, const Spartan6& rhs) {
		// insert the bitstream header
		// be careful to cast this as a Spartan6Bitstream, rather than an ordinary Bitstream
		os << static_cast<const Spartan6Bitstream>(rhs) << std::endl;
		uint32_t cumulativeWordLength = 0;

		// iterate over the packets
		Spartan6::ERegister address = Spartan6::ERegister();
		//SpartanPacketVector::const_iterator p = rhs.begin();
		std::vector<Spartan6Packet>::const_iterator p = rhs.begin();
		//SpartanPacketVector::const_iterator e = rhs.end();
		std::vector<Spartan6Packet>::const_iterator e = rhs.end();
		while(p < e) {

			// insert the byte address
			os << "    " << Hex32(rhs.getHeaderByteLength() + (cumulativeWordLength << 1)) << ": ";
			// look up the packet
			const Spartan6Packet& packet = *p++;
			cumulativeWordLength += packet.getWordSize();

			// handle dummy words
			if(packet.isDummyWord()) {
				os << "DUMMY" << std::endl;

			// handle sync words
			} else if(packet.isSyncWord0()) {
				os << "SYNC0" << std::endl;

				// handle sync words
			} else if(packet.isSyncWord1()) {
				os << "SYNC1" << std::endl;

			// handle reserved packets
			} else if(packet.isReserved()) {
				os << Spartan6::sOpcodeName[packet.getOpcode()] << std::endl;
			
			// handle NOP packets
			} else if(packet.isNop()) {
				int nops = 1;
				while(p < e && p->isNop()) { nops++; p++; }
				cumulativeWordLength += nops - 1;
				os << "NOP x " << nops << std::endl;

			// handle regular type 1 or type 2 packets
			} else {

				// look up the packet details
				Spartan6::EPacketType type = packet.getType();
				Spartan6::EOpcode opcode = packet.getOpcode();
				uint32_t wordCount = packet.getWordCount();
				const uint16_t word = packet[1];
				const uint32_t word32 = (packet[1] << 16) | packet[2];

				// account for the packet type
				os << Spartan6::sPacketTypeName[type];
				switch(type) {
				case Spartan6::ePacketType1: 
				case Spartan6::ePacketType2: 
					address = Spartan6::ERegister(packet.getAddress());
					break;
				default: 
					os << std::endl;
					continue;
				}

				// handle read packets
				if(opcode == packet.isRead()) {
					os << " READ " << Spartan6::sRegisterName[address];

				// handle write packets
				} else if(opcode == Spartan6::eOpcodeWrite) {
					os << " WRITE " << Spartan6::sRegisterName[address];
					// process according to register address
					switch(address) {
					case Spartan6::eRegisterCMD: 
						os << " " << Spartan6::sCommandName[word];
						break;
					case Spartan6::eRegisterCOR1:
						os << ": " << Hex16(word);
						Spartan6::writeSubfieldSettings(os, uint16_t(word), Spartan6::sCOR1);
						break;
					case Spartan6::eRegisterCOR2:
						os << ": " << Hex16(word);
						Spartan6::writeSubfieldSettings(os, uint16_t(word), Spartan6::sCOR2);
						break;
					case Spartan6::eRegisterSTAT:
						os << ": " << Hex16(word);
						Spartan6::writeSubfieldSettings(os, uint16_t(word), Spartan6::sSTAT);
						break;
					case Spartan6::eRegisterCTL:
						os << ": " << Hex16(word);
						Spartan6::writeSubfieldSettings(os, uint16_t(word), Spartan6::sCTL);
						break;
					case Spartan6::eRegisterMASK:
						os << ": " << Hex16(word);
						Spartan6::writeSubfieldSettings(os, uint16_t(word), Spartan6::sMASK);
						break;
					case Spartan6::eRegisterPWRDN_REG:
						os << ": " << Hex16(word);
						Spartan6::writeSubfieldSettings(os, uint16_t(word), Spartan6::sPWRDN_REG);
						break;
					case Spartan6::eRegisterHC_OPT_REG:
						os << ": " << Hex16(word);
						Spartan6::writeSubfieldSettings(os, uint16_t(word), Spartan6::sHC_OPT_REG);
						break;
					case Spartan6::eRegisterMODE_REG:
						os << ": " << Hex16(word);
						Spartan6::writeSubfieldSettings(os, uint16_t(word), Spartan6::sMODE_REG);
						break;
					case Spartan6::eRegisterBOOSTS:
						os << ": " << Hex16(word);
						Spartan6::writeSubfieldSettings(os, uint16_t(word), Spartan6::sBOOSTS);
						break;
					case Spartan6::eRegisterSEU_OPT:
						os << ": " << Hex16(word);
						Spartan6::writeSubfieldSettings(os, uint16_t(word), Spartan6::sSEU_OPT);
						break;
					case Spartan6::eRegisterRDBK_SIGN:
						os << ": " << Hex32(word32);
						break;
					case Spartan6::eRegisterIDCODE:
						os << ": " << Hex32(word32);
						break;
					case Spartan6::eRegisterCRC:
						os << ": " << Hex32(word32);
						break;
					case Spartan6::eRegisterFDRI:
					//    if(packet.hasAutoCRC())
					//	{
					//	  os << ": " << Hex32(wordCount) << " words\n";
					//	  os << "\t\t\t\tAUTOCRC: " << Hex32(packet.getAutoCRC());
					//	}
					//	else
						  os << ": " << Hex32(wordCount) << " words\n";
						break;
					case Spartan6::eRegisterFARMAJ:
						// Only updates the FAR Major
						if (wordCount == 1)
  						  os << "Major Address: " << Hex16(packet[1]);
						// Updates both the FAR Major and Minor address
						// Word 1: FAR Major
						// Word 2: FAR Minor
						else if (wordCount == 2) {
						  std::cout << std::endl;
						  os << "\t\t\t\tMajor Address: " << Hex16(packet[1]);
						  std::cout << std::endl;
						  os << "\t\t\t\tMinor Address: " << Hex16(packet[2] >> 1);
						}
						break;
					default:
						// output the register contents
						os << ": " << Hex16(word);
						break;
					}
					os << std::endl;
				}

			}

		}

		// return the stream reference
		return os;
	}


	// Virtex Output Stream
	std::ostream& operator <<(std::ostream& os, const Virtex& rhs) {
		// insert the bitstream header
		os << static_cast<const Bitstream>(rhs) << std::endl;
		uint32_t cumulativeWordLength = 0;
		bool newColumn = false;
		bool newBlock = false;
		uint32_t oldColumnValue = 0;
		uint32_t oldBlockValue = 0;
		uint32_t currentColumnValue = 0;
		uint32_t currentBlockValue = 0;

		// iterate over the packets
		Virtex::ERegister address = Virtex::ERegister();
		VirtexPacketVector::const_iterator p = rhs.begin();
		VirtexPacketVector::const_iterator e = rhs.end();
		while(p < e) {

			// insert the byte address
			os << "    " << Hex32(rhs.getHeaderByteLength() + (cumulativeWordLength << 2)) << ": ";
			// look up the packet
			const VirtexPacket& packet = *p++;
			cumulativeWordLength += packet.getWordSize();

			// handle dummy words
			if(packet.isDummyWord()) {
				os << "DUMMY" << std::endl;

			// handle sync words
			} else if(packet.isSyncWord()) {
				os << "SYNC" << std::endl;

			// handle reserved packets
			} else if(packet.isReserved()) {
				os << Virtex::sOpcodeName[packet.getOpcode()] << std::endl;
			
			/* Not used in Virtex
			// handle NOP packets
			} else if(packet.isNop()) {
				int nops = 1;
				while(p < e && p->isNop()) { nops++; p++; }
				cumulativeWordLength += nops - 1;
				os << "NOP x " << nops << std::endl;
			*/

			// handle regular type 1 or type 2 packets
			} else {

				// look up the packet details
				Virtex::EPacketType type = packet.getType();
				Virtex::EOpcode opcode = packet.getOpcode();
				uint32_t wordCount = packet.getWordCount();
				const uint32_t word = packet[1];

				// account for the packet type
				os << Virtex::sPacketTypeName[type];
				switch(type) {
				case Virtex::ePacketType1: 
					address = Virtex::ERegister(packet.getAddress());
					break;
				case Virtex::ePacketType2: 
					break;
				default: 
					os << std::endl;
					continue;
				}

				// handle read packets
				if(opcode == packet.isRead()) {
					os << " READ " << Virtex::sRegisterName[address];

				// handle write packets
				} else if(opcode == Virtex::eOpcodeWrite) {
					os << " WRITE " << Virtex::sRegisterName[address];
					// process according to register address
					switch(address) {
					case Virtex::eRegisterFDRI:
						os << ": " << Hex32(wordCount) << " words";
						break;
					case Virtex::eRegisterCMD: 
						os << " " << Virtex::sCommandName[word];
						break;
					case Virtex::eRegisterCOR:
						os << ": " << Hex32(word);
						Virtex::writeSubfieldSettings(os, uint32_t(word), Virtex::sCOR);
						break;
					case Virtex::eRegisterSTAT:
						os << ": " << Hex32(word);
						Virtex::writeSubfieldSettings(os, uint32_t(word), Virtex::sSTAT);
						break;
					case Virtex::eRegisterCTL:
						os << ": " << Hex32(word);
						Virtex::writeSubfieldSettings(os, uint32_t(word), Virtex::sCTL);
						break;
					case Virtex::eRegisterMASK:
						os << ": " << Hex32(word);
						Virtex::writeSubfieldSettings(os, uint32_t(word), Virtex::sMASK);
						break;
					// Added to make frame mapping debug easier
					case Virtex::eRegisterLOUT:
						oldColumnValue = currentColumnValue;
						oldBlockValue = currentBlockValue;
						currentColumnValue = (word & Virtex::eFarMaskMajor);
						currentBlockValue = (word & Virtex::eFarMaskBlockType);
						newColumn = (currentColumnValue != oldColumnValue);
						newBlock = (currentBlockValue != oldBlockValue);
						os << ": " << Hex32(word);
						if(newColumn) std::cout << "\t\t\t!!!New Column!!!";
						if(newBlock) std::cout << "\t\t\t\t\t***New Block***" << Hex32(currentBlockValue);
						break;
					default:
						// output the register contents
						os << ": " << Hex32(word);
						break;
					}
					os << std::endl;
				}

			}

		}

		// return the stream reference
		return os;
	}


	// Virtex2 Output Stream
	std::ostream& operator <<(std::ostream& os, const Virtex2& rhs) {
		// insert the bitstream header
		os << static_cast<const Bitstream>(rhs) << std::endl;
		uint32_t cumulativeWordLength = 0;

		// iterate over the packets
		Virtex2::ERegister address = Virtex2::ERegister();
		VirtexPacketVector::const_iterator p = rhs.begin();
		VirtexPacketVector::const_iterator e = rhs.end();
		bool newColumn = false;
		bool newBlock = false;
		uint32_t oldColumnValue = 0;
		uint32_t oldBlockValue = 0;
		uint32_t currentColumnValue = 0;
		uint32_t currentBlockValue = 0;
		while(p < e) {

			// insert the byte address
			os << "    " << Hex32(rhs.getHeaderByteLength() + (cumulativeWordLength << 2)) << ": ";
			// look up the packet
			const VirtexPacket& packet = *p++;
			cumulativeWordLength += packet.getWordSize();

			// handle dummy words
			if(packet.isDummyWord()) {
				os << "DUMMY" << std::endl;

			// handle sync words
			} else if(packet.isSyncWord()) {
				os << "SYNC" << std::endl;

			// handle reserved packets
			} else if(packet.isReserved()) {
				os << Virtex2::sOpcodeName[packet.getOpcode()] << std::endl;

			// handle NOP packets
			} else if(packet.isNop()) {
				int nops = 1;
				while(p < e && p->isNop()) { nops++; p++; }
				cumulativeWordLength += nops - 1;
				os << "NOP x " << nops << std::endl;

			// handle regular type 1 or type 2 packets
			} else {

				// look up the packet details
				Virtex2::EPacketType type = packet.getType();
				Virtex2::EOpcode opcode = packet.getOpcode();
				uint32_t wordCount = packet.getWordCount();
				const uint32_t word = packet[1];

				// account for the packet type
				os << Virtex2::sPacketTypeName[type];
				switch(type) {
				case Virtex2::ePacketType1: 
					address = Virtex2::ERegister(packet.getAddress());
					break;
				case Virtex2::ePacketType2: 
					break;
				default: 
					os << std::endl;
					continue;
				}

				// handle read packets
				if(opcode == packet.isRead()) {
					os << " READ " << Virtex2::sRegisterName[address];

				// handle write packets
				} else if(opcode == Virtex2::eOpcodeWrite) {
					os << " WRITE " << Virtex2::sRegisterName[address];
					// process according to register address
					switch(address) {
					case Virtex2::eRegisterFDRI:
						os << ": " << Hex32(wordCount) << " words";
						break;
					case Virtex2::eRegisterCMD: 
						os << " " << Virtex2::sCommandName[word];
						break;
					case Virtex2::eRegisterCOR:
						os << ": " << Hex32(word);
						Virtex2::writeSubfieldSettings(os, uint32_t(word), Virtex2::sCOR);
						break;
					case Virtex2::eRegisterSTAT:
						os << ": " << Hex32(word);
						Virtex2::writeSubfieldSettings(os, uint32_t(word), Virtex2::sSTAT);
						break;
					case Virtex2::eRegisterCTL:
						os << ": " << Hex32(word);
						Virtex2::writeSubfieldSettings(os, uint32_t(word), Virtex2::sCTL);
						break;
					case Virtex2::eRegisterMASK:
						os << ": " << Hex32(word);
						Virtex2::writeSubfieldSettings(os, uint32_t(word), Virtex2::sMASK);
						break;
					// Added to make frame mapping debug easier
					case Virtex2::eRegisterLOUT:
						oldColumnValue = currentColumnValue;
						oldBlockValue = currentBlockValue;
						currentColumnValue = (word & Virtex2::eFarMaskMajor);
						currentBlockValue = (word & Virtex2::eFarMaskBlockType);
						newColumn = (currentColumnValue != oldColumnValue);
						newBlock = (currentBlockValue != oldBlockValue);
						os << ": " << Hex32(word);
						if(newColumn) std::cout << "\t\t\t!!!New Column!!!";
						if(newBlock) std::cout << "\t\t\t\t\t***New Block***" << Hex32(currentBlockValue);
						break;
					default:
						// output the register contents
						os << ": " << Hex32(word);
						break;
					}
					os << std::endl;
				}

			}

		}

		// return the stream reference
		return os;
	}


	// Virtex4 Output Stream
	std::ostream& operator <<(std::ostream& os, const Virtex4& rhs) {
		// insert the bitstream header
		os << static_cast<const Bitstream>(rhs) << std::endl;
		uint32_t cumulativeWordLength = 0;

		// iterate over the packets
		Virtex4::ERegister address = Virtex4::ERegister();
		VirtexPacketVector::const_iterator p = rhs.begin();
		VirtexPacketVector::const_iterator e = rhs.end();
		bool newColumn = false;
		bool newRow = false;
		bool newTop = false;
		bool newBlock = false;
		uint32_t oldColumnValue = 0;
		uint32_t oldRowValue = 0;
		uint32_t oldTopValue = 0;
		uint32_t oldBlockValue = 0;
		uint32_t currentColumnValue = 0;
		uint32_t currentRowValue = 0;
		uint32_t currentTopValue = 0;
		uint32_t currentBlockValue = 0;
		while(p < e) {

			// insert the byte address
			os << "    " << Hex32(rhs.getHeaderByteLength() + (cumulativeWordLength << 2)) << ": ";
			// look up the packet
			const VirtexPacket& packet = *p++;
			cumulativeWordLength += packet.getWordSize();

			// handle dummy words
			if(packet.isDummyWord()) {
				os << "DUMMY" << std::endl;

			// handle sync words
			} else if(packet.isSyncWord()) {
				os << "SYNC" << std::endl;

			// handle reserved packets
			} else if(packet.isReserved()) {
				os << Virtex4::sOpcodeName[packet.getOpcode()] << std::endl;

			// handle NOP packets
			} else if(packet.isNop()) {
				int nops = 1;
				while(p < e && p->isNop()) { nops++; p++; }
				cumulativeWordLength += nops - 1;
				os << "NOP x " << nops << std::endl;

			// handle regular type 1 or type 2 packets
			} else {

				// look up the packet details
				Virtex4::EPacketType type = packet.getType();
				Virtex4::EOpcode opcode = packet.getOpcode();
				uint32_t wordCount = packet.getWordCount();
				const uint32_t word = packet[1];

				// account for the packet type
				os << Virtex4::sPacketTypeName[type];
				switch(type) {
				case Virtex4::ePacketType1: 
					address = Virtex4::ERegister(packet.getAddress());
					break;
				case Virtex4::ePacketType2: 
					break;
				default: 
					os << std::endl;
					continue;
				}

				// handle read packets
				if(opcode == packet.isRead()) {
					os << " READ " << Virtex4::sRegisterName[address];

				// handle write packets
				} else if(opcode == Virtex4::eOpcodeWrite) {
					os << " WRITE " << Virtex4::sRegisterName[address];
					// process according to register address
					switch(address) {
					case Virtex4::eRegisterFDRI:
						os << ": " << Hex32(wordCount) << " words";
						break;
					case Virtex4::eRegisterCMD: 
						os << " " << Virtex4::sCommandName[word];
						break;
					case Virtex4::eRegisterCOR:
						os << ": " << Hex32(word);
						Virtex4::writeSubfieldSettings(os, uint32_t(word), Virtex4::sCOR);
						break;
					case Virtex4::eRegisterSTAT:
						os << ": " << Hex32(word);
						Virtex4::writeSubfieldSettings(os, uint32_t(word), Virtex4::sSTAT);
						break;
					case Virtex4::eRegisterCTL:
						os << ": " << Hex32(word);
						Virtex4::writeSubfieldSettings(os, uint32_t(word), Virtex4::sCTL);
						break;
					case Virtex4::eRegisterMASK:
						os << ": " << Hex32(word);
						Virtex4::writeSubfieldSettings(os, uint32_t(word), Virtex4::sMASK);
						break;
					// Added to make frame mapping debug easier
					case Virtex4::eRegisterLOUT:
						oldColumnValue = currentColumnValue;
						oldRowValue = currentRowValue;
						oldTopValue = currentTopValue;
						oldBlockValue = currentBlockValue;
						currentColumnValue = (word & Virtex6::eFarMaskMajor);
						currentRowValue = (word & Virtex6::eFarMaskRow);
						currentTopValue = (word & Virtex6::eFarMaskTopBottom);
						currentBlockValue = (word & Virtex6::eFarMaskBlockType);
						newColumn = (currentColumnValue != oldColumnValue);
						newRow = (currentRowValue != oldRowValue);
						newTop = (currentTopValue != oldTopValue);
						newBlock = (currentBlockValue != oldBlockValue);
						os << ": " << Hex32(word);
						if(newColumn) std::cout << "\t\t\t!!!New Column!!!";
						if(newRow) std::cout << "\t\t\t$$$New Row$$$";
						if(newTop) std::cout << "\t\t\t&&&New Top&&&";
						if(newBlock) std::cout << "\t\t\t\t\t***New Block***" << Hex32(currentBlockValue);
						break;
					default:
						// output the register contents
						os << ": " << Hex32(word);
						break;
					}
					os << std::endl;
				}

			}

		}

		// return the stream reference
		return os;
	}

	// Virtex5 Output Stream
	std::ostream& operator <<(std::ostream& os, const Virtex5& rhs) {
		// insert the bitstream header
		os << static_cast<const Bitstream>(rhs) << std::endl;
		uint32_t cumulativeWordLength = 0;

		// iterate over the packets
		Virtex5::ERegister address = Virtex5::ERegister();
		VirtexPacketVector::const_iterator p = rhs.begin();
		VirtexPacketVector::const_iterator e = rhs.end();
		bool synchronized = false;
		bool newColumn = false;
		bool newRow = false;
		bool newTop = false;
		bool newBlock = false;
		uint32_t oldColumnValue = 0;
		uint32_t oldRowValue = 0;
		uint32_t oldTopValue = 0;
		uint32_t oldBlockValue = 0;
		uint32_t currentColumnValue = 0;
		uint32_t currentRowValue = 0;
		uint32_t currentTopValue = 0;
		uint32_t currentBlockValue = 0;
		while(p < e) {

			// insert the byte address
			os << "    " << Hex32(rhs.getHeaderByteLength() + (cumulativeWordLength << 2)) << ": ";
			// look up the packet
			const VirtexPacket& packet = *p++;
			cumulativeWordLength += packet.getWordSize();

			// catch bus sync words
			if(!synchronized) {
				if(packet.isBusWidthSyncWord()) {
					os << "BUS WIDTH SYNC" << std::endl;
					continue;
				} else if(packet.isBusWidthDetectWord()) {
					os << "BUS WIDTH DETECT" << std::endl;
					continue;
				}
			}

			// handle dummy words
			if(packet.isDummyWord()) {
				os << "DUMMY" << std::endl;

			// handle sync words
			} else if(packet.isSyncWord()) {
				os << "SYNC" << std::endl;

			// handle reserved packets
			} else if(packet.isReserved()) {
				os << Virtex5::sOpcodeName[packet.getOpcode()] << std::endl;

			// handle NOP packets
			} else if(packet.isNop()) {
				int nops = 1;
				while(p < e && p->isNop()) { nops++; p++; }
				cumulativeWordLength += nops - 1;
				os << "NOP x " << nops << std::endl;

			// handle regular type 1 or type 2 packets
			} else {

				// look up the packet details
				Virtex5::EPacketType type = packet.getType();
				Virtex5::EOpcode opcode = packet.getOpcode();
				uint32_t wordCount = packet.getWordCount();
				const uint32_t word = packet[1];
				// account for the packet type
				os << Virtex5::sPacketTypeName[type];
				switch(type) {
				case Virtex5::ePacketType1: 
					address = Virtex5::ERegister(packet.getAddress());
					break;
				case Virtex5::ePacketType2: 
					break;
				default: 
					os << std::endl;
					continue;
				}

				// handle read packets
				if(opcode == packet.isRead()) {
					os << " READ " << Virtex5::sRegisterName[address];

				// handle write packets
				} else if(opcode == Virtex5::eOpcodeWrite) {
					os << " WRITE " << Virtex5::sRegisterName[address];
					// process according to register address
					switch(address) {
					case Virtex5::eRegisterFDRI:
						os << ": " << Hex32(wordCount) << " words";
						break;
					case Virtex5::eRegisterCMD: 
						os << " " << Virtex5::sCommandName[word];
						break;
					case Virtex5::eRegisterCOR0:
						os << ": " << Hex32(word);
						Virtex5::writeSubfieldSettings(os, (uint32_t) word, Virtex5::sCOR0);
						break;
					case Virtex5::eRegisterCOR1:
						os << ": " << Hex32(word);
						Virtex5::writeSubfieldSettings(os, (uint32_t) word, Virtex5::sCOR1);
						break;
					case Virtex5::eRegisterSTAT:
						os << ": " << Hex32(word);
						Virtex5::writeSubfieldSettings(os, (uint32_t) word, Virtex5::sSTAT);
						break;
					case Virtex5::eRegisterCTL0:
						os << ": " << Hex32(word);
						Virtex5::writeSubfieldSettings(os, (uint32_t) word, Virtex5::sCTL0);
						break;
					case Virtex5::eRegisterCTL1:
						os << ": " << Hex32(word);
						Virtex5::writeSubfieldSettings(os, (uint32_t) word, Virtex5::sCTL1);
						break;
					case Virtex5::eRegisterMASK:
						os << ": " << Hex32(word);
						// we need to snoop the next packet, because the documented mask subfields 
						// apply to CTL0 only, and not to CTL1 which is completely undefined
						if(p < e) {
							const VirtexPacket& nextPacket = *p;
							if(nextPacket.isType1() && nextPacket.isWrite() 
								&& nextPacket.getAddress() == Virtex5::eRegisterCTL1) {
								os << " ()";
								break;
							}
						}
						Virtex5::writeSubfieldSettings(os, (uint32_t) word, Virtex5::sMASK0);
						break;
					case Virtex5::eRegisterWBSTAR:
						os << ": " << Hex32(word);
						Virtex5::writeSubfieldSettings(os, (uint32_t) word, Virtex5::sWBSTAR);
						break;
					case Virtex5::eRegisterTIMER:
						os << ": " << Hex32(word);
						Virtex5::writeSubfieldSettings(os, (uint32_t) word, Virtex5::sTIMER);
						break;
					// Added to make frame mapping debug easier
					case Virtex5::eRegisterLOUT:
						oldColumnValue = currentColumnValue;
						oldRowValue = currentRowValue;
						oldTopValue = currentTopValue;
						oldBlockValue = currentBlockValue;
						currentColumnValue = (word & Virtex6::eFarMaskMajor);
						currentRowValue = (word & Virtex6::eFarMaskRow);
						currentTopValue = (word & Virtex6::eFarMaskTopBottom);
						currentBlockValue = (word & Virtex6::eFarMaskBlockType);
						newColumn = (currentColumnValue != oldColumnValue);
						newRow = (currentRowValue != oldRowValue);
						newTop = (currentTopValue != oldTopValue);
						newBlock = (currentBlockValue != oldBlockValue);
						os << ": " << Hex32(word);
						if(newColumn) std::cout << "\t\t\t!!!New Column!!!";
						if(newRow) std::cout << "\t\t\t$$$New Row$$$";
						if(newTop) std::cout << "\t\t\t&&&New Top&&&";
						if(newBlock) std::cout << "\t\t\t\t\t***New Block***" << Hex32(currentBlockValue);
						break;
					default:
						// output the register contents
						os << ": " << Hex32(word);
						break;
					}

					os << std::endl;
				}

			}

		}

		// return the stream reference
		return os;
	}

	// Virtex6 Output Stream
	std::ostream& operator <<(std::ostream& os, const Virtex6& rhs) {
		// insert the bitstream header
		os << static_cast<const Bitstream>(rhs) << std::endl;
		uint32_t cumulativeWordLength = 0;

		// iterate over the packets
		Virtex6::ERegister address = Virtex6::ERegister();
		VirtexPacketVector::const_iterator p = rhs.begin();
		VirtexPacketVector::const_iterator e = rhs.end();
		bool newColumn = false;
		bool newRow = false;
		bool newTop = false;
		bool newBlock = false;
		uint32_t oldColumnValue = 0;
		uint32_t oldRowValue = 0;
		uint32_t oldTopValue = 0;
		uint32_t oldBlockValue = 0;
		uint32_t currentColumnValue = 0;
		uint32_t currentRowValue = 0;
		uint32_t currentTopValue = 0;
		uint32_t currentBlockValue = 0;
		bool synchronized = false;
		while(p < e) {

			// insert the byte address
			os << "    " << Hex32(rhs.getHeaderByteLength() + (cumulativeWordLength << 2)) << ": ";
			// look up the packet
			const VirtexPacket& packet = *p++;
			cumulativeWordLength += packet.getWordSize();

			// catch bus sync words
			if(!synchronized) {
				if(packet.isBusWidthSyncWord()) {
					os << "BUS WIDTH SYNC" << std::endl;
					continue;
				} else if(packet.isBusWidthDetectWord()) {
					os << "BUS WIDTH DETECT" << std::endl;
					continue;
				}
			}

			// handle dummy words
			if(packet.isDummyWord()) {
				os << "DUMMY" << std::endl;

			// handle sync words
			} else if(packet.isSyncWord()) {
				os << "SYNC" << std::endl;

			// handle reserved packets
			} else if(packet.isReserved()) {
				os << Virtex6::sOpcodeName[packet.getOpcode()] << std::endl;

			// handle NOP packets
			} else if(packet.isNop()) {
				int nops = 1;
				while(p < e && p->isNop()) { nops++; p++; }
				cumulativeWordLength += nops - 1;
				os << "NOP x " << nops << std::endl;

			// handle regular type 1 or type 2 packets
			} else {

				// look up the packet details
				Virtex6::EPacketType type = packet.getType();
				Virtex6::EOpcode opcode = packet.getOpcode();
				uint32_t wordCount = packet.getWordCount();
				const uint32_t word = packet[1];
				// account for the packet type
				os << Virtex6::sPacketTypeName[type];
				switch(type) {
				case Virtex6::ePacketType1: 
					address = Virtex6::ERegister(packet.getAddress());
					break;
				case Virtex6::ePacketType2: 
					break;
				default: 
					os << std::endl;
					continue;
				}

				// handle read packets
				if(opcode == packet.isRead()) {
					os << " READ " << Virtex6::sRegisterName[address];

				// handle write packets
				} else if(opcode == Virtex6::eOpcodeWrite) {
					os << " WRITE " << Virtex6::sRegisterName[address];
					// process according to register address
					switch(address) {
					case Virtex6::eRegisterFDRI:
						os << ": " << Hex32(wordCount) << " words";
						break;
					case Virtex6::eRegisterCMD: 
						os << " " << Virtex6::sCommandName[word];
						break;
					case Virtex6::eRegisterCOR0:
						os << ": " << Hex32(word);
						Virtex6::writeSubfieldSettings(os, (uint32_t) word, Virtex6::sCOR0);
						break;
					case Virtex6::eRegisterCOR1:
						os << ": " << Hex32(word);
						Virtex6::writeSubfieldSettings(os, (uint32_t) word, Virtex6::sCOR1);
						break;
					case Virtex6::eRegisterSTAT:
						os << ": " << Hex32(word);
						Virtex6::writeSubfieldSettings(os, (uint32_t) word, Virtex6::sSTAT);
						break;
					case Virtex6::eRegisterCTL0:
						os << ": " << Hex32(word);
						Virtex6::writeSubfieldSettings(os, (uint32_t) word, Virtex6::sCTL0);
						break;
					case Virtex6::eRegisterCTL1:
						os << ": " << Hex32(word);
						Virtex6::writeSubfieldSettings(os, (uint32_t) word, Virtex6::sCTL1);
						break;
					case Virtex6::eRegisterMASK:
						os << ": " << Hex32(word);
						// we need to snoop the next packet, because the documented mask subfields 
						// apply to CTL0 only, and not to CTL1 which is completely undefined
						if(p < e) {
							const VirtexPacket& nextPacket = *p;
							if(nextPacket.isType1() && nextPacket.isWrite() 
								&& nextPacket.getAddress() == Virtex6::eRegisterCTL1) {
								os << " ()";
								break;
							}
						}
						Virtex6::writeSubfieldSettings(os, (uint32_t) word, Virtex6::sMASK0);
						break;
					case Virtex6::eRegisterWBSTAR:
						os << ": " << Hex32(word);
						Virtex6::writeSubfieldSettings(os, (uint32_t) word, Virtex6::sWBSTAR);
						break;
					case Virtex6::eRegisterTIMER:
						os << ": " << Hex32(word);
						Virtex6::writeSubfieldSettings(os, (uint32_t) word, Virtex6::sTIMER);
						break;
					// Added to make frame mapping debug easier
					case Virtex6::eRegisterLOUT:
						oldColumnValue = currentColumnValue;
						oldRowValue = currentRowValue;
						oldTopValue = currentTopValue;
						oldBlockValue = currentBlockValue;
						currentColumnValue = (word & Virtex6::eFarMaskMajor);
						currentRowValue = (word & Virtex6::eFarMaskRow);
						currentTopValue = (word & Virtex6::eFarMaskTopBottom);
						currentBlockValue = (word & Virtex6::eFarMaskBlockType);
						newColumn = (currentColumnValue != oldColumnValue);
						newRow = (currentRowValue != oldRowValue);
						newTop = (currentTopValue != oldTopValue);
						newBlock = (currentBlockValue != oldBlockValue);
						os << ": " << Hex32(word);
						if(newColumn) std::cout << "\t\t\t!!!New Column!!!";
						if(newRow) std::cout << "\t\t\t$$$New Row$$$";
						if(newTop) std::cout << "\t\t\t&&&New Top&&&";
						if(newBlock) std::cout << "\t\t\t\t\t***New Block***" << Hex32(currentBlockValue);
						break;
					default:
						// output the register contents
						os << ": " << Hex32(word);
						break;
					}

					os << std::endl;
				}

			}

		}

		// return the stream reference
		return os;
	}

	// Virtex7 Output Stream
	std::ostream& operator <<(std::ostream& os, const Virtex7& rhs) {
		// insert the bitstream header
		os << static_cast<const Bitstream>(rhs) << std::endl;

		// return the stream reference
		return os;
	}


} // namespace bitstream
} // namespace torc
