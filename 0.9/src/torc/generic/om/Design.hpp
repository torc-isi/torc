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

#ifndef TORC_GENERIC_DESIGN_HPP
#define TORC_GENERIC_DESIGN_HPP

#include "torc/generic/om/Nameable.hpp"
#include "torc/generic/om/Commentable.hpp"
#include "torc/generic/om/ParentedObject.hpp"
#include "torc/generic/om/PropertyContainer.hpp"
#include "torc/generic/om/Renamable.hpp"
#include "torc/generic/om/Root.hpp"
#include "torc/generic/om/SelfReferencing.hpp"
#include "torc/generic/om/SymTab.hpp"
#include "torc/generic/om/Visitable.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/FactoryType.hpp"

namespace torc { namespace generic { class BaseVisitor; }  }

namespace torc {

namespace generic {

/**
 * The Design class identifies the Cell at the top level of the 
 * hierarchy of a particular design within a library.
 */

class Design : 
    public Nameable,
    public Commentable, 
    public PropertyContainer, 
    public Renamable,
    public Visitable,
    public ParentedObject<Root>,
    public SelfReferencing<Design> {

    friend class FactoryType<Design>;

  public:
    /**
     * Convenience typedef for visiting a design
     */
    typedef VisitorType<Design> Visitor;

    /**
     * Convenience class for creating a design
     */

    class Factory: public FactoryType<Design>
    {
        public:
            using FactoryType<Design>::create;
        /**
         * Create a design
         *
         * @param[in] inName Name of the design to be created.
         * @param[in] inRootPtr Pointer to parented(Root) object.
         * @param[in] inCellRefName The top level cell ref name for this design.
         * @param[in] inLibraryRefName The top level Library ref name for this design.
         * @param[in] inOriginalName Original name of the design [optional].
         *
         * @return Pointer to created design.
         **/
            virtual DesignSharedPtr
            newDesignPtr( const std::string &inName,
                const RootSharedPtr & inRootPtr,
                const std::string &inCellRefName,
                const std::string &inLibraryRefName,
                const std::string &inOriginalName = std::string() ) throw(Error);
    };

    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    /**
     * Get the top cell ref name for this Design.
     *
     * @return The top cell ref name for this Design.
     */
    inline const std::string 
    getCellRefName() const throw();

    /**
     * Set the top cell ref name for this Design.
     *
     * @param[in] inCellRefName The top level cell ref name for this design.
     */
    void
    setCellRefName( const std::string & inCellRefName ) throw();

    /**
     * Get the top Library ref name for this Design.
     *
     * @return The top Library ref name for this Design.
     */
    inline const std::string 
    getLibraryRefName() const throw();

    /**
     * Set the top Library ref name for this Design.
     *
     * @param[in] inLibraryRefName The top level Library ref name for this design.
     */
    void
    setLibraryRefName( const std::string & inLibraryRefName ) throw();

    virtual
    ~ Design() throw();

  protected:
    Design();
    Design( const std::string & inCellRefName,
                const std::string &inLibraryRefName ); 

  private:
    Design(const Design & inSource) throw();
    Design &
    operator=(const Design & inSource ) throw();

  private:
    std::string mCellRefName;
    std::string mLibraryRefName;
};

/**
 * Get the top cell ref name for this Design.
 *
 * @return The top cell ref name for this Design.
 */
inline const std::string 
Design::getCellRefName() const throw() {
    return mCellRefName;    
}

/**
 * Get the top Library ref name for this Design.
 *
 * @return The top Library ref name for this Design.
 */
inline const std::string 
Design::getLibraryRefName() const throw() {
    return mLibraryRefName;    
}


} // namespace torc::generic

} // namespace torc
#endif
