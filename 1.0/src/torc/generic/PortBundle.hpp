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

#ifndef TORC_GENERIC_PORTBUNDLE_HPP
#define TORC_GENERIC_PORTBUNDLE_HPP

#include "torc/generic/PointerTypes.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/Bundle.hpp"
#include "torc/generic/BundleFlattener.hpp"
#include "torc/generic/Connectable.hpp"
#include "torc/generic/Error.hpp"
#include "torc/generic/FactoryType.hpp"
#include "torc/generic/Port.hpp"
#include "torc/generic/VisitorType.hpp"

namespace torc { namespace generic { class Net; } }
namespace torc { namespace generic { class BaseVisitor; } }

namespace torc {
namespace generic {

/**
 * @brief Represents a bundle of ports.
 * 
 * The PortBundle class represents an EDIF port bundle. PortBundles are ordered collection of scalar and vector ports and can be created using the (portBundle ...) syntax.
 */
class PortBundle : public Port, public Bundle<Port> {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION
	friend class FactoryType<PortBundle> ;

public:
	typedef VisitorType<PortBundle> Visitor;

	/**
	 * Convenience class to create a port bundle.
	 */
	class Factory : public FactoryType<PortBundle> {
	public:
		using FactoryType<PortBundle>::create;
		/**
		 * Create a port bundle.
		 *
		 * @param[in] inName Name of the port bundle to be created.
		 * @param[in] inViewPtr Pointer to parented(View) object.
		 * @param[in] inParentCollection Pointer to parent bundle.
		 * @param[in] inOriginalName Original name of the port bundle [optional].
		 *
		 * @return Pointer to created port bundle.
		 **/
		virtual PortBundleSharedPtr newPortBundlePtr(const std::string& inName,
			const ViewSharedPtr& inViewPtr,
			const PortBundleSharedPtr& inParentCollection = PortBundleSharedPtr(),
			const std::string& inOriginalName = std::string()) throw (Error);
	};

	virtual void setParent(const ViewSharedPtr& inParent);

	/**
	 * Connect a Net to this object.
	 *
	 * @note This metod can be overridden by derived classes. However, the method must call the
	 * on_connected() method after this. The sigConnected_ signal must also be invoked in the
	 * overriding method.
	 *
	 * @param[in] inNet A pointer to the Net object that needs to be connected
	 * @return A connection that has been established. This can be used later for disconnection.
	 *
	 * @exception Error Pointer to the Net object does not exists
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorPointerToItemDoesNotExist
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Pointe to Net object - <i>boost::shared_ptr</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 * @exception Error  Net size does not match with port bundle size
	 *       <ul>
	 *         <li>
	 *             Id : eMessageIdErrorItemSizeMismatch
	 *         </li>
	 *         <li> Context Data
	 *           <ul>
	 *               <li>Net Size - <i>SizeType</i></li>
	 *               <li>Port Size - <i>SizeType</i></li>
	 *           </ul>
	 *         </li>
	 *       </ul>
	 *
	 */
	virtual Connectable::Connection connect(const NetSharedPtr& inNet) throw (Error);

	/**
	 * Disconnect a Net from this object.
	 * @note This metod can be overridden by derived classes. However, the method must call the
	 * on_connected() method after this. The sigConnected_ signal must also be invoked in the
	 * overriding method.

	 * @param[in] inConnection A connection as returned by the connect() method
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
	 */
	void disconnect(const Connectable::Connection& inConnection) throw (Error);

	using Connectable::disconnect;

	virtual void accept(BaseVisitor& inoutVisitor) throw (Error);

	virtual ~PortBundle() throw ();

protected:
	PortBundle();

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_PORTBUNDLE_HPP
