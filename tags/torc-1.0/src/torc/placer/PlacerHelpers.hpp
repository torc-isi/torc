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
/// \brief Header for Placer helper functions.

#ifndef TORC_PLACER_PLACERHELPERS_HPP
#define TORC_PLACER_PLACERHELPERS_HPP

#include "torc/common/Annotated.hpp"
#include "torc/physical/Instance.hpp"
#include "torc/architecture/Sites.hpp"

namespace torc {
namespace placer {

void setInstanceTypeIndex(physical::InstanceSharedPtr inInstance, boost::uint32_t inType);

boost::uint32_t getInstanceTypeIndex(physical::InstanceSharedPtr inInstance);

void setInstanceSitePtr(physical::InstanceSharedPtr inInstance,
	const architecture::Site* inSite);
	
	
const architecture::Site* getInstanceSitePtr(physical::InstanceSharedPtr inInstance);

} // namespace placer
} // namespace torc

#endif // TORC_PLACER_PLACERHELPERS_HPP
