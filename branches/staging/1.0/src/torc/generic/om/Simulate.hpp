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

#ifndef TORC_GENERIC_OM_SIMULATE_HPP
#define TORC_GENERIC_OM_SIMULATE_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/Nameable.hpp"
#include "torc/generic/om/Renamable.hpp"
#include "torc/generic/om/Commentable.hpp"
#include "torc/generic/om/Visitable.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/SelfReferencing.hpp"
#include "torc/generic/om/SymTab.hpp"
#include "torc/generic/om/UserDataContainer.hpp"
#include "torc/generic/om/PortListAlias.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/util/Error.hpp"

#include <vector>

namespace torc { namespace generic { class BaseVisitor; }  }
namespace torc { namespace generic { class Apply; }  } 
namespace torc { namespace generic { class WaveValue; }  } 

namespace torc {

namespace generic {

/**
 * @brief This class is to model simulate construct which is a named collection 
 * of simulation stimulus and responses statements and is used in the interface and
 * contents of views.
 */
class Simulate 
    : public Nameable, 
    public Renamable,
    public Commentable, 
    public Visitable, 
    public SelfReferencing<Simulate>,
    public UserDataContainer
{
   friend class FactoryType<Simulate>;
    
  public:
    /**
     * Convenience class to visit a simulate
     */
    typedef VisitorType<Simulate> Visitor;

    /**
     * Convenience class for creating a simulate
     */
    class Factory: public FactoryType<Simulate>
    {
        public:
            using FactoryType<Simulate>::create;
        /**
         * Create a simulate.
         *
         * @param[in] inName Name of the simulate to be created.
         * @param[in] inPortListAliases Vector of port list aliases to this simulate.
         * @param[in] inAllApply Vector of apply to this simulate.
         * @param[in] inWaveValues Vector of WaveValues to this simulate.
         * @param[in] inView Pointer to parented (view) object.
         * @param[in] inOriginalName Original name of the simulate [optional].
         * @param[in] inInterfaceAttributes Pointer to 
         *  parented (InterfaceAttributes) object [optional]. If mentioned then this
         *  will decompile within (contents ...) construct.
         *
         * @return Pointer to created simulate.
         */
         SimulateSharedPtr
         virtual newSimulatePtr( const std::string & inName,
                const std::vector< PortListAliasSharedPtr > & inPortListAliases,
                const std::vector< ApplySharedPtr > & inAllApply,
                const std::vector< WaveValueSharedPtr > & inWaveValues,
                const ViewSharedPtr & inView,
                const std::string &inOriginalName = std::string(),
                const InterfaceAttributesSharedPtr & inInterfaceAttributes
                            = InterfaceAttributesSharedPtr() ) throw(Error);
    };

    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    /**
     * Add a port list alias to the symbol table of port list alias.
     * If an empty pointer is supplied, it returns without doing anything.
     *
     * @param[in] inPortListAlias A pointer to a port list alias object.
     *
     * @exception Error PortListAlias could not be added, empty PortListAlias name
     * @exception Error PortListAlias already exists in simulate
     */
    void 
    addPortListAlias(const PortListAliasSharedPtr & inPortListAlias) throw(Error);

    /**
     * Find a PortListAlias by name, in the symbol table of port list alias.
     *
     * @param[in] inName String specifying the name of the PortListAlias.
     *
     * @return A pointer to the PortListAlias if found, an empty pointer otherwise.
     */
    PortListAliasSharedPtr 
    findPortListAlias(const std::string & inName) throw();

    /**
     * Get all the PortListAlias of this simulation.
     *
     * @param[out] outPortListAliases Vector of PortListAlias to be appended to
     */
    inline void
    getPortListAliases(
        std::vector< PortListAliasSharedPtr > & outPortListAliases ) const throw();

    /**
     * Set all the PortListAlias of this simulation.
     *
     * @param[in] inPortListAliases Vector of PortListAlias to be appended to
     */
    void
    setPortListAliases( 
        const std::vector< PortListAliasSharedPtr > & inPortListAliases ) throw(Error);

    /**
     * Get all the apply objects.
     *
     * @param[out] outAllApply Vector of apply objects to be appended to
     */
    inline void 
    getAllApply( std::vector< ApplySharedPtr > & outAllApply ) const throw();

    /**
     * Set all the apply objects.
     *
     * @param[in] inAllApply Vector of apply objects to be appended to
     */
    void 
    setAllApply( const std::vector< ApplySharedPtr > & inAllApply ) throw(Error);

    /**
     * Add an apply objects to the vector of apply objects.
     *
     * @param[in] inSource Pointer to Apply object
     */
    void 
    addApply( const ApplySharedPtr & inSource ) throw(Error);

    /**
     * Get the vector of wave values.
     *
     * @param[out] outWaveValues Vector of wave values to be appended to
     */
    inline void 
    getWaveValues( std::vector< WaveValueSharedPtr > & outWaveValues ) const throw();

    /**
     * Set the vector of wave values.
     *
     * @param[in] inWaveValues Vector of wave values to be appended to
     */
    void 
    setWaveValues( const std::vector< WaveValueSharedPtr > & inWaveValues ) throw(Error);

    /**
     * Add a wave value to the symbol table of wave values.   
     *
     * @param[in] inSource Pointer to WaveValue to be appended to
     *
     * @exception Error WaveValue could not be added, empty WaveValue name
     * @exception Error WaveValue already exists in simulate
     */
    void 
    addWaveValue( const WaveValueSharedPtr & inSource ) throw(Error);

    /**
     * Find a WaveValue by name, in the symbol table of port list alias.
     *
     * @param[in] inName String specifying the name of the WaveValue.
     *
     * @return A pointer to the WaveValue if found, an empty pointer otherwise.
     */
    WaveValueSharedPtr 
    findWaveValue(const std::string & inName) throw();

   /**
    * Apply action on all PortListAlias.
    * @param[in] action Action to be applied
    *
    */
    template<typename _Action>
    inline void
    applyOnAllPortListAlias( const _Action &action ) throw(Error);

   /**
    * Apply action on all WaveValues.
    * @param[in] action Action to be applied
    *
    */
    template<typename _Action>
    inline void
    applyOnAllWaveValues( const _Action &action ) throw(Error);

    ~Simulate() throw();

  protected:
    Simulate();

  private:
    Simulate(const Simulate & source) throw();

    Simulate & 
    operator=(const Simulate & source) throw();

    SymTab< std::string, PortListAliasSharedPtr > mPortListAliasSymTab;
    SymTab< std::string, WaveValueSharedPtr > mWaveValueSymTab;
    std::vector< ApplySharedPtr > mAllApply;

};

/**
 * Get all the PortListAlias of this simulation.
 *
 * @param[out] outPortListAliases Vector of PortListAlias to be appended to
 */
inline void
Simulate::getPortListAliases(
    std::vector< PortListAliasSharedPtr > & outPortListAliases ) const throw() {
    mPortListAliasSymTab.getValues( outPortListAliases );
}

/**
 * Get all the apply objects.
 *
 * @param[out] outAllApply Vector of apply objects to be appended to
 */
inline void 
Simulate::getAllApply( std::vector< ApplySharedPtr > & outAllApply ) const throw() {
    outAllApply.insert( outAllApply.end(),
        mAllApply.begin(), mAllApply.end() );
}

/**
 * Get the vector of wave values.
 *
 * @param[out] outWaveValues Vector of wave values to be appended to
 */
inline void
Simulate::getWaveValues( std::vector< WaveValueSharedPtr > & outWaveValues ) const throw() {
        mWaveValueSymTab.getValues( outWaveValues );
}

/**
 * Apply action on all PortListAlias.
 * @param[in] action Action to be applied
 *
 */
template<typename _Action>
inline void
Simulate::applyOnAllPortListAlias( const _Action &action ) throw(Error) {
    try
    {
        mPortListAliasSymTab.applyOnAll( action );
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
 * Apply action on all WaveValues.
 * @param[in] action Action to be applied
 *
 */
template<typename _Action>
inline void
Simulate::applyOnAllWaveValues( const _Action &action ) throw(Error) {
    try
    {
        mWaveValueSymTab.applyOnAll( action );
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_SIMULATE_HPP
