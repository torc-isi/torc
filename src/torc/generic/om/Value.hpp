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

#ifndef TORC_GENERIC_VALUE_HPP
#define TORC_GENERIC_VALUE_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#include <boost/variant.hpp>

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/util/Error.hpp"

namespace torc {

namespace generic {

/**
 * Represents a Value in EDIF. Values are used in different regions in EDIF such as in properties, parametrs etc.
 */
class Value
{
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif
  public:
    /**
     * @enum Type
     *
     * Defines the type of inSource stored in this Value object.
     */
    enum Type
    {
      eValueTypeUndefined,
      eValueTypeBoolean,
      eValueTypeInteger,
      eValueTypeMiNoMax,
      eValueTypeNumber,
      eValueTypePoint,
      eValueTypeString

    };
    /**
     * Boolean Type
     */
    typedef bool Boolean;

    /**
     * 32 bit Signed Integer type.
     */
    typedef int32_t Integer;

    /**
     * Defines a non-unicode standard string.
     */
    typedef std::string String;

    /**
     * Represents an EDIF MiNoMax inSource. This is basically a ranged integer. As such it stores the minium, nominal and maximum values.
     */
    class MiNoMax
    {
#ifdef GENOM_SERIALIZATION
        friend class boost::serialization::access;
#endif
      private:
        int32_t mMin;
        bool mMinUndefined;
        int32_t mNominal;
        int32_t mMax;
        bool mMaxUndefined;

      public:
        MiNoMax();

        MiNoMax(int32_t inMin,
                int32_t inNominal, int32_t inMax );

        ~MiNoMax() throw();

        MiNoMax(const MiNoMax & inSource) throw();

        MiNoMax &
        operator=(const MiNoMax & inSource) throw();

        bool
        operator <(const MiNoMax & inRhs) const throw();

        bool
        operator ==(const MiNoMax & inRhs) const throw();

        bool
        operator !=(const MiNoMax & inRhs) const throw();

        bool
        operator >(const MiNoMax & inRhs) const throw();

        bool
        operator >=(const MiNoMax & inRhs) const throw();

        bool
        operator <=(const MiNoMax & inRhs) const throw();

        void
        setMax(const int32_t & inSource) throw();

        inline const int32_t
        getMax() const throw();

        inline const bool
        getMinUndefined() const throw();

        inline const bool
        getMaxUndefined() const throw();

        inline const int32_t
        getMin() const throw();

        void
        setMin(const int32_t & inSource) throw();

        inline const int32_t
        getNominal() const throw();

        void
        setNominal(const int32_t & inSource) throw();

#ifdef GENOM_SERIALIZATION
        template<class Archive> void
        serialize( Archive &ar, unsigned int ) {
            ar & mMin;
            ar & mMinUndefined;
            ar & mNominal;
            ar & mMax;
            ar & mMaxUndefined;
        }
#endif //GENOM_SERIALIZATION

    };

    /**
     * Represents a decimal number. This is used to represent decimal floating point numbers in EDIF. This has a exponent and mantissa part.
     */
    class Number
    {
#ifdef GENOM_SERIALIZATION
        friend class boost::serialization::access;
#endif

      private:
        int32_t mMantissa;
        int32_t mExponent;

      public:
        Number();

        Number(int32_t inMantissa, int32_t inExponent);

        ~Number() throw();

        Number(const Number & inSource) throw();

        Number &
        operator=(const Number & inSource) throw();

        bool
        operator <(const Number & inRhs) const throw();

        bool
        operator ==(const Number & inRhs) const throw();

        bool
        operator >(const Number & inRhs) const throw();

        bool
        operator >=(const Number & inRhs) const throw();

        bool
        operator <=(const Number & inRhs) const throw();

        bool
        operator !=(const Number & inRhs) const throw();

        inline const int32_t
        getMantissa() const throw();

        void
        setMantissa(const int32_t & inSource) throw();

        inline const int32_t
        getExponent() const throw();

        void
        setExponent(const int32_t & inSource) throw();

      private:
        double
        eval() const throw();
#ifdef GENOM_SERIALIZATION
        template<class Archive> void
        serialize( Archive &ar, unsigned int ) {
            ar & mExponent;
            ar & mMantissa;
        }
#endif //GENOM_SERIALIZATION
    };

    /**
     * Represents a point in cartesian coordinates. This stores the X and Y coordinates.
     */
    class Point
    {
#ifdef GENOM_SERIALIZATION
        friend class boost::serialization::access;
#endif

      private:
        int32_t mX;
        int32_t mY;

      public:
        Point();

        Point(int32_t inX, int32_t inY);

        ~Point() throw();

        Point(const Point & inSource);

        Point &
        operator=(const Point & inSource) throw();

        bool
        operator <(const Point & inRhs) const throw();

        bool
        operator ==(const Point & inRhs) const throw();

        bool
        operator !=(const Point & inRhs) const throw();

