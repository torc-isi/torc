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

/// \file
/// \brief Source for the VerilogImporterVisitor class.

#include "torc/generic/verilog/VerilogImporterVisitor.hpp"
#include "torc/generic/verilog/TemporaryAssignment.hpp"
#include "boost/regex.hpp"

namespace torc {
namespace generic {

/// \todo: "assign n1274_n = datain[7];" generates "(property torc_assign_rhs (string "7"))"

#define DEBUG true

	/// \brief Initial file line information.
	FileLine VerilogImporterVisitor::sNullFileLine("", -1);

	template<class T> std::string compositionTypeString(const T& inPtr) {
		if(!inPtr) return "null";
		switch(inPtr->getCompositionType()) {
			case eCompositionTypeScalar: return "scalar";
			case eCompositionTypeVector: return "vector";
			case eCompositionTypeVectorBit: return "vector bit";
			case eCompositionTypeBundle: return "bundle";
			default: return "unknown";
		}
	}

	void VerilogImporterVisitor::createDesigns(void) {
		// find all cells that are not instantiated, and make them into top-level designs
		CellToBoolMap::const_iterator p = mCellInstantiationFlag.begin();
		CellToBoolMap::const_iterator e = mCellInstantiationFlag.end();
		while(p != e) {
			std::pair<const CellSharedPtr, bool> element = *p++;
			if(element.second) continue;
			CellSharedPtr cellPtr = element.first;
			LibrarySharedPtr libraryPtr = cellPtr->getParent();
			std::string name = cellPtr->getName();
			//std::cerr << "Found top-level design " << name << std::endl;
			mObjectFactoryPtr->newDesignPtr(name, mRootPtr, name, libraryPtr->getName(), 
				cellPtr->getOriginalName());
		}
	}

	/// \brief Visit the top-level netlist.
	void VerilogImporterVisitor::visit(AstNetlist* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstNetlist" << std::endl;
		(void) userPtr;
		//std::cerr << "top-level netlist " << std::endl;
		nodePtr->iterateChildren(*this);

		// create EDIF designs for non-instantiated non-library cells
		createDesigns();
		// delete empty libraries
		typedef vector<CellSharedPtr> CellSharedPtrVector;
		CellSharedPtrVector cells;
		mInferredBlackBoxesLibraryPtr->getCells(cells);
		if(cells.size() == 0) mRootPtr->removeLibrary(mInferredBlackBoxesLibraryPtr->getName());
		mImportedCellLibraryPtr->getCells(cells);
		if(cells.size() == 0) mRootPtr->removeLibrary(mImportedCellLibraryPtr->getName());
	}

	/// \brief Visit a Verilog module, and create an EDIF cell and view.
	void VerilogImporterVisitor::visit(AstModule* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstModule" << std::endl;
		(void) userPtr;
		// if the module is already defined, do not attempt to redefine it
		if(mMasterNameToView[nodePtr->name()]) return;
		// if the module belongs to a library, define it appropriately
		bool libraryCell = nodePtr->inLibrary();
		if(libraryCell && !mImportLibraryCells) return;
		if(libraryCell) mCurrentLibraryPtr = mImportedCellLibraryPtr;
		std::cerr << "module " << nodePtr->name() << std::endl;
		// create and add the cell
		mCurrentCellPtr = mObjectFactoryPtr->newCellPtr(nodePtr->name(), mCurrentLibraryPtr, 
			Cell::eTypeGeneric, "");
		if(nodePtr->prettyName() != nodePtr->name()) 
			mCurrentCellPtr->setOriginalName(nodePtr->prettyName());
//std::cerr << "Original module name " << mCurrentCellPtr->getOriginalName() << std::endl;
		if(!libraryCell) mCellInstantiationFlag[mCurrentCellPtr] = false;
		// create and add the view
		mCurrentViewPtr = mObjectFactoryPtr->newViewPtr(VerilogNames::getImportedVerilogViewName(),
			mCurrentCellPtr, View::eTypeNetlist, "");
		// map the master name to this view
		mMasterNameToView[nodePtr->name()] = mCurrentViewPtr;
		// clear the map of vector bit nets
		mVectorBitNameToNet.clear();
		// visit our children
		nodePtr->iterateChildren(*this);
	}

