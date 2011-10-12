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

#ifndef TORC_GENERIC_VECTOR_PORT_BIT_REFERENCE_HPP
#define TORC_GENERIC_VECTOR_PORT_BIT_REFERENCE_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION


#include "torc/generic/om/PortReference.hpp"
#include "torc/generic/om/VectorBit.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/util/Error.hpp"

namespace torc { namespace generic { class BaseVisitor; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents a reference to a bit of a port.
 *
 * The VectorPortBitReference class represents a reference to  a bit of an array port.
 */
class VectorPortBitReference
    : public PortReference,
    public VectorBit<PortReference> {
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

  public:
    typedef FactoryType<VectorPortBitReference> Factory;
    typedef VisitorType<VectorPortBitReference> Visitor;

    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    virtual Connection
    connect(const NetSharedPtr &inNet) throw(Error);

    using Connectable::disconnect;

    virtual void
    disconnect(const Connection &inConnection) throw(Error);

    virtual void
    getConnectedNets(
        std::vector< NetSharedPtr > &outNets,
        bool inSkipChildConnections = false    ) const throw(Error);

    VectorPortBitReference();
    virtual
    ~VectorPortBitReference() throw();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

};

} // namespace torc::generic

} // namespace torc
#endif
