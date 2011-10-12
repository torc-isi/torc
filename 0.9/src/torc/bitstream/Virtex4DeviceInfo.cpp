// Torc - Copyright 2010 University of Southern California.  All Rights Reserved.
// $HeadURL$
// $Id$

/// \file
/// \brief Static definition of the Virtex4 class.
/// \details This is a generated file.  Please modify with care.

#include "torc/bitstream/Virtex4.hpp"

namespace torc {
namespace bitstream {

	enum { Empty = Virtex4::eColumnTypeEmpty, Bram = Virtex4::eColumnTypeBram, 
		Clb = Virtex4::eColumnTypeClb, Clock = Virtex4::eColumnTypeClock, 
		Dsp = Virtex4::eColumnTypeDsp, Gtx = Virtex4::eColumnTypeGtx, 
		Iob = Virtex4::eColumnTypeIob, END = boost::uint32_t(-1) };

	// xc4vfx12 static device information.
	boost::uint32_t xc4vfx12Columns[] = { Empty, Empty, Iob, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, END };
	DeviceInfo xc4vfx12(4818, 73, 66, xc4vfx12Columns);

	// xc4vfx20 static device information.
	boost::uint32_t xc4vfx20Columns[] = { Gtx, Empty, Empty, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Gtx, END };
	DeviceInfo xc4vfx20(7227, 73, 99, xc4vfx20Columns);

	// xc4vfx40 static device information.
	boost::uint32_t xc4vfx40Columns[] = { Gtx, Empty, Empty, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Gtx, END };
	DeviceInfo xc4vfx40(14933, 109, 137, xc4vfx40Columns);

	// xc4vfx60 static device information.
	boost::uint32_t xc4vfx60Columns[] = { Gtx, Empty, Empty, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Gtx, END };
	DeviceInfo xc4vfx60(20445, 145, 141, xc4vfx60Columns);

	// xc4vfx100 static device information.
	boost::uint32_t xc4vfx100Columns[] = { Gtx, Empty, Empty, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Gtx, END };
	DeviceInfo xc4vfx100(32037, 181, 177, xc4vfx100Columns);

	// xc4vfx140 static device information.
	boost::uint32_t xc4vfx140Columns[] = { Gtx, Empty, Empty, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Gtx, END };
	DeviceInfo xc4vfx140(46655, 217, 215, xc4vfx140Columns);

	// xc4vlx15 static device information.
	boost::uint32_t xc4vlx15Columns[] = { Empty, Empty, Iob, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, END };
	DeviceInfo xc4vlx15(4818, 73, 66, xc4vlx15Columns);

	// xc4vlx25 static device information.
	boost::uint32_t xc4vlx25Columns[] = { Empty, Empty, Iob, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, END };
	DeviceInfo xc4vlx25(8066, 109, 74, xc4vlx25Columns);

	// xc4vlx40 static device information.
	boost::uint32_t xc4vlx40Columns[] = { Empty, Empty, Iob, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, END };
	DeviceInfo xc4vlx40(13050, 145, 90, xc4vlx40Columns);

	// xc4vlx60 static device information.
	boost::uint32_t xc4vlx60Columns[] = { Empty, Empty, Iob, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, END };
	DeviceInfo xc4vlx60(18560, 145, 128, xc4vlx60Columns);

	// xc4vlx80 static device information.
	boost::uint32_t xc4vlx80Columns[] = { Empty, Empty, Iob, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, END };
	DeviceInfo xc4vlx80(24616, 181, 136, xc4vlx80Columns);

	// xc4vlx100 static device information.
	boost::uint32_t xc4vlx100Columns[] = { Empty, Empty, Iob, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, END };
	DeviceInfo xc4vlx100(32984, 217, 152, xc4vlx100Columns);

	// xc4vlx160 static device information.
	boost::uint32_t xc4vlx160Columns[] = { Empty, Empty, Iob, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, END };
	DeviceInfo xc4vlx160(44268, 217, 204, xc4vlx160Columns);

	// xc4vlx200 static device information.
	boost::uint32_t xc4vlx200Columns[] = { Empty, Empty, Iob, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, END };
	DeviceInfo xc4vlx200(56854, 217, 262, xc4vlx200Columns);

	// xc4vsx25 static device information.
	boost::uint32_t xc4vsx25Columns[] = { Empty, Empty, Iob, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, END };
	DeviceInfo xc4vsx25(8468, 73, 116, xc4vsx25Columns);

	// xc4vsx35 static device information.
	boost::uint32_t xc4vsx35Columns[] = { Empty, Empty, Iob, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, END };
	DeviceInfo xc4vsx35(12644, 109, 116, xc4vsx35Columns);

	// xc4vsx55 static device information.
	boost::uint32_t xc4vsx55Columns[] = { Empty, Empty, Iob, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Clock, Empty, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Dsp, Empty, Clb, Empty, Clb, Empty, Bram, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Clb, Empty, Iob, Empty, END };
	DeviceInfo xc4vsx55(20880, 145, 144, xc4vsx55Columns);

} // namespace bitstream
} // namespace torc
