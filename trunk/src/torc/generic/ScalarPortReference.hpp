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

#ifndef TORC_GENERIC_SCALARPORTREFERENCE_HPP
#define TORC_GENERIC_SCALARPORTREFERENCE_HPP

#include "torc/generic/PointerTypes.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/PortReference.hpp"
#include "torc/generic/Scalar.hpp"
#include "torc/generic/FactoryType.hpp"
#include "torc/generic/VisitorType.hpp"
#include "torc/generic/Error.hpp"

namespace torc { namespace generic { class BaseVisitor; } }

namespace torc {
namespace generic {

/**
 * @brief Represents areference to a standalone port
 *
 * The ScalarPortReference class represents a reference to a a standalone port object.
 */
class ScalarPortReference : public PortReference, public Scalar<PortReference> {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION
	friend class FactoryType<ScalarPortReference> ;

public:
	typedef VisitorType<ScalarPortReference> Visitor;

	/**
	 * Convenience class to create a scalar port ref.
	 */
	class Factory : public FactoryType<ScalarPortReference> {
	public:
		using FactoryType<ScalarPortReference>::create;
		/**
		 * Create a scalar port ref.
		 *
		 * @param[in] inInstancePtr Pointer to parented(Instance) object.
		 * @param[in] inPortPtr Pointer to master(Port) object.
		 * @param[in] inParentCollection Pointer to parent bundle.
		 *
		 * @return Pointer to created scalar port ref.
		 **/
		virtual ScalarPortReferenceSharedPtr newScalarPortReferencePtr(
			const InstanceSharedPtr& inInstancePtr, const PortSharedPtr& inPortPtr,
			const PortBundleReferenceSharedPtr& inParentCollection = PortBundleReferenceSharedPtr())
				throw (Error);
	};

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
	 * @exception Error  Net size does not match with scalar port reference size
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorItemSizeMismatch
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Net Size - <i>SizeType</i></li>
	 *               <li>Scalar port reference Size - <i>SizeType</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 */

	virtual Connection connect(const NetSharedPtr& inNet) throw (Error);

	using Connectable::disconnect;

	virtual void disconnect(const Connection& inConnection) throw (Error);

	~ScalarPortReference() throw ();

protected:
	ScalarPortReference();

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_SCALARPORTREFERENCE_HPP
