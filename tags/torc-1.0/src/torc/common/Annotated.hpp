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
/// \brief Header for the Annotated class.

#ifndef TORC_COMMON_ANNOTATED_HPP
#define TORC_COMMON_ANNOTATED_HPP

#include <boost/unordered_map.hpp>
#include <boost/any.hpp>

namespace torc {
namespace common {

	/// \brief Concept for any object that can be annotated.
	class Annotated {
		typedef boost::uint32_t uint32;
	protected:
	// members
		/// \brief Map containing any annotations.
		boost::unordered_map<boost::uint32_t, boost::any> mAnnotations;
		
	public:
	// constructors
		/// \brief Default constructor containing no annotations.
		Annotated() {}
		/// \brief Destructor.
		~Annotated() {}
	// accessors
		/// \brief Get an annotation.
		/// \detail Returns an empty annotation if the index doesn't exist.
		boost::any getAnnotation(uint32 inKey) { return mAnnotations[inKey]; }
		/// \brief Set an annotation.
		void setAnnotation(uint32 inKey, boost::any inValue) { mAnnotations[inKey] = inValue; }
		/// \brief Remove an annotation.
		void removeAnnotation(uint32 inKey) { mAnnotations.erase(inKey); }
		/// \brief Check if an annotation exists.
		bool hasAnnotation(uint32 inKey) { 
			return (mAnnotations.find(inKey) != mAnnotations.end());
		}
		
	// enums
		/// \brief Enumeration for all types of annotations.
		enum EAnnotationType {
			ePlacerInstanceTypeIndex,
			ePlacerInstanceSitePtr,
			ePlacerInstanceDependentInstanceAbove,
			ePlacerInstanceDependsOnInstanceBelow,
			eRouterNetTilewireSources,
			eRouterNetTilewireSinks,
			eRouterNetArcList,
			eRouterNetRouteNodePtrVector,
			eRouterNetRouteTime,
			eRouterNetArcVector,
			eAnnotationGlobalRouter,
			eAnnotationCount
		};
	};

} // namespace common
} // namespace torc

#endif // TORC_COMMON_ANNOTATED_HPP
