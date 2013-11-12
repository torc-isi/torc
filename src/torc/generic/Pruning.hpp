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

#ifndef TORC_GENERIC_PRUNING_HPP
#define TORC_GENERIC_PRUNING_HPP

#include "torc/generic/PointerTypes.hpp"
#include <map>

namespace torc {
namespace generic {

/**
 * @brief A map from cell shared pointer to usage count.
 */
typedef std::map<CellSharedPtr, int> CellUsageMap;

/**
 * @brief Prune all unused cells and libraries from the given root.
 * @param inRootPtr The root to prune.
 */
void prune(RootSharedPtr& inRootPtr);

/** 
 * @brief Recursively collect and count all cells used in the given view.
 * @detail The CellUsageMap must already be initialized with all expected cell counts set to zero.
 * @param inViewPtr The view to begin recursing from.
 * @param inoutCellUsageMap A map from cell to usage count.
 */
void collectCells(ViewSharedPtr& inViewPtr, CellUsageMap& inoutCellUsageMap);

} //namespace generic
} //namespace torc

#endif // TORC_GENERIC_PRUNING_HPP
