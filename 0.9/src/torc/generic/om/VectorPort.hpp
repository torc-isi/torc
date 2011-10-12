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

#ifndef TORC_GENERIC_OM_VECTORPORT_HPP
#define TORC_GENERIC_OM_VECTORPORT_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Port.hpp"
#include "torc/generic/om/Vector.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/Connectable.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/VectorPortBit.hpp"

namespace torc { namespace generic { class Net; }  }
namespace torc { namespace generic { class BaseVisitor; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents a port array.
 *
 * The VectorPort class represents a port array. Such objects in EDIF are declard using the (port  (array ...) ) syntax.
 */
class VectorPort
    : public Port,
    public Vector<Port, VectorPortBit,
                VectorPortBit::Factory, true>
{
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

    friend class FactoryType<VectorPort>;

  public:
    typedef Vector<Port, VectorPortBit,
                VectorPortBit::Factory, true> BaseVectorType;

    typedef VisitorType<VectorPort> Visitor;

    /**
     * Convenience class to create a vector port.
     */
    class Factory: public FactoryType<VectorPort>
    {
        public:
            using FactoryType<VectorPort>::create;
        /**
         * Create a vector port.
         *
         * @param[in] inName Name of the vector port to be created.
         * @param[in] inDirection Direction of port.
         * @param[in] inViewPtr Pointer to parented(View) object.
         * @param[in] inSize Size of the port array.
         * @param[in] inParentCollection Pointer to parent bundle.
         * @param[in] inFactory Factory for the child.
         * @param[in] inOriginalName Original name of the vector port [optional].
         *
         * @return Pointer to created vector port.
         **/
            virtual VectorPortSharedPtr
            newVectorPortPtr( const std::string &inName,
                const PortDirection &inDirection,
                const ViewSharedPtr &inViewPtr,
                const size_t &inSize,
                const PortBundleSharedPtr &inParentCollection = PortBundleSharedPtr(),
                const ChildFactorySharedPtr &inFactory
                            = BaseVectorType::ChildFactorySharedPtr(
                                                new BaseVectorType::ChildFactory() ),
                const std::string &inOriginalName = std::string()) throw(Error);
        /**
         * Create a vector port.
         *
         * @param[in] inName Name of the vector port to be created.
         * @param[in] inDirection Direction of port.
         * @param[in] inViewPtr Pointer to parented(View) object.
         * @param[in] inLimits Dimensions of the vector.
         * @param[in] inParentCollection Pointer to parent bundle.
         * @param[in] inFactory Factory for the child.
         * @param[in] inOriginalName Original name of the vector port [optional].
         *
         * @return Pointer to created vector port.
         **/
            virtual VectorPortSharedPtr
            newVectorPortPtr( const std::string &inName,
                const PortDirection &inDirection,
                const ViewSharedPtr &inViewPtr,
                const std::vector<size_t> &inLimits,
                const PortBundleSharedPtr &inParentCollection = PortBundleSharedPtr(),
                const ChildFactorySharedPtr &inFactory
                            = BaseVectorType::ChildFactorySharedPtr(
                                                new BaseVectorType::ChildFactory() ),
                const std::string &inOriginalName = std::string()) throw(Error);
    };

    virtual void
    setParent( const ViewSharedPtr &inParent ) throw();

    virtual void
    accept(BaseVisitor &inoutVisitor) throw(Error);

    virtual Connectable::Connection
    connect(const NetSharedPtr &inNet) throw(Error);

    using Connectable::disconnect;

    void
    disconnect(
        const Connectable::Connection &inConnection
            ) throw(Error);

    /*
     * Returns the list of connected nets. The return list consists of the following:
     * 1) Nets connected to the whole port
     * 2) Nets connected to the individual nodes of the port
     *
     * param[out] outNets List of nets to be populated
     */

    virtual void getConnectedNets(
        std::vector< NetSharedPtr > &outNets,
        bool inSkipChildConnections = false 
            ) const throw(Error);

    virtual
    ~VectorPort() throw();

  protected:
    virtual void
    onChildCreate( const boost::shared_ptr<BaseVectorType::ChildType> &inCreatedChild
            ) const throw(Error);

    VectorPort();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

};

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_VECTORPORT_HPP
