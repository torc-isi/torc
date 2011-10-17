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

#ifndef TORC_GENERIC_OM_PARAMETERMAP_HPP
#define TORC_GENERIC_OM_PARAMETERMAP_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include <string>

//BOOST
#include <boost/shared_ptr.hpp>

#include "torc/generic/om/ParameterContext.hpp"
#include "torc/generic/om/SymTab.hpp"

namespace torc { namespace generic { class Parameter; }  }

namespace torc {

namespace generic {

/**
 * Stores Name-Parameter mappings. This map can be used to get and set values for different parameters.
 */
class ParameterMap
{
  public:
    typedef SymTab< std::string,
        ParameterSharedPtr, true> TabParams;
    typedef boost::shared_ptr<TabParams> TabParamsPtr;
    /**
    * Get a named parameter or null if none exists
    * @return The parameter
    */
    ParameterSharedPtr
    get(const ParameterContext &inContext,
        const std::string &inName) const throw();

    /**
    * Saves a parameter by name. Overrides existing parameters if it exists.
    * @param[in] inContext the context for which the param is valid
    * @param[in] inName name of param
    * @param[in] inParam pointer to the parameter
    */
    void
    set(const ParameterContext &inContext,
        const std::string &inName,
        const ParameterSharedPtr &inParam) throw();

    /**
     * Get all parameter name-value-pairs
     * @param[in] inContext the context for which the param is valid
     * @param[out] outParams Map to be populated
     */
    void
    getAllParameters( const ParameterContext &inContext,
        std::map< std::string,ParameterSharedPtr > &outParams
) const throw();

    /**
     * Get all parameter name-value-pairs that have been overridden in this context. For a view context this will return an empty map.
     * @param[in] inContext the context for which the param is valid
     * @param[out] outParams Map to be populated
     */
    void
    getOverriddenParameters( const ParameterContext &inContext,
        std::map< std::string,ParameterSharedPtr > &outParams
) const throw();

   /**
    * Apply action on all parameters.
    * @param[in] inContext the context for which the param is valid
    * @param[in] action Action to be applied
    *
    */
    template<typename _Action>
    inline void
    applyOnAllParameters( const ParameterContext &inContext,
            const _Action &action ) throw(Error); 

   /**
    * Apply action on overridden parameters
    * @param[in] inContext the context for which the param is valid
    * @param[in] action Action to be applied
    *
    */
    template<typename _Action>
    inline void
    applyOnOverriddenParameters( const ParameterContext &inContext,
            const _Action &action ) throw(Error); 

    ParameterContext
    getNewContext() throw();

    void
    registerContext(
        const ParameterContext &inContext,
        const ParameterContext &inParentContext = ParameterContext()
                        ) throw(Error);

	bool
	isContextRegistered(
			const ParameterContext &inContext ) const throw();

    void
    unregisterContext( const ParameterContext &inContext ) throw();

    ParameterMap();

    ~ParameterMap() throw();

    ParameterMap(const ParameterMap & source) throw();

    ParameterMap &
    operator=(const ParameterMap & source) throw();

  private:
    struct ParamData {
        TabParams mParams;
        ParameterContext mParentContext;
        ParamData()
            :mParams(),
            mParentContext() {
        }
    };
	typedef boost::shared_ptr<ParamData> ParamDataPtr;
    SymTab<ParameterContext, ParamDataPtr> mParameters;
    ParameterContext mNextContext;
};

template<typename _Action>
inline void
ParameterMap::applyOnAllParameters(
        const ParameterContext &inContext,
        const _Action &action ) throw(Error)
{
    if( !inContext )
    {
        return;
    }
    try
    {
        ParamDataPtr data;
        mParameters.get( inContext, data );
        if( !data )
        {
            return;
        }
        else
        {
            data->mParams.applyOnAll( action );
            applyOnAllParameters(
                    data->mParentContext, action );
        }
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<typename _Action>
inline void
ParameterMap::applyOnOverriddenParameters(
        const ParameterContext &inContext,
        const _Action &action ) throw(Error)
{
    if( !inContext )
    {
        return;
    }
    try
    {
        ParamDataPtr data;
        mParameters.get( inContext, data );
        if( !data )
        {
            return;
        }
        else
        {
            data->mParams.applyOnAll( action );
        }
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

} // namespace torc::generic

} // namespace torc
#endif // TORC_GENERIC_OM_PARAMETERMAP_HPP
