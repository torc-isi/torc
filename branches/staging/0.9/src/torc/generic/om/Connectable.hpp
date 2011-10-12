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

#ifndef TORC_GENERIC_CONNECTIBLE_HPP
#define TORC_GENERIC_CONNECTIBLE_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

#include <list>
#include <vector>

//BOOST
#include <boost/signal.hpp>
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/is_abstract.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/util/Error.hpp"

namespace torc { namespace generic { class Net; }  }

namespace torc {

namespace generic {

/**
 * @brief An object that is connectable to a Net
 *
 * The Connectable class can act as a base for objects that can connect to a Net. This interface provides event functionality for connection and disconnection. It also provides polymorphic methods to implement connectivity beween different components in a design.
 */
class Connectable
{
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

  public:
    /**
    * A connection between a net and this object
    */
    typedef std::list< NetSharedPtr >::iterator Connection;

    /**
     * A signal to indicate that a new connection has been made
     */
    typedef boost::signal<void (const NetSharedPtr &)> Connected;

    /**
     * A signal to indicate that a connection has been removed
     */
    typedef boost::signal<void (const NetSharedPtr &)> DisConnected;

  public:
    /**
     * A signal to indicate that a new connection has been made
     */
    inline Connected &
    signalNetConnected() throw();

    /**
     * A signal to indicate that a new connection has been made
     */
    inline DisConnected &
    signalNetDisconnected() throw();

    /**
     * Get the vector of Nets that are Connected to the current object. The connected elements are appended to the given vector
     *
     * @param[out] outNets A vector of Connected nets
     */
    virtual void
    getConnectedNets(
        std::vector< NetSharedPtr > &outNets,
        bool inSkipChildConnections = false ) const throw(Error);

    /**
     * Connect a Net to this object.
     *
     * @note This metod can be overridden by derived classes. However, the method must call the on_connected() method after this. The sigConnected_ signal must also be invoked in the overriding method.
     *
     * @param[in] inNet A pointer to the Net object that needs to be Connected
     * @return A connection that has been established. This can be used later for disconnection.
     */
    virtual Connection
    connect(const NetSharedPtr & inNet) throw(Error) = 0;

    /**
    * Disconnect a Net from this object.
    * @note This metod can be overridden by derived classes. However, the method must call the on_connected() method after this. The sigConnected_ signal must also be invoked in the overriding method.

    * @param[in] inConnection A connection as returned by the connect() method
    * @exception Error Provided connection is invalid
    */
    virtual void
    disconnect(const Connection & inConnection) throw(Error) = 0;

    /**
    * Disconnect the named Net from this object.
    @note This metod can be overridden by derived classes. However, the method must call the on_connected() method after this. The sigConnected_ signal must also be invoked in the overriding method.

    * @param[in] inName Name of the net to be DisConnected
    * @exception Error Provided net was not found
    */
    void
    disconnect(const std::string &inName) throw(Error);

    /**
    * Disconnect the given Net from this object.
    @note This metod can be overridden by derived classes. However, the method must call the on_disconnected() method after this. The sigDisconnected_ signal must also be invoked in the overriding method.

    * @param[in] net Pointer to a net
    * @exception Error Provided net was not found
    */
    void
    disconnect(const NetSharedPtr & net) throw(Error);

    /**
     * Disconnect all connections to this port.
     *
     */
    virtual void
    disconnect() throw(Error);

  protected:
    /**
     * A polymorphic function that is called after a net is Connected to this object
     */
    virtual void
    onConnect() throw(Error);

    /**
     * A polymorphic function that is called after a net is DisConnected from this object
     */
    virtual void
    onDisconnect() throw(Error);

    Connectable();

    virtual
    ~Connectable() throw();

  private:
    Connectable( const Connectable &rhs );

    Connectable &
    operator =( const Connectable &rhs );

#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

  private:
    std::list< NetSharedPtr > mConnectedNets;
    Connected mSigNetConnected;
    DisConnected mSigNetDisconnected;
};

/**
 * A signal to indicate that a new connection has been made
 */
inline Connectable::Connected &
Connectable::signalNetConnected() throw() {
    return mSigNetConnected;
}

/**
 * A signal to indicate that a new connection has been made
 */
inline Connectable::DisConnected &
Connectable::signalNetDisconnected() throw() {
    return mSigNetDisconnected;
}

} // namespace torc::generic

} // namespace torc

#ifdef GENOM_SERIALIZATION
BOOST_IS_ABSTRACT( torc::generic::Connectable )
#endif //GENOM_SERIALIZATION

#endif
