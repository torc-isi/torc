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
/// \brief Source for the DDBConsoleStreams class.

#include <iostream>
#include "torc/architecture/DDBConsoleStreams.hpp"
#include "torc/common/NullOutputStream.hpp"

using namespace std;
using namespace torc::common;

namespace torc {
namespace architecture {

	DDBConsoleStreams DDBConsoleStreams::sDefaultConsoleStreams;
	DDBConsoleStreams DDBConsoleStreams::sNullConsoleStreams(cin, cnull, cnull, cnull);

	DDBConsoleStreams::DDBConsoleStreams(void) {
		mInPtr = &std::cin;
		mOutPtr = &std::cout;
		mErrPtr = &std::cerr;
		mLogPtr = &std::clog;
	}

} // namespace architecture
} // namespace torc
