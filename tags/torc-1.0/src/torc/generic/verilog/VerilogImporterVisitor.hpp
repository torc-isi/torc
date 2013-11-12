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
/// \brief Header for the VerilogImporterVisitor class.

#ifndef TORC_GENERIC_VERILOG_VERILOGIMPORTERVISITOR_HPP
#define TORC_GENERIC_VERILOG_VERILOGIMPORTERVISITOR_HPP

#include "torc/generic/Root.hpp"
#include "torc/generic/ObjectFactory.hpp"
#include "torc/generic/Net.hpp"
#include "torc/generic/verilog/VerilogNames.hpp"
#include "torc/externals/verilator/src/V3Global.h"
#include "torc/externals/verilator/src/V3Ast.h"
#include <boost/smart_ptr.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>

namespace torc {
namespace generic {

namespace generic { class VerilogImporterUnitTest; }

	/// \brief AST visitor to convert structural Verilog into a generic design.
	class VerilogImporterVisitor : public AstNVisitor {
	protected:
	// friends
		/// \brief The unit test class has access to our internals.
		friend class torc::generic::generic::VerilogImporterUnitTest;
	// typedefs
		/// \brief Imported type name.
		typedef std::string string;
		/// \brief A map from string to view shared pointer.
		typedef map<string, ViewSharedPtr> StringToViewMap;
		/// \brief A map from string to net shared pointer.
		typedef map<string, NetSharedPtr> StringToNetMap;
		/// \brief A map of cell shared pointers to booleans.
		typedef map<CellSharedPtr, bool> CellToBoolMap;
		/// \brief A vector of net shared pointers.
		typedef vector<NetSharedPtr> NetSharedPtrVector;
		/// \brief A vector of element indices.
		typedef vector<size_t> IndexVector;
	// constants
		/// \brief Undefined index constant.
		static const int32_t cUndefined = INT_MIN; // numeric_limits<boost::int32_t>::min();
	// members
		/// \brief The object factory shared pointer.
		ObjectFactorySharedPtr mObjectFactoryPtr;
		/// \brief The root shared pointer.
		RootSharedPtr mRootPtr;
		/// \brief The inferred black-box library shared pointer.
		LibrarySharedPtr mInferredBlackBoxesLibraryPtr;
		/// \brief The cell library shared pointer.
		LibrarySharedPtr mImportedCellLibraryPtr;
		/// \brief The current library shared pointer.
		LibrarySharedPtr mCurrentLibraryPtr;
		/// \brief The current cell shared pointer.
		CellSharedPtr mCurrentCellPtr;
		/// \brief The current view shared pointer.
		ViewSharedPtr mCurrentViewPtr;
		/// \brief The current instance shared pointer.
		InstanceSharedPtr mCurrentInstancePtr;
		/// \brief The current net shared pointer;
		NetSharedPtr mCurrentNetPtr;
		/// \brief The current net vector.
		NetSharedPtrVector mCurrentNetPtrVector;
		/// \brief The current constant string.
		string mCurrentConstStr;
		/// \brief The current constant signed integer.
		int32_t mCurrentConstInt;
		/// \brief The current constant number object.
		V3Number mCurrentConstNum;
		/// \brief The current text string.
		string mCurrentText;
		/// \brief The current inversion flag.
		bool mCurrentNotFlag;
		/// \brief The current concatenation flag.
		bool mCurrentConcatFlag;
		/// \brief The current range.
		int32_t mCurrentRange[2];
		/// \brief The current array indices.
		IndexVector mCurrentIndices;
		/// \brief A map from module name to master view pointer.
		StringToViewMap mMasterNameToView;
		/// \brief A unique index to avoid name collisions.
		uint32_t mCurrentIndex;
		/// \brief A map of net bit names to net shared pointers.
		StringToNetMap mVectorBitNameToNet;
		/// \brief A map of cell shared pointers to instantiation settings.
		CellToBoolMap mCellInstantiationFlag;
		/// \brief A flag to indicate whether we should currently allow library cells
		bool mImportLibraryCells;
	// statics
		static FileLine sNullFileLine;
	// functions
		virtual void visit(AstNetlist* nodePtr, AstNUser* userPtr);
		virtual void visit(AstModule* nodePtr, AstNUser* userPtr);
		virtual void visit(AstCell* nodePtr, AstNUser* userPtr);
		virtual void visit(AstPin* nodePtr, AstNUser* userPtr);
		virtual void visit(AstPort* nodePtr, AstNUser* userPtr);
		virtual void visit(AstSelBit* nodePtr, AstNUser* userPtr);
		virtual void visit(AstSelExtract* nodePtr, AstNUser* userPtr);
		virtual void visit(AstRange* nodePtr, AstNUser* userPtr);
		virtual void visit(AstDefParam* nodePtr, AstNUser* userPtr);
		virtual void visit(AstVar* nodePtr, AstNUser* userPtr);
		virtual void visit(AstAssignW* nodePtr, AstNUser* userPtr);
		virtual void visit(AstParseRef* nodePtr, AstNUser* userPtr);
		virtual void visit(AstText* nodePtr, AstNUser* userPtr);
		virtual void visit(AstConst* nodePtr, AstNUser* userPtr);
		virtual void visit(AstNot* nodePtr, AstNUser* userPtr);
		virtual void visit(AstNotFoundModule* nodePtr, AstNUser* userPtr);
		virtual void visit(AstBasicDType* nodePtr, AstNUser* userPtr);
		virtual void visit(AstConcat* nodePtr, AstNUser* userPtr);
		virtual void visit(AstNode* nodePtr, AstNUser* userPtr);
		/// \brief Find the specified view, or optionally create it.
		ViewSharedPtr findMasterView(std::string inMasterName, std::string inOriginalName, 
			bool inCreate) {
			// keep track of what we have or haven't found
			ViewSharedPtr masterViewPtr;
			// search our map for the module of interest
			StringToViewMap::iterator e = mMasterNameToView.end();
			StringToViewMap::iterator p = mMasterNameToView.find(inMasterName);
			if(p != e) masterViewPtr = p->second;
			// if we didn't find a master, and the caller requested it, create it now
			if(!masterViewPtr && inCreate) {
				// create and add the inferred cell
				CellSharedPtr cellPtr = mObjectFactoryPtr->newCellPtr(inMasterName, 
					mInferredBlackBoxesLibraryPtr, Cell::eTypeGeneric, "");
				if(inMasterName != inOriginalName && !inOriginalName.empty()) 
					cellPtr->setOriginalName(inOriginalName);
				// create and add the inferred view
				masterViewPtr = mObjectFactoryPtr->newViewPtr(
					VerilogNames::getImportedVerilogViewName(), cellPtr, View::eTypeNetlist, "");
				// map the master name to this view
				mMasterNameToView[inMasterName] = masterViewPtr;
			}
			// return the view that we found or created
			return masterViewPtr;
		}
		/// \brief Create top-level designs for all Verilog modules that are never instantiated.
		void createDesigns(void);
	public:
	// constructors
		/// \brief Public constructor.
		/// \param inRootPtr A torc::generic Root shared pointer to populate.
		VerilogImporterVisitor(ObjectFactorySharedPtr inObjectFactoryPtr, RootSharedPtr inRootPtr) 
			: mObjectFactoryPtr(inObjectFactoryPtr), mRootPtr(inRootPtr), 
			mCurrentConstNum(&sNullFileLine), mImportLibraryCells(false) {
			// initialize what remains
			mCurrentConstInt = cUndefined;
			mCurrentNotFlag = false;
			mCurrentConcatFlag = false;
			mCurrentRange[0] = mCurrentRange[1] = cUndefined;
			mCurrentIndex = 1000000;
			// create a new library for inferred black boxes
			mInferredBlackBoxesLibraryPtr = mObjectFactoryPtr->newLibraryPtr(
				VerilogNames::getInferredBlackBoxesLibraryName(), mRootPtr, eEdifLevel0, "");
			// create a new library for imported library cells
			mImportedCellLibraryPtr = mObjectFactoryPtr->newLibraryPtr(
				VerilogNames::getImportedCellLibraryName(), mRootPtr, eEdifLevel0, "");
			// create a new library for use
			mCurrentLibraryPtr = mObjectFactoryPtr->newLibraryPtr(
				VerilogNames::getImportedVerilogLibraryName(), mRootPtr, eEdifLevel0, "");
		}
	};

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_VERILOG_VERILOGIMPORTERVISITOR_HPP
