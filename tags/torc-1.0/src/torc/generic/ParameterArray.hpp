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

#ifndef TORC_GENERIC_PARAMETERARRAY_HPP
#define TORC_GENERIC_PARAMETERARRAY_HPP

#include "torc/generic/PointerTypes.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/Error.hpp"
#include "torc/generic/FactoryType.hpp"
#include "torc/generic/Parameter.hpp"
#include "torc/generic/ParameterArrayElement.hpp"
#include "torc/generic/Vector.hpp"
#include "torc/generic/VisitorType.hpp"

namespace torc { namespace generic { class BaseVisitor; } }

namespace torc {
namespace generic {

/**
 * @brief Represents a parameter array.
 *
 * The ParameterArray class represents an array of parameters.
 */
class ParameterArray : public Parameter, public Vector<Parameter, ParameterArrayElement,
	ParameterArrayElement::Factory, false> {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION
	friend class FactoryType<ParameterArray> ;

public:
	typedef Vector<Parameter, ParameterArrayElement, ParameterArrayElement::Factory, false>
		BaseVectorType;
	typedef VisitorType<ParameterArray> Visitor;

	/**
	 * Convenience class to create a parameter array.
	 */
	class Factory : public FactoryType<ParameterArray> {
	public:
		using FactoryType<ParameterArray>::create;
		/**
		 * Create a parameter array.
		 *
		 * @param[in] inName Name of the parameter array to be created.
		 * @param[in] inSize Size of the instance array.
		 * @param[in] inFactory Factory for the child.
		 * @param[in] inOriginalName Original name of the parameter array [optional].
		 *
		 * @return Pointer to created parameter array.
		 */
		virtual ParameterArraySharedPtr newParameterArrayPtr(const std::string& inName,
			const size_t& inSize, const ChildFactorySharedPtr& inFactory
				= BaseVectorType::ChildFactorySharedPtr(new BaseVectorType::ChildFactory()),
			const std::string& inOriginalName = std::string()) throw (Error);

		/**
		 * Create a parameter array.
		 *
		 * @param[in] inName Name of the parameter array to be created.
		 * @param[in] inLimits Dimensions of the vector.
		 * @param[in] inFactory Factory for the child.
		 * @param[in] inOriginalName Original name of the parameter array [optional].
		 *
		 * @return Pointer to created parameter array.
		 */
		virtual ParameterArraySharedPtr newParameterArrayPtr(const std::string& inName,
			const std::vector<size_t>& inLimits, const ChildFactorySharedPtr& inFactory
				= BaseVectorType::ChildFactorySharedPtr(new BaseVectorType::ChildFactory()),
			const std::string& inOriginalName = std::string()) throw (Error);
	};

	virtual ~ParameterArray() throw ();

	/**
	 * Receive an inoutVisitor to this class. The visit method of the inoutVisitor is called and a
	 * reference to this object is passed as a parameter. It has to be noted however, that a
	 * dynamic_cast is performed inside this method. If the cast fails, an appropriate exception is
	 * thrown by this method. This situation can arise when the passed Visitor object does not
	 * inherit from the appropriate inoutVisitor specialization. See Visitor documentation for more
	 * details.
	 *
	 * @param[in,out] inoutVisitor A reference to the inoutVisitor object
	 * @exception Error Visitor type inappropriate for visiting this object or any other error
	 * thrown by the Visitor::throw() method.
	 */
	virtual void accept(BaseVisitor& inoutVisitor) throw (Error);

protected:
	virtual void onChildCreate(const boost::shared_ptr<BaseVectorType::ChildType>& inCreatedChild)
		const throw (Error);

	ParameterArray();

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_PARAMETERARRAY_HPP
