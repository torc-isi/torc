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
/// \brief Header for the DigestStream class.

#ifndef TORC_ARCHITECTURE_DIGESTSTREAM_HPP
#define TORC_ARCHITECTURE_DIGESTSTREAM_HPP

#include "torc/common/Endian.hpp"
#include <boost/filesystem/convenience.hpp>
#include <boost/cstdint.hpp>
#include "torc/externals/zlib/contrib/iostream3/zfstream.h"

namespace torc {
namespace architecture {

	/// \brief Encapsulation of a device or family digest stream.
	/// \details DigestStreams are used to read family and device database files.
	class DigestStream : public gzifstream {
	protected:
	// types
		/// \brief Imported type name.
		typedef std::string string;
		/// \brief Imported type name.
		typedef boost::uint16_t uint16_t;
		/// \brief Imported type name.
		typedef boost::uint32_t uint32_t;
		/// \brief Definition of a digest section header.
		typedef char DigestSectionHeader[16];
	// members
		/// \brief The number of bytes read.
		size_t mBytesRead;
	public:
		/// \brief Public constructor.
		DigestStream(const boost::filesystem::path& inPath)
			: gzifstream(inPath.string().c_str(), ios_base::in), mBytesRead(0) {}
	// accessors
		/// \brief Returns the number of bytes read.
		size_t getBytesRead(void) const { return mBytesRead; }
	// functions
		/// \brief Read and return a section header.
		/// \param outHeader The section header that was read from the stream.
		void readSectionHeader(string& outHeader) {
			DigestSectionHeader sectionHeader;
			gzifstream::read(static_cast<char*>(sectionHeader), sizeof(sectionHeader));
			mBytesRead += sizeof(sectionHeader);
			outHeader.assign(sectionHeader, sizeof(sectionHeader));
		}
		/// \brief Read and return a uint8_t.
		/// \param outValue The uint8_t that was read from the stream.
		std::istream& read(uint8_t& outValue) {
			gzifstream::read(reinterpret_cast<char*>(&outValue), sizeof(uint8_t));
			//outValue = ntohs(outValue); // no endian conversion needed for single bytes
			mBytesRead += sizeof(uint8_t);
			return *this;
		}
		/// \brief Read and return a uint16_t.
		/// \param outValue The uint16_t that was read from the stream.
		std::istream& read(uint16_t& outValue) {
			gzifstream::read(reinterpret_cast<char*>(&outValue), sizeof(uint16_t));
			outValue = ntohs(outValue); // endian conversion
			mBytesRead += sizeof(uint16_t);
			return *this;
		}
		/// \brief Read and return a uint32_t.
		/// \param outValue The uint32_t that was read from the stream.
		std::istream& read(uint32_t& outValue) {
			gzifstream::read(reinterpret_cast<char*>(&outValue), sizeof(uint32_t));
			outValue = ntohl(outValue); // endian conversion
			mBytesRead += sizeof(uint32_t);
			return *this;
		}
		/// \brief Read and return a character string.
		/// \details This overrides the superclass behavior exclusively for the sake of tracking 
		///		the number of bytes read.
		/// \param s The character pointer to read into.
		/// \param n The number of characters to read.
		std::istream& read(char* s, std::streamsize n) {
			gzifstream::read(s, n);
			mBytesRead += n;
			return *this;
		}
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_DIGESTSTREAM_HPP
