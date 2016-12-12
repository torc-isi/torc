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

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif 

#include "torc/generic/edif/Decompiler.hpp"
#include "torc/generic/PortAttributes.hpp"
#include "torc/generic/NetAttributes.hpp"
#include "torc/generic/TimeStamp.hpp"
#include "torc/generic/Written.hpp"
#include "torc/generic/InterfaceAttributes.hpp"
#include "torc/generic/LogicValueAttributes.hpp"
#include "torc/generic/PathDelay.hpp"

namespace {

using namespace torc::generic;
typedef std::vector<LibrarySharedPtr> Libraries;
typedef std::vector<CellSharedPtr> Cells;
typedef std::vector<ViewSharedPtr> Views;
typedef std::vector<InstanceSharedPtr> Instances;

/*std::string trimLeading(const std::string& inBuffer) {
	const size_t beginStr = inBuffer.find_first_of("(");
	if(beginStr != std::string::npos) {
		const size_t endStr = inBuffer.find_last_of(")");
		const size_t range = endStr - beginStr + 1;
		return inBuffer.substr(beginStr, range);
	}
	return std::string();
}*/

// Code for Trim Leading Spaces only
std::string trimLeadingSpaces(const std::string& inBuffer) {
	// Find the first character position after excluding leading blank spaces
	const size_t startpos = inBuffer.find_first_not_of(" \t");
	if(std::string::npos != startpos) {
		return inBuffer.substr(startpos, std::string::npos);
	}
	return std::string();
}

struct CircularDependencyResolver {

	enum PrintType {
		ePrintTypeExtern, ePrintTypeConcrete
	};

	void resolve(const RootSharedPtr& inRoot) {
		SymTab<std::string, PrintType, true> discoveredLibs;
		Libraries libs;
		inRoot->getLibraries(libs);
		for(Libraries::iterator lib = libs.begin(); lib != libs.end(); ++lib) {
			if((*lib)->getIsExtern()) {
				PrintType type;
				if(!discoveredLibs.get((*lib)->getName(), type)) {
					//Extern but not found
					mExterns.push_back(*lib);
					discoveredLibs.set((*lib)->getName(), ePrintTypeExtern);
				}
				continue;
				//Views here will most likely have
				//no bodies anyway
			}
			discoveredLibs.set((*lib)->getName(), ePrintTypeConcrete);
			mConcreteLibs.push_back(*lib);
			Cells cells;
			(*lib)->getCells(cells);
			for(Cells::iterator cell = cells.begin(); cell != cells.end(); ++cell) {
				Views views;
				(*cell)->getViews(views);
				for(Views::iterator view = views.begin(); view != views.end(); ++view) {
					Instances instances;
					(*view)->getInstances(instances);
					for(Instances::iterator inst = instances.begin(); inst != instances.end();
						++inst) {
						switch((*inst)->getCompositionType()) {
						case eCompositionTypeScalar: {
							checkLib(*inst, discoveredLibs);
							break;
						}
						case eCompositionTypeVector: {
							Instances children;
							(*inst)->getChildren(children);
							for(Instances::iterator c = children.begin(); c != children.end();
								++c) {
								checkLib(*c, discoveredLibs);
							}
							break;
						}
						default: {
						}
						}
					}
				}
			}
		}
	}

	void checkLib(const InstanceSharedPtr& inInst,
		SymTab<std::string, PrintType, true>& inDiscoveredLibs) {
		ViewSharedPtr iV = inInst->getMaster();
		LibrarySharedPtr targetL = iV->getParent()->getParent();
		PrintType type;
		if(!inDiscoveredLibs.get(targetL->getName(), type)) {
			mExterns.push_back(targetL);
			inDiscoveredLibs.set(targetL->getName(), ePrintTypeExtern);
			//We will print extern for this...
			//concrete may come later...
		}
	}
	std::vector<LibrarySharedPtr> mExterns;
	std::vector<LibrarySharedPtr> mConcreteLibs;
};

class Indenter {
public:
	size_t getIndentation() const;

	Indenter(Decompiler& inPrinter, bool inChangeIndentation = true);
	~Indenter();

private:
	Decompiler& mPrinter;
	bool mChangeIndentation;
};

size_t Indenter::getIndentation() const {
	return mPrinter.getIndentation();
}

Indenter::Indenter(Decompiler& inPrinter, bool inChangeIndentation) :
	mPrinter(inPrinter), mChangeIndentation(inChangeIndentation) {
	size_t indentation = getIndentation();
	indentation += (mChangeIndentation) ? 4 : 0;
	mPrinter.setIndentation(indentation);
}

Indenter::~Indenter() {
	size_t indentation = getIndentation();
	indentation -= (mChangeIndentation) ? 4 : 0;
	mPrinter.setIndentation(indentation);
}

}

namespace std {

std::ostream& operator<<(std::ostream& outStream, const Indenter& indent);

std::ostream& operator<<(std::ostream& outStream, const Indenter& indent) {
	for(size_t i = 0; i < indent.getIndentation(); i++) {
		outStream << " ";
	}
	return outStream;
}

} // namespace std

