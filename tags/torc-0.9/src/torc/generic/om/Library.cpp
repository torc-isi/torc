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

#include <fstream>

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/is_abstract.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/shared_ptr.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Cell.hpp"
#include "torc/generic/om/Library.hpp"
#include "torc/generic/util/Log.hpp"
#include "torc/generic/om/ScaleFactor.hpp"
#ifdef GENOM_SERIALIZATION
#include "torc/generic/om/DumpRestoreData.hpp"
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

#ifdef GENOM_SERIALIZATION
class RestoredCellUpdater {
  public:
    void
    operator()( const CellSharedPtr &inCell
                                        ) const throw(Error) {
        try
        {
            inCell->setParent( mLibrary );
            inCell->restoreActions();
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    RestoredCellUpdater ( const LibrarySharedPtr &inLibrary )
        : mLibrary( inLibrary ) {
    }    
  private:
    LibrarySharedPtr mLibrary;
};
#endif //GENOM_SERIALIZATION

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
LibrarySharedPtr
Library::Factory::newLibraryPtr( const std::string &inName,
    const RootSharedPtr &inRootPtr,
    const EdifLevel &inEdifLevel,
    const std::string &inOriginalName ) throw(Error) {
    try
    {
        LibrarySharedPtr newLibrary;
        create( newLibrary );
        newLibrary->setName( inName );
        newLibrary->setParent( inRootPtr );
        inRootPtr->addLibrary( newLibrary );
        newLibrary->setLevel( inEdifLevel );
        newLibrary->setOriginalName( inOriginalName );
        return newLibrary;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__,
                __FILE__, __LINE__ );
        throw;
    }
}

void
Library::accept(BaseVisitor & inoutVisitor) throw(Error) {
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

/**
 * Set the EDIF level.
 *
 * @note Current parser supports LEVEL_1 edif only.
 *
 * @param[in] inSource EdifLevel object
 */
void
Library::setLevel(const EdifLevel & inSource) throw() {
  mLevel = inSource;
}

/**
 * Find a scale factor for the given unit.
 *
 * @param[in] inUnit Units for which scale factor is to be found.
 * @param[out] outResult The scale factor if found.
 *
 * @return true if found, false otherwise
 */
bool
Library::findScaleFactor(
        Unit inUnit, ScaleFactor &outResult) const throw() {
    return mScaleFactors.get( inUnit, outResult );
}

/**
 * Set a scale factor for the given unit.
 *
 * @param[in] inUnit Units for which scale factor is to be set.
 * @param[out] inScale The scale factor to store.
 *
 */
bool
Library::setScaleFactor(
        Unit inUnit, const ScaleFactor &inScale) throw() {
    return mScaleFactors.set( inUnit, inScale );
}

/**
 * Add a cell to the list of cells. If an empty pointer is supplied, it returns without doing anything.
 *
 * @param[in] inCell A pointer to a cell object.
 *
 * @exception Error Cell could not be added.
 */
void
Library::addCell(
        const CellSharedPtr &inCell) throw(Error) {
    if( !inCell )
    {
        return;
    }
    std::string cellName = inCell->getName();
    if( cellName.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Cell name", cellName);
        throw e;
    }
#ifdef GENOM_SERIALIZATION
    std::list< std::string >::iterator it
            = std::find( mDumpedCells.begin(), mDumpedCells.end(),
                                cellName );
    if( it != mDumpedCells.end() )
    {
        Error e( eMessageIdErrorItemAlreadyExists,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Cell name", cellName);
        throw e;
    }
#endif //GENOM_SERIALIZATION    
    if( false == mCellSymTab.set( cellName, inCell ) )
    {
        Error e( eMessageIdErrorItemAlreadyExists,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Cell name", cellName);
        throw e;
    }
    inCell->setParent( getSharedThis() );
}

/**
 * Find a cell by name, in the list of cells.
 *
 * @param[in] inName String inSource specifying the name of the cell.
 *
 * @return A pointer to the cell if found, an empty pointer otherwise.
 */
CellSharedPtr
Library::findCell(const std::string &inName) throw(Error) {
    if( inName.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Cell name", inName);
        throw e;
    }
    CellSharedPtr cell;
    mCellSymTab.get( inName, cell );
#ifdef GENOM_SERIALIZATION
    if( !cell && !mDumpedCells.empty() )
    {
        cell = restoreSingleCell( inName );
    }
#endif //GENOM_SERIALIZATION
    return cell;
}

/**
 * Remove the specified cell from the list of cells. If an empty pointer is passed, it returns without doing anything
 *
 * @param inCell Pointer to a cell object.
 *
 * @exception Error Cell name is empty
 * @exception Error Cell name not preset in collection.
 */
void
Library::removeCell( const std::string &inName) throw(Error) {
    if( inName.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Cell name", inName);
        throw e;
    }

#ifdef GENOM_SERIALIZATION
    std::list< std::string >::iterator it
            = std::find( mDumpedCells.begin(), mDumpedCells.end(),
                                inName);
    if( it == mDumpedCells.end() )
    {
        Error e( eMessageIdErrorItemNotFound,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Cell name", inName);
        throw e;
    }
    else
    {
        mDumpedCells.erase( it );
    }
#endif //GENOM_SERIALIZATION    

    if( false == mCellSymTab.remove( inName ) )
    {
        Error e( eMessageIdErrorItemNotFound,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Cell name", inName);
        throw e;
    }
}

/**
 * Get the list of cells.
 *
 * @return List of cells in the library.
 */
void
Library::getCells(
    std::vector< CellSharedPtr > &outCells) throw(){
#ifdef GENOM_SERIALIZATION
    restoreAllCells();
#endif //GENOM_SERIALIZATION
  return mCellSymTab.getValues( outCells );
}

/**
 * Set the pointer to the simulation info.
 *
 * @param[in] inSource Pointer to the simulation info
 */
void
Library::setSimulationInfo(const SimulationInfoSharedPtr & inSource ) throw() {
    mSimulationInfo = inSource;
} 

Library::Library()
    : Commentable(),
    Extern(),
    Nameable(),
    Renamable(),
    Visitable(),
    ParentedObject<Root>(),
    SelfReferencing<Library>(),
    UserDataContainer(),
    StatusContainer(),
    mScaleFactors(),
    mLevel(),
    mCellSymTab(),
    mSimulationInfo() {
}

Library::~Library() throw() {
    log("Library destroyed \n");
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
Library::save( Archive &ar, unsigned int ) const {
    typedef std::vector< CellSharedPtr > Cells;
    ar & boost::serialization::base_object<Commentable>( *this );
    ar & boost::serialization::base_object<Extern>( *this );
    ar & boost::serialization::base_object<Nameable>( *this );
    ar & boost::serialization::base_object<Renamable>( *this );
    ar & boost::serialization::base_object<Visitable>( *this );
//    ar & boost::serialization::base_object<
//                                ParentedObject<Root> >( *this );
    ar & boost::serialization::base_object<
                                SelfReferencing<Library> >( *this );
    ar & mScaleFactors;
    ar & mLevel;
    Cells cells;
    mCellSymTab.getValues( cells );
    Cells::iterator cell = cells.begin();
    Cells::iterator end = cells.end();
    for(; cell != end; ++cell )
    {
        std::string fileName
            = getParent()->getDumpRestoreData().getDumpPath() + "/";
        fileName += getName() + "_";
        fileName += (*cell)->getName() + ".cell";
        std::ofstream fout( fileName.c_str(),
                            std::ios::out | std::ios::binary );
        if( !fout )
        {
            //TBD::ERROR
        }
        boost::archive::binary_oarchive cellAr( fout );

        (*cell)->setParent( LibrarySharedPtr() );
        //We reset the parent to avoid dumping the library again
        cellAr & (*cell); //Dump cell
        (*cell)->setParent( getSharedThis() ); //For the rest of
                                            //this session
        mDumpedCells.push_back( (*cell)->getName() );
    }
    ar & mDumpedCells; //Dump list of cell names for this lib
    mDumpedCells.clear(); //We clear this list now, since
                          //the cells are still in memory
    //Common sense dictates that we remove the dumped cells from lib
    //.. however, they may have been instantiated else where.
    //So we have to keep them in memory and connected to this lib
}

template<class Archive> void
Library::load( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object<Commentable>( *this );
    ar & boost::serialization::base_object<Extern>( *this );
    ar & boost::serialization::base_object<Nameable>( *this );
    ar & boost::serialization::base_object<Renamable>( *this );
    ar & boost::serialization::base_object<Visitable>( *this );
//    ar & boost::serialization::base_object<
//                                ParentedObject<Root> >( *this );
    ar & boost::serialization::base_object<
                                SelfReferencing<Library> >( *this );
    ar & mScaleFactors;
    ar & mLevel;
    ar & mDumpedCells;
    //We do not retrieve cells now
    //We retreive when required
}

CellSharedPtr
Library::restoreSingleCell( const std::string &inName ) throw(Error)
{
    CellSharedPtr cell;
    std::list< std::string >::iterator it
                = std::find( mDumpedCells.begin(), mDumpedCells.end(),
                                inName );
    if( it == mDumpedCells.end() )
    {
        return cell;
    }
    std::string fileName
        = getParent()->getDumpRestoreData().getDumpPath() + "/";
    fileName += getName() + "_";
    fileName += inName + ".cell";
    std::ifstream fin( fileName.c_str(),
                std::ios::in | std::ios::binary );
    if( !fin )
    {
        //TBD::ERROR
    }
    boost::archive::binary_iarchive cellAr( fin );
    cellAr & cell;
    if( cell )
    {
        mDumpedCells.erase( it );
        RestoredCellUpdater updater( getSharedThis() );
        updater( cell );
        addCell( cell );
    }
    return cell;
}

void
Library::restoreAllCells() throw(Error)
try
{
    std::vector<std::string> cells;
    cells.insert( cells.end(),
            mDumpedCells.begin(), mDumpedCells.end() );
    for( std::vector<std::string>::iterator cell = cells.begin();
                                cell != cells.end(); ++cell)
    {
        try
        {
            restoreSingleCell( *cell );
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
Library::load<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
Library::save<boost::archive::binary_oarchive>(
    boost::archive::binary_oarchive & ar, const unsigned int) const;

void
dump( const LibrarySharedPtr &inLibrary ) throw(Error) {
    if( !inLibrary )
    {
        //TBD::ERROR
    }
    if( inLibrary->getName().empty() )
    {
        //TBD::ERROR
    }
    if( !inLibrary->getParent() )
    {
        //TBD::ERROR
    }
    try
    {
        dump( inLibrary, inLibrary->getParent()
                                    ->getDumpRestoreData());
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
dump( const LibrarySharedPtr &inLibrary,
        const DumpRestoreData &inData ) throw(Error) {
    if( !inLibrary )
    {
        //TBD::ERROR
    }
    if( inLibrary->getName().empty() )
    {
        //TBD::ERROR
    }
    if( inData.getDumpPath().empty() )
    {
        //TBD::ERROR
    }
    std::string fileName = inData.getDumpPath() + "/";
    fileName += inLibrary->getName() + ".lib";
    std::ofstream fout( fileName.c_str(),
                        std::ios::out | std::ios::binary );
    if( !fout )
    {
        //TBD::ERROR
    }
    boost::archive::binary_oarchive libAr( fout );

    libAr & inLibrary; //Dump lib
}

LibrarySharedPtr
restore( const std::string &inName,
        const RootSharedPtr &inParent) throw(Error) {
    LibrarySharedPtr lib;
    if( !inParent )
    {
        //TBD::ERROR
    }
    try
    {
        lib = restore( inName, inParent,
                                inParent->getDumpRestoreData() );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
    return lib;
}

LibrarySharedPtr
restore( const std::string &inName,
        const RootSharedPtr &inParent,
        const DumpRestoreData &inData ) throw(Error) {
    if( inName.empty() )
    {
        //TBD::ERROR
    }
    LibrarySharedPtr library;
    std::string fileName = inData.getDumpPath() + "/";
    fileName += inName + ".lib";
    std::ifstream fin( fileName.c_str(),
                std::ios::in | std::ios::binary );
    if( !fin )
    {
        //TBD::ERROR
    }
    boost::archive::binary_iarchive libAr( fin );
    libAr & library;
    if( library )
    {
        library->setParent( inParent );
    }
    return library;
}

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
