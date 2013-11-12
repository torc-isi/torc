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

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif 

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/PortAttributes.hpp"

namespace torc {
namespace generic {

void PortAttributes::setUnused(const Value& inSource) {
	mUnused = inSource;
}

void PortAttributes::setDesignator(const Value& inSource) {
	mDesignator = inSource;
}

void PortAttributes::setAcLoad(const Value& inSource) {
	mAcLoad = inSource;
}

void PortAttributes::setPortDelay(const PortDelay& inSource) {
	mPortDelay = inSource;
	mPortDelaySet = true;
}

void PortAttributes::setDcFaninLoad(const Value& inSource) {
	mDcFaninLoad = inSource;
}

void PortAttributes::setDcFanoutLoad(const Value& inSource) {
	mDcFanoutLoad = inSource;
}

void PortAttributes::setDcMaxFanin(const Value& inSource) {
	mDcMaxFanin = inSource;
}

void PortAttributes::setDcMaxFanout(const Value& inSource) {
	mDcMaxFanout = inSource;
}

PortAttributes::PortAttributes() : mDcFaninLoad(), mDcFanoutLoad(), mDcMaxFanin(), mDcMaxFanout(),
	mAcLoad(), mPortDelay(), mPortDelaySet(false) {}

PortAttributes::~PortAttributes() throw () {}

PortAttributes::PortAttributes(const PortAttributes& inSource)
	: mDcFaninLoad(inSource.mDcFaninLoad), mDcFanoutLoad(inSource.mDcFanoutLoad),
	  mDcMaxFanin(inSource.mDcMaxFanin), mDcMaxFanout(inSource.mDcMaxFanout),
	  mAcLoad(inSource.mAcLoad), mPortDelay(inSource.mPortDelay),
	  mPortDelaySet(inSource.mPortDelaySet) {}

PortAttributes& PortAttributes::operator=(const PortAttributes& inSource) {
	if(this != &inSource) {
		mDcFaninLoad = inSource.mDcFaninLoad;
		mDcFanoutLoad = inSource.mDcFanoutLoad;
		mDcMaxFanin = inSource.mDcMaxFanin;
		mDcMaxFanout = inSource.mDcMaxFanout;
		mAcLoad = inSource.mAcLoad;
		mPortDelay = inSource.mPortDelay;
		mPortDelaySet = inSource.mPortDelaySet;
	}
	return *this;
}

#ifdef GENOM_SERIALIZATION
template <class Archive> void PortAttributes::serialize(Archive& ar, unsigned int) {
	ar & mDcFaninLoad;
	ar & mDcFanoutLoad;
	ar & mDcMaxFanin;
	ar & mDcMaxFanout;
	ar & mAcLoad;
	ar & mPortDelay;
	ar & mPortDelaySet;
}

//TO SATISFY THE LINKER
template void PortAttributes::serialize<boost::archive::binary_iarchive>(
	boost::archive::binary_iarchive& ar, const unsigned int);

template void PortAttributes::serialize<boost::archive::binary_oarchive>(
	boost::archive::binary_oarchive& ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace generic
} // namespace torc