namespace torc {
namespace generic {

// For Root
void Decompiler::visit(Root& root) throw (Error) {
	try {
		CircularDependencyResolver resolver;
		resolver.resolve(root.getSharedThis());
		if(!root.getOriginalName().empty()) {
			mOut << "(edif (rename " << root.getName() << " " << "\"" << root.getOriginalName()
				<< "\"" << ")" << std::endl;
		} else {
			mOut << "(edif " << root.getName() << std::endl;
		}
		mOut << "  (edifVersion " << (int32_t) root.getVersion().mFirst << " "
			<< (int32_t) root.getVersion().mSecond << " " << (int32_t) root.getVersion().mThird
			<< ")" << std::endl;
		mOut << "  (edifLevel " << root.getLevel() << ")" << std::endl;
		mOut << "  (keywordMap (keywordLevel  0" << "))";

		//Print Comments
		std::vector < std::string > comments = root.getComments();
		printComments(comments);

		root.applyOnAllStatuses(mApplier);

		mIsExternContext = true;
		std::for_each(resolver.mExterns.begin(), resolver.mExterns.end(), mApplier);
		mIsExternContext = false;

		std::for_each(resolver.mConcreteLibs.begin(), resolver.mConcreteLibs.end(), mApplier);
		root.applyOnAllDesigns(mApplier);

		std::list < std::string > elements;
		root.getUserData(elements);
		printUserData(elements);

		mOut << std::endl << ")" << std::endl;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For Design
void Decompiler::visit(Design& design) throw (Error) {
	try {
		Indenter indent(*this);
		if(!design.getOriginalName().empty()) {
			mOut << std::endl << indent << "(design " << "(rename " << design.getName() << " "
				<< "\"" << design.getOriginalName() << "\"" << ")";
		} else {
			mOut << std::endl << indent << "(design " << design.getName();
		}
		mOut << std::endl << indent << "  (cellRef " << design.getCellRefName() << " (libraryRef "
			<< design.getLibraryRefName() << "))";

		//Print Comments
		std::vector < std::string > comments = design.getComments();
		printComments(comments);

		design.applyOnAllStatuses(mApplier);
		design.applyOnAllProperties(mApplier);

		std::list < std::string > elements;
		design.getUserData(elements);
		printUserData(elements);

		mOut << std::endl << indent << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For Library
void Decompiler::visit(Library& library) throw (Error) {
	try {
		Indenter indent(*this);
		if(mIsExternContext || library.getIsExtern()) {
			if(!library.getOriginalName().empty()) {
				mOut << std::endl << indent << "(external ";
				mOut << std::endl << indent << "  (rename " << library.getName() << " " << "\""
					<< library.getOriginalName() << "\"" << ")";
			} else {
				mOut << std::endl << indent << "(external " << library.getName();
			}
		} else {
			if(!library.getOriginalName().empty()) {
				mOut << std::endl << indent << "(library ";
				mOut << std::endl << indent << "  (rename " << library.getName() << " " << "\""
					<< library.getOriginalName() << "\"" << ")";
			} else {
				mOut << std::endl << indent << "(library " << library.getName();
			}
		}
		mOut << std::endl << indent << "  (ediflevel " << library.getLevel() << ")"
			<< " (technology (numberDefinition";

		std::map<Unit, ScaleFactor> outScaleFactors;
		outScaleFactors = library.getScaleFactors();
		std::map<Unit, ScaleFactor>::iterator it = outScaleFactors.begin();
		for(; it != outScaleFactors.end(); ++it) {
			Unit unit = (*it).first;
			ScaleFactor scale = (*it).second;
			mOut << std::endl << indent << "  (scale ";
			printValue(Value(Value::eValueTypeNumber, scale.getSiValue()));
			printValue(Value(Value::eValueTypeNumber, scale.getEdifValue()));
			printUnit(unit);
			mOut << ")";
		}
		mOut << ")";

		SimulationInfoSharedPtr simuInfo = library.getSimulationInfo();
		if(simuInfo) {
			simuInfo->accept(*this);
		}
		mOut << ")";

		//Print Comments
		std::vector < std::string > comments = library.getComments();
		printComments(comments);

		library.applyOnAllStatuses(mApplier);

		std::list < std::string > elements;
		library.getUserData(elements);
		printUserData(elements);

		library.applyOnAllCells(mApplier);
		mOut << std::endl << indent << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For Cell
void Decompiler::visit(Cell& cell) throw (Error) {
	try {
		Indenter indent(*this);
		std::string cellType;
		switch(cell.getType()) {
		case Cell::eTypeTie: {
			cellType = "TIE";
			break;
		}
		case Cell::eTypeRipper: {
			cellType = "RIPPER";
			break;
		}
		case Cell::eTypeGeneric: {
			cellType = "GENERIC";
			break;
		}
		default: {
		}
		}
		if(!cell.getOriginalName().empty()) {
			mOut << std::endl << indent << "(cell ";
			mOut << std::endl << indent << "  (rename " << cell.getName() << " " << "\""
				<< cell.getOriginalName() << "\"" << ")";
		} else {
			mOut << std::endl << indent << "(cell " << cell.getName();
		}
		mOut << std::endl << indent << "  (cellType " << cellType << ")";

		//Print Comments
		std::vector < std::string > comments = cell.getComments();
		printComments(comments);

		cell.applyOnAllStatuses(mApplier);

		std::list < std::string > elements;
		cell.getUserData(elements);
		printUserData(elements);

		cell.applyOnAllViews(mApplier);
		cell.applyOnAllProperties(mApplier);
		mOut << std::endl << indent << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For View
void Decompiler::visit(View& view) throw (Error) {
	try {
		Indenter indent(*this);
		std::string viewType;
		switch(view.getType()) {
		case View::eTypeMaskLayout: {
			viewType = "MASKLAYOUT";
			break;
		}
		case View::eTypePCBLayout: {
			viewType = "PCBLAYOUT";
			break;
		}
		case View::eTypeNetlist: {
			viewType = "NETLIST";
			break;
		}
		case View::eTypeSchematic: {
			viewType = "SCHEMATIC";
			break;
		}
		case View::eTypeSymbolic: {
			viewType = "SYMBOLIC";
			break;
		}
		case View::eTypeBehavior: {
			viewType = "BEHAVIOR";
			break;
		}
		case View::eTypeLogicModel: {
			viewType = "LOGICMODEL";
			break;
		}
		case View::eTypeDocument: {
			viewType = "DOCUMENT";
			break;
		}
		case View::eTypeGraphic: {
			viewType = "GRAPHIC";
			break;
		}
		case View::eTypeStranger: {
			viewType = "STRANGER";
			break;
		}
		}
		if(!view.getOriginalName().empty()) {
			mOut << std::endl << indent << "(view ";
			mOut << std::endl << indent << "  (rename " << view.getName() << " " << "\""
				<< view.getOriginalName() << "\"" << ")";
		} else {
			mOut << std::endl << indent << "(view " << view.getName();
		}
		mOut << std::endl << indent << "  (viewType " << viewType << ")";

		if(View::eTypeNetlist != view.getType()) {
			mOut << view.getNonNetlistViewData();
			return;
		}

		mOut << std::endl << indent << "  (interface ";
		mIsJoiningContext = false;
		view.applyOnAllPorts(mApplier);
		printViewInterfaceAttributes(view.getInterfaceAttributes());
		typedef std::map<std::string, ParameterSharedPtr> Params;
		ParameterMapSharedPtr params = view.getParameters();
		ParameterContext context = view.getParameterContext();
		Params paramMap;
		params->getAllParameters(context, paramMap);
		for(Params::iterator it = paramMap.begin(); it != paramMap.end(); it++) {
			(*it).second->accept(*this);
		}
		mIsJoiningContext = true;
		view.applyOnAllPermutables(mApplier);
		view.applyOnAllInterfaceJoinedInfos(mApplier);
		mOut << std::endl << indent << "  )";
		view.applyOnAllProperties(mApplier);

		//Print Comments
		std::vector < std::string > comments = view.getComments();
		printComments(comments);

		view.applyOnAllStatuses(mApplier);

		std::list < std::string > elements;
		view.getUserData(elements);
		printUserData(elements);

		if(!mIsExternContext) {

			std::vector<InstanceSharedPtr> instVec;
			view.getInstances(instVec);

			std::vector<NetSharedPtr> netVec;
			view.getNets(netVec);

			if(!instVec.empty() || !netVec.empty()) {
				mOut << std::endl << indent << "  (contents ";
				//Print all instances of the current view
				mIsJoiningContext = false;
				view.applyOnAllInstances(mApplier);

				if(NULL != view.getSimulate()) {
					view.getSimulate()->accept(*this);
				}

				//Print all net info
				mIsJoiningContext = true;
				view.applyOnAllNets(mApplier);

				if(NULL != view.getTiming()) {
					view.getTiming()->accept(*this);
				}

				mOut << std::endl << indent << "  )";
			}
		}
		mOut << std::endl << indent << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For ScalarPort
void Decompiler::visit(ScalarPort& port) throw (Error) {
	try {
		Indenter indent(*this);
		if(!mIsJoiningContext) {
			if(!port.getOriginalName().empty()) {
				mOut << std::endl << indent << "(port ";
				mOut << std::endl << indent << "  (rename " << port.getName() << " " << "\""
					<< port.getOriginalName() << "\"" << ")";
			} else {
				mOut << std::endl << indent << "(port " << port.getName();
			}
			switch(port.getDirection()) {
			case ePortDirectionIn: {
				mOut << std::endl << indent << "  (direction INPUT)";
				break;
			}
			case ePortDirectionOut: {
				mOut << std::endl << indent << "  (direction OUTPUT)";
				break;
			}
			case ePortDirectionInOut: {
				mOut << std::endl << indent << "  (direction INOUT)";
				break;
			}
			default: {
			}
			}
			printPortAttributes(port.getAttributes());

			//Print Comments
			std::vector < std::string > comments = port.getComments();
			printComments(comments);

			std::list < std::string > elements;
			port.getUserData(elements);
			printUserData(elements);

			port.applyOnAllProperties(mApplier);
			mOut << std::endl << indent << ")";
		} else {
			mOut << std::endl << indent << "(portRef " << port.getName();
			PortSharedPtr parentCol = port.getParentCollection();
			if(parentCol) {
				parentCol->accept(*this);
			}
			mOut << ")";
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For ScalarPortReference
void Decompiler::visit(ScalarPortReference& portRef) throw (Error) {
	try {
		Indenter indent(*this);
		if(mIsJoiningContext) {
			mOut << std::endl << indent << "(portRef " << portRef.getName();
			PortReferenceSharedPtr parentCol = portRef.getParentCollection();
			if(parentCol) {
				parentCol->accept(*this);
			} else {
				if(!mIsPortInstanceContext) {
					portRef.getParent()->accept(*this);
				}
			}
			mOut << ")";
		} else {
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For VectorPort
void Decompiler::visit(VectorPort& port) throw (Error) {
	try {
		Indenter indent(*this);
		if(mIsJoiningContext) {
			mOut << std::endl << indent << "(portRef " << port.getName();
			PortSharedPtr parentCol = port.getParentCollection();
			if(parentCol) {
				parentCol->accept(*this);
			}
			mOut << ")";
		} else {
			if(!port.getOriginalName().empty()) {
				mOut << std::endl << indent << "(port ";
				mOut << std::endl << indent << "  (array (rename " << port.getName() << " " << "\""
					<< port.getOriginalName() << "\"" << ")" << " ";
			} else {
				mOut << std::endl << indent << "(port (array " << port.getName() << " ";
			}
			std::vector < size_t > limits;
			port.getLimits(limits);
			copy(limits.begin(), limits.end(), std::ostream_iterator < size_t > (mOut, " "));
			mOut << ")";
			switch(port.getDirection()) {
			case ePortDirectionIn: {
				mOut << std::endl << indent << "  (direction INPUT)";
				break;
			}
			case ePortDirectionOut: {
				mOut << std::endl << indent << "  (direction OUTPUT)";
				break;
			}
			case ePortDirectionInOut: {
				mOut << std::endl << indent << "  (direction INOUT)";
				break;
			}
			default: {
			}

			}
			printPortAttributes(port.getAttributes());

			//Print Comments
			std::vector < std::string > comments = port.getComments();
			printComments(comments);

			std::list < std::string > elements;
			port.getUserData(elements);
			printUserData(elements);

			port.applyOnAllProperties(mApplier);
			mOut << std::endl << indent << ")";
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For VectorPortReference
void Decompiler::visit(VectorPortReference& portRef) throw (Error) {
	try {
		Indenter indent(*this);
		if(mIsJoiningContext) {
			mOut << std::endl << indent << "(portRef " << portRef.getName() << " ";
			PortReferenceSharedPtr parentCol = portRef.getParentCollection();
			if(parentCol) {
				parentCol->accept(*this);
			} else {
				if(!mIsPortInstanceContext) {
					portRef.getParent()->accept(*this);
				}
			}
			mOut << ")";
		} else {
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For VectorPortBit
void Decompiler::visit(VectorPortBit& bit) throw (Error) {
	try {
		Indenter indent(*this);
		if(mIsJoiningContext) {
			PortSharedPtr parent = bit.getParentCollection();
			mOut << std::endl << indent << "(portRef (member " << parent->getName() << " ";
			copy(bit.getIndices().begin(), bit.getIndices().end(),
				std::ostream_iterator < size_t > (mOut, " "));
			mOut << ")";
			PortSharedPtr parentCol = parent->getParentCollection();
			if(parentCol) {
				parentCol->accept(*this);
			}
			mOut << ")";
			bit.applyOnAllProperties(mApplier);
		} else {
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For VectorPortBitReference
void Decompiler::visit(VectorPortBitReference& bitRef) throw (Error) {
	try {
		Indenter indent(*this);
		if(mIsJoiningContext) {
			PortReferenceSharedPtr parent = bitRef.getParentCollection();
			mOut << std::endl << indent << "(portRef (member " << parent->getName() << " ";
			copy(bitRef.getIndices().begin(), bitRef.getIndices().end(),
				std::ostream_iterator < size_t > (mOut, " "));
			mOut << ")";
			PortReferenceSharedPtr parentCol = parent->getParentCollection();
			if(parentCol) {
				parentCol->accept(*this);
			} else {
				if(!mIsPortInstanceContext) {
					parent->getParent()->accept(*this);
				}
			}
			mOut << ")";
		} else {
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For PortBundle
void Decompiler::visit(PortBundle& port) throw (Error) {
	try {
		Indenter indent(*this);
		if(mIsJoiningContext) {
			if(mIsPermutableContext) {
				mOut << std::endl << indent << "(portRef " << port.getName();
			} else {
				mOut << " (portRef " << port.getName();
			}
			PortSharedPtr parentCol = port.getParentCollection();
			if(parentCol) {
				parentCol->accept(*this);
			}
			mOut << ")";
		} else {
			if(!port.getOriginalName().empty()) {
				mOut << std::endl << indent << "(portBundle ";
				mOut << std::endl << indent << "  (rename " << port.getName() << " " << "\""
					<< port.getOriginalName() << "\"" << ")";
			} else {
				mOut << std::endl << indent << "(portBundle " << port.getName();
			}
			{
				Indenter subIndent(*this);
				mOut << std::endl << subIndent << "(listOfPorts ";
				port.applyOnAllChildren(mApplier);
				mOut << std::endl << subIndent << ")";
			}

			//Print Comments
			std::vector < std::string > comments = port.getComments();
			printComments(comments);

			std::list < std::string > elements;
			port.getUserData(elements);
			printUserData(elements);

			mOut << std::endl << indent << ")";
		}
		port.applyOnAllProperties(mApplier);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For PortBundleReference
void Decompiler::visit(PortBundleReference& portRef) throw (Error) {
	try {
		Indenter indent(*this);
		mOut << std::endl << indent << "(portRef " << portRef.getName() << " ";
		PortReferenceSharedPtr parentCol = portRef.getParentCollection();
		if(parentCol) {
			parentCol->accept(*this);
		} else {
			portRef.getParent()->accept(*this);
		}
		mOut << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For SingleInstance
void Decompiler::visit(SingleInstance& instance) throw (Error) {
	try {

		if(mIsJoiningContext) {
			mOut << " (instanceRef " << instance.getName() << ")";
		} else {
			Indenter indent(*this);
			if(!instance.getOriginalName().empty()) {
				mOut << std::endl << indent << "(instance ";
				mOut << std::endl << indent << "  (rename " << instance.getName() << " " << "\""
					<< instance.getOriginalName() << "\"" << ")";
			} else {
				mOut << std::endl << indent << "(instance " << instance.getName();
			}
			mOut << std::endl << indent << "  (viewRef " << instance.getMaster()->getName()
				<< " (cellRef " << instance.getMaster()->getParent()->getName() << " (libraryRef "
				<< instance.getMaster()->getParent()->getParent()->getName() << ")))";

			if(!instance.getDesignator().empty()) {
				mOut << std::endl << indent << "  (designator " << "\"" << instance.getDesignator()
					<< "\"" << ")";
			}

			if(NULL != instance.getTiming()) {
				instance.getTiming()->accept(*this);
			}

			//Print Comments
			std::vector < std::string > comments = instance.getComments();
			printComments(comments);

			std::list < std::string > elements;
			instance.getUserData(elements);
			printUserData(elements);

			mParamAssignContext = true;
			ParameterMapSharedPtr params = instance.getParameters();
			ParameterContext context = instance.getParameterContext();
			params->applyOnOverriddenParameters(context, mApplier);
			mParamAssignContext = false;
			instance.applyOnAllProperties(mApplier);
			std::vector<PortReferenceSharedPtr> portRefs;
			instance.getPortReferences(portRefs);
			for(std::vector<PortReferenceSharedPtr>::iterator portRef = portRefs.begin();
				portRef != portRefs.end(); ++portRef) {
				PortAttributesSharedPtr attribs = (*portRef)->getAttributes();
				if(attribs) {
					mOut << std::endl << indent << "  (portInstance ";
					mIsPortInstanceContext = true;
					mIsJoiningContext = true;
					(*portRef)->accept(*this);
					mIsJoiningContext = false;
					mIsPortInstanceContext = false;
					printPortAttributes(attribs);
					mOut << std::endl << indent << "  )";
				}
			}
			mOut << std::endl << indent << ")";
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For ScalarNet
void Decompiler::visit(ScalarNet& net) throw (Error) {
	try {
		Indenter indent(*this);
		if(mIsJoiningContext) {
			if(!net.getOriginalName().empty()) {
				mOut << std::endl << indent << "(net ";
				mOut << std::endl << indent << "  (rename " << net.getName() << " " << "\""
					<< net.getOriginalName() << "\"" << ")";
			} else {
				mOut << std::endl << indent << "(net " << net.getName();
			}

			mOut << std::endl << indent << "  (joined ";
			mIsJoiningContext = true;
			net.applyOnAllConnectedPorts(mApplier);
			net.applyOnAllConnectedPortRefs(mApplier);
			net.applyOnAllConnectedPortLists(mApplier);
			mOut << std::endl << indent << "  )";

			printNetAttributes(net.getAttributes());

			//Print Comments
			std::vector < std::string > comments = net.getComments();
			printComments(comments);

			std::list < std::string > elements;
			net.getUserData(elements);
			printUserData(elements);

			net.applyOnAllSubnets(mApplier);
			net.applyOnAllProperties(mApplier);
			mOut << std::endl << indent << ")";
		} else {
			mOut << std::endl << indent << "(netRef " << net.getName();
			NetSharedPtr parentCol = net.getParentCollection();
			if(parentCol) {
				parentCol->accept(*this);
			}
			mOut << ")";
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For NetBundle
void Decompiler::visit(NetBundle& net) throw (Error) {
	try {
		Indenter indent(*this);
		if(mIsJoiningContext) {
			if(!net.getOriginalName().empty()) {
				mOut << std::endl << indent << "(netbundle ";
				mOut << std::endl << indent << "  (rename " << net.getName() << " " << "\""
					<< net.getOriginalName() << "\"" << ")";
			} else {
				mOut << std::endl << indent << "(netbundle " << net.getName();
			}
			mOut << std::endl << indent << "  (listOfNets ";
			net.applyOnAllSubnets(mApplier);
			net.applyOnAllChildren(mApplier);
			net.applyOnAllConnectedPorts(mApplier);
			net.applyOnAllConnectedPortRefs(mApplier);
			net.applyOnAllConnectedPortLists(mApplier);

			mOut << std::endl << indent << "  )";

			//Print Comments
			std::vector < std::string > comments = net.getComments();
			printComments(comments);

			std::list < std::string > elements;
			net.getUserData(elements);
			printUserData(elements);

			net.applyOnAllProperties(mApplier);
			mOut << std::endl << indent << ")";
		} else {
			mOut << std::endl << indent << "(netRef " << net.getName();
			NetSharedPtr parentCol = net.getParentCollection();
			if(parentCol) {
				parentCol->accept(*this);
			}
			mOut << ")";
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For VectorNet
void Decompiler::visit(VectorNet& net) throw (Error) {
	try {
		Indenter indent(*this);
		if(mIsJoiningContext) {
			if(!net.getOriginalName().empty()) {
				mOut << std::endl << indent << "(net ";
				mOut << std::endl << indent << "  (array (rename " << net.getName() << " " << "\""
					<< net.getOriginalName() << "\"" << ")" << " ";
			} else {
				mOut << std::endl << indent << "(net (array " << net.getName() << " ";
			}
			std::vector < size_t > limits;
			net.getLimits(limits);
			copy(limits.begin(), limits.end(), std::ostream_iterator < size_t > (mOut, " "));
			mOut << ")";

			mOut << std::endl << indent << "  (joined";
			mIsJoiningContext = true;
			net.applyOnAllConnectedPorts(mApplier);
			net.applyOnAllConnectedPortRefs(mApplier);
			net.applyOnAllConnectedPortLists(mApplier);
			mOut << std::endl << indent << "  )";

			printNetAttributes(net.getAttributes());

			//Print Comments
			std::vector < std::string > comments = net.getComments();
			printComments(comments);

			std::list < std::string > elements;
			net.getUserData(elements);
			printUserData(elements);

			net.applyOnAllSubnets(mApplier);
			net.applyOnAllProperties(mApplier);
			mOut << std::endl << indent << ")";
		} else {
			mOut << std::endl << indent << "(netRef " << net.getName();
			NetSharedPtr parentCol = net.getParentCollection();
			if(parentCol) {
				parentCol->accept(*this);
			}
			mOut << ")";
		}

	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For VectorNetBit
void Decompiler::visit(VectorNetBit& net) throw (Error) {
	try {
		//mIsJoiningContext = true;
#if 0    
		mOut << "                VectorNetBit name : " << net.getName() << std::endl;
		mOut << "                  Size            : " << net.getSize() << std::endl;
		net.applyOnAllSubnets(mApplier);
		net.applyOnAllConnectedPorts(mApplier);
		net.applyOnAllConnectedPortRefs(mApplier);
		net.applyOnAllProperties(mApplier);
#endif
		//TBD
		//mIsJoiningContext = false;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For InstanceArray
void Decompiler::visit(InstanceArray& instanceArray) throw (Error) {
	try {
		Indenter indent(*this);
		if(!instanceArray.getOriginalName().empty()) {
			mOut << std::endl << indent << "(instance ";
			mOut << std::endl << indent << "  (array (rename " << instanceArray.getName() << " "
				<< "\"" << instanceArray.getOriginalName() << "\"" << ")" << " ";
		} else {
			mOut << std::endl << indent << "(instance (array " << instanceArray.getName() << " ";
		}
		std::vector < size_t > limits;
		instanceArray.getLimits(limits);
		copy(limits.begin(), limits.end(), std::ostream_iterator < size_t > (mOut, " "));
		mOut << ")";
		mOut << std::endl << indent << "  (viewRef " << instanceArray.getMaster()->getName()
			<< " (cellRef " << instanceArray.getMaster()->getParent()->getName() << " (libraryRef "
			<< instanceArray.getMaster()->getParent()->getParent()->getName() << ")))";

		if(!instanceArray.getDesignator().empty()) {
			mOut << std::endl << indent << "  (designator " << "\"" << instanceArray.getDesignator()
				<< "\"" << ")";
		}

		//Print Comments
		std::vector < std::string > comments = instanceArray.getComments();
		printComments(comments);

		std::list < std::string > elements;
		instanceArray.getUserData(elements);
		printUserData(elements);

		mParamAssignContext = true;
		ParameterMapSharedPtr params = instanceArray.getParameters();
		ParameterContext context = instanceArray.getParameterContext();
		params->applyOnOverriddenParameters(context, mApplier);
		mParamAssignContext = false;
		instanceArray.applyOnAllProperties(mApplier);
		mOut << std::endl << indent << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For InstanceArrayMember
void Decompiler::visit(InstanceArrayMember& instance) throw (Error) {
	try {
		if(mIsJoiningContext) {
			mOut << "(instanceRef (member " << instance.getParentCollection()->getName() << " ";
			copy(instance.getIndices().begin(), instance.getIndices().end(),
				std::ostream_iterator < size_t > (mOut, " "));
			mOut << "))";
		} else {
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For SingleParameter
void Decompiler::visit(SingleParameter& inSingleParameter) throw (Error) {
	try {
		Indenter indent(*this);
		if(!mParamAssignContext) {
			if(!inSingleParameter.getOriginalName().empty()) {
				mOut << std::endl << indent << "(parameter ";
				mOut << std::endl << indent << "  (rename " << inSingleParameter.getName() << " "
					<< "\"" << inSingleParameter.getOriginalName() << "\"" << ")";
			} else {
				mOut << std::endl << indent << "(parameter " << inSingleParameter.getName() << " ";
			}
			printValueType(inSingleParameter.getValue());
			printValue(inSingleParameter.getValue());
			mOut << ")";
			printUnit(inSingleParameter.getUnit());
			mOut << ")";
		} else {
			mOut << std::endl << indent << "  (parameterAssign " << inSingleParameter.getName()
				<< " ";
			printValueType(inSingleParameter.getValue());
			printValue(inSingleParameter.getValue());
			mOut << "))";
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For ParameterArray
void Decompiler::visit(ParameterArray& inparamArray) throw (Error) {
	try {
		Indenter indent(*this);
		if(!mParamAssignContext) {
			if(!inparamArray.getOriginalName().empty()) {
				mOut << std::endl << indent << "(parameter ";
				mOut << std::endl << indent << "  (array (rename " << inparamArray.getName() << " "
					<< "\"" << inparamArray.getOriginalName() << "\"" << ")" << " ";
			} else {
				mOut << std::endl << indent << "(parameter (array " << inparamArray.getName()
					<< " ";
			}
			std::vector < size_t > limits;
			inparamArray.getLimits(limits);
			copy(limits.begin(), limits.end(), std::ostream_iterator < size_t > (mOut, " "));
			mOut << ")" << std::endl << indent << "  ";
			std::vector<ParameterSharedPtr> children;
			inparamArray.getChildren(children);
			std::vector<ParameterSharedPtr>::iterator start = children.begin();
			std::vector<ParameterSharedPtr>::iterator end = children.end();
			printValueType((*start)->getValue());
			printArray(limits, limits.size(), start, end);
			mOut << ")";
			printUnit(inparamArray.getUnit());
			mOut << std::endl << indent << ")";
		} else {
			mOut << std::endl << indent << "  (parameterAssign " << inparamArray.getName();
			std::vector<ParameterSharedPtr> children;
			inparamArray.getChildren(children);
			if(!children.empty()) {
				std::vector < size_t > limits;
				inparamArray.getLimits(limits);
				std::vector<ParameterSharedPtr>::iterator start = children.begin();
				std::vector<ParameterSharedPtr>::iterator end = children.end();
				mOut << std::endl << indent << "    ";
				printValueType((*start)->getValue());
				printArray(limits, limits.size(), start, end);
				mOut << ")";
				mOut << std::endl << indent << "  )";
			}
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

void Decompiler::visit(ParameterArrayElement& bit) throw (Error) {
	try {
		printValue(bit.getValue());
		printUnit(bit.getUnit());
		mOut << " ";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

//For Property
void Decompiler::visit(Property& inProperty) throw (Error) {
	try {
		Indenter indent(*this);
		if(!inProperty.getOriginalName().empty()) {
			mOut << std::endl << indent << "(property ";
			mOut << std::endl << indent << "  (rename " << inProperty.getName() << " " << "\""
				<< inProperty.getOriginalName() << "\"" << ")";
		} else {
			mOut << std::endl << indent << "(property " << inProperty.getName();
		}
		printValueType(inProperty.getValue());
		printValue(inProperty.getValue());
		mOut << ")";

		if(!inProperty.getOwner().empty()) {
			mOut << std::endl << indent << "  (owner " << "\"" << inProperty.getOwner() << "\""
				<< ")";
		}

		//Print Comments
		std::vector < std::string > comments = inProperty.getComments();
		printComments(comments);

		inProperty.applyOnAllChildren(mApplier);
		mOut << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For PortList
void Decompiler::visit(PortList& inPortList) throw (Error) {
	try {
		Indenter indent(*this);
		if(mIsJoiningContext) {
			std::list<PortList::PortListElement> elements;
			inPortList.getChildren(elements);
			mOut << std::endl << indent << "(portList ";
			for(std::list<PortList::PortListElement>::iterator it = elements.begin();
				it != elements.end(); ++it) {
				switch((*it).getType()) {
				case PortList::PortListElement::eElementTypePort: {
					(*it).getPort()->accept(*this);
					break;
				}
				case PortList::PortListElement::eElementTypePortReference: {
					(*it).getPortReference()->accept(*this);
					break;
				}
				}
			}
			mOut << std::endl << indent << ")";
		} else {
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For PortListAlias
void Decompiler::visit(PortListAlias& inPortListAlias) throw (Error) {
	try {
		Indenter indent(*this);
		mOut << std::endl << indent << "(portListAlias " << inPortListAlias.getName();
		mIsJoiningContext = true;
		inPortListAlias.getPortList()->accept(*this);
		mOut << std::endl << indent << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

//For Status
void Decompiler::visit(Status& status) throw (Error) {
	try {
		Indenter indent(*this);
		mOut << std::endl << indent << "(status ";
		std::vector<WrittenSharedPtr> elements;
		status.getWrittens(elements);

		for(std::vector<WrittenSharedPtr>::iterator it = elements.begin(); it != elements.end();
			++it) {
			mOut << std::endl << indent << "  (written ";
			mOut << std::endl << indent << "    (timeStamp ";
			WrittenSharedPtr written = *it;
			TimeStamp timestamp = written->getTimeStamp();
			mOut << timestamp.getYear() << " " << timestamp.getMonth() << " " << timestamp.getDay()
				<< " " << timestamp.getHour() << " " << timestamp.getMinute() << " "
				<< timestamp.getSecond() << ")";
			if(!written->getAuthorName().empty()) {
				mOut << std::endl << indent << "    (author " << "\"" << written->getAuthorName()
					<< "\"" << ")";
			}
			if(!written->getProgramName().empty()) {
				mOut << std::endl << indent << "    (program " << "\"" << written->getProgramName()
					<< "\"";
				if(!written->getProgramVersion().empty()) {
					mOut << std::endl << indent << "      (version " << "\""
						<< written->getProgramVersion() << "\"" << ")";
				}
				mOut << ")";
			}

			if(!written->getDataOriginLocationName().empty()) {
				mOut << std::endl << indent << "    (dataOrigin " << "\""
					<< written->getDataOriginLocationName() << "\"";
				if(!written->getDataOriginVersion().empty()) {
					mOut << std::endl << indent << "      (version " << "\""
						<< written->getDataOriginVersion() << "\"" << ")";
				}
				mOut << ")";
			}

			//Print Comments
			std::vector < std::string > comments = written->getComments();
			printComments(comments);

			std::list < std::string > userDatas;
			written->getUserData(userDatas);
			printUserData(userDatas);

			written->applyOnAllProperties(mApplier);

			mOut << std::endl << indent << "  )";
		}
		//Print Comments
		std::vector < std::string > comments = status.getComments();
		printComments(comments);

		std::list < std::string > userDatas;
		status.getUserData(userDatas);
		printUserData(userDatas);

		mOut << std::endl << indent << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

//For Permutable
void Decompiler::visit(Permutable& inPermutable) throw (Error) {
	try {
		mIsPermutableContext = true;
		Indenter indent(*this);
		if(mIsJoiningContext) {
			std::vector<PortSharedPtr> outPorts;
			inPermutable.getPorts(outPorts);
			if(!inPermutable.getIsNonPermutable()) {
				mOut << std::endl << indent << "(permutable ";
			} else {
				mOut << std::endl << indent << "(nonPermutable ";
			}
			std::vector<PortSharedPtr>::iterator port = outPorts.begin();
			std::vector<PortSharedPtr>::iterator end = outPorts.end();
			for(; port != end; ++port) {
				(*port)->accept(*this);
			}
			inPermutable.applyOnAllChildren(mApplier);
			mOut << std::endl << indent << ")";
		} else {
		}
		mIsPermutableContext = false;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

//For InterfaceJoinedInfo
void Decompiler::visit(InterfaceJoinedInfo& inInterfaceJoinedInfo) throw (Error) {
	try {
		mIsPermutableContext = true;
		Indenter indent(*this);
		if(mIsJoiningContext) {
			std::string joinedType;
			switch(inInterfaceJoinedInfo.getJoinedType()) {
			case InterfaceJoinedInfo::eJoinedTypeJoin: {
				joinedType = "joined";
				break;
			}
			case InterfaceJoinedInfo::eJoinedTypeWeak: {
				joinedType = "weakJoined";
				break;
			}
			case InterfaceJoinedInfo::eJoinedTypeMust: {
				joinedType = "mustJoin";
				break;
			}
			}
			mOut << std::endl << indent << "(" << joinedType;
			std::list<PortSharedPtr> outPorts;
			inInterfaceJoinedInfo.getPorts(outPorts);

			if(!outPorts.empty()) {
				std::list<PortSharedPtr>::iterator port = outPorts.begin();
				std::list<PortSharedPtr>::iterator end = outPorts.end();
				for(; port != end; ++port) {
					(*port)->accept(*this);
				}
			}

			std::list<PortListSharedPtr> outPortLists;
			inInterfaceJoinedInfo.getPortLists(outPortLists);
			if(!outPortLists.empty()) {
				std::list<PortListSharedPtr>::iterator portList = outPortLists.begin();
				std::list<PortListSharedPtr>::iterator end = outPortLists.end();
				for(; portList != end; ++portList) {
					(*portList)->accept(*this);
				}
			}

			inInterfaceJoinedInfo.applyOnAllChildren(mApplier);
			mOut << std::endl << indent << ")";
		} else {
		}
		mIsPermutableContext = false;
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For SimulationInfo
void Decompiler::visit(SimulationInfo& simuInfo) throw (Error) {
	try {
		Indenter indent(*this);
		mOut << std::endl << indent << "(simulationInfo";
		simuInfo.applyOnAllLogicValues(mApplier);

		//Print Comments
		std::vector < std::string > comments = simuInfo.getComments();
		printComments(comments);

		//Print UserData
		std::list < std::string > elements;
		simuInfo.getUserData(elements);
		printUserData(elements);

		mOut << std::endl << indent << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For Simulate
void Decompiler::visit(Simulate& simulate) throw (Error) {
	try {
		Indenter indent(*this);
		if(!simulate.getOriginalName().empty()) {
			mOut << std::endl << indent << "(simulate (rename " << simulate.getName() << " " << "\""
				<< simulate.getOriginalName() << "\"" << ")";
		} else {
			mOut << std::endl << indent << "(simulate " << simulate.getName();
		}

		simulate.applyOnAllPortListAlias(mApplier);
		simulate.applyOnAllWaveValues(mApplier);

		std::vector<ApplySharedPtr> outAllApply;
		simulate.getAllApply(outAllApply);
		std::vector<ApplySharedPtr>::iterator it = outAllApply.begin();
		for(; it != outAllApply.end(); it++) {
			(*it)->accept(*this);
		}

		//Print Comments
		std::vector < std::string > comments = simulate.getComments();
		printComments(comments);

		//Print UserData
		std::list < std::string > elements;
		simulate.getUserData(elements);
		printUserData(elements);

		mOut << std::endl << indent << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For Apply
void Decompiler::visit(Apply& apply) throw (Error) {
	try {
		Indenter indent(*this);
		mOut << std::endl << indent << "(apply " << "(cycle " << apply.getNoOfCycle()
			<< " (duration ";
		printValue(apply.getCycleDuration());
		mOut << "))";

		std::list<LogicalResponseSharedPtr> outLogicResponces;
		apply.getLogicResponses(outLogicResponces);
		std::list<LogicalResponseSharedPtr>::iterator logicIt = outLogicResponces.begin();
		for(; logicIt != outLogicResponces.end(); logicIt++) {
			(*logicIt)->accept(*this);
		}

		//Print Comments
		std::vector < std::string > comments = apply.getComments();
		printComments(comments);

		//Print UserData
		std::list < std::string > elements;
		apply.getUserData(elements);
		printUserData(elements);

		mOut << std::endl << indent << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For LogicalResponse logicInput/logicOutput()
void Decompiler::visit(LogicalResponse& logicalResponse) throw (Error) {
	try {
		Indenter indent(*this);
		std::string responseType;
		switch(logicalResponse.getResponseType()) {
		case LogicalResponse::eResponseTypeInput: {
			responseType = "logicInput";
			break;
		}
		case LogicalResponse::eResponseTypeOutput: {
			responseType = "logicOutput";
			break;
		}
		}
		mOut << std::endl << indent << "(" << responseType;
		if(NULL != logicalResponse.getConnectedPort()) {
			mOut << " " << logicalResponse.getConnectedPort()->getName();
		}
		if(NULL != logicalResponse.getConnectedPortListAlias()) {
			mOut << " " << logicalResponse.getConnectedPortListAlias()->getName();
		}
		if(NULL != logicalResponse.getConnectedPortList()) {
			logicalResponse.getConnectedPortList()->accept(*this);
		}
		LogicElementSharedPtr logicElement = logicalResponse.getLogicWaveForm();
		if(logicElement) {
			logicElement->accept(*this);
		}
		mOut << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For LogicElement
void Decompiler::visit(LogicElement& logicElem) throw (Error) {
	try {
		Indenter indent(*this);
		std::string type;
		std::string logicValueName;
		switch(logicElem.getType()) {
		case LogicElement::eTypeSingle: {
			type = "";
			logicValueName = logicElem.getName();
			break;
		}
		case LogicElement::eTypeList: {
			type = "logicList";
			break;
		}
		case LogicElement::eTypeOneOf: {
			type = "logicOneOf";
			break;
		}
		case LogicElement::eTypeWaveForm: {
			type = "logicWaveForm";
			break;
		}
		case LogicElement::eTypeIgnored: {
			type = "ignore";
			break;
		}
		case LogicElement::eTypeTransition: {
			type = "transition";
			break;
		}
		case LogicElement::eTypeBecomes: {
			type = "becomes";
			break;
		}
		}

		if(LogicElement::eTypeSingle == logicElem.getType()) {
			mOut << " " << logicValueName;
		} else {
			mOut << std::endl << indent << "(" << type;
			logicElem.applyOnAllChildren(mApplier);
			mOut << ")";
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

//For WaveValue
void Decompiler::visit(WaveValue& waveValue) throw (Error) {
	try {
		Indenter indent(*this);
		mOut << std::endl << indent << "(waveValue " << waveValue.getName() << " ";
		printValue(waveValue.getDeltaTimeDuration());
		if(NULL != waveValue.getLogicWaveform()) {
			waveValue.getLogicWaveform()->accept(*this);
		}
		mOut << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For LogicValue
void Decompiler::visit(LogicValue& logicVal) throw (Error) {
	try {
		Indenter indent(*this);
		if(!logicVal.getOriginalName().empty()) {
			mOut << std::endl << indent << "(logicValue ";
			mOut << std::endl << indent << "  (rename " << logicVal.getName() << " " << "\""
				<< logicVal.getOriginalName() << "\"" << ")";
		} else {
			mOut << std::endl << indent << "(logicValue " << logicVal.getName();
		}

		//TBD
		//Print attributes
		printLogicValueAttributes(logicVal.getAttributes());

		//Print Comments
		std::vector < std::string > comments = logicVal.getComments();
		printComments(comments);

		logicVal.applyOnAllProperties(mApplier);

		//Print UserData
		std::list < std::string > elements;
		logicVal.getUserData(elements);
		printUserData(elements);

		mOut << std::endl << indent << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For Timing
void Decompiler::visit(Timing& timing) throw (Error) {
	try {
		Indenter indent(*this);
		mOut << std::endl << indent << "(timing " << std::endl;
		mOut << indent;
		printDerivation(timing.getDerivation());

		std::list<PathDelaySharedPtr> outPathDelays;
		timing.getPathDelays(outPathDelays);
		std::list<PathDelaySharedPtr>::iterator pathDelayIt = outPathDelays.begin();
		for(; pathDelayIt != outPathDelays.end(); pathDelayIt++) {
			mOut << indent << "    (pathDelay " << std::endl;
			mOut << indent << "        (delay ";
			PathDelaySharedPtr pathDelay = *pathDelayIt;
			Value::MiNoMax mnm = pathDelay->getDelay();
			Value val(Value::eValueTypeMiNoMax, mnm);
			printValue(val);
			mOut << ")" << std::endl;

			//Print event
			std::list<EventSharedPtr> outEvents;
			pathDelay->getEvents(outEvents);
			std::list<EventSharedPtr>::iterator eventIt = outEvents.begin();
			for(; eventIt != outEvents.end(); eventIt++) {
				mOut << "    ";
				(*eventIt)->accept(*this);
			}
			mOut << indent << "    )" << std::endl;
		}

		//Print forbiddenEvent
		std::list<ForbiddenEventSharedPtr> outForbiddentEvents;
		timing.getForbiddentEvents(outForbiddentEvents);
		std::list<ForbiddenEventSharedPtr>::iterator forbiddentEventIt =
			outForbiddentEvents.begin();
		for(; forbiddentEventIt != outForbiddentEvents.end(); forbiddentEventIt++) {
			(*forbiddentEventIt)->accept(*this);
		}

		//Print Comments
		std::vector < std::string > comments = timing.getComments();
		printComments(comments);

		//Print UserData
		std::list < std::string > elements;
		timing.getUserData(elements);
		printUserData(elements);

		mOut << std::endl << indent << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

//For Event
void Decompiler::visit(Event& event) throw (Error) {
	try {
		Indenter indent(*this);
		std::string type;
		switch(event.getType()) {
		case Event::eTypeEvent: {
			type = "(event";
			break;
		}
		case Event::eTypeOffsetEvent: {
			type = "(offsetEvent (event";
			break;
		}
		}

		mOut << indent << type;

		//Print port or portGroup
		mIsJoiningContext = true;
		std::list<PortElement> outPortElements;
		event.getPortElements(outPortElements);
		std::list<PortElement>::iterator portElem = outPortElements.begin();
		std::list<PortElement>::iterator end = outPortElements.end();
		if(outPortElements.size() > 1) {
			(*this).setIndentation((*this).getIndentation() + 8);
			mOut << std::endl << indent << "(portGroup";
		}
		if(outPortElements.size() == 1) {
			(*this).setIndentation((*this).getIndentation() + 4);
		}

		for(; portElem != end; ++portElem) {
			switch((*portElem).getType()) {
			case PortElement::eElementTypePort: {
				(*portElem).getPort()->accept(*this);
				break;
			}
			case PortElement::eElementTypePortReference: {
				(*portElem).getPortReference()->accept(*this);
				break;
			}
			}
		}

		if(outPortElements.size() > 1) {
			mOut << ")";
			(*this).setIndentation((*this).getIndentation() - 8);
		}
		if(outPortElements.size() == 1) {
			(*this).setIndentation((*this).getIndentation() - 4);
		}
		//Print portList
		if(NULL != event.getPortList()) {
			(*this).setIndentation((*this).getIndentation() + 4);
			event.getPortList()->accept(*this);
			(*this).setIndentation((*this).getIndentation() - 4);
		}
		//Print net or netGroup
		mIsJoiningContext = false;
		std::list<NetSharedPtr> outNets;
		event.getNets(outNets);
		std::list<NetSharedPtr>::iterator net = outNets.begin();
		if(outNets.size() > 1) {
			(*this).setIndentation((*this).getIndentation() + 8);
			mOut << std::endl << indent << "(netGroup";
		}
		if(outNets.size() == 1) {
			(*this).setIndentation((*this).getIndentation() + 4);
		}

		for(; net != outNets.end(); ++net) {
			(*net)->accept(*this);
		}

		if(outNets.size() > 1) {
			mOut << ")";
			(*this).setIndentation((*this).getIndentation() - 8);
		}
		if(outNets.size() == 1) {
			(*this).setIndentation((*this).getIndentation() - 4);
		}

		//Print transition/becomes
		if(NULL != event.getTransition()) {
			(*this).setIndentation((*this).getIndentation() + 4);
			event.getTransition()->accept(*this);
			(*this).setIndentation((*this).getIndentation() - 4);
		}

		switch(event.getType()) {
		case Event::eTypeEvent: {
			mOut << ")" << std::endl;
			break;
		}
		case Event::eTypeOffsetEvent: {
			mOut << ") "; //end of event
			printValue(event.getOffsetTime());
			mOut << ")" << std::endl;
			break;
		}
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

//For ForbiddenEvent
void Decompiler::visit(ForbiddenEvent& forbiddenevent) throw (Error) {
	try {
		Indenter indent(*this);
		mOut << indent << "(forbiddenEvent" << std::endl;
		mOut << indent << "    (timeInterval" << std::endl;
		if(NULL != forbiddenevent.getStartTimeInterval()) {
			mOut << "    ";
			forbiddenevent.getStartTimeInterval()->accept(*this);
		}
		if(NULL != forbiddenevent.getEndTimeInterval()) {
			mOut << "    ";
			forbiddenevent.getEndTimeInterval()->accept(*this);
		} else {
			mOut << "    ";
			mOut << indent << "    (duration ";
			printValue(forbiddenevent.getDuration());
			mOut << ")" << std::endl;
		}
		mOut << indent << "    )" << std::endl;

		std::list<EventSharedPtr> outEvents;
		forbiddenevent.getEvents(outEvents);
		std::list<EventSharedPtr>::iterator eventIt = outEvents.begin();
		for(; eventIt != outEvents.end(); eventIt++) {
			(*eventIt)->accept(*this);
		}
		mOut << indent << ")";
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For UserData
void Decompiler::printUserData(std::list<std::string> inElements) throw (Error) {
	try {
		Indenter indent(*this);
		if(!inElements.empty()) {
			for(std::list<std::string>::iterator it = inElements.begin(); it != inElements.end();
				++it) {
				mOut << std::endl << indent << "(userData " << trimLeadingSpaces(*it);
			}
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

// For Comment
void Decompiler::printComments(std::vector<std::string>& inElements) throw (Error) {
	try {
		Indenter indent(*this);
		if(!inElements.empty()) {
			for(std::vector<std::string>::iterator it = inElements.begin(); it != inElements.end();
				++it) {
				mOut << std::endl << indent << "(comment " << "\"" << trimLeadingSpaces(*it) << "\""
					<< ")";
			}
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}
void Decompiler::printValueType(const Value& value) throw (Error) {
	switch(value.getType()) {
	case Value::eValueTypeBoolean: {
		mOut << " (boolean ";
		break;
	}
	case Value::eValueTypeInteger: {
		mOut << " (integer ";
		break;
	}
	case Value::eValueTypeMiNoMax: {
		mOut << " (minomax ";
		break;
	}
	case Value::eValueTypeNumber: {
		mOut << " (number ";
		break;
	}
	case Value::eValueTypePoint: {
		mOut << " (point ";
		break;
	}
	case Value::eValueTypeString: {
		mOut << " (string ";
		break;
	}
	default: {
	}
	}
}

void Decompiler::printValue(const Value& value) throw (Error) {
	if(!value.getIsSet()) {
		return;
	}
	switch(value.getType()) {
	case Value::eValueTypeBoolean: {
		mOut << ((value.get<Value::Boolean>()) ? "(true)" : "(false)");
		break;
	}
	case Value::eValueTypeInteger: {
		mOut << value.get<Value::Integer>();
		break;
	}
	case Value::eValueTypeMiNoMax: {
		Value::MiNoMax mnm = value.get<Value::MiNoMax>();
		mOut << "(mnm ";
		if(!mnm.getMinUndefined()) {
			Value val(Value::eValueTypeNumber, mnm.getMin());
			printValue(val);
			mOut << " ";
		} else {
			mOut << "(undefined) ";
		}

		Value val(Value::eValueTypeNumber, mnm.getNominal());
		printValue(val);
		mOut << " ";

		if(!mnm.getMinUndefined()) {
			Value val(Value::eValueTypeNumber, mnm.getMax());
			printValue(val);
		} else {
			mOut << "(undefined)";
		}
		mOut << ")";
		break;
	}
	case Value::eValueTypeNumber: {
		Value::Number num = value.get<Value::Number>();
		mOut << "(e " << num.getMantissa() << " " << num.getExponent() << ")";
		break;
	}
	case Value::eValueTypePoint: {
		Value::Point pt = value.get<Value::Point>();
		mOut << "(pt " << pt.getX() << " " << pt.getY() << ")";
		break;
	}
	case Value::eValueTypeString: {
		mOut << "\"" << value.get<Value::String>() << "\"";
		break;
	}
	default: {
	}
	}
}

void Decompiler::printUnit(const Unit unit) throw (Error) {
	switch(unit) {
	case eUnitAngle: {
		mOut << " (unit ANGLE )";
		break;
	}
	case eUnitCapacitance: {
		mOut << " (unit CAPACITANCE )";
		break;
	}
	case eUnitConductance: {
		mOut << " (unit CONDUCTANCE )";
		break;
	}
	case eUnitCharge: {
		mOut << " (unit CHARGE )";
		break;
	}
	case eUnitCurrent: {
		mOut << " (unit CURRENT )";
		break;
	}
	case eUnitDistance: {
		mOut << " (unit DISTANCE )";
		break;
	}
	case eUnitEnergy: {
		mOut << " (unit ENERGY )";
		break;
	}
	case eUnitFlux: {
		mOut << " (unit FLUX )";
		break;
	}
	case eUnitFrequency: {
		mOut << " (unit FREQUENCY )";
		break;
	}
	case eUnitInductance: {
		mOut << " (unit INDUCTANCE )";
		break;
	}
	case eUnitMass: {
		mOut << " (unit MASS )";
		break;
	}
	case eUnitPower: {
		mOut << " (unit POWER )";
		break;
	}
	case eUnitResistance: {
		mOut << " (unit RESISTANCE )";
		break;
	}
	case eUnitTemperature: {
		mOut << " (unit TEMPERATURE )";
		break;
	}
	case eUnitTime: {
		mOut << " (unit TIME )";
		break;
	}
	case eUnitVoltage: {
		mOut << " (unit VOLTAGE )";
		break;
	}
	default: {
	}
	}
}

void Decompiler::printDerivation(const Derivation derivation) throw (Error) {
	mOut << "    (derivation ";
	switch(derivation) {
	case eDerivationCalculated: {
		mOut << "CALCULATED";
		break;
	}
	case eDerivationMeasured: {
		mOut << "MEASURED";
		break;
	}
	case eDerivationRequired: {
		mOut << "REQUIRED";
		break;
	}
	default: {
	}
	}
	mOut << ")" << std::endl;
}

void Decompiler::printArray(const std::vector<size_t>& outVector, size_t depth,
	std::vector<ParameterSharedPtr>::iterator& itStart,
	std::vector<ParameterSharedPtr>::iterator& itEnd) throw (Error) {
	Indenter indent(*this);

	size_t array_size = outVector.size();
	/*std::vector<size_t>::const_iterator it = outVector.begin();*/
	for(size_t i = 0; i < outVector.at(array_size - depth); i++) {
		if(depth == 1) {
			printValue((*itStart)->getValue());
			mOut << " ";
			if(itStart != itEnd) {
				++itStart;
			}
		} else {
			mOut << std::endl << indent << "  ";
			printValueType((*itStart)->getValue());
			printArray(outVector, depth - 1, itStart, itEnd);
			mOut << ")";
		}
	}
}

void Decompiler::printPortAttributes(const PortAttributesSharedPtr& inAttrib) throw (Error) {
	if(!inAttrib)
		return;

	Indenter indent(*this);
	try {
		{
			Value unused = inAttrib->getUnused();
			if(unused.getIsSet()) {
				mOut << std::endl << indent << "(unused)";
			}
		}
		{
			Value designator = inAttrib->getDesignator();
			if(designator.getIsSet()) {
				mOut << std::endl << indent << "(designator " << "\""
					<< designator.get<Value::String>() << "\"" << ")";
			}
		}
		{
			Value val = inAttrib->getDcFaninLoad();
			if(val.getIsSet()) {
				mOut << std::endl << indent << "(dcFaninLoad ";
				printValue(val);
				mOut << ")";
			}
		}
		{
			Value val = inAttrib->getDcFanoutLoad();
			if(val.getIsSet()) {
				mOut << std::endl << indent << "(dcFanoutLoad ";
				printValue(val);
				mOut << ")";
			}
		}
		{
			Value val = inAttrib->getDcMaxFanin();
			if(val.getIsSet()) {
				mOut << std::endl << indent << "(dcMaxFanin ";
				printValue(val);
				mOut << ")";
			}
		}
		{
			Value val = inAttrib->getDcMaxFanout();
			if(val.getIsSet()) {
				mOut << std::endl << indent << "(dcMaxFanout ";
				printValue(val);
				mOut << ")";
			}
		}
		{
			Value val = inAttrib->getAcLoad();
			if(val.getIsSet()) {
				mOut << std::endl << indent << "(acLoad ";
				printValue(val);
				mOut << ")";
			}
		}
		if(inAttrib->getIsPortDelaySet()) {
			PortDelay delay = inAttrib->getPortDelay();
			mOut << std::endl << indent << "(portDelay " << std::endl;
			mOut << indent;
			printDerivation(delay.getDerivation());
			switch(delay.getType()) {
			case PortDelay::eTypeDelay: {
				mOut << indent << "    (delay ";
				Value::MiNoMax mnm = delay.getDelay();
				Value val(Value::eValueTypeMiNoMax, mnm);
				printValue(val);
				mOut << ")";
				break;
			}
			case PortDelay::eTypeLoadDelay: {
				mOut << indent << "    (loadDelay ";
				{
					Value::MiNoMax mnm = delay.getDelay();
					Value val(Value::eValueTypeMiNoMax, mnm);
					printValue(val);
				}
				{
					Value::MiNoMax mnm = delay.getAcLoad();
					Value val(Value::eValueTypeMiNoMax, mnm);
					printValue(val);
				}
				mOut << ")";
				break;
			}
			}
			//Print transition/becomes
			if(NULL != delay.getTransition()) {
				delay.getTransition()->accept(*this);
			}
			mOut << ")";
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

void Decompiler::printNetAttributes(const NetAttributesSharedPtr& inAttrib) throw (Error) {
	if(!inAttrib)
		return;

	Indenter indent(*this);
	try {
		(*this).setIndentation((*this).getIndentation() - 2);
		if(inAttrib->getIsCriticalitySet()) {
			int32_t criticality = inAttrib->getCriticality();
			mOut << std::endl << indent << "(criticality " << criticality << ")";
		}
		if(inAttrib->getIsNetDelaySet()) {
			NetDelay netDelay = inAttrib->getNetDelay();
			mOut << std::endl << indent << "(netDelay " << std::endl;
			mOut << indent;
			printDerivation(netDelay.getDerivation());

			mOut << indent << "    (delay ";
			Value::MiNoMax mnm = netDelay.getDelay();
			Value val(Value::eValueTypeMiNoMax, mnm);
			printValue(val);
			mOut << ")";

			//Print transition/becomes
			if(NULL != netDelay.getTransition()) {
				netDelay.getTransition()->accept(*this);
			}
			mOut << std::endl << indent << ")";
		}
		(*this).setIndentation((*this).getIndentation() + 2);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

void Decompiler::printViewInterfaceAttributes(const InterfaceAttributesSharedPtr& inAttrib)
	throw (Error) {
	if(!inAttrib)
		return;

	Indenter indent(*this);
	try {
		if(!inAttrib->getDesignator().empty()) {
			mOut << std::endl << indent << "  (designator " << "\"" << inAttrib->getDesignator()
				<< "\"" << ")";
		}

		if(NULL != inAttrib->getSimulate()) {
			(*this).setIndentation((*this).getIndentation() - 4);
			inAttrib->getSimulate()->accept(*this);
			(*this).setIndentation((*this).getIndentation() + 4);
		}

		if(NULL != inAttrib->getTiming()) {
			(*this).setIndentation((*this).getIndentation() - 4);
			inAttrib->getTiming()->accept(*this);
			(*this).setIndentation((*this).getIndentation() + 4);
		}

		//Print Comments
		std::vector < std::string > comments = inAttrib->getComments();
		printComments(comments);

		std::list < std::string > userDatas;
		inAttrib->getUserData(userDatas);
		printUserData(userDatas);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

//For LogicValueAttributes
void Decompiler::printLogicValueAttributes(const LogicValueAttributesSharedPtr& inAttrib)
	throw (Error) {

	if(!inAttrib)
		return;

	Indenter indent(*this);
	try {
		{
			if(inAttrib->getIsVoltageMapSet()) {
				Value::MiNoMax mnm = inAttrib->getVoltageMap();
				Value val(Value::eValueTypeMiNoMax, mnm);
				mOut << std::endl << indent << "(voltageMap ";
				printValue(val);
				mOut << ")";
			}
		}
		{
			if(inAttrib->getIsCurrentMapSet()) {
				Value::MiNoMax mnm = inAttrib->getCurrentMap();
				Value val(Value::eValueTypeMiNoMax, mnm);
				mOut << std::endl << indent << "(currentMap ";
				printValue(val);
				mOut << ")";
			}
		}
		{
			if(inAttrib->getIsBooleanMapSet()) {
				Value::Boolean boolean = inAttrib->getBooleanMap();
				Value val(Value::eValueTypeBoolean, boolean);
				mOut << std::endl << indent << "(booleanMap ";
				printValue(val);
				mOut << ")";
			}
		}
		{
			LogicValueSharedPtr logicVal = inAttrib->getWeakLogicValue();
			if(logicVal) {
				mOut << std::endl << indent << "(weak " << logicVal->getName() << ")";
			}
		}
		{
			LogicValueSharedPtr logicVal = inAttrib->getStrongLogicValue();
			if(logicVal) {
				mOut << std::endl << indent << "(strong " << logicVal->getName() << ")";
			}
		}
		{
			std::list<LogicValueSharedPtr> outLogicValues;
			inAttrib->getCompoundLogicValues(outLogicValues);
			if(!outLogicValues.empty()) {
				mOut << std::endl << indent << "(compound";
				std::list<LogicValueSharedPtr>::iterator it = outLogicValues.begin();
				for(; it != outLogicValues.end(); it++) {
					mOut << " " << (*it)->getName();
				}
				mOut << ")";
			} else {
			}
		}
		{
			std::list<LogicValueSharedPtr> outLogicValues;
			inAttrib->getDominatedLogicValues(outLogicValues);
			if(!outLogicValues.empty()) {
				mOut << std::endl << indent << "(dominates";
				std::list<LogicValueSharedPtr>::iterator it = outLogicValues.begin();
				for(; it != outLogicValues.end(); it++) {
					mOut << " " << (*it)->getName();
				}
				mOut << ")";
			} else {
			}
		}
		{
			std::list<LogicValueSharedPtr> outLogicValues;
			inAttrib->getResolvedLogicValues(outLogicValues);
			if(!outLogicValues.empty()) {
				mOut << std::endl << indent << "(resolves";
				std::list<LogicValueSharedPtr>::iterator it = outLogicValues.begin();
				for(; it != outLogicValues.end(); it++) {
					mOut << " " << (*it)->getName();
				}
				mOut << ")";
			} else {
			}
		}
		{
			bool isolated = inAttrib->getIsIsolated();
			if(isolated) {
				mOut << std::endl << indent << "(isolated)";
			}
		}
		{
			typedef LogicValueAttributes::LogicMap LogicMap;
			std::list<LogicMap*> outLogicMaps;
			inAttrib->getLogicMaps(outLogicMaps);
			if(!outLogicMaps.empty()) {
				std::list<LogicMap*>::iterator it = outLogicMaps.begin();

				for(; it != outLogicMaps.end(); it++) {
					if((*it)->mLogicMapType == LogicMap::eLogicMapTypeInput) {
						mOut << std::endl << indent << "(logicMapInput ";
					} else {
						mOut << std::endl << indent << "(logicMapOutput ";
					}
					mOut << "(logicRef " << (*it)->mLogicRef;
					mOut << " (libraryRef " << (*it)->mLibraryRef << ")";
					mOut << "))";
				}
			} else {
			}
		}
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}
void Decompiler::operator()() throw (Error)
try {
	decompile();
}
catch(Error& e) {
	e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
	throw;
}

/**
 * Decompile the tree.
 * 
 * @param[in] root Root of the OM tree.
 */
void Decompiler::decompile() throw (Error) {
	try {
		mRoot->accept(*this);
	} catch(Error& e) {
		e.setCurrentLocation(__FUNCTION__, __FILE__, __LINE__);
		throw;
	}
}

void Decompiler::setIndentation(size_t inIndentation) {
	mIndentation = inIndentation;
}

Decompiler::Decompiler(const RootSharedPtr& inRoot, std::ostream& outStream) : mRoot(inRoot),
	mOut(outStream), mIsJoiningContext(false), mIsPermutableContext(false),
	mIsExternContext(false), mParamAssignContext(false), mIsPortInstanceContext(false),
	mIndentation(0), mApplier(*this) {}

Decompiler::~Decompiler() throw () {}

Decompiler::Decompiler(const Decompiler& inSource) :
	mRoot(inSource.mRoot), mOut(inSource.mOut), mIsJoiningContext(inSource.mIsJoiningContext), mIsExternContext(
		inSource.mIsExternContext), mParamAssignContext(inSource.mParamAssignContext), mIsPortInstanceContext(
		inSource.mIsPortInstanceContext), mIndentation(inSource.mIndentation), mApplier(*this) {}

Decompiler& Decompiler::operator=(const Decompiler& inSource) {
	if(this != &inSource) {
		mRoot = inSource.mRoot;
		mIsJoiningContext = inSource.mIsJoiningContext;
		mIsExternContext = inSource.mIsExternContext;
		mParamAssignContext = inSource.mParamAssignContext;
		mIsPortInstanceContext = inSource.mIsPortInstanceContext;
		mIndentation = inSource.mIndentation;
	}
	return *this;
}

} // namespace generic
} // namespace torc