	/// \brief Visit a Verilog instance, and create and EDIF instance with populated pins.
	void VerilogImporterVisitor::visit(AstCell* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstCell" << std::endl;
		(void) userPtr;
		std::cerr << "    " << nodePtr->modName() << " " << nodePtr->name();
		if(nodePtr->prettyName() != nodePtr->name()) 
			std::cerr << " \"" << nodePtr->prettyName() << "\"";
		std::cerr << endl;
		// import the associated library cell if applicable
		AstNode* modPtr = nodePtr->modp();
		bool found = (modPtr != 0 && !modPtr->castNotFoundModule());
		if(found) {
			TemporaryAssignment<ViewSharedPtr> v(mCurrentViewPtr);
			TemporaryAssignment<LibrarySharedPtr> l(mCurrentLibraryPtr);
			TemporaryAssignment<bool> b(mImportLibraryCells, true);
			modPtr->accept(*this, userPtr);
		}
		// look up the master view (or create/infer it if it doesn't exist)
		ViewSharedPtr masterViewPtr = findMasterView(nodePtr->modName(), modPtr->prettyName(), 
			!found);
		// note that this cell has been instantiated
		mCellInstantiationFlag[masterViewPtr->getParent()] = true;
		// create and add the instance
		mCurrentInstancePtr = mObjectFactoryPtr->newSingleInstancePtr(nodePtr->name(), 
			mCurrentViewPtr, masterViewPtr);
		if(nodePtr->prettyName() != nodePtr->name()) 
			mCurrentInstancePtr->setOriginalName(nodePtr->prettyName());
		// visit our children
		nodePtr->iterateChildren(*this);
	}

	/// \brief Visit a Verilog pin, and add an endpoint to the corresponding EDIF net.
	void VerilogImporterVisitor::visit(AstPin* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstPin" << std::endl;
		(void) userPtr;
		string pinName = nodePtr->name();
		if(pinName.substr(0, 11) == "__pinNumber") {
			std::cerr << "Warning: Instance port binding by port order is not currently supported." 
				<< std::endl;
			return;
		}

//std::cerr << "    PIN: " << mCurrentCellPtr->getName() << " " << mCurrentInstancePtr->getName() << "." << pinName << std::endl;

		//std::cerr << "    pin " << pinName << std::endl;
		// visit our children
		mCurrentIndices.clear(); 
		mCurrentNetPtrVector.clear(); mCurrentNetPtr.reset();
		mCurrentText = ""; mCurrentConstStr = ""; mCurrentConstInt = cUndefined; 
		mCurrentConstNum.setZero(); mCurrentNotFlag = false; mCurrentConcatFlag = false;
		nodePtr->iterateChildren(*this);

		// if the binding was a concatenation, we will have a vector of scalar nets or of vector 
		// bit nets; otherwise we may have a scalar net, a vector net, or a vector bit net; 
		// if we have no net at all, the port is open

		// a non-empty net vector takes precedence over a regular net
		size_t netSize = 0;
		bool isConcat = false;
		bool isVector = false;
		if(mCurrentNetPtrVector.size()) {
			// this is a vector of individual nets
			netSize = mCurrentNetPtrVector.size();
			isConcat = true;
			isVector = true;
		} else if(mCurrentNetPtr) {
			// this is an individual net
			netSize = mCurrentNetPtr->getSize();
			isConcat = false;
			isVector = mCurrentNetPtr->getCompositionType() == eCompositionTypeVector;
		} else {
			// the port must be open - we return without doing anything more
			return;
		}
//std::cerr << "     netSize = " << netSize << ", isConcat = " << isConcat << ", isVector = " << isVector << std::endl;

		// try to look up the port reference
		PortReferenceSharedPtr portReferencePtr = mCurrentInstancePtr->findPortReference(pinName);
//if(portReferencePtr) std::cerr << "FOUND PORTREF " << portReferencePtr << " FOR INSTANCE " << mCurrentInstancePtr->getName() << " PIN " << pinName << std::endl;
		// if this instance comes from an inferred library cell, we create any ports that are 
		// undefined -- note that we have no information concerning port directions
		if(!portReferencePtr) {
			// look up the master view and library
			ViewSharedPtr viewPtr = mCurrentInstancePtr->getMaster();
			LibrarySharedPtr libraryPtr = viewPtr->getParent()->getParent();
//std::cerr << "        inferring ports for master cell " << viewPtr->getParent()->getName() << std::endl;
			// may need to infer new ports if this instance originates from an inferred library
			bool infer = libraryPtr == mInferredBlackBoxesLibraryPtr;
			// handle vector ports
			if(isVector) {
				// look up the port
				PortSharedPtr portPtr = viewPtr->findPort(pinName);
				if(!portPtr) {
					// optionally add a vector port to the master of the inferred instance
					if(infer) {
						portPtr = mObjectFactoryPtr->newVectorPortPtr(pinName, 
							ePortDirectionUndefined, viewPtr, netSize, PortBundleSharedPtr(), 
							mObjectFactoryPtr, "");
						if(nodePtr->name() != nodePtr->prettyName()) 
							portPtr->setOriginalName(nodePtr->prettyName());
					} else ; /// \todo Throw an exception.
				}
				// create a new port reference
				portReferencePtr = mObjectFactoryPtr->newVectorPortReferencePtr(
					mCurrentInstancePtr, portPtr, netSize, 
					PortBundleReferenceSharedPtr(), mObjectFactoryPtr);
			// handle scalar ports or single bits of vector ports
			} else {
				// look up the port
				PortSharedPtr portPtr = viewPtr->findPort(pinName);
				if(!portPtr) {
					// optionally add a scalar port to the master of the inferred instance
					if(infer) {
						portPtr = mObjectFactoryPtr->newScalarPortPtr(pinName, 
							ePortDirectionUndefined, viewPtr, PortBundleSharedPtr(), "");
						if(nodePtr->name() != nodePtr->prettyName()) 
							portPtr->setOriginalName(nodePtr->prettyName());
					} else ; /// \todo Throw an exception.
				}
				// create a new port reference
				portReferencePtr = mObjectFactoryPtr->newScalarPortReferencePtr(
					mCurrentInstancePtr, portPtr, PortBundleReferenceSharedPtr());
			}
//std::cerr << "        done inferring ports for master cell." << std::endl;
		}
//PortSharedPtr backPortPtr = viewPtr->findPort(pinName);
//std::cerr << "FOUND PORT " << backPortPtr << " FOR VIEW " << viewPtr->getParent()->getName() << " PIN " << pinName << std::endl;
//PortReferenceSharedPtr backPortReferencePtr = mCurrentInstancePtr->findPortReference(pinName);
//std::cerr << "FOUND PORTREF " << backPortReferencePtr << " FOR INSTANCE " << mCurrentInstancePtr->getName() << " PIN " << pinName << std::endl;
		// we should have a valid port reference by now
		if(!portReferencePtr) {
			std::cerr << "Warning: Unable to find or infer port " << pinName 
				<< " for instance " << mCurrentInstancePtr->getName() << std::endl;
			throw;
		}

		// the AstParseRef code should ensure that we never get here with multidimensional nets

		// determine the port size
		size_t portSize = portReferencePtr->getSize();
		// check for mismatched sizes
		if(portSize != netSize) {
			std::cerr << "Warning: bundle and pin sizes do not match on " 
				<< mCurrentInstancePtr->getName() << "." << pinName << std::endl;
		}

		// connect concatenations
		if(isConcat) {
			// gather the children
			NetSharedPtrVector::const_iterator p = mCurrentNetPtrVector.begin();
			NetSharedPtrVector::const_iterator e = mCurrentNetPtrVector.end();
			IndexVector portIndices;
			portIndices.push_back(0);
			while(p < e) {
				// look up the current net
				NetSharedPtr netPtr = *p++;
				CompositionType compositionType = netPtr->getCompositionType();
				if(compositionType != eCompositionTypeScalar 
					&& compositionType != eCompositionTypeVectorBit) {
					std::cerr << "Warning: unexpected vector net or bundle remaining in "
						"concatenation for " << mCurrentInstancePtr->getName() << "." << pinName 
						<< std::endl;
					continue;
				}
				// connect the net to the appropriate port member
				PortReferenceSharedPtr portBitReferencePtr = portReferencePtr->get(portIndices);
//std::cerr << "            connecting port " << portReferencePtr->getName() << " index " 
//	<< portIndices[0] << " of type " << compositionTypeString(portReferencePtr)
//	<< " to scalar net " << netPtr->getName() << " of type " << compositionTypeString(netPtr) 
//	<< std::endl;
				portBitReferencePtr->connect(netPtr);
				portIndices[0]++;
			}
		// connect individual nets
		} else {
//std::cerr << "            connecting port " << mCurrentInstancePtr->getName() << "." << pinName << " of type " << compositionTypeString(portReferencePtr)
//	<< " to net " << mCurrentNetPtr->getName() << " of type " << compositionTypeString(mCurrentNetPtr) 
//	<< std::endl;
			portReferencePtr->connect(mCurrentNetPtr);
		}

	}

