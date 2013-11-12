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

/// \file
/// \brief Unit test for the Pad class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/Pad.hpp"

namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the Pad class.
BOOST_AUTO_TEST_CASE(PadUnitTest) {
	// members tested:
	//		SiteIndex mSiteIndex;
	//		string mName;
	//		SiteFlags mFlags;
	// functions tested:
	//		Pad(SiteIndex inSiteIndex, const string& inName, SiteFlags inFlags);
	//		Pad(void);
	//		SiteIndex getSiteIndex(void) const;
	//		const string& getName(void) const;
	//		SiteFlags getFlags(void) const;
	xilinx::SiteIndex siteIndex(15);
	std::string name("name");
	xilinx::SiteFlags flags(3);
	Pad pad1(siteIndex, name, flags);
	BOOST_CHECK_EQUAL(pad1.getSiteIndex(), siteIndex);
	BOOST_CHECK_EQUAL(pad1.getName(), name);
	BOOST_CHECK_EQUAL(pad1.getFlags(), flags);
	Pad pad2;
	BOOST_CHECK_EQUAL(pad2.getSiteIndex(), xilinx::SiteIndex());
	BOOST_CHECK_EQUAL(pad2.getName(), "");
	BOOST_CHECK_EQUAL(pad2.getFlags(), xilinx::SiteFlags());
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
