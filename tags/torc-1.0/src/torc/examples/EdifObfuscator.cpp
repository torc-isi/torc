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
/// \brief Source for the EDIF obfuscator.

#include "torc/Generic.hpp"
#include "torc/Common.hpp"
#include "torc/externals/md5/md5.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>

using namespace torc::generic;

boost::uint32_t mIdentifierCount = 0;

// function prototypes
std::string md5(const char* inMessage);
std::string obfuscate(const std::string& inIdentifier);
void obfuscate(RootSharedPtr rootPtr, std::fstream& log);

// calculate and return the MD5 hash on the given message
std::string md5(const char* inMessage) {
	// prepare for hashing
	md5_state_t md5_state;
	md5_byte_t digest[16];
	md5_init(&md5_state);
	// hash the input
	md5_append(&md5_state, (md5_byte_t*) inMessage, strlen(inMessage));
	md5_finish(&md5_state, digest);
	// return the hash as a hex string
	std::ostringstream md5oss;
	md5oss << 'i';
	for(boost::uint16_t i = 0; i < sizeof(digest); i++) md5oss << std::hex << std::setw(2) 
		<< std::setfill('0') << (boost::uint16_t) (digest[i] & 0xff) << std::dec;
	return md5oss.str();
}

// obfuscate the given identifier and return it
std::string obfuscate(const std::string& inIdentifier) {
	// we don't actually need to hash the original string; we'll just hash a unique identifier 
	// count instead
	std::string message = boost::lexical_cast<std::string>(mIdentifierCount++);
	// hash the message
	std::string hash = md5(message.c_str());
	// return the result
	return hash;
}

