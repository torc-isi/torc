// TORC - Copyright 2011 University of Southern California.  All Rights Reserved.
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
/// \brief Header for the CodingStyle class.

#ifndef TORC_CODINGSTYLE_HPP
#define TORC_CODINGSTYLE_HPP

#include <stdint.h>
#include <string>
#include <vector>
#include <map>

// namespaces are lowercased
namespace torc {

/// \brief Coding style synopsys.
/// \details Extended dummy description for coding style class.  Lines should be wrapped at 100 
///		characters.  4 character tabs are preferred, but spaces are permissible in torc::generic.
class CodingStyle {

	// internal convenience typedefs are just my way of avoiding lots of namespace usage 
	// in the class definition.
	typedef std::string string;
	typedef std::vector<uint64_t> Uint64Vector;
	typedef std::map<uint64_t, string> Uint64ToStringMap;

public:
	/// \brief Pip direction enumeration.
	/// \details Setting the enumerations to constants is appropriate when we need to respect 
	///		external constants.  Another common trick is to declare a final ...Count entry, for use 
	///		as an automatic count, even if entries are added or removed.  Refer to the cpp file for 
	///		an example of how this is used.
	enum EPipDirection {
		ePipBidirectionalUnbuffered = 0,
		ePipBidirectionalUnidirectionallyBuffered,
		ePipBidirectionalBidirectionallyBuffered,
		ePipUnidirectionalBuffered,
		ePipDirectionCount
	};

	/// \brief Construct from tile and wire names and directionality.
	CodingStyle(const string& inMember, EPipDirection inPipDirection) : mMember(inMember), 
		mPipDirection(inPipDirection), cConstant("constant"), mVoidPtr(0), mEdifFlag(false), 
		mState(eStateNull),	mPrivate(false), private_(0) {}

	/// \brief Returns the member, with get___() semantics.
	const string& getMember(void) const { return mMember; }
	/// \brief Sets the member, with set___() semantics.
	void setMember(const string& inMember) { mMember = inMember; }
	/// \brief Returns the pip direction, with get___() semantics.
	EPipDirection getPipDirection(void) const { return mPipDirection; }
	/// \brief Sets the pip direction, with set___() semantics.
	void setPipDirection(EPipDirection inPipDirection) { mPipDirection = inPipDirection; }
	/// \brief Test functions are typically prefixed with has or sometimes is or alternatives.
	bool hasVoidPtr(void) const { 
		// C++ uses 0 rather than NULL for uninitialized pointers
		return mVoidPtr != 0; 
	}
	/// \brief Returns the pip direction as a string.
	const char* getPipDirectionString(void) const;
	/// \brief Sample function.
	void sampleFunction(void);

	/// \brief Example of a function accepting and returning multiple parameters
	bool complexFunction(const string& inName, Uint64Vector& inIndexVector, 
		Uint64ToStringMap& inoutIndexToNameMap);

protected:
	/// \details In many cases there is no need to tie the enumerations to specific values.
	enum EState { eStateNull, eStateOne, eStateTwo, eStateThree, eStateSpecial, eStateFour };

	/// \brief Member variables are prefixed with m.
	string mMember;
	/// \brief Static variables are prefixed with s.
	static string sStatic;
	/// \brief Declared constants are prefixed with c.
	const string cConstant;
	/// \brief Pointers may be explicitly suffixed with Ptr.
	void* mVoidPtr;
	/// \brief Only the first letter of acronyms is capitalized when used within a name.
	bool mEdifFlag;
	/// \brief Another member variable.
	EPipDirection mPipDirection;
	/// \brief Yet another member variable
	EState mState;

	/// \brief String representation of pip directions.
	static const char* sPipDirectionStrings[];

private:
	/// \brief Private variables normally follow the naming convention described above.
	bool mPrivate;
	/// \brief Private variables may alternatively suffixed with _.
	uint32_t private_;
};

} // namespace torc

#endif // TORC_CODINGSTYLE_HPP
