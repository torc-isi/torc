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

#ifndef TORC_GENERIC_NETBUNDLE_HPP
#define TORC_GENERIC_NETBUNDLE_HPP

#include "torc/generic/PointerTypes.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/Net.hpp"
#include "torc/generic/Bundle.hpp"
#include "torc/generic/VisitorType.hpp"
#include "torc/generic/FactoryType.hpp"
#include "torc/generic/Error.hpp"
#include "torc/generic/Connectable.hpp"

namespace torc { namespace generic { class BaseVisitor; } }

namespace torc {
namespace generic {

/**
 * @brief Represents a bundle of nets.
 *
 * The NetBundle class represents an EDIF net bundle. NetBundles are ordered collection of scalar
 * and vector nets and can be created using the (nettBundle ...) syntax.
 */
class NetBundle : public Net, public Bundle<Net> {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION
	friend class FactoryType<NetBundle> ;

public:
	typedef VisitorType<NetBundle> Visitor;

	/**
	 * Convenience class to create a net bundle.
	 */
	class Factory : public FactoryType<NetBundle> {
	public:
		using FactoryType<NetBundle>::create;
		/**
		 * Create a net bundle.
		 *
		 * @param[in] inName Name of the net bundle to be created.
		 * @param[in] inViewPtr Pointer to parented(View) object.
		 * @param[in] inParentCollection Pointer to parent bundle.
		 * @param[in] inOriginalName Original name of the net bundle [optional].
		 *
		 * @return Pointer to created net bundle.
		 **/
		virtual NetBundleSharedPtr newNetBundlePtr(const std::string& inName,
			const ViewSharedPtr& inViewPtr, const NetBundleSharedPtr& inParentCollection =
				NetBundleSharedPtr(), const std::string& inOriginalName = std::string())
				throw (Error);
	};

	virtual void accept(BaseVisitor& inoutVisitor) throw (Error);

	virtual void setParent(const ViewSharedPtr& inParent);

	/**
	 * Connect the given net with this net. During connection, a none-to-one correspondence is
	 * maintained, and all children of the given net are connected to the children of the current
	 * net. As a result, if the given net is a preserved vector, it will be exploded.
	 * @param[in] inNet Net to be connected to the current net
	 *
	 * @exception Error Provided connection is invalid, because pointer to the Net does not exist
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorPointerToItemDoesNotExist
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Pointer to Net - <i>boost::shared_ptr</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 * @exception Error Net size does not match with NetBundle size
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorItemSizeMismatch
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Net Size - <i>SizeType</i></li>
	 *               <li>NetBundle Size - <i>SizeType</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 */
	virtual Connection connect(const NetSharedPtr& inNet) throw (Error);

	void disconnect(const Connection& inConnection) throw (Error);

	void disconnect() throw (Error);

	virtual ~NetBundle() throw ();

protected:
	NetBundle();

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_NETBUNDLE_HPP
