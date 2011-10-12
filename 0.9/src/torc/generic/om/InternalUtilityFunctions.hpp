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

#ifndef TORC_GENERIC_INTERNAL_UTILITY_FUNCTIONS_HPP
#define TORC_GENERIC_INTERNAL_UTILITY_FUNCTIONS_HPP


namespace torc {
namespace generic {

template<typename _Connectable>
void
findLeafConnectable( std::vector< std::string > &nestedNames,
        boost::shared_ptr<_Connectable> &conn ) throw(Error)
{
    std::vector< std::string >::reverse_iterator name
                                = nestedNames.rbegin();
    std::vector< std::string >::reverse_iterator end
                                = nestedNames.rend();
    ++name;
    if( !conn )
    {
        Error e( eMessageIdErrorNullPointer,
            __FUNCTION__, __FILE__, __LINE__ );
        throw e;
    }
    for( ;name != end; ++name )
    {
        if( eCompositionTypeBundle
                != conn->getCompositionType() )
        {
            Error e( eMessageIdErrorUnsupoortedOperation,
                __FUNCTION__, __FILE__, __LINE__ );
            e.saveContextData("Name", *name );
            e.saveContextData("Operation",
                                    std::string( "Find child" ) );
            throw e;
        }
        std::vector< boost::shared_ptr<_Connectable> > children;
        conn->getChildren( children );
        bool found = false;
        for( typename
            std::vector< boost::shared_ptr<_Connectable> >::iterator
                it = children.begin(); it != children.end(); ++it )
        {
            if( (*it)->getName() == *name )
            {
                conn = (*it);
                found = true;
                break;
            }
        }
        if( !found )
        {
            Error e( eMessageIdErrorItemNotFound,
                __FUNCTION__, __FILE__, __LINE__ );
            e.saveContextData("Name", (*name));
            throw;
        }
    }
}

template<typename _Connectable>
void
connectNetToElement( const std::vector<size_t> &inIndices,
            const boost::shared_ptr<_Connectable> &inConn,
            const NetSharedPtr &inNet ) throw(Error)
{
    try
    {
        if( inIndices.empty() )
        {
            //TBD::CHECK WIDTH
            inConn->connect( inNet );
        }
        else
        {
            boost::shared_ptr<_Connectable> bit
                = inConn->get( inIndices);
            bit->connect( inNet );
        }
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<typename _Base, typename _Derived>
struct IndexFinder
        : _Derived::Visitor {
    std::vector<size_t>
    operator()( const boost::shared_ptr<_Base> &inPtr )
    {
        inPtr->accept( *this );
        return mIndices;
    }

    void
    visit( _Derived &inObj) throw(Error)
    {
        try
        {
            mIndices = inObj.getIndices();
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }

    IndexFinder()
        : _Derived::Visitor(),
        mIndices() {
    }
    ~IndexFinder() throw() {
    }

    std::vector<size_t> mIndices;
};

} //namespace generic
} //namespace torc
#endif //TORC_GENERIC_INTERNAL_UTILITY_FUNCTIONS_HPP