	/// \brief Visit a Verilog port, and create an EDIF port.
	void VerilogImporterVisitor::visit(AstPort* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstPort" << std::endl;
		(void) userPtr;
//		std::cerr << "    port " << nodePtr->name() << std::endl;
	}

	/// \brief Visit a Verilog bit index.
	void VerilogImporterVisitor::visit(AstSelBit* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstSelBit" << std::endl;
		(void) userPtr;
		// look up the bit index, and process it if it is a constant
		AstNode* bitp = nodePtr->bitp();
		switch(bitp->type()) {
		case AstType::atCONST: 
			mCurrentIndices.push_back(bitp->castConst()->num().toSInt()); break;
		default: 
			std::cerr << "FAILED TO PROCESS SELBIT OF TYPE " << bitp->type().ascii(); throw;
		}
		// visit our children
		nodePtr->iterateChildren(*this);
	}

	void VerilogImporterVisitor::visit(AstSelExtract* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstSelExtract" << std::endl;
		(void) userPtr;
		// visit our children
		nodePtr->iterateChildren(*this);
		// look up the bit range, and process it if it consists of two constants
		AstNode* msbp = nodePtr->msbp();
		AstNode* lsbp = nodePtr->lsbp();
		if(msbp->type() == AstType::atCONST && lsbp->type() == AstType::atCONST) {
			mCurrentRange[0] = msbp->castConst()->num().toSInt();
			mCurrentRange[1] = lsbp->castConst()->num().toSInt();
		} else {
			std::cerr << "FAILED TO PROCESS SELEXTRACT OF TYPES " << msbp->type().ascii() 
				<< " and " << lsbp->type().ascii() << std::endl;
//			throw;
		}
//std::cerr << "        leaving AstSelExtract: " << mCurrentText << "[" << mCurrentRange[0] << ":" << mCurrentRange[1] << "]" << std::endl;
	}

