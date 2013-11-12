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
/// \brief Header for the Factory class.

#ifndef TORC_BITSTREAM_FACTORY_HPP
#define TORC_BITSTREAM_FACTORY_HPP

#include "torc/bitstream/Bitstream.hpp"
#include <boost/filesystem.hpp>
#include <boost/smart_ptr.hpp>

namespace torc {
namespace bitstream {

namespace bitstream { class FactoryUnitTest; }

	typedef boost::shared_ptr<Bitstream> BitstreamSharedPtr;

	/// \brief Helper class for bitstreams.
	class Factory {
		friend class torc::bitstream::bitstream::FactoryUnitTest;
	public:
		static BitstreamSharedPtr newBitstreamPtr(const boost::filesystem::path& inPath) {
			return newBitstreamPtr(inPath.string().c_str());
		}

		static BitstreamSharedPtr newBitstreamPtr(const char* inFileName);
	};

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_FACTORY_HPP
