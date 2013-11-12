// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
// $HeadURL: https://svn.east.isi.edu/torc/trunk/src/torc/bitstream/assembler/lut/parser.yy $
// $Id: parser.yy 1303 2013-02-25 23:18:16Z nsteiner $

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

#ifndef TORC_BITSTREAM_ASSEMBLER_VIRTEX5ASSEMBLER_HPP
#define TORC_BITSTREAM_ASSEMBLER_VIRTEX5ASSEMBLER_HPP

#include "torc/bitstream/assembler/VirtexAssembler.hpp"
#include "torc/Physical.hpp"
#include "torc/architecture/DDB.hpp"
#include "build/AssemblerLibGen.hpp"
#include "build/Virtex5LibGen.hpp"

namespace torc {
namespace bitstream {

class Virtex5Assembler: public VirtexAssembler {
public:
	// Constructor
	Virtex5Assembler(torc::architecture::DDB &inDB) :
		VirtexAssembler(inDB) {
		initialize();
	}

protected:
	void initialize(void) {
		//mCompoundSettingMap = torc::bitstream::AssemblerLibGen::getCompoundSettingsMapForVirtex5();
		mCompoundSettingMap = torc::bitstream::Virtex5LibGen::getCompoundSettingsMap();
		mParentFamilyName = "Virtex5";

		// Get the compressed bitstreams library database file path
		string libDBFileName = mParentFamilyName + sLibraryExtension;
		boost::filesystem::path libDBPath = mLibraryPath / libDBFileName;
		if(!boost::filesystem::exists(libDBPath)) {
			std::cout << "ERROR: Library database file - " << libDBPath.string() << " - does not exist."
				<< std::endl;
			throw "Library file does not exist";
		}
		// Populate map data structure from the library database
		populateLibraryMap(libDBPath);
	}
	/// \brief Returns true if site type is supported
	virtual bool isSiteTypeSupported(const std::string &inSiteType) {
		return (inSiteType.compare("SLICEL") == 0 ||
				inSiteType.compare("SLICEM") == 0 ||
				inSiteType.compare("RAMB36_EXP") == 0 ||
				inSiteType.compare("DSP48E") == 0);
	}
	/// \brief Returns true if input resource is associated with compound group
	virtual bool isCompoundSetting(string inElementName) {
		CompoundSettingMap::const_iterator pElement = mCompoundSettingMap.find(inElementName);
		// If element found in the compound settings map
		if(pElement != mCompoundSettingMap.end()) {
			return true;
		}
		return false;
	}

	/// \brief Returs vector of related configurations
	virtual vector<string> getDependantConfigs(string inElement1Name) {
		return mCompoundSettingMap[inElement1Name];
	}

	virtual void initializeFullFrameBlocks() {
		torc::bitstream::Virtex5 *virtexBitStream =
				dynamic_cast<torc::bitstream::Virtex5 *> (mBitstreamPtr.get());
		virtexBitStream->initializeFullFrameBlocks();
	}

	virtual void updateFullFrameBlocks() {
		torc::bitstream::Virtex5 *virtexBitStream =
				dynamic_cast<torc::bitstream::Virtex5 *> (mBitstreamPtr.get());
		virtexBitStream->updateFullFrameBlocks();
	}

	/// \brief For an INT tile, returns associated CLB column
	virtual uint32_t getPrimaryXdlColumn(TileCol inTileColumn) {
		torc::bitstream::Virtex5 *virtex5Bitstream =
				dynamic_cast<torc::bitstream::Virtex5 *> (mBitstreamPtr.get());
		return virtex5Bitstream->getPrimaryXdlColumn(inTileColumn);
	}

	virtual torc::bitstream::VirtexFrameBlocks getXdlFrames(TileRow inTileRow,
			uint32_t inPrimaryXdlCol, uint32_t inBitIndexBegin, uint32_t inBitIndexEnd) {
		torc::bitstream::Virtex5 *virtex5Bitstream =
				dynamic_cast<torc::bitstream::Virtex5 *> (mBitstreamPtr.get());
		return virtex5Bitstream->getXdlFrames(inTileRow, inPrimaryXdlCol, inBitIndexBegin,
				inBitIndexEnd);
	}
	/// \brief Store frame blocks and bit offset for given tile index
	virtual void initializeFrameDataForTile(TileIndex tileIndex);
protected:
	// Member variables
		CompoundSettingMap mCompoundSettingMap;

};

}
}

#endif // TORC_BITSTREAM_ASSEMBLER_VIRTEX5ASSEMBLER_HPP