void obfuscate(RootSharedPtr rootPtr, std::fstream& log) {

	// reset the md5 hashing scheme
	mIdentifierCount = 0;

	// track libraries and cells that should be protected from obfuscation
	typedef std::map<std::string, bool> StringToBoolMap;
	StringToBoolMap protectedCells;
	StringToBoolMap protectedLibraries;

	// enumerate the root designs
	typedef std::vector<DesignSharedPtr> DesignSharedPtrVector;
	DesignSharedPtrVector designs;
	rootPtr->getDesigns(designs);
	DesignSharedPtrVector::iterator dp = designs.begin();
	DesignSharedPtrVector::iterator de = designs.end();
	while(dp < de) {

		// look up the design name and specifics
		DesignSharedPtr& designPtr = *dp++;
		std::string designName = designPtr->getName();
		std::string cellRefName = designPtr->getCellRefName();
		std::string libraryRefName = designPtr->getLibraryRefName();
		std::cout << "    Protecting design \"" << designName << "\" (cell \"" << cellRefName 
			<< "\", library \"" << libraryRefName << "\")" << std::endl;
		protectedCells[libraryRefName + ":" + cellRefName] = true;
		protectedLibraries[libraryRefName] = true;

	}

	// define libraries that must not be obfuscated
	boost::regex regexVendorLibraries("^(VhdlGenLib|virtex|virtexr|unilib)$", 
		boost::regex_constants::icase);

	// iterate over the root libraries
	typedef std::vector<LibrarySharedPtr> LibrarySharedPtrVector;
	LibrarySharedPtrVector libraries;
	rootPtr->getLibraries(libraries);
	LibrarySharedPtrVector::iterator lp = libraries.begin();
	LibrarySharedPtrVector::iterator le = libraries.end();
	while(lp < le) {

		// look up the library name
		LibrarySharedPtr& libraryPtr = *lp++;
		std::string libraryName = libraryPtr->getName();
		//std::cout << "    found library \"" << libraryName << "\"" << std::endl;

		// leave vendor libraries alone
		if(boost::regex_search(libraryName, regexVendorLibraries)) continue;

		// identify and protect any black boxes in the library
		typedef std::vector<CellSharedPtr> CellSharedPtrVector;
		CellSharedPtrVector cells;
		libraryPtr->getCells(cells);
		CellSharedPtrVector::iterator cp = cells.begin();
		CellSharedPtrVector::iterator ce = cells.end();
		while(cp < ce) {
			// look up the cell name
			CellSharedPtr& cellPtr = *cp++;
			std::string cellName = cellPtr->getName();
			// iterate over the cell modules
			typedef std::vector<ViewSharedPtr> ViewSharedPtrVector;
			ViewSharedPtrVector views;
			cellPtr->getViews(views);
			ViewSharedPtrVector::iterator vp = views.begin();
			ViewSharedPtrVector::iterator ve = views.end();
			while(vp < ve) {
				// look up the view name
				ViewSharedPtr& viewPtr = *vp++;
				std::string viewName = viewPtr->getName();
				if(viewName == "syn_black_box") {
					protectedLibraries[libraryName] = true;
					protectedCells[libraryName + ":" + cellName] = true;
				}
			}
		}

		// obfuscate the name
		bool libraryIsProtected = protectedLibraries.count(libraryName) > 0;
		if(!libraryIsProtected) {
			std::string obfuscatedLibraryName 
				= libraryIsProtected ? libraryName : obfuscate(libraryName);
			libraryPtr->setName(obfuscatedLibraryName);
			libraryPtr->setOriginalName("");
			log << "library " << libraryName << "=> " << obfuscatedLibraryName << std::endl;
		}

		// iterate over the library cells
		//typedef std::vector<CellSharedPtr> CellSharedPtrVector;
		//CellSharedPtrVector cells;
		//libraryPtr->getCells(cells);
		//CellSharedPtrVector::iterator cp = cells.begin();
		//CellSharedPtrVector::iterator ce = cells.end();
		cp = cells.begin();
		ce = cells.end();
		while(cp < ce) {

			// look up the cell name
			CellSharedPtr& cellPtr = *cp++;
			std::string cellName = cellPtr->getName();
			//std::cout << "        found cell \"" << cellName << "\"" << std::endl;

			// obfuscate the name
			bool cellIsProtected = protectedCells.count(libraryName + ":" + cellName) > 0;
			if(cellIsProtected == false) {
				std::string obfuscatedCellName = obfuscate(cellName);
				cellPtr->setName(obfuscatedCellName);
				cellPtr->setOriginalName("");
				log << "cell " << cellName << " => " << obfuscatedCellName << std::endl;
			} else {
				log << "cell " << cellName << " => " << cellName << std::endl;
			}

			// iterate over the cell modules
			typedef std::vector<ViewSharedPtr> ViewSharedPtrVector;
			ViewSharedPtrVector views;
			cellPtr->getViews(views);
			ViewSharedPtrVector::iterator vp = views.begin();
			ViewSharedPtrVector::iterator ve = views.end();
			while(vp < ve) {

				// look up the view name
				ViewSharedPtr& viewPtr = *vp++;
				std::string viewName = viewPtr->getName();
				//std::cout << "            found view \"" << viewName << "\"" << std::endl;

				// obfuscate the name
				if(cellIsProtected == false) {
					std::string obfuscatedViewName = obfuscate(viewName);
					viewPtr->setName(obfuscatedViewName);
					viewPtr->setOriginalName("");
					log << "    view " << viewName << " => " << obfuscatedViewName << std::endl;
				} else {
					log << "    view " << viewName << " => " << viewName << std::endl;
				}

				// iterate over all the ports
				typedef std::vector<PortSharedPtr> PortSharedPtrVector;
				PortSharedPtrVector ports;
				viewPtr->getPorts(ports);
				PortSharedPtrVector::iterator pp = ports.begin();
				PortSharedPtrVector::iterator pe = ports.end();
				while(pp < pe) {

					// look up the port name
					PortSharedPtr& portPtr = *pp++;
					std::string portName = portPtr->getName();
					//std::cout << "                found port \"" << portName << "\"" << std::endl;

					// obfuscate the name
					if(cellIsProtected == false) {
						std::string obfuscatedPortName = obfuscate(portName);
						portPtr->setName(obfuscatedPortName);
						std::string originalName = portPtr->getOriginalName();
						// protect bus nets
						boost::smatch what;
						boost::regex re(".*([\\(\\[].+[\\)\\]])");
						if(boost::regex_match(originalName, what, re, boost::match_default)) {
							std::string newName = obfuscatedPortName 
								+ std::string(what[1].first, what[1].second);
							//std::cout << "replacing \"" << originalName << "\" with \"" 
							//	<< newName << "\"" << std::endl;
							originalName = newName;
						}
						portPtr->setOriginalName(originalName);
						log << "        port " << portName << " => " << obfuscatedPortName 
							<< std::endl;
					} else {
						log << "        port " << portName << " => " << portName << std::endl;
					}

				}

				// iterate over all the instances
				typedef std::vector<InstanceSharedPtr> InstanceSharedPtrVector;
				InstanceSharedPtrVector instances;
				viewPtr->getInstances(instances);
				InstanceSharedPtrVector::iterator ip = instances.begin();
				InstanceSharedPtrVector::iterator ie = instances.end();
				while(ip < ie) {

					// look up the instance name
					InstanceSharedPtr& instancePtr = *ip++;
					std::string instanceName = instancePtr->getName();
					//std::cout << "                found instance \"" << instanceName << "\"" 
					//	<< std::endl;

					// obfuscate the name
					std::string obfuscatedInstanceName = obfuscate(instanceName);
					instancePtr->setName(obfuscatedInstanceName);
					instancePtr->setOriginalName("");
					log << "        instance " << instanceName << " => " << obfuscatedInstanceName 
						<< std::endl;

				}

				// iterate over all the nets
				typedef std::vector<NetSharedPtr> NetSharedPtrVector;
				NetSharedPtrVector nets;
				viewPtr->getNets(nets);
				NetSharedPtrVector::iterator np = nets.begin();
				NetSharedPtrVector::iterator ne = nets.end();
				while(np < ne) {

					// look up the net name
					NetSharedPtr& netPtr = *np++;
					std::string netName = netPtr->getName();
					//std::cout << "                found net \"" << netName << "\"" << std::endl;

					// obfuscate the name
					std::string obfuscatedNetName = obfuscate(netName);
					netPtr->setName(obfuscatedNetName);
					netPtr->setOriginalName("");
					log << "        net " << netName << " => " << obfuscatedNetName << std::endl;

					// iterate over all the ports
					typedef std::vector<PortSharedPtr> PortSharedPtrVector;
					PortSharedPtrVector ports;
					netPtr->getConnectedPorts(ports, false);
					PortSharedPtrVector::iterator pp = ports.begin();
					PortSharedPtrVector::iterator pe = ports.end();
					while(pp < pe) {
						// look up the port name
						PortSharedPtr& portPtr = *pp++;
						std::string portName = portPtr->getName();
						std::string parentPortName;
						CompositionType compositionType = portPtr->getCompositionType();
						switch(compositionType) {
							case eCompositionTypeScalar:
								parentPortName = portPtr->getName();
								if(parentPortName == portName) break;
								portPtr->setName(parentPortName);
								//std::cout << "                    NOTE: changing port name from \"" 
								//	<< portName << "\" to \"" << parentPortName << "\"" 
								//	<< std::endl;
								break;
							case eCompositionTypeVectorBit: {
								const PortSharedPtr& vectorPtr = portPtr->getParentCollection();
								parentPortName = vectorPtr->getName();
								if(parentPortName == portName) break;
								vectorPtr->setName(parentPortName);
								//std::cout << "                    NOTE: changing port name from \"" 
								//	<< portName << "\" to \"" << parentPortName << "\"" 
								//	<< std::endl;
								} break;
							default:
								std::cerr << "ERROR: Unsupported composition type " 
									<< compositionType << " in " << __FILE__ << " line " 
									<< __LINE__ << std::endl;
								break;
						}

					}

					/* this should no longer be necessary with the updated EDIF portref handling
					// iterate over all the portRefs
					typedef std::vector<PortReferenceSharedPtr> PortReferenceSharedPtrVector;
					PortReferenceSharedPtrVector portRefs;
					netPtr->getConnectedPortRefs(portRefs, false);
					PortReferenceSharedPtrVector::iterator prp = portRefs.begin();
					PortReferenceSharedPtrVector::iterator pre = portRefs.end();
					while(prp < pre) {
						// look up the portRef name
						PortReferenceSharedPtr& portRefPtr = *prp++;
						std::string portRefName = portRefPtr->getName();
						std::string masterPortName;
						CompositionType compositionType = portRefPtr->getCompositionType();
						switch(compositionType) {
							case eCompositionTypeScalar:
								masterPortName = portRefPtr->getMaster()->getName();
								if(masterPortName == portRefName) break;
								//portRefPtr->setName(masterPortName);
								//std::cout << "                    NOTE: changing portRef name from \"" 
								//	<< portRefName << "\" to \"" << masterPortName << "\"" 
								//	<< std::endl;
								break;
							case eCompositionTypeVectorBit: {
								const PortReferenceSharedPtr& vectorRefPtr 
									= portRefPtr->getParentCollection();
								masterPortName = vectorRefPtr->getMaster()->getName();
								if(masterPortName == portRefName) break;
								//vectorRefPtr->setName(masterPortName);
								//std::cout << "                    NOTE: changing portRef name from \"" 
								//	<< portRefName << "\" to \"" << masterPortName << "\"" 
								//	<< std::endl;
								} break;
							default:
								std::cerr << "ERROR: Unsupported composition type " 
									<< compositionType << " in " << __FILE__ << " line " 
									<< __LINE__ << std::endl;
								break;
						}
					}
					*/

				}
			}

		}

	}
}

int main(int argc, char** argv) {

	// iterate through each EDIF file specified
	for(int i = 1; i < argc; i++) {
		// build the file names
		boost::filesystem::path sourcePath = argv[i];
		boost::filesystem::path targetPath = sourcePath;
		targetPath = targetPath.replace_extension().string() 
			+ ".mod" + boost::filesystem::extension(sourcePath);
		boost::filesystem::path logPath = sourcePath;
		logPath = logPath.replace_extension().string() + ".obfuscation_log";
		// obfuscate the file
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
			// open the log file
			std::fstream logStream(logPath.string().c_str(), std::ios_base::out);
			// obfuscate the design
			obfuscate(rootPtr, logStream);
			// write the target file
			std::cout << "Writing target: " << targetPath.string() << " ...  ";
			std::cout.flush();
			std::fstream targetStream(targetPath.string().c_str(), std::ios_base::out);
			Decompiler decompiler(rootPtr, targetStream);
			decompiler();
			std::cout << "done." << std::endl;
		}
		// report failure
		catch(...) {
			std::cerr << "ERROR: Failed to obfuscate " << sourcePath.string() << std::endl;
		}
	}

	return 0;
}
