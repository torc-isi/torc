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
/// \brief Header for the Placement class.

#ifndef TORC_PLACER_PLACEMENTSITETYPEMAPPINGVIRTEX5_HPP
#define TORC_PLACER_PLACEMENTSITETYPEMAPPINGVIRTEX5_HPP

#include "torc/placer/PlacementSiteTypeMapping.hpp"

namespace torc {
namespace placer {

	/// \brief Placement mapping for legal instance types to site types.
	
	class PlacementSiteTypeMappingVirtex5 : public PlacementSiteTypeMapping {
		
	public:
		PlacementSiteTypeMappingVirtex5(DDB& inDB) : PlacementSiteTypeMapping(inDB) {
			// Don't want to call the parent constructor since it sets up the default mapping
		
			prepareStructures();
			
			// Record legal mappings for this family
			// Instance type to legal site type	
			
			// ONE TO ONE SITE TYPES
			addMapData("BSCAN", "BSCAN");
			addMapData("BUFDS", "BUFDS");
			addMapData("BUFIO", "BUFIO");
			addMapData("BUFR", "BUFR");
			addMapData("CAPTURE", "CAPTURE");
			addMapData("CRC32", "CRC32");
			addMapData("CRC64", "CRC64");
			addMapData("DCI", "DCI");
			addMapData("DCIRESET", "DCIRESET");
			addMapData("DCM_ADV", "DCM_ADV");
			addMapData("DSP48E", "DSP48E");
			addMapData("EFUSE_USR", "EFUSE_USR");
			addMapData("FRAME_ECC", "FRAME_ECC");
			addMapData("GLOBALSIG", "GLOBALSIG");
			addMapData("GTP_DUAL", "GTP_DUAL");
			addMapData("GTX_DUAL", "GTX_DUAL");
			addMapData("ICAP", "ICAP");
			addMapData("IDELAYCTRL", "IDELAYCTRL");
			addMapData("JTAGPPC", "JTAGPPC");
			addMapData("KEY_CLEAR", "KEY_CLEAR");
			addMapData("PCIE", "PCIE");
			addMapData("PLL_ADV", "PLL_ADV");
			addMapData("PMV", "PMV");
			addMapData("PMVBRAM", "PMVBRAM");
			addMapData("PPC440", "PPC440");
			addMapData("STARTUP", "STARTUP");
			addMapData("SYSMON", "SYSMON");
			addMapData("TEMAC", "TEMAC");
			addMapData("TIEOFF", "TIEOFF");
			addMapData("USR_ACCESS", "USR_ACCESS");
			
			// BUFG
			addMapData("BUFG", "BUFGCTRL");
			addMapData("BUFGCTRL", "BUFGCTRL");
			
			// IO LOGIC SITES
			addMapData("ILOGIC", "ILOGIC");
			addMapData("ISERDES", "ILOGIC");
			addMapData("IODELAY", "IODELAY");
			addMapData("OLOGIC", "OLOGIC");
			addMapData("OSERDES", "OLOGIC");

			// IO PADS			
			addMapData("OPAD", "OPAD");
			addMapData("IOBM", "IOBM");
			addMapData("IOB", "IOBM");
			addMapData("IPAD", "IOBM");
			addMapData("IOBS", "IOBS");
			addMapData("IOB", "IOBS");
			addMapData("IPAD", "IOBS");
			addMapData("IPAD", "IPAD");
			
			// BRAM SITES
			addMapData("FIFO36_72_EXP", "RAMBFIFO36");
			addMapData("FIFO36_EXP", "RAMBFIFO36");
			addMapData("RAMB18X2", "RAMBFIFO36");
			addMapData("RAMB18X2SDP", "RAMBFIFO36");
			addMapData("RAMB36SDP_EXP", "RAMBFIFO36");
			addMapData("RAMB36_EXP", "RAMBFIFO36");
			addMapData("RAMBFIFO18", "RAMBFIFO36");
			addMapData("RAMBFIFO18_36", "RAMBFIFO36");
			addMapData("RAMBFIFO36", "RAMBFIFO36");

			// SLICE SITES
			addMapData("SLICEL", "SLICEL");
			addMapData("SLICEL", "SLICEM");
			addMapData("SLICEM", "SLICEM");
			
		}
		~PlacementSiteTypeMappingVirtex5() {}
	
	}; // class PlacementSiteTypeMappingVirtex5
} // namespace placer
} // namespace torc

#endif // TORC_PLACER_PLACEMENTSITETYPEMAPPINGVIRTEX5_HPP
