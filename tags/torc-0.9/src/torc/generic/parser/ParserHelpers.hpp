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

#ifndef TORC_GENERIC_PARSER_PARSERHELPERS_HPP
#define TORC_GENERIC_PARSER_PARSERHELPERS_HPP

#include <string>
#include <vector>
#include <list>

#include <boost/shared_ptr.hpp>

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/Value.hpp"

namespace torc { namespace generic { class Parameter; } }
namespace torc { namespace generic { class PortAttributes; } }
namespace torc { namespace generic { class TimeStamp; } }
namespace torc { namespace generic { class Written; } }
namespace torc { namespace generic { class InterfaceAttributes; } }
namespace torc { namespace generic { class LogicValueAttributes; } }

namespace torc {
namespace generic {

struct NameData {
    std::string mName;
    std::string mOriginalName;
    std::vector<size_t> mIndices;

    ~NameData() throw()
    {
    }
};

struct ArrayData {
    NameData *mNameData;
    std::vector<size_t> mIndices;
};

struct ViewRefData {
    NameData *mViewName;
    NameData *mCellName;
    NameData *mLibraryName;

    ViewRefData()
        :mViewName( NULL ),
        mCellName( NULL ),
        mLibraryName( NULL ) {
    }

    ~ViewRefData() {
        delete mViewName;
        delete mCellName;
        delete mLibraryName;
    }
};

struct CellRefData {
    NameData *mCellName;
    NameData *mLibraryName;

    CellRefData()
        :mCellName( NULL ),
        mLibraryName( NULL ) {
    }

    ~CellRefData() {
        delete mCellName;
        delete mLibraryName;
    }
};

struct LogicRefData {
    NameData *mLogicName;
    NameData *mLibraryName;

    LogicRefData()
        :mLogicName( NULL ),
        mLibraryName( NULL ) {
    }

    ~LogicRefData() {
        delete mLogicName;
        delete mLibraryName;
    }
};

struct InstanceRefData {
    NameData *mName;
    ViewRefData *mView;
    InstanceRefData()
        :mName( NULL ),
        mView( NULL ) {
    }
};

struct PortRefData {
    PortRefData *mParentPort;
    NameData *mPortName;
    NameData *mInstanceName;
    ViewRefData *mView;

    PortRefData()
        :mParentPort( NULL ),
        mPortName( NULL ),
        mInstanceName( NULL ),
        mView( NULL ) {
    }

    ~PortRefData()
    {
        delete mParentPort;
        delete mPortName;
        delete mInstanceName;
        delete mView;
    }
};

struct NetRefData {
    NetRefData *mParentNet;
    NameData *mNetName;
    NameData *mInstanceName;
    ViewRefData *mView;

    NetRefData()
        :mParentNet( NULL ),
        mNetName( NULL ),
        mInstanceName( NULL ),
        mView( NULL ) {
    }

    ~NetRefData()
    {
        delete mParentNet;
        delete mNetName;
        delete mInstanceName;
        delete mView;
    }
};

struct PortInstData {
    PortRefData *mRefData;
    PortAttributes *mAttribute;

    PortInstData()
        :mRefData( NULL ),
        mAttribute( NULL ) {
    }

    ~PortInstData() {
        delete mRefData;
    }
};

struct PortListData {
    std::vector<PortRefData *> mPortRefs;

    PortListData()
        :mPortRefs() {
    }

    ~PortListData() {
        for( std::vector<PortRefData *>::iterator it
                                            = mPortRefs.begin();
                it != mPortRefs.end(); ++it )
        {
            delete *it;
        }
    }
};

struct VersionData
{
    int32_t mFirst;
    int32_t mSecond;
    int32_t mThird;
};

struct ValueData
{
    std::vector<Value> mValues;
};

struct PairData
{
    int32_t mX;
    int32_t mY;

    PairData( int32_t inX = 0, int32_t inY = 0 )
        : mX( inX ),
        mY( inY ) {
    }
};

struct PairStrData {
    std::string mFirst;
    std::string mSecond;
    
    PairStrData( std::string inFirst = std::string(), 
                    std::string inSecond = std::string())
        :mFirst( inFirst ), 
        mSecond( inSecond ) { 
    }
};

struct ParamAssignInfo
{
    NameData *mNameData;
    ValueData *mValueData;

    ParamAssignInfo( NameData *inNameData = NULL,
            ValueData *inValueData = NULL )
        :mNameData( inNameData ),
        mValueData( inValueData ) {
    }
    ~ParamAssignInfo() {
        delete mNameData;
        delete mValueData;
    }
};

struct InstanceInfo {
    ViewRefData *mViewRefData;
    std::vector<ParamAssignInfo *> mParamAssign;
    std::vector<PortInstData *> mPortInst;
    std::vector< std::string > mUserData;
    std::vector< std::string > mComment;
    std::string mDesignator;
    TimingSharedPtr mTiming;

    InstanceInfo()
        :mViewRefData(NULL),
         mParamAssign(),
         mPortInst(),
         mUserData(),
         mComment(),
         mDesignator(),
         mTiming() {
    }

    ~InstanceInfo() {
        delete mViewRefData;
        for( std::vector<ParamAssignInfo *>::iterator
                it = mParamAssign.begin();
                it != mParamAssign.end(); it++ )
        {
            delete (*it);
        }
        for( std::vector<PortInstData *>::iterator
                it = mPortInst.begin();
                it != mPortInst.end(); it++ )
        {
            delete (*it);
        } 
        mUserData.clear(); 
        mComment.clear();
   }
};

struct LogicListData {
    std::list<NameData *> mNameDataList;

    LogicListData()
        :mNameDataList() {
    }

    ~LogicListData() {
        for( std::list<NameData *>::iterator it
                                            = mNameDataList.begin();
                it != mNameDataList.end(); ++it )
        {
            delete *it;
        }
    }
};

} //namespace generic 
} //namespace torc

#endif // TORC_GENERIC_PARSER_PARSERHELPERS_HPP