	/// \brief Visit a Verilog range.
	void VerilogImporterVisitor::visit(AstRange* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstRange" << std::endl;
		(void) userPtr;
		mCurrentRange[0] = nodePtr->msbConst();
		mCurrentRange[1] = nodePtr->lsbConst();
		// no need to visit our children
		//nodePtr->iterateChildren(*this);
	}

	void VerilogImporterVisitor::visit(AstDefParam* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstDefParam" << std::endl;
		(void) userPtr;
//		std::cerr << "    defParam " << nodePtr->path() << ": " << nodePtr->name() 
//			<< " = " << nodePtr->rhsp()->name() << std::endl;
		// look up the relevant instance
		InstanceSharedPtr instancePtr = mCurrentViewPtr->findInstance(nodePtr->path());
		if(!instancePtr) {
			std::cerr << "FAILED TO FIND DEFPARAM INSTANCE " << nodePtr->path() << std::endl;
			return;
		}
		// visit our children
		mCurrentText = ""; mCurrentConstStr = ""; mCurrentConstInt = cUndefined; 
			mCurrentConstNum.setZero(); mCurrentNotFlag = false;
		nodePtr->iterateChildren(*this);
		if(!mCurrentConstStr.length()) {
			std::cerr << "FAILED TO FIND A CONSTANT FOR DEFPARAM " << nodePtr->path() << std::endl;
			return;
		}
		// convert the defparam into a property
		if(mCurrentConstNum.isFromString()) {
			// handle numeric defparams
			mObjectFactoryPtr->newPropertyPtr(nodePtr->name(), instancePtr, 
				Value(Value::eValueTypeString, Value::String(mCurrentConstNum.toString())));
		} else {
			// handle string defparams
			mObjectFactoryPtr->newPropertyPtr(nodePtr->name(), instancePtr, 
				Value(Value::eValueTypeString, Value::String(mCurrentConstStr)));
		}
	}

