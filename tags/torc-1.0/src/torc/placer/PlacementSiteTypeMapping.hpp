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
/// \brief Header for the Placement class.

#ifndef TORC_PLACER_PLACEMENTSITETYPEMAPPING_HPP
#define TORC_PLACER_PLACEMENTSITETYPEMAPPING_HPP

#include "torc/architecture/DDB.hpp"
#include <string>
#include <vector>
#include <map>
#include <set>

namespace torc {
namespace placer {

	/// \brief Placement mapping for legal instance types to site types.
	
	class PlacementSiteTypeMapping {
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
		class MappingSiteType {
			std::string mName;
			std::vector<uint32> mLegalSiteTypes;
			std::vector<uint32> mLegalInstanceTypes;
		public:
			MappingSiteType (std::string inName) : mName(inName) {}
			const std::string& getName() { return mName; }
			std::vector<uint32>& getSiteTypes() { return mLegalSiteTypes; }
			std::vector<uint32>& getInstanceTypes() { return mLegalInstanceTypes; }
		};
		
		typedef std::vector<MappingSiteType> TypeVector;
		
		
	protected:
	// members	
		DDB& mDB;
		const Sites& mSites;
		const architecture::Array<const architecture::PrimitiveDef>& mSiteTypes;
		
		TypeVector mTypeVector;
		std::map<std::string, uint32> mTypeLookup;
		
	public:
		PlacementSiteTypeMapping(DDB& inDB)
			: mDB(inDB), mSites(inDB.getSites()), mSiteTypes(mSites.getSiteTypes()) {
			
			prepareStructures();
			// this is the default mapping so it is simple matching of instance to site types.
			// it is assumed that the type index on the instance side matches the database.
			for (uint32 i = 0; i < mSites.getSiteTypeCount(); i++) {
				addMapData(mSiteTypes[i].getName(), mSiteTypes[i].getName());
			}
			
			//populateGroups();
		}
		~PlacementSiteTypeMapping() {}
	protected:
		// Set up the structures to receive mapping data
		void prepareStructures() {
			mTypeVector.clear();
			mTypeLookup.clear();
			// set up a map of site type indices
			for (uint32 i = 0; i < mSites.getSiteTypeCount(); i++) {
				mTypeVector.push_back(MappingSiteType(mSiteTypes[i].getName()));
				mTypeLookup.insert(std::pair<std::string, uint32>(mSiteTypes[i].getName(), i));
			}
		}

		void addMapData(std::string itype, std::string stype) {
			uint32 ii = mTypeLookup[itype];
			uint32 si = mTypeLookup[stype];
			
			mTypeVector[ii].getSiteTypes().push_back(si);
			mTypeVector[si].getInstanceTypes().push_back(ii);
			
		}
	public:
		uint32 getNumTypes() { return mTypeLookup.size(); }
		
		std::vector<MappingSiteType>& getTypeVector() { 
			return mTypeVector;
		}
		
		std::vector<uint32>& getLegalSitesForInstance(uint32 inInstanceTypeIndex) {
			return mTypeVector[inInstanceTypeIndex].getSiteTypes();
		}
		std::vector<uint32>& getLegalInstancesForSite(uint32 inSiteTypeIndex) {
			return mTypeVector[inSiteTypeIndex].getInstanceTypes();
		}
		MappingSiteType& getType(uint32 inIndex) { return mTypeVector[inIndex]; }
		
		MappingSiteType& getType(std::string& inName) {
			return mTypeVector[mTypeLookup[inName]];
		}
		const std::string& getName(uint32 inIndex) { return mTypeVector[inIndex].getName(); }
		
		/// \brief Get the type index for a given type name, creates a new entry if not found.
		uint32 getTypeIndex(const std::string& inType) { return mTypeLookup[inType]; }
		
		/*std::vector<uint32>& operator[] (const uint32 inIndex) {
			return mTypeVector[inIndex].getDeviceSiteTypes();
		}
		uint32& operator[] (const std::string& inString) {
			return mTypeLookup[inString];
		}*/

		
	}; // class PlacementSiteTypeMapping
} // namespace placer
} // namespace torc

#endif // TORC_PLACER_PLACEMENTSITETYPEMAPPING_HPP
