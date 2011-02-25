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

#ifndef TORC_GENERIC_PORT_BUNDLE_REFERENCE_HPP
#define TORC_GENERIC_PORT_BUNDLE_REFERENCE_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION


#include "torc/generic/om/Bundle.hpp"
#include "torc/generic/om/BundleFlattener.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/PortReference.hpp"
#include "torc/generic/om/VisitorType.hpp"

namespace torc { namespace generic { class BaseVisitor; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents a reference to  a bundle of ports.
 *
 * The PortBundle class represents a reference to an EDIF port bundle.
 */
class PortBundleReference :
    public PortReference,
    public Bundle<PortReference> {
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

    friend class  FactoryType<PortBundleReference>;
  public:
    typedef VisitorType<PortBundleReference> Visitor;
    
    /**
     * Convenience class to create a port bundle reference.
     */
    class Factory: public FactoryType<PortBundleReference>
    {
        public:
            using FactoryType<PortBundleReference>::create;

            /**
            * Create a portbundle ref.
            *
            * @param[in] inInstancePtr Pointer to parented(Instance) object.
            * @param[in] inPortPtr Pointer to master(Port) object.
            * @param[in] inFactory Factory for the child.
            * @param[in] inParentCollection Pointer to parent bundle.
            *
            * @return Pointer to created portbundle ref.
            **/
            virtual PortBundleReferenceSharedPtr
            newPortBundleReferencePtr( const InstanceSharedPtr &inInstancePtr,
                const PortSharedPtr &inPortPtr,
                const ObjectFactorySharedPtr &inFactory,
                const PortBundleReferenceSharedPtr &inParentCollection 
                                = PortBundleReferenceSharedPtr() ) throw(Error);
    };

    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    virtual void
    setParent( const InstanceSharedPtr &inParent ) throw();

    /**
     * Connect a Net to this object.
     * 
     * @note This metod can be overridden by derived classes. However, the method must call the on_connected() method after this. The sigConnected_ signal must also be invoked in the overriding method.
     * 
     * @param[in] inNet A pointer to the Net object that needs to be connected
     * @return A connection that has been established. This can be used later for disconnection.
     *
     * @exception Error  PortBundleReference size does not match with Master port size 
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorPointerToItemDoesNotExist
     *         </li>
     *         <li> Context Data
     *           <ul> 
     *               <li>PortBundleReference Size - <i>SizeType</i></li>
     *               <li>Master Port Size - <i>SizeType</i></li>
     *           </ul> 
     *         </li>
     *       </ul> 
     *
     * @exception Error  Net size does not match with PortBundleReference size
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemSizeMismatch
     *         </li>
     *         <li> Context Data
     *           <ul> 
     *               <li>Net Size - <i>SizeType</i></li>
     *               <li>PortBundleReference Size - <i>SizeType</i></li>
     *           </ul> 
     *         </li>
     *       </ul> 
     *    
     */
    virtual Connection
    connect(const NetSharedPtr & inNet) throw(Error)
;
    /**
    * Disconnect a Net from this object.
    * @note This metod can be overridden by derived classes. However, the method must call the on_connected() method after this. The sigConnected_ signal must also be invoked in the overriding method.
    
    * @param[in] connection A connection as returned by the connect() method
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
    virtual void
    disconnect(const Connection & inConnection) throw(Error);

    using Connectable::disconnect;

    /**
     * Set master port.
     *
     * @param[in] inMaster Set the master port.
     * @exception Error  Master port size does not match with port bundle reference size
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
    virtual void
    bindToMasterPort(const PortSharedPtr &inMaster) throw(Error);

    PortBundleReference();

    virtual
    ~PortBundleReference() throw();

  private:
    PortBundleReference( const PortBundleReference &);
    PortBundleReference &
    operator =( const PortBundleReference &);
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

};

} // namespace torc::generic

} // namespace torc
#endif
