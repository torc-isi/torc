// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
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

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/is_abstract.hpp>
#include <boost/serialization/shared_ptr.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/Cell.hpp"
#include "torc/generic/Library.hpp"
#include "torc/generic/VisitorType.hpp"
#include "torc/generic/View.hpp"

namespace torc {
namespace generic {

#ifdef GENOM_SERIALIZATION
class RestoredViewUpdater {
public:
	void operator()(const ViewSharedPtr& inView) const throw (Error) {
		try {
			inView->setParent(mCell);
			inView->restoreActions();
		} catch(Error& e) {
			e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
			throw;
		}
	}

	RestoredViewUpdater(const CellSharedPtr& inCell) : mCell(inCell) {
	}
private:
	CellSharedPtr mCell;
};
#endif //GENOM_SERIALIZATION
/**
 * Create a cell
 *
 * @param[in] inName Name of the cell to be created.
 * @param[in] inLibraryPtr Pointer to parented(Library) object.
 * @param[in] inCellType Cell Type
 * @param[in] inOriginalName Original name of the cell [optional]
 *
 * @return Pointer to created cell.
 **/
CellSharedPtr Cell::Factory::newCellPtr(const std::string& inName,
	const LibrarySharedPtr& inLibraryPtr, const Cell::Type& inCellType,
	const std::string& inOriginalName) throw (Error) {
	try {
		CellSharedPtr newCell;
		create(newCell);
		newCell->setName(inName);
		newCell->setParent(inLibraryPtr);
		inLibraryPtr->addCell(newCell);
		newCell->setType(inCellType);
		newCell->setOriginalName(inOriginalName);
		return newCell;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

void Cell::accept(BaseVisitor& inoutVisitor) throw (Error) {
	try {
		runVisitor(*this, inoutVisitor);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

/**
 * Set the type of cell
 *
 * @param[in] inSource Type of cell
 */
void Cell::setType(const Cell::Type& inSource) {
	mType = inSource;
}

/**
 * Add a view to the list of views. An empty pointer will be ignored.
 *
 * @param[in] inView Pointer to a view.
 *
 * @exception Error Empty View type 
 * @exception Error View name already exists
 *
 */
void Cell::addView(const ViewSharedPtr& inView) throw (Error) {
	if(!inView) {
		return;
	}
	std::string name = inView->getName();
	if(name.empty()) {
		Error e(eMessageIdErrorEmptyItemName, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Empty View type", name);
		throw e;
	}
	if(false == mViewSymTab.set(name, inView)) {
		Error e(eMessageIdErrorItemAlreadyExists, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("View name", name);
		throw e;
	}
	inView->setParent(getSharedThis());
}

/**
 * Find a view by name.
 *
 * @param[in] inName Name of the cell to be found.
 *
 * @return Pointer to cell if found, empty pointer otherwise.
 */
ViewSharedPtr Cell::findView(const std::string& inName) {
	if(inName.empty()) {
		Error e(eMessageIdErrorEmptyItemName, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("Empty Cell name", inName);
		throw e;
	}
	ViewSharedPtr view;
	mViewSymTab.get(inName, view);
	return view;
}

/**
 * Remove the specified pointer from the view. Empty pointer is ignored.
 *
 * @param[in] inName name of object to be removed
 *
 * @exception Error View is not a member of the views list.
 */
void Cell::removeView(const std::string& inName) throw (Error) {
	if(inName.empty()) {
		Error e(eMessageIdErrorEmptyItemName, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("View name", inName);
		throw e;
	}
	if(false == mViewSymTab.remove(inName)) {
		Error e(eMessageIdErrorItemNotFound, __FUNCTION__, __FILE__, __LINE__);
		e.saveContextData("View name", inName);
		throw e;
	}
	return;
}

Cell::Cell() : Commentable(), Extern(), Nameable(), ParentedObject<Library>(), PropertyContainer(),
	Renamable(), Visitable(), SelfReferencing<Cell>(), UserDataContainer(), StatusContainer(),
	mViewSymTab(), mType(eTypeGeneric) {}

Cell::~Cell() throw () {}

#ifdef GENOM_SERIALIZATION
template <class Archive> void Cell::serialize(Archive& ar, unsigned int) {
	ar & boost::serialization::base_object < Commentable > (*this);
	ar & boost::serialization::base_object < Extern > (*this);
	ar & boost::serialization::base_object < Nameable > (*this);
	ar & boost::serialization::base_object < PropertyContainer > (*this);
	ar & boost::serialization::base_object < Renamable > (*this);
	ar & boost::serialization::base_object < Visitable > (*this);
	ar & boost::serialization::base_object < SelfReferencing<Cell> > (*this);
	ar & mViewSymTab;
	//TBD:: ar & mParameters;
}

void Cell::restoreActions() throw (Error) {
	try {
		mViewSymTab.applyOnAll(RestoredViewUpdater(getSharedThis()));
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

//TO SATISFY THE LINKER
template void Cell::serialize<boost::archive::binary_iarchive>(boost::archive::binary_iarchive& ar,
	const unsigned int);

template void Cell::serialize<boost::archive::binary_oarchive>(boost::archive::binary_oarchive& ar,
	const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace generic
} // namespace torc
