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

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

#include <algorithm>
#include <fstream>

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/util/Log.hpp"
#include "torc/generic/om/Root.hpp"
#include "torc/generic/om/Library.hpp"
#include "torc/generic/om/Design.hpp"

namespace torc {

namespace generic {

/**
 * Add a library to the list of libraries. If an empty pointer is supplied, it returns without doing anything.
 *
 * @param[in] inLibrary A pointer to a library object.
 *
 * @exception Error Library could not be added. because Library name is empty
 * @exception Error Library could not be added. because Library name is already exists
 */
void
Root::addLibrary(
    const LibrarySharedPtr &inLibrary) throw(Error) {
    if( !inLibrary )
    {
        return;
    }
    std::string name = inLibrary->getName();
    if( name.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Library name", name);
        throw e;
    }
#ifdef GENOM_SERIALIZATION
    std::list< std::string >::iterator it
            = std::find( mDumpedLibraries.begin(),
                    mDumpedLibraries.end(), name );
    if( it != mDumpedLibraries.end() )
    {
        Error e( eMessageIdErrorItemAlreadyExists,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Library name", name);
        throw e;
    }
#endif //GENOM_SERIALIZATION    
    if( false == mLibraries.set( name, inLibrary ) )
    {
        Error e( eMessageIdErrorItemAlreadyExists,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Library name", name);
        throw e;
    }
    inLibrary->setParent( getSharedThis() );
    return;
}

/**
 * Remove the specified library from the list of libraries. If an empty pointer is passed, it returns without doing anything
 *
 * @param inName Name of the object to be delete
 *
 * @exception Error Empty Library name
 * @exception Error Library not preset in collection
 */
void
Root::removeLibrary(const std::string &inName) throw(Error) {
    if( inName.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Library name", inName);
        throw e;
    }
#ifdef GENOM_SERIALIZATION
    std::list< std::string >::iterator it
            = std::find( mDumpedLibraries.begin(),
                    mDumpedLibraries.end(), inName );
    if( it == mDumpedLibraries.end() )
    {
        Error e( eMessageIdErrorItemNotFound,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Library name", inName);
        throw e;
    }
    else
    {
        mDumpedLibraries.erase( it );
    }
#endif //GENOM_SERIALIZATION    

    if( false == mLibraries.remove( inName ) )
    {
        Error e( eMessageIdErrorItemNotFound,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Library name", inName);
        throw e;
    }
    return;
}

/**
 * Get the list of libraries.
 *
 * @param[out] outLibraries List of libraries
 */
void
Root::getLibraries(
    std::vector< LibrarySharedPtr > &outLibraries) throw() {
#ifdef GENOM_SERIALIZATION
    restoreAllLibraries();
#endif //GENOM_SERIALIZATION
  mLibraries.getValues( outLibraries );
}

/**
 * Find a library by name, in the list of libraries.
 *
 * @param[in] inName String inSource specifying the name of the library.
 *
 * @return A pointer to the libary if found, an empty pointer otherwise.
 */
LibrarySharedPtr
Root::findLibrary(const std::string &inName) throw() {
    LibrarySharedPtr library;
    mLibraries.get( inName, library );
#ifdef GENOM_SERIALIZATION
    if( !library && !mDumpedLibraries.empty() )
    {
        library = restoreSingleLibrary( inName );
    }
#endif //GENOM_SERIALIZATION
    return library;
}

/**
 * Add a design to the list of designs. If an empty pointer is supplied, it returns without doing anything.
 */
void
Root::addDesign(
    const DesignSharedPtr &inDesign) throw(Error) {
    if( !inDesign )
    {
        return;
    }
    std::string name = inDesign->getName();
    if( name.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Design name", name);
        throw e;
    }
    if( false == mDesignSymTab.set( name, inDesign ) )
    {
        Error e( eMessageIdErrorItemAlreadyExists,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Design name", name);
        throw e;
    }
    inDesign->setParent( getSharedThis() );
    return;
}

/**
 * Remove the specified Design from the list of libraries. 
 * If an empty pointer is passed, it returns without doing anything
 */
void
Root::removeDesign(const std::string &inName) throw(Error) {
    if( inName.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Design name", inName);
        throw e;
    }
    if( false == mDesignSymTab.remove( inName ) )
    {
        Error e( eMessageIdErrorItemNotFound,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Design name", inName);
        throw e;
    }
    return;
}

/**
 * Get the list of designs.
 *
 * @param[out] outDesigns List of libraries
 */
void
Root::getDesigns(
    std::vector< DesignSharedPtr > &outDesigns) throw() {
  mDesignSymTab.getValues( outDesigns );
}

/**
 * Find a Design by name, in the list of libraries.
 *
 * @param[in] inName String inSource specifying the name of the Design.
 *
 * @return A pointer to the design if found, an empty pointer otherwise.
 */
DesignSharedPtr
Root::findDesign(const std::string &inName) throw() {
    DesignSharedPtr design;
    mDesignSymTab.get( inName, design );
    return design;
}

/**
 * Create a root
 *
 * @param[in] inName Name of the root to be created.
 * @param[in] inEdifLevel Edif level.
 * @param[in] inOriginalName Original name of the root [optional]
 *
 * @return Pointer to created root.
 */
RootSharedPtr
Root::Factory::newRootPtr( const std::string &inName,
    const EdifLevel &inEdifLevel,
    const std::string &inOriginalName ) throw(Error) {
    try
    {
        RootSharedPtr newRoot;
        create( newRoot );
        newRoot->setName( inName );
        newRoot->setLevel( inEdifLevel );
        newRoot->setOriginalName( inOriginalName );
        return newRoot;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
Root::accept(BaseVisitor & inoutVisitor) throw(Error) {
    try
    {
        runVisitor( *this, inoutVisitor );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
 * Set the version of EDIF being used. This should be 2 0 0 for the current EOM version.
 *
 * @param[in] inSource An object of type EdifVersion.
 */
void
Root::setVersion(const EdifVersion & inSource) throw() {
  mVersion = inSource;
}

/**
 * Set the EDIF level.
 *
 * @note Current parser supports LEVEL_1 edif only.
 *
 * @param[in] inSource EdifLevel object
 */
void
Root::setLevel(const EdifLevel & inSource) throw() {
  mLevel = inSource;
}

#ifdef GENOM_SERIALIZATION

void
Root::setDumpRestoreData(
        const DumpRestoreData &inDumpRestoreData ) throw() {
    mDumpRestoreData = inDumpRestoreData;
}

void
Root::handleNewDumpRestoreData(
        const DumpRestoreData &inDumpRestoreData ) throw(Error) {
    mDumpRestoreData = inDumpRestoreData;
    if( mDumpRestoreData.getRestoreAllComponents() )
    {
        try
        {
            restoreAllLibraries();
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
}

#endif //GENOM_SERIALIZATION

Root::Root()
    :Commentable(),
    Nameable(),
    Renamable(),
    Visitable(),
    SelfReferencing<Root>(),
    mLevel(),
    mLibraries(),
    mVersion()
#ifdef GENOM_SERIALIZATION
    , mDumpRestoreData(),
    mDumpedLibraries()
#endif //GENOM_SERIALIZATION
{
}

Root::~Root() throw() {
    log("Root destroyed\n");
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
Root::load( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object<Commentable>( *this );
    ar & boost::serialization::base_object<Nameable>( *this );
    ar & boost::serialization::base_object<Renamable>( *this );
    ar & boost::serialization::base_object<Visitable>( *this );
    ar & boost::serialization::base_object<
                                SelfReferencing<Root> >( *this );
    ar & mLevel;
    ar & mVersion;
    //ar & mDumpRestoreData;
    ar & mDumpedLibraries;
    //We do not retrieve libararies now
    //We retreive when required
}

template<class Archive> void
Root::save( Archive &ar, unsigned int ) const {
    typedef std::vector< LibrarySharedPtr > Libs;
    ar & boost::serialization::base_object<Commentable>( *this );
    ar & boost::serialization::base_object<Nameable>( *this );
    ar & boost::serialization::base_object<Renamable>( *this );
    ar & boost::serialization::base_object<Visitable>( *this );
    ar & boost::serialization::base_object<
                                SelfReferencing<Root> >( *this );
    ar & mLevel;
    ar & mVersion;
    //ar & mDumpRestoreData;
    Libs libs;
    mLibraries.getValues( libs );
    Libs::iterator lib = libs.begin();
    Libs::iterator end = libs.end();
    for(; lib != end; ++lib )
    {
        log("HERE\n");
        try
        {
            dump( *lib ); 
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
        mDumpedLibraries.push_back( (*lib)->getName() );
    }
    ar & mDumpedLibraries; //Dump list of lib names for this root
    mDumpedLibraries.clear(); //We clear this list now, since
                          //the libs are still in memory
    //Common sense dictates that we remove the dumped libs from root
    //.. however, they may have been instantiated else where.
    //So we have to keep them in memory and connected to this root
    log("Root dumped");
}

LibrarySharedPtr
Root::restoreSingleLibrary( const std::string &inName ) throw(Error) {
    LibrarySharedPtr library;
    std::list< std::string >::iterator lib
            = find( mDumpedLibraries.begin(),
                    mDumpedLibraries.end(), inName );
    if( lib == mDumpedLibraries.end() )
    {
        return library;
    }
    try
    {
        library = restore( *lib, getSharedThis() );
        if( library )
        {
            mDumpedLibraries.erase( lib );
            addLibrary( library );
        }
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
    return library;
}

void
Root::restoreAllLibraries() throw(Error)
try
{
    std::vector<std::string> libs;
    libs.insert( libs.end(),
            mDumpedLibraries.begin(), mDumpedLibraries.end());
    for( std::vector<std::string>::iterator lib = libs.begin();
                                lib != libs.end(); ++lib)
    {
        try
        {
            restoreSingleLibrary( *lib );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
}
catch( Error &e )
{
    e.setCurrentLocation(
        __FUNCTION__, __FILE__, __LINE__ );
    throw;
}

//TO SATISFY THE LINKER
template void
Root::load<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
Root::save<boost::archive::binary_oarchive>(
    boost::archive::binary_oarchive & ar, const unsigned int) const;

void
dump( const RootSharedPtr &inRoot ) throw(Error) {
    if( !inRoot )
    {
        //TBD::ERROR
    }
    try
    {
        dump( inRoot, inRoot->getDumpRestoreData() );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
dump( const RootSharedPtr &inRoot,
        const DumpRestoreData &inData ) throw(Error) {
    if( !inRoot )
    {
        //TBD::ERROR
    }
    if( inRoot->getName().empty() )
    {
        //TBD::ERROR
    }
    if( inData.getDumpPath().empty() )
    {
        //TBD::ERROR
    }
    inRoot->setDumpRestoreData( inData );
    std::string fileName = inData.getDumpPath() + "/";
    fileName += inRoot->getName() + ".root";
    std::ofstream dout( fileName.c_str(),
                        std::ios::out | std::ios::binary );
    if( !dout )
    {
        //TBD::ERROR
    }
    boost::archive::binary_oarchive rootAr( dout );
    rootAr & inRoot;
}

RootSharedPtr
restore( const std::string &inName,
        const DumpRestoreData &inData ) throw(Error) {
    RootSharedPtr root;
    if( inName.empty() )
    {
        //TBD::ERROR
    }
    if( inData.getDumpPath().empty() )
    {
        //TBD::ERROR
    }
    std::string fileName = inData.getDumpPath() + "/";
    fileName += inName + ".root";
    std::ifstream din( fileName.c_str(),
                        std::ios::out | std::ios::binary );
    if( !din )
    {
        //TBD::ERROR
    }
    boost::archive::binary_iarchive rootAr( din );
    rootAr & root;
    if( root )
    {
        try
        {
            root->handleNewDumpRestoreData( inData );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
    return root;
}
#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
