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

#ifndef TORC_GENERIC_OM_INSTANCE_HPP
#define TORC_GENERIC_OM_INSTANCE_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Commentable.hpp"
#include "torc/generic/om/Composite.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/Nameable.hpp"
#include "torc/generic/om/ParameterMap.hpp"
#include "torc/generic/om/ParentedObject.hpp"
#include "torc/generic/om/PropertyContainer.hpp"
#include "torc/generic/om/Renamable.hpp"
#include "torc/generic/om/SymTab.hpp"
#include "torc/generic/om/Visitable.hpp"
#include "torc/generic/om/View.hpp"
#include "torc/generic/om/UserDataContainer.hpp"

namespace torc { namespace generic { class ObjectFactory; }  }
namespace torc { namespace generic { class PortReference; }  }
namespace torc { namespace generic { class RestoredInstanceUpdater; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents an instantiation of a cell view in the view of another cell
 *
 * The Instance class is used to represent the use of a cell in another cell. EDIF supoorts single instances and instance arrays. The Intstance acts as a base class for these the acutal scalar and vector representations of instances. Parameters defined in a cell declaration can be overridden in an Instance.
 */
class Instance :
    virtual public Composite<Instance>,
    public Commentable,
    public Nameable,
    public PropertyContainer,
    public Renamable,
    public Visitable,
    public ParentedObject<View>,
    public UserDataContainer {
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
    friend class RestoredInstanceUpdater;
#endif //GENOM_SERIALIZATION

  public:
    struct MasterData {
        std::string mLibrary;
        std::string mCell;
        std::string mView;
        std::map< std::string,ParameterSharedPtr > mParams;

        template<class Archive> void
        serialize( Archive &ar, unsigned int ) {
            ar & mLibrary;
            ar & mCell;
            ar & mView;
            ar & mParams;
        }
    };

    /**
      * Returns the context that will be used to get parameters from
      * the parameter map. For an unbound instance,
      * this will return an invalid value
      */
    virtual ParameterContext
    getParameterContext() const throw();

    /**
     * Get the ParameterMap corresponding to this instance and it's master
     *
     * @return A ParameterMap consisiting of only the overridden parameters.
     */
    virtual ParameterMapSharedPtr
    getParameters() const throw(Error);

    /**
     * Get the master view for this instance.
     *
     * @return Master view for this instance.
     */
    inline ViewSharedPtr
    getMaster() const throw();

    /**
     * Bind an instance to it's master. Instance binding consists of two steps:
     * <ul>
     *    <li> Bind instance to cell </li>
     *    <li> Bind existing port references to ports of the cell </li>
     * </ul>
     * @note Instances can be bound in two states, namely, immediately, with no port reference bindings, or delayed, when all used port references are bound to master ports. There is no port binding for extern views.
     * @param[in] inMaster Source Master view for this instance.
     * @param[in] inMapPortReferences Whether port references are to be mapped to ports of the new master
     * @exception Error If there is any kind of mismatch between ports etc.
     */
    virtual void
    bindToMasterView(
        const ViewSharedPtr &inMaster,
		bool inMapPortReferences = true ) throw(Error);

  protected:
    void
    setMaster( const ViewSharedPtr &inMaster ) throw();

  public:    
    /**
     * Add a port reference to this master. When a master is set using the set_master(), the list of port references is not created. It has to be extrinsically set.
     *
     * @param[in] inPortRef A port reference.
     *
     * @exception Error The port ref could not be added, because PortReference name is empty
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>PortReference name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error The port ref could not be added, because PortReference name is already exists
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemAlreadyExists
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>PortReference name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     */
    virtual void
    addPortReference(
        const PortReferenceSharedPtr &inPortRef) throw(Error);

    /**
     * Find a port reference.
     *
     * @param[in] inName String containing the name of the port.
     */
    virtual PortReferenceSharedPtr
    findPortReference(const std::string &inPortRef) throw(Error);

    /**
     * Find a Net reference.
     *
     * @param[in] inName String containing the name of the Net.
     */
    virtual NetReferenceSharedPtr
    findNetReference(const std::string &inNetRef) throw(Error);

    /**
     * Remove a given port reference.
     *
     * @param inName Name of the object to be delete
     *
     * @exception Error PortReference name is empty 
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>PortReference name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error PortReference name could not be found
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemNotFound
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>PortReference name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */
    virtual void
    removePortReference(const std::string &inName) throw(Error);

    /**
     * Get list of all port references.
     *
     * @return List of port references
     */
    virtual void
    getPortReferences(std::vector< PortReferenceSharedPtr > &) const throw(); 

    /**
     * Set list of all port references.
     *
     * @param[in] inSource List of port references
     */
    virtual void
    setPortReferences(const std::vector< PortReferenceSharedPtr > & inSource) throw(Error);

   /**
    * Apply action on all PortReferences.
    * @param[in] action Action to be applied
    *
    */
    template<typename _Action>
    inline void
    applyOnAllPortReferences( const _Action &action ) throw(Error); 

    /**
     * Get the designated number.
     *
     * @return std::string Value representing designated number of a view interface.
    */
    inline const std::string
    getDesignator() const throw();

    /**
     * Set the designated number.
     *
     * @param[in] inSource std::string representing designated number of a view interface.
    */
    void
    setDesignator(const std::string & inSource) throw();

    /**
     * Get the pointer to the timing object
     *
     * @return Pointer to the timing object
     */
    inline const TimingSharedPtr
    getTiming() const throw();

    /**
     * Set the pointer to the timing object
     *
     * @param[in] inSource Pointer to the timing object
     */
    void
    setTiming(const TimingSharedPtr & inSource ) throw();    

    /**
     * Flatten an instance so that internal references
     */
    virtual void
    flatten() throw(Error);

    Instance();

    virtual
    ~Instance() throw();

  private:
    Instance(const Instance & source) throw();
    Instance &
    operator=(const Instance & source) throw();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    load( Archive &ar, unsigned int );

    template<class Archive> void
    save( Archive &ar, unsigned int ) const;

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void
    restoreMaster() throw(Error);

    mutable MasterData *mMasterData;
#endif //GENOM_SERIALIZATION

     ViewSharedPtr mMaster;
     SymTab< std::string,
        PortReferenceSharedPtr > mPortReferences;
     mutable ParameterContext mMyContext;
     std::string mDesignator;  
     TimingSharedPtr mTiming; 
     SymTab< std::string,
        NetReferenceSharedPtr > mNetReferences;
};

/**
 * Get the master view for this instance.
 *
 * @return Master view for this instance.
 */
inline ViewSharedPtr
Instance::getMaster() const throw() {
  return mMaster;
}

/**
 * Apply action on all PortReferences.
 * @param[in] action Action to be applied
 *
 */
template<typename _Action>
inline void
Instance::applyOnAllPortReferences( const _Action &action ) throw(Error)
{
    try
    {
        mPortReferences.applyOnAll( action );
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

inline const std::string
Instance::getDesignator() const throw() {
    return mDesignator;
}

/**
 * Get the pointer to the timing object
 *
 * @return Pointer to the timing object
 */
inline const TimingSharedPtr
Instance::getTiming() const throw() {
    return mTiming;
}


} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_INSTANCE_HPP
