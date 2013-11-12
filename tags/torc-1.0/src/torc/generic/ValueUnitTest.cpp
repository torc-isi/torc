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
/// \brief Unit test for the Value class.

#include <boost/test/unit_test.hpp>
#include "torc/generic/Value.hpp"

namespace torc {
namespace generic {

BOOST_AUTO_TEST_SUITE(generic)

/// \brief Unit test for the Value class.
BOOST_AUTO_TEST_CASE(ValueUnitTest) {
	// functions not tested:
	//		Value::MiNoMax::MiNoMax();
	//		Value::MiNoMax::MiNoMax(Value::Number inMin, Value::Number inNominal, Value::Number inMax);
	//		Value::MiNoMax::~MiNoMax();
	//		Value::MiNoMax::MiNoMax(const Value::MiNoMax& inSource);
	//		Value::MiNoMax& Value::MiNoMax::operator=(const Value::MiNoMax& inSource);
	//		bool Value::MiNoMax::operator <(const Value::MiNoMax& inRhs) const;
	//		bool Value::MiNoMax::operator ==(const Value::MiNoMax& inRhs) const;
	//		bool Value::MiNoMax::operator >(const Value::MiNoMax& inRhs) const;
	//		bool Value::MiNoMax::operator >=(const Value::MiNoMax& inRhs) const;
	//		bool Value::MiNoMax::operator <=(const Value::MiNoMax& inRhs) const;
	//		bool Value::MiNoMax::operator !=(const Value::MiNoMax& inRhs) const;
	//		void Value::MiNoMax::setMax(const Value::Number& inSource);
	//		void Value::MiNoMax::setMin(const Value::Number& inSource);
	//		void Value::MiNoMax::setNominal(const Value::Number& inSource);
	//		Value::Number::Number();
	//		Value::Number::Number(int32_t inMantissa, int32_t inExponent);
	//		Value::Number::~Number();
	//		Value::Number::Number(const Value::Number& inSource);
	//		Value::Number& Value::Number::operator=(const Value::Number& inSource);
	//		bool Value::Number::operator <(const Value::Number& inRhs) const;
	//		bool Value::Number::operator ==(const Value::Number& inRhs) const;
	//		bool Value::Number::operator >(const Value::Number& inRhs) const;
	//		bool Value::Number::operator >=(const Value::Number& inRhs) const;
	//		bool Value::Number::operator <=(const Value::Number& inRhs) const;
	//		bool Value::Number::operator !=(const Value::Number& inRhs) const;
	//		void Value::Number::setMantissa(const int32_t& inSource);
	//		void Value::Number::setExponent(const int32_t& inSource);
	//		Value::Point::Point();
	//		Value::Point::Point(int32_t inX, int32_t inY);
	//		Value::Point::~Point();
	//		Value::Point::Point(const Value::Point& inSource);
	//		Value::Point& Value::Point::operator=(const Value::Point& inSource);
	//		bool Value::Point::operator <(const Value::Point& inRhs) const;
	//		bool Value::Point::operator ==(const Value::Point& inRhs) const;
	//		bool Value::Point::operator >(const Value::Point& inRhs) const;
	//		bool Value::Point::operator >=(const Value::Point& inRhs) const;
	//		bool Value::Point::operator <=(const Value::Point& inRhs) const;
	//		bool Value::Point::operator !=(const Value::Point& inRhs) const;
	//		void Value::Point::setX(const int32_t& inSource);
	//		void Value::Point::setY(const int32_t& inSource);
	//		Value::Value();
	//		Value::Value(Value::Type type);
	//		Value::~Value();
	//		Value::Value(const Value& inSource);
	//		Value& Value::operator=(const Value& inSource);
	//		void Value::setType(const Value::Type& inSource);
	//		void Value::setIsSet(const bool& inSource);
	//		template <class Archive> void Value::serialize(Archive& ar, unsigned int);
	//		template void Value::serialize<boost::archive::binary_iarchive>(
	//			boost::archive::binary_iarchive& ar, const unsigned int);
	//		template void Value::serialize<boost::archive::binary_oarchive>(
	//			boost::archive::binary_oarchive& ar, const unsigned int);

	// functions tested:
	//		Value::Number::Number(int32_t inMantissa, int32_t inExponent);
	//		double Value::Number::eval() const;
	Value::Number number(2, 2);
	BOOST_CHECK_EQUAL(number.eval(), 200); // for SourceForge bug 3587203.
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace generic
} // namespace torc
