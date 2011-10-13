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

#ifndef TORC_GENERIC_OM_ROOT_HPP
#define TORC_GENERIC_OM_ROOT_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

#include <list>
#include <string>

//BOOST
#include <boost/shared_ptr.hpp>
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Commentable.hpp"
#include "torc/generic/om/EdifVersion.hpp"
#include "torc/generic/om/EdifLevel.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/Nameable.hpp"
#include "torc/generic/om/Renamable.hpp"
#include "torc/generic/om/SelfReferencing.hpp"
#include "torc/generic/om/SymTab.hpp"
#include "torc/generic/om/Visitable.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/UserDataContainer.hpp"
#include "torc/generic/om/StatusContainer.hpp"

#ifdef GENOM_SERIALIZATION
#include "torc/generic/om/DumpRestoreData.hpp"
#endif //GENOM_SERIALIZATION

namespace torc { namespace generic { class Library; }  }
namespace torc { namespace generic { class BaseVisitor; }  }

namespace torc {

namespace generic {

/**
 * @brief Root of the EDIF Object Model
 *
 * The Root class represents and EDIF hierarchy. All parsed EDIF files contribute to the growth of the object tree rooted here. At the top level, it provides access to the libraries present in the design.
 *
 * @note In case the tree has been programatically created, clients need to decompile this tree, a name has to be given to the tree, otherwise an exception will be generated. This name will become the design name of the decompiled output file. For parser generated trees, the design name of the last file parsed becomes the name of the root. This can obviously be changed by the client, by calling the set_name() method.
 */
class Root
    : public Commentable,
    public Nameable,
	public Renamable,
    public Visitable,
    public SelfReferencing<Root>, 
    public UserDataContainer,
    public StatusContainer {

#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

    friend class FactoryType<Root>;

  public:
    /**
     * Convenience typedef for visiting the root
     */
    typedef VisitorType<Root> Visitor;

    /**
     * Convenience class for creating a root
     */
    class Factory: public FactoryType<Root>
    {
        public:
            using FactoryType<Root>::create;
        /**
         * Create a root
         *
         * @param[in] inName Name of the root to be created.
         * @param[in] inEdifLevel Edif level.
         * @param[in] inOriginalName Original name of the root [optional].
         *
         * @return Pointer to created root.
         */
            virtual RootSharedPtr
            newRootPtr( const std::string &inName,
                const EdifLevel &inEdifLevel = eEdifLevel0,
                const std::string &inOriginalName = std::string() ) throw(Error); 
    };
    
    /**
     * Add a library to the list of libraries. If an empty pointer is supplied, it returns without doing anything.
     *
     * @param[in] inLibrary A pointer to a library object.
     *
     * @exception Error Library could not be added. because Library name is empty
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Library name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error Library could not be added. because Library name is already exists
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemAlreadyExists
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Library name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */
    virtual void
    addLibrary(const LibrarySharedPtr &inLibrary) throw(Error);

    /**
     * Remove the specified library from the list of libraries.
     *
     * @param inName Name of the object to be delete
     *
     * @exception Error Empty Library name
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Library name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error Library not preset in collection
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemNotFound
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Library name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */
    virtual void
    removeLibrary(const std::string &inName) throw(Error);

    /**
     * Find a library by name, in the list of libraries.
     *
     * @param[in] inName String specifying the name of the library.
     *
     * @return A pointer to the libary if found, an empty pointer otherwise.
     * @note If some libraries are yet to be restored, this method is not thread safe. For non-serializable mode this is thread safe. For dump mode derived classes can override this to make it thread safe by placing appropriate locks.
     */
    virtual LibrarySharedPtr
    findLibrary(const std::string &inName) throw();

    /**
     * Get the list of libraries.
     *
     * @param[out] outLibraries List of libraries contained in the root
     * @note If some libraries are yet to be restored, this method is not thread safe. For non-serializable mode this is thread safe. For dump mode derived classes can override this to make it thread safe by placing appropriate locks. In dump mode using this method will lead to restoration of all children.
     */
    virtual void
    getLibraries(
        std::vector< LibrarySharedPtr > &outLibraries) throw();

   /**
    * Apply action on all Libraries.
    * @param[in] action Action to be applied
     * @note If some libraries are yet to be restored, this method is not thread safe. For non-serializable mode this is thread safe. For dump mode derived classes can write an appropriate thread safe wrapper method that can be accesed using visitors. However, using this method in dump mode should be avoided unless under dire situations, as it will lead to restoration of all libraries under the root.
    *
    */
    template<typename _Action>
    inline void
    applyOnAllLibraries( const _Action &action ) throw(Error); 


    /**
     * Add a design to the list of designs. If an empty pointer is supplied, it returns without doing anything.
     *
     * @param[in] inDesign A pointer to a design object.
     *
     * @exception Error Design could not be added. because design name is empty
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *       </ul>
     *
     * @exception Error Design could not be added. because Library name is already exists
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemAlreadyExists
     *         </li>
     *       </ul>
     *
    */

    virtual void
    addDesign(const DesignSharedPtr &inDesign) throw(Error);

    /**
     * Remove the specified design from the list of designs.
     *
     * @param inName Name of the object to be delete
     *
     * @exception Error Empty design name
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *       </ul>
     *
     * @exception Error design not preset in collection
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemNotFound
     *         </li>
     *       </ul>
     *
     */
    virtual void
    removeDesign(const std::string &inName) throw(Error);

    /**
     * Find a design by name, in the list of designs.
     *
     * @param[in] inName String specifying the name of the design.
     *
     * @return A pointer to the design if found, an empty pointer otherwise.
     */
    virtual DesignSharedPtr
    findDesign(const std::string &inName) throw();

    /**
     * Get the list of designs.
     *
     * @param[out] outDesigns List of designs contained in the root
     */
    virtual void
    getDesigns(
        std::vector< DesignSharedPtr > &outDesigns) throw();

   /**
    * Apply action on all Designs.
    * @param[in] action Action to be applied
    *
    */
    template<typename _Action>
    inline void
    applyOnAllDesigns( const _Action &action ) throw(Error); 


    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    /**
     * Get the version of EDIF present in the tree.
     *
     * @return Version of edif
     */
    inline const EdifVersion
    getVersion() const throw();

    /**
     * Set the version of EDIF being used. This should be 2 0 0 for the current EOM version.
     *
     * @param[in] inSource An object of type EdifVersion.
     */
    void
    setVersion(const EdifVersion & inSource) throw();

    /**
     * The the level of EDIF file.
     *
     * @return The EDIF level for this file
     */
    inline const EdifLevel
    getLevel() const throw();

    /**
     * Set the EDIF level.
     *
     * @note Current parser supports LEVEL_0 edif only.
     *
     * @param[in] inSource EdifLevel object
     */
    void
    setLevel(const EdifLevel & inSource) throw();

#ifdef GENOM_SERIALIZATION
    inline const DumpRestoreData &
    getDumpRestoreData() const throw();

    void
    setDumpRestoreData(
            const DumpRestoreData &inDumpRestoreData ) throw();
    void
    handleNewDumpRestoreData(
            const DumpRestoreData &inDumpRestoreData ) throw(Error);
#endif //GENOM_SERIALIZATION

    virtual
    ~Root() throw();

  protected:  
    Root();

  private:

#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    load( Archive &ar, unsigned int );

    template<class Archive> void
    save( Archive &ar, unsigned int ) const;

    LibrarySharedPtr
    restoreSingleLibrary( const std::string &inName ) throw(Error);

    void
    restoreAllLibraries() throw(Error);

    BOOST_SERIALIZATION_SPLIT_MEMBER()
#endif //GENOM_SERIALIZATION

    Root(const Root & source) throw();

    Root &
    operator=(const Root & source) throw();

    EdifLevel mLevel;
    SymTab< std::string, LibrarySharedPtr > mLibraries;
    SymTab< std::string, DesignSharedPtr > mDesignSymTab;
    EdifVersion mVersion;
#ifdef GENOM_SERIALIZATION
    DumpRestoreData mDumpRestoreData;
    mutable std::list< std::string > mDumpedLibraries;
#endif //GENOM_SERIALIZATION

};

/**
 * Apply action on all Libraries.
 * @param[in] action Action to be applied
 *
 */
template<typename _Action>
inline void
Root::applyOnAllLibraries( const _Action &action ) throw(Error)
{
    try
    {
#ifdef GENOM_SERIALIZATION
        restoreAllLibraries();
#endif //GENOM_SERIALIZATION
        mLibraries.applyOnAll( action );
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
 * Apply action on all Designs.
 * @param[in] action Action to be applied
 *
 */
template<typename _Action>
inline void
Root::applyOnAllDesigns( const _Action &action ) throw(Error)
{
    try
    {
        mDesignSymTab.applyOnAll( action );
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
 * Get the version of EDIF present in the tree.
 *
 * @return Version of edif
 */
inline const EdifVersion
Root::getVersion() const throw() {
  return mVersion;
}

/**
 * The the level of EDIF file.
 *
 * @return The EDIF level for this file
 */
inline const EdifLevel
Root::getLevel() const throw() {
  return mLevel;
}

#ifdef GENOM_SERIALIZATION
inline const DumpRestoreData &
Root::getDumpRestoreData() const throw() {
    return mDumpRestoreData;
}

void
dump( const RootSharedPtr &inRoot ) throw(Error);

void
dump( const RootSharedPtr &inRoot,
        const DumpRestoreData &inData ) throw(Error);

RootSharedPtr
restore( const std::string &inName,
        const DumpRestoreData &inData ) throw(Error);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc

#endif // TORC_GENERIC_OM_ROOT_HPP
