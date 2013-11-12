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
/// \brief Static definition of the VirtexE class.
/// \details This is a generated file.  Please modify with care.

#include "torc/bitstream/VirtexE.hpp"

namespace torc {
namespace bitstream {

	enum { Empty = VirtexE::eColumnTypeEmpty, Bram = VirtexE::eColumnTypeBram, 
		Clb = VirtexE::eColumnTypeClb, Clock = VirtexE::eColumnTypeClock, 
		Dsp = VirtexE::eColumnTypeDsp, Gtx = VirtexE::eColumnTypeGtx, 
		Iob = VirtexE::eColumnTypeIob, END = boost::uint32_t(-1) };

	// xcv50e static device information.
	boost::uint32_t xcv50eColumns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv50e(627, 19, 33, xcv50eColumns);

	// xcv100e static device information.
	boost::uint32_t xcv100eColumns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv100e(897, 23, 39, xcv100eColumns);

	// xcv200e static device information.
	boost::uint32_t xcv200eColumns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv200e(1581, 31, 51, xcv200eColumns);

	// xcv300e static device information.
	boost::uint32_t xcv300eColumns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv300e(1995, 35, 57, xcv300eColumns);

	// xcv400e static device information.
	boost::uint32_t xcv400eColumns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv400e(3139, 43, 73, xcv400eColumns);

	// xcv405e static device information.
	boost::uint32_t xcv405eColumns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Empty, Clb, Bram, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Empty, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Empty, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Empty, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Empty, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Bram, Clb, Empty, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv405e(3569, 43, 83, xcv405eColumns);

	// xcv600e static device information.
	boost::uint32_t xcv600eColumns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv600e(4437, 51, 87, xcv600eColumns);

	// xcv812e static device information.
	boost::uint32_t xcv812eColumns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Empty, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Empty, Clb, Bram, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Bram, Clb, Empty, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Empty, Bram, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Empty, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv812e(6667, 59, 113, xcv812eColumns);

	// xcv1000e static device information.
	boost::uint32_t xcv1000eColumns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv1000e(7437, 67, 111, xcv1000eColumns);

	// xcv1600e static device information.
	boost::uint32_t xcv1600eColumns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv1600e(9375, 75, 125, xcv1600eColumns);

	// xcv2000e static device information.
	boost::uint32_t xcv2000eColumns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv2000e(11371, 83, 137, xcv2000eColumns);

	// xcv2600e static device information.
	boost::uint32_t xcv2600eColumns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv2600e(15105, 95, 159, xcv2600eColumns);

	// xcv3200e static device information.
	boost::uint32_t xcv3200eColumns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv3200e(19367, 107, 181, xcv3200eColumns);

} // namespace bitstream
} // namespace torc
