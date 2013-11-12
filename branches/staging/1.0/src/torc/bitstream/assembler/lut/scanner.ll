/*
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
*/


%{


// ------------------------------------------------------------------------------------------------
// -------------------------------------- C/C++ declarations --------------------------------------
// ------------------------------------------------------------------------------------------------


// disable GCC warnings that Bison gives us little control over
//#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
//#pragma GCC diagnostic ignored "-Werror"
//#pragma GCC diagnostic ignored "-Wall"

#include <string>
#include "torc/bitstream/assembler/lut/LutScanner.hpp"

namespace torc { namespace bitstream {} }
using namespace torc::bitstream;

/// \brief Import the parser's token types into a local typedef
typedef torc::LutParser::token token;
typedef torc::LutParser::token_type token_type;

/// \details By default, yylex() returns int, but we want token_type instead.  Unfortunately, 
///		yyterminate returns 0 by default, which is not of token_type.
#define yyterminate() return token::END

/// \brief Disable the inclusion of unistd.h, which is not available under Visual C++ on Win32.
/// \details The C++ scanner uses STL streams instead.
#define YY_NO_UNISTD_H

//#define DEBUG(a) printf a
#define DEBUG(a)
#define DEBUG1(a) printf a


%}


/* ---------------------------------------------------------------------------------------------- */
/* -------------------------------- Flex declarations and options ------------------------------- */
/* ---------------------------------------------------------------------------------------------- */


/** \brief Enable C++ generation for the scanner class. **/
%option c++
%option noyywrap

/** \brief Change the name of the scanner class to "LutFlexLexer". **/
%option prefix="Lut"

/** \brief The manual says batch is "somewhat more optimized". **/
%option batch

/** \brief Enable debug output from the scanner. **/
/*%option debug*/

/** \brief We anticipate no included LUT files. **/
%option yywrap nounput 

/** \brief Track location information.  Each time yylex is invoked, begin is moved to end. **/
%{
#define YY_USER_ACTION  yylloc->columns(yyleng);
%}

%%


 /* ---------------------------------------------------------------------------------------------- */
 /* --------------------------------- scanner regular expressions -------------------------------- */
 /* ---------------------------------------------------------------------------------------------- */


 /** \brief Code belonging at the beginning of yylex(). **/
%{
    yylloc->step();
%}


 /* ---------------------------------------------------------------------------------------------- */
 /* ------------------------------------ Begin XDL lexer rules ----------------------------------- */
 /* ---------------------------------------------------------------------------------------------- */


[a-zA-Z][0-9]			{ *yylval = yytext; return token::VAR; }
"="						{  }
"("						{ return token::LP; }
")"						{ return token::RP; }
"~"						{ *yylval = "NOT"; return token::NOT; }
[\+]					{ *yylval = "OR"; return token::OR;}
[\*]					{ *yylval = "AND"; return token::AND;}
[\@]					{ *yylval = "XOR"; return token::XOR;}
[ \t\n]					{  }
[0-1]+					{ *yylval = yytext; return token::VAR; }
.						{ std::cout << "Flex Unrecognised token!" << std::endl; exit (-1); }


 /* ---------------------------------------------------------------------------------------------- */
 /* ------------------------------------- End XDL lexer rules ------------------------------------ */
 /* ---------------------------------------------------------------------------------------------- */


%%


// ------------------------------------------------------------------------------------------------
// ---------------------------------------- Additional code ---------------------------------------
// ------------------------------------------------------------------------------------------------


namespace torc {

	LutScanner::LutScanner(std::istream* in, std::ostream* out) : LutFlexLexer(in, out) {}

	LutScanner::~LutScanner() {}

	void LutScanner::set_debug(bool b) {
		yy_flex_debug = b;
	}

} // namespace torc


/// \details This implementation of LutFlexLexer::yylex() is necessary to fill the vtable of class 
///		LutFlexLexer.  We define the scanner's main yylex() function via YY_DECL to reside in class 
///		LutScanner instead.
#ifdef yylex
#undef yylex
#endif

int LutFlexLexer::yylex() {
    std::cerr << "in LutFlexLexer::yylex() !" << std::endl;
    return 0;
}

/// \details When the scanner receives an End-Of-File result from YY_INPUT, it checks yywrap() for 
///		permission to proceed.  If yywrap() returns false (zero), we assume that the function has 
///		set up yyin to point to the next input file, and the scanning continues.  If yywrap() 
///		instead returns true (non-zero), the scanner terminates and returns 0 to the caller.
int LutFlexLexer::yywrap() {
    return 1;
}

//namespace {
//	/// \brief Suppress GCC warnings about unused boost global variables.
//	void suppress_boost_gcc_warnings(void);
//	void suppress_boost_gcc_warnings(void) {
//		(void) boost::system::system_category;
//		(void) boost::system::generic_category;
//		(void) boost::system::posix_category;
//		(void) boost::system::errno_ecat;
//		(void) boost::system::native_ecat;
//	}
//}
