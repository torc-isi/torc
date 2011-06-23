// Torc - Copyright 2010 University of Southern California.  All Rights Reserved.
// $HeadURL$
// $Id$

/// \file
/// \brief Static definition of the Virtex2P class.
/// \details This is a generated file.  Please modify with care.

#include "torc/bitstream/Virtex2P.hpp"

namespace torc {
namespace bitstream {

	enum { Empty = Virtex2P::eColumnTypeEmpty, Bram = Virtex2P::eColumnTypeBram, 
		Clb = Virtex2P::eColumnTypeClb, Clock = Virtex2P::eColumnTypeClock, 
		Dsp = Virtex2P::eColumnTypeDsp, Gtx = Virtex2P::eColumnTypeGtx, 
		Iob = Virtex2P::eColumnTypeIob, END = boost::uint32_t(-1) };

	// xc2vp2 static device information.
	boost::uint32_t xc2vp2Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2vp2(805, 23, 35, xc2vp2Columns);

	// xc2vp4 static device information.
	boost::uint32_t xc2vp4Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2vp4(1820, 52, 35, xc2vp4Columns);

	// xc2vp7 static device information.
	boost::uint32_t xc2vp7Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2vp7(2652, 52, 51, xc2vp7Columns);

	// xc2vp20 static device information.
	boost::uint32_t xc2vp20Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2vp20(4556, 68, 67, xc2vp20Columns);

	// xc2vp30 static device information.
	boost::uint32_t xc2vp30Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2vp30(6432, 96, 67, xc2vp30Columns);

	// xc2vp40 static device information.
	boost::uint32_t xc2vp40Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2vp40(8632, 104, 83, xc2vp40Columns);

	// xc2vp50 static device information.
	boost::uint32_t xc2vp50Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2vp50(10296, 104, 99, xc2vp50Columns);

	// xc2vp70 static device information.
	boost::uint32_t xc2vp70Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2vp70(14260, 124, 115, xc2vp70Columns);

	// xc2vp100 static device information.
	boost::uint32_t xc2vp100Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2vp100(18340, 140, 131, xc2vp100Columns);

	// xc2vpx20 static device information.
	boost::uint32_t xc2vpx20Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2vpx20(4556, 68, 67, xc2vpx20Columns);

	// xc2vpx70 static device information.
	boost::uint32_t xc2vpx70Columns[] = { Iob, Gtx, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Bram, Empty, Clb, Clb, Gtx, Iob, END };
	DeviceInfo xc2vpx70(14260, 124, 115, xc2vpx70Columns);

} // namespace bitstream
} // namespace torc
