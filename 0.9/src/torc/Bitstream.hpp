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
/// \brief Main torc::bitstream namespace header.

/// \namespace torc::bitstream
/// \brief Namespace for the Torc bitstream frame-level support, including packet parsing and 
///		writing.

#ifndef TORC_BITSTREAM_HPP
#define TORC_BITSTREAM_HPP

#include "torc/bitstream/Bitstream.hpp"
#include "torc/bitstream/DeviceInfo.hpp"
#include "torc/bitstream/OutputStreamHelpers.hpp"
#include "torc/bitstream/Spartan6Bitstream.hpp"
#include "torc/bitstream/Spartan6Packet.hpp"
#include "torc/bitstream/Spartan3E.hpp"
#include "torc/bitstream/Spartan6.hpp"
#include "torc/bitstream/SpartanBitstream.hpp"
#include "torc/bitstream/SpartanPacket.hpp"
#include "torc/bitstream/Virtex.hpp"
#include "torc/bitstream/Virtex2.hpp"
#include "torc/bitstream/Virtex2P.hpp"
#include "torc/bitstream/Virtex4.hpp"
#include "torc/bitstream/Virtex5.hpp"
#include "torc/bitstream/Virtex6.hpp"
#include "torc/bitstream/VirtexBitstream.hpp"
#include "torc/bitstream/VirtexE.hpp"
#include "torc/bitstream/VirtexPacket.hpp"

#endif // TORC_BITSTREAM_HPP
