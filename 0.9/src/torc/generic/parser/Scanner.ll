% Torc - Copyright 2011 University of Southern California.  All Rights Reserved.
% $HeadURL$
% $Id$

% This program is free software: you can redistribute it and/or modify it under the terms of the 
% GNU General Public License as published by the Free Software Foundation, either version 3 of the 
% License, or (at your option) any later version.
% 
% This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
% without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
% the GNU General Public License for more details.
% 
% You should have received a copy of the GNU General Public License along with this program.  If 
% not, see <http://www.gnu.org/licenses/>.

/*******************************************************************************
* TORC - Copyright 2010 University of Southern California. All Rights Reserved.
*
* FILE : Scanner.ll
*
* DATE : 08-July-2010
*
* DESCRIPTION : Define the EDIF Flex lexical scanner
*
* REVISION HISTORY:
*
* SI        REVISION        AUTHOR               CHANGES          PRs
*[0]    Initial Version    Niladri
*
*******************************************************************************/

%{ /*** C/C++ Declarations ***/

#include <string>

#include "torc/generic/parser/ParserHelpers.hpp"
#include "torc/generic/om/PortAttributes.hpp"
#include "torc/generic/om/NetAttributes.hpp"
#include "torc/generic/parser/Parser.h"
#include "torc/generic/parser/Scanner.hpp"
#include "torc/generic/om/InterfaceAttributes.hpp"
#include "torc/generic/om/LogicValueAttributes.hpp"

/* import the parser's Token type into a local typedef */
typedef torc::generic::Parser::token Token;
typedef torc::generic::Parser::token_type TokenType;

/* By default yylex returns int, we use TokenType. Unfortunately yyterminate
 * by default returns 0, which is not of TokenType. */
#define yyterminate() return Token::END

/* This disables inclusion of unistd.h, which is not available under Visual C++
 * on Win32. The C++ scanner uses STL streams instead. */
#define YY_NO_UNISTD_H

%}

/*** Flex Declarations and Options ***/

/* enable c++ scanner class generation */
%option c++
%option prefix="Edif"

/* the manual says "somewhat more optimized" */
%option batch

/* enable scanner to generate debug output. disable this for release
 * versions. */
%option debug

/* no support for include files is planned */
%option yywrap nounput 

/* enables the use of start condition stacks */
%option stack

/* instructs flex to generate a case-insensitive scanner */
%option case-insensitive

/* The following paragraph suffices to track locations accurately. Each time
 * yylex is invoked, the begin position is moved onto the end position. */
%{
#define YY_USER_ACTION  { yylloc->columns(yyleng); addToBuffer( yytext ); \
                          yylloc->columns(yyleng); addToUserDataBuffer( yytext ); \
                        }
%}

Digit [0-9]
Identifier (&[A-Za-z_0-9]+|[A-Za-z][A-Za-z_0-9]*)
WholeNum {Digit}+
IntegerNum ("+"|"-")?{WholeNum}
DoubleNum ("-"?{Digit}*"."{Digit}+)
String \"[^\"]*\"

%% /*** Regular Expressions Part ***/

 /* code to place at the beginning of yylex() */
%{
    // reset location
    yylloc->step();
%}

"("                            {
                                setIsIdContext( false );
                                return Token::LBKT;
                            }
")"                            { return Token::RBKT; }
"angle"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ANGLE; 
                            }
"behavior"                    {
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              } 
                              return Token::BEHAVIOR; 
                            }
"calculated"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CALCULATED; 
                            }
"capacitance"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CAPACITANCE; 
                            }
"centercenter"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CENTERCENTER; 
                            }
"centerleft"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CENTERLEFT; 
                            }
"centerright"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CENTERRIGHT; 
                            }
"charge"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CHARGE; 
                            }
"conductance"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CONDUCTANCE; 
                            }
"current"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CURRENT; 
                            }
"distance"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DISTANCE; 
                            }
