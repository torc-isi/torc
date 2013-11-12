// Torc - Copyright 2013-2013 University of Southern California.  All Rights Reserved.
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

/// \brief Base class to encapsulate micro-bitstream library generation code

#ifndef ASSEMBLERLIBGEN_HPP_
#define ASSEMBLERLIBGEN_HPP_

#include <iostream>
#include <vector>
#include <map>
#include "../Assembler.hpp"
#include "torc/Architecture.hpp"
#include "torc/physical/Design.hpp"
#include "torc/physical/Instance.hpp"
#include "torc/physical/Net.hpp"
#include "torc/packer/PrimitiveStructure.hpp"
#include "torc/bitstream/Bitstream.hpp"
#include <boost/filesystem/path.hpp>
#include <boost/regex.hpp>

namespace torc {
namespace bitstream {
	// Class to encapsulate micro-bitstream generation functions
	class AssemblerLibGen {
public:
	// Type definitions imported from torc namespace
		/// \brief Imported type name.
	    typedef std::string string;
		/// \brief Imported type name.
	    typedef boost::filesystem::path path;
		/// \brief Imported type name.
	    typedef torc::architecture::DDB DDB;
		/// \brief Imported type name.
	    typedef torc::architecture::Sites Sites;
		/// \brief Imported type name.
		typedef torc::architecture::Site Site;
		/// \brief Imported type name.
		typedef torc::architecture::Tiles Tiles;
		/// \brief Imported type name.
		typedef torc::architecture::TileInfo TileInfo;
		/// \brief Imported type name.
		typedef torc::architecture::xilinx::TileIndex TileIndex;
		/// \brief Imported type name.
		typedef torc::architecture::Tilewire Tilewire;
		/// \brief Imported type name.
		typedef torc::architecture::ExtendedWireInfo ExtendedWireInfo;
		/// \brief Imported type name.
		typedef torc::architecture::PrimitiveElement PrimitiveElement;
		/// \brief Imported type name.
		typedef torc::architecture::PrimitiveElementArray PrimitiveElementArray;
		/// \brief Imported type name.
		typedef torc::physical::DesignSharedPtr DesignSharedPtr;
		/// \brief Imported type name.
		typedef torc::physical::InstanceSharedPtr InstanceSharedPtr;
		/// \brief Imported type name.
		typedef torc::physical::NetSharedPtr NetSharedPtr;
		/// \brief Imported type name.
		typedef torc::packer::PrimitiveStructureSharedPtr PrimitiveStructureSharedPtr;
		/// \brief Imported type name.
		typedef torc::bitstream::BitstreamSharedPtr BitstreamSharedPtr;
	//Typedefs from Assembler class
		typedef torc::bitstream::Assembler::ConfigValuesToBits ConfigValuesToBits;
		typedef torc::bitstream::Assembler::ConfigSettingsToValues ConfigSettingsToValues;
		typedef torc::bitstream::Assembler::SiteTypeToConfigSettings SiteTypeToConfigSettings;
		typedef torc::bitstream::Assembler::TileTypeToSiteType TileTypeToSiteType;
		/// \brief Map from one setting to vector of related setings
		typedef torc::bitstream::Assembler::CompoundSettingMap CompoundSettingMap;

	// Type definitions for Maps
		/// \brief Typedef of map from tile type to set of site type
		typedef std::map<std::string,std::set<std::string> > TileTypeToSiteTypeSet;
		/// \brief Typedef of map from primitive name to primitive structure shared pointer
		typedef std::map<std::string, PrimitiveStructureSharedPtr> PrimitiveStructuresSharedPtrMap;
	public:
		/// \brief Constructor
		AssemblerLibGen(string inDevice, path inHarnessFolder, path inXdlGenerationFolder)
			:mDB(*(new DDB(inDevice))), mSites(mDB.getSites()), mTiles(mDB.getTiles()),
			 mHarnessFolder(inHarnessFolder), mXdlGenerationFolder(inXdlGenerationFolder) {

			initialize();
		}
		/// \brief Destructor
		virtual ~AssemblerLibGen() {
			delete &mDB;
		}

