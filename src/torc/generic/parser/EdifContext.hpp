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

#ifndef TORC_GENERIC_EDIFCONTEXT_HPP
#define TORC_GENERIC_EDIFCONTEXT_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/parser/ParserPointerTypes.hpp"
#include <stack>

//BOOST
#include <boost/shared_ptr.hpp>

namespace torc { namespace generic { class Cell; } }
namespace torc { namespace generic { class Design; } }
namespace torc { namespace generic { class Instance; } }
namespace torc { namespace generic { class Library; } }
namespace torc { namespace generic { class Linker; } }
namespace torc { namespace generic { class Net; } }
namespace torc { namespace generic { class NetBundle; } }
namespace torc { namespace generic { class ObjectFactory; } }
namespace torc { namespace generic { class ParserOptions; } }
namespace torc { namespace generic { class Port; } }
namespace torc { namespace generic { class PortReference; } }
namespace torc { namespace generic { class PortBundle; } }
namespace torc { namespace generic { class Property; } }
namespace torc { namespace generic { class PropertyContainer; } }
namespace torc { namespace generic { class Root; } }
namespace torc { namespace generic { class View; } }

namespace torc {
namespace generic {

class EdifContext {
  public:
    inline RootSharedPtr
    getRoot() const throw();

    void
    setRoot( const  RootSharedPtr &inRoot ) throw();

    inline LinkerSharedPtr
    getLinker() const throw();

    void
    setLinker( const  LinkerSharedPtr &inLinker ) throw();

    inline LibrarySharedPtr
    getCurrentLibrary() const throw();

    void
    setCurrentLibrary(
            const LibrarySharedPtr &inLibrary ) throw();

    inline CellSharedPtr
    getCurrentCell() const throw();

    void
    setCurrentCell( const  CellSharedPtr &inCell ) throw();

    inline DesignSharedPtr
    getCurrentDesign() const throw();

    void
    setCurrentDesign( const  DesignSharedPtr &inDesign ) throw();

    inline ViewSharedPtr
    getCurrentView() const throw();

    void
    setCurrentView( const ViewSharedPtr &inView ) throw();

    inline PortSharedPtr
    getCurrentPort() const throw();

    void
    setCurrentPort( const PortSharedPtr &inPort ) throw();

    inline PortReferenceSharedPtr
    getCurrentPortRef() const throw();

    void
    setCurrentPortRef(
        const PortReferenceSharedPtr &inPortRef ) throw();

    void
    pushCurrentNet( const NetSharedPtr &inNet ) throw(); 

    NetSharedPtr
    getCurrentNet() throw();

    void
    popCurrentNet() throw();

    void
    setCurrentNet( const NetSharedPtr &inNet ) throw();

    PortBundleSharedPtr
    getCurrentPortBundleContext() throw();

    void
    pushPortBundleContext( PortBundleSharedPtr inPortBundleContext ) throw();

    void
    popPortBundleContext() throw();

    NetBundleSharedPtr
    getCurrentNetBundleContext() throw();

    void
    pushNetBundleContext( NetBundleSharedPtr inNetBundleContext ) throw();

    void
    popNetBundleContext() throw();

    inline bool
    getIsInInstance() const throw();

    void
    setIsInInstance( bool inIsInInstance ) throw();

    inline const ParserOptions &
    getOptions() const throw();

    inline ObjectFactorySharedPtr
    getFactory() const throw();

    void
    setFactory(
        const ObjectFactorySharedPtr &inFactory) throw();

    inline bool
    getIsViewBeingLinked() const throw();    

    void
    setIsViewBeingLinked( bool inIsViewBeingLinked ) throw();

    void
    pushProperty(
            const PropertySharedPtr &inProperty) throw();

    void
    popProperty() throw();

    PropertySharedPtr
    getCurrentProperty() throw();

    void
    popPropertyContainer() throw();

    void
    pushPropertyContainer(
        const PropertyContainerSharedPtr &inPropertyContainer ) throw();

    PropertyContainerSharedPtr
    getCurrentPropertyContainer() const throw();

    inline int32_t
    getPropertyDepth() const throw();

    void
    incrementPropertyDepth() throw(); 

    void
    decrementPropertyDepth() throw(); 

    EdifContext( const RootSharedPtr &inRoot,
                const LinkerSharedPtr &inLinker,
                const ObjectFactorySharedPtr &inFactory,
                const ParserOptions &inOptions );
    ~EdifContext() throw();

  private:
    RootSharedPtr mRoot;
    LinkerSharedPtr mLinker;
    ObjectFactorySharedPtr mFactory;
    const ParserOptions &mOptions;
    LibrarySharedPtr mCurrentLibrary;
    DesignSharedPtr mCurrentDesign;
    CellSharedPtr mCurrentCell;
    ViewSharedPtr mCurrentView;
    PortSharedPtr mCurrentPort;
    PortReferenceSharedPtr mCurrentPortRef;
    NetSharedPtr mCurrentNet;
    std::stack< PortBundleSharedPtr > mPortBundleContext;
    std::stack< NetBundleSharedPtr > mNetBundleContext;
    bool mIsInInstance;
    bool mIsViewBeingLinked;
    std::stack< PropertySharedPtr > mProperty;
    std::stack< PropertyContainerSharedPtr > mPropertyContainer;
    std::stack< NetSharedPtr > mNet;
    int32_t mPropertyDepth;
}; 

inline RootSharedPtr
EdifContext::getRoot() const throw() {
    return mRoot;
}

inline LinkerSharedPtr
EdifContext::getLinker() const throw() {
    return mLinker;
}

inline LibrarySharedPtr
EdifContext::getCurrentLibrary() const throw() {
    return mCurrentLibrary;
}

inline DesignSharedPtr
EdifContext::getCurrentDesign() const throw() {
    return mCurrentDesign;
}

inline CellSharedPtr
EdifContext::getCurrentCell() const throw() {
    return mCurrentCell;
}

inline ViewSharedPtr
EdifContext::getCurrentView() const throw() {
    return mCurrentView;
}

inline PortSharedPtr
EdifContext::getCurrentPort() const throw() {
    return mCurrentPort;
}

inline PortReferenceSharedPtr
EdifContext::getCurrentPortRef() const throw() {
    return mCurrentPortRef;
}

inline bool
EdifContext::getIsInInstance() const throw() {
    return mIsInInstance;
}

inline const ParserOptions &
EdifContext::getOptions() const throw() {
    return mOptions;
}

inline ObjectFactorySharedPtr
EdifContext::getFactory() const throw() {
    return mFactory;
}

inline bool
EdifContext::getIsViewBeingLinked() const throw() {
    return mIsViewBeingLinked;
}


inline int32_t
EdifContext::getPropertyDepth() const throw() {
    return mPropertyDepth;
}

} //namespace generic
} //namespace torc

#endif //TORC_GENERIC_EDIFCONTEXT_HPP
