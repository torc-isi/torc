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
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/SingleParameter.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT( torc::generic::SingleParameter )
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

/**
 * Create a single parameter.
 * @param[in] inName Name of the single parameter to be created.
 * @param[in] inValue Value of the parameter.
 * @param[in] inUnit Unit of the parameter.
 * @return Pointer to created single parameter.
 */
SingleParameterSharedPtr
SingleParameter::Factory::newSingleParameterPtr( const std::string &inName,
    const Value &inValue,
    const Unit  &inUnit,
    const std::string &inOriginalName ) throw(Error) {
    try
    {
        SingleParameterSharedPtr newSingleParameter;
        create( newSingleParameter );
        newSingleParameter->setName(inName);
        newSingleParameter->setValue( inValue );
        newSingleParameter->setUnit( inUnit );
        newSingleParameter->setOriginalName( inOriginalName );
        return newSingleParameter;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
SingleParameter::accept(
    BaseVisitor & inoutVisitor) throw(Error) {
    try
    {
        runVisitor( *this, inoutVisitor );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

SingleParameter::SingleParameter()
    :Parameter(),
    Scalar<Parameter>() {
}

SingleParameter::~SingleParameter() throw() {
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
SingleParameter::serialize( Archive &ar, unsigned int) {
	ar & boost::serialization::base_object< Parameter >( *this );
	ar & boost::serialization::base_object<
								Scalar<Parameter> >( *this );
}

//TO SATISFY THE LINKER
template void
SingleParameter::serialize<boost::archive::binary_oarchive>(
    boost::archive::binary_oarchive & ar, const unsigned int);

template void
SingleParameter::serialize<boost::archive::binary_iarchive>(
    boost::archive::binary_iarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION


} // namespace torc::generic

} // namespace torc
