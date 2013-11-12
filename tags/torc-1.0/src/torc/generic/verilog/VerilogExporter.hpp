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
/// \brief Header for the VerilogExporter class.

#ifndef TORC_GENERIC_VERILOG_VERILOGEXPORTER_HPP
#define TORC_GENERIC_VERILOG_VERILOGEXPORTER_HPP

#include "torc/generic/Root.hpp"
#include <boost/filesystem.hpp>
#include <string>
#include <vector>

namespace torc {
namespace generic {

namespace generic { class VerilogExporterUnitTest; }

	/// \brief Exporter from a generic netlist into structural verilog.
	class VerilogExporter {
	protected:
	// friends
		/// \brief The unit test class has access to our internals.
		friend class torc::generic::generic::VerilogExporterUnitTest;
	// types
		/// \brief Imported type name.
		typedef std::string string;
	// members
		/// \brief Root of the generic netlist.
		RootSharedPtr mRootPtr;
	public:
	// constructors
		/// \brief Public constructor.
		VerilogExporter(RootSharedPtr inRootPtr);
	// operators
		/// \brief Export Verilog to a file.
		/// \param inFilename Output file name.
		/// \returns true if successfully exported.
		bool operator()(const boost::filesystem::path& inFilename);
		/// \brief Export Verilog to an output stream.
		/// \param inOutputStream.
		/// \returns true if successfully exported.
		bool operator()(std::ostream& inOutputStream = std::cout);
	};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_VERILOG_VERILOGEXPORTER_HPP
