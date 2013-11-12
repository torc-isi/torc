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

#include "torc/generic/DumpRestoreData.hpp"

#ifdef GENOM_SERIALIZATION

namespace torc {
namespace generic {

DumpRestoreData::DumpRestoreData(const std::string& inDumpPath, ObjectFactorySharedPtr inFactory,
	bool inRestoreAllComponents) : mDumpPath(inDumpPath), mFactory(inFactory),
	mRestoreAllComponents(inRestoreAllComponents) {}

DumpRestoreData::DumpRestoreData(const DumpRestoreData& inRhs) : mDumpPath(inRhs.mDumpPath),
	mFactory(inRhs.mFactory), mRestoreAllComponents(inRhs.mRestoreAllComponents) {}

DumpRestoreData::~DumpRestoreData() throw () {}

DumpRestoreData& DumpRestoreData::operator =(const DumpRestoreData& inRhs) {
	if(this != &inRhs) {
		mDumpPath = inRhs.mDumpPath;
		mFactory = inRhs.mFactory;
		mRestoreAllComponents = inRhs.mRestoreAllComponents;
	}
	return *this;
}

} // namespace torc
} // namespace generic

#endif //GENOM_SERIALIZATION