	// Public functions
		/// \brief Generrate Xdls for logic sites
		void generateLogicXdls();
		/// \brief Generate Xdls for routing pips
		void generateWireXdls();
		/// \brief Generate Xdls for both logic sites and routing pips
		void generateLogicAndWireXdl();
		/// \brief Generate bitstream for all Xdls in mXdlGenerationFolder and mXdlGenerationReferenceFolder
		void generateBitstreams();
		/// \brief Compress all Xilinx bitfiles in mXdlGenerationFolder
		void compressBitFiles();
		/// \brief Stitch micro-bitstreams in mXdlGenerationFolder
		void stitchMicroBitstreams();
		/// \brief Populate map of tile type to set of include site types
		TileTypeToSiteTypeSet getTileTypeToSiteTypeMap(void);
		/// \brief Print the tile type to site type information
		void printTileTypeToSiteTypeMapInfo(TileTypeToSiteTypeSet tileTypeToSiteType);

	protected:
		/// \brief Initialize object of this class
		void initialize();
		/// \brief Export all pips within the specified tile
		void exportTileSinks(TileIndex inTileIndex);
		/// \brief Export a single pip.
		void exportSink(Tilewire& inSource, Tilewire& inSink);
		/// \brief Return true if site type is SLICEL
		bool isSlicelType(const string &inSiteType) {
			return boost::regex_search(inSiteType.begin(), inSiteType.end(), boost::regex("SLICEL"));
		}
		/// \brief Returns harness design pointer for the given site type
		DesignSharedPtr getHarnessDesignForSiteType(const string &inSiteType);
		/// \brief Return config value which results in off bit in bitstream
		string getReferenceConfigValue(const string &inConfigSetting);
		/// \brief Create an empty design with same metadata. Does not copy configurations
		DesignSharedPtr createEmptyDesignWithSameMetadata(DesignSharedPtr inDesignPtr);
		/// \brief Returns true if input is a RAMB site
		bool isRambSite(const string &inSiteType) {
			return boost::regex_search(inSiteType, boost::regex("RAMB"));
		}
		/// \brief Returns true is input is a DSP site
		bool isDspSite(const string &inSiteType) {
			return boost::regex_search(inSiteType, boost::regex("DSP"));
		}
		/// \brief Is given setting head of compound setting group
		bool isCompoundSettingHead(const string &inSetting) {
			CompoundSettingMap::const_iterator pElement = mCompoundSettingMap.find(inSetting);
			// If element found in the compound settings map
			if(pElement != mCompoundSettingMap.end()) {
				return true;
			}
			return false;
		}
		/// \breif Create Xdls for each config value of the config setting
		void createXdlsForConfigSetting(const torc::architecture::PrimitiveElement &inElement,
				PrimitiveStructureSharedPtr const inPrimitiveStructurePtr, const string &inSiteType,
				DesignSharedPtr inHarnessDesignPtr);
		/// \brief Set config value and create Xdl
		void createXdlForGeneralConfigSetting(const string &inConfigSetting, const string &inConfigVal,
				const string &inXdlFileNamePartial, DesignSharedPtr inHarnessDesignPtr);
		/// \brief Create Xdl with a Ramb instane and no configuration
		void createRambBaseXdl(const string &inSiteType, DesignSharedPtr inHarnessDesignPtr);
		/// \brief Create Xdl for configurations which hex string values
		void createHexConfigXdl(const string &inConfigSetting, const string &inSiteType, int numHexChars,
				DesignSharedPtr inHarnessDesignPtr);
		/// \brief Create Xdls for compound settings
		void createXdlForCompoundSettings(const PrimitiveElement &inElement, const string &inSiteType, 
				PrimitiveStructureSharedPtr inPrimitiveStructurePtr, DesignSharedPtr inHarnessDesignPtr);
		/// \brief Create Xdls for orphan elements with empty reference Xdl
		void createXdlForOrphanElements(const torc::architecture::PrimitiveElement &inElement, 
				const string &inXdlFileNamePartial, DesignSharedPtr inHarnessDesignPtr);
		/// \brief Create Xdl for Lut
		void createXdlForLut(const torc::architecture::PrimitiveElement &inElement, const string &inXdlFileNamePartial,
			DesignSharedPtr inHarnessDesignPtr);
		/// \brief Create reference Xdl for given config setting
		void createReferenceXdlForConfigSetting(const string &inConfigSetting, DesignSharedPtr outHarnessDesignPtr,
			const string &inXdlFileNamePartial);
		/// \brief Set given config and generate Xdl
		void setConfigAndGenerateXdl(const string &inConfigSetting, const string & inConfigValue,
			const string &inXdlFileName, DesignSharedPtr inOutDesignPtr);
		/// \brief Get word offset in frame for the site/pip in corresponding Xdl
		uint32_t getWordOffsetFromXdlFile(path inBitstreamPath, BitstreamSharedPtr inBitstreamPtr);
		/// \brief Get index in Element array
		int getElementIndexFromName(const string inElementName, PrimitiveStructureSharedPtr inPrimitiveStructurePtr);
		/// \brief Save library map to file
		void saveLibraryMapToFile(TileTypeToSiteType &inLibrary);
		/// \brief Get a map from device to default package
		std::map<string, string> getDeviceToPackageMap() {
			std::map<string, string> deviceToPackageMap;
			deviceToPackageMap["xc5vfx130t"] = "FF1738";
			deviceToPackageMap["xc5vlx110t"] = "FF1136";
			deviceToPackageMap["xc7a100t"] = "FGG484";
			deviceToPackageMap["xc7z045"] = "FBG676";
			return deviceToPackageMap;
		}

