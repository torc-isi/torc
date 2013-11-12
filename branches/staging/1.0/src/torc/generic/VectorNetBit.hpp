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

#ifndef TORC_GENERIC_VECTORNETBIT_HPP
#define TORC_GENERIC_VECTORNETBIT_HPP

#include "torc/generic/PointerTypes.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/VectorBit.hpp"
#include "torc/generic/Net.hpp"
#include "torc/generic/FactoryType.hpp"
#include "torc/generic/VisitorType.hpp"
#include "torc/generic/Error.hpp"

namespace torc { namespace generic { class BaseVisitor; } }

namespace torc {
namespace generic {

/**
 * @brief Represents a bit of a net array.
 *
 * The VectorNetBit class represents a bit of an array net. Such objects cannot be created in EDIF.
 * They are automatically created when a particular member of an array is accessed using the
 * (member ...) syntax.
 */
class VectorNetBit : public Net, public VectorBit<Net> {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION
public:
	typedef FactoryType<VectorNetBit> Factory;

	typedef VisitorType<VectorNetBit> Visitor;

	virtual void accept(BaseVisitor& inoutVisitor) throw (Error);

	/**
	 * @exception Error Pointer to the Net object does not exists
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorPointerToItemDoesNotExist
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Pointe to Net object - <i>Net</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 * @exception Error  Net size does not match with vector net bit size
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorItemSizeMismatch
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Net Size - <i>SizeType</i></li>
	 *               <li>VectorNetBit Size - <i>SizeType</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 */

	virtual Connection connect(const NetSharedPtr& inNet) throw (Error);

	using Connectable::disconnect;

	virtual void disconnect(const Connection& inConnection) throw (Error);

	virtual void getConnectedNets(std::vector<NetSharedPtr>& outNets,
		bool inSkipChildConnections = false) const throw (Error);

	virtual void getConnectedPorts(std::vector<PortSharedPtr>& outPorts,
		bool inSkipChildConnections = false) const;

	virtual void getConnectedPortRefs(std::vector<PortReferenceSharedPtr>& outPortRefs,
		bool inSkipChildConnections = false) const;

	VectorNetBit();

	virtual ~VectorNetBit() throw ();

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_VECTORNETBIT_HPP
