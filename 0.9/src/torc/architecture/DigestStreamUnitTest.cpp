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
/// \brief Unit test for the DigestStream class.

#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>
#include "torc/architecture/DigestStream.hpp"
#include "torc/architecture/Versions.hpp"
#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/common/DirectoryTree.hpp"

namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the DigestStream class.
BOOST_AUTO_TEST_CASE(DigestStreamUnitTest) {
	// functions tested:
	//		DigestStream(const boost::filesystem::path& inPath);
	DigestStream digestStream(torc::common::DirectoryTree::getDevicesPath() / "xc5vlx30.db");
	Versions versions;

	// functions tested:
	//		std::istream& read(uint8_t& outValue);
	//		std::istream& read(uint16_t& outValue);
	//		std::istream& read(uint32_t& outValue);
	//		std::istream& read(char* s, std::streamsize n);
	BOOST_CHECK(versions.readVersions(digestStream, true) > 0);
	std::string header;

	// functions tested:
	//		void readSectionHeader(string& outHeader);
	//		std::istream& read(uint8_t& outValue);
	//		std::istream& read(uint16_t& outValue);
	//		std::istream& read(uint32_t& outValue);
	//		std::istream& read(char* s, std::streamsize n);
	digestStream.readSectionHeader(header);
	BOOST_CHECK_EQUAL(header, ">>>> Family >>>>");
	boost::uint16_t familyNameLength;
	digestStream.read(familyNameLength);
	BOOST_CHECK_EQUAL(familyNameLength, 7u);
	boost::scoped_ptr<char> familyNamePtr(new char[familyNameLength]);
	digestStream.read(familyNamePtr.get(), familyNameLength);
	BOOST_CHECK(memcmp(familyNamePtr.get(), "Virtex5", familyNameLength) == 0);
	digestStream.readSectionHeader(header);
	BOOST_CHECK_EQUAL(header, ">>>> Speeds >>>>");
	uint16_t speedGradeCount;
	digestStream.read(speedGradeCount);
	BOOST_CHECK_EQUAL(speedGradeCount, 3);

	// members tested:
	//		size_t mBytesRead;
	// functions tested:
	//		size_t getBytesRead(void) const;
	BOOST_CHECK(digestStream.getBytesRead() > 0u);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
