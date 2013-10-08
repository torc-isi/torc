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

#ifndef TORC_GENERIC_OM_PORTLIST_HPP
#define TORC_GENERIC_OM_PORTLIST_HPP

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include <list>

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"
#include "torc/generic/om/Connectable.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/PortElement.hpp"

namespace torc { namespace generic { class PortElement; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents an ordered list of port references.
 *
 * The PortList class represents an ordered list of port references. 
 * Such objects in EDIF are declard using the (portList portRefName1 portRefName2 ... ) syntax.
 */

class PortList
    : public Connectable,
    public SelfReferencing<PortList> {

#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

    friend class FactoryType<PortList>;

  public:
    typedef PortElement PortListElement;

    typedef VisitorType<PortList> Visitor;

    /**
     * Convenience class to create a port list.
     */
    class Factory: public FactoryType<PortList>
    {
        public:
            using FactoryType<PortList>::create;
        /**
         * Create a port list.
         *
         * @param[in] inPorts List of ports to this composition.
         * @param[in] inPortReferences List of port references to this composition.
         *
         * @return Pointer to created port list.
         */
        virtual PortListSharedPtr
        newPortListPtr( const std::list< PortSharedPtr > & inPorts,
            const std::list< PortReferenceSharedPtr > & inPortReferences ) throw(Error);
    };

    /**
     * Recive a visitor to this class. The visit method of the visitor is called
     * and a reference to this object is passed as a parameter. It has to be noted however,
     * that a dynamic_cast is performed inside this method. If the cast fails,
     * an appropriate exception is thrown by this method. This sitation can arise when
     * the passed Visitor object does not inherit from the appropriate visitor specialization.
     * See Visitor documentation for more details.
     *
     * @param[in,out] inoutVisitor A reference to the visitor object
     * @exception Error Visitor type inappropriate for visiting this object
     * or any other error thrown by the Visitor::throw() method.
     */
    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    /**
     * Connect a Net to this object.
     *
     * @param[in] inNet A pointer to the Net object that needs to be connected
     * @return A connection that has been established. This can be used later for disconnection.
     */
    virtual Connection
    connect(const NetSharedPtr & inNet) throw(Error);

    using Connectable::disconnect;

    /**
    * Disconnect a Net from this object.
    *
    * @param[in] inConnection A connection as returned by the connect() method
    * @exception Error Provided connection is invalid
    */

    virtual void
    disconnect(const Connection & inConnection) throw(Error);

    /**
     * Get the total number of bits of the composition
     * @return Number of bits
     */
    size_t
    getSize() const throw();

    /**
     * Add a port to the port list.
     *
     * @param[in] inPort Pointer to port to be added.
     */
    void
    addChildPort( const PortSharedPtr &inPort ) throw();

    /**
     * Add a port reference to the port list.
     *
     * @param[in] inPortRef Pointer to port reference to be added.
     */
    void
    addChildPortReference(
            const PortReferenceSharedPtr &inPortRef ) throw();

    /**
     * Get all the children of this composition
     * @return[out] outPorts List containing all children 
     */
    void
    getChildren( std::list< PortListElement > &outPorts ) throw();

    virtual
    ~PortList() throw();

  protected:
    PortList();

  private:
    template<typename _Tp>
    void
    connectElementToNet( const NetSharedPtr &inNet,
        const boost::shared_ptr<_Tp> &inType,
        size_t &inoutCurrentWidth ) throw(Error);

#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

  private:
    std::list< PortListElement > mElements;
};

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_PORTLIST_HPP
