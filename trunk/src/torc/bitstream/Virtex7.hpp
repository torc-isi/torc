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
/// \brief Header for the Virtex7 class.

#ifndef TORC_BITSTREAM_VIRTEX7_HPP
#define TORC_BITSTREAM_VIRTEX7_HPP

#include <boost/cstdint.hpp>
#include <boost/filesystem.hpp>
#include "torc/bitstream/VirtexBitstream.hpp"
#include <map>

namespace torc { namespace architecture { class DDB; } }

namespace torc {
namespace bitstream {

//// To include once the UnitTest is set up
namespace bitstream { class Virtex7UnitTest; }    
namespace bitstream { class Virtex7FarUnitTest; }    
namespace bitstream { class Zynq7000DebugUnitTest; }    
namespace bitstream { void testVirtex7Device(const std::string& inDeviceName,             
	const boost::filesystem::path& inWorkingPath); }
  
	/// \brief Virtex7 bitstream.
	class Virtex7 : public VirtexBitstream {      
		friend class torc::bitstream::bitstream::Virtex7UnitTest;      
		friend class torc::bitstream::bitstream::Virtex7FarUnitTest;      
		friend class torc::bitstream::bitstream::Zynq7000DebugUnitTest;      
		friend void torc::bitstream::bitstream::testVirtex7Device(const std::string& inDeviceName,
			const boost::filesystem::path& inWorkingPath); 
	protected:
	// typedefs
		/// \brief Imported type name
		typedef boost::uint32_t uint32_t;
	public:
	// enumerations
		//
		/// \brief Configuration register enumeration.
		/// \see Type 1 Packet Registers: UG470, v1.1, March 28, 2011, Table 5-19.
		enum ERegister { eRegisterCRC = 0, eRegisterFAR, eRegisterFDRI, eRegisterFDRO, 
			eRegisterCMD, eRegisterCTL0, eRegisterMASK, eRegisterSTAT, eRegisterLOUT, 
			eRegisterCOR0, eRegisterMFWR, eRegisterCBC, eRegisterIDCODE, eRegisterAXSS, 
			eRegisterCOR1, eRegisterWBSTAR = 16, eRegisterTIMER, eRegisterBOOTSTS = 22, 
			eRegisterCTL1 = 24, eRegisterCount };
		//
		/// \brief Configuration register enumeration.
		/// \see Command Register Codes: UG470, v1.1, March 28, 2011, Table 5-21.
		enum ECommand { eCommandNULL = 0, eCommandWCFG, eCommandMFW, eCommandDGHIGH, 
			eCommandLFRM = eCommandDGHIGH, eCommandRCFG, eCommandSTART, eCommandRCAP, eCommandRCRC, 
			eCommandAGHIGH, eCommandSWITCH, eCommandGRESTORE, eCommandSHUTDOWN, eCommandGCAPTURE, 
			eCommandDESYNCH, eCommandReserved, eCommandIPROG, eCommandCRCC, eCommandLTIMER, 
			eCommandCount };
		//
		/// \brief Frame Address Register subfields.
		/// \see Frame Address Register Description: UG470, v1.1, March 28 2011, Table 5-20.
		enum EFar {
			eFarMaskBlockType =		0x03800000,		eFarShiftBlockType =	23,			
			eFarMaskTopBottom =		0x00400000,		eFarShiftTopBottom =	22,			
			eFarMaskRow =			0x003e0000,		eFarShiftRow =			17,			
			eFarMaskMajor =			0x0001ff80,		eFarShiftMajor =		 7,			
			eFarMaskMinor =			0x0000007f,		eFarShiftMinor =		 0
		};
		//
		/// \brief Frame Address Register top and bottom constants.
		/// \see Frame Address Register Description: UG470, v1.1, March 28, 2011, Table 5-20.
		enum EFarTopBottom { eFarTop = 0, eFarBottom = 1 };
		//
		/// \brief Frame Address Register block type constants.
		enum EFarBlockType { eFarBlockType0 = 0, eFarBlockType1, eFarBlockType2, eFarBlockType3, 	 
        	eFarBlockType4, eFarBlockType5, eFarBlockType6, eFarBlockType7, eFarBlockTypeCount };
		//
		/// \brief Assorted Constants.
		/// \see WBSTAR Register Description: UG470, v1.1, March 28, 2011, Table 5-31.
		enum {
			eStartAddr =	0x1fffffff,
			eTimerValue =	0x3fffffff,
		};
		//
		/// \brief Major Column Types.
		/// \details These are defined and used for internal purposes only, and are not derived 
		///		from any Xilinx documentation.
		enum EColumnType { eColumnTypeEmpty = 0, eColumnTypeInt, eColumnTypeBram, eColumnTypeCfg, 
			eColumnTypeClb, eColumnTypeClock, eColumnTypeCmt, eColumnTypeDsp, eColumnTypeGtx, 
			eColumnTypeIoi, eColumnTypeVframe, eColumnTypePss, eColumnTypeCount };
		/// \brief Frame length.
		/// \details Constant frame length of 101 32-bit words for the entire Virtex7 family.
		/// \see Frame Address Register (00001): UG470, v1.1, March 28, 2011.
		enum { eFrameLength = 101 };
		/// \brief Number of pad frames after each frame row.
		/// \details The configuration controller expects two frames of NOPs after each frame row.
		enum { eRowPadFrames = 2 };
		/// \brief Number of rows in a clock region.
		enum { eClockRegionRows = 52 };
	protected:
	// members  
		/// \brief Configuration controller registers.  
		//uint32_t mRegister[eRegisterCount];  
		/// \brief Packet Type names.  
		static const char* sPacketTypeName[ePacketTypeCount];  
		///\brief Packet Opcode names.  
		static const char* sOpcodeName[eOpcodeCount];  
		///\brief Configuration Register names.  
		static const char* sRegisterName[eRegisterCount];  
		///\brief Configuration Command names.  
		static const char* sCommandName[eCommandCount];  
		///\brief Configurations Options Register 0 (COR) subfields.  
		static const Subfield sCOR0[];  
		///\brief Configurations Options Register 1 (COR) subfields.  
		static const Subfield sCOR1[];  
		///\brief Status Register (STAT) subfields.  
		static const Subfield sSTAT[];  
		///\brief Control Register 0 (CTL) subfields.  
		static const Subfield sCTL0[];  
		///\brief Control Register 1 (CTL) subfields.  
		static const Subfield sCTL1[];  
		///\brief Control Mask Register (MASK) subfields.  
		static const Subfield sMASK0[];  
		///\brief Warm Boot Start Address Register (WBSTAR) subfields.  
		static const Subfield sWBSTAR[];  
		///\brief Watchdog Timer Register (TIMER) subfields.  
		static const Subfield sTIMER[];  
		///\brief Boot History Status Register (BOOTSTS) subfields.  
		static const Subfield sBOOTSTS[];  
		///\brief The number of top bitstream rows.  
		uint32_t mTopRowCount;  
		///\brief The number of bottom bitstream rows.  
		uint32_t mBottomRowCount;
	// functions  
		///\brief Set the number of top and bottom bitstream rows.  
		void setRowCounts (const string& inDeviceName);  
		//string mPrivateDeviceName;
	public:  
	// constructors  
		///\brief Basic Constructor.     
		Virtex7(void) : VirtexBitstream(), mTopRowCount(0), mBottomRowCount(0), mFrameRowCount(0) {     
//			for(int i = 0; i < eRegisterCount; i++) mRegister[i] =0;
			for(int i = 0; i < eFarBlockTypeCount; i++) mBlockFrameIndexBounds[i] = 0;
			// initialize miscellaneous variables
			mFrameRowCount = 0;
			// initialize the column type widths for this family
			mColumnDefs.resize(eColumnTypeCount);
			mColumnDefs[eColumnTypeEmpty] 	= ColumnDef("Empty",    0,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeInt] 	= ColumnDef("Int",      0,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeBram]	= ColumnDef("Bram",    28,128,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeCfg]		= ColumnDef("Cfg",     36,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeClb]		= ColumnDef("Clb",     36,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeClock] 	= ColumnDef("Clock",   30,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeCmt]		= ColumnDef("Cmt",     30,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeDsp]		= ColumnDef("Dsp",     28,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeGtx]		= ColumnDef("Gtx",     32,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeIoi]		= ColumnDef("Ioi",     42,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypeVframe]	= ColumnDef("Vframe",  30,  0,  0,  0,  0,  0,  0,  0);
			mColumnDefs[eColumnTypePss]		= ColumnDef("Pss",      0,  0,  0,  0,  0,  0,  0,  0);
			// map type type names to column types
			mTileTypeNameToColumnType["INT_L"]		= eColumnTypeInt;
			mTileTypeNameToColumnType["INT_R"]		= eColumnTypeInt;
			mTileTypeNameToColumnType["BRAM_L"]		= eColumnTypeBram;
			mTileTypeNameToColumnType["BRAM_R"]		= eColumnTypeBram;
			mTileTypeNameToColumnType["CFG_CENTER_TOP"]	= eColumnTypeCfg;
			mTileTypeNameToColumnType["CFG_CENTER_BOT"]	= eColumnTypeCfg;
			mTileTypeNameToColumnType["MONITOR_TOP_PELE1"] = eColumnTypeCfg;
			mTileTypeNameToColumnType["CLBLL_L"]	= eColumnTypeClb;
			mTileTypeNameToColumnType["CLBLL_R"]	= eColumnTypeClb;
			mTileTypeNameToColumnType["CLBLM_L"]	= eColumnTypeClb;
			mTileTypeNameToColumnType["CLBLM_R"]	= eColumnTypeClb;
			mTileTypeNameToColumnType["INT_FEEDTHRU_1"] = eColumnTypeClb;
			mTileTypeNameToColumnType["CMT_FIFO_L"]	= eColumnTypeCmt;
			mTileTypeNameToColumnType["CMT_FIFO_R"]	= eColumnTypeCmt;
			mTileTypeNameToColumnType["CLK_FEED"]	= eColumnTypeClock;
			mTileTypeNameToColumnType["DSP_L"]		= eColumnTypeDsp;
			mTileTypeNameToColumnType["DSP_R"]		= eColumnTypeDsp;
			mTileTypeNameToColumnType["GTP_COMMON"]	= eColumnTypeGtx;
			mTileTypeNameToColumnType["GTP_COMMON_MID_LEFT"] = eColumnTypeGtx;
			mTileTypeNameToColumnType["GTP_COMMON_MID_RIGHT"] = eColumnTypeGtx;
			mTileTypeNameToColumnType["GTX_COMMON"]	= eColumnTypeGtx;
			mTileTypeNameToColumnType["GTH_COMMON"]	= eColumnTypeGtx;
			mTileTypeNameToColumnType["HCLK_IOI"]	= eColumnTypeIoi;
			mTileTypeNameToColumnType["HCLK_IOI3"]	= eColumnTypeIoi;
			mTileTypeNameToColumnType["VFRAME"]		= eColumnTypeVframe;
			mTileTypeNameToColumnType["PSS1"]		= eColumnTypePss;
			mTileTypeNameToColumnType["PSS4"]		= eColumnTypePss;
		}
	// functions  
		///\brief Return the masked value for a subfield of the specified register.
		static uint32_t makeSubfield(ERegister inRegister, const std::string& inSubfield, 
			const std::string& inSetting);
		///\brief Initialize the Device Information.  
		virtual void initializeDeviceInfo(const std::string& inDeviceName);  
		///\brief Initialize the maps between frame indexes and frame addresses.  
		///\detail This is generally useful only for internal purposes.  
		virtual void initializeFrameMaps(void);  
		/// \brief Read frame data into the frame blocks structure.
		virtual void readFramePackets(void) {
			readFramePackets4567<Virtex7>(mBlockFrameIndexBounds, mFrameAddressToIndex, 
				mFrameIndexToAddress);
		}
/* no longer needed
		/// \brief Update the bitstream packets to include full frame data.
		virtual void updateFullBitstreamPackets(void);
		/// \brief Update the bitstream packets to include partial frame data.
		/// \param inBitstreamType The type of partial bitstream to generate: active or shutdown.
		/// \param inFrameInclusion The type of frames to include: only dirty frames or all frames.
		virtual void updatePartialBitstreamPackets(EBitstreamType inBitstreamType, 
			EFrameInclude inFrameInclusion);
		/// \brief Discard existing packets and generate bitstream packets for a full bitstream.
		virtual void generateFullBitstreamPackets(void);
		/// \brief Discard existing packets and generate bitstream packets for a partial bitstream.
		/// \param inBitstreamType The type of partial bitstream to generate: active or shutdown.
		/// \param inFrameInclusion The type of frames to include: only dirty frames or all frames.
		virtual void generatePartialBitstreamPackets(EBitstreamType inBitstreamType, 
			EFrameInclude inFrameInclusion);
*/
		/// \brief Discard the existing frame packets and return an iterator to the start position.
		virtual VirtexPacketVector::iterator deleteFramePackets(void) {
			return deleteFramePackets4567<Virtex7>();
		}
		/// \brief Return a packet vector with full frame data.
		virtual VirtexPacketVector generateFullBitstreamPackets(void) {
			return generateFullBitstreamPackets4567<Virtex7>(mBlockFrameIndexBounds);
		}
		/// \brief Return a packet vector with the full bitstream prefix.
		virtual VirtexPacketVector generateFullBitstreamPrefix(void);
		/// \brief Return a packet vector with the full bitstream suffix.
		virtual VirtexPacketVector generateFullBitstreamSuffix(void);
		/// \brief Return a packet vector with partial frame data.
		/// \param inFrameInclusion The type of frames to include: only dirty frames or all frames.
		virtual VirtexPacketVector generatePartialBitstreamPackets(EFrameInclude inFrameInclusion) {
			return generatePartialBitstreamPackets4567<Virtex7>(inFrameInclusion, 
				mFrameAddressToIndex, mFrameIndexToAddress);
		}
		/// \brief Return a packet vector with the partial bitstream prefix.
		/// \param inBitstreamType The type of partial bitstream to generate: active or shutdown.
		virtual VirtexPacketVector generatePartialBitstreamPrefix(EBitstreamType inBitstreamType);
		/// \brief Return a packet vector with the partial bitstream suffix.
		/// \param inBitstreamType The type of partial bitstream to generate: active or shutdown.
		virtual VirtexPacketVector generatePartialBitstreamSuffix(EBitstreamType inBitstreamType);
		/// \brief Returns frames for specified bitstream tile column.
		/// \parameter inTopBottom The top or bottom half of the device: eFarTop or eFarBottom.
		/// \parameter inFrameRow The frame or clock region row in the area specified by 
		///		inTopBottom.
		/// \parameter inBitCol The bitstream column coordinate (different than the XDL column 
		///		coordinate).
		/// \parameter inBlockCount The highest block type requested (8 for Xilinx architectures).
		VirtexFrameBlocks getBitstreamFrames(EFarTopBottom inTopBottom, uint32_t inFrameRow, 
			uint32_t inBitCol, uint32_t inBlockCount = eBlockTypeCount);
		/// \brief Returns frames for specified bitstream tile column.
		/// \parameter inBitRow The bitstream row coordinate (same as the XDL row coordinate).
		/// \parameter inBitCol The bitstream column coordinate (different than the XDL column 
		///		coordinate).
		/// \parameter outBeginBit The bit index of the beginning of the requested tile.
		/// \parameter outEndBit The bit index of the end of the requested tile.
		/// \parameter inBlockCount The highest block type requested (8 for Xilinx architectures).
		VirtexFrameBlocks getBitstreamFrames(uint32_t inBitRow, uint32_t inBitCol, 
			uint32_t& outBeginBit, uint32_t& outEndBit, uint32_t inBlockCount = eBlockTypeCount);
		/// \brief Returns frames for specified XDL tile coordinates.
		/// \parameter inXldRow The XDL row coordinate.
		/// \parameter inXdlCol The XDL column coordinate.
		/// \parameter outBeginBit The bit index of the beginning of the requested tile.
		/// \parameter outEndBit The bit index of the end of the requested tile.
		/// \parameter inBlockCount The highest block type requested (8 for Xilinx architectures).
		VirtexFrameBlocks getXdlFrames(uint32_t inXdlRow, uint32_t inXdlCol, uint32_t& outBeginBit, 
			uint32_t& outEndBit, uint32_t inBlockCount = eBlockTypeCount);
		/// \brief Returns the primary column corresponding to the given tile.
		/// \detail If the specified tile falls in an INT_L or INT_R tile, this function returns 
		///		the index of the corresponding primary XDL column.
		/// \parameter inXdlCol The XDL column coordinate.
		uint32_t getPrimaryXdlColumn(uint32_t inXdlRow, uint32_t inXdlCol);
		/// \brief Split the given frame index into the base index of the major frame and the index 
		///		of the minor frame.
		virtual void splitFrameIndex(uint32_t inFrameIndex, uint32_t& outMajorIndex, 
			uint32_t& outMinorIndex) {
			outMinorIndex = mFrameIndexToAddress[inFrameIndex].mMinor;
			outMajorIndex = inFrameIndex - outMinorIndex;
		}
	// deprecation macro
		/// \cond OMIT_FROM_DOXYGEN
		// Doxygen gets confused by the explicit "__attribute__ ((deprecated))" so we used this
		#define DEPRECATED __attribute__ ((deprecated))
		/// \endcond
	// deprecated functions
		/// \brief Loads full bitstream frames into block data structure.
		DEPRECATED void initializeFullFrameBlocks(void);
		/// \brief Transfers frame block data into the full bitstream frame packet
		DEPRECATED void updateFullFrameBlocks(void);
	// accessors
		/// \brief Return the number of frame rows for the current device.
		virtual uint32_t getFrameRowCount(void) const { return mFrameRowCount; }
		/// \brief Return the frame length for the current device.
		virtual uint32_t getFrameLength(void) const { return eFrameLength; }
		/// \brief Return the number of pad frames after each row.
		virtual uint32_t getRowPadFrames(void) const { return eRowPadFrames; }
	// inserters  
		/// \brief Insert the bitstream header into and output stream.  
		friend std::ostream& operator<< (std::ostream& os, const Virtex7& rhs);
	// inner classes
		class FrameAddress{     
		protected:        
			void assign(uint32_t inAddress) {	  
				mTopBottom = EFarTopBottom((inAddress & eFarMaskTopBottom) >> eFarShiftTopBottom);	  
				mBlockType = EFarBlockType((inAddress & eFarMaskBlockType) >> eFarShiftBlockType);	  
				mRow = (inAddress & eFarMaskRow) >> eFarShiftRow;	  
				mMajor = (inAddress & eFarMaskMajor) >> eFarShiftMajor;	  
				mMinor = (inAddress & eFarMaskMinor) >> eFarShiftMinor;	
			}
		public:   
			FrameAddress(void) : mTopBottom(eFarTop), mBlockType(eFarBlockType0), mRow(0), 
				mMajor(0), mMinor(0) {}  
			FrameAddress(EFarTopBottom inTopBottom, EFarBlockType inBlockType, uint32_t inRow, 
				uint32_t inMajor, uint32_t inMinor) : mTopBottom(inTopBottom), 
				mBlockType(inBlockType), mRow(inRow), mMajor(inMajor), mMinor(inMinor) {}  
			FrameAddress(uint32_t inAddress) { assign(inAddress); }    
			EFarTopBottom mTopBottom;    
			EFarBlockType mBlockType;    
			uint32_t mRow;    
			uint32_t mMajor;    
			uint32_t mMinor;    
			bool operator == (const FrameAddress& rhs) const {      
				return mTopBottom == rhs.mTopBottom && mBlockType == rhs.mBlockType && 
					mRow == rhs.mRow &&mMajor == rhs.mMajor && mMinor == rhs.mMinor; 
			}    
			bool operator< (const FrameAddress& rhs) const {      
				int diffBlockType = mBlockType - rhs.mBlockType;      
				if(diffBlockType) return diffBlockType < 0;      
				int diffTopBottom = mTopBottom - rhs.mTopBottom;      
				if(diffTopBottom) return diffTopBottom < 0;      
				int diffRow = mRow - rhs.mRow;      
				if(diffRow) return diffRow < 0;      
				int diffMajor = mMajor - rhs.mMajor;      
				if(diffMajor) return diffMajor < 0;      
				return mMinor < rhs.mMinor;      
			}
			operator uint32_t (void) const {    
				return     
					((mTopBottom << eFarShiftTopBottom) & eFarMaskTopBottom) |     
					((mBlockType << eFarShiftBlockType) & eFarMaskBlockType) |    
					((mRow << eFarShiftRow) & eFarMaskRow) |     
					((mMajor << eFarShiftMajor) & eFarMaskMajor) |     
					((mMinor << eFarShiftMinor) & eFarMaskMinor);    
			}  
			friend std::ostream& operator<< (std::ostream& os, const Virtex7::FrameAddress& rhs) {
				return os << (rhs.mTopBottom == Virtex7::eFarTop ? 'T' : 'B')
					<< "" << rhs.mBlockType << "(" << rhs.mRow << "," << rhs.mMajor << "." 
					<< rhs.mMinor << ")";
			}
		};
	protected:
	// inner classes
		/// \brief Frame row designator: top/bottom flag and row index.
		struct FrameRowDesignator {
			/// \brief Top/bottom flag.
			EFarTopBottom mTopBottom;
			/// \brief Frame row.
			uint32_t mFrameRow;
			/// \brief Base XDL row.
			uint32_t mXdlBaseRow;
			/// \brief Offset for this row into the column type vector.
			uint32_t mColumnVectorBase;
			/// \brief Default constructor.
			FrameRowDesignator(void) : mTopBottom(eFarTop), mFrameRow(0), mXdlBaseRow(0), 
				mColumnVectorBase(0) {}
			/// \brief Constructor.
			FrameRowDesignator(EFarTopBottom inTopBottom, uint32_t inFrameRow, 
				uint32_t inXdlBaseRow, uint32_t inColumnVectorBase) : mTopBottom(inTopBottom), 
				mFrameRow(inFrameRow), mXdlBaseRow(inXdlBaseRow), 
				mColumnVectorBase(inColumnVectorBase) {}
			/// \brief Comparison operator (for map operations).
			bool operator< (const FrameRowDesignator& rhs) const {
				if(mTopBottom < rhs.mTopBottom) return true;
				return mFrameRow < rhs.mFrameRow;
			}
		};
	// typedefs  
		/// \brief Map from frame index to frame address.  
		typedef std::map<uint32_t, Virtex7::FrameAddress> FrameIndexToAddress;  
		/// \brief Map from frame address to frame index.
		typedef std::map<Virtex7::FrameAddress, uint32_t> FrameAddressToIndex;  
		/// \brief Vector of frame indexes.
		typedef std::vector<uint32_t> IndexVector;
		/// \brief Map from bitstream column index to XDL column index.
		typedef std::map<uint32_t, uint32_t> BitColumnToXdlColumn;
		/// \brief Vector of FrameRowDesignator entries.
		typedef std::vector<FrameRowDesignator> FrameRowDesignatorVector;
	// functions
		/// \brief Returns frames for queried bitstream coordinates
		/// \parameter inSerialFrameRow The frame row as indexed starting from top row zero and 
		///		incrementing toward the top of the device, and then continuing from bottom row zero 
		///		and incrementing toward the bottom of the device.
		VirtexFrameBlocks getBitstreamFrames(uint32_t inSerialFrameRow, uint32_t inBitCol, 
			uint32_t inBlockCount = eBlockTypeCount);
		/// \brief Returns frames for queried XDL coordinates
		/// \parameter inSerialFrameRow The frame row as indexed starting from top row zero and 
		///		incrementing toward the top of the device, and then continuing from bottom row zero 
		///		and incrementing toward the bottom of the device.
		VirtexFrameBlocks getXdlFrames(uint32_t inSerialFrameRow, uint32_t inColumnVectorBase, 
			uint32_t inXdlCol, uint32_t inBlockCount = eBlockTypeCount);
	// members  
		/// \brief Map of frame indexes to frame addresses.  
		FrameIndexToAddress mFrameIndexToAddress;  
		/// \brief Map of frame addresses to frame indexes.  
		FrameAddressToIndex mFrameAddressToIndex;
		/// \brief Vector to store frame indexes of XDL columns.
		IndexVector mBitColumnIndexes[Virtex7::eFarBlockTypeCount];
		/// \brief Vector to store frame indexes of Bitstream columns.
		IndexVector mXdlColumnIndexes[Virtex7::eFarBlockTypeCount];
		/// \brief Array to hold frame index boundaries for blocks.
		uint32_t mBlockFrameIndexBounds[Virtex7::eFarBlockTypeCount];
		/// \brief Map of bitstream column indexes to XDL column indexes.
		BitColumnToXdlColumn mBitColumnToXdlColumn;
		/// \brief Number of frame rows.
		uint32_t mFrameRowCount;
		/// \brief Vector of FrameRowDesignator entries for each XDL row.
		FrameRowDesignatorVector mXdlRowToFrameRowDesignator;
	};
  
} // namespace bitstream
} // namespace torc


#endif // TORC_BITSTREAM_VIRTEX7_HPP
