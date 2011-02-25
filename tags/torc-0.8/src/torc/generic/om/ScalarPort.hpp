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

#ifndef TORC_GENERIC_SCALAR_PORT_HPP
#define TORC_GENERIC_SCALAR_PORT_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/Port.hpp"
#include "torc/generic/om/Scalar.hpp"
#include "torc/generic/om/VisitorType.hpp"

namespace torc { namespace generic { class BaseVisitor; }  } 

namespace torc {

namespace generic {

/**
 * @brief Represents a standalone port
 * 
 * The ScalarPort class represents a standalone port object. Such objects in EDIF are declard using the (port portName ... ) syntax.
 */
class ScalarPort
	: public Port,
	public Scalar<Port> {
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

    friend class FactoryType<ScalarPort>;

  public:
    typedef VisitorType<ScalarPort> Visitor;

    /**
     * Convenience class to create a scalar port.
     */
    class Factory: public FactoryType<ScalarPort>
    {
        public:
            using FactoryType<ScalarPort>::create;
        /**
         * Create a scalar port.
         *
         * @param[in] inName Name of the scalar port to be created.
         * @param[in] inDirection Direction of port. 
         * @param[in] inViewPtr Pointer to parented(View) object.
         * @param[in] inParentCollection Pointer to parent bundle.
         * @param[in] inOriginalName Original name of the scalar port [optional].
         *
         * @return Pointer to created scalar port.
         **/
            virtual ScalarPortSharedPtr
            newScalarPortPtr( const std::string &inName,
                const PortDirection &inDirection,
                const ViewSharedPtr &inViewPtr,
                const PortBundleSharedPtr &inParentCollection = PortBundleSharedPtr(),
                const std::string &inOriginalName = std::string()) throw(Error);
    };

    virtual void
	accept(BaseVisitor & inoutVisitor) throw(Error);

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
     * @exception Error  Net size does not match with scalar port size
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemSizeMismatch
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Net Size - <i>SizeType</i></li>
     *               <li>Scalar port Size - <i>SizeType</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
	 */

    virtual Connection
    connect(const NetSharedPtr &inNet) throw(Error);

    using Connectable::disconnect;

    virtual void
    disconnect(const Connection &inConnection) throw(Error);

    virtual
	~ScalarPort() throw();

  protected:  
    ScalarPort();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

};

} // namespace torc::generic

} // namespace torc
#endif
