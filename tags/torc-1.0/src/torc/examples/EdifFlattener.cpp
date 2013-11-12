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
/// \brief Source for the EDIF flattener.

#include "torc/Generic.hpp"
#include <fstream>
#include <boost/filesystem.hpp>

using namespace torc::generic;

int main(int argc, char** argv) {

	// iterate through each EDIF file specified
	for(int i = 1; i < argc; i++) {

		// build the file names
		boost::filesystem::path sourcePath = argv[i];
		boost::filesystem::path targetPath = sourcePath;
		targetPath = targetPath.replace_extension().string() 
			+ ".flat" + boost::filesystem::extension(sourcePath);

		try {
			// read the source file
			std::cout << "Reading source: " << sourcePath.string() << " ...  ";
			std::cout.flush();
			boost::shared_ptr<ObjectFactory> factoryPtr(new ObjectFactory());
			boost::shared_ptr<Root> rootPtr;
			factoryPtr->create(rootPtr);
			boost::shared_ptr<Linker> linkerPtr(new Linker(rootPtr));
			ParserOptions options;
			EdifParser parser;
			try {
				parser.parse(sourcePath.string(), rootPtr, linkerPtr, factoryPtr, options);
				std::cout << "done." << std::endl;
			} catch(Error& e) {
				std::cerr << std::endl;
				std::cerr << MessageTable::instance()->getMessage(e.getErrorMessageId()) 
					<< std::endl;
				const std::vector<Error::StackFrameInfo> &stack = e.getStackTrace();
				for(std::vector<Error::StackFrameInfo>::const_iterator it = stack.begin(); 
					it != stack.end(); it++) {
					std::cerr << "    " << (*it).getFunction() << "() [" << (*it).getFile() << ":" 
						<< (*it).getLine() << "]" << std::endl;
				}
			}

			// flatten all designs that are present
			typedef std::vector<DesignSharedPtr> DesignSharedPtrVector;
			DesignSharedPtrVector designs;
			rootPtr->getDesigns(designs);
			DesignSharedPtrVector::iterator dp = designs.begin();
			DesignSharedPtrVector::iterator de = designs.end();
			while(dp < de) {
				// look up the design name and specifics
				DesignSharedPtr designPtr = *dp++;
				std::string designName = designPtr->getName();
				std::cout << "    Flattening design " << designName  << " ...  ";
				std::cout.flush();
				flatten(designPtr, factoryPtr, true);
				std::cout << "done." << std::endl;
			}

			// prune unused cells and libraries
			prune(rootPtr);

			// write the target file
			std::cout << "    Writing target: " << targetPath.string() << " ...  ";
			std::cout.flush();
			std::fstream targetStream(targetPath.string().c_str(), std::ios_base::out);
			Decompiler decompiler(rootPtr, targetStream);
			decompiler();
			std::cout << "done." << std::endl;
		}
		// report failure
		catch(...) {
			std::cerr << "ERROR: Failed to flatten " << sourcePath.string() << std::endl;
		}
	}

	return 0;
}
