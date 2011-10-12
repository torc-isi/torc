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

#ifndef TORC_GENERIC_LIBRARY_HPP
#define TORC_GENERIC_LIBRARY_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Commentable.hpp"
#include "torc/generic/om/Extern.hpp"
#include "torc/generic/om/EdifLevel.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/Nameable.hpp"
#include "torc/generic/om/ParentedObject.hpp"
#include "torc/generic/om/Renamable.hpp"
#include "torc/generic/om/Root.hpp"
#include "torc/generic/om/ScaleFactor.hpp"
#include "torc/generic/om/SelfReferencing.hpp"
#include "torc/generic/om/SymTab.hpp"
#include "torc/generic/om/Visitable.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/Unit.hpp"

namespace torc { namespace generic { class BaseVisitor; }  }
namespace torc { namespace generic { class Cell; }  }

namespace torc {

namespace generic {

/**
 * @brief An EDIF cell library
 *
 * A Library object defines a collection of EDIF libraries
 */
class Library :
    public Commentable,
    public Extern,
    public Nameable,
    public Renamable,
    public Visitable,
    public ParentedObject<Root>,
    public SelfReferencing<Library> {

#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

    friend class FactoryType<Library>; 

  public:
    /**
     * Convenience typedef for visiting a library
     */
    typedef VisitorType<Library> Visitor;

    /**
     * Convenience class for creating a library
     */

    class Factory: public FactoryType<Library>
    {
        public:
            using FactoryType<Library>::create;
        /**
         * Create a library
         *
         * @param[in] inName Name of the library to be created.
         * @param[in] inRootPtr Pointer to parented(Root) object.
         * @param[in] inEdifLevel Edif level. 
         * @param[in] inOriginalName Original name of the library [optional] 
         *
         * @return Pointer to created library.
         **/
            virtual LibrarySharedPtr
            newLibraryPtr( const std::string &inName,
                const RootSharedPtr &inRootPtr,
                const EdifLevel &inEdifLevel = eEdifLevel0,
                const std::string &inOriginalName = std::string()) throw(Error);
    };

    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

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
     * @note Current parser supports LEVEL_1 edif only.
     *
     * @param[in] inSource EdifLevel object
     */
    void
    setLevel(const EdifLevel & inSource) throw();

    /**
     * Get scale factors for different units in the library.
     *
     * @return Scale factors
     */
    inline const std::map<Unit,ScaleFactor>
    getScaleFactors() const throw();

    /**
     * Find a scale factor for the given unit.
     *
     * @param[in] inUnit Units for which scale factor is to be found.
     * @param[out] outResult The scale factor if found.
     *
     * @return true if found, false otherwise
     */
    bool
    findScaleFactor(
        Unit inUnit, ScaleFactor &outResult ) const throw();

    /**
     * Set a scale factor for the given unit.
     *
     * @param[in] inUnit Units for which scale factor is to be set.
     * @param[out] inScale The scale factor to store.
     * @return True if set
     */
    bool
    setScaleFactor(
            Unit inUnit, const ScaleFactor &inScale) throw();

    /**
     * Add a cell to the list of cells. If an empty pointer is supplied, it returns without doing anything.
     * If an cell already exists in EDIF file in same library, parser ignores the cell.
     *
     * @param[in] inCell A pointer to a cell object.
     *
     * @exception Error Cell could not be added, because Cell name is empty
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Cell name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error Cell could not be added, because Cell name is already exists
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemAlreadyExists
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Cell name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */
    void
    addCell(const CellSharedPtr &inCell) throw(Error);

    /**
     * Find a cell by name, in the list of cells.
     *
     * @param[in] inName String inSource specifying the name of the cell.
     *
     * @return A pointer to the cell if found, an empty pointer otherwise.
     * @exception Error Cell could not be found, because Cell name is empty
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Cell name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @note If some cells are yet to be restored, this method is not thread safe. For non-serializable mode this is thread safe. For dump mode derived classes can override this to make it thread safe by placing appropriate locks.
     */
    virtual CellSharedPtr
    findCell(const std::string &inName) throw(Error);

    /**
     * Remove the specified cell from the list of cells. If an empty pointer is passed, it returns without doing anything
     *
     * @param inName Name of the object to be delete
     *
     * @exception Error Cell name is empty
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Cell name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error Cell name not preset in collection.
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemNotFound
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Cell name - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */
    void
    removeCell(const std::string &inName) throw(Error);

    /**
     * Get the list of cells. The list of cells is appended to the provided list
     * @param[out] outCells List of cells to be populated
     * @return List of cells in the library.
     * @note If some cells are yet to be restored, this method is not thread safe. For non-serializable mode this is thread safe. For dump mode derived classes can override this to make it thread safe by placing appropriate locks. In dump mode using this method will lead to restoration of all children.
     */
    virtual void
    getCells(
        std::vector< CellSharedPtr > &outCells) throw();

   /**
    * Apply action on all Cells.
    * @param[in] action Action to be applied
     * @note If some cells are yet to be restored, this method is not thread safe. For non-serializable mode this is thread safe. For dump mode derived classes can write an appropriate thread safe wrapper method that can be accesed using visitors. However, using this method in dump mode should be avoided unless under dire situations, as it will lead to restoration of all cells under the library.
    */
    template<typename _Action>
    inline void
    applyOnAllCells( const _Action &action ) throw(Error); 

    virtual
    ~Library() throw();
 
  protected:  
    Library();

  private:
    Library( const Library &);

    Library &
    operator =(const Library &);

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    load( Archive &ar, unsigned int );

    template<class Archive> void
    save( Archive &ar, unsigned int ) const;

    CellSharedPtr
    restoreSingleCell( const std::string &inName ) throw(Error);

    void
    restoreAllCells() throw(Error);

    BOOST_SERIALIZATION_SPLIT_MEMBER()

#endif //GENOM_SERIALIZATION

    SymTab<Unit,ScaleFactor> mScaleFactors;
    EdifLevel mLevel;
    SymTab< std::string,CellSharedPtr > mCellSymTab;
#ifdef GENOM_SERIALIZATION
    mutable std::list< std::string > mDumpedCells;
#endif //GENOM_SERIALIZATION
};


/**
 * The the level of EDIF file.
 *
 * @return The EDIF level for this file
 */
inline const EdifLevel
Library::getLevel() const throw() {
  return mLevel;
}

/**
 * Get scale factors for different units in the library.
 *
 * @return Scale factors
 */
inline const std::map<Unit,ScaleFactor>
Library::getScaleFactors() const throw() {
  std::map<Unit,ScaleFactor> temp;
  mScaleFactors.getValueMap( temp );
  return temp;
}

/**
 * Apply action on all Cells.
 * @param[in] action Action to be applied
 *
 */
template<typename _Action>
inline void
Library::applyOnAllCells( const _Action &action ) throw(Error) {
    try
    {
#ifdef GENOM_SERIALIZATION
        restoreAllCells();
#endif //GENOM_SERIALIZATION    
        mCellSymTab.applyOnAll( action );
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

#ifdef GENOM_SERIALIZATION
void
dump( const LibrarySharedPtr &inLibrary ) throw(Error);

void
dump( const LibrarySharedPtr &inLibrary,
		const DumpRestoreData &inData ) throw(Error);

LibrarySharedPtr
restore( const std::string &inName,
		const RootSharedPtr &inParent) throw(Error);

LibrarySharedPtr
restore( const std::string &inName,
		const RootSharedPtr &inParent,
		const DumpRestoreData &inData ) throw(Error);
#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
#endif
