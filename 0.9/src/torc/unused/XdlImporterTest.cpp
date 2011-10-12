// Gizmo - Copyright 2009 University of Southern California.  All Rights Reserved.
// $HeadURL$
// $Id$
#if 0
#include <iostream>
#include <fstream>
#include "torc/physical/XdlImporter.hpp"
#include "torc/physical/XdlExporter.hpp"

int xdlmain(int argc, char *argv[]);
int xdlmain(int argc, char *argv[])
{
    torc::physical::XdlImporter importer;
    bool readfile = false;
	
    for(int ai = 1; ai < argc; ++ai)
    {
		if (argv[ai] == std::string ("-p")) {
			importer.mTraceParsing = true;
		}
		else if (argv[ai] == std::string ("-s")) {
			importer.mTraceScanning = true;
		}
		else
		{
			// read a file with expressions
			
			std::fstream infile(argv[ai]);
			if (!infile.good())
			{
				std::cerr << "Could not open file: " << argv[ai] << std::endl;
				return 0;
			}
			
			bool result = importer(infile, argv[ai]);
			if (result)
			{
				std::cout << "Successfully parsed \"" << argv[ai] << "\"." << std::endl;
				torc::physical::XdlExporter exporter(std::cout);
				exporter(importer.getDesignPtr());
			}
			else
			{
				std::cout << "Failed to parse \"" << argv[ai] << "\"." << std::endl;
			}
			
			readfile = true;
		}
    }
	
    if (readfile) return 0;
    
    std::cout << "Reading expressions from stdin" << std::endl;
	
    std::string line;
    while( std::cout << "input: " &&
		  std::getline(std::cin, line) &&
		  !line.empty() )
    {
		bool result = importer(line, "input");
		
		if (result)
		{
			std::cout << "Successfully parsed stdin" << std::endl;
			return 0;
		}
	}

	return -1;
}
#endif