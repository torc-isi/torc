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

#include "torc/generic/Cloning.hpp"

namespace torc {
namespace generic {

void copyParams(const Instance& inOriginal, const InstanceSharedPtr& outCloned,
	const ObjectFactorySharedPtr& inFactory) {

	ParameterMapSharedPtr params = inOriginal.getParameters();
	ParameterMapSharedPtr clonedParams = outCloned->getParameters();
	ParameterContext origContext = inOriginal.getParameterContext();
	ParameterContext clonedContext = outCloned->getParameterContext();
	std::map<std::string, ParameterSharedPtr> overriddenParams;
	params->getOverriddenParameters(origContext, overriddenParams);
	for(std::map<std::string, ParameterSharedPtr>::iterator it = overriddenParams.begin();
		it != overriddenParams.end(); ++it) {
		ParameterSharedPtr clonedParam = clone((*it).second, inFactory);
		clonedParams->set(clonedContext, (*it).first, clonedParam);
	}
}

} //namespace generic
} //namespace torc
