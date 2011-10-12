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

#ifndef TORC_GENERIC_OM_VISITORAPPLIER_HPP
#define TORC_GENERIC_OM_VISITORAPPLIER_HPP

namespace torc
{
namespace generic
{

/**
 * This is an utility functor class that can be used to apply
 * visitor to a list of genOM objects. The type _Tp should be
 * replaced by the visitor type.
 */

template<typename _Tp>
class VisitorApplier {
    public:
        VisitorApplier( _Tp &inVisitor )
            :mVisitor( inVisitor ) {
        }

        template<typename _Up>
        void
        operator()( const boost::shared_ptr<_Up> &inVisited
                            ) const throw( Error ) {
            try
            {
                inVisited->accept( mVisitor );
            }
            catch( Error &e )
            {
                e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                throw;
            }
        }
    private:
        _Tp &mVisitor;
};

} //namespace generic
} //namespace torc


#endif // TORC_GENERIC_OM_VISITORAPPLIER_HPP
