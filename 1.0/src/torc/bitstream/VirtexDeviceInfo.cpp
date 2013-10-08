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
/// \brief Static definition of the Virtex class.
/// \details This is a generated file.  Please modify with care.

#include "torc/bitstream/Virtex.hpp"

namespace torc {
namespace bitstream {

	enum { Empty = Virtex::eColumnTypeEmpty, Bram = Virtex::eColumnTypeBram, 
		Clb = Virtex::eColumnTypeClb, Clock = Virtex::eColumnTypeClock, 
		Dsp = Virtex::eColumnTypeDsp, Gtx = Virtex::eColumnTypeGtx, 
		Iob = Virtex::eColumnTypeIob, END = boost::uint32_t(-1) };

	// xcv50 static device information.
	boost::uint32_t xcv50Columns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv50(589, 19, 31, xcv50Columns);

	// xcv100 static device information.
	boost::uint32_t xcv100Columns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv100(851, 23, 37, xcv100Columns);

	// xcv150 static device information.
	boost::uint32_t xcv150Columns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv150(1161, 27, 43, xcv150Columns);

	// xcv200 static device information.
	boost::uint32_t xcv200Columns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv200(1519, 31, 49, xcv200Columns);

	// xcv300 static device information.
	boost::uint32_t xcv300Columns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv300(1925, 35, 55, xcv300Columns);

	// xcv400 static device information.
	boost::uint32_t xcv400Columns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv400(3053, 43, 71, xcv400Columns);

	// xcv600 static device information.
	boost::uint32_t xcv600Columns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv600(4233, 51, 83, xcv600Columns);

	// xcv800 static device information.
	boost::uint32_t xcv800Columns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv800(5605, 59, 95, xcv800Columns);

	// xcv1000 static device information.
	boost::uint32_t xcv1000Columns[] = { Iob, Bram, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Iob, END };
	DeviceInfo xcv1000(7169, 67, 107, xcv1000Columns);

} // namespace bitstream
} // namespace torc
