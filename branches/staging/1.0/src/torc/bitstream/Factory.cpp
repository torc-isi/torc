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
/// \brief Source for the Factory class.

#include "torc/bitstream/Factory.hpp"
#include "torc/bitstream/Virtex.hpp"
#include "torc/bitstream/VirtexE.hpp"
#include "torc/bitstream/Virtex2.hpp"
#include "torc/bitstream/Virtex2P.hpp"
#include "torc/bitstream/Virtex4.hpp"
#include "torc/bitstream/Virtex5.hpp"
#include "torc/bitstream/Virtex6.hpp"
#include "torc/bitstream/Virtex7.hpp"
#include "torc/bitstream/Spartan3E.hpp"
#include "torc/bitstream/Spartan6.hpp"
#include <fstream>

namespace torc {
namespace bitstream {

	using torc::common::DeviceDesignator;

	BitstreamSharedPtr Factory::newBitstreamPtr(const char* inFileName) {
		// open the file as a stream
		BitstreamSharedPtr bitstreamPtr;
		std::fstream fileStream(inFileName, std::ios::binary | std::ios::in);
		if(!fileStream.good()) return bitstreamPtr;

		// read the header and determine the device family
		Bitstream bitstream;
		bitstream.readHeader(fileStream);
		DeviceDesignator designator(bitstream.getDeviceName());

		// instantiate the appropriate bitstream
		switch(designator.getFamily()) {
		case DeviceDesignator::eFamilyVirtex:
			bitstreamPtr = BitstreamSharedPtr(new Virtex()); break;
		case DeviceDesignator::eFamilyVirtexE:
			bitstreamPtr = BitstreamSharedPtr(new VirtexE()); break;
		case DeviceDesignator::eFamilyVirtex2:
			bitstreamPtr = BitstreamSharedPtr(new Virtex2()); break;
		case DeviceDesignator::eFamilyVirtex2P:
			bitstreamPtr = BitstreamSharedPtr(new Virtex2P()); break;
		case DeviceDesignator::eFamilyVirtex4:
			bitstreamPtr = BitstreamSharedPtr(new Virtex4()); break;
		case DeviceDesignator::eFamilyVirtex5:
			bitstreamPtr = BitstreamSharedPtr(new Virtex5()); break;
		case DeviceDesignator::eFamilyVirtex6:
			bitstreamPtr = BitstreamSharedPtr(new Virtex6()); break;
		case DeviceDesignator::eFamilyArtix7:
		case DeviceDesignator::eFamilyKintex7:
		case DeviceDesignator::eFamilyVirtex7:
		case DeviceDesignator::eFamilyZynq7000:
			bitstreamPtr = BitstreamSharedPtr(new Virtex7()); break;
		case DeviceDesignator::eFamilySpartan3E:
			bitstreamPtr = BitstreamSharedPtr(new Spartan3E()); break;
		case DeviceDesignator::eFamilySpartan6:
			bitstreamPtr = BitstreamSharedPtr(new Spartan6()); break;
		default:
			return bitstreamPtr;
		}

		// rewind the stream
		fileStream.seekg(0, std::ios::beg);
		bitstreamPtr->read(fileStream);

		// return the newly created bitstream
		return bitstreamPtr;
	}

} // namespace bitstream
} // namespace torc
