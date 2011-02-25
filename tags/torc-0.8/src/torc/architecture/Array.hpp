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

/// \file
/// \brief Header for the Array class.

#ifndef TORC_ARCHITECTURE_ARRAY_HPP
#define TORC_ARCHITECTURE_ARRAY_HPP

#include <boost/cstdint.hpp>
#include <boost/type_traits.hpp>
#include <stdexcept>

namespace torc {
namespace architecture {

	/// \brief Encapsulation of a static array.
	/// \details Arrays are intended for fixed-size operation without frills, and are used 
	///		extensively to represent device wiring data.  Although this class could be used more 
	///		broadly, it is likely that most code would be better served by raw arrays or by STL 
	///		containers.
	///		<p>
	///		Arrays can be resized, but their contents are not retained when resized, nor are the 
	///		contents cleared upon initialization.
	template <class T> class Array {
	protected:
	// types
		/// \brief Imported type name.
		typedef boost::uint32_t uint32_t;
		/// \brief A type identical to template parameter T, but with any 'const' trait removed.
		typedef typename boost::remove_const<T>::type T_non_const;
	// members
		/// \brief The internal array.
		T* mArray;
		/// \brief The logical and actual size of the array.
		uint32_t mSize;
	private:
		/// \brief Allocate an array of the specified size.
		void allocate(uint32_t inSize) {
			if(mArray != 0) deallocate();
			if(inSize > 0) {
				// T might be a const type, so we get Boost to remove the const trait
				mArray = new T_non_const[inSize];
				mSize = inSize;
			}
		}
		/// \brief Deallocate the array.
		void deallocate(void) {
			if(mArray != 0) delete[] mArray;
			mArray = 0;
			mSize = 0;
		}
#ifdef DEBUG
		/// \brief Enforce array bounds if so requested.
		#define enforceBounds(index) \
			if(index >= mSize || index < 0) { \
				char scratch[1 << 10]; \
				snprintf(scratch, sizeof(scratch), "Index %d not in valid range [0,%d).", index, \
					mSize); \
				throw std::out_of_range(scratch); \
			}
#else
		/// \brief Enforce array bounds if so requested.
		#define enforceBounds(index) ;
#endif
	public:
	// types
		/// \brief Constant T iterator type.
		typedef const T* const_iterator;
		/// \brief Non-constant T iterator type.
		typedef T* iterator;
	// constructors
		/// \brief Null constructor.
		Array<T>(void) : mArray(0), mSize(0) {}
		/// \brief Public constructor.
		Array<T>(uint32_t inSize) : mArray(0), mSize(0) { allocate(inSize); }
		/// \brief Non-virtual destructor.
		~Array<T>(void) { deallocate(); }
	// iterators
		/// \brief Returns the non-constant begin iterator.
		T* begin(void) { return mArray; }
		/// \brief Returns the non-constant end iterator.
		T* end(void) { return mArray + mSize; }
		/// \brief Returns the constant begin iterator.
		const T* begin(void) const { return mArray; }
		/// \brief Returns the constant end iterator.
		const T* end(void) const { return mArray + mSize; }
	// accessors
		/// \brief Returns the array size.
		inline uint32_t getSize(void) const { return mSize; }
	// functions
		/// \brief Discards all contents and resizes the array.
		inline void setSize(uint32_t inSize) { allocate(inSize); }
	// operators
		/// \brief Non-constant subscript operator.
		T& operator [](uint32_t inIndex) {
			enforceBounds(inIndex);
			return mArray[inIndex];
		}
		/// \brief Constant subscript operator.
		const T& operator [](uint32_t inIndex) const {
			enforceBounds(inIndex);
			return mArray[inIndex];
		}
	};

	/// \brief Encapsulation of a 2D static array.
	/// \details 2D arrays are used to represent parts of the device wiring data.  In general, the 
	///		nested arrays will not be identically sized, so we provide no deep constructor here.
	///		See Array for more details on the superclass.
	template <class T> class Array2D : public Array< Array<T> > {
	protected:
	// types
		/// \brief Typedef for the base class.
		typedef Array< Array<T> > super;
	public:
	// constructors
		/// \brief Null constructor.
		Array2D<T>(void) : super() {}
		/// \brief Public constructor.
		/// \param inSize The size of the outer array.  The inner arrays will be constructed with 
		///		their unsized default constructor.
		Array2D<T>(uint32_t inSize) : super(inSize) {}
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_ARRAY_HPP
