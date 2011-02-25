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

#ifndef TORC_GENERIC_VIEW_HPP
#define TORC_GENERIC_VIEW_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Commentable.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/Extern.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/Nameable.hpp"
#include "torc/generic/om/ParameterMap.hpp"
#include "torc/generic/om/ParentedObject.hpp"
#include "torc/generic/om/PropertyContainer.hpp"
#include "torc/generic/om/Renamable.hpp"
#include "torc/generic/om/SelfReferencing.hpp"
#include "torc/generic/om/SymTab.hpp"
#include "torc/generic/om/Visitable.hpp"
#include "torc/generic/om/VisitorType.hpp"

namespace torc { namespace generic { class BaseVisitor; }  }
namespace torc { namespace generic { class Cell; }  }
namespace torc { namespace generic { class Instance; }  }
namespace torc { namespace generic { class Net; }  }
namespace torc { namespace generic { class Port; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents and EDIF View.
 *
 * The View class represents an EDIF view. A view can be conrete or extern (black-box).
 *
 * @note Only views of type NETLIST are supported.
 */
class View :
    public Commentable,
    public Extern,
    public Nameable,
    public ParentedObject<Cell>,
    public PropertyContainer,
    public Renamable,
    public SelfReferencing<View>,
    public Visitable {

#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
    friend class RestoredViewUpdater;
#endif //GENOM_SERIALIZATION

    friend class FactoryType<View>; 

  public:
    /**
     * @enum Type
     * Type of the view
     */
    enum Type
    {
        eTypeMaskLayout = 0,
        eTypePCBLayout,
        eTypeNetlist,
        eTypeSchematic,
        eTypeSymbolic,
        eTypeBehavior,
        eTypeLogicModel,
        eTypeDocument,
        eTypeGraphic,
        eTypeStranger
    };

    /**
     * Convenience
     */
    typedef VisitorType<View> Visitor;

    /**
     * Convenience class to create a view.
     */
    class Factory: public FactoryType<View> 
    {
        public:
            using FactoryType<View>::create;
        /**
         * Create a view
         *
         * @param[in] inName Name of the view to be created.
         * @param[in] inCellPtr Pointer to parented(Cell) object.
         * @param[in] inViewType View Type. 
         * @param[in] inOriginalName Original name of the view [optional]. 
         *
         * @return Pointer to created view.
         **/
            virtual ViewSharedPtr
            newViewPtr( const std::string &inName,
                const CellSharedPtr &inCellPtr,
                const View::Type &inViewType = View::eTypeNetlist,
                const std::string &inOriginalName = std::string()) throw(Error); 
    };
    
    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    /**
      * Returns the context that will be used to get parameters from
      * the parameter map. For an unbound instance,
      * this will return an invalid value
      */
    inline ParameterContext
    getParameterContext() const throw();

    /**
     * Get the ParameterMap corresponding to this cell
     *
     * @return A ParameterMap consisiting of only the overridden parameters.
     */
    ParameterMapSharedPtr
    getParameters() throw(Error);

    void
    setParameters(
        const ParameterMapSharedPtr &inSource) throw();

    /**
     * Get the type of view.
     *
     * @return Type of view
     */
    inline const Type
    getType() const throw();

    /**
     * Set the type of view
     *
     * @param[in] inSource Type of view
     */
    void
    setType(const Type & inSource) throw();

    /**
     * Get the list of instances for this view.
     *
     * @param[out] outInstances List of instances to be appended to
     */
    inline void
    getInstances(
        std::vector< InstanceSharedPtr > &outInstances
        ) const throw();

    /**
     * Set the list of instances to this view. It will lead to a linear traversal on the list. So usage of this API is not recommended.
     *
     * @param[in] inSource List of instances.
     */
    void
    setInstances(
        const std::vector< InstanceSharedPtr > &inSource) throw(Error);

    /**
     * Add an instance to the list of instances. Empty pointer is ignored.
     *
     * @param[in] inInstance Pointer to instance to be added.
     *
     * @exception Error Could not add instance because pointer to the Instance does not exist
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorPointerToItemDoesNotExist
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Pointer to Instance - <i>boost::shared_ptr</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error Could not add instance because Instance name is empty
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Instance name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error Could not add instance because Instance name already exists 
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemAlreadyExists
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Instance name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */
    void
    addInstance(const InstanceSharedPtr &inInstance) throw(Error);

    /**
     * Find an instance by name, in the list of instances.
     *
     * @param[in] inName String inSource specifying the name of the instance.
     *
     * @return A pointer to the instance if found, an empty pointer otherwise.
     *
     * @exception Error Empty Instance name 
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Instance name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */
    InstanceSharedPtr
    findInstance(const std::string &inName) throw();

    /**
     * Remove the specified instance from the list of cells.
     *
     * @param inName Name of the object to be delete
     *
     * @exception Error Empty Instance name
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Instance name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error Instance name not preset in collection.
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemNotFound
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Instance name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */
    void
    removeInstance( const std::string &inName) throw(Error);

   /**
    * Apply action on all Instances.
    * @param[in] action Action to be applied
    *
    */
    template<typename _Action>
    inline void
    applyOnAllInstances( const _Action &action ) throw(Error); 

    /**
     * Get the list of nets for this view.
     *
     * @param[out] outNets List of nets to be appended to
     */
    inline void
    getNets(
        std::vector< NetSharedPtr > &outNets
            ) const throw();

    /**
     * Set the list of nets to this view. It will lead to a linear traversal on the list. So usage of this API is not recommended.
     *
     * @param[in] inSource List of nets.
     */
    void
    setNets(const std::vector< NetSharedPtr > & inSource) throw(Error);

    /**
     * Add a net to the list of nets. Empty pointer is ignored.
     *
     * @param[in] inNet Pointer to net to be added.
     *
     * @exception Error Could not add Net, because Net name is empty
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Net name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error Could not add Net, because Net name already exists
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemAlreadyExists
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Net name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */
    void
    addNet(const NetSharedPtr & inNet) throw(Error);

    /**
     * Find a net by name, in the list of net.
     *
     * @param[in] inName String inSource specifying the name of the Net
     *
     * @return A pointer to the net if found, an empty pointer otherwise.
     *
     * @exception Error Empty Net name
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Net name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */
    NetSharedPtr
    findNet(const std::string &inName) throw();

    /**
     * Remove the specified net from the list of nets. If an empty pointer is passed, it returns without doing anything
     *
     * @param inName Name of the object to be delete
     *
     * @exception Error Empty Net name
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Net name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error Net not preset in collection.
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemNotFound
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Net name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */
    void
    removeNet(const std::string &inName) throw(Error);

   /**
    * Apply action on all Nets.
    * @param[in] action Action to be applied
    *
    */
    template<typename _Action>
    inline void
    applyOnAllNets( const _Action &action ) throw(Error); 

    /**
     * Get the list of ports for this view.
     *
     * @param[out] outPorts List of ports to be appended to
     */
    inline void
    getPorts(
        std::vector< PortSharedPtr > &outPorts
        ) const throw();

    /**
     * Set the list of ports to this view. It will lead to a linear traversal on the list. So usage of this API is not recommended.
     *
     * @param[in] inSource List of ports
     */
    void
    setPorts(const std::vector< PortSharedPtr > & inSource) throw(Error);

    /**
     * Add a port to the list of ports. Empty pointer is ignored.
     *
     * @param[in] inPort Pointer to port to be added.
     *
     * @exception Error Could not add port, because Port name is empty
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Port name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error Could not add port, because Port name already exists
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemAlreadyExists
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Port name - <i>View</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */

    void
    addPort(const PortSharedPtr & inPort) throw(Error);

    /**
     * Find a port by name, in the list of ports.
     *
     * @param[in] inName String inSource specifying the name of the port.
     *
     * @return A pointer to the port if found, an empty pointer otherwise.
     *
     * @exception Error Empty Port name
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Port Name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */

    PortSharedPtr
    findPort(const std::string &inName) throw();

    /**
     * Remove the specified port from the list of ports.
     *
     * @param inName Name of the port to be removed
     *
     * @exception Error Empty Port name 
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Port name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error Port not preset in collection.
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemNotFound
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Port name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */
    void
    removePort(const std::string &inName) throw(Error);

   /**
    * Apply action on all Ports.
    * @param[in] action Action to be applied
    *
    */
    template<typename _Action>
    inline void
    applyOnAllPorts( const _Action &action ) throw(Error); 
    virtual ~View() throw();

    inline const std::string &
    getNonNetlistViewData() const throw();

    void
    setNonNetlistViewData( const std::string &inData) throw();

  protected:  
    View();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    load( Archive &ar, unsigned int );

    template<class Archive> void
    save( Archive &ar, unsigned int ) const;

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void
    restoreActions() throw(Error);    
#endif //GENOM_SERIALIZATION

    ParameterMapSharedPtr mParameters;
    ParameterContext mMyContext;
    SymTab< std::string, InstanceSharedPtr > mInstanceSymTab;
    SymTab< std::string, NetSharedPtr > mNetSymTab;
    SymTab< std::string, PortSharedPtr > mPortSymTab;
    std::string mNonNetlistViewData;
    Type mType;
};

inline ParameterContext
View::getParameterContext() const throw() {
    return mMyContext;
}

/**
 * Get the type of view.
 *
 * @return Type of view
 */
inline const View::Type
View::getType() const throw() {
  return mType;
}

inline void
View::getInstances(
    std::vector< InstanceSharedPtr > &outInstances
        ) const throw() {
  return mInstanceSymTab.getValues( outInstances );
}


template<typename _Action>
inline void
View::applyOnAllInstances( const _Action &action ) throw(Error)
{
    try
    {
        mInstanceSymTab.applyOnAll( action );
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

inline void
View::getNets(
    std::vector< NetSharedPtr > &outNets
        ) const throw() {
  return mNetSymTab.getValues( outNets );
}

template<typename _Action>
inline void
View::applyOnAllNets( const _Action &action ) throw(Error)
{
    try
    {
        mNetSymTab.applyOnAll( action );
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

inline void
View::getPorts(
    std::vector< PortSharedPtr > &outPorts
        ) const throw() {
  return mPortSymTab.getValues( outPorts );
}

template<typename _Action>
inline void
View::applyOnAllPorts( const _Action &action ) throw(Error)
{
    try
    {
        mPortSymTab.applyOnAll( action );
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

inline const std::string &
View::getNonNetlistViewData() const throw() {
    return mNonNetlistViewData;
}
} // namespace torc::generic

} // namespace torc
#endif
