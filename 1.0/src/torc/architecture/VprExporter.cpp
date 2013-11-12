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
/// \brief Source for the VprExporter class.

#include "torc/architecture/VprExporter.hpp"
#include "torc/architecture/OutputStreamHelpers.hpp"
#include "torc/common/DirectoryTree.hpp"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

// avoid pulling in additional structures
typedef void t_pb_graph_pin;
typedef void t_tnode;

// ================================================================================================= 
// begin declarations imported verbatim from VPR v6.0
// ================================================================================================= 

typedef enum e_rr_type
{ SOURCE = 0, SINK, IPIN, OPIN, CHANX, CHANY, INTRA_CLUSTER_EDGE, NUM_RR_TYPES }
t_rr_type;

enum e_direction
{
    INC_DIRECTION = 0,
    DEC_DIRECTION = 1,
    BI_DIRECTION = 2
};                              /* UDSD by AY */

enum e_drivers
{ MULTI_BUFFERED, SINGLE }; /* legacy routing drivers by Andy Ye (remove or integrate in future) */

typedef struct s_rr_node
{
	short xlow;
	short xhigh;
	short ylow;
	short yhigh;

	short ptc_num;

	short cost_index;
	short occ;
	short capacity;
	short fan_in;
	short num_edges;
	t_rr_type type;
	int *edges;
	short *switches;

	float R;
	float C;

	enum e_direction direction; /* UDSD by AY */
	enum e_drivers drivers;     /* UDSD by AY */
	int num_wire_drivers;       /* UDSD by WMF */
	int num_opin_drivers;       /* UDSD by WMF (could use "short") */

		/* Used by clustering only (TODO, may wish to extend to regular router) */
		int prev_node;
		int prev_edge; 
		int net_num;
		t_pb_graph_pin *pb_graph_pin;
		t_tnode *tnode;
		float pack_intrinsic_cost;
}
t_rr_node;

// ================================================================================================= 
// end declarations imported verbatim from VPR v6.0
// ================================================================================================= 

namespace torc {
namespace architecture {

// 10x10 bounding box
// xc6vlx240t (ml605)

	using namespace std;