	/// \brief Visit a Verilog variable, and create an EDIF wire or port.
	void VerilogImporterVisitor::visit(AstVar* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstVar" << std::endl;
// WARNING: the next line needs to be removed!
//return;
		(void) userPtr;
		// look up the variable type
		AstVarType varType = nodePtr->varType();
		// process wires
		if(varType == AstVarType::WIRE) {
			// determine the port width
			mCurrentRange[0] = mCurrentRange[1] = cUndefined;
			// visit our children
			nodePtr->iterateChildren(*this);
			// create and add a scalar net
			if(mCurrentRange[0] == cUndefined && mCurrentRange[1] == cUndefined) {
//				std::cerr << "    wire " << nodePtr->name() << std::endl;
				ScalarNetSharedPtr netPtr = mObjectFactoryPtr->newScalarNetPtr(nodePtr->name(), 
					mCurrentViewPtr, NetBundleSharedPtr(), "");
				if(nodePtr->prettyName() != nodePtr->name()) 
					netPtr->setOriginalName(nodePtr->prettyName());
			}
			// create and add a vector net
			else {
//				std::cerr << "    wire " << nodePtr->name() << "[" << mCurrentRange[0] << ":" 
//					<< mCurrentRange[1] << "]" << std::endl;
				VectorNetSharedPtr netPtr = mObjectFactoryPtr->newVectorNetPtr(nodePtr->name(), 
					mCurrentViewPtr, mCurrentRange[0] + 1, NetBundleSharedPtr(), 
					mObjectFactoryPtr, "");
				if(nodePtr->prettyName() != nodePtr->name()) 
					netPtr->setOriginalName(nodePtr->prettyName());
				// attach the original port range in two properties
				mObjectFactoryPtr->newPropertyPtr(VerilogNames::getTorcRangeMSBPropertyName(), 
					netPtr, Value(Value::eValueTypeInteger, Value::Integer(mCurrentRange[0])));
				mObjectFactoryPtr->newPropertyPtr(VerilogNames::getTorcRangeLSBPropertyName(), 
					netPtr, Value(Value::eValueTypeInteger, Value::Integer(mCurrentRange[1])));
			}
		// process ports
		} else if(varType == AstVarType::PORT || varType == AstVarType::INPUT 
			|| varType == AstVarType::OUTPUT || varType == AstVarType::INOUT) {
			// determine the direction
			EPortDirection direction = ePortDirectionUndefined;
			if(nodePtr->isInout()) { direction = ePortDirectionInOut; }
			else if(nodePtr->isInput()) { direction = ePortDirectionIn; }
			else if(nodePtr->isOutput()) { direction = ePortDirectionOut; }
			// determine the port width
			mCurrentRange[0] = mCurrentRange[1] = cUndefined;
			// visit our children
			nodePtr->iterateChildren(*this);
			// create and add a scalar port
			if(mCurrentRange[0] == cUndefined && mCurrentRange[1] == cUndefined) {
//				std::cerr << "    port " << nodePtr->name() << std::endl;
				ScalarPortSharedPtr portPtr = mObjectFactoryPtr->newScalarPortPtr(
					nodePtr->name(), direction, mCurrentViewPtr, PortBundleSharedPtr(), "");
				if(nodePtr->prettyName() != nodePtr->name()) 
					portPtr->setOriginalName(nodePtr->prettyName());
			}
			// create and add a vector port
			else {
//				std::cerr << "    port " << nodePtr->name() << "[" << mCurrentRange[0] << ":" 
//					<< mCurrentRange[1] << "]" << std::endl;
				VectorPortSharedPtr portPtr = mObjectFactoryPtr->newVectorPortPtr(
					nodePtr->name(), direction, mCurrentViewPtr, mCurrentRange[0] + 1, 
					PortBundleSharedPtr(), mObjectFactoryPtr, "");
				//stringstream ss;
				//ss << nodePtr->name() << "[" << mCurrentRange[0] << ":" << mCurrentRange[1] 
				//	<< "]";
				//portPtr->setOriginalName(ss.str());
				if(nodePtr->prettyName() != nodePtr->name()) 
					portPtr->setOriginalName(nodePtr->prettyName());
				// attach the original port range in two properties
				mObjectFactoryPtr->newPropertyPtr(VerilogNames::getTorcRangeMSBPropertyName(), 
					portPtr, Value(Value::eValueTypeInteger, Value::Integer(mCurrentRange[0])));
				mObjectFactoryPtr->newPropertyPtr(VerilogNames::getTorcRangeLSBPropertyName(), 
					portPtr, Value(Value::eValueTypeInteger, Value::Integer(mCurrentRange[1])));
			}
		// catch everything else
		} else {
			visit(dynamic_cast<AstNode*>(nodePtr), userPtr);
		}
	}

	/// \brief Visit a Verilog assignment, and add an appropriate EDIF property.
	void VerilogImporterVisitor::visit(AstAssignW* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstAssignW" << std::endl;
		(void) userPtr;
		// visit our left-hand side
		mCurrentText = ""; mCurrentConstStr = ""; mCurrentConstInt = cUndefined; 
			mCurrentConstNum.setZero(); mCurrentNotFlag = false;
		nodePtr->lhsp()->iterateAndNext(*this, userPtr);
		string signal = mCurrentText;
		// visit our right-hand side
		mCurrentText = ""; mCurrentConstStr = ""; mCurrentConstInt = cUndefined; 
			mCurrentConstNum.setZero(); mCurrentNotFlag = false;
		nodePtr->rhsp()->iterateAndNext(*this, userPtr);
		string value = (mCurrentNotFlag ? "~" : "") 
			+ (mCurrentConstStr.length() ? mCurrentConstStr : mCurrentText);
//nodePtr->rhsp()->dumpTree();
		// try to look up the left-hand side as a net or port
		PortSharedPtr portPtr;
		NetSharedPtr netPtr;
		portPtr = mCurrentViewPtr->findPort(signal);
		if(!portPtr) netPtr = mCurrentViewPtr->findNet(signal);
		if(portPtr) {
			mObjectFactoryPtr->newPropertyPtr(VerilogNames::getTorcAssignRHSPropertyName(), portPtr, 
				Value(Value::eValueTypeString, Value::String(value)));
//			std::cerr << "    assign " << signal << " = " << value << std::endl;
		} else if(netPtr) {
			mObjectFactoryPtr->newPropertyPtr(VerilogNames::getTorcAssignRHSPropertyName(), netPtr, 
				Value(Value::eValueTypeString, Value::String(value)));
//			std::cerr << "    assign " << signal << " = " << value << std::endl;
		} else {
			std::cerr << "FAILED TO FIND PORT OR NET ASSIGNMENT TARGET FOR " << signal << std::endl;
		}
	}

