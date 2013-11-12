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
/// \brief Source for the VerilogNames class.

#include "torc/generic/verilog/VerilogNames.hpp"

namespace torc {
namespace generic {

	std::string VerilogNames::sInferredBlackBoxesLibraryName = "TorcInferredBlackBoxes";
	std::string VerilogNames::sImportedCellLibraryName = "TorcImportedCellLibrary";
	std::string VerilogNames::sImportedVerilogLibraryName = "ImportedVerilog";
	std::string VerilogNames::sImportedVerilogViewName = "Verilog";
	std::string VerilogNames::sTorcRangeMSBPropertyName = "torc_range_msb";
	std::string VerilogNames::sTorcRangeLSBPropertyName = "torc_range_lsb";
	std::string VerilogNames::sTorcAssignRHSPropertyName = "torc_assign_rhs";

} // namespace generic
} // namespace torc
