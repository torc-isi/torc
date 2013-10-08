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

/// \file
/// \brief Header for the HeuristicBase class.

#ifndef TORC_ROUTER_ROUTERHEURISTICBASE_HPP
#define TORC_ROUTER_ROUTERHEURISTICBASE_HPP

#include "torc/architecture/DDB.hpp"
#include <boost/unordered_map.hpp>
#include <boost/any.hpp>


namespace torc {
namespace router {

	/// \brief Provides the interface for net routers.
	class RouterHeuristicBase {
	// types
		/// \brief Imported type names
		typedef architecture::DDB DDB;
		typedef boost::unordered_map<boost::uint32_t, boost::any> ParameterMap;
				
	protected:
	// members
		/// \brief Database reference.
		DDB& mDB;
		/// \brief Parameter map.
		ParameterMap mParameters;
		
	public:
	// constructor
		/// \brief Public Constructor
		RouterHeuristicBase(DDB& inDB) : mDB(inDB) {}
		/// \brief Destructor.
		virtual ~RouterHeuristicBase() {}
		
		/// \brief Get a parameter.
		boost::any getParameter(boost::uint32_t index) {
			ParameterMap::iterator p = mParameters.find(index);
			if (p != mParameters.end()) {
				return p->second;
			}
			return boost::any();
		}
		/// \brief Set a parameter.
		void setParameter(boost::uint32_t index, boost::any inParameter) {
			mParameters[index] = inParameter;
		}
		/// \brief Do something with the parameters.
		virtual void processParameters() {}

	};	

} // namespace router
} // namespace torc

#endif // TORC_ROUTER_ROUTERHEURISTICBASE_HPP
