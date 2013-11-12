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

#ifndef TORC_GENERIC_VISITABLE_HPP
#define TORC_GENERIC_VISITABLE_HPP

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/is_abstract.hpp>
#endif //GENOM_SERIALIZATION
#include "torc/generic/Error.hpp"

namespace torc { namespace generic { class BaseVisitor; } }

namespace torc {
namespace generic {

/**
 * @brief An object that receives an inoutVisitor
 * 
 * The Visitable class provides an interface to all classes that want to make themselves visitable
 * by the clients. Typically such classes will be leaf types that cannot be directly accessed by
 * clients without using a dynamic_cast. This design is loosely based on the  acyclic inoutVisitor
 * concept defined by Alexandrescu in "Modern C++ Design".
 */
class Visitable {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif
protected:
	Visitable();

public:
	virtual ~Visitable() throw ();

private:
	Visitable(const Visitable& source);

	Visitable& operator=(const Visitable& source);

public:
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
	virtual void accept(BaseVisitor& inoutVisitor) throw (Error) = 0;

public:
#ifdef GENOM_SERIALIZATION
	template <class Archive> void serialize(Archive& ar, unsigned int);
#endif //GENOM_SERIALIZATION
};

} // namespace generic
} // namespace torc

#ifdef GENOM_SERIALIZATION
BOOST_IS_ABSTRACT(torc::generic::Visitable)
#endif // GENOM_SERIALIZATION
#endif // TORC_GENERIC_VISITABLE_HPP
