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

#ifndef TORC_GENERIC_VECTOR_NET_HPP
#define TORC_GENERIC_VECTOR_NET_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Net.hpp"
#include "torc/generic/om/Vector.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/VectorNetBit.hpp"

namespace torc { namespace generic { class BaseVisitor; }  } 

namespace torc {

namespace generic {

/**
 * @brief Represents a net array.
 * 
 * The VectorNet class represents a net array. Such objects in EDIF are declard using the (net  (array ...) ) syntax.
 */
class VectorNet
    : public Net,
    public Vector<Net, VectorNetBit, VectorNetBit::Factory, true>
{
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

    friend class FactoryType<VectorNet>;

  public:
    typedef Vector<Net, VectorNetBit, VectorNetBit::Factory, true> BaseVectorType;
    typedef VisitorType< VectorNet > Visitor;

    /**
     * Convenience class to create a vector net.
     */
    class Factory: public FactoryType<VectorNet>
    {
        public:
            using FactoryType<VectorNet>::create;
        /**
         * Create a vector net.
         *
         * @param[in] inName Name of the vector net to be created.
         * @param[in] inViewPtr Pointer to parented(View) object.
         * @param[in] inSize Size of the net array.
         * @param[in] inParentCollection Pointer to parent bundle.
         * @param[in] inFactory Factory for the child.
         * @param[in] inOriginalName Original name of the vector net [optional].
         *
         * @return Pointer to created vector net.
         **/
            virtual VectorNetSharedPtr
            newVectorNetPtr( const std::string &inName,
                const ViewSharedPtr &inViewPtr,
                const size_t &inSize,
                const NetBundleSharedPtr &inParentCollection= NetBundleSharedPtr(),
                const ChildFactorySharedPtr &inFactory
                            = BaseVectorType::ChildFactorySharedPtr(
                                                new BaseVectorType::ChildFactory() ),
                const std::string &inOriginalName = std::string()) throw(Error);
         /**
         * Create a vector net.
         *
         * @param[in] inName Name of the vector net to be created.
         * @param[in] inViewPtr Pointer to parented(View) object.
         * @param[in] inLimits Dimensions of the vector.
         * @param[in] inParentCollection Pointer to parent bundle.
         * @param[in] inFactory Factory for the child.
         * @param[in] inOriginalName Original name of the vector net [optional].
         *
         * @return Pointer to created vector net.
         **/
            virtual VectorNetSharedPtr
            newVectorNetPtr( const std::string &inName,
                const ViewSharedPtr &inViewPtr,
                const std::vector<size_t> &inLimits,
                const NetBundleSharedPtr &inParentCollection = NetBundleSharedPtr(),
                const ChildFactorySharedPtr &inFactory
                            = BaseVectorType::ChildFactorySharedPtr(
                                                new BaseVectorType::ChildFactory() ),
                const std::string &inOriginalName = std::string()) throw(Error);
    };

    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    virtual void
    setParent( const ViewSharedPtr &inParent ) throw();

    /**
     * Connect a Net to this object.
     * 
     * @note This metod can be overridden by derived classes. However, the method must call the on_connected() method after this. The sigConnected_ signal must also be invoked in the overriding method.
     * 
     * @param[in] inNet A pointer to the Net object that eeds to be connected
     * @return A connection that has been established. This can be used later for disconnection.
     */
    virtual Connectable::Connection
    connect(const NetSharedPtr &inNet) throw(Error);

    /**
    * Disconnect a Net from this object.
    * @note This metod can be overridden by derived classes. However, the method must call the on_connected() method after this. The sigConnected_ signal must also be invoked in the overriding method.
    
    * @param[in] inConnection A connection as returned by the connect() method
    * @exception Error Provided connection is invalid
    */
    void
    disconnect(const Connectable::Connection &inConnection) throw(Error);

    void
    disconnect() throw(Error);

    virtual void
    getConnectedNets(
        std::vector< NetSharedPtr > &outNets,
        bool inSkipChildConnections = false ) const throw(Error);

    virtual void
    getConnectedPorts(
        std::vector< PortSharedPtr > &outPorts,
        bool inSkipChildConnections  = false) const throw();

    virtual void
    getConnectedPortRefs(
        std::vector< PortReferenceSharedPtr > &outPortRefs,
        bool inSkipChildConnections = false ) const throw();

    virtual
    ~VectorNet() throw();

  protected:
    virtual void
    onChildCreate( const boost::shared_ptr<BaseVectorType::ChildType> &inCreatedChild
            ) const throw(Error);

    VectorNet();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

};

} // namespace torc::generic

} // namespace torc
#endif