	void VerilogImporterVisitor::visit(AstParseRef* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstParseRef" << std::endl;
		(void) userPtr;
//		std::cerr << "    parseref " << nodePtr->name() << ": " 
//			<< nodePtr->lhsp()->type().ascii() << std::endl;

		// visit our children
		mCurrentIndices.clear();
		mCurrentRange[0] = mCurrentRange[1] = cUndefined;
		mCurrentText = ""; mCurrentConstStr = ""; mCurrentConstInt = cUndefined; 
		mCurrentConstNum.setZero(); mCurrentNotFlag = false;
		nodePtr->iterateChildren(*this);

		// try to look up the current net based on its name
		mCurrentNetPtr = mCurrentViewPtr->findNet(mCurrentText);
		size_t netSize = mCurrentNetPtr ? mCurrentNetPtr->getSize() : 1;

		// account for nets that are bits of busses
		string netName = mCurrentText;
		string indexedNetName;
		size_t numIndices = mCurrentIndices.size();
		if(numIndices > 1) {
			std::cerr << "Warning: Multidimensional arrays and ports are not yet supported." 
				<< std::endl;
			// to-do: throw an exception
			return;
		}
		// a scalar bit of a bus
		if(numIndices) mCurrentRange[1] = mCurrentRange[0] = mCurrentIndices[0];
		// a slice of a bus
		bool hasRange = mCurrentRange[0] != cUndefined && mCurrentRange[1] != cUndefined;

		// if the net does not exist, try to derive it from an identically named port
		if(!mCurrentNetPtr) {
			PortSharedPtr portPtr = mCurrentViewPtr->findPort(mCurrentText);
			if(portPtr) {
				// look up the port width
				netSize = portPtr->getSize();
				// entire vector ports inside concatenations must be treated as ranges
				if(mCurrentConcatFlag && !hasRange 
					&& portPtr->getCompositionType() == eCompositionTypeVector) {
					hasRange = true;
					PropertySharedPtr msbPtr 
						= portPtr->getProperty(VerilogNames::getTorcRangeMSBPropertyName());
					PropertySharedPtr lsbPtr 
						= portPtr->getProperty(VerilogNames::getTorcRangeLSBPropertyName());
					mCurrentRange[0] = msbPtr->getValue().get<Value::Integer>();
					mCurrentRange[1] = lsbPtr->getValue().get<Value::Integer>();
				}
				// create a scalar net
				if(netSize == 1) {
					mCurrentNetPtr = mObjectFactoryPtr->newScalarNetPtr(netName, mCurrentViewPtr, 
						NetBundleSharedPtr(), "");
					if(mCurrentConcatFlag) mCurrentNetPtrVector.push_back(mCurrentNetPtr);
					portPtr->connect(mCurrentNetPtr);
				// create scalar nets for an index or a slice
				} else if(hasRange) {
					// look up the port range
					PropertySharedPtr msbPtr 
						= portPtr->getProperty(VerilogNames::getTorcRangeMSBPropertyName());
					PropertySharedPtr lsbPtr 
						= portPtr->getProperty(VerilogNames::getTorcRangeLSBPropertyName());
					int32_t msb = msbPtr->getValue().get<Value::Integer>();
					int32_t lsb = lsbPtr->getValue().get<Value::Integer>();
					// iterate over the range
//std::cerr << "    creating port bit for " << mCurrentText << "[" << mCurrentRange[0] << ":" 
//	<< mCurrentRange[1] << "]" << std::endl;
					int32_t increment = mCurrentRange[0] < mCurrentRange[1] ? 1 : -1;
					for(int32_t i = mCurrentRange[0]; ; i += increment) {
						// look up the appropriate port index (does not support multiple dimensions)
						int32_t formalIndex = i;
						int32_t index = 0;
						if(formalIndex <= msb && formalIndex >= lsb) {
							index = msb - formalIndex;
						} else if(formalIndex >= msb && formalIndex <= lsb) {
							index = formalIndex - msb;
						} else {
							// this case would be a bit out of range
						}
						// compose the net name
						stringstream un;
						stringstream ss;
						un << mCurrentText << "_" << formalIndex << "_" << mCurrentIndex++;
						ss << mCurrentText << "[" << formalIndex << "]";
						netName = un.str();
						indexedNetName = ss.str();
						// create a temporary net
						StringToNetMap::iterator it = mVectorBitNameToNet.find(indexedNetName);
//std::cerr << "    YOOHOO: got " << (it != mVectorBitNameToNet.end()) << " when looking for " << indexedNetName << std::endl;
						if(it != mVectorBitNameToNet.end()) { mCurrentNetPtr = it->second; }
						else {
							mCurrentNetPtr = mObjectFactoryPtr->newScalarNetPtr(netName, 
								mCurrentViewPtr, NetBundleSharedPtr(), "");
							// remember the original name
							mCurrentNetPtr->setOriginalName(indexedNetName);
							mVectorBitNameToNet[indexedNetName] = mCurrentNetPtr;
							IndexVector indices;
							indices.push_back(index);
							PortSharedPtr portBitPtr = portPtr->get(indices);
							portBitPtr->connect(mCurrentNetPtr);
						}
						if(mCurrentConcatFlag) mCurrentNetPtrVector.push_back(mCurrentNetPtr);
						// break out of the loop when we're done
						if(i == mCurrentRange[1]) break;
					}
				// create a vector net
				} else {
					// we should never get here if we are in a concatenation
					if(mCurrentConcatFlag) {
						std::cerr << "Warning: Encountered vector net within concatenation." 
							<< std::endl;
					}
					// create a new vector net
					mCurrentNetPtr = mObjectFactoryPtr->newVectorNetPtr(netName, mCurrentViewPtr, 
						netSize, NetBundleSharedPtr(), mObjectFactoryPtr, "");
					VectorNetSharedPtr vectorNetPtr 
						= boost::dynamic_pointer_cast<VectorNet>(mCurrentNetPtr);
					vectorNetPtr->forceAutoBlast();
					// set the port range as the net range (this can probably be cleaned up)
					PropertySharedPtr msbPtr 
						= portPtr->getProperty(VerilogNames::getTorcRangeMSBPropertyName());
					PropertySharedPtr lsbPtr 
						= portPtr->getProperty(VerilogNames::getTorcRangeLSBPropertyName());
					int32_t msb = msbPtr->getValue().get<Value::Integer>();
					int32_t lsb = lsbPtr->getValue().get<Value::Integer>();
					mObjectFactoryPtr->newPropertyPtr(VerilogNames::getTorcRangeMSBPropertyName(), 
						mCurrentNetPtr, Value(Value::eValueTypeInteger, Value::Integer(msb)));
					mObjectFactoryPtr->newPropertyPtr(VerilogNames::getTorcRangeLSBPropertyName(), 
						mCurrentNetPtr, Value(Value::eValueTypeInteger, Value::Integer(lsb)));
				}
			}
else std::cerr << "FAILED TO FIND PORT " << netName << std::endl;

		// the net already exists
		} else {
//printf("mCurrentConcatFlag = %d, hasRange = %d, compoositionType = %d\n", 
//	mCurrentConcatFlag, hasRange, mCurrentNetPtr->getCompositionType());
			// entire vector ports inside concatenations must be treated as ranges
			if(mCurrentConcatFlag && !hasRange 
				&& mCurrentNetPtr->getCompositionType() == eCompositionTypeVector) {
				hasRange = true;
				PropertySharedPtr msbPtr 
					= mCurrentNetPtr->getProperty(VerilogNames::getTorcRangeMSBPropertyName());
				PropertySharedPtr lsbPtr 
					= mCurrentNetPtr->getProperty(VerilogNames::getTorcRangeLSBPropertyName());
				mCurrentRange[0] = msbPtr->getValue().get<Value::Integer>();
				mCurrentRange[1] = lsbPtr->getValue().get<Value::Integer>();
			}
			// if a range or indices were supplied, we need to extract the relevant children and 
			// add them to the current vector
			if(hasRange) {
//std::cerr << "    hasRange mCurrentNetPtr = " << mCurrentNetPtr->getName() << std::endl;
				// look up the children
				VectorNet::List children;
				mCurrentNetPtr->getChildren(children);
				// look up the port range
				PropertySharedPtr msbPtr 
					= mCurrentNetPtr->getProperty(VerilogNames::getTorcRangeMSBPropertyName());
				PropertySharedPtr lsbPtr 
					= mCurrentNetPtr->getProperty(VerilogNames::getTorcRangeLSBPropertyName());
				int32_t msb = msbPtr->getValue().get<Value::Integer>();
				int32_t lsb = lsbPtr->getValue().get<Value::Integer>();
				// iterate over the range
				int32_t increment = mCurrentRange[0] < mCurrentRange[1] ? 1 : -1;
				for(int32_t i = mCurrentRange[0]; ; i += increment) {
					// look up the appropriate port index (does not support multiple dimensions)
					int32_t formalIndex = i;
					int32_t index = 0;
					if(formalIndex <= msb && formalIndex >= lsb) {
						index = msb - formalIndex;
					} else if(formalIndex >= msb && formalIndex <= lsb) {
						index = formalIndex - msb;
					} else {
						// this case would be a bit out of range
					}
					(void) index;
					// compose the net name
					stringstream un;
					stringstream ss;
					un << mCurrentText << "_" << formalIndex << "_" << mCurrentIndex++;
					ss << mCurrentText << "[" << formalIndex << "]";
					netName = un.str();
					indexedNetName = ss.str();
					// look up the appropriate children
					NetSharedPtr netPtr;
					StringToNetMap::iterator it = mVectorBitNameToNet.find(indexedNetName);
//std::cerr << "    YOOHOO: got " << (it != mVectorBitNameToNet.end()) << " when looking for " << indexedNetName << std::endl;
					if(it != mVectorBitNameToNet.end()) { netPtr = it->second; }
					else {
						//netPtr = children[index];
						netPtr = mObjectFactoryPtr->newScalarNetPtr(netName, 
							mCurrentViewPtr, NetBundleSharedPtr(), "");
//std::cerr << "    created net for " << netName << " of type " << compositionTypeString(netPtr) << std::endl;
						mCurrentViewPtr->removeNet(netName);
						mCurrentNetPtr->addSubnet(netPtr);
						// remember the original name
						netPtr->setOriginalName(indexedNetName);
						mVectorBitNameToNet[indexedNetName] = netPtr;
					}
					if(mCurrentConcatFlag) mCurrentNetPtrVector.push_back(netPtr);
					else mCurrentNetPtr = netPtr;
//std::cerr << "    mCurrentNetPtr is of type " << compositionTypeString(mCurrentNetPtr) 
//	<< " in view " << netPtr->getParent() << std::endl;
					// break out of the loop when we're done
					if(i == mCurrentRange[1]) break;
				}
				// the range has now been added to the current bundle

			// otherwise, we are using a full scalar or vector net, so we push it if applicable
			} else {
				// if the net has no children, auto-blast it
//std::cerr << "    no range mCurrentNetPtr = " << mCurrentNetPtr->getName() << std::endl;
				if(mCurrentNetPtr->getCompositionType() == eCompositionTypeVector) {
					VectorNet::List children;
					mCurrentNetPtr->getChildren(children);
					VectorNetSharedPtr vectorNetPtr = boost::dynamic_pointer_cast<VectorNet>(mCurrentNetPtr);
//					if(!children.size()) vectorNetPtr->forceAutoBlast();
				}
				// add the net to the current vector if approriate
				if(mCurrentConcatFlag) mCurrentNetPtrVector.push_back(mCurrentNetPtr);
			}
		}

	}

