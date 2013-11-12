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

#ifndef TORC_GENERIC_INSTANCEARRAYMEMBER_HPP
#define TORC_GENERIC_INSTANCEARRAYMEMBER_HPP

#include "torc/generic/PointerTypes.hpp"

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/Error.hpp"
#include "torc/generic/FactoryType.hpp"
#include "torc/generic/Instance.hpp"
#include "torc/generic/VectorBit.hpp"
#include "torc/generic/VisitorType.hpp"

namespace torc { namespace generic { class BaseVisitor; } }
namespace torc { namespace generic { class ParameterMap; } }

namespace torc {
namespace generic {

/**
 * @brief Represents a member of an instance array.
 *
 * The InstanceArrayMember represents a single instance from an array of instances.
 */
class InstanceArrayMember : public Instance, public VectorBit<Instance> {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION
public:
	typedef VisitorType<InstanceArrayMember> Visitor;
	/**
	 * Convenience typedef to create a parameter array element.
	 */
	typedef FactoryType<InstanceArrayMember> Factory;

	/**
	 * Receive an inoutVisitor to this class. The visit method of the inoutVisitor is called and a
	 * reference to this object is passed as a parameter. It has to be noted however, that a
	 * dynamic_cast is performed inside this method. If the cast fails, an appropriate exception
	 * is thrown by this method. This situation can arise when the passed Visitor object does not
	 * inherit from the appropriate inoutVisitor specialization. See Visitor documentation for more
	 * details.
	 *
	 * @param[in,out] inoutVisitor A reference to the inoutVisitor object
	 * @exception Error Visitor type inappropriate for visiting this object or any other error
	 * thrown by the Visitor::throw() method.
	 */
	virtual void accept(BaseVisitor& inoutVisitor) throw (Error);

	/**
	 * Returns the parent container's context that will be used to get parameters from
	 * the parameter map. For an unbound instance,
	 * this will return an invalid value
	 */
	virtual ParameterContext getParameterContext() const;

	/**
	 * Get the ParameterMap corresponding to this instance and it's master
	 *
	 * @return A ParameterMap consisiting of only the overridden parameters.
	 */
	virtual ParameterMapSharedPtr getParameters() const throw (Error);

	InstanceArrayMember();
	virtual ~InstanceArrayMember() throw ();

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_INSTANCEARRAYMEMBER_HPP
