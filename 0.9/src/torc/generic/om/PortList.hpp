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

#ifndef TORC_GENERIC_PORT_LIST_H
#define TORC_GENERIC_PORT_LIST_H

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"
#include "torc/generic/om/Connectable.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/VisitorType.hpp"

namespace torc {

namespace generic {

class PortList
    : public Connectable,
    public SelfReferencing<PortList> {

#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

    friend class FactoryType<PortList>;

  public:

    class PortListElement {
#ifdef GENOM_SERIALIZATION
        friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION


        public:
            enum ElementType {
                eElementTypePort,
                eElementTypePortReference
            };

            ElementType
            getType() const throw() {
                return mType;
            }

            PortSharedPtr
            getPort() const throw() {
                return mPort;
            }

            PortReferenceSharedPtr
            getPortReference() const throw() {
                return mPortReference;
            }
        
        public:
            PortListElement( const PortSharedPtr &inPort )
                :mType( eElementTypePort ),
                mPort( inPort ),
                mPortReference() {
            }
            PortListElement(
                    const PortReferenceSharedPtr &inPortRef )
                :mType( eElementTypePortReference ),
                mPort( ),
                mPortReference( inPortRef ) {
            }

            PortListElement( const PortListElement &inRhs )
                :mType( inRhs.mType ),
                mPort( inRhs.mPort ),
                mPortReference( inRhs.mPortReference ) {
            }

            PortListElement &
            operator =( const PortListElement &inRhs ) {
                if( this == &inRhs )
                {
                    mType = inRhs.mType;
                    mPort = inRhs.mPort;
                    mPortReference = inRhs.mPortReference;
                }
                return *this;
            }

        private:
            PortListElement()
                :mType( eElementTypePort ),
                mPort(),
                mPortReference() {
            }

#ifdef GENOM_SERIALIZATION
            template<class Archive> void
            serialize( Archive &ar, unsigned int );
#endif
            ElementType mType;
            PortSharedPtr mPort;
            PortReferenceSharedPtr mPortReference;    
    };
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
         * @return Pointer to created single instance.
         */
            virtual PortListSharedPtr
            newPortListPtr( ) throw(Error);
    };

    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

     virtual Connection
    connect(const NetSharedPtr & inNet) throw(Error);

    using Connectable::disconnect;

    virtual void
    disconnect(const Connection & inConnection) throw(Error);

    size_t
    getSize() const throw();

    void
    addChildPort( const PortSharedPtr &inPort ) throw();

    void
    addChildPortReference(
            const PortReferenceSharedPtr &inPortRef ) throw();

    void
    getChildren( std::list< PortListElement > &outPorts ) throw();

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
#endif
