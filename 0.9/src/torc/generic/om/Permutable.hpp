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

#ifndef TORC_GENERIC_OM_PERMUTABLE_HPP
#define TORC_GENERIC_OM_PERMUTABLE_HPP

#include <vector>

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/SelfReferencing.hpp"
#include "torc/generic/om/Visitable.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/FactoryType.hpp"

namespace torc { namespace generic { class Port; }  } 
namespace torc { namespace generic { class BaseVisitor; }  }

namespace torc {

namespace generic {

/**
 * @brief  Permutable is used to describe a relationship in which ports are interchangeable. 
 * 
 * It may reference individual ports, arrayed ports, bundled ports, permutable and nonpermutable
 * statements. 
 */
class Permutable :
    public Visitable,
    public SelfReferencing<Permutable> 
{

    friend class FactoryType<Permutable>;

  public:

    /**
     * @enum PermutableType
     *
     * Type of the permutable, Is it parent or child.
     */
    enum PermutableType
    {
        ePermutableParent = 0,
        ePermutableChild
    };

    /**
     * Convenience class to visit a permutable.
     */
    typedef VisitorType<Permutable> Visitor;

    /**
     * Convenience class to create a permutable.
     */
    class Factory: public FactoryType<Permutable>
    {
        public:
        using FactoryType<Permutable>::create;
        /**
         * Create a permutable.
         *
         * @param[in] inPorts Vector of ports to this permutable.
         * @param[in] inViewPtr Pointer to parented(View) object.
         * @param[in] inParentPermutable Pointer to parent permutable.
         *
         * @return Pointer to created permutable.
         */
         PermutableSharedPtr
         virtual newPermutablePtr( const std::vector< PortSharedPtr > & inPorts,
                const ViewSharedPtr & inViewPtr,
                const PermutableSharedPtr & inParentPermutable 
                        = PermutableSharedPtr() ) throw(Error);
    };

    /**
     * Get the list of ports for this permutable.
     *
     * @param[out] outPorts Vector of ports to be appended to
     */
    inline void
    getPorts( std::vector< PortSharedPtr > &outPorts ) const throw();

    /**
     * Set the list of ports to this permutable. 
     * It will lead to a linear traversal on the list. 
     * So usage of this API is not recommended.
     *
     * @param[in] inSource Vector of ports
     */
    void
    setPorts(const std::vector< PortSharedPtr > & inSource) throw(Error);

     /**
     * Add a port to the list of ports. Empty pointer is ignored.
     *
     * @param[in] inPort Pointer to port to be added.
     *
     * @exception Error Could not add port, because Port name is empty
    */
    void
    addPort(const PortSharedPtr & inPort) throw(Error);

    /**
     * Get the nested permutable.
     *
     * @param[out] outPermutables Vector of permutable to be appended to
     */
    inline void
    getChildren( 
        std::vector< PermutableSharedPtr > & outPermutables ) const throw();

   /**
    * Apply action on all children.
    * @param[in] action Action to be applied
    *
    */
    template<typename _Action>
    inline void
    applyOnAllChildren( const _Action &action ) throw(Error);

    /**
     * Set the nested permutable.
     *
     * @param[in] inSource Vector containing permutables
     */
    void
    setChildren( const std::vector< PermutableSharedPtr > 
         & inSource ) throw(Error);

    /**
     * Add a nested permutable/nonpermutable to parent permutable.
     *
     * @param[in] inPermutable Nested permutable/nonpermutable
     */
    bool
    addChildPermutable(
        const PermutableSharedPtr & inPermutable) throw(Error);

    /**
     * Get the boolean flag whether the current object is permutable/nonpermutable.
     *
     * @return Boolean flag
    */
    inline const bool 
    getIsNonPermutable() const throw();

    void 
    setIsNonPermutable(const bool & value) throw();

    /**
     * Get the permutable type.
     *
     * @return PermutableType  
     */
    inline const PermutableType
    getPermutableType() const throw();    

    /**
     * Set the permutable type.
     *
     * @param[in] inSource PermutableType
     */
    void
    setPermutableType(const PermutableType & inSource) throw();

    /**
     * Get the total number of bits of the composition
     * @return Number of bits
     */
    size_t
    getSize() const throw();
    
    virtual
    ~Permutable() throw();

    /**
     * Recive a visitor to this class. The visit method of the visitor is called
     * and a reference to this object is passed as a parameter. It has to be noted however,
     * that a dynamic_cast is performed inside this method. If the cast fails,
     * an appropriate exception is thrown by this method. This sitation can arise when
     * the passed Visitor object does not inherit from the appropriate visitor specialization.
     * See Visitor documentation for more details.
     *
     * @param[in,out] visitor A reference to the visitor object
     * @exception Error Visitor type inappropriate for visiting this object
     * or any other error thrown by the Visitor::throw() method.
     */
    virtual void
    accept(BaseVisitor & visitor) throw(Error);

  protected:
    Permutable();

  private:
    std::vector< PortSharedPtr > mPorts;
    std::vector< PermutableSharedPtr > mChildren;
    bool mIsNonPermutable;
    PermutableType mPermutableType;

};

/**
 * Get the list of ports for this permutable.
 *
 * @param[out] outPorts Vector of ports to be appended to
 */
inline void
Permutable::getPorts(
        std::vector< PortSharedPtr > &outValues ) const throw() {
    outValues.insert( outValues.end(),
                mPorts.begin(), mPorts.end() );
}

/**
 * Get the nested permutable.
 *
 * @param[out] outPermutables Vector of permutable to be appended to
 */
inline void
Permutable::getChildren(
        std::vector< PermutableSharedPtr > & outPermutables ) const throw() {
    outPermutables.insert( outPermutables.end(),
                mChildren.begin(), mChildren.end() );
}

/**
 * Apply action on all children.
 * @param[in] action Action to be applied
 *
 */
template<typename _Action>
inline void
Permutable::applyOnAllChildren( const _Action &action ) throw(Error) {
    try
    {
        std::vector< PermutableSharedPtr >::iterator it = mChildren.begin();
        for(; it != mChildren.end(); ++ it )
        {
            action( *it );
        }
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
 * Get the boolean flag whether the current object is permutable/nonpermutable.
 * @return Boolean flag
*/
inline const bool 
Permutable::getIsNonPermutable() const throw() {
    return mIsNonPermutable;
}

/**
 * Get the permutable type.
 *
 * @return PermutableType
 */
inline const Permutable::PermutableType
Permutable::getPermutableType() const throw() {
    return mPermutableType;
}

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_PERMUTABLE_HPP
