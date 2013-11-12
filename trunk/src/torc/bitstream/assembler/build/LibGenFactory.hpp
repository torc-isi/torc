// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
// $HeadURL: https://svn.east.isi.edu/torc/trunk/src/torc/bitstream/assembler/lut/parser.yy $
// $Id: parser.yy 1303 2013-02-25 23:18:16Z nsteiner $

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

/// \file Factory.h
/// \brief Returns appropriate bitstream assembler object
//	\author Ritesh Kumar Soni

#ifndef TORC_BITSTREAM_ASSEMBLER_ASSEMBLERFACTORY_HPP
#define TORC_BITSTREAM_ASSEMBLER_ASSEMBLERFACTORY_HPP

#include "torc/bitstream/Bitstream.hpp"
#include "AssemblerLibGen.hpp"
#include "Virtex5LibGen.hpp"
#include "Virtex7LibGen.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/smart_ptr.hpp>

namespace torc {
namespace bitstream {

	typedef boost::shared_ptr<AssemblerLibGen> AssemblerLibGenSharedPtr;
	typedef torc::common::DeviceDesignator DeviceDesignator;
	typedef std::string string;
	typedef boost::filesystem::path path;

	/// \brief Helper class for bitstreams.
	class LibGenFactory {
	public:
		static AssemblerLibGenSharedPtr newLibGenPtr(string inFamily, path inXdlGenerationFolder) {
			AssemblerLibGenSharedPtr libGenPtr;
			path harnessFolder("torc/bitstream/assembler/build/harness");
			// instantiate the appropriate bitstream
			if(boost::iequals(inFamily, "virtex5")) {
				libGenPtr = AssemblerLibGenSharedPtr(new Virtex5LibGen("xc5vlx110t", harnessFolder, inXdlGenerationFolder));
			} else if( boost::iequals(inFamily, "virtex7") || boost::iequals(inFamily, "artix7") || 
						boost::iequals(inFamily, "kintex7") || boost::iequals(inFamily, "zynq7") ) {
				libGenPtr = AssemblerLibGenSharedPtr(new Virtex7LibGen("xc7a100t", harnessFolder, inXdlGenerationFolder));
			} else {
				std::cout << "WARNING: Unsupported device" << std::endl;
			}
			return libGenPtr;
		}
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_ASSEMBLER_ASSEMBLERFACTORY_HPP
