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

#include <algorithm>
#include <cstdio>
#include <cstdarg>
#include <iterator>
#include <string>
#include <vector>
#include "torc/generic/parser/EdifContext.hpp"
#include "torc/generic/parser/ParserHelpers.hpp"
#include "torc/generic/util/Log.hpp"
#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/PortAttributes.hpp"
#include "torc/generic/om/PortRefCreator.hpp"

%}

/*** yacc/bison Declarations ***/

/* Require bison 2.3 or later */
/* %require "2.3" */

/* add debug output code to generated parser. disable this for release
 * versions. */
%debug

/* start symbol is named "Edif" */
%start  Edif

/* write out a header file containing the token defines */
%defines

/* use newer C++ skeleton file */
%skeleton "lalr1.cc"

/* namespace to enclose parser in */
%define "namespace" "torc::generic"
//%name-prefix="torc::generic::"
/* set the parser's class identifier */
%define "parser_class_name" "Parser"

/* keep track of the current position within the input */
%locations
%initial-action
{
    // initialize the initial location object
    @$.begin.filename = @$.end.filename = &inDriver.getStreamName();
};

/* The driver is passed by reference to the parser and to the scanner. This
 * provides a simple but effective pure interface, not relying on global
 * variables. */
%parse-param { class Driver& inDriver }

/* verbose error messages */
%error-verbose

 /*** BEGIN EXAMPLE - Change the example grammar's tokens below ***/

%union {
    int                                 integerVal;
    double                              doubleVal;
    std::string                        *stringVal;
    torc::generic::NameData            *nameData;
    torc::generic::ViewRefData         *viewRefData;
    torc::generic::CellRefData         *cellRefData;
    torc::generic::PortRefData         *portRefData;
    torc::generic::InstanceRefData     *instanceRef;
    torc::generic::PortListData        *portListData;
    torc::generic::InstanceInfo        *instanceInfo;
    std::vector<size_t>                *arrayDimensions;
    torc::generic::VersionData         *edifVersion;
    torc::generic::ValueData           *valueData;
    torc::generic::PairData            *pairData;
    torc::generic::ParamAssignInfo     *paramAssignInfo;
    torc::generic::PortAttributes      *portAttributes;
    torc::generic::PortDelay           *portDelay;
    torc::generic::PortInstData        *portInst;
}

%token            END         0    "end of file"
%token            EOL        "end of line"
%token <stringVal>     IDENTIFIER    "identifier"
%token <integerVal>     INTEGER        "integer"
%token <doubleVal>     DOUBLE        "double"
%token <stringVal>     STRING        "string"

%token          ANGLE
%token          BEHAVIOR
%token          CALCULATED
%token          CAPACITANCE
%token          CENTERCENTER
%token          CENTERLEFT
%token          CENTERRIGHT
%token          CHARGE
%token          CONDUCTANCE
%token          CURRENT
%token          DISTANCE
%token          DOCUMENT
%token          ENERGY
%token          EXTEND
%token          FLUX
%token          FREQUENCY
%token          GENERIC
%token          GRAPHIC
%token          INDUCTANCE
%token          INOUT
%token          INPUT
%token          LOGICMODEL
%token          LOWERCENTER
%token          LOWERLEFT
%token          LOWERRIGHT
%token          MASKLAYOUT
%token          MASS
%token          MEASURED
%token          MX
%token          MXR90
%token          MY
%token          MYR90
%token          NETLIST
%token          OUTPUT
%token          PCBLAYOUT
%token          POWER
%token          R0
%token          R180
%token          R270
%token          R90
%token          REQUIRED
%token          RESISTANCE
%token          RIPPER
%token          ROUND
%token          SCHEMATIC
%token          STRANGER
%token          SYMBOLIC
%token          TEMPERATURE
%token          TIE
%token          TIME
%token          TRUNCATE
%token          UPPERCENTER
%token          UPPERLEFT
%token          UPPERRIGHT
%token          VOLTAGE


%token          ACLOAD
%token          AFTER
%token          ANNOTATE
%token          APPLY
%token          ARC
%token          ARRAY
%token          ARRAYMACRO
%token          ARRAYRELATEDINFO
%token          ARRAYSITE
%token          ATLEAST
%token          ATMOST
%token          AUTHOR
%token          BASEARRAY
%token          BECOMES
%token          BETWEEN
%token          BOOLEAN
%token          BOOLEANDISPLAY
%token          BOOLEANMAP
%token          BORDERPATTERN
%token          BORDERWIDTH
%token          BOUNDINGBOX
%token          CELL
%token          CELLREF
%token          CELLTYPE
%token          CHANGE
%token          CIRCLE
%token          COLOR
%token          COMMENT
%token          COMMENTGRAPHICS
%token          COMPOUND
%token          CONNECTLOCATION
%token          CONTENTS
%token          CORNERTYPE
%token          CRITICALITY
%token          CURRENTMAP
%token          CURVE
%token          CYCLE
%token          DATAORIGIN
%token          DCFANINLOAD
%token          DCFANOUTLOAD
%token          DCMAXFANIN
%token          DCMAXFANOUT
%token          DELAY
%token          DELTA
%token          DERIVATION
%token          DESIGN
%token          DESIGNATOR
%token          DIFFERENCE
%token          DIRECTION
%token          DISPLAY
%token          DOMINATES
%token          DOT
%token          DURATION
%token          E
%token          EDIF
%token          EDIFLEVEL
%token          EDIFVERSION
%token          ENCLOSUREDISTANCE
%token          ENDTYPE
%token          ENTRY
%token          EVENT
%token          EXACTLY
%token          EXTERNAL
%token          FABRICATE
%token          FALSE
%token          FIGURE
%token          FIGUREAREA
%token          FIGUREGROUP
%token          FIGUREGROUPOBJECT
%token          FIGUREGROUPOVERRIDE
%token          FIGUREGROUPREF
%token          FIGUREPERIMETER
%token          FIGUREWIDTH
%token          FILLPATTERN
%token          FOLLOW
%token          FORBIDDENEVENT
%token          GLOBALPORTREF
%token          GREATERTHAN
%token          GRIDMAP
%token          IGNORE
%token          INCLUDEFIGUREGROUP
%token          INITIAL_KW
%token          INSTANCE
%token          INSTANCEBACKANNOTATE
%token          INSTANCEGROUP
%token          INSTANCEMAP
%token          INSTANCEREF
%token          INTEGERDISPLAY
%token          INTERFACE
%token          INTERFIGUREGROUPSPACING
%token          INTERSECTION
%token          INTRAFIGUREGROUPSPACING
%token          INVERSE
%token          ISOLATED
%token          JOINED
%token          JUSTIFY
%token          KEYWORDDISPLAY
%token          KEYWORDLEVEL
%token          KEYWORDMAP
%token          LESSTHAN
%token          LIBRARY
%token          LIBRARYREF
%token          LISTOFNETS
%token          LISTOFPORTS
%token          LOADDELAY
%token          LOGICASSIGN
%token          LOGICINPUT
%token          LOGICLIST
%token          LOGICMAPINPUT
%token          LOGICMAPOUTPUT
%token          LOGICONEOF
%token          LOGICOUTPUT
%token          LOGICPORT
%token          LOGICREF
%token          LOGICVALUE
%token          LOGICWAVEFORM
%token          MAINTAIN
%token          MATCH
%token          MEMBER
%token          MINOMAX
%token          MINOMAXDISPLAY
%token          MNM
%token          MULTIPLEVALUESET
%token          MUSTJOIN
%token          NAME
%token          NET
%token          NETBACKANNOTATE
%token          NETBUNDLE
%token          NETDELAY
%token          NETGROUP
%token          NETMAP
%token          NETREF
%token          NOCHANGE
%token          NONPERMUTABLE
%token          NOTALLOWED
%token          NOTCHSPACING
%token          NUMBER
%token          NUMBERDEFINITION
%token          NUMBERDISPLAY
%token          OFFPAGECONNECTOR
%token          OFFSETEVENT
%token          OPENSHAPE
%token          ORIENTATION
%token          ORIGIN
%token          OVERHANGDISTANCE
%token          OVERLAPDISTANCE
%token          OVERSIZE
%token          OWNER
%token          PAGE
%token          PAGESIZE_TORC
%token          PARAMETER
%token          PARAMETERASSIGN
%token          PARAMETERDISPLAY
%token          PATH
%token          PATHDELAY
%token          PATHWIDTH
%token          PERMUTABLE
%token          PHYSICALDESIGNRULE
%token          PLUG
%token          POINT
%token          POINTDISPLAY
%token          POINTLIST
%token          POLYGON
%token          PORT
%token          PORTBACKANNOTATE
%token          PORTBUNDLE
%token          PORTDELAY
%token          PORTGROUP
%token          PORTIMPLEMENTATION
%token          PORTINSTANCE
%token          PORTLIST
%token          PORTLISTALIAS
%token          PORTMAP
%token          PORTREF
%token          PROGRAM
%token          PROPERTY
%token          PROPERTYDISPLAY
%token          PROTECTIONFRAME
%token          PT
%token          RANGEVECTOR
%token          RECTANGLE
%token          RECTANGLESIZE
%token          RENAME
%token          RESOLVES
%token          SCALE
%token          SCALEX
%token          SCALEY
%token          SECTION
%token          SHAPE
%token          SIMULATE
%token          SIMULATIONINFO
%token          SINGLEVALUESET
%token          SITE
%token          SOCKET
%token          SOCKETSET
%token          STATUS
%token          STEADY
%token          STRINGDISPLAY
%token          STRONG
%token          SYMBOL
%token          SYMMETRY
%token          TABLE
%token          TABLEDEFAULT
%token          TECHNOLOGY
%token          TEXTHEIGHT
%token          TIMEINTERVAL
%token          TIMESTAMP
%token          TIMING
%token          TRANSFORM
%token          TRANSITION
%token          TRIGGER
%token          TRUE
%token          UNCONSTRAINED
%token          UNDEFINED
%token          UNION
%token          UNIT
%token          UNUSED
%token          USERDATA
%token          VERSION
%token          VIEW
%token          VIEWLIST
%token          VIEWMAP
%token          VIEWREF
%token          VIEWTYPE
%token          VISIBLE
%token          VOLTAGEMAP
%token          WAVEVALUE
%token          WEAK
%token          WEAKJOINED
%token          WHEN
%token          WRITTEN

%token          LBKT
%token          RBKT

%type <integerVal>Int
%type <stringVal>Str
%type <stringVal>Ident
%type <nameData>_Name
%type <nameData>ArrayData
%type <arrayDimensions>_IntegerList
%type <arrayDimensions>_ArrayData
%type <nameData>MemberData
%type <arrayDimensions>_MemberData
%type <nameData>Name
%type <nameData>NameDefData
%type <nameData>__Rename
%type <stringVal>_Rename
%type <nameData>Rename
%type <stringVal>Author
%type <stringVal>Program
%type <stringVal>Version
%type <nameData>LibNameDef
%type <integerVal>EdifLevel
%type <nameData>CellNameDef
%type <integerVal>_CellType
%type <nameData>ViewNameDef
%type <integerVal>_ViewType
%type <nameData>PortNameDefData
%type <integerVal>_Direction
%type <nameData>NameRefData
%type <nameData>NameRef
%type <nameData>LibNameRefData
%type <nameData>LibraryRefData
%type <nameData>CellNameRefData
%type <nameData>CellNameRef
%type <viewRefData>_CellRefData
%type <viewRefData>CellRefData
%type <cellRefData>_CellRef
%type <cellRefData>CellRef
%type <cellRefData>_Design
%type <nameData>DesignNameDef
%type <nameData>ViewNameRefData
%type <viewRefData>_ViewRefData
%type <viewRefData>ViewRefData
%type <instanceInfo>_Instance
%type <nameData>InstNameDef
%type <nameData>NetNameDef
%type <nameData>InstNameRefData
%type <nameData>PortNameRefData
%type <portRefData>_PortRefData
%type <portRefData>PortRefData
%type <portListData>_PortListData
%type <portListData>PortListData
%type <nameData>GlobPortRefData
%type <instanceRef>InstanceRefData
%type <instanceRef>_InstanceRefData
%type <edifVersion>EdifVersion
%type <nameData>EdifFileName
%type <valueData>_String
%type <valueData>String
%type <stringVal>StrDisplay
%type <stringVal>_StrDisplay
%type <valueData>TypedValue
%type <pairData>PointValue
%type <valueData>_Point
%type <valueData>Point
%type <pairData>_PointDisp
%type <pairData>PointDisp
%type <pairData>ScaledInt
%type <valueData>_Number
%type <valueData>Number
%type <pairData>NumbDisplay
%type <pairData>_NumbDisplay
%type <pairData>_Mnm
%type <valueData>Mnm
%type <valueData>MiNoMaValue
%type <valueData>_MiNoMa
%type <valueData>MiNoMa
%type <valueData>MiNoMaDisp
%type <valueData>_MiNoMaDisp
%type <valueData>_Integer
%type <valueData>Integer
%type <integerVal>IntDisplay
%type <integerVal>_IntDisplay
%type <integerVal>BooleanValue
%type <valueData>_Boolean
%type <valueData>Boolean
%type <integerVal>BooleanDisp
%type <integerVal>_BooleanDisp
%type <nameData>ValueNameDefData
%type <nameData>ValueNameRefData
%type <integerVal>_Unit
%type <integerVal>Unit
%type <integerVal>_Parameter
%type <paramAssignInfo>ParamAssign
%type <stringVal>Owner
%type <nameData>PropNameDef
%type <integerVal>_DerivationData
%type <integerVal>DerivationData
%type <portDelay>_DelayData
%type <portDelay>DelayData
%type <valueData>_LoadDelayData
%type <portDelay>LoadDelayData
%type <portDelay>_PortDelayData
%type <portDelay>PortDelayData
%type <portAttributes>_Port
%type <portAttributes>_PortBackAn
%type <portInst>PortInst
%type <portInst>_PortInst
%type <integerVal>UnusedData
%type <stringVal>_DesignatorData
%type <stringVal>DesignatorData
%type <pairData>_DcFanInLoad
%type <pairData>DcFanInLoad
%type <pairData>_DcFanOutLoad
%type <pairData>DcFanOutLoad
%type <pairData>_DcMaxFanIn
%type <pairData>DcMaxFanIn
%type <pairData>_DcMaxFanOut
%type <pairData>DcMaxFanOut
%type <valueData>_AcLoad
%type <valueData>AcLoad

%destructor { } <integerVal>
%destructor { } <doubleVal>
%destructor { } <nameData>
%destructor { delete $$; } <*>

