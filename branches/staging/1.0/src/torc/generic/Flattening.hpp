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

#ifndef TORC_GENERIC_FLATTENING_HPP
#define TORC_GENERIC_FLATTENING_HPP

#include <boost/function.hpp>

#include "torc/generic/Error.hpp"
#include "torc/generic/PointerTypes.hpp"

/**
 * \file Flattening.hpp
 * \brief Contains functions for flattening a design
 *
 * This file contains functions and helpers for flattening a design
 */

namespace torc {
namespace generic {

/**
 * @typedef NetNamingFunction
 * @brief Function to create net names during flattening
 */
typedef boost::function<std::string(const std::string& inParentInstanceName,
	const NetSharedPtr& inNet, const std::vector<size_t>& inIndicesVector)> NetNamingFunction;

/**
 * @typedef InstanceNamingFunction
 * @brief Function to create instance names during flattening
 */
typedef boost::function<std::string(const std::string& inParentInstanceName,
	const InstanceSharedPtr& inInstance, const std::vector<size_t>& inIndicesVector)>
	InstanceNamingFunction;

/**
 * @typedef NetRenamingFunction
 * @brief Function to create names of nets that are set in rename constructs
 */
typedef boost::function<std::string(const InstanceSharedPtr& inParentInstance,
	const NetSharedPtr& inNet, const std::vector<size_t>& inIndicesVector)> NetRenamingFunction;

/**
 * @typedef InstanceRenamingFunction
 * @brief Function to create names of nets that are set in rename constructs
 */
typedef boost::function<std::string(const InstanceSharedPtr& inParentInstance,
	const InstanceSharedPtr& inInstance, const std::vector<size_t>& inIndicesVector)>
	InstanceRenamingFunction;

/**
 * Generate name for a newly generated object during flattening
 * This function generates names for objects during flattening. The default generated name is of
 * the form of parent_child_index. However if the name exceeds 255 characters this will be changed
 * to N00000001, N00000002 etc. The naming N00000001 etc restart for each object type. Therefore,
 * there may be both and instance and a net having the name N00000001. This is allowed in EDIF
 * 2 0 0.
 */
template <typename _Pointer> std::string getModifiedName(const std::string& inInstName,
	const _Pointer& inNameable, const std::vector<size_t>& inIndicesVector = std::vector<size_t>());

/**
 * Generate original name for a newly generated object during flattening
 * This function generates original names for objects during flattening. Unlike names these string
 * do not have a restriction of 255 characters and are stored in the rename construct attached to a
 * net or instance. The generated name is of the form Parent[index]/Child However the '/' can be
 * replaced by a custom separator that can be specified as a parameter to the flatten() function.
 */
template <typename _Pointer> std::string getRenamedName(const InstanceSharedPtr& inParentInstance,
	const _Pointer& inRenamable, const std::vector<size_t>& inIndicesVector =
	std::vector<size_t>());

/**
 * Flatten a given instance.
 * Instance flattening is an operation where a given instance is replaced the internals of it's
 * master in a given view. Instance flattening can be achieved on Single or Array Instances.
 * However, it is not supported on the elements of an instance array.
 *
 * @param[in] inInstance The instance that need to be flattened. This will be removed after the
 * operation.
 * @param[in] inFactory The object factory used for cloning components in the instance.
 *
 * @param[in] inRecursive If true, Flatten instance hierarchy. 
 * @param[in] inSeparator Use this separator to separate hierarchy elements
 * @param[in] inInstanceRenameFunc Function used to generate original name of new instances
 * @param[in] inNetRenamingFunc Function used to generate original name of new rename
 * @param[in] inInstanceNameFunc Function used to generate name of new instances
 * @param[in] inNetNamingFunc Function used to generate name of new nets
 *   
 * @note When connections are made to individual elements of a vector net using for example, say,
 * the portList construct, the bits of the vectors are appropriately broken up into individual
 * scalar nets, and connections removed from the vector nets.
 */
void flatten(const InstanceSharedPtr& inInstance, const ObjectFactorySharedPtr& inFactory,
	bool inRecursive = false, const std::string& inSeparator = "/",
	const InstanceRenamingFunction& inInstanceRenameFunc
		= InstanceRenamingFunction(getRenamedName<InstanceSharedPtr>),
	const NetRenamingFunction& inNetRenameFunc = NetRenamingFunction(getRenamedName<NetSharedPtr>),
	const InstanceNamingFunction& inInstanceNameFunc
		= InstanceNamingFunction(getModifiedName<InstanceSharedPtr>),
	const NetNamingFunction& inNetNameFunc
		= NetNamingFunction(getModifiedName<NetSharedPtr>)) throw (Error);

/**
 * Flatten all instances in a given view.
 * Instance flattening is an operation where a given instance is replaced the internals of it's
 * master in a given view. Instance flattening can be achieved on Single or Array Instances.
 * However, it is not supported on the elements of an instance array. Difference with the normal
 * <i>flatten()</i> function is that this will keep flattening all instances in the gien view until
 * a set of leaf instances is received. A <i>Leaf Instance</i> is an instance where the view has
 * only interface delcarations and no instantiations of other cells within it.
 *
 * @param[in] inView All instances in the given view will be flatten.
 * @param[in] inFactory The object factory used for cloning components in the instance.
 *
 * @param[in] inRecursive If true, Flatten instance hierarchy.
 * @param[in] inSeparator Use this separator to separate hierarchy elements
 * @param[in] inInstanceRenameFunc Function used to generate original name of new instances
 * @param[in] inNetRenamingFunc Function used to generate original name of new rename
 * @param[in] inInstanceNameFunc Function used to generate name of new instances
 * @param[in] inNetNamingFunc Function used to generate name of new nets
 *
 * @note When connections are made to individual elements of a vector net using for example, say,
 * the portList construct, the bits of the vectors are appropriately broken up into individual
 * scalar nets, and conections removed from the vector nets.
 */
void flatten(const ViewSharedPtr& inView, const ObjectFactorySharedPtr& inFactory,
	bool inRecursive = false, const std::string& inSeparator = "/",
	const InstanceRenamingFunction& inInstanceRenameFunc
		= InstanceRenamingFunction(getRenamedName<InstanceSharedPtr>),
	const NetRenamingFunction& inNetRenameFunc = NetRenamingFunction(getRenamedName<NetSharedPtr>),
	const InstanceNamingFunction& inInstanceNameFunc
		= InstanceNamingFunction(getModifiedName<InstanceSharedPtr>),
	const NetNamingFunction& inNetNameFunc
		= NetNamingFunction(getModifiedName<NetSharedPtr>)) throw (Error);
/**
 * Flatten a given design, all NETLIST views owned by the design cell will be flattened. 
 *          Instance flattening is an operation where a given instance is replaced the internals of
 *          it's master in a given view. Instance flattening can be achieved on Single or Array
 *          Instances. However, it is not supported on the elements of an instance array.
 *          Difference with the normal <i>flatten()</i> function is that this will keep flattening
 *          all instances in the given design until a set of leaf instances is received. A <i>Leaf
 *          Instance</i> is an instance where the view has only interface delcarations and no
 *          instantiations of other cells within it.
 *
 * @param[in] inDesign The design that need to be flattened.
 * @param[in] inFactory The object factory used for cloning components in the instance.
 *
 * @param[in] inRecursive If true, Flatten instance hierarchy.
 * @param[in] inSeparator Use this separator to separate hierarchy elements
 * @param[in] inInstanceRenameFunc Function used to generate original name of new instances
 * @param[in] inNetRenamingFunc Function used to generate original name of new rename
 * @param[in] inInstanceNameFunc Function used to generate name of new instances
 * @param[in] inNetNamingFunc Function used to generate name of new nets
 *
 * @note When connections are made to individual elements of a vector net using for example, say,
 * the portList construct, the bits of the vectors are appropriately broken up into individual
 * scalar nets, and conections removed from the vector nets.
 */
void flatten(const DesignSharedPtr& inDesign, const ObjectFactorySharedPtr& inFactory,
	bool inRecursive = false, const std::string& inSeparator = "/",
	const InstanceRenamingFunction& inInstanceRenameFunc
		= InstanceRenamingFunction(getRenamedName<InstanceSharedPtr>),
	const NetRenamingFunction& inNetRenameFunc = NetRenamingFunction(getRenamedName<NetSharedPtr>),
	const InstanceNamingFunction& inInstanceNameFunc
		= InstanceNamingFunction(getModifiedName<InstanceSharedPtr>),
	const NetNamingFunction& inNetNameFunc
		= NetNamingFunction(getModifiedName<NetSharedPtr>)) throw (Error);

} //namespace generic
} //namespace torc

#endif // TORC_GENERIC_FLATTENING_HPP