"document"                    {
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              } 
                              return Token::DOCUMENT; 
                            }    
"energy"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ENERGY; 
                            }
"extend"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::EXTEND; 
                            }
"flux"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::FLUX; 
                            }
"frequency"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::FREQUENCY; 
                            }
"generic"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::GENERIC; 
                            }
"graphic"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::GRAPHIC; 
                            }    
"inductance"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::INDUCTANCE; 
                            }
"inout"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::INOUT; 
                            }
"input"                        {
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              } 
                              return Token::INPUT; 
                            }    
"logicmodel"                {
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              } 
                              return Token::LOGICMODEL; 
                            } 
"lowercenter"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::LOWERCENTER; 
                            }
"lowerleft"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::LOWERLEFT; 
                            }
"lowerright"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::LOWERRIGHT; 
                            }
"masklayout"                {
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              } 
                              return Token::MASKLAYOUT; 
                            }
"mass"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::MASS; 
                            }
"measured"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::MEASURED; 
                            }
"mx"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::MX; 
                            }
"mxr90"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::MXR90; 
                            }
"my"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::MY; 
                            }
"myr90"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::MYR90; 
                            }
"netlist"                    {
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::NETLIST;
                            }
"output"                    {
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              } 
                              return Token::OUTPUT; 
                            }
"pcblayout"                    {
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              } 
                              return Token::PCBLAYOUT; 
                            }
"power"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::POWER; 
                            }
"r0"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::R0; 
                            }
"r180"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::R180; 
                            }
"r270"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::R270; 
                            }
"r90"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::R90; 
                            }
"required"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::REQUIRED; 
                            }
"resistance"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::RESISTANCE; 
                            }
"ripper"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::RIPPER; 
                            }
"round"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ROUND; 
                            }
"schematic"                    {
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::SCHEMATIC; 
                            }
"stranger"                    {
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              } 
                              return Token::STRANGER; 
                            }
"symbolic"                    {
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              } 
                              return Token::SYMBOLIC; 
                            }
"temperature"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::TEMPERATURE; 
                            }
"tie"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::TIE; 
                            }
"time"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::TIME; 
                            }
"truncate"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::TRUNCATE; 
                            }
"uppercenter"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::UPPERCENTER; 
                            }
"upperleft"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::UPPERLEFT; 
                            }
"upperright"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::UPPERRIGHT; 
                            }
"voltage"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::VOLTAGE; 
                            }

"acload"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ACLOAD; 
                            }
"after"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::AFTER; 
                            }
"annotate"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ANNOTATE; 
                            }
"apply"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::APPLY; 
                            }
"arc"                       { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ARC; 
                            }
"array"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::ARRAY; 
                            }
"arraymacro"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ARRAYMACRO; 
                            }
"arrayrelatedinfo"          { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ARRAYRELATEDINFO; 
                            }
"arraysite"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ARRAYSITE; 
                            }
"atleast"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ATLEAST; 
                            }
"atmost"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ATMOST; 
                            }
"author"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::AUTHOR; 
                            }
"basearray"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::BASEARRAY; 
                            }
"becomes"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::BECOMES; 
                            }
"between"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::BETWEEN; 
                            }
"boolean"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::BOOLEAN; 
                            }
"booleandisplay"            { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::BOOLEANDISPLAY; 
                            }
"booleanmap"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::BOOLEANMAP; 
                            }
"borderpattern"             { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::BORDERPATTERN; 
                            }
"borderwidth"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::BORDERWIDTH; 
                            }
"boundingbox"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::BOUNDINGBOX; 
                            }
"cell"                      {
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::CELL; 
                            }
"cellref"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::CELLREF; 
                            }
"celltype"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CELLTYPE; 
                            }
"change"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CHANGE; 
                            }
"circle"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CIRCLE; 
                            }
"color"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::COLOR; 
                            }
"comment"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::COMMENT; 
                            }
"commentgraphics"           { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::COMMENTGRAPHICS; 
                            }
