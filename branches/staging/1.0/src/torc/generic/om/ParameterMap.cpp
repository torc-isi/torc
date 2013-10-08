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

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif


#include "torc/generic/om/ParameterMap.hpp"
#include "torc/generic/om/Parameter.hpp"
#include "torc/generic/om/Instance.hpp"
#include "torc/generic/om/View.hpp"

namespace torc {

namespace generic {

ParameterSharedPtr
ParameterMap::get(const ParameterContext &inContext,
                    const std::string &inName) const throw() {
    if( !inContext )
    {
        return ParameterSharedPtr();
    }
    ParameterSharedPtr param;
    ParamDataPtr data;
    mParameters.get( inContext, data );
    if( !data )
    {
        return ParameterSharedPtr();
    }
    else
    {
        data->mParams.get( inName, param );
        if( !param ) //Get default param value from parent
        {
            return get( data->mParentContext, inName );
        }
    }
    return param;
}

void
ParameterMap::set(const ParameterContext &inContext,
    const std::string &inName,
    const ParameterSharedPtr &inParam) throw() {
    ParamDataPtr data;
    mParameters.get( inContext, data );
    if( !data )
    {
        return;
    }
    else
    {
        data->mParams.set( inName, inParam );
    }
    return;
}

void
ParameterMap::getAllParameters(
    const ParameterContext &inContext,
    std::map< std::string,ParameterSharedPtr > &outParams) const throw() {
    typedef std::map< std::string,ParameterSharedPtr > DataMap;
    ParamDataPtr data;
    mParameters.get( inContext, data );
    if( !data )
    {
        return;
    }
    else
    {
        if( data->mParentContext )
        {
            ParamDataPtr parentData;
            mParameters.get( data->mParentContext, parentData );
            parentData->mParams.getValueMap(outParams);
            DataMap::iterator param = outParams.begin();
            DataMap::iterator pEnd = outParams.end();
            for(;param != pEnd; ++param )
            {
                ParameterSharedPtr newValue;
                data->mParams.get( (*param).first, newValue );
                //Value was overridden in child
                if( newValue )
                {
                    outParams[ (*param).first ] = newValue;
                }
            }
        }
        else
        {
            //We have the cell context itself
            data->mParams.getValueMap(outParams);
        }
    }

    return;
}

void
ParameterMap::getOverriddenParameters(
    const ParameterContext &inContext,
    std::map< std::string,ParameterSharedPtr > &outParams) const throw() {
    typedef std::map< std::string,ParameterSharedPtr > DataMap;
    ParamDataPtr data;
    mParameters.get( inContext, data );
    if( !data )
    {
        return;
    }
    else
    {
        //We have the cell context itself
        data->mParams.getValueMap(outParams);
    }
    return;
}

ParameterContext
ParameterMap::getNewContext() throw() {
    return mNextContext++;
}

void
ParameterMap::registerContext(
        const ParameterContext &inContext,
        const ParameterContext &inParentContext) throw(Error)
{
    ParamDataPtr data;
    mParameters.get( inContext, data);
    if( data )
    {
        //TBD::ERROR
    }
    else
    {
        data = boost::shared_ptr<ParamData>( new ParamData() );
        data->mParentContext = inParentContext;
        mParameters.set( inContext, data );
    }
}

bool
ParameterMap::isContextRegistered(
        const ParameterContext &inContext ) const throw()
{
    ParamDataPtr data;
    mParameters.get( inContext, data);
    return data;
}

void
ParameterMap::unregisterContext(
                const ParameterContext &inContext ) throw()
{
    mParameters.remove( inContext );
}

ParameterMap::ParameterMap()
    :mParameters(),
    mNextContext( 1 ) {
}

ParameterMap::~ParameterMap() throw() {
}

ParameterMap::ParameterMap(
                const ParameterMap & source) throw()
    :mParameters( source.mParameters ),
    mNextContext( 1 ) {
}

ParameterMap &
ParameterMap::operator=(
                const ParameterMap & source) throw() {
    if( this != &source )
    {
        mParameters = source.mParameters;
        mNextContext = 1;
    }
    return *this;
}


} // namespace torc::generic

} // namespace torc
