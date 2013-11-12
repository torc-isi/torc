// Torc - Copyright 2013-2013 University of Southern California.  All Rights Reserved.
// $HeadURL: https://svn.east.isi.edu/torc/trunk/src/torc/bitstream/assembler/Assembler.hpp $
// $Id: Assembler.hpp 1303 2013-02-25 23:18:16Z nsteiner $

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

/// \file Assembler.hpp
/// \brief Base class for Xdl to bitstream conversion. This class is abstract but still contains lot of architecture
///	independent implementation
/// \author Ritesh Kumar Soni

#ifndef TORC_BITSTREAM_ASSEMBLER_ASSEMBLER_HPP
#define TORC_BITSTREAM_ASSEMBLER_ASSEMBLER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "torc/Bitstream.hpp"
#include "torc/architecture/DDB.hpp"
#include "torc/common/DirectoryTree.hpp"
#include "torc/Physical.hpp"
#include <boost/regex.hpp>
#include <boost/smart_ptr.hpp>

// forward declarations of friends outside our namespace.
namespace torc {
	class LutParser;
	class LutScanner;
	class location;
} // torc

namespace torc {
namespace bitstream {

/// \brief Base class for bitstream assembly
class Assembler {

public:
	// enumerations
		/// \brief Permissible operations on LUT frames.
		enum EOperation { eAND, eOR, eXOR, eNOT };
		/// \brief
		enum EMergeMode { eSet, eClear }; 

	// types
		/// \brief Imported type name.
		typedef std::string string;
		/// \brief Imported type name.
		typedef std::istream istream;
		/// \brief Imported type name.
		typedef torc::physical::DesignSharedPtr DesignSharedPtr;
		/// \brief Imported type name.
		typedef torc::physical::InstanceSharedPtr InstanceSharedPtr;
		/// \brief Imported type name.
		typedef torc::architecture::DDB DDB;
		/// \brief Imported type name.
		typedef boost::filesystem::path path;

	// Typedefs
		/// \brief Element name to vector of related element names that collectively affect bits.
		typedef std::map<const string, std::vector<string> > CompoundSettingMap;
		/// \brief Map from config value to vector of config bits.  ConfigValuesToBits.
		typedef std::map<const string, std::vector<uint32_t> > ConfigValuesToBits;
		/// \brief Map from config setting to config values.  ConfigSettingsToValues.
		typedef std::map<const string, ConfigValuesToBits> ConfigSettingsToValues;
		/// \brief Map from site type to config settings.  SiteTypeToConfigSettings.
		typedef std::map<const string, ConfigSettingsToValues> SiteTypeToConfigSettings;
		/// \brief Map from tile type to site types. Includes routing as a site type.
		typedef std::map<const string, SiteTypeToConfigSettings> TileTypeToSiteType;
		/// Currently not in use. Datastructure to store LUT frames in efficient and flexible way
		typedef std::map<int32_t, map<int32_t, uint32_t> > FrameWords;

	/// \brief Constructor take only Xdl design pointer.
//	Assembler(DesignSharedPtr inDesignPtr) : mTraceScanning(false), mTraceParsing(false),
//		mDesignPtr(inDesignPtr), mDB(*(new DDB(inDesignPtr->getDevice()))), mSites(mDB.getSites()),
//		mTiles(mDB.getTiles()), mFreeDB(true) {
//		initialize();
//	}
	/// \brief Constructor takes Xdl design pointer and architecture database object
	Assembler(DDB &inDB) : mTraceScanning(false),
		mTraceParsing(false), mDB(inDB), mSites(mDB.getSites()),
		mTiles(mDB.getTiles()), mFreeDB(false) {
		initialize();
	}
	/// \brief Destructor does not do anything
	virtual ~Assembler(void) { 
		if(mFreeDB) 
			delete &mDB;
	} 

