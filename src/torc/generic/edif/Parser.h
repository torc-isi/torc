/* A Bison parser, made by GNU Bison 2.7.  */

/* Skeleton interface for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/**
 ** \file Parser.h
 ** Define the torc::generic::parser class.
 */

/* C++ LALR(1) parser skeleton written by Akim Demaille.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED



#include <string>
#include <iostream>
#include "stack.hh"
#include "location.hh"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Line 33 of lalr1.cc  */
#line 59 "Parser.yy"
namespace torc { namespace generic {
/* Line 33 of lalr1.cc  */
#line 60 "Parser.h"

  /// A Bison parser.
  class Parser
  {
  public:
    /// Symbol semantic values.
#ifndef YYSTYPE
    union semantic_type
    {
/* Line 33 of lalr1.cc  */
#line 82 "Parser.yy"

    int                                   integerVal;
    double                                doubleVal;
    std::string                          *stringVal;
    torc::generic::NameData              *nameData;
    torc::generic::ViewRefData           *viewRefData;
    torc::generic::CellRefData           *cellRefData;
    torc::generic::PortRefData           *portRefData;
    torc::generic::InstanceRefData       *instanceRef;
    torc::generic::PortListData          *portListData;
    torc::generic::InstanceInfo          *instanceInfo;
    std::vector<size_t>                  *arrayDimensions;
    torc::generic::VersionData           *edifVersion;
    torc::generic::ValueData             *valueData;
    torc::generic::PairData              *pairData;
    torc::generic::ParamAssignInfo       *paramAssignInfo;
    torc::generic::PortAttributes        *portAttributes;
    torc::generic::NetAttributes         *netAttributes;
    torc::generic::PortDelay             *portDelay;
    torc::generic::NetDelay              *netDelay;
    torc::generic::PortInstData          *portInst;
    torc::generic::TimeStamp             *timeStamp;
    torc::generic::PairStrData           *pairStrData;
    torc::generic::InterfaceAttributes   *interfaceAttributes;
    torc::generic::LogicValueAttributes  *logicValueAttributes;
    torc::generic::LogicListData         *logicListData;
    torc::generic::LogicRefData          *logicRefData;
    torc::generic::NetRefData            *netRefData;


/* Line 33 of lalr1.cc  */
#line 103 "Parser.h"
    };
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;
    /// Tokens.
    struct token
    {
      /* Tokens.  */
   enum yytokentype {
     END = 0,
     EOL = 258,
     IDENTIFIER = 259,
     INTEGER = 260,
     DOUBLE = 261,
     STRING = 262,
     ANGLE = 263,
     BEHAVIOR = 264,
     CALCULATED = 265,
     CAPACITANCE = 266,
     CENTERCENTER = 267,
     CENTERLEFT = 268,
     CENTERRIGHT = 269,
     CHARGE = 270,
     CONDUCTANCE = 271,
     CURRENT = 272,
     DISTANCE = 273,
     DOCUMENT = 274,
     ENERGY = 275,
     EXTEND = 276,
     FLUX = 277,
     FREQUENCY = 278,
     GENERIC = 279,
     GRAPHIC = 280,
     INDUCTANCE = 281,
     INOUT = 282,
     INPUT = 283,
     LOGICMODEL = 284,
     LOWERCENTER = 285,
     LOWERLEFT = 286,
     LOWERRIGHT = 287,
     MASKLAYOUT = 288,
     MASS = 289,
     MEASURED = 290,
     MX = 291,
     MXR90 = 292,
     MY = 293,
     MYR90 = 294,
     NETLIST = 295,
     OUTPUT = 296,
     PCBLAYOUT = 297,
     POWER = 298,
     R0 = 299,
     R180 = 300,
     R270 = 301,
     R90 = 302,
     REQUIRED = 303,
     RESISTANCE = 304,
     RIPPER = 305,
     ROUND = 306,
     SCHEMATIC = 307,
     STRANGER = 308,
     SYMBOLIC = 309,
     TEMPERATURE = 310,
     TIE = 311,
     TIME = 312,
     TRUNCATE = 313,
     UPPERCENTER = 314,
     UPPERLEFT = 315,
     UPPERRIGHT = 316,
     VOLTAGE = 317,
     ACLOAD = 318,
     AFTER = 319,
     ANNOTATE = 320,
     APPLY = 321,
     ARC = 322,
     ARRAY = 323,
     ARRAYMACRO = 324,
     ARRAYRELATEDINFO = 325,
     ARRAYSITE = 326,
     ATLEAST = 327,
     ATMOST = 328,
     AUTHOR = 329,
     BASEARRAY = 330,
     BECOMES = 331,
     BETWEEN = 332,
     BOOLEAN = 333,
     BOOLEANDISPLAY = 334,
     BOOLEANMAP = 335,
     BORDERPATTERN = 336,
     BORDERWIDTH = 337,
     BOUNDINGBOX = 338,
     CELL = 339,
     CELLREF = 340,
     CELLTYPE = 341,
     CHANGE = 342,
     CIRCLE = 343,
     COLOR = 344,
     COMMENT = 345,
     COMMENTGRAPHICS = 346,
     COMPOUND = 347,
     CONNECTLOCATION = 348,
     CONTENTS = 349,
     CORNERTYPE = 350,
     CRITICALITY = 351,
     CURRENTMAP = 352,
     CURVE = 353,
     CYCLE = 354,
     DATAORIGIN = 355,
     DCFANINLOAD = 356,
     DCFANOUTLOAD = 357,
     DCMAXFANIN = 358,
     DCMAXFANOUT = 359,
     DELAY = 360,
     DELTA = 361,
     DERIVATION = 362,
     DESIGN = 363,
     DESIGNATOR = 364,
     DIFFERENCE = 365,
     DIRECTION = 366,
     DISPLAY = 367,
     DOMINATES = 368,
     DOT = 369,
     DURATION = 370,
     E = 371,
     EDIF = 372,
     EDIFLEVEL = 373,
     EDIFVERSION = 374,
     ENCLOSUREDISTANCE = 375,
     ENDTYPE = 376,
     ENTRY = 377,
     EVENT = 378,
     EXACTLY = 379,
     EXTERNAL = 380,
     FABRICATE = 381,
     FALSE = 382,
     FIGURE = 383,
     FIGUREAREA = 384,
     FIGUREGROUP = 385,
     FIGUREGROUPOBJECT = 386,
     FIGUREGROUPOVERRIDE = 387,
     FIGUREGROUPREF = 388,
     FIGUREPERIMETER = 389,
     FIGUREWIDTH = 390,
     FILLPATTERN = 391,
     FOLLOW = 392,
     FORBIDDENEVENT = 393,
     GLOBALPORTREF = 394,
     GREATERTHAN = 395,
     GRIDMAP = 396,
     IGNORE = 397,
     INCLUDEFIGUREGROUP = 398,
     INITIAL_KW = 399,
     INSTANCE = 400,
     INSTANCEBACKANNOTATE = 401,
     INSTANCEGROUP = 402,
     INSTANCEMAP = 403,
     INSTANCEREF = 404,
     INTEGERDISPLAY = 405,
     INTERFACE = 406,
     INTERFIGUREGROUPSPACING = 407,
     INTERSECTION = 408,
     INTRAFIGUREGROUPSPACING = 409,
     INVERSE = 410,
     ISOLATED = 411,
     JOINED = 412,
     JUSTIFY = 413,
     KEYWORDDISPLAY = 414,
     KEYWORDLEVEL = 415,
     KEYWORDMAP = 416,
     LESSTHAN = 417,
     LIBRARY = 418,
     LIBRARYREF = 419,
     LISTOFNETS = 420,
     LISTOFPORTS = 421,
     LOADDELAY = 422,
     LOGICASSIGN = 423,
     LOGICINPUT = 424,
     LOGICLIST = 425,
     LOGICMAPINPUT = 426,
     LOGICMAPOUTPUT = 427,
     LOGICONEOF = 428,
     LOGICOUTPUT = 429,
     LOGICPORT = 430,
     LOGICREF = 431,
     LOGICVALUE = 432,
     LOGICWAVEFORM = 433,
     MAINTAIN = 434,
     MATCH = 435,
     MEMBER = 436,
     MINOMAX = 437,
     MINOMAXDISPLAY = 438,
     MNM = 439,
     MULTIPLEVALUESET = 440,
     MUSTJOIN = 441,
     NAME = 442,
     NET = 443,
     NETBACKANNOTATE = 444,
     NETBUNDLE = 445,
     NETDELAY = 446,
     NETGROUP = 447,
     NETMAP = 448,
     NETREF = 449,
     NOCHANGE = 450,
     NONPERMUTABLE = 451,
     NOTALLOWED = 452,
     NOTCHSPACING = 453,
     NUMBER = 454,
     NUMBERDEFINITION = 455,
     NUMBERDISPLAY = 456,
     OFFPAGECONNECTOR = 457,
     OFFSETEVENT = 458,
     OPENSHAPE = 459,
     ORIENTATION = 460,
     ORIGIN = 461,
     OVERHANGDISTANCE = 462,
     OVERLAPDISTANCE = 463,
     OVERSIZE = 464,
     OWNER = 465,
     PAGE = 466,
     PAGESIZE_EDIF = 467,
     PARAMETER = 468,
     PARAMETERASSIGN = 469,
     PARAMETERDISPLAY = 470,
     PATH = 471,
     PATHDELAY = 472,
     PATHWIDTH = 473,
     PERMUTABLE = 474,
     PHYSICALDESIGNRULE = 475,
     PLUG = 476,
     POINT = 477,
     POINTDISPLAY = 478,
     POINTLIST = 479,
     POLYGON = 480,
     PORT = 481,
     PORTBACKANNOTATE = 482,
     PORTBUNDLE = 483,
     PORTDELAY = 484,
     PORTGROUP = 485,
     PORTIMPLEMENTATION = 486,
     PORTINSTANCE = 487,
     PORTLIST = 488,
     PORTLISTALIAS = 489,
     PORTMAP = 490,
     PORTREF = 491,
     PROGRAM = 492,
     PROPERTY = 493,
     PROPERTYDISPLAY = 494,
     PROTECTIONFRAME = 495,
     PT = 496,
     RANGEVECTOR = 497,
     RECTANGLE = 498,
     RECTANGLESIZE = 499,
     RENAME = 500,
     RESOLVES = 501,
     SCALE = 502,
     SCALEX = 503,
     SCALEY = 504,
     SECTION = 505,
     SHAPE = 506,
     SIMULATE = 507,
     SIMULATIONINFO = 508,
     SINGLEVALUESET = 509,
     SITE = 510,
     SOCKET = 511,
     SOCKETSET = 512,
     STATUS = 513,
     STEADY = 514,
     STRINGDISPLAY = 515,
     STRONG = 516,
     SYMBOL = 517,
     SYMMETRY = 518,
     TABLE = 519,
     TABLEDEFAULT = 520,
     TECHNOLOGY = 521,
     TEXTHEIGHT = 522,
     TIMEINTERVAL = 523,
     TIMESTAMP = 524,
     TIMING = 525,
     TRANSFORM = 526,
     TRANSITION = 527,
     TRIGGER = 528,
     TRUE = 529,
     UNCONSTRAINED = 530,
     UNDEFINED = 531,
     UNION = 532,
     UNIT = 533,
     UNUSED = 534,
     USERDATA = 535,
     VERSION = 536,
     VIEW = 537,
     VIEWLIST = 538,
     VIEWMAP = 539,
     VIEWREF = 540,
     VIEWTYPE = 541,
     VISIBLE = 542,
     VOLTAGEMAP = 543,
     WAVEVALUE = 544,
     WEAK = 545,
     WEAKJOINED = 546,
     WHEN = 547,
     WRITTEN = 548,
     LBKT = 549,
     RBKT = 550
   };

    };
    /// Token type.
    typedef token::yytokentype token_type;

