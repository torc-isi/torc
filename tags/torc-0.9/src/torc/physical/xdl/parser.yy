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

%{


// ------------------------------------------------------------------------------------------------
// -------------------------------------- C/C++ declarations --------------------------------------
// ------------------------------------------------------------------------------------------------


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


#include "torc/physical/XdlImporter.hpp"
#include "torc/physical/Factory.hpp"
#include "torc/physical/OutputStreamHelpers.hpp"
#include "torc/common/DeviceDesignator.hpp"
#include <sstream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/smart_ptr.hpp>

//#define YYPRINT(file, type, value) fprintf(file, "token [%d] \"%s\"", type, value);

using std::string;
using namespace torc::physical;

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

/// \brief Grammar start symbol is named "start".
%start start

/// \brief Generate a header file that contains the token definitions
%defines

/// \brief Use the newer C++ skeleton file.
%skeleton "lalr1.cc"

/// \brief Namespace for the parser.
/// \details Undue grief resulted from trying to pull the parser and scanner into torc::physical.
%name-prefix="torc"

/// \brief The name of the parser class is XdlParser
%define "parser_class_name" "XdlParser"

/// \brief Track current position within the input.
%locations

/// \brief Initialize the location object
%initial-action
{
    // initialize the initial location object
    @$.begin.filename = @$.end.filename = &xdl.mStreamName;
};

/// \brief Pass the importer by reference to the parser and scanner.
/// \details This provides a simple but effective pure interface, requiring no global variables.
%parse-param { class XdlImporter& xdl }

/// \brief Request verbose error messages.
%error-verbose


// ------------------------------------------------------------------------------------------------
// ----------------------------------- Begin XDL grammar tokens -----------------------------------
// ------------------------------------------------------------------------------------------------

%token END	     0	"end of file"
%token EOL		"end of line"

%token DESIGN
%token MODULE
%token ENDMODULE
%token PORT
%token INST
%token NET
%token CFG
%token PLACED
%token UNPLACED
%token BONDED
%token UNBONDED
%token POWER
%token GROUND
%token INPIN
%token OUTPIN
%token PIP
%token ROUTETHROUGH
%token STRING
%token CFGSETTING
%token CFGNAME
%token CFGVALUE
%token IDENTIFIER
%token SPEEDGRADE
%token XDLVERSION
%token BIDIRUNBUF
%token BIDIRUNIBUF
%token BIDIRBIBUF
%token UNIDIRBUF

%token WORD
%token CHAR
%token QUOTE
%token OBRACE
%token EBRACE

// ------------------------------------------------------------------------------------------------
// ------------------------------------ End XDL grammar tokens ------------------------------------
// ------------------------------------------------------------------------------------------------

%{

/// \brief Code to connect the bison parser in the importer to the flex scanner object.
/// \details This defines the yylex() function call to pull the next token from the current lexer 
///		object in the importer.
#undef yylex
#define yylex xdl.lexer->lex

%}


%%


// ------------------------------------------------------------------------------------------------
// ------------------------------------ Begin XDL grammar rules -----------------------------------
// ------------------------------------------------------------------------------------------------

start
					: design
					;

// ---------------------------------------------- top ---------------------------------------------

design
					: design_statement statements { 
							std::clog << "INFO: Parsed " << xdl.mInstanceCount << " instances, " 
								<< xdl.mNetCount << " nets, " << xdl.mConfigCount << " settings" 
								<< std::endl;
						 }
					;

statements			: /* empty */
					| statements statement
					;

statement
					: module_block
					| instance_statement
					| net_statement
					| error { /*std::clog << "ERROR at " << yylval << std::endl;*/ } ';'
					;

// -------------------------------------------- design --------------------------------------------

//# design <design_name> <part> <ncd version>;             
//# design <design_name> <device> <package> <speed> <ncd_version>

design_statement
					: DESIGN { xdl.mConfigMap.clearConfig(); } design_name device_info version 
						optional_config ';' {
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
					;

design_name
					: double_quoted_string { 
							xdl.mDesignName = xdl.lexer->last_string; 
							xdl.lexer->last_string.clear(); 
						}
					;

device_info
					: part
					| device package speed
					;

part
					: IDENTIFIER speed {
							torc::common::DeviceDesignator designator($1);
							xdl.mDesignDevice = designator.getDeviceName();
							xdl.mDesignPackage = designator.getDevicePackage();
							xdl.mDesignSpeedGrade = $2;
						}
					;

device				: IDENTIFIER { xdl.mDesignDevice = $1; }
					;

package				: IDENTIFIER { xdl.mDesignPackage = $1; }
					;

speed				: SPEEDGRADE { xdl.mDesignSpeedGrade = $1; }
					;

version
					: /* empty */
					| XDLVERSION { xdl.mDesignXdlVersion = $1; }
					;


// -------------------------------------------- modules -------------------------------------------

//# module <name> <inst_name> ;
//#   port <name> <inst_name> <inst_pin> ;
//#   ...
//#   ...
//#   instance ... ;
//#   ...
//#   ...
//#   net ... ;
//#   ...
//#   ...
//# endmodule <name> ;

module_block		: module_statement module_elements endmodule_statement
					;

module_elements
					: /* empty */
					| module_elements module_element
					;

module_element
					: port_statement
					| instance_statement
					| net_statement
					;

module_statement
					: MODULE { xdl.mConfigMap.clearConfig(); xdl.mPortTempVector.clear(); }
						double_quoted_string { xdl.mModuleName = xdl.lexer->last_string; }
						double_quoted_string { xdl.mModuleAnchor = xdl.lexer->last_string; }
						optional_config ';' {
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
					;

endmodule_statement
					: ENDMODULE double_quoted_string ';' {
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
					;

// --------------------------------------------- ports --------------------------------------------

port_statement
					: PORT 
						double_quoted_string { xdl.mPortName = xdl.lexer->last_string; }
						double_quoted_string { xdl.mPortInstance = xdl.lexer->last_string; }
						double_quoted_string { xdl.mPortPin = xdl.lexer->last_string; } ';' {
							xdl.mPortTempVector.push_back(PortTemp(xdl.mPortName, xdl.mPortInstance, 
								xdl.mPortPin));
						}
					;

// ------------------------------------------- instances ------------------------------------------

//# instance <name> <sitedef>, placed <tile> <site>, cfg <string> ;
//# instance <name> <sitedef>, placed <tile> <site>, module "instantiation_name" "module_name" 
//		"instance_name", cfg <string> ;
//# instance <name> <sitedef>, unplaced, cfg <string> ;

instance_statement
					: INST { xdl.mConfigMap.clearConfig(); xdl.mInstanceReferencePtr.reset(); } 
						double_quoted_string { xdl.mInstanceName = xdl.lexer->last_string; } 
						double_quoted_string { xdl.mInstanceType = xdl.lexer->last_string; } ',' 
						instance_placement 
						reference_and_config
						';' {
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
					;

instance_placement
					: UNPLACED instance_bonding { 
							xdl.mInstanceSite.clear(); xdl.mInstanceTile.clear(); 
							xdl.mInstanceBonding = eInstanceBondingUnknown; 
						}
					| PLACED IDENTIFIER IDENTIFIER instance_bonding { 
							xdl.mInstanceTile = $2; xdl.mInstanceSite = $3; 
							if(false) std::clog << "INFO:     " << xdl.mInstanceTile << " " << 
								xdl.mInstanceSite << " " << xdl.mInstanceBonding << std::endl;
						}
					;

instance_bonding
					: /* empty */ { xdl.mInstanceBonding = eInstanceBondingUnknown; }
					| BONDED { xdl.mInstanceBonding = eInstanceBondingBonded; }
					| UNBONDED { xdl.mInstanceBonding = eInstanceBondingUnbonded; }
					;

reference_and_config
					: /* empty */
					| ',' config
					| ',' module_reference
					| ',' module_reference ',' config
					;

module_reference
					: MODULE { xdl.mReferenceModulePtr.reset(); xdl.mReferenceInstancePtr.reset(); }
						double_quoted_string { xdl.mReferenceInstantiation 
							= xdl.lexer->last_string; } 
						double_quoted_string { 
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
						double_quoted_string { 
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
					;

// --------------------------------------------- nets ---------------------------------------------

//# net <name> <type>,
//#	  outpin <inst_name> <inst_pin>,
//#	  ...
//#	  ...
//#	  inpin <inst_name> <inst_pin>,
//#	  ...
//#	  ...
//#	  pip <tile> <wire0> <dir> <wire1> , # [<rt>]
//#	  ...
//#	  ...
//#	  ;

net_statement
					: NET { xdl.mConfigMap.clearConfig(); } 
						double_quoted_string { xdl.mNetName = xdl.lexer->last_string; } 
						net_power ',' {
							xdl.mNetCount++;
							// create the net
							xdl.mNetPtr = Factory::newNetPtr(xdl.mNetName, xdl.mNetType);
						} net_pins_or_pips_or_cfg ';' {
							// add the net to the current circuit
							CircuitSharedPtr circuitPtr = xdl.mCircuitPtr.lock();
							circuitPtr->addNet(xdl.mNetPtr);
							// add the net configs
							xdl.mNetPtr->addConfigs(xdl.mConfigMap);
							if(false) std::clog << "INFO: net " << xdl.mNetName << std::endl;
						}
					;

net_power
					: /* empty */ { xdl.mNetType = eNetTypeNormal; }
					| POWER { xdl.mNetType = eNetTypePower; }
					| GROUND { xdl.mNetType = eNetTypeGround; }
					;

net_pins_or_pips_or_cfg
					: /* empty */
					| net_pins_or_pips_or_cfg net_pin_or_pip_or_cfg
					;

net_pin_or_pip_or_cfg
					: net_pin ','
					| net_pin
					| net_pip ',' routethrough
					| net_pip routethrough
					| net_cfg ','
					| net_cfg
					;

net_cfg
					: config
					;

// ---------------------------------------------- pins --------------------------------------------

net_pin
					: net_pin_direction double_quoted_string IDENTIFIER {
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
					;

net_pin_direction
					: INPIN { xdl.mPinDirection = ePinDirectionInpin; }
					| OUTPIN { xdl.mPinDirection = ePinDirectionOutpin; }
					;

// ---------------------------------------------- pips --------------------------------------------

net_pip
					: PIP IDENTIFIER { xdl.mPipTile = yylval; } 
						IDENTIFIER { xdl.mPipSource = yylval; } 
						net_pip_direction 
						IDENTIFIER { xdl.mPipSink = yylval; }
					;

net_pip_direction
					: BIDIRUNBUF // ==   bidirectional, unbuffered
						{ xdl.mPipDirection = ePipBidirectionalUnbuffered; }
					| BIDIRUNIBUF // =>   bidirectional, buffered in one direction
						{ xdl.mPipDirection = ePipBidirectionalUnidirectionallyBuffered; }
					| BIDIRBIBUF // =-   bidirectional, buffered in both directions
						{ xdl.mPipDirection = ePipBidirectionalBidirectionallyBuffered; }
					| UNIDIRBUF // ->   directional, buffered
						{ xdl.mPipDirection = ePipUnidirectionalBuffered; }
					;

routethrough
					: /* empty */ {
							Pip pip = Factory::newPip(xdl.mPipTile, xdl.mPipSource, xdl.mPipSink, 
								xdl.mPipDirection);
							xdl.mNetPtr->addPip(pip);
							xdl.bind(pip, XdlImporter::ePipTypeRegular);
						}
					| ROUTETHROUGH { xdl.mRoutethroughConfigSetting = yylval; } ':' 
						IDENTIFIER { xdl.mRoutethroughConfigName = yylval; } ':' 
						IDENTIFIER { xdl.mRoutethroughConfigValue = yylval; } 
						double_quoted_string { xdl.mRoutethroughInstance = xdl.lexer->last_string; } 
						IDENTIFIER { xdl.mRoutethroughSource = yylval; } net_pip_direction 
						IDENTIFIER {
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
					;

// -------------------------------------------- configs -------------------------------------------

optional_config
					: /* empty */
					| ',' config
					;

config
					: CFG { xdl.mConfigMap.clearConfig(); } '"' config_settings '"'
					;

config_settings
					: /* empty */
					| config_settings config_setting
					;

config_setting
					: config_setting ':' config_name ':' config_value {
							if(xdl.mConfigValue.compare("#OFF")) {
								xdl.mConfigMap.setConfig(xdl.mConfigSetting, xdl.mConfigName, 
									xdl.mConfigValue);
								if(false) std::clog << "INFO:     " << xdl.mConfigSetting << ":" 
									<< xdl.mConfigName << ":" << xdl.mConfigValue << std::endl;
								xdl.mConfigCount++;
							}
						}
					;

config_setting
					: CFGSETTING { xdl.mConfigSetting = $1; }
					;

config_name
					: /* empty */ { xdl.mConfigName.clear(); }
					| CFGNAME { xdl.mConfigName = $1; }
					;

config_value
					: /* empty */ { xdl.mConfigValue.clear(); }
					| CFGVALUE { xdl.mConfigValue = $1; }
					;

// -------------------------------------------- strings -------------------------------------------

double_quoted_string
					: '"' string '"' { 
							if(false) std::clog << "STRING: \"" << xdl.lexer->last_string << "\"" 
							<< std::endl; 
						}
					;

string
					: STRING
					| string STRING
					;


// ------------------------------------------------------------------------------------------------
// ------------------------------------- End XDL grammar rules ------------------------------------
// ------------------------------------------------------------------------------------------------


%%


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