	void VprExporter::operator()(void) {

		bool debug = true;

		cout << mDDB;
		cout << "sizeof(short) = " << sizeof(short) << endl;
		cout << "sizeof(int) = " << sizeof(int) << endl;
		cout << "sizeof(float) = " << sizeof(float) << endl;
		cout << "sizeof(t_rr_node) = " << sizeof(t_rr_node) << endl;

		// name the output file
		mMaxTileRow = min(mMaxTileRow, mDDB.getTiles().getRowCount());
		mMaxTileCol = min(mMaxTileCol, mDDB.getTiles().getColCount());
		stringstream ss;
		ss << mDDB.getDeviceName() << "-[" << mMinTileRow << "," << mMinTileCol << "]-[" 
			<< mMaxTileRow << "," << mMaxTileCol << "].vpr";
		boost::filesystem::path generatedPath = torc::common::DirectoryTree::getExecutablePath()
			/ "regression" / ss.str();
		mStream.open(generatedPath.string().c_str(), std::ios::out | std::ios::binary);

		// declare maps to track tilewires
		typedef map<Tilewire, uint32_t> TilewireToIndex;
		TilewireToIndex tilewireToIndex;
		typedef map<Tilewire, bool> TilewireIsExcluded;
		TilewireIsExcluded tilewireIsExcluded;
		// iterate over every tile
		TileCount tileCount = mTiles.getTileCount();
		size_t size = 0;
		for(TileIndex tileIndex(0); tileIndex < tileCount; tileIndex++) {
			// iterate over every wire in the tile
			const TileInfo& tileInfo = mTiles.getTileInfo(tileIndex);
			TileTypeIndex tileTypeIndex = tileInfo.getTypeIndex();
			// TileRow TileRow = tileInfo.getRow();
			// TileCol TileCol = tileInfo.getCol();
			WireCount wireCount = mTiles.getWireCount(tileTypeIndex);
			for(WireIndex wireIndex(0); wireIndex < wireCount; wireIndex++) {
				// take a quick exit if we've already looked at this segment
				Tilewire currentTilewire(tileIndex, wireIndex);
				if(tilewireIsExcluded.find(currentTilewire) != tilewireIsExcluded.end()) continue;
				// look up the segment information for this tilewire
				TilewireVector tilewires;
				mDDB.expandSegment(currentTilewire, tilewires);
				// if a segment is not real in this device, it won't have any tilewires
				if(tilewires.empty()) continue;
				// determine whether to include this segment or not
				TilewireVector::iterator tp = tilewires.begin();
				TilewireVector::iterator te = tilewires.end();
				bool include = true;
				while(include && tp < te) {
					ExtendedWireInfo ewi(mDDB, *tp++);
					if(ewi.mTileRow < mMinTileRow) include = false;
					if(ewi.mTileCol < mMinTileCol) include = false;
					if(ewi.mTileRow > mMaxTileRow) include = false;
					if(ewi.mTileCol > mMaxTileCol) include = false;
				}
				// mark the tilewires as visited
				tp = tilewires.begin();
				while(tp < te) tilewireIsExcluded[*tp++] = !include;
				if(!include) continue;
				// push the anchor tilewire if we haven't seen it before
				if(tilewireToIndex.find(tilewires[0]) == tilewireToIndex.end())
					tilewireToIndex[tilewires[0]] = size++;
			}
		}
		cout << "Found " << tilewireToIndex.size() << " total segments compared to expected "
			<< mSegments.getTotalSegmentCount() << endl;

		// create a vector of Node elements
		typedef vector<Node> NodeVector;
		NodeVector nodes;
		nodes.resize(tilewireToIndex.size());
		// iterate over every indexed wire and write it out
		TilewireToIndex::const_iterator p = tilewireToIndex.begin();
		TilewireToIndex::const_iterator e = tilewireToIndex.end();
		while(p != e) {
			TilewireToIndex::value_type value = *p++;
			uint32_t index = value.second;
			Tilewire tilewire = value.first;
			nodes[index].mIndex = index;
			nodes[index].mTilewire = tilewire;
			// expand this segment's sinks
			ArcVector arcVector;
			mDDB.expandSegmentSinks(tilewire, arcVector, DDB::eExpandDirectionNone, true, true, 
				true, false);
			ArcVector::const_iterator ap = arcVector.begin();
			ArcVector::const_iterator ae = arcVector.end();
			while(ap < ae) {
				Arc arc = *ap++;
				// expand the sink wire's segment to recover the anchor and segment index
				TilewireVector tilewires;
				mDDB.expandSegment(arc.getSinkTilewire(), tilewires);
				// if we have deliberately pruned this sink tilewire, do not include it in the data
				if(tilewireToIndex.find(tilewires[0]) == tilewireToIndex.end()) continue;
				// convert the sink tilewire to an index
				uint32_t sinkIndex = tilewireToIndex[tilewires[0]];
				nodes[index].mEdges.push_back(sinkIndex);
			}
			// iterate over every segment wire to extract attributes
			{
				TilewireVector tilewires;
				mDDB.expandSegment(tilewire, tilewires);
				TilewireVector::iterator tp = tilewires.begin();
				TilewireVector::iterator te = tilewires.end();
				while(tp < te) {
					ExtendedWireInfo ewi(mDDB, *tp++);
					nodes[index].mMinRow = min(nodes[index].mMinRow, ewi.mTileRow);
					nodes[index].mMinCol = min(nodes[index].mMinCol, ewi.mTileCol);
					nodes[index].mMaxRow = max(nodes[index].mMaxRow, ewi.mTileRow);
					nodes[index].mMaxCol = max(nodes[index].mMaxCol, ewi.mTileCol);
					nodes[index].mRowRange = nodes[index].mMaxRow - nodes[index].mMinRow;
					nodes[index].mColRange = nodes[index].mMaxCol - nodes[index].mMinCol;
					nodes[index].mFlags |= ewi.mWireFlags;
				}
			}
		}

		// iterate through the VPR nodes and output everything except for the edges
		size_t nodeCount = tilewireToIndex.size();
		size_t edgeCount = 0;
		mStream.write(reinterpret_cast<char*>(&nodeCount), sizeof(nodeCount));
		int* edgePtr = 0;
		NodeVector::iterator np = nodes.begin();
		NodeVector::iterator ne = nodes.end();
		while(np < ne) {
			Node& node = *np++;
			if(debug) cout << node.mIndex << ": (" << node.mTilewire << "): ";
			if(debug) cout << "[" << node.mMinRow << "," << node.mMinCol << "]-["
				<< node.mMaxRow << "," << node.mMaxCol << "] ";
			if(debug) cout << (WireInfo::isInput(node.mFlags) ? "INPUT " : "");
			if(debug) cout << (WireInfo::isOutput(node.mFlags) ? "OUTPUT " : "");
			Uint32Vector::const_iterator ep = node.mEdges.begin();
			Uint32Vector::const_iterator ee = node.mEdges.end();
			if(debug) while(ep < ee) {
				cout << *ep++ << " ";
			}
			if(debug) cout << endl;

			t_rr_node vpr_rr_node;
			// segment bounds
			vpr_rr_node.xlow = node.mMinCol;
			vpr_rr_node.xhigh = node.mMaxCol;
			vpr_rr_node.ylow = node.mMinRow;
			vpr_rr_node.yhigh = node.mMaxRow;
			// we don't have track numbers, but perhaps the wire index will do
			vpr_rr_node.ptc_num = node.mTilewire.getWireIndex();
			// no intrinsic cost, and current occupancy is zero
			vpr_rr_node.cost_index = vpr_rr_node.occ = 0;
			// the capacity of every XDLRC wire is one
			vpr_rr_node.capacity = 1;
			// does VPR really use fanin information?  we can provide this if it's important
			vpr_rr_node.fan_in = 0;
			// fanout
			vpr_rr_node.num_edges = node.mEdges.size();
			// node type; not sure if this captures what Eddie Hung was saying about their types
			if(WireInfo::isInput(node.mFlags)) { vpr_rr_node.type = SINK; }
			else if(WireInfo::isOutput(node.mFlags)) { vpr_rr_node.type = SOURCE; }
			else if(node.mColRange > node.mRowRange) { vpr_rr_node.type = CHANX; }
			else if(node.mColRange < node.mRowRange) { vpr_rr_node.type = CHANY; }
			else { vpr_rr_node.type = CHANX; /* what if we can't tell? */ }
			// we will fill this in in a minute
			vpr_rr_node.edges = edgePtr;
			edgePtr += vpr_rr_node.num_edges + 1;
			edgeCount += vpr_rr_node.num_edges + 1;
			// Eddie is supposed to look this up
			vpr_rr_node.switches = 0;
			// we have no resistance or capacitance data
			vpr_rr_node.R = vpr_rr_node.C = 0;
			// all I know is that most wires are not bidirectional
			vpr_rr_node.direction = INC_DIRECTION; // what does INC_DIRECTION mean?
			// all XDLRC wires have a single driver
			vpr_rr_node.drivers = SINGLE;
			// what exactly do the following two fields mean?
			vpr_rr_node.num_wire_drivers = 0; // unless it's important to provide this
			vpr_rr_node.num_opin_drivers = 0; // unless it's important to provide this
			// VPR will use this to trace back after routing
			vpr_rr_node.prev_node = vpr_rr_node.prev_edge = vpr_rr_node.net_num = 0;
			// we aren't populating graph or timing information
			vpr_rr_node.pb_graph_pin = 0;
			vpr_rr_node.tnode = 0;
			// we currently provide no packing cost hints
			vpr_rr_node.pack_intrinsic_cost = 0;

			// write the node to the file
			mStream.write(reinterpret_cast<char*>(&vpr_rr_node), sizeof(t_rr_node));
		}

		// iterate through the VPR nodes and output everything except for the edges
		mStream.write(reinterpret_cast<char*>(&edgeCount), sizeof(edgeCount));
		uint32_t delimiter = -1;
		np = nodes.begin();
		while(np < ne) {
			Node& node = *np++;
			Uint32Vector::const_iterator ep = node.mEdges.begin();
			Uint32Vector::const_iterator ee = node.mEdges.end();
			while(ep < ee) {
				uint32_t edge = *ep++;
				mStream.write(reinterpret_cast<char*>(&edge), sizeof(edge));
			}
			mStream.write(reinterpret_cast<char*>(&delimiter), sizeof(delimiter));
		}

	}

} // namespace architecture
} // namespace torc
