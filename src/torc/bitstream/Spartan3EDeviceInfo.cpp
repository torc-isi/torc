// Torc - Copyright 2010 University of Southern California.  All Rights Reserved.
// $HeadURL$
// $Id$

/// \file
/// \brief Static definition of the Spartan3E class.
/// \details This is a generated file.  Please modify with care.

#include "torc/bitstream/Spartan3E.hpp"

namespace torc {
namespace bitstream {

	enum { Empty = Spartan3E::eColumnTypeEmpty, Bram = Spartan3E::eColumnTypeBram, 
		BramInt = Spartan3E::eColumnTypeBramInt, Clb = Spartan3E::eColumnTypeClb,
		Clock = Spartan3E::eColumnTypeClock, ClockLL = Spartan3E::eColumnTypeClockLL,
		Term = Spartan3E::eColumnTypeTerm, Iob = Spartan3E::eColumnTypeIob,
		END = boost::uint32_t(-1) };

	// xc3s100e static device information.
	boost::uint32_t xc3s100eColumns[] = { Term, Iob, Clb, Clb, Empty, BramInt, Bram, Empty, Empty, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Iob, Term, END };
	DeviceInfo xc3s100e(667, 29, 23, xc3s100eColumns);

	// xc3s250e static device information.
	boost::uint32_t xc3s250eColumns[] = { Term, Iob, Clb, Clb, BramInt, Bram, Empty, Empty, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, BramInt, Bram, Empty, Empty, Clb, Clb, Iob, Term, END };
	DeviceInfo xc3s250e(1419, 43, 33, xc3s250eColumns);

	// xc3s500e static device information.
	boost::uint32_t xc3s500eColumns[] = { Term, Iob, Clb, Clb, BramInt, Bram, Empty, Empty, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clock, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, BramInt, Bram, Empty, Empty, Clb, Clb, Iob, Term, END };
	DeviceInfo xc3s500e(2255, 55, 41, xc3s500eColumns);

	// xc3s1200e static device information.
	boost::uint32_t xc3s1200eColumns[] = { Term, Iob, Clb, Clb, BramInt, Bram, Empty, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, ClockLL, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, BramInt, Bram, Empty, Empty, Clb, Clb, Iob, Term, END };
	DeviceInfo xc3s1200e(3710, 70, 53, xc3s1200eColumns);

	// xc3s1600e static device information.
	boost::uint32_t xc3s1600eColumns[] = { Term, Iob, Clb, Clb, BramInt, Bram, Empty, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, ClockLL, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Empty, Clb, Clb, Clb, Clb, Clb, Clb, Clb, Clb, BramInt, Bram, Empty, Empty, Clb, Clb, Iob, Term, END };
	DeviceInfo xc3s1600e(5720, 88, 65, xc3s1600eColumns);

} // namespace bitstream
} // namespace torc