"compound"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::COMPOUND; 
                            }
"connectlocation"           { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CONNECTLOCATION; 
                            }
"contents"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CONTENTS; 
                            }
"cornertype"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CORNERTYPE; 
                            }
"criticality"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CRITICALITY; 
                            }
"currentmap"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CURRENTMAP; 
                            }
"curve"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CURVE; 
                            }
"cycle"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::CYCLE; 
                            }
"dataorigin"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DATAORIGIN; 
                            }
"dcfaninload"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DCFANINLOAD; 
                            }
"dcfanoutload"              { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DCFANOUTLOAD; 
                            }
"dcmaxfanin"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DCMAXFANIN; 
                            }
"dcmaxfanout"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DCMAXFANOUT; 
                            }
"delay"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DELAY; 
                            }
"delta"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DELTA; 
                            }
"derivation"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DERIVATION; 
                            }
"design"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DESIGN; 
                            }
"designator"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DESIGNATOR; 
                            }
"difference"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DIFFERENCE; 
                            }
"direction"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DIRECTION; 
                              }
"display"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DISPLAY; 
                            }
"dominates"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DOMINATES; 
                            }
"dot"                       { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DOT; 
                            }
"duration"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::DURATION; 
                            }
"e"                         { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::E; 
                            }
"edif"                      { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::EDIF; 
                            }
"ediflevel"                 { 
                             if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::EDIFLEVEL; 
                            }
"edifversion"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::EDIFVERSION; 
                            }
"enclosuredistance"         { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ENCLOSUREDISTANCE; 
                            }
"endtype"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ENDTYPE; 
                            }
"entry"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ENTRY; 
                            }
"event"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::EVENT; 
                            }

"exactly"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::EXACTLY; 
                            }
"external"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                                return Token::EXTERNAL; 
                            }
"fabricate"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::FABRICATE; 
                            }
"false"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::FALSE; 
                            }
"figure"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::FIGURE; 
                            }
"figurearea"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::FIGUREAREA; 
                            }
"figuregroup"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::FIGUREGROUP; 
                            }
"figuregroupobject"         { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::FIGUREGROUPOBJECT; 
                            }
"figuregroupoverride"       { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::FIGUREGROUPOVERRIDE; 
                            }
"figuregroupref"            { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::FIGUREGROUPREF; 
                            }
"figureperimeter"           { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::FIGUREPERIMETER; 
                            }
"figurewidth"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::FIGUREWIDTH; 
                            }
"fillpattern"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::FILLPATTERN; 
                            }
"follow"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::FOLLOW; 
                            }
"forbiddenevent"            { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::FORBIDDENEVENT; 
                            }
"globalportref"             { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::GLOBALPORTREF; 
                            }
"greaterthan"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::GREATERTHAN; 
                            }
"gridmap"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::GRIDMAP; 
                            }
"ignore"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::IGNORE; 
                            }
"includefiguregroup"        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::INCLUDEFIGUREGROUP; 
                            }
"initial"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::INITIAL_KW; 
                            }
"instance"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::INSTANCE; 
                            }
"instancebackannotate"      { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::INSTANCEBACKANNOTATE; 
                            }
"instancegroup"             { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::INSTANCEGROUP; 
                            }
"instancemap"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::INSTANCEMAP; 
                            }
"instanceref"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::INSTANCEREF; 
                            }
"integer"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::INTEGER; 
                            }
"integerdisplay"            { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::INTEGERDISPLAY; 
                            }
"interface"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::INTERFACE; 
                            }
"interfiguregroupspacing"   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                               return Token::INTERFIGUREGROUPSPACING; 
                            }
"intersection"              { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::INTERSECTION; 
                            }
"intrafiguregroupspacing"   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::INTRAFIGUREGROUPSPACING; 
                            }
"inverse"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::INVERSE; 
                            }
"isolated"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ISOLATED; 
                            }
"joined"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::JOINED; 
                            }
