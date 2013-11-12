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
/// \brief Header for the RouteTreeNode class.

#ifndef TORC_ROUTER_ROUTETREENODE_HPP
#define TORC_ROUTER_ROUTETREENODE_HPP

#include "torc/router/RouteNode.hpp"

namespace torc {
namespace router {

	// Pack RouteTreeNode objects tightly.
	/// \todo Have to justify the packing decision, and its impact on memory footprint versus 
	///		performance.
	#ifdef __GNUC__
		#pragma pack(push, 2)
	#endif
	
	/// \brief An object that holds more complete path information for routing and tracing.
	/// \details A RouteTreeNode contains children pointers to allow tracing of arcs through a
	/// 	device to recover complete nets from the device usage information.
	class RouteTreeNode : public RouteNode {
	// types
		typedef std::vector<RouteTreeNode*>::iterator RouteTreeNodeIterator;
	protected:
	// members
		/// \brief Pointer to a child node that is the only one.
		RouteTreeNode* mOnlyChild;
		/// \brief Pointer to a vector of child nodes that is dynamically allocated.
		std::vector<RouteTreeNode*>* mChildren;
	public:
	// constructors
		/// \brief Null Constructor.
		//RouteTreeNode() : RouteNode(), mDepth(0), mOnlyChild(0), mChildren(0) {}
		RouteTreeNode() : RouteNode(), mOnlyChild(0), mChildren(0) {}
		/// \brief Public Constructor.
		RouteTreeNode(Tilewire inSource, Tilewire inSink, boost::int32_t inCost, 
			RouteTreeNode* inParent)
			: RouteNode(inSource, inSink, inCost, inCost, 0, inParent), mOnlyChild(0), 
			mChildren(0) {
			if (inParent != 0) {
				mDepth = inParent->getDepth() + 1;
			} else {
				mDepth = 0;
			}
		}
		/// \brief Public Constructor.
		RouteTreeNode(Arc inArc, boost::int32_t inCost, RouteTreeNode* inParent)
			: RouteNode(inArc, inCost, inCost, 0, inParent), mOnlyChild(0), mChildren(0) {
			if (inParent != 0) {
				mDepth = inParent->getDepth() + 1;
			} else {
				mDepth = 0;
			}
		}
		/// \brief Destructor.
		~RouteTreeNode() {
			if (mOnlyChild != 0) { delete mOnlyChild; mOnlyChild = 0; }
			if (mChildren != 0) {
				RouteTreeNodeIterator p = mChildren->begin();
				RouteTreeNodeIterator e = mChildren->end();
				while (p < e) { delete *p, p++; }
				delete mChildren; mChildren = 0;
			}
		}
	// accessors
		
	// functions
		/// \brief Add children to the node.
		void addChildren(const std::vector<RouteTreeNode*>& newChildren) {
			boost::uint32_t size = newChildren.size();
			if (size == 0) { return; }
			
			// no child or 1 child
			if (mChildren == 0) {
				// adding 1 child to a node with 0
				if (mOnlyChild == 0 && size == 1) {
					mOnlyChild = newChildren[0];
					return;
				}
				// node will have more than 1 child, create vector
				mChildren = new std::vector<RouteTreeNode*>();
				// if the node had 1 child, move it to the vector
				if (mOnlyChild != 0) {
					mChildren->reserve(size+1);
					mChildren->push_back(mOnlyChild);
					mOnlyChild = 0;
				// node had 0 children, reserve space for the new children
				} else {
					mChildren->reserve(size);
				}
			// already 2 or more children
			} else {
				mChildren->reserve(mChildren->size() + size);
			}
			// insert children into the node
			mChildren->insert(mChildren->end(), newChildren.begin(), newChildren.end());
		}
		/// \brief Allocate a new node and make it the parent of this node.
		void makeParent(const Tilewire& inSource, const Tilewire& inSink) {
			//mArc = Arc(inSource, inSink); // DON'T WANT TO DO THIS
			//mParent = new RouteTreeNode(mArc.getSourceTilewire(), Tilewire::sInvalid, 0, 0);
			mParent = new RouteTreeNode(inSource, inSink, 0, 0);
			((RouteTreeNode*)mParent)->mOnlyChild = this;
			((RouteTreeNode*)mParent)->mDepth = mDepth - 1;
		}
		/// \brief Get the number of children.
		boost::uint16_t getNumChildren() {
			if (mOnlyChild != 0) return 1;
			else if (mChildren == 0) return 0;
			return mChildren->size();
		}
		/// \brief Get a child by index, returns 0 for invalid index.
		RouteTreeNode* getChild(unsigned int index) {
			if (mOnlyChild != 0 && index == 0) return mOnlyChild;
			if (mChildren != 0 && index < mChildren->size()) return (*mChildren)[index];
			return 0;
		}
		/// \brief Normalize depth of nodes.
		void normalizeDepth() {
			RouteTreeNode* top = (RouteTreeNode*) getTop();
			int topDepth = top->mDepth;
			if (topDepth == 0) return;
			top->adjustDepth(-topDepth);
		}
	protected:
		/// \brief Recursively adjust node depths.
		void adjustDepth(int adjustment) {
			mDepth += adjustment;
			if (mOnlyChild != 0) mOnlyChild->adjustDepth(adjustment);
			else if (mChildren != 0) {
				RouteTreeNodeIterator p = mChildren->begin();
				RouteTreeNodeIterator e = mChildren->end();
				while (p < e) {
					(*p)->adjustDepth(adjustment);
					p++;
				}
			}
		}
	};
	
	#ifdef __GNUC__
		#pragma pack(pop)
	#endif
	
} // namespace router
} // namespace torc

#endif // TORC_ROUTER_ROUTETREENODE_HPP
