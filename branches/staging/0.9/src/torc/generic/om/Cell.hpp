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

#ifndef TORC_GENERIC_OM_CELL_HPP
#define TORC_GENERIC_OM_CELL_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/PointerTypes.hpp"
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
#include "torc/generic/om/Visitable.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/UserDataContainer.hpp"
#include "torc/generic/om/StatusContainer.hpp"

namespace torc { namespace generic { class BaseVisitor; }  }
namespace torc { namespace generic { class Library; }  }
namespace torc { namespace generic { class View; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents an EDIF cell
 *
 * The Cell class is used to implement an EDIF cell object. It contains the different views of a cell. A cell can be concrete or extern (black-box).
 *
 * @note  Currently parser performs semantic checks for GENERIC type only. Programattic creation may be achieved. However, a warning may be emitted when cell type is set to TIE or RIPPER.
 */
class Cell :
    public Commentable,
    public Extern,
    public Nameable,
    public ParentedObject<Library>,
    public PropertyContainer,
    public Renamable,
    public Visitable,
    public SelfReferencing<Cell>,
    public UserDataContainer,
    public StatusContainer
{
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
	friend class RestoredCellUpdater;
#endif

    friend class FactoryType<Cell>;

  public:
    /**
     * @enum Type
     *
     * Type of cell.
     */
    enum Type
    {
      eTypeTie = 0,
      eTypeRipper,
      eTypeGeneric
    };

    /**
     * Convenience class to visit a Cell.
     */
    typedef VisitorType<Cell> Visitor;

    /**
     * Convenience class to create a cell.
     */
    class Factory: public FactoryType<Cell> 
    {
        public:
            using FactoryType<Cell>::create;
        /**
         * Create a cell
         *
         * @param[in] inName Name of the cell to be created.
         * @param[in] inLibraryPtr Pointer to parented(Library) object.
         * @param[in] inCellType Cell Type. 
         * @param[in] inOriginalName Original name of the cell [optional].
         *
         * @return Pointer to created cell.
         **/
            virtual CellSharedPtr
            newCellPtr( const std::string &inName,
                const LibrarySharedPtr &inLibraryPtr,
                const Cell::Type &inCellType = Cell::eTypeGeneric,
                const std::string &inOriginalName = std::string()) throw(Error); 
    };

    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    /**
     * Get the type of cell
     *
     * @return Type of cell
     */
    inline const Type
    getType() const throw();

    /**
     * Set the type of cell
     *
     * @param[in] inSource Type of cell
     */
    void
    setType(const Type & inSource) throw();

    /**
     * Add a view to the list of views. An empty pointer will be ignored.
     * If an view already exists in EDIF file in same cell, parser ignores the view.
     *
     * @param[in] inView Pointer to a view.
     *
     * @exception Error Empty View type
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *       </ul> 
     *
     * @exception Error View name already exists
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemAlreadyExists
     *         </li>
     *       </ul> 
     *
     */

    void
    addView(const ViewSharedPtr & inView) throw(Error);

    /**
     * Find a view by name.
     *
     * @param[in] inName Name of the cell to be found.
     *
     * @return Pointer to cell if found, empty pointer otherwise.
     *
     * @exception Error Empty Cell name 
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
     */
    ViewSharedPtr
    findView(const std::string &inName) throw();

    /**
     * Remove the specified pointer from the view. Empty pointer is ignored.
     *
     * @param[in] inName name of the object to be removed
     *
     * @exception Error View is not a member of the views list.
     */
    void
    removeView(const std::string &inName ) throw(Error);

    /**
     * Get the views of this cell.
     *
     * @param[out] outViews List of views to be appended to
     */
    inline void
    getViews(
        std::vector< ViewSharedPtr > &outViews
                ) const throw();

   /**
    * Apply action on all Views.
    * @param[in] action Action to be applied
    *
    */
    template<typename _Action>
    inline void
    applyOnAllViews( const _Action &action ) throw(Error); 


    virtual ~Cell() throw();

  protected:
    Cell();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );

	void
	restoreActions() throw(Error);
#endif //GENOM_SERIALIZATION

  Cell( const Cell &rhs);

  Cell &
  operator = ( const Cell &rhs);

    SymTab< std::string,
            ViewSharedPtr > mViewSymTab;
    Type mType;
    ParameterMapSharedPtr mParameters;
};

/**
 * Get the type of cell
 *
 * @return Type of cell
 */
inline const Cell::Type
Cell::getType() const throw() {
  return mType;
}

/**
 * Get the views of this cell.
 *
 * @return A list of views of this cell.
 */
inline void
Cell::getViews(
    std::vector< ViewSharedPtr > &outViews) const throw() {
  mViewSymTab.getValues( outViews );
}

template<typename _Action>
inline void
Cell::applyOnAllViews( const _Action &action ) throw(Error)
{
    try
    {
        mViewSymTab.applyOnAll( action );
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_CELL_HPP
