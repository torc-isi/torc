// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
// $HeadURL: https://svn.east.isi.edu/torc/trunk/src/torc/bitstream/assembler/VirtexAssembler.hpp $
// $Id: VirtexAssembler.hpp 1303 2013-02-25 23:18:16Z nsteiner $

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

/// \file VirtexAssembler.hpp
/// \brief Definition of base Virtex class to implement functions common across all/some Virtex architectures.

#ifndef TORC_BITSTREAM_ASSEMBLER_VIRTEXASSEMBLER_HPP
#define TORC_BITSTREAM_ASSEMBLER_VIRTEXASSEMBLER_HPP

#include "torc/bitstream/assembler/Assembler.hpp"
#include "torc/Bitstream.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/Physical.hpp"
#include <iostream>

namespace torc {
namespace bitstream {

/// \brief Base Virtex class to implement functions common across all/some Virtex architectures
class VirtexAssembler: public Assembler {
public:

	// Types
		/// \brief Imported type
		typedef torc::bitstream::VirtexFrameBlocks VirtexFrameBlocks;

	// Constructors
		/// \brief Constructor taking in both design pointer and database object
		VirtexAssembler(torc::architecture::DDB &inDB) :
			Assembler(inDB) {
		}
		/// \brief Constructor taking in both design pointer and database object
		//VirtexAssembler(DesignSharedPtr inDesignPtr) : Assembler(inDesignPtr) {
		//}
		/// \brief Destructor
		virtual ~VirtexAssembler() {
		}

	// Public functions
		/// \brief Function to generate bitstream to be called by user.
		virtual int generateBitstream(DesignSharedPtr inDesignPtr,
			const path inTargetBitstreamPath,
			EMergeMode inMergeMode = eSet,
			path baseBitstreamPath = path());

protected:

	// Protected functions
		/// \brrief Interface to check if site type is supported. To be implemented by every derived architecture.
		virtual bool isSiteTypeSupported(const std::string &siteType) = 0;
		/// \brief Interface to check if given setting is part of compound setting
		virtual bool isCompoundSetting(string inElementName) = 0;
		/// \brief Interface to get vector of related configurations
		virtual vector<string> getDependantConfigs(string inElement1Name) = 0;
		/// \brief Initialize frame blocks after reading a bitstream
		virtual void initializeFullFrameBlocks() = 0;
		/// \brief Update frame blocks after modifying bitstream and just beore saving it
		virtual void updateFullFrameBlocks() = 0;
		/// \brief Get primary Xdl column for given tile column
		virtual uint32_t getPrimaryXdlColumn(TileCol inTileColumn) = 0;
		/// \brief Get Xdl frames for given tile row
		virtual VirtexFrameBlocks
				getXdlFrames(TileRow inTileRow, uint32_t inPrimaryXdlCol,
						uint32_t inBitIndexBegin, uint32_t inBitIndexEnd) = 0;
		/// \brief Store frame blocks and bit offset for given site
		virtual void initializeFrameDataForSite(const std::string &siteName);
		/// \brief Store frame blocks and bit offset for given tile index
		virtual void initializeFrameDataForTile(TileIndex tileIndex) = 0;
		/// \brief Merge compressed micro-bitstream with base bitstream
		virtual void mergeWithBaseBitstream(const std::vector<uint32_t> &bitAddresses,
				uint32_t blockIndex);

protected:
	// Protected members
		torc::bitstream::VirtexFrameBlocks mCurrFrameBlocks; ///<  Frame blocks for current site/tile
		boost::uint32_t mCurrWordOffset; ///< Word offset withing frame for current site/tile
};
} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_ASSEMBLER_VIRTEXASSEMBLER_HPP