        bool
        operator >(const Point & inRhs) const throw();

        bool
        operator >=(const Point & inRhs) const throw();

        bool
        operator <=(const Point & inRhs) const throw();

        inline const int32_t
        getX() const throw();

        void
        setX(const int32_t & inSource) throw();

        inline const int32_t
        getY() const throw();

        void
        setY(const int32_t & inSource) throw();

      private:    
#ifdef GENOM_SERIALIZATION
        template<class Archive> void
        serialize( Archive &ar, unsigned int ) {
            ar & mX;
            ar & mY;
        }
#endif //GENOM_SERIALIZATION
    };

    Value();

    Value(Type type);

    template<typename _ValueType>
    Value(Type type, const _ValueType & inSource);

    ~Value() throw();

    Value(const Value & inSource);

    Value &
    operator=(const Value & inSource) throw();

    /**
     * Get the inSource stored in this object.
     *
     * @note This needs to be called as: v.get<Value::Integer>();
     *
     * @return The inSource according to the specified type.
     *
     * @exception Error The stored an requested types are incompatible.
     */
    template<typename _ValueType>
    _ValueType
    get() const throw(Error);

    /**
     * Set a inSource object.
     *
     * @note This needs to be called as: v.set<Value::Integer>(10);
     *
     * @param[in] inSource The inSource according to the specified type.
     *
     * @exception Error The requested type is wrong.
     */
    template<typename _ValueType>
    void
    set(const _ValueType & inSource) throw(Error);

    /**
     * Get the type of inSource stored in this object.
     *
     * @return The type of object
     */
    inline const Type
    getType() const throw();

    /**
     * Get the type of inSource stored in this object.
     *
     * @return The type of object
     */
    void
    setType(const Type & inSource) throw();

    /**
     * Get whether this object is defined with a inSource, or is undefined.
     *
     * @return True if initialized.
     */
    inline const bool
    getIsSet() const throw();

    /**
     * Set whether this object is defined with a inSource, or is undefined.
     *
     * @param[in] inSource True if initialized.
     */
    void
    setIsSet(const bool & inSource) throw();


  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    Type mType;
    boost::variant<
        Value::Boolean,
        Value::Integer,
        Value::Number,
        Value::MiNoMax,
        Value::Point,
        Value::String> mValue;
    bool mIsSet;
};

inline const int32_t
Value::MiNoMax::getMax() const throw() {
  return mMax;
}

inline const bool
Value::MiNoMax::getMinUndefined() const throw() {
  return mMinUndefined;
}

inline const bool
Value::MiNoMax::getMaxUndefined() const throw() {
  return mMaxUndefined;
}

inline const int32_t
Value::MiNoMax::getMin() const throw() {
  return mMin;
}

inline const int32_t
Value::MiNoMax::getNominal() const throw() {
  return mNominal;
}

inline const int32_t
Value::Number::getMantissa() const throw() {
  return mMantissa;
}

inline const int32_t
Value::Number::getExponent() const throw() {
  return mExponent;
}

inline const int32_t
Value::Point::getX() const throw() {
  return mX;
}

inline const int32_t
Value::Point::getY() const throw() {
  return mY;
}

/**
 * Get the type of inSource stored in this object.
 *
 * @return The type of object
 */
inline const Value::Type
Value::getType() const throw() {
  return mType;
}

/**
 * Get whether this object is defined with a inSource, or is undefined.
 *
 * @return True if initialized.
 */
inline const bool
Value::getIsSet() const throw() {
  return mIsSet;
}

template<typename _ValueType>
Value::Value(Value::Type type, const _ValueType & inSource)
    :mType( type ),
    mValue( inSource ),
    mIsSet( true ) {
}

/**
 * Set a inSource object.
 *
 * @note This needs to be called as: v.set<Value::Integer>(10);
 *
 * @param[in] inSource The inSource according to the specified type.
 *
 * @exception Error The requested type is wrong.
 */
template<typename _ValueType>
void
Value::set(const _ValueType & inSource) throw(Error) {
    mValue = inSource;
}

/**
 * Get the inSource stored in this object.
 *
 * @note This needs to be called as: v.get<Value::Integer>();
 *
 * @return The inSource according to the specified type.
 *
 * @exception Error The stored an requested types are incompatible.
 */
template<typename _ValueType>
_ValueType
Value::get() const throw(Error) {
    if( !mIsSet )
    {
        Error e( eMessageIdErrorValueNotSet,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Value set", mIsSet );
        throw e;
    }
    _ValueType v;
    try
    {
        v = boost::get<_ValueType>(mValue);
    }
    catch( const boost::bad_get &e )
    {
        Error e( eMessageIdErrorTypeCast,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Value", mValue );
        e.saveContextData("Casted Type",
                    std::string("_ValueType") );
        throw e;
    }
    return v;
}


} // namespace torc::generic

} // namespace torc
#endif
