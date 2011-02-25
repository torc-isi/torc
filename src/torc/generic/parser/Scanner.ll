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

%{ /*** C/C++ Declarations ***/

/*
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
*/

#include <string>

#include "torc/generic/parser/ParserHelpers.hpp"
#include "torc/generic/om/PortAttributes.hpp"
#include "torc/generic/parser/Parser.h"
#include "torc/generic/parser/Scanner.hpp"

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
#define YY_USER_ACTION  { yylloc->columns(yyleng); addToBuffer( yytext ); }
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
                                  REJECT;
                              }
                              return Token::ANGLE; 
                            }
"behavior"                    {
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              } 
                              return Token::BEHAVIOR; 
                            }
"calculated"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CALCULATED; 
                            }
"capacitance"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CAPACITANCE; 
                            }
"centercenter"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CENTERCENTER; 
                            }
"centerleft"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CENTERLEFT; 
                            }
"centerright"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CENTERRIGHT; 
                            }
"charge"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CHARGE; 
                            }
"conductance"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CONDUCTANCE; 
                            }
"current"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CURRENT; 
                            }
"distance"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DISTANCE; 
                            }
"document"                    {
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              } 
                              return Token::DOCUMENT; 
                            }    
"energy"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ENERGY; 
                            }
"extend"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::EXTEND; 
                            }
"flux"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::FLUX; 
                            }
"frequency"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::FREQUENCY; 
                            }
"generic"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::GENERIC; 
                            }
"graphic"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::GRAPHIC; 
                            }    
"inductance"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::INDUCTANCE; 
                            }
"inout"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::INOUT; 
                            }
"input"                        {
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              } 
                              return Token::INPUT; 
                            }    
"logicmodel"                {
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              } 
                              return Token::LOGICMODEL; 
                            } 
"lowercenter"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LOWERCENTER; 
                            }
"lowerleft"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LOWERLEFT; 
                            }
"lowerright"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LOWERRIGHT; 
                            }
"masklayout"                {
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              } 
                              return Token::MASKLAYOUT; 
                            }
"mass"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::MASS; 
                            }
"measured"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::MEASURED; 
                            }
"mx"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::MX; 
                            }
"mxr90"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::MXR90; 
                            }
"my"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::MY; 
                            }
"myr90"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::MYR90; 
                            }
"netlist"                    {
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::NETLIST;
                            }
"output"                    {
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              } 
                              return Token::OUTPUT; 
                            }
"pcblayout"                    {
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              } 
                              return Token::PCBLAYOUT; 
                            }
"power"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::POWER; 
                            }
"r0"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::R0; 
                            }
"r180"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::R180; 
                            }
"r270"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::R270; 
                            }
"r90"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::R90; 
                            }
"required"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::REQUIRED; 
                            }
"resistance"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::RESISTANCE; 
                            }
"ripper"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::RIPPER; 
                            }
"round"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ROUND; 
                            }
"schematic"                    {
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::SCHEMATIC; 
                            }
"stranger"                    {
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              } 
                              return Token::STRANGER; 
                            }
"symbolic"                    {
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              } 
                              return Token::SYMBOLIC; 
                            }
"temperature"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::TEMPERATURE; 
                            }
"tie"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::TIE; 
                            }
"time"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::TIME; 
                            }
"truncate"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::TRUNCATE; 
                            }
"uppercenter"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::UPPERCENTER; 
                            }
"upperleft"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::UPPERLEFT; 
                            }
"upperright"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::UPPERRIGHT; 
                            }
"voltage"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::VOLTAGE; 
                            }

"acload"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ACLOAD; 
                            }
"after"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::AFTER; 
                            }
"annotate"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ANNOTATE; 
                            }
"apply"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::APPLY; 
                            }
"arc"                       { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ARC; 
                            }
"array"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::ARRAY; 
                            }
"arraymacro"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ARRAYMACRO; 
                            }
"arrayrelatedinfo"          { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ARRAYRELATEDINFO; 
                            }
"arraysite"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ARRAYSITE; 
                            }
"atleast"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ATLEAST; 
                            }
"atmost"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ATMOST; 
                            }
"author"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::AUTHOR; 
                            }
"basearray"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::BASEARRAY; 
                            }
"becomes"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::BECOMES; 
                            }
"between"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::BETWEEN; 
                            }
"boolean"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::BOOLEAN; 
                            }