"justify"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::JUSTIFY; 
                            }
"keyworddisplay"            { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
							  setIsIdContext( true );
                              return Token::KEYWORDDISPLAY; 
                            }
"keywordlevel"              { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::KEYWORDLEVEL; 
                            }
"keywordmap"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::KEYWORDMAP; 
                            }
"lessthan"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::LESSTHAN; 
                            }
"library"                   {
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              } 
                              setIsIdContext( true );
                              return Token::LIBRARY; 
                            }
"libraryref"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::LIBRARYREF; 
                            }
"listofnets"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::LISTOFNETS; 
                            }
"listofports"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::LISTOFPORTS; 
                            }
"loaddelay"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::LOADDELAY; 
                            }
"logicassign"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::LOGICASSIGN; 
                            }
"logicinput"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::LOGICINPUT; 
                            }
"logiclist"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::LOGICLIST; 
                            }
"logicmapinput"             { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::LOGICMAPINPUT; 
                            }
"logicmapoutput"            { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::LOGICMAPOUTPUT; 
                            }
"logiconeof"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::LOGICONEOF; 
                            }
"logicoutput"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::LOGICOUTPUT; 
                            }
"logicport"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::LOGICPORT; 
                            }
"logicref"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::LOGICREF; 
                            }
"logicvalue"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::LOGICVALUE; 
                            }
"logicwaveform"             { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::LOGICWAVEFORM; 
                            }
"maintain"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::MAINTAIN; 
                            }
"match"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::MATCH; 
                            }
"member"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::MEMBER; 
                            }
"minomax"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::MINOMAX; 
                            }
"minomaxdisplay"            { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::MINOMAXDISPLAY; 
                            }
"mnm"                       { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::MNM; 
                            }
"multiplevalueset"          { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::MULTIPLEVALUESET; 
                            }
"mustjoin"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::MUSTJOIN; 
                            }
"name"                      { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
							  setIsIdContext( true );
                              return Token::NAME; 
                            }
"net"                       { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::NET; 
                            }
"netbackannotate"           { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::NETBACKANNOTATE; 
                            }
"netbundle"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::NETBUNDLE; 
                            }
"netdelay"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::NETDELAY; 
                            }
"netgroup"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::NETGROUP; 
                            }
"netmap"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::NETMAP; 
                            }
"netref"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::NETREF; 
                            }
"nochange"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::NOCHANGE; 
                            }
"nonpermutable"             { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::NONPERMUTABLE; 
                            }
"notallowed"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::NOTALLOWED; 
                            }
"notchspacing"              { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::NOTCHSPACING; 
                            }
"number"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::NUMBER; 
                            }
"numberdefinition"          { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::NUMBERDEFINITION; 
                            }
"numberdisplay"             { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::NUMBERDISPLAY; 
                            }
"offpageconnector"          { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::OFFPAGECONNECTOR; 
                            }
"offsetevent"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }    
                              return Token::OFFSETEVENT; 
                            }
"openshape"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::OPENSHAPE; 
                            }
"orientation"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ORIENTATION; 
                            }
"origin"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::ORIGIN; 
                            }
"overhangdistance"          { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::OVERHANGDISTANCE; 
                            }
"overlapdistance"           { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::OVERLAPDISTANCE; 
                            }
"oversize"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::OVERSIZE; 
                            }
"owner"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }    
                              return Token::OWNER; 
                            }
"page"                      { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PAGE; 
                            }
"pagesize"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PAGESIZE_EDIF; 
                            }
"parameter"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::PARAMETER; 
                            }
"parameterassign"           { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::PARAMETERASSIGN; 
                            }
"parameterdisplay"          { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PARAMETERDISPLAY; 
                            }
"path"                      { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PATH; 
                            }
"pathdelay"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PATHDELAY; 
                            }
"pathwidth"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PATHWIDTH; 
                            }
"permutable"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PERMUTABLE; 
                            }
"physicaldesignrule"        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PHYSICALDESIGNRULE; 
                            }
