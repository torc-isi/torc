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
/// \brief Header for torc::physical output stream helpers.
/// \details The coverage and support of classes is quite limited at present, but may be expanded 
///		upon request.

#ifndef TORC_PHYSICAL_OUTPUT_STREAM_HELPERS_HPP
#define TORC_PHYSICAL_OUTPUT_STREAM_HELPERS_HPP

#include <iosfwd>

namespace torc {
namespace physical {

	class Design;
	class Module;
	class Instance;
	class Net;
	class InstancePin;
	class Pip;
	class Config;

	/// \brief Design stream inserter.
	std::ostream& operator <<(std::ostream& os, const Design& rhs);
	/// \brief Module stream inserter.
	std::ostream& operator <<(std::ostream& os, const Module& rhs);
	/// \brief Instance stream inserter.
	std::ostream& operator <<(std::ostream& os, const Instance& rhs);
	/// \brief Net stream inserter.
	std::ostream& operator <<(std::ostream& os, const Net& rhs);
	/// \brief InstancePin stream inserter.
	std::ostream& operator <<(std::ostream& os, const InstancePin& rhs);
	/// \brief Pip stream inserter.
	std::ostream& operator <<(std::ostream& os, const Pip& rhs);
	/// \brief Config iterator stream inserter
	std::ostream& operator <<(std::ostream& os, const Config& rhs);

} // namespace physical
} // namespace torc

#endif // TORC_PHYSICAL_OUTPUT_STREAM_HELPERS_HPP
