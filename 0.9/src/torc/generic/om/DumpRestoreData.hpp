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

#ifndef TORC_GENERIC_DUMP_RESTORE_DATA_HPP
#define TORC_GENERIC_DUMP_RESTORE_DATA_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifdef GENOM_SERIALIZATION
#include <string>

#include <boost/shared_ptr.hpp>

namespace torc { namespace generic { class ObjectFactory; } }

namespace torc {
namespace generic {

class DumpRestoreData {
  public:
    inline std::string
    getDumpPath() const throw();

    inline ObjectFactorySharedPtr
    getFactory() const throw();

    inline bool
    getRestoreAllComponents() const throw();

    explicit DumpRestoreData(
			const std::string &inDumpPath = std::string(),
			ObjectFactorySharedPtr inFactory
						= ObjectFactorySharedPtr(),
			bool inRestoreAllComponents = false  );

    DumpRestoreData( const DumpRestoreData &inRhs );

    ~DumpRestoreData() throw();

    DumpRestoreData &
    operator = ( const DumpRestoreData &inRhs );

  private:
    std::string mDumpPath;
    ObjectFactorySharedPtr mFactory;
    bool mRestoreAllComponents;
};

inline std::string
DumpRestoreData::getDumpPath() const throw() {
    return mDumpPath;
}

inline ObjectFactorySharedPtr
DumpRestoreData::getFactory() const throw() {
	return mFactory;
}

inline bool
DumpRestoreData::getRestoreAllComponents() const throw() {
    return mRestoreAllComponents;
}

} // namespace torc
} // namespace generic
#endif //GENOM_SERIALIZATION

#endif //TORC_GENERIC_DUMP_RESTORE_DATA_HPP
