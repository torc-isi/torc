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
/// \brief Header for the Segments class.

#ifndef TORC_ARCHITECTURE_SEGMENTS_HPP
#define TORC_ARCHITECTURE_SEGMENTS_HPP

#include "torc/architecture/XilinxDatabaseTypes.hpp"
#include "torc/architecture/DigestStream.hpp"
#include "torc/architecture/Array.hpp"
#include "torc/architecture/Tilewire.hpp"
#include "torc/architecture/DDBConsoleStreams.hpp"

namespace torc {
namespace architecture {

// forward declaration of our unit test class within its namespace
namespace architecture { class architecture_segments; }
namespace architecture { class segments_regression_test_helper; }
namespace architecture { class segments_unit_test_helper; }

	/// \brief Segment and irregular arc data for the device.
	/// \details Segment data is encoded in a collection of compact segments (segment shapes 
	///		normalized to tile index 0) and a mapping from tilewires to segment references and 
	///		anchor tile indexes.
	class Segments : DDBConsoleStreams {
	// friends
		/// \brief The database has access to our protected functions.
		friend class DDB;
		/// \brief Our unit test has access to our protected functions.
		friend class torc::architecture::architecture::architecture_segments;
		friend class torc::architecture::architecture::segments_regression_test_helper;
		friend class torc::architecture::architecture::segments_unit_test_helper;
	protected:
	// types
		typedef std::string string;						///< \brief Imported type name.
		typedef xilinx::TileCount TileCount;			///< \brief Imported type name.
		typedef xilinx::TileIndex TileIndex;			///< \brief Imported type name.
		typedef xilinx::WireIndex WireIndex;			///< \brief Imported type name.
		typedef xilinx::TileOffset TileOffset;			///< \brief Imported type name.
		typedef xilinx::CompactSegmentCount CompactSegmentCount; ///< \brief Imported type name.
		typedef xilinx::CompactSegmentIndex CompactSegmentIndex; ///< \brief Imported type name.
	// enums
		enum { eSegmentUndefined = 0xffffffff, eSegmentTrivial = 0 };
	// nested classes
		/// \brief Encapsulation of a wire belonging to a compact segment.
		/// \details A compact segment is defined by wire indexes in tiles normalized relative to 
		///		tile index 0.  The set of wire index and tile offset pairs fully defines the shape 
		///		of the compact segment.
		class CompactSegmentTilewire {
			WireIndex mWireIndex;
			TileOffset mTileOffset;
		public:
			inline WireIndex getWireIndex(void) const { return mWireIndex; }
			inline TileOffset getTileOffset(void) const { return mTileOffset; }
			CompactSegmentTilewire(void) : mWireIndex(), mTileOffset() {}
			CompactSegmentTilewire(WireIndex inWireIndex, TileOffset inTileOffset)
				: mWireIndex(inWireIndex), mTileOffset(inTileOffset) {}
		};
	public:
		/// \brief Encapsulation of compact segment index and an anchoring tile index.
		/// \details Compact segments are normalized relative to tile index 0.  The accompanying 
		///		tile index serves as an anchor from which to derive a full segment description.
		class SegmentReference {
			CompactSegmentIndex mCompactSegmentIndex;
			TileIndex mAnchorTileIndex;
		public:
			inline CompactSegmentIndex getCompactSegmentIndex(void) const 
				{ return mCompactSegmentIndex; }
			inline TileIndex getAnchorTileIndex(void) const { return mAnchorTileIndex; }
			SegmentReference(void) : mCompactSegmentIndex(), mAnchorTileIndex() {}
			SegmentReference(CompactSegmentIndex inCompactSegmentIndex, TileIndex inAnchorTileIndex)
				: mCompactSegmentIndex(inCompactSegmentIndex), mAnchorTileIndex(inAnchorTileIndex) 
				{}
			void undefine(void) { 
				mCompactSegmentIndex = CompactSegmentIndex(eSegmentUndefined); 
				mAnchorTileIndex = TileIndex(eSegmentUndefined);
			}
			bool isDefined(void) const { return mCompactSegmentIndex != eSegmentUndefined; }
			bool isTrivial(void) const { return mCompactSegmentIndex == eSegmentTrivial; }
			static const SegmentReference& getTrivialSegment(void) 
				{ return sTrivialSegmentReference; }
			static const SegmentReference sTrivialSegmentReference;
		};
	protected:
		/// \brief Encapsulation of an irregular arc.
		/// \details Arcs which are defined for a tile type, but which are not instantiated in 
		///		every single tile of that type are considered irregular, and must be considered on 
		///		a case by case basis.  Every irregular arc in a given tile defines its source and 
		///		sink wire indexes.
		class IrregularArc {
			WireIndex mSourceWireIndex;
			WireIndex mSinkWireIndex;
		public:
			inline WireIndex getSourceWireIndex(void) const { return mSourceWireIndex; }
			inline WireIndex getSinkWireIndex(void) const { return mSinkWireIndex; }
			IrregularArc(void) : mSourceWireIndex(), mSinkWireIndex() {}
			IrregularArc(WireIndex inSourceWireIndex, WireIndex inSinkWireIndex)
				: mSourceWireIndex(inSourceWireIndex), mSinkWireIndex(inSinkWireIndex) {}
		};
	// members
		/// \brief The compact segments in the device.
		Array2D<CompactSegmentTilewire> mCompactSegments;
		/// \brief The segment references for every wire in every tile.
		Array2D<SegmentReference> mTilewireSegments;
		/// \brief The irregular arcs for in the device.
		Array2D<IrregularArc> mIrregularArcs;
		/// \brief The number of compact segments in the device.
		CompactSegmentCount mCompactSegmentCount;
		/// \brief The number of irregular arcs in the device.
		uint32_t mIrregularArcCount;
		/// \brief The total number of wires in the device (pruned and actual).
		/// \note We cannot use a WireCount for this, because it is only intended for use in a 
		///		single tile, and is consequently only 16 bits wide.
		uint32_t mTotalWireCount;
		/// \brief The number of pruned wires in the device.
		/// \note We cannot use a WireCount for this, because it is only intended for use in a 
		///		single tile, and is consequently only 16 bits wide.
		uint32_t mPrunedWireCount;
		/// \brief The number of actual wires on non-trivial segments in the device.
		/// \note We cannot use a WireCount for this, because it is only intended for use in a 
		///		single tile, and is consequently only 16 bits wide.
		uint32_t mActualWireCount;
		/// \brief The number of defined trivial segments.
		/// \note We could use a CompactSegmentCount for this, but that would be semantically 
		///		incorrect because we are really counting full segments, not compact segments.
		uint32_t mTrivialSegmentCount;
		/// \brief The number of defined non-trivial segments.
		/// \note We could use a CompactSegmentCount for this, but that would be semantically 
		///		incorrect because we are really counting full segments, not compact segments.
		uint32_t mNonTrivialSegmentCount;
		/// \brief The total number of defined segments.
		/// \note We could use a CompactSegmentCount for this, but that would be semantically 
		///		incorrect because we are really counting full segments, not compact segments.
		uint32_t mTotalSegmentCount;
	// constructors
		/// \brief Protected constructor.
		Segments(void);
	// functions
		/// \brief Read the segment entries for every tile.
		size_t readTilewireSegments(DigestStream& inStream);
		/// \brief Read the compact segments for the device.
		size_t readSegments(DigestStream& inStream, bool inExtendedAnchorTileCount);
		/// \brief Read the irregular arcs for the device.
		size_t readIrregularArcs(DigestStream& inStream);
		/// \brief Return a pointer to the requested IrregularArc, or 0 if the arc does not exist.
		const Segments::IrregularArc* getIrregularArc(TileIndex inTileIndex, 
			WireIndex inSourceWireIndex, WireIndex inSinkWireIndex);
	public:
	// accessors
		/// \brief Return the total number of wires in the device (pruned and actual).
		uint32_t getTotalWireCount(void) const { return mTotalWireCount; }
		/// \brief Return the number of actual wires in the device (unpruned and non-trivial).
		uint32_t getActualWireCount(void) const { return mActualWireCount; }
		/// \brief Return the number of pruned wires in the device.
		uint32_t getPrunedWireCount(void) const { return mPrunedWireCount; }
		/// \brief Return the total number of trivial segments in the device.
		uint32_t getTrivialSegmentCount(void) const { return mNonTrivialSegmentCount; }
		/// \brief Return the total number of non-trivial segments in the device.
		uint32_t getNonTrivialSegmentCount(void) const { return mNonTrivialSegmentCount; }
		/// \brief Return the total number of full segments in the device.
		uint32_t getTotalSegmentCount(void) const { return mTotalSegmentCount; }
		/// \brief Return the number of compact segments in the device.
		uint32_t getCompactSegmentCount(void) const { return mCompactSegmentCount; }
		/// \brief Return the total number of irregular args in the device.
		uint32_t getIrregularArcCount(void) const { return mIrregularArcCount; }
		/// \brief Return the segment reference for the given tile and wire index.
		const Segments::SegmentReference& getTilewireSegment(const Tilewire& inTilewire) const {
			return mTilewireSegments[inTilewire.getTileIndex()][inTilewire.getWireIndex()];
		}
	};

} // namespace architecture
} // namespace torc

#endif // TORC_ARCHITECTURE_SEGMENTS_HPP