    /// Build a parser object.
    Parser (class Driver& inDriver_yyarg);
    virtual ~Parser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

  private:
    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Generate an error message.
    /// \param state   the state where the error occurred.
    /// \param tok     the lookahead token.
    virtual std::string yysyntax_error_ (int yystate, int tok);

#if YYDEBUG
    /// \brief Report a symbol value on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_value_print_ (int yytype,
					 const semantic_type* yyvaluep,
					 const location_type* yylocationp);
    /// \brief Report a symbol on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_print_ (int yytype,
				   const semantic_type* yyvaluep,
				   const location_type* yylocationp);
#endif


    /// State numbers.
    typedef int state_type;
    /// State stack type.
    typedef stack<state_type>    state_stack_type;
    /// Semantic value stack type.
    typedef stack<semantic_type> semantic_stack_type;
    /// location stack type.
    typedef stack<location_type> location_stack_type;

    /// The state stack.
    state_stack_type yystate_stack_;
    /// The semantic value stack.
    semantic_stack_type yysemantic_stack_;
    /// The location stack.
    location_stack_type yylocation_stack_;

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    /// Internal symbol numbers.
    typedef unsigned short int token_number_type;
    /* Tables.  */
    /// For a state, the index in \a yytable_ of its portion.
    static const short int yypact_[];
    static const short int yypact_ninf_;

