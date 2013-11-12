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
/// \brief Source for the VerilogImporter class.

#include "torc/generic/verilog/VerilogImporter.hpp"
#include "torc/generic/verilog/VerilogImporterVisitor.hpp"
#include "torc/externals/verilator/src/V3Ast.h"
#include "torc/externals/verilator/src/V3File.h"
#include "torc/externals/verilator/src/V3Graph.h"
#include "torc/externals/verilator/src/V3LinkCells.h"
#include "torc/externals/verilator/src/V3Parse.h"
#include "torc/externals/verilator/src/V3PreShell.h"
#include <fstream>
#include <sstream>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

// debug
#include "torc/Generic.hpp"

/// \brief Global variables for Verilator.
/// \note The name v3Global is expected by Verilator code and therefore cannot be changed.
V3Global v3Global;

/// \brief Create a netlist for Verilator to use.
AstNetlist* V3Global::makeNetlist() {
    AstNetlist* newp = new AstNetlist();
    return newp;
}

/// \brief Check the AST for consistency.
void V3Global::checkTree() { 
	rootp()->checkTree(); 
}

/// \brief Delete the AST tree.
void V3Global::clear() {
    if (m_rootp) m_rootp->deleteTree(); m_rootp=NULL;
}

/// \brief Read all input files and libraries.
void V3Global::readFiles() {
	// set up the parser
    V3InFilter filter(v3Global.opt.pipeFilter());
    V3Parse parser(v3Global.rootp(), &filter);

    // read the top module
    for(V3StringList::const_iterator it = v3Global.opt.vFiles().begin(); 
		it != v3Global.opt.vFiles().end(); ++it) {
		string filename = *it;
		parser.parseFile(new FileLine("COMMAND_LINE",0), filename, false, 
			"Cannot find file containing module: ");
    }

    // read any libraries
    // "To be compatible with other simulators, this needs to be done after the top file is read"
    for(V3StringSet::const_iterator it = v3Global.opt.libraryFiles().begin(); 
		it != v3Global.opt.libraryFiles().end(); ++it) {
		string filename = *it;
		parser.parseFile(new FileLine("COMMAND_LINE",0), filename, true, 
			"Cannot find file containing library module: ");
    }
	// dump the AST parse tree if the user has requested it with --dump-tree
    v3Global.rootp()->dumpTreeFile(v3Global.debugFilename("parse.tree"));
	// fail on errors
    V3Error::abortIfErrors();

	// link instances to modules
    if(!v3Global.opt.preprocOnly()) V3LinkCells::link(v3Global.rootp(), &filter);
}

namespace torc {
namespace generic {

	using namespace std;
	using namespace torc;

	VerilogImporter::VerilogImporter(void) {
		// assume success unless we encounter an error
		mSuccess = true;
	}

	bool VerilogImporter::operator()(const boost::filesystem::path& inFilename, 
		const std::string& inArguments) {

		int argc = 0;
		char* env[] = {
			0
		};
		std::string arguments = 
			"./VerilogImporter " 
			+ inFilename.string()
			+ " --cc "
			+ "-Itorc/externals/verilator/include "
		//	+ "--dump-tree "
			;
		// combine the local arguments and the user supplied arguments
		std::string input = arguments + " " + inArguments;
		// tokenize just so we can count arguments (so much uglier than it should be ...)
		boost::char_separator<char> sep(" ");
		boost::tokenizer< boost::char_separator<char> > tokens(input, sep);
		typedef boost::token_iterator_generator< boost::char_separator<char> >::type Iterator;
		Iterator p = boost::make_token_iterator<string>(input.begin(), input.end(), sep);
		Iterator e = boost::make_token_iterator<string>(input.end(), input.end(), sep);
		while(p != e) { *p++; argc++; }
		// declare an array of sufficient size
		char* argv[argc];
		// put all of the arguments into the array
		argc = 0;
		BOOST_FOREACH(const string& t, tokens) { argv[argc++] = strdup(t.c_str()); }
		/// \todo Release the arguments in argv after use with free()

		// General initialization
		ios::sync_with_stdio();

		// enable the following if we ever allow command line -D definitions
		V3PreShell::boot(env);

		// parse command line options
		v3Global.opt.bin(argv[0]);
		string argString = V3Options::argString(argc-1, argv+1);
		v3Global.opt.parseOpts(new FileLine("COMMAND_LINE",0), argc-1, argv+1);
		if(!v3Global.opt.outFormatOk() && !v3Global.opt.preprocOnly() && !v3Global.opt.lintOnly() 
			&& !v3Global.opt.cdc()) {
			v3fatal("verilator: Need --cc, --sc, --sp, --cdc, --lint-only or --E option");
		}
		// release memory allocated for the command line arguments
		while(argc) { free(argv[--argc]); }

		// extract environment variables
		V3Options::getenvSYSTEMC();
		V3Options::getenvSYSTEMC_ARCH();
		V3Options::getenvSYSTEMC_INCLUDE();
		V3Options::getenvSYSTEMC_LIBDIR();
		V3Options::getenvSYSTEMPERL();
		V3Options::getenvSYSTEMPERL_INCLUDE();

		V3Error::abortIfErrors();

		V3File::addSrcDepend(v3Global.opt.bin());

		// Internal tests (after option parsing as need debug() setting)
		V3Graph::test();

		// Read first filename
		v3Global.readFiles();

		// restore C++ and C io to their default unsyncronized state
		ios::sync_with_stdio(false);

		// create a Generic object factory and root
		torc::generic::ObjectFactorySharedPtr objectFactoryPtr(new torc::generic::ObjectFactory());
		mRootPtr = objectFactoryPtr->newRootPtr("WORK");

		// visit the abstract syntax tree and extract the generic netlist representation
		torc::generic::VerilogImporterVisitor verilogImporterVisitor(objectFactoryPtr, mRootPtr);
		v3Global.rootp()->accept(verilogImporterVisitor);

		// export the EDIF design
		string outFileName = "output.edf";
		fstream edifExport(outFileName.c_str(), ios_base::out);
		torc::generic::EdifExporter exporter(edifExport);
		exporter(mRootPtr);

		return mSuccess;
	}

} // namespace generic
} // namespace torc
