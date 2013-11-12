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

/// \file
/// \brief Header for the DeviceWrapper class.

#ifndef TORC_PLACER_DEVICEWRAPPER_HPP
#define TORC_PLACER_DEVICEWRAPPER_HPP

#include "torc/architecture/DDB.hpp"
#include "torc/placer/PlacementSiteTypeMapping.hpp"

namespace torc {
namespace placer {

	/// \brief Wrapper of the device database for placing the design.
	
	class DeviceWrapper {
	protected:
	//types
		typedef architecture::DDB DDB;
		typedef architecture::Sites Sites;
		typedef boost::uint32_t uint32;
		
		//typedef std::map<uint32, uint32> InstanceTypeToCandidateTypeMap;
		typedef std::vector<uint32> InstanceTypeToCandidateTypeMap;
		
		typedef architecture::Site Site;
		typedef std::vector<const Site*> SitePtrVector;
		typedef std::vector<SitePtrVector> SitePtrVectorVector;
		typedef std::vector<SitePtrVectorVector> SitePtrVectorVectorVector;
		
		typedef std::set<Site*> SitePtrSet;
		typedef std::vector<SitePtrSet> SitePtrSetVector;
		typedef std::vector<SitePtrSetVector> SitePtrSetVectorVector;

	public:	
		DDB& mDB;
	public:
		const Sites& mSites;
		PlacementSiteTypeMapping& mTypeMapping;
		

	public:
		DeviceWrapper(DDB& inDB, PlacementSiteTypeMapping& inMapping)
			: mDB(inDB), mSites(inDB.getSites()), mTypeMapping(inMapping) {
			
		}
		~DeviceWrapper() {}
		
	}; // class DeviceWrapper
} // namespace placer
} // namespace torc

#endif // TORC_PLACER_DEVICEWRAPPER_HPP