"plug"                      { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PLUG; 
                            }
"point"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::POINT; 
                            }
"pointdisplay"              { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::POINTDISPLAY; 
                            }
"pointlist"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::POINTLIST; 
                            }
"polygon"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::POLYGON; 
                            }
"port"                      { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;    
                              }
                              setIsIdContext( true );
                              return Token::PORT; 
                            }
"portbackannotate"          { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PORTBACKANNOTATE; 
                            }
"portbundle"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::PORTBUNDLE; 
                            }
"portdelay"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PORTDELAY; 
                            }
"portgroup"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PORTGROUP; 
                            }
"portimplementation"        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PORTIMPLEMENTATION; 
                            }
"portinstance"              { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PORTINSTANCE; 
                            }
"portlist"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::PORTLIST; 
                            }
"portlistalias"             { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::PORTLISTALIAS; 
                            }
"portmap"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PORTMAP; 
                            }
"portref"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::PORTREF; 
                            }
"program"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PROGRAM; 
                            }
"property"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::PROPERTY; 
                            }
"propertydisplay"           { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PROPERTYDISPLAY; 
                            }
"protectionframe"           { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PROTECTIONFRAME; 
                            }
"pt"                        { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::PT; 
                            }
"rangevector"               { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::RANGEVECTOR; 
                            }
"rectangle"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::RECTANGLE; 
                            }
"rectanglesize"             { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::RECTANGLESIZE; 
                            }
"rename"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::RENAME; 
                            }
"resolves"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::RESOLVES; 
                            }
"scale"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::SCALE; 
                            }
"scalex"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                               return Token::SCALEX; 
                            }
"scaley"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::SCALEY; 
                            }
"section"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::SECTION; 
                            }
"shape"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::SHAPE; 
                            }
"simulate"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::SIMULATE; 
                            }
"simulationinfo"            { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::SIMULATIONINFO; 
                            }
"singlevalueset"            { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::SINGLEVALUESET; 
                            }
"site"                      { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::SITE; 
                            }
"socket"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::SOCKET; 
                            }
"socketset"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::SOCKETSET; 
                            }
"status"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::STATUS; 
                            }
"steady"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::STEADY; 
                            }
"string"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::STRING; 
                            }
"stringdisplay"             { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::STRINGDISPLAY; 
                            }
"strong"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::STRONG; 
                            }
"symbol"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::SYMBOL; 
                            }
"symmetry"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::SYMMETRY; 
                            }
"table"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::TABLE; 
                            }
"tabledefault"              { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::TABLEDEFAULT; 
                            }
"technology"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::TECHNOLOGY; 
                            }
"textheight"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::TEXTHEIGHT; 
                            }
"timeinterval"              { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::TIMEINTERVAL; 
                            }
"timestamp"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::TIMESTAMP; 
                            }
"timing"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::TIMING; 
                            }
"transform"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::TRANSFORM; 
                            }
"transition"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::TRANSITION; 
                            }
"trigger"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::TRIGGER; 
                            }
"true"                      { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::TRUE; 
                            }
"unconstrained"             { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::UNCONSTRAINED; 
                            }
"undefined"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::UNDEFINED; 
                            }
"union"                     { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::UNION; 
                            }
"unit"                      { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::UNIT; 
                            }
"unused"                    { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::UNUSED; 
                            }
"userdata"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::USERDATA; 
                            }
"version"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::VERSION; 
                            }
"view"                      { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::VIEW; 
                            }
"viewlist"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::VIEWLIST; 
                            }
"viewmap"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::VIEWMAP; 
                            }
"viewref"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::VIEWREF; 
                            }
"viewtype"                  { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::VIEWTYPE; 
                            }
"visible"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::VISIBLE; 
                            }
"voltagemap"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::VOLTAGEMAP; 
                            }
"wavevalue"                 { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::WAVEVALUE; 
                            }
"weak"                      { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::WEAK; 
                            }
