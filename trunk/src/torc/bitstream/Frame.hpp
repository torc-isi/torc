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
/// \brief Header for the Frame class.

#ifndef TORC_BITSTREAM_FRAME_HPP
#define TORC_BITSTREAM_FRAME_HPP

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <vector>

namespace torc {
namespace bitstream {

	/// \brief Bitstream frame.
	template <typename WORD_TYPE> class Frame {
	protected:
	// typedefs
		/// \brief Imported type name.
		typedef boost::uint32_t uint32_t;
	// members
		/// \brief Flag to indicate whether the frame is in use.
		bool mIsUsed;
		/// \brief Flag to indicate whether the frame has been modified.
		bool mIsDirty;
		/// \brief Frame length in words.
		uint32_t mLength;
		/// \brief Array of frame words.
		WORD_TYPE* mWords;
	public:
	// typedefs
		/// \brief Frame word type.
		typedef WORD_TYPE word_t;
	// constructors
		/// \brief Basic constructor.
		Frame(uint32_t inLength, word_t* inWords = 0) : mIsUsed(true), mIsDirty(false), 
			mLength(inLength) {
			if(inWords && inLength) {
				mWords = new word_t[inLength];
				word_t* p = mWords;
				for(uint32_t i = 0; i < inLength; i++) *p++ = *inWords++;
			} else {
				mWords = 0;
			}
		}
		~Frame(void) {
			if(mWords) delete mWords;
			mWords = 0;
		}
	// functions
		/// \brief Touch the frame by marking it dirty.
		/// \details WARNING: This is a temporary convenience function that will not be retained.
		inline void touch(void) { mIsUsed = mIsDirty = true; }
	// accessors
		/// \brief Returns the length of the frame in words.
		uint32_t getLength(void) const { return mLength; }
		/// \brief Returns a const raw word pointer.
		const word_t* getWords(void) const { return mWords; }
		/// \brief Returns a non-const raw word pointer.
		//word_t* getWords(void) { return mWords; }
		/// \brief Returns true if the frame is in use in the bitstream, or false otherwise.
		bool isUsed(void) const { return mIsUsed; }
		/// \brief Returns true if the frame has been modified, or false otherwise.
		bool isDirty(void) const { return mIsDirty; }
		/// \brief Returns the specified frame word, or 0 if out of bounds.
		word_t operator[] (int index) {
			if(mWords == 0 || index < 0 || static_cast<uint32_t>(index) >= mLength) return 0;
			return mWords[index];
		}
		/// \brief Sets the value of the specified frame word.
		void setWord(int index, int inWord) {
			// do nothing if the index is out of bounds
			if(index < 0 || static_cast<uint32_t>(index) >= mLength) return;
			// allocate and zero out the words if they were previously unallocated
			if(mWords == 0) {
				mWords = new word_t[mLength];
				word_t* p = mWords;
				for(uint32_t i = 0; i < mLength; i++) *p++ = 0;
			}
			// set the new word value, and mark it used and dirty
			mWords + index = inWord;
			touch();
		}
		/// \brief Sets the value of the specified frame words.
		void setWords(int inWord) {
			// allocate and zero out the words if they were previously unallocated
			if(mWords == 0) {
				mWords = new word_t[mLength];
				word_t* p = mWords;
				for(uint32_t i = 0; i < mLength; i++) *p++ = 0;
			}
			// set the new word value, and mark it used and dirty
			mWords = inWord;
			touch();
		}
	};

	typedef Frame<uint32_t> VirtexFrame;	///< \brief Virtex frame type.
	typedef Frame<uint32_t> SpartanFrame;	///< \brief Spartan frame type.
	typedef Frame<uint16_t> Spartan6Frame;	///< \brief Spartan6 frame type.

	typedef boost::shared_ptr<VirtexFrame> VirtexFrameSharedPtr;	  ///< \brief Virtex frame type.
	typedef boost::shared_ptr<SpartanFrame> SpartanFrameSharedPtr;  ///< \brief Spartan frame type.
	typedef boost::shared_ptr<Spartan6Frame> Spartan6FrameSharedPtr;///< \brief Spartan6 frame type.

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_FRAME_HPP
