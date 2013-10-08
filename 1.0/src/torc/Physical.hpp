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
/// \brief Main torc::physical namespace header.

/// \namespace torc
/// \brief Top-level Torc namespace.

/// \namespace torc::physical
/// \brief Namespace for the Torc physical netlist, including the XDL importer, exporter, placer, 
///		router, unpacker, and packer.

#ifndef TORC_PHYSICAL_HPP
#define TORC_PHYSICAL_HPP

#include "torc/physical/Circuit.hpp"
#include "torc/physical/Config.hpp"
#include "torc/physical/ConfigMap.hpp"
#include "torc/physical/Design.hpp"
#include "torc/physical/Factory.hpp"
#include "torc/physical/Instance.hpp"
#include "torc/physical/InstancePin.hpp"
#include "torc/physical/InstanceReference.hpp"
#include "torc/physical/Module.hpp"
#include "torc/physical/Named.hpp"
#include "torc/physical/Net.hpp"
#include "torc/physical/OutputStreamHelpers.hpp"
#include "torc/physical/Pip.hpp"
#include "torc/physical/Port.hpp"
#include "torc/physical/Progenitor.hpp"
#include "torc/physical/Progeny.hpp"
#include "torc/physical/Routethrough.hpp"
#include "torc/physical/TilewirePlaceholder.hpp"
#include "torc/physical/XdlExporter.hpp"
#include "torc/physical/XdlImporter.hpp"
#include "torc/physical/XilinxPhysicalTypes.hpp"

#endif // TORC_PHYSICAL_HPP