    /// For a state, default reduction number.
    /// Unless\a  yytable_ specifies something else to do.
    /// Zero means the default is an error.
    static const unsigned short int yydefact_[];

    static const short int yypgoto_[];
    static const short int yydefgoto_[];

    /// What to do in a state.
    /// \a yytable_[yypact_[s]]: what to do in state \a s.
    /// - if positive, shift that token.
    /// - if negative, reduce the rule which number is the opposite.
    /// - if zero, do what YYDEFACT says.
    static const unsigned short int yytable_[];
    static const signed char yytable_ninf_;

    static const short int yycheck_[];

    /// For a state, its accessing symbol.
    static const unsigned short int yystos_[];

    /// For a rule, its LHS.
    static const unsigned short int yyr1_[];
    /// For a rule, its RHS length.
    static const unsigned char yyr2_[]; 

    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
    /// A type to store symbol numbers and -1.
    typedef short int rhs_number_type;
    /// A `-1'-separated list of the rules' RHS.
    static const rhs_number_type yyrhs_[];
    /// For each rule, the index of the first RHS symbol in \a yyrhs_.
    static const unsigned short int yyprhs_[];
    /// For each rule, its source line number.
    static const unsigned short int yyrline_[];
    /// For each scanner token number, its symbol number.
    static const unsigned short int yytoken_number_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /* Debugging.  */
    int yydebug_;
    std::ostream* yycdebug_;
#endif

    /// Convert a scanner token number \a t to a symbol number.
    token_number_type yytranslate_ (int t);

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg        Why this token is reclaimed.
    ///                     If null, do not display the symbol, just free it.
    /// \param yytype       The symbol type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    inline void yydestruct_ (const char* yymsg,
			     int yytype,
			     semantic_type* yyvaluep,
			     location_type* yylocationp);

    /// Pop \a n symbols the three stacks.
    inline void yypop_ (unsigned int n = 1);

    /* Constants.  */
    static const int yyeof_;
    /* LAST_ -- Last index in TABLE_.  */
    static const int yylast_;
    static const int yynnts_;
    static const int yyempty_;
    static const int yyfinal_;
    static const int yyterror_;
    static const int yyerrcode_;
    static const int yyntokens_;
    static const unsigned int yyuser_token_number_max_;
    static const token_number_type yyundef_token_;

    /* User arguments.  */
    class Driver& inDriver;
  };
/* Line 33 of lalr1.cc  */
#line 59 "Parser.yy"
} } // torc::generic
/* Line 33 of lalr1.cc  */
#line 587 "Parser.h"



#endif /* !YY_YY_PARSER_H_INCLUDED  */
