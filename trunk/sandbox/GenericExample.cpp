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
/// \brief Example program to read in an EDIF file, do something with it, and write it back out.

#include "torc/Generic.hpp"
#include "torc/Common.hpp"
#include <fstream>
#include <boost/regex.hpp>

using namespace std;
using namespace torc::generic;

int main(int argc, char* argv[]) {

	// build the file paths
	(void) argc;
	torc::common::DirectoryTree directoryTree(argv[0]);
	boost::filesystem::path referencePath = torc::common::DirectoryTree::getExecutablePath() 
		/ "regression" / "GenericExample.reference.edf";
	boost::filesystem::path generatedPath = torc::common::DirectoryTree::getExecutablePath() 
		/ "regression" / "GenericExample.generated.edf";

	// import the EDIF design
	string inFileName = referencePath.string();
	fstream fileStream(inFileName.c_str());
	ObjectFactorySharedPtr factoryPtr(new ObjectFactory());
	EdifImporter importer(factoryPtr);
	importer(fileStream, inFileName);

	// look up an instance of interest
	RootSharedPtr rootPtr = importer.getRootPtr();
	InstanceSharedPtr instancePtr = rootPtr->findLibrary("work")->findCell("and")
		->findView("verilog")->findInstance("oZ0");

	// change the INIT property (LUT mask) to XOR
	PropertySharedPtr initPropertyPtr = instancePtr->getProperty("INIT");
	string originalMask = initPropertyPtr->getValue().get<Value::String>();
	std::cout << "The original LUT mask was \"" << originalMask << "\"." << std::endl;
	Value xorMask(Value::eValueTypeString, string("6"));
	initPropertyPtr->setValue(xorMask);

	// export the EDIF design
	string outFileName = generatedPath.string();
	fstream edifExport(outFileName.c_str(), ios_base::out);
	EdifExporter exporter(edifExport);
	exporter(rootPtr);

	return 0;
}
