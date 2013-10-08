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
/// \brief Header for the XdlScanner class.

#ifndef TORC_PHYSICAL_XDL_XDLSCANNER_HPP
#define TORC_PHYSICAL_XDL_XDLSCANNER_HPP

// Flex expects the signature of yylex to be defined in the macro YY_DECL, and
// the C++ parser expects it to be declared. We can factor both as follows.

#ifndef YY_DECL

#define YY_DECL                                         \
    torc::XdlParser::token_type                         \
    torc::XdlScanner::lex(                              \
        torc::XdlParser::semantic_type* yylval,         \
        torc::XdlParser::location_type* yylloc          \
    )
#endif

#ifndef __FLEX_LEXER_H
#define yyFlexLexer XdlFlexLexer
#include "torc/physical/xdl/FlexLexer.h"
#undef yyFlexLexer
#endif

#include "torc/physical/xdl/XdlParser.hpp"

namespace torc {

	class XdlScanner : public XdlFlexLexer {
	public:
	// constructors
		/// \brief Public constructor.
		/// \details Scans the input stream for tokens, and sends any output to the output stream.
		/// \param arg_yyin The input stream.
		/// \param arg_yyout The output stream.
		XdlScanner(std::istream* arg_yyin = 0, std::ostream* arg_yyout = 0);
		/// \brief Virtual destructor.
		virtual ~XdlScanner();
	// functions
		/// \brief Main scanning function.
		virtual XdlParser::token_type lex(XdlParser::semantic_type* yylval,
			XdlParser::location_type* yylloc);
		/// \brief Enables or disables debugging output.
		void set_debug(bool b);

	protected:
	// friends
		/// \brief The XDL parser has full access to our internals.
		friend class XdlParser;
	// members
		/// \brief Flag to track whether we are inside a double quote.
		bool in_double_quote;
		/// \brief Flag to count the number of separator colons seen.
		int colon;
		/// \biref Last double-quoted string that we saw.
		std::string last_string;
	};

} // namespace torc

#endif // TORC_PHYSICAL_XDL_XDLSCANNER_HPP