"booleandisplay"            { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::BOOLEANDISPLAY; 
                            }
"booleanmap"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::BOOLEANMAP; 
                            }
"borderpattern"             { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::BORDERPATTERN; 
                            }
"borderwidth"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::BORDERWIDTH; 
                            }
"boundingbox"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::BOUNDINGBOX; 
                            }
"cell"                      {
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::CELL; 
                            }
"cellref"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::CELLREF; 
                            }
"celltype"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CELLTYPE; 
                            }
"change"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CHANGE; 
                            }
"circle"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CIRCLE; 
                            }
"color"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::COLOR; 
                            }
"comment"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::COMMENT; 
                            }
"commentgraphics"           { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::COMMENTGRAPHICS; 
                            }
"compound"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::COMPOUND; 
                            }
"connectlocation"           { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CONNECTLOCATION; 
                            }
"contents"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CONTENTS; 
                            }
"cornertype"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CORNERTYPE; 
                            }
"criticality"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CRITICALITY; 
                            }
"currentmap"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CURRENTMAP; 
                            }
"curve"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CURVE; 
                            }
"cycle"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::CYCLE; 
                            }
"dataorigin"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DATAORIGIN; 
                            }
"dcfaninload"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DCFANINLOAD; 
                            }
"dcfanoutload"              { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DCFANOUTLOAD; 
                            }
"dcmaxfanin"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DCMAXFANIN; 
                            }
"dcmaxfanout"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DCMAXFANOUT; 
                            }
"delay"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DELAY; 
                            }
"delta"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DELTA; 
                            }
"derivation"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DERIVATION; 
                            }
"design"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DESIGN; 
                            }
"designator"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DESIGNATOR; 
                            }
"difference"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DIFFERENCE; 
                            }
"direction"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DIRECTION; 
                              }
"display"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DISPLAY; 
                            }
"dominates"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DOMINATES; 
                            }
"dot"                       { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DOT; 
                            }
"duration"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::DURATION; 
                            }
"e"                         { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::E; 
                            }
"edif"                      { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::EDIF; 
                            }
"ediflevel"                 { 
                             if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::EDIFLEVEL; 
                            }
"edifversion"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::EDIFVERSION; 
                            }
"enclosuredistance"         { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ENCLOSUREDISTANCE; 
                            }
"endtype"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ENDTYPE; 
                            }
"entry"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ENTRY; 
                            }
"event"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::EVENT; 
                            }

"exactly"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::EXACTLY; 
                            }
"external"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                                return Token::EXTERNAL; 
                            }
"fabricate"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::FABRICATE; 
                            }
"false"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::FALSE; 
                            }
"figure"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::FIGURE; 
                            }
"figurearea"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::FIGUREAREA; 
                            }
"figuregroup"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::FIGUREGROUP; 
                            }
"figuregroupobject"         { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::FIGUREGROUPOBJECT; 
                            }
"figuregroupoverride"       { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::FIGUREGROUPOVERRIDE; 
                            }
"figuregroupref"            { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::FIGUREGROUPREF; 
                            }
"figureperimeter"           { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::FIGUREPERIMETER; 
                            }
"figurewidth"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::FIGUREWIDTH; 
                            }
"fillpattern"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::FILLPATTERN; 
                            }
"follow"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::FOLLOW; 
                            }
"forbiddenevent"            { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::FORBIDDENEVENT; 
                            }
"globalportref"             { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::GLOBALPORTREF; 
                            }
"greaterthan"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::GREATERTHAN; 
                            }
"gridmap"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::GRIDMAP; 
                            }
"ignore"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::IGNORE; 
                            }
"includefiguregroup"        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::INCLUDEFIGUREGROUP; 
                            }
"initial"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::INITIAL_KW; 
                            }
"instance"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::INSTANCE; 
                            }
"instancebackannotate"      { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::INSTANCEBACKANNOTATE; 
                            }
"instancegroup"             { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::INSTANCEGROUP; 
                            }
"instancemap"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::INSTANCEMAP; 
                            }
"instanceref"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::INSTANCEREF; 
                            }
"integer"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::INTEGER; 
                            }
"integerdisplay"            { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::INTEGERDISPLAY; 
                            }
"interface"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::INTERFACE; 
                            }
"interfiguregroupspacing"   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                               return Token::INTERFIGUREGROUPSPACING; 
                            }
"intersection"              { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::INTERSECTION; 
                            }
