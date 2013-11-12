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
#include "torc/physical/xdl/XdlScanner.hpp"

namespace torc { namespace physical {} }
using namespace torc::physical;

/// \brief Import the parser's token types into a local typedef
typedef torc::XdlParser::token token;
typedef torc::XdlParser::token_type token_type;

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

/** \brief Change the name of the scanner class to "XdlFlexLexer". **/
%option prefix="Xdl"

/** \brief The manual says batch is "somewhat more optimized". **/
%option batch

/** \brief Enable debug output from the scanner. **/
/*%option debug*/

/** \brief We anticipate no included XDL files. **/
%option yywrap nounput 

/** \brief Enable the use of start condition stacks. **/
%option stack

/** \brief Track location information.  Each time yylex is invoked, begin is moved to end. **/
%{
#define YY_USER_ACTION  yylloc->columns(yyleng);
%}

/** \brief Special lexing state for config strings. **/
%x CONFIGSTRING
/** \brief Special second lexing state for config strings. **/
%x CONFIGSTRING2
/** \brief Special lexing state for double-quoted strings. **/
%x DOUBLEQUOTE


%%


 /* --------------------------------------------------------------------------------------------- */
 /* --------------------------------- scanner regular expressions ------------------------------- */
 /* --------------------------------------------------------------------------------------------- */


 /** \brief Code belonging at the beginning of yylex(). **/
%{
    yylloc->step();
%}


 /* --------------------------------------------------------------------------------------------- */
 /* ------------------------------------ Begin XDL lexer rules ---------------------------------- */
 /* --------------------------------------------------------------------------------------------- */


						// Symbol Description
						// ====== ==========================================
						//   ==   Bidirectional, unbuffered
						//   =>   Bidirectional, buffered in one direction
						//   =-   Bidirectional, buffered in both directions
						//   ->   Directional, buffered
==						{ return token::BIDIRUNBUF; }	// == Bidirectional, unbuffered
=>						{ return token::BIDIRUNIBUF; }	// => Bidirectional, buf in one direction
=-						{ return token::BIDIRBIBUF; }	// =- Bidirectional, buf in both directions
->						{ return token::UNIDIRBUF; }	// -> Directional, buffered

design					{ return token::DESIGN; }
module					{ return token::MODULE; }
endmodule				{ return token::ENDMODULE; }
port					{ return token::PORT; }
(instance|inst)			{ return token::INST; }
net						{ return token::NET; }

cfg[ \t]+[""]			{ 
;						  BEGIN CONFIGSTRING; quote = colon = 0; yyless(yyleng-1); 
;						  return token::CFG; 
;						}
<CONFIGSTRING>[""]		{ 
;						  quote++; if(quote == 2) BEGIN INITIAL; /* must have been an empty config*/
;						  return static_cast<token_type>('"'); 
;						}
<CONFIGSTRING>[ \t]+	{ /* discard whitespace */ }
<CONFIGSTRING>\r?\n		{ yylloc->lines(); /* discard whitespace */ }
<CONFIGSTRING>:			{ 
;						  colon++; if(colon == 2) BEGIN CONFIGSTRING2; 
;						  return static_cast<token_type>(':'); 
;						}
<CONFIGSTRING>[^: \t\n\r""]+	{ 
;						  *yylval = yytext; return colon == 0 ? token::CFGSETTING : token::CFGNAME; 
;						}
<CONFIGSTRING2>(\\.|[^ \t\n\r""])+	{ *yylval = yytext; return token::CFGVALUE; }
<CONFIGSTRING2>[ \t\r\n]*[""]	{ 
;						  /* account for newlines embedded within trailing whitespace */
;						  for(char* ptr = yytext; *ptr != 0; ptr++) { 
;							if(*ptr == 0) break; 
;							if(false && *ptr == '\n') yylloc->lines();
;						  } 
;						  colon = 0; BEGIN INITIAL; return static_cast<token_type>('"'); 
;						}
<CONFIGSTRING2>[ \t]+	{ colon = 0; BEGIN CONFIGSTRING; }
<CONFIGSTRING2>\r?\n	{ yylloc->lines(); /* discard whitespace */ }

