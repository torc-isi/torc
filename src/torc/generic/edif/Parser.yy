/*******************************************************************************
* TORC - Copyright 2010 University of Southern California. All Rights Reserved.
*
* FILE : parser.yy
*
* DATE : 08-July-2010
*
* DESCRIPTION : Contains the EDIF Bison parser source
*
* REVISION HISTORY:
*
* SI        REVISION        AUTHOR               CHANGES          PRs
*[0]    Initial Version    Niladri
*
*******************************************************************************/

%{

#include <algorithm>
#include <cstdio>
#include <cstdarg>
#include <iterator>
#include <string>
#include <vector>
#include "torc/generic/edif/EdifContext.hpp"
#include "torc/generic/edif/ParserHelpers.hpp"
#include "torc/generic/Log.hpp"
#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/PortDelay.hpp"
#include "torc/generic/PortAttributes.hpp"
#include "torc/generic/NetAttributes.hpp"
#include "torc/generic/PortRefCreator.hpp"
#include "torc/generic/TimeStamp.hpp"
#include "torc/generic/Written.hpp"
#include "torc/generic/InterfaceAttributes.hpp"
#include "torc/generic/LogicValueAttributes.hpp"

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
%token          PAGESIZE_EDIF
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
%type <integerVal>_Derivation
%type <integerVal>Derivation
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

%type <stringVal>UserData
%type <timeStamp>TimeStamp
%type <stringVal>_Program
%type <pairStrData>Program
%type <stringVal>_DataOrigin
%type <pairStrData>DataOrigin   
%type <stringVal>_Comment
%type <stringVal>Comment 
%type <integerVal>_Criticality
%type <integerVal>Criticality
%type <netAttributes>_Net
%type <interfaceAttributes>_Interface
%type <interfaceAttributes>Interface
%type <nameData>LogicNameDef
%type <valueData>VoltageMap
%type <valueData>CurrentMap
%type <integerVal>BooleanMap
%type <logicValueAttributes>_LogicValue
%type <nameData>LogicNameRefData
%type <nameData>Weak
%type <nameData>Strong
%type <logicListData>_Compound
%type <logicListData>Compound
%type <logicListData>_Dominates
%type <logicListData>Dominates
%type <logicListData>_Resolves
%type <logicListData>Resolves
%type <logicRefData>_LogicRef
%type <logicRefData>LogicRef
%type <logicRefData>LogicMapIn
%type <logicRefData>LogicMapOut
%type <nameData>SimNameDef
%type <pairData>Duration
%type <pairData>_Cycle
%type <valueData>_PathDelayValue
%type <pairData>OffsetEvent
%type <netRefData>_NetRefData
%type <netRefData>NetRefData
%type <nameData>NetNameRefData
%type <netDelay>NetDelayData
%type <netDelay>_NetDelayData
%type <netDelay>_NetDelay
%type <netDelay>NetDelay


%destructor { } <integerVal>
%destructor { } <doubleVal>
%destructor { } <nameData>
%destructor { } <edifVersion>
%destructor { } <pairData>
%destructor { delete $$; } <*>

%{

#include "torc/generic/edif/Driver.hpp"
#include "torc/generic/edif/Scanner.hpp"
#include "torc/generic/Root.hpp"
#include "torc/generic/ObjectFactory.hpp"
#include "torc/generic/Library.hpp"
#include "torc/generic/edif/Linker.hpp"
#include "torc/generic/VisitorApplier.hpp"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex inDriver.getLexer()->lex

namespace {

std::string
constructErrorMessage(const char *fmt, ...)
{
    char msg[BUFSIZ];
    va_list args;
    va_start(args, fmt);
    int nbytes = vsnprintf(msg, BUFSIZ, fmt, args);
    va_end(args);
    if(BUFSIZ <= nbytes)
    {
        return std::string();
    }
    return std::string(msg);
}

using namespace torc::generic;

class AssignedParamCreator
        :public SingleParameter::Visitor,
        public ParameterArrayElement::Visitor,
        public ParameterArray::Visitor {
  public:
    void
    visit(SingleParameter &single) throw(Error)
    try
    {
        SingleParameterSharedPtr singleP;
        mFactory->create(singleP);
        singleP->setName(single.getName());
        singleP->setValue(mValues[0]);
        mMap->set(mContext, single.getName(), singleP);
    }
    catch(Error &e)
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

    void
    visit(ParameterArrayElement &element) throw(Error)
    try
    {
        ParameterSharedPtr array
                                    = element.getParentCollection();
        if(array == mMap->get(mContext, array->getName()))
        {
            AssignedParamCreator creator(
                    mInstance, mContext, mMap, mFactory, mValues );
            array->accept(creator);
        }
        std::vector<size_t> indices = element.getIndices();
        ParameterSharedPtr arrayP
                    =  mMap->get(mContext, array->getName());
        ParameterSharedPtr bit = arrayP->get(indices);
        bit->setValue(mValues[0]);
    }
    catch(Error &e)
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

    void
    visit(ParameterArray &array) throw(Error)
    try
    {
        std::vector<size_t> limits;
        array.getLimits(limits);
        ParameterArraySharedPtr arrayP;
        mFactory->create(arrayP);
        arrayP->setName(array.getName());
        std::vector<size_t> dimensions;
        array.getLimits(dimensions);
        arrayP->constructChildren(mFactory, limits);
        std::vector< ParameterSharedPtr > children;
        arrayP->getChildren(children);
        std::vector< ParameterSharedPtr >::iterator
                                            it = children.begin();
        std::vector< ParameterSharedPtr >::iterator
                                            end = children.end();
        size_t i = 0;
        for(; it != end; ++it, i++ )
        {
            (*it)->setValue(mValues[i]);
        }
        mMap->set(mContext, array.getName(), arrayP);
    }
    catch(Error &e)
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
        :mInstance(inInstance),
        mContext(inContext),
        mMap(inMap),
        mFactory(inFactory),
        mValues(inValues) {
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

template <typename _Connectable>
void
connectibleFinder( std::vector< std::string > &nestedNames,
        boost::shared_ptr<_Connectable> &conn ) throw(Error)
{
    std::vector< std::string >::reverse_iterator name
                                = nestedNames.rbegin();
    std::vector< std::string >::reverse_iterator end
                                = nestedNames.rend();
    ++name;
    if(!conn)
    {
        std::string message = constructErrorMessage(
                "Nested connectible name %s not found",
                                            (*name).c_str() );
        log("%s\n", message.c_str());
        Error e( eMessageIdErrorNullPointer,
            __FUNCTION__, __FILE__, __LINE__ );
        throw e;
    }
    for(;name != end; ++name)
    {
        if( eCompositionTypeBundle
                != conn->getCompositionType() )
        {
            log("Item is not a bundle .. invalid nesting\n");
            Error e( eMessageIdErrorUnsupoortedOperation,
                __FUNCTION__, __FILE__, __LINE__ );
            e.saveContextData("Name", *name );
            e.saveContextData("Operation",
                                    std::string("Find child") );
            throw e;
        }
        std::vector< boost::shared_ptr<_Connectable> > children;
        conn->getChildren(children);
        bool found = false;
        for( typename
            std::vector< boost::shared_ptr<_Connectable> >::iterator
                it = children.begin(); it != children.end(); ++it )
        {
            if((*it)->getName() == *name)
            {
                conn = (*it);
                found = true;
                break;
            }
        }
        if(!found)
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

template <typename _Connectable>
void
portJoiner( NameData *inPortNameData,
            const boost::shared_ptr<_Connectable> &inConn,
            const NetSharedPtr &inNet ) throw(Error)
{
    try
    {
        if(inPortNameData->mIndices.empty())
        {
            //TBD::CHECK WIDTH
            inConn->connect(inNet);
        }
        else
        {
            boost::shared_ptr<_Connectable> bit
                = inConn->get(inPortNameData->mIndices);
            bit->connect(inNet);
        }
        log( "Net %s connected with connectible %s\n",
                    inNet->getName().c_str(), inConn->getName().c_str() );
    }
    catch(Error &e)
    {
        e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
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
    while(nextPortRef)
    {
        currPortRef = nextPortRef;
        nestedNames.push_back(nextPortRef->mPortName->mName);
        nextPortRef = currPortRef->mParentPort;
    }
    bool isPort = (NULL == currPortRef->mInstanceName);
    NameData *portNameData = currPortRef->mPortName;
    std::string topName = *(nestedNames.rbegin());
    EdifContextSharedPtr ctx = inContext;
    ViewSharedPtr view = ctx->getCurrentView();
    if(isPort)
    {
        PortSharedPtr port = view->findPort(topName);
        if(!port)
        {
            std::string message = constructErrorMessage(
                    "No port %s found",
                        portNameData->mName.c_str());
            log("%s\n", message.c_str());
            Error e( eMessageIdParserError,
                __FUNCTION__, __FILE__, __LINE__ );
            e.saveContextData("Parser error message", message);
            e.saveContextData("Filename", inLocation.begin.filename);
            e.saveContextData("StartLine", inLocation.begin.line);
            e.saveContextData("EndLine", inLocation.end.line);
            outErrorMessage = message;
            throw e;
        }
        try
        {
            NetSharedPtr net = ctx->getCurrentNet();
            connectibleFinder(nestedNames, port);
            portJoiner(portData->mPortName, port, net);
        }
        catch(Error &e)
        {
            std::string message = constructErrorMessage(
                    "Unable to connect port : %s", port->getName().c_str());
            log("%s\n", message.c_str());
            e.setCurrentLocation( __FUNCTION__,
                                    __FILE__, __LINE__ );
            e.saveContextData("Parser error message", message);
            e.saveContextData("Filename", inLocation.begin.filename);
            e.saveContextData("StartLine", inLocation.begin.line);
            e.saveContextData("EndLine", inLocation.end.line);
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
        if(!instance)
        {
            std::string message = constructErrorMessage(
                    "Instance %s not known",
                    instanceName->mName.c_str() );
            log("%s\n", message.c_str());
            Error e( eMessageIdParserError,
                __FUNCTION__, __FILE__, __LINE__ );
            e.saveContextData("Parser error message", message);
            e.saveContextData("Filename", inLocation.begin.filename);
            e.saveContextData("StartLine", inLocation.begin.line);
            e.saveContextData("EndLine", inLocation.end.line);
            outErrorMessage = message;
            throw e;
        }
        if(!instanceName->mIndices.empty())
        {
            InstanceSharedPtr bit
                    = instance->get(instanceName->mIndices);
            instance = bit;
        }
        PortReferenceSharedPtr portRef
                = instance->findPortReference(topName);
        if(!portRef)
        {
            std::string message = constructErrorMessage(
                    "No port ref %s found",
                        portNameData->mName.c_str());
            log("%s\n", message.c_str());
            Error e( eMessageIdParserError,
                __FUNCTION__, __FILE__, __LINE__ );
            e.saveContextData("Parser error message", message);
            e.saveContextData("Filename", inLocation.begin.filename);
            e.saveContextData("StartLine", inLocation.begin.line);
            e.saveContextData("EndLine", inLocation.end.line);
            outErrorMessage = message;
            throw e;
        }
        try
        {
            connectibleFinder(nestedNames, portRef);
            portJoiner( portData->mPortName, portRef,
                            ctx->getCurrentNet() );
        }
        catch(Error &e)
        {
            std::string message = constructErrorMessage(
                    "Unable to connect port ref : %s", portRef->getName().c_str());
            log("%s\n", message.c_str());
            e.setCurrentLocation( __FUNCTION__,
                                    __FILE__, __LINE__ );
            e.saveContextData("Parser error message", message);
            e.saveContextData("Filename", inLocation.begin.filename);
            e.saveContextData("StartLine", inLocation.begin.line);
            e.saveContextData("EndLine", inLocation.end.line);
            outErrorMessage = message;
            throw e;
        }
    }
}

}

%}


%% /*** Grammar Rules ***/

Edif : LBKT EDIF EdifFileName EdifVersion EdifLevel KeywordMap 
        {
            boost::scoped_ptr<NameData> fileName($3);
            boost::scoped_ptr<VersionData> versionData($4);
            EdifContextSharedPtr ctx = inDriver.getContext();
            RootSharedPtr root = ctx->getRoot();
            root->setName(fileName->mName);
            if(!fileName->mOriginalName.empty())
            {
                root->setOriginalName(fileName->mOriginalName);
            }
            EdifVersion version;
            version.mFirst = versionData->mFirst;
            version.mSecond = versionData->mSecond;
            version.mThird = versionData->mThird;
            root->setVersion(version);
            root->setLevel(static_cast<EdifLevel>($5) );
            ctx->pushStatusContainer(root);
        } _Edif RBKT
        {
            inDriver.getContext()->popStatusContainer();
        }
     ;

_Edif :
      | _Edif Status
      | _Edif External
      | _Edif Library
      | _Edif Design
      | _Edif Comment
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            RootSharedPtr root = ctx->getRoot();
            root->addComment(*$2);
            delete $2;
        }
      | _Edif UserData
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            RootSharedPtr root = ctx->getRoot();
            root->addUserData(*$2);
            delete $2;
        }
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

_After :     MiNoMaValue
       |     _After Follow
       |     _After Maintain
       |     _After LogicAssn
       |     _After Comment
            {
                delete $2;
            }
       |    _After UserData
            {
                delete $2;
            }
       ;

Annotate : LBKT ANNOTATE _Annotate RBKT
         ;

_Annotate : Str
            {
                delete $1;
            }
          | StrDisplay
            {
                delete $1;
            }
          ;

Apply : LBKT APPLY 
        {
            ApplySharedPtr apply;
            EdifContextSharedPtr ctx
                        = inDriver.getContext();
            try
            {
                ctx->getFactory()->create(apply);
            }
            catch(Error &e)
            {
                std::string message = constructErrorMessage(
                        "Unable to create apply" );
                log("%s\n", message.c_str());
                e.setCurrentLocation( __FUNCTION__,
                                        __FILE__, __LINE__ );
                e.saveContextData("Parser error message", message);
                e.saveContextData("Filename", yyloc.begin.filename);
                e.saveContextData("StartLine", yyloc.begin.line);
                e.saveContextData("EndLine", yyloc.end.line);
                inDriver.setParserError(e);
                error(yyloc, message);
                YYABORT;
            }
            ctx->setCurrentApply(apply);
            SimulateSharedPtr simulate
                    = ctx->getCurrentSimulate();
            simulate->addApply(apply);
        } _Apply RBKT
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            ctx->setCurrentApply(ApplySharedPtr());
        }
      ;

_Apply :    Cycle
       |    _Apply LogicIn
       |    _Apply LogicOut
       |    _Apply Comment
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                ApplySharedPtr apply = ctx->getCurrentApply();
                apply->addComment(*$2);
                delete $2;
            }
       |    _Apply UserData
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                ApplySharedPtr apply = ctx->getCurrentApply();
                apply->addUserData(*$2);
                delete $2;
            }
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
            $$->mIndices.push_back($4);
            if($5)
            {
                $$->mIndices.insert($$->mIndices.end(),
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
            $$->push_back($1);
            if($2)
            {
                $$->insert($$->end(), $2->begin(), $2->end());
                delete $2;
            }
        }
        | Int
        {
            $$ = new std::vector<size_t>();
            $$->push_back($1);
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
                {
                    delete $2;
                }
              | _ArrayRelInfo UserData
                {
                    delete $2;
                }
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

Becomes : LBKT BECOMES 
        {
                EdifContextSharedPtr ctx
                                = inDriver.getContext();
                LogicElementSharedPtr logicElem;
                try
                {
                    ctx->getFactory()->create(logicElem);
                    logicElem->setType(LogicElement::eTypeBecomes);
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create becomes" );
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                ctx->pushLogicElement(logicElem);
                ctx->incrementLogicElementDepth();
            } _PresentValue RBKT
        ;

_PresentValue : LogicNameData
              | LogicList
              | LogicOneOf
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
            if(NULL == $3)
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
                $$ = ($1) ? $1 : new ValueData();
                bool val = $2;
                $$->mValues.push_back( Value(
                            Value::eValueTypeBoolean, val ) );
            }
         |      _Boolean BooleanDisp
            {
                $$ = ($1) ? $1 : new ValueData();
                bool val = $2;
                $$->mValues.push_back( Value(
                            Value::eValueTypeBoolean, val ) );
            }
         |      _Boolean Boolean
         {
            $$ = $2;
            if($1)
            {
               $1->mValues.insert($1->mValues.end(),
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
                $$ = $3;
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
                if($3);
                if($4);
                delete $5;
            }
          ;

BorderWidth : LBKT BORDERWIDTH Int RBKT
            {
                if($3);
            }
            ;

BoundBox : LBKT BOUNDINGBOX Rectangle RBKT
         ;

Cell : LBKT CELL CellNameDef
        {
            boost::scoped_ptr<NameData> nameData($3);
            std::string name = nameData->mName;
            EdifContextSharedPtr ctx = inDriver.getContext();
            CellSharedPtr cell;
            LibrarySharedPtr lib = ctx->getCurrentLibrary();
            cell = lib->findCell(name);
            if(cell)
            {
                log("Found existing cell\n");
                if(lib->getIsExtern())
                {
                    if(!cell->getIsExtern())
                    {
                        std::string message = constructErrorMessage(
                            "Cell %s already exists in lib %s\n",
                                name.c_str(), lib->getName().c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
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
                    if(cell->getIsExtern())
                    {
                        log("Found extern cell of this name\n");
                        cell->setIsExtern(false);
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
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
#endif
                    }
                }
            }
            else
            {
                try
                {
                    ctx->getFactory()->create(cell);
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create Cell : %s", cell->getName().c_str());
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                cell->setName(name);
                if(!$3->mOriginalName.empty())
                {
                    cell->setOriginalName($3->mOriginalName);
                }
                cell->setIsExtern(lib->getIsExtern());
                cell->setParent(lib);
                lib->addCell(cell);
                log("Created cell %s\n", cell->getName().c_str());
            }
            ctx->setCurrentCell(cell);
            ctx->pushPropertyContainer(cell);
            ctx->pushStatusContainer(cell);
        } _Cell RBKT
        {
            inDriver.getContext()->setCurrentCell(
                                    CellSharedPtr() );
            inDriver.getContext()->popPropertyContainer();
            inDriver.getContext()->popStatusContainer();
        }
        ;

_Cell : CellType
      | _Cell Status
      | _Cell ViewMap
      | _Cell View
      | _Cell Comment
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            CellSharedPtr cell = ctx->getCurrentCell();
            cell->addComment(*$2);
            delete $2;
        }
      | _Cell UserData
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            CellSharedPtr cell = ctx->getCurrentCell();
            cell->addUserData(*$2);
            delete $2;
        }
      | _Cell Property
      ;

CellNameDef :   NameDefData
            ;


CellRef : LBKT CELLREF CellNameRef _CellRef RBKT
        {
            if(NULL == $4)
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
            if(NULL == $4)
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
                        static_cast<Cell::Type>($3) );
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
        {
            $$ = $3;
        }
        ;

_Comment :  {
                $$ = NULL;
            }
         |  _Comment Str
            {
                $$ = ($1) ? $1 : $2;
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
                {
                    delete $2;
                }
           |    _CommGraph UserData
                {
                    delete $2;
                }
           ;

Compound : LBKT COMPOUND _Compound RBKT
            {
                $$ = $3;
            }
         ;

_Compound :
            {
                $$ = NULL;
            }
          | _Compound LogicNameRefData
            {
                $$ = ($1)?$1:new LogicListData();
                $$->mNameDataList.push_back($2);
            }
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
                {
                    EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                    ViewSharedPtr view
                            = ctx->getCurrentView();
                    TimingSharedPtr timing
                            = ctx->getCurrentTiming();
                    view->setTiming(timing);
                    ctx->setCurrentTiming(TimingSharedPtr());
                }
          |     _Contents Simulate
                {
                    EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                    ViewSharedPtr view
                            = ctx->getCurrentView();
                    SimulateSharedPtr simulate
                            = ctx->getCurrentSimulate();
                    view->setSimulate(simulate);
                    ctx->setCurrentSimulate(SimulateSharedPtr());
                }
          |     _Contents When
          |     _Contents Follow
          |     _Contents LogicPort
          |     _Contents BoundBox
          |     _Contents Comment
                {
                    delete $2;
                }
          |     _Contents UserData
                {
                    delete $2;
                }
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
            {   
                $$ = $3;
            }
            ;

_Criticality :  Int
                {
                    if($1);
                    $$ = $1;
                }
             |  IntDisplay
                {
                    if($1);
                    $$ = $1;
                }
             ;

CurrentMap : LBKT CURRENTMAP MiNoMaValue RBKT
            {
                $$ = $3;
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
            if($3)
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                ApplySharedPtr apply = ctx->getCurrentApply();
                apply->setNoOfCycle($3);

                boost::scoped_ptr<PairData> data($4);
                Value::Number num(data->mX, data->mY);
                Value val(Value::eValueTypeNumber, num);
                apply->setCycleDuration(val);
            }
        }
      ;

_Cycle :
            {
                $$ = new PairData();
            }
       |    Duration
            {
                $$ = $1;
            }     
       ;

DataOrigin : LBKT DATAORIGIN Str _DataOrigin RBKT
            {
                $$ = new PairStrData(*$3, *$4);
            }
           ;

_DataOrigin :   {
                    $$ = new std::string();
                }
            |   Version
                {
                    $$ = $1;
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
            boost::scoped_ptr<ValueData> data($1);
            $$ = new PortDelay();
            $$->setType(PortDelay::eTypeDelay);
            $$->setDelay(data->mValues[0].get<Value::MiNoMax>());
        }
       |     MiNoMaDisp
        {
            boost::scoped_ptr<ValueData> data($1);
            $$ = new PortDelay();
            $$->setType(PortDelay::eTypeDelay);
            $$->setDelay(data->mValues[0].get<Value::MiNoMax>());
        }
       ;

Delay : LBKT DELAY _Delay RBKT
      ;

_Delay : MiNoMaValue
        {
            delete $1;
        }
       | MiNoMaDisp
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
            {
                $$ = $3;
            }
           ;

_Derivation :   CALCULATED
            {
                $$ = eDerivationCalculated;
            }
            |   MEASURED
            {
                $$ = eDerivationMeasured;
            }
            |   REQUIRED
            {
                $$ = eDerivationRequired;
            }
            ;

Design : LBKT DESIGN DesignNameDef 
        {
            if($3)
            {
                EdifContextSharedPtr ctx
                                      = inDriver.getContext();
                RootSharedPtr root = ctx->getRoot();
                DesignSharedPtr design;
                design = root->findDesign($3->mName);
                if(design)
                {
                    log("Skipped creation of existing design.\n");
                }
                else
                {
                    try
                    {
                        ctx->getFactory()->create(design);
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create design : %s", 
                                        design->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    design->setName($3->mName);
                    if(!$3->mOriginalName.empty())
                    {
                        design->setOriginalName($3->mOriginalName);
                    }
                    design->setParent(root);
                    root->addDesign(design);
                }
                ctx->setCurrentDesign(design);
                ctx->pushPropertyContainer(design);
                ctx->pushStatusContainer(design);
                delete $3;
            }
        } _Design RBKT
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            ctx->popPropertyContainer();
            ctx->popStatusContainer();

            boost::scoped_ptr<CellRefData> cellRefData($5);
            std::string cellName =  cellRefData->mCellName->mName;
            std::string libName = cellRefData->mLibraryName->mName;
            LibrarySharedPtr library 
                                = ctx->getRoot()->findLibrary(libName) ;
            if(!library)
            {
                std::string message = constructErrorMessage(
                                    "Library %s not found",
                                            libName.c_str() );
                log("%s\n", message.c_str());
                Error e( eMessageIdParserError,
                    __FUNCTION__, __FILE__, __LINE__ );
                e.saveContextData("Parser error message", message);
                e.saveContextData("Filename", yyloc.begin.filename);
                e.saveContextData("StartLine", yyloc.begin.line);
                e.saveContextData("EndLine", yyloc.end.line);
                inDriver.setParserError(e);
                error(yyloc, message);
                YYABORT;
            }

            CellSharedPtr cell
                            = library->findCell(cellName);
            if(!cell)
            {
                std::string message = constructErrorMessage(
                                    "Cell %s not found",
                                            cellName.c_str() );
                log("%s\n", message.c_str());
                Error e( eMessageIdParserError,
                    __FUNCTION__, __FILE__, __LINE__ );
                e.saveContextData("Parser error message", message);
                e.saveContextData("Filename", yyloc.begin.filename);
                e.saveContextData("StartLine", yyloc.begin.line);
                e.saveContextData("EndLine", yyloc.end.line);
                inDriver.setParserError(e);
                error(yyloc, message);
                YYABORT;
            }
            DesignSharedPtr design = ctx->getCurrentDesign();
            design->setCellRefName(cellName);
            design->setLibraryRefName(libName);
            ctx->setCurrentDesign(DesignSharedPtr());
        }            
       ;

_Design : CellRef
          {
              $$ = new CellRefData();
              $$ = $1;
          }
        | _Design Status
          {
              $$ = $1;
          }
        | _Design Comment
          {
              $$ = $1;
              EdifContextSharedPtr ctx = inDriver.getContext();
              DesignSharedPtr design = ctx->getCurrentDesign();
              design->addComment(*$2);
              delete $2;  
          }
        | _Design Property
          {
              $$ = $1;
          }
        | _Design UserData
          {
              $$ = $1;
              EdifContextSharedPtr ctx = inDriver.getContext();
              DesignSharedPtr design = ctx->getCurrentDesign();
              design->addUserData(*$2);
              delete $2;
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
                {
                    delete $2;
                }
            |   _DesignRule UserData
                {
                    delete $2;
                }
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
                                static_cast<EPortDirection>($3) );
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
            {
                $$ = $3;
            }
          ;

_Dominates :
            {
                $$ = NULL;
            }
           |    _Dominates LogicNameRefData
            {
                $$ = ($1)?$1:new LogicListData();
                $$->mNameDataList.push_back($2);
            }
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
                $$ = $3;
            }
         ;

EncloseDist : LBKT ENCLOSUREDISTANCE RuleNameDef FigGrpObj FigGrpObj _EncloseDist RBKT
            ;

_EncloseDist :  Range
             |  SingleValSet
             |  _EncloseDist Comment
                {
                    delete $2;
                }
             |  _EncloseDist UserData
                {
                    delete $2;
                }
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

__Entry :   LogicRef
            {
                delete $1;
            }
        |   PortRef
        |   NoChange
        |   Table
        ;

_Entry :
       |        Delay
       |        LoadDelay
       ;

Event : LBKT EVENT 
        {
            EventSharedPtr event;
            EdifContextSharedPtr ctx
                        = inDriver.getContext();
            try
            {
                ctx->getFactory()->create(event);
            }
            catch(Error &e)
            {
                std::string message = constructErrorMessage(
                        "Unable to create event" );
                log("%s\n", message.c_str());
                e.setCurrentLocation( __FUNCTION__,
                                        __FILE__, __LINE__ );
                e.saveContextData("Parser error message", message);
                e.saveContextData("Filename", yyloc.begin.filename);
                e.saveContextData("StartLine", yyloc.begin.line);
                e.saveContextData("EndLine", yyloc.end.line);
                inDriver.setParserError(e);
                error(yyloc, message);
                YYABORT;
            }
            ctx->setCurrentEvent(event);
        } _Event RBKT
        {
            // Note : Need to do the followings, from where Event called
            //EdifContextSharedPtr ctx = inDriver.getContext();
            //ctx->setCurrentEvent(EventSharedPtr());
        }
      ;

_Event :    EventPortRefData
       |    EventPortListData
       |    EventPortGroup
       |    EventNetRefData
       |    EventNetGroup
       |    _Event Transition
            {
                EdifContextSharedPtr ctx
                        = inDriver.getContext();
                EventSharedPtr event
                        = ctx->getCurrentEvent();
                LogicElementSharedPtr logicElem
                            = ctx->getCurrentLogicElement();
                if(event)
                {
                    if(logicElem)
                    {
                        event->setTransition(logicElem);
                    }
                }
            }
       |    _Event Becomes
            {
                EdifContextSharedPtr ctx
                        = inDriver.getContext();
                EventSharedPtr event
                        = ctx->getCurrentEvent();
                LogicElementSharedPtr logicElem
                            = ctx->getCurrentLogicElement();
                if(event)
                {
                    if(logicElem)
                    {
                        event->setTransition(logicElem);
                    }
                }
            }
        ;

EventPortRefData : PortRefData
                {
                    boost::scoped_ptr<PortRefData> portData($1);
                    EdifContextSharedPtr ctx
                                = inDriver.getContext();
                    ViewSharedPtr view 
                                = ctx->getCurrentView();
                    std::vector< std::string > nestedNames;
                    PortRefData *currPortRef = $1;
                    PortRefData *nextPortRef = portData->mParentPort;
                    nestedNames.push_back(currPortRef->mPortName->mName);
                    while(nextPortRef)    
                    {
                        currPortRef = nextPortRef;
                        nestedNames.push_back(nextPortRef->mPortName->mName);
                        nextPortRef = currPortRef->mParentPort;
                    }
                    bool isPort = (NULL == currPortRef->mInstanceName);
                    NameData *portNameData = currPortRef->mPortName;
                    std::string topName = *(nestedNames.rbegin());
                    if(isPort)
                    {
                        PortSharedPtr port = view->findPort(topName);
                        if(!port)
                        {
                            std::string message = constructErrorMessage(
                                    "No port %s found",
                                        portNameData->mName.c_str());
                            log("%s\n", message.c_str());
                            Error e( eMessageIdParserError,
                                __FUNCTION__, __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
                            YYABORT;
                        }
                        try
                        {
                            connectibleFinder(nestedNames, port);
                            if(!portData->mPortName->mIndices.empty())
                            {
                                port = port->get(portData->mPortName->mIndices);
                            }
                            EventSharedPtr event
                                        = ctx->getCurrentEvent();
                            event->addPort(port);
                        }
                        catch(Error &e)
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
                        if(!instance)
                        {
                            std::string message = constructErrorMessage(
                                    "Instance %s not known",
                                    instanceName->mName.c_str() );
                            log("%s\n", message.c_str());
                            Error e( eMessageIdParserError,
                                __FUNCTION__, __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
                            YYABORT;
                        }
                        if(!instanceName->mIndices.empty())
                        {
                            InstanceSharedPtr bit
                                    = instance->get(instanceName->mIndices);
                            instance = bit;
                        }
                        PortReferenceSharedPtr portRef
                                = instance->findPortReference(topName);
                        if(!portRef)
                        {
                            std::string message = constructErrorMessage(
                                    "No port ref %s found",
                                        portNameData->mName.c_str());
                            log("%s\n", message.c_str());
                            Error e( eMessageIdParserError,
                                __FUNCTION__, __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
                            YYABORT;
                        }
                        try
                        {
                            connectibleFinder(nestedNames, portRef);
                            if(!portData->mPortName->mIndices.empty())
                            {
                                portRef = portRef->get( 
                                            portData->mPortName->mIndices );
                            }
                            EventSharedPtr event
                                        = ctx->getCurrentEvent();
                            event->addPortReference(portRef);
                        }
                        catch(Error &e)
                        {
                            std::string message = constructErrorMessage(
                                    "Unable to connect port ref : %s", 
                                            portRef->getName().c_str());
                            log("%s\n", message.c_str());
                            e.setCurrentLocation( __FUNCTION__,
                                                    __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
                            YYABORT;
                        }
                    }
                }
                ;

EventPortListData : PortListData
                    {
                        if(NULL != $1 && !$1->mPortRefs.empty())
                        {
                            boost::scoped_ptr<PortListData> portListData($1);
                            std::string message;

                            EdifContextSharedPtr ctx = inDriver.getContext();

                            PortListSharedPtr portList;
                            ctx->getFactory()->create(portList);    

                            for( std::vector<PortRefData *>::iterator it
                                    = portListData->mPortRefs.begin();
                                    it != portListData->mPortRefs.end(); ++it )
                            {
                                PortRefData *portData = *it;
                                std::vector< std::string > nestedNames;
                                PortRefData *currPortRef = *it;
                                PortRefData *nextPortRef = portData->mParentPort;
                                nestedNames.push_back(currPortRef->mPortName->mName);
                                while(nextPortRef)
                                {
                                    currPortRef = nextPortRef;
                                    nestedNames.push_back(nextPortRef->mPortName->mName);
                                    nextPortRef = currPortRef->mParentPort;
                                }
                                bool isPort = (NULL == currPortRef->mInstanceName);
                                NameData *portNameData = currPortRef->mPortName;
                                std::string topName = *(nestedNames.rbegin());
                                ViewSharedPtr view = ctx->getCurrentView();
                                if(isPort)
                                {
                                    PortSharedPtr port = view->findPort(topName);
                                    if(!port)
                                    {
                                        SimulateSharedPtr simulate 
                                                    = ctx->getCurrentSimulate();
                                        if(simulate)
                                        { 
                                            PortListAliasSharedPtr portList 
                                                        = simulate->findPortListAlias(topName);
                                            if(!portList)
                                            {
                                                std::string message = constructErrorMessage(
                                                        "No port %s found in portListAlias\n", 
                                                            portNameData->mName.c_str());
                                                log("%s\n", message.c_str());
                                                Error e( eMessageIdParserError,
                                                    __FUNCTION__, __FILE__, __LINE__ );
                                                e.saveContextData("Parser error message", message);
                                                e.saveContextData("Filename", yyloc.begin.filename);
                                                e.saveContextData("StartLine", yyloc.begin.line);
                                                e.saveContextData("EndLine", yyloc.end.line);
                                                inDriver.setParserError(e);
                                                error(yyloc, message);
                                                YYABORT;
                                            }
                                        }
                                        else
                                        {
                                            std::string message = constructErrorMessage(
                                                   "No port %s found in view %s",
                                                        portNameData->mName.c_str(),
                                                        view->getName().c_str() );
                                            log("%s\n", message.c_str());
                                            Error e( eMessageIdParserError,
                                                __FUNCTION__, __FILE__, __LINE__ );
                                            e.saveContextData("Parser error message", message);
                                            e.saveContextData("Filename", yyloc.begin.filename);
                                            e.saveContextData("StartLine", yyloc.begin.line);
                                            e.saveContextData("EndLine", yyloc.end.line);
                                            inDriver.setParserError(e);
                                            error(yyloc, message);
                                            YYABORT;
                                        }
                                    }
                                    try
                                    {
                                        connectibleFinder(nestedNames, port);
                                        if(!portData->mPortName->mIndices.empty())
                                        {
                                            port = port->get(portData->mPortName->mIndices);
                                        }
                                        portList->addChildPort(port);
                                    }
                                    catch(Error &e)
                                    {
                                        std::string message = constructErrorMessage(
                                                "Unable to connect port : %s", 
                                                        port->getName().c_str());
                                        log("%s\n", message.c_str());
                                        e.setCurrentLocation( __FUNCTION__,
                                                                __FILE__, __LINE__ );
                                        e.saveContextData("Parser error message", message);
                                        e.saveContextData("Filename", yyloc.begin.filename);
                                        e.saveContextData("StartLine", yyloc.begin.line);
                                        e.saveContextData("EndLine", yyloc.end.line);
                                        inDriver.setParserError(e);
                                        error(yyloc, message);
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
                                    if(!instance)
                                    {
                                        std::string message = constructErrorMessage(
                                                "Instance %s not known",
                                                instanceName->mName.c_str() );
                                        log("%s\n", message.c_str());
                                        Error e( eMessageIdParserError,
                                            __FUNCTION__, __FILE__, __LINE__ );
                                        e.saveContextData("Parser error message", message);
                                        e.saveContextData("Filename", yyloc.begin.filename);
                                        e.saveContextData("StartLine", yyloc.begin.line);
                                        e.saveContextData("EndLine", yyloc.end.line);
                                        inDriver.setParserError(e);
                                        error(yyloc, message);
                                        YYABORT;
                                    }
                                    if(!instanceName->mIndices.empty())
                                    {
                                        InstanceSharedPtr bit
                                                = instance->get(instanceName->mIndices);
                                        instance = bit;
                                    }
                                    PortReferenceSharedPtr portRef
                                            = instance->findPortReference(topName);
                                    if(!portRef)
                                    {
                                        std::string message = constructErrorMessage(
                                                "No port ref %s found",
                                                    portNameData->mName.c_str());
                                        log("%s\n", message.c_str());
                                        Error e( eMessageIdParserError,
                                            __FUNCTION__, __FILE__, __LINE__ );
                                        e.saveContextData("Parser error message", message);
                                        e.saveContextData("Filename", yyloc.begin.filename);
                                        e.saveContextData("StartLine", yyloc.begin.line);
                                        e.saveContextData("EndLine", yyloc.end.line);
                                        inDriver.setParserError(e);
                                        error(yyloc, message);
                                        YYABORT;
                                    }
                                    try
                                    {
                                        connectibleFinder(nestedNames, portRef);
                                        if(!portData->mPortName->mIndices.empty())
                                        {
                                            portRef = portRef->get( 
                                                        portData->mPortName->mIndices );
                                        }
                                        portList->addChildPortReference(portRef);
                                    }
                                    catch(Error &e)
                                    {
                                        std::string message = constructErrorMessage(
                                                "Unable to connect port ref : %s", 
                                                        portRef->getName().c_str());
                                        log("%s\n", message.c_str());
                                        e.setCurrentLocation( __FUNCTION__,
                                                                __FILE__, __LINE__ );
                                        e.saveContextData("Parser error message", message);
                                        e.saveContextData("Filename", yyloc.begin.filename);
                                        e.saveContextData("StartLine", yyloc.begin.line);
                                        e.saveContextData("EndLine", yyloc.end.line);
                                        inDriver.setParserError(e);
                                        error(yyloc, message);
                                        YYABORT;
                                    }
                                }
                            }
                            EventSharedPtr event
                                        = ctx->getCurrentEvent();
                            event->setPortList(portList);
                        }
                    }
                    ; 

EventNetRefData : NetRefData
                {
                    boost::scoped_ptr<NetRefData> netData($1);
                    EdifContextSharedPtr ctx
                                = inDriver.getContext();
                    ViewSharedPtr view 
                                = ctx->getCurrentView();
                    std::vector< std::string > nestedNames;
                    NetRefData *currNetRef = $1;
                    NetRefData *nextNetRef = netData->mParentNet;
                    nestedNames.push_back(currNetRef->mNetName->mName);
                    while(nextNetRef)    
                    {
                        currNetRef = nextNetRef;
                        nestedNames.push_back(nextNetRef->mNetName->mName);
                        nextNetRef = currNetRef->mParentNet;
                    }
                    bool isNet = (NULL == currNetRef->mInstanceName);
                    NameData *netNameData = currNetRef->mNetName;
                    std::string topName = *(nestedNames.rbegin());
                    if(isNet)
                    {
                        NetSharedPtr net = view->findNet(topName);
                        if(!net)
                        {
                            std::string message = constructErrorMessage(
                                    "No Net %s found",
                                        netNameData->mName.c_str());
                            log("%s\n", message.c_str());
                            Error e( eMessageIdParserError,
                                __FUNCTION__, __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
                            YYABORT;
                        }
                        try
                        {
                            connectibleFinder(nestedNames, net);
                            if(!netData->mNetName->mIndices.empty())
                            {
                                net = net->get(netData->mNetName->mIndices);
                            }
                            EventSharedPtr event
                                        = ctx->getCurrentEvent();
                            event->addNet(net);
                        }
                        catch(Error &e)
                        {
                            e.setCurrentLocation(
                                __FUNCTION__, __FILE__, __LINE__ );
                            throw;
                        }
                    }
                    else
                    {
                        NameData *instanceName
                                        = currNetRef->mInstanceName;
                        InstanceSharedPtr instance
                                        = view->findInstance(
                                                    instanceName->mName );
                        if(!instance)
                        {
                            std::string message = constructErrorMessage(
                                    "Instance %s not known",
                                    instanceName->mName.c_str() );
                            log("%s\n", message.c_str());
                            Error e( eMessageIdParserError,
                                __FUNCTION__, __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
                            YYABORT;
                        }
                        if(!instanceName->mIndices.empty())
                        {
                            InstanceSharedPtr bit
                                    = instance->get(instanceName->mIndices);
                            instance = bit;
                        }
                        NetSharedPtr net = view->findNet(topName);
                        if(!net)
                        {
                            std::string message = constructErrorMessage(
                                    "No Net ref %s found",
                                        netNameData->mName.c_str());
                            log("%s\n", message.c_str());
                            Error e( eMessageIdParserError,
                                __FUNCTION__, __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
                            YYABORT;
                        }
                        try
                        {
                            connectibleFinder(nestedNames, net);
                            if(!netData->mNetName->mIndices.empty())
                            {
                                net = net->get(netData->mNetName->mIndices);
                            }
                            EventSharedPtr event
                                        = ctx->getCurrentEvent();
                            event->addNet(net);
                        }
                        catch(Error &e)
                        {
                            e.setCurrentLocation(
                                __FUNCTION__, __FILE__, __LINE__ );
                            throw;
                        }
                    }
                }
                ;

Exactly : LBKT EXACTLY ScaledInt RBKT
            {
                delete $3;
            }
        ;

External : LBKT EXTERNAL LibNameDef EdifLevel
        {
            boost::scoped_ptr<NameData> name($3);
            if(name && !name->mName.empty())
            {
                EdifContextSharedPtr ctx
                                            = inDriver.getContext();
                RootSharedPtr root = ctx->getRoot();
                LibrarySharedPtr lib;
                // We do not create a new extern lib when one
                // exists already
                lib = root->findLibrary(name->mName);
                if(lib)
                {
                    log("Skipped creation of existing lib.\n");
                }
                EdifLevel level;
                switch($4)
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
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
                if(lib)
                {
                    if(lib->getLevel() != level)
                    {
                        std::string message = constructErrorMessage(
                                            "Level mismatch - old %d new %d\n",
                                                lib->getLevel(), level );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    else
                    {
                        log("We have found the actual lib corresponding to this lib\n");
                    }
                }
                else
                {
                    ctx->getFactory()->create(lib);
                    lib->setName(name->mName);
                    if(! name->mOriginalName.empty())
                    {
                        lib->setOriginalName(name->mOriginalName);
                    }
                    lib->setLevel(level);
                    lib->setParent(root);
                    lib->setIsExtern(true);
                    root->addLibrary(lib);
                    log("Created extern lib %s level %d\n",
                                lib->getName().c_str(), level );
                }
                ctx->setCurrentLibrary(lib);
                ctx->pushStatusContainer(lib);
            }
            else
            {
                //TBD::POSSIBLE?
            }
        } _External RBKT
        {
            inDriver.getContext()->setCurrentLibrary(
                                    LibrarySharedPtr() );
            inDriver.getContext()->popStatusContainer();
        }
        ;
         ;

_External :     Technology
          |     _External Status
          |     _External Cell
          |     _External Comment
                {
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    LibrarySharedPtr lib = ctx->getCurrentLibrary();
                    lib->addComment(*$2);
                    delete $2;
                }
          |     _External UserData
                {
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    LibrarySharedPtr lib = ctx->getCurrentLibrary();
                    lib->addUserData(*$2);
                    delete $2;
                }
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
                {
                    delete $2;
                }
        |       _FigGrp Property
        |       _FigGrp UserData
                {
                    delete $2;
                }
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
                {
                    delete $2;
                }
            |   _FigGrpOver Property
            |   _FigGrpOver UserData
                {
                    delete $2;
                }
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
                {
                    delete $2;
                }
        |       _Figure UserData
                {
                    delete $2;
                }
        ;

FigureArea : LBKT FIGUREAREA RuleNameDef FigGrpObj _FigureArea RBKT
           ;

_FigureArea :   Range
            |   SingleValSet
            |   _FigureArea Comment
                {
                    delete $2;
                }
            |   _FigureArea UserData
                {
                    delete $2;
                }
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
                {
                    delete $2;
                }
             |  _FigurePerim UserData
                {
                    delete $2;
                }
             ;

FigureWidth : LBKT FIGUREWIDTH RuleNameDef FigGrpObj _FigureWidth RBKT
            ;

_FigureWidth :  Range
             |  SingleValSet
             |  _FigureWidth Comment
                {
                    delete $2;
                }
             |  _FigureWidth UserData
                {
                    delete $2;
                }
             ;

FillPattern : LBKT FILLPATTERN Int Int Boolean RBKT
            {
                if($3);
                if($4);
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

Forbidden : LBKT FORBIDDENEVENT 
            {
                ForbiddenEventSharedPtr forbiddenEvent;
                EdifContextSharedPtr ctx
                            = inDriver.getContext();
                try
                {
                    ctx->getFactory()->create(forbiddenEvent);
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create forbiddenEvent" );
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                ctx->setCurrentForbiddenEvent(forbiddenEvent);
                TimingSharedPtr timing = ctx->getCurrentTiming();
                timing->addForbiddenEvent(forbiddenEvent);
            } _Forbidden RBKT
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                ctx->setCurrentForbiddenEvent(ForbiddenEventSharedPtr());
            }
            ;

_Forbidden : TimeIntval
           | _Forbidden Event
             {
                 EdifContextSharedPtr ctx = inDriver.getContext();
                 ForbiddenEventSharedPtr forbiddenEvent 
                             = ctx->getCurrentForbiddenEvent();
                 EventSharedPtr event
                         = ctx->getCurrentEvent();
                 event->setType(Event::eTypeEvent);
                 forbiddenEvent->addEvent(event);
                 ctx->setCurrentEvent(EventSharedPtr());
             }
           ;

Form : LBKT
        {
            inDriver.getLexer()->setIsIdContext(true);
        }
        Keyword _Form RBKT
        ;

_Form :
      | _Form Int
        {
            if($2);
        }
      | _Form Str
        {
            delete $2;
        }
      | _Form Ident
        {
            delete $2;
        }
      | _Form Form
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
        {
            LogicElementSharedPtr ignoreLogicElem;
            EdifContextSharedPtr ctx
                        = inDriver.getContext();
            try
            {
                ctx->getFactory()->create(ignoreLogicElem);
                ignoreLogicElem->setType(LogicElement::eTypeIgnored);
            }
            catch(Error &e)
            {
                std::string message = constructErrorMessage(
                        "Unable to create Ignore" );
                log("%s\n", message.c_str());
                e.setCurrentLocation( __FUNCTION__,
                                        __FILE__, __LINE__ );
                e.saveContextData("Parser error message", message);
                e.saveContextData("Filename", yyloc.begin.filename);
                e.saveContextData("StartLine", yyloc.begin.line);
                e.saveContextData("EndLine", yyloc.end.line);
                inDriver.setParserError(e);
                error(yyloc, message);
                YYABORT;
            }
            LogicElementSharedPtr logicElem
                            = ctx->getCurrentLogicElement();
            logicElem->addChildLogicElement(ignoreLogicElem);
        }
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
                inDriver.getContext()->setIsInInstance(true);
            } _Instance RBKT
            {
                boost::scoped_ptr<NameData> nameData($3);
                EdifContextSharedPtr ctx = inDriver.getContext();
                boost::scoped_ptr<InstanceInfo> info($5);
                ViewRefData *viewData = info->mViewRefData;
                std::string cellName
                                = viewData->mCellName->mName;
                std::string viewName
                                = viewData->mViewName->mName;
                LibrarySharedPtr library;
                std::string libraryName;
                if(!viewData->mLibraryName)
                {
                    library = ctx->getCurrentLibrary();
                }
                else
                {
                     libraryName = viewData->mLibraryName->mName;
                     library = ctx->getRoot()->findLibrary(libraryName);
                }
                if(!library)
                {
                    std::string message = constructErrorMessage(
                                        "Library %s not found",
                                                libraryName.c_str() );
                    log("%s\n", message.c_str());
                    Error e( eMessageIdParserError,
                        __FUNCTION__, __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                libraryName = library->getName();
                ViewSharedPtr masterView;
                bool masterIsExtern = false;
                LinkerSharedPtr linker = ctx->getLinker();
                Linker::NameSpec viewNameSpec(
                                    libraryName, cellName, viewName );
                if(!libraryName.empty())
                {
                    masterView
                        = linker->findExternView(viewNameSpec);
                    if(!masterView)
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

                if(!masterIsExtern) //View is not extern
                {
                    CellSharedPtr cell;
                    cell = library->findCell(cellName);
                    if(!cell)
                    {
                        std::string message = constructErrorMessage(
                                                "Cell %s not found",
                                                        cellName.c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    masterView = cell->findView(viewName);
                    if(!masterView)
                    {
                        std::string message = constructErrorMessage(
                                    "View %s not found",
                                            viewName.c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
                InstanceSharedPtr instance;
                if(nameData->mIndices.empty())
                {
                    try
                    {
                        SingleInstanceSharedPtr singleInst;
                        ctx->getFactory()->create(singleInst);
                        instance = singleInst;
                        instance->setName(nameData->mName);
                        if(!nameData->mOriginalName.empty())
                        {
                            instance->setOriginalName(nameData->mOriginalName);
                        }
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create Instance : %s", 
                                        instance->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
                else
                {
                    try
                    {
                        InstanceArraySharedPtr instArray;
                        ctx->getFactory()->create(instArray);
                        instArray->setName(nameData->mName);
                        if(!nameData->mOriginalName.empty())
                        {
                            instArray->setOriginalName(nameData->mOriginalName);
                        }
                        instArray->constructChildren(
                                        ctx->getFactory(),
                                        nameData->mIndices );
                        instance = instArray;
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create Instance Array : %s", 
                                        instance->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
                ctx->setCurrentInstance(instance);
                try
                {
                    instance->bindToMasterView(masterView);
                    if( eCompositionTypeVector
                            == instance->getCompositionType() )
                    {
                        std::vector< InstanceSharedPtr > children;
                        instance->getChildren(children);
                        std::vector< InstanceSharedPtr >::iterator inst
                                            = children.begin();
                        std::vector< InstanceSharedPtr >::iterator end
                                            = children.end();
                        for(;inst != end; ++inst)
                        {
                            //Create Port references now
                            PortRefCreator<PortReferenceSharedPtr> creator(
                                    ctx->getFactory(), *inst );
                            masterView->applyOnAllPorts(
                                VisitorApplier< PortRefCreator<PortReferenceSharedPtr> >(creator) );
                        }
                    }
                    else
                    {
                        //Create Port references now
                        PortRefCreator<PortReferenceSharedPtr> 
                                creator(ctx->getFactory(), instance);
                        masterView->applyOnAllPorts(
                            VisitorApplier< PortRefCreator<PortReferenceSharedPtr> >(creator) );
                    }
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create Port ref");
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                if(masterIsExtern)
                {
                    linker->registerUnresolvedInstance(
                                    viewNameSpec, instance );
                }
                instance->setParent(ctx->getCurrentView());
                ctx->getCurrentView()->addInstance(instance);
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
                    if(!chkP)
                    {
                        //CAN ONLY OVERRIDE EXISTING PARAMS
                        std::string message = constructErrorMessage(
                                "Can only override existing params");
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    if(pA->mValueData->mValues.size()
                                            != chkP->getSize() )
                    {
                        //SIZE MISMATCH
                        std::string message = constructErrorMessage(
                                "Params size mismatch : %d with %d",
                                    pA->mValueData->mValues.size(), 
                                    chkP->getSize());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
#if 0
                    if(pA->mValueData->mValues[0].getType()
                            != chkP->getValue().getType() )
                    {
                        std::string message
                                        = constructErrorMessage(
                                    "Type mismatch during assignment for param %s",
                                    pA->mNameData->mName.c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
#endif
                    AssignedParamCreator creator(
                        instance, instance->getParameterContext(),
                        instance->getParameters(), ctx->getFactory(),
                        pA->mValueData->mValues );
                    try
                    {
                        if(!pA->mNameData->mIndices.empty())
                        {
                            //ASSIGNMENT TO A BIT
                            chkP = chkP->get(
                                        pA->mNameData->mIndices );
                        }
                        chkP->accept(creator);
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to assign param: %s",
                                            chkP->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    log("Param %s overridden at context %lu\n",
                                        pA->mNameData->mName.c_str(),
                                        context);
                    ctx->setIsInInstance(false);
                }

                //Properties
                std::vector<PropertySharedPtr> props;
                PropertySharedPtr property
                            = ctx->getCurrentProperty();
                while(property)
                {
                    props.push_back(property);
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
                inDriver.getContext()->setIsInInstance(false);
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
                    while(nextPortRef)
                    {
                        currPortRef = nextPortRef;
                        nestedNames.push_back(nextPortRef->mPortName->mName);
                        nextPortRef = currPortRef->mParentPort;
                    }
                    NameData *portNameData = currPortRef->mPortName;
                    std::string topName = *(nestedNames.rbegin());
                    PortReferenceSharedPtr portRef
                            = instance->findPortReference(topName);
                    if(!portRef)
                    {
                        std::string message = constructErrorMessage(
                                "No port ref %s found",
                                    portNameData->mName.c_str());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    try
                    {
                        connectibleFinder(nestedNames, portRef);
                        if(!portData->mPortName->mIndices.empty())
                        {
                            portRef = portRef->get(
                                        portData->mPortName->mIndices );
                        }
                        portRef->setAttributes(
                                PortAttributesSharedPtr(
                                    instData->mAttribute) );
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to connect port ref : %s", 
                                    portRef->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
                //Handle UserData
                for( std::vector< std::string >::iterator it
                        = info->mUserData.begin();
                        it != info->mUserData.end(); ++it )
                {
                    std::string val = *it;
                    instance->addUserData(val);
                }
                //Handle Comment
                for( std::vector< std::string >::iterator it
                        = info->mComment.begin();
                        it != info->mComment.end(); ++it )
                {
                    std::string val = *it;
                    instance->addComment(val);
                }
                //Handle Designator
                instance->setDesignator(info->mDesignator);
                
                //Handle Timing
                instance->setTiming(info->mTiming);

                inDriver.getContext()->setCurrentInstance( 
                                            InstanceSharedPtr() );
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
                $$->mParamAssign.push_back($2);
            }
          |     _Instance PortInst
            {
                $$ = ($1)?$1:new InstanceInfo();
                $$->mPortInst.push_back($2);
            }
          |     _Instance Timing
            {
                $$ = ($1)?$1:new InstanceInfo();
                EdifContextSharedPtr ctx
                                = inDriver.getContext();
                TimingSharedPtr timing
                        = ctx->getCurrentTiming();
                $$->mTiming = timing;
            }
          |     _Instance DesignatorData
            {
                $$ = ($1)?$1:new InstanceInfo();
                $$->mDesignator = *$2;
            }
          |     _Instance Property
            {
                $$ = $1;
            }
          |     _Instance Comment
            {
                $$ = ($1)?$1:new InstanceInfo();
                $$->mComment.push_back(*$2);
            }
          |     _Instance UserData
            {
                $$ = ($1)?$1:new InstanceInfo();
                $$->mUserData.push_back(*$2);
            }
          ;

InstanceRef : LBKT INSTANCEREF InstNameRef _InstanceRef RBKT

_InstanceRef :
             |  InstanceRef
             |  ViewRefData
             ;

InstanceRefData : LBKT INSTANCEREF InstNameRefData _InstanceRefData RBKT
                {
                    $$ = ($4) ? $4 : new InstanceRefData();
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
                {
                    delete $2;
                }
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
                {
                    delete $2;
                }
         |      _InstMap UserData
                {
                    delete $2;
                }
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
                if($2);
                if(NULL == $3)
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
                $$ = ($1) ? $1 : new ValueData();
                $$->mValues.push_back( Value(
                            Value::eValueTypeInteger, $2 ) );
            }
         |      _Integer IntDisplay
            {
                $$ = ($1) ? $1 : new ValueData();
                $$->mValues.push_back( Value(
                            Value::eValueTypeInteger, $2 ) );
            }
         |      _Integer Integer
         {
            $$ = $2;
            if($1)
            {
               $1->mValues.insert($1->mValues.end(),
                       $$->mValues.begin(), $$->mValues.end() );
               $$->mValues = $1->mValues;
               delete $1;
            }
         }
         ;

Interface : LBKT INTERFACE _Interface RBKT
            {
                $$ = $3;
            }
          ;

_Interface :
            {
                $$ = NULL;
            }
           |    _Interface Port
                {
                    $$ = $1;
                }
           |    _Interface PortBundle
                {
                    $$ = $1;
                }
           |    _Interface Symbol
                {
                    $$ = $1;
                }
           |    _Interface ProtectFrame
                {
                    $$ = $1;
                }
           |    _Interface ArrayRelInfo
                {
                    $$ = $1;
                }
           |    _Interface Parameter
                {
                    $$ = $1;
                }
           |    _Interface Joined
                {
                    $$ = $1;
                }
           |    _Interface MustJoin
                {
                    $$ = $1;
                }
           |    _Interface WeakJoined
                {
                    $$ = $1;
                }
           |    _Interface Permutable
                {
                    $$ = $1;
                }
           |    _Interface Timing
                {
                    $$ = ($1)?$1:new InterfaceAttributes();
                    EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                    TimingSharedPtr timing
                            = ctx->getCurrentTiming();
                    $$->setTiming(timing);
                    ctx->setCurrentTiming(TimingSharedPtr());
                }
           |    _Interface Simulate
                {
                    $$ = ($1)?$1:new InterfaceAttributes();
                    EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                    SimulateSharedPtr simulate
                            = ctx->getCurrentSimulate();
                    $$->setSimulate(simulate);
                    ctx->setCurrentSimulate(SimulateSharedPtr());
                }
           |    _Interface DesignatorData
                {
                    $$ = ($1)?$1:new InterfaceAttributes();
                    $$->setDesignator(*$2);
                }
           |    _Interface Property
                {
                    $$ = $1;
                }
           |    _Interface Comment
                {
                    $$ = ($1)?$1:new InterfaceAttributes();
                    $$->addComment(*$2);
                }
           |    _Interface UserData
                {
                    $$ = ($1)?$1:new InterfaceAttributes();
                    $$->addUserData(*$2);
                    delete $2;
                }
           ;

InterFigGrp : LBKT INTERFIGUREGROUPSPACING RuleNameDef FigGrpObj FigGrpObj _InterFigGrp RBKT
            ;

_InterFigGrp :  Range
             |  SingleValSet
             |  _InterFigGrp Comment
                {
                    delete $2;
                }
             |  _InterFigGrp UserData
                {
                    delete $2;
                }
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
                {
                    delete $2;
                }
             |  _IntraFigGrp UserData
                {
                    delete $2;
                }
             ;

Inverse : LBKT INVERSE _Inverse RBKT
        ;

_Inverse :      FigGrpRef
         |      FigureOp
         ;

Isolated : LBKT ISOLATED RBKT
         ;

Joined : LBKT JOINED 
            {
                InterfaceJoinedInfoSharedPtr joinedInfo;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                try
                {
                    ctx->getFactory()->create(joinedInfo);
                    joinedInfo->setJoinedType( 
                            InterfaceJoinedInfo::eJoinedTypeJoin );
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create joined info" );
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                if(ctx->getInterfaceJoinedInfoDepth() == 0 )
                {
                    ctx->getCurrentView()->addInterfaceJoinedInfo(joinedInfo);
                }
                ctx->pushInterfaceJoinedInfo(joinedInfo);
                ctx->incrementInterfaceJoinedInfoDepth();
            } _Joined RBKT
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                InterfaceJoinedInfoSharedPtr currentJoinedInfo 
                        = ctx->getCurrentInterfaceJoinedInfo();
                ctx->popInterfaceJoinedInfo();
                InterfaceJoinedInfoSharedPtr parentJoinedInfo; 
                if(ctx->getInterfaceJoinedInfoDepth() > 1 )
                {
                    parentJoinedInfo = ctx->getCurrentInterfaceJoinedInfo();
                }
                ctx->decrementInterfaceJoinedInfoDepth();
                if(parentJoinedInfo)
                {
                    parentJoinedInfo->setRelationType( 
                            InterfaceJoinedInfo::eRelationTypeParent );
                    currentJoinedInfo->setRelationType( 
                            InterfaceJoinedInfo::eRelationTypeChild );
                    //Check size
                    size_t parentJoinedInfoSize = parentJoinedInfo->getSize();
                    size_t currentJoinedInfoSize = currentJoinedInfo->getSize();

                    log("Parent joined info Size :: %d\n",
                            parentJoinedInfoSize ); 
                    log("Current joined info Size :: %d\n", 
                            currentJoinedInfoSize );

                    //std::cout << "Parent joined info Size :: " << parentJoinedInfoSize << std::endl;
                    //std::cout << "Current joined info Size :: " << currentJoinedInfoSize << std::endl;

                    std::vector< InterfaceJoinedInfoSharedPtr > outJoinedInfos;
                    parentJoinedInfo->getChildren(outJoinedInfos);

                    std::list< PortSharedPtr > outPorts;
                    parentJoinedInfo->getPorts(outPorts);
                    
                    std::list< PortListSharedPtr > outPortLists;
                    parentJoinedInfo->getPortLists(outPortLists);
                    
                    if(parentJoinedInfoSize != 0) 
                    {
                        if( parentJoinedInfoSize
                                == currentJoinedInfoSize )
                        {
                            parentJoinedInfo->addChildJoinedInfo(
                                    currentJoinedInfo );
                        }
                        else
                        {
                            std::string message = constructErrorMessage(
                                    "Child joined info can't be added, port size mismatch %d with %d",
                                        parentJoinedInfoSize,
                                        currentJoinedInfoSize);
                            log("%s\n", message.c_str());
                            Error e( eMessageIdParserError,
                                __FUNCTION__, __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
                            YYABORT;
                        }
                    }
                    else
                    {
                        parentJoinedInfo->addChildJoinedInfo(
                                currentJoinedInfo );
                    }
                } 
                else
                {
                    ctx->pushInterfaceJoinedInfo(currentJoinedInfo);
                }              
            }
       ;

_Joined :
        |   _Joined PortRefData
        {
                boost::scoped_ptr<PortRefData> portData($2);
                NameData *portNameData = portData->mPortName;
                std::string name = portNameData->mName;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                ViewSharedPtr view = ctx->getCurrentView();

                PortRefData *currPortRef = $2;
                PortRefData *nextPortRef = portData->mParentPort;
                std::vector< std::string > nestedNames;
                nestedNames.push_back(currPortRef->mPortName->mName);
                while(nextPortRef)
                {
                    currPortRef = nextPortRef;
                    nestedNames.push_back(nextPortRef->mPortName->mName);
                    nextPortRef = currPortRef->mParentPort;
                }
                bool isPort = (NULL == currPortRef->mInstanceName);
                std::string topName = *(nestedNames.rbegin());
                if(isPort)
                {
                    PortSharedPtr port = view->findPort(topName);
                    if(!port)
                    {
                        std::string message = constructErrorMessage(
                                "No port %s found in view %s\n", 
                                            name.c_str(),
                                            view->getName().c_str());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }

                    InterfaceJoinedInfoSharedPtr currentJoinedInfo 
                                        = ctx->getCurrentInterfaceJoinedInfo();
                    try
                    {             
                        connectibleFinder(nestedNames, port);  
                        if(!portData->mPortName->mIndices.empty())
                        {
                            port = port->get(portData->mPortName->mIndices);
                        }
                        //Check port size
                        std::list< PortSharedPtr > outPorts;
                        currentJoinedInfo->getPorts(outPorts);
                        
                        if(!outPorts.empty())
                        {
                            PortSharedPtr firstPort = outPorts.front();
                            std::list< PortSharedPtr >::iterator it
                                            = std::find(outPorts.begin(), outPorts.end(),
                                                    port);
                            if(it != outPorts.end())
                            {
                                std::string message = constructErrorMessage(
                                            "Port %s already exist in joined info",
                                                port->getName().c_str() );
                                    log("%s\n", message.c_str());
                                    Error e( eMessageIdParserError,
                                        __FUNCTION__, __FILE__, __LINE__ );
                                    e.saveContextData("Parser error message", message);
                                    e.saveContextData("Filename", yyloc.begin.filename);
                                    e.saveContextData("StartLine", yyloc.begin.line);
                                    e.saveContextData("EndLine", yyloc.end.line);
                                    inDriver.setParserError(e);
                                    error(yyloc, message);
                                    YYABORT;
                            }
                            else
                            {
                                if(firstPort->getSize() == port->getSize())
                                {
                                    currentJoinedInfo->addPort(port);
                                }
                                else
                                {
                                    std::string message = constructErrorMessage(
                                            "Port %s can't be added, port size mismatch %d with %d",
                                                name.c_str(),
                                                firstPort->getSize(),
                                                port->getSize());
                                    log("%s\n", message.c_str());
                                    Error e( eMessageIdParserError,
                                        __FUNCTION__, __FILE__, __LINE__ );
                                    e.saveContextData("Parser error message", message);
                                    e.saveContextData("Filename", yyloc.begin.filename);
                                    e.saveContextData("StartLine", yyloc.begin.line);
                                    e.saveContextData("EndLine", yyloc.end.line);
                                    inDriver.setParserError(e);
                                    error(yyloc, message);
                                    YYABORT;
                                }
                            }
                        }
                        else
                        {
                            currentJoinedInfo->addPort(port);
                        }
                    }
                    catch(Error &e)
                    {
                        e.setCurrentLocation(
                            __FUNCTION__, __FILE__, __LINE__ );
                        throw;
                    }
                }
                else
                {
                }
            }
        |   _Joined PortListData
            {
                if(NULL != $2 && !$2->mPortRefs.empty())
                {
                    boost::scoped_ptr<PortListData> portListData($2);
                    std::string message;

                    EdifContextSharedPtr ctx = inDriver.getContext();
                    PortListSharedPtr portList;
                    ctx->getFactory()->create(portList);

                    InterfaceJoinedInfoSharedPtr currentJoinedInfo 
                                = ctx->getCurrentInterfaceJoinedInfo();

                    for( std::vector<PortRefData *>::iterator it
                            = portListData->mPortRefs.begin();
                            it != portListData->mPortRefs.end(); ++it )
                    {
                        PortRefData *portData = *it;
                        std::vector< std::string > nestedNames;
                        PortRefData *currPortRef = *it;
                        PortRefData *nextPortRef = portData->mParentPort;
                        nestedNames.push_back(currPortRef->mPortName->mName);
                        while(nextPortRef)
                        {
                            currPortRef = nextPortRef;
                            nestedNames.push_back(nextPortRef->mPortName->mName);
                            nextPortRef = currPortRef->mParentPort;
                        }
                        bool isPort = (NULL == currPortRef->mInstanceName);
                        NameData *portNameData = currPortRef->mPortName;
                        std::string topName = *(nestedNames.rbegin());
                        ViewSharedPtr view = ctx->getCurrentView();
                        if(isPort)
                        {
                            PortSharedPtr port = view->findPort(topName);
                            if(!port)
                            {
                                std::string message = constructErrorMessage(
                                        "No port %s found in view %s\n", 
                                            portNameData->mName.c_str(),
                                            view->getName().c_str());
                                log("%s\n", message.c_str());
                                Error e( eMessageIdParserError,
                                    __FUNCTION__, __FILE__, __LINE__ );
                                e.saveContextData("Parser error message", message);
                                e.saveContextData("Filename", yyloc.begin.filename);
                                e.saveContextData("StartLine", yyloc.begin.line);
                                e.saveContextData("EndLine", yyloc.end.line);
                                inDriver.setParserError(e);
                                error(yyloc, message);
                                YYABORT;
                            }

                            try
                            {
                                connectibleFinder(nestedNames, port);
                                if(!portData->mPortName->mIndices.empty())
                                {
                                    port = port->get(portData->mPortName->mIndices);
                                }
                                portList->addChildPort(port);
                            }
                            catch(Error &e)
                            {
                                std::string message = constructErrorMessage(
                                        "Unable to connect port : %s", 
                                                port->getName().c_str());
                                log("%s\n", message.c_str());
                                e.setCurrentLocation( __FUNCTION__,
                                                        __FILE__, __LINE__ );
                                e.saveContextData("Parser error message", message);
                                e.saveContextData("Filename", yyloc.begin.filename);
                                e.saveContextData("StartLine", yyloc.begin.line);
                                e.saveContextData("EndLine", yyloc.end.line);
                                inDriver.setParserError(e);
                                error(yyloc, message);
                                YYABORT;
                            }
                        }
                        else
                        {
                        }
                    }
                    if(currentJoinedInfo->getSize() == portList->getSize())
                    {
                        currentJoinedInfo->addPortList(portList);
                    }
                    else
                    {
                        std::string message = constructErrorMessage(
                                "Port size mismatch %d with %d",
                                    currentJoinedInfo->getSize(),
                                    portList->getSize());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
            }
        ;

JoinedElements : LBKT JOINED _JoinedElements RBKT
       ;

_JoinedElements :
        |   _JoinedElements PortRefData
            {
                std::string message;
                boost::scoped_ptr<PortRefData> portData($2);
                try
                {
                    connectElementToNet(
                            $2, inDriver.getContext(),
                            yyloc, message );
                }
                catch(Error &e)
                {
                    e.setCurrentLocation(
                        __FUNCTION__, __FILE__, __LINE__ );
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
            }
        |   _JoinedElements PortListData
            {
                if(NULL != $2 && !$2->mPortRefs.empty())
                {
                    boost::scoped_ptr<PortListData> portListData($2);
                    std::string message;
                    PortListSharedPtr portList;
                    inDriver.getContext()->getFactory()->create(portList);
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
                        while(nextPortRef)
                        {
                            currPortRef = nextPortRef;
                            nestedNames.push_back(nextPortRef->mPortName->mName);
                            nextPortRef = currPortRef->mParentPort;
                        }
                        bool isPort = (NULL == currPortRef->mInstanceName);
                        NameData *portNameData = currPortRef->mPortName;
                        std::string topName = *(nestedNames.rbegin());
                        ViewSharedPtr view = ctx->getCurrentView();
                        if(isPort)
                        {
                            PortSharedPtr port = view->findPort(topName);
                            if(!port)
                            {
                                std::string message = constructErrorMessage(
                                        "No port %s found",
                                            portNameData->mName.c_str());
                                log("%s\n", message.c_str());
                                Error e( eMessageIdParserError,
                                    __FUNCTION__, __FILE__, __LINE__ );
                                e.saveContextData("Parser error message", message);
                                e.saveContextData("Filename", yyloc.begin.filename);
                                e.saveContextData("StartLine", yyloc.begin.line);
                                e.saveContextData("EndLine", yyloc.end.line);
                                inDriver.setParserError(e);
                                error(yyloc, message);
                                YYABORT;
                            }
                            try
                            {
                                connectibleFinder(nestedNames, port);
                                if(!portData->mPortName->mIndices.empty())
                                {
                                    port = port->get(portData->mPortName->mIndices);
                                }
                                portList->addChildPort(port);
                            }
                            catch(Error &e)
                            {
                                std::string message = constructErrorMessage(
                                        "Unable to connect port : %s", 
                                                port->getName().c_str());
                                log("%s\n", message.c_str());
                                e.setCurrentLocation( __FUNCTION__,
                                                        __FILE__, __LINE__ );
                                e.saveContextData("Parser error message", message);
                                e.saveContextData("Filename", yyloc.begin.filename);
                                e.saveContextData("StartLine", yyloc.begin.line);
                                e.saveContextData("EndLine", yyloc.end.line);
                                inDriver.setParserError(e);
                                error(yyloc, message);
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
                            if(!instance)
                            {
                                std::string message = constructErrorMessage(
                                        "Instance %s not known",
                                        instanceName->mName.c_str() );
                                log("%s\n", message.c_str());
                                Error e( eMessageIdParserError,
                                    __FUNCTION__, __FILE__, __LINE__ );
                                e.saveContextData("Parser error message", message);
                                e.saveContextData("Filename", yyloc.begin.filename);
                                e.saveContextData("StartLine", yyloc.begin.line);
                                e.saveContextData("EndLine", yyloc.end.line);
                                inDriver.setParserError(e);
                                error(yyloc, message);
                                YYABORT;
                            }
                            if(!instanceName->mIndices.empty())
                            {
                                InstanceSharedPtr bit
                                        = instance->get(instanceName->mIndices);
                                instance = bit;
                            }
                            PortReferenceSharedPtr portRef
                                    = instance->findPortReference(topName);
                            if(!portRef)
                            {
                                std::string message = constructErrorMessage(
                                        "No port ref %s found",
                                            portNameData->mName.c_str());
                                log("%s\n", message.c_str());
                                Error e( eMessageIdParserError,
                                    __FUNCTION__, __FILE__, __LINE__ );
                                e.saveContextData("Parser error message", message);
                                e.saveContextData("Filename", yyloc.begin.filename);
                                e.saveContextData("StartLine", yyloc.begin.line);
                                e.saveContextData("EndLine", yyloc.end.line);
                                inDriver.setParserError(e);
                                error(yyloc, message);
                                YYABORT;
                            }
                            try
                            {
                                connectibleFinder(nestedNames, portRef);
                                if(!portData->mPortName->mIndices.empty())
                                {
                                    portRef = portRef->get( 
                                                portData->mPortName->mIndices );
                                }
                                portList->addChildPortReference(portRef);
                            }
                            catch(Error &e)
                            {
                                std::string message = constructErrorMessage(
                                        "Unable to connect port ref : %s", 
                                                portRef->getName().c_str());
                                log("%s\n", message.c_str());
                                e.setCurrentLocation( __FUNCTION__,
                                                        __FILE__, __LINE__ );
                                e.saveContextData("Parser error message", message);
                                e.saveContextData("Filename", yyloc.begin.filename);
                                e.saveContextData("StartLine", yyloc.begin.line);
                                e.saveContextData("EndLine", yyloc.end.line);
                                inDriver.setParserError(e);
                                error(yyloc, message);
                                YYABORT;
                            }
                        }
                    }
                    try
                    {
                        portList->connect(ctx->getCurrentNet());
                    }
                    catch(Error &e)
                    {
                        e.setCurrentLocation(
                            __FUNCTION__, __FILE__, __LINE__ );
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
            }
        |   _JoinedElements GlobPortRefData
            {
                boost::scoped_ptr<NameData> globPortRef($2);
                std::string name = globPortRef->mName;
                //Find ports of this name
                EdifContextSharedPtr ctx = inDriver.getContext();
                ViewSharedPtr view = ctx->getCurrentView();
                NetSharedPtr net = ctx->getCurrentNet();
                size_t netSize = net->getSize();
                PortSharedPtr port = view->findPort(name);
                if(port)
                {
                    try
                    {
                        port->connect(net);
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to connect port : %s", 
                                        port->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
                std::vector< InstanceSharedPtr > leafInstances;
                {
                    std::vector< InstanceSharedPtr > instances;
                    view->getInstances(instances);
                    for( std::vector< InstanceSharedPtr >::iterator it
                        = instances.begin(); it != instances.end(); ++it )
                    {
                        if( eCompositionTypeScalar
                            == (*it)->getCompositionType() )
                        {
                            leafInstances.push_back(*it);
                        }
                        else
                        {
                            (*it)->getChildren(leafInstances);
                        }
                    }
                }
                for( std::vector< InstanceSharedPtr >::iterator it 
                    = leafInstances.begin(); it != leafInstances.end(); ++it )
                {
                    PortReferenceSharedPtr pRef
                                 = (*it)->findPortReference(name);
                    if( pRef
                            && netSize == pRef->getSize() )
                    {
                        try
                        {
                            pRef->connect(net);
                        }
                        catch(Error &e)
                        {
                            std::string message = constructErrorMessage(
                                    "Unable to connect port : %s", 
                                            pRef->getName().c_str());
                            log("%s\n", message.c_str());
                            e.setCurrentLocation( __FUNCTION__,
                                                    __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
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
                    if($3);
                }
             ;

KeywordMap : LBKT KEYWORDMAP _KeywordMap RBKT
           ;

_KeywordMap :   KeywordLevel
            |   _KeywordMap Comment
                {
                    delete $2;
                }
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
            if($3)
            {
                EdifContextSharedPtr ctx
                                            = inDriver.getContext();
                RootSharedPtr root = ctx->getRoot();
                LibrarySharedPtr lib;
                // We do not create a new extern lib when one
                // exists already
                lib = root->findLibrary($3->mName);
                if(lib)
                {
                    log("Skipped creation of existing lib.\n");
                }
                EdifLevel level;
                switch($4)
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
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
                if(lib)
                {
                    if(lib->getLevel() != level)
                    {
                        std::string message = constructErrorMessage(
                                "Level mismatch - old %d new %d\n",
                                    lib->getLevel(), level );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    else
                    {
                        if(lib->getIsExtern())
                        {
                            log("Exising lib was extern .. changing\n");
                            lib->setIsExtern(false);
                        }
                    }
                }
                else
                {
                    try
                    {
                        ctx->getFactory()->create(lib);
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create lib : %s", 
                                        lib->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    lib->setName($3->mName);
                    if(! $3->mOriginalName.empty())
                    {
                        lib->setOriginalName($3->mOriginalName);
                    }
                    lib->setLevel(level);
                    lib->setParent(root);
                    root->addLibrary(lib);
                    log("Created lib %s level %d\n",
                                lib->getName().c_str(), level );
                }
                ctx->setCurrentLibrary(lib);
                ctx->pushStatusContainer(lib);
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
            inDriver.getContext()->popStatusContainer();
        }
        ;

_Library : Technology
         | _Library Status
         | _Library Cell
         | _Library Comment
           {
                EdifContextSharedPtr ctx = inDriver.getContext();
                LibrarySharedPtr lib = ctx->getCurrentLibrary();
                lib->addComment(*$2);
                delete $2;
           }
         | _Library UserData
           {
                EdifContextSharedPtr ctx = inDriver.getContext();
                LibrarySharedPtr lib = ctx->getCurrentLibrary();
                lib->addUserData(*$2);
                delete $2;
           }
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
             | _ListOfPorts PortBundle
             ;

LoadDelayData : LBKT LOADDELAY _LoadDelayData _LoadDelayData RBKT
            {
                boost::scoped_ptr<ValueData> delay($3);
                boost::scoped_ptr<ValueData> acLoad($4);
                $$ = new PortDelay();
                $$->setType(PortDelay::eTypeLoadDelay);
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
                {
                    delete $1;
                }
            |   Table
            ;

_LogicAssn :
           |    Delay
           |    LoadDelay
           ;

LogicIn : LBKT LOGICINPUT 
        {
            LogicalResponseSharedPtr logicalResponse;
            EdifContextSharedPtr ctx
                        = inDriver.getContext();
            try
            {
                ctx->getFactory()->create(logicalResponse);
            }
            catch(Error &e)
            {
                std::string message = constructErrorMessage(
                        "Unable to create logicInput/logicOutput" );
                log("%s\n", message.c_str());
                e.setCurrentLocation( __FUNCTION__,
                                        __FILE__, __LINE__ );
                e.saveContextData("Parser error message", message);
                e.saveContextData("Filename", yyloc.begin.filename);
                e.saveContextData("StartLine", yyloc.begin.line);
                e.saveContextData("EndLine", yyloc.end.line);
                inDriver.setParserError(e);
                error(yyloc, message);
                YYABORT;
            }
            logicalResponse->setResponseType(LogicalResponse::eResponseTypeInput);
            ctx->setCurrentLogicalResponse(logicalResponse);
            ApplySharedPtr apply
                    = ctx->getCurrentApply();
            apply->addLogicResponse(logicalResponse);
        } _LogicIn RBKT
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            ctx->setCurrentLogicalResponse(LogicalResponseSharedPtr());
        }
        ;

_LogicIn :  CommonPortData
         |  _LogicIn LogicWave
         ; 

CommonPortData :  PortListData
            {
                if(NULL != $1 && !$1->mPortRefs.empty())
                {
                    boost::scoped_ptr<PortListData> portListData($1);
                    std::string message;

                    EdifContextSharedPtr ctx = inDriver.getContext();
                    LogicalResponseSharedPtr logicalResponse 
                                = ctx->getCurrentLogicalResponse();

                    PortListSharedPtr portList;
                    ctx->getFactory()->create(portList);    

                    for( std::vector<PortRefData *>::iterator it
                            = portListData->mPortRefs.begin();
                            it != portListData->mPortRefs.end(); ++it )
                    {
                        PortRefData *portData = *it;
                        std::vector< std::string > nestedNames;
                        PortRefData *currPortRef = *it;
                        PortRefData *nextPortRef = portData->mParentPort;
                        nestedNames.push_back(currPortRef->mPortName->mName);
                        while(nextPortRef)
                        {
                            currPortRef = nextPortRef;
                            nestedNames.push_back(nextPortRef->mPortName->mName);
                            nextPortRef = currPortRef->mParentPort;
                        }
                        bool isPort = (NULL == currPortRef->mInstanceName);
                        NameData *portNameData = currPortRef->mPortName;
                        std::string topName = *(nestedNames.rbegin());
                        ViewSharedPtr view = ctx->getCurrentView();
                        if(isPort)
                        {
                            PortSharedPtr port = view->findPort(topName);
                            if(!port)
                            {
                                SimulateSharedPtr simulate 
                                            = ctx->getCurrentSimulate(); 
                                PortListAliasSharedPtr portList 
                                            = simulate->findPortListAlias(topName);
                                if(!portList)
                                {
                                    std::string message = constructErrorMessage(
                                            "No port %s found in view/portListAlias\n", 
                                                portNameData->mName.c_str());
                                    log("%s\n", message.c_str());
                                    Error e( eMessageIdParserError,
                                        __FUNCTION__, __FILE__, __LINE__ );
                                    e.saveContextData("Parser error message", message);
                                    e.saveContextData("Filename", yyloc.begin.filename);
                                    e.saveContextData("StartLine", yyloc.begin.line);
                                    e.saveContextData("EndLine", yyloc.end.line);
                                    inDriver.setParserError(e);
                                    error(yyloc, message);
                                    YYABORT;
                                }
                            }

                            try
                            {
                                connectibleFinder(nestedNames, port);
                                if(!portData->mPortName->mIndices.empty())
                                {
                                    port = port->get(portData->mPortName->mIndices);
                                }
                                portList->addChildPort(port);
                            }
                            catch(Error &e)
                            {
                                std::string message = constructErrorMessage(
                                        "Unable to connect port : %s", 
                                                port->getName().c_str());
                                log("%s\n", message.c_str());
                                e.setCurrentLocation( __FUNCTION__,
                                                        __FILE__, __LINE__ );
                                e.saveContextData("Parser error message", message);
                                e.saveContextData("Filename", yyloc.begin.filename);
                                e.saveContextData("StartLine", yyloc.begin.line);
                                e.saveContextData("EndLine", yyloc.end.line);
                                inDriver.setParserError(e);
                                error(yyloc, message);
                                YYABORT;
                            }
                        }
                        else
                        {
                        }
                    }
                    logicalResponse->setConnectedPortList(portList);
                }
            }
         |  PortRefData
            {
                boost::scoped_ptr<PortRefData> portData($1);
                NameData *portNameData = portData->mPortName;
                std::string name = portNameData->mName;
                EdifContextSharedPtr ctx
                                = inDriver.getContext();
                ViewSharedPtr view = ctx->getCurrentView();

                PortRefData *currPortRef = $1;
                PortRefData *nextPortRef = portData->mParentPort;
                std::vector< std::string > nestedNames;
                nestedNames.push_back(currPortRef->mPortName->mName);
                while(nextPortRef)
                {
                    currPortRef = nextPortRef;
                    nestedNames.push_back(nextPortRef->mPortName->mName);
                    nextPortRef = currPortRef->mParentPort;
                }

                bool isPort = (NULL == currPortRef->mInstanceName);
                std::string topName = *(nestedNames.rbegin());
                if(isPort)
                {
                    PortSharedPtr port = view->findPort(topName);
                    if(!port)
                    {
                        std::string message = constructErrorMessage(
                                "No port %s found in view %s\n", 
                                            name.c_str(),
                                            view->getName().c_str());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }

                    LogicalResponseSharedPtr logicalResponse 
                                = ctx->getCurrentLogicalResponse();
                    try
                    {             
                        connectibleFinder(nestedNames, port);  
                        if(!portData->mPortName->mIndices.empty())
                        {
                            port = port->get(portData->mPortName->mIndices);
                        }
                        logicalResponse->setConnectedPort(port);
                    }
                    catch(Error &e)
                    {
                        e.setCurrentLocation(
                            __FUNCTION__, __FILE__, __LINE__ );
                        throw;
                    }
                }
            }
         |  PortNameRefData
            {
                boost::scoped_ptr<NameData> portData($1);
                std::string name = portData->mName;
                EdifContextSharedPtr ctx
                                = inDriver.getContext();
                ViewSharedPtr view = ctx->getCurrentView();
                PortSharedPtr port = view->findPort(name);
                LogicalResponseSharedPtr logicalResponse 
                                = ctx->getCurrentLogicalResponse();
                if(!port)
                {
                    SimulateSharedPtr simulate 
                                = ctx->getCurrentSimulate(); 
                    PortListAliasSharedPtr portListAlias 
                                = simulate->findPortListAlias(name);
                    if(!portListAlias)
                    {
                        std::string message = constructErrorMessage(
                                "No port %s found in view/portListAlias\n", 
                                            name.c_str());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    else
                    {
                        logicalResponse->setConnectedPortListAlias(portListAlias);
                    }
                }
                else
                {
                    logicalResponse->setConnectedPort(port);
                }
            }
         ;

LogicList : LBKT LOGICLIST 
            {
                LogicElementSharedPtr logicElem;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                try
                {
                    ctx->getFactory()->create(logicElem);
                    logicElem->setType(LogicElement::eTypeList);
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create logicList" );
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                ctx->pushLogicElement(logicElem);
                ctx->incrementLogicElementDepth();
            } _LogicList RBKT
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                LogicElementSharedPtr currentLogicElem
                            = ctx->getCurrentLogicElement();
                ctx->popLogicElement();
                LogicElementSharedPtr parentLogicElem;
                if(ctx->getLogicElementDepth() > 1 )
                {
                    parentLogicElem = ctx->getCurrentLogicElement();
                }
                ctx->decrementLogicElementDepth();
                if(parentLogicElem)
                {                    
                    parentLogicElem->setRelationType( 
                            LogicElement::eRelationTypeParent );
                    currentLogicElem->setRelationType( 
                            LogicElement::eRelationTypeChild );
                    //Check size
                    size_t parentLogicElemSize = parentLogicElem->getSize();
                    size_t currentLogicElemSize = currentLogicElem->getSize();

                    log("Parent LogicList Size :: %d\n",
                            parentLogicElemSize); 
                    log("Current LogicList Size :: %d\n", 
                            currentLogicElemSize);

                    //std::cout << "Parent LogicList Size :: " << parentLogicElemSize << std::endl;
                    //std::cout << "Current LogicList Size :: " << currentLogicElemSize << std::endl;
 
                    if(parentLogicElemSize != 0)                   
                    {
                        if(parentLogicElemSize == currentLogicElemSize)
                        {
                            parentLogicElem->addChildLogicElement(currentLogicElem);
                        }
                        else
                        {
                            std::string message = constructErrorMessage(
                                    "Child logicList can't be added, size mismatch %d with %d",
                                        parentLogicElemSize,
                                        currentLogicElemSize);
                            log("%s\n", message.c_str());
                            Error e( eMessageIdParserError,
                                __FUNCTION__, __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
                            YYABORT;
                        }
                    }
                    else
                    {
                        parentLogicElem->addChildLogicElement(currentLogicElem);
                    }
                } 
                else
                {
                    ctx->pushLogicElement(currentLogicElem);
                    ctx->incrementLogicElementDepth();
                }              
            }
          ;

_LogicList :
           |    _LogicList LogicNameData
           |    _LogicList LogicOneOf
           |    _LogicList Ignore
           ;

LogicMapIn : LBKT LOGICMAPINPUT LogicRef RBKT
            {
                $$ = $3;
            }
           ;

LogicMapOut : LBKT LOGICMAPOUTPUT LogicRef RBKT
            {
                $$ = $3;
            }
            ;

LogicNameDef :  NameDefData
             ;

LogicNameRefData :  NameRef
                    {
                        $$ = $1;
                    }
                ;

LogicNameRef :  NameRef
             ;

LogicOneOf : LBKT LOGICONEOF 
            {
                LogicElementSharedPtr logicElem;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                try
                {
                    ctx->getFactory()->create(logicElem);
                    logicElem->setType(LogicElement::eTypeOneOf);
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create logicOneOf" );
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                ctx->pushLogicElement(logicElem);
                ctx->incrementLogicElementDepth();
            } _LogicOneOf RBKT
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                LogicElementSharedPtr currentLogicElem
                            = ctx->getCurrentLogicElement();
                ctx->popLogicElement();
                LogicElementSharedPtr parentLogicElem;
                if(ctx->getLogicElementDepth() > 1 )
                {
                    parentLogicElem = ctx->getCurrentLogicElement();
                }
                ctx->decrementLogicElementDepth();
                if(parentLogicElem)
                {                    
                    parentLogicElem->setRelationType( 
                            LogicElement::eRelationTypeParent );
                    currentLogicElem->setRelationType( 
                            LogicElement::eRelationTypeChild );
                    //Check size
                    size_t parentLogicElemSize = parentLogicElem->getSize();
                    size_t currentLogicElemSize = currentLogicElem->getSize();

                    log("Parent LogicOneOf Size :: %d\n",
                            parentLogicElemSize); 
                    log("Current LogicOneOf Size :: %d\n", 
                            currentLogicElemSize);

                    //std::cout << "Parent LogicOneOf Size :: " << parentLogicElemSize << std::endl;
                    //std::cout << "Current LogicOneOf Size :: " << currentLogicElemSize << std::endl;
                
                    if(parentLogicElemSize != 0)
                    {
                        if(parentLogicElemSize == currentLogicElemSize)
                        {
                            parentLogicElem->addChildLogicElement(currentLogicElem);
                        }
                        else
                        {
                            std::string message = constructErrorMessage(
                                    "Child logictOneOf can't be added, size mismatch %d with %d",
                                        parentLogicElemSize,
                                        currentLogicElemSize);
                            log("%s\n", message.c_str());
                            Error e( eMessageIdParserError,
                                __FUNCTION__, __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
                            YYABORT;
                        }
                    }
                    else
                    {
                        parentLogicElem->addChildLogicElement(currentLogicElem);
                    }
                } 
                else
                {
                    ctx->pushLogicElement(currentLogicElem);
                    ctx->incrementLogicElementDepth();
                }              
            }
           ;

LogicNameData : LogicNameRefData
            {
                boost::scoped_ptr<NameData> logicData($1);
                std::string name = logicData->mName;
                EdifContextSharedPtr ctx
                                = inDriver.getContext();
                LibrarySharedPtr lib 
                                = ctx->getCurrentLibrary();
                SimulationInfoSharedPtr simuInfo
                                = lib->getSimulationInfo();
                if(!simuInfo)
                {
                    std::string message = constructErrorMessage(
                                        "SimulatioInfo not found in %s library",
                                            lib->getName().c_str() );
                    log("%s\n", message.c_str());
                    Error e( eMessageIdParserError,
                        __FUNCTION__, __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                LogicValueSharedPtr logicVal
                                = simuInfo->findLogicValue(name);
                if(!logicVal)
                {
                    SimulateSharedPtr simulate
                                = ctx->getCurrentSimulate();
                    if(simulate)
                    {
                        WaveValueSharedPtr waveValue
                                    = simulate->findWaveValue(name);
                        if(!waveValue)
                        {
                            std::string message = constructErrorMessage(
                                                "Wave Value %s not found",
                                                    logicData->mName.c_str() );
                            log("%s\n", message.c_str());
                            Error e( eMessageIdParserError,
                                __FUNCTION__, __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
                            YYABORT;
                        }
                    }
                    else
                    {
                        std::string message = constructErrorMessage(
                                            "Logic Value %s not found",
                                                name.c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
                LogicElementSharedPtr singleElem;
                try
                {
                    ctx->getFactory()->create(singleElem);
                    singleElem->setType(LogicElement::eTypeSingle);
                    singleElem->setName(name);
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create logic single element" );
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                LogicElementSharedPtr logicElem
                                = ctx->getCurrentLogicElement();
                logicElem->addChildLogicElement(singleElem);
            }
          ;  

_LogicOneOf :
            |   _LogicOneOf LogicNameData
            |   _LogicOneOf LogicList
            ;

LogicOut : LBKT LOGICOUTPUT 
        {
            LogicalResponseSharedPtr logicalResponse;
            EdifContextSharedPtr ctx
                        = inDriver.getContext();
            try
            {
                ctx->getFactory()->create(logicalResponse);
            }
            catch(Error &e)
            {
                std::string message = constructErrorMessage(
                        "Unable to create logicOutput" );
                log("%s\n", message.c_str());
                e.setCurrentLocation( __FUNCTION__,
                                        __FILE__, __LINE__ );
                e.saveContextData("Parser error message", message);
                e.saveContextData("Filename", yyloc.begin.filename);
                e.saveContextData("StartLine", yyloc.begin.line);
                e.saveContextData("EndLine", yyloc.end.line);
                inDriver.setParserError(e);
                error(yyloc, message);
                YYABORT;
            }
            logicalResponse->setResponseType(LogicalResponse::eResponseTypeOutput);
            ctx->setCurrentLogicalResponse(logicalResponse);
            ApplySharedPtr apply
                    = ctx->getCurrentApply();
            apply->addLogicResponse(logicalResponse);
        }  _LogicOut RBKT
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            ctx->setCurrentLogicalResponse(LogicalResponseSharedPtr());
        }
        ;

_LogicOut :     CommonPortData     
          |     _LogicOut LogicWave
          ;

LogicPort : LBKT LOGICPORT _LogicPort RBKT
          ;

_LogicPort :    PortNameDef
           |    _LogicPort Property
           |    _LogicPort Comment
                {
                    delete $2;
                }
           |    _LogicPort UserData
                {
                    delete $2;
                }
           ;

LogicRef : LBKT LOGICREF LogicNameRefData _LogicRef RBKT
            {
                if(NULL == $4)
                {
                    $$ = new LogicRefData();
                }
                else
                {
                    $$ = $4;
                }
                $$->mLogicName = $3;
            }
         ;

_LogicRef :
            {
                $$ = NULL;
            }
          |     LibraryRefData
            {
                $$ = new LogicRefData();
                $$->mLibraryName = $1;
            }
          ;

LogicValue : LBKT LOGICVALUE LogicNameDef
            {
                boost::scoped_ptr<NameData> nameData($3);
                std::string name = nameData->mName;
                EdifContextSharedPtr ctx = inDriver.getContext();
                LogicValueSharedPtr logicVal;
                SimulationInfoSharedPtr simuInfo
                            = ctx->getCurrentSimulationInfo();
                logicVal = simuInfo->findLogicValue(name);
                if(logicVal)
                {
                    log("Found existing logic value\n");
                    std::string message = constructErrorMessage(
                        "Logic value %s already exists in simulation info\n", 
                            name.c_str() );
                    log("%s\n", message.c_str());
                    Error e( eMessageIdParserError,
                        __FUNCTION__, __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                else
                {
                    try
                    {
                        ctx->getFactory()->create(logicVal);
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                            "Unable to create Logic Value : %s", name.c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    logicVal->setName(name);
                    if(!nameData->mOriginalName.empty())
                    {
                        logicVal->setOriginalName(nameData->mOriginalName);
                    }
                    logicVal->setParent(simuInfo);
                    simuInfo->addLogicValue(logicVal);
                    log("Created Logic Value %s\n", logicVal->getName().c_str());
                } 
                ctx->setCurrentLogicValue(logicVal);
                ctx->pushPropertyContainer(logicVal); 
            } _LogicValue RBKT
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                LogicValueSharedPtr logicVal = ctx->getCurrentLogicValue();
                if($5)
                {
                    boost::shared_ptr<LogicValueAttributes> attrib($5);
                    logicVal->setAttributes(attrib);
                }
                ctx->setCurrentLogicValue(LogicValueSharedPtr());
                ctx->popPropertyContainer();
            }
           ;

_LogicValue : 
                {
                    $$ = NULL;
                }
            |   _LogicValue VoltageMap
                {
                    boost::scoped_ptr<ValueData> data($2);
                    $$ = ($1) ? $1 : new LogicValueAttributes();
                    $$->setVoltageMap(data->mValues[0].get<Value::MiNoMax>());
                }
            |   _LogicValue CurrentMap
                {
                    boost::scoped_ptr<ValueData> data($2);
                    $$ = ($1) ? $1 : new LogicValueAttributes();
                    $$->setCurrentMap(data->mValues[0].get<Value::MiNoMax>());
                }
            |   _LogicValue BooleanMap
                {
                    $$ = ($1) ? $1 : new LogicValueAttributes();
                    Value::Boolean val = $2;
                    $$->setBooleanMap(val);
                }
            |   _LogicValue Compound
                {
                    $$ = ($1) ? $1 : new LogicValueAttributes();
                    std::list< LogicValueSharedPtr > logicValues;
                    boost::scoped_ptr<LogicListData> logicListData($2);
                    if(NULL != $2 && !$2->mNameDataList.empty())
                    {
                        for( std::list<NameData *>::iterator it
                                = logicListData->mNameDataList.begin();
                                it != logicListData->mNameDataList.end(); it++ )
                        {
                            NameData *pNameData = *it;
                            std::string name = pNameData->mName;
                            EdifContextSharedPtr ctx = inDriver.getContext();
                            LogicValueSharedPtr logicVal;
                            SimulationInfoSharedPtr simuInfo
                                    = ctx->getCurrentSimulationInfo();
                            logicVal = simuInfo->findLogicValue(name); 
                            if(!logicVal)
                            {
                                std::string message = constructErrorMessage(
                                                    "Logic Value %s not found",
                                                        name.c_str() );
                                log("%s\n", message.c_str());
                                Error e( eMessageIdParserError,
                                    __FUNCTION__, __FILE__, __LINE__ );
                                e.saveContextData("Parser error message", message);
                                e.saveContextData("Filename", yyloc.begin.filename);
                                e.saveContextData("StartLine", yyloc.begin.line);
                                e.saveContextData("EndLine", yyloc.end.line);
                                inDriver.setParserError(e);
                                error(yyloc, message);
                                YYABORT;
                            }
                            else
                            {
                                if(!logicValues.empty())
                                {
                                    std::list< LogicValueSharedPtr >::iterator logicIt 
                                        = std::find(logicValues.begin(), logicValues.end(), logicVal); 
                                    if(logicIt != logicValues.end())
                                    {
                                        std::string message = constructErrorMessage(
                                                "Logic value %s already exist in compound list",
                                                     name.c_str());
                                        log("%s\n", message.c_str());
                                        Error e( eMessageIdParserError,
                                            __FUNCTION__, __FILE__, __LINE__ );
                                        e.saveContextData("Parser error message", message);
                                        e.saveContextData("Filename", yyloc.begin.filename);
                                        e.saveContextData("StartLine", yyloc.begin.line);
                                        e.saveContextData("EndLine", yyloc.end.line);
                                        inDriver.setParserError(e);
                                        error(yyloc, message);
                                        YYABORT;
                                    }
                                    else
                                    {
                                        logicValues.push_back(logicVal);
                                    }
                                }
                                else
                                {
                                    logicValues.push_back(logicVal);
                                }
                            }
                        }
                        $$->setCompoundLogicValues(logicValues);
                    }
                }
            |   _LogicValue Weak
                {
                    boost::scoped_ptr<NameData> pNameData($2);
                    std::string name = pNameData->mName;
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    LogicValueSharedPtr logicVal;
                    SimulationInfoSharedPtr simuInfo
                                = ctx->getCurrentSimulationInfo();
                    logicVal = simuInfo->findLogicValue(name);
                    if(!logicVal)
                    {
                        std::string message = constructErrorMessage(
                                            "Logic Value %s not found",
                                                name.c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    else
                    {
                        $$ = ($1) ? $1 : new LogicValueAttributes();
                        $$->setWeakLogicValue(logicVal);
                    }
                }
            |   _LogicValue Strong
                {
                    boost::scoped_ptr<NameData> pNameData($2);
                    std::string name = pNameData->mName;
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    LogicValueSharedPtr logicVal;
                    SimulationInfoSharedPtr simuInfo
                                = ctx->getCurrentSimulationInfo();
                    logicVal = simuInfo->findLogicValue(name);
                    if(!logicVal)
                    {
                        std::string message = constructErrorMessage(
                                            "Logic Value %s not found",
                                                name.c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    else
                    {
                        $$ = ($1) ? $1 : new LogicValueAttributes();
                        $$->setStrongLogicValue(logicVal);
                    }
                }
            |   _LogicValue Dominates
                {
                    $$ = ($1) ? $1 : new LogicValueAttributes();
                    std::list< LogicValueSharedPtr > logicValues;
                    boost::scoped_ptr<LogicListData> logicListData($2);
                    if(NULL != $2 && !$2->mNameDataList.empty())
                    {
                        for( std::list<NameData *>::iterator it
                                = logicListData->mNameDataList.begin();
                                it != logicListData->mNameDataList.end(); it++ )
                        {
                            NameData *pNameData = *it;
                            std::string name = pNameData->mName;
                            EdifContextSharedPtr ctx = inDriver.getContext();
                            LogicValueSharedPtr logicVal;
                            SimulationInfoSharedPtr simuInfo
                                    = ctx->getCurrentSimulationInfo();
                            logicVal = simuInfo->findLogicValue(name); 
                            if(!logicVal)
                            {
                                std::string message = constructErrorMessage(
                                                    "Logic Value %s not found",
                                                        name.c_str() );
                                log("%s\n", message.c_str());
                                Error e( eMessageIdParserError,
                                    __FUNCTION__, __FILE__, __LINE__ );
                                e.saveContextData("Parser error message", message);
                                e.saveContextData("Filename", yyloc.begin.filename);
                                e.saveContextData("StartLine", yyloc.begin.line);
                                e.saveContextData("EndLine", yyloc.end.line);
                                inDriver.setParserError(e);
                                error(yyloc, message);
                                YYABORT;
                            }
                            else
                            {
                                if(!logicValues.empty())
                                {
                                    std::list< LogicValueSharedPtr >::iterator logicIt 
                                        = std::find(logicValues.begin(), logicValues.end(), logicVal); 
                                    if(logicIt != logicValues.end())
                                    {
                                        std::string message = constructErrorMessage(
                                                "Logic value %s already exist in dominates list",
                                                     name.c_str());
                                        log("%s\n", message.c_str());
                                        Error e( eMessageIdParserError,
                                            __FUNCTION__, __FILE__, __LINE__ );
                                        e.saveContextData("Parser error message", message);
                                        e.saveContextData("Filename", yyloc.begin.filename);
                                        e.saveContextData("StartLine", yyloc.begin.line);
                                        e.saveContextData("EndLine", yyloc.end.line);
                                        inDriver.setParserError(e);
                                        error(yyloc, message);
                                        YYABORT;
                                    }
                                    else
                                    {
                                        logicValues.push_back(logicVal);
                                    }
                                }
                                else
                                {
                                    logicValues.push_back(logicVal);
                                }
                            }
                        }
                        $$->setDominatedLogicValues(logicValues);
                    }
                }
            |   _LogicValue LogicMapOut
                {
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    boost::scoped_ptr<LogicRefData> logicRefData($2);
                    std::string logicValName = logicRefData->mLogicName->mName;
                    std::string libName;
                    bool isLocalLib = (NULL == logicRefData->mLibraryName);
                    if(isLocalLib)
                    {
                        libName = ctx->getCurrentLibrary()->getName();     
                    }
                    else
                    {
                        libName = logicRefData->mLibraryName->mName;
                    }    
                    LibrarySharedPtr library 
                                        = ctx->getRoot()->findLibrary(libName) ;
                    if(!library)
                    {
                        std::string message = constructErrorMessage(
                                            "Library %s not found",
                                                    libName.c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    LogicValueSharedPtr logicVal;
                    SimulationInfoSharedPtr simuInfo
                                = library->getSimulationInfo();
                    if(!simuInfo)
                    {
                        std::string message = constructErrorMessage(
                                            "SimulatioInfo not found in %s library",
                                                library->getName().c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    logicVal = simuInfo->findLogicValue(logicValName);
                    if(!logicVal)
                    {
                        std::string message = constructErrorMessage(
                                            "Logic Value %s not found",
                                                logicValName.c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    else
                    {
                        $$ = ($1) ? $1 : new LogicValueAttributes();
                        LogicValueAttributes::LogicMap *logicMap 
                                = new LogicValueAttributes::LogicMap();
                        logicMap->mLogicRef = logicValName;
                        logicMap->mLibraryRef = libName;
                        logicMap->mLogicMapType 
                                = LogicValueAttributes::LogicMap::eLogicMapTypeOutput;
                        $$->addLogicMap(logicMap);
                    }
                }
            |   _LogicValue LogicMapIn
                {
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    boost::scoped_ptr<LogicRefData> logicRefData($2);
                    std::string logicValName = logicRefData->mLogicName->mName;
                    std::string libName;
                    bool isLocalLib = (NULL == logicRefData->mLibraryName);
                    if(isLocalLib)
                    {
                        libName = ctx->getCurrentLibrary()->getName();     
                    }
                    else
                    {
                        libName = logicRefData->mLibraryName->mName;
                    }    
                    LibrarySharedPtr library 
                                        = ctx->getRoot()->findLibrary(libName) ;
                    if(!library)
                    {
                        std::string message = constructErrorMessage(
                                            "Library %s not found",
                                                    libName.c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    LogicValueSharedPtr logicVal;
                    SimulationInfoSharedPtr simuInfo
                                = library->getSimulationInfo();
                    if(!simuInfo)
                    {
                        std::string message = constructErrorMessage(
                                            "SimulatioInfo not found in %s library",
                                                library->getName().c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    logicVal = simuInfo->findLogicValue(logicValName);
                    if(!logicVal)
                    {
                        std::string message = constructErrorMessage(
                                            "Logic Value %s not found",
                                                logicValName.c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    else
                    {
                        $$ = ($1) ? $1 : new LogicValueAttributes();
                        LogicValueAttributes::LogicMap *logicMap 
                                = new LogicValueAttributes::LogicMap();
                        logicMap->mLogicRef = logicValName;
                        logicMap->mLibraryRef = libName;
                        logicMap->mLogicMapType 
                                = LogicValueAttributes::LogicMap::eLogicMapTypeInput;
                        $$->addLogicMap(logicMap);
                    }
                }
            |   _LogicValue Isolated
                {
                    $$ = ($1) ? $1 : new LogicValueAttributes();
                    $$->setIsIsolated(true);
                }
            |   _LogicValue Resolves
                {
                    $$ = ($1) ? $1 : new LogicValueAttributes();
                    std::list< LogicValueSharedPtr > logicValues;
                    boost::scoped_ptr<LogicListData> logicListData($2);
                    if(NULL != $2 && !$2->mNameDataList.empty())
                    {
                        for( std::list<NameData *>::iterator it
                                = logicListData->mNameDataList.begin();
                                it != logicListData->mNameDataList.end(); it++ )
                        {
                            NameData *pNameData = *it;
                            std::string name = pNameData->mName;
                            EdifContextSharedPtr ctx = inDriver.getContext();
                            LogicValueSharedPtr logicVal;
                            SimulationInfoSharedPtr simuInfo
                                    = ctx->getCurrentSimulationInfo();
                            logicVal = simuInfo->findLogicValue(name); 
                            if(!logicVal)
                            {
                                std::string message = constructErrorMessage(
                                                    "Logic Value %s not found",
                                                        name.c_str() );
                                log("%s\n", message.c_str());
                                Error e( eMessageIdParserError,
                                    __FUNCTION__, __FILE__, __LINE__ );
                                e.saveContextData("Parser error message", message);
                                e.saveContextData("Filename", yyloc.begin.filename);
                                e.saveContextData("StartLine", yyloc.begin.line);
                                e.saveContextData("EndLine", yyloc.end.line);
                                inDriver.setParserError(e);
                                error(yyloc, message);
                                YYABORT;
                            }
                            else
                            {
                                if(!logicValues.empty())
                                {
                                    std::list< LogicValueSharedPtr >::iterator logicIt 
                                        = std::find(logicValues.begin(), logicValues.end(), logicVal); 
                                    if(logicIt != logicValues.end())
                                    {
                                        std::string message = constructErrorMessage(
                                                "Logic value %s already exist in resolve list",
                                                     name.c_str());
                                        log("%s\n", message.c_str());
                                        Error e( eMessageIdParserError,
                                            __FUNCTION__, __FILE__, __LINE__ );
                                        e.saveContextData("Parser error message", message);
                                        e.saveContextData("Filename", yyloc.begin.filename);
                                        e.saveContextData("StartLine", yyloc.begin.line);
                                        e.saveContextData("EndLine", yyloc.end.line);
                                        inDriver.setParserError(e);
                                        error(yyloc, message);
                                        YYABORT;
                                    }
                                    else
                                    {
                                        logicValues.push_back(logicVal);
                                    }
                                }
                                else
                                {
                                    logicValues.push_back(logicVal);
                                }
                            }
                        }
                        $$->setResolvedLogicValues(logicValues);
                    }
                }
            |   _LogicValue Property
                {   
                    $$ = $1;
                }
            |   _LogicValue Comment
                {
                    $$ = $1;
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    LogicValueSharedPtr logicVal = ctx->getCurrentLogicValue();
                    logicVal->addComment(*$2);
                    delete $2;
                }
            |   _LogicValue UserData
                {
                    $$ = $1;
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    LogicValueSharedPtr logicVal = ctx->getCurrentLogicValue();
                    logicVal->addUserData(*$2);
                    delete $2;
                }
            ;

LogicWave : LBKT LOGICWAVEFORM 
            {
                LogicElementSharedPtr logicElem;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                try
                {
                    ctx->getFactory()->create(logicElem);
                    logicElem->setType(LogicElement::eTypeWaveForm);
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create logicWaveForm" );
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                if(ctx->getLogicElementDepth() == 0 )
                {
                    LogicalResponseSharedPtr logicalResponse
                            = ctx->getCurrentLogicalResponse();
                    if(logicalResponse)
                    {
                        logicalResponse->setLogicWaveForm(logicElem);
                    }

                    WaveValueSharedPtr waveValue
                            = ctx->getCurrentWaveValue();
                    if(waveValue)
                    {
                        waveValue->setLogicWaveform(logicElem);
                    }
                }
                ctx->pushLogicElement(logicElem);
                ctx->incrementLogicElementDepth();
            } _LogicWave RBKT
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                LogicElementSharedPtr currentLogicElem
                            = ctx->getCurrentLogicElement();
                ctx->popLogicElement();
                LogicElementSharedPtr parentLogicElem;
                if(ctx->getLogicElementDepth() > 1 )
                {
                    parentLogicElem = ctx->getCurrentLogicElement();
                }
                ctx->decrementLogicElementDepth();
                if(parentLogicElem)
                {                    
                    parentLogicElem->setRelationType( 
                            LogicElement::eRelationTypeParent );
                    currentLogicElem->setRelationType( 
                            LogicElement::eRelationTypeChild );
                    //Check size
                    //size_t parentLogicElemSize = parentLogicElem->getSize();
                    //size_t currentLogicElemSize = currentLogicElem->getSize();

                    //log("Parent LogicWaveForm Size :: %d\n",
                    //        parentLogicElemSize); 
                    //log("Current LogicWaveForm Size :: %d\n", 
                    //        currentLogicElemSize);

                    //std::cout << "Parent LogicWaveForm Size :: " << parentLogicElemSize << std::endl;
                    //std::cout << "Current LogicWaveForm Size :: " << currentLogicElemSize << std::endl;
                
                    parentLogicElem->addChildLogicElement(currentLogicElem);
                } 
                else
                {
                  ctx->pushLogicElement(currentLogicElem);
                }              
            }
          ;

_LogicWave :
           |    _LogicWave LogicNameData
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
            $$->mIndices.push_back($4);
            if($5)
            {
                $$->mIndices.insert($$->mIndices.end(),
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
            if($4);
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
            if(NULL == $3)
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
                $$ = ($1) ? $1 : new ValueData();
                $$->mValues.push_back( Value(
                            Value::eValueTypeMiNoMax,
                            $2->mValues[0].get<Value::MiNoMax>() ) );
                delete $2;
            }
        |       _MiNoMa MiNoMaDisp
            {
                $$ = ($1) ? $1 : new ValueData();
                $$->mValues.push_back( Value(
                            Value::eValueTypeMiNoMax,
                            $2->mValues[0].get<Value::MiNoMax>() ) );
                delete $2;
            }
        |       _MiNoMa MiNoMa
         {
            $$ = $2;
            if($1)
            {
               $1->mValues.insert($1->mValues.end(),
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
                Value::Number num($1->mX, $1->mY);
                val.setNominal(num);
                $$ = new ValueData();
                $$->mValues.push_back(
                        Value(Value::eValueTypeMiNoMax, val) );
                delete $1;
            }
            ;

Mnm : LBKT MNM _Mnm _Mnm _Mnm RBKT
    {
        Value::MiNoMax val;
        if($3)
        {
            Value::Number num($3->mX, $3->mY);
            val.setMin(num);
        }
        if($4)
        {
            Value::Number num($4->mX, $4->mY);
            val.setNominal(num);
        }
        if($5)
        {
            Value::Number num($5->mX, $5->mY);
            val.setMax(num);
        }
        $$ = new ValueData();
        $$->mValues.push_back(
                        Value(Value::eValueTypeMiNoMax, val) );
        delete $3;
        delete $4;
        delete $5;
    }
    ;

_Mnm :  ScaledInt
        {
            $$ = $1;
        }
     |  Undefined
        {
            $$ = NULL;
        }
     |  Unconstrained
        {
            $$ = NULL;
        }
     ;

MultValSet : LBKT MULTIPLEVALUESET _MultValSet RBKT
           ;

_MultValSet :
            |   _MultValSet RangeVector
            ;

MustJoin : LBKT MUSTJOIN 
            {
                InterfaceJoinedInfoSharedPtr joinedInfo;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                try
                {
                    ctx->getFactory()->create(joinedInfo);
                    joinedInfo->setJoinedType( 
                            InterfaceJoinedInfo::eJoinedTypeMust );
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create must joined info" );
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                if(ctx->getInterfaceJoinedInfoDepth() == 0 )
                {
                    ctx->getCurrentView()->addInterfaceJoinedInfo(joinedInfo);
                }
                ctx->pushInterfaceJoinedInfo(joinedInfo);
                ctx->incrementInterfaceJoinedInfoDepth();
            } _MustJoin RBKT
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                InterfaceJoinedInfoSharedPtr currentJoinedInfo 
                        = ctx->getCurrentInterfaceJoinedInfo();
                ctx->popInterfaceJoinedInfo();
                InterfaceJoinedInfoSharedPtr parentJoinedInfo; 
                if(ctx->getInterfaceJoinedInfoDepth() > 1 )
                {
                    parentJoinedInfo = ctx->getCurrentInterfaceJoinedInfo();
                }
                ctx->decrementInterfaceJoinedInfoDepth();
                if(parentJoinedInfo)
                {
                    parentJoinedInfo->setRelationType( 
                            InterfaceJoinedInfo::eRelationTypeParent );
                    currentJoinedInfo->setRelationType( 
                            InterfaceJoinedInfo::eRelationTypeChild );
                    //Check size
                    size_t parentJoinedInfoSize = parentJoinedInfo->getSize();
                    size_t currentJoinedInfoSize = currentJoinedInfo->getSize();

                    log("Parent joined info Size :: %d\n",
                            parentJoinedInfoSize ); 
                    log("Current joined info Size :: %d\n", 
                            currentJoinedInfoSize );

                    //std::cout << "Parent joined info Size :: " << parentJoinedInfoSize << std::endl;
                    //std::cout << "Current joined info Size :: " << currentJoinedInfoSize << std::endl;

                    std::vector< InterfaceJoinedInfoSharedPtr > outJoinedInfos;
                    parentJoinedInfo->getChildren(outJoinedInfos);

                    std::list< PortSharedPtr > outPorts;
                    parentJoinedInfo->getPorts(outPorts);
                    
                    std::list< PortListSharedPtr > outPortLists;
                    parentJoinedInfo->getPortLists(outPortLists);
                    
                    if(parentJoinedInfoSize != 0) 
                    {
                        if( parentJoinedInfoSize
                                == currentJoinedInfoSize )
                        {
                            parentJoinedInfo->addChildJoinedInfo(
                                    currentJoinedInfo );
                        }
                        else
                        {
                            std::string message = constructErrorMessage(
                                    "Child joined info can't be added, port size mismatch %d with %d",
                                        parentJoinedInfoSize,
                                        currentJoinedInfoSize);
                            log("%s\n", message.c_str());
                            Error e( eMessageIdParserError,
                                __FUNCTION__, __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
                            YYABORT;
                        }
                    }
                    else
                    {
                        parentJoinedInfo->addChildJoinedInfo(
                                currentJoinedInfo );
                    }
                } 
                else
                {
                    ctx->pushInterfaceJoinedInfo(currentJoinedInfo);
                }              
            }
         ;

_MustJoin :
          |     _MustJoin PortRefData
            {
                boost::scoped_ptr<PortRefData> portData($2);
                NameData *portNameData = portData->mPortName;
                std::string name = portNameData->mName;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                ViewSharedPtr view = ctx->getCurrentView();

                PortRefData *currPortRef = $2;
                PortRefData *nextPortRef = portData->mParentPort;
                std::vector< std::string > nestedNames;
                nestedNames.push_back(currPortRef->mPortName->mName);
                while(nextPortRef)
                {
                    currPortRef = nextPortRef;
                    nestedNames.push_back(nextPortRef->mPortName->mName);
                    nextPortRef = currPortRef->mParentPort;
                }
                bool isPort = (NULL == currPortRef->mInstanceName);
                std::string topName = *(nestedNames.rbegin());
                if(isPort)
                {
                    PortSharedPtr port = view->findPort(topName);
                    if(!port)
                    {
                        std::string message = constructErrorMessage(
                                "No port %s found in view %s\n", 
                                            name.c_str(),
                                            view->getName().c_str());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }

                    InterfaceJoinedInfoSharedPtr currentJoinedInfo 
                                        = ctx->getCurrentInterfaceJoinedInfo();
                    try
                    {             
                        connectibleFinder(nestedNames, port);  
                        if(!portData->mPortName->mIndices.empty())
                        {
                            port = port->get(portData->mPortName->mIndices);
                        }
                        //Check port size
                        std::list< PortSharedPtr > outPorts;
                        currentJoinedInfo->getPorts(outPorts);
                        
                        if(!outPorts.empty())
                        {
                            PortSharedPtr firstPort = outPorts.front();
                            std::list< PortSharedPtr >::iterator it
                                            = std::find(outPorts.begin(), outPorts.end(),
                                                    port);
                            if(it != outPorts.end())
                            {
                                std::string message = constructErrorMessage(
                                            "Port %s already exist in mustJoin info",
                                                port->getName().c_str() );
                                    log("%s\n", message.c_str());
                                    Error e( eMessageIdParserError,
                                        __FUNCTION__, __FILE__, __LINE__ );
                                    e.saveContextData("Parser error message", message);
                                    e.saveContextData("Filename", yyloc.begin.filename);
                                    e.saveContextData("StartLine", yyloc.begin.line);
                                    e.saveContextData("EndLine", yyloc.end.line);
                                    inDriver.setParserError(e);
                                    error(yyloc, message);
                                    YYABORT;
                            }
                            else
                            {
                                if(firstPort->getSize() == port->getSize())
                                {
                                    currentJoinedInfo->addPort(port);
                                }
                                else
                                {
                                    std::string message = constructErrorMessage(
                                            "Port %s can't be added, port size mismatch %d with %d",
                                                name.c_str(),
                                                firstPort->getSize(),
                                                port->getSize());
                                    log("%s\n", message.c_str());
                                    Error e( eMessageIdParserError,
                                        __FUNCTION__, __FILE__, __LINE__ );
                                    e.saveContextData("Parser error message", message);
                                    e.saveContextData("Filename", yyloc.begin.filename);
                                    e.saveContextData("StartLine", yyloc.begin.line);
                                    e.saveContextData("EndLine", yyloc.end.line);
                                    inDriver.setParserError(e);
                                    error(yyloc, message);
                                    YYABORT;
                                }
                            }
                        }
                        else
                        {
                            currentJoinedInfo->addPort(port);
                        }
                    }
                    catch(Error &e)
                    {
                        e.setCurrentLocation(
                            __FUNCTION__, __FILE__, __LINE__ );
                        throw;
                    }
                }
                else
                {
                }
            }
          |     _MustJoin PortListData
            {
                if(NULL != $2 && !$2->mPortRefs.empty())
                {
                    boost::scoped_ptr<PortListData> portListData($2);
                    std::string message;

                    EdifContextSharedPtr ctx = inDriver.getContext();
                    PortListSharedPtr portList;
                    ctx->getFactory()->create(portList);

                    InterfaceJoinedInfoSharedPtr currentJoinedInfo 
                                = ctx->getCurrentInterfaceJoinedInfo();

                    for( std::vector<PortRefData *>::iterator it
                            = portListData->mPortRefs.begin();
                            it != portListData->mPortRefs.end(); ++it )
                    {
                        PortRefData *portData = *it;
                        std::vector< std::string > nestedNames;
                        PortRefData *currPortRef = *it;
                        PortRefData *nextPortRef = portData->mParentPort;
                        nestedNames.push_back(currPortRef->mPortName->mName);
                        while(nextPortRef)
                        {
                            currPortRef = nextPortRef;
                            nestedNames.push_back(nextPortRef->mPortName->mName);
                            nextPortRef = currPortRef->mParentPort;
                        }
                        bool isPort = (NULL == currPortRef->mInstanceName);
                        NameData *portNameData = currPortRef->mPortName;
                        std::string topName = *(nestedNames.rbegin());
                        ViewSharedPtr view = ctx->getCurrentView();
                        if(isPort)
                        {
                            PortSharedPtr port = view->findPort(topName);
                            if(!port)
                            {
                                std::string message = constructErrorMessage(
                                        "No port %s found in view %s\n", 
                                            portNameData->mName.c_str(),
                                            view->getName().c_str());
                                log("%s\n", message.c_str());
                                Error e( eMessageIdParserError,
                                    __FUNCTION__, __FILE__, __LINE__ );
                                e.saveContextData("Parser error message", message);
                                e.saveContextData("Filename", yyloc.begin.filename);
                                e.saveContextData("StartLine", yyloc.begin.line);
                                e.saveContextData("EndLine", yyloc.end.line);
                                inDriver.setParserError(e);
                                error(yyloc, message);
                                YYABORT;
                            }

                            try
                            {
                                connectibleFinder(nestedNames, port);
                                if(!portData->mPortName->mIndices.empty())
                                {
                                    port = port->get(portData->mPortName->mIndices);
                                }
                                portList->addChildPort(port);
                            }
                            catch(Error &e)
                            {
                                std::string message = constructErrorMessage(
                                        "Unable to connect port : %s", 
                                                port->getName().c_str());
                                log("%s\n", message.c_str());
                                e.setCurrentLocation( __FUNCTION__,
                                                        __FILE__, __LINE__ );
                                e.saveContextData("Parser error message", message);
                                e.saveContextData("Filename", yyloc.begin.filename);
                                e.saveContextData("StartLine", yyloc.begin.line);
                                e.saveContextData("EndLine", yyloc.end.line);
                                inDriver.setParserError(e);
                                error(yyloc, message);
                                YYABORT;
                            }
                        }
                        else
                        {
                        }
                    }
                    if(currentJoinedInfo->getSize() == portList->getSize())
                    {
                        currentJoinedInfo->addPortList(portList);
                    }
                    else
                    {
                        std::string message = constructErrorMessage(
                                "Port size mismatch %d with %d",
                                    currentJoinedInfo->getSize(),
                                    portList->getSize());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
            }
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
            boost::scoped_ptr<NameData> pNameData($3);
            std::string name = pNameData->mName;
            EdifContextSharedPtr ctx = inDriver.getContext();
            NetSharedPtr net;
            ViewSharedPtr view = ctx->getCurrentView();
            NetBundleSharedPtr bundle
                                = ctx->getCurrentNetBundleContext();
            bool bExistingNetFound = false;
            if(bundle)
            {
                typedef std::vector<NetSharedPtr > NBList;
                NBList children;
                bundle->getChildren(children);
                for(NBList::iterator it = children.begin();
                        it != children.end(); it++ )
                {
                    if((*it)->getName() == name)
                    {
                        bExistingNetFound = true;
                    }
                }
            }
            else
            {
                net  = view->findNet(name);
                bExistingNetFound = false;
            }
            if(bExistingNetFound)
            {
                std::string message = constructErrorMessage(
                    "View %s already contains a net of name %s\n",
                                view->getName().c_str(),
                                net->getName().c_str() );
                log("%s\n", message.c_str());
                Error e( eMessageIdParserError,
                    __FUNCTION__, __FILE__, __LINE__ );
                e.saveContextData("Parser error message", message);
                e.saveContextData("Filename", yyloc.begin.filename);
                e.saveContextData("StartLine", yyloc.begin.line);
                e.saveContextData("EndLine", yyloc.end.line);
                inDriver.setParserError(e);
                error(yyloc, message);
                YYABORT;
            }
            else
            {
                if(!pNameData->mIndices.empty())
                {
                    try
                    {
                        VectorNetSharedPtr vectorNet;
                        ctx->getFactory()->create(vectorNet);
                        vectorNet->setName(name);
                        vectorNet->constructChildren(
                                            ctx->getFactory(),
                                            pNameData->mIndices );
                        net = vectorNet;
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create net : %s", 
                                        net->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
                else
                {
                    try
                    {
                        ScalarNetSharedPtr scalarNet;
                        ctx->getFactory()->create(scalarNet);
                        net = scalarNet;
                        net->setName(name);
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create net : %s", 
                                        net->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
                if(!pNameData->mOriginalName.empty())
                {
                    net->setOriginalName(
                                    pNameData->mOriginalName );
                }
#if 0
                if(bundle)
                {
                    net->setParentCollection(bundle);
                    bundle->addChild(net);
                    log("Added net to existing bundle\n");
                }
                else
                {
                    ctx->pushCurrentNet(net);
                    NetSharedPtr currentNet = ctx->getCurrentNet();
                    ctx->popCurrentNet();

                    NetSharedPtr parentNet = ctx->getCurrentNet();
                    if(parentNet)
                    {
                        parentNet->addSubnet(currentNet);
                        printf("Current net %s ",
                            net->getName().c_str() );
                    }
                    else
                    {
                        view->addNet(net);
                        net->setParent(view);
                    }
                }
#endif
                if(pNameData->mIndices.empty())
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
            ctx->pushCurrentNet(net);
            ctx->pushPropertyContainer(net);
        } JoinedElements _Net RBKT
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            ViewSharedPtr view = ctx->getCurrentView();
            NetSharedPtr currentNet  = ctx->getCurrentNet();
            if($6) 
            {
                NetAttributesSharedPtr attrib($6);
                currentNet->setAttributes(attrib);
            }
            ctx->popCurrentNet();
            NetSharedPtr parentNet = ctx->getCurrentNet();

            NetBundleSharedPtr bundle
                                = ctx->getCurrentNetBundleContext();
            if(bundle)
            {
                if(parentNet)
                {
                    parentNet->addSubnet(currentNet);
                }
                else
                {
                    currentNet->setParentCollection(bundle);
                    bundle->addChild(currentNet);
                }
                log("Added net to existing bundle\n");
            }
            else
            {
                if(parentNet)
                {
                    parentNet->addSubnet(currentNet);
                }
                else
                {
                    view->addNet(currentNet);
                    currentNet->setParent(view);
                }
            }
            inDriver.getContext()->popPropertyContainer();
        }
        ;

_Net :
        {
            $$ = NULL;
        }          
     |  _Net Criticality
        {
            $$ = ($1)?$1:new NetAttributes();
            $$->setCriticality($2);
        }
     |  _Net NetDelay
        {
            boost::scoped_ptr<NetDelay> netDelay($2);
            $$ = ($1)?$1:new NetAttributes();
            $$->setNetDelay(*netDelay);
        }
     |  _Net Figure
        {
            $$ = $1;
        }
     |  _Net Net
        {
            $$ = $1;
        }
     |  _Net Instance
        {
            $$ = $1;
        }
     |  _Net CommGraph
        {
            $$ = $1;
        }
     |  _Net Property
        {
            $$ = $1;
        }
     |  _Net Comment
        {
            $$ = $1;
            EdifContextSharedPtr ctx = inDriver.getContext();
            NetSharedPtr net = ctx->getCurrentNet();
            net->addComment(*$2);
            delete $2;
        }
     |  _Net UserData
        {
            $$ = $1;
            EdifContextSharedPtr ctx = inDriver.getContext();
            NetSharedPtr net = ctx->getCurrentNet();
            net->addUserData(*$2);
            delete $2;
        }
     ;

NetBackAn : LBKT NETBACKANNOTATE _NetBackAn RBKT
          ;

_NetBackAn :    NetRef
           |    _NetBackAn NetDelay
                {
                    if($2);
                }
           |    _NetBackAn Criticality
                { 
                    if($2); 
                }
           |    _NetBackAn Property
           |    _NetBackAn Comment
                {
                    delete $2;
                }
           ;

NetBundle : LBKT NETBUNDLE NetNameDef
            {
                boost::scoped_ptr<NameData> pNameData($3);
                std::string name = pNameData->mName;
                EdifContextSharedPtr ctx
                                                = inDriver.getContext();
                NetSharedPtr net;
                ViewSharedPtr view = ctx->getCurrentView();
                NetBundleSharedPtr bundle
                                = ctx->getCurrentNetBundleContext();
                if(bundle)
                {
                    typedef std::vector<NetSharedPtr > NBList;
                    NBList children;
                    bundle->getChildren(children);
                    for(NBList::iterator it = children.begin();
                            it != children.end(); it++ )
                    {
                        if((*it)->getName() == name)
                        {
                            net = *it;
                        }
                    }
                }
                else
                {
                    net  = view->findNet(name);
                }
                if(net)
                {
                    log("Found existing net\n");
                    std::string message = constructErrorMessage(
                        "View %s already contains a net of name %s\n",
                                    view->getName().c_str(),
                                    net->getName().c_str() );
                    Error e( eMessageIdParserError,
                        __FUNCTION__, __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                else
                {
                    try
                    {
                        NetBundleSharedPtr netBundle;
                        ctx->getFactory()->create(netBundle);
                        ctx->pushNetBundleContext(netBundle);
                        net = netBundle;
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create net bundle : %s", 
                                        net->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    net->setName(name);
                    net->setParent(view);
                    if(!pNameData->mOriginalName.empty())
                    {
                        net->setOriginalName(pNameData->mOriginalName);
                    }
                    if(bundle)
                    {
                        net->setParentCollection(bundle);
                        bundle->addChild(net);
                        log("Added net to existing bundle\n");
                    }
                    else
                    {
                        view->addNet(net);
                    }
                    log( "Created net bundle %s\n",
                                            net->getName().c_str() );
                }
                //ctx->pushCurrentNet(net);
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
                {
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    NetBundleSharedPtr bundle 
                                = ctx->getCurrentNetBundleContext();
                    bundle->addComment(*$2);
                    delete $2;
                }
           |    _NetBundle UserData
                {
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    NetBundleSharedPtr bundle 
                                = ctx->getCurrentNetBundleContext();
                    bundle->addUserData(*$2);
                    delete $2;
                }
           ;

NetDelay : LBKT NETDELAY Derivation _NetDelay RBKT
            {
                $$ = $4;
                $$->setDerivation(
                        static_cast<Derivation>($3) );
            }
            ;


_NetDelay : NetDelayData
          {
            $$ = $1;
          }
          | _NetDelay Transition
          {
            $$ = ($1) ? $1 : new NetDelay();
            EdifContextSharedPtr ctx
                        = inDriver.getContext();
            LogicElementSharedPtr logicElem
                            = ctx->getCurrentLogicElement();
            if(logicElem)
            {
                $$->setTransition(logicElem);
            }
          }  
          | _NetDelay Becomes
          {
            $$ = ($1) ? $1 : new NetDelay();
            EdifContextSharedPtr ctx
                        = inDriver.getContext();
            LogicElementSharedPtr logicElem
                            = ctx->getCurrentLogicElement();
            if(logicElem)
            {
                $$->setTransition(logicElem);
            }
          }
          ;

NetDelayData : LBKT DELAY _NetDelayData RBKT
            {
                $$ = $3;
            }
            ;

_NetDelayData :  MiNoMaValue
        {
            boost::scoped_ptr<ValueData> data($1);
            $$ = new NetDelay();
            $$->setDelay(data->mValues[0].get<Value::MiNoMax>());
        }
       |     MiNoMaDisp
        {
            boost::scoped_ptr<ValueData> data($1);
            $$ = new NetDelay();
            $$->setDelay(data->mValues[0].get<Value::MiNoMax>());
        }
       ;

EventNetGroup : LBKT NETGROUP _EventNetGroup RBKT
              ;

_EventNetGroup :
                |     _EventNetGroup EventNetNameRef
                |     _EventNetGroup EventNetRefData
                ;

EventNetNameRef : NetNameRefData
                {
                    boost::scoped_ptr<NameData> netData($1);
                    std::string name = netData->mName;
                    EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                    ViewSharedPtr view = ctx->getCurrentView();
                    NetSharedPtr net = view->findNet(name);
                    if(!net)
                    {
                        std::string message = constructErrorMessage(
                                "No Net %s found in view %s\n", 
                                            name.c_str(),
                                            view->getName().c_str());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    else
                    {
                        EventSharedPtr event
                                    = ctx->getCurrentEvent();
                        event->addNet(net);
                    }
                }
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
                {
                    delete $2;
                }
        |       _NetMap UserData
                {
                    delete $2;
                }
        ;

NetNameDef :    NameDefData
           |    ArrayData
           ;

NetNameRef :    NameRef
           |    Member
           ;

NetNameRefData :    NameRefData
               |    MemberData
               ;

NetRef : LBKT NETREF NetNameRef _NetRef RBKT
       ;

_NetRef :
        |       NetRef
        |       InstanceRef
        |       ViewRef
        ;

NetRefData : LBKT NETREF NetNameRefData _NetRefData RBKT
            {
                NetRefData *nData = $4;
                if(nData)
                {
                    if(nData->mNetName)
                    {
                        $$ = new NetRefData();
                        $$->mParentNet = nData;
                        $$->mView = nData->mView;
                        nData->mView = NULL;
                    }
                    else
                    {
                        if( nData->mInstanceName
                             || nData->mView )
                        {
                            $$ = nData;
                        }
                    }
                }
                else
                {
                    $$ = new NetRefData();
                }
                $$->mNetName = $3;
            }
            ;

_NetRefData :
            {
                $$ = NULL;
            }
         |      NetRefData
             {
                $$ = $1;
            }
         |      InstanceRefData
             {
                $$ = new NetRefData();
                $$->mInstanceName = $1->mName;
                $$->mView = $1->mView;
                delete $1;
            }
         |      ViewRefData
            {
                $$ = new NetRefData();
                $$->mView = $1;
            }
         ;

NoChange : LBKT NOCHANGE RBKT
         ;

NonPermut : LBKT NONPERMUTABLE 
            {
                PermutableSharedPtr permutable;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                try
                {
                    ctx->getFactory()->create(permutable);
                    permutable->setIsNonPermutable(true);
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create Permutable" );
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                ctx->pushPermutable(permutable);
                ctx->incrementPermutableDepth();
            } _NonPermut RBKT
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                PermutableSharedPtr currentPermutable 
                                    = ctx->getCurrentPermutable();
                ctx->popPermutable();
                PermutableSharedPtr parentPermutable; 
                if(ctx->getPermutableDepth() > 1 )
                {
                    parentPermutable = ctx->getCurrentPermutable();
                }
                ctx->decrementPermutableDepth();
                if(parentPermutable)
                {
                    parentPermutable->setPermutableType( 
                            Permutable::ePermutableParent );
                    currentPermutable->setPermutableType( 
                            Permutable::ePermutableChild );
                    //Check permutable port size
                    size_t parentPermutableSize = parentPermutable->getSize();
                    size_t currentPermutableSize = currentPermutable->getSize();

                    log("Parent permutable Size :: %d\n",
                            parentPermutableSize ); 
                    log("Current permutable Size :: %d\n", 
                            currentPermutableSize );
                    std::vector< PermutableSharedPtr > outPermutables;
                    parentPermutable->getChildren(outPermutables);
                    std::vector< PortSharedPtr > outPorts;
                    parentPermutable->getPorts(outPorts);
            
                    if(!outPermutables.empty() || !outPorts.empty())
                    {
                        if( parentPermutableSize
                                == currentPermutableSize )
                        {
                            parentPermutable->addChildPermutable(
                                    currentPermutable );
                        }
                        else
                        {
                            std::string message = constructErrorMessage(
                                    "Child permutable can't be added, port size mismatch %d with %d", 
                                        parentPermutableSize,
                                        currentPermutableSize);
                            log("%s\n", message.c_str());
                            Error e( eMessageIdParserError,
                                __FUNCTION__, __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
                            YYABORT;
                        }
                    }
                    else
                    {
                        parentPermutable->addChildPermutable(
                                currentPermutable );
                    }
                } 
                else
                {
                    ctx->pushPermutable(currentPermutable);
                }              
            }
          ;

_NonPermut :
           | _NonPermut PortRefData
            {
                boost::scoped_ptr<PortRefData> portData($2);
                NameData *portNameData = portData->mPortName;
                std::string name = portNameData->mName;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                ViewSharedPtr view = ctx->getCurrentView();
                if(view && ( View::eTypeGraphic == view->getType() || 
                    View::eTypeDocument == view->getType() ) )
                {
                    std::string message = constructErrorMessage(
                            "Permutable can not be used in this view type");
                    log("%s\n", message.c_str());
                    Error e( eMessageIdErrorNullPointer,
                        __FUNCTION__, __FILE__, __LINE__ );
                    throw e;
                }    
                PortRefData *currPortRef = $2;
                PortRefData *nextPortRef = portData->mParentPort;
                std::vector< std::string > nestedNames;
                nestedNames.push_back(currPortRef->mPortName->mName);
                while(nextPortRef)
                {
                    currPortRef = nextPortRef;
                    nestedNames.push_back(nextPortRef->mPortName->mName);
                    nextPortRef = currPortRef->mParentPort;
                }
                bool isPort = (NULL == currPortRef->mInstanceName);
                std::string topName = *(nestedNames.rbegin());
                if(isPort)
                {
                    PortSharedPtr port = view->findPort(topName);
                    if(!port)
                    {
                        std::string message = constructErrorMessage(
                                "No port %s found in view %s\n", 
                                            name.c_str(),
                                            view->getName().c_str());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    PermutableSharedPtr currentPermutable 
                                        = ctx->getCurrentPermutable();
                    try
                    {             
                        connectibleFinder(nestedNames, port);  
                        if(!portData->mPortName->mIndices.empty())
                        {
                            port = port->get(portData->mPortName->mIndices);
                        }
                        //Check permutable port size
                        std::vector< PortSharedPtr > outPorts;
                        currentPermutable->getPorts(outPorts);
                        
                        if(!outPorts.empty())
                        {
                            PortSharedPtr firstPort = outPorts.front();
                            std::vector< PortSharedPtr >::iterator it
                                            = std::find(outPorts.begin(), outPorts.end(),
                                                    port);
                            if(it != outPorts.end())
                            {
                                std::string message = constructErrorMessage(
                                            "Port %s already exist in permutable",
                                                port->getName().c_str() );
                                    log("%s\n", message.c_str());
                                    Error e( eMessageIdParserError,
                                        __FUNCTION__, __FILE__, __LINE__ );
                                    e.saveContextData("Parser error message", message);
                                    e.saveContextData("Filename", yyloc.begin.filename);
                                    e.saveContextData("StartLine", yyloc.begin.line);
                                    e.saveContextData("EndLine", yyloc.end.line);
                                    inDriver.setParserError(e);
                                    error(yyloc, message);
                                    YYABORT;
                            }
                            else
                            {
                                if(firstPort->getSize() == port->getSize())
                                {
                                    currentPermutable->addPort(port);
                                }
                                else
                                {
                                    std::string message = constructErrorMessage(
                                            "Port %s can't be added, port size mismatch %d with %d",
                                                name.c_str(),
                                                firstPort->getSize(),
                                                port->getSize());
                                    log("%s\n", message.c_str());
                                    Error e( eMessageIdParserError,
                                        __FUNCTION__, __FILE__, __LINE__ );
                                    e.saveContextData("Parser error message", message);
                                    e.saveContextData("Filename", yyloc.begin.filename);
                                    e.saveContextData("StartLine", yyloc.begin.line);
                                    e.saveContextData("EndLine", yyloc.end.line);
                                    inDriver.setParserError(e);
                                    error(yyloc, message);
                                    YYABORT;
                                }
                            }
                        }
                        else
                        {
                            currentPermutable->addPort(port);
                        }
                    }
                    catch(Error &e)
                    {
                        e.setCurrentLocation(
                            __FUNCTION__, __FILE__, __LINE__ );
                        throw;
                    }
                }
            }
           |    _NonPermut Permutable
           ;

NotAllowed : LBKT NOTALLOWED RuleNameDef _NotAllowed RBKT
           ;

_NotAllowed :   FigGrpObj
            |   _NotAllowed Comment
                {
                    delete $2;
                }
            |   _NotAllowed UserData
                {
                    delete $2;
                }
            ;

NotchSpace : LBKT NOTCHSPACING RuleNameDef FigGrpObj _NotchSpace RBKT
           ;

_NotchSpace :   Range
            |   SingleValSet
            |   _NotchSpace Comment
                {
                    delete $2;
                }
            |   _NotchSpace UserData
                {
                    delete $2;
                }
            ;

Number : LBKT NUMBER _Number RBKT
        {
            if(NULL == $3)
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
             $$ = ($1) ? $1 : new ValueData();
             $$->mValues.push_back( Value( Value::eValueTypeNumber,
                             Value::Number($2->mX, $2->mY) ) );
             delete $2;
         }
        |       _Number NumbDisplay
         {
             $$ = ($1) ? $1 : new ValueData();
             $$->mValues.push_back( Value( Value::eValueTypeNumber,
                             Value::Number($2->mX, $2->mY) ) );
             delete $2;
         }
        |       _Number Number
         {
            $$ = $2;
            if($1)
            {
               $1->mValues.insert($1->mValues.end(),
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
                {
                    delete $2;
                }
            ;

OffPageConn : LBKT OFFPAGECONNECTOR _OffPageConn RBKT
            ;

_OffPageConn :  PortNameDef
             |  _OffPageConn Unused
             |  _OffPageConn Property
             |  _OffPageConn Comment
                {
                    delete $2;
                }
             |  _OffPageConn UserData
                {
                    delete $2;
                }
             ;

OffsetEvent : LBKT OFFSETEVENT Event ScaledInt RBKT
            {
                $$ = $4;
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
                {
                    delete $2;
                }
             |  _OverhngDist UserData
                {
                    delete $2;
                }
             ;

OverlapDist : LBKT  OVERLAPDISTANCE RuleNameDef FigGrpObj FigGrpObj _OverlapDist RBKT
            ;

_OverlapDist :  Range
             |  SingleValSet
             |  _OverlapDist Comment
                {
                    delete $2;
                }
             |  _OverlapDist UserData
                {
                    delete $2;
                }
             ;

Oversize : LBKT OVERSIZE Int _Oversize CornerType RBKT
                {
                    if($3);
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
                {
                    delete $2;
                }
      |         _Page UserData
                {
                    delete $2;
                }
      ;

PageSize : LBKT PAGESIZE_EDIF Rectangle RBKT
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
                boost::scoped_ptr<NameData> nameData($3);
                boost::scoped_ptr<ValueData> valueData($4);
                ParameterSharedPtr param;
                if($3->mIndices.empty())
                {
                    try
                    {
                        SingleParameterSharedPtr single;
                        factory->create(single);
                        param = single;
                        param->setName(nameData->mName);
                        if(!nameData->mOriginalName.empty())
                        {
                            param->setOriginalName(nameData->mOriginalName);
                        } 
                        param->setValue(valueData->mValues[0]);
                    }
                    catch(Error &e)
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
                    if(valueData->mValues.size() != arraySize)
                    {
                        std::string message = constructErrorMessage(
                                "Size mismatch %d with %d", valueData->mValues.size(),
                                 nameData->mIndices.size() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    try
                    {
                        ParameterArraySharedPtr array;
                        factory->create(array);
                        array->setName(nameData->mName);
                        if(!nameData->mOriginalName.empty())
                        {
                            array->setOriginalName(nameData->mOriginalName);
                        } 
                        array->constructChildren( factory,
                                                    $3->mIndices );
                        param = array;
                        std::vector< ParameterSharedPtr > children;
                        param->getChildren(children);
                        std::vector< ParameterSharedPtr >::iterator it = children.begin();
                        std::vector< ParameterSharedPtr >::iterator end = children.end();
                        size_t i = 0;
                        for(; it != end; ++it, i++ )
                        {
                            (*it)->setValue(valueData->mValues[i]);
                        }
                    }
                    catch(Error &e)
                    {
                        e.setCurrentLocation(
                            __FUNCTION__, __FILE__, __LINE__ );
                        throw;
                    }
                }
                param->setUnit(static_cast<Unit>($5) );
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

PathDelayData : LBKT PATHDELAY 
                {
                    PathDelaySharedPtr pathDelay;
                    EdifContextSharedPtr ctx
                                = inDriver.getContext();
                    try
                    {
                        ctx->getFactory()->create(pathDelay);
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create pathDelay" );
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    ctx->setCurrentPathDelay(pathDelay);
                    TimingSharedPtr timing = ctx->getCurrentTiming();
                    timing->addPathDelay(pathDelay);
                } _PathDelay RBKT
                {
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    ctx->setCurrentPathDelay(PathDelaySharedPtr());
                }
                ;

_PathDelay : PathDelay
           | _PathDelay Event
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                PathDelaySharedPtr pathDelay 
                            = ctx->getCurrentPathDelay();
                EventSharedPtr event
                        = ctx->getCurrentEvent();
                event->setType(Event::eTypeEvent);
                pathDelay->addEvent(event);
                ctx->setCurrentEvent(EventSharedPtr());
            }
           ;

PathDelay : LBKT DELAY _PathDelayValue RBKT
            {
                boost::scoped_ptr<ValueData> data($3);
                EdifContextSharedPtr ctx = inDriver.getContext();
                PathDelaySharedPtr pathDelay 
                            = ctx->getCurrentPathDelay();
                pathDelay->setDelay(data->mValues[0].get<Value::MiNoMax>());
            }
          ;

_PathDelayValue : MiNoMaValue
                | MiNoMaDisp
                ;

PathWidth : LBKT PATHWIDTH Int RBKT
            {
                if($3);
            }
          ;

Permutable : LBKT PERMUTABLE 
            {
                PermutableSharedPtr permutable;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                try
                {
                    ctx->getFactory()->create(permutable);
                    permutable->setIsNonPermutable(false);
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create Permutable" );
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                if(ctx->getPermutableDepth() == 0 )
                {
                    ctx->getCurrentView()->addPermutable(permutable);
                }
                ctx->pushPermutable(permutable);
                ctx->incrementPermutableDepth();
            } _Permutable RBKT
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                PermutableSharedPtr currentPermutable 
                                    = ctx->getCurrentPermutable();
                ctx->popPermutable();
                PermutableSharedPtr parentPermutable; 
                if(ctx->getPermutableDepth() > 1 )
                {
                    parentPermutable = ctx->getCurrentPermutable();
                }
                ctx->decrementPermutableDepth();
                if(parentPermutable)
                {
                    parentPermutable->setPermutableType( 
                            Permutable::ePermutableParent );
                    currentPermutable->setPermutableType( 
                            Permutable::ePermutableChild );
                    //Check permutable port size
                    size_t parentPermutableSize = parentPermutable->getSize();
                    size_t currentPermutableSize = currentPermutable->getSize();

                    log("Parent permutable Size :: %d\n",
                            parentPermutableSize ); 
                    log("Current permutable Size :: %d\n", 
                            currentPermutableSize );
                    std::vector< PermutableSharedPtr > outPermutables;
                    parentPermutable->getChildren(outPermutables);
                    std::vector< PortSharedPtr > outPorts;
                    parentPermutable->getPorts(outPorts);
            
                    if(!outPermutables.empty() || !outPorts.empty())
                    {
                        if( parentPermutableSize
                                == currentPermutableSize )
                        {
                            parentPermutable->addChildPermutable(
                                    currentPermutable );
                        }
                        else
                        {
                            std::string message = constructErrorMessage(
                                    "Child permutable can't be added, port size mismatch %d with %d",
                                        parentPermutableSize,
                                        currentPermutableSize);
                            log("%s\n", message.c_str());
                            Error e( eMessageIdParserError,
                                __FUNCTION__, __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
                            YYABORT;
                        }
                    }
                    else
                    {
                        parentPermutable->addChildPermutable(
                                currentPermutable );
                    }
                } 
                else
                {
                    ctx->pushPermutable(currentPermutable);
                }              
            }
           ;

_Permutable :
            |   _Permutable PortRefData
            {
                boost::scoped_ptr<PortRefData> portData($2);
                NameData *portNameData = portData->mPortName;
                std::string name = portNameData->mName;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                ViewSharedPtr view = ctx->getCurrentView();
                if(view && ( View::eTypeGraphic == view->getType() || 
                    View::eTypeDocument == view->getType() ) )
                {
                    std::string message = constructErrorMessage(
                            "Permutable can not be used in this view type");
                    log("%s\n", message.c_str());
                    Error e( eMessageIdErrorNullPointer,
                        __FUNCTION__, __FILE__, __LINE__ );
                    throw e;
                }    
                PortRefData *currPortRef = $2;
                PortRefData *nextPortRef = portData->mParentPort;
                std::vector< std::string > nestedNames;
                nestedNames.push_back(currPortRef->mPortName->mName);
                while(nextPortRef)
                {
                    currPortRef = nextPortRef;
                    nestedNames.push_back(nextPortRef->mPortName->mName);
                    nextPortRef = currPortRef->mParentPort;
                }
                bool isPort = (NULL == currPortRef->mInstanceName);
                std::string topName = *(nestedNames.rbegin());
                if(isPort)
                {
                    PortSharedPtr port = view->findPort(topName);
                    if(!port)
                    {
                        std::string message = constructErrorMessage(
                                "No port %s found in view %s\n", 
                                            name.c_str(),
                                            view->getName().c_str());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    PermutableSharedPtr currentPermutable 
                                        = ctx->getCurrentPermutable();
                    try
                    {             
                        connectibleFinder(nestedNames, port);  
                        if(!portData->mPortName->mIndices.empty())
                        {
                            port = port->get(portData->mPortName->mIndices);
                        }
                        //Check permutable port size
                        std::vector< PortSharedPtr > outPorts;
                        currentPermutable->getPorts(outPorts);
                        
                        if(!outPorts.empty())
                        {
                            PortSharedPtr firstPort = outPorts.front();
                            std::vector< PortSharedPtr >::iterator it
                                            = std::find(outPorts.begin(), outPorts.end(),
                                                    port);
                            if(it != outPorts.end())
                            {
                                std::string message = constructErrorMessage(
                                            "Port %s already exist in permutable",
                                                port->getName().c_str() );
                                    log("%s\n", message.c_str());
                                    Error e( eMessageIdParserError,
                                        __FUNCTION__, __FILE__, __LINE__ );
                                    e.saveContextData("Parser error message", message);
                                    e.saveContextData("Filename", yyloc.begin.filename);
                                    e.saveContextData("StartLine", yyloc.begin.line);
                                    e.saveContextData("EndLine", yyloc.end.line);
                                    inDriver.setParserError(e);
                                    error(yyloc, message);
                                    YYABORT;
                            }
                            else
                            {
                                if(firstPort->getSize() == port->getSize())
                                {
                                    currentPermutable->addPort(port);
                                }
                                else
                                {
                                    std::string message = constructErrorMessage(
                                            "Port %s can't be added, port size mismatch %d with %d",
                                                name.c_str(),
                                                firstPort->getSize(),
                                                port->getSize());
                                    log("%s\n", message.c_str());
                                    Error e( eMessageIdParserError,
                                        __FUNCTION__, __FILE__, __LINE__ );
                                    e.saveContextData("Parser error message", message);
                                    e.saveContextData("Filename", yyloc.begin.filename);
                                    e.saveContextData("StartLine", yyloc.begin.line);
                                    e.saveContextData("EndLine", yyloc.end.line);
                                    inDriver.setParserError(e);
                                    error(yyloc, message);
                                    YYABORT;
                                }
                            }
                        }
                        else
                        {
                            currentPermutable->addPort(port);
                        }
                    }
                    catch(Error &e)
                    {
                        e.setCurrentLocation(
                            __FUNCTION__, __FILE__, __LINE__ );
                        throw;
                    }
                }
            }
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
            if(NULL == $3)
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
            $$ = ($1) ? $1 : new ValueData();
            $$->mValues.push_back( Value(
                        Value::eValueTypePoint,
                            Value::Point($2->mX, $2->mY) ) );
            delete $2;
        }
       |        _Point PointDisp
        {
            $$ = ($1) ? $1 : new ValueData();
            $$->mValues.push_back( Value(
                        Value::eValueTypePoint,
                            Value::Point($2->mX, $2->mY) ) );
            delete $2;
        }
       |        _Point Point
         {
            $$ = $2;
            if($1)
            {
               $1->mValues.insert($1->mValues.end(),
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
                $$ = new PairData($3, $4);
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
            if(bundle)
            {
                typedef std::vector<PortSharedPtr > PBList;
                PBList children;
                bundle->getChildren(children);
                for(PBList::iterator it = children.begin();
                        it != children.end(); it++ )
                {
                    if((*it)->getName() == name)
                    {
                        bExistingPortFound = true;
                    }
                }
            }
            else
            {
                port = view->findPort(name);
                bExistingPortFound = port;
            }
            if(bExistingPortFound)
            {
                if(!ctx->getIsViewBeingLinked())
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
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
#endif
                }
            }
            else
            {
                if(!pNameData->mIndices.empty())
                {
                    try
                    {
                        VectorPortSharedPtr vectorPort;
                        ctx->getFactory()->create(vectorPort);
                        vectorPort->setName(name);
                        vectorPort->constructChildren(
                                ctx->getFactory(),
                                pNameData->mIndices );
                        port = vectorPort;
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create vector port : %s", 
                                        port->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
                else
                {
                    try
                    {
                        ScalarPortSharedPtr scalarPort;
                        ctx->getFactory()->create(scalarPort);
                        port = scalarPort;
                        port->setName(name);
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create port : %s", 
                                        port->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
                if(!pNameData->mOriginalName.empty())
                {
                    port->setOriginalName(
                                    pNameData->mOriginalName );
                }
                port->setParent(view);
                if(bundle)
                {
                    port->setParentCollection(bundle);
                    bundle->addChild(port);
                    log("Added port to existing bundle\n");
                }
                else
                {
                    view->addPort(port);
                }
                if(pNameData->mIndices.empty())
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
            ctx->setCurrentPort(port);
            ctx->pushPropertyContainer(port);
        } _Port RBKT
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            if($5)
            {
                boost::shared_ptr<PortAttributes> attrib($5);
                ctx->getCurrentPort()->setAttributes(attrib);
            }
            ctx->setCurrentPort(PortSharedPtr());
            ctx->popPropertyContainer();
        }
     ;

_Port :
        {
            $$ = NULL;
        }
      | _Port Direction
        {
            $$ = $1;
        }
      | _Port UnusedData
        {
            $$ = ($1)?$1:new PortAttributes();
            Value::Boolean unused = $2;
            Value val(Value::eValueTypeBoolean, unused);
            $$->setUnused(val);
        }
      | _Port PortDelayData
        {
            boost::scoped_ptr<PortDelay> delay($2);
            $$ = ($1)?$1:new PortAttributes();
            $$->setPortDelay(*delay);
        }
      | _Port DesignatorData
        {
            $$ = ($1)?$1:new PortAttributes();
            boost::scoped_ptr<std::string> data($2);
            Value val(Value::eValueTypeString, *data);
            $$->setDesignator(val);
        }
      | _Port DcFanInLoad
        {
            boost::scoped_ptr<PairData> data($2);
            Value::Number num(data->mX, data->mY);
            Value val(Value::eValueTypeNumber, num);
            $$ = ($1)?$1:new PortAttributes();
            $$->setDcFaninLoad(val);
        }
      | _Port DcFanOutLoad
        {
            boost::scoped_ptr<PairData> data($2);
            Value::Number num(data->mX, data->mY);
            Value val(Value::eValueTypeNumber, num);
            $$ = ($1)?$1:new PortAttributes();
            $$->setDcFanoutLoad(val);
        }
      | _Port DcMaxFanIn
        {
            boost::scoped_ptr<PairData> data($2);
            Value::Number num(data->mX, data->mY);
            Value val(Value::eValueTypeNumber, num);
            $$ = ($1)?$1:new PortAttributes();
            $$->setDcMaxFanin(val);
        }
      | _Port DcMaxFanOut
        {
            boost::scoped_ptr<PairData> data($2);
            Value::Number num(data->mX, data->mY);
            Value val(Value::eValueTypeNumber, num);
            $$ = ($1)?$1:new PortAttributes();
            $$->setDcMaxFanout(val);
        }
      | _Port AcLoad
        {
            boost::scoped_ptr<ValueData> data($2);
            $$ = ($1)?$1:new PortAttributes();
            $$->setAcLoad(data->mValues[0]);
        }
      | _Port Property
        {
            $$ = $1;
        }
      | _Port Comment
        {
            $$ = $1;
            EdifContextSharedPtr ctx = inDriver.getContext();
            PortSharedPtr port = ctx->getCurrentPort();
            port->addComment(*$2);
            delete $2;
        }
      | _Port UserData
        {
            $$ = $1;
            EdifContextSharedPtr ctx = inDriver.getContext();
            PortSharedPtr port = ctx->getCurrentPort();
            port->addUserData(*$2);
            delete $2;
        }
      ;

PortBackAn : LBKT PORTBACKANNOTATE PortRefData
            {
                boost::scoped_ptr<PortRefData> portData($3);
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                std::vector< std::string > nestedNames;
                PortRefData *currPortRef = $3;
                PortRefData *nextPortRef = portData->mParentPort;
                nestedNames.push_back(currPortRef->mPortName->mName);
                while(nextPortRef)
                {
                    currPortRef = nextPortRef;
                    nestedNames.push_back(nextPortRef->mPortName->mName);
                    nextPortRef = currPortRef->mParentPort;
                }
                bool isPort = (NULL == currPortRef->mInstanceName);
                NameData *portNameData = currPortRef->mPortName;
                std::string topName = *(nestedNames.rbegin());
                CellSharedPtr cell = ctx->getCurrentCell();
                ViewSharedPtr view = cell->findView(
                                portData->mView->mViewName->mName );
                if(!view)
                {
                    std::string message = constructErrorMessage(
                            "No view %s found",
                        portData->mView->mViewName->mName.c_str());
                    log("%s\n", message.c_str());
                    Error e( eMessageIdParserError,
                        __FUNCTION__, __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                if(isPort)
                {
                    PortSharedPtr port = view->findPort(topName);
                    if(!port)
                    {
                        std::string message = constructErrorMessage(
                                "No port %s found",
                                    portNameData->mName.c_str());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    try
                    {
                        connectibleFinder(nestedNames, port);
                        if(!portData->mPortName->mIndices.empty())
                        {
                            port = port->get(portData->mPortName->mIndices);
                        }
                        ctx->setCurrentPort(port);
                        ctx->pushPropertyContainer(port);
                    }
                    catch(Error &e)
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
                    if(!instance)
                    {
                        std::string message = constructErrorMessage(
                                "Instance %s not known",
                                instanceName->mName.c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    if(!instanceName->mIndices.empty())
                    {
                        InstanceSharedPtr bit
                                = instance->get(instanceName->mIndices);
                        instance = bit;
                    }
                    PortReferenceSharedPtr portRef
                            = instance->findPortReference(topName);
                    if(!portRef)
                    {
                        std::string message = constructErrorMessage(
                                "No port ref %s found",
                                    portNameData->mName.c_str());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    try
                    {
                        connectibleFinder(nestedNames, portRef);
                        if(!portData->mPortName->mIndices.empty())
                        {
                            portRef = portRef->get(portData->mPortName->mIndices);
                        }
                        ctx->setCurrentPortRef(portRef);
                        ctx->pushPropertyContainer(portRef);
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to connect port ref : %s", 
                                        portRef->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
            } _PortBackAn RBKT
            {
                EdifContextSharedPtr ctx
                            = inDriver.getContext();
                if($5)
                {
                    boost::shared_ptr<PortAttributes>
                                                attrib($5);
                    PortSharedPtr port
                                    = ctx->getCurrentPort();
                    if(port)
                    {
                        port->setAttributes(attrib);
                    }
                    else
                    {
                        ctx->getCurrentPortRef()
                                    ->setAttributes(attrib);
                    }
                }
                ctx->setCurrentPort(PortSharedPtr());
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
                    Value val(Value::eValueTypeBoolean, unused);
                    $$->setUnused(val);
                }
      |         _PortBackAn PortDelayData
                {
                    boost::scoped_ptr<PortDelay> delay($2);
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setPortDelay(*delay);
                }
      |         _PortBackAn DesignatorData
                {
                    $$ = ($1)?$1:new PortAttributes();
                    boost::scoped_ptr<std::string> data($2);
                    Value val(Value::eValueTypeString, *data);
                    $$->setDesignator(val);
                }
      |         _PortBackAn DcFanInLoad
                  {
                    boost::scoped_ptr<PairData> data($2);
                    Value::Number num(data->mX, data->mY);
                    Value val(Value::eValueTypeNumber, num);
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setDcFaninLoad(val);
                }
      |         _PortBackAn DcFanOutLoad
                  {
                    boost::scoped_ptr<PairData> data($2);
                    Value::Number num(data->mX, data->mY);
                    Value val(Value::eValueTypeNumber, num);
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setDcFanoutLoad(val);
                }
      |         _PortBackAn DcMaxFanIn
                  {
                    boost::scoped_ptr<PairData> data($2);
                    Value::Number num(data->mX, data->mY);
                    Value val(Value::eValueTypeNumber, num);
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setDcMaxFanin(val);
                }
      |         _PortBackAn DcMaxFanOut
                  {
                    boost::scoped_ptr<PairData> data($2);
                    Value::Number num(data->mX, data->mY);
                    Value val(Value::eValueTypeNumber, num);
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setDcMaxFanout(val);
                }
      |         _PortBackAn AcLoad
                  {
                    boost::scoped_ptr<ValueData> data($2);
                    $$ = ($1)?$1:new PortAttributes();
                    $$->setAcLoad(data->mValues[0]);
                }
       |   _PortBackAn Property
                {
                    $$ = $1;
                }
       |   _PortBackAn Comment
                {
                    $$ = $1;
                    delete $2;
                }
            ;

PortBundle : LBKT PORTBUNDLE NameDefData
        {
            boost::scoped_ptr<NameData> nameData($3);
            std::string name = nameData->mName;
            EdifContextSharedPtr ctx = inDriver.getContext();
            PortSharedPtr port;
            ViewSharedPtr view = ctx->getCurrentView();
            PortBundleSharedPtr bundle
                                = ctx->getCurrentPortBundleContext();
            bool bExistingPortFound = false;
            if(bundle)
            {
                typedef std::vector<PortSharedPtr > PBList;
                PBList children;
                bundle->getChildren(children);
                for(PBList::iterator it = children.begin();
                        it != children.end(); it++ )
                {
                    if((*it)->getName() == name)
                    {
                        bExistingPortFound = true;
                    }
                }
            }
            else
            {
                port = view->findPort(name);
                bExistingPortFound = port;
            }
            if(bExistingPortFound)
            {
                if(!ctx->getIsViewBeingLinked())
                {
                    std::string message = constructErrorMessage(
                        "View %s already contains a port of name %s\n",
                                    view->getName().c_str(),
                                    port->getName().c_str() );
                    log("%s\n", message.c_str());
                    Error e( eMessageIdParserError,
                        __FUNCTION__, __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
            }
            else
            {
                try
                {
                    PortBundleSharedPtr portBundle;
                    ctx->getFactory()->create(portBundle);
                    ctx->pushPortBundleContext(portBundle);
                    port = portBundle;
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create port bundle : %s", 
                                    port->getName().c_str());
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                port->setName(name);
                if(!nameData->mOriginalName.empty())
                {
                    port->setOriginalName(nameData->mOriginalName);
                }
                port->setParent(view);
                if(bundle)
                {
                    port->setParentCollection(bundle);
                    bundle->addChild(port);
                    log("Added port to existing bundle\n");
                }
                else
                {
                    view->addPort(port);
                }
                log( "Created port bundle %s\n",
                                        port->getName().c_str() );
            }
            ctx->setCurrentPort(port);
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
                {
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    PortBundleSharedPtr bundle 
                                = ctx->getCurrentPortBundleContext();
                    bundle->addComment(*$2);
                    delete $2;
                }
            |   _PortBundle UserData
                {
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    PortBundleSharedPtr bundle 
                                = ctx->getCurrentPortBundleContext();
                    bundle->addUserData(*$2);
                    delete $2;
                }
            ;

PortDelayData : LBKT PORTDELAY Derivation _PortDelayData RBKT
            {
                $$ = $4;
                $$->setDerivation(
                        static_cast<Derivation>($3) );
                inDriver.getLexer()->setAppendToBuffer(false);
                inDriver.getLexer()->resetBuffer();
            }
          ;

_PortDelayData :    DelayData
                {
                    inDriver.getLexer()->setAppendToBuffer(true);
                    inDriver.getLexer()->resetBuffer();
                    $$ = $1;
                }
           |    LoadDelayData
                {
                    inDriver.getLexer()->setAppendToBuffer(true);
                    inDriver.getLexer()->resetBuffer();
                    $$ = $1;
                }
           |    _PortDelayData Transition
                {
                    $$ = ($1) ? $1 : new PortDelay();
                    EdifContextSharedPtr ctx
                                = inDriver.getContext();
                    LogicElementSharedPtr logicElem
                                    = ctx->getCurrentLogicElement();
                    if(logicElem)
                    {
                        $$->setTransition(logicElem);
                    }
                }
           |    _PortDelayData Becomes
                {
                    $$ = ($1) ? $1 : new PortDelay();
                    EdifContextSharedPtr ctx
                                = inDriver.getContext();
                    LogicElementSharedPtr logicElem
                                    = ctx->getCurrentLogicElement();
                    if(logicElem)
                    {
                        $$->setTransition(logicElem);
                    }
                }
           ;

EventPortGroup : LBKT PORTGROUP _EventPortGroup RBKT
               ;

_EventPortGroup :
                |    _EventPortGroup EventPortNameRef
                |    _EventPortGroup EventPortRefData
                ;

EventPortNameRef : PortNameRefData
                {
                    boost::scoped_ptr<NameData> portData($1);
                    std::string name = portData->mName;
                    EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                    ViewSharedPtr view = ctx->getCurrentView();
                    PortSharedPtr port = view->findPort(name);
                    if(!port)
                    {
                        std::string message = constructErrorMessage(
                                "No port %s found in view %s\n", 
                                            name.c_str(),
                                            view->getName().c_str());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    else
                    {
                        EventSharedPtr event
                                    = ctx->getCurrentEvent();
                        event->addPort(port);
                    }
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
                {
                    delete $2;
                }
          |     _PortImpl Comment
                {
                    delete $2;
                }
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
                $$->mAttribute = ($$->mAttribute)
                                ? $$->mAttribute
                                : new PortAttributes();
                Value::Boolean unused = $2;
                Value val(Value::eValueTypeBoolean, unused);
                $$->mAttribute->setUnused(val);
            }
      |     _PortInst PortDelayData
            {
                $$ = ($1)?$1:new PortInstData();
                $$->mAttribute = ($$->mAttribute)
                                ? $$->mAttribute
                                : new PortAttributes();
                boost::scoped_ptr<PortDelay> delay($2);
                $$->mAttribute->setPortDelay(*delay);
            }
      |     _PortInst DesignatorData
            {
                $$ = ($1)?$1:new PortInstData();
                $$->mAttribute = ($$->mAttribute)
                                ? $$->mAttribute
                                : new PortAttributes();
                boost::scoped_ptr<std::string> data($2);
                Value val(Value::eValueTypeString, *data);
                $$->mAttribute->setDesignator(val);
            }
      |     _PortInst DcFanInLoad
            {
                boost::scoped_ptr<PairData> data($2);
                Value::Number num(data->mX, data->mY);
                Value val(Value::eValueTypeNumber, num);
                $$ = ($1)?$1:new PortInstData();
                $$->mAttribute = ($$->mAttribute)
                                ? $$->mAttribute
                                : new PortAttributes();
                $$->mAttribute->setDcFaninLoad(val);
            }
      |     _PortInst DcFanOutLoad
            {
                boost::scoped_ptr<PairData> data($2);
                Value::Number num(data->mX, data->mY);
                Value val(Value::eValueTypeNumber, num);
                $$ = ($1)?$1:new PortInstData();
                $$->mAttribute = ($$->mAttribute)
                                ? $$->mAttribute
                                : new PortAttributes();
                $$->mAttribute->setDcFanoutLoad(val);
            }
      |     _PortInst DcMaxFanIn
            {
                boost::scoped_ptr<PairData> data($2);
                Value::Number num(data->mX, data->mY);
                Value val(Value::eValueTypeNumber, num);
                $$ = ($1)?$1:new PortInstData();
                $$->mAttribute = ($$->mAttribute)
                                ? $$->mAttribute
                                : new PortAttributes();
                $$->mAttribute->setDcMaxFanin(val);
            }
      |     _PortInst DcMaxFanOut
            {
                boost::scoped_ptr<PairData> data($2);
                Value::Number num(data->mX, data->mY);
                Value val(Value::eValueTypeNumber, num);
                $$ = ($1)?$1:new PortInstData();
                $$->mAttribute = ($$->mAttribute)
                                ? $$->mAttribute
                                : new PortAttributes();
                $$->mAttribute->setDcMaxFanout(val);
            }
      |     _PortInst AcLoad
            {
                boost::scoped_ptr<ValueData> data($2);
                $$ = ($1)?$1:new PortInstData();
                $$->mAttribute = ($$->mAttribute)
                                ? $$->mAttribute
                                : new PortAttributes();
                $$->mAttribute->setAcLoad(data->mValues[0]);
            }
       |   _PortInst Property
            {
                $$ = $1;
            }
       |   _PortInst Comment
            {
                $$ = $1;
                delete $2;
            }
       |   _PortInst UserData
            {
                $$ = $1;
                delete $2;
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
                $$->mPortRefs.push_back($2);
            }
          |     _PortListData PortNameRefData
              {
                $$ = ($1)?$1:new PortListData();
                PortRefData *portRef = new PortRefData();
                portRef->mPortName = $2;
                $$->mPortRefs.push_back(portRef);
            }
          ;

PortListAls : LBKT PORTLISTALIAS PortNameDefData PortListData RBKT
            {
                if($3)
                {
                    boost::scoped_ptr<NameData> nameData($3);
                    std::string name = nameData->mName;
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    PortListAliasSharedPtr portlistAlias;
                    ctx->getFactory()->create(portlistAlias);
                    portlistAlias->setName(name);

                    if(NULL != $4 && !$4->mPortRefs.empty())
                    {
                        boost::scoped_ptr<PortListData> portListData($4);
                        std::string message;

                        EdifContextSharedPtr ctx = inDriver.getContext();
                        PortListSharedPtr portList;
                        ctx->getFactory()->create(portList);

                        for( std::vector<PortRefData *>::iterator it
                                = portListData->mPortRefs.begin();
                                it != portListData->mPortRefs.end(); ++it )
                        {
                            PortRefData *portData = *it;
                            std::vector< std::string > nestedNames;
                            PortRefData *currPortRef = *it;
                            PortRefData *nextPortRef = portData->mParentPort;
                            nestedNames.push_back(currPortRef->mPortName->mName);
                            while(nextPortRef)
                            {
                                currPortRef = nextPortRef;
                                nestedNames.push_back(nextPortRef->mPortName->mName);
                                nextPortRef = currPortRef->mParentPort;
                            }
                            bool isPort = (NULL == currPortRef->mInstanceName);
                            NameData *portNameData = currPortRef->mPortName;
                            std::string topName = *(nestedNames.rbegin());
                            ViewSharedPtr view = ctx->getCurrentView();
                            if(isPort)
                            {
                                PortSharedPtr port = view->findPort(topName);
                                if(!port)
                                {
                                    std::string message = constructErrorMessage(
                                            "No port %s found in view %s\n", 
                                                portNameData->mName.c_str(),
                                                view->getName().c_str());
                                    log("%s\n", message.c_str());
                                    Error e( eMessageIdParserError,
                                        __FUNCTION__, __FILE__, __LINE__ );
                                    e.saveContextData("Parser error message", message);
                                    e.saveContextData("Filename", yyloc.begin.filename);
                                    e.saveContextData("StartLine", yyloc.begin.line);
                                    e.saveContextData("EndLine", yyloc.end.line);
                                    inDriver.setParserError(e);
                                    error(yyloc, message);
                                    YYABORT;
                                }

                                try
                                {
                                    connectibleFinder(nestedNames, port);
                                    if(!portData->mPortName->mIndices.empty())
                                    {
                                        port = port->get(portData->mPortName->mIndices);
                                    }
                                    portList->addChildPort(port);
                                }
                                catch(Error &e)
                                {
                                    std::string message = constructErrorMessage(
                                            "Unable to connect port : %s", 
                                                    port->getName().c_str());
                                    log("%s\n", message.c_str());
                                    e.setCurrentLocation( __FUNCTION__,
                                                            __FILE__, __LINE__ );
                                    e.saveContextData("Parser error message", message);
                                    e.saveContextData("Filename", yyloc.begin.filename);
                                    e.saveContextData("StartLine", yyloc.begin.line);
                                    e.saveContextData("EndLine", yyloc.end.line);
                                    inDriver.setParserError(e);
                                    error(yyloc, message);
                                    YYABORT;
                                }
                            }
                            else
                            {
                            }
                        }
                        portlistAlias->setPortList(portList);
                    }
                    SimulateSharedPtr simulate 
                            = ctx->getCurrentSimulate();
                    try
                    {
                        simulate->addPortListAlias(portlistAlias);
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to add port list alias : %s", 
                                        portlistAlias->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
            }
            ;

PortMap : LBKT PORTMAP _PortMap RBKT
        ;

_PortMap :
         |      _PortMap PortRef
         |      _PortMap PortGroup
         |      _PortMap Comment
                {
                    delete $2;
                }
         |      _PortMap UserData
                {
                    delete $2;
                }
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
                if(pData)
                {
                    if(pData->mPortName)
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
            $$ = new PairStrData(*$3, *$4);
        }
        ;

_Program :
            {
                $$ = new std::string();
            }
         |      Version
             {
                $$ = $1;
                //delete $1;
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
                if(!nameData->mIndices.empty())
                {
                    std::string message = constructErrorMessage(
                            "Property %s cannot be an array",
                            nameData->mName.c_str() );
                    log("%s\n", message.c_str());
                    Error e( eMessageIdParserError,
                        __FUNCTION__, __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                PropertySharedPtr property;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                try
                {
                    ctx->getFactory()->create(property);
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create Property : %s",
                            nameData->mName.c_str());
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                property->setName(nameData->mName.c_str());
                if(!nameData->mOriginalName.empty())
                {
                    property->setOriginalName(nameData->mOriginalName);
                }
                ctx->pushProperty(property);
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
                if(parentP)
                {
                    parentP->addChildProperty(
                            currentP->getName(), currentP );
                }
                else
                {
                    if(ctx->getIsInInstance())
                    {
                        //Can happen only in case of instances
                        ctx->pushProperty(currentP);
                    }
                    else
                    {
                        if(container)
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
                boost::scoped_ptr<ValueData> value($1);
                if(value->mValues.size() > 1)
                {
                    std::string message = constructErrorMessage(
                            "Property value cannot be an array" );
                    log("%s\n", message.c_str());
                    Error e( eMessageIdParserError,
                        __FUNCTION__, __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                inDriver.getContext()->getCurrentProperty()
                            ->setValue(value->mValues[0]);
            }
          | _Property Owner
            {
                inDriver.getContext()->getCurrentProperty()
                                    ->setOwner(*$2);
                delete $2;
            }
          | _Property Unit
            {
                inDriver.getContext()->getCurrentProperty()
                                ->setUnit(static_cast<Unit>($2));
            }
          | _Property Property
          | _Property Comment
            {
                inDriver.getContext()->getCurrentProperty()
                            ->addComment(*$2);
                delete $2;
            }
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
                {
                    delete $2;
                }
              | _ProtectFrame UserData
                {
                    delete $2;
                }
              ;

Range : LessThan
      | GreaterThan
      | AtMost
      | AtLeast
      | Exactly
      | Between
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
                {
                    delete $2;
                }
          |     _RectSize UserData
                {
                    delete $2;
                }
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
         |  Name
            {
                $$ = $1;
            }
         ;

_Rename :   Str
        {
            $$ = $1;
        }
        |   StrDisplay
        {
            $$ = $1;
        }
        ;

Resolves : LBKT RESOLVES _Resolves RBKT
            {
                $$ = $3;
            }
         ;

_Resolves :
            {
                $$ = NULL;
            }
          |     _Resolves LogicNameRefData
            {
                $$ = ($1)?$1:new LogicListData();
                $$->mNameDataList.push_back($2);
            }
          ;

RuleNameDef :   NameDef
            ;

Scale : LBKT SCALE ScaledInt ScaledInt Unit RBKT
        {
            delete $3;
            delete $4;
            if($5);
        }
      ;

ScaledInt : Int
            {
                $$ = new PairData($1);
            }
          | LBKT E Int Int RBKT
            {
                $$ = new PairData($3, $4);
            }
          ;

ScaleX : LBKT SCALEX Int Int RBKT
        {
            if($3);
            if($4);
        }
       ;

ScaleY : LBKT SCALEY Int Int RBKT
        {
            if($3);
            if($4);
        }
       ;

Section : LBKT SECTION _Section RBKT
        ;

_Section :  Str
            {
                delete $1;
            }
         |  _Section Section
         |  _Section Str
            {
                delete $2;
            }
         |  _Section Instance
         ;

Shape : LBKT SHAPE _Shape RBKT
      ;

_Shape :        Curve
       |        _Shape Property
       ;

SimNameDef :    NameDefData
           ;

Simulate : LBKT SIMULATE 
            {
                SimulateSharedPtr simulate;
                EdifContextSharedPtr ctx
                            = inDriver.getContext();
                try
                {
                    ctx->getFactory()->create(simulate);
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create simulate" );
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                ctx->setCurrentSimulate(simulate);
            } _Simulate RBKT
            {
                // Note : Need to do the followings, from where Simulate called
                //EdifContextSharedPtr ctx = inDriver.getContext();
                //ctx->setCurrentSimulate(SimulateSharedPtr());
            }
            ;

_Simulate : SimNameDef
            {
                if($1)
                {
                    EdifContextSharedPtr ctx
                                = inDriver.getContext();
                    SimulateSharedPtr simulate
                                = ctx->getCurrentSimulate();
                    simulate->setName($1->mName);
                    if(!$1->mOriginalName.empty())
                    {
                        simulate->setOriginalName($1->mOriginalName);
                    }
                    delete $1;
                }
                else
                {
                    //TBD::POSSIBLE?
                }
            }
          | _Simulate PortListAls
          | _Simulate WaveValue
          | _Simulate Apply
          | _Simulate Comment
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                SimulateSharedPtr simulate 
                        = ctx->getCurrentSimulate();
                simulate->addComment(*$2);
                delete $2;
            }
          | _Simulate UserData
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                SimulateSharedPtr simulate 
                        = ctx->getCurrentSimulate();
                simulate->addUserData(*$2);
                delete $2;
            }
          ;

SimulInfo : LBKT SIMULATIONINFO 
            {
                SimulationInfoSharedPtr simuInfo;
                EdifContextSharedPtr ctx
                            = inDriver.getContext();
                try
                {
                    ctx->getFactory()->create(simuInfo);
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create simulation info" );
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                ctx->setCurrentSimulationInfo(simuInfo);
                LibrarySharedPtr lib = ctx->getCurrentLibrary();
                lib->setSimulationInfo(simuInfo);
            } _SimulInfo RBKT
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                ctx->setCurrentSimulationInfo(
                        SimulationInfoSharedPtr() );
            }
          ;

_SimulInfo :
           |    _SimulInfo LogicValue
           |    _SimulInfo Comment
                {
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    SimulationInfoSharedPtr simuInfo 
                                    = ctx->getCurrentSimulationInfo();
                    simuInfo->addComment(*$2);
                    delete $2;
                }
           |    _SimulInfo UserData
                {
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    SimulationInfoSharedPtr simuInfo 
                                    = ctx->getCurrentSimulationInfo();
                    simuInfo->addUserData(*$2);
                    delete $2;
                }
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

Status : LBKT STATUS 
        {
            StatusSharedPtr status;
            EdifContextSharedPtr ctx
                                = inDriver.getContext();
            try
            {
                ctx->getFactory()->create(status);
            }
            catch(Error &e)
            {
                std::string message = constructErrorMessage(
                        "Unable to create Status" );
                log("%s\n", message.c_str());
                e.setCurrentLocation( __FUNCTION__,
                                        __FILE__, __LINE__ );
                e.saveContextData("Parser error message", message);
                e.saveContextData("Filename", yyloc.begin.filename);
                e.saveContextData("StartLine", yyloc.begin.line);
                e.saveContextData("EndLine", yyloc.end.line);
                inDriver.setParserError(e);
                error(yyloc, message);
                YYABORT;
            }
            ctx->setCurrentStatus(status);
        } _Status RBKT
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            StatusSharedPtr currentStatus 
                                = ctx->getCurrentStatus(); 
                                    
            StatusContainerSharedPtr container
                                    = ctx->getCurrentStatusContainer();
            if(container) 
            {
                container->addStatus(currentStatus);
            }
            else
            {
                log("Status is being discarded\n");
            }
        }
       ;

_Status :
        |   _Status Written
        |   _Status Comment
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                StatusSharedPtr status = ctx->getCurrentStatus();
                status->addComment(*$2);
                delete $2;
            }
        |   _Status UserData
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                StatusSharedPtr status = ctx->getCurrentStatus();
                status->addUserData(*$2);
                delete $2;
            }
        ;

Steady : LBKT STEADY __Steady _Steady RBKT
       ;

__Steady :      PortNameRef
         |      PortRef
         |      PortList
         ;

_Steady :       Duration
                {
                    delete $1;
                }
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
            if($2);
            if(NULL == $3)
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
                $$ = ($1) ? $1 : new ValueData();
                $$->mValues.push_back( Value(
                            Value::eValueTypeString, *$2 ) );
                delete $2;
            }
        |       _String StrDisplay
            {
                $$ = ($1) ? $1 : new ValueData();
                $$->mValues.push_back( Value(
                            Value::eValueTypeString, *$2 ) );
                delete $2;
            }
        |       _String String
         {
            $$ = $2;
            if($1)
            {
               $1->mValues.insert($1->mValues.end(),
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

Strong : LBKT STRONG LogicNameRefData RBKT
        {
            $$ = $3;
        }
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
                {
                    delete $2;
                }
        |       _Symbol UserData
                {
                    delete $2;
                }
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
                {
                    delete $1;
                }
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
                {
                    delete $2;
                }
            |   _Technology UserData
                {
                    delete $2;
                }
            ;

TextHeight : LBKT TEXTHEIGHT Int RBKT
            {
                if($3);
            }
           ;

TimeIntval : LBKT TIMEINTERVAL StartTimeIntval EndTimeIntval RBKT
           ;

StartTimeIntval :  Event
                {
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    ForbiddenEventSharedPtr forbiddenEvent 
                                = ctx->getCurrentForbiddenEvent();
                    EventSharedPtr event
                            = ctx->getCurrentEvent();
                    event->setType(Event::eTypeEvent);
                    forbiddenEvent->setStartTimeInterval(event);
                    ctx->setCurrentEvent(EventSharedPtr());
                }
                |  OffsetEvent
                {
                    boost::scoped_ptr<PairData> data($1);
                    Value::Number num(data->mX, data->mY);
                    Value val(Value::eValueTypeNumber, num);

                    EdifContextSharedPtr ctx = inDriver.getContext();
                    ForbiddenEventSharedPtr forbiddenEvent 
                                = ctx->getCurrentForbiddenEvent();
                    EventSharedPtr event
                            = ctx->getCurrentEvent();
                    event->setType(Event::eTypeOffsetEvent);
                    event->setOffsetTime(val);
                    forbiddenEvent->setStartTimeInterval(event);
                    ctx->setCurrentEvent(EventSharedPtr());
                }
                ;

EndTimeIntval :  Event
                {
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    ForbiddenEventSharedPtr forbiddenEvent 
                                = ctx->getCurrentForbiddenEvent();
                    EventSharedPtr event
                            = ctx->getCurrentEvent();
                    event->setType(Event::eTypeEvent);
                    forbiddenEvent->setEndTimeInterval(event);
                    ctx->setCurrentEvent(EventSharedPtr());
                }
              |  OffsetEvent
                {
                    boost::scoped_ptr<PairData> data($1);
                    Value::Number num(data->mX, data->mY);
                    Value val(Value::eValueTypeNumber, num);
                    
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    ForbiddenEventSharedPtr forbiddenEvent 
                                = ctx->getCurrentForbiddenEvent();
                    EventSharedPtr event
                            = ctx->getCurrentEvent();
                    event->setType(Event::eTypeOffsetEvent);
                    event->setOffsetTime(val);
                    forbiddenEvent->setEndTimeInterval(event);
                    ctx->setCurrentEvent(EventSharedPtr());
                }
              |  Duration
                {
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    ForbiddenEventSharedPtr forbiddenEvent 
                                = ctx->getCurrentForbiddenEvent();

                    boost::scoped_ptr<PairData> data($1);
                    Value::Number num(data->mX, data->mY);
                    Value val(Value::eValueTypeNumber, num);
                    forbiddenEvent->setDuration(val);
                }
              ;

TimeStamp : LBKT TIMESTAMP Int Int Int Int Int Int RBKT
        {
            if($3);
            if($4);
            if($5);
            if($6);
            if($7);
            if($8);
            $$ = new TimeStamp(); 
            $$->setYear($3);
            $$->setMonth($4);
            $$->setDay($5);
            $$->setHour($6);
            $$->setMinute($7);
            $$->setSecond($8); 
        }
          ;

Timing : LBKT TIMING
        {
            TimingSharedPtr timing;
            EdifContextSharedPtr ctx
                        = inDriver.getContext();
            try
            {
                ctx->getFactory()->create(timing);
            }
            catch(Error &e)
            {
                std::string message = constructErrorMessage(
                        "Unable to create timing" );
                log("%s\n", message.c_str());
                e.setCurrentLocation( __FUNCTION__,
                                        __FILE__, __LINE__ );
                e.saveContextData("Parser error message", message);
                e.saveContextData("Filename", yyloc.begin.filename);
                e.saveContextData("StartLine", yyloc.begin.line);
                e.saveContextData("EndLine", yyloc.end.line);
                inDriver.setParserError(e);
                error(yyloc, message);
                YYABORT;
            }
            ctx->setCurrentTiming(timing);
        } _Timing RBKT
        {
            // Note : Need to do the followings, from where Timing called
            //EdifContextSharedPtr ctx = inDriver.getContext();
            //ctx->setCurrentTiming(TimingSharedPtr());
        }
       ;

_Timing :   Derivation
            {
                EdifContextSharedPtr ctx
                        = inDriver.getContext();
                TimingSharedPtr timing
                        = ctx->getCurrentTiming();
                timing->setDerivation( 
                          static_cast<Derivation>($1) );    
            }
        |   _Timing PathDelayData
        |   _Timing Forbidden
        |   _Timing Comment
            {
                EdifContextSharedPtr ctx
                        = inDriver.getContext();
                TimingSharedPtr timing
                        = ctx->getCurrentTiming();
                timing->addComment(*$2);
                delete $2;
            }
        |   _Timing UserData
            {
                EdifContextSharedPtr ctx
                        = inDriver.getContext();
                TimingSharedPtr timing
                        = ctx->getCurrentTiming();
                timing->addUserData(*$2);
                delete $2;
            }
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

Transition : LBKT TRANSITION 
            {
                EdifContextSharedPtr ctx
                                = inDriver.getContext();
                LogicElementSharedPtr logicElem;
                try
                {
                    ctx->getFactory()->create(logicElem);
                    logicElem->setType(LogicElement::eTypeTransition);
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create transition" );
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                ctx->pushLogicElement(logicElem);
                ctx->incrementLogicElementDepth();
            } _PreviousValue _PresentValue RBKT
           ;

_PreviousValue :   LogicNameData
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

UserData : LBKT USERDATA 
            {
                inDriver.getLexer()->setAppendToUserDataBuffer(true);
                inDriver.getLexer()->resetUserDataBuffer();
            } _UserData RBKT
            {
                std::string val = inDriver.getLexer()->getUserDataBuffer();
                $$ = new std::string(val);
                inDriver.getLexer()->setAppendToUserDataBuffer(false);
                inDriver.getLexer()->resetUserDataBuffer();
            }
         ;

_UserData : Ident
            {
                delete $1;
            }
          | _UserData Int
            {
                if($2);
            }
          | _UserData Str
            {
                delete $2;
            }
          | _UserData Ident
            {
                delete $2;
            }
          | _UserData Form
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
            view = cell->findView(name);
            bool isCellExtern = cell->getIsExtern();
            if(view)
            {
                log("Found existing view\n");
                if(isCellExtern)
                {
                    if(!view->getIsExtern())
                    {
                        std::string message = constructErrorMessage(
                            "View %s already exists in cell %s\n",
                                name.c_str(), view->getName().c_str() );
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
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
                    if(view->getIsExtern())
                    {
                        log("Found extern view of this name\n");
                        view->setIsExtern(false);
                        ctx->setIsViewBeingLinked(true);
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
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
#endif
                    }
                }
            }
            else
            {
                try
                {
                    ctx->getFactory()->create(view);
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create View : %s", 
                                    view->getName().c_str());
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                view->setName(name);
                if(!nameData->mOriginalName.empty())
                {
                    view->setOriginalName(nameData->mOriginalName);
                }
                view->setIsExtern(isCellExtern);
                view->setParent(cell);
                cell->addView(view);
                LinkerSharedPtr linker = ctx->getLinker();
                Linker::NameSpec viewNameSpec(
                                cell->getParent()->getName(),
                                cell->getName(), view->getName() );
                if(isCellExtern)
                {
                    linker->setExternView(viewNameSpec, view);
                    log("Extern view %s saved in linker.\n",
                                        view->getName().c_str() );
                }
                else
                {
                    linker->linkUnresolved(viewNameSpec, view);
                }
                log("Created view %s\n", view->getName().c_str());
            }
            ctx->setCurrentView(view);
            log("View %s added\n",
                    view->getName().c_str());
            ctx->pushPropertyContainer(view);
            ctx->pushStatusContainer(view);
        } ViewType _View RBKT
        {
            ViewSharedPtr view = inDriver.getContext()->getCurrentView();
            if(View::eTypeNetlist != view->getType())
            {
                view->setNonNetlistViewData(inDriver.getLexer()->getBuffer());
            }
            inDriver.getContext()->setCurrentView(
                            ViewSharedPtr() );
            inDriver.getContext()->setIsViewBeingLinked(false);
            inDriver.getContext()->popPropertyContainer();
            inDriver.getContext()->popStatusContainer();

            inDriver.getLexer()->setAppendToBuffer(false);
            log("%s\n", inDriver.getLexer()->getBuffer().c_str());

            inDriver.getLexer()->resetBuffer();
        }
     ;

_View : Interface
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            ViewSharedPtr view = ctx->getCurrentView();
            if($1)
            {
                InterfaceAttributesSharedPtr attrib($1);
                view->setInterfaceAttributes(attrib);
            }
        }
      | _View Status
      | _View Contents
      | _View Comment
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            ViewSharedPtr view = ctx->getCurrentView();
            view->addComment(*$2);
            delete $2;
        }
      | _View Property
      | _View UserData
        {
            EdifContextSharedPtr ctx = inDriver.getContext();
            ViewSharedPtr view = ctx->getCurrentView();
            if(View::eTypeNetlist == view->getType())
            {
                view->addUserData(*$2);
                delete $2;
            }
        }
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
                {
                    delete $2;
                }
         |      _ViewMap UserData
                {
                    delete $2;
                }
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
            if(NULL == $4)
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
                View::Type viewType = static_cast<View::Type>($3);
                if(View::eTypeNetlist != viewType)
                {
                    //TBD::WARN
                    std::string message = constructErrorMessage(
                            "Warning::View types other than netlist are ignored");
                    log("%s\n", message.c_str());
                    // Error e( eMessageIdParserWarning,
                    //     __FUNCTION__, __FILE__, __LINE__ );
                    // e.saveContextData("Parser warning message", message);
                    // e.saveContextData("Filename", yyloc.begin.filename);
                    // e.saveContextData("StartLine", yyloc.begin.line);
                    // e.saveContextData("EndLine", yyloc.end.line);
                    // inDriver.setParserError(e);
                    // error(yyloc, message);
                    // YYABORT;
                    inDriver.getLexer()->setAppendToBuffer(true);
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
            if($3);
        }
        ;

VoltageMap : LBKT VOLTAGEMAP MiNoMaValue RBKT
            {
                $$ = $3;
            }
            ;

WaveValue : LBKT WAVEVALUE LogicNameDef ScaledInt 
            {
                if($3)
                {
                    boost::scoped_ptr<NameData> nameData($3);
                    std::string name = nameData->mName;
                    EdifContextSharedPtr ctx = inDriver.getContext();
                    WaveValueSharedPtr waveValue;
                    try
                    {
                        ctx->getFactory()->create(waveValue);
                        waveValue->setName(name);
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to create WaveValue" );
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    boost::scoped_ptr<PairData> data($4);
                    Value::Number num(data->mX, data->mY);
                    Value val(Value::eValueTypeNumber, num);
                    waveValue->setDeltaTimeDuration(val);

                    SimulateSharedPtr simulate 
                            = ctx->getCurrentSimulate();
                    try
                    {
                        simulate->addWaveValue(waveValue);
                    }
                    catch(Error &e)
                    {
                        std::string message = constructErrorMessage(
                                "Unable to add wave value : %s", 
                                        waveValue->getName().c_str());
                        log("%s\n", message.c_str());
                        e.setCurrentLocation( __FUNCTION__,
                                                __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                    ctx->setCurrentWaveValue(waveValue);
                }
            } LogicWave RBKT
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                ctx->setCurrentWaveValue(WaveValueSharedPtr()); 
            }
          ;

Weak : LBKT WEAK LogicNameRefData RBKT
        {
            $$ = $3;
        }
     ;

WeakJoined : LBKT WEAKJOINED 
            {
                InterfaceJoinedInfoSharedPtr joinedInfo;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                try
                {
                    ctx->getFactory()->create(joinedInfo);
                    joinedInfo->setJoinedType( 
                            InterfaceJoinedInfo::eJoinedTypeWeak );
                }
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create weak joined info" );
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                if(ctx->getInterfaceJoinedInfoDepth() == 0 )
                {
                    ctx->getCurrentView()->addInterfaceJoinedInfo(joinedInfo);
                }
                ctx->pushInterfaceJoinedInfo(joinedInfo);
                ctx->incrementInterfaceJoinedInfoDepth();
            } _WeakJoined RBKT
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                InterfaceJoinedInfoSharedPtr currentJoinedInfo 
                        = ctx->getCurrentInterfaceJoinedInfo();
                ctx->popInterfaceJoinedInfo();
                InterfaceJoinedInfoSharedPtr parentJoinedInfo; 
                if(ctx->getInterfaceJoinedInfoDepth() > 1 )
                {
                    parentJoinedInfo = ctx->getCurrentInterfaceJoinedInfo();
                }
                ctx->decrementInterfaceJoinedInfoDepth();
                if(parentJoinedInfo)
                {                    
                    parentJoinedInfo->setRelationType( 
                            InterfaceJoinedInfo::eRelationTypeParent );
                    currentJoinedInfo->setRelationType( 
                            InterfaceJoinedInfo::eRelationTypeChild );
                    //Check size
                    size_t parentJoinedInfoSize = parentJoinedInfo->getSize();
                    size_t currentJoinedInfoSize = currentJoinedInfo->getSize();

                    log("Parent joined info Size :: %d\n",
                            parentJoinedInfoSize ); 
                    log("Current joined info Size :: %d\n", 
                            currentJoinedInfoSize );

                    //std::cout << "Parent joined info Size :: " << parentJoinedInfoSize << std::endl;
                    //std::cout << "Current joined info Size :: " << currentJoinedInfoSize << std::endl;

                    std::vector< InterfaceJoinedInfoSharedPtr > outJoinedInfos;
                    parentJoinedInfo->getChildren(outJoinedInfos);

                    std::list< PortSharedPtr > outPorts;
                    parentJoinedInfo->getPorts(outPorts);
                    
                    std::list< PortListSharedPtr > outPortLists;
                    parentJoinedInfo->getPortLists(outPortLists);
                    
                    if(parentJoinedInfoSize != 0) 
                    {
                        if( parentJoinedInfoSize
                                == currentJoinedInfoSize )
                        {
                            parentJoinedInfo->addChildJoinedInfo(
                                    currentJoinedInfo );
                        }
                        else
                        {
                            std::string message = constructErrorMessage(
                                    "Child joined info can't be added, port size mismatch %d with %d",
                                        parentJoinedInfoSize,
                                        currentJoinedInfoSize);
                            log("%s\n", message.c_str());
                            Error e( eMessageIdParserError,
                                __FUNCTION__, __FILE__, __LINE__ );
                            e.saveContextData("Parser error message", message);
                            e.saveContextData("Filename", yyloc.begin.filename);
                            e.saveContextData("StartLine", yyloc.begin.line);
                            e.saveContextData("EndLine", yyloc.end.line);
                            inDriver.setParserError(e);
                            error(yyloc, message);
                            YYABORT;
                        }
                    }
                    else
                    {
                        parentJoinedInfo->addChildJoinedInfo(
                                currentJoinedInfo );
                    }
                } 
                else
                {
                    ctx->pushInterfaceJoinedInfo(currentJoinedInfo);
                }              
            }
           ;

_WeakJoined :
            |   _WeakJoined PortRefData
            {
                boost::scoped_ptr<PortRefData> portData($2);
                NameData *portNameData = portData->mPortName;
                std::string name = portNameData->mName;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                ViewSharedPtr view = ctx->getCurrentView();

                PortRefData *currPortRef = $2;
                PortRefData *nextPortRef = portData->mParentPort;
                std::vector< std::string > nestedNames;
                nestedNames.push_back(currPortRef->mPortName->mName);
                while(nextPortRef)
                {
                    currPortRef = nextPortRef;
                    nestedNames.push_back(nextPortRef->mPortName->mName);
                    nextPortRef = currPortRef->mParentPort;
                }
                bool isPort = (NULL == currPortRef->mInstanceName);
                std::string topName = *(nestedNames.rbegin());
                if(isPort)
                {
                    PortSharedPtr port = view->findPort(topName);
                    if(!port)
                    {
                        std::string message = constructErrorMessage(
                                "No port %s found in view %s\n", 
                                            name.c_str(),
                                            view->getName().c_str());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }

                    InterfaceJoinedInfoSharedPtr currentJoinedInfo 
                                        = ctx->getCurrentInterfaceJoinedInfo();
                    try
                    {             
                        connectibleFinder(nestedNames, port);  
                        if(!portData->mPortName->mIndices.empty())
                        {
                            port = port->get(portData->mPortName->mIndices);
                        }
                        //Check port size
                        std::list< PortSharedPtr > outPorts;
                        currentJoinedInfo->getPorts(outPorts);
                        
                        if(!outPorts.empty())
                        {
                            PortSharedPtr firstPort = outPorts.front();
                            std::list< PortSharedPtr >::iterator it
                                            = std::find(outPorts.begin(), outPorts.end(),
                                                    port);
                            if(it != outPorts.end())
                            {
                                std::string message = constructErrorMessage(
                                            "Port %s already exist in weakJoined info",
                                                port->getName().c_str() );
                                    log("%s\n", message.c_str());
                                    Error e( eMessageIdParserError,
                                        __FUNCTION__, __FILE__, __LINE__ );
                                    e.saveContextData("Parser error message", message);
                                    e.saveContextData("Filename", yyloc.begin.filename);
                                    e.saveContextData("StartLine", yyloc.begin.line);
                                    e.saveContextData("EndLine", yyloc.end.line);
                                    inDriver.setParserError(e);
                                    error(yyloc, message);
                                    YYABORT;
                            }
                            else
                            {
                                if(firstPort->getSize() == port->getSize())
                                {
                                    currentJoinedInfo->addPort(port);
                                }
                                else
                                {
                                    std::string message = constructErrorMessage(
                                            "Port %s can't be added, port size mismatch %d with %d",
                                                name.c_str(),
                                                firstPort->getSize(),
                                                port->getSize());
                                    log("%s\n", message.c_str());
                                    Error e( eMessageIdParserError,
                                        __FUNCTION__, __FILE__, __LINE__ );
                                    e.saveContextData("Parser error message", message);
                                    e.saveContextData("Filename", yyloc.begin.filename);
                                    e.saveContextData("StartLine", yyloc.begin.line);
                                    e.saveContextData("EndLine", yyloc.end.line);
                                    inDriver.setParserError(e);
                                    error(yyloc, message);
                                    YYABORT;
                                }
                            }
                        }
                        else
                        {
                            currentJoinedInfo->addPort(port);
                        }
                    }
                    catch(Error &e)
                    {
                        e.setCurrentLocation(
                            __FUNCTION__, __FILE__, __LINE__ );
                        throw;
                    }
                }
                else
                {
                }
            }
            |   _WeakJoined PortListData
            {
                if(NULL != $2 && !$2->mPortRefs.empty())
                {
                    boost::scoped_ptr<PortListData> portListData($2);
                    std::string message;

                    EdifContextSharedPtr ctx = inDriver.getContext();
                    PortListSharedPtr portList;
                    ctx->getFactory()->create(portList);

                    InterfaceJoinedInfoSharedPtr currentJoinedInfo 
                                = ctx->getCurrentInterfaceJoinedInfo();

                    for( std::vector<PortRefData *>::iterator it
                            = portListData->mPortRefs.begin();
                            it != portListData->mPortRefs.end(); ++it )
                    {
                        PortRefData *portData = *it;
                        std::vector< std::string > nestedNames;
                        PortRefData *currPortRef = *it;
                        PortRefData *nextPortRef = portData->mParentPort;
                        nestedNames.push_back(currPortRef->mPortName->mName);
                        while(nextPortRef)
                        {
                            currPortRef = nextPortRef;
                            nestedNames.push_back(nextPortRef->mPortName->mName);
                            nextPortRef = currPortRef->mParentPort;
                        }
                        bool isPort = (NULL == currPortRef->mInstanceName);
                        NameData *portNameData = currPortRef->mPortName;
                        std::string topName = *(nestedNames.rbegin());
                        ViewSharedPtr view = ctx->getCurrentView();
                        if(isPort)
                        {
                            PortSharedPtr port = view->findPort(topName);
                            if(!port)
                            {
                                std::string message = constructErrorMessage(
                                        "No port %s found in view %s\n", 
                                            portNameData->mName.c_str(),
                                            view->getName().c_str());
                                log("%s\n", message.c_str());
                                Error e( eMessageIdParserError,
                                    __FUNCTION__, __FILE__, __LINE__ );
                                e.saveContextData("Parser error message", message);
                                e.saveContextData("Filename", yyloc.begin.filename);
                                e.saveContextData("StartLine", yyloc.begin.line);
                                e.saveContextData("EndLine", yyloc.end.line);
                                inDriver.setParserError(e);
                                error(yyloc, message);
                                YYABORT;
                            }

                            try
                            {
                                connectibleFinder(nestedNames, port);
                                if(!portData->mPortName->mIndices.empty())
                                {
                                    port = port->get(portData->mPortName->mIndices);
                                }
                                portList->addChildPort(port);
                            }
                            catch(Error &e)
                            {
                                std::string message = constructErrorMessage(
                                        "Unable to connect port : %s", 
                                                port->getName().c_str());
                                log("%s\n", message.c_str());
                                e.setCurrentLocation( __FUNCTION__,
                                                        __FILE__, __LINE__ );
                                e.saveContextData("Parser error message", message);
                                e.saveContextData("Filename", yyloc.begin.filename);
                                e.saveContextData("StartLine", yyloc.begin.line);
                                e.saveContextData("EndLine", yyloc.end.line);
                                inDriver.setParserError(e);
                                error(yyloc, message);
                                YYABORT;
                            }
                        }
                        else
                        {
                        }
                    }
                    if(currentJoinedInfo->getSize() == portList->getSize())
                    {
                        currentJoinedInfo->addPortList(portList);
                    }
                    else
                    {
                        std::string message = constructErrorMessage(
                                "Port size mismatch %d with %d",
                                    currentJoinedInfo->getSize(),
                                    portList->getSize());
                        log("%s\n", message.c_str());
                        Error e( eMessageIdParserError,
                            __FUNCTION__, __FILE__, __LINE__ );
                        e.saveContextData("Parser error message", message);
                        e.saveContextData("Filename", yyloc.begin.filename);
                        e.saveContextData("StartLine", yyloc.begin.line);
                        e.saveContextData("EndLine", yyloc.end.line);
                        inDriver.setParserError(e);
                        error(yyloc, message);
                        YYABORT;
                    }
                }
            }
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
                {
                    delete $2;
                }
      |         _When UserData
                {
                    delete $2;
                }
      ;

Written :  LBKT WRITTEN 
            {
                WrittenSharedPtr written;
                EdifContextSharedPtr ctx
                                    = inDriver.getContext();
                try
                {
                    ctx->getFactory()->create(written);
                }            
                catch(Error &e)
                {
                    std::string message = constructErrorMessage(
                            "Unable to create Written" );
                    log("%s\n", message.c_str());
                    e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                    e.saveContextData("Parser error message", message);
                    e.saveContextData("Filename", yyloc.begin.filename);
                    e.saveContextData("StartLine", yyloc.begin.line);
                    e.saveContextData("EndLine", yyloc.end.line);
                    inDriver.setParserError(e);
                    error(yyloc, message);
                    YYABORT;
                }
                ctx->setCurrentWritten(written);
                ctx->pushPropertyContainer(written);
            } _Written RBKT
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                StatusSharedPtr status = ctx->getCurrentStatus();
                WrittenSharedPtr written = ctx->getCurrentWritten();
                status->addWritten(written);
                ctx->setCurrentWritten(WrittenSharedPtr());
                ctx->popPropertyContainer();
            }
        ;

_Written :  TimeStamp
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                WrittenSharedPtr written = ctx->getCurrentWritten();
                written->setTimeStamp(*$1);
                delete $1;
            }
         |  _Written Author
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                WrittenSharedPtr written = ctx->getCurrentWritten();
                written->setAuthorName(*$2);
                delete $2;
            }
         |  _Written Program
            {
                boost::scoped_ptr<PairStrData> data($2);
                EdifContextSharedPtr ctx = inDriver.getContext();
                WrittenSharedPtr written = ctx->getCurrentWritten();
                written->setProgramName(data->mFirst);
                written->setProgramVersion(data->mSecond);
            }
         |  _Written DataOrigin
            {
                boost::scoped_ptr<PairStrData> data($2);
                EdifContextSharedPtr ctx = inDriver.getContext();
                WrittenSharedPtr written = ctx->getCurrentWritten();
                written->setDataOriginLocationName(data->mFirst);
                written->setDataOriginVersion(data->mSecond);
            }
         |  _Written Property
         |  _Written Comment
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                WrittenSharedPtr written = ctx->getCurrentWritten();
                written->addComment(*$2);
                delete $2;
            }
         |  _Written UserData
            {
                EdifContextSharedPtr ctx = inDriver.getContext();
                WrittenSharedPtr written = ctx->getCurrentWritten();
                written->addUserData(*$2);
                delete $2;
            }
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
            inDriver.getLexer()->setIsIdContext(true);
            delete $1;
        }
        ;

 /*** END grammar rules ***/

%% /*** Additional Code ***/

void
torc::generic::Parser::error(const Parser::location_type& l,
                const std::string& m)
{
    if(!inDriver.getIsParserErrorSet())
    {
        Error e( eMessageIdParserError,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Parser error message", m);
        e.saveContextData("Filename", l.begin.filename);
        e.saveContextData("StartLine", l.begin.line);
        e.saveContextData("EndLine", l.end.line);
        inDriver.setParserError(e);
        log("Synthetic parser error created\n");
    }
    inDriver.error(l, m);
}

