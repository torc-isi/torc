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
/// \brief Unit test for the Package class.

#include <boost/test/unit_test.hpp>
#include "torc/architecture/Package.hpp"
#include "torc/architecture/DDB.hpp"

namespace torc {
namespace architecture {

BOOST_AUTO_TEST_SUITE(architecture)

/// \brief Unit test for the Package class.
BOOST_AUTO_TEST_CASE(PackageUnitTest) {
	// members tested:
	//		string mName;
	//		PadArray mPads;
	//		PadNameToPadIndexMap mPadNameToPadIndex;
	// functions tested:
	//		Package(const string& inName);
	//		PadArray& getPads(void);
	//		const string& getName(void) const;
	//		const PadArray& getPads(void) const;
	//		PadIndex findPadIndexByName(const string& inName) const;
	int index = 0;
	std::string name1("pad1");
	std::string name2("pad2");
	std::string name3("pad3");
	Pad pad1(xilinx::SiteIndex(), name1, xilinx::SiteFlags());
	Pad pad2(xilinx::SiteIndex(), name2, xilinx::SiteFlags());
	Pad pad3(xilinx::SiteIndex(), name3, xilinx::SiteFlags());
	std::string name("name");
	Package package1(name);
	PadArray& pads1 = package1.getPads();
	pads1.setSize(3);
	package1.mPadNameToPadIndex[name1] = xilinx::PadIndex(index);
	const_cast<Pad&>(pads1[index++]) = pad1;
	package1.mPadNameToPadIndex[name2] = xilinx::PadIndex(index);
	const_cast<Pad&>(pads1[index++]) = pad2;
	package1.mPadNameToPadIndex[name3] = xilinx::PadIndex(index);
	const_cast<Pad&>(pads1[index++]) = pad3;
	BOOST_CHECK_EQUAL(package1.getName(), name);
	const PadArray& pads2 = package1.getPads();
	index = 0;
	BOOST_CHECK_EQUAL(pads2[index++].getName(), pad1.getName());
	BOOST_CHECK_EQUAL(pads2[index++].getName(), pad2.getName());
	BOOST_CHECK_EQUAL(pads2[index++].getName(), pad3.getName());
	BOOST_CHECK_EQUAL(package1.findPadIndexByName(name1), xilinx::PadIndex(0));
	BOOST_CHECK_EQUAL(package1.findPadIndexByName(name2), xilinx::PadIndex(1));
	BOOST_CHECK_EQUAL(package1.findPadIndexByName(name3), xilinx::PadIndex(2));

	// functions tested:
	//		Package(void);
	//		PadArray& getPads(void);
	Package package2;
	BOOST_CHECK_EQUAL(package2.getName(), "");
	BOOST_CHECK_EQUAL(package2.getPads().getSize(), 0u);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace architecture
} // namespace torc
