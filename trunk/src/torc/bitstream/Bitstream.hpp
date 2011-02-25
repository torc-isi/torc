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
/// \brief Header for the Bitstream class.

#ifndef TORC_BITSTREAM_BITSTREAM_HPP
#define TORC_BITSTREAM_BITSTREAM_HPP

#include "torc/common/Endian.hpp"
#include "torc/bitstream/DeviceInfo.hpp"
#include "torc/common/EncapsulatedInteger.hpp"
#include <istream>
#include <ostream>
#include <string>
#include <map>

/// \todo remove <iostream>
#include <iostream>

namespace torc {
namespace bitstream {

namespace bitstream { class bitstream_bitstream; }

	/// \brief Xilinx bitstream base class.
	class Bitstream {
		friend class torc::bitstream::bitstream::bitstream_bitstream;
/// \todo Bitstream access needs to be changed from public back to protected.
//	protected: changed to run tests
		public:
		typedef std::string string; ///< Imported type name.
		typedef boost::uint8_t uint8_t; ///< Imported type name.
		typedef boost::uint16_t uint16_t; ///< Imported type name.
		typedef boost::uint32_t uint32_t; ///< Imported type name.

		/// \brief Write a uint8_t to the stream.
		void write(std::ostream& inStream, uint8_t inVal) {
			uint8_t actual = inVal;
			inStream.write((char*) &actual, sizeof(actual));
		}
		/// \brief Write a uint16_t to the stream.
		void write(std::ostream& inStream, uint16_t inVal) {
			uint16_t actual = htons(inVal);
			inStream.write((char*) &actual, sizeof(actual));
		}
		/// \brief Write a uint32_t to the stream.
		void write(std::ostream& inStream, uint32_t inVal) {
			uint32_t actual = htonl(inVal);
			inStream.write((char*) &actual, sizeof(actual));
		}
		/// \brief Write a Xilinx-style header string to the stream.
		void writeHeaderString(std::ostream& inStream, const string& inString) {
			// write the string length
			uint16_t length = inString.length() + 1;
			write(inStream, uint16_t(length));
			inStream.write(inString.c_str(), length);
		}
		/// \brief Look for the expected uint8_t in the stream and return true if it matches.
		static bool expect(std::istream& inStream, uint8_t inExpected) {
			// read the actual data from the stream
			uint8_t actual = 0;
			inStream.read((char*) &actual, sizeof(actual));
			// return equality
			return inExpected == actual;
		}
		/// \brief Look for the expected uint16_t in the stream and return true if it matches.
		static bool expect(std::istream& inStream, uint16_t inExpected) {
			// read the actual data from the stream
			uint16_t actual = 0;
			inStream.read((char*) &actual, sizeof(actual));
			// return equality
			return inExpected == ntohs(actual);
		}
		/// \brief Look for the expected uint32_t in the stream and return true if it matches.
		static bool expect(std::istream& inStream, uint32_t inExpected) {
			// read the actual data from the stream
			uint32_t actual = 0;
			inStream.read((char*) &actual, sizeof(actual));
			// return equality
			return inExpected == ntohl(actual);
		}
//		/// \brief Look for the expected string in the stream and return true if it matches.
//		static bool expect(std::istream& inStream, const string& inExpected) {
//			// look for the length;
//			if(!expect(inStream, (uint16_t) inExpected.size())) return false;
//			// create a buffer to use
//			int adjustedLength = inExpected.size() + 1;
//			char* buffer = new char[adjustedLength];
//			// read the actual data from the stream
//			inStream.read(buffer, adjustedLength);
//			return buffer == inExpected;
//		}
		/// \brief Read and return a bitstream header string.
		static void readHeaderString(std::istream& inStream, string& outString) {
			// read the string length
			uint16_t length = 0;
			inStream.read((char*) &length, sizeof(length));
			length = ntohs(length);
			if(length > 0) {
				// create a buffer
				char* buffer = new char[length];
				// read the null-terminated string
				inStream.read(buffer, length);
				// copy the data into the string
				outString.assign(buffer, length - 1);
				delete[] buffer;
			} else {
				outString.clear();
			}
		}
		/// \brief Read a bitstream header, and return its fields.
		static bool readHeader(std::istream& inStream, string& outDesignName, 
			string& outDeviceName, string& outDesignDate, string& outDesignTime, 
			uint32_t& outBitstreamLength, uint32_t& outHeaderLength) {
			// assume success unless we find otherwise
			bool success = true;
			// read the magic length
			success &= expect(inStream, uint16_t(0x0009));
			// read the magic bytes
			success &= expect(inStream, uint32_t(0x0ff00ff0));
			success &= expect(inStream, uint32_t(0x0ff00ff0));
			success &= expect(inStream, uint8_t(0x00));
			// read the mysterious 0x0001
			success &= expect(inStream, uint16_t(0x0001));
			// read the 'a' byte
			success &= expect(inStream, uint8_t('a'));
			// read the design name length
			readHeaderString(inStream, outDesignName);
			// read the 'b' byte
			success &= expect(inStream, uint8_t('b'));
			// read the device name length
			readHeaderString(inStream, outDeviceName);
			// read the 'c' byte
			success &= expect(inStream, uint8_t('c'));
			// read the design date length
			readHeaderString(inStream, outDesignDate);
			// read the 'd' byte
			success &= expect(inStream, uint8_t('d'));
			// read the design time length
			readHeaderString(inStream, outDesignTime);
			// read the 'e' byte
			success &= expect(inStream, uint8_t('e'));
			// read the inStream length
			inStream.read((char*) &outBitstreamLength, sizeof(outBitstreamLength));
			outBitstreamLength = ntohl(outBitstreamLength);
			// determine the header length
			outHeaderLength = 34 + outDesignName.length() + outDeviceName.length() 
				+ outDesignDate.length() + outDesignTime.length();
			// return the result
			return success;
		}
		/// \brief Clean up the header date and time by replacing embedded spaces with zeros.
		void cleanDateAndTime(void) {
			// some versions of the tools leave spaces inside the date and time fields
			size_t pos = 0;
			while((pos = mDesignDate.find(' ', pos)) != string::npos) mDesignDate[pos] = '0';
			pos = 0;
			while((pos = mDesignTime.find(' ', pos)) != string::npos) mDesignTime[pos] = '0';
		}
	// inner class
		struct Subfield {
			/// \brief The subfield bit mask.
			uint32_t mMask;
			/// \brief The subfield LSB position.
			uint32_t mShift;
			/// \brief The subfield name documented in bitgen.
			const char* mBitgenName;
			/// \brief The subfield name documented in the configuration guide.
			const char* mConfigGuideName;
			/// \brief The default subfield value.
			uint32_t mDefault;
			/// \brief The allowable subfield values.
			const char** mValues;
		};
		/// \brief Insert 32 bit subfield settings into an output stream.
		static void writeSubfieldSettings(std::ostream& inStream, uint32_t inWord, 
			const Subfield* inSubfields);
		/// \brief Insert 16 bit subfield settings into an output stream.
		static void writeSubfieldSettings(std::ostream& inStream, uint16_t inWord, 
			const Subfield* inSubfields);
		/// \brief Initialize the maps between frame indexes and frame addresses.
		/// \detail This is generally only useful for internal purposes.
		/// \todo This function should be made pure virtual.
		virtual void initializeDeviceInfo(const std::string& inDeviceName) {}
		/// \brief Output static device information to a stream.
		/// \details This is used to generate the static column maps for bitstream frame mapping.
		virtual void writeDeviceInfo(std::ostream& inStream, const std::string& inDeviceName);
	// accessors
		/// \brief Assign static device information for the current bitstream.
		void setDeviceInfo(const DeviceInfo& rhs) { mDeviceInfo = rhs; }
	// members
		/// \brief Header design name.
		string mDesignName;
		/// \brief Header device name.
		string mDeviceName;
		/// \brief Header design date.
		string mDesignDate;
		/// \brief Header design time.
		string mDesignTime;
		/// \brief Bitstream packet length in bytes.
		/// \details This is the length in bytes of all the bitstream packets, without the 
		///		bitstream header.
		uint32_t mBitstreamByteLength;
		/// \brief Header length in bytes.
		/// \details This is the length of the header itself, as opposed to mBitstreamByteLength, 
		///		which is the length of bitstream reported by the header.
		uint32_t mHeaderByteLength;
	// members for frame addressing
		/// \brief Column type widths.
		ColumnDefVector mColumnDefs;
		/// \brief Mapping from tile type names to column types.
		typedef std::map<std::string, uint32_t> TileTypeNameToColumnType;
		TileTypeNameToColumnType mTileTypeNameToColumnType;
		/// \brief Mapping from tile indexes to column types.
		typedef std::map<uint16_t, uint32_t> TileTypeIndexToColumnType;
		TileTypeIndexToColumnType mTileTypeIndexToColumnType;
		/// \brief Device information.
		DeviceInfo mDeviceInfo;
	public:
	// constructors
		/// \brief Basic constructor.
		Bitstream(void) : mBitstreamByteLength(0) {}
		/// \brief Virtual destructor.
		virtual ~Bitstream(void) {}
	// functions
		/// \brief Read the bitstream header and packets from a stream.
		virtual void read(std::istream& inStream, bool inCleanDateAndTime = true) {
			readHeader(inStream);
			readPackets(inStream);
			if(inCleanDateAndTime) cleanDateAndTime();
		}
		/// \brief Read the bitstream packets
		virtual void readPackets(std::istream& inStream) {}
		/// \brief Read the bitstream header.
		virtual void readHeader(std::istream& inStream) {
			readHeader(inStream, mDesignName, mDeviceName, mDesignDate, mDesignTime, 
				mBitstreamByteLength, mHeaderByteLength);
		}
		/// \brief Write the bitstream header and packets to a stream.
		virtual void write(std::ostream& inStream) {
			preflightPackets();
			updatePacketLength();
			writeHeader(inStream);
			writePackets(inStream);
		}
		/// \brief Preflight the packets.
		virtual void preflightPackets(void) {}
		/// \brief Update the header packet length.
		virtual void updatePacketLength(void) {}
		/// \brief Write the bitstream packets.
		virtual void writePackets(std::ostream& inStream) {}
		/// \brief Write the bitstream header to the stream.
		virtual void writeHeader(std::ostream& inStream) {
			// write the magic length
			write(inStream, uint16_t(0x0009));
			// write the magic bytes with null termination
			write(inStream, uint32_t(0x0ff00ff0));
			write(inStream, uint32_t(0x0ff00ff0));
			write(inStream, uint8_t(0x00));
			// write the mysterious 0x0001
			write(inStream, uint16_t(0x0001));
			// write the 'a' byte and the design name
			write(inStream, uint8_t('a'));
			writeHeaderString(inStream, mDesignName);
			// write the 'b' byte and the device name
			write(inStream, uint8_t('b'));
			writeHeaderString(inStream, mDeviceName);
			// write the 'c' byte and the design date
			write(inStream, uint8_t('c'));
			writeHeaderString(inStream, mDesignDate);
			// write the 'd' byte and the design time
			write(inStream, uint8_t('d'));
			writeHeaderString(inStream, mDesignTime);
			// write the 'e' byte and the design name
			write(inStream, uint8_t('e'));
			write(inStream, uint32_t(mBitstreamByteLength));
		}
	// inserters
		/// \brief Insert the bitstream header into an output stream.
		friend std::ostream& operator<< (std::ostream& os, const Bitstream& rhs);
	// accessors
		/// \brief Set the design name.
		void setDesignName(const string& inDesignName) { mDesignName = inDesignName; }
		/// \brief Set the device name.
		void setDeviceName(const string& inDeviceName) { mDeviceName = inDeviceName; }
		/// \brief Set the design date.
		void setDesignDate(const string& inDesignDate) { mDesignDate = inDesignDate; }
		/// \brief Set the design time.
		void setDesignTime(const string& inDesignTime) { mDesignTime = inDesignTime; }
		/// \brief Return the design name.
		const string& getDesignName(void) const { return mDesignName; }
		/// \brief Return the device name.
		const string& getDeviceName(void) const { return mDeviceName; }
		/// \brief Return the design date.
		const string& getDesignDate(void) const { return mDesignDate; }
		/// \brief Return the design time.
		const string& getDesignTime(void) const { return mDesignTime; }
		/// \brief Return the bitstream packet length in bytes.
		/// \details The length includes all packet data, but does not include the bitstream header.
		uint32_t getBitstreamByteLength(void) const { return mBitstreamByteLength; }
		/// \brief Return the bitstream header length in bytes.
		uint32_t getHeaderByteLength(void) const { return mHeaderByteLength; }
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_BITSTREAM_HPP
