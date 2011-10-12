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

#ifndef TORC_GENERIC_OM_PORTLISTALIAS_HPP
#define TORC_GENERIC_OM_PORTLISTALIAS_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/Nameable.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/SelfReferencing.hpp"
#include "torc/generic/om/VisitorType.hpp"

namespace torc { namespace generic { class PortList; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents an ordered list of port references with a name aliased.
 *
 * The PortList class represents an ordered list of port references. 
 * Such objects in EDIF are declard using the (portList portRefName1 portRefName2 ... ) syntax.
 */

class PortListAlias
    : public Nameable,
    public SelfReferencing<PortListAlias> {

    friend class FactoryType<PortListAlias>;

  public:
    
    /**
     * Convenience class to visit a port list alias.
     */
    typedef VisitorType<PortListAlias> Visitor;

    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    /**
     * Convenience class to create a port list alias.
     */
    class Factory: public FactoryType<PortListAlias>
    {
        public:
            using FactoryType<PortListAlias>::create;
        /**
         * Create a port list alias
         *
         * @param[in] inName Name of the port list alias to be created.
         * @param[in] inPortList Pointer to port list object.
         * @param[in] inSimulate Pointer to parented (Simulate) object [optional].
         *            If not mentioned PortListAlias will not be added to simulate.
         *
         * @return Pointer to created port list alias.
         **/
         virtual PortListAliasSharedPtr
         newPortListAliasPtr( const std::string & inName,
                const PortListSharedPtr & inPortList,
                const SimulateSharedPtr & inSimulate
                            = SimulateSharedPtr() ) throw(Error);
    };

    /**
     * Get the portList accociated with this PortListAlias.
     *
     * @return Pointer to PortList 
     */
    inline const PortListSharedPtr
    getPortList() const throw(); 

    /**
     * Set the portList accociated with this PortListAlias.
     *
     * @param[in] inPortList Pointer to PortList
     */
    void 
    setPortList( const PortListSharedPtr & inPortList ) throw();
   
    ~PortListAlias() throw();

  protected:  
    PortListAlias();

  private:
    PortListSharedPtr mPortList;    
};

inline const PortListSharedPtr
PortListAlias::getPortList() const throw() {
    return mPortList;
}

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_PORTLISTALIAS_HPP
