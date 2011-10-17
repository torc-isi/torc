/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++

   Copyright (C) 2002, 2003, 2004, 2005, 2006 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

// Take the name prefix into account.
#define yylex   torclex

#include "y.tab.h"

/* User implementation prologue.  */


/* Line 317 of lalr1.cc.  */
#line 45 "XdlParser.cpp"

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG							\
  for (bool yydebugcond_ = yydebug_; yydebugcond_; yydebugcond_ = false)	\
    (*yycdebug_)

/* Enable debugging if requested.  */
#if YYDEBUG

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab

namespace torc
{
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  XdlParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  XdlParser::XdlParser (class XdlImporter& xdl_yyarg)
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
      xdl (xdl_yyarg)
  {
  }

  XdlParser::~XdlParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  XdlParser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  XdlParser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif /* ! YYDEBUG */

  void
  XdlParser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  XdlParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

  std::ostream&
  XdlParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  XdlParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  XdlParser::debug_level_type
  XdlParser::debug_level () const
  {
    return yydebug_;
  }

  void
  XdlParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }


  int
  XdlParser::parse ()
  {
    /// Look-ahead and look-ahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the look-ahead.
    semantic_type yylval;
    /// Location of the look-ahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    #line 93 "/project/dev/torc/trunk/src/torc/physical/xdl/parser.yy"
{
    // initialize the initial location object
    yylloc.begin.filename = yylloc.end.filename = &xdl.mStreamName;
}
  /* Line 555 of yacc.c.  */
#line 282 "XdlParser.cpp"
    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;
    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without look-ahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a look-ahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Accept?  */
    if (yyn == yyfinal_)
      goto yyacceptlab;

    /* Shift the look-ahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted unless it is eof.  */
    if (yychar != yyeof_)
      yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 3:
#line 177 "parser.yy"
    { 
							std::clog << "INFO: Parsed " << xdl.mInstanceCount << " instances, " 
								<< xdl.mNetCount << " nets, " << xdl.mConfigCount << " settings" 
								<< std::endl;
						 }
    break;

  case 9:
#line 192 "parser.yy"
    { /*std::clog << "ERROR at " << yylval << std::endl;*/ }
    break;

  case 11:
#line 201 "parser.yy"
    { xdl.mConfigMap.clearConfig(); }
    break;

  case 12:
#line 202 "parser.yy"
    {
							if(true) std::clog << "INFO: design \"" << xdl.mDesignName << "\" " 
								<< xdl.mDesignDevice << xdl.mDesignPackage << xdl.mDesignSpeedGrade 
								<< std::endl;
							// create the design
							xdl.mDesignPtr = Factory::newDesignPtr(xdl.mDesignName, 
								xdl.mDesignDevice, xdl.mDesignPackage, xdl.mDesignSpeedGrade, 
								xdl.mDesignXdlVersion);
							// add the design configs
							xdl.mDesignPtr->addConfigs(xdl.mConfigMap);
							// the design is the de facto circuit for instances and nets encountered
							xdl.mCircuitPtr = xdl.mDesignPtr;
							// construct and initialize the device database if applicable
							xdl.initializeDatabase();
						}
    break;

  case 13:
#line 220 "parser.yy"
    { 
							xdl.mDesignName = xdl.lexer->last_string; 
							xdl.lexer->last_string.clear(); 
						}
    break;

  case 16:
#line 232 "parser.yy"
    {
							torc::common::DeviceDesignator designator((yysemantic_stack_[(2) - (1)]));
							xdl.mDesignDevice = designator.getDeviceName();
							xdl.mDesignPackage = designator.getDevicePackage();
							xdl.mDesignSpeedGrade = (yysemantic_stack_[(2) - (2)]);
						}
    break;

  case 17:
#line 240 "parser.yy"
    { xdl.mDesignDevice = (yysemantic_stack_[(1) - (1)]); }
    break;

  case 18:
#line 243 "parser.yy"
    { xdl.mDesignPackage = (yysemantic_stack_[(1) - (1)]); }
    break;

  case 19:
#line 246 "parser.yy"
    { xdl.mDesignSpeedGrade = (yysemantic_stack_[(1) - (1)]); }
    break;

  case 21:
#line 251 "parser.yy"
    { xdl.mDesignXdlVersion = (yysemantic_stack_[(1) - (1)]); }
    break;

  case 28:
#line 284 "parser.yy"
    { xdl.mConfigMap.clearConfig(); xdl.mPortTempVector.clear(); }
    break;

  case 29:
#line 285 "parser.yy"
    { xdl.mModuleName = xdl.lexer->last_string; }
    break;

  case 30:
#line 286 "parser.yy"
    { xdl.mModuleAnchor = xdl.lexer->last_string; }
    break;

  case 31:
#line 287 "parser.yy"
    {
							xdl.mModuleCount++;
							if(false) std::clog << "INFO: module \"" << xdl.mModuleName 
								<< "\" @ \"" << xdl.mModuleAnchor << std::endl;
							// create the module
							xdl.mModulePtr = Factory::newModulePtr(xdl.mModuleName, 
								xdl.mModuleAnchor);
							// add the instance to the design
							xdl.mDesignPtr->addModule(xdl.mModulePtr);
							xdl.mModulePtr->setParentWeakPtr(xdl.mDesignPtr);
							// add the module configs
							xdl.mModulePtr->addConfigs(xdl.mConfigMap);
							// set this module as the current circuit
							xdl.mCircuitPtr = xdl.mModulePtr;
						}
    break;

  case 32:
#line 305 "parser.yy"
    {
							if(false) std::clog << "INFO: endmodule" << std::endl;
							// iterate through the ports and instantiate them
							PortTempVector::const_iterator p = xdl.mPortTempVector.begin();
							PortTempVector::const_iterator e = xdl.mPortTempVector.end();
							while(p < e) {
								// get the port fields
								const string& name = p->getName();
								const string& instance = p->getInstance();
								const string& pin = p->getPin();
								// try to look up the instance in this module
								// we should normally be more careful with lock(), but who else is 
								// going to discard our pointer targets if the parser hasn't even 
								// exposed the design to the world yet?
								Circuit::InstanceSharedPtrIterator instanceIterator 
									= xdl.mCircuitPtr.lock()->findInstance(instance);
								Circuit::InstanceSharedPtrIterator end = 
									xdl.mCircuitPtr.lock()->instancesEnd();
								// catch references to undeclared instances
								if(instanceIterator == end) {
									std::stringstream ss;
									ss << "Error: unknown instance \"" << xdl.mInstanceName 
										<< "\" for port \"" << xdl.mPortName << "\" in module \"" 
										<< xdl.mModuleName << "\".";
									xdl.error(ss.str()); //YYERROR;
									xdl.failure();
								} else {
									if(false) std::clog << "INFO: port \"" << name << "\" " 
										<< instance << "." << pin << std::endl;
									PortSharedPtr portPtr = Factory::newPortPtr(name, 
										*instanceIterator, pin);
									xdl.mModulePtr->addPort(portPtr);
								}
								// proceed to the next port
								p++;
							}
							// the design is the de facto circuit for instances and nets 
							// encountered outside of this module
							xdl.mCircuitPtr = xdl.mDesignPtr;
						}
    break;

  case 33:
#line 351 "parser.yy"
    { xdl.mPortName = xdl.lexer->last_string; }
    break;

  case 34:
#line 352 "parser.yy"
    { xdl.mPortInstance = xdl.lexer->last_string; }
    break;

  case 35:
#line 353 "parser.yy"
    { xdl.mPortPin = xdl.lexer->last_string; }
    break;

  case 36:
#line 353 "parser.yy"
    {
							xdl.mPortTempVector.push_back(PortTemp(xdl.mPortName, xdl.mPortInstance, 
								xdl.mPortPin));
						}
    break;

  case 37:
#line 367 "parser.yy"
    { xdl.mConfigMap.clearConfig(); xdl.mInstanceReferencePtr.reset(); }
    break;

  case 38:
#line 368 "parser.yy"
    { xdl.mInstanceName = xdl.lexer->last_string; }
    break;

  case 39:
#line 369 "parser.yy"
    { xdl.mInstanceType = xdl.lexer->last_string; }
    break;

  case 40:
#line 372 "parser.yy"
    {
							xdl.mInstanceCount++;
							if(false) std::clog << "INFO: inst " << xdl.mInstanceType << " \"" 
								<< xdl.mInstanceName << "\"" << " @ " << xdl.mInstanceSite << " (" 
								<< xdl.mInstanceTile << ")" << std::endl;
							// create the instance
							xdl.mInstancePtr = Factory::newInstancePtr(xdl.mInstanceName, 
								xdl.mInstanceType, xdl.mInstanceTile, xdl.mInstanceSite, 
								xdl.mInstanceBonding, xdl.mInstanceReferencePtr);
							// add the instance to the current circuit
							xdl.mCircuitPtr.lock()->addInstance(xdl.mInstancePtr);
							xdl.mInstancePtr->setParentWeakPtr(xdl.mCircuitPtr);
							// add the instance configs
							xdl.mInstancePtr->addConfigs(xdl.mConfigMap);
						}
    break;

  case 41:
#line 390 "parser.yy"
    { 
							xdl.mInstanceSite.clear(); xdl.mInstanceTile.clear(); 
							xdl.mInstanceBonding = eInstanceBondingUnknown; 
						}
    break;

  case 42:
#line 394 "parser.yy"
    { 
							xdl.mInstanceTile = (yysemantic_stack_[(4) - (2)]); xdl.mInstanceSite = (yysemantic_stack_[(4) - (3)]); 
							if(false) std::clog << "INFO:     " << xdl.mInstanceTile << " " << 
								xdl.mInstanceSite << " " << xdl.mInstanceBonding << std::endl;
						}
    break;

  case 43:
#line 402 "parser.yy"
    { xdl.mInstanceBonding = eInstanceBondingUnknown; }
    break;

  case 44:
#line 403 "parser.yy"
    { xdl.mInstanceBonding = eInstanceBondingBonded; }
    break;

  case 45:
#line 404 "parser.yy"
    { xdl.mInstanceBonding = eInstanceBondingUnbonded; }
    break;

  case 50:
#line 415 "parser.yy"
    { xdl.mReferenceModulePtr.reset(); xdl.mReferenceInstancePtr.reset(); }
    break;

  case 51:
#line 416 "parser.yy"
    { xdl.mReferenceInstantiation 
							= xdl.lexer->last_string; }
    break;

  case 52:
#line 418 "parser.yy"
    { 
							xdl.mReferenceModule = xdl.lexer->last_string; 
							// look up the module
							Design::ModuleSharedPtrIterator moduleIterator
								= xdl.mDesignPtr->findModule(xdl.mReferenceModule);
							Design::ModuleSharedPtrIterator end = xdl.mDesignPtr->modulesEnd();
							if(moduleIterator == end) {
								std::stringstream ss;
								ss << "Error: unknown module \"" << xdl.mReferenceModule 
									<< "\" referenced by instance \"" << xdl.mInstanceName << "\".";
								xdl.error(ss.str()); YYERROR;
							}
							xdl.mReferenceModulePtr = *moduleIterator;
						}
    break;

  case 53:
#line 432 "parser.yy"
    { 
							xdl.mReferenceInstance = xdl.lexer->last_string;
							// look up the instance
							Circuit::InstanceSharedPtrIterator instanceIterator
								= xdl.mReferenceModulePtr->findInstance(xdl.mReferenceInstance);
							Circuit::InstanceSharedPtrIterator end 
								= xdl.mReferenceModulePtr->instancesEnd();
							if(instanceIterator == end) {
								std::stringstream ss;
								ss << "Error: unknown instance \"" << xdl.mReferenceInstance 
									<< "\" in module \"" << xdl.mReferenceModule 
									<< "\" referenced by instance \"" << xdl.mInstanceName << "\".";
								xdl.error(ss.str()); YYERROR;
							}
							xdl.mReferenceInstancePtr = *instanceIterator;
							// create an instance reference
							InstanceReferenceSharedPtr instanceReferencePtr 
								= Factory::newInstanceReferencePtr(xdl.mReferenceInstantiation, 
								xdl.mReferenceModulePtr, xdl.mReferenceInstancePtr);
							xdl.mInstanceReferencePtr = instanceReferencePtr;
						}
    break;

  case 54:
#line 470 "parser.yy"
    { xdl.mConfigMap.clearConfig(); }
    break;

  case 55:
#line 471 "parser.yy"
    { xdl.mNetName = xdl.lexer->last_string; }
    break;

  case 56:
#line 472 "parser.yy"
    {
							xdl.mNetCount++;
							// create the net
							xdl.mNetPtr = Factory::newNetPtr(xdl.mNetName, xdl.mNetType);
						}
    break;

  case 57:
#line 476 "parser.yy"
    {
							// add the net to the current circuit
							CircuitSharedPtr circuitPtr = xdl.mCircuitPtr.lock();
							circuitPtr->addNet(xdl.mNetPtr);
							// add the net configs
							xdl.mNetPtr->addConfigs(xdl.mConfigMap);
							if(false) std::clog << "INFO: net " << xdl.mNetName << std::endl;
						}
    break;

  case 58:
#line 487 "parser.yy"
    { xdl.mNetType = eNetTypeNormal; }
    break;

  case 59:
#line 488 "parser.yy"
    { xdl.mNetType = eNetTypePower; }
    break;

  case 60:
#line 489 "parser.yy"
    { xdl.mNetType = eNetTypeGround; }
    break;

  case 70:
#line 513 "parser.yy"
    {
							xdl.mInstanceName = xdl.lexer->last_string;
							xdl.mPinName = yylval;
							CircuitSharedPtr circuitPtr = xdl.mCircuitPtr.lock();
							Circuit::InstanceSharedPtrIterator instanceIterator 
								= circuitPtr->findInstance(xdl.mInstanceName);
							Circuit::InstanceSharedPtrIterator end = circuitPtr->instancesEnd();
							// catch references to undeclared instances
							if(instanceIterator == end) {
								std::stringstream ss;
								ss << "Error: unknown instance \"" << xdl.mInstanceName 
									<< "\" for pin \"" << xdl.mInstanceName << "." 
									<< xdl.mPinName << "\".";
								xdl.error(ss.str()); YYERROR;
							}
							// proceed with the instance pin creation
							InstanceSharedPtr instancePtr = *instanceIterator;
							InstancePinSharedPtr instancePinPtr 
								= Factory::newInstancePinPtr(instancePtr, xdl.mPinName);
							xdl.bind(instancePinPtr);
							switch(xdl.mPinDirection) {
							case ePinDirectionInpin: 
								xdl.mNetPtr->addSink(instancePinPtr); 
								break;
							case ePinDirectionOutpin: 
								xdl.mNetPtr->addSource(instancePinPtr); 
								break;
							default: 
								break;
							}
						}
    break;

  case 71:
#line 547 "parser.yy"
    { xdl.mPinDirection = ePinDirectionInpin; }
    break;

  case 72:
#line 548 "parser.yy"
    { xdl.mPinDirection = ePinDirectionOutpin; }
    break;

  case 73:
#line 554 "parser.yy"
    { xdl.mPipTile = yylval; }
    break;

  case 74:
#line 555 "parser.yy"
    { xdl.mPipSource = yylval; }
    break;

  case 75:
#line 557 "parser.yy"
    { xdl.mPipSink = yylval; }
    break;

  case 76:
#line 562 "parser.yy"
    { xdl.mPipDirection = ePipBidirectionalUnbuffered; }
    break;

  case 77:
#line 564 "parser.yy"
    { xdl.mPipDirection = ePipBidirectionalUnidirectionallyBuffered; }
    break;

  case 78:
#line 566 "parser.yy"
    { xdl.mPipDirection = ePipBidirectionalBidirectionallyBuffered; }
    break;

  case 79:
#line 568 "parser.yy"
    { xdl.mPipDirection = ePipUnidirectionalBuffered; }
    break;

  case 80:
#line 572 "parser.yy"
    {
							Pip pip = Factory::newPip(xdl.mPipTile, xdl.mPipSource, xdl.mPipSink, 
								xdl.mPipDirection);
							xdl.mNetPtr->addPip(pip);
							xdl.bind(pip, XdlImporter::ePipTypeRegular);
						}
    break;

  case 81:
#line 578 "parser.yy"
    { xdl.mRoutethroughConfigSetting = yylval; }
    break;

  case 82:
#line 579 "parser.yy"
    { xdl.mRoutethroughConfigName = yylval; }
    break;

  case 83:
#line 580 "parser.yy"
    { xdl.mRoutethroughConfigValue = yylval; }
    break;

  case 84:
#line 581 "parser.yy"
    { xdl.mRoutethroughInstance = xdl.lexer->last_string; }
    break;

  case 85:
#line 582 "parser.yy"
    { xdl.mRoutethroughSource = yylval; }
    break;

  case 86:
#line 583 "parser.yy"
    {
							xdl.mRoutethroughSink = yylval;
							// look up the routethrough instance
							CircuitSharedPtr circuitPtr = xdl.mCircuitPtr.lock();
							Circuit::InstanceSharedPtrConstIterator instanceIterator 
								= circuitPtr->findInstance(xdl.mRoutethroughInstance);
							Circuit::InstanceSharedPtrConstIterator end 
								= circuitPtr->instancesEnd();
							// catch references to undeclared instances
							if(instanceIterator == end) {
								PipVector::const_iterator pipIter = --(xdl.mNetPtr->pipsEnd());
								std::stringstream ss;
								ss << "Error: unknown routhethrough instance \"" 
									<< xdl.mRoutethroughInstance << "\" for pip \"" << *pipIter 
									<< "\".";
								xdl.error(ss.str()); YYERROR;
							}
							// proceed with the routethrough creation
							const InstanceWeakPtr instancePtr = *instanceIterator;
							// create a routethrough
							RoutethroughSharedPtr routethroughPtr 
								= Factory::newRoutethroughPtr(xdl.mRoutethroughConfigSetting, 
								xdl.mRoutethroughConfigName, xdl.mRoutethroughConfigValue, 
								instancePtr, xdl.mRoutethroughSource, xdl.mRoutethroughSink);
							Pip pip = Factory::newPip(xdl.mPipTile, xdl.mPipSource, xdl.mPipSink, 
								xdl.mPipDirection, routethroughPtr);
							xdl.mNetPtr->addPip(pip);
							xdl.bind(pip, XdlImporter::ePipTypeRoutethrough);
						}
    break;

  case 89:
#line 622 "parser.yy"
    { xdl.mConfigMap.clearConfig(); }
    break;

  case 93:
#line 631 "parser.yy"
    {
							if(xdl.mConfigValue.compare("#OFF")) {
								xdl.mConfigMap.setConfig(xdl.mConfigSetting, xdl.mConfigName, 
									xdl.mConfigValue);
								if(false) std::clog << "INFO:     " << xdl.mConfigSetting << ":" 
									<< xdl.mConfigName << ":" << xdl.mConfigValue << std::endl;
								xdl.mConfigCount++;
							}
						}
    break;

  case 94:
#line 643 "parser.yy"
    { xdl.mConfigSetting = (yysemantic_stack_[(1) - (1)]); }
    break;

  case 95:
#line 647 "parser.yy"
    { xdl.mConfigName.clear(); }
    break;

  case 96:
#line 648 "parser.yy"
    { xdl.mConfigName = (yysemantic_stack_[(1) - (1)]); }
    break;

  case 97:
#line 652 "parser.yy"
    { xdl.mConfigValue.clear(); }
    break;

  case 98:
#line 653 "parser.yy"
    { xdl.mConfigValue = (yysemantic_stack_[(1) - (1)]); }
    break;

  case 99:
#line 659 "parser.yy"
    { 
							if(false) std::clog << "STRING: \"" << xdl.lexer->last_string << "\"" 
							<< std::endl; 
						}
    break;


    /* Line 675 of lalr1.cc.  */
#line 941 "XdlParser.cpp"
	default: break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse look-ahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    if (yyn == yyfinal_)
      goto yyacceptlab;

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the look-ahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		   &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyeof_ && yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  XdlParser::yysyntax_error_ (int yystate, int tok)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char XdlParser::yypact_ninf_ = -81;
  const signed char
  XdlParser::yypact_[] =
  {
         1,   -81,    11,   -81,   -81,   -22,   -81,    29,    15,    -3,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -17,    13,     8,   -81,    16,     5,   -22,   -22,   -22,
      38,   -81,   -81,   -81,   -81,   -81,    10,   -81,    13,   -81,
     -81,   -81,   -81,   -22,   -22,   -81,   -81,   -81,   -81,   -81,
      39,    19,   -81,   -22,   -22,    -7,    20,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,    17,   -81,   -22,    21,    10,
      22,   -81,   -81,   -81,    25,     9,   -81,   -22,   -15,   -81,
      33,    18,    26,    -4,   -81,   -81,   -81,    24,    34,   -81,
     -81,   -81,    14,    28,   -81,   -81,    41,   -81,   -81,    30,
      31,   -22,   -10,   -81,    35,    44,    18,   -81,    32,   -81,
     -81,   -81,   -81,   -81,    46,   -81,    53,   -81,   -81,   -81,
      36,   -81,   -22,    39,    51,   -81,    40,   -81,    54,   -81,
     -81,   -81,    52,   -81,   -81,   -22,    23,   -81,   -81,   -81,
     -81,   -81,   -81,    55,    42,   -22,   -81,    57,   -81,   -81,
     -22,   -81,    58,   -81,    23,    59,   -81
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  XdlParser::yydefact_[] =
  {
         0,    11,     0,     2,     4,     0,     1,     0,     0,     0,
      13,     9,    28,    37,    54,     5,     6,    23,     7,     8,
     100,     0,    17,    20,    14,     0,     0,     0,     0,     0,
       0,   101,    99,    19,    16,    21,    87,    18,     0,    10,
      29,    38,    55,     0,     0,    24,    22,    25,    26,    27,
       0,     0,    15,     0,     0,    58,     0,    33,    89,    88,
      12,    30,    39,    59,    60,     0,    32,     0,     0,    87,
       0,    56,    34,    91,     0,     0,    61,     0,     0,    31,
       0,    43,    46,     0,    35,    94,    90,    92,     0,    44,
      45,    41,     0,     0,    71,    72,     0,    57,    62,    68,
      64,     0,    80,    69,     0,    95,    43,    50,    48,    47,
      40,    73,    67,    63,     0,    81,    80,    66,    36,    96,
       0,    42,     0,     0,     0,    70,     0,    65,    97,    51,
      49,    74,     0,    98,    93,     0,     0,    82,    52,    76,
      77,    78,    79,     0,     0,     0,    75,     0,    53,    83,
       0,    84,     0,    85,     0,     0,    86
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  XdlParser::yypgoto_[] =
  {
       -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,    47,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,    56,   -81,   -81,
     -81,   -81,   -19,   -81,   -81,   -81,   -81,   -81,    60,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -66,   -25,   -81,   -81,   -81,   -81,   -81,    27,   -80,
     -81,   -81,   -81,   -81,   -81,   -27,   -81
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  XdlParser::yydefgoto_[] =
  {
        -1,     2,     3,     7,    15,    26,     4,     5,     9,    23,
      24,    25,    38,    34,    36,    16,    30,    45,    17,    27,
      53,    69,    46,    47,    67,    77,   104,    18,    28,    54,
      70,    82,    91,    93,   108,   122,   135,   145,    19,    29,
      55,    76,    65,    83,    98,    99,   100,   101,   102,   124,
     136,   143,   117,   126,   144,   150,   152,   154,    51,    59,
      68,    78,    87,   120,   134,    10,    21
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char XdlParser::yytable_ninf_ = -4;
  const short int
  XdlParser::yytable_[] =
  {
        40,    41,    42,   103,    31,     1,    58,    85,    63,    64,
     115,     6,   109,    94,    95,    96,    56,    57,     8,   107,
      80,    81,    22,    32,    58,    86,    61,    62,   116,    -3,
      11,    89,    90,    97,    12,    35,    20,    13,    14,    33,
      72,    37,    39,   130,    43,    44,    13,    14,    50,    58,
      84,   139,   140,   141,   142,    71,    60,    66,    88,   106,
      75,    73,    79,   105,    92,   110,   111,   119,   112,   113,
     123,   125,   118,   115,   114,   128,   131,   137,   133,   132,
     146,   147,   149,   153,   156,    52,    48,   121,   155,     0,
      49,   127,     0,     0,     0,   129,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   138,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   148,     0,
       0,     0,     0,   151
  };

  /* YYCHECK.  */
  const short int
  XdlParser::yycheck_[] =
  {
        27,    28,    29,    83,    21,     4,    10,    22,    15,    16,
      20,     0,    92,    17,    18,    19,    43,    44,    40,     5,
      11,    12,    25,    40,    10,    40,    53,    54,    38,     0,
       1,    13,    14,    37,     5,    27,    21,     8,     9,    26,
      67,    25,    37,   123,     6,     7,     8,     9,    38,    10,
      77,    28,    29,    30,    31,    38,    37,    37,    25,    25,
      38,    40,    37,    39,    38,    37,    25,    23,    38,    38,
      38,    25,    37,    20,   101,    39,    25,    25,    24,    39,
      25,    39,    25,    25,    25,    38,    30,   106,   154,    -1,
      30,   116,    -1,    -1,    -1,   122,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,   150
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  XdlParser::yystos_[] =
  {
         0,     4,    42,    43,    47,    48,     0,    44,    40,    49,
     106,     1,     5,     8,     9,    45,    56,    59,    68,    79,
      21,   107,    25,    50,    51,    52,    46,    60,    69,    80,
      57,    21,    40,    26,    54,    27,    55,    25,    53,    37,
     106,   106,   106,     6,     7,    58,    63,    64,    68,    79,
      38,    99,    54,    61,    70,    81,   106,   106,    10,   100,
      37,   106,   106,    15,    16,    83,    37,    65,   101,    62,
      71,    38,   106,    40,    99,    38,    82,    66,   102,    37,
      11,    12,    72,    84,   106,    22,    40,   103,    25,    13,
      14,    73,    38,    74,    17,    18,    19,    37,    85,    86,
      87,    88,    89,   100,    67,    39,    25,     5,    75,   100,
      37,    25,    38,    38,   106,    20,    38,    93,    37,    23,
     104,    73,    76,    38,    90,    25,    94,    93,    39,   106,
     100,    25,    39,    24,   105,    77,    91,    25,   106,    28,
      29,    30,    31,    92,    95,    78,    25,    39,   106,    25,
      96,   106,    97,    25,    98,    92,    25
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  XdlParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,    59,    44,    58,
      34
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  XdlParser::yyr1_[] =
  {
         0,    41,    42,    43,    44,    44,    45,    45,    45,    46,
      45,    48,    47,    49,    50,    50,    51,    52,    53,    54,
      55,    55,    56,    57,    57,    58,    58,    58,    60,    61,
      62,    59,    63,    65,    66,    67,    64,    69,    70,    71,
      68,    72,    72,    73,    73,    73,    74,    74,    74,    74,
      76,    77,    78,    75,    80,    81,    82,    79,    83,    83,
      83,    84,    84,    85,    85,    85,    85,    85,    85,    86,
      87,    88,    88,    90,    91,    89,    92,    92,    92,    92,
      93,    94,    95,    96,    97,    98,    93,    99,    99,   101,
     100,   102,   102,   103,   103,   104,   104,   105,   105,   106,
     107,   107
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  XdlParser::yyr2_[] =
  {
         0,     2,     1,     2,     0,     2,     1,     1,     1,     0,
       3,     0,     7,     1,     1,     3,     2,     1,     1,     1,
       0,     1,     3,     0,     2,     1,     1,     1,     0,     0,
       0,     8,     3,     0,     0,     0,     8,     0,     0,     0,
      10,     2,     4,     0,     1,     1,     0,     2,     2,     4,
       0,     0,     0,     7,     0,     0,     0,     9,     0,     1,
       1,     0,     2,     2,     1,     3,     2,     2,     1,     1,
       3,     1,     1,     0,     0,     7,     1,     1,     1,     1,
       0,     0,     0,     0,     0,     0,    14,     0,     2,     0,
       5,     0,     2,     5,     1,     0,     1,     0,     1,     3,
       1,     2
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const XdlParser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "\"end of line\"", "DESIGN",
  "MODULE", "ENDMODULE", "PORT", "INST", "NET", "CFG", "PLACED",
  "UNPLACED", "BONDED", "UNBONDED", "POWER", "GROUND", "INPIN", "OUTPIN",
  "PIP", "ROUTETHROUGH", "STRING", "CFGSETTING", "CFGNAME", "CFGVALUE",
  "IDENTIFIER", "SPEEDGRADE", "XDLVERSION", "BIDIRUNBUF", "BIDIRUNIBUF",
  "BIDIRBIBUF", "UNIDIRBUF", "WORD", "CHAR", "QUOTE", "OBRACE", "EBRACE",
  "';'", "','", "':'", "'\"'", "$accept", "start", "design", "statements",
  "statement", "@1", "design_statement", "@2", "design_name",
  "device_info", "part", "device", "package", "speed", "version",
  "module_block", "module_elements", "module_element", "module_statement",
  "@3", "@4", "@5", "endmodule_statement", "port_statement", "@6", "@7",
  "@8", "instance_statement", "@9", "@10", "@11", "instance_placement",
  "instance_bonding", "reference_and_config", "module_reference", "@12",
  "@13", "@14", "net_statement", "@15", "@16", "@17", "net_power",
  "net_pins_or_pips_or_cfg", "net_pin_or_pip_or_cfg", "net_cfg", "net_pin",
  "net_pin_direction", "net_pip", "@18", "@19", "net_pip_direction",
  "routethrough", "@20", "@21", "@22", "@23", "@24", "optional_config",
  "config", "@25", "config_settings", "config_setting", "config_name",
  "config_value", "double_quoted_string", "string", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const XdlParser::rhs_number_type
  XdlParser::yyrhs_[] =
  {
        42,     0,    -1,    43,    -1,    47,    44,    -1,    -1,    44,
      45,    -1,    56,    -1,    68,    -1,    79,    -1,    -1,     1,
      46,    37,    -1,    -1,     4,    48,    49,    50,    55,    99,
      37,    -1,   106,    -1,    51,    -1,    52,    53,    54,    -1,
      25,    54,    -1,    25,    -1,    25,    -1,    26,    -1,    -1,
      27,    -1,    59,    57,    63,    -1,    -1,    57,    58,    -1,
      64,    -1,    68,    -1,    79,    -1,    -1,    -1,    -1,     5,
      60,   106,    61,   106,    62,    99,    37,    -1,     6,   106,
      37,    -1,    -1,    -1,    -1,     7,   106,    65,   106,    66,
     106,    67,    37,    -1,    -1,    -1,    -1,     8,    69,   106,
      70,   106,    71,    38,    72,    74,    37,    -1,    12,    73,
      -1,    11,    25,    25,    73,    -1,    -1,    13,    -1,    14,
      -1,    -1,    38,   100,    -1,    38,    75,    -1,    38,    75,
      38,   100,    -1,    -1,    -1,    -1,     5,    76,   106,    77,
     106,    78,   106,    -1,    -1,    -1,    -1,     9,    80,   106,
      81,    83,    38,    82,    84,    37,    -1,    -1,    15,    -1,
      16,    -1,    -1,    84,    85,    -1,    87,    38,    -1,    87,
      -1,    89,    38,    93,    -1,    89,    93,    -1,    86,    38,
      -1,    86,    -1,   100,    -1,    88,   106,    25,    -1,    17,
      -1,    18,    -1,    -1,    -1,    19,    25,    90,    25,    91,
      92,    25,    -1,    28,    -1,    29,    -1,    30,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    94,    39,
      25,    95,    39,    25,    96,   106,    97,    25,    98,    92,
      25,    -1,    -1,    38,   100,    -1,    -1,    10,   101,    40,
     102,    40,    -1,    -1,   102,   103,    -1,   103,    39,   104,
      39,   105,    -1,    22,    -1,    -1,    23,    -1,    -1,    24,
      -1,    40,   107,    40,    -1,    21,    -1,   107,    21,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  XdlParser::yyprhs_[] =
  {
         0,     0,     3,     5,     8,     9,    12,    14,    16,    18,
      19,    23,    24,    32,    34,    36,    40,    43,    45,    47,
      49,    50,    52,    56,    57,    60,    62,    64,    66,    67,
      68,    69,    78,    82,    83,    84,    85,    94,    95,    96,
      97,   108,   111,   116,   117,   119,   121,   122,   125,   128,
     133,   134,   135,   136,   144,   145,   146,   147,   157,   158,
     160,   162,   163,   166,   169,   171,   175,   178,   181,   183,
     185,   189,   191,   193,   194,   195,   203,   205,   207,   209,
     211,   212,   213,   214,   215,   216,   217,   232,   233,   236,
     237,   243,   244,   247,   253,   255,   256,   258,   259,   261,
     265,   267
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  XdlParser::yyrline_[] =
  {
         0,   171,   171,   177,   184,   185,   189,   190,   191,   192,
     192,   201,   201,   220,   227,   228,   232,   240,   243,   246,
     249,   251,   269,   272,   274,   278,   279,   280,   284,   285,
     286,   284,   305,   351,   352,   353,   350,   367,   368,   369,
     367,   390,   394,   402,   403,   404,   407,   409,   410,   411,
     415,   416,   418,   415,   470,   471,   472,   470,   487,   488,
     489,   492,   494,   498,   499,   500,   501,   502,   503,   507,
     513,   547,   548,   554,   555,   554,   561,   563,   565,   567,
     572,   578,   579,   580,   581,   582,   578,   616,   618,   622,
     622,   625,   627,   631,   643,   647,   648,   652,   653,   659,
     666,   667
  };

  // Print the state stack on the debug stream.
  void
  XdlParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  XdlParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "), ";
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  XdlParser::token_number_type
  XdlParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    40,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    38,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    39,    37,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int XdlParser::yyeof_ = 0;
  const int XdlParser::yylast_ = 123;
  const int XdlParser::yynnts_ = 67;
  const int XdlParser::yyempty_ = -2;
  const int XdlParser::yyfinal_ = 6;
  const int XdlParser::yyterror_ = 1;
  const int XdlParser::yyerrcode_ = 256;
  const int XdlParser::yyntokens_ = 41;

  const unsigned int XdlParser::yyuser_token_number_max_ = 291;
  const XdlParser::token_number_type XdlParser::yyundef_token_ = 2;

} // namespace torc

#line 676 "parser.yy"



// ------------------------------------------------------------------------------------------------
// ------------------------------------- Additional parser code -----------------------------------
// ------------------------------------------------------------------------------------------------


void torc::XdlParser::error(const XdlParser::location_type& l, const std::string& m) {
    xdl.error(l, m);
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

