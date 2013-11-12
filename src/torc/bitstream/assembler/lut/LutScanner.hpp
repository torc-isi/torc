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
/// \brief Header for the LutScanner class.

#ifndef TORC_BITSTREAM_ASSEMBLER_LUT_LUTSCANNER_HPP
#define TORC_BITSTREAM_ASSEMBLER_LUT_LUTSCANNER_HPP

// Flex expects the signature of yylex to be defined in the macro YY_DECL, and
// the C++ parser expects it to be declared. We can factor both as follows.

#ifndef YY_DECL

#define YY_DECL                                         \
    torc::LutParser::token_type                         \
    torc::LutScanner::lex(                              \
        torc::LutParser::semantic_type* yylval,         \
        torc::LutParser::location_type* yylloc          \
    )
#endif

#ifndef __FLEX_LEXER_H
#define yyFlexLexer LutFlexLexer
#include "torc/bitstream/assembler/lut/FlexLexer.h"
#undef yyFlexLexer
#endif

#include "torc/bitstream/assembler/lut/LutParser.hpp"

namespace torc {

	class LutScanner : public LutFlexLexer {
	public:
	// constructors
		/// \brief Public constructor.
		/// \details Scans the input stream for tokens, and sends any output to the output stream.
		/// \param arg_yyin The input stream.
		/// \param arg_yyout The output stream.
		LutScanner(std::istream* arg_yyin = 0, std::ostream* arg_yyout = 0);
		/// \brief Virtual destructor.
		virtual ~LutScanner();
	// functions
		/// \brief Main scanning function.
		virtual LutParser::token_type lex(LutParser::semantic_type* yylval,
			LutParser::location_type* yylloc);
		/// \brief Enables or disables debugging output.
		void set_debug(bool b);

	protected:
	// friends
		/// \brief The Lut parser has full access to our internals.
		friend class LutParser;
	// members
		/// \brief Flag to track whether we are inside a double quote.
		bool in_double_quote;
		/// \brief Flag to count the number of separator colons seen.
		int colon;
		/// \biref Last double-quoted string that we saw.
		std::string last_string;
	};

} // namespace torc

#endif // TORC_BITSTREAM_ASSEMBLER_LUT_LUTSCANNER_HPP