"intrafiguregroupspacing"   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::INTRAFIGUREGROUPSPACING; 
                            }
"inverse"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::INVERSE; 
                            }
"isolated"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ISOLATED; 
                            }
"joined"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::JOINED; 
                            }
"justify"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::JUSTIFY; 
                            }
"keyworddisplay"            { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
							  setIsIdContext( true );
                              return Token::KEYWORDDISPLAY; 
                            }
"keywordlevel"              { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::KEYWORDLEVEL; 
                            }
"keywordmap"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::KEYWORDMAP; 
                            }
"lessthan"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LESSTHAN; 
                            }
"library"                   {
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              } 
                              setIsIdContext( true );
                              return Token::LIBRARY; 
                            }
"libraryref"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::LIBRARYREF; 
                            }
"listofnets"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LISTOFNETS; 
                            }
"listofports"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LISTOFPORTS; 
                            }
"loaddelay"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LOADDELAY; 
                            }
"logicassign"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LOGICASSIGN; 
                            }
"logicinput"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LOGICINPUT; 
                            }
"logiclist"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LOGICLIST; 
                            }
"logicmapinput"             { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LOGICMAPINPUT; 
                            }
"logicmapoutput"            { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LOGICMAPOUTPUT; 
                            }
"logiconeof"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LOGICONEOF; 
                            }
"logicoutput"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LOGICOUTPUT; 
                            }
"logicport"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LOGICPORT; 
                            }
"logicref"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LOGICREF; 
                            }
"logicvalue"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LOGICVALUE; 
                            }
"logicwaveform"             { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::LOGICWAVEFORM; 
                            }
"maintain"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::MAINTAIN; 
                            }
"match"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::MATCH; 
                            }
"member"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::MEMBER; 
                            }
"minomax"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::MINOMAX; 
                            }
"minomaxdisplay"            { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::MINOMAXDISPLAY; 
                            }
"mnm"                       { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::MNM; 
                            }
"multiplevalueset"          { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::MULTIPLEVALUESET; 
                            }
"mustjoin"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::MUSTJOIN; 
                            }
"name"                      { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
							  setIsIdContext( true );
                              return Token::NAME; 
                            }
"net"                       { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::NET; 
                            }
"netbackannotate"           { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::NETBACKANNOTATE; 
                            }
"netbundle"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::NETBUNDLE; 
                            }
"netdelay"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::NETDELAY; 
                            }
"netgroup"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::NETGROUP; 
                            }
"netmap"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::NETMAP; 
                            }
"netref"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::NETREF; 
                            }
"nochange"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::NOCHANGE; 
                            }
"nonpermutable"             { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::NONPERMUTABLE; 
                            }
"notallowed"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::NOTALLOWED; 
                            }
"notchspacing"              { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::NOTCHSPACING; 
                            }
"number"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::NUMBER; 
                            }
"numberdefinition"          { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::NUMBERDEFINITION; 
                            }
"numberdisplay"             { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::NUMBERDISPLAY; 
                            }
"offpageconnector"          { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::OFFPAGECONNECTOR; 
                            }
"offsetevent"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }    
                              return Token::OFFSETEVENT; 
                            }
"openshape"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::OPENSHAPE; 
                            }
"orientation"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ORIENTATION; 
                            }
"origin"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::ORIGIN; 
                            }
"overhangdistance"          { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::OVERHANGDISTANCE; 
                            }
"overlapdistance"           { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::OVERLAPDISTANCE; 
                            }
"oversize"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::OVERSIZE; 
                            }
"owner"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }    
                              return Token::OWNER; 
                            }
"page"                      { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PAGE; 
                            }
"pagesize"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PAGESIZE; 
                            }
"parameter"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::PARAMETER; 
                            }
"parameterassign"           { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::PARAMETERASSIGN; 
                            }
"parameterdisplay"          { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PARAMETERDISPLAY; 
                            }
"path"                      { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PATH; 
                            }
"pathdelay"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PATHDELAY; 
                            }
"pathwidth"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PATHWIDTH; 
                            }
"permutable"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PERMUTABLE; 
                            }
"physicaldesignrule"        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PHYSICALDESIGNRULE; 
                            }
"plug"                      { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PLUG; 
                            }
"point"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::POINT; 
                            }
"pointdisplay"              { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::POINTDISPLAY; 
                            }
"pointlist"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::POINTLIST; 
                            }
