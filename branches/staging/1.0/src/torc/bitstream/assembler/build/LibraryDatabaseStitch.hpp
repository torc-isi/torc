// This program takes in path to a folder. It goes over all the files with extension .cbit
// in the folder and coalates them into one file.

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
//
#ifndef LIBRARYDATABASE_HPP
#define LIBRARYDATABASE_HPP

// Typedefs for Maps
typedef std::map<const std::string, std::vector<uint32_t> > ConfigBitMap;
typedef std::map<const std::string, ConfigBitMap> ElementConfigMap;
typedef std::map<const std::string, ElementConfigMap> TiletypeElementMap;

const std::string cNameSeparator = "-";
const std::string cFamily = "Virtex5";
#endif
