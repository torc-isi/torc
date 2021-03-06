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

#ifndef TORC_GENERIC_SINGLEINSTANCE_HPP
#define TORC_GENERIC_SINGLEINSTANCE_HPP

#include "torc/generic/PointerTypes.hpp"

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/Instance.hpp"
#include "torc/generic/Scalar.hpp"
#include "torc/generic/FactoryType.hpp"
#include "torc/generic/VisitorType.hpp"
#include "torc/generic/Error.hpp"

namespace torc { namespace generic { class BaseVisitor; } }

namespace torc {
namespace generic {

/**
 * @brief Represents a single instance of the view of a cell.
 *
 * The SingleInstance class represents a usage instance of a cell in the design specification
 * (View) of another cell. A single instance can be created using the (instance name ...) syntax.
 */
class SingleInstance : public Instance, public Scalar<Instance> {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION
	friend class FactoryType<SingleInstance> ;

public:
	typedef VisitorType<SingleInstance> Visitor;

	/**
	 * Convenience class to create a single instance.
	 */
	class Factory : public FactoryType<SingleInstance> {
	public:
		using FactoryType<SingleInstance>::create;
		/**
		 * Create a single instance.
		 * @param[in] inName Name of the single instance to be created.
		 * @param[in] inViewPtr Pointer to parented(View) object.
		 * @param[in] inMasterPtr Pointer to master(View) object.
		 * @param[in] inOriginalName Original name of the single instance [optional].
		 *
		 * @return Pointer to created single instance.
		 */
		virtual SingleInstanceSharedPtr newSingleInstancePtr(const std::string& inName,
			const ViewSharedPtr& inViewPtr, const ViewSharedPtr& inMasterPtr,
			const std::string& inOriginalName = std::string()) throw (Error);
	};

	virtual void accept(BaseVisitor& inoutVisitor) throw (Error);

	virtual ~SingleInstance() throw ();

protected:
	SingleInstance();

private:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_SINGLEINSTANCE_HPP
