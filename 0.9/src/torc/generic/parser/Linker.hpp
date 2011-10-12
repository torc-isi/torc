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

#ifndef TORC_GENERIC_LINKER_HPP
#define TORC_GENERIC_LINKER_HPP

#include <algorithm>
#include <string>
#include <vector>

//BOOST
#include <boost/shared_ptr.hpp>

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/parser/ParserPointerTypes.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/SymTab.hpp"

namespace torc { namespace generic { class View; }  } 
namespace torc { namespace generic { class Instance; }  } 
namespace torc { namespace generic { class Root; }  } 

namespace torc {

namespace generic {

/**
 * @brief Represents a repository of unresolved usage references.
 * 
 * The Linker class represents a repository of unresolved instances, that is instances that are yet to to be linked to concrete veiws. When an instance is created by the parser, it tries to find a concrete view using the name specification present in the EDIF file. If no such references were found, the instance is attached to a Extern view, that is a view that is only a place holder. As newer cells are discovered by the parser, the unresolved instances are linked to the concrete views and removed from the linker.
 * 
 * @note This class is typically used by EdifParser.
 */
class Linker
{
  public:
    /**
     * The name specification of a view consisting of a library name, cell name and a view name.
     */
    class NameSpec
    {
      public:
        NameSpec();

        NameSpec( const std::string &inLibraryName,
                const std::string &inCellName,
                const std::string &inViewName );

        ~NameSpec() throw();

        NameSpec(const NameSpec &inSource);

        NameSpec & operator=(const NameSpec &inSource) throw();

        inline bool
        operator <(const NameSpec &inRhs) const throw();

        inline std::string
        getLibraryName() const throw();

        void
        setLibraryName(const std::string &inValue) throw();

        inline std::string
        getCellName() const throw();

        void
        setCellName(const std::string &inValue) throw();

        inline std::string
        getViewName() const throw();

        void
        setViewName(const std::string &inValue) throw();

      private:
        std::string mLibraryName;
        std::string mCellName;
        std::string mViewName;

    };
    
    /**
     * Represents a collection of instances that are connected to an extern.
     */
    class UnresolvedInstances
    {
      public:
        inline const ViewSharedPtr
        getExternView() const throw();

        void
        setExternView(const ViewSharedPtr &inValue) throw();

        void
        addInstance(const InstanceSharedPtr &inInstance) throw();

        inline const std::vector< InstanceSharedPtr > &
        getInstances() const throw();

        void
        setInstances(
            const std::vector< InstanceSharedPtr > & value
                    ) throw();

        template<typename _Tp> void
        applyActionOnInstances( const _Tp &action ) {
            std::vector< InstanceSharedPtr >::iterator first
                                                = mInstances.begin();
            std::vector< InstanceSharedPtr >::iterator last
                                                = mInstances.end();
            for(; first != last; ++first )
            {
                try
                {
                    action( *first );
                }
                catch(Error &e)
                {
                    e.setCurrentLocation(
                        __FUNCTION__, __FILE__, __LINE__ );
                    throw;
                }
            }
        }


        UnresolvedInstances();

        ~UnresolvedInstances() throw();

        UnresolvedInstances(const UnresolvedInstances &inSource);

        UnresolvedInstances & operator=(
                        const UnresolvedInstances &inSource) throw();

      private:
        ViewSharedPtr mExternView;
        std::vector< InstanceSharedPtr > mInstances;
    };

    typedef boost::shared_ptr<UnresolvedInstances> UnresolvedInstancesPtr;

    typedef SymTab<NameSpec,UnresolvedInstancesPtr> UnresolvedInfoMap;

  public:
    /**
     * Find the extern View if already present.
     * 
     * @param[in] inNameSpec Name of the view
     * 
     * @return Extern view
     */
    ViewSharedPtr
    findExternView(const NameSpec &inNameSpec) throw();

    /**
     * Set an extern View 
     * 
     * @param[in] inNameSpec Name of the view
     * @param[in] inExternView an Extern View
     */
    void
    setExternView(const NameSpec &inNameSpec,
            const ViewSharedPtr &inExternView) throw();

    /**
     * Find the list of instances that are waiting for a view with the specified name.
     * 
     * @param[in] inNameSpec Name of the view to be linked with.
     * @param[out] outInstances Pointer to unresolved object
     * 
     * @return True if any such instances exist
     */
    bool
    findUnresolvedInstances(const NameSpec &inNameSpec,
                UnresolvedInstancesPtr &outInstances) throw(Error);

    /**
     * Add an instance to the list of unresolved instances for a given name specification. If a new UnresolvedInstances object is to be created, this method will also call the setExternView() method to update the externView, with the view set for this object.
     * 
     * @param[in] inNameSpec name specification for a view.
     * @param[in] inInstance instance to be added.
     * 
     * @exception Error The master for this instance is not an extern or no extern is set for this view.
     */
    void registerUnresolvedInstance(const NameSpec &inNameSpec,
        const InstanceSharedPtr &inInstance) throw(Error);

    /**
     * Remove all unresolved objects for the given name specification.
     * 
     * @param[in] inNameSpec Name specification
     */
    void
    removeUnresolvedInstances(
                    const NameSpec &inNameSpec) throw(Error);

    /**
     * Resolves all unlinked references for the given namespec to the given view
     * @param[in] inNameSpec Name specification for unresolved view
     * @param[in] inView View to be bound to
     */

    void
    linkUnresolved(const NameSpec &inNameSpec,
            ViewSharedPtr &inView) throw(Error);

    /**
     * Constructor
     * 
     * @param[in] root Root of the OM.
     */
    Linker(const RootSharedPtr & root);

    ~Linker() throw();

    Linker(const Linker & source);

    Linker &
    operator=(const Linker &inSource) throw();

  private:
    RootSharedPtr mRoot;
    UnresolvedInfoMap mInfos;
};

inline bool
Linker::NameSpec::operator <(
        const Linker::NameSpec &inRhs) const throw() {
    return mLibraryName < inRhs.mLibraryName
        || ( mLibraryName == inRhs.mLibraryName 
                && mCellName < inRhs.mCellName )
        || ( mLibraryName == inRhs.mLibraryName
                && mCellName == inRhs.mCellName
                && mViewName < inRhs.mViewName );
}

inline std::string
Linker::NameSpec::getLibraryName() const throw() {
  return mLibraryName;
}

inline std::string
Linker::NameSpec::getCellName() const throw() {
  return mCellName;
}

inline std::string
Linker::NameSpec::getViewName() const throw() {
  return mViewName;
}

inline const ViewSharedPtr
Linker::UnresolvedInstances::getExternView() const throw() {
  return mExternView;
}

inline const std::vector< InstanceSharedPtr > &
Linker::UnresolvedInstances::getInstances() const throw() {
  return mInstances;
}

} // namespace torc::generic

} // namespace torc
#endif
