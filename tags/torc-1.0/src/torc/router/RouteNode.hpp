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
/// \brief Header for the RouteNode class.

#ifndef TORC_ROUTER_ROUTENODE_HPP
#define TORC_ROUTER_ROUTENODE_HPP

#include "torc/architecture/Arc.hpp"
#include "torc/common/EncapsulatedInteger.hpp"
#include <boost/cstdint.hpp>
#include <functional>

namespace torc {
namespace router {

	// Pack RouteTreeNode objects tightly.
	/// \todo Have to justify the packing decision, and its impact on memory footprint versus 
	///		performance.
	#ifdef __GNUC__
		#pragma pack(push, 2)
	#endif
	
	/// \brief An object that holds an arc and path information for routing.
	/// \details A RouteNode contains an arc in the device and includes a parent pointer to allow
	/// 	recovery of the actual path to configure following completion of routing a net or sink.
	class RouteNode {
	protected:
	// types
		/// \brief Imported type name.
		typedef architecture::Tilewire Tilewire;
		/// \brief Imported type name.
		typedef architecture::Arc Arc;
	protected:
	// members
		/// \brief Arc that this node describes.
		architecture::Arc mArc;
		/// \brief Cost associated with this arc.
		boost::int32_t mCost;
		/// \brief Path cost for this arc.
		boost::int32_t mPathCost;
		/// \brief Depth of node from the source.
		boost::int32_t mDepth;
		/// \brief Pointer to parent node.
		RouteNode* mParent;
	public:
	// constructors
		/// \brief Null Constructor.
		RouteNode() : mArc(), mCost(0), mPathCost(0), mDepth(-1), mParent(0) {}
		/// \brief Arc only constructor.
		RouteNode(Tilewire inSource, Tilewire inSink, boost::int32_t inDepth, RouteNode* inParent)
			: mArc(inSource, inSink), mCost(0), mPathCost(0), mDepth(inDepth), mParent(0) {}
		/// \brief Complete Tilewire based constructor.
		RouteNode(Tilewire inSource, Tilewire inSink, boost::int32_t inCost, 
			boost::int32_t inPathCost, boost::int32_t inDepth, RouteNode* inParent)
			: mArc(inSource, inSink), mCost(inCost), mPathCost(inPathCost), mDepth(inDepth), 
			mParent(inParent) {}
		/// \brief Public Constructor.
		RouteNode(Arc inArc, boost::int32_t inDepth, RouteNode* inParent)
			:mArc(inArc), mCost(0), mPathCost(0), mDepth(inDepth), mParent(inParent) {}
		/// \brief Public Constructor.
		RouteNode(Arc inArc, boost::int32_t inCost, boost::int32_t inPathCost, 
			boost::int32_t inDepth, RouteNode* inParent) : mArc(inArc), mCost(inCost), 
			mPathCost(inPathCost), mDepth(inDepth), mParent(inParent) {}
	// accessors
		/// \brief Get the associated Arc.
		const Arc& getArc() const { return mArc; }
		/// \brief Get the source Tilewire.
		const Tilewire& getSourceTilewire() const { return mArc.getSourceTilewire(); }
		/// \brief Get the sink Tilewire.
		const Tilewire& getSinkTilewire() const { return mArc.getSinkTilewire(); }
		/// \brief Get the heuristic node cost.
		const boost::int32_t getCost() const { return mCost; }
		/// \brief Set the heuristic node cost.
		void setCost(boost::int32_t inHeuristicCost) { mCost = inHeuristicCost; }
		/// \brief Get the path cost.
		const boost::int32_t getPathCost() const { return mPathCost; }
		/// \brief Set the path cost.
		void setPathCost(boost::int32_t inPathCost) { mPathCost = inPathCost; }
		/// \brief Get the node depth.
		const boost::int32_t getDepth() const { return mDepth; }
		/// \brief Set the node depth.
		void setDepth(boost::int32_t inDepth) { mDepth = inDepth; }
		/// \brief Get the node's parent.
		RouteNode* getParent() const { return mParent; }
		/// \brief Return the top node by tracing parent pointers.
		RouteNode* getTop() {
			RouteNode* top = this;
			while (top->mParent != 0) top = top->mParent;
			return top;
		}
		bool operator< (const RouteNode* rhs) const {
			return (mCost < rhs->mCost);
		}
	};
	
	#ifdef __GNUC__
		#pragma pack(pop)
	#endif
	
	/// \brief Vector of RouteNode pointers
	typedef std::vector<RouteNode*> RouteNodePtrVector;
	
	/// \brief Binary predicate for comparing RouteNode pointers based on cost.
	class RouteNodePtrCostCompare : std::binary_function<RouteNode*, RouteNode*, bool> {
	public:
		bool operator() (const RouteNode* a, const RouteNode* b) const {
			return (a->getCost() > b->getCost());
		}
	};
	
	/// \brief Binary predicate for checking equality of two RouteNode pointers.
	/// \details The sink tilewires are compared.
	//class RouteNodePtrSinkCompare : std::binary_function<RouteNode*, RouteNode*, bool> {
	//public:
	//	bool operator() (const RouteNode* a, const RouteNode* b) const {
	//		return a->getSinkTilewire() == b->getSinkTilewire();
	//	}

} // namespace router
} // namespace torce

#endif // TORC_ROUTER_ROUTENODE_HPP