	// Public functions
		/// \brief Function to generate bitstream, takes target bitstream path and optional base bitstream path
		virtual int generateBitstream(DesignSharedPtr inDesignPtr, const path inTargetBitstreamPath, 
				EMergeMode inMergeMode = eSet, path baseBitstreamPath = path());

	// members
		/// \brief Enable debug output in the flex scanner.
		bool mTraceScanning;
		/// \brief Enable debug output in the bison parser.
		bool mTraceParsing;
		/// \brief Name of file or input stream for error messages.
		string mStreamName;
		/// \brief Pointer to the current lexer instance.
		/// \details This serves to connect the parser to the scanner, and is used by the yylex
		///		macro.
		class torc::LutScanner* lexer;

	// macros
		/// \cond OMIT_FROM_DOXYGEN
		// Doxygen gets confused by the explicit "__attribute__ ((deprecated))" so we used this
		#define DEPRECATED __attribute__ ((deprecated))
		/// \endcond

	// functions
		/// \brief Import Lut expression from a string.
		/// \param input Input stream.
		/// \param name Stream name to use for error messages.
		/// \returns true if successfully parsed.
		bool processLut(const string& input, const string& name = "string stream");
		/// \brief Lut parse error handling.
		void error(const location& l, const string& m);
		/// \brief Lut general error handling.
		void error(const string& m);
		/// \brief Signals a parsing failure by deasserting the success flag.
		void failure(void) { mSuccess = false; }

		/// \brief Convert a hex ASCII character to a decimal value.
		/// \returns The equivalent decimal value, or -1 if the input is not a valid hex digit.
		static inline int hexCharacterToDec(char inDigit) {
			// handle numeric hex digits
			if(inDigit >= '0' && inDigit <= '9') return inDigit - '0';
			// force lower case to upper case
			inDigit &= 0xdf;
			// handle alphabetic hex digits
			if(inDigit >= 'A' && inDigit <= 'F') return inDigit - 'A' + 10;
			// reject non-hex digits
			return -1;
		}

		/// \brief Function called from parser when it encounters a bit operator
		void binaryLutFrameOperation(EOperation inOperation);
		/// \brief Function called from parser when it encounters a literal
		void pushLutFrame(string inLiteral);

protected:
	// friends
		/// \brief The LutParse has access to our members.
		friend class torc::LutParser;

