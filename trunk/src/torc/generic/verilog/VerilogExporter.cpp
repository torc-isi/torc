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
/// \brief Source for the VerilogExporter class.

#include "torc/generic/verilog/VerilogExporter.hpp"
#include "torc/generic/verilog/VerilogExporterVisitor.hpp"
#include "torc/Generic.hpp"
#include <fstream>
#include <sstream>

namespace torc {
namespace generic {

	using namespace std;
	using namespace torc;

	VerilogExporter::VerilogExporter(RootSharedPtr inRootPtr) : mRootPtr(inRootPtr) {}

	bool VerilogExporter::operator()(const boost::filesystem::path& path) {

		// prepare the output stream
		string outFileName = path.string();
		fstream verilogExport(outFileName.c_str(), ios_base::out);
		return (*this)(verilogExport);
	}

	bool VerilogExporter::operator()(std::ostream& inOutputStream) {

		// export the Verilog code
		torc::generic::VerilogExporterVisitor verilogExporterVisitor(mRootPtr, inOutputStream);
		try {
			mRootPtr->accept(verilogExporterVisitor);
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}

		return true; //mSuccess;
	}

} // namespace generic
} // namespace torc