	/// \brief Visit a text node and store it.
	void VerilogImporterVisitor::visit(AstText* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstText" << std::endl;
		(void) userPtr;
		// store the current text
		mCurrentText = nodePtr->text();
//std::cerr << "        text: " << mCurrentText << std::endl;
		// we have no children to visit
		//nodePtr->iterateChildren(*this);
	}

	/// \brief Visit a constant node and store it.
	void VerilogImporterVisitor::visit(AstConst* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstConst" << std::endl;
		(void) userPtr;
		// store the current constant
		int widthMin = nodePtr->widthMin();
		mCurrentConstNum = nodePtr->num();
		mCurrentConstStr = mCurrentConstNum.ascii(false);
//std::cerr << "    number: " << nodePtr->name() << std::endl;
//std::cerr << "        AstConst 0: widthMin = " << mCurrentConstNum.widthMin() << std::endl;
//std::cerr << "        AstConst 1: width = " << mCurrentConstNum.width() << std::endl;
//std::cerr << "        AstConst 2: ascii = " << mCurrentConstStr << std::endl;
// WARNING: the next line needs to be removed!
//if(widthMin > 64) return;
		mCurrentConstInt = widthMin > 32 
			? cUndefined : nodePtr->num().toSInt();
		// we have no children to visit
		//nodePtr->iterateChildren(*this);
	}

