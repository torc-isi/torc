
#ifndef TORC_PLACER_DEVICESITE_HPP
#define TORC_PLACER_DEVICESITE_HPP

#include <string>
#include "DeviceSiteType.hpp"
#include "DeviceSitePin.hpp"
#include "NetlistInstance.hpp"

namespace torc {
namespace placer {
	class NetlistInstance;
	class DeviceSitePin;
	
	class DeviceSite {
		typedef std::vector<DeviceSitePin*> DeviceSitePinPtrVector;
	protected:
		NetlistInstance* mInstance; // this is the instance that is contained in this site
		const std::string& mName;
		int mSiteTypeIndex;
		int mTileIndex;
		
		DeviceSitePinPtrVector mPins;
		
		//CSites::CSite& mSite;
		int mRow; // these are the coordinates extracted from the site name
		int mCol;
		//int mTileRow; // tile row and column
		//int mTileCol;
	
		//std::vector<PlacerSite*>* // pointer to candidate list that needs update
		//int // index into candidate list that needs update
		
		
		
	public:
		//PlacerSite(CSite& inSite) : mInstance(NULL), mSite(inSite)
		DeviceSite(const std::string& inName, int inSiteTypeIndex, int inTileIndex, int inRow, 
			int inCol) : mInstance(NULL),
			mName(inName), mSiteTypeIndex(inSiteTypeIndex), mTileIndex(inTileIndex), mRow(inRow), 
			mCol(inCol) {}
		~DeviceSite() {}
		void setInstance(NetlistInstance* instptr) {
			mInstance = instptr;
		}
		NetlistInstance* getInstance() {
			return mInstance;
		}
		const std::string& getName() {
			return mName;
		}
		int getSiteTypeIndex() {
			return mSiteTypeIndex;
		}
		int getTileIndex() {
			return mTileIndex;
		}
		int getRow() {
			return mRow;
		}
		int getCol() {
			return mCol;
		}
		void addPin(std::string inName, int inRow, int inCol) {
			DeviceSitePin* pin = new DeviceSitePin(inName, inRow, inCol);
			mPins.push_back(pin); // this is a nasty thing, copying this around, but probably
				// don't matter, only to initialize the thing
		}
		int getNumPins() {
			return mPins.size();
		}
		DeviceSitePin* getPin(int i) {
			return mPins[i];
		}
		
	}; //class DeviceSite
} // namespace placer
} // namespace torc
#endif // TORC_PLACER_DEVICESITE_HPP



