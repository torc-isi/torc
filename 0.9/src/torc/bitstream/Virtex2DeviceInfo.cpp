// Torc - Copyright 2010 University of Southern California.  All Rights Reserved.
// $HeadURL$
// $Id$

/// \file
/// \brief Static definition of the Virtex2 class.
/// \details This is a generated file.  Please modify with care.

#include "torc/bitstream/Virtex2.hpp"

namespace torc {
namespace bitstream {

	enum { Empty = Virtex2::eColumnTypeEmpty, Bram = Virtex2::eColumnTypeBram, 
		Clb = Virtex2::eColumnTypeClb, Clock = Virtex2::eColumnTypeClock, 
		Dsp = Virtex2::eColumnTypeDsp, Gtx = Virtex2::eColumnTypeGtx, 
		Iob = Virtex2::eColumnTypeIob, END = boost::uint32_t(-1) };

	// xc2v40 static device information.
	boost::uint32_t xc2v40Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clock, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2v40(255, 15, 17, xc2v40Columns);

	// xc2v80 static device information.
	boost::uint32_t xc2v80Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clock, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2v80(391, 23, 17, xc2v80Columns);

	// xc2v250 static device information.
	boost::uint32_t xc2v250Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clock, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2v250(899, 31, 29, xc2v250Columns);

	// xc2v500 static device information.
	boost::uint32_t xc2v500Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clock, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2v500(1591, 43, 37, xc2v500Columns);

	// xc2v1000 static device information.
	boost::uint32_t xc2v1000Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clock, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2v1000(2295, 51, 45, xc2v1000Columns);

	// xc2v1500 static device information.
	boost::uint32_t xc2v1500Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clock, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2v1500(3127, 59, 53, xc2v1500Columns);

	// xc2v2000 static device information.
	boost::uint32_t xc2v2000Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clock, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2v2000(4087, 67, 61, xc2v2000Columns);

	// xc2v3000 static device information.
	boost::uint32_t xc2v3000Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clock, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2v3000(5767, 79, 73, xc2v3000Columns);

	// xc2v4000 static device information.
	boost::uint32_t xc2v4000Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clock, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2v4000(8455, 95, 89, xc2v4000Columns);

	// xc2v6000 static device information.
	boost::uint32_t xc2v6000Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clock, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2v6000(12075, 115, 105, xc2v6000Columns);

	// xc2v8000 static device information.
	boost::uint32_t xc2v8000Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clock, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2v8000(15851, 131, 121, xc2v8000Columns);

} // namespace bitstream
} // namespace torc