%{

#include "torc/generic/parser/Driver.hpp"
#include "torc/generic/parser/Scanner.hpp"
#include "torc/generic/om/Root.hpp"
#include "torc/generic/om/ObjectFactory.hpp"
#include "torc/generic/om/Library.hpp"
#include "torc/generic/parser/Linker.hpp"
#include "torc/generic/om/VisitorApplier.hpp"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex inDriver.getLexer()->lex

namespace {

std::string
constructErrorMessage( const char *fmt, ... )
{
    char msg[BUFSIZ];
    va_list args;
    va_start( args, fmt );
    int nbytes = vsnprintf( msg, BUFSIZ, fmt, args );
    va_end( args );
    if( BUFSIZ <= nbytes )
    {
        return std::string();
    }
    return std::string( msg );
}

using namespace torc::generic;

class AssignedParamCreator
        :public SingleParameter::Visitor,
        public ParameterArrayElement::Visitor,
        public ParameterArray::Visitor {
  public:
    void
    visit( SingleParameter &single ) throw(Error)
    try
    {
        SingleParameterSharedPtr singleP;
        mFactory->create( singleP );
        singleP->setName( single.getName() );
        singleP->setValue( mValues[0] );
        mMap->set( mContext, single.getName(), singleP );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

    void
    visit( ParameterArrayElement &element ) throw(Error)
    try
    {
        ParameterSharedPtr array
                                    = element.getParentCollection();
        if( array == mMap->get( mContext, array->getName() ))
        {
            AssignedParamCreator creator(
                    mInstance, mContext, mMap, mFactory, mValues );
            array->accept( creator );
        }
        std::vector<size_t> indices = element.getIndices();
        ParameterSharedPtr arrayP
                    =  mMap->get( mContext, array->getName() );
        ParameterSharedPtr bit = arrayP->get( indices );
        bit->setValue( mValues[0] );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

    void
    visit( ParameterArray &array ) throw(Error)
    try
    {
        std::vector<size_t> limits;
        array.getLimits( limits );
        ParameterArraySharedPtr arrayP;
        mFactory->create( arrayP );
        arrayP->setName( array.getName() );
        std::vector<size_t> dimensions;
        array.getLimits( dimensions );
        arrayP->constructChildren( mFactory, limits );
        std::vector< ParameterSharedPtr > children;
        arrayP->getChildren( children );
        std::vector< ParameterSharedPtr >::iterator
                                            it = children.begin();
        std::vector< ParameterSharedPtr >::iterator
                                            end = children.end();
        size_t i = 0;
        for(; it != end; ++it, i++ )
        {
            (*it)->setValue( mValues[i] );
        }
        mMap->set( mContext, array.getName(), arrayP );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

    AssignedParamCreator(
                const InstanceSharedPtr &inInstance,
                ParameterContext inContext,
                const ParameterMapSharedPtr &inMap,
                const ObjectFactorySharedPtr &inFactory,
                std::vector<Value> &inValues )
        :mInstance( inInstance ),
        mContext( inContext ),
        mMap( inMap ),
        mFactory( inFactory ),
        mValues( inValues ) {
    }

    ~AssignedParamCreator() throw() {
    }

  private:
    InstanceSharedPtr mInstance;
    ParameterContext mContext;
    ParameterMapSharedPtr mMap;
    ObjectFactorySharedPtr mFactory;
    std::vector<Value> &mValues;
};

template<typename _Connectable>
void
connectibleFinder( std::vector< std::string > &nestedNames,
        boost::shared_ptr<_Connectable> &conn ) throw(Error)
{
    std::vector< std::string >::reverse_iterator name
                                = nestedNames.rbegin();
    std::vector< std::string >::reverse_iterator end
                                = nestedNames.rend();
    ++name;
    if( !conn )
    {
        std::string message = constructErrorMessage(
                "Nested connectible name %s not found",
                                            (*name).c_str() );
        log("%s\n", message.c_str());
        Error e( eMessageIdErrorNullPointer,
            __FUNCTION__, __FILE__, __LINE__ );
        throw e;
    }
    for( ;name != end; ++name )
    {
        if( eCompositionTypeBundle
                != conn->getCompositionType() )
        {
            log("Item is not a bundle .. invalid nesting\n");
            Error e( eMessageIdErrorUnsupoortedOperation,
                __FUNCTION__, __FILE__, __LINE__ );
            e.saveContextData("Name", *name );
            e.saveContextData("Operation",
                                    std::string( "Find child" ) );
            throw e;
        }
        std::vector< boost::shared_ptr<_Connectable> > children;
        conn->getChildren( children );
        bool found = false;
        for( typename
            std::vector< boost::shared_ptr<_Connectable> >::iterator
                it = children.begin(); it != children.end(); ++it )
        {
            if( (*it)->getName() == *name )
            {
                conn = (*it);
                found = true;
                break;
            }
        }
        if( !found )
        {
            std::string message = constructErrorMessage(
                    "Nested connectible %s not found",
                    (*name).c_str() );
            log("%s\n", message.c_str());
            Error e( eMessageIdErrorItemNotFound,
                __FUNCTION__, __FILE__, __LINE__ );
            e.saveContextData("Name", (*name));
            throw e;
        }
    }
}

template<typename _Connectable>
void
portJoiner( NameData *inPortNameData,
            const boost::shared_ptr<_Connectable> &inConn,
            const NetSharedPtr &inNet ) throw(Error)
{
    try
    {
        if( inPortNameData->mIndices.empty() )
        {
            //TBD::CHECK WIDTH
            inConn->connect( inNet );
        }
        else
        {
            boost::shared_ptr<_Connectable> bit
                = inConn->get( inPortNameData->mIndices);
            bit->connect( inNet );
        }
        log( "Net %s connected with connectible %s\n",
                    inNet->getName().c_str(), inConn->getName().c_str() );
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
connectElementToNet( PortRefData *inPortData,
                const EdifContextSharedPtr &inContext,
                Parser::location_type& inLocation,
                std::string &outErrorMessage ) throw(Error)
{
    PortRefData *portData = inPortData;
    std::vector< std::string > nestedNames;
    PortRefData *currPortRef = inPortData;
    PortRefData *nextPortRef = portData->mParentPort;
    nestedNames.push_back(currPortRef->mPortName->mName);
    while( nextPortRef )
    {
        currPortRef = nextPortRef;
        nestedNames.push_back( nextPortRef->mPortName->mName );
        nextPortRef = currPortRef->mParentPort;
    }
    bool isPort = ( NULL == currPortRef->mInstanceName );
    NameData *portNameData = currPortRef->mPortName;
    std::string topName = *(nestedNames.rbegin());
    EdifContextSharedPtr ctx = inContext;
    ViewSharedPtr view = ctx->getCurrentView();
    if( isPort )
    {
        PortSharedPtr port = view->findPort( topName );
        if( !port )
        {
            std::string message = constructErrorMessage(
                    "No port %s found",
                        portNameData->mName.c_str());
            log("%s\n", message.c_str());
            Error e( eMessageIdParserError,
                __FUNCTION__, __FILE__, __LINE__ );
            e.saveContextData( "Parser error message", message );
            e.saveContextData( "Filename", inLocation.begin.filename );
            e.saveContextData( "StartLine", inLocation.begin.line );
            e.saveContextData( "EndLine", inLocation.end.line );
            outErrorMessage = message;
            throw e;
        }
        try
        {
            NetSharedPtr net = ctx->getCurrentNet();
            connectibleFinder( nestedNames, port );
            portJoiner( portData->mPortName, port, net );
        }
        catch( Error &e )
        {
            std::string message = constructErrorMessage(
                    "Unable to connect port : %s", port->getName().c_str());
            log("%s\n", message.c_str());
            e.setCurrentLocation( __FUNCTION__,
                                    __FILE__, __LINE__ );
            e.saveContextData( "Parser error message", message );
            e.saveContextData( "Filename", inLocation.begin.filename );
            e.saveContextData( "StartLine", inLocation.begin.line );
            e.saveContextData( "EndLine", inLocation.end.line );
            outErrorMessage = message;
            throw e;
        }
    }
    else
    {
        NameData *instanceName
                        = currPortRef->mInstanceName;
        InstanceSharedPtr instance
                        = view->findInstance(
                                    instanceName->mName );
        if( !instance )
        {
            std::string message = constructErrorMessage(
                    "Instance %s not known",
                    instanceName->mName.c_str() );
            log("%s\n", message.c_str());
            Error e( eMessageIdParserError,
                __FUNCTION__, __FILE__, __LINE__ );
            e.saveContextData( "Parser error message", message );
            e.saveContextData( "Filename", inLocation.begin.filename );
            e.saveContextData( "StartLine", inLocation.begin.line );
            e.saveContextData( "EndLine", inLocation.end.line );
            outErrorMessage = message;
            throw e;
        }
        if( !instanceName->mIndices.empty() )
        {
            InstanceSharedPtr bit
                    = instance->get( instanceName->mIndices );
            instance = bit;
        }
        PortReferenceSharedPtr portRef
                = instance->findPortReference( topName );
        if( !portRef )
        {
            std::string message = constructErrorMessage(
                    "No port ref %s found",
                        portNameData->mName.c_str());
            log("%s\n", message.c_str());
            Error e( eMessageIdParserError,
                __FUNCTION__, __FILE__, __LINE__ );
            e.saveContextData( "Parser error message", message );
            e.saveContextData( "Filename", inLocation.begin.filename );
            e.saveContextData( "StartLine", inLocation.begin.line );
            e.saveContextData( "EndLine", inLocation.end.line );
            outErrorMessage = message;
            throw e;
        }
        try
        {
            connectibleFinder( nestedNames, portRef );
            portJoiner( portData->mPortName, portRef,
                            ctx->getCurrentNet() );
        }
        catch( Error &e )
        {
            std::string message = constructErrorMessage(
                    "Unable to connect port ref : %s", portRef->getName().c_str());
            log("%s\n", message.c_str());
            e.setCurrentLocation( __FUNCTION__,
                                    __FILE__, __LINE__ );
            e.saveContextData( "Parser error message", message );
            e.saveContextData( "Filename", inLocation.begin.filename );
            e.saveContextData( "StartLine", inLocation.begin.line );
            e.saveContextData( "EndLine", inLocation.end.line );
            outErrorMessage = message;
            throw e;
        }
    }
}

}

%}


%% /*** Grammar Rules ***/

Edif : LBKT EDIF EdifFileName EdifVersion EdifLevel KeywordMap _Edif RBKT
        {
            boost::scoped_ptr<NameData> fileName( $3 );
            boost::scoped_ptr<VersionData> versionData( $4 );
            EdifContextSharedPtr ctx = inDriver.getContext();
            RootSharedPtr root = ctx->getRoot();
            root->setName( fileName->mName );
            if( !fileName->mOriginalName.empty() )
            {
                root->setOriginalName( fileName->mOriginalName );
            }
            EdifVersion version;
            version.mFirst = versionData->mFirst;
            version.mSecond = versionData->mSecond;
            version.mThird = versionData->mThird;
            root->setVersion( version );
            root->setLevel( static_cast<EdifLevel>( $5 ) );
        }
     ;

_Edif :
      |         _Edif Status
      |         _Edif External
      |         _Edif Library
      |         _Edif Design
      |         _Edif Comment
      |         _Edif UserData
      ;

EdifFileName :  NameDefData
             ;

EdifLevel : LBKT EDIFLEVEL Int RBKT
            {
                $$ = $3;
            }
          ;

EdifVersion : LBKT EDIFVERSION Int Int Int RBKT
                {
                    $$ = new VersionData();
                    $$->mFirst = $3;
                    $$->mSecond = $4;
                    $$->mThird = $5;
                }
            ;

AcLoad : LBKT ACLOAD _AcLoad RBKT
            {
                $$ = $3;
            }
       ;

_AcLoad :       MiNoMaValue
        |       MiNoMaDisp
        ;

After : LBKT AFTER _After RBKT
      ;

_After :        MiNoMaValue
       |        _After Follow
       |        _After Maintain
       |        _After LogicAssn
       |        _After Comment
       |        _After UserData
       ;

Annotate : LBKT ANNOTATE _Annotate RBKT
         ;

_Annotate :     Str
            {
                delete $1;
            }
          |     StrDisplay
              {
                delete $1;
            }
          ;

Apply : LBKT APPLY _Apply RBKT
      ;

_Apply :        Cycle
       |        _Apply LogicIn
       |        _Apply LogicOut
       |        _Apply Comment
       |        _Apply UserData
       ;

Arc : LBKT ARC PointValue PointValue PointValue RBKT
        {
            delete $3;
            delete $4;
            delete $5;
        }
    ;

ArrayData : LBKT ARRAY NameDefData Int _ArrayData RBKT
        {
            $$ = $3;
            $$->mIndices.push_back( $4 );
            if( $5 )
            {
                $$->mIndices.insert( $$->mIndices.end(),
                                    $5->begin(), $5->end() );
                delete $5;
            }
        }
      ;

_ArrayData :
        {
            $$ = NULL;
        }
       |        _IntegerList
       ;

_IntegerList : Int _IntegerList
        {
            $$ = new std::vector<size_t>();
            $$->push_back( $1 );
            if( $2 )
            {
                $$->insert( $$->end(), $2->begin(), $2->end() );
                delete $2;
            }
        }
        | Int
        {
            $$ = new std::vector<size_t>();
            $$->push_back( $1 );
        }
        ;

ArrayMacro : LBKT ARRAYMACRO Plug RBKT
           ;

ArrayRelInfo : LBKT ARRAYRELATEDINFO _ArrayRelInfo RBKT
             ;

_ArrayRelInfo : BaseArray
              | ArraySite
              | ArrayMacro
              | _ArrayRelInfo Comment
              | _ArrayRelInfo UserData
              ;

ArraySite : LBKT ARRAYSITE Socket RBKT
          ;

AtLeast : LBKT ATLEAST ScaledInt RBKT
        {
            delete $3;
        }
        ;

AtMost : LBKT ATMOST ScaledInt RBKT
        {
            delete $3;
        }
       ;

Author : LBKT AUTHOR Str RBKT
        {
            $$ = $3;
        }
       ;

BaseArray : LBKT BASEARRAY RBKT
          ;

Becomes : LBKT BECOMES _Becomes RBKT
        ;

_Becomes :      LogicNameRef
         |      LogicList
         |      LogicOneOf
         ;

Between : LBKT BETWEEN __Between _Between RBKT
        ;

__Between :     AtLeast
          |     GreaterThan
          ;

_Between :      AtMost
         |      LessThan
         ;

Boolean : LBKT BOOLEAN _Boolean RBKT
        {
            if(  NULL == $3 )
            {
                $$ = new ValueData();
                $$->mValues.push_back( Value(
                        Value::eValueTypeBoolean ) );
            }
            else
            {
                $$ = $3;
            }
        }
        ;

_Boolean :
            {
                $$ = NULL;
            }
         |      _Boolean BooleanValue
            {
                $$ = ( $1 ) ? $1 : new ValueData();
                bool val = $2;
                $$->mValues.push_back( Value(
                            Value::eValueTypeBoolean, val ) );
            }
         |      _Boolean BooleanDisp
            {
                $$ = ( $1 ) ? $1 : new ValueData();
                bool val = $2;
                $$->mValues.push_back( Value(
                            Value::eValueTypeBoolean, val ) );
            }
         |      _Boolean Boolean
         {
            $$ = $2;
            if( $1 )
            {
               $1->mValues.insert( $1->mValues.end(),
                       $$->mValues.begin(), $$->mValues.end() );
               $$->mValues = $1->mValues;
               delete $1;
            }
         }
         ;

BooleanDisp : LBKT BOOLEANDISPLAY _BooleanDisp RBKT
            {
                $$ = $3;
            }
            ;

_BooleanDisp :  BooleanValue
             {
                $$ = $1;
             }
             |  _BooleanDisp Display
             {
                $$ = $1;
             }
             ;

BooleanMap : LBKT BOOLEANMAP BooleanValue RBKT
            {
                if($3);
            }
           ;

BooleanValue :  True
                {
                    $$ = 1;
                }
             |  False
                {
                    $$ = 0;
                }
             ;

BorderPat : LBKT BORDERPATTERN Int Int Boolean RBKT
            {
                if( $3 );
                if( $4 );
                delete $5;
            }
          ;

BorderWidth : LBKT BORDERWIDTH Int RBKT
            {
                if( $3 );
            }
            ;

BoundBox : LBKT BOUNDINGBOX Rectangle RBKT
         ;

Cell : LBKT CELL CellNameDef
        {
            boost::scoped_ptr<NameData> nameData( $3 );
            std::string name = nameData->mName;
            EdifContextSharedPtr ctx = inDriver.getContext();
            CellSharedPtr cell;
            LibrarySharedPtr lib = ctx->getCurrentLibrary();
            cell = lib->findCell( name );
            if( cell )
            {
                log("Found existing cell\n");
                if( lib->getIsExtern() )
                {
                    if( !cell->getIsExtern() )
                    {
                        std::string message = constructErrorMessage(
                            "Cell %s already exists in lib %s\n",
                                name.c_str(), lib->getName().c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                    else
                    {
                        log( "Extern cell %s already exists in %s\n",
                            name.c_str(), lib->getName().c_str() );
                    }
                }
                else
                {
                    if( cell->getIsExtern() )
                    {
                        log("Found extern cell of this name\n");
                        cell->setIsExtern( false );
                    }
                    else
                    {
                        //TBD::WARN
                        log("Concrete cell %s already found... Ignoring\n",
                                name.c_str());
#if 0
                        std::string message = constructErrorMessage(
                            "Cell %s already exists in lib %s\n",
                                name.c_str(), lib->getName().c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
#endif
                    }
                }
            }
            else
            {
                try
                {
                    ctx->getFactory()->create( cell );
                }
                catch( Error &e )
                {
                    std::string message = constructErrorMessage(
                            "Unable to create Cell : %s", cell->getName().c_str());
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData( "Parser error message", message );
                    e.saveContextData( "Filename", yyloc.begin.filename );
                    e.saveContextData( "StartLine", yyloc.begin.line );
                    e.saveContextData( "EndLine", yyloc.end.line );
                    inDriver.setParserError( e );
                    error( yyloc, message );
                    YYABORT;
                }
                cell->setName( name );
                if( !$3->mOriginalName.empty() )
                {
                    cell->setOriginalName( $3->mOriginalName );
                }
                cell->setIsExtern( lib->getIsExtern() );
                cell->setParent( lib );
                lib->addCell( cell );
                log( "Created cell %s\n", cell->getName().c_str() );
            }
            ctx->setCurrentCell( cell );
            ctx->pushPropertyContainer( cell );
        } _Cell RBKT
        {
            inDriver.getContext()->setCurrentCell(
                                    CellSharedPtr() );
            inDriver.getContext()->popPropertyContainer();
        }
        ;

_Cell :         CellType
      |         _Cell Status
      |         _Cell ViewMap
      |         _Cell View
      |         _Cell Comment
      |         _Cell UserData
      |         _Cell Property
      ;

CellNameDef :   NameDefData
            ;


CellRef : LBKT CELLREF CellNameRef _CellRef RBKT
        {
            if( NULL == $4 )
            {
                $$ = new CellRefData();
            }
            else
            {
                $$ = $4;
            }
            $$->mCellName = $3;
        }
        ;

_CellRef :
        {
            $$ = NULL;
        }
         |  LibraryRefData  
        {
            $$ = new CellRefData();
            $$->mLibraryName = $1;    
        }
         ;

CellRefData : LBKT CELLREF CellNameRefData _CellRefData RBKT
        {
            if( NULL == $4 )
            {
                $$ = new ViewRefData();
            }
            else
            {
                $$ = $4;
            }
            $$->mCellName = $3;
        }
        ;

_CellRefData :
         {
             $$ = NULL;
         }
         |      LibraryRefData
         {
             $$ = new ViewRefData();
             $$->mLibraryName = $1;
         }
         ;

CellNameRef :   NameRef
            {
                $$ = $1;
            }
            ;

CellNameRefData :   NameRefData
            {
                $$ = $1;
            }
            ;

CellType : LBKT CELLTYPE _CellType RBKT
            {
                inDriver.getContext()->getCurrentCell()->setType(
                        static_cast<Cell::Type>( $3 ) );
            }
         ;

_CellType :     TIE
            {
                $$ = Cell::eTypeTie;
            }
          |     RIPPER
              {
                $$ = Cell::eTypeRipper;
            }
          |     GENERIC
              {
                $$ = Cell::eTypeGeneric;
            }
          ;

Change : LBKT CHANGE __Change _Change RBKT
       ;

__Change :      PortNameRef
         |      PortRef
         |      PortList
         ;

_Change :
        |       Becomes
        |       Transition
        ;

Circle : LBKT CIRCLE PointValue PointValue _Circle RBKT
        {
            delete $3;
            delete $4;
        }
       ;

_Circle :
        |       _Circle Property
        ;

Color : LBKT COLOR ScaledInt ScaledInt ScaledInt RBKT
        {
            delete $3;
            delete $4;
            delete $5;
        }
      ;

Comment : LBKT COMMENT _Comment RBKT
        ;

_Comment :
         |      _Comment Str
        {
            //TBD::HANDLE COMMENT
            delete $2;
        }
         ;

CommGraph : LBKT COMMENTGRAPHICS _CommGraph RBKT
          ;

_CommGraph :
           |    _CommGraph Annotate
           |    _CommGraph Figure
           |    _CommGraph Instance
           |    _CommGraph BoundBox
           |    _CommGraph Property
           |    _CommGraph Comment
           |    _CommGraph UserData
           ;

Compound : LBKT COMPOUND LogicNameRef RBKT
         ;

Contents : LBKT CONTENTS _Contents RBKT
         ;

_Contents :
          |     _Contents Instance
          |     _Contents OffPageConn
          |     _Contents Figure
          |     _Contents Section
          |     _Contents Net
          |     _Contents NetBundle
          |     _Contents Page
          |     _Contents CommGraph
          |     _Contents PortImpl
          |     _Contents Timing
          |     _Contents Simulate
          |     _Contents When
          |     _Contents Follow
          |     _Contents LogicPort
          |     _Contents BoundBox
          |     _Contents Comment
          |     _Contents UserData
          ;

ConnectLoc : LBKT CONNECTLOCATION _ConnectLoc RBKT
           ;

_ConnectLoc :
            |   Figure
            ;

CornerType : LBKT CORNERTYPE _CornerType RBKT
           ;

_CornerType :   EXTEND
            |   ROUND
            |   TRUNCATE
            ;

Criticality : LBKT CRITICALITY _Criticality RBKT
            ;

_Criticality :  Int
                {
                    if( $1 );
                }
             |  IntDisplay
                {
                    if( $1 );
                }
             ;

CurrentMap : LBKT CURRENTMAP MiNoMaValue RBKT
            {
                delete $3;
            }
           ;

Curve : LBKT CURVE _Curve RBKT
      ;

_Curve :
       |        _Curve Arc
       |        _Curve PointValue
        {
            delete $2;
        }
       ;

Cycle : LBKT CYCLE Int _Cycle RBKT
        {
            if( $3 );
        }
      ;

_Cycle :
       |        Duration
       ;

DataOrigin : LBKT DATAORIGIN Str _DataOrigin RBKT
            {
                delete $3;
            }
           ;

_DataOrigin :
            |   Version
                {
                    delete $1;
                }
            ;

DcFanInLoad : LBKT DCFANINLOAD _DcFanInLoad RBKT
            {
                $$ = $3;
            }
            ;

_DcFanInLoad :  ScaledInt
             |  NumbDisplay
             ;

DcFanOutLoad : LBKT DCFANOUTLOAD _DcFanOutLoad RBKT
            {
                $$ = $3;
            }
             ;

_DcFanOutLoad : ScaledInt
              | NumbDisplay
              ;

DcMaxFanIn : LBKT DCMAXFANIN _DcMaxFanIn RBKT
            {
                $$ = $3;
            }
           ;

_DcMaxFanIn :   ScaledInt
            |   NumbDisplay
            ;

DcMaxFanOut : LBKT DCMAXFANOUT _DcMaxFanOut RBKT
            {
                $$ = $3;
            }
            ;

_DcMaxFanOut :  ScaledInt
             |  NumbDisplay
             ;

DelayData : LBKT DELAY _DelayData RBKT
            {
                $$ = $3;
            }
      ;

_DelayData :  MiNoMaValue
        {
            boost::scoped_ptr<ValueData> data( $1 );
            $$ = new PortDelay();
            $$->setType( PortDelay::eTypeDelay );
            $$->setDelay( data->mValues[0].get<Value::MiNoMax>() );
        }
       |     MiNoMaDisp
        {
            boost::scoped_ptr<ValueData> data( $1 );
            $$ = new PortDelay();
            $$->setType( PortDelay::eTypeDelay );
            $$->setDelay( data->mValues[0].get<Value::MiNoMax>() );
        }
       ;

Delay: LBKT DELAY _Delay RBKT
      ;

_Delay:     MiNoMaValue
            {
                delete $1;
            }
       |    MiNoMaDisp
            {
                delete $1;
            }
       ;

Delta : LBKT DELTA _Delta RBKT
      ;

_Delta :
       |        _Delta PointValue
           {
            delete $2;
        }
       ;

Derivation : LBKT DERIVATION _Derivation RBKT
           ;

_Derivation :   CALCULATED
            |   MEASURED
            |   REQUIRED
            ;

DerivationData : LBKT DERIVATION _DerivationData RBKT
            {
                $$ = $3;
            }
           ;

_DerivationData :   CALCULATED
            {
                $$ = PortDelay::eDerivationCalculated;
            }
            |   MEASURED
            {
                $$ = PortDelay::eDerivationMeasured;
            }
            |   REQUIRED
            {
                $$ = PortDelay::eDerivationRequired;
            }
            ;

Design : LBKT DESIGN DesignNameDef 
        {
            if( $3 )
            {
                EdifContextSharedPtr ctx
                                      = inDriver.getContext();
                RootSharedPtr root = ctx->getRoot();
                DesignSharedPtr design;
                design = root->findDesign( $3->mName );
                if( design )
                {
                    log("Skipped creation of existing design.\n");
                }
                else
                {
                    try
                    {
                        ctx->getFactory()->create( design );
                    }
                    catch( Error &e )
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create design : %s", design->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                    design->setName( $3->mName );
                    if( !$3->mOriginalName.empty() )
                    {
                        design->setOriginalName( $3->mOriginalName );
                    }
                    design->setParent( root );
                    root->addDesign( design );
                }
                ctx->setCurrentDesign( design );
                ctx->pushPropertyContainer( design );
                delete $3;
            }
        } _Design RBKT
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            ctx->popPropertyContainer();
            boost::scoped_ptr<CellRefData> cellRefData( $5 );
            std::string cellName =  cellRefData->mCellName->mName;
            std::string libName = cellRefData->mLibraryName->mName;
            LibrarySharedPtr library 
                                = ctx->getRoot()->findLibrary( libName ) ;
            if( !library )
            {
                std::string message = constructErrorMessage(
                                    "Library %s not found",
                                            libName.c_str() );
                log("%s\n", message.c_str());
                Error e( eMessageIdParserError,
                    __FUNCTION__, __FILE__, __LINE__ );
                e.saveContextData( "Parser error message", message );
                e.saveContextData( "Filename", yyloc.begin.filename );
                e.saveContextData( "StartLine", yyloc.begin.line );
                e.saveContextData( "EndLine", yyloc.end.line );
                inDriver.setParserError( e );
                error( yyloc, message );
                YYABORT;
            }

            CellSharedPtr cell
                            = library->findCell( cellName );
            if( !cell )
            {
                std::string message = constructErrorMessage(
                                    "Cell %s not found",
                                            cellName.c_str() );
                log("%s\n", message.c_str());
                Error e( eMessageIdParserError,
                    __FUNCTION__, __FILE__, __LINE__ );
                e.saveContextData( "Parser error message", message );
                e.saveContextData( "Filename", yyloc.begin.filename );
                e.saveContextData( "StartLine", yyloc.begin.line );
                e.saveContextData( "EndLine", yyloc.end.line );
                inDriver.setParserError( e );
                error( yyloc, message );
                YYABORT;
            }
            
            DesignSharedPtr design = ctx->getCurrentDesign();
            design->setCellRefName( cellName );
            design->setLibraryRefName( libName );
        }            
       ;

_Design :       CellRef
        {
            $$ = new CellRefData();
            $$ = $1;
        }
        |       _Design Status
        {
            $$ = $1;
        }
        |       _Design Comment
        {
            $$ = $1;
        }
        |       _Design Property
        {
            $$ = $1;
        }
        |       _Design UserData
        {
            $$ = $1;
        }
        ;

Designator : LBKT DESIGNATOR _Designator RBKT
           ;

_Designator :   Str
                {
                    delete $1;
                }
            |   StrDisplay
                {
                    delete $1;
                }
            ;

DesignatorData : LBKT DESIGNATOR _DesignatorData RBKT
            {
                $$ = $3;
            }
           ;

_DesignatorData :   Str
            |   StrDisplay
            ;

DesignNameDef : NameDefData
            ;

DesignRule : LBKT PHYSICALDESIGNRULE _DesignRule RBKT
           ;

_DesignRule :
            |   _DesignRule FigureWidth
            |   _DesignRule FigureArea
            |   _DesignRule RectSize
            |   _DesignRule FigurePerim
            |   _DesignRule OverlapDist
            |   _DesignRule OverhngDist
            |   _DesignRule EncloseDist
            |   _DesignRule InterFigGrp
            |   _DesignRule IntraFigGrp
            |   _DesignRule NotchSpace
            |   _DesignRule NotAllowed
            |   _DesignRule FigGrp
            |   _DesignRule Comment
            |   _DesignRule UserData
            ;

Difference : LBKT DIFFERENCE _Difference RBKT
           ;

_Difference :   FigGrpRef
            |   FigureOp
            |   _Difference FigGrpRef
            |   _Difference FigureOp
            ;

Direction : LBKT DIRECTION _Direction RBKT
            {
                inDriver.getContext()->getCurrentPort()->setDirection(
                                static_cast<PortDirection>( $3 ) );
            }
          ;

_Direction : INPUT
            {
                $$ = 0;
            }
           | OUTPUT
            {
                $$ = 1;
            }
           | INOUT
            {
                $$ = 2;
            }
           ;

Display : LBKT DISPLAY _Display _DisplayExt RBKT
        ;

_Display :      FigGrpNameRef
         |      FigGrpOver
         ;

_DisplayExt:
            | _DisplayExt Justify
            | _DisplayExt Orientation
            | _DisplayExt Origin
            ;

Dominates : LBKT DOMINATES _Dominates RBKT
          ;

_Dominates :
           |    _Dominates LogicNameRef
           ;

Dot : LBKT DOT _Dot RBKT
    ;

_Dot :          PointValue
        {
            delete $1;
        }
     |          _Dot Property
     ;

Duration : LBKT DURATION ScaledInt RBKT
            {
                delete $3;
            }
         ;

EncloseDist : LBKT ENCLOSUREDISTANCE RuleNameDef FigGrpObj FigGrpObj _EncloseDist RBKT
            ;

_EncloseDist :  Range
             |  SingleValSet
             |  _EncloseDist Comment
             |  _EncloseDist UserData
             ;

EndType : LBKT ENDTYPE _EndType RBKT
        ;

_EndType :      EXTEND
         |      ROUND
         |      TRUNCATE
         ;

Entry : LBKT ENTRY ___Entry __Entry _Entry RBKT
      ;

___Entry :      Match
         |      Change
         |      Steady
         ;

__Entry :       LogicRef
        |       PortRef
        |       NoChange
        |       Table
        ;

_Entry :
       |        Delay
       |        LoadDelay
       ;

Event : LBKT EVENT _Event RBKT
      ;

_Event :        PortRef
       |        PortList
       |        PortGroup
       |        NetRef
       |        NetGroup
       |        _Event Transition
       |        _Event Becomes
       ;

Exactly : LBKT EXACTLY ScaledInt RBKT
            {
                delete $3;
            }
        ;

External : LBKT EXTERNAL LibNameDef EdifLevel
        {
            boost::scoped_ptr<NameData> name( $3 );
            if( name && !name->mName.empty() )
            {
                EdifContextSharedPtr ctx
                                            = inDriver.getContext();
                RootSharedPtr root = ctx->getRoot();
                LibrarySharedPtr lib;
                // We do not create a new extern lib when one
                // exists already
                lib = root->findLibrary( name->mName );
                if( lib )
                {
                    log("Skipped creation of existing lib.\n");
                }
                EdifLevel level;
                switch( $4 )
                {
                    case 0:
                    {
                        level = eEdifLevel0;
                        break;
                    }
                    case 1:
                    {
                        level = eEdifLevel1;
                        break;
                    }
                    case 2:
                    {
                        level = eEdifLevel2;
                        break;
                    }
                    default:
                    {
                        std::string message = constructErrorMessage(
                                "Edif level %s is not supported", $4 );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                }
                if( lib )
                {
                    if( lib->getLevel() != level )
                    {
                        std::string message = constructErrorMessage(
                                            "Level mismatch - old %d new %d\n",
                                                lib->getLevel(), level );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                    else
                    {
                        log("We have found the actual lib corresponding to this lib\n");
                    }
                }
                else
                {
                    ctx->getFactory()->create( lib );
                    lib->setName( name->mName );
                    if( ! name->mOriginalName.empty() )
                    {
                        lib->setOriginalName( name->mOriginalName );
                    }
                    lib->setLevel( level );
                    lib->setParent( root );
                    lib->setIsExtern( true );
                    root->addLibrary( lib );
                    log("Created extern lib %s level %d\n",
                                lib->getName().c_str(), level );
                }
                ctx->setCurrentLibrary( lib );
            }
            else
            {
                //TBD::POSSIBLE?
            }
        } _External RBKT
        {
            inDriver.getContext()->setCurrentLibrary(
                                    LibrarySharedPtr() );
        }
        ;
         ;

_External :     Technology
          |     _External Status
          |     _External Cell
          |     _External Comment
          |     _External UserData
          ;

Fabricate : LBKT FABRICATE LayerNameDef FigGrpNameRef RBKT
          ;

False : LBKT FALSE RBKT
      ;

FigGrp : LBKT FIGUREGROUP _FigGrp RBKT
       ;

_FigGrp :       FigGrpNameDef
        |       _FigGrp CornerType
        |       _FigGrp EndType
        |       _FigGrp PathWidth
        |       _FigGrp BorderWidth
        |       _FigGrp Color
        |       _FigGrp FillPattern
        |       _FigGrp BorderPat
        |       _FigGrp TextHeight
        |       _FigGrp Visible
        |       _FigGrp Comment
        |       _FigGrp Property
        |       _FigGrp UserData
        |       _FigGrp IncFigGrp
        ;

FigGrpNameDef : NameDef
              ;

FigGrpNameRef : NameRef
              ;

FigGrpObj : LBKT FIGUREGROUPOBJECT _FigGrpObj RBKT
          ;

_FigGrpObj :    FigGrpNameRef
           |    FigGrpRef
           |    FigureOp
           ;

FigGrpOver : LBKT FIGUREGROUPOVERRIDE _FigGrpOver RBKT
           ;

_FigGrpOver :   FigGrpNameRef
            |   _FigGrpOver CornerType
            |   _FigGrpOver EndType
            |   _FigGrpOver PathWidth
            |   _FigGrpOver BorderWidth
            |   _FigGrpOver Color
            |   _FigGrpOver FillPattern
            |   _FigGrpOver BorderPat
            |   _FigGrpOver TextHeight
            |   _FigGrpOver Visible
            |   _FigGrpOver Comment
            |   _FigGrpOver Property
            |   _FigGrpOver UserData
            ;

FigGrpRef : LBKT FIGUREGROUPREF FigGrpNameRef _FigGrpRef RBKT
          ;

_FigGrpRef :
           |    LibraryRef
           ;

Figure : LBKT FIGURE _Figure RBKT
       ;

_Figure :       FigGrpNameDef
        |       FigGrpOver
        |       _Figure Circle
        |       _Figure Dot
        |       _Figure OpenShape
        |       _Figure Path
        |       _Figure Polygon
        |       _Figure Rectangle
        |       _Figure Shape
        |       _Figure Comment
        |       _Figure UserData
        ;

FigureArea : LBKT FIGUREAREA RuleNameDef FigGrpObj _FigureArea RBKT
           ;

_FigureArea :   Range
            |   SingleValSet
            |   _FigureArea Comment
            |   _FigureArea UserData
            ;

FigureOp :      Intersection
         |      Union
         |      Difference
         |      Inverse
         |      Oversize
         ;

FigurePerim : LBKT FIGUREPERIMETER RuleNameDef FigGrpObj _FigurePerim RBKT
            ;


_FigurePerim :  Range
             |  SingleValSet
             |  _FigurePerim Comment
             |  _FigurePerim UserData
             ;

FigureWidth : LBKT FIGUREWIDTH RuleNameDef FigGrpObj _FigureWidth RBKT
            ;

_FigureWidth :  Range
             |  SingleValSet
             |  _FigureWidth Comment
             |  _FigureWidth UserData
             ;

FillPattern : LBKT FILLPATTERN Int Int Boolean RBKT
            {
                if( $3 );
                if( $4 );
                delete $5;
            }
            ;

Follow : LBKT FOLLOW __Follow _Follow RBKT
       ;

__Follow :      PortNameRef
         |      PortRef
         ;

_Follow :       PortRef
        |       Table
        |       _Follow Delay
        |       _Follow LoadDelay
        ;

Forbidden : LBKT FORBIDDENEVENT _Forbidden RBKT
          ;

_Forbidden :    TimeIntval
           |    _Forbidden Event
           ;

Form : LBKT
        {
            inDriver.getLexer()->setIsIdContext( true );
        }
        Keyword _Form RBKT
           ;

_Form :
      |         _Form Int
                  {
                    if( $2 );
                }
      |         _Form Str
                  {
                    delete $2;
                }
      |         _Form Ident
                  {
                    delete $2;
                }
      |         _Form Form
      ;

GlobPortRef : LBKT GLOBALPORTREF PortNameRef RBKT
            ;

GlobPortRefData : LBKT GLOBALPORTREF PortNameRefData RBKT
            {
                $$ = $3;
            }
            ;

GreaterThan : LBKT GREATERTHAN ScaledInt RBKT
                {
                    delete $3;
                }
            ;

GridMap : LBKT GRIDMAP ScaledInt ScaledInt RBKT
            {
                delete $3;
                delete $4;
            }
        ;

Ignore : LBKT IGNORE RBKT
       ;

IncFigGrp : LBKT INCLUDEFIGUREGROUP _IncFigGrp RBKT
          ;

_IncFigGrp :    FigGrpRef
           |    FigureOp
           ;

Initial : LBKT INITIAL_KW RBKT
        ;

Instance : LBKT INSTANCE InstNameDef
            {
                inDriver.getContext()->setIsInInstance( true );
            } _Instance RBKT
            {
                boost::scoped_ptr<NameData> nameData( $3 );
                EdifContextSharedPtr ctx = inDriver.getContext();
                boost::scoped_ptr<InstanceInfo> info( $5 );
                ViewRefData *viewData = info->mViewRefData;
                std::string cellName
                                = viewData->mCellName->mName;
                std::string viewName
                                = viewData->mViewName->mName;
                LibrarySharedPtr library;
                std::string libraryName;
                if( !viewData->mLibraryName )
                {
                    library = ctx->getCurrentLibrary();
                }
                else
                {
                     libraryName = viewData->mLibraryName->mName;
                     library = ctx->getRoot()->findLibrary( libraryName );
                }
                if( !library )
                {
                    std::string message = constructErrorMessage(
                                        "Library %s not found",
                                                libraryName.c_str() );
                    log("%s\n", message.c_str());
                    Error e( eMessageIdParserError,
                        __FUNCTION__, __FILE__, __LINE__ );
                    e.saveContextData( "Parser error message", message );
                    e.saveContextData( "Filename", yyloc.begin.filename );
                    e.saveContextData( "StartLine", yyloc.begin.line );
                    e.saveContextData( "EndLine", yyloc.end.line );
                    inDriver.setParserError( e );
                    error( yyloc, message );
                    YYABORT;
                }
                libraryName = library->getName();
                ViewSharedPtr masterView;
                bool masterIsExtern = false;
                LinkerSharedPtr linker = ctx->getLinker();
                Linker::NameSpec viewNameSpec(
                                    libraryName, cellName, viewName );
                if( !libraryName.empty() )
                {
                    masterView
                        = linker->findExternView( viewNameSpec );
                    if( !masterView )
                    {
                        //View is not extern
                        library = ctx->getRoot()->findLibrary(
                                                    libraryName );
                    }
                    else
                    {
                        masterIsExtern = true;
                    }
                }
                else
                {
                    library = ctx->getCurrentLibrary();
                }

                if( !masterIsExtern ) //View is not extern
                {
                    CellSharedPtr cell;
                    cell = library->findCell( cellName );
                    if( !cell )
                    {
                        std::string message = constructErrorMessage(
                                                "Cell %s not found",
                                                        cellName.c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                    masterView = cell->findView( viewName );
                    if( !masterView )
                    {
                        std::string message = constructErrorMessage(
                                    "View %s not found",
                                            viewName.c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                }
                InstanceSharedPtr instance;
                if( nameData->mIndices.empty() )
                {
                    try
                    {
                        SingleInstanceSharedPtr singleInst;
                        ctx->getFactory()->create( singleInst );
                        instance = singleInst;
                        instance->setName( nameData->mName );
                        if( !nameData->mOriginalName.empty() )
                        {
                            instance->setOriginalName( nameData->mOriginalName );
                        }
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create Instance : %s", instance->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                }
                else
                {
                    try
                    {
                        InstanceArraySharedPtr instArray;
                        ctx->getFactory()->create( instArray );
                        instArray->setName( nameData->mName );
                        if( !nameData->mOriginalName.empty() )
                        {
                            instArray->setOriginalName( nameData->mOriginalName );
                        }
                        instArray->constructChildren(
                                        ctx->getFactory(),
                                        nameData->mIndices );
                        instance = instArray;
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create Instance Array : %s", instance->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                }
                try
                {
                    instance->bindToMasterView( masterView );
                    if( eCompositionTypeVector
                            == instance->getCompositionType() )
                    {
                        std::vector< InstanceSharedPtr > children;
                        instance->getChildren( children );
                        std::vector< InstanceSharedPtr >::iterator inst
                                            = children.begin();
                        std::vector< InstanceSharedPtr >::iterator end
                                            = children.end();
                        for( ;inst != end; ++inst )
                        {
                            //Create Port references now
                            PortRefCreator<PortReferenceSharedPtr> creator(
                                    ctx->getFactory(), *inst );
                            masterView->applyOnAllPorts(
                                VisitorApplier< PortRefCreator<PortReferenceSharedPtr> >( creator ) );
                        }
                    }
                    else
                    {
                        //Create Port references now
                        PortRefCreator<PortReferenceSharedPtr> creator( ctx->getFactory(),
                                                        instance );
                        masterView->applyOnAllPorts(
                            VisitorApplier< PortRefCreator<PortReferenceSharedPtr> >( creator ) );
                    }
                }
                catch( Error &e )
                {
                    std::string message = constructErrorMessage(
                            "Unable to create Port ref");
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData( "Parser error message", message );
                    e.saveContextData( "Filename", yyloc.begin.filename );
                    e.saveContextData( "StartLine", yyloc.begin.line );
                    e.saveContextData( "EndLine", yyloc.end.line );
                    inDriver.setParserError( e );
                    error( yyloc, message );
                    YYABORT;
                }
                if( masterIsExtern )
                {
                    linker->registerUnresolvedInstance(
                                    viewNameSpec, instance );
                }
                instance->setParent( ctx->getCurrentView() );
                ctx->getCurrentView()->addInstance( instance );
                log("Instance for lib %s, cell %s, view %s\n",
                        libraryName.c_str(), cellName.c_str(),
                        viewName.c_str());
                log("Added instance to view %s \n",
                        ctx->getCurrentView()->getName().c_str());

                //Parameter assignment
                ParameterContext context
                               = instance->getParameterContext();
                ParameterSharedPtr chkP;
                std::vector<ParamAssignInfo *>::iterator it
                                        = info->mParamAssign.begin();
                std::vector<ParamAssignInfo *>::iterator end
                                        = info->mParamAssign.end();
                for(; it != end; ++it )
                {
                    ParamAssignInfo *pA = *it;
                    chkP = instance->getParameters()->get(
                                                context, pA->mNameData->mName );
                    if( !chkP )
                    {
                        //CAN ONLY OVERRIDE EXISTING PARAMS
                        std::string message = constructErrorMessage(
                                "Can only override existing params");
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                    if( pA->mValueData->mValues.size()
                                            != chkP->getSize() )
                    {
                        //SIZE MISMATCH
                        std::string message = constructErrorMessage(
                                "Params size mismatch : %d with %d",
                                    pA->mValueData->mValues.size(), chkP->getSize());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
#if 0
                    if( pA->mValueData->mValues[0].getType()
                            != chkP->getValue().getType() )
                    {
                        std::string message
                                        = constructErrorMessage(
                                    "Type mismatch during assignment for param %s",
                                    pA->mNameData->mName.c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
#endif
                    AssignedParamCreator creator(
                        instance, instance->getParameterContext(),
                        instance->getParameters(), ctx->getFactory(),
                        pA->mValueData->mValues );
                    try
                    {
                        if( !pA->mNameData->mIndices.empty() )
                        {
                            //ASSIGNMENT TO A BIT
                            chkP = chkP->get(
                                        pA->mNameData->mIndices );
                        }
                        chkP->accept( creator );
                    }
                    catch( Error &e )
                    {
                        std::string message = constructErrorMessage(
                                "Unable to assign param: %s",
                                            chkP->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                    log("Param %s overridden at context %lu\n",
                                        pA->mNameData->mName.c_str(),
                                        context);
                    ctx->setIsInInstance( false );
                }

                //Properties
                std::vector<PropertySharedPtr> props;
                PropertySharedPtr property
                            = ctx->getCurrentProperty();
                while( property )
                {
                    props.push_back( property );
                    ctx->popProperty();
                    property = ctx->getCurrentProperty();
                }
                for( std::vector<PropertySharedPtr>::reverse_iterator
                    it = props.rbegin(); it != props.rend(); ++it )
                {
                    PropertySharedPtr property = *it;
                    instance->setProperty(
                            property->getName(), property );
                }
                inDriver.getContext()->setIsInInstance( false );
                //Handle PortInst
                for( std::vector<PortInstData *>::iterator it
                        = info->mPortInst.begin();
                        it != info->mPortInst.end(); ++it )
                {
                    PortInstData *instData = *it;
                    std::vector< std::string > nestedNames;
                    PortRefData *portData = instData->mRefData;
                    PortRefData *currPortRef = portData;
                    PortRefData *nextPortRef
                                    = currPortRef->mParentPort;
                    nestedNames.push_back(
                                    currPortRef->mPortName->mName);
                    while( nextPortRef )
                    {
                        currPortRef = nextPortRef;
                        nestedNames.push_back( nextPortRef->mPortName->mName );
                        nextPortRef = currPortRef->mParentPort;
                    }
                    NameData *portNameData = currPortRef->mPortName;
                    std::string topName = *(nestedNames.rbegin());
                    PortReferenceSharedPtr portRef
                            = instance->findPortReference( topName );
                    if( !portRef )
                    {
                        std::string message = constructErrorMessage(
                                "No port ref %s found",
                                    portNameData->mName.c_str());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                    try
                    {
                        connectibleFinder( nestedNames, portRef );
                        if( !portData->mPortName->mIndices.empty() )
                        {
                            portRef = portRef->get(
                                        portData->mPortName->mIndices );
                        }
                        portRef->setAttributes(
                                PortAttributesSharedPtr(
                                    instData->mAttribute) );
                    }
                    catch( Error &e )
                    {
                        std::string message = constructErrorMessage(
                                "Unable to connect port ref : %s", portRef->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                }
            }
         ;

_Instance :     ViewRefData
            {
                $$ = new InstanceInfo();
                $$->mViewRefData = $1;
            }
          |     ViewList
            {
                $$ = NULL;
            }
          |     _Instance Transform
            {
                $$ = $1;
            }
          |     _Instance ParamAssign
            {
                $$ = ($1)?$1:new InstanceInfo();
                $$->mParamAssign.push_back( $2 );
            }
          |     _Instance PortInst
            {
                $$ = ($1)?$1:new InstanceInfo();
                $$->mPortInst.push_back( $2 );
            }
          |     _Instance Timing
            {
                $$ = $1;
            }
          |     _Instance Designator
            {
                $$ = $1;
            }
          |     _Instance Property
            {
                $$ = $1;
            }
          |     _Instance Comment
            {
                $$ = $1;
            }
          |     _Instance UserData
            {
                $$ = $1;
            }
          ;

InstanceRef : LBKT INSTANCEREF InstNameRef _InstanceRef RBKT

_InstanceRef :
             |  InstanceRef
             |  ViewRefData
             ;

InstanceRefData : LBKT INSTANCEREF InstNameRefData _InstanceRefData RBKT
                {
                    $$ = ( $4 ) ? $4 : new InstanceRefData();
                    $$->mName = $3;
                }
            ;

_InstanceRefData :
                {
                    $$ = NULL;
                }
             |  InstanceRef
                 {
                    $$ = NULL;
                }
             |  ViewRefData
                 {
                    $$ = new InstanceRefData();
                    $$->mView = $1;
                }
             ;

InstBackAn : LBKT INSTANCEBACKANNOTATE _InstBackAn RBKT
           ;

_InstBackAn :   InstanceRef
            |   _InstBackAn Designator
            |   _InstBackAn Timing
            |   _InstBackAn Property
            |   _InstBackAn Comment
            ;

InstGroup : LBKT INSTANCEGROUP _InstGroup RBKT
          ;

_InstGroup :
           |    _InstGroup InstanceRef
           ;

InstMap : LBKT INSTANCEMAP _InstMap RBKT
        ;

_InstMap :
         |      _InstMap InstanceRef
         |      _InstMap InstGroup
         |      _InstMap Comment
         |      _InstMap UserData
         ;

InstNameDef :   NameDefData
            |   ArrayData
            ;

InstNameRef :   NameRef
            |   Member
            ;

InstNameRefData :   NameRefData
            |   MemberData
            ;

IntDisplay : LBKT INTEGERDISPLAY _IntDisplay RBKT
            {
                $$ = $3;
            }
           ;

_IntDisplay :   Int
            {
                $$ = $1;
            }
            |   _IntDisplay Display
            {
                $$ = $1;
            }
            ;

Integer : LBKT INTEGER _Integer RBKT
            {
                if( $2 );
                if(  NULL == $3 )
                {
                    $$ = new ValueData();
                    $$->mValues.push_back( Value( 
                                Value::eValueTypeInteger ) );
                }
                else
                {
                    $$ = $3;
                }
            }
        ;

_Integer :
            {
                $$ = NULL;
            }
         |      _Integer Int
            {
                $$ = ( $1 ) ? $1 : new ValueData();
                $$->mValues.push_back( Value(
                            Value::eValueTypeInteger, $2 ) );
            }
         |      _Integer IntDisplay
            {
                $$ = ( $1 ) ? $1 : new ValueData();
                $$->mValues.push_back( Value(
                            Value::eValueTypeInteger, $2 ) );
            }
         |      _Integer Integer
         {
            $$ = $2;
            if( $1 )
            {
               $1->mValues.insert( $1->mValues.end(),
                       $$->mValues.begin(), $$->mValues.end() );
               $$->mValues = $1->mValues;
               delete $1;
            }
         }
         ;

Interface : LBKT INTERFACE _Interface RBKT
          ;

_Interface :
           |    _Interface Port
           |    _Interface PortBundle
           |    _Interface Symbol
           |    _Interface ProtectFrame
           |    _Interface ArrayRelInfo
           |    _Interface Parameter
           |    _Interface Joined
           |    _Interface MustJoin
           |    _Interface WeakJoined
           |    _Interface Permutable
           |    _Interface Timing
           |    _Interface Simulate
           |    _Interface Designator
           |    _Interface Property
           |    _Interface Comment
           |    _Interface UserData
           ;

InterFigGrp : LBKT INTERFIGUREGROUPSPACING RuleNameDef FigGrpObj FigGrpObj _InterFigGrp RBKT
            ;

_InterFigGrp :  Range
             |  SingleValSet
             |  _InterFigGrp Comment
             |  _InterFigGrp UserData
             ;

Intersection : LBKT INTERSECTION _Intersection RBKT
             ;

_Intersection : FigGrpRef
              | FigureOp
              | _Intersection FigGrpRef
              | _Intersection FigureOp
              ;

IntraFigGrp : LBKT INTRAFIGUREGROUPSPACING RuleNameDef FigGrpObj _IntraFigGrp RBKT
            ;

_IntraFigGrp :  Range
             |  SingleValSet
             |  _IntraFigGrp Comment
             |  _IntraFigGrp UserData
             ;

Inverse : LBKT INVERSE _Inverse RBKT
        ;

_Inverse :      FigGrpRef
         |      FigureOp
         ;

Isolated : LBKT ISOLATED RBKT
         ;

Joined : LBKT JOINED _Joined RBKT
       ;

_Joined :
        |   _Joined PortRef
        |   _Joined PortList
        |   _Joined GlobPortRef
        ;

JoinedElements : LBKT JOINED _JoinedElements RBKT
       ;

_JoinedElements :
        |   _JoinedElements PortRefData
            {
                std::string message;
                boost::scoped_ptr<PortRefData> portData( $2 );
                try
                {
                    connectElementToNet(
                            $2, inDriver.getContext(),
                            yyloc, message );
                }
                catch( Error &e )
                {
                    e.setCurrentLocation(
                        __FUNCTION__, __FILE__, __LINE__ );
                    inDriver.setParserError( e );
                    error( yyloc, message );
                    YYABORT;
                }
            }
        |   _JoinedElements PortListData
            {
                if( NULL != $2 && !$2->mPortRefs.empty() )
                {
                    boost::scoped_ptr<PortListData> portListData( $2 );
                    std::string message;
                    PortListSharedPtr portList;
                    inDriver.getContext()->getFactory()->create( portList );
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    for( std::vector<PortRefData *>::iterator it
                            = portListData->mPortRefs.begin();
                            it != portListData->mPortRefs.end(); ++it )
                    {
                        PortRefData *portData = *it;
                        std::vector< std::string > nestedNames;
                        PortRefData *currPortRef = *it;
                        PortRefData *nextPortRef = portData->mParentPort;
                        nestedNames.push_back(currPortRef->mPortName->mName);
                        while( nextPortRef )
                        {
                            currPortRef = nextPortRef;
                            nestedNames.push_back( nextPortRef->mPortName->mName );
                            nextPortRef = currPortRef->mParentPort;
                        }
                        bool isPort = ( NULL == currPortRef->mInstanceName );
                        NameData *portNameData = currPortRef->mPortName;
                        std::string topName = *(nestedNames.rbegin());
                        ViewSharedPtr view = ctx->getCurrentView();
                        if( isPort )
                        {
                            PortSharedPtr port = view->findPort( topName );
                            if( !port )
                            {
                                std::string message = constructErrorMessage(
                                        "No port %s found",
                                            portNameData->mName.c_str());
                                log("%s\n", message.c_str());
                                Error e( eMessageIdParserError,
                                    __FUNCTION__, __FILE__, __LINE__ );
                                e.saveContextData( "Parser error message", message );
                                e.saveContextData( "Filename", yyloc.begin.filename );
                                e.saveContextData( "StartLine", yyloc.begin.line );
                                e.saveContextData( "EndLine", yyloc.end.line );
                                inDriver.setParserError( e );
                                error( yyloc, message );
                                YYABORT;
                            }
                            try
                            {
                                connectibleFinder( nestedNames, port );
                                if( !portData->mPortName->mIndices.empty() )
                                {
                                    port = port->get( portData->mPortName->mIndices );
                                }
                                portList->addChildPort( port );
                            }
                            catch( Error &e )
                            {
                                std::string message = constructErrorMessage(
                                        "Unable to connect port : %s", port->getName().c_str());
                                log("%s\n", message.c_str());
                                e.setCurrentLocation( __FUNCTION__,
                                                        __FILE__, __LINE__ );
                                e.saveContextData( "Parser error message", message );
                                e.saveContextData( "Filename", yyloc.begin.filename );
                                e.saveContextData( "StartLine", yyloc.begin.line );
                                e.saveContextData( "EndLine", yyloc.end.line );
                                inDriver.setParserError( e );
                                error( yyloc, message );
                                YYABORT;
                            }
                        }
                        else
                        {
                            NameData *instanceName
                                            = currPortRef->mInstanceName;
                            InstanceSharedPtr instance
                                            = view->findInstance(
                                                        instanceName->mName );
                            if( !instance )
                            {
                                std::string message = constructErrorMessage(
                                        "Instance %s not known",
                                        instanceName->mName.c_str() );
                                log("%s\n", message.c_str());
                                Error e( eMessageIdParserError,
                                    __FUNCTION__, __FILE__, __LINE__ );
                                e.saveContextData( "Parser error message", message );
                                e.saveContextData( "Filename", yyloc.begin.filename );
                                e.saveContextData( "StartLine", yyloc.begin.line );
                                e.saveContextData( "EndLine", yyloc.end.line );
                                inDriver.setParserError( e );
                                error( yyloc, message );
                                YYABORT;
                            }
                            if( !instanceName->mIndices.empty() )
                            {
                                InstanceSharedPtr bit
                                        = instance->get( instanceName->mIndices );
                                instance = bit;
                            }
                            PortReferenceSharedPtr portRef
                                    = instance->findPortReference( topName );
                            if( !portRef )
                            {
                                std::string message = constructErrorMessage(
                                        "No port ref %s found",
                                            portNameData->mName.c_str());
                                log("%s\n", message.c_str());
                                Error e( eMessageIdParserError,
                                    __FUNCTION__, __FILE__, __LINE__ );
                                e.saveContextData( "Parser error message", message );
                                e.saveContextData( "Filename", yyloc.begin.filename );
                                e.saveContextData( "StartLine", yyloc.begin.line );
                                e.saveContextData( "EndLine", yyloc.end.line );
                                inDriver.setParserError( e );
                                error( yyloc, message );
                                YYABORT;
                            }
                            try
                            {
                                connectibleFinder( nestedNames, portRef );
                                if( !portData->mPortName->mIndices.empty() )
                                {
                                    portRef = portRef->get( portData->mPortName->mIndices );
                                }
                                portList->addChildPortReference( portRef );
                            }
                            catch( Error &e )
                            {
                                std::string message = constructErrorMessage(
                                        "Unable to connect port ref : %s", portRef->getName().c_str());
                                log("%s\n", message.c_str());
                                e.setCurrentLocation( __FUNCTION__,
                                                        __FILE__, __LINE__ );
                                e.saveContextData( "Parser error message", message );
                                e.saveContextData( "Filename", yyloc.begin.filename );
                                e.saveContextData( "StartLine", yyloc.begin.line );
                                e.saveContextData( "EndLine", yyloc.end.line );
                                inDriver.setParserError( e );
                                error( yyloc, message );
                                YYABORT;
                            }
                        }
                    }
                    try
                    {
                        portList->connect( ctx->getCurrentNet() );
                    }
                    catch( Error &e )
                    {
                        e.setCurrentLocation(
                            __FUNCTION__, __FILE__, __LINE__ );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                }
            }
        |   _JoinedElements GlobPortRefData
            {
                boost::scoped_ptr<NameData> globPortRef( $2 );
                std::string name = globPortRef->mName;
                //Find ports of this name
                EdifContextSharedPtr ctx = inDriver.getContext();
                ViewSharedPtr view = ctx->getCurrentView();
                NetSharedPtr net = ctx->getCurrentNet();
                size_t netSize = net->getSize();
                PortSharedPtr port = view->findPort( name );
                if( port )
                {
                    try
                    {
                        port->connect( net );
                    }
                    catch( Error &e )
                    {
                        std::string message = constructErrorMessage(
                                "Unable to connect port : %s", port->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                }
                std::vector< InstanceSharedPtr > leafInstances;
                {
                    std::vector< InstanceSharedPtr > instances;
                    view->getInstances( instances );
                    for( std::vector< InstanceSharedPtr >::iterator it
                                = instances.begin(); it != instances.end(); ++it )
                    {
                        if( eCompositionTypeScalar
                            == (*it)->getCompositionType() )
                        {
                            leafInstances.push_back( *it );
                        }
                        else
                        {
                            (*it)->getChildren( leafInstances );
                        }
                    }
                }
                for( std::vector< InstanceSharedPtr >::iterator it = leafInstances.begin();
                        it != leafInstances.end(); ++it )
                {
                    PortReferenceSharedPtr pRef
                                 = (*it)->findPortReference( name );
                    if( pRef
                            && netSize == pRef->getSize() )
                    {
                        try
                        {
                            pRef->connect( net );
                        }
                        catch( Error &e )
                        {
                            std::string message = constructErrorMessage(
                                    "Unable to connect port : %s", pRef->getName().c_str());
                            log("%s\n", message.c_str());
                            e.setCurrentLocation( __FUNCTION__,
                                                    __FILE__, __LINE__ );
                            e.saveContextData( "Parser error message", message );
                            e.saveContextData( "Filename", yyloc.begin.filename );
                            e.saveContextData( "StartLine", yyloc.begin.line );
                            e.saveContextData( "EndLine", yyloc.end.line );
                            inDriver.setParserError( e );
                            error( yyloc, message );
                            YYABORT;
                        }
                    }
                }
            }
        ;

Justify : LBKT JUSTIFY _Justify RBKT
        ;

_Justify :      CENTERCENTER
         |      CENTERLEFT
         |      CENTERRIGHT
         |      LOWERCENTER
         |      LOWERLEFT
         |      LOWERRIGHT
         |      UPPERCENTER
         |      UPPERLEFT
         |      UPPERRIGHT
         ;

KeywordDisp : LBKT KEYWORDDISPLAY _KeywordDisp RBKT
            ;

_KeywordDisp :  KeywordName
             |  _KeywordDisp Display
             ;

KeywordLevel : LBKT KEYWORDLEVEL Int RBKT
                {
                    if( $3 );
                }
             ;

KeywordMap : LBKT KEYWORDMAP _KeywordMap RBKT
           ;

_KeywordMap :   KeywordLevel
            |   _KeywordMap Comment
            ;

KeywordName :   Ident
            {
                delete $1;
            }
            ;

LayerNameDef :  NameDef
             ;

LessThan : LBKT LESSTHAN ScaledInt RBKT
            {
                delete $3;
            }
         ;

LibNameDef :    NameDefData
           ;

LibNameRef:    NameRef
            ;

LibNameRefData :    NameRefData
            {
                $$ = $1;
            }
           ;

Library :  LBKT LIBRARY LibNameDef EdifLevel
        {
            if( $3 )
            {
                EdifContextSharedPtr ctx
                                            = inDriver.getContext();
                RootSharedPtr root = ctx->getRoot();
                LibrarySharedPtr lib;
                // We do not create a new extern lib when one
                // exists already
                lib = root->findLibrary( $3->mName );
                if( lib )
                {
                    log("Skipped creation of existing lib.\n");
                }
                EdifLevel level;
                switch( $4 )
                {
                    case 0:
                    {
                        level = eEdifLevel0;
                        break;
                    }
                    case 1:
                    {
                        level = eEdifLevel1;
                        break;
                    }
                    case 2:
                    {
                        level = eEdifLevel2;
                        break;
                    }
                    default:
                    {
                        std::string message = constructErrorMessage(
                                "Edif level %s is not supported", $4 );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                }
                if( lib )
                {
                    if( lib->getLevel() != level )
                    {
                        std::string message = constructErrorMessage(
                                "Level mismatch - old %d new %d\n",
                                    lib->getLevel(), level );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                    else
                    {
                        if( lib->getIsExtern() )
                        {
                            log("Exising lib was extern .. changing\n");
                            lib->setIsExtern( false );
                        }
                    }
                }
                else
                {
                    try
                    {
                        ctx->getFactory()->create( lib );
                    }
                    catch( Error &e )
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create lib : %s", lib->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                    lib->setName( $3->mName );
                    if( ! $3->mOriginalName.empty() )
                    {
                        lib->setOriginalName( $3->mOriginalName );
                    }
                    lib->setLevel( level );
                    lib->setParent( root );
                    root->addLibrary( lib );
                    log("Created lib %s level %d\n",
                                lib->getName().c_str(), level );
                }
                ctx->setCurrentLibrary( lib );
                delete $3;
            }
            else
            {
                //TBD::POSSIBLE?
            }
        } _Library RBKT
        {
            inDriver.getContext()->setCurrentLibrary(
                                    LibrarySharedPtr() );
        }
        ;

_Library :      Technology
         |      _Library Status
         |      _Library Cell
         |      _Library Comment
         |      _Library UserData
         ;

LibraryRef : LBKT LIBRARYREF LibNameRef RBKT
           ;

LibraryRefData : LBKT LIBRARYREF LibNameRefData RBKT
            {
                $$ = $3;
            }
           ;

ListOfNets : LBKT LISTOFNETS _ListOfNets RBKT
           ;

_ListOfNets :
            |   _ListOfNets Net
            ;

ListOfPorts : LBKT LISTOFPORTS _ListOfPorts RBKT
            ;

_ListOfPorts :
             | _ListOfPorts Port
             |  _ListOfPorts PortBundle
             ;

LoadDelayData : LBKT LOADDELAY _LoadDelayData _LoadDelayData RBKT
            {
                boost::scoped_ptr<ValueData> delay( $3 );
                boost::scoped_ptr<ValueData> acLoad( $4 );
                $$ = new PortDelay();
                $$->setType( PortDelay::eTypeLoadDelay );
                $$->setDelay(
                        delay->mValues[0].get<Value::MiNoMax>() );
                $$->setAcLoad(
                        acLoad->mValues[0].get<Value::MiNoMax>() );
            }
          ;

_LoadDelayData :    MiNoMaValue
           |    MiNoMaDisp
           ;

LoadDelay : LBKT LOADDELAY _LoadDelay _LoadDelay RBKT
          ;

_LoadDelay :    MiNoMaValue
           |    MiNoMaDisp
           ;

LogicAssn : LBKT LOGICASSIGN ___LogicAssn __LogicAssn _LogicAssn RBKT
          ;

___LogicAssn :  PortNameRef
             |  PortRef
             ;

__LogicAssn :   PortRef
            |   LogicRef
            |   Table
            ;

_LogicAssn :
           |    Delay
           |    LoadDelay
           ;

LogicIn : LBKT LOGICINPUT _LogicIn RBKT
        ;

_LogicIn :      PortList
         |      PortRef
         |      PortNameRef
         |      _LogicIn LogicWave
         ;

LogicList : LBKT LOGICLIST _LogicList RBKT
          ;

_LogicList :
           |    _LogicList LogicNameRef
           |    _LogicList LogicOneOf
           |    _LogicList Ignore
           ;

LogicMapIn : LBKT LOGICMAPINPUT _LogicMapIn RBKT
           ;

_LogicMapIn :
            |   _LogicMapIn LogicNameRef
            ;

LogicMapOut : LBKT LOGICMAPOUTPUT _LogicMapOut RBKT
            ;

_LogicMapOut :
             |  _LogicMapOut LogicNameRef
             ;

LogicNameDef :  NameDef
             ;

LogicNameRef :  NameRef
             ;

LogicOneOf : LBKT LOGICONEOF _LogicOneOf RBKT
           ;

_LogicOneOf :
            |   _LogicOneOf LogicNameRef
            |   _LogicOneOf LogicList
            ;

LogicOut : LBKT LOGICOUTPUT _LogicOut RBKT
         ;

_LogicOut :     PortList
          |     PortRef
          |     PortNameRef
          |     _LogicOut LogicWave
          ;

LogicPort : LBKT LOGICPORT _LogicPort RBKT
          ;

_LogicPort :    PortNameDef
           |    _LogicPort Property
           |    _LogicPort Comment
           |    _LogicPort UserData
           ;

LogicRef : LBKT LOGICREF LogicNameRef _LogicRef RBKT
         ;

_LogicRef :
          |     LibraryRef
          ;

LogicValue : LBKT LOGICVALUE _LogicValue RBKT
           ;

_LogicValue :   LogicNameDef
            |   _LogicValue VoltageMap
            |   _LogicValue CurrentMap
            |   _LogicValue BooleanMap
            |   _LogicValue Compound
            |   _LogicValue Weak
            |   _LogicValue Strong
            |   _LogicValue Dominates
            |   _LogicValue LogicMapOut
            |   _LogicValue LogicMapIn
            |   _LogicValue Isolated
            |   _LogicValue Resolves
            |   _LogicValue Property
            |   _LogicValue Comment
            |   _LogicValue UserData
            ;

LogicWave : LBKT LOGICWAVEFORM _LogicWave RBKT
          ;

_LogicWave :
           |    _LogicWave LogicNameRef
           |    _LogicWave LogicList
           |    _LogicWave LogicOneOf
           |    _LogicWave Ignore
           ;

Maintain : LBKT MAINTAIN __Maintain _Maintain RBKT
         ;

__Maintain :    PortNameRef
           |    PortRef
           ;

_Maintain :
          |     Delay
          |     LoadDelay
          ;

Match : LBKT MATCH __Match _Match RBKT
      ;

__Match :       PortNameRef
        |       PortRef
        |       PortList
        ;

_Match :        LogicNameRef
       |        LogicList
       |        LogicOneOf
       ;

MemberData : LBKT MEMBER NameRefData Int _MemberData RBKT
        {
            $$ = $3;
            $$->mIndices.push_back( $4 );
            if( $5 )
            {
                $$->mIndices.insert( $$->mIndices.end(),
                                    $5->begin(), $5->end() );
                delete $5;
            }
        }
      ;

_MemberData :
        {
            $$ = NULL;
        }
        |      _IntegerList
        ;

Member : LBKT MEMBER NameRef Int _Member RBKT
        {
            if( $4 );
            delete $3;
        }
      ;

_Member :
        |      _IntegerList
        {
            delete $1;
        }
        ;

MiNoMa : LBKT MINOMAX _MiNoMa RBKT
        {
            if(  NULL == $3 )
            {
                $$ = new ValueData();
                $$->mValues.push_back( Value(
                        Value::eValueTypeMiNoMax ) );
            }
            else
            {
                $$ = $3;
            }
        }
       ;

_MiNoMa :
            {
                $$ = NULL;
            }
        |       _MiNoMa MiNoMaValue
            {
                $$ = ( $1 ) ? $1 : new ValueData();
                $$->mValues.push_back( Value(
                            Value::eValueTypeMiNoMax,
                            $2->mValues[0].get<Value::MiNoMax>() ) );
                delete $2;
            }
        |       _MiNoMa MiNoMaDisp
            {
                $$ = ( $1 ) ? $1 : new ValueData();
                $$->mValues.push_back( Value(
                            Value::eValueTypeMiNoMax,
                            $2->mValues[0].get<Value::MiNoMax>() ) );
                delete $2;
            }
        |       _MiNoMa MiNoMa
         {
            $$ = $2;
            if( $1 )
            {
               $1->mValues.insert( $1->mValues.end(),
                       $$->mValues.begin(), $$->mValues.end() );
               $$->mValues = $1->mValues;
               delete $1;
            }
         }
        ;

MiNoMaDisp : LBKT MINOMAXDISPLAY _MiNoMaDisp RBKT
            {
                $$ = $3;
            }
           ;

_MiNoMaDisp :   MiNoMaValue
            {
                $$ = $1;
            }
            |   _MiNoMaDisp Display
            {
                $$ = $1;
            }
            ;

MiNoMaValue :   Mnm
            {
                $$ = $1;
            }
            |   ScaledInt
            {
                Value::MiNoMax val;
                val.setNominal( $1->mX );
                $$ = new ValueData();
                $$->mValues.push_back(
                        Value( Value::eValueTypeMiNoMax, val ) );
                delete $1;
            }
            ;

Mnm : LBKT MNM _Mnm _Mnm _Mnm RBKT
    {
        Value::MiNoMax val;
        if( $3 )
        {
            val.setMin( $3->mX );
        }
        if( $4 )
        {
            val.setNominal( $4->mX );
        }
        if( $5 )
        {
            val.setMax( $5->mX );
        }
        $$ = new ValueData();
        $$->mValues.push_back(
                        Value( Value::eValueTypeMiNoMax, val ) );
        delete $3;
        delete $4;
        delete $5;
    }
    ;

_Mnm :          ScaledInt
     |          Undefined
         {
            $$ = NULL;
        }
     |          Unconstrained
         {
            $$ = NULL;
        }
     ;

MultValSet : LBKT MULTIPLEVALUESET _MultValSet RBKT
           ;

_MultValSet :
            |   _MultValSet RangeVector
            ;

MustJoin : LBKT MUSTJOIN _MustJoin RBKT
         ;

_MustJoin :
          |     _MustJoin PortRef
          |     _MustJoin PortList
          |     _MustJoin WeakJoined
          |     _MustJoin Joined
          ;

Name : LBKT NAME _Name RBKT
        {
            $$ = $3;
        }
     ;

_Name : Ident
        {
            $$ = new NameData();
            $$->mName = *$1;
            delete $1;
        }
      |         _Name Display
      ;

NameDef : Ident
        {
            delete $1;
        }
        |       Name
        {
            delete $1;
        }
        |       Rename
        {
            delete $1;
        }
        ;

NameDefData : Ident
        {
            $$ = new NameData();
            $$->mName = *$1;
            delete $1;
        }
        |       Name
        {
            $$ = $1;
        }
        |       Rename
        {
            $$ = $1;
        }
        ;

NameRef :       Ident
        {
            $$ = new NameData();
            $$->mName = *$1;
            delete $1;
        }
        |       Name
        {
            $$ = $1;
        }
        ;

NameRefData :       Ident
        {
            $$ = new NameData();
            $$->mName = *$1;
            delete $1;
        }
        |       Name
        {
            $$ = $1;
        }
        ;

Net : LBKT NET NetNameDef
        {
            boost::scoped_ptr<NameData> pNameData( $3 );
            std::string name = pNameData->mName;
            EdifContextSharedPtr ctx = inDriver.getContext();
            NetSharedPtr net;
            ViewSharedPtr view = ctx->getCurrentView();
            NetBundleSharedPtr bundle
                                = ctx->getCurrentNetBundleContext();
            bool bExistingNetFound = false;
            if( bundle )
            {
                typedef std::vector<NetSharedPtr > NBList;
                NBList children;
                bundle->getChildren( children );
                for( NBList::iterator it = children.begin();
                        it != children.end(); it++ )
                {
                    if( (*it)->getName() == name )
                    {
                        bExistingNetFound = true;
                    }
                }
            }
            else
            {
                net  = view->findNet( name );
                bExistingNetFound = false;
            }
            if( bExistingNetFound )
            {
                std::string message = constructErrorMessage(
                    "View %s already contains a net of name %s\n",
                                view->getName().c_str(),
                                net->getName().c_str() );
                log("%s\n", message.c_str());
                Error e( eMessageIdParserError,
                    __FUNCTION__, __FILE__, __LINE__ );
                e.saveContextData( "Parser error message", message );
                e.saveContextData( "Filename", yyloc.begin.filename );
                e.saveContextData( "StartLine", yyloc.begin.line );
                e.saveContextData( "EndLine", yyloc.end.line );
                inDriver.setParserError( e );
                error( yyloc, message );
                YYABORT;
            }
            else
            {
                if( !pNameData->mIndices.empty() )
                {
                    try
                    {
                        VectorNetSharedPtr vectorNet;
                        ctx->getFactory()->create( vectorNet );
                        vectorNet->setName( name );
                        vectorNet->constructChildren(
                                            ctx->getFactory(),
                                            pNameData->mIndices );
                        net = vectorNet;
                    }
                    catch( Error &e )
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create net : %s", net->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                }
                else
                {
                    try
                    {
                        ScalarNetSharedPtr scalarNet;
                        ctx->getFactory()->create( scalarNet );
                        net = scalarNet;
                        net->setName( name );
                    }
                    catch( Error &e )
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create net : %s", net->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                }
                if( !pNameData->mOriginalName.empty() )
                {
                    net->setOriginalName(
                                    pNameData->mOriginalName );
                }
#if 0
                if( bundle )
                {
                    net->setParentCollection( bundle );
                    bundle->addChild( net );
                    log("Added net to existing bundle\n");
                }
                else
                {
                    ctx->pushCurrentNet( net );
                    NetSharedPtr currentNet = ctx->getCurrentNet();
                    ctx->popCurrentNet();

                    NetSharedPtr parentNet = ctx->getCurrentNet();
                    if( parentNet )
                    {
                        parentNet->addSubnet( currentNet );
                        printf("Current net %s ",
                            net->getName().c_str() );
                    }
                    else
                    {
                        view->addNet( net );
                        net->setParent( view );
                    }
                }
#endif
                if( pNameData->mIndices.empty() )
                {
                    log("Created scalar net %s ",
                            net->getName().c_str() );
                }
                else
                {
                    log("Created vector net %s with dimensions ",
                            net->getName().c_str() );
                    for( std::vector<size_t>::const_iterator it
                                    = pNameData->mIndices.begin();
                            it != pNameData->mIndices.end(); ++it )
                    {
                        log("%lu ", *it );
                    }
                }
                log("\n");
            }
            ctx->pushCurrentNet( net );
            ctx->pushPropertyContainer( net );
        } JoinedElements _Net RBKT
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            ViewSharedPtr view = ctx->getCurrentView();
            NetSharedPtr currentNet  = ctx->getCurrentNet();
            ctx->popCurrentNet();
            NetSharedPtr parentNet = ctx->getCurrentNet();

            NetBundleSharedPtr bundle
                                = ctx->getCurrentNetBundleContext();
            if( bundle )
            {
                if( parentNet )
                {
                    parentNet->addSubnet( currentNet );
                }
                else
                {
                    currentNet->setParentCollection( bundle );
                    bundle->addChild( currentNet );
                }
                log("Added net to existing bundle\n");
            }
            else
            {
                if( parentNet )
                {
                    parentNet->addSubnet( currentNet );
                }
                else
                {
                    view->addNet( currentNet );
                    currentNet->setParent( view );
                }
            }
            inDriver.getContext()->popPropertyContainer();
        }
        ;

_Net :          
     |          _Net Criticality
     |          _Net NetDelay
     |          _Net Figure
     |          _Net Net
     |          _Net Instance
     |          _Net CommGraph
     |          _Net Property
     |          _Net Comment
     |          _Net UserData
     ;

NetBackAn : LBKT NETBACKANNOTATE _NetBackAn RBKT
          ;

_NetBackAn :    NetRef
           |    _NetBackAn NetDelay
           |    _NetBackAn Criticality
           |    _NetBackAn Property
           |    _NetBackAn Comment
           ;

NetBundle : LBKT NETBUNDLE NetNameDef
            {
                std::string name = $3->mName;
                EdifContextSharedPtr ctx
                                                = inDriver.getContext();
                NetSharedPtr net;
                ViewSharedPtr view = ctx->getCurrentView();
                NetBundleSharedPtr bundle
                                = ctx->getCurrentNetBundleContext();
                if( bundle )
                {
                    typedef std::vector<NetSharedPtr > NBList;
                    NBList children;
                    bundle->getChildren( children );
                    for( NBList::iterator it = children.begin();
                            it != children.end(); it++ )
                    {
                        if( (*it)->getName() == name )
                        {
                            net = *it;
                        }
                    }
                }
                else
                {
                    net  = view->findNet( name );
                }
                if( net )
                {
                    log("Found existing net\n");
                    std::string message = constructErrorMessage(
                        "View %s already contains a net of name %s\n",
                                    view->getName().c_str(),
                                    net->getName().c_str() );
                    Error e( eMessageIdParserError,
                        __FUNCTION__, __FILE__, __LINE__ );
                    e.saveContextData( "Parser error message", message );
                    e.saveContextData( "Filename", yyloc.begin.filename );
                    e.saveContextData( "StartLine", yyloc.begin.line );
                    e.saveContextData( "EndLine", yyloc.end.line );
                    inDriver.setParserError( e );
                    error( yyloc, message );
                    YYABORT;
                }
                else
                {
                    try
                    {
                        NetBundleSharedPtr netBundle;
                        ctx->getFactory()->create( netBundle );
                        ctx->pushNetBundleContext( netBundle );
                        net = netBundle;
                    }
                    catch( Error &e )
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create net bundle : %s", net->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                    net->setName( name );
                    net->setParent( view );
                    if( !$3->mOriginalName.empty() )
                    {
                        net->setOriginalName( $3->mOriginalName );
                    }
                    if( bundle )
                    {
                        net->setParentCollection( bundle );
                        bundle->addChild( net );
                        log("Added net to existing bundle\n");
                    }
                    else
                    {
                        view->addNet( net );
                    }
                    log( "Created net bundle %s\n",
                                            net->getName().c_str() );
                }
                //ctx->pushCurrentNet( net );
                delete $3;
            } _NetBundle RBKT
            {
                EdifContextSharedPtr ctx
                                                = inDriver.getContext();
                //ctx->popCurrentNet();
                ctx->popNetBundleContext();
            }
          ;

_NetBundle :    ListOfNets
           |    _NetBundle Figure
           |    _NetBundle CommGraph
           |    _NetBundle Property
           |    _NetBundle Comment
           |    _NetBundle UserData
           ;

NetDelay : LBKT NETDELAY Derivation _NetDelay RBKT
         ;

_NetDelay :     Delay
          |     _NetDelay Transition
          |     _NetDelay Becomes
          ;

NetGroup : LBKT NETGROUP _NetGroup RBKT
         ;

_NetGroup :
          |     _NetGroup NetNameRef
          |     _NetGroup NetRef
          ;

NetMap :  LBKT NETMAP _NetMap RBKT
       ;

_NetMap :
        |       _NetMap NetRef
        |       _NetMap NetGroup
        |       _NetMap Comment
        |       _NetMap UserData
        ;

NetNameDef :    NameDefData
           |    ArrayData
           ;

NetNameRef :    NameRef
           |    Member
           ;

NetRef : LBKT NETREF NetNameRef _NetRef RBKT
       ;

_NetRef :
        |       NetRef
        |       InstanceRef
        |       ViewRef
        ;

NoChange : LBKT NOCHANGE RBKT
         ;

NonPermut : LBKT NONPERMUTABLE _NonPermut RBKT
          ;

_NonPermut :
           |    _NonPermut PortRef
           |    _NonPermut Permutable
           ;

NotAllowed : LBKT NOTALLOWED RuleNameDef _NotAllowed RBKT
           ;

_NotAllowed :   FigGrpObj
            |   _NotAllowed Comment
            |   _NotAllowed UserData
            ;

NotchSpace : LBKT NOTCHSPACING RuleNameDef FigGrpObj _NotchSpace RBKT
           ;

_NotchSpace :   Range
            |   SingleValSet
            |   _NotchSpace Comment
            |   _NotchSpace UserData
            ;

Number : LBKT NUMBER _Number RBKT
        {
            if(  NULL == $3 )
            {
                $$ = new ValueData();
                $$->mValues.push_back( Value(
                        Value::eValueTypeNumber ) );
            }
            else
            {
                $$ = $3;
            }
        }
       ;

_Number :
         {
             $$ = NULL;
         }
        |       _Number ScaledInt
         {
             $$ = ( $1 ) ? $1 : new ValueData();
             $$->mValues.push_back( Value( Value::eValueTypeNumber,
                             Value::Number( $2->mX, $2->mY ) ) );
             delete $2;
         }
        |       _Number NumbDisplay
         {
             $$ = ( $1 ) ? $1 : new ValueData();
             $$->mValues.push_back( Value( Value::eValueTypeNumber,
                             Value::Number( $2->mX, $2->mY ) ) );
             delete $2;
         }
        |       _Number Number
         {
            $$ = $2;
            if( $1 )
            {
               $1->mValues.insert( $1->mValues.end(),
                       $$->mValues.begin(), $$->mValues.end() );
               $$->mValues = $1->mValues;
               delete $1;
            }
         }
        ;

NumbDisplay : LBKT NUMBERDISPLAY _NumbDisplay RBKT
            {
                $$ = $3;
            }
            ;

_NumbDisplay :  ScaledInt
                {
                    $$ = $1;
                }
             |  _NumbDisplay Display
                {
                    $$ = $1;
                }
             ;

NumberDefn : LBKT NUMBERDEFINITION _NumberDefn RBKT
           ;

_NumberDefn :
            |   _NumberDefn Scale
            |   _NumberDefn GridMap
            |   _NumberDefn Comment
            ;

OffPageConn : LBKT OFFPAGECONNECTOR _OffPageConn RBKT
            ;

_OffPageConn :  PortNameDef
             |  _OffPageConn Unused
             |  _OffPageConn Property
             |  _OffPageConn Comment
             |  _OffPageConn UserData
             ;

OffsetEvent : LBKT OFFSETEVENT Event ScaledInt RBKT
            {
                delete $4;
            }
            ;

OpenShape : LBKT OPENSHAPE _OpenShape RBKT
          ;

_OpenShape :    Curve
           |    _OpenShape Property
           ;

Orientation : LBKT ORIENTATION _Orientation RBKT
            ;

_Orientation :  R0
             |  R90
             |  R180
             |  R270
             |  MX
             |  MY
             |  MYR90
             |  MXR90
             ;

Origin : LBKT ORIGIN PointValue RBKT
        {
            delete $3;
        }
       ;

OverhngDist : LBKT  OVERHANGDISTANCE RuleNameDef FigGrpObj FigGrpObj _OverhngDist RBKT
            ;

_OverhngDist :  Range
             |  SingleValSet
             |  _OverhngDist Comment
             |  _OverhngDist UserData
             ;

OverlapDist : LBKT  OVERLAPDISTANCE RuleNameDef FigGrpObj FigGrpObj _OverlapDist RBKT
            ;

_OverlapDist :  Range
             |  SingleValSet
             |  _OverlapDist Comment
             |  _OverlapDist UserData
             ;

Oversize : LBKT OVERSIZE Int _Oversize CornerType RBKT
                {
                    if( $3 );
                }
         ;

_Oversize :     FigGrpRef
          |     FigureOp
          ;

Owner : LBKT OWNER Str RBKT
        {
            $$ = $3;
        }
      ;

Page : LBKT PAGE _Page RBKT
     ;

_Page :         InstNameDef
        {
            delete $1;
        }
      |         _Page Instance
      |         _Page Net
      |         _Page NetBundle
      |         _Page CommGraph
      |         _Page PortImpl
      |         _Page PageSize
      |         _Page BoundBox
      |         _Page Comment
      |         _Page UserData
      ;

PageSize : LBKT PAGESIZE_TORC Rectangle RBKT
         ;

ParamDisp : LBKT PARAMETERDISPLAY _ParamDisp RBKT
          ;

_ParamDisp :    ValueNameRef
           |    _ParamDisp Display
           ;

Parameter : LBKT PARAMETER ValueNameDefData TypedValue _Parameter RBKT
            {
                EdifContextSharedPtr ctx
                                            = inDriver.getContext();
                ObjectFactorySharedPtr factory
                                            = ctx->getFactory();
                boost::scoped_ptr<NameData> nameData( $3 );
                boost::scoped_ptr<ValueData> valueData( $4 );
                ParameterSharedPtr param;
                if( $3->mIndices.empty() )
                {
                    try
                    {
                        SingleParameterSharedPtr single;
                        factory->create( single );
                        param = single;
                        param->setName( nameData->mName );
                        if( !nameData->mOriginalName.empty() )
                        {
                            param->setOriginalName( nameData->mOriginalName );
                        } 
                        param->setValue( valueData->mValues[0] );
                    }
                    catch( Error &e )
                    {
                        e.setCurrentLocation(
                            __FUNCTION__, __FILE__, __LINE__ );
                        throw;
                    }
                }
                else
                {
                    size_t arraySize = accumulate(
                            nameData->mIndices.begin(),
                            nameData->mIndices.end(), 1,
                            std::multiplies<size_t>());
                    if( valueData->mValues.size() != arraySize )
                    {
                        std::string message = constructErrorMessage(
                                "Size mismatch %d with %d", valueData->mValues.size(),
                                 nameData->mIndices.size() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                    try
                    {
                        ParameterArraySharedPtr array;
                        factory->create( array );
                        array->setName( nameData->mName );
                        if( !nameData->mOriginalName.empty() )
                        {
                            array->setOriginalName( nameData->mOriginalName );
                        } 
                        array->constructChildren( factory,
                                                    $3->mIndices );
                        param = array;
                        std::vector< ParameterSharedPtr > children;
                        param->getChildren( children );
                        std::vector< ParameterSharedPtr >::iterator it = children.begin();
                        std::vector< ParameterSharedPtr >::iterator end = children.end();
                        size_t i = 0;
                        for(; it != end; ++it, i++ )
                        {
                            (*it)->setValue( valueData->mValues[i] );
                        }
                    }
                    catch( Error &e )
                    {
                        e.setCurrentLocation(
                            __FUNCTION__, __FILE__, __LINE__ );
                        throw;
                    }
                }
                param->setUnit( static_cast<Unit>( $5 ) );
                ViewSharedPtr view = ctx->getCurrentView();
                ParameterContext context
                                    = view->getParameterContext();
                view->getParameters()->set(
                            context, nameData->mName, param );
                log("New param %s created for context %lu\n",
                        nameData->mName.c_str(), context );
            }
          ;

_Parameter :
            {
                $$ = 0;
            }
           |    Unit
           ;

ParamAssign : LBKT PARAMETERASSIGN ValueNameRefData TypedValue RBKT
            {
                $$ = new ParamAssignInfo();
                $$->mNameData = $3;
                $$->mValueData = $4;
            }
            ;

Path : LBKT PATH _Path RBKT
     ;

_Path :         PointList
      |         _Path Property
      ;

PathDelay : LBKT PATHDELAY _PathDelay RBKT
          ;

_PathDelay :    Delay
           |    _PathDelay Event
           ;

PathWidth : LBKT PATHWIDTH Int RBKT
            {
                if( $3 );
            }
          ;

Permutable : LBKT PERMUTABLE _Permutable RBKT
           ;

_Permutable :
            |   _Permutable PortRef
            |   _Permutable Permutable
            |   _Permutable NonPermut
            ;

Plug : LBKT PLUG _Plug RBKT
     ;

_Plug :
      |         _Plug SocketSet
      ;

Point : LBKT POINT _Point RBKT
        {
            if(  NULL == $3 )
            {
                $$ = new ValueData();
                $$->mValues.push_back( Value(
                        Value::eValueTypePoint ) );
            }
            else
            {
                $$ = $3;
            }
        }
      ;

_Point :
        {
            $$ = NULL;
        }
       |        _Point PointValue
        {
            $$ = ( $1 ) ? $1 : new ValueData();
            $$->mValues.push_back( Value(
                        Value::eValueTypePoint,
                            Value::Point( $2->mX, $2->mY ) ) );
            delete $2;
        }
       |        _Point PointDisp
        {
            $$ = ( $1 ) ? $1 : new ValueData();
            $$->mValues.push_back( Value(
                        Value::eValueTypePoint,
                            Value::Point( $2->mX, $2->mY ) ) );
            delete $2;
        }
       |        _Point Point
         {
            $$ = $2;
            if( $1 )
            {
               $1->mValues.insert( $1->mValues.end(),
                       $$->mValues.begin(), $$->mValues.end() );
               $$->mValues = $1->mValues;
               delete $1;
            }
         }
       ;

PointDisp : LBKT POINTDISPLAY _PointDisp RBKT
            {
                $$ = $3;
            }
          ;

_PointDisp :    PointValue
            {
                $$ = $1;
            }
           |    _PointDisp Display
            {
                $$ = $1;
            }
           ;

PointList : LBKT POINTLIST _PointList RBKT
          ;

_PointList :
           |    _PointList PointValue
            {
                delete $2;
            }
           ;

PointValue : LBKT PT Int Int RBKT
            {
                $$ = new PairData( $3, $4 );
            }
           ;

Polygon : LBKT POLYGON _Polygon RBKT
        ;

_Polygon :      PointList
         |      _Polygon Property
         ;

Port : LBKT PORT PortNameDefData
        {
            boost::scoped_ptr<NameData> pNameData($3);
            std::string name = pNameData->mName;
            EdifContextSharedPtr ctx
                                            = inDriver.getContext();
            PortSharedPtr port;
            ViewSharedPtr view = ctx->getCurrentView();
            PortBundleSharedPtr bundle
                                    = ctx->getCurrentPortBundleContext();
            bool bExistingPortFound = false;
            if( bundle )
            {
                typedef std::vector<PortSharedPtr > PBList;
                PBList children;
                bundle->getChildren( children );
                for( PBList::iterator it = children.begin();
                        it != children.end(); it++ )
                {
                    if( (*it)->getName() == name )
                    {
                        bExistingPortFound = true;
                    }
                }
            }
            else
            {
                port = view->findPort( name );
                bExistingPortFound = port;
            }
            if( bExistingPortFound )
            {
                if( !ctx->getIsViewBeingLinked() )
                {
                    log("Existing port %s already found..Ignoring\n",
                            port->getName().c_str());
#if 0
                    std::string message = constructErrorMessage(
                        "View %s already contains a port of name %s\n",
                                    view->getName().c_str(),
                                    port->getName().c_str() );
                    log("%s\n", message.c_str());
                    Error e( eMessageIdParserError,
                        __FUNCTION__, __FILE__, __LINE__ );
                    e.saveContextData( "Parser error message", message );
                    e.saveContextData( "Filename", yyloc.begin.filename );
                    e.saveContextData( "StartLine", yyloc.begin.line );
                    e.saveContextData( "EndLine", yyloc.end.line );
                    inDriver.setParserError( e );
                    error( yyloc, message );
                    YYABORT;
#endif
                }
            }
            else
            {
                if( !pNameData->mIndices.empty() )
                {
                    try
                    {
                        VectorPortSharedPtr vectorPort;
                        ctx->getFactory()->create( vectorPort );
                        vectorPort->setName( name );
                        vectorPort->constructChildren(
                                ctx->getFactory(),
                                pNameData->mIndices );
                        port = vectorPort;
                    }
                    catch( Error &e )
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create vector port : %s", port->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                }
                else
                {
                    try
                    {
                        ScalarPortSharedPtr scalarPort;
                        ctx->getFactory()->create( scalarPort );
                        port = scalarPort;
                        port->setName( name );
                    }
                    catch( Error &e )
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create port : %s", port->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                }
                if( !pNameData->mOriginalName.empty() )
                {
                    port->setOriginalName(
                                    pNameData->mOriginalName );
                }
                port->setParent( view );
                if( bundle )
                {
                    port->setParentCollection( bundle );
                    bundle->addChild( port );
                    log("Added port to existing bundle\n");
                }
                else
                {
                    view->addPort( port );
                }
                if( pNameData->mIndices.empty() )
                {
                    log("Created scalar port %s ",
                            port->getName().c_str() );
                }
                else
                {
                    log("Created vector port %s with dimensions ",
                            port->getName().c_str() );
                    for( std::vector<size_t>::const_iterator it
                                    = pNameData->mIndices.begin();
                                it != pNameData->mIndices.end(); ++it )
                    {
                        log("%lu ", *it );
                    }
                }
                log("\n");
            }
            ctx->setCurrentPort( port );
            ctx->pushPropertyContainer( port );
        } _Port RBKT
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            if( $5 )
            {
                boost::shared_ptr<PortAttributes> attrib( $5 );
                ctx->getCurrentPort()->setAttributes( attrib );
            }
            ctx->setCurrentPort( PortSharedPtr() );
            ctx->popPropertyContainer();
        }
     ;

_Port :
                {
                    $$ = NULL;
                }
      |         _Port Direction
                {
                    $$ = $1;
                }
      |         _Port UnusedData
                {
                    $$ = ($1)?$1:new PortAttributes();
                    Value::Boolean unused = $2;
                    Value val( Value::eValueTypeBoolean, unused );
                    $$->setUnused( val );
                }
      |         _Port PortDelayData
                {
                    boost::scoped_ptr<PortDelay> delay( $2 );
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setPortDelay( *delay );
                }
      |         _Port DesignatorData
                {
                    $$ = ($1)?$1:new PortAttributes();
                    boost::scoped_ptr<std::string> data($2);
                    Value val( Value::eValueTypeString, *data );
                    $$->setDesignator( val );
                }
      |         _Port DcFanInLoad
                  {
                    boost::scoped_ptr<PairData> data($2);
                    Value::Number num( data->mX, data->mY );
                    Value val( Value::eValueTypeNumber, num );
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setDcFaninLoad( val );
                }
      |         _Port DcFanOutLoad
                  {
                    boost::scoped_ptr<PairData> data($2);
                    Value::Number num( data->mX, data->mY );
                    Value val( Value::eValueTypeNumber, num );
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setDcFanoutLoad( val );
                }
      |         _Port DcMaxFanIn
                  {
                    boost::scoped_ptr<PairData> data($2);
                    Value::Number num( data->mX, data->mY );
                    Value val( Value::eValueTypeNumber, num );
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setDcMaxFanin( val );
                }
      |         _Port DcMaxFanOut
                  {
                    boost::scoped_ptr<PairData> data($2);
                    Value::Number num( data->mX, data->mY );
                    Value val( Value::eValueTypeNumber, num );
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setDcMaxFanout( val );
                }
      |         _Port AcLoad
                  {
                    boost::scoped_ptr<ValueData> data($2);
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setAcLoad( data->mValues[0] );
                }
      |         _Port Property
                  {
                    $$ = $1;
                }
      |         _Port Comment
                  {
                    $$ = $1;
                }
      |         _Port UserData
                  {
                    $$ = $1;
                }
      ;

PortBackAn : LBKT PORTBACKANNOTATE PortRefData
            {
                boost::scoped_ptr<PortRefData> portData( $3 );
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                std::vector< std::string > nestedNames;
                PortRefData *currPortRef = $3;
                PortRefData *nextPortRef = portData->mParentPort;
                nestedNames.push_back(currPortRef->mPortName->mName);
                while( nextPortRef )
                {
                    currPortRef = nextPortRef;
                    nestedNames.push_back( nextPortRef->mPortName->mName );
                    nextPortRef = currPortRef->mParentPort;
                }
                bool isPort = ( NULL == currPortRef->mInstanceName );
                NameData *portNameData = currPortRef->mPortName;
                std::string topName = *(nestedNames.rbegin());
                CellSharedPtr cell = ctx->getCurrentCell();
                ViewSharedPtr view = cell->findView(
                                portData->mView->mViewName->mName );
                if( !view )
                {
                    std::string message = constructErrorMessage(
                            "No view %s found",
                        portData->mView->mViewName->mName.c_str());
                    log("%s\n", message.c_str());
                    Error e( eMessageIdParserError,
                        __FUNCTION__, __FILE__, __LINE__ );
                    e.saveContextData( "Parser error message", message );
                    e.saveContextData( "Filename", yyloc.begin.filename );
                    e.saveContextData( "StartLine", yyloc.begin.line );
                    e.saveContextData( "EndLine", yyloc.end.line );
                    inDriver.setParserError( e );
                    error( yyloc, message );
                    YYABORT;
                }
                if( isPort )
                {
                    PortSharedPtr port = view->findPort( topName );
                    if( !port )
                    {
                        std::string message = constructErrorMessage(
                                "No port %s found",
                                    portNameData->mName.c_str());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                    try
                    {
                        connectibleFinder( nestedNames, port );
                        if( !portData->mPortName->mIndices.empty() )
                        {
                            port = port->get( portData->mPortName->mIndices );
                        }
                        ctx->setCurrentPort( port );
                        ctx->pushPropertyContainer( port );
                    }
                    catch( Error &e )
                    {
                        e.setCurrentLocation(
                            __FUNCTION__, __FILE__, __LINE__ );
                        throw;
                    }
                }
                else
                {
                    NameData *instanceName
                                    = currPortRef->mInstanceName;
                    InstanceSharedPtr instance
                                    = view->findInstance(
                                                instanceName->mName );
                    if( !instance )
                    {
                        std::string message = constructErrorMessage(
                                "Instance %s not known",
                                instanceName->mName.c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                    if( !instanceName->mIndices.empty() )
                    {
                        InstanceSharedPtr bit
                                = instance->get( instanceName->mIndices );
                        instance = bit;
                    }
                    PortReferenceSharedPtr portRef
                            = instance->findPortReference( topName );
                    if( !portRef )
                    {
                        std::string message = constructErrorMessage(
                                "No port ref %s found",
                                    portNameData->mName.c_str());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                    try
                    {
                        connectibleFinder( nestedNames, portRef );
                        if( !portData->mPortName->mIndices.empty() )
                        {
                            portRef = portRef->get( portData->mPortName->mIndices );
                        }
                        ctx->setCurrentPortRef( portRef );
                        ctx->pushPropertyContainer( portRef );
                    }
                    catch( Error &e )
                    {
                        std::string message = constructErrorMessage(
                                "Unable to connect port ref : %s", portRef->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                }
            } _PortBackAn RBKT
            {
                EdifContextSharedPtr ctx
                            = inDriver.getContext();
                if( $5 )
                {
                    boost::shared_ptr<PortAttributes>
                                                attrib( $5 );
                    PortSharedPtr port
                                    = ctx->getCurrentPort();
                    if( port )
                    {
                        port->setAttributes( attrib );
                    }
                    else
                    {
                        ctx->getCurrentPortRef()
                                    ->setAttributes( attrib );
                    }
                }
                ctx->setCurrentPort( PortSharedPtr() );
                ctx->setCurrentPortRef(
                                PortReferenceSharedPtr() );
                ctx->popPropertyContainer();
            }
            ;

_PortBackAn :
            {
                $$ = NULL;
            }
      |         _PortBackAn UnusedData
                {
                    $$ = ($1)?$1:new PortAttributes();
                    Value::Boolean unused = $2;
                    Value val( Value::eValueTypeBoolean, unused );
                    $$->setUnused( val );
                }
      |         _PortBackAn PortDelayData
                {
                    boost::scoped_ptr<PortDelay> delay( $2 );
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setPortDelay( *delay );
                }
      |         _PortBackAn DesignatorData
                {
                    $$ = ($1)?$1:new PortAttributes();
                    boost::scoped_ptr<std::string> data($2);
                    Value val( Value::eValueTypeString, *data );
                    $$->setDesignator( val );
                }
      |         _PortBackAn DcFanInLoad
                  {
                    boost::scoped_ptr<PairData> data($2);
                    Value::Number num( data->mX, data->mY );
                    Value val( Value::eValueTypeNumber, num );
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setDcFaninLoad( val );
                }
      |         _PortBackAn DcFanOutLoad
                  {
                    boost::scoped_ptr<PairData> data($2);
                    Value::Number num( data->mX, data->mY );
                    Value val( Value::eValueTypeNumber, num );
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setDcFanoutLoad( val );
                }
      |         _PortBackAn DcMaxFanIn
                  {
                    boost::scoped_ptr<PairData> data($2);
                    Value::Number num( data->mX, data->mY );
                    Value val( Value::eValueTypeNumber, num );
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setDcMaxFanin( val );
                }
      |         _PortBackAn DcMaxFanOut
                  {
                    boost::scoped_ptr<PairData> data($2);
                    Value::Number num( data->mX, data->mY );
                    Value val( Value::eValueTypeNumber, num );
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setDcMaxFanout( val );
                }
      |         _PortBackAn AcLoad
                  {
                    boost::scoped_ptr<ValueData> data($2);
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setAcLoad( data->mValues[0] );
                }
       |   _PortBackAn Property
                {
                    $$ = $1;
                }
       |   _PortBackAn Comment
                {
                    $$ = $1;
                }
            ;

PortBundle : LBKT PORTBUNDLE NameDefData
        {
            boost::scoped_ptr<NameData> nameData( $3 );
            std::string name = nameData->mName;
            EdifContextSharedPtr ctx = inDriver.getContext();
            PortSharedPtr port;
            ViewSharedPtr view = ctx->getCurrentView();
            PortBundleSharedPtr bundle
                                = ctx->getCurrentPortBundleContext();
            bool bExistingPortFound = false;
            if( bundle )
            {
                typedef std::vector<PortSharedPtr > PBList;
                PBList children;
                bundle->getChildren( children );
                for( PBList::iterator it = children.begin();
                        it != children.end(); it++ )
                {
                    if( (*it)->getName() == name )
                    {
                        bExistingPortFound = true;
                    }
                }
            }
            else
            {
                port = view->findPort( name );
                bExistingPortFound = port;
            }
            if( bExistingPortFound )
            {
                if( !ctx->getIsViewBeingLinked() )
                {
                    std::string message = constructErrorMessage(
                        "View %s already contains a port of name %s\n",
                                    view->getName().c_str(),
                                    port->getName().c_str() );
                    log("%s\n", message.c_str());
                    Error e( eMessageIdParserError,
                        __FUNCTION__, __FILE__, __LINE__ );
                    e.saveContextData( "Parser error message", message );
                    e.saveContextData( "Filename", yyloc.begin.filename );
                    e.saveContextData( "StartLine", yyloc.begin.line );
                    e.saveContextData( "EndLine", yyloc.end.line );
                    inDriver.setParserError( e );
                    error( yyloc, message );
                    YYABORT;
                }
            }
            else
            {
                try
                {
                    PortBundleSharedPtr portBundle;
                    ctx->getFactory()->create( portBundle );
                    ctx->pushPortBundleContext( portBundle );
                    port = portBundle;
                }
                catch( Error &e )
                {
                    std::string message = constructErrorMessage(
                            "Unable to create port bundle : %s", port->getName().c_str());
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData( "Parser error message", message );
                    e.saveContextData( "Filename", yyloc.begin.filename );
                    e.saveContextData( "StartLine", yyloc.begin.line );
                    e.saveContextData( "EndLine", yyloc.end.line );
                    inDriver.setParserError( e );
                    error( yyloc, message );
                    YYABORT;
                }
                port->setName( name );
                if( !nameData->mOriginalName.empty() )
                {
                    port->setOriginalName( nameData->mOriginalName );
                }
                port->setParent( view );
                if( bundle )
                {
                    port->setParentCollection( bundle );
                    bundle->addChild( port );
                    log("Added port to existing bundle\n");
                }
                else
                {
                    view->addPort( port );
                }
                log( "Created port bundle %s\n",
                                        port->getName().c_str() );
            }
            ctx->setCurrentPort( port );
        } _PortBundle RBKT
        {
            inDriver.getContext()->popPortBundleContext();
            inDriver.getContext()->setCurrentPort(
                                        PortSharedPtr() );
        }
        ;

_PortBundle :   ListOfPorts
            |   _PortBundle Property
            |   _PortBundle Comment
            |   _PortBundle UserData
            ;

PortDelayData : LBKT PORTDELAY DerivationData _PortDelayData RBKT
            {
                $$ = $4;
                $$->setDerivation(
                        static_cast<PortDelay::Derivation>( $3 ) );
                inDriver.getLexer()->setAppendToBuffer( false );
                inDriver.getLexer()->resetBuffer();
            }
          ;

_PortDelayData :    DelayData
                {
                    inDriver.getLexer()->setAppendToBuffer( true );
                    inDriver.getLexer()->resetBuffer();
                    $$ = $1;
                }
           |    LoadDelayData
                {
                    inDriver.getLexer()->setAppendToBuffer( true );
                    inDriver.getLexer()->resetBuffer();
                    $$ = $1;
                }
           |    _PortDelayData Transition
                {
                    $$ = ( $1 ) ? $1 : new PortDelay();
                    $$->setTransition( inDriver.getLexer()->getBuffer() );
                    inDriver.getLexer()->resetBuffer();
                }
           |    _PortDelayData Becomes
                {
                    $$ = ( $1 ) ? $1 : new PortDelay();
                    $$->setBecomes( inDriver.getLexer()->getBuffer() );
                    inDriver.getLexer()->resetBuffer();
                }
           ;

PortGroup : LBKT PORTGROUP _PortGroup RBKT
          ;

_PortGroup :
           |    _PortGroup PortNameRef
           |    _PortGroup PortRef
           ;

PortImpl : LBKT PORTIMPLEMENTATION _PortImpl RBKT
         ;

_PortImpl :     PortRef
          |     PortNameRef
          |     _PortImpl ConnectLoc
          |     _PortImpl Figure
          |     _PortImpl Instance
          |     _PortImpl CommGraph
          |     _PortImpl PropDisplay
          |     _PortImpl KeywordDisp
          |     _PortImpl Property
          |     _PortImpl UserData
          |     _PortImpl Comment
          ;

PortInst : LBKT PORTINSTANCE _PortInst RBKT
            {
                $$ = $3;
            }
         ;

_PortInst : PortRefData
            {
                $$ = new PortInstData();
                $$->mRefData = $1;
            }
      |     PortNameRefData
            {
                $$ = new PortInstData();
                $$->mRefData = new PortRefData();
                $$->mRefData->mPortName = $1;
            }
      |     _PortInst UnusedData
            {
                $$ = ($1)?$1:new PortInstData();
                $$->mAttribute = ( $$->mAttribute )
                                ? $$->mAttribute
                                : new PortAttributes();
                Value::Boolean unused = $2;
                Value val( Value::eValueTypeBoolean, unused );
                $$->mAttribute->setUnused( val );
            }
      |     _PortInst PortDelayData
            {
                $$ = ($1)?$1:new PortInstData();
                $$->mAttribute = ( $$->mAttribute )
                                ? $$->mAttribute
                                : new PortAttributes();
                boost::scoped_ptr<PortDelay> delay( $2 );
                $$->mAttribute->setPortDelay( *delay );
            }
      |     _PortInst DesignatorData
            {
                $$ = ($1)?$1:new PortInstData();
                $$->mAttribute = ( $$->mAttribute )
                                ? $$->mAttribute
                                : new PortAttributes();
                boost::scoped_ptr<std::string> data($2);
                Value val( Value::eValueTypeString, *data );
                $$->mAttribute->setDesignator( val );
            }
      |     _PortInst DcFanInLoad
            {
                boost::scoped_ptr<PairData> data($2);
                Value::Number num( data->mX, data->mY );
                Value val( Value::eValueTypeNumber, num );
                $$ = ($1)?$1:new PortInstData();
                $$->mAttribute = ( $$->mAttribute )
                                ? $$->mAttribute
                                : new PortAttributes();
                $$->mAttribute->setDcFaninLoad( val );
            }
      |     _PortInst DcFanOutLoad
            {
                boost::scoped_ptr<PairData> data($2);
                Value::Number num( data->mX, data->mY );
                Value val( Value::eValueTypeNumber, num );
                $$ = ($1)?$1:new PortInstData();
                $$->mAttribute = ( $$->mAttribute )
                                ? $$->mAttribute
                                : new PortAttributes();
                $$->mAttribute->setDcFanoutLoad( val );
            }
      |     _PortInst DcMaxFanIn
            {
                boost::scoped_ptr<PairData> data($2);
                Value::Number num( data->mX, data->mY );
                Value val( Value::eValueTypeNumber, num );
                $$ = ($1)?$1:new PortInstData();
                $$->mAttribute = ( $$->mAttribute )
                                ? $$->mAttribute
                                : new PortAttributes();
                $$->mAttribute->setDcMaxFanin( val );
            }
      |     _PortInst DcMaxFanOut
            {
                boost::scoped_ptr<PairData> data($2);
                Value::Number num( data->mX, data->mY );
                Value val( Value::eValueTypeNumber, num );
                $$ = ($1)?$1:new PortInstData();
                $$->mAttribute = ( $$->mAttribute )
                                ? $$->mAttribute
                                : new PortAttributes();
                $$->mAttribute->setDcMaxFanout( val );
            }
      |     _PortInst AcLoad
            {
                boost::scoped_ptr<ValueData> data($2);
                $$ = ($1)?$1:new PortInstData();
                $$->mAttribute = ( $$->mAttribute )
                                ? $$->mAttribute
                                : new PortAttributes();
                $$->mAttribute->setAcLoad( data->mValues[0] );
            }
       |   _PortInst Property
            {
                $$ = $1;
            }
       |   _PortInst Comment
            {
                $$ = $1;
            }
       |   _PortInst UserData
            {
                $$ = $1;
            }
          ;

PortList : LBKT PORTLIST _PortList RBKT
         ;

_PortList :
          |     _PortList PortRef
          |     _PortList PortNameRef
          ;

PortListData : LBKT PORTLIST _PortListData RBKT
        {
            $$ = $3;
        }
         ;

_PortListData :
            {
                $$ = NULL;
            }
          |     _PortListData PortRefData
            {
                $$ = ($1)?$1:new PortListData();
                $$->mPortRefs.push_back( $2 );
            }
          |     _PortListData PortNameRefData
              {
                $$ = ($1)?$1:new PortListData();
                PortRefData *portRef = new PortRefData();
                portRef->mPortName = $2;
                $$->mPortRefs.push_back( portRef );
            }
          ;

PortListAls : LBKT PORTLISTALIAS PortNameDef PortList RBKT
            ;

PortMap : LBKT PORTMAP _PortMap RBKT
        ;

_PortMap :
         |      _PortMap PortRef
         |      _PortMap PortGroup
         |      _PortMap Comment
         |      _PortMap UserData
         ;

PortNameDef :   NameDef
            |   ArrayData
                {
                    delete $1;
                }
            ;

PortNameDefData :   NameDefData
            |   ArrayData
            ;

PortNameRef :   NameRef
            |   Member
            ;

PortNameRefData :   NameRefData
            |   MemberData
            ;

PortRef : LBKT PORTREF PortNameRef _PortRef RBKT

_PortRef :
         |      PortRef
         |      InstanceRef
         |      ViewRef
         ;

PortRefData : LBKT PORTREF PortNameRefData _PortRefData RBKT
            {
                PortRefData *pData = $4;
                if( pData )
                {
                    if( pData->mPortName )
                    {
                        $$ = new PortRefData();
                        $$->mParentPort = pData;
                        $$->mView = pData->mView;
                        pData->mView = NULL;
                    }
                    else
                    {
                        if( pData->mInstanceName
                             || pData->mView )
                        {
                            $$ = pData;
                        }
                    }
                }
                else
                {
                    $$ = new PortRefData();
                }
                $$->mPortName = $3;
            }
            ;

_PortRefData :
            {
                $$ = NULL;
            }
         |      PortRefData
             {
                $$ = $1;
            }
         |      InstanceRefData
             {
                $$ = new PortRefData();
                $$->mInstanceName = $1->mName;
                $$->mView = $1->mView;
                delete $1;
            }
         |      ViewRefData
            {
                $$ = new PortRefData();
                $$->mView = $1;
            }
         ;

Program : LBKT PROGRAM Str _Program RBKT
        {
            $$ = $3;
            //delete $4; TBD::FIX
        }
        ;

_Program :
            {
                //$$ = NULL;
            }
         |      Version
             {
                //$$ = $1;
                delete $1;
            }
         ;

PropDisplay : LBKT PROPERTYDISPLAY _PropDisplay RBKT
            ;

_PropDisplay :  PropNameRef
             |  _PropDisplay Display
             ;

Property : LBKT PROPERTY PropNameDef
            {
                boost::scoped_ptr<NameData> nameData($3);
                if( !nameData->mIndices.empty() )
                {
                    std::string message = constructErrorMessage(
                            "Property %s cannot be an array",
                            nameData->mName.c_str() );
                    log("%s\n", message.c_str());
                    Error e( eMessageIdParserError,
                        __FUNCTION__, __FILE__, __LINE__ );
                    e.saveContextData( "Parser error message", message );
                    e.saveContextData( "Filename", yyloc.begin.filename );
                    e.saveContextData( "StartLine", yyloc.begin.line );
                    e.saveContextData( "EndLine", yyloc.end.line );
                    inDriver.setParserError( e );
                    error( yyloc, message );
                    YYABORT;
                }
                PropertySharedPtr property;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                try
                {
                    ctx->getFactory()->create( property );
                }
                catch( Error &e )
                {
                    std::string message = constructErrorMessage(
                            "Unable to create Property : %s",
                            nameData->mName.c_str());
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData( "Parser error message", message );
                    e.saveContextData( "Filename", yyloc.begin.filename );
                    e.saveContextData( "StartLine", yyloc.begin.line );
                    e.saveContextData( "EndLine", yyloc.end.line );
                    inDriver.setParserError( e );
                    error( yyloc, message );
                    YYABORT;
                }
                property->setName( nameData->mName.c_str() );
                if( !nameData->mOriginalName.empty() )
                {
                    property->setOriginalName( nameData->mOriginalName );
                }
                ctx->pushProperty( property );
                ctx->incrementPropertyDepth();
            } _Property RBKT
            {
                EdifContextSharedPtr ctx
                                        = inDriver.getContext();
                PropertySharedPtr currentP
                                        = ctx->getCurrentProperty();
                ctx->popProperty();
                PropertySharedPtr parentP;
                if(ctx->getPropertyDepth() > 1 )
                {
                    parentP = ctx->getCurrentProperty();
                }
                ctx->decrementPropertyDepth();
                PropertyContainerSharedPtr container
                                =ctx->getCurrentPropertyContainer();
                if( parentP )
                {
                    parentP->addChildProperty(
                            currentP->getName(), currentP );
                }
                else
                {
                    if( ctx->getIsInInstance() )
                    {
                        //Can happen only in case of instances
                        ctx->pushProperty( currentP );
                    }
                    else
                    {
                        if( container )
                        {
                            container->setProperty(
                                    currentP->getName(), currentP );
                        }
                        else
                        {
                            log("Property %s is being discarded\n",
                                    currentP->getName().c_str());
                        }
                    }
                }
            }
         ;

_Property :     TypedValue
            {
                boost::scoped_ptr<ValueData> value( $1 );
                if( value->mValues.size() > 1 )
                {
                    std::string message = constructErrorMessage(
                            "Property value cannot be an array" );
                    log("%s\n", message.c_str());
                    Error e( eMessageIdParserError,
                        __FUNCTION__, __FILE__, __LINE__ );
                    e.saveContextData( "Parser error message", message );
                    e.saveContextData( "Filename", yyloc.begin.filename );
                    e.saveContextData( "StartLine", yyloc.begin.line );
                    e.saveContextData( "EndLine", yyloc.end.line );
                    inDriver.setParserError( e );
                    error( yyloc, message );
                    YYABORT;
                }
                inDriver.getContext()->getCurrentProperty()
                            ->setValue( value->mValues[0] );
            }
          |     _Property Owner
            {
                inDriver.getContext()->getCurrentProperty()
                                    ->setOwner( *$2 );
                delete $2;
            }
          |     _Property Unit
            {
                inDriver.getContext()->getCurrentProperty()
                                ->setUnit( static_cast<Unit>($2) );
            }
          |     _Property Property
          |     _Property Comment
          ;

PropNameDef :   NameDefData
            ;

PropNameRef :   NameRef
            ;

ProtectFrame : LBKT PROTECTIONFRAME _ProtectFrame RBKT
             ;

_ProtectFrame :
              | _ProtectFrame PortImpl
              | _ProtectFrame Figure
              | _ProtectFrame Instance
              | _ProtectFrame CommGraph
              | _ProtectFrame BoundBox
              | _ProtectFrame PropDisplay
              | _ProtectFrame KeywordDisp
              | _ProtectFrame ParamDisp
              | _ProtectFrame Property
              | _ProtectFrame Comment
              | _ProtectFrame UserData
              ;

Range :         LessThan
      |         GreaterThan
      |         AtMost
      |         AtLeast
      |         Exactly
      |         Between
      ;

RangeVector : LBKT RANGEVECTOR _RangeVector RBKT
            ;

_RangeVector :
             |  _RangeVector Range
             |  _RangeVector SingleValSet
             ;

Rectangle : LBKT RECTANGLE PointValue _Rectangle RBKT
            {
                delete $3;
            }
          ;

_Rectangle :    PointValue
            {
                delete $1;
            }
           |    _Rectangle Property
           ;

RectSize : LBKT RECTANGLESIZE RuleNameDef FigGrpObj _RectSize RBKT
         ;

_RectSize :     RangeVector
          |     MultValSet
          |     _RectSize Comment
          |     _RectSize UserData
          ;

Rename : LBKT RENAME __Rename _Rename RBKT
        {
            $$ = $3;
            $$->mOriginalName = *$4;
            delete $4;
        }
       ;

__Rename :  Ident
            {
                $$ = new NameData();
                $$->mName = *$1;
                delete $1;
            }
         |      Name
             {
                $$ = $1;
            }
         ;

_Rename :       Str
        {
            $$ = $1;
        }
        |       StrDisplay
        {
            $$ = $1;
        }
        ;

Resolves : LBKT RESOLVES _Resolves RBKT
         ;

_Resolves :
          |     _Resolves LogicNameRef
          ;

RuleNameDef :   NameDef
            ;

Scale : LBKT SCALE ScaledInt ScaledInt Unit RBKT
        {
            delete $3;
            delete $4;
            if( $5 );
        }
      ;

ScaledInt : Int
            {
                $$ = new PairData( $1 );
            }
          | LBKT E Int Int RBKT
            {
                $$ = new PairData( $3, $4 );
            }
          ;

ScaleX : LBKT SCALEX Int Int RBKT
        {
            if( $3 );
            if( $4 );
        }
       ;

ScaleY : LBKT SCALEY Int Int RBKT
        {
            if( $3 );
            if( $4 );
        }
       ;

Section : LBKT SECTION _Section RBKT
        ;

_Section :      Str
            {
                delete $1;
            }
         |      _Section Section
         |      _Section Str
             {
                delete $2;
            }
         |      _Section Instance
         ;

Shape : LBKT SHAPE _Shape RBKT
      ;

_Shape :        Curve
       |        _Shape Property
       ;

SimNameDef :    NameDef
           ;

Simulate : LBKT SIMULATE
        {
            boost::shared_ptr<View> currV = inDriver.getContext()->getCurrentView();
            if( currV && View::eTypeNetlist == currV->getType() )
            {
                inDriver.getLexer()->setAppendToBuffer( true );
                inDriver.getLexer()->resetBuffer();
            }
        } _Simulate RBKT
        {
            boost::shared_ptr<View> currV = inDriver.getContext()->getCurrentView();
            if( currV && View::eTypeNetlist == currV->getType() )
            {
                inDriver.getLexer()->setAppendToBuffer( false );
                log("%s\n", inDriver.getLexer()->getBuffer().c_str());
                inDriver.getLexer()->resetBuffer();
            }
        };

_Simulate :     SimNameDef
          |     _Simulate PortListAls
          |     _Simulate WaveValue
          |     _Simulate Apply
          |     _Simulate Comment
          |     _Simulate UserData
          ;

SimulInfo : LBKT SIMULATIONINFO _SimulInfo RBKT
          ;

_SimulInfo :
           |    _SimulInfo LogicValue
           |    _SimulInfo Comment
           |    _SimulInfo UserData
           ;

SingleValSet : LBKT SINGLEVALUESET _SingleValSet RBKT
             ;

_SingleValSet :
              | Range
              ;

Site : LBKT SITE ViewRef _Site RBKT
     ;

_Site :
      |         Transform
      ;

Socket : LBKT SOCKET _Socket RBKT
       ;

_Socket :
        |       Symmetry
        ;

SocketSet : LBKT SOCKETSET _SocketSet RBKT
          ;

_SocketSet :    Symmetry
           |    _SocketSet Site
           ;

Status : LBKT STATUS _Status RBKT
       ;

_Status :
        |       _Status Written
        |       _Status Comment
        |       _Status UserData
        ;

Steady : LBKT STEADY __Steady _Steady RBKT
       ;

__Steady :      PortNameRef
         |      PortRef
         |      PortList
         ;

_Steady :       Duration
        |       _Steady Transition
        |       _Steady Becomes
        ;

StrDisplay : LBKT STRINGDISPLAY _StrDisplay RBKT
            {
                $$ = $3;
            }
           ;

String : LBKT STRING _String RBKT
        {
            if( $2 );
            if(  NULL == $3 )
            {
                $$ = new ValueData();
                $$->mValues.push_back( Value(
                        Value::eValueTypeString ) );
            }
            else
            {
                $$ = $3;
            }
        }
       ;

_String :
            {
                $$ = NULL;
            }
        |       _String Str
            {
                $$ = ( $1 ) ? $1 : new ValueData();
                $$->mValues.push_back( Value(
                            Value::eValueTypeString, *$2 ) );
                delete $2;
            }
        |       _String StrDisplay
            {
                $$ = ( $1 ) ? $1 : new ValueData();
                $$->mValues.push_back( Value(
                            Value::eValueTypeString, *$2 ) );
                delete $2;
            }
        |       _String String
         {
            $$ = $2;
            if( $1 )
            {
               $1->mValues.insert( $1->mValues.end(),
                       $$->mValues.begin(), $$->mValues.end() );
               $$->mValues = $1->mValues;
               delete $1;
            }
         }
        ;

_StrDisplay :   Str
            {
                $$ = $1;
            }
            |   _StrDisplay Display
            {
                $$ = $1;
            }
            ;

Strong : LBKT STRONG LogicNameRef RBKT
       ;

Symbol : LBKT SYMBOL _Symbol RBKT
       ;

_Symbol :
        |       _Symbol PortImpl
        |       _Symbol Figure
        |       _Symbol Instance
        |       _Symbol CommGraph
        |       _Symbol Annotate
        |       _Symbol PageSize
        |       _Symbol BoundBox
        |       _Symbol PropDisplay
        |       _Symbol KeywordDisp
        |       _Symbol ParamDisp
        |       _Symbol Property
        |       _Symbol Comment
        |       _Symbol UserData
        ;

Symmetry : LBKT SYMMETRY _Symmetry RBKT
         ;

_Symmetry :
          |     _Symmetry Transform
          ;

Table : LBKT TABLE _Table RBKT
      ;

_Table :
       |        _Table Entry
       |        _Table TableDeflt
       ;

TableDeflt : LBKT TABLEDEFAULT __TableDeflt _TableDeflt RBKT
           ;

__TableDeflt :  LogicRef
             |  PortRef
             |  NoChange
             |  Table
             ;

_TableDeflt :
            |   Delay
            |   LoadDelay
            ;

Technology : LBKT TECHNOLOGY _Technology RBKT
           {
              log("PRINT TECHNOLOGY \n");
           }
           ;

_Technology :   NumberDefn
            |   _Technology FigGrp
            |   _Technology Fabricate
            |   _Technology SimulInfo
            |   _Technology DesignRule
            |   _Technology Comment
            |   _Technology UserData
            ;

TextHeight : LBKT TEXTHEIGHT Int RBKT
            {
                if( $3 );
            }
           ;

TimeIntval : LBKT TIMEINTERVAL __TimeIntval _TimeIntval RBKT
           ;

__TimeIntval :  Event
             |  OffsetEvent
             ;

_TimeIntval :   Event
            |   OffsetEvent
            |   Duration
            ;

TimeStamp : LBKT TIMESTAMP Int Int Int Int Int Int RBKT
        {
            if( $3 );
            if( $4 );
            if( $5 );
            if( $6 );
            if( $7 );
            if( $8 );
        }
          ;

Timing : LBKT TIMING
        {
            boost::shared_ptr<View> currV = inDriver.getContext()->getCurrentView();
            if( currV && View::eTypeNetlist == currV->getType() )
            {
                inDriver.getLexer()->setAppendToBuffer( true );
                inDriver.getLexer()->resetBuffer();
            }
        } _Timing RBKT
        {
            boost::shared_ptr<View> currV = inDriver.getContext()->getCurrentView();
            if( currV && View::eTypeNetlist == currV->getType() )
            {
                inDriver.getLexer()->setAppendToBuffer( false );
                log("%s\n", inDriver.getLexer()->getBuffer().c_str());
                inDriver.getLexer()->resetBuffer();
            }
        }
       ;

_Timing :       Derivation
        |       _Timing PathDelay
        |       _Timing Forbidden
        |       _Timing Comment
        |       _Timing UserData
        ;

Transform : LBKT TRANSFORM _TransformExt RBKT
          ;

_TransformExt:
            | _TransformExt ScaleX
            | _TransformExt ScaleY
            | _TransformExt Delta
            | _TransformExt Orientation
            | _TransformExt Origin
            ;

Transition : LBKT TRANSITION _Transition _Transition RBKT
           ;

_Transition :   LogicNameRef
            |   LogicList
            |   LogicOneOf
            ;

Trigger : LBKT TRIGGER _Trigger RBKT
        ;

_Trigger :
         |      _Trigger Change
         |      _Trigger Steady
         |      _Trigger Initial
         ;

True : LBKT TRUE RBKT
     ;

TypedValue :    Boolean
           |    Integer
           |    MiNoMa
           |    Number
           |    Point
           |    String
           ;

Unconstrained : LBKT UNCONSTRAINED RBKT
              ;

Undefined : LBKT UNDEFINED RBKT
          ;

Union : LBKT UNION _Union RBKT
      ;

_Union :        FigGrpRef
       |        FigureOp
       |        _Union FigGrpRef
       |        _Union FigureOp
       ;

Unit : LBKT UNIT _Unit RBKT
        {
            $$ = $3;
        }
     ;

_Unit :  ANGLE
        {
            $$ = 1;
        }
        | CAPACITANCE
        {
            $$ = 2;
        }
        | CONDUCTANCE
        {
            $$ = 3;
        }
        | CHARGE
        {
            $$ = 4;
        }
        | CURRENT
        {
            $$ = 5;
        }
        | DISTANCE
        {
            $$ = 6;
        }
        | ENERGY
        {
            $$ = 7;
        }
        | FLUX
        {
            $$ = 8;
        }
        | FREQUENCY
        {
            $$ = 9;
        }
        | INDUCTANCE
        {
            $$ = 10;
        }
        | MASS
        {
            $$ = 11;
        }
        | POWER
        {
            $$ = 12;
        }
        | RESISTANCE
        {
            $$ = 13;
        }
        | TEMPERATURE
        {
            $$ = 14;
        }
        | TIME
        {
            $$ = 15;
        }
        | VOLTAGE
        {
            $$ = 16;
        }
        ;

Unused : LBKT UNUSED RBKT
       ;

UnusedData : LBKT UNUSED RBKT
        {
            $$ = 1;
        }
       ;

UserData : LBKT USERDATA _UserData RBKT
         ;

_UserData :     Ident
            {
                delete $1;
            }
          |     _UserData Int
                {
                    if( $2 );
                }
          |     _UserData Str
                  {
                    delete $2;
                }
          |     _UserData Ident
                  {
                    delete $2;
                }
          |     _UserData Form
          ;

ValueNameDefData :  NameDefData
             |  ArrayData
             ;

ValueNameRef :  NameRef
             |  Member
             ;

ValueNameRefData :  NameRefData
             |  MemberData
             ;

Version : LBKT VERSION Str RBKT
        {
            $$ = $3;
        }
        ;

View : LBKT VIEW ViewNameDef
        {
            boost::scoped_ptr<NameData> nameData($3);
            std::string name = nameData->mName;
            EdifContextSharedPtr ctx = inDriver.getContext();
            ViewSharedPtr view;
            CellSharedPtr cell = ctx->getCurrentCell();
            view = cell->findView( name );
            bool isCellExtern = cell->getIsExtern();
            if( view )
            {
                log("Found existing view\n");
                if( isCellExtern )
                {
                    if( !view->getIsExtern() )
                    {
                        std::string message = constructErrorMessage(
                            "View %s already exists in cell %s\n",
                                name.c_str(), view->getName().c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
                    }
                    else
                    {
                        log( "Extern view %s already exists in %s\n",
                            name.c_str(), cell->getName().c_str() );
                    }
                }
                else
                {
                    if( view->getIsExtern() )
                    {
                        log("Found extern view of this name\n");
                        view->setIsExtern( false );
                        ctx->setIsViewBeingLinked( true );
                    }
                    else
                    {
                        log("View %s already found.. Ignoring\n",
                                name.c_str());
                        //TBD::WARN
#if 0
                        std::string message = constructErrorMessage(
                            "View %s already exists in cell %s\n",
                                name.c_str(), cell->getName().c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData( "Parser error message", message );
                        e.saveContextData( "Filename", yyloc.begin.filename );
                        e.saveContextData( "StartLine", yyloc.begin.line );
                        e.saveContextData( "EndLine", yyloc.end.line );
                        inDriver.setParserError( e );
                        error( yyloc, message );
                        YYABORT;
#endif
                    }
                }
            }
            else
            {
                try
                {
                    ctx->getFactory()->create( view );
                }
                catch( Error &e )
                {
                    std::string message = constructErrorMessage(
                            "Unable to create View : %s", view->getName().c_str());
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData( "Parser error message", message );
                    e.saveContextData( "Filename", yyloc.begin.filename );
                    e.saveContextData( "StartLine", yyloc.begin.line );
                    e.saveContextData( "EndLine", yyloc.end.line );
                    inDriver.setParserError( e );
                    error( yyloc, message );
                    YYABORT;
                }
                view->setName( name );
                if( !nameData->mOriginalName.empty() )
                {
                    view->setOriginalName( nameData->mOriginalName );
                }
                view->setIsExtern( isCellExtern );
                view->setParent( cell );
                cell->addView( view );
                LinkerSharedPtr linker = ctx->getLinker();
                Linker::NameSpec viewNameSpec(
                                cell->getParent()->getName(),
                                cell->getName(), view->getName() );
                if( isCellExtern )
                {
                    linker->setExternView( viewNameSpec, view );
                    log("Extern view %s saved in linker.\n",
                                        view->getName().c_str() );
                }
                else
                {
                    linker->linkUnresolved( viewNameSpec, view );
                }
                log( "Created view %s\n", view->getName().c_str() );
            }
            ctx->setCurrentView( view );
            log("View %s added\n",
                    view->getName().c_str());
            ctx->pushPropertyContainer( view );
        } ViewType _View RBKT
        {
            ViewSharedPtr view = inDriver.getContext()->getCurrentView();
            if( View::eTypeNetlist != view->getType() )
            {
                view->setNonNetlistViewData(inDriver.getLexer()->getBuffer());
            }
            inDriver.getContext()->setCurrentView(
                            ViewSharedPtr() );
            inDriver.getContext()->setIsViewBeingLinked( false );
            inDriver.getContext()->popPropertyContainer();
            inDriver.getLexer()->setAppendToBuffer( false );
            log("%s\n", inDriver.getLexer()->getBuffer().c_str());

            inDriver.getLexer()->resetBuffer();
        }
     ;

_View :         Interface
      |         _View Status
      |         _View Contents
      |         _View Comment
      |         _View Property
      |         _View UserData
      ;

ViewList : LBKT VIEWLIST _ViewList RBKT
         ;

_ViewList :
          |     _ViewList ViewRef
          |     _ViewList ViewList
          ;

ViewMap : LBKT VIEWMAP _ViewMap RBKT
        ;

_ViewMap :
         |      _ViewMap PortMap
         |      _ViewMap PortBackAn
         |      _ViewMap InstMap
         |      _ViewMap InstBackAn
         |      _ViewMap NetMap
         |      _ViewMap NetBackAn
         |      _ViewMap Comment
         |      _ViewMap UserData
         ;

ViewNameDef :   NameDefData
            ;

ViewNameRef :   NameRef
            ;

ViewNameRefData :   NameRefData
            ;

ViewRef : LBKT VIEWREF ViewNameRef _ViewRef RBKT
        ;

_ViewRef :
         | CellRef
         ;

ViewRefData : LBKT VIEWREF ViewNameRefData _ViewRefData RBKT
        {
            if( NULL == $4 )
            {
                $$ = new ViewRefData();
            }
            else
            {
                $$ = $4;
            }
            $$->mViewName = $3;
        }
        ;

_ViewRefData :
         {
             $$ = NULL; //Current view
         }
         | CellRefData
         {
            $$ = $1;
         }
         ;

ViewType : LBKT VIEWTYPE _ViewType RBKT
            {
                View::Type viewType = static_cast<View::Type>( $3 );
                if( View::eTypeNetlist != viewType )
                {
                    //TBD::WARN
                    std::string message = constructErrorMessage(
                            "Warning::View types other than netlist are ignored");
                    log("%s\n", message.c_str());
                    // Error e( eMessageIdParserWarning,
                    //     __FUNCTION__, __FILE__, __LINE__ );
                    // e.saveContextData( "Parser warning message", message );
                    // e.saveContextData( "Filename", yyloc.begin.filename );
                    // e.saveContextData( "StartLine", yyloc.begin.line );
                    // e.saveContextData( "EndLine", yyloc.end.line );
                    // inDriver.setParserError( e );
                    // error( yyloc, message );
                    // YYABORT;
                    inDriver.getLexer()->setAppendToBuffer( true );
                    inDriver.getLexer()->resetBuffer();
                }
                inDriver.getContext()->getCurrentView()->setType(
                                                        viewType );
            }
         ;

_ViewType :     MASKLAYOUT
            {
                $$ = View::eTypeMaskLayout;
            }
          |     PCBLAYOUT
            {
                $$ = View::eTypePCBLayout;
            }
          |     NETLIST
            {
                $$ = View::eTypeNetlist;
            }
          |     SCHEMATIC
            {
                $$ = View::eTypeSchematic;
            }
          |     SYMBOLIC
            {
                $$ = View::eTypeSymbolic;
            }
          |     BEHAVIOR
            {
                $$ = View::eTypeBehavior;
            }
          |     LOGICMODEL
            {
                $$ = View::eTypeLogicModel;
            }
          |     DOCUMENT
            {
                $$ = View::eTypeDocument;
            }
          |     GRAPHIC
            {
                $$ = View::eTypeGraphic;
            }
          |     STRANGER
            {
                $$ = View::eTypeStranger;
            }
          ;

Visible : LBKT VISIBLE BooleanValue RBKT
        {
            if( $3 );
        }
        ;

VoltageMap : LBKT VOLTAGEMAP MiNoMaValue RBKT
        {
            delete $3;
        }
        ;

WaveValue : LBKT WAVEVALUE LogicNameDef ScaledInt LogicWave RBKT
            {
                delete $4;
            }
          ;

Weak : LBKT WEAK LogicNameRef RBKT
     ;

WeakJoined : LBKT WEAKJOINED _WeakJoined RBKT
           ;

_WeakJoined :
            |   _WeakJoined PortRef
            |   _WeakJoined PortList
            |   _WeakJoined Joined
            ;

When : LBKT WHEN _When RBKT
     ;

_When :         Trigger
      |         _When After
      |         _When Follow
      |         _When Maintain
      |         _When LogicAssn
      |         _When Comment
      |         _When UserData
      ;

Written :  LBKT WRITTEN _Written RBKT
        ;

_Written :      TimeStamp
         |      _Written Author
                 {
                    delete $2;
                }
         |      _Written Program
                 {
                    delete $2;
                }
         |      _Written DataOrigin
         |      _Written Property
         |      _Written Comment
         |      _Written UserData
         ;

Ident : IDENTIFIER
        {
            $$ = $1;
        }
      ;

Str : STRING
        {
            $$ =$1;
        }
    ;

Int : INTEGER
        {
            $$ =$1;
        }
    ;

Keyword : Ident
        {
            delete $1;
        }
        ;

 /*** END grammar rules ***/

%% /*** Additional Code ***/

void
torc::generic::Parser::error(const Parser::location_type& l,
                const std::string& m)
{
    if( !inDriver.getIsParserErrorSet() )
    {
        Error e( eMessageIdParserError,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData( "Parser error message", m );
        e.saveContextData( "Filename", l.begin.filename );
        e.saveContextData( "StartLine", l.begin.line );
        e.saveContextData( "EndLine", l.end.line );
        inDriver.setParserError( e );
        log("Synthetic parser error created\n");
    }
    inDriver.error(l, m);
}

