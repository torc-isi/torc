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

#ifndef TORC_GENERIC_OM_INSTANCEARRAY_HPP
#define TORC_GENERIC_OM_INSTANCEARRAY_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

#include <algorithm>

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Instance.hpp"
#include "torc/generic/om/Vector.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/Composite.hpp"
#include <vector>

#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/InstanceArrayMember.hpp"

namespace torc { namespace generic { class BaseVisitor; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents an array of instances.
 *
 * The InstanceArray class represents an array of instances. They can be created using the (instance (array ...) ... ) syntax.
 */
class InstanceArray :
    public Instance,
    public Vector<Instance, InstanceArrayMember,
                InstanceArrayMember::Factory, false> {
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

    friend class FactoryType<InstanceArray>;

  public:
    typedef Vector<Instance, InstanceArrayMember,
                InstanceArrayMember::Factory, false> BaseVectorType;

    /**
     * Convenience class to create an instance array.
     */
    class Factory: public FactoryType<InstanceArray>
    {
        public:
            using FactoryType<InstanceArray>::create;
        /**
         * Create an instance array
         *
         * @param[in] inName Name of the instance array to be created.
         * @param[in] inViewPtr Pointer to parented(view) object.
         * @param[in] inMaster Pointer to master object.
         * @param[in] inSize Size of the instance array.
         * @param[in] inFactory Factory for the child.
         * @param[in] inOriginalName Original name of the instance array [optional].
         *
         * @return Pointer to created instance array.
         **/
            virtual InstanceArraySharedPtr
            newInstanceArrayPtr( const std::string &inName,
                const ViewSharedPtr &inViewPtr,
                const ViewSharedPtr &inMaster,
                const size_t &inSize,
                const ChildFactorySharedPtr &inFactory
                            = BaseVectorType::ChildFactorySharedPtr(
                                                new BaseVectorType::ChildFactory() ),
                const std::string &inOriginalName = std::string()) throw(Error);

        /**
         * Create an instance array
         *
         * @param[in] inName Name of the instance array to be created.
         * @param[in] inViewPtr Pointer to parented(view) object.
         * @param[in] inMaster Pointer to master object.
         * @param[in] inLimits Dimensions of the vector.
         * @param[in] inFactory Factory for the child.
         * @param[in] inOriginalName Original name of the instance array [optional].
         *
         * @return Pointer to created instance array.
         **/
            virtual InstanceArraySharedPtr
            newInstanceArrayPtr( const std::string &inName,
                const ViewSharedPtr &inViewPtr,
                const ViewSharedPtr &inMaster,
                const std::vector<size_t> &inLimits,
                const ChildFactorySharedPtr &inFactory
                            = BaseVectorType::ChildFactorySharedPtr(
                                                new BaseVectorType::ChildFactory() ),
                const std::string &inOriginalName = std::string()) throw(Error);
    };

    typedef VisitorType<InstanceArray> Visitor;

    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    virtual void
    setParent( const ViewSharedPtr &inParent ) throw();

    virtual void
    bindToMasterView(
        const ViewSharedPtr &inMaster,
        bool inMapPortReferences = true    ) throw(Error);
    /**
     * Add a port reference to this master.
     *
     * @param[in] inPortRef A port reference.
     *
     * @exception Error The method is unsupoorted for this class.
     */
    virtual void
    addPortReference(
        const PortReferenceSharedPtr &inPortRef) throw(Error);

    /**
     * Find a port reference.
     *
     * @param[in] inName String containing the name of the port.
     * @exception Error The method is unsupoorted for this class.
     */
    virtual PortReferenceSharedPtr
    findPortReference(const std::string &inPortRef) throw(Error);

    /**
     * Remove a given port reference.
     *
     * @param inName Name of the object to be delete
     *
     * @exception Error The method is unsupoorted for this class.
     */
    virtual void
    removePortReference(const std::string &inName) throw(Error);

    /**
     * Get list of all port references.
     *
     * @return List of port references

     */
    virtual void
    getPortReferences(
        std::vector< PortReferenceSharedPtr > &) const throw();

    /**
     * Set list of all port references.
     *
     * @param[in] inSource List of port references
     * @exception Error The method is unsupoorted for this class.
     */
    virtual void
    setPortReferences(
        const std::vector< PortReferenceSharedPtr > & inSource) throw(Error);

    /**
     * Flatten an instance so that internal references
     */
    virtual void
    flatten() throw(Error);

   /**
    * Apply action on all bits of the array.
    * @param[in] action Action to be applied
    *
    */
    template<typename _Action>
    inline void
    applyOnAllBits( const _Action &action ) throw(Error); 

    virtual
    ~InstanceArray() throw();

  protected:
    virtual void
    onChildCreate( const boost::shared_ptr<BaseVectorType::ChildType> &inCreatedChild
            ) const throw(Error);

    InstanceArray();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

};

/**
 * Apply action on all bits.
 * @param[in] action Action to be applied
 *
 */
template<typename _Action>
inline void
InstanceArray::applyOnAllBits( const _Action &action ) throw(Error)
{
    try
    {
        std::vector< InstanceSharedPtr > children;
        getChildren( children );
        std::for_each( children.begin(), children.end(), action );
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_INSTANCEARRAY_HPP
