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

#ifndef TORC_GENERIC_EDIF_DECOMPILER_HPP
#define TORC_GENERIC_EDIF_DECOMPILER_HPP

#include <iostream>

#include "torc/generic/PointerTypes.hpp"
#include "torc/generic/Error.hpp"
#include "torc/generic/edif/ObjectVisitor.hpp"
#include "torc/generic/VisitorApplier.hpp"
#include "torc/generic/Derivation.hpp"

namespace torc {
namespace generic {

/**
 * This class generates an EDIF 2 0 0 file from an existing class hierarchy. It writes to a stream,
 * so this can be used to write to any stream including string streams.
 */
class Decompiler : public ObjectVisitor {
public:
	void visit(Root& inroot) throw (Error);

	void visit(Design& inDesign) throw (Error);

	void visit(Library& inlibrary) throw (Error);

	void visit(Cell& incell) throw (Error);

	void visit(View& inview) throw (Error);

	void visit(ScalarPort& inscalarPort) throw (Error);

	void visit(VectorPort& invectorPort) throw (Error);

	void visit(VectorPortBit& inportBit) throw (Error);

	void visit(PortBundle& inportBundle) throw (Error);

	void visit(ScalarNet& inscalarNet) throw (Error);

	void visit(VectorNet& invectorNet) throw (Error);

	void visit(VectorNetBit& innetBit) throw (Error);

	void visit(NetBundle& innetBundle) throw (Error);

	void visit(SingleInstance& insingleInstance) throw (Error);

	void visit(InstanceArray& ininstanceArray) throw (Error);

	void visit(InstanceArrayMember& ininstanceArray) throw (Error);

	void visit(ScalarPortReference& inScalarPortRef) throw (Error);

	void visit(VectorPortReference& inVectorPortRef) throw (Error);

	void visit(VectorPortBitReference& inVectorPortBitRef) throw (Error);

	void visit(PortBundleReference& inPortBundleRef) throw (Error);

	void visit(SingleParameter& insingleParameter) throw (Error);

	void visit(ParameterArray& inparamArray) throw (Error);

	void visit(ParameterArrayElement& inParamArrayElement) throw (Error);

	void visit(Property& inProperty) throw (Error);

	void visit(PortList& inPortList) throw (Error);

	void visit(PortListAlias& inPortListAlias) throw (Error);

	void visit(Status& inStatus) throw (Error);

	void visit(Permutable& inPermutable) throw (Error);

	void visit(InterfaceJoinedInfo& inInterfaceJoinedInfo) throw (Error);

	void visit(SimulationInfo& simuInfo) throw (Error);

	void visit(Simulate& simulate) throw (Error);

	void visit(Apply& apply) throw (Error);

	void visit(LogicalResponse& logicalResponse) throw (Error);

	void visit(LogicValue& logicVal) throw (Error);

	void visit(LogicElement& logicElement) throw (Error);

	void visit(WaveValue& waveValue) throw (Error);

	void visit(Timing& timing) throw (Error);

	void visit(Event& event) throw (Error);

	void visit(ForbiddenEvent& forbiddenevent) throw (Error);

	void printValueType(const Value& value) throw (Error);

	void printValue(const Value& value) throw (Error);

	void printUnit(const Unit unit) throw (Error);

	void printDerivation(const Derivation derivation) throw (Error);

	void printArray(const std::vector<size_t>& outVector, size_t depth,
		std::vector<ParameterSharedPtr>::iterator& itStart,
		std::vector<ParameterSharedPtr>::iterator& itEnd) throw (Error);

	void printPortAttributes(const PortAttributesSharedPtr& inAttrib) throw (Error);

	void printNetAttributes(const NetAttributesSharedPtr& inAttrib) throw (Error);

	void printViewInterfaceAttributes(const InterfaceAttributesSharedPtr& inAttrib) throw (Error);

	void printLogicValueAttributes(const LogicValueAttributesSharedPtr& inAttrib) throw (Error);

	void printUserData(std::list<std::string> inElements) throw (Error);

	void printComments(std::vector<std::string>& inElements) throw (Error);
	/**
	 * Convenience wrapper function to call decompile()
	 */
	void operator()() throw (Error);

	/**
	 * Decompile the tree.
	 *
	 * @param[in] root Root of the OM tree.
	 */
	void decompile() throw (Error);

	inline size_t getIndentation() const;

	void setIndentation(size_t inIndentation);

	Decompiler(const RootSharedPtr& inRoot, std::ostream& outStream = std::cout);

	Decompiler(const Decompiler& inSource);

	~Decompiler() throw ();

	Decompiler& operator=(const Decompiler& inSource);

private:
	RootSharedPtr mRoot;
	std::ostream& mOut;
	bool mIsJoiningContext;
	bool mIsPermutableContext;
	bool mIsExternContext;
	bool mParamAssignContext;
	bool mIsPortInstanceContext;
	size_t mIndentation;
	VisitorApplier<Decompiler> mApplier;
};

inline size_t Decompiler::getIndentation() const {
	return mIndentation;
}

} // namespace generic
} // namespace torc

#endif // TORC_GENERIC_EDIF_DECOMPILER_HPP
