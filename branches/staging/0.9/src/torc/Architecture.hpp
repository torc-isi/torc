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
/// \brief Main torc::architecture namespace header.

/// \namespace torc::architecture
/// \brief Namespace for the Torc architecture support, providing device logic and wiring 
///		information, and design usage tracking.

/// \namespace torc::architecture::xilinx
/// \brief Namespace for Xilinx architecture support.
/// \details This namespace defines types suitable for Xilinx architectures.  These types may or 
///		may not resemble those for other architectures.

#ifndef TORC_ARCHITECTURE_HPP
#define TORC_ARCHITECTURE_HPP

#include "torc/architecture/Arc.hpp"
#include "torc/architecture/ArcUsage.hpp"
#include "torc/architecture/Array.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/architecture/DDBStreamHelper.hpp"
#include "torc/common/DeviceDesignator.hpp"
#include "torc/architecture/DigestStream.hpp"
#include "torc/architecture/ExtendedWireInfo.hpp"
#include "torc/architecture/InstancePin.hpp"
#include "torc/architecture/OutputStreamHelpers.hpp"
#include "torc/architecture/Segments.hpp"
#include "torc/architecture/Sites.hpp"
#include "torc/architecture/TileInfo.hpp"
#include "torc/architecture/Tiles.hpp"
#include "torc/architecture/Tilewire.hpp"
#include "torc/architecture/Versions.hpp"
#include "torc/architecture/WireInfo.hpp"
#include "torc/architecture/WireUsage.hpp"
#include "torc/architecture/XdlImporter.hpp"
#include "torc/architecture/XilinxDatabaseTypes.hpp"

#endif // TORC_ARCHITECTURE_HPP
