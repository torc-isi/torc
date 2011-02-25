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

/*******************************************************************************
* TORC - Copyright 2010 University of Southern California. All Rights Reserved.
*
* FILE : driver.hpp
*
* DATE : 08-July-2010
*
* DESCRIPTION : Declaration of the torc::generic::Driver class
*
* REVISION HISTORY:
*
* SI        REVISION        AUTHOR               CHANGES          PRs
*[0]    Initial Version    Niladri
*[1]    First   Version    Santanu
*
*******************************************************************************/

#ifndef TORC_EDIF_DRIVER_HPP
#define TORC_EDIF_DRIVER_HPP

#include <string>
#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/parser/ParserPointerTypes.hpp"
#include "torc/generic/util/Error.hpp"

//BOOST
#include <boost/shared_ptr.hpp>

// forward declaration
namespace torc{ namespace generic { class EdifContext; } }
namespace torc{ namespace generic { class Scanner; } }

namespace torc {
namespace generic {

/** The Driver class brings together all components. It creates an instance of
 * the Parser and Scanner classes and connects them. Then the input stream is
 * fed into the scanner object and the parser gets it's token
 * sequence. Furthermore the driver object is available in the grammar rules as
 * a parameter. Therefore the driver class contains a reference to the
 * structure into which the parsed data is saved. */
class Driver {
public:
    /** Invoke the scanner and parser for a stream.
     * @param inStream    input stream
     * @param inStreamName    stream name for error messages
     * @return        true if successfully parsed
     */
    bool
    parseStream(std::istream& inStream,
            const std::string& inStreamName = "stream input");

    /** Invoke the scanner and parser on an input string.
     * @param inString input string
     * @param inStreamName    stream name for error messages
     * @return        true if successfully parsed
     */
    bool
    parseString(const std::string& inString,
            const std::string& inStreamName = "string stream");

    /** Invoke the scanner and parser on a file. Use parseStream with a
     * std::ifstream if detection of file reading errors is required.
     * @param inFileName    input file name
     * @return        true if successfully parsed
     */
    bool
    parseFile(const std::string& inFileName);

    // To demonstrate pure handling of parse errors, instead of
    // simply dumping them on the standard error output, we will pass
    // them to the driver using the following two member functions.

    /** Error handling with associated line number. This can be modified to
     * output the error e.g. to a dialog box. */
    void
    error(const class location& inLocation, const std::string& inMessage);

    /** General error handling. This can be modified to output the error
     * e.g. to a dialog box. */
    void error(const std::string& inMessage);

    inline Scanner *
    getLexer() const throw();

    //Note: Let the address be returned .. this is used by the parser
    inline std::string &
    getStreamName() throw();

    inline EdifContextSharedPtr
    getContext() const throw();

    inline Error
    getParserError() const throw();

    inline bool
    getIsParserErrorSet() const throw();

    void
    setParserError(
        const Error & inSource) throw();

    /// construct a new parser driver context
    Driver(const EdifContextSharedPtr &inEdifCntx);

    ~Driver() throw();

  private:    
    bool mTraceScanning;
    bool mTraceParsing;
    std::string mStreamName;

    /** Pointer to the current lexer instance, this is used to connect the
     * parser to the scanner. It is used in the yylex macro. */
    Scanner* mLexer;

    /** Reference to the Edif context filled during parsing of the
     * Edif file. */
    EdifContextSharedPtr mEdifCntx;

    /** Reference to the Error object    */
    ErrorSharedPtr mErrorObj;
};

inline Scanner *
Driver::getLexer() const throw()
{
    return mLexer;
}

inline std::string &
Driver::getStreamName() throw() {
    return mStreamName;
}

inline EdifContextSharedPtr
Driver::getContext() const throw() {
    return mEdifCntx;
}

inline Error
Driver::getParserError() const throw() {
    return *mErrorObj;
}

inline bool
Driver::getIsParserErrorSet() const throw() {
    return mErrorObj;
}

} // namespace generic
} // namespace torc

#endif // TORC_EDIF_DRIVER_HPP
