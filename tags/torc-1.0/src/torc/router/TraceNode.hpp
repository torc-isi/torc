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
/// \brief Header for the TraceNode class.

#ifndef TORC_ROUTER_TRACENODE_HPP
#define TORC_ROUTER_TRACENODE_HPP

#include "torc/architecture/Arc.hpp"
#include "torc/common/EncapsulatedInteger.hpp"
#include <boost/cstdint.hpp>
#include <functional>
#include <vector>
#include <list>
#include <set>
#include <iostream>

namespace torc {
namespace router {

	// Pack TraceNode objects tightly.
	/// \todo Have to justify the packing decision, and its impact on memory footprint versus 
	///		performance.
	#ifdef __GNUC__
		#pragma pack(push, 2)
	#endif
	
	/// \brief An object that holds more complete path information for routing and tracing.
	/// \details A TraceNode contains children pointers to allow tracing of arcs through a
	/// 	device to recover complete nets from the device usage information.
	class TraceNode {
	// friends
		/// \brief The TraceNode allows access to protected functions from Trace objects.
		friend class Trace;
	// types
		/// \brief Imported type name.
		typedef architecture::Tilewire Tilewire;
		/// \brief Imported type name.
		typedef architecture::Arc Arc;
		/// \brief Imported type name.
		typedef architecture::TilewireVector TilewireVector;
	public:
		/// \brief Vector of TraceNode pointers.
		typedef std::vector<TraceNode*> TraceNodePtrVector;
		/// \brief List of TraceNode pointers.
		typedef std::list<TraceNode*> TraceNodePtrList;
		/// \brief Pair consisting of a Tilewire and TraceNode pointer.
		typedef std::pair<Tilewire, TraceNode*> TilewireTraceNodePtrPair;
		/// \brief Iterator for vector of TraceNode pointers.
		typedef TraceNodePtrVector::iterator TraceNodePtrVectorIterator;
		/// \brief Pair consiting of an Arc and a TraceNode pointer.
		typedef std::pair<Arc, TraceNode*> ArcTraceNodePtrPair;
		/// \brief Vector of pairs of source Tilewires and TraceNode pointers.
		typedef std::vector<TilewireTraceNodePtrPair> TilewireTraceNodePtrPairVector;
		/// \brief Vector of paires of Arcs and TraceNode pointers.
		typedef std::vector<ArcTraceNodePtrPair> ArcTraceNodePtrPairVector;
	protected:
	// members
		// /// \brief Tilewire of this node.
		Tilewire mTilewire;
		/// \brief TilewireVector representing this node.
		//TilewireVector mSegment;
		/// \brief Vector of child pointers.
		TraceNodePtrVector mChildren;
		/// \brief Vector of parent pointers.
		//TilewireTraceNodePtrPairVector mParents;
		TraceNodePtrVector mParents;
		/// \brief Depth from furthest parent with no parent.
		boost::int32_t mDepth;
	public:
		/// \brief Static allocation and deallocation count
		static boost::int32_t sLiveNodes;
	public:
	// constructors
		/// \brief Null Constructor.
		//TraceNode() : mTilewire(Tilewire::sInvalid) , mDepth(-1) {
		TraceNode() : mDepth(-1) {
			sLiveNodes++;
		}
		/// \brief Public Constructor.
		TraceNode(Tilewire inTilewire) : mTilewire(inTilewire), mDepth(-1) {
			sLiveNodes++;
		}
		/// \brief Destructor.
		/// \details Recursively deletes all connected nodes.
		~TraceNode() {
			//std::cout << "DESTRUCT! " << mTilewire.getWireIndex() << "@" 
			//	<< mTilewire.getTileIndex() << std::endl;
			/*for (unsigned int i = 0; i < mChildren.size(); i++) {
				for (unsigned int j = 0; j < mChildren[i]->getNumParents(); j++) {
					if (mChildren[i]->getParent(j) == this) {
						mChildren[i]->removeParent(j);
						break;
					}
				}
				delete mChildren[i];
				mChildren[i] = 0;
			}
			mChildren.clear();
			for (unsigned int i = 0; i < mParents.size(); i++) {
				if (mParents[i]->getNumChildren() > 0) {
					for (unsigned int j = 0; j < mParents[i]->getNumChildren(); j++) {
						if (mParents[i]->getChild(j) == this) {
							mParents[i]->removeChild(j);
							break;
						}
					}
				}
				delete mParents[i];
				mParents[i] = 0;
			}
			mParents.clear();*/
			sLiveNodes--;
		}
	// accessors
		/// \brief Get the Tilewire associated with this node.
		inline Tilewire getTilewire() { return mTilewire; }
		//TilewireVector& getSegment() { return mSegment; }
		/// \brief Get the depth of this node from the furthest node with no parent.
		inline boost::int32_t getDepth() const { return mDepth; }
		/// \brief Set the depth of this node.
		inline void setDepth(boost::int32_t inDepth) { mDepth = inDepth; }
		/// \brief Add children to the node.
		void addChildren(const TraceNodePtrVector& newChildren) {
			boost::uint32_t size = newChildren.size();
			if (size == 0) return;
			mChildren.reserve(mChildren.size() + size);
			// insert children into the node
			mChildren.insert(mChildren.end(), newChildren.begin(), newChildren.end());
		}
		/// brief Add child to the node.
		void addChild(TraceNode* newChild) {
			if (newChild == 0) return;
			mChildren.push_back(newChild);
		}
		/// \brief Add parent to the node.
		void addParent(TraceNode* newParent) {
			mParents.push_back(newParent);
		}
		/// \brief Get the number of children.
		boost::uint32_t getNumChildren() {
			return mChildren.size();
		}
		/// \brief Get the number of parents.
		boost::uint32_t getNumParents() {
			return mParents.size();
		}
		/// \brief Get a child by index, returns 0 for invalid index.
		TraceNode* getChild(boost::uint32_t index) {
			if (index >= mChildren.size()) return 0;
			return mChildren[index];
		}
		/// \brief Get a parent by index, returns 0 for invalid index.
		TraceNode* getParent(boost::uint32_t index) {
			if (index >= mParents.size()) return 0;
			return mParents[index];
		}
		/// \brief Remove a child by index, returns 0 for invalid index.
		TraceNode* removeChild(boost::uint32_t index) {
			if (index >= mChildren.size()) return 0;
			TraceNode* node = mChildren[index];
			mChildren.erase(mChildren.begin() + index);
			return node;
		}
		/// \brief Remove a parent by index, returns 0 for invalid index.
		TraceNode* removeParent(boost::uint32_t index) {
			if (index >= mParents.size()) return 0;
			TraceNode* node = mParents[index];
			mParents.erase(mParents.begin() + index);
			return node;
		}
	};
		
	#ifdef __GNUC__
		#pragma pack(pop)
	#endif
	
	/// \brief Vector of TraceNode pointer.
	typedef std::vector<TraceNode*> TraceNodePtrVector;
	
} // namespace router
} // namespace torc

#endif // TORC_ROUTER_TRACENODE_HPP
