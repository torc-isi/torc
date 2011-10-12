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

#include "torc/generic/parser/Linker.hpp"
#include "torc/generic/om/Instance.hpp"

namespace {

using namespace torc::generic;

class LinkAction
{
    public:
        LinkAction( const ViewSharedPtr &inView )
            :mView( inView ) {
        }

        void
        operator()( const InstanceSharedPtr &outInstance
                                            ) const throw( Error ) {
            try
            {
                outInstance->bindToMasterView( mView );
            }
            catch( Error &e )
            {
                e.setCurrentLocation( __FUNCTION__,
                                        __FILE__, __LINE__ );
                throw;
            }
        }    
    private:
         ViewSharedPtr mView;    
};
}

namespace torc {

namespace generic {

Linker::NameSpec::NameSpec()
    :mLibraryName(),
    mCellName(),
    mViewName() {
}

Linker::NameSpec::NameSpec( const std::string &inLibraryName,
                  const std::string &inCellName,
                  const std::string &inViewName )
    : mLibraryName( inLibraryName ),
    mCellName( inCellName ),
    mViewName( inViewName ) {
}

Linker::NameSpec::~NameSpec() throw() {
}

Linker::NameSpec::NameSpec(const Linker::NameSpec &inSource)
    :mLibraryName( inSource.mLibraryName ),
    mCellName( inSource.mCellName ),
    mViewName( inSource.mViewName ) {
}

Linker::NameSpec &
Linker::NameSpec::operator=(
                    const Linker::NameSpec &inSource) throw() {
    if( this != &inSource )
    {
        mLibraryName = inSource.mLibraryName;
        mCellName = inSource.mCellName;
        mViewName = inSource.mViewName;
    }
    return *this;
}

void
Linker::NameSpec::setLibraryName(
        const std::string &inValue) throw() {
  mLibraryName = inValue;
}

void
Linker::NameSpec::setCellName(const std::string &inValue) throw() {
  mCellName = inValue;
}

void
Linker::NameSpec::setViewName(const std::string & inValue) throw() {
  mViewName = inValue;
}

void
Linker::UnresolvedInstances::setExternView(
                const ViewSharedPtr & inValue) throw() {
  mExternView = inValue;
}

void
Linker::UnresolvedInstances::addInstance(
        const InstanceSharedPtr &inInstance) throw() {
    mInstances.push_back( inInstance );
}

void
Linker::UnresolvedInstances::setInstances(
        const std::vector< InstanceSharedPtr > &inValue
        ) throw() {
  mInstances = inValue;
}

Linker::UnresolvedInstances::UnresolvedInstances()
    :mExternView(),
    mInstances() {
}

Linker::UnresolvedInstances::~UnresolvedInstances() throw() {
}

Linker::UnresolvedInstances::UnresolvedInstances(
            const Linker::UnresolvedInstances &inSource)
    :mExternView( inSource.mExternView ),
    mInstances( inSource.mInstances ) {
}

Linker::UnresolvedInstances &
Linker::UnresolvedInstances::operator=(
            const Linker::UnresolvedInstances &inSource) throw() {
    if( this != &inSource )
    {
        mExternView = inSource.mExternView;
        mInstances =  inSource.mInstances;
    }
    return *this;
}

/**
 * Find the extern View if already present. Otherwise null
 * 
 * @param[in] inNameSpec Name of the view
 * 
 * @return Extern view
 */
ViewSharedPtr
Linker::findExternView(
            const Linker::NameSpec &inNameSpec) throw() {
    UnresolvedInstancesPtr info;
    mInfos.get( inNameSpec, info );
    if( info )
    {
        return info->getExternView();
    }
    return ViewSharedPtr();
}

/**
 * Set an extern View
 * 
 * @param[in] inNameSpec Name of the view
 * @param[in] externView an Extern View
 */
void
Linker::setExternView(
        const Linker::NameSpec &inNameSpec,
        const ViewSharedPtr &inExternView) throw() {
    UnresolvedInstancesPtr info;
    mInfos.get( inNameSpec, info );
    if( !info )
    {
        info = UnresolvedInstancesPtr( new UnresolvedInstances() );
        mInfos.set( inNameSpec, info );
    }
    info->setExternView( inExternView );
    return;
}

/**
 * Find the list of instances that are waiting for a view with the specified name.
 * 
 * @param[in] inNameSpec Name of the view to be linked with.
 * @param[out] outInstances Pointer to unresolved object
 * 
 * @return True if such instance exists
 */
bool
Linker::findUnresolvedInstances(
       const Linker::NameSpec &inNameSpec,
       Linker::UnresolvedInstancesPtr &outInstances) throw(Error) {
    return mInfos.get( inNameSpec, outInstances );
}

/**
 * Add an instance to the list of unresolved instances for a given name specification. If a new UnresolvedInstances object is to be created, this method will also call the setExternView() method to update the externView, with the view set for this object.
 * 
 * @param[in] inNameSpec name specification for a view.
 * @param[in] inInstance instance to be added.
 * 
 * @exception Error The master for this instance is not an extern or no extern is set for this view.
 */
void
Linker::registerUnresolvedInstance(
        const Linker::NameSpec &inNameSpec,
           const InstanceSharedPtr &inInstance) throw(Error) {
    UnresolvedInstancesPtr info;
    mInfos.get( inNameSpec, info );
    if( !info )
    {
        info = UnresolvedInstancesPtr( new UnresolvedInstances() );
        mInfos.set( inNameSpec, info );
    }
    info->addInstance( inInstance );
    return;
}

/**
 * Remove all unresolved objects for the given name specification.
 * 
 * @param[in] inNameSpec Name specification
 */
void
Linker::removeUnresolvedInstances(
        const Linker::NameSpec &inNameSpec) throw(Error) {
    mInfos.remove( inNameSpec );
}

void
Linker::linkUnresolved(const NameSpec &inNameSpec,
            ViewSharedPtr &inView) throw(Error) {
    UnresolvedInstancesPtr info;
    mInfos.get( inNameSpec, info );
    LinkAction linkAction( inView );
    if( info )
    {
        try
        {
            info->applyActionOnInstances( linkAction );
        }
        catch( Error &e )
        {
            e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
        removeUnresolvedInstances( inNameSpec );
    }
}

/**
 * Constructor
 * 
 * @param[in] root Root of the OM.
 */
Linker::Linker(const RootSharedPtr &inRoot)
    :mRoot( inRoot ),
    mInfos() {
}

Linker::~Linker() throw() {
}

Linker::Linker(const Linker &inSource)
    : mRoot( inSource.mRoot ),
    mInfos( inSource.mInfos ) {
}

Linker &
Linker::operator=(const Linker &inSource) throw() {
    if( this != &inSource )
    {
        mRoot = inSource.mRoot;
        mInfos = inSource.mInfos;
    }
    return *this;
}


} // namespace torc::generic

} // namespace torc