placed					{ return token::PLACED; }
unplaced				{ return token::UNPLACED; }
bonded					{ return token::BONDED; }
unbonded				{ return token::UNBONDED; }
(power|vcc|vdd)			{ return token::POWER; }
(ground|gnd)			{ return token::GROUND; }
inpin					{ return token::INPIN; }
outpin					{ return token::OUTPIN; }
pip						{ return token::PIP; }
-[0-9]+					{ *yylval = yytext; return token::SPEEDGRADE; }
v[0-9]+\.[0-9]+			{ *yylval = yytext; return token::XDLVERSION; 
							/** \todo Need to avoid obscuring IDENTIFIER. */ }
[a-zA-Z][\._a-zA-Z0-9]* { *yylval = yytext; return token::IDENTIFIER; }
\{                      { return token::OBRACE; }
\}                      { return token::EBRACE; }

#[ \t]+_ROUTETHROUGH	{ 
;						  DEBUG(("ROUTETHROUGH "));
;						  // be sure to strip the leading hash and spaces from the token
;						  char* ptr = yytext + 1;
;						  char* end = ptr + yyleng;
;						  while(*ptr == ' ' && ptr < end) ptr++;
;						  *yylval = ptr;
;						  return token::ROUTETHROUGH;
;						}
#.*						{ // be careful not to let _ROUTETHROUGH expressions pass for comments
;						  char* ptr = yytext + 1;
;						  char* end = ptr + yyleng;
;						  while((*ptr == ' ' || *ptr == '\t') && ptr < end) ptr++;
;						  if((end - ptr > 13) && strncmp(ptr, "_ROUTETHROUGH", 13) == 0) REJECT;
;						  // okay, this really is a comment
;						  DEBUG(("COMMENT "));
;						}

[""]					{ 
;						  BEGIN DOUBLEQUOTE; in_double_quote = true; last_string = ""; 
;						  /* enter exclusive quoted string state */
;						  return static_cast<token_type>(yytext[0]);
;						}
<DOUBLEQUOTE>\\.		{ 
;						  last_string += yytext; 
;						  return token::STRING; /* append escaped characters to the input */ 
;						}
<DOUBLEQUOTE>\r?\n		{ yylloc->lines(); /* ignore unescaped line breaks */ }
<DOUBLEQUOTE>[^\\""\n\r]*	{ 
;						  last_string += yytext; 
;						  return token::STRING; /* append characters other than \, ", \n, \r */ 
;						}
<DOUBLEQUOTE>[""]		{ if(!in_double_quote) {
;							BEGIN INITIAL; /* exit the exclusive quoted string state */
;							DEBUG(("\" "));
;							return static_cast<token_type>('"');
;						  }
;						  in_double_quote = false; /* we're no longer in double-quote mode */
;						  yylloc->end.columns(-1); 
;						  yyless(yyleng-1); /* push back the trailing double quote */
;						  //DEBUG(("[%s: length %d]\n", last_string.c_str(), last_string.length()));
;						  return token::STRING; /* and return the STRING token */
;						}

\r?\n                   { yylloc->lines(); }
[ \t]+                  { }
.						{ return static_cast<token_type>(*yytext); }



 /* --------------------------------------------------------------------------------------------- */
 /* ------------------------------------- End XDL lexer rules ----------------------------------- */
 /* --------------------------------------------------------------------------------------------- */


%%


// ------------------------------------------------------------------------------------------------
// ---------------------------------------- Additional code ---------------------------------------
// ------------------------------------------------------------------------------------------------


namespace torc {

	XdlScanner::XdlScanner(std::istream* in, std::ostream* out) : XdlFlexLexer(in, out) {}

	XdlScanner::~XdlScanner() {}

	void XdlScanner::set_debug(bool b) {
		yy_flex_debug = b;
	}

} // namespace torc


/// \details This implementation of XdlFlexLexer::yylex() is necessary to fill the vtable of class 
///		XdlFlexLexer.  We define the scanner's main yylex() function via YY_DECL to reside in class 
///		XdlScanner instead.
#ifdef yylex
#undef yylex
#endif

int XdlFlexLexer::yylex() {
    std::cerr << "in XdlFlexLexer::yylex() !" << std::endl;
    return 0;
}

/// \details When the scanner receives an End-Of-File result from YY_INPUT, it checks yywrap() for 
///		permission to proceed.  If yywrap() returns false (zero), we assume that the function has 
///		set up yyin to point to the next input file, and the scanning continues.  If yywrap() 
///		instead returns true (non-zero), the scanner terminates and returns 0 to the caller.
int XdlFlexLexer::yywrap() {
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