	// Protected functions
		/// \brief Initializations done in constructor
		void initialize(void);
		/// \brief Initiates Xdl to bitstream conversion
		void convertXdlToBitstream(void);
		/// \brief Save bitstream
		void saveBitstream(void);
		/// \brief Assemble bitstream for instances
		void assembleInstances(void);
		/// \brief Assemble bitstream for nets
		void assembleNets(void);
		/// \brief Assemble bitstream for Ramb
		void assembleRamb(torc::physical::InstanceSharedPtr inInstancePtr);
		/// \brief Get tile type from site type
		string getTiletypeFromSitename(const string &inSiteType);
		/// \brief Verify the configuratino and merge with base bitstream
		virtual void checkValidityAndMergeBitstream(string inElementName, string inConfigValue,
				const ConfigSettingsToValues &inConfigMap);
		/// \brief Is config value a LUT equation
		virtual bool isLutEquationSetting(const string &inConfigValue) {
			return boost::regex_match(inConfigValue, sLutConfigRegEx);
		}
		/// \brief Is config a _ROUTETHROUGH
		virtual bool isRoutethrough(const string &inSetting) {
			return boost::regex_match(inSetting, sRoutethroughRegEx);
		}
		/// \brief Is config value a LUT ram setting
		virtual bool isLutRamOrRomSetting(const string &inConfigVal) {
			return boost::regex_match(inConfigVal, sLutRamOrRomConfigRegEx);
		}
		/// \brief Returns true if the site is some form of RAMB site.
		/// \details This function might have to be reimplemented in different
		/// 	architectures as name of RAMB site might change.
		virtual bool isRambSite(const string &inSiteType) {
			if (inSiteType.length() > 3) {
				return inSiteType.substr(0, 4).compare("RAMB") == 0;
			}
			return false;
		}
		/// \breif Does the configuration have hex string in config value.
		virtual bool isConfigValHexString(const string &inSiteType,
				const string &inElementName) {

			if (inSiteType.compare("DSP48E") == 0) {
				if (inElementName.compare("PATTERN") == 0 || inElementName.compare(
						"MASK") == 0) {
					return true;
				}
			}
			return false;
		}
		/// \brief Is ramb memory init setting
		bool isMemoryInitSetting(const string &inConfigName) {
			return inConfigName.length() == 7 && inConfigName.substr(0, 5).compare(
					"INIT_") == 0;
		}
		/// \brief Is ramb parity init setting
		bool isMemoryParityInitSetting(const string &inConfigName) {
			return inConfigName.substr(0, 6).compare("INITP_") == 0;
		}
		/// \brief Is site present in library database
		/// ToDo: This function will need tile type also.
		bool tileAndSiteExistInLibrary(const string &inTileType, const string &inSiteType) {
			
			//unused(inSiteType);
			TileTypeToSiteType::iterator pTiles = mLibrary.find(inTileType);
			if(pTiles == mLibrary.end()) {
				mUnsupportedTileCount++;
				mUnsupportedTileTypeSet.insert(inTileType+"-"+inSiteType);
				return false;
			} else {
				SiteTypeToConfigSettings::iterator pSites = pTiles->second.find(inSiteType);
				if(pSites == pTiles->second.end()) {
					mUnsupportedTileCount++;
					mUnsupportedTileTypeSet.insert(inTileType+"-"+inSiteType);
					return false;
				}
			}
			return true;
		}
		/// \brief Is configuration present in given map
		bool elementAndConfigExistInLibrary(const string &inElementName,
				const string &configValue, ConfigSettingsToValues &inConfigSettingToValues);
		/// \brief If slice site type, annotate type as per even or odd column
		virtual void getAnnotatedSiteTypeForSlicel(string &inOutSiteType, 
				const string &siteLocation);
		/// \brief Merge micro-bitstream for Lut equation
		void mergeLutEquationBits(const string &inElementName,
				const string &inConfigValue, ConfigSettingsToValues &inConfigSettingToValues);
		/// \brief Merge micro-bitsram for Lut in ram/rom mode
		void mergeLutRamOrRomBits(const string &inElementName, const string &inConfigVal,
				ConfigSettingsToValues &inConfigSettingToValues);
		/// \brief Merge micro-bitstream for compound setting
		void mergeCompoundSettingBits(string inElement1Name,
				string inConfig1Val,
				InstanceSharedPtr inInstancePtr,
				const ConfigSettingsToValues &inConfigSettingToValues);
		/// \brief Merge micro-bitstream for configuration with Hex values
		void mergeHexConfigBits(string inElementName,
				string inConfigVal,
				const ConfigSettingsToValues &inConfigSettingToValues);
		/// \brief Merge micro-bitstreams for ramb memory init values
		void mergeRambInitBits(const string &inConfigVal,
				uint32_t inMemoryInitRow, const vector<uint32_t> &inBitAddresses,
				uint32_t inBlock);
		/// \brief Read micro-bitstream library file and populate it in a map
		void populateLibraryMap(path inLibDBPath);
		/// \brief Check if processor is big endian
		bool isBigEndianMachine() {
			uint32_t check = 0x01020304;
			char *p = (char *)&check;
			return  (p[0] == 0x01);
		}
		/// \brief Read word from stream and convert endianness if required
		void readWord(std::ifstream &fileStream, uint32_t &outWord) {
			fileStream.read((char *) &outWord, sizeof(uint32_t));
			if(mBigEndian)
				outWord = reverseEndianness(outWord);
		}
		/// \brief Reverse endianness of input word
		uint32_t reverseEndianness(uint32_t inWord) {
			uint32_t outWord = 0;
			char *in = (char *)&inWord;
			char *out = (char *)&outWord;
			uint wordSize = sizeof(inWord);
			for(uint i = 0; i< wordSize; i++) {
				out[i] = in[wordSize-i-1];
			}
			return outWord;
		}

