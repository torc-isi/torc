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

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

#include "torc/generic/om/Design.hpp"

namespace torc {

namespace generic {

DesignSharedPtr
Design::Factory::newDesignPtr( const std::string &inName,
    const RootSharedPtr &inRootPtr,
    const std::string &inCellRefName,
    const std::string &inLibraryRefName,
    const std::string &inOriginalName ) throw(Error) {
    try
    {
        DesignSharedPtr newDesign;
        create( newDesign );
        newDesign->setName( inName );
        newDesign->setParent( inRootPtr );
        inRootPtr->addDesign( newDesign );
        newDesign->setCellRefName( inCellRefName );
        newDesign->setLibraryRefName( inLibraryRefName );
        newDesign->setOriginalName( inOriginalName );
        return newDesign;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__,
                __FILE__, __LINE__ );
        throw;
    }
}

void
Design::accept(BaseVisitor & inoutVisitor) throw(Error) {
    try
    {
        runVisitor( *this, inoutVisitor );
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__,
                __FILE__, __LINE__ );
        throw;
    }
}

Design::Design()
    :Nameable(),
    Commentable(),
    PropertyContainer(),
    Renamable(),
    Visitable(),
    ParentedObject<Root>(),
    SelfReferencing<Design>(),
    UserDataContainer(),
    StatusContainer(),
    mCellRefName(),
    mLibraryRefName() {
}

Design::Design( const std::string & inCellRefName,
                    const std::string &inLibraryRefName )
    :Nameable(),
    Commentable(),
    PropertyContainer(),
    Renamable(),
    Visitable(),
    ParentedObject<Root>(),
    SelfReferencing<Design>(),
    UserDataContainer(),
    StatusContainer(),
    mCellRefName( inCellRefName ),
    mLibraryRefName( inLibraryRefName ) {
}

Design::~Design() throw() {
}

void
Design::setCellRefName( const std::string & inCellRefName ) throw(){
    mCellRefName = inCellRefName;
}

void
Design::setLibraryRefName( const std::string & inLibraryRefName ) throw(){
    mLibraryRefName = inLibraryRefName;
}

} // namespace torc::generic

} // namespace torc
