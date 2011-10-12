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
/// \brief Source file for the Placer helper functions.

#include "torc/placer/PlacerHelpers.hpp"

namespace torc {
namespace placer {


void setInstanceTypeIndex(physical::InstanceSharedPtr inInstance, 
	boost::uint32_t inType) {
	inInstance->setAnnotation(common::Annotated::ePlacerInstanceTypeIndex, inType);
}

boost::uint32_t getInstanceTypeIndex(physical::InstanceSharedPtr inInstance) {
	return boost::any_cast<boost::uint32_t>(inInstance->getAnnotation(
		common::Annotated::ePlacerInstanceTypeIndex));
}

void setInstanceSitePtr(physical::InstanceSharedPtr inInstance,
	const architecture::Site* inSite) {
	inInstance->setAnnotation(common::Annotated::ePlacerInstanceSitePtr, inSite);
}

const architecture::Site* getInstanceSitePtr(physical::InstanceSharedPtr inInstance) {
	return boost::any_cast<const architecture::Site*>(
		inInstance->getAnnotation(common::Annotated::ePlacerInstanceSitePtr));
}

} // namespace placer
} // namespace torc
