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

#ifndef TORC_GENERIC_OM_FLATTENING_HPP
#define TORC_GENERIC_OM_FLATTENING_HPP

#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/PointerTypes.hpp"

namespace torc {
namespace generic {

/**
 * Flatten a given instance.
 * Instance flattening is an operation where a given instance is replaced the internals of it's master in a given view. Instance flattening can be achieved on Single or Array Instances. However, it is not supported on the elements of an instance array.
 *
 * @param[in] inInstance The instance that need to be flattened. This will be removed aafter the operation.
 * @param[in] inFactory The object factory used for cloning components in the instance.
 *
 * @note When connections are made to individual elements of a vector net using for example, say, the portList construct, the bits of the vectors are appropriately broken up into individual scalar nets, and conections removed from the vector nets.
 */
void
flatten( const InstanceSharedPtr &inInstance,
        const ObjectFactorySharedPtr &inFactory) throw(Error);

/**
 * Flatten a given instance hierarchy.
 * Instance flattening is an operation where a given instance is replaced the internals of it's master in a given view. Instance flattening can be achieved on Single or Array Instances. However, it is not supported on the elements of an instance array. Difference with the normal <i>flatten()</i> function is that this will keep flattening the given instance until a set of leaf instances is received. A <i>Leaf Instance</i> is an instance where the view has only interface delcarations and no instantiations of other cells within it.

 *
 * @param[in] inInstance The instance that need to be flattened. This will be removed aafter the operation.
 * @param[in] inFactory The object factory used for cloning components in the instance.
 *
 * @note When connections are made to individual elements of a vector net using for example, say, the portList construct, the bits of the vectors are appropriately broken up into individual scalar nets, and conections removed from the vector nets.
 */
void
flatten_hierarchy( const InstanceSharedPtr &inInstance,
        const ObjectFactorySharedPtr &inFactory) throw(Error);

} //namespace generic
} //namespace torc

#endif // TORC_GENERIC_OM_FLATTENING_HPP
