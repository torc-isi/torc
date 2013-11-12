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
#include "Assembler.hpp"
#include "Virtex5Assembler.hpp"
#include "Virtex7Assembler.hpp"
#include <boost/smart_ptr.hpp>

namespace torc {
namespace bitstream {

	typedef boost::shared_ptr<Assembler> AssemblerSharedPtr;
	typedef torc::common::DeviceDesignator DeviceDesignator;

	/// \brief Helper class for bitstreams.
	class AssemblerFactory {
	public:
		static AssemblerSharedPtr newAssemblerPtr(torc::physical::DesignSharedPtr xdlDesignPtr, torc::architecture::DDB &inDB) {
			AssemblerSharedPtr assemblerPtr;

			// read the header and determine the device family
			DeviceDesignator designator(xdlDesignPtr->getDevice());

			// instantiate the appropriate bitstream
			switch(designator.getFamily()) {
				case DeviceDesignator::eFamilyVirtex5:
					assemblerPtr = AssemblerSharedPtr(new Virtex5Assembler(inDB)); break;
				case DeviceDesignator::eFamilyVirtex7:
				case DeviceDesignator::eFamilyArtix7:
				case DeviceDesignator::eFamilyKintex7:
				case DeviceDesignator::eFamilyZynq7000:
					assemblerPtr = AssemblerSharedPtr(new Virtex7Assembler(inDB)); break;
				case DeviceDesignator::eFamilyVirtex:
				case DeviceDesignator::eFamilyVirtexE:
				case DeviceDesignator::eFamilyVirtex2:
				case DeviceDesignator::eFamilyVirtex2P:
				case DeviceDesignator::eFamilyVirtex4:
				default:
					std::cerr << "Device " << xdlDesignPtr->getDevice() << " not supported." << std::endl;
					return assemblerPtr;
			}
			return assemblerPtr;
		}
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_ASSEMBLER_ASSEMBLERFACTORY_HPP
