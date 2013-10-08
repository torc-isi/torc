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

#ifndef TORC_GENERIC_OM_VECTORPORTREFERENCE_HPP
#define TORC_GENERIC_OM_VECTORPORTREFERENCE_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Vector.hpp"
#include "torc/generic/om/PortReference.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/Connectable.hpp"
#include "torc/generic/om/VectorPortBitReference.hpp"

namespace torc { namespace generic { class BaseVisitor; }  }
namespace torc { namespace generic { class Net; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents a reference to a port array.
 *
 * The VectorPortReference class represents a reference to a port array.
 */
class VectorPortReference
    : public PortReference,
    public Vector<PortReference, VectorPortBitReference,
                    VectorPortBitReference::Factory, true> {
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

    friend class FactoryType<VectorPortReference>;

  public:
    typedef Vector<PortReference, VectorPortBitReference,
            VectorPortBitReference::Factory, true> BaseType;
    typedef VisitorType<VectorPortReference> Visitor;

    /**
     * Convenience class to create a vector port ref.
     */
    class Factory: public FactoryType<VectorPortReference>
    {
        public:
            using FactoryType<VectorPortReference>::create;
        /**
         * Create a vector port ref.
         *
         * @param[in] inInstancePtr Pointer to parented(Instance) object.
         * @param[in] inPortPtr Pointer to master(Port) object.
         * @param[in] inSize Size of the net array.
         * @param[in] inParentCollection Pointer to parent bundle.
         * @param[in] inFactory Factory for the child.
         *
         * @return Pointer to created scalar port ref.
         **/
            virtual VectorPortReferenceSharedPtr
            newVectorPortReferencePtr( const InstanceSharedPtr &inInstancePtr,
                    const PortSharedPtr &inPortPtr,
                    const size_t &inSize,
                    const PortBundleReferenceSharedPtr &inParentCollection 
                            = PortBundleReferenceSharedPtr(),    
                    const ChildFactorySharedPtr &inFactory
                            = BaseType::ChildFactorySharedPtr(
                                    new BaseType::ChildFactory() )) throw(Error);

        /**
         * Create a vector port ref.
         *
         * @param[in] inInstancePtr Pointer to parented(Instance) object.
         * @param[in] inPortPtr Pointer to master(Port) object.
         * @param[in] inLimits Dimensions of the vector.
         * @param[in] inParentCollection Pointer to parent bundle.
         * @param[in] inFactory Factory for the child.
         *
         * @return Pointer to created scalar port ref.
         **/
            virtual VectorPortReferenceSharedPtr
            newVectorPortReferencePtr( const InstanceSharedPtr &inInstancePtr,
                const PortSharedPtr &inPortPtr,
                const std::vector<size_t> &inLimits,
                const PortBundleReferenceSharedPtr &inParentCollection = PortBundleReferenceSharedPtr(),
                const ChildFactorySharedPtr &inFactory
                            = BaseType::ChildFactorySharedPtr(
                                    new BaseType::ChildFactory() )) throw(Error);

    };

    virtual void
    accept(BaseVisitor &inoutVisitor) throw(Error);

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
     * @exception Error Pointer to the Net object does not exist
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorPointerToItemDoesNotExist
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Pointer to the Net - <i>boost::shared_ptr</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error Net size does not match with VectorPortReference size 
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemSizeMismatch
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Net Size - <i>SizeType</i></li>
     *               <li>VectorPortReference Size - <i>SizeType</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */
    virtual Connectable::Connection
    connect(const NetSharedPtr &inNet) throw(Error);

     /**
     * Disconnect a Net from this object.
     * @note This metod can be overridden by derived classes. However, the method must call the on_connected() method after this. The sigConnected_ signal must also be invoked in the overriding method.

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
    void
    disconnect( const Connectable::Connection &inConnection
            ) throw(Error);

    using Connectable::disconnect;

    /*
     * Returns the list of connected nets. The return list consists of the following:
     * 1) Nets connected to the whole port
     * 2) Nets connected to the individual nodes of the port
     *
     * @param[out] outNets List of nets to be populated
     * @param[in] inSkipChildConnections bool Whether child connection is skiped or not
     */

    virtual void getConnectedNets(
        std::vector< NetSharedPtr > &outNets,
        bool inSkipChildConnections = false 
            ) const throw(Error);

    virtual void
    onChildCreate(
        const VectorPortBitReferenceSharedPtr &inCreatedChild
            ) const throw(Error);
    /**
     * Set master port.
     *
     * @param[in] inMaster Set the master port.
     */
    virtual void
    bindToMasterPort(const PortSharedPtr &inMaster) throw(Error);

    ~VectorPortReference() throw();

  protected:  
    VectorPortReference();

  private:    
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

};

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_VECTORPORTREFERENCE_HPP