	// Pure virtual functions
		/// \brief Is site supporeted for Xdl to bitstream conversion
		virtual bool isSiteTypeSupported(const string &inSiteType) = 0;
		/// \brief Store frame blocks and bit offset for given site
		virtual void initializeFrameDataForSite(const string &inSiteName) = 0;
		/// \brief Store frame blocks and bit offset for given tile index
		virtual void initializeFrameDataForTile(TileIndex inTileIndex) = 0;
		/// \brief Returs vector of related configurations
		virtual vector<string> getDependantConfigs(string inElement1Name) = 0;
		/// \brief Does element need compound setting
		virtual bool isCompoundSetting(string inElementName) = 0;
		/// \brief Merge micro-bitstream with base bitstream
		virtual void mergeWithBaseBitstream(
				const vector<uint32_t> &inBitAddresses, uint32_t inBlockIndex) = 0;
		
protected:
	// Protected members
		torc::physical::DesignSharedPtr mDesignPtr; ///< Xdl design pointer
		path mTargetBitstreamPath; ///< Target bitstream path
		EMergeMode mMergeMode; ///< Merge mode - set bits or clear bits
		TileTypeToSiteType mLibrary; ///< Micro-bitstream library in map datastructure
		const torc::architecture::DDB &mDB; ///< Torc database of architecutre on which design is implemented
		const torc::architecture::Sites &mSites; ///< Sites from the architecture database
		const torc::architecture::Tiles &mTiles; ///< Tiles from architecture database
		bool mFreeDB; ///< To track if DB object was allocated in constructor
		torc::bitstream::BitstreamSharedPtr mBitstreamPtr; /// Pointer to bitstream object to which micro bitstreams are merged.
		boost::filesystem::path mLibraryPath;
		string mParentFamilyName;
		bool mSuccess; ///< Flag signaling parsing success.
		bool mBigEndian;
	// Member variables to store state during Lut equation parsing
		vector<vector<uint32_t> > mLutFrameSetStack; ///< Frame set for literals are pushed to this stack
		uint32_t mLutCurrentReferenceFrameIndex; ///< Address of 1st frame being affected by Lut equation
		uint32_t mCurrentReferenceWordIndex; ///< Address of 1st work within frame affected by Lut equation
		std::vector<uint32_t> mLutFrameSetForOne; ///< Frame set for Lut output assigned to 1.
		ConfigValuesToBits mCurrentConfigToBitMap; ///< Map from Lut config value to micro-bitstreams
		string mLutCurrentEquationLhs; ///< LHS of Lut equation currently being processed
	// Member variables to track unsupported resources
		int mUnsupportedTileTypeCount;
		int mUnsupportedTileCount;
		int mUnsupportedPipCount;
		int mUnsupportedInstCount;
		int mMissingConfigs;
		std::set<std::string> mUnsupportedTileTypeSet;
	// Static member variables
		static const string sLibraryRelativePath; ///< Path to library folder containing mirco-bitstream database
		static const string sLibraryExtension; ///< Extension used for micro-bitstream libraries
		static const string sConfigOff; ///< String used when an element is explicity configured off
		static const boost::regex sLutRamOrRomConfigRegEx; ///< Regex to identify Luts configured as ram/rom
		static const boost::regex sLutConfigRegEx; ///< Regex to identify Lut configuration
		static const boost::regex sRoutethroughRegEx; ///< Regex to indentify ROUTETHROUGH configuration in instances

};
/// \brief Typedef for shared pointer of Assembler class
typedef boost::shared_ptr<Assembler> AssemblerSharedPtr;

} // namespace bitstream
} // namespace torc

#endif // TORC_BITSTREAM_ASSEMBLER_ASSEMBLER_HPP