"polygon"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::POLYGON; 
                            }
"port"                      { 
                              if( getIsIdContext() )
                              {
                                  REJECT;    
                              }
                              setIsIdContext( true );
                              return Token::PORT; 
                            }
"portbackannotate"          { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PORTBACKANNOTATE; 
                            }
"portbundle"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::PORTBUNDLE; 
                            }
"portdelay"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PORTDELAY; 
                            }
"portgroup"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PORTGROUP; 
                            }
"portimplementation"        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PORTIMPLEMENTATION; 
                            }
"portinstance"              { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PORTINSTANCE; 
                            }
"portlist"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PORTLIST; 
                            }
"portlistalias"             { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PORTLISTALIAS; 
                            }
"portmap"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PORTMAP; 
                            }
"portref"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::PORTREF; 
                            }
"program"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PROGRAM; 
                            }
"property"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::PROPERTY; 
                            }
"propertydisplay"           { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PROPERTYDISPLAY; 
                            }
"protectionframe"           { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PROTECTIONFRAME; 
                            }
"pt"                        { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::PT; 
                            }
"rangevector"               { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::RANGEVECTOR; 
                            }
"rectangle"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::RECTANGLE; 
                            }
"rectanglesize"             { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::RECTANGLESIZE; 
                            }
"rename"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::RENAME; 
                            }
"resolves"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::RESOLVES; 
                            }
"scale"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::SCALE; 
                            }
"scalex"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                               return Token::SCALEX; 
                            }
"scaley"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::SCALEY; 
                            }
"section"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::SECTION; 
                            }
"shape"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::SHAPE; 
                            }
"simulate"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::SIMULATE; 
                            }
"simulationinfo"            { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::SIMULATIONINFO; 
                            }
"singlevalueset"            { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::SINGLEVALUESET; 
                            }
"site"                      { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::SITE; 
                            }
"socket"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::SOCKET; 
                            }
"socketset"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::SOCKETSET; 
                            }
"status"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::STATUS; 
                            }
"steady"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::STEADY; 
                            }
"string"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::STRING; 
                            }
"stringdisplay"             { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::STRINGDISPLAY; 
                            }
"strong"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::STRONG; 
                            }
"symbol"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::SYMBOL; 
                            }
"symmetry"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::SYMMETRY; 
                            }
"table"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::TABLE; 
                            }
"tabledefault"              { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::TABLEDEFAULT; 
                            }
"technology"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::TECHNOLOGY; 
                            }
"textheight"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::TEXTHEIGHT; 
                            }
"timeinterval"              { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::TIMEINTERVAL; 
                            }
"timestamp"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::TIMESTAMP; 
                            }
"timing"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::TIMING; 
                            }
"transform"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::TRANSFORM; 
                            }
"transition"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::TRANSITION; 
                            }
"trigger"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::TRIGGER; 
                            }
"true"                      { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::TRUE; 
                            }
"unconstrained"             { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::UNCONSTRAINED; 
                            }
"undefined"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::UNDEFINED; 
                            }
"union"                     { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::UNION; 
                            }
"unit"                      { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::UNIT; 
                            }
"unused"                    { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::UNUSED; 
                            }
"userdata"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::USERDATA; 
                            }
"version"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::VERSION; 
                            }
"view"                      { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::VIEW; 
                            }
"viewlist"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::VIEWLIST; 
                            }
"viewmap"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::VIEWMAP; 
                            }
"viewref"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              setIsIdContext( true );
                              return Token::VIEWREF; 
                            }
"viewtype"                  { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::VIEWTYPE; 
                            }
"visible"                   { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::VISIBLE; 
                            }
"voltagemap"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::VOLTAGEMAP; 
                            }
"wavevalue"                 { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::WAVEVALUE; 
                            }
"weak"                      { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::WEAK; 
                            }
"weakjoined"                { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::WEAKJOINED; 
                            }
"when"                      { 
                              if( getIsIdContext() )
                              {
                                  REJECT;
                              }
                              return Token::WHEN; 
                            }
"written"                   { 
                              if( getIsIdContext() )
                              {
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

{String}            {  std::string str = std::string(yytext);
                        yylval->stringVal = new std::string(str, 1, yyleng -2);
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
  mBuffer()
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
Scanner::addToBuffer( const char *str ) throw() {
	if( getAppendToBuffer() )
	{
		mBuffer += str;
	}
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
