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
/// \brief Header for the VerilogNames class.

#ifndef TORC_GENERIC_VERILOG_VERILOGNAMES_HPP
#define TORC_GENERIC_VERILOG_VERILOGNAMES_HPP

#include <string>

namespace torc {
namespace generic {

namespace generic { class VerilogNamesUnitTest; }

	/// \brief Encapsulation of library names used for Verilog importing and exporting.
	class VerilogNames {
	protected:
	// friends
		/// \brief The unit test class has access to our internals.
		friend class torc::generic::generic::VerilogNamesUnitTest;
	// types
		/// \brief Imported type name.
		typedef std::string string;
	// static members
		/// \brief Library name for inferred black boxes.
		static string sInferredBlackBoxesLibraryName;
		/// \brief Library name for verilog library cells.
		static string sImportedCellLibraryName;
		/// \brief Library name for imported verilog.
		static string sImportedVerilogLibraryName;
		/// \brief View name for imported verilog.
		static string sImportedVerilogViewName;
		/// \brief Property name for the vector range MSB.
		static string sTorcRangeMSBPropertyName;
		/// \brief Property name for the vector range LSB.
		static string sTorcRangeLSBPropertyName;
		/// \brief Property name for the wire assignment right-hand-side.
		static string sTorcAssignRHSPropertyName;
	public:
	// constructors
		/// \brief Protected constructor.
		VerilogNames(void);
	// accessors
		/// \brief Returns the inferred black box library name.
		static string getInferredBlackBoxesLibraryName(void) 
			{ return sInferredBlackBoxesLibraryName; }
		/// \brief Returns the imported cell library name.
		static string getImportedCellLibraryName(void) { return sImportedCellLibraryName; }
		/// \brief Returns the imported Verilog library name.
		static string getImportedVerilogLibraryName(void) { return sImportedVerilogLibraryName; }
		/// \brief Returns the imported Verilog view name.
		static string getImportedVerilogViewName(void) { return sImportedVerilogViewName; }
		/// \brief Property name for the vector range MSB.
		static string getTorcRangeMSBPropertyName(void) { return sTorcRangeMSBPropertyName; }
		/// \brief Property name for the vector range LSB.
		static string getTorcRangeLSBPropertyName(void) { return sTorcRangeLSBPropertyName; }
		/// \brief Property name for the wire assignment right-hand-side.
		static string getTorcAssignRHSPropertyName(void) { return sTorcAssignRHSPropertyName; }
	};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_VERILOG_VERILOGNAMES_HPP