	/// Virtual Functions
		/// \brief Returns parent family name
		virtual string getParentFamilyName() {
			return mDB.getFamilyName();
		}
		/// \brief Returns true if site type is supported
		virtual bool isSiteTypeSupported(const string &inSiteType) = 0;
		/// \brief Returns vector of supported tiles.
		virtual std::vector<std::string> getTilesSupportedForPips() = 0;
		/// \brie Returns primary primitivie for RAMB site type
		virtual std::string getRambPrimaryPrimitive() = 0;
		/// \brief Compress the diff of primary bitstream and reference bitstream
		virtual void compressBitFile(path primaryFile, path referenceFile) = 0;
		/// \brief Get word offset from tile location
		virtual uint32_t getWordOffsetFromTileLocation(TileRow tileRow, TileCol tileCol, 
							BitstreamSharedPtr inBitstreamPtr) = 0;
		/// \brief Populate primitive name to primitive structure map
		virtual PrimitiveStructuresSharedPtrMap getPrimitiveNameToStructreMap() = 0;
		/// \brief Generate micro-bitstreams for memory elements like LUT in RAM mode
		virtual void generateMemoryMicroBitstream() = 0;

	protected:
		DDB &mDB; ///< Database object
		const Sites &mSites; ///< Sites from the database object
		const Tiles &mTiles; ///< Tiles from the database object
		path mHarnessFolder; ///< Harness folder 
		path mXdlGenerationFolder; ///< Folder in which Xdls will be generated
		path mXdlGenerationReferenceFolder; ///< Folder in which reference Xdls will be genered
		path mLibraryFolder; ///< Path to library folder
		CompoundSettingMap mCompoundSettingMap; ///< Map from one compound setting to vector of other related settings
		std::map<string, string> mDeviceToPackageMap; ///< Map from device to pakcage
		std::map<string, string> mReferenceConfigMap; ///< Map from config setting to reference config value

		string mCurTileType; ///< Tile type for which Xdl are being generated currently
		static const char *sNameSeparator; ///< Symbol to separate out parts of file name
		static const char *sConfigValueOff; ///< Config value to switch off element (#OFF)
		static const char *sXdlExtension; ///< .xdl
		static const char *sLibExtension; ///< .ldb
		
	};
}
}

#endif /* ASSEMBLERLIBGEN_HPP_ */
