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
/// \brief Header for the DDBConsoleStreams class.

#ifndef TORC_ARCHITECTURE_DDBCONSOLESTREAMS_HPP
#define TORC_ARCHITECTURE_DDBCONSOLESTREAMS_HPP

//#include <iostream>

namespace std {
	/// \brief Forward declaration of template basic_istream.
	template <typename _CharT, typename _Traits> class basic_istream;
	/// \brief Forward declaration of template basic_ostream.
	template <typename _CharT, typename _Traits> class basic_ostream;
}

namespace torc {
namespace architecture {

	/// \brief Device database console streams class.
	/// \details This allows the user to specify alternate streams for cin, cout, and cerr, to 
	///		redirect console input to or output from the device database.
	class DDBConsoleStreams {
	public:
	// types
		/// \brief Internal typedef of istream;
		typedef std::basic_istream<char> istream;
		/// \brief Internal typedef of ostream;
		typedef std::basic_ostream<char> ostream;
	// statics
		static DDBConsoleStreams sDefaultConsoleStreams;
		static DDBConsoleStreams sNullConsoleStreams;
	// constructors
		/// \brief Null constructor.
		DDBConsoleStreams(void);
		/// \brief Explicit constructor.
		DDBConsoleStreams(istream& inIn, ostream& inOut, ostream& inErr, ostream& inLog) 
			{ setConsoleStreams(inIn, inOut, inErr, inLog); }
		/// \brief Virtual destructor.
		virtual ~DDBConsoleStreams(void) {}
	// accessors
		/// \brief Returns the database console input stream.
		istream& mIn(void) { return *mInPtr; }
		/// \brief Returns the database console output stream.
		ostream& mOut(void) { return *mOutPtr; }
		/// \brief Returns the database console error stream.
		ostream& mErr(void) { return *mErrPtr; }
		/// \brief Returns the database console log stream.
		ostream& mLog(void) { return *mLogPtr; }
		/// \brief Returns the database console input stream.
		istream& getConsoleIn(void) { return mIn(); }
		/// \brief Returns the database console output stream.
		ostream& getConsoleOut(void) { return mOut(); }
		/// \brief Returns the database console error stream.
		ostream& getConsoleErr(void) { return mErr(); }
		/// \brief Returns the database console log stream.
		ostream& getConsoleLog(void) { return mLog(); }
		/// \brief Sets the database console input stream.
		void setConsoleIn(istream& inStream) { mInPtr = &inStream; }
		/// \brief Sets the database console output stream.
		void setConsoleOut(ostream& inStream) { mOutPtr = &inStream; }
		/// \brief Sets the database console error stream.
		void setConsoleErr(ostream& inStream) { mErrPtr = &inStream; }
		/// \brief Sets the database console log stream.
		void setConsoleLog(ostream& inStream) { mLogPtr = &inStream; }
		/// \brief Sets all of the database console streams.
		/// \param inIn The console input stream.
		/// \param inOut The console output stream.
		/// \param inErr The console error stream.
		/// \param inLog The console log stream.
		virtual void setConsoleStreams(istream& inIn, ostream& inOut, ostream& inErr, 
			ostream& inLog) { 
			setConsoleIn(inIn);
			setConsoleOut(inOut);
			setConsoleErr(inErr); 
			setConsoleLog(inLog);
		}
	protected:
		/// \brief Pointer to the database console input stream.
		istream* mInPtr;
		ostream* mOutPtr;
		ostream* mErrPtr;
		ostream* mLogPtr;
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_DDBCONSOLESTREAMS_HPP
