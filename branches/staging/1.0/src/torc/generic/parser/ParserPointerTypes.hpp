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

#ifndef TORC_GENERIC_PARSER_PARSERPOINTERTYPES_HPP
#define TORC_GENERIC_PARSER_PARSERPOINTERTYPES_HPP

//BOOST
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace torc {
namespace generic {

class EdifContext;
typedef boost::shared_ptr<EdifContext> EdifContextSharedPtr;
typedef boost::weak_ptr<EdifContext> EdifContextWeakPtr;

class Linker;
typedef boost::shared_ptr<Linker> LinkerSharedPtr;
typedef boost::weak_ptr<Linker> LinkerWeakPtr;

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_PARSER_PARSERPOINTERTYPES_HPP
