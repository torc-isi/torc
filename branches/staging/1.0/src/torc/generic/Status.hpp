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

#ifndef TORC_GENERIC_STATUS_HPP
#define TORC_GENERIC_STATUS_HPP

#include <vector>

#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/Commentable.hpp"
#include "torc/generic/Visitable.hpp"
#include "torc/generic/VisitorType.hpp"
#include "torc/generic/UserDataContainer.hpp"
#include "torc/generic/SelfReferencing.hpp"
#include "torc/generic/Error.hpp"
#include "torc/generic/FactoryType.hpp"
#include "torc/generic/StatusContainer.hpp"

namespace torc { namespace generic { class Written; } }
namespace torc { namespace generic { class BaseVisitor; } }

namespace torc {
namespace generic {

/**
 * @brief Represents EDIF status construct
 * 
 * Status is used to convey accounting and problem analysis information for the design.
 */
class Status : public Commentable, public Visitable, public UserDataContainer,
	public SelfReferencing<Status> {

	friend class FactoryType<Status> ;

public:

	/**
	 * Convenience class to visit a status.
	 */
	typedef VisitorType<Status> Visitor;

	/**
	 * Convenience class to create a status.
	 */
	class Factory : public FactoryType<Status> {
	public:
		using FactoryType<Status>::create;
		/**
		 * Create a status.
		 *
		 * @param[in] inContainer Pointer of the status container.
		 *
		 * @return Pointer to created status.
		 */
		StatusSharedPtr virtual newStatusPtr(const StatusContainerSharedPtr& inContainer)
			throw (Error);

	};

	virtual ~Status() throw ();

	/**
	 * Get the vector of written statements.
	 *
	 * @return vector Containing written statements
	 */
	inline void getWrittens(std::vector<WrittenSharedPtr>& outValues) const;

	/**
	 * Set the vector of written statements.
	 *
	 * @param[in] inSource Vector containing written statements
	 */
	void setWrittens(const std::vector<WrittenSharedPtr>& inSource);

	/**
	 * Add a written statement to the vector of written statements.
	 * If an empty pointer is supplied, it returns without doing anything.
	 *
	 * @param[in] inWritten A pointer to a written object.
	 *
	 * @exception Error Written could not be added.
	 */
	void addWritten(WrittenSharedPtr& inWritten) throw (Error);

	/**
	 * Receive a visitor to this class. The visit method of the visitor is called
	 * and a reference to this object is passed as a parameter. It has to be noted however,
	 * that a dynamic_cast is performed inside this method. If the cast fails,
	 * an appropriate exception is thrown by this method. This situation can arise when
	 * the passed Visitor object does not inherit from the appropriate visitor specialization.
	 * See Visitor documentation for more details.
	 *
	 * @param[in,out] visitor A reference to the visitor object
	 * @exception Error Visitor type inappropriate for visiting this object
	 * or any other error thrown by the Visitor::throw() method.
	 */
	virtual void accept(BaseVisitor& visitor) throw (Error);

protected:
	Status();

private:
	std::vector<WrittenSharedPtr> mWrittens;

};

inline void Status::getWrittens(std::vector<WrittenSharedPtr>& outValues) const {
	outValues.insert(outValues.end(), mWrittens.begin(), mWrittens.end());
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_STATUS_HPP
