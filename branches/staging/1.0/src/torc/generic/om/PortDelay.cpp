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

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/string.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/PortDelay.hpp"

namespace torc {

namespace generic {

void
PortDelay::setDerivation(
    const Derivation & inSource) throw() {
    mDerivation = inSource;
}

void
PortDelay::setType(Type inSource) throw() {
    mType = inSource;
}

void
PortDelay::setDelay(
    const Value::MiNoMax & inSource) throw() {
    mDelay = inSource;
}

void
PortDelay::setAcLoad(const Value::MiNoMax & inSource) throw() {
    mAcLoad = inSource;
}

void
PortDelay::setTransition( const LogicElementSharedPtr & inSource ) throw() {
    mTransition = inSource;
}

PortDelay::PortDelay()
    :mDerivation( eDerivationUndefined ),
    mType( eTypeDelay ),
    mDelay(),
    mAcLoad(),
    mTransition() {
}

PortDelay::~PortDelay() throw() {
}

PortDelay::PortDelay(const PortDelay & source)
    :mDerivation( source.mDerivation ),
    mType( source.mType ),
    mDelay( source.mDelay ),
    mAcLoad( source.mAcLoad ),
    mTransition( source.mTransition ) {
}

PortDelay &
PortDelay::operator=(const PortDelay & source) throw() {
    if( this != &source )
    {
        mDerivation = source.mDerivation;
        mType = source.mType;
        mDelay = source.mDelay;
        mAcLoad = source.mAcLoad;
        mTransition = source.mTransition;
    }
    return *this;
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
PortDelay::serialize( Archive &ar, unsigned int ) {
    ar & mDerivation;
    ar & mType;
    ar & mDelay;
    ar & mAcLoad;
    ar & mTransition;
}

//TO SATISFY THE LINKER
template void
PortDelay::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
PortDelay::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION


} // namespace torc::generic

} // namespace torc
