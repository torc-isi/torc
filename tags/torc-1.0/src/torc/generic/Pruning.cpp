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

/*
#ifdef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif //HAVE_CONFIG_H

*/
#include "torc/generic/Pruning.hpp"

#include "torc/generic/Cell.hpp"
#include "torc/generic/Design.hpp"
#include "torc/generic/Instance.hpp"
#include "torc/generic/Library.hpp"
#include "torc/generic/View.hpp"
#include "torc/generic/Log.hpp"

namespace torc {
namespace generic {

using namespace torc::generic;

void collectCells(ViewSharedPtr& inViewPtr, CellUsageMap& inoutCellUsageMap) {

	// iterate over all of the instances
	typedef std::vector<InstanceSharedPtr> InstanceSharedPtrVector;
	InstanceSharedPtrVector instances;
	inViewPtr->getInstances(instances);
	InstanceSharedPtrVector::iterator ip = instances.begin();
	InstanceSharedPtrVector::iterator ie = instances.end();
	while(ip < ie) {
		// look up the instance
		InstanceSharedPtr instancePtr = *ip++;
		// look up the master view and its parent cell
		ViewSharedPtr masterViewPtr = instancePtr->getMaster();
		CellSharedPtr masterCellPtr = masterViewPtr->getParent();
		// look up the master cell, and increment its usage count
		inoutCellUsageMap[masterCellPtr]++;
		// recurse
		collectCells(masterViewPtr, inoutCellUsageMap);
	}

}

void prune(RootSharedPtr& inRootPtr) {

	// declare a map of all cells actually used by this root
	CellUsageMap cellUsageMap;

	// iterate over all of the libraries and collect all cells
	typedef std::vector<LibrarySharedPtr> LibrarySharedPtrVector;
	LibrarySharedPtrVector libraries;
	inRootPtr->getLibraries(libraries);
	LibrarySharedPtrVector::iterator lp = libraries.begin();
	LibrarySharedPtrVector::iterator le = libraries.end();
	while(lp < le) {
		// look up the library name and specifics
		LibrarySharedPtr libraryPtr = *lp++;
		// iterate over all library cells
		typedef std::vector<CellSharedPtr> CellSharedPtrVector;
		CellSharedPtrVector cells;
		libraryPtr->getCells(cells);
		CellSharedPtrVector::iterator cp = cells.begin();
		CellSharedPtrVector::iterator ce = cells.end();
		while(cp < ce) {
			// look up the cell
			CellSharedPtr cellPtr = *cp++;
			cellUsageMap[cellPtr] = 0;
		}
	}

	// iterate over all of the designs and collect cell usage counts
	typedef std::vector<DesignSharedPtr> DesignSharedPtrVector;
	DesignSharedPtrVector designs;
	inRootPtr->getDesigns(designs);
	DesignSharedPtrVector::iterator dp = designs.begin();
	DesignSharedPtrVector::iterator de = designs.end();
	while(dp < de) {
		// look up the design name and specifics
		DesignSharedPtr designPtr = *dp++;
		std::string designName = designPtr->getName();
		std::string cellName = designPtr->getCellRefName();
		std::string libraryName = designPtr->getLibraryRefName();
		// look up the design cell
		LibrarySharedPtr libraryPtr = inRootPtr->findLibrary(libraryName);
		CellSharedPtr cellPtr = libraryPtr->findCell(cellName);
		log("Examining library %s, cell %s, design %s.\n", libraryName.c_str(), cellName.c_str(), 
			designName.c_str());
		// ensure that this design cell is properly counted
		cellUsageMap[cellPtr]++;
		// iterate over all cell views
		typedef std::vector<ViewSharedPtr> ViewSharedPtrVector;
		ViewSharedPtrVector views;
		cellPtr->getViews(views);
		ViewSharedPtrVector::iterator vp = views.begin();
		ViewSharedPtrVector::iterator ve = views.end();
		while(vp < ve) {
			// look up the view
			ViewSharedPtr viewPtr = *vp++;
			// collect all cells
			collectCells(viewPtr, cellUsageMap);
		}
	}

	// display all cells that are in use
	CellUsageMap::iterator cp = cellUsageMap.begin();
	CellUsageMap::iterator ce = cellUsageMap.end();
	while(cp != ce) {
		// look up the cell information
		CellUsageMap::value_type cellUsage = *cp++;
		CellSharedPtr cellPtr = cellUsage.first;
		LibrarySharedPtr libraryPtr = cellUsage.first->getParent();
		std::string cellName = cellPtr->getName();
		std::string libraryName = libraryPtr->getName();
		log("    library %s, cell %s: %d\n", libraryName.c_str(), cellName.c_str(), 
			cellUsage.second);
		if(cellUsage.second == 0) {
			log("    removing unused cell %s from library %s: %s.\n", cellName.c_str(), 
				libraryName.c_str(), libraryPtr->findCell(cellName) ? "found" : "not found");
			libraryPtr->removeCell(cellName);
		}
	}

	// iterate over all of the libraries and remove empty ones
	for(LibrarySharedPtrVector::size_type i = 0; i < libraries.size(); i++) {
		// look up the library name and specifics
		LibrarySharedPtr libraryPtr = libraries[i];
		std::string libraryName = libraryPtr->getName();
		// if the library has no cells, remove it
		typedef std::vector<CellSharedPtr> CellSharedPtrVector;
		CellSharedPtrVector cells;
		libraryPtr->getCells(cells);
		if(cells.size() == 0) {
			log("    removing unused library %s.\n", libraryName.c_str());
			inRootPtr->removeLibrary(libraryName);
		}
	}

}

} //namespace generic
} //namespace torc