"weakjoined"                { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::WEAKJOINED; 
                            }
"when"                      { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::WHEN; 
                            }
"written"                   { 
                              if( getIsIdContext() )
                              {
                                  mIsIdAlreadyAdded = true;
                                  REJECT;
                              }
                              return Token::WRITTEN; 
                            }


{Identifier}       {
                      yylval->stringVal = new std::string(yytext, yyleng);
                      setIsIdContext( false );
                      return Token::IDENTIFIER;
                   }    

{IntegerNum}        { yylval->integerVal = atoi(yytext);
                      return Token::INTEGER;
                    }

{DoubleNum}         { yylval->doubleVal = atof(yytext);
                      return Token::DOUBLE;
                    }

{String}            { 
                      yylval->stringVal = new std::string(yytext +1, yyleng -2);
                      return Token::STRING;
                    }


 /* gobble up white-spaces */
[ \t\r]+ {
    yylloc->step();
}

 /* gobble up end-of-lines */
\n {
    yylloc->lines(yyleng); yylloc->step();
    /* return Token::EOL; */
}

 /* pass all other characters up to bison */
. {
    return static_cast<TokenType>(*yytext);
}


%% /*** Additional Code ***/

namespace torc {
namespace generic {
    
Scanner::Scanner(std::istream* in,
     std::ostream* out)
: EdifFlexLexer(in, out),
  mIsIdContext( false ),
  mAppendToBuffer( false ),
  mAppendToUserDataBuffer( false ),
  mBuffer(),
  mUserDataBuffer(),
  mIsIdAlreadyAdded( false )  
{
}

Scanner::~Scanner()
{
}

void
Scanner::skipThisRule( char *yytext ) {
    unsigned char c;
    int scope = 0;
    while( true )
    {
        c=yyinput();
        switch( c )
        {
            case '(':
                {
                    scope++;
                    break;
                }
            case ')':
                {
                    if( -1 == scope )
                    {
                        yyunput(c, yytext);
                        return;
                    }
                    scope--;
                    break;
                }
            default:
                {
                }
        }
    }
}

void
Scanner::setAppendToBuffer( bool inAppendToBuffer ) throw() {
	mAppendToBuffer = inAppendToBuffer;
}

void
Scanner::resetBuffer() throw() {
	mBuffer = "";
}

void
Scanner::setAppendToUserDataBuffer( bool inAppendToBuffer ) throw() {
	mAppendToUserDataBuffer = inAppendToBuffer;
}

void
Scanner::resetUserDataBuffer() throw() {
	mUserDataBuffer = "";
}

void
Scanner::addToBuffer( const char *str ) throw() {
	if( getAppendToBuffer() )
	{
		mBuffer += str;
	}
}

void
Scanner::addToUserDataBuffer( const char *str ) throw() {
	if( getAppendToUserDataBuffer() && (mIsIdAlreadyAdded == false) )
	{
		mUserDataBuffer += str;
	}
    mIsIdAlreadyAdded = false;
}

void
Scanner::set_debug(bool b)
{
    yy_flex_debug = b;
}
    
} // namespace generic
} // namespace torc

/* This implementation of EdifFlexLexer::yylex() is required to fill the
 * vtable of the class EdifFlexLexer. We define the scanner's main yylex
 * function via YY_DECL to reside in the Scanner class instead. */

#ifdef yylex
#undef yylex
#endif

int EdifFlexLexer::yylex()
{
    std::cerr << "in EdifFlexLexer::yylex() !" << std::endl;
    return 0;
}

/* When the scanner receives an end-of-file indication from YY_INPUT, it then
 * checks the yywrap() function. If yywrap() returns false (zero), then it is
 * assumed that the function has gone ahead and set up `yyin' to point to
 * another input file, and scanning continues. If it returns true (non-zero),
 * then the scanner terminates, returning 0 to its caller. */

int EdifFlexLexer::yywrap()
{
    return 1;
}
