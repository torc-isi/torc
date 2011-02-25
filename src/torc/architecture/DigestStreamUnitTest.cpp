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
/// \brief Source for the DigestStream unit test.

#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>
#include "torc/architecture/DigestStream.hpp"
#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/common/DirectoryTree.hpp"

namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the DigestStream class.
BOOST_AUTO_TEST_CASE(architecture_digest_stream) {
	DigestStream digestStream(torc::common::DirectoryTree::getDevicesPath() / "xc5vlx30.db");
	std::string header;
	digestStream.readSectionHeader(header);
	BOOST_CHECK_EQUAL(header, ">>>> Family >>>>");
	boost::uint16_t familyNameLength;
	digestStream.read(familyNameLength);
	BOOST_CHECK_EQUAL(familyNameLength, 7u);
	boost::scoped_ptr<char> familyNamePtr(new char[familyNameLength]);
	digestStream.read(familyNamePtr.get(), familyNameLength);
	BOOST_CHECK(memcmp(familyNamePtr.get(), "Virtex5", familyNameLength) == 0);
	digestStream.readSectionHeader(header);
	BOOST_CHECK_EQUAL(header, ">>>>TileMap >>>>");
	torc::architecture::xilinx::TileCount tileCount;
	digestStream.read(tileCount);
	BOOST_CHECK_EQUAL(tileCount, 8633u);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
