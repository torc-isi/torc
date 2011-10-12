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

#ifndef TORC_GENERIC_SCALAR_NET_HPP
#define TORC_GENERIC_SCALAR_NET_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/Net.hpp"
#include "torc/generic/om/Scalar.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/FactoryType.hpp"

namespace torc { namespace generic { class BaseVisitor; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents a standalone net.
 *
 * The ScalarNet class represents a standalone net object. Such objects in EDIF are declard using the (net netName ... ) syntax.
 */
class ScalarNet
    : public Net,
    public Scalar<Net> {
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

    friend class FactoryType<ScalarNet>;

  public:
    typedef VisitorType<ScalarNet> Visitor;

    /**
     * Convenience class to create a scalar net.
     */
    class Factory: public FactoryType<ScalarNet>
    {
        public:
            using FactoryType<ScalarNet>::create;
        /**
         * Create a scalar net.
         *
         * @param[in] inName Name of the scalar net to be created.
         * @param[in] inViewPtr Pointer to parented(View) object.
         * @param[in] inParentCollection Pointer to parent bundle.
         * @param[in] inOriginalName Original name of the scalar net [optional].
         *
         * @return Pointer to created scalar net.
         **/
            virtual ScalarNetSharedPtr
            newScalarNetPtr( const std::string &inName,
                const ViewSharedPtr &inViewPtr,
                const NetBundleSharedPtr &inParentCollection = NetBundleSharedPtr(),
                const std::string &inOriginalName = std::string()) throw(Error);
    };
    
    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

	/**s
     * @exception Error Pointer to the Net object does not exists
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorPointerToItemDoesNotExist
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Pointe to Net object - <i>boost::shared_ptr</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error  Net size does not match with connectable object size
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemSizeMismatch
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Net Size - <i>SizeType</i></li>
     *               <li>Connectable object Size - <i>SizeType</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
	 */

    virtual Connection
    connect(const NetSharedPtr &inNet) throw(Error);

    using Net::disconnect;

    virtual void
    disconnect(const Connection &inConnection) throw(Error);

    virtual
    ~ScalarNet() throw();

  protected:
    ScalarNet();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

};

} // namespace torc::generic

} // namespace torc
#endif
