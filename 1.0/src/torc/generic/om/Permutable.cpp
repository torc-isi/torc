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

#include "torc/generic/om/Permutable.hpp"
#include "torc/generic/om/Port.hpp"

namespace torc {

namespace generic {

/**
 * Create a permutable.
 *
 * @param[in] inPorts Vector of ports to this permutable.
 * @param[in] inViewPtr Pointer to parented(View) object.
 * @param[in] inParentPermutable Pointer to parent permutable.
 *
 * @return Pointer to created permutable.
 */
PermutableSharedPtr
Permutable::Factory::newPermutablePtr( const std::vector< PortSharedPtr > & inPorts,
        const ViewSharedPtr & inViewPtr,
        const PermutableSharedPtr & inParentPermutable ) throw(Error) {
    try
    {
        PermutableSharedPtr newPermutable;
        create( newPermutable ); 
        newPermutable->setPorts( inPorts );
        if( inParentPermutable )
        {
            inParentPermutable->addChildPermutable( newPermutable );
        }
        else if( inViewPtr )
        {
            inViewPtr->addPermutable( newPermutable );
        }
        return newPermutable;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
 * Set the list of ports to this permutable.
 * It will lead to a linear traversal on the list.
 * So usage of this API is not recommended.
 *
 * @param[in] inSource Vector of ports
 */
void
Permutable::setPorts(const std::vector< PortSharedPtr > &inSource) throw(Error) {
    std::vector< PortSharedPtr >::const_iterator port = inSource.begin();
    std::vector< PortSharedPtr >::const_iterator end = inSource.end();
    for(;port != end; ++port)
    {
        try
        {
            addPort( *port );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
}

/**
 * Add a port to the list of ports. Empty pointer is ignored.
 *
 * @param[in] inPort Pointer to port to be added.
 *
 * @exception Error Could not add port, because Port name is empty
 */
void
Permutable::addPort(
    const PortSharedPtr &inPort) throw(Error) {
    if( !inPort )
    {
        return;
    }
    std::string name = inPort->getName();
    if( name.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Port name", name);
        throw e;
    }
    mPorts.push_back( inPort );
}

/**
 * Set the nested permutable.
 *
 * @param[in] inSource Vector containing permutables
 * @exception Error Could not add permutable because pointer to the permutable does not exist
 */
void
Permutable::setChildren(
        const std::vector< PermutableSharedPtr > & inSource) throw(Error) {
    std::vector< PermutableSharedPtr >::const_iterator entry = inSource.begin();
    std::vector< PermutableSharedPtr >::const_iterator end = inSource.end();
    for(; entry != end; ++entry )
    {
        try 
        {
            addChildPermutable( *entry );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
}

/**
 * Add a nested permutable/nonpermutable to this permutable.
 *
 * @param[in] inPermutable Nested permutable/nonpermutable
 * @exception Error Could not add permutable because pointer to the permutable does not exist
 */
bool
Permutable::addChildPermutable(
        const PermutableSharedPtr & inPermutable ) throw(Error) {
    if( !inPermutable )
    {
        Error e( eMessageIdErrorPointerToItemDoesNotExist,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Pointer to the permutable object does not exist", inPermutable);
        throw e;
    }
    if( inPermutable ) {
        mChildren.push_back( inPermutable );
        return true;
    }
    else {
        return false;
    }
}

void 
Permutable::setIsNonPermutable( const bool & value ) throw() {
    mIsNonPermutable = value;
}

void 
Permutable::accept(BaseVisitor & visitor) throw(Error) {
    try
    {
        runVisitor( *this, visitor );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
 * Get the total number of bits of the composition
 * @return Number of bits
 */
size_t
Permutable::getSize() const throw() {
    size_t size = 0;
    size_t pSize = 0;
    size_t cSize = 0;
    Permutable::PermutableType pType = getPermutableType();

    std::vector< PermutableSharedPtr > outPermutables;
    getChildren( outPermutables );
    std::vector< PermutableSharedPtr >::iterator permutableIt 
                    = outPermutables.begin();

    std::vector< PortSharedPtr > outPorts;
    getPorts( outPorts );
    std::vector< PortSharedPtr >::iterator portIt
                    = outPorts.begin();

    if( Permutable::ePermutableParent == pType )
    {
        if( !outPorts.empty() )
        {
            pSize = (*portIt)->getSize();
        }
        else if( !outPermutables.empty() )
        {
            for( ; permutableIt != outPermutables.end() ; permutableIt ++ )
            {
                pSize += (*permutableIt)->getSize();
                for( ; portIt != outPorts.end(); portIt ++ )
                {
                    pSize += (*portIt)->getSize();
                } 
            }
        }    
        else
        {
            pSize = 0;
        }
    }
    else
    {
        if( !outPorts.empty() )
        {
            for( ; portIt != outPorts.end(); portIt ++ )
            {
                cSize += (*portIt)->getSize();
            } 
        }
        if( !outPermutables.empty() )
        {
            for( ; permutableIt != outPermutables.end() ; permutableIt ++ )
            {
                cSize += (*permutableIt)->getSize();
            }
        }
    }
    size = pSize + cSize;
    return size;
}

/**
 * Set the permutable type.
 *
 * @param[in] inSource PermutableType
 */
void
Permutable::setPermutableType(const PermutableType & inSource) throw() {
    mPermutableType = inSource;
}

Permutable::Permutable() 
    : Visitable(),
    SelfReferencing<Permutable>(), 
    mPorts(),
    mChildren(),
    mIsNonPermutable( false ),
    mPermutableType() {
}

Permutable::~Permutable() throw() {
}

} // namespace torc::generic

} // namespace torc