	/// \brief Visit a NOT node and store it.
	void VerilogImporterVisitor::visit(AstNot* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstNot" << std::endl;
		(void) userPtr;
		// store the flag
		mCurrentNotFlag = true;
		// we have no children to visit
		nodePtr->iterateChildren(*this);
	}

	void VerilogImporterVisitor::visit(AstNotFoundModule* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstNotFoundModule" << std::endl;
		(void) userPtr;
//		std::cerr << "not found module " << nodePtr->name() << std::endl;
		std::cerr << "// NOTICE: black-boxed module " << nodePtr->name() << std::endl;
		// visit our children
		nodePtr->iterateChildren(*this);
	}

	void VerilogImporterVisitor::visit(AstBasicDType* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstBasicDType" << std::endl;
		(void) userPtr;
		// nodePtr->name() is typically "logic" -- I'm not sure that helps us out a lot
		if(nodePtr->name() != "logic") 
			std::cerr << "    UNPROCESSED: basicdtype " << nodePtr->name() << std::endl;
		// visit our children
		nodePtr->iterateChildren(*this);
	}

	void VerilogImporterVisitor::visit(AstConcat* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstConcat" << std::endl;
		(void) userPtr;
		// we are now in a concatenation
		mCurrentConcatFlag = true;
		// visit our children
		nodePtr->iterateChildren(*this);
	}

	void VerilogImporterVisitor::visit(AstNode* nodePtr, AstNUser* userPtr) {
if(DEBUG) std::cerr << "AstNode" << std::endl;
		(void) userPtr;
//		std::cerr << "    " << nodePtr->verilogKwd() << std::endl;
		std::cerr << "FAILED TO PROCESS NODE "; nodePtr->dump(std::cerr); std::cerr 
			<< " in line " << nodePtr->fileline() << std::endl;
		// visit our children
		nodePtr->iterateChildren(*this);
	}

} // namespace generic
} // namespace torc
