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
/// \brief Header for the Spartan6BuildHelper class.

#ifndef TORC_BITSTREAM_BUILD_SPARTAN6BUILDHELPER_HPP
#define TORC_BITSTREAM_BUILD_SPARTAN6BUILDHELPER_HPP

#include "torc/bitstream/Bitstream.hpp"
#include "torc/common/Devices.hpp"

namespace torc {
namespace bitstream {

	/// \brief Xilinx bitstream base class.
	class Spartan6BuildHelper {
	public:
	// functions
		/// \brief Build the static device information for the specified family.
		static void buildFamilyDeviceInfo(const std::string& inFamilyName, 
			const std::string& inTemplateName, const std::string& inOutputName, 
			const torc::common::DeviceVector& inDeviceNames, Bitstream& inBitstream);
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_BUILD_SPARTAN6BUILDHELPER_HPP
