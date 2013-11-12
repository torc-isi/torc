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

%{


// ------------------------------------------------------------------------------------------------
// -------------------------------------- C/C++ declarations --------------------------------------
// ------------------------------------------------------------------------------------------------


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

#include "torc/bitstream/assembler/Assembler.hpp"
#include <cstdio>
#include <iostream>

//#define YYPRINT(file, type, value) fprintf(file, "token [%d] \"%s\"", type, value);

using std::string;
using namespace torc::bitstream;

#define YYSTYPE std::string


%}


// ------------------------------------------------------------------------------------------------
// ------------------------------------ yacc/bison declarations -----------------------------------
// ------------------------------------------------------------------------------------------------


/// \brief Require Bison 2.3 or later.
/// \details In actuality, newer versions may not work correctly with the combined parser/scanner 
///		code.  Version 2.4.2 is a prime example of that.
%require "2.3"

/// \brief Add debugging output code.
//%debug

/// \brief Grammar start symbol is named "lut_equation".
%start lut_equation

/// \brief Generate a header file that contains the token definitions
%defines

/// \brief Use the newer C++ skeleton file.
%skeleton "lalr1.cc"

/// \brief Namespace for the parser.
/// \details Undue grief resulted from trying to pull the parser and scanner into torc::physical.
%name-prefix="torc"

/// \brief The name of the parser class is LutParser
%define "parser_class_name" "LutParser"

/// \brief Track current position within the input.
%locations

/// \brief Initialize the location object
%initial-action
{
    // initialize the initial location object
    @$.begin.filename = @$.end.filename = &lut.mStreamName;
};

/// \brief Pass the importer by reference to the parser and scanner.
/// \details This provides a simple but effective pure interface, requiring no global variables.
%parse-param { class Assembler& lut }

/// \brief Request verbose error messages.
%error-verbose


// ------------------------------------------------------------------------------------------------
// ----------------------------------- Begin Lut grammar tokens -----------------------------------
// ------------------------------------------------------------------------------------------------

%token END	     0	"end of file"
%token EOL		"end of line"

%token VAR
%token LITERAL
%token AND
%token OR
%token NOT
%token XOR
%token RP
%token LP
%token INVALID

// ------------------------------------------------------------------------------------------------
// ------------------------------------ End Lut grammar tokens ------------------------------------
// ------------------------------------------------------------------------------------------------

%{

/// \brief Code to connect the bison parser in the importer to the flex scanner object.
/// \details This defines the yylex() function call to pull the next token from the current lexer 
///		object in the importer.
#undef yylex
#define yylex lut.lexer->lex

%}


%%


// ------------------------------------------------------------------------------------------------
// ------------------------------------ Begin Lut grammar rules -----------------------------------
// ------------------------------------------------------------------------------------------------

lut_equation
					: expression
					;

expression
					: xor_expression
					| expression OR xor_expression 
						{ $$ = $1 + " xor " + $3; lut.binaryLutFrameOperation(Assembler::eOR); }
					;

xor_expression
					: and_expression
					| xor_expression XOR and_expression 
						{ $$ = $1 + " xor " + $3; lut.binaryLutFrameOperation(Assembler::eXOR); }
					;

and_expression		
					: not_expression
					| and_expression AND not_expression 
						{ $$ = $1 + " and " + $3; lut.binaryLutFrameOperation(Assembler::eAND); }
					;

not_expression
					: factor
					| NOT factor { $$ = "not " + $2; lut.binaryLutFrameOperation(Assembler::eNOT); }

factor
					: LP expression RP { $$ = $2; }
					| VAR { $$ = $1; lut.pushLutFrame(yylval); }
					| LITERAL
					;


// ------------------------------------------------------------------------------------------------
// ------------------------------------- End Lut grammar rules ------------------------------------
// ------------------------------------------------------------------------------------------------


%%


// ------------------------------------------------------------------------------------------------
// ------------------------------------- Additional parser code -----------------------------------
// ------------------------------------------------------------------------------------------------


void torc::LutParser::error(const LutParser::location_type& l, const std::string& m) {
    lut.error(l, m);
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
