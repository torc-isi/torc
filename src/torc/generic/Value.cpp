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
#include <boost/serialization/variant.hpp>
#endif //GENOM_SERIALIZATION
#include <cmath>

#include "torc/generic/Value.hpp"

namespace torc {
namespace generic {

Value::MiNoMax::MiNoMax() : mMin(), mMinUndefined(true), mNominal(), mMax(), mMaxUndefined(true) {}

Value::MiNoMax::MiNoMax(Value::Number inMin, Value::Number inNominal, Value::Number inMax) :
	mMin(inMin), mMinUndefined(false), mNominal(inNominal), mMax(inMax), mMaxUndefined(false) {}

Value::MiNoMax::~MiNoMax() throw () {}

Value::MiNoMax::MiNoMax(const Value::MiNoMax& inSource) : 	mMin(inSource.mMin),
	mMinUndefined(inSource.mMinUndefined), mNominal(inSource.mNominal), mMax(inSource.mMax),
	mMaxUndefined(inSource.mMaxUndefined) {}

Value::MiNoMax& Value::MiNoMax::operator=(const Value::MiNoMax& inSource) {
	if(this != &inSource) {
		mMin = inSource.mMin;
		mMinUndefined = inSource.mMinUndefined;
		mNominal = inSource.mNominal;
		mMax = inSource.mMax;
		mMaxUndefined = inSource.mMaxUndefined;
	}
	return *this;
}

bool Value::MiNoMax::operator <(const Value::MiNoMax& inRhs) const {
	return mNominal.eval() < inRhs.mNominal.eval();
}

bool Value::MiNoMax::operator ==(const Value::MiNoMax& inRhs) const {
	return mNominal.eval() == inRhs.mNominal.eval();
}

bool Value::MiNoMax::operator >(const Value::MiNoMax& inRhs) const {
	return mNominal.eval() > inRhs.mNominal.eval();
}

bool Value::MiNoMax::operator >=(const Value::MiNoMax& inRhs) const {
	return mNominal.eval() >= inRhs.mNominal.eval();
}

bool Value::MiNoMax::operator <=(const Value::MiNoMax& inRhs) const {
	return mNominal.eval() <= inRhs.mNominal.eval();
}

bool Value::MiNoMax::operator !=(const Value::MiNoMax& inRhs) const {
	return !operator ==(inRhs);
}

void Value::MiNoMax::setMax(const Value::Number& inSource) {
	mMax = inSource;
	mMaxUndefined = false;
}

void Value::MiNoMax::setMin(const Value::Number& inSource) {
	mMin = inSource;
	mMinUndefined = false;
}

void Value::MiNoMax::setNominal(const Value::Number& inSource) {
	mNominal = inSource;
}

Value::Number::Number() : mMantissa(0), mExponent(0) {}

Value::Number::Number(int32_t inMantissa, int32_t inExponent) : mMantissa(inMantissa),
	mExponent(inExponent) {}

Value::Number::~Number() throw () {}

Value::Number::Number(const Value::Number& inSource) : mMantissa(inSource.mMantissa),
	mExponent(inSource.mExponent) {}

Value::Number& Value::Number::operator=(const Value::Number& inSource) {
	if(this != &inSource) {
		mMantissa = inSource.mMantissa;
		mExponent = inSource.mExponent;
	}
	return *this;
}

bool Value::Number::operator <(const Value::Number& inRhs) const {
	return eval() < inRhs.eval();
}

bool Value::Number::operator ==(const Value::Number& inRhs) const {
	return eval() == inRhs.eval();
}

bool Value::Number::operator >(const Value::Number& inRhs) const {
	return eval() > inRhs.eval();
}

bool Value::Number::operator >=(const Value::Number& inRhs) const {
	return eval() >= inRhs.eval();
}

bool Value::Number::operator <=(const Value::Number& inRhs) const {
	return eval() <= inRhs.eval();
}

bool Value::Number::operator !=(const Value::Number& inRhs) const {
	return !operator ==(inRhs);
}

void Value::Number::setMantissa(const int32_t& inSource) {
	mMantissa = inSource;
}

void Value::Number::setExponent(const int32_t& inSource) {
	mExponent = inSource;
}

double Value::Number::eval() const {
	return static_cast<double>(mMantissa) * pow(10, static_cast<double>(mExponent));
}

Value::Point::Point() : mX(0), mY(0) {}

Value::Point::Point(int32_t inX, int32_t inY) : mX(inX), mY(inY) {}

Value::Point::~Point() throw () {}

Value::Point::Point(const Value::Point& inSource) : mX(inSource.mX), mY(inSource.mY) {}

Value::Point& Value::Point::operator=(const Value::Point& inSource) {
	if(this != &inSource) {
		mX = inSource.mX;
		mY = inSource.mY;
	}
	return *this;
}

bool Value::Point::operator <(const Value::Point& inRhs) const {
	return mX < inRhs.mY || (mX == inRhs.mX && mY < inRhs.mY);
}

bool Value::Point::operator ==(const Value::Point& inRhs) const {
	return mX == inRhs.mY && mY == inRhs.mY;
}

bool Value::Point::operator >(const Value::Point& inRhs) const {
	return mX > inRhs.mY || (mX == inRhs.mX && mY > inRhs.mY);
}

bool Value::Point::operator >=(const Value::Point& inRhs) const {
	return mX >= inRhs.mY && mY >= inRhs.mY;
}

bool Value::Point::operator <=(const Value::Point& inRhs) const {
	return mX <= inRhs.mY && mY <= inRhs.mY;
}

bool Value::Point::operator !=(const Value::Point& inRhs) const {
	return !operator ==(inRhs);
}

void Value::Point::setX(const int32_t& inSource) {
	mX = inSource;
}

void Value::Point::setY(const int32_t& inSource) {
	mX = inSource;
}

Value::Value() : mType(eValueTypeUndefined), mValue(), mIsSet(false) {}

Value::Value(Value::Type type) : mType(type), mValue(), mIsSet(false) {}

Value::~Value() throw () {}

Value::Value(const Value& inSource) : mType(inSource.mType), mValue(inSource.mValue),
	mIsSet(inSource.mIsSet) {}

Value& Value::operator=(const Value& inSource) {
	if(this != &inSource) {
		mType = inSource.mType;
		mValue = inSource.mValue;
		mIsSet = inSource.mIsSet;
	}
	return *this;
}

/**
 * Get the type of inSource stored in this object.
 *
 * @return The type of object
 */
void Value::setType(const Value::Type& inSource) {
	mType = inSource;
}

/**
 false* Set whether this object is defined with a inSource, or is undefined.
 *
 * @param[in] inSource True if initialized.
 */
void Value::setIsSet(const bool& inSource) {
	mIsSet = inSource;
}

#ifdef GENOM_SERIALIZATION
template <class Archive> void Value::serialize(Archive& ar, unsigned int) {
	ar & mType;
	ar & mValue;
	ar & mIsSet;
}

//TO SATISFY THE LINKER
template void Value::serialize<boost::archive::binary_iarchive>(
	boost::archive::binary_iarchive& ar, const unsigned int);

template void Value::serialize<boost::archive::binary_oarchive>(
	boost::archive::binary_oarchive& ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace generic
} // namespace torc
