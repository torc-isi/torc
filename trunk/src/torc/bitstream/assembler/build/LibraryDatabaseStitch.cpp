// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
// $HeadURL: https://svn.east.isi.edu/torc/trunk/src/torc/packer/Virtex5PrimitiveStructureUnitTest.cpp $
// $Id: Virtex5PrimitiveStructureUnitTest.cpp 768 2011-08-24 19:44:37Z nsteiner $

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
/// \brief This program takes in path to a folder. It goes over all the files with extension .cbit
// in the folder and coalates them into one file. Format of the colated file -
// <<<<BITLIBDB>>>>
// Number of tile types
// Tiletype name char count
// Tile Name
// Number of elements
// 	Element name char count
//	Element name
//	Config count
//		Config name char count
//		Config name
//		Number of 32bit words in compressed bitstream
//			words

//#include "LibraryDatabaseStitch.hpp"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "SharedFunctions.hpp"

// Given path to folder of compressed bit files, go over all the files and put them in a a map
// data structure as described above.
void PopulateMaps(boost::filesystem::path libPath, TiletypeElementMap &outTileMap,
	std::vector<ElementConfigMap> &outElemMapVector, std::vector<ConfigBitMap> &outConfigMapVector) {

	// Go over all the files in the directory
	boost::filesystem::directory_iterator eFiles;
	boost::filesystem::directory_iterator pFiles(libPath);
	while(pFiles != eFiles) {

		// If it is a compressed bit file (.cbit extension)
		if(pFiles->path().extension() == ".cbit") {

			std::ifstream libBitFile(pFiles->path().string().c_str(), std::ios::binary);
			if(!libBitFile.good()) {
				std::cerr << "ERROR: Could not open micro-bitstream file " << pFiles->path().string() << std::endl;
				return;
			}

			// Get number of bits set and put the bit addresses in a vector
			int32_t numSetBits, bitAddr;
			libBitFile.read((char *) &numSetBits, 4);
			std::cout << pFiles->path().filename() <<  " opened. Bit count " << numSetBits << std::endl;
			std::vector<uint32_t> bitAddresses;
			for(int i = 0; i < numSetBits && !libBitFile.eof(); i++) {
				libBitFile.read((char *) &bitAddr, 4);
				bitAddresses.push_back(bitAddr);
			}

			// Get the file name without extension and split the name to get tile name, element/source wire and config/sink wire.
			boost::filesystem::path filePath(pFiles->path());
			std::string fileName(filePath.replace_extension().filename());

			std::vector<std::string> splitVector;
			boost::algorithm::split(splitVector, fileName, boost::algorithm::is_any_of(kNameSeparator));
			for(std::vector<std::string>::const_iterator splitIter = splitVector.begin(); splitIter
				!= splitVector.end(); splitIter++) {
				std::cout << "   " << *splitIter << std::endl;

			}

			// Get the tile type, element(source wire) and config (sink wire)from the split string
			std::string tileType(splitVector[1]);
			std::string element(splitVector[2]);
			std::string config(splitVector[3]);

			// if tile not found in map, a new tile type has come up. Add it to the map.
			// Create another map for Element-config and push it in vector so that it is not destroyed.
			if(outTileMap.find(tileType) == outTileMap.end()) {
				ElementConfigMap elementConfigMap;
				outElemMapVector.push_back(elementConfigMap);
				outTileMap[tileType] = outElemMapVector.back();
			}

			// if the element in the tile doesn't exist
			if(outTileMap[tileType].find(element) == outTileMap[tileType].end()) {
				ConfigBitMap configBitmap;
				outConfigMapVector.push_back(configBitmap);
				outTileMap[tileType][element] = outConfigMapVector.back();
			}

			// Mapping the config to bit addresses
			outTileMap[tileType][element][config] = bitAddresses;

		}

		pFiles++;
	}
}
//
int main(int argc, char **argv) {

	// This program should receive one parameter - the folder of compressed bits
	if(argc != 2) {
		std::cout << "Pass correct parameters. Usage: " << argv[0]
			<< " <folder of compressed bit files" << std::endl;
		return -1;
	}

	// Check if the folder exist
	boost::filesystem::path libPath(argv[1]);
	if(!boost::filesystem::exists(libPath)) {
		std::cout << "The folder " << libPath.string().c_str() << " does not exist" << std::endl;
		return -1;
	}

	// Maps to store the information on tile type, elements and configs
	std::vector<ConfigBitMap> configMapVector;
	std::vector<ElementConfigMap> elementMapVector;
	TiletypeElementMap tileMap;

	// Write the database file to disk
	std::string dbFileName = kFamily + ".ldb";
	boost::filesystem::path dbFilePath = libPath / dbFileName;
	std::ofstream dbFile(dbFilePath.string().c_str(), std::ofstream::binary);
	if(!dbFile.good()) {
		std::cout << "Could not open file " << dbFilePath.string() << " to write" << std::endl;
		return -1;
	}

	// Populate the maps
	PopulateMaps(libPath, tileMap, elementMapVector, configMapVector);

	std::cout << "Opened file " << dbFilePath.string() << " to store library database" << std::endl;
	dbFile.write("<<<<BITLIBDB>>>>", 16);
//	std::cout << "Tile map size " << sizeof(tileMap.size()) << std::endl;
	uint32_t dataSize;
	dataSize = tileMap.size();
	dbFile.write((char *) (&dataSize), sizeof(dataSize));


	// Now go over the map data structure and write it to a file
	// File format is given at the top of this file
	TiletypeElementMap::const_iterator pTiletypes = tileMap.begin();
	TiletypeElementMap::const_iterator eTiletypes = tileMap.end();
	while(pTiletypes != eTiletypes) {

		// Write the tile name char count, name string and element count
		dataSize = pTiletypes->first.size();
		dbFile.write((const char *) &dataSize, sizeof(dataSize));
		dbFile.write((const char *) pTiletypes->first.c_str(), pTiletypes->first.size());
		dataSize = pTiletypes->second.size();
		dbFile.write((const char*) &dataSize, sizeof(dataSize));
		std::cout << "Tile type " << pTiletypes->first << " has " << pTiletypes->second.size()
			<< " elements." << std::endl;

		// Iterate over elements
		ElementConfigMap::const_iterator pElements = pTiletypes->second.begin();
		ElementConfigMap::const_iterator eElements = pTiletypes->second.end();
		while(pElements != eElements) {

			// Write element name char count, name string and config count
			dataSize = pElements->first.size();
			dbFile.write((const char *) &dataSize, sizeof(dataSize));
			dbFile.write((const char *) pElements->first.c_str(), pElements->first.size());
			dataSize = pElements->second.size();
			dbFile.write((const char *) &dataSize, sizeof(dataSize));
//			std::cout << "   Element type " << pElements->first << " has "
//				<< pElements->second.size() << " configs." << std::endl;

			// Itereate over configs
			ConfigBitMap::const_iterator pConfigs = pElements->second.begin();
			ConfigBitMap::const_iterator eConfigs = pElements->second.end();
			while(pConfigs != eConfigs) {
				// Write config namem char count, name string and count of bit addresses
				dataSize = pConfigs->first.size();
				dbFile.write((const char *) &dataSize, sizeof(dataSize));
				dbFile.write(pConfigs->first.c_str(), pConfigs->first.size());
				dataSize = pConfigs->second.size();
				dbFile.write((const char *) &dataSize, sizeof(dataSize));

				// Write the bit addresses
				for(std::vector<uint32_t>::const_iterator iter = pConfigs->second.begin(); iter
					!= pConfigs->second.end(); iter++) {
					dbFile.write((char *) &*iter, sizeof(uint32_t));
				}
//				std::cout << "\t" << pConfigs->first << " " << pConfigs->second.size();
				pConfigs++;
			}
//			std::cout << std::endl;
			pElements++;
		}
		pTiletypes++;

	}

	return 0;
}
