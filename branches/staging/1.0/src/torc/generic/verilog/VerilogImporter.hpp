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
/// \brief Header for the VerilogImporter class.

#ifndef TORC_GENERIC_VERILOG_VERILOGIMPORTER_HPP
#define TORC_GENERIC_VERILOG_VERILOGIMPORTER_HPP

// The foundation of this code comes from Wilson Snyder's Verilator available at 
// http://www.veripool.org/wiki/verilator.

#include "torc/generic/Root.hpp"
#include "torc/externals/verilator/src/V3Global.h"
#include <boost/smart_ptr.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>

namespace torc {
namespace generic {

namespace generic { class VerilogImporterUnitTest; }

	/// \brief Importer from structural verilog format into a generic design.
	class VerilogImporter {
	protected:
	// friends
		/// \brief The unit test class has access to our internals.
		friend class torc::generic::generic::VerilogImporterUnitTest;
	// types
		/// \brief Imported type name.
		typedef std::string string;
	// members
		/// \brief Name of file or input stream for error messages.
		string mStreamName;
		// error flags
		/// \brief Flag signaling parsing success.
		bool mSuccess;
		/// \brief Root of generic netlist.
		RootSharedPtr mRootPtr;
	public:
	// constructors
		/// \brief Construct the parser importer context.
		VerilogImporter(void);
	// operators
		/// \brief Import Verilog from a file.
		/// \param inFilename Input file name.
		/// \param inArguments Additional Verilator command line arguments.
		/// \returns true if successfully parsed.
		bool operator()(const boost::filesystem::path& inFilename, 
			const std::string& inArguments = std::string());
	// functions
		/// \brief Signals a parsing failure by deasserting the success flag.
		void failure(void) { mSuccess = false; }
	// accessors
		/// \brief Returns a shared pointer for the root.
		/// \details This root is created and populated during verilog import process.
		RootSharedPtr getRootPtr(void) { return mRootPtr; }
	};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_VERILOG_VERILOGIMPORTER_HPP
